
/// @file magus/logbase/BNetNdCmd.cc
/// @brief BNetNdCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BNetNdCmd.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "BNetNdCmd.h"
#include "ym_tclpp/TclPopt.h"
#include "ym_lexp/LogExprWriter.h"
#include "ym_bnet/BNetManip.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// ノードの諸元を取り出すコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
NodeInfo::NodeInfo(MagMgr* mgr) :
  BNetCmd(mgr)
{
  const char* usage =
    "<ATTR> node\n"
    "\t<ATTR> is name, input_num, fanin_num, fanout_num, lit_num,\n"
    "\t\tsop_lit_num, inputs, fanins, fanouts\n"
    "\t\tfunc, value, mapped_cell, atime, atime_up, atime_down,\n"
    "\t\trtime, rtime_up, rtime_down, slew, slew_up, slew_down,\n"
    "\t\tload, load_up, load_down, slack\n";
  set_usage_string(usage);
}

// @brief デストラクタ
NodeInfo::~NodeInfo()
{
}

// コマンドを実行する．
int
NodeInfo::cmd_proc(TclObjVector& objv)
{
  size_t objc = objv.size();

  // このコマンドはノード名と属性の2つの引数をとる．
  if ( objc != 3 ) {
    // 引数の数が合わない．
    print_usage();
    // 異常終了
    return TCL_ERROR;
  }

  // ノードを取り出す．
  string node_str = objv[2].get_string(NULL);
  BNode* node = cur_network()->find_node(node_str);
  if ( node == NULL ) {
    TclObj emsg;
    emsg << node_str << " : does not exist";
    set_result(emsg);
    return TCL_ERROR;
  }

  string attr = objv[1];
  TclObj result;
  if ( attr == "name" ) {
    result = node->name();
  }
  else if ( attr == "input_num" || attr == "fanin_num" ) {
    result = node->ni();
  }
  else if ( attr == "fanout_num" ) {
    result = node->fanout_num();
  }
  else if ( attr == "lit_num" ) {
    result = node->litnum();
  }
  else if ( attr == "sop_lit_num" ) {
    result = node->sop_litnum();
  }
  else if ( attr == "inputs" || attr == "fanins" ) {
    result.clear();
    size_t ni = node->ni();
    for (size_t i = 0; i < ni; i ++) {
      BNode* inode = node->fanin(i);
      result.append_element(inode->name());
    }
  }
  else if ( attr == "fanouts" ) {
    result.clear();
    for (BNodeFoList::const_iterator p = node->fanouts_begin();
	 p != node->fanouts_end(); ++ p) {
      BNodeEdge* edge = *p;
      BNode* onode = edge->to();
      result.append_element(onode->name());
    }
  }
  else if ( attr == "func" ) {
    LogExprWriter writer;
    result = writer.dump_string(node->func());
  }
  else if ( attr == "value" ) {
    result = node->value();
  }
#if defined(USE_LIBCELL)
  else if (attr == "mapped_cell") {
    const hf_CUcell* cell = node->Cell();
    if (cell != NULL) {
      result = Cell2TclObj(cell);
    }
    else {
      result = TclObj("not mapped");
    }
  }
  else if (attr == "atime") {
    const hf_CTime2& atime = node->Atime();
    TclObj tmp[2];
    tmp[0] = atime[hf_kBvUp];
    tmp[1] = atime[hf_kBvDn];
    result.SetList(2, tmp);
  }
  else if (attr == "atime_up") {
    const hf_CTime2& atime = node->Atime();
    result = atime[hf_kBv1];
  }
  else if (attr == "atime_down") {
    const hf_CTime2& atime = node->Atime();
    result = atime[hf_kBv0];
  }
  else if (attr == "rtime") {
    const hf_CTime2& rtime = node->Rtime();
    TclObj tmp[2];
    tmp[0] = rtime[hf_kBvUp];
    tmp[1] = rtime[hf_kBvDn];
    result.SetList(2, tmp);
  }
  else if (attr == "rtime_up") {
    const hf_CTime2& rtime = node->Rtime();
    result = rtime[hf_kBv1];
  }
  else if (attr == "rtime_down") {
    const hf_CTime2& rtime = node->Rtime();
    result = rtime[hf_kBv0];
  }
  else if (attr == "slew") {
    const hf_CSlew2& slew = node->Slew();
    TclObj tmp[2];
    tmp[0] = slew[hf_kBvUp];
    tmp[1] = slew[hf_kBvDn];
    result.SetList(2, tmp);
  }
  else if (attr == "slew_up") {
    const hf_CSlew2& slew = node->Slew();
    result = slew[hf_kBv1];
  }
  else if (attr == "slew_down") {
    const hf_CSlew2& slew = node->Slew();
    result = slew[hf_kBv0];
  }
  else if (attr == "load") {
    const hf_CLoad2& load = node->Load();
    TclObj tmp[2];
    tmp[0] = load[hf_kBvUp];
    tmp[1] = load[hf_kBvDn];
    result.SetList(2, tmp);
  }
  else if (attr == "load_up") {
    const hf_CLoad2& load = node->Load();
    result = load[hf_kBv1];
  }
  else if (attr == "load_down") {
    const hf_CLoad2& load = node->Load();
    result = load[hf_kBv0];
  }
  else if (attr == "slack") {
    const hf_CTime2& slack = node->Slack();
    TclObj tmp[2];
    tmp[0] = slack[hf_kBvUp];
    tmp[1] = slack[hf_kBvDn];
    result.SetList(2, tmp);
  }
#endif
  else {
    TclObj emsg;
    emsg << attr << " : illegal attribute";
    set_result(emsg);
    return TCL_ERROR;
  }
  set_result(result);

  // 正常終了
  return TCL_OK;
}


