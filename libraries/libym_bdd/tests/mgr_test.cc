
/// @file libym_bdd/tests/mgr_test.cc
/// @brief BddMgr 用のテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: mgr_test.cc 700 2007-05-31 00:41:30Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.

// BDD パッケージのテスト
// 複数のBDDを引数に取る関数中で各々のBDDのマネージャが異なっているとき
// の処理を正しく行っているかのチェック


#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include <ym_bdd/BmmFactory.h>

#include "bddtest.h"

using namespace std;
using namespace nsYm::nsBdd;

using nsYm::VarList;
using nsYm::LogExpr;
using nsYm::nsBdd::BmmFactory;

BddMgrRef mgr1(BmmFactory(false, "manager 1"));
BddMgrRef mgr2(BmmFactory(false, "manager 2"));

// error BDD になっているかチェック
bool
check_errbdd(const Bdd& bdd, const char* bdd_str)
{
  if ( !bdd.is_error() ) {
    cout << "ERROR[errbdd]: " << bdd_str << endl;
    bdd.display(cout);
    return false;
  }

  return true;
}

// 二項演算で双方のマネージャが異なっているときのエラーチェック
bool
test_binary()
{
  Bdd var0 = mgr1.make_posiliteral(0);
  Bdd var1 = mgr1.make_posiliteral(1);
  Bdd var2 = mgr2.make_posiliteral(2);

  if ( !check_errbdd(var0 & var2, "var0 & var2") )
    return false;

  if ( !check_errbdd(var0 | var2, "var0 | var2") )
    return false;

  if ( !check_errbdd(var0 ^ var2, "var0 ^ var2") )
    return false;

  Bdd bdd4 = var0;
  if ( !check_errbdd(bdd4 &= var2, "var0 &= var2") )
    return false;

  Bdd bdd5 = var0;
  if ( !check_errbdd(bdd5 |= var2, "var0 &= var2") )
    return false;

  Bdd bdd6 = var0;
  if ( !check_errbdd(bdd6 ^= var2, "var0 &= var2") )
    return false;
  
  return true;
}

// ite_op のオペランドの BddMgr が異なっているときのエラーチェック
bool
test_ite()
{
  Bdd bdd0[2];
  Bdd bdd1[2];
  Bdd bdd2[2];
  bdd0[0] = str2bdd(mgr1, "0");
  bdd0[1] = str2bdd(mgr2, "0");
  bdd1[0] = str2bdd(mgr1, "1");
  bdd1[1] = str2bdd(mgr2, "1");
  bdd2[0] = str2bdd(mgr1, "2");
  bdd2[1] = str2bdd(mgr2, "2");

  for (size_t i = 0; i < 8; ++ i) {
    Bdd f = (i & 4) ? bdd0[1] : bdd0[0];
    Bdd g = (i & 2) ? bdd1[1] : bdd1[0];
    Bdd h = (i & 1) ? bdd2[1] : bdd2[0];

    Bdd bdd = ite_op(f, g, h);
    if ( i == 0 || i == 7 ) {
      if ( !check_bdde(bdd, "ite_op(0, 1, 2)", "0 & 1 | ~0 & 2") ) {
	return false;
      }
    }
    else if ( !bdd.is_error() ) {
      f.display(cout);
      g.display(cout);
      h.display(cout);
      cout << "ERROR[test_ite]: bdd should be an Error-BDD" << endl;
      return false;
    }
  }
  return true;
}

// compose のオペランドのマネージャが異なっているときのエラーチェック
bool
test_compose()
{
  Bdd bdd1 = str2bdd(mgr1, "0 & 1 & 2 & 3");
  Bdd bdd2 = str2bdd(mgr1, "~4 | 5");
  Bdd bdd3 = str2bdd(mgr2, "~4 | 5");

  Bdd bdd5 = bdd1.compose(0, bdd3);
  if ( !bdd5.is_error() ) {
    bdd1.display(cout);
    bdd2.display(cout);
    cout << "ERROR[test_compose(a)]: bdd5 should be an Error-BDD" << endl;
    return false;
  }

  VarBddMap vbmap;
  vbmap.insert(make_pair(0, bdd3));
  Bdd bdd6 = bdd1.compose(vbmap);
  if ( !bdd6.is_error() ) {
    bdd1.display(cout);
    bdd2.display(cout);
    cout << "ERROR[test_compose(b)]: bdd6 should be an Error-BDD" << endl;
    return false;
  }

  return true;
}

// (a|e)smooth のテスト
bool
test_smooth()
{
  Bdd bdd1 = str2bdd(mgr1, "0 & 1 | ~0 & 2");
  VarList vl;
  vl.push_back(0);

  BddVarSet vs2(mgr2, vl);
  Bdd bdd4 = bdd1.esmooth(vs2);
  if ( !bdd4.is_error() ) {
    bdd1.display(cout);
    vs2.function().display(cout);
    cout << "ERROR[test_esmooth]: bdd4 should be an Error-BDD" << endl;
    return false;
  }
  Bdd bdd5 = bdd1.asmooth(vs2);
  if ( !bdd5.is_error() ) {
    bdd1.display(cout);
    vs2.function().display(cout);
    cout << "ERROR[test_asmooth]: bdd5 should be and Error-BDD" << endl;
    return false;
  }

  return true;
}

