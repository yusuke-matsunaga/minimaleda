
/// @file magus/logbase/BNetNodeCmd.cc
/// @brief BNetNodeCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: NodeCmd.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "BNetNodeCmd.h"
#include "ym_tclpp/TclPopt.h"
#include "ym_bnet/BNetwork.h"


BEGIN_NAMESPACE_MAGUS


//////////////////////////////////////////////////////////////////////
// 複数のノード名を引数にとって処理を行なうコマンドの親クラス NodeCmd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
NodeCmd::NodeCmd(MagMgr* mgr) :
  BNetCmd(mgr)
{
  mPoptGlob = new TclPopt(this, "glob",
			  "glob matching");
  mPoptRegexp = new TclPopt(this, "regexp",
			    "regex matching");
  new_popt_group(mPoptGlob, mPoptRegexp);

  mPoptAscOrder = new TclPopt(this, "ascent",
			      "ascent order");
  mPoptDscOrder = new TclPopt(this, "decent",
			      "decent order");
  new_popt_group(mPoptAscOrder, mPoptDscOrder);

  mPoptCond = new TclPoptObj(this, "cond",
			     "specify condition",
			     "<expression>");

  mPoptPI = new TclPopt(this, "pi",
			"enumerate PIs");
  mPoptPO = new TclPopt(this, "po",
			"enumerate POs");
  mPoptLogic = new TclPopt(this, "logic",
			   "enumerate logic nodes");
  mPoptFF = new TclPopt(this, "ff",
			"enumerate FFs");
  new_popt_group(mPoptPI, mPoptPO, mPoptLogic, mPoptFF);

  set_usage_string("<node-name>* or <pattern>*");
}

// @brief デストラクタ
NodeCmd::~NodeCmd()
{
}

// コマンド行のオプション引数を解析する．
int
NodeCmd::parse_args(TclObjVector& objv)
{
  mMatchMode = kNONE;
  mNodeType = kINT;
  mNodeOrder = kNRM;
  mNodeCond = true;
  mBaseNode = NULL;

  if ( mPoptGlob->is_specified() ) {
    mMatchMode = kGLOB;
  }
  if ( mPoptRegexp->is_specified() ) {
    mMatchMode = kREGEXP;
  }

  if ( mPoptAscOrder->is_specified() ) {
    mNodeOrder = kASC;
  }
  if ( mPoptDscOrder->is_specified() ) {
    mNodeOrder = kDSC;
  }

  if ( mPoptCond->is_specified() ) {
    mNodeCond = mPoptCond->val();
  }

  if ( mPoptPI->is_specified() ) {
    mNodeType = kPI;
  }
  if ( mPoptPO->is_specified() ) {
    mNodeType = kPO;
  }
  if ( mPoptLogic->is_specified() ) {
    mNodeType = kINT;
  }
  if ( mPoptFF->is_specified() ) {
    mNodeType = kFF;
  }

  if ( mNodeType != kINT && mNodeOrder != kNRM ) {
    set_result("-ascent or -descent can not be specified with -pi, -po or -ff");
    return TCL_ERROR;
  }

  return TCL_OK;
}

// マッチングモードを返す．
NodeCmd::tMode
NodeCmd::match_mode() const
{
  return mMatchMode;
}

// 引数の情報を基に処理すべきノードを配列にセットする．
// 引数が何も与えられなかったときの振る舞いを変えたいので仮想関数と
// している．
// デフォルトの定義では空の引数ならノードを処理しない．
// 普通は下の2つの set_nodes() を組み合わせれば良いはず．
int
NodeCmd::set_nodes(TclObjVector& objv)
{
  int code;
  if ( match_mode() == kNONE ) {
    code = set_nodes_from_args(objv);
  }
  else {
    code = set_all_nodes(objv);
  }
  return code;
}

// 引数をノード名だと思って配列に納める．
int
NodeCmd::set_nodes_from_args(TclObjVector& objv)
{
  BNetwork* network = cur_network();
  size_t objc = objv.size();
  init_array(objc - 1);
  for (size_t i = 1; i < objc; ++ i) {
    string node_str = objv[i];
    BNode* node = network->find_node(node_str);
    if ( node == NULL ) {
      // ノード名が不正
      TclObj emsg;
      emsg << node_str << " : does not exist";
      set_result(emsg);
      return TCL_ERROR;
    }
    if ( mNodeType == kINT && node->is_output() ) {
      node = node->fanin(0);
    }
    mNodeArray.push_back(node);
  }

  return TCL_OK;
}

