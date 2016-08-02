
/// @file libym_cec/check_ceq.cc
/// @brief 組み合わせ回路の検証を行う関数の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: check_equiv.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cec/cec_nsdef.h"
#include "ym_aig/AigMgr.h"
#include "ym_aig/AigNode.h"
#include "FraigMgr.h"


BEGIN_NAMESPACE_YM_CEC

BEGIN_NONAMESPACE

int debug = 0;

// @brief 2つのネットワークの miter 構造を作る．
// @param[in] src_network1, src_network2 検証対象のネットワーク
// @param[in] iassoc 対応する2つのネットワークの入力のペアのリスト
// @param[in] oassoc 対応する2つのネットワークの出力のペアのリスト
// @param[out] comp_pairs 比較対象の AigHandle のペアの配列．
void
make_miter(AigMgr& aig_mgr,
	   const BNetwork& src_network1,
	   const BNetwork& src_network2,
	   const vector<pair<ymuint, ymuint> >& iassoc,
	   const vector<pair<ymuint, ymuint> >& oassoc,
	   vector<pair<AigHandle, AigHandle> >& comp_pairs)
{
  // ネットワーク1のノードに対する AIG ノードを納めた配列
  ymuint max1 = src_network1.max_node_id();
  vector<AigHandle> aig_array1(max1);

  // ネットワーク2のノードに対する AIG ノードを納めた配列
  ymuint max2 = src_network2.max_node_id();
  vector<AigHandle> aig_array2(max2);
  
  comp_pairs.clear();
  comp_pairs.resize(oassoc.size());
  
  // 入力用の AIG ノードを作る．
  // これは2つのネットワークに共通
  vector<AigHandle> input_array(iassoc.size());
  ymuint iid = 0;
  for (vector<pair<ymuint, ymuint> >::const_iterator p = iassoc.begin();
       p != iassoc.end(); ++ p) {
    AigHandle anode = aig_mgr.make_input();
    input_array[iid] = anode;
    ++ iid;
    aig_array1[p->first] = anode;
    aig_array2[p->second] = anode;
    if ( debug ) {
      BNode* bnode1 = src_network1.node(p->first);
      BNode* bnode2 = src_network2.node(p->second);
      cout << endl
	   << bnode1->name() << "@network1 = " << anode << endl
	   << bnode2->name() << "@network2 = " << anode << endl;
    }
  }
  
  // 内部のノード用の AIG ノードを作る．
  for (ymuint i = 0; i < 2; ++ i) {
    const BNetwork& network = (i == 0) ? src_network1 : src_network2;
    vector<AigHandle>& aig_array = (i == 0) ? aig_array1 : aig_array2;
    // 内部ノードを作る．
    // まず入力からのトポロジカル順にソートし buff に入れる．
    BNodeVector node_list;
    network.tsort(node_list);
    ymuint nv = network.logic_node_num();
    for (ymuint j = 0; j < nv; ++ j) {
      BNode* bnode = node_list[j];
      ymuint ni = bnode->ni();
      vector<AigHandle> ianodes(ni);
      for (ymuint pos = 0; pos < ni; ++ pos) {
	ianodes[pos] = aig_array[bnode->fanin(pos)->id()];
      }
      if ( debug ) {
	cout << endl;
	cout << "registering " << bnode->name() << "@network" << (i + 1) << endl;
	cout << " expr: " << bnode->func() << endl;
	cout << " fanins:";
	for (ymuint i = 0; i < ni; ++ i) {
	  cout << " \"" << bnode->fanin(i)->name() << "\": "
	       << ianodes[i];
	}
	cout << endl;
      }
      AigHandle anode = aig_mgr.make_logic(bnode->func(), ianodes);
      aig_array[bnode->id()] = anode;
      if ( debug ) {
	cout << bnode->name() << "@network" << (i + 1) << " = "
	     << anode << endl;
      }
    }
  }
  
  // 外部出力用の AIG ノードを作る．
  ymuint oid = 0;
  for (vector<pair<ymuint, ymuint> >::const_iterator p = oassoc.begin();
       p != oassoc.end(); ++ p) {
    BNode* onode1 = src_network1.node(p->first);
    AigHandle ainode1 = aig_array1[onode1->fanin(0)->id()];
    BNode* onode2 = src_network2.node(p->second);
    AigHandle ainode2 = aig_array2[onode2->fanin(0)->id()];
    if ( debug ) {
      cout << "Output#" << oid << ": " << endl
	   << onode1->name() << "@network1" << " = " << ainode1
	   << " -> " << ainode1 << endl
	   << onode2->name() << "@network2" << " = " << ainode2
	   << " -> " << ainode2 << endl;
    }
    comp_pairs[oid] = make_pair(ainode1, ainode2);
    ++ oid;
  }
}