// && 演算子のテスト
bool
test_intersect()
{
  Bdd bdd1 = str2bdd(mgr1, "0 & 1 | 2 & 3");
  Bdd bdd4 = str2bdd(mgr2, "0 | 1");
  if ( bdd1 && bdd4 ) {
    bdd1.display(cout);
    bdd4.display(cout);
    cout << "ERROR[test_intersect(a)]: bdd1 && bdd4 failed" << endl;
    return false;
  }
  Bdd bdd5 = str2bdd(mgr2, "0 & 1");
  if ( bdd1 >= bdd5 ) {
    bdd1.display(cout);
    bdd5.display(cout);
    cout << "ERROR[test_intersect(b)]: bdd1 >= bdd5 failed" << endl;
    return false;
  }
  return true;
}

// マネージャが異なるときの / 演算子のテスト
bool
test_cofactor()
{
  Bdd bdd1 = str2bdd(mgr1, "0 & 1 | 2 & 3");
  Bdd bdd4 = str2bdd(mgr2, "0 & 2");
  Bdd bdd5 = bdd1 / bdd4;
  if ( !bdd5.is_error() ) {
    bdd1.display(cout);
    bdd4.display(cout);
    cout << "ERROR[48]: bdd1 / bdd4 failed" << endl;
    return false;
  }
  return true;
}

// and_exist演算
bool
test_and_exist()
{
  const char* str = "0 | 2";
  VarList vl;
  vl.push_back(0);
  
  Bdd bdd1 = str2bdd(mgr1, "0 & 1 | ~0 & 2");
  Bdd bdd2 = str2bdd(mgr1, str);
  Bdd bdd4 = str2bdd(mgr2, str);
  BddVarSet svars1(mgr1, vl);
  BddVarSet svars2(mgr2, vl);
  Bdd bdd5 = and_exist(bdd1, bdd4, svars1);
  if ( !bdd5.is_error() ) {
    bdd1.display(cout);
    bdd4.display(cout);
    svars1.function().display(cout);
    cout << "ERROR[test_and_exist(a)]: and_exist(bdd1, bdd4, svars1) failed"
	 << endl;
    return false;
  }

  Bdd bdd6 = and_exist(bdd1, bdd2, svars2);
  if ( !bdd6.is_error() ) {
    bdd1.display(cout);
    bdd2.display(cout);
    svars2.function().display(cout);
    cout << "ERROR[test_and_exist(b)]: and_exist(bdd1, bdd2, svars2) failed"
	 << endl;
    return false;
  }

  return true;
}

// isop, prime_cover, minimal_support のテスト
bool
test_isop()
{
  Bdd bdd1 = str2bdd(mgr1, "0 & 1 & 2 | ~0 & 1 & 3 | ~1 & ~2 & ~3");
  Bdd bdd4 = str2bdd(mgr2, "0 & 2 | 1 & 3 | ~1 & ~3 | ~0 & ~2 & ~3");
  LogExpr cover2;
  Bdd bdd5 = isop(bdd1, bdd4, cover2);
  if ( !bdd5.is_error() ) {
    bdd1.display(cout);
    bdd4.display(cout);
    cout << "ERROR[test_isop(a)]: isop(bdd1, bdd4, cover2) failed" << endl;
    return false;
  }

  LogExpr ans_cover2 = prime_cover(bdd1, bdd4);
  if ( !ans_cover2.is_zero() ) {
    bdd1.display(cout);
    bdd4.display(cout);
    cout << ans_cover2 << endl;
    cout << "ERROR[test_isop(b)]: prime_cover(bdd1, bdd4) failed" << endl;
    return false;
  }

  Bdd bdd9 = minimal_support(bdd1, bdd4);
  if ( !bdd9.is_error() ) {
    bdd1.display(cout);
    bdd4.display(cout);
    cout << "ERROR[test_isop(c)]: minimal_support(bdd1, bdd4) failed" << endl;
    return false;
  }
      
  return true;
}

int
main(int argc, char** argv)
{
  try {
    for (size_t i = 0; i < 10; ++ i) {
      mgr1.new_var(i);
      mgr2.new_var(i);
    }
    if ( !test_binary() ) {
      return 255;
    }
    if ( !test_ite() ) {
      return 255;
    }
    if ( !test_compose() ) {
      return 255;
    }
    if ( !test_smooth() ) {
      return 255;
    }
    if ( !test_intersect() ) {
      return 255;
    }
    if ( !test_cofactor() ) {
      return 255;
    }
    if ( !test_and_exist() ) {
      return 255;
    }
    if ( !test_isop() ) {
      return 255;
    }
    Bdd f;
    {
      BddMgrRef mgr(BmmFactory(true, "temporary manager"));
      mgr.new_var(0);
      mgr.new_var(1);
      mgr.new_var(2);
      f = str2bdd(mgr, "0 * 1 + ~0 * 2");
    }
    BddMgrRef defmgr;
    Bdd f2 = defmgr.make_error();
    if ( f.mgr() != f2.mgr() || f != f2 ) {
      cout << "ERROR[bddmgr]: f != theMgr().make_zero()" << endl;
      return 255;
    }
  }
  catch ( nsYm::AssertError a ) {
    cerr << a << endl;
    return 255;
  }

  return 0;
}
