
/// @file magus/lutmap/CutResub.cc
/// @brief CutResub の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BoolResub.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2007 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include "magus_config.h"
#endif

#include "BoolResub.h"
#include "LutNetwork.h"

#include <ym_sat/SatSolver.h>


#define dout cerr

const ymuint debug_none             = 0x00000000;
const ymuint debug_check_node_subst = 0x00000001;
const ymuint debug_check_node_merge = 0x00000002;
const ymuint debug_all              = 0xFFFFFFFF;

const ymuint debug = debug_all;


BEGIN_NAMESPACE_MAGUS_LUTMAP

BEGIN_NONAMESPACE

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス BoolResub
//////////////////////////////////////////////////////////////////////

// コンストラクタ
BoolResub::BoolResub()
{
}

// デストラクタ
BoolResub::~BoolResub()
{
}

// @brief LUT の置き換えを行って LUT 数の削減を行う．
// @param[in] lutnetwork LUT ネットワーク
// @param[in] npat シミュレーションで用いるパタンサイズ
void
BoolResub::node_subst(LnGraph& lutnetwork,
		      ymuint npat)
{
  init(lutnetwork, npat);
  
  cout << "before node substitution: " << mLutNetwork->n_lnodes() << endl;

  // 作業領域を初期化
  ymuint vect_size = npat;
  mLutNetwork->init_pat(vect_size);
  mLutNetwork->sort(mNodeList);

  // global function を計算
  calc_gf();

  // ODC(CSPF) を計算
  calc_odc();
  
  ymuint max_n = mLutNetwork->max_node_id();
  for (ymuint i = 0; i < max_n; ++ i) {
    mDmark[i] = false;
    mTfomark[i] = false;
  }
  
  // ノードの置き換えが可能か試す．
  for (vector<LnNode*>::reverse_iterator p = mNodeList.rbegin();
       p != mNodeList.rend(); ++ p) {
    LnNode* node = *p;
    if ( mDmark[node->id()] ) continue;
    
    set_tfomark(node);
    
    const PatVect& gf = node->gfpat();
    const PatVect& odc = node->odcpat();

    for (vector<LnNode*>::const_iterator q = mNodeList.begin();
	 q != mNodeList.end(); ++ q) {
      LnNode* node1 = *q;
      if ( mDmark[node1->id()] || mTfomark[node1->id()] ) continue;
      const PatVect& gf1 = node1->gfpat();
      if ( eq_with_dc(gf, gf1, odc) &&
	   check_node_subst(node, node1) ) {
    
	// node を node1 で置き換える．
	mLutNetwork->subst_node(node, node1);
	// node およびその TFI のうち不要となったものに印をつける．
	delete_tfi(node);
	// node1 の TFO の global function を更新する．
	update_patvect(node1);
	
	break;
      }
    }
    
    clear_tfomark(node);
  }

  // 不要になったノードを削除する．
  for (vector<LnNode*>::reverse_iterator p = mNodeList.rbegin();
       p != mNodeList.rend(); ++ p) {
    LnNode* node = *p;
    if ( mDmark[node->id()] ) {
      assert_cond(node->n_fanout() == 0, __FILE__, __LINE__);
      mLutNetwork->delete_lut(node);
    }
  }

  cout << "after node substitution: " << mLutNetwork->n_lnodes() << endl;
}

// @brief LUT のマージを行って LUT 数の削減を行う．
// @param[in] lutgraph サブジェクトグラフ
// @param[in] npat シミュレーションで用いるパタンサイズ
void
BoolResub::node_merge(LnGraph& lutgraph,
		      ymuint max_ni,
		      ymuint npat)
{
  init(lutgraph, npat);
  mMaxNi = max_ni;
  
  ymuint nl0 = mLutNetwork->n_lnodes();

  mLutNetwork->sort(mNodeList);
  
  // global function を計算
  calc_gf();

  // ODC(CSPF) を計算
  calc_odc();

  ymuint max_n = mLutNetwork->max_node_id();
  for (ymuint i = 0; i < max_n; ++ i) {
    mDmark[i] = false;
    mTfimark[i] = false;
    mTfomark[i] = false;
  }
  
  // ノードのマージが可能か試す．
  for (vector<LnNode*>::reverse_iterator p = mNodeList.rbegin();
       p != mNodeList.rend(); ++ p) {
    LnNode* node1 = *p;
    
    if ( mDmark[node1->id()] ) continue;

    set_tfimark(node1);
    set_tfomark(node1);
    
    const PatVect& gf1 = node1->gfpat();
    const PatVect& odc1 = node1->odcpat();

    for (vector<LnNode*>::const_iterator q = mNodeList.begin();
	 q != mNodeList.end(); ++ q) {
      LnNode* node2 = *q;
      if ( mDmark[node2->id()] ||
	   mTfimark[node2->id()] ||
	   mTfomark[node2->id()] ) continue;
      const PatVect& gf2 = node2->gfpat();
      const PatVect& odc2 = node2->odcpat();
      if ( eq_with_dc(gf1, gf2, odc1, odc2) &&
	   check_node_merge(node1, node2) ) {
  
	// node1, node2 およびその TFI のうち不要となったものに印をつける．
	delete_tfi(node1);
	delete_tfi(node2);
  
	// node1 の TFO の global function を更新する．
	update_patvect(node1);
	break;
      }
    }
    
    clear_tfimark(node1);
    clear_tfomark(node1);
  }

  // 不要になったノードを削除する．
  for (vector<LnNode*>::reverse_iterator p = mNodeList.rbegin();
       p != mNodeList.rend(); ++ p) {
    LnNode* node = *p;
    if ( mDmark[node->id()] ) {
      assert_cond(node->n_fanout() == 0, __FILE__, __LINE__);
      mLutNetwork->delete_lut(node);
    }
  }

  cout << "before node merge: " << nl0 << endl;
  cout << "after node merge: " << mLutNetwork->n_lnodes() << endl;
}
  
