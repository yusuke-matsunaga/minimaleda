
/// @file libym_mvn/tests/makenode_test.cc
/// @brief MvNode を生成するテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: makenode.cc 1343 2008-03-25 17:15:35Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/MvMgr.h"
#include "ym_mvn/MvModule.h"
#include "ym_mvn/MvNode.h"


int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm;
  
  try {
    MvMgr mgr;

    MvModule* module = mgr.new_module("test", 2, 1, 1, 0);

    MvNode* node3 = mgr.new_through(module, 1);

    MvNode* node4 = mgr.new_not(module, 2);

    MvNode* node5 = mgr.new_and(module, 3);

    MvNode* node6 = mgr.new_or(module, 4);

    MvNode* node7 = mgr.new_xor(module, 5);

    MvNode* node8 = mgr.new_rand(module, 6);

    MvNode* node9 = mgr.new_ror(module, 7);

    MvNode* node10 = mgr.new_rxor(module, 8);

    MvNode* node11 = mgr.new_equal(module, 9);

    MvNode* node12 = mgr.new_lt(module, 10);

    MvNode* node13 = mgr.new_sll(module, 1, 2, 3);

    MvNode* node14 = mgr.new_srl(module, 6, 5, 4);

    MvNode* node15 = mgr.new_sla(module, 1, 1, 1);

    MvNode* node16 = mgr.new_sra(module, 1, 1, 1);

    MvNode* node17 = mgr.new_add(module, 32, 32, 33);

    MvNode* node18 = mgr.new_sub(module, 64, 64, 64);

    MvNode* node19 = mgr.new_mult(module, 16, 16, 32);

    MvNode* node20 = mgr.new_div(module, 32, 16, 16);

    MvNode* node21 = mgr.new_mod(module, 32, 32, 32);

    MvNode* node22 = mgr.new_pow(module, 32, 32, 32);

    MvNode* node23 = mgr.new_ite(module, 32);

    vector<ymuint> ibw_array(2);
    ibw_array[0] = 32;
    ibw_array[1] = 30;
    MvNode* node24 = mgr.new_concat(module, ibw_array);

    MvModule* module1 = mgr.new_module("sub", 8, 4, 4, 0);

    vector<ymuint32> val(1);
    val[0] = 10;
    MvNode* node41 = mgr.new_const(module, 8, val);
    
    dump(cout, mgr);
  }
  catch ( AssertError x) {
    cout << x << endl;
  }

  return 0;
}
