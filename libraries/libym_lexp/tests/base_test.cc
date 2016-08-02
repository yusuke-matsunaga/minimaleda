
/// @file libym_lexp/tests/base_test.cc
/// @brief LEXP パッケージのテストその1 --- 基本関数 ---
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: base_test.cc 1920 2008-12-20 15:52:42Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_lexp/LogExpr.h"


using namespace nsYm;

// 定数ノードの検査
bool
test_const()
{
  LogExpr one = LogExpr::make_one();
  if ( !one.is_one() ) {
    cout << one << endl;
    cout << "ERROR[const-1]: !one.is_one()" << endl;
    return false;
  }
  
  LogExpr zero = LogExpr::make_zero();
  if ( !zero.is_zero() ) {
    cout << zero << endl;
    cout << "ERROR[const-2]: !zero.is_zero()" << endl;
    return false;
  }

  return true;
}

// リテラル関数の生成
bool
test_literal()
{
  LogExpr var0 = LogExpr::make_posiliteral(0);
  if ( !check_exprv(var0, "make_posiliteral(0)", "0|01") )
    return false;

  LogExpr var0bar = LogExpr::make_negaliteral(0);
  if ( !check_exprv(var0bar, "make_negaliteral(0)", "0|10") )
    return false;

  LogExpr var2 = LogExpr::make_literal(2, kPolPosi);
  if ( !check_exprv(var2, "make_literal(2, kPolPosi)", "2|01") )
    return false;

  LogExpr var3bar = LogExpr::make_literal(3, kPolNega);
  if ( !check_exprv(var3bar, "make_literal(3, kPolNega)", "3|10") )
    return false;

  LogExpr zero = LogExpr::make_zero();
  LogExpr one = LogExpr::make_one();
  LogExpr var4 = LogExpr::make_expr(4, zero, one);
  if ( !check_exprv(var4, "make_expr(4, zero, one)", "4|01") )
    return false;

  return true;
}

// 単項演算
bool
test_unary()
{
  LogExpr var0 = LogExpr::make_posiliteral(0);

  // って否定しかない
  if ( !check_exprv(~var0, "~var0", "0|10") )
    return false;

  // 否定つき代入
  LogExpr expr1 = var0;
  expr1.negate();
  if ( !check_exprv(expr1, "var0.negate()", "0|10") )
    return false;

  return true;
}

// 簡単な二項演算の検査
bool
test_binary()
{
  LogExpr var0 = LogExpr::make_posiliteral(0);
  LogExpr var1 = LogExpr::make_posiliteral(1);

  if ( !check_exprv(var0 & var1, "var0 & var1", "0,1|0001") )
    return false;

  if ( !check_exprv(var0 & ~var1, "var0 & ~var1", "0,1|0010") )
    return false;

  if ( !check_exprv(var0 & ~var1, "var0 & ~var1", "1,0|0100") )
    return false;
  
  if ( !check_exprv(var0 | var1, "var0 | var1", "0,1|0111") )
    return false;
  
  if ( !check_exprv(var0 ^ var1, "var0 ^ var1", "0,1|0110") )
    return false;
  
  LogExpr expr1 = var0;
  if ( !check_exprv(expr1 &= var1, "var0 &= var1", "0,1|0001") )
    return false;
  
  LogExpr expr2 = var0;
  if ( !check_exprv(expr2 |= var1, "var0 |= var1", "0,1|0111") )
    return false;
  
  LogExpr expr3 = var0;
  if ( !check_exprv(expr3 ^= var1, "var0 ^= var1", "0,1|0110") )
    return false;
  
  return true;
}

// compose のテスト
bool
test_compose()
{
  LogExpr expr1 = str2expr(mgr, "0 & 1 & 2 & 3");
  LogExpr expr2 = str2expr(mgr, "~4 | 5");

  LogExpr expr4 = expr1.compose(0, expr2);
  if ( !check_expre(expr4, "0 & 1 & 2 & 3 compose(0, ~4 | 5)",
		   "1 & 2 & 3 & (~4 | 5)") ) {
    return false;
  }

  return true;
}

bool
test()
{
  return test_const() &&
    test_unary() &&
    test_binary() &&
    test_make_expr() &&
    test_compose()
    ;
}

int
main(int argc,
     char** argv)
{
  try {
    if ( !test() ) {
      return 1;
    }
  }
  catch ( AssertError a ) {
    cerr << a << endl;
    return 255;
  }

  return 0;
}
