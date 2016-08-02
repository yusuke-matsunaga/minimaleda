
/// @file magus/src/lutmap/CalcOdc.cc
/// @brief CalcOdc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CalcOdc.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include "magus_config.h"
#endif

#include "CalcOdc.h"


BEGIN_NAMESPACE_MAGUS_LUTMAP

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// ODC 計算用のデータ構造
//////////////////////////////////////////////////////////////////////
class OdcNode
{
public:
  
  // OdcNode のタイプ
  enum tType {
    kConst0,
    kConst1,
    kPosiLiteral,
    kNegaLiteral,
    kAnd,
    kOr,
    kXor
  };


private:

  // コンストラクタ
  OdcNode();
  
  // デストラクタ
  ~OdcNode();


public:

  /// @brief OdcNode の木を生成する．
  /// @param[in] expr 論理式
  /// @param[in] leaf_gf 葉の global function
  /// @param[in] bddmgr BddMgr
  /// @param[in] alloc メモリアロケータ
  static
  OdcNode*
  make_tree(const LogExpr& expr,
	    const vector<Bdd>& leaf_gf,
	    BddMgrRef& bddmgr,
	    AllocBase& alloc);
  
  /// @brief OdcNode の木を削除する．
  /// @param[in] node 対象のノード
  /// @param[in] alloc メモリアロケータ
  static
  void
  delete_tree(OdcNode* node,
	      AllocBase& alloc);


public:

  /// @brief ODC を計算する
  void
  calc_odc(Bdd odc,
	   vector<Bdd>& leaf_odc);

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // タイプ
  tType mType;
  
  // 葉のノードの場合のリテラル番号
  tVarId mVarId;

  // global function
  Bdd mGf;
  
  // 入力のリスト
  vector<OdcNode*> mInputs;
  
};


// コンストラクタ
OdcNode::OdcNode()
{
}
  
// デストラクタ
OdcNode::~OdcNode()
{
}

// @brief OdcNode の木を生成する．
// @param[in] expr 論理式
// @param[in] leaf_gf 葉の global function
// @param[in] bddmgr BddMgr
// @param[in] alloc メモリアロケータ
OdcNode*
OdcNode::make_tree(const LogExpr& expr,
		   const vector<Bdd>& leaf_gf,
		   BddMgrRef& bddmgr,
		   AllocBase& alloc)
{
  void* p = alloc.get_memory(sizeof(OdcNode));
  OdcNode* node = new (p) OdcNode;
  
  if ( expr.is_zero() ) {
    node->mType = kConst0;
    node->mGf = bddmgr.make_zero();
  }
  else if ( expr.is_one() ) {
    node->mType = kConst1;
    node->mGf = bddmgr.make_one();
  }
  else if ( expr.is_posiliteral() ) {
    node->mType = kPosiLiteral;
    node->mVarId = expr.varid();
    node->mGf = leaf_gf[expr.varid()];
    return node;
  }
  else if ( expr.is_negaliteral() ) {
    node->mType = kNegaLiteral;
    node->mVarId = expr.varid();
    node->mGf = ~leaf_gf[expr.varid()];
  }
  else if ( expr.is_and() ) {
    node->mType = kAnd;
    ymuint32 nc = expr.child_num();
    node->mInputs.resize(nc, NULL);
    Bdd f = bddmgr.make_one();
    for (ymuint32 i = 0; i < nc; ++ i) {
      OdcNode* inode = make_tree(expr.child(i), leaf_gf, bddmgr, alloc);
      node->mInputs[i] = inode;
      f &= inode->mGf;
    }
    node->mGf = f;
  }
  else if ( expr.is_or() ) {
    node->mType = kOr;
    ymuint32 nc = expr.child_num();
    node->mInputs.resize(nc, NULL);
    Bdd f = bddmgr.make_zero();
    for (ymuint32 i = 0; i < nc; ++ i) {
      OdcNode* inode = make_tree(expr.child(i), leaf_gf, bddmgr, alloc);
      node->mInputs[i] = inode;
      f |= inode->mGf;
    }
    node->mGf = f;
  }
  else if ( expr.is_xor() ) {
    node->mType = kXor;
    ymuint32 nc = expr.child_num();
    node->mInputs.resize(nc, NULL);
    Bdd f = bddmgr.make_zero();
    for (ymuint32 i = 0; i < nc; ++ i) {
      OdcNode* inode = make_tree(expr.child(i), leaf_gf, bddmgr, alloc);
      node->mInputs[i] = inode;
      f |= inode->mGf;
    }
    node->mGf = f;
  }
  else {
    assert_not_reached(__FILE__, __LINE__);
  }
  return node;
}
  