// ノードのタイプおよび処理順に従ってノードを配列に納める．
int
NodeCmd::set_all_nodes(TclObjVector& objv)
{
  size_t objc = objv.size();
  mPats.clear();
  mPats.reserve(objc - 1);
  for (size_t i = 1; i < objc; ++ i) {
    string pat = objv[i];
    if ( match_mode() == kREGEXP ) {
      // パタンが正しいかチェックしておく．
      // dummy はどんな文字列でも良い．
      if ( regexp_match("dummy", pat) == -1 ) {
	// 正規表現のパタンが間違い．
	return TCL_ERROR;
      }
    }
    mPats.push_back(pat);
  }

  // 配列に納める．
  switch ( mNodeType ) {
  case kPI:
    set_pi_node();
    break;
  case kPO:
    set_po_node();
    break;
  case kFF:
    set_latch_node();
    break;
  case kFI:
    set_fanin(mBaseNode);
    break;
  case kFO:
    set_fanout(mBaseNode);
    break;
  case kINT:
    switch (mNodeOrder) {
    case kNRM:
      set_logic_node();
      break;
    case kASC:
      set_logic_node_asc();
      break;
    case kDSC:
      set_logic_node_dsc();
      break;
    }
    break;
  default:
    break;
  }
  return TCL_OK;
}

// 外部入力ノードを適当な順番で取り出し，
// パタンにマッチするものを配列に入れる．
void
NodeCmd::set_pi_node()
{
  BNetwork* network = cur_network();
  size_t n = network->input_num();
  init_array(n);
  for (BNodeList::const_iterator p = network->inputs_begin();
       p != network->inputs_end(); ++ p) {
    BNode* node = *p;
    add_node(node);
  }
}

// 外部出力ノードを適当な順番で取り出し，
// パタンにマッチするものを配列に入れる．
void
NodeCmd::set_po_node()
{
  BNetwork* network = cur_network();
  size_t n = network->output_num();
  init_array(n);
  for (BNodeList::const_iterator p = network->outputs_begin();
       p != network->outputs_end(); ++ p) {
    BNode* node = *p;
    add_node(node);
  }
}

// ラッチノードを適当な順番で取り出し，
// パタンにマッチするものを配列に入れる．
void
NodeCmd::set_latch_node()
{
  BNetwork* network = cur_network();
  size_t n = network->latch_node_num();
  init_array(n);
  for (BNodeList::const_iterator p = network->latch_nodes_begin();
       p != network->latch_nodes_end(); ++ p) {
    BNode* node = *p;
    add_node(node);
  }
}

// ロジックノードを適当な順番で取り出し，
// パタンにマッチするものを配列に入れる．
void
NodeCmd::set_logic_node()
{
  BNetwork* network = cur_network();
  size_t n = network->logic_node_num();
  init_array(n);
  for (BNodeList::const_iterator p = network->logic_nodes_begin();
       p != network->logic_nodes_end(); ++ p) {
    BNode* node = *p;
    add_node(node);
  }
}

// 入力からのトポロジカル順に中間ノードを取り出し，
// パタンにマッチするものを配列に入れる．
void
NodeCmd::set_logic_node_asc()
{
  BNetwork* network = cur_network();
  network->tsort(mNodeArray);
}

// 出力からのトポロジカル順に中間ノードを取り出し，
// パタンにマッチするものを配列に入れる．
void
NodeCmd::set_logic_node_dsc()
{
  BNetwork* network = cur_network();
  network->tsort(mNodeArray, true);
}

// ノードのファンインを配列に入れる．
void
NodeCmd::set_fanin(BNode* node)
{
  size_t ni = node->ni();
  init_array(ni);
  for (size_t i = 0; i < ni; i ++) {
    BNode* inode = node->fanin(i);
    add_node(inode);
  }
}

// ノードのファンアウトを配列に入れる．
void
NodeCmd::set_fanout(BNode* node)
{
  size_t no = node->fanout_num();
  init_array(no);
  for (BNodeFoList::const_iterator p = node->fanouts_begin();
       p != node->fanouts_end(); ++ p) {
    BNodeEdge* edge = *p;
    BNode* onode = edge->to();
    add_node(onode);
  }
}

