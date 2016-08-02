
/// @file magus/src/lutmap/OdcMgr.cc
/// @brief OdcMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: OdcMgr.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include "magus_config.h"
#endif

#include "OdcMgr.h"
#include <ym_bnet/BNetwork.h>


BEGIN_NAMESPACE_MAGUS_LUTMAP

// @brief コンストラクタ
OdcMgr::OdcMgr()
{
}

// @brief デストラクタ
OdcMgr::~OdcMgr()
{
}

// @brief ODC の計算を行う．
// @param[in] bddmgr BDD マネージャ
// @param[in] bnetwork 対象のネットワーク
void
OdcMgr::calc_odc(BddMgrRef& bddmgr,
		 const BNetwork& bnetwork)
{
  ymuint32 n = bnetwork.max_node_id();
  mNodeData.resize(n);
  
  // global function の計算
  cout << "calculating GF" << endl;
  ymuint32 index = 0;
  for (BNodeList::const_iterator p = bnetwork.inputs_begin();
       p != bnetwork.inputs_end(); ++ p, ++ index) {
    BNode* node = *p;
    NodeData& data = mNodeData[node->id()];
    data.mGf = bddmgr.make_posiliteral(index);
    cout << "  " << node->name() << endl;
  }
  BNodeVector node_list;
  bnetwork.tsort(node_list);
  for (BNodeVector::const_iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    BNode* node = *p;
    ymuint32 ni = node->ni();
    VarBddMap varmap;
    for (ymuint32 i = 0; i < ni; ++ i) {
      BNode* inode = node->fanin(i);
      Bdd igf = mNodeData[inode->id()].mGf;
      varmap.insert(make_pair(i, igf));
    }
    mNodeData[node->id()].mGf = bddmgr.expr_to_bdd(node->func(), varmap);
    cout << "  " << node->name() << endl;
  }
  
  // ODC の計算
  cout << "calculating ODC" << endl;
  bnetwork.tsort(node_list, true);
  for (BNodeVector::const_iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    BNode* node = *p;
    NodeData& node_data = mNodeData[node->id()];
    Bdd odc = bddmgr.make_one();
    for (BNodeFoList::const_iterator q = node->fanouts_begin();
	 q != node->fanouts_end(); ++ q) {
      BNodeEdge* e = *q;
      BNode* onode = e->to();
      if ( onode->is_output() ) {
	odc = bddmgr.make_zero();
	break;
      }
      odc &= input_odc(onode, e->pos());
    }
    node_data.mOutputOdc = odc;
    ymuint32 ni = node->ni();
    vector<Bdd> leaf_gf(ni);
    node_data.mInputOdc.resize(ni);
    for (ymuint32 i = 0; i < ni; ++ i) {
      leaf_gf[i] = mNodeData[node->fanin(i)->id()].mGf;
    }
    mCalcOdc(node->func(), odc, leaf_gf, node_data.mInputOdc);
    cout << "  " << node->name() << endl;
  }
  cout << "end" << endl;
}

// @brief ノードの出力の ODC を得る．
// @param[in] node 対象のノード
Bdd
OdcMgr::output_odc(const BNode* node)
{
  return mNodeData[node->id()].mOutputOdc;
}

// @brief ノードの入力の ODC を得る．
// @param[in] node 対象のノード
// @param[in] pos 入力位置
Bdd
OdcMgr::input_odc(const BNode* node,
		  ymuint32 pos)
{
  return mNodeData[node->id()].mInputOdc[pos];
}
 


END_NAMESPACE_MAGUS_LUTMAP
