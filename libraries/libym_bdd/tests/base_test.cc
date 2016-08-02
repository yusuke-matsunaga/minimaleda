
/// @file libym_bdd/tests/base_test.cc
/// @brief BDD パッケージのテストその1 --- 基本関数 ---
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: base_test.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include <ym_bdd/BmcFactory.h>
#include <ym_bdd/BmmFactory.h>

#include "bddtest.h"



BEGIN_NAMESPACE_YM_BDD

// 変数リストの検査
bool
test_var_list(BddMgrRef mgr,
	      ymuint n)
{
  list<tVarId> vlist;
  ymuint nn = mgr.var_list(vlist);
  bool result = true;
  if ( nn != n ) {
    result = false;
  }
  else {
    ymuint i = 0;
    for (list<tVarId>::const_iterator p = vlist.begin();
	 p != vlist.end(); ++ p) {
      if ( *p != i ) {
	result = false;
	break;
      }
      ++ i;
    }
  }
  if ( !result ) {
    cout << "ERROR in var_list" << endl;
    for (list<tVarId>::const_iterator p = vlist.begin();
	 p != vlist.end(); ++ p) {
      cout << " " << *p;
    }
    cout << endl;
  }
  return result;
}

// 定数ノードの検査
bool
test_const(BddMgrRef mgr)
{
  Bdd one = mgr.make_one();
  if ( !one.is_one() ) {
    one.display(cout);
    cout << "ERROR[const-1]: !one.is_one()" << endl;
    return false;
  }
  
  Bdd zero = mgr.make_zero();
  if ( !zero.is_zero() ) {
    zero.display(cout);
    cout << "ERROR[const-2]: !zero.is_zero()" << endl;
    return false;
  }
  
  Bdd err = mgr.make_error();
  if ( !err.is_error() ) {
    err.display(cout);
    cout << "ERROR[const-3]: !err.is_error()" << endl;
    return false;
  }
  
  Bdd ovf = mgr.make_overflow();
  if ( !ovf.is_overflow() ) {
    ovf.display(cout);
    cout << "ERROR[const-4]: !ovf.is_overflow()" << endl;
    return false;
  }

  return true;
}

// リテラル関数の生成
bool
test_literal(BddMgrRef mgr)
{
  Bdd var0 = mgr.make_posiliteral(0);
  if ( !check_bddv(var0, "make_posiliteral(0)", "0|01") )
    return false;

  Bdd var0bar = mgr.make_negaliteral(0);
  if ( !check_bddv(var0bar, "make_negaliteral(0)", "0|10") )
    return false;

  Bdd var2 = mgr.make_literal(2, kPolPosi);
  if ( !check_bddv(var2, "make_literal(2, kPolPosi)", "2|01") )
    return false;

  Bdd var3bar = mgr.make_literal(3, kPolNega);
  if ( !check_bddv(var3bar, "make_literal(3, kPolNega)", "3|10") )
    return false;

  Bdd zero = mgr.make_zero();
  Bdd one = mgr.make_one();
  Bdd var4 = mgr.make_bdd(4, zero, one);
  if ( !check_bddv(var4, "make_bdd(4, zero, one)", "4|01") )
    return false;

  return true;
}

// 単項演算
bool
test_unary(BddMgrRef mgr)
{
  Bdd var0 = mgr.make_posiliteral(0);

  // って否定しかない
  if ( !check_bddv(~var0, "~var0", "0|10") )
    return false;

  // 否定つき代入
  Bdd bdd1 = var0;
  bdd1.negate();
  if ( !check_bddv(bdd1, "var0.negate()", "0|10") )
    return false;

  return true;
}