// @brief データ構造を初期化する．
void
BoolResub::init(LnGraph& lutnetwork,
		ymuint npat)
{
  mLutNetwork = &lutnetwork;
  mNpat = npat;
  mLutNetwork->init_pat(mNpat);
  ymuint n = mLutNetwork->max_node_id();
  mMark.resize(n);
  mDmark.resize(n);
  mTfimark.resize(n);
  mTfomark.resize(n);
  mQueue1.reserve(n);
  mQueue2.reserve(n);
}

// @brief global function の計算を行う．
void
BoolResub::calc_gf()
{
  ymuint npi = mLutNetwork->n_inputs();
  for (ymuint i = 0; i < npi; ++ i) {
    LnNode* node = mLutNetwork->input(i);
    node->gfpat().set_random(mRandGen);
  }
  for (vector<LnNode*>::const_iterator p = mNodeList.begin();
       p != mNodeList.end(); ++ p) {
    LnNode* node = *p;
    const LogExpr& expr = node->expr();
    ymuint ni = node->ni();
    vector<PatVect*> ipat(ni);
    for (ymuint i = 0; i < ni; ++ i) {
      LnNode* inode = node->fanin(i);
      ipat[i] = &(inode->gfpat());
    }
    calc_pat(expr, ipat, node->gfpat());
  }
}

// @brief ODC の計算を行う．
void
BoolResub::calc_odc()
{
  ymuint n = mLutNetwork->max_node_id();
  for (ymuint i = 0; i < n; ++ i) {
    mLutNetwork->node(i)->odcpat().set();
  }

  // 外部出力に接続するノードの ODC は空
  ymuint npo = mLutNetwork->n_outputs();
  for (ymuint i = 0; i < npo; ++ i) {
    LnNode* node = mLutNetwork->output(i);
    LnNode* inode = node->fanin(0);
    inode->odcpat().clear();
  }
  for (vector<LnNode*>::reverse_iterator p = mNodeList.rbegin();
       p != mNodeList.rend(); ++ p) {
    LnNode* node = *p;
    
    // ノードのファンインの枝の ODC (にせMSPF)を計算する．
    const LogExpr& expr = node->expr();
    ymuint ni = node->ni();
    vector<PatVect*> ipat(ni);
    for (ymuint i = 0; i < ni; ++ i) {
      LnNode* inode = node->fanin(i);
      ipat[i] = &(inode->gfpat());
    }
    for (ymuint i = 0; i < ni; ++ i) {
      LnNode* inode = node->fanin(i);
      ipat[i]->negate();
      PatVect opat2;
      calc_pat(expr, ipat, opat2);
      inode->odcpat() &= (node->gfpat() ^ opat2);
      ipat[i]->negate();
    }
  }
}

