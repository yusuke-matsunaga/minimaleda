
/// @file libym_bdn/tests/change_test.cc
/// @brief BdNetwork::change_logic() のテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_bdn/BdNetwork.h"
#include "ym_bdn/BdnNodeHandle.h"


BEGIN_NAMESPACE_YM_BDN

void
change_test()
{
  BdNetwork network;

  network.set_name("change_test");

  BdnNode* a = network.new_input("a");
  BdnNode* b = network.new_input("b");
  BdnNode* c = network.new_input("c");
  BdnNode* d = network.new_output("x", BdnNodeHandle(NULL, false));
  
  // a & b
  BdnNodeHandle h1 = network.new_logic(0x8,
				       BdnNodeHandle(a, false),
				       BdnNodeHandle(b, false));

  // a & c
  BdnNodeHandle h2 = network.new_logic(0x8,
				       BdnNodeHandle(a, false),
				       BdnNodeHandle(c, false));

  // (a & b) | (a & c)
  BdnNodeHandle h3 = network.new_logic(0xe, h1, h2);

  network.change_output(d, h3);
  
  cout << "===Before change_logic===" << endl;
  dump(cout, network);

  // (b | c)
  BdnNodeHandle h4 = network.new_logic(0xe,
				       BdnNodeHandle(b, false),
				       BdnNodeHandle(c, false));

  network.change_logic(h3.node(),
		       0x8,
		       BdnNodeHandle(a, false),
		       h4);

  cout << "===After change_logic===" << endl;
  dump(cout, network);

  // 論理を反転
  network.change_logic(h3.node(),
		       0x7,
		       BdnNodeHandle(a, false),
		       h4);

  cout << "===After change_logic(2)===" << endl;
  dump(cout, network);
  
}

END_NAMESPACE_YM_BDN


int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm;
  using nsYm::nsBdn::change_test;

  change_test();

  return 0;
}