// @brief OdcNode の木を削除する．
// @param[in] node 対象のノード
// @param[in] alloc メモリアロケータ
void
OdcNode::delete_tree(OdcNode* node,
		     AllocBase& alloc)
{
  switch ( node->mType ) {
  case kAnd:
  case kOr:
  case kXor:
    for (vector<OdcNode*>::iterator p = node->mInputs.begin();
	 p != node->mInputs.end(); ++ p) {
      delete_tree(*p, alloc);
    }
    break;
  default:
    break;
  }
  
  // デストラクタを明示的に呼ぶ珍しいコード
  node->~OdcNode();
  alloc.put_memory(sizeof(OdcNode), node);
}

// @brief ODC を計算する
void
OdcNode::calc_odc(Bdd odc,
		  vector<Bdd>& leaf_odc)
{
  switch ( mType ) {
  case kConst0:
  case kConst1:
    break;
    
  case kPosiLiteral:
  case kNegaLiteral:
    leaf_odc[mVarId] |= odc;
    break;

  case kAnd:
    { // 優先順位の高い他の入力が 0 を出している場合と
      // 自分が 1 で出力が 0 の場合が don't care となる．
      ymuint32 n = mInputs.size();
      for (ymuint32 i = 0; i < n; ++ i) {
	OdcNode* inode = mInputs[i];
	Bdd i_odc = odc;
	for (ymuint32 j = 0; j < i; ++ j) {
	  i_odc |= ~mInputs[j]->mGf;
	}
	i_odc |= inode->mGf & ~mGf;
	inode->calc_odc(i_odc, leaf_odc);
      }
    }
    break;

  case kOr:
    { // 優先順位の高い他の入力が 1 を出している場合と
      // 自分が 0 で出力が 1 の場合が don't care となる．
      ymuint32 n = mInputs.size();
      for (ymuint32 i = 0; i < n; ++ i) {
	OdcNode* inode = mInputs[i];
	Bdd i_odc = odc;
	for (ymuint32 j = 0; j < i; ++ j) {
	  i_odc |= mInputs[j]->mGf;
	}
	i_odc |= ~inode->mGf & mGf;
	inode->calc_odc(i_odc, leaf_odc);
      }
    }
    break;

  case kXor:
    { // XOR は don't care を生み出さない．
      ymuint32 n = mInputs.size();
      for (ymuint32 i = 0; i < n; ++ i) {
	mInputs[i]->calc_odc(odc, leaf_odc);
      }
    }
    break;
  }
}