// @brief node の変化に合わせて global function と ODC の再計算を行う．
void
BoolResub::update_patvect(LnNode* node)
{
  // global function を計算
  for (vector<LnNode*>::const_iterator p = mNodeList.begin();
       p != mNodeList.end(); ++ p) {
    LnNode* node = *p;
    const LogExpr& expr = node->expr();
    ymuint ni = node->ni();
    vector<PatVect*> ipat(ni);
    for (ymuint i = 0; i < ni; ++ i) {
      LnNode* inode = node->fanin(i);
      ipat[i] = &(inode->gfpat());
    }
    calc_pat(expr, ipat, node->gfpat());
  }

  // ODC(CSPF) を計算
  ymuint n = mLutNetwork->max_node_id();
  for (ymuint i = 0; i < n; ++ i) {
    mLutNetwork->node(i)->odcpat().set();
  }
  ymuint npo = mLutNetwork->n_outputs();
  for (ymuint i = 0; i < npo; ++ i) {
    LnNode* node = mLutNetwork->output(i);
    LnNode* inode = node->fanin(0);
    inode->odcpat().clear();
  }
  for (vector<LnNode*>::reverse_iterator p = mNodeList.rbegin();
       p != mNodeList.rend(); ++ p) {
    LnNode* node = *p;
    
    // ノードのファンインの枝の CODC を計算する．
    const LogExpr& expr = node->expr();
    ymuint ni = node->ni();
    vector<PatVect*> ipat(ni);
    for (ymuint i = 0; i < ni; ++ i) {
      LnNode* inode = node->fanin(i);
      ipat[i] = &(inode->gfpat());
    }
    for (ymuint i = 0; i < ni; ++ i) {
      ipat[i]->negate();
      PatVect opat2;
      calc_pat(expr, ipat, opat2);
      LnNode* inode = node->fanin(i);
      inode->odcpat() &= (node->gfpat() ^ opat2);
      ipat[i]->negate();
    }
  }
}


BEGIN_NONAMESPACE

// ノードの正常回路の値を表すリテラルを返す．
inline
Literal
gval_lit(LnNode* node)
{
  return Literal(node->id() * 2 + 0, kPolPosi);
}

// ノードの故障回路の値を表すリテラルを返す．
inline
Literal
fval_lit(LnNode* node)
{
  return Literal(node->id() * 2 + 1, kPolPosi);
}

inline
Literal
add_dlit(SatSolver* solver,
	 LnNode* node)
{
  Literal glit = gval_lit(node);
  Literal flit = fval_lit(node);
  tVarId vid = solver->new_var();
  Literal dlit(vid, kPolPosi);
  solver->add_clause(~glit, ~flit, ~dlit);
  solver->add_clause(~glit,  flit,  dlit);
  solver->add_clause( glit, ~flit,  dlit);
  solver->add_clause( glit,  flit, ~dlit);
  return dlit;
}

END_NONAMESPACE


// @brief ノードの置き換えをチェックする．
bool
BoolResub::check_node_subst(LnNode* node1,
			    LnNode* node2)
{
  if ( debug & debug_check_node_subst ) {
    dout << "  check_node_subst(" << node1->name()
	 << " [" << node1->id_str() << "], "
	 << node2->name() << " [" << node2->id_str() << "]" << endl;
  }
  
  SatSolver* solver = SatSolverFactory::gen_solver();
  
  ymuint n = mLutNetwork->max_node_id();
  
  // 変数を確保する．
  for (ymuint i = 0; i < n; ++ i) {
    (void) solver->new_var();
    (void) solver->new_var();
  }

  // まず node の TFO にマークをつける．
  // 同時にマークの付いたノードを queue につむ．
  for (ymuint i = 0; i < n; ++ i) {
    mMark[i] = 0;
  }
  mQueue1.clear();

  mMark[node1->id()] = 3;
  mQueue1.push_back(node1);
  ymuint rpos = 0;
  while ( rpos < mQueue1.size() ) {
    LnNode* node = mQueue1[rpos];
    ++ rpos;
    const LnEdgeList& fo_list = node->fanout_list();
    for (LnEdgeList::const_iterator p = fo_list.begin();
	 p != fo_list.end(); ++ p) {
      LnNode* onode = (*p)->to();
      if ( !mMark[onode->id()] ) {
	mMark[onode->id()] = 3;
	mQueue1.push_back(onode);
      }
    }
  }

  // マークの付いたノードの TFI に別のマークをつける．
  mQueue2.clear();
  for (ymuint rpos = 0; rpos < mQueue1.size(); ++ rpos) {
    LnNode* node = mQueue1[rpos];
    ymuint ni = node->ni();
    for (ymuint i = 0; i < ni; ++ i) {
      LnNode* inode = node->fanin(i);
      if ( mMark[inode->id()] == 0 ) {
	// マークの付いていないファンインがあった．
	mMark[inode->id()] = 2;
	mQueue2.push_back(inode);
      }
    }
  }
  for (ymuint rpos2 = 0; rpos2 < mQueue2.size(); ++ rpos2) {
    LnNode* node = mQueue2[rpos2];
    ymuint ni = node->ni();
    for (ymuint i = 0; i < ni; ++ i) {
      LnNode* inode = node->fanin(i);
      if ( mMark[inode->id()] == 0 ) {
	// マークの付いていないファンインがあった．
	mMark[inode->id()] = 2;
	mQueue2.push_back(inode);
      }
    }
  }
  // 以降はこのマークのついたノードのみを対象とする．

  // 正常回路の CNF を生成
  for (vector<LnNode*>::const_iterator p = mNodeList.begin();
       p != mNodeList.end(); ++ p) {
    LnNode* node = *p;
    if ( mMark[node->id()] == 0 ) continue;
    ymuint ni = node->ni();
    vector<Literal> inputs(ni);
    for (ymuint i = 0; i < ni; ++ i) {
      LnNode* inode = node->fanin(i);
      inputs[i] = gval_lit(inode);
    }
    Literal output = gval_lit(node);
    const LogExpr& expr = node->expr();
    make_cnf(solver, expr, output, inputs);
  }
  ymuint npo = mLutNetwork->n_outputs();
  for (ymuint i = 0; i < npo; ++ i) {
    LnNode* node = mLutNetwork->output(i);
    LnNode* inode = node->fanin(0);
    Literal input = gval_lit(inode);
    Literal output = gval_lit(node);
    solver->add_clause(~input, output);
    solver->add_clause(input, ~output);
  }
  
  // 変更回路の CNF を生成
  { // node1 の変更値 = node2 の正常値ということ．
    Literal lit1 = fval_lit(node1);
    Literal lit2 = gval_lit(node2);
    solver->add_clause(~lit1,  lit2);
    solver->add_clause( lit1, ~lit2);
  }
  
  vector<Literal> tmp;
  tmp.reserve(npo);
  for (ymuint i = 1; i < mQueue1.size(); ++ i) {
    LnNode* node = mQueue1[i];
    ymuint ni = node->ni();
    vector<Literal> inputs(ni);
    for (ymuint j = 0; j < ni; ++ j) {
      LnNode* inode = node->fanin(j);
      if ( mMark[inode->id()] == 3 ) {
	inputs[j] = fval_lit(inode);
      }
      else {
	inputs[j] = gval_lit(inode);
      }
    }
    Literal flit = fval_lit(node);
    if ( node->is_output() ) {
      solver->add_clause(~inputs[0],  flit);
      solver->add_clause( inputs[0], ~flit);
    }
    else {
      const LogExpr& expr = node->expr();
      make_cnf(solver, expr, flit, inputs);
    }
    
    if ( node->is_output() ) {
      Literal dlit = add_dlit(solver, node);
      tmp.push_back(dlit);
    }
  }
  solver->add_clause(tmp);
  
  bool ans = false;
  vector<Bool3> model;
  Bool3 stat = solver->solve(model);
  if ( stat == kB3False ) {
    // 変更回路はオリジナル回路と等価
    ans = true;
  }

  delete solver;

  if ( ans ) {
    if ( debug & debug_check_node_subst ) {
      dout << "  OK" << endl;
    }
    
    return true;
  }
  else {
    if ( debug & debug_check_node_subst ) {
      dout << "  NG" << endl;
    }
    return false;
  }
}