// 簡単な二項演算の検査
bool
test_binary(BddMgrRef mgr)
{
  Bdd var0 = mgr.make_posiliteral(0);
  Bdd var1 = mgr.make_posiliteral(1);

  if ( !check_bddv(var0 & var1, "var0 & var1", "0,1|0001") )
    return false;

  if ( !check_bddv(var0 & ~var1, "var0 & ~var1", "0,1|0010") )
    return false;

  if ( !check_bddv(var0 & ~var1, "var0 & ~var1", "1,0|0100") )
    return false;
  
  if ( !check_bddv(var0 | var1, "var0 | var1", "0,1|0111") )
    return false;
  
  if ( !check_bddv(var0 ^ var1, "var0 ^ var1", "0,1|0110") )
    return false;
  
  Bdd bdd1 = var0;
  if ( !check_bddv(bdd1 &= var1, "var0 &= var1", "0,1|0001") )
    return false;
  
  Bdd bdd2 = var0;
  if ( !check_bddv(bdd2 |= var1, "var0 |= var1", "0,1|0111") )
    return false;
  
  Bdd bdd3 = var0;
  if ( !check_bddv(bdd3 ^= var1, "var0 ^= var1", "0,1|0110") )
    return false;
  
  return true;
}

// LogExpr や文字列からBDDを作る関数の検査
bool
test_make_bdd(BddMgrRef mgr)
{
  LogExpr expr1 = LogExpr::make_posiliteral(0) & LogExpr::make_negaliteral(1) |
    LogExpr::make_posiliteral(2) & LogExpr::make_posiliteral(1);
  Bdd bdd1 = mgr.expr_to_bdd(expr1);
  if ( !check_bddv(bdd1, "0 & ~1 | 2 & 1", "0, 1, 2|00011101" ) ) {
    return false;
  }

  VarVarMap vvmap;
  vvmap.insert(make_pair(0, 1));
  vvmap.insert(make_pair(1, 2));
  vvmap.insert(make_pair(2, 0));
  Bdd bdd3 = mgr.expr_to_bdd(expr1, vvmap);
  if ( !check_bddv(bdd3, "(0 & ~1 | 2 & 1)(0->1, 1->2, 2->0)",
		  "0, 1, 2|00100111") ) {
    return false;
  }

  VarVarMap vvmap2;
  vvmap2.insert(make_pair(0, 1));
  vvmap2.insert(make_pair(1, 0));
  Bdd bdd5 = mgr.expr_to_bdd(expr1, vvmap2);
  if ( !check_bddv(bdd5, "(0 & ~1 | 2 & 1)(0->1, 1->0)",
		  "0, 1, 2|00110101") ) {
    return false;
  }

  return true;
}

// ite_op のテスト
bool
test_ite(BddMgrRef mgr)
{
  if ( !check_ite(mgr, "0", "1", "2") ) {
    return false;
  }
  if ( !check_ite(mgr, "1 & ~3", "1 | 2 & 3", "2 & 3") ) {
    return false;
  }
  return true;
}

// compose のテスト
bool
test_compose(BddMgrRef mgr)
{
  Bdd bdd1 = str2bdd(mgr, "0 & 1 & 2 & 3");
  Bdd bdd2 = str2bdd(mgr, "~4 | 5");

  Bdd bdd4 = bdd1.compose(0, bdd2);
  if ( !check_bdde(bdd4, "0 & 1 & 2 & 3 compose(0, ~4 | 5)",
		   "1 & 2 & 3 & (~4 | 5)") ) {
    return false;
  }

  return true;
}

// (a|e)smooth のテスト
bool
test_smooth(BddMgrRef mgr)
{
  Bdd bdd1 = str2bdd(mgr, "0 & 1 | ~0 & 2");
  VarList vl;
  vl.push_back(0);
  BddVarSet vs1(mgr, vl);
  Bdd bdd2 = bdd1.esmooth(vs1);
  if ( !check_bdde(bdd2, "0 & 1 | ~0 & 2 esmooth(0)", "1 | 2") ) {
    return false;
  }
  Bdd bdd3 = bdd1.asmooth(vs1);
  if ( !check_bdde(bdd3, "0 & 1 | ~0 & 2 asmooth(0)", "1 & 2") ) {
    return false;
  }
  return true;
}

