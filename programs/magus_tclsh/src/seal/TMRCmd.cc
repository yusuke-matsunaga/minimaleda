
/// @file magus/src/seal/TMRCmd.cc
/// @brief TMRCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TMRCmd.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "TMRCmd.h"
#include "ym_bnet/BNetManip.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// 回路を3重化するコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
TMRCmd::TMRCmd(MagMgr* mgr) :
  BNetCmd(mgr)
{
}

// @brief デストラクタ
TMRCmd::~TMRCmd()
{
}

// コマンド処理関数
int
TMRCmd::cmd_proc(TclObjVector& objv)
{
  BNetwork& network = *cur_network();
  BNetManip manip(&network);

  vector<BNode*> fanins;
  LogExpr l0 = LogExpr::make_posiliteral(0);
  LogExpr l1 = LogExpr::make_posiliteral(1);
  LogExpr l2 = LogExpr::make_posiliteral(2);
  LogExpr maj_exp = (l0 & l1) | (l1 & l2) | (l2 & l0);

  // 論理ノードの複製を2つづつ作る．
  bool stat;
  size_t max_id = network.max_node_id();
  vector<BNode*> map1(max_id);
  vector<BNode*> map2(max_id);

  // 外部入力は複製しないで map1/map2 に登録する．
  for (BNodeList::const_iterator p = network.inputs_begin();
       p != network.inputs_end(); ++ p) {
    BNode* node = *p;
    map1[node->id()] = node;
    map2[node->id()] = node;
  }

  BNodeVector node_list;
  network.tsort(node_list);
  for (BNodeVector::iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    BNode* node = *p;
    size_t ni = node->ni();
    fanins.resize(ni);
    LogExpr lexp = node->func();

    // 1つめの複製
    BNode* node1 = manip.new_logic();
    for (size_t i = 0; i < ni; ++ i) {
      fanins[i] = map1[node->fanin(i)->id()];
    }
    stat = manip.change_logic(node1, lexp, fanins);
    assert_cond(stat, __FILE__, __LINE__);
    map1[node->id()] = node1;

    // 2つめの複製
    BNode* node2 = manip.new_logic();
    for (size_t i = 0; i < ni; ++ i) {
      fanins[i] = map2[node->fanin(i)->id()];
    }
    stat = manip.change_logic(node2, lexp, fanins);
    assert_cond(stat, __FILE__, __LINE__);
    map2[node->id()] = node2;
  }

  // 複製されたノードを使って出力に多数決論理を作る．
  for (BNodeList::const_iterator p = network.outputs_begin();
       p != network.outputs_end(); ++ p) {
    // 外部出力
    BNode* obnode = *p;
    // 外部出力から参照されているノード
    BNode* ibnode = obnode->fanin(0);

    // ibnode の複製をもう一つ作る．
    size_t ni = ibnode->ni();
    fanins.resize(ni);
    for (size_t i = 0; i < ni; ++ i) {
      fanins[i] = ibnode->fanin(i);
    }
    LogExpr lexp = ibnode->func();
    BNode* ibnode0 = manip.new_logic();
    bool stat = manip.change_logic(ibnode0, lexp, fanins);
    assert_cond(stat, __FILE__, __LINE__);

    // その3つの多数決論理を作る．
    fanins.resize(3);
    fanins[0] = ibnode0;
    fanins[1] = map1[ibnode->id()];
    fanins[2] = map2[ibnode->id()];
    stat = manip.change_logic(ibnode, maj_exp, fanins);
    assert_cond(stat, __FILE__, __LINE__);
  }

  return TCL_OK;
}

END_NAMESPACE_MAGUS