// @brief ノードのマージをチェックする．
bool
BoolResub::check_node_merge(LnNode* node1,
			    LnNode* node2)
{
  if ( debug & debug_check_node_merge ) {
    dout << "  check_node_merge("
	 << node1->name() << " [" << node1->id_str() << "], "
	 << node2->name() << " [" << node2->id_str() << "]"
	 << endl;
  }
  
  // 新しいノードのファンインの候補を求める．
  vector<LnNode*> support;
  ymuint ni1 = node1->ni();
  ymuint ni2 = node2->ni();
  support.reserve(ni1 + ni2);

  for (ymuint i = 0; i < ni1; ++ i) {
    support.push_back(node1->fanin(i));
  }
  for (ymuint i = 0; i < ni2; ++ i) {
    LnNode* inode = node2->fanin(i);
    bool found = false;
    for (ymuint j = 0; j < ni1; ++ j) {
      if ( inode == node1->fanin(j) ) {
	found = true;
	break;
      }
    }
    if ( !found ) {
      support.push_back(inode);
    }
  }
  // これ以外にも候補を増やしてもよい．
  ymuint ni3 = support.size();
  
  if ( ni3 > 10 ) {
    if ( debug & debug_check_node_merge ) {
      dout << "  NG(0)" << endl;
    }
    return false;
  }
  
  ymuint np = 1U << ni3;
  vector<int> lutvect(np, 2);
#if 0
  // パタンベクタで必要条件のチェックを行う．
  PatVect careset = ~(node1->odcpat() | node2->odcpat());
  PatVect onset = (node1->gfpat() | node2->gfpat()) & careset;
  PatVect offset = (~node1->gfpat() | ~node2->gfpat()) & careset;
  
  PatVect zero(careset.size());
  zero.clear();
  bool ng = false;
  for (ymuint p = 0U; p < np; ++ p) {
    PatVect tmp(careset.size());
    tmp.set();
    for (ymuint i = 0; i < ni3; ++ i) {
      LnNode* inode = support[i];
      if ( p & (1U << i) ) {
	tmp &= inode->gfpat();
      }
      else {
	tmp &= ~inode->gfpat();
      }
    }
    if ( (onset & tmp) != zero ) {
      if ( (offset & tmp) != zero ) {
	ng = true;
	break;
      }
      lutvect[p] = 1;
    }
    else if ( (offset & tmp) != zero ) {
      lutvect[p] = 0;
    }
  }
  if ( ng ) {
    if ( debug & debug_check_node_merge ) {
      dout << "  NG(1)" << endl;
    }
    return false;
  }
#endif
  
  if ( debug & debug_check_node_merge ) {
    dout << "  Phase1 passed: ni = " << ni3 << endl;
  }

  // あとは SAT ソルバで検証する．
  SatSolver* solver = SatSolverFactory::gen_solver();
  
  ymuint n = mLutNetwork->max_node_id();
  
  // 変数を確保する．
  for (ymuint i = 0; i < n; ++ i) {
    (void) solver->new_var();
    (void) solver->new_var();
  }
  
  // 新しい LUT の個々の真理値を表す変数
#if 0
  vector<Literal> lutvect_lit(np);
  for (ymuint p = 0; p < np; ++ p) {
    Literal lit(solver->new_var(), kPolPosi);
    lutvect_lit[p] = lit;
  }
#else
  Literal olit(solver->new_var(), kPolPosi);
#endif

  // まず node1 と node2 の TFO にマークをつける．
  // 同時にマークの付いたノードを queue につむ．
  for (ymuint i = 0; i < n; ++ i) {
    mMark[i] = 0;
  }
  mQueue1.clear();
  
  mMark[node1->id()] = 3;
  mQueue1.push_back(node1);
  mMark[node2->id()] = 3;
  mQueue1.push_back(node2);
  ymuint rpos = 0;
  while ( rpos < mQueue1.size() ) {
    LnNode* node = mQueue1[rpos];
    ++ rpos;
    const LnEdgeList& fo_list = node->fanout_list();
    for (LnEdgeList::const_iterator p = fo_list.begin();
	 p != fo_list.end(); ++ p) {
      LnNode* onode = (*p)->to();
      if ( !mMark[onode->id()] ) {
	mMark[onode->id()] = 3;
	mQueue1.push_back(onode);
      }
    }
  }

  // マークの付いたノードの TFI に別のマークをつける．
  mQueue2.clear();
  for (ymuint rpos = 0; rpos < mQueue2.size(); ++ rpos) {
    LnNode* node = mQueue2[rpos];
    ymuint ni = node->ni();
    for (ymuint i = 0; i < ni; ++ i) {
      LnNode* inode = node->fanin(i);
      if ( mMark[inode->id()] == 0 ) {
	// マークの付いていないファンインがあった．
	mMark[inode->id()] = 2;
	mQueue2.push_back(inode);
      }
    }
  }
  for (ymuint rpos2 = 0; rpos2 < mQueue2.size(); ++ rpos2) {
    LnNode* node = mQueue2[rpos2];
    ymuint ni = node->ni();
    for (ymuint i = 0; i < ni; ++ i) {
      LnNode* inode = node->fanin(i);
      if ( mMark[inode->id()] == 0 ) {
	// マークの付いていないファンインがあった．
	mMark[inode->id()] = 2;
	mQueue2.push_back(inode);
      }
    }
  }
  // 以降はこのマークのついたノードのみを対象とする．

  // 正常回路の CNF を生成
  for (vector<LnNode*>::const_iterator p = mNodeList.begin();
       p != mNodeList.end(); ++ p) {
    LnNode* node = *p;
    if ( mMark[node->id()] == 0 ) continue;
    ymuint ni = node->ni();
    vector<Literal> inputs(ni);
    for (ymuint i = 0; i < ni; ++ i) {
      LnNode* inode = node->fanin(i);
      inputs[i] = gval_lit(inode);
    }
    Literal output = gval_lit(node);
    const LogExpr& expr = node->expr();
    make_cnf(solver, expr, output, inputs);
  }
  ymuint npo = mLutNetwork->n_outputs();
  for (ymuint i = 0; i < npo; ++ i) {
    LnNode* node = mLutNetwork->output(i);
    LnNode* inode = node->fanin(0);
    Literal input = gval_lit(inode);
    Literal output = gval_lit(node);
    solver->add_clause(~input,  output);
    solver->add_clause( input, ~output);
  }
  
  // 変更回路の CNF を生成
  {
#if 0
    Literal lit0(solver->new_var(), kPolPosi);
    for (ymuint p = 0; p < np; ++ p) {
      vector<Literal> tmp;
      tmp.reserve(ni3 + 2);
      for (ymuint i = 0; i < ni3; ++ i) {
	LnNode* inode = support[i];
	Literal ilit = gval_lit(inode);
	if ( p & (1U << i) ) {
	  tmp.push_back(~ilit);
	}
	else {
	  tmp.push_back(ilit);
	}
      }
      vector<Literal> tmp2(tmp);
      tmp2.push_back(lutvect_lit[p]);
      tmp2.push_back(~lit0);
      solver->add_clause(tmp2);
      tmp2 = tmp;
      tmp2.push_back(~lutvect_lit[p]);
      tmp2.push_back(lit0);
      solver->add_clause(tmp2);
    }
    Literal lit1 = fval_lit(node1);
    Literal lit2 = fval_lit(node2);
    solver->add_clause( lit0, ~lit1);
    solver->add_clause(~lit0,  lit1);
    solver->add_clause( lit0, ~lit2);
    solver->add_clause(~lit0,  lit2);
#else
    for (ymuint p = 0; p < np; ++ p) {
      if ( lutvect[p] == 0 ) {
	vector<Literal> tmp;
	tmp.reserve(ni3 + 1);
	for (ymuint i = 0; i < ni3; ++ i) {
	  LnNode* inode = support[i];
	  Literal ilit = gval_lit(inode);
	  if ( p & (1U << i) ) {
	    tmp.push_back(~ilit);
	  }
	  else {
	    tmp.push_back(ilit);
	  }
	}
	tmp.push_back(~olit);
	solver->add_clause(tmp);
      }
      else if ( lutvect[p] == 1 ) {
	vector<Literal> tmp;
	tmp.reserve(ni3 + 1);
	for (ymuint i = 0; i < ni3; ++ i) {
	  LnNode* inode = support[i];
	  Literal ilit = gval_lit(inode);
	  if ( p & (1U << i) ) {
	    tmp.push_back(~ilit);
	  }
	  else {
	    tmp.push_back(ilit);
	  }
	}
	tmp.push_back(olit);
	solver->add_clause(tmp);
      }
    }
#endif
  }
  
  vector<Literal> tmp;
  tmp.reserve(npo);
  for (ymuint i = 2; i < mQueue1.size(); ++ i) {
    LnNode* node = mQueue1[i];
    ymuint ni = node->ni();
    vector<Literal> inputs(ni);
    for (ymuint j = 0; j < ni; ++ j) {
      LnNode* inode = node->fanin(j);
      if ( mMark[inode->id()] == 3 ) {
	inputs[j] = fval_lit(inode);
      }
      else {
	inputs[j] = gval_lit(inode);
      }
    }
    Literal flit = fval_lit(node);
    if ( node->is_output() ) {
      solver->add_clause(~inputs[0],  flit);
      solver->add_clause( inputs[0], ~flit);
    }
    else {
      const LogExpr& expr = node->expr();
      make_cnf(solver, expr, flit, inputs);
    }
    
    if ( node->is_output() ) {
      Literal dlit = add_dlit(solver, node);
      tmp.push_back(dlit);
    }
  }
  solver->add_clause(tmp);
  
#if 0
  for (ymuint p = 0; p < np; ++ p) {
    if ( lutvect[p] == 1 ) {
      solver->add_clause(lutvect_lit[p]);
    }
    else if ( lutvect[p] == 0 ) {
      solver->add_clause(~lutvect_lit[p]);
    }
  }
#endif
  
  delete solver;
  return false;
  
  bool ans = false;
  for (ymuint c = 1; ; ++ c) {
    if ( debug & debug_check_node_merge ) {
      dout << "  Phase2: round " << c << endl;
    }
    vector<Bool3> model;
    Bool3 stat = solver->solve(model);
    if ( stat == kB3False ) {
      // 変更回路はオリジナル回路と等価
      ans = true;
      break;
    }
    // 外部出力が異なる入力割り当てが存在した．
#if 0
    ymuint p = 0U;
    for (ymuint i = 0; i < ni3; ++ i) {
      LnNode* inode = support[i];
      Literal ilit = gval_lit(inode);
      if ( model[ilit.varid()] == kB3True ) {
	p |= (1U << i);
      }
    }
    if ( lutvect[p] != 2 ) {
      // これは回避不能
      break;
    }
#else
    // 同じ外部入力の値で逆の出力値を試す．
    {
      vector<Literal> tmp;
      tmp.reserve(ni3 + 1);
      for (ymuint i = 0; i < ni3; ++ i) {
	LnNode* inode = support[i];
	Literal ilit = gval_lit(inode);
	if ( model[ilit.varid()] == kB3True ) {
	  tmp.push_back(ilit);
	}
	else if ( model[ilit.varid()] == kB3False ) {
	  tmp.push_back(~ilit);
	}
      }
      bool inv = false;
      if ( model[olit.varid()] == kB3True ) {
	tmp.push_back(~olit);
	inv = true;
      }
      else {
	tmp.push_back(olit);
      }
      vector<Bool3> model2;
      if ( solver->solve(model2) == kB3True ) {
	// 失敗
	break;
      }
    }
      
#endif
    // 回避するための値を設定する．
#if 0
    tVarId id = lutvect_lit[p].varid();
    if ( model[id] == kB3True ) {
      lutvect[p] = 0;
      solver->add_clause(~lutvect_lit[p]);
    }
    else {
      lutvect[p] = 1;
      solver->add_clause(lutvect_lit[p]);
    }
#else
    ymuint p = 0U;
    for (ymuint i = 0; i < ni3; ++ i) {
      LnNode* inode = support[i];
      Literal ilit = gval_lit(inode);
      if ( model[ilit.varid()] == kB3True ) {
	p |= (1U << i);
      }
    }
    vector<Literal> tmp;
    tmp.reserve(ni3 + 1);
    for (ymuint i = 0; i < ni3; ++ i) {
      LnNode* inode = support[i];
      Literal ilit = gval_lit(inode);
      if ( model[ilit.varid()] == kB3True ) {
	tmp.push_back(~ilit);
      }
      else if ( model[ilit.varid()] == kB3False ) {
	tmp.push_back(ilit);
      }
    }
    if ( model[olit.varid()] == kB3True ) {
      tmp.push_back(~olit);
      lutvect[p] = 0;
    }
    else if ( model[olit.varid()] == kB3False ) {
      tmp.push_back(olit);
      lutvect[p] = 1;
    }
    solver->add_clause(tmp);
#endif
  }

  delete solver;

  if ( !ans ) {
    if ( debug & debug_check_node_merge ) {
      dout << "  NG(2)" << endl;
    }
    return false;
  }

  // supoort 数を K 以下にできるか調べる．
  vector<LnNode*> support1;
  vector<int> tv;
  ans = find_support(support, lutvect,
		     support1, tv);
  if ( !ans ) {
    if ( debug & debug_check_node_merge ) {
      dout << "  NG(3)" << endl;
    }
    return false;
  }
  
  // node1, node2 を tv の内容で置き換える．
  LogExpr expr = tv2expr(support1.size(), tv);
  LnNode* new_node = mLutNetwork->new_lut(string(), support1, expr, tv);
  new_node->init_pat(mNpat);
  
  mLutNetwork->subst_node(node1, new_node);
  mLutNetwork->subst_node(node2, new_node);
  
  if ( debug & debug_check_node_merge ) {
    dout << "  OK" << endl;
  }
  
  return true;
}