// && 演算子のテスト
bool
test_intersect(BddMgrRef mgr)
{
  const char* str1 = "0 | 1";
  const char* str2 = "0 & 1";
  
  Bdd bdd1 = str2bdd(mgr, "0 & 1 | 2 & 3");
  Bdd bdd2 = str2bdd(mgr, str1);
  if ( !(bdd1 && bdd2) ) {
    bdd1.display(cout);
    bdd2.display(cout);
    cout << "ERROR[44]: bdd1 && bdd2 failed" << endl;
    return false;
  }

  Bdd bdd3 = str2bdd(mgr, str2);
  if ( !(bdd1 >= bdd3) ) {
    bdd1.display(cout);
    bdd3.display(cout);
    cout << "ERROR[45]: bdd1 >= bdd3 failed" << endl;
    return false;
  }
  return true;
}

//  / 演算子のテスト
bool
test_cofactor(BddMgrRef mgr)
{
  const char* str = "0 & 2";
  
  Bdd bdd1 = str2bdd(mgr, "0 & 1 | 2 & 3");
  Bdd bdd2 = str2bdd(mgr, str);
  Bdd bdd3 = bdd1 / bdd2;
  if ( !check_bdde(bdd3, "0 & 1 | 1 & 2 / 0 & 2", "1 | 3") ) {
    return false;
  }
  return true;
}

// and_exist演算
bool
test_and_exist(BddMgrRef mgr)
{
  const char* str = "0 | 2";
  VarList vl;
  vl.push_back(0);
  
  Bdd bdd1 = str2bdd(mgr, "0 & 1 | ~0 & 2");
  Bdd bdd2 = str2bdd(mgr, str);
  BddVarSet svars1(mgr, vl);
  Bdd bdd3 = and_exist(bdd1, bdd2, svars1);
  if ( !check_bdde(bdd3, "and_exist(bdd1, bdd2, svars1)", "1 | 2") ) {
    return false;
  }

  return true;
}

// isop, prime_cover, minimal_support のテスト
bool
test_isop(BddMgrRef mgr)
{
  const char* str = "0 & 2 | 1 & 3 | ~1 & ~3 | ~0 & ~2 & ~3";
  Bdd bdd1 = str2bdd(mgr, "0 & 1 & 2 | ~0 & 1 & 3 | ~1 & ~2 & ~3");

  Bdd bdd2 = str2bdd(mgr, str);
  LogExpr cover1;
  Bdd bdd3 = isop(bdd1, bdd2, cover1);
  if ( !check_bdde(bdd3, "isop(bdd1, bdd2, cover1)",
		   "0 & 2 | 1 & 3 | ~1 & ~3") ) {
    return false;
  }

  LogExpr ans_cover1 = prime_cover(bdd1, bdd2);
  Bdd bdd6 = mgr.expr_to_bdd(ans_cover1);
  if ( !check_bdde(bdd6, "prime_cover(bdd1, bdd2)",
		   "0 & 2 | 1 & 3 | ~1 & ~3") ) {
    return false;
  }

  Bdd bdd8 = minimal_support(bdd1, bdd2);
  if ( !check_bdde(bdd8, "minimal_support(bdd1, bdd2)", "0 & 1 & 2 & 3") ) {
    return false;
  }
      
  return true;
}

// support のテスト
bool
test_support(BddMgrRef mgr)
{
  if ( !check_support(mgr, "0") ) {
    return false;
  }
  if ( !check_support(mgr, "0 | 1") ) {
    return false;
  }
  if ( !check_support(mgr, "0 & 1 | ~0 & 3") ) {
    return false;
  }
  return true;
}

