
/// @file libym_bdn/BdnBlifWriter.cc
/// @brief ブーリアンネットワークの内容を出力する関数の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BdnBlifWriter.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_bdn/BdnBlifWriter.h"
#include "ym_bdn/BdNetwork.h"
#include "ym_bdn/BdnNode.h"
#include "ym_utils/NameMgr.h"


BEGIN_NAMESPACE_YM_BDN


// @brief コンストラクタ
BdnBlifWriter::BdnBlifWriter()
{
}

// @brief デストラクタ
BdnBlifWriter::~BdnBlifWriter()
{
}

// ブーリアンネットワークをblif形式で表示
void
BdnBlifWriter::operator()(ostream& s,
			  const BdNetwork& network)
{
  // 名前を管理するオブジェクト
  NameMgr name_mgr("[", "]");

  ymuint n = network.max_node_id();

  // ID 番号をキーにして名前を入れる配列
  vector<string> name_array(n);

  // 外部入力，外部出力の名前を登録
  ymuint ni = network.input_num();
  for (ymuint i = 0; i < ni; ++ i) {
    string name = network.input_name(i);
    if ( name != string() ) {
      name_mgr.add(name.c_str());
    }
    else {
      name = name_mgr.new_name(true);
    }
    BdnNode* node = network.input(i);
    name_array[node->id()] = name;
  }
  ymuint no = network.output_num();
  for (ymuint i = 0; i < no; ++ i) {
    string name = network.output_name(i);
    if ( name != string() ) {
      name_mgr.add(name.c_str());
    }
    else {
      name = name_mgr.new_name(true);
    }
    BdnNode* node = network.output(i);
    name_array[node->id()] = name;
  }

  // ラッチノードに名前をつける．
  const BdnNodeList& latch_list = network.latch_list();
  for (BdnNodeList::const_iterator p = latch_list.begin();
       p != latch_list.end(); ++ p) {
    const BdnNode* node = *p;
    string name = name_mgr.new_name(true);
    name_array[node->id()] = name;
  }

  // 論理ノードに名前をつける．
  const BdnNodeList& lnode_list = network.lnode_list();
  for (BdnNodeList::const_iterator p = lnode_list.begin();
       p != lnode_list.end(); ++ p) {
    const BdnNode* node = *p;
    if ( node->fanout_num() == 1 &&
	 node->pomark() ) {
      BdnFanoutList::const_iterator p = node->fanout_list().begin();
      const BdnNode* onode = (*p)->to();
      if ( !onode->output_inv() ) {
	// 外部出力と同じ名前にする．
	name_array[node->id()] = name_array[onode->id()];
	continue;
      }
    }
    string name = name_mgr.new_name(true);
    name_array[node->id()] = name;
  }

  // ファイルに出力する．
  s << ".model " << network.name() << endl;
  for (ymuint i = 0; i < ni; ++ i) {
    const BdnNode* node = network.input(i);
    s << ".inputs " << name_array[node->id()] << endl;
  }

  for (ymuint i = 0; i < no; ++ i) {
    const BdnNode* node = network.output(i);
    s << ".outputs " << name_array[node->id()] << endl;
  }

  for (BdnNodeList::const_iterator p = latch_list.begin();
       p != latch_list.end(); ++ p) {
    const BdnNode* node = *p;
    const BdnNode* inode = node->fanin0();
    string iname;
    if ( inode ) {
      iname = name_array[inode->id()];
      if ( node->output_inv() ) {
	string name = name_mgr.new_name(true);
	s << ".names " << iname
	  << " " << name << endl
	  << "0 1" << endl;
	iname = name;
      }
    }
    else {
      iname = name_mgr.new_name(true);
      s << ".names " << iname << endl;
      if ( node->output_inv() ) {
	s << "0" << endl;
      }
      else {
	s << "1" << endl;
      }
    }
    s << ".latch " << iname << " " << name_array[node->id()];
    if ( node->reset_val() != 2 ) {
      s << " " << node->reset_val();
    }
    s << endl;
  }

  for (ymuint i = 0; i < no; ++ i) {
    const BdnNode* node = network.output(i);
    const BdnNode* inode = node->fanin(0);
    if ( inode == 0 ) {
      s << ".names " << name_array[node->id()] << endl;
      if ( node->output_inv() ) {
	s << "1" << endl;
      }
      else {
	s << "0" << endl;
      }
      s << endl;
    }
    else if ( name_array[inode->id()] != name_array[node->id()] ) {
      s << ".names " << name_array[inode->id()]
	<< " " << name_array[node->id()] << endl;
      if ( node->output_inv() ) {
	s << "0 1" << endl;
      }
      else {
	s << "1 1" << endl;
      }
      s << endl;
    }
  }

  for (BdnNodeList::const_iterator p = lnode_list.begin();
       p != lnode_list.end(); ++ p) {
    const BdnNode* node = *p;
    s << ".names " << name_array[node->fanin(0)->id()]
      << " " << name_array[node->fanin(1)->id()]
      << " " << name_array[node->id()] << endl;
    switch ( node->fcode() ) {
    case 0x0: // 0000
    case 0x3: // 0011
    case 0x5: // 0101
    case 0xa: // 1010
    case 0xc: // 1100
    case 0xf: // 1111
      // 定数関数や1入力関数は現れないはず．
      assert_not_reached(__FILE__, __LINE__);
      break;

    case 0x1: // 0001 = ~x0 & ~x1
      s << "00 1" << endl;
      break;

    case 0x2: // 0010 =  x0 & ~x1
      s << "10 1" << endl;
      break;

    case 0x4: // 0100 = ~x0 &  x1
      s << "01 1" << endl;
      break;

    case 0x6: // 0110 =  x0 ^  x1
      s << "10 1" << endl
	<< "01 1" << endl;
      break;

    case 0x7: // 0111 = ~x0 | ~x1
      s << "0- 1" << endl
	<< "-0 1" << endl;
      break;

    case 0x8: // 1000 =  x0 &  x1
      s << "11 1" << endl;
      break;

    case 0x9: // 1001 = ~(x0 ^ x1)
      s << "00 1" << endl
	<< "11 1" << endl;
      break;

    case 0xb: // 1011 =  x0 | ~x1
      s << "1- 1" << endl
	<< "-0 1" << endl;
      break;

    case 0xd: // 1101 = ~x0 |  x1
      s << "0- 1" << endl
	<< "-1 1" << endl;
      break;

    case 0xe: // 1110 =  x0 |  x1
      s << "1- 1" << endl
	<< "-1 1" << endl;
      break;
    }
    s << endl;
  }
  s << ".end" << endl;
}

END_NAMESPACE_YM_BDN