// ノード名をとって来てマッチすれば配列に入れる．
void
NodeCmd::add_node(BNode* node)
{
  string name = node->name();
  bool found = false;
  switch ( match_mode() ) {
  case kNONE:
    found = true;
    break;

  case kGLOB:
    for (vector<string>::const_iterator i = mPats.begin();
	 i != mPats.end(); ++ i) {
      const string& pat = *i;
      if ( string_match(name, pat) == 1 ) {
	found = true;
	break;
      }
    }
    break;

  case kREGEXP:
    for (vector<string>::const_iterator i = mPats.begin();
	 i != mPats.end(); ++ i) {
      const string& pat = *i;
      if ( regexp_match(name, pat) == 1 ) {
	found = true;
	break;
      }
    }
    break;
  }
  if ( found ) {
    mNodeArray.push_back(node);
  }
}

// foreach_node() の直前に呼ばれる関数
int
NodeCmd::before_node_proc()
{
  return TCL_OK;
}

// 配列に納められたノードに対して node_proc() を呼び出す．
// 実行結果を返す．
// cond が与えられたときは実行時にノード毎に評価して
// 真の時のみ node_proc() を呼び出す．
int
NodeCmd::foreach_node(const TclObj& cond)
{
  // 配列の内容に対して NodeProc() を呼び出す．
  // その前にそのノードをカレントノードにしておく．
  // ただし，cond がある場合にはその条件に合致するかどうか調べる．
  int result = TCL_OK;
  for (vector<BNode*>::iterator p = mNodeArray.begin();
       p != mNodeArray.end(); ++ p) {
    BNode* node = *p;

    // 条件コマンドを実行する．
    bool b;
    int code = expr_bool(cond, b);
    if ( code == TCL_ERROR ) {
      // 結果がエラー？ 話にならない．
      result = TCL_ERROR;
      break;
    }
    if ( b == false ) {
      // 判定結果が偽なら NodeProc() は実行しない．
      continue;
    }

    // 本当の処理関数を呼び出す．
    code = node_proc(node);
    if ( code == TCL_ERROR ) {
      // 異常終了
      result = TCL_ERROR;
      break;
    }
    if ( code == TCL_BREAK ) {
      // ここでループを抜ける．
      // 実行結果は正常終了
      result = TCL_OK;
      break;
    }
    if ( code == TCL_CONTINUE ) {
      // 次の要素に行くって，ここなら意味ないじゃん
      continue;
    }
  }

  // 終了ステータスを返す．
  return result;
}

// foreach_node() の直後に呼ばれる関数
int
NodeCmd::after_node_proc()
{
  return TCL_OK;
}

// コマンド処理関数
int
NodeCmd::cmd_proc(TclObjVector& objv)
{
  int code;

  code = parse_args(objv);
  if ( code != TCL_OK ) {
    return code;
  }

  // 処理するノードを配列に納める．
  code = set_nodes(objv);
  if ( code == TCL_ERROR ) {
    return TCL_ERROR;
  }

  // 前処理
  code = before_node_proc();
  if ( code == TCL_ERROR ) {
    return TCL_ERROR;
  }

  // 配列からノードを取り出して処理する．
  code = foreach_node(mNodeCond);
  if ( code == TCL_ERROR ) {
    return TCL_ERROR;
  }

  // 後処理
  code = after_node_proc();

  return code;
}

// mNodeArray をクリアして，size ぶんの領域を確保する．
void
NodeCmd::init_array(size_t size)
{
  mNodeArray.clear();
  mNodeArray.reserve(size);
}


//////////////////////////////////////////////////////////////////////
// 複数のノードに対して処理を行うコマンドの親クラス
// NodeCmd との違いは引数が省略されたときに
// すべてのノードを対象にするところ
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
AllNodeCmd::AllNodeCmd(MagMgr* mgr) :
  NodeCmd(mgr)
{
}

// @brief デストラクタ
AllNodeCmd::~AllNodeCmd()
{
}

// 引数の情報を基に処理すべきノードを配列にセットする．
// 引数が何も与えられなかったときにはすべてのノードをセットする．
int
AllNodeCmd::set_nodes(TclObjVector& objv)
{
  int code;
  if ( match_mode() == kNONE && objv.size() > 1 ) {
    code = NodeCmd::set_nodes_from_args(objv);
  }
  else {
    code = NodeCmd::set_all_nodes(objv);
  }
  return code;
}

END_NAMESPACE_MAGUS