// test_scc のサブルーティン
bool
check_scc(BddMgrRef mgr,
	  string expr_str,
	  const char* scc_str)
{
  Bdd bdd = str2bdd(mgr, expr_str.c_str());
  Bdd scc = bdd.SCC();
  string tmp = "SCC(" + expr_str + ")";
  return check_bdde(scc, tmp.c_str(), scc_str);
}

// SCC のテスト
bool
test_scc(BddMgrRef mgr)
{
  if ( !check_scc(mgr, "0 & 1 & ~2", "0 & 1 & ~2") ) {
    return false;
  }
  if ( !check_scc(mgr, "0 & 1 | ~0 & ~2", "one") ) {
    return false;
  }
  if ( !check_scc(mgr, "(0 ^ 1) & 2", "2") ) {
    return false;
  }
  if ( !check_scc(mgr, "0 & 1 & ~2 | ~0 & 1 & 2", "1") ) {
    return false;
  }
  if ( !check_scc(mgr, "0 | 1 | 2", "one") ) {
    return false;
  }
  return true;
}

// test_symmetry のサブルーティン
bool
check_sym(BddMgrRef mgr,
	  string expr_str)
{
  Bdd bdd = str2bdd(mgr, expr_str.c_str());
  VarVector sup;
  ymuint ni = bdd.support(sup);
  if ( ni < 2 ) {
    // 無意味
    return true;
  }
  for (ymuint i = 0; i < ni - 1; ++ i) {
    tVarId pos1 = sup[i];
    Bdd bdd_0 = bdd.cofactor(pos1, kPolNega);
    Bdd bdd_1 = bdd.cofactor(pos1, kPolPosi);
    for (ymuint j = i + 1; j < ni; ++ j) {
      tVarId pos2 = sup[j];
      Bdd bdd_01 = bdd_0.cofactor(pos2, kPolPosi);
      Bdd bdd_10 = bdd_1.cofactor(pos2, kPolNega);
      bool expected_result1 = (bdd_01 == bdd_10);
      if ( bdd.check_symmetry(pos1, pos2, kPolPosi) != expected_result1 ) {
	cout << "ERROR[test_symmetry(positive)]" << endl;
	bdd.display(cout);
	cout << "pos1 : " << pos1 << ", pos2 : " << pos2 << endl;
	cout << "expected result = ";
	if ( expected_result1 ) {
	  cout << "true" << endl;
	}
	else {
	  cout << "false" << endl;
	}
	return false;
      }
      Bdd bdd_00 = bdd_0.cofactor(pos2, kPolNega);
      Bdd bdd_11 = bdd_1.cofactor(pos2, kPolPosi);
      bool expected_result2 = (bdd_00 == bdd_11);
      if ( bdd.check_symmetry(pos1, pos2, kPolNega) != expected_result2 ) {
	cout << "ERROR[test_invsymmetry(negative)]" << endl;
	bdd.display(cout);
	cout << "pos1 : " << pos1 << ", pos2 : " << pos2 << endl;
	cout << "expected result = ";
	if ( expected_result2 ) {
	  cout << "true" << endl;
	}
	else {
	  cout << "false" << endl;
	}
	return false;
      }
    }
  }
  return true;
}

// check_symmetry のテスト
bool
test_symmetry(BddMgrRef mgr)
{
  if ( !check_sym(mgr, "0 & 2 | ~0 & 1 & ~2") ) {
    return false;
  }
  if ( !check_sym(mgr, "0 & 1 | ~0 & ~1 & 2") ) {
    return false;
  }
  if ( !check_sym(mgr, "0 & 2 | 0 & ~1 | ~0 & 1 | ~0 & ~1 & ~2") ) {
    return false;
  }
  if ( !check_sym(mgr, "~0 & 1 & ~2 | 0 & ~1 | 0 & 2") ) {
    return false;
  }
  if ( !check_sym(mgr, "~0 & ~1 & 2 | 0 & 1 & ~2") ) {
    return false;
  }
  if ( !check_sym(mgr, "~0 & 1 & 2 | 0 & ~1 & ~2") ) {
    return false;
  }
  if ( !check_sym(mgr, "0 & 2 | ~0 & 1 & ~2") ) {
    return false;
  }
  return true;
}