void
BoolResub::set_tfimark(LnNode* node)
{
  if ( !mTfimark[node->id()] ) {
    mTfimark[node->id()] = true;
    ymuint ni = node->ni();
    for (ymuint i = 0; i < ni; ++ i) {
      set_tfimark(node->fanin(i));
    }
  }
}

void
BoolResub::clear_tfimark(LnNode* node)
{
  if ( mTfimark[node->id()] ) {
    mTfimark[node->id()] = false;
    ymuint ni = node->ni();
    for (ymuint i = 0; i < ni; ++ i) {
      clear_tfimark(node->fanin(i));
    }
  }
}

void
BoolResub::set_tfomark(LnNode* node)
{
  if ( !mTfomark[node->id()] ) {
    mTfomark[node->id()] = true;
    const LnEdgeList& fo_list = node->fanout_list();
    for (LnEdgeList::const_iterator p = fo_list.begin();
	 p != fo_list.end(); ++ p) {
      LnEdge* e = *p;
      set_tfomark(e->to());
    }
  }
}

void
BoolResub::clear_tfomark(LnNode* node)
{
  if ( mTfomark[node->id()] ) {
    mTfomark[node->id()] = false;
    const LnEdgeList& fo_list = node->fanout_list();
    for (LnEdgeList::const_iterator p = fo_list.begin();
	 p != fo_list.end(); ++ p) {
      LnEdge* e = *p;
      clear_tfomark(e->to());
    }
  }
}

