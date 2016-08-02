#ifndef LIBYM_BDD_TESTS_BDDTEST_H
#define LIBYM_BDD_TESTS_BDDTEST_H

/// @file libym_bdd/tests/bddtest.h
/// @brief テスト用の関数定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: bddtest.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#include <ym_bdd/Bdd.h>
#include <ym_bdd/BddVarSet.h>
#include <ym_bdd/BddLitSet.h>


BEGIN_NAMESPACE_YM_BDD

// 論理式を表す文字列からBDDを作る．
// 基本的には BddMgr::make_bdd() を呼ぶだけだが
// エラー時に終了するようになっている．
Bdd
str2bdd(BddMgrRef mgr,
	const char* expr_str,
	int exit_code = 1);

// 文字列から変数集合を作る．
// 文字列は',' で区切られた数字の列
// エラーが起きたら exit_code で終了する．
BddVarSet
str2varset(BddMgrRef mgr,
	   const char* vs_str,
	   int exit_code = 1);

// 文字列からリテラル集合を作る．
// 文字列は',' で区切られた '~' + 数字 の列
// '~' は否定のリテラルを表す．
// エラーが起きたら exit_code で終了する．
BddLitSet
str2litset(BddMgrRef mgr,
	   const char* ls_str,
	   int exit_code = 1);

// bdd が spec の文字列で指定された関数と等しいか調べる．
// spec の仕様は以下の通り
// "d,d,d,d|vvvvvvvvvv"
// d : 変数番号 [0-9]+
// v : 真理値   [01]
// d の個数を n とすると v は 2^n 個なければならない．
bool
check_bddv(const Bdd& bdd,
	   const char* bdd_str,
	   const char* spec);

// bdd が str の論理式と等しいかどうか確かめる．
bool
check_bdde(const Bdd& bdd,
	   const char* bdd_str,
	   const char* str);

// ite_op のテスト
bool
check_ite(BddMgrRef mgr,
	  const char* if_str,
	  const char* then_str,
	  const char* else_str);

// support のテスト
// 0 〜 10 までの変数でコファクタリングして2つのコファクターが異なる
//ものがサポートだと思う．
bool
check_support(BddMgrRef mgr,
	      const char* str);

END_NAMESPACE_YM_BDD

#endif // LIBYM_BDD_TESTS_BDDTEST_H