// minterm_count のテスト
bool
test_minterm_count(BddMgrRef mgr)
{
  Bdd bdd = str2bdd(mgr, "0 & 2 | 1 & 3 | ~1 & ~3");
  mpz_class mc = bdd.minterm_count(4);
  if ( mc != 10 ) {
    cout << "minterm_count(0 & 2 | 1 & 3 | ~1 & ~3) != 10" << endl;
    return false;
  }

  bdd = mgr.make_one();
  for (ymuint i = 0; i < 100; ++ i) {
    Bdd bdd1 = mgr.make_posiliteral(i);
    bdd &= bdd1;
  }
  mc = bdd.minterm_count(100);
  if ( mc != 1 ) {
    cout << "mc != 1" << endl;
    cout << "mc = " << mc.get_str() << endl;
    return false;
  }
  Bdd bdd2 = ~bdd;
  mpz_class mc2 = bdd2.minterm_count(100);
  if ( mc2 != (mpz_class(1) << 100) - mpz_class(1) ) {
    cout << "mc != 2^100 - 1" << endl;
    return false;
  }
  return true;
}

// dump/restore のテスト
bool
test_dump(BddMgrRef mgr)
{
  Bdd bdd = str2bdd(mgr, "0 & 2 | 1 & 3 | ~1 & ~3");
  const char* fn = "/tmp/magus_bdd_base_test";
  {
    ofstream ofs(fn);
    if ( !ofs ) {
      cout << "cannot open output file: " << fn << endl;
      return false;
    }
    bdd.dump(ofs);
  }
  Bdd bdd2;
  {
    ifstream ifs(fn);
    if ( !ifs ) {
      cout << "cannont open input file: " << fn << endl;
      return false;
    }
    bdd2 = mgr.restore(ifs);
  }
  if ( bdd != bdd2 ) {
    cout << "ERROR[test_dump]" << endl;
    bdd.display(cout);
    bdd2.display(cout);
    return false;
  }
  return true;
}

bool
test(BddMgrRef mgr)
{
  for (ymuint i = 0; i < 10; ++ i) {
    mgr.new_var(i);
  }
  return test_var_list(mgr, 10) &&
    test_const(mgr) &&
    test_unary(mgr) &&
    test_binary(mgr) &&
    test_make_bdd(mgr) &&
    test_ite(mgr) &&
    test_compose(mgr) &&
    test_smooth(mgr) &&
    test_intersect(mgr) &&
    test_cofactor(mgr) &&
    test_and_exist(mgr) &&
    test_isop(mgr) &&
    test_support(mgr) &&
    test_scc(mgr) &&
    test_minterm_count(mgr) &&
    test_symmetry(mgr) &&
    test_dump(mgr)
    ;
}

END_NAMESPACE_YM_BDD


int
main(int argc,
     char** argv)
{
  using namespace std;
  using nsYm::AssertError;
  using nsYm::nsBdd::BddMgrRef;
  using nsYm::nsBdd::BmmFactory;
  using nsYm::nsBdd::BmcFactory;

  try {
    BddMgrRef mgr1(BmcFactory("classic mgr"));
    if ( !test(mgr1) ) {
      return 1;
    }
    BddMgrRef mgr2(BmmFactory(false, "fixed order mgr"));
    if ( !test(mgr2) ) {
      return 2;
    }
    BddMgrRef mgr3(BmmFactory(true, "reorder mgr"));
    if ( !test(mgr3) ) {
      return 3;
    }
  }
  catch ( AssertError a ) {
    cerr << a << endl;
    return 255;
  }

  return 0;
}