void
BoolResub::delete_tfi(LnNode* node)
{
  bool ok = true;
  const LnEdgeList& fo_list = node->fanout_list();
  for (LnEdgeList::const_iterator p = fo_list.begin();
       p != fo_list.end(); ++ p) {
    LnEdge* e = *p;
    LnNode* onode = e->to();
    if ( !mDmark[onode->id()] ) {
      ok = false;
      break;
    }
  }
  if ( ok ) {
    mDmark[node->id()] = true;
    ymuint ni = node->ni();
    for (ymuint i = 0; i < ni; ++ i) {
      delete_tfi(node->fanin(i));
    }
  }
}

/// @brief ノードの入出力の関係を表す CNF クローズを生成する．
/// @param[in] solver SAT ソルバ
/// @param[in] lexp 論理式
/// @param[in] output 出力リテラル
/// @param[in] ipos_begin 入力リテラルの開始位置
/// @param[in] ipos_end 入力リテラルの終了位置
void
BoolResub::make_cnf(SatSolver* solver,
		    const LogExpr& lexp,
		    Literal output,
		    const vector<Literal>& inputs)
{
  if ( lexp.is_literal() ) {
    assert_not_reached(__FILE__, __LINE__);
    return;
  }
  if ( lexp.is_one() ) {
    solver->add_clause(output);
    return;
  }
  if ( lexp.is_zero() ) {
    solver->add_clause(~output);
    return;
  }
  
  ymuint nc = lexp.child_num();
  vector<Literal> local_inputs(nc);
  for (ymuint i = 0; i < nc; ++ i) {
    LogExpr lexp1 = lexp.child(i);
    if ( lexp1.is_posiliteral() ) {
      local_inputs[i] = inputs[lexp1.varid()];
    }
    else if ( lexp1.is_negaliteral() ) {
      local_inputs[i] = ~inputs[lexp1.varid()];
    }
    else {
      ymuint new_varid = solver->new_var();
      local_inputs[i] = Literal(new_varid, kPolPosi);
      make_cnf(solver, lexp1, local_inputs[i], inputs);
    }
  }

  mTmpLit.clear();
  mTmpLit.resize(nc + 1);
  if ( lexp.is_and() ) {
    for (ymuint i = 0; i < nc; ++ i) {
      mTmpLit[i] = ~local_inputs[i];
    }
    mTmpLit[nc] = output;
    solver->add_clause(mTmpLit);
    for (ymuint i = 0; i < nc; ++ i) {
      solver->add_clause(local_inputs[i], ~output);
    }
  }
  else if ( lexp.is_or() ) {
    for (ymuint i = 0; i < nc; ++ i) {
      mTmpLit[i] = local_inputs[i];
    }
    mTmpLit[nc] = ~output;
    solver->add_clause(mTmpLit);
    for (ymuint i = 0; i < nc; ++ i) {
      solver->add_clause(~local_inputs[i], output);
    }
  }
  else if ( lexp.is_xor() ) {
    ymuint np = (1U << nc);
    for (ymuint p = 0; p < np; ++ p) {
      ymuint c = 0;
      for (ymuint i = 0; i < nc; ++ i) {
	if ( p & (1U << i) ) {
	  mTmpLit[i] = local_inputs[i];
	}
	else {
	  mTmpLit[i] = ~local_inputs[i];
	  ++ c;
	}
      }
      if ( (c % 2) == 0 ) {
	mTmpLit[nc] = ~output;
      }
      else {
	mTmpLit[nc] = output;
      }
      solver->add_clause(mTmpLit);
    }
  }
}

