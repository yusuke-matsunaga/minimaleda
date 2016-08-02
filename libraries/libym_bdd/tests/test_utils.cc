
/// @file libym_bdd/tests/test_utils.cc
/// @brief BDD パッケージのテスト用の便利関数
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: test_utils.cc 700 2007-05-31 00:41:30Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include "bddtest.h"


BEGIN_NAMESPACE_YM_BDD

// 論理式を表す文字列からBDDを作る．
// 基本的には BddMgr::expr_to_bdd() を呼ぶだけだが
// エラー時には exit_code で終了するようになっている．
Bdd
str2bdd(BddMgrRef mgr,
	const char* expr_str,
	int exit_code)
{
  string err_msg;
  Bdd ans = mgr.expr_to_bdd(expr_str, err_msg);
  if ( ans.is_error() ) {
    cerr << "ERROR[str2bdd]: " << err_msg << endl;
    exit(exit_code);
  }
  return ans;
}

// 文字列から変数集合を作る．
// 文字列は',' で区切られた数字の列
// エラーが起きたら exit_code で終了する．
BddVarSet
str2varset(BddMgrRef mgr,
	   const char* vs_str,
	   int exit_code)
{
  BddVarSet vs(mgr);
  string buf;
  for (const char* s = vs_str; ; ++ s) {
    char c = *s;
    if ( c == ',' || c == '\0' ) {
      int d = atoi(buf.c_str());
      vs += BddVarSet(mgr, d);
      if ( c == '\0' ) {
	return vs;
      }
      buf = "";
    }
    else if ( isspace(c) ) {
      continue;
    }
    else if ( isdigit(c) ) {
      buf += c;
    }
    else {
      cout << "ERROR[str2varset]: illegal character in " << vs_str << endl;
      exit(exit_code);
    }
  }
}

// 文字列からリテラル集合を作る．
// 文字列は',' で区切られた '~' + 数字 の列
// '~' は否定のリテラルを表す．
// エラーが起きたら exit_code で終了する．
BddLitSet
str2litset(BddMgrRef mgr,
	   const char* ls_str,
	   int exit_code)
{
  BddLitSet ls(mgr);
  string buf;
  tPol pol = kPolPosi;
  for (const char* s = ls_str; ; ++ s) {
    char c = *s;
    if ( c == '~' ) {
      if ( pol == kPolNega ) {
	cout << "ERROR[str2varset]: syntax error in " << ls_str << endl;
	exit(exit_code);
      }
      pol = kPolNega;
    }
    else if ( c == ',' || c == '\0' ) {
      int d = atoi(buf.c_str());
      ls += BddLitSet(mgr, d, pol);
      if ( c == '\0' ) {
	return ls;
      }
      pol = kPolPosi;
      buf = "";
    }
    else if ( isspace(c) ) {
      continue;
    }
    else if ( isdigit(c) ) {
      buf += c;
    }
    else {
      cout << "ERROR[str2varset]: illegal character in " << ls_str << endl;
      exit(exit_code);
    }
  }
}
	   
// bdd が spec の文字列で指定された関数と等しいか調べる．
// spec の仕様は以下の通り
// "d,d,d,d|vvvvvvvvvv"
// d : 変数番号 [0-9]+
// v : 真理値   [01]
// d の個数を n とすると v は 2^n 個なければならない．
bool
check_bddv(const Bdd& bdd,
	   const char* bdd_str,
	   const char* spec)
{
  VarVector vars;
  string buf;
  const char* s;
  char c;
  for (s = spec; (c = *s) && c != '|'; ++ s) {
    if ( c == ',' ) {
      int d = atoi(buf.c_str());
      vars.push_back(d);
      buf = "";
    }
    else {
      buf += c;
    }
  }
  int d = atoi(buf.c_str());
  vars.push_back(d);

  if ( c != '|' ) {
    cout << "ERROR[check_bddv(a)]: illegal spec \"" << spec << "\"" << endl;
    return false;
  }
  size_t ni = vars.size();
  size_t nv = 1 << ni;
  vector<int> v(nv);
  ++ s;
  for (size_t i = 0; i < nv; ++ i, ++ s) {
    c = *s;
    if ( c == '0' ) {
      v[i] = 0;
    }
    else if ( c == '1' ) {
      v[i] = 1;
    }
    else {
      cout << "ERROR[check_bddv(b)]: illegal spec \"" << spec << "\"" << endl;
      return false;
    }
  }
  if ( *s != '\0' ) {
    cout << "ni = " << ni << ", nv = " << nv << endl;
    cout << "ERROR[check_bddv(c)]: illegal spec \"" << spec << "\"" << endl;
    return false;
  }

  BddMgrRef mgr = bdd.mgr();
  Bdd spec_bdd = mgr.tvec_to_bdd(v, vars);
  if ( spec_bdd.is_error() ) {
    cout << "ERROR[check_bddv(d)]: error in tvec_to_bdd(v, vars) with spec \""
	 << spec << "\"" << endl;
    return false;
  }

  if ( bdd != spec_bdd ) {
    cout << "Error[check_bddv(e)]: " << bdd_str << " != \"" << spec << "\""
	 << endl;
    bdd.display(cout);
    spec_bdd.display(cout);
    return false;
  }
  return true;
}

// bdd が str の論理式と等しいかどうか確かめる．
bool
check_bdde(const Bdd& bdd,
	   const char* bdd_str,
	   const char* str)
{
  BddMgrRef mgr = bdd.mgr();
  Bdd bdd2 = str2bdd(mgr, str);
  if ( bdd != bdd2 ) {
    bdd.display(cout);
    bdd2.display(cout);
    cout << "ERROR[check_bdde]: " << bdd_str << " != " << str << endl;
    return false;
  }
  return true;
}

// ite_op のテスト
bool
check_ite(BddMgrRef mgr,
	  const char* if_str,
	  const char* then_str,
	  const char* else_str)
{
  Bdd if_bdd = str2bdd(mgr, if_str);
  Bdd then_bdd = str2bdd(mgr, then_str);
  Bdd else_bdd = str2bdd(mgr, else_str);

  Bdd ans_ref = if_bdd & then_bdd | ~if_bdd & else_bdd;
  Bdd ans = ite_op(if_bdd, then_bdd, else_bdd);
  if ( ans != ans_ref ) {
    ans_ref.display(cout);
    ans.display(cout);
    cout << "ERROR[check_ite]: ite_op( " << if_str << ", " << then_str
	 << ", " << else_str << ")" << endl;
    return false;
  }
  return true;
}

// support のテスト
// 0 〜 10 までの変数でコファクタリングして2つのコファクターが異なる
//ものがサポートだと思う．
bool
check_support(BddMgrRef mgr,
	      const char* str)
{
  Bdd bdd = str2bdd(mgr, str);
  BddVarSet ref_vs(mgr);
  for (size_t i = 0; i < 10; ++ i) {
    Bdd bdd0 = bdd.cofactor(i, kPolNega);
    Bdd bdd1 = bdd.cofactor(i, kPolPosi);
    if ( bdd0 != bdd1 ) {
      ref_vs += BddVarSet(mgr, i);
    }
  }
  BddVarSet vs = bdd.support();
  if ( vs != ref_vs ) {
    bdd.display(cout);
    cout << "reference: " << ref_vs << endl;
    cout << "bdd.support(): " << vs << endl;
    cout << "ERROR[check_support]" << endl;
    return false;
  }
  return true;
}

END_NAMESPACE_YM_BDD
