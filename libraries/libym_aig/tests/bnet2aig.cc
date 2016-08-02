
/// @file libym_aig/tests/bnet2aig.cc
/// @brief bnet から aig を作るテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: bnet2aig.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_bnet/BNetwork.h"
#include "ym_bnet/BNetBlifReader.h"
#include "ym_aig/AigMgr.h"
#include "ym_aig/AigHandle.h"


BEGIN_NAMESPACE_YM

typedef hash_map<BNode*, AigHandle> BNodeMap;

BEGIN_NONAMESPACE

// bnode に対応する AigHandle を連想配列 assoc から探す．
// 見つからなかったらアボートする．
AigHandle
find_node(BNode* bnode,
	  const BNodeMap& assoc)
{
  BNodeMap::const_iterator p = assoc.find(bnode);
  assert_cond(p != assoc.end(), __FILE__, __LINE__);
  return p->second;
}

END_NONAMESPACE

// BNetwork から AIG をつくるコンストラクタ
void
bnet2aig(const BNetwork& network,
	 AigMgr& aig_mgr)
{
  // BNetwork 中のノードと AIG 中のノードの対応を持つ連想配列
  BNodeMap assoc;
  
  // 外部入力を作る．
  for (BNodeList::const_iterator p = network.inputs_begin();
       p != network.inputs_end(); ++p) {
    BNode* bnode = *p;
    AigHandle anode = aig_mgr.make_input();
    assoc.insert(make_pair(bnode, anode));
  }

  // 内部ノードを作る．
  // まず入力からのトポロジカル順にソートし buff に入れる．
  BNodeVector node_list;
  network.tsort(node_list);
  size_t nv = network.logic_node_num();
  for (size_t i = 0; i < nv; ++ i) {
    BNode* bnode = node_list[i];
    size_t ni = bnode->ni();
    vector<AigHandle> ianodes(ni);
    for (size_t pos = 0; pos < ni; ++ pos) {
      ianodes[pos] = find_node(bnode->fanin(pos), assoc);
    }
    AigHandle anode = aig_mgr.make_logic(bnode->func(), ianodes);
    assoc.insert(make_pair(bnode, anode));
  }

  // 外部出力を作る．
  const BNodeList& output_list = network.outputs();
  for (BNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    BNode* obnode = *p;
    BNode* ibnode = obnode->fanin(0);
    AigHandle ianode = find_node(ibnode, assoc);
    assoc.insert(make_pair(obnode, ianode));
  }
}

END_NAMESPACE_YM


int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm;
  
  if ( argc != 2 ) {
    cerr << "USAGE : " << argv[0] << " blif-file" << endl;
    return 2;
  }

  string filename = argv[1];
  
  try {
    MsgHandler* msg_handler = new StreamMsgHandler(&cerr);

    BNetwork network;
    BNetBlifReader reader;
    reader.add_msg_handler(msg_handler);
    if ( !reader.read(filename, network) ) {
      cerr << "Error in reading " << filename << endl;
      return 4;
    }

    AigMgr aig_mgr;
    bnet2aig(network, aig_mgr);

    //dump(outputs, cout);
  }
  catch ( AssertError x) {
    cout << x << endl;
  }

  return 0;
}