// @brief 一定数以下のサポートを求める．
// @param[in] support もとのサポート
// @param[in] vect 仕様となる不完全指定論理関数
// @param[in] new_support 結果のサポートを納めるベクタ
// @param[in] new_vect 結果の真理値表
// @note サイズが mMaxNi 以下のサポートを探す．
bool
BoolResub::find_support(const vector<LnNode*>& support,
			const vector<int>& vect,
			vector<LnNode*>& new_support,
			vector<int>& new_vect)
{
  ymuint ni = support.size();
  ymuint np = vect.size();
  assert_cond((1U << ni) == np, __FILE__, __LINE__);

  if ( ni <= mMaxNi ) {
    new_support.resize(ni);
    new_vect.resize(np);
    for (ymuint i = 0; i < ni; ++ i) {
      new_support[i] = support[i];
    }
    for (ymuint p = 0; p < np; ++ p) {
      if ( vect[p] == 1 ) {
	new_vect[p] = 1;
      }
      else {
	new_vect[p] = 0;
      }
    }
    return true;
  }

  // vect から ON-set と OFF-set のカバーを作る．

  // ON-set のキューブと OFF-set のキューブのすべての対に対して
  // 相反する変数のグループを求める．
  // その変数グループの中の最低1つは残されていないと ON-set と OFF-set
  // が交わってしまう．
  return false;
}
  
// @brief 真理値表から論理式を得る．
LogExpr
BoolResub::tv2expr(ymuint ni,
		   const vector<int>& tv)
{
  ymuint np = 1U << ni;
  assert_cond(np == tv.size(), __FILE__, __LINE__);

  LogExpr ans = LogExpr::make_zero();
  for (ymuint p = 0U; p < np; ++ p) {
    if ( tv[p] ) {
      LogExpr prd = LogExpr::make_one();
      for (ymuint i = 0; i < ni; ++ i) {
	if ( p & (1U << i) ) {
	  prd &= LogExpr::make_posiliteral(i);
	}
	else {
	  prd &= LogExpr::make_negaliteral(i);
	}
      }
      ans |= prd;
    }
  }
  return ans;
}

END_NAMESPACE_MAGUS_LUTMAP