// @brief handle の TFI を作る．
FraigHandle
make_tficone(AigHandle handle,
	     FraigMgr& fraig_mgr,
	     vector<FraigHandle>& map)
{
  if ( handle.is_zero() ) {
    return fraig_mgr.make_zero();
  }
  if ( handle.is_one() ) {
    return fraig_mgr.make_one();
  }
  
  AigNode* node = handle.node();
  if ( map[node->node_id()] == FraigHandle() ) {
    assert_cond(node->is_and(), __FILE__, __LINE__);
    AigHandle ih0 = node->fanin0_handle();
    FraigHandle fh0 = make_tficone(ih0, fraig_mgr, map);
    AigHandle ih1 = node->fanin1_handle();
    FraigHandle fh1 = make_tficone(ih1, fraig_mgr, map);
    FraigHandle fh = fraig_mgr.make_and(fh0, fh1);
    map[node->node_id()] = fh;
  }
  if ( handle.inv() ) {
    return ~map[node->node_id()];
  }
  else {
    return map[node->node_id()];
  }
}

END_NONAMESPACE

// 新しい関数
void
check_ceq(const BNetwork& src_network1,
	  const BNetwork& src_network2,
	  const vector<pair<ymuint32, ymuint32> >& iassoc,
	  const vector<pair<ymuint32, ymuint32> >& oassoc,
	  ymint log_level,
	  ostream* log_out,
	  const string& sat_type,
	  const string& sat_option,
	  ostream* sat_out,
	  ymuint sigsize,
	  vector<Bool3>& stats)
{
  // 2つのネットワークの出力の miter 構造を AigMgr に設定する．
  AigMgr aig_mgr;
  vector<pair<AigHandle, AigHandle> > comp_pairs;
  make_miter(aig_mgr, src_network1, src_network2,
	     iassoc, oassoc,
	     comp_pairs);
    
  FraigMgr fraig_mgr(sigsize, sat_type, sat_option, sat_out);
    
  // オプションを設定する．
  fraig_mgr.set_loglevel(log_level);
  fraig_mgr.set_logstream(log_out);
    
  ymuint naig = aig_mgr.node_num();
  vector<FraigHandle> map(naig);
  ymuint ni = aig_mgr.input_num();
  for (ymuint i = 0; i < ni; ++ i) {
    AigNode* node = aig_mgr.input_node(i);
    map[node->node_id()] = fraig_mgr.make_input();
  }
  ymuint no = src_network1.output_num();
  stats.resize(no);
  for (ymuint i = 0; i < no; ++ i) {
    AigHandle handle1 = comp_pairs[i].first;
    AigHandle handle2 = comp_pairs[i].second;
    if ( handle1 == handle2 ) {
      stats[i] = kB3True;
    }
    else {
      FraigHandle fhandle1 = make_tficone(handle1, fraig_mgr, map);
      FraigHandle fhandle2 = make_tficone(handle2, fraig_mgr, map);
      stats[i] = fraig_mgr.check_equiv(fhandle1, fhandle2);
    }
  }
  if ( log_level > 1 ) {
    fraig_mgr.dump_stats(*log_out);
  }
}

END_NAMESPACE_YM_CEC