// check_odc のサブルーティン
bool
check_odc_sub(BddMgrRef bddmgr,
	      Bdd lf,
	      const vector<Bdd>& leaf_gf,
	      const vector<Bdd>& leaf_odc,
	      ymuint32 pos,
	      ymuint32 n)
{
  if ( pos == n ) {
    return lf.is_const();
  }
  else {
    {
      // pos 番目の入力が 0 の領域に束縛する．
      Bdd range0 = ~leaf_gf[pos] & ~leaf_odc[pos];
      vector<Bdd> leaf_gf0(n);
      vector<Bdd> leaf_odc0(n);
      for (ymuint32 i = pos + 1; i < n; ++ i) {
	leaf_gf0[i] = leaf_gf[i] & range0;
	leaf_odc0[i] = leaf_odc[i] & range0;
      }
      // lf も束縛する．
      Bdd lf0 = lf / bddmgr.make_negaliteral(pos);
      if ( !check_odc_sub(bddmgr, lf0, leaf_gf0, leaf_odc0, pos + 1, n) ) {
	return false;
      }
    }
    
    {
      // pos 番目の入力が 1 の領域に束縛する．
      Bdd range1 = leaf_gf[pos] & ~leaf_odc[pos];
      vector<Bdd> leaf_gf1(n);
      vector<Bdd> leaf_odc1(n);
      for (ymuint32 i = pos + 1; i < n; ++ i) {
	leaf_gf1[i] = leaf_gf[i] & range1;
	leaf_odc1[i] = leaf_odc[i] & range1;
      }
      // lf も束縛する．
      Bdd lf1 = lf / bddmgr.make_posiliteral(pos);
      if ( !check_odc_sub(bddmgr, lf1, leaf_gf1, leaf_odc1, pos + 1, n) ) {
	return false;
      }
    }
    
    {
      // pos 番目の入力がドントケアの領域に束縛する．
      Bdd range2 = leaf_odc[pos];
      vector<Bdd> leaf_gf2(n);
      vector<Bdd> leaf_odc2(n);
      for (ymuint32 i = pos + 1; i < n; ++ i) {
	leaf_gf2[i] = leaf_gf[i] & range2;
	leaf_odc2[i] = leaf_odc[i] & range2;
      }
      if ( !check_odc_sub(bddmgr, lf, leaf_gf2, leaf_odc2, pos + 1, n) ) {
	return false;
      }
    }
  }
  return true;
}
	      
// CalcOdc の検算を行う．
bool
check_odc(const LogExpr& expr,
	  Bdd root_odc,
	  const vector<Bdd>& leaf_gf,
	  const vector<Bdd>& leaf_odc)
{
  BddMgrRef bddmgr = root_odc.mgr();
  Bdd lf = bddmgr.expr_to_bdd(expr);
  ymuint32 n = leaf_gf.size();
  vector<Bdd> leaf_gf1(n);
  vector<Bdd> leaf_odc1(n);
  for (ymuint32 i = 0; i < n; ++ i) {
    leaf_gf1[i] = leaf_gf[i] & ~root_odc;
    leaf_odc1[i] = leaf_odc[i] & ~root_odc;
  }
  return check_odc_sub(bddmgr, lf, leaf_gf1, leaf_odc1, 0, n);
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス CalcOdc
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CalcOdc::CalcOdc() :
  mAlloc(sizeof(OdcNode), 1024)
{
}

// @brief デストラクタ
CalcOdc::~CalcOdc()
{
}

// @brief 論理式の葉の ODC を計算する．
// @param[in] expr 論理式
// @param[in] root_odc 根の部分の ODC
// @param[in] leaf_gf 葉の global function の配列
// @param[out] leaf_odc 葉の ODC
void
CalcOdc::operator()(const LogExpr& expr,
		    Bdd root_odc,
		    const vector<Bdd>& leaf_gf,
		    vector<Bdd>& leaf_odc)
{
  ymuint32 n_leaf = leaf_gf.size();
  assert_cond(n_leaf == leaf_odc.size(), __FILE__, __LINE__);
  BddMgrRef bddmgr = root_odc.mgr();
  for (ymuint32 i = 0; i < n_leaf; ++ i) {
    assert_cond(bddmgr == leaf_gf[i].mgr(), __FILE__, __LINE__);
  }
  
  // OdcNode を作成する．
  OdcNode* root_node = OdcNode::make_tree(expr, leaf_gf, bddmgr, mAlloc);

  // 各ノードの ODC を計算する．
  for (ymuint32 i = 0; i < n_leaf; ++ i) {
    leaf_odc[i] = root_odc;
  }
  root_node->calc_odc(root_odc, leaf_odc);

  // OdcNode を削除する．
  OdcNode::delete_tree(root_node, mAlloc);

  assert_cond( check_odc(expr, root_odc, leaf_gf, leaf_odc), __FILE__, __LINE__);
}

END_NAMESPACE_MAGUS_LUTMAP
