
/// @file libym_bdn/tests/base_test.cc
/// @brief BdNetwork の基本機能のテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: base_test.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_bdn/BdNetwork.h"
#include "ym_bdn/BdnNode.h"
#include "ym_bdn/BdnNodeHandle.h"


BEGIN_NAMESPACE_YM_BDN

bool
base_test()
{
  BdNetwork network;

  // name(), set_name() のテスト
  network.set_name("foo");
  if ( network.name() != "foo" ) {
    cout << "ERROR[set_name]: name() != \"foo\"" << endl;
    return false;
  }
  network.set_name("bar");
  if ( network.name() != "bar" ) {
    cout << "ERROR[set_name]: name() != \"bar\"" << endl;
    return false;
  }

  // new_input() のテスト
  if ( network.input_num() != 0 ) {
    cout << "ERROR[new_input_test]: input_num() != 0" << endl;
    return false;
  }
  BdnNode* input0 = network.new_input("input0");
  if ( network.input_num() != 1 ) {
    cout << "ERROR[new_input_test]: input_num() != 1" << endl;
    return false;
  }
  if ( input0 == NULL ) {
    cout << "ERROR[new_input_test]: input0 == NULL" << endl;
    return false;
  }
  if ( network.input_name(0) != "input0" ) {
    cout << "ERROR[new_input_test]: input0_name == \"input0\"" << endl;
    return false;
  }

  BdnNode* input1 = network.new_input("input1");
  if ( network.input_num() != 2 ) {
    cout << "ERROR[new_input_test]: input_num() != 2" << endl;
    return false;
  }
  if ( input1 == NULL ) {
    cout << "ERROR[new_input_test]: input1 == NULL" << endl;
    return false;
  }
  if ( network.input_name(1) != "input1" ) {
    cout << "ERROR[new_input_test]: input_name == \"input1\"" << endl;
    return false;
  }

  // new_logic() のテスト

  // 0 & 1 を作る．
  BdnNodeHandle and11_h = network.new_logic(0x8,
					    BdnNodeHandle(input0, false),
					    BdnNodeHandle(input1, false));
  if ( network.lnode_num() != 1 ) {
    cout << "ERROR[new_logic_test]: lnode_num() != 1" << endl;
    return false;
  }
  if ( and11_h.node() == NULL ) {
    cout << "ERROR[new_logic_test]: and11_h.node() == NULL" << endl;
    return false;
  }
  if ( and11_h.inv() == true ) {
    cout << "ERROR[new_logic_test]: and11_h.inv() == true" << endl;
    return false;
  }
  BdnNode* and11 = and11_h.node();
  if ( and11->fcode() != 0x8 ) {
    cout << "ERROR[new_logic_test]: and11->fcode() != 1000" << endl;
    return false;
  }
  if ( and11->fanin0() != input0 ) {
    cout << "ERROR[new_logic_test]: and11->fanin0() != input0" << endl;
    return false;
  }
  if ( and11->fanin1() != input1 ) {
    cout << "ERROR[new_logic_test]: and11->fanin1() != input1" << endl;
    return false;
  }

  BdnNodeHandle and11_h2 = network.new_logic(0x4,
					     BdnNodeHandle(input0, true),
					     BdnNodeHandle(input1, false));
  if ( and11_h2 != and11_h ) {
    cout << "ERROR[new_logic_test]: and11_h2 != and11_h" << endl;
    return false;
  }

  BdnNodeHandle and11_h3 = network.new_logic(0x2,
					     BdnNodeHandle(input0, false),
					     BdnNodeHandle(input1, true));
  if ( and11_h3 != and11_h ) {
    cout << "ERROR[new_logic_test]: and11_h3 != and11_h" << endl;
    return false;
  }

  BdnNodeHandle and11_h4 = network.new_logic(0x1,
					     BdnNodeHandle(input0, true),
					     BdnNodeHandle(input1, true));
  if ( and11_h4 != and11_h ) {
    cout << "ERROR[new_logic_test]: and11_h4 != and11_h" << endl;
    return false;
  }

  // ~0 & 1 を作る．
  BdnNodeHandle and01_h = network.new_logic(0x8,
					    BdnNodeHandle(input0, true),
					    BdnNodeHandle(input1, false));
  if ( network.lnode_num() != 2 ) {
    cout << "ERROR[new_logic_test]: lnode_num() != 2" << endl;
    return false;
  }
  if ( and01_h.node() == NULL ) {
    cout << "ERROR[new_logic_test]: and01_h.node() == NULL" << endl;
    return false;
  }
  if ( and01_h.inv() == true ) {
    cout << "ERROR[new_logic_test]: and01_h.inv() == true" << endl;
    return false;
  }
  BdnNode* and01 = and01_h.node();
  if ( and01->fcode() != 0x4 ) {
    cout << "ERROR[new_logic_test]: and01->fcode() != 0100" << endl;
    return false;
  }
  if ( and01->fanin0() != input0 ) {
    cout << "ERROR[new_logic_test]: and01->fanin0() != input0" << endl;
    return false;
  }
  if ( and01->fanin1() != input1 ) {
    cout << "ERROR[new_logic_test]: and01->fanin1() != input1" << endl;
    return false;
  }

  BdnNodeHandle and01_h2 = network.new_logic(0x4,
					     BdnNodeHandle(input0, false),
					     BdnNodeHandle(input1, false));
  if ( and01_h2 != and01_h ) {
    cout << "ERROR[new_logic_test]: and01_h2 != and01_h" << endl;
    return false;
  }

  BdnNodeHandle and01_h3 = network.new_logic(0x1,
					     BdnNodeHandle(input0, false),
					     BdnNodeHandle(input1, true));
  if ( and01_h3 != and01_h ) {
    cout << "ERROR[new_logic_test]: and01_h3 != and01_h" << endl;
    return false;
  }

  BdnNodeHandle and01_h4 = network.new_logic(0x2,
					     BdnNodeHandle(input0, true),
					     BdnNodeHandle(input1, true));
  if ( and01_h4 != and01_h ) {
    cout << "ERROR[new_logic_test]: and01_h4 != and01_h" << endl;
    return false;
  }

  // 定数0との XOR
  BdnNodeHandle tmp1_h = network.new_logic(0x6,
					   BdnNodeHandle::make_zero(),
					   and11_h);
  if ( tmp1_h != and11_h ) {
    cout << "ERROR[new_logic_test]: tmp1_h != and11_h" << endl;
    return false;
  }

  // 定数1との XOR
  BdnNodeHandle tmp2_h = network.new_logic(0x6,
					   and11_h,
					   BdnNodeHandle::make_one());
  if ( tmp2_h != ~and11_h ) {
    cout << "ERROR[new_logic_test]: tmp2_h != ~and11_h" << endl;
    return false;
  }

  // 自分自身との AND
  BdnNodeHandle tmp3_h = network.new_logic(0x8,
					   and11_h, and11_h);
  if ( tmp3_h != and11_h ) {
    cout << "ERROR[new_logic_test]: tmp3_h != and11_h" << endl;
    return false;
  }

  // 自分自身との XOR
  BdnNodeHandle tmp4_h = network.new_logic(0x6,
					   and11_h, and11_h);
  if ( tmp4_h != BdnNodeHandle::make_zero() ) {
    cout << "ERROR[new_logic_test]: tmp4_h != 0" << endl;
    return false;
  }

  // 定数0との AND
  BdnNodeHandle tmp5_h = network.new_logic(0x8,
					   BdnNodeHandle::make_zero(),
					   and11_h);
  if ( tmp5_h != BdnNodeHandle::make_zero() ) {
    cout << "ERROR[new_logic_test]: tmp5_h != 0" << endl;
    return false;
  }

  // 定数1との AND
  BdnNodeHandle tmp6_h = network.new_logic(0x8,
					   and11_h,
					   BdnNodeHandle::make_one());
  if ( tmp6_h != and11_h ) {
    cout << "ERROR[new_logic_test]: tmp6_h != and11_h" << endl;
    return false;
  }

  return true;
}

END_NAMESPACE_YM_BDN

int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm;
  using namespace nsYm::nsBdn;

  if ( !base_test() ) {
    return 255;
  }

  return 0;
}