//////////////////////////////////////////////////////////////////////
// ノード名のリストを作るコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ListNode::ListNode(MagMgr* mgr) :
  AllNodeCmd(mgr)
{
}

// @brief デストラクタ
ListNode::~ListNode()
{
}

// foreach_node() の前処理
int
ListNode::before_node_proc()
{
  // 結果を納めるオブジェクトをクリアしておく．
  mResult.clear();

  return TCL_OK;
}

// node を mResult に追加する．
int
ListNode::node_proc(BNode* node)
{
  int code = mResult.append_element(node->name(), interp());
  return code;
}

// foreach_node() の後処理
int
ListNode::after_node_proc()
{
  // 結果にセットする．
  set_result(mResult);

  // 念のため mResult をクリア
  mResult.clear();

  return TCL_OK;
}


//////////////////////////////////////////////////////////////////////
// ノードの elimination を行う．
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElimNode::ElimNode(MagMgr* mgr) :
  NodeCmd(mgr)
{
}

// @brief デストラクタ
ElimNode::~ElimNode()
{
}

// name という名のノードを eliminate する．
int
ElimNode::node_proc(BNode* node)
{
  // elimination を行う．
  BNetManip manip(cur_network());
  manip.eliminate_node(node);

  // 正常終了
  return TCL_OK;
}


//////////////////////////////////////////////////////////////////////
// ノードの削除を行う．
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DelNode::DelNode(MagMgr* mgr) :
  NodeCmd(mgr)
{
}

// @brief デストラクタ
DelNode::~DelNode()
{
}

// name という名前のノードを delete する．
int
DelNode::node_proc(BNode* node)
{
  // ファンアウトがあったら削除できない．
  if ( node->fanout_num() > 0 ) {
    TclObj emsg;
    emsg << node->name() << " has fanouts";
    set_result(emsg);
    return TCL_ERROR;
  }

  // このノードを削除する．
  BNetManip manip(cur_network());
  manip.delete_node(node);

  return TCL_OK;
}


//////////////////////////////////////////////////////////////////////
// ノードの論理式の簡単化を行う．
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SimplifyNode::SimplifyNode(MagMgr* mgr) :
  AllNodeCmd(mgr)
{
}

// @brief デストラクタ
SimplifyNode::~SimplifyNode()
{
}

// name という名前のノードを simplify する．
int
SimplifyNode::node_proc(BNode* node)
{
  // 論理式の簡単化を行う．
  cur_network()->lexp_simplify_node(node);

  // 正常終了
  return TCL_OK;
}


END_NAMESPACE_MAGUS
