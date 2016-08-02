
/// @file libym_npn/tests/TvFuncTest.cc
/// @brief TvFuncTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TvFuncTest.cc 1594 2008-07-18 10:26:12Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_npn/TvFunc.h"

#include "TvFuncTest.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// ベクタの内容を出力する
void
dump_vect(ostream& s,
	  size_t ni,
	  const vector<int>& vect)
{
  size_t ni_pow = 1 << ni;
  for (size_t i = 0; i < ni_pow; ++ i) {
    if ( vect[i] ) {
      s << "1";
    }
    else {
      s << "0";
    }
  }
  s << endl;
}

// vect 中の 1 の数を数える．
size_t
count_onebits(size_t vect)
{
  size_t ans = 0;
  for (size_t i = 0; i < 32; ++ i) {
    if ( vect & 1 ) {
      ++ ans;
    }
    vect >>= 1;
  }
  return ans;
}

END_NONAMESPACE

// @brief コンストラクタ
// @param[in] out 出力用ストリーム
TvFuncTest::TvFuncTest(ostream& out) :
  mOut(out),
  mNerr(0),
  mW1Cache(NULL),
  mW1CacheSize(0),
  mW2Cache(NULL),
  mW2CacheSize(0)
{
}

/// @brief デストラクタ
TvFuncTest::~TvFuncTest()
{
  delete [] mW1Cache;
  delete [] mW2Cache;
}

// 個別の関数とは無関係のテスト
void
TvFuncTest::check0(size_t ni)
{
  size_t ni_pow = 1 << ni;

  // const_zero() のテスト
  TvFunc zero = TvFunc::const_zero(ni);
  for (size_t i = 0; i < ni_pow; ++ i) {
    if ( zero.value(i) != 0 ) {
      mOut << "Error: TvFunc::const_zero(" << ni << ")" << endl
	   << "  func: " << zero << endl;
      ++ mNerr;
      break;
    }
  }

  // const_one() のテスト
  TvFunc one = TvFunc::const_one(ni);
  for (size_t i = 0; i < ni_pow; ++ i) {
    if ( one.value(i) != 1 ) {
      mOut << "Error: TvFunc::const_one(" << ni << ")" << endl
	   << "  func: " << one << endl;
      ++ mNerr;
      break;
    }
  }

  // posi_literal() のテスト
  for (size_t i = 0; i < ni; ++ i) {
    TvFunc pl = TvFunc::posi_literal(ni, i);
    bool error = false;
    for (size_t j = 0; j < ni_pow; ++ j) {
      if ( j & (1 << i) ) {
	if ( pl.value(j) != 1 ) {
	  error = true;
	  break;
	}
      }
      else {
	if ( pl.value(j) != 0 ) {
	  error = true;
	  break;
	}
      }
    }
    if ( error ) {
      mOut << "Error: TvFunc::posi_literal(" << ni << ", "
	   << i << ")" << endl
	   << "  func:  " << pl << endl;
      ++ mNerr;
    }
  }

  // nega_literal() のテスト
  for (size_t i = 0; i < ni; ++ i) {
    TvFunc pl = TvFunc::nega_literal(ni, i);
    bool error = false;
    for (size_t j = 0; j < ni_pow; ++ j) {
      if ( j & (1 << i) ) {
	if ( pl.value(j) != 0 ) {
	  error = true;
	  break;
	}
      }
      else {
	if ( pl.value(j) != 1 ) {
	  error = true;
	  break;
	}
      }
    }
    if ( error ) {
      mOut << "Error: TvFunc::nega_literal(" << ni << ", "
	   << i << ")" << endl
	   << "  func:  " << pl << endl;
      ++ mNerr;
    }
  }
}

// 基本的なテスト
void
TvFuncTest::check_base(size_t ni,
		       const vector<int>& vect)
{
  size_t ni_pow = 1UL << ni;

  TvFunc func(ni, vect);

  // ni() のテスト
  if ( func.ni() != ni ) {
    mOut << "Error: TvFunc::ni()" << endl
	 << "  func: " << func << endl
	 << "  vect[]: ";
    dump_vect(mOut, ni, vect);
    mOut << "  func.ni() = " << func.ni() << endl
	 << "  ni = " << ni << endl;
    ++ mNerr;
  }

  // value() のテスト
  for (size_t i = 0; i < ni_pow; ++ i) {
    if ( func.value(i) != vect[i] ) {
      mOut << "Error: TvFunc::value(size_t pos)" << endl
	   << "  func: " << func << endl
	   << "  vect[]: ";
      dump_vect(mOut, ni, vect);
      mOut << "  func.value(" << i << ") = " << func.value(i) << endl
	   << "  vect[" << i << "] = " << vect[i] << endl;
      ++ mNerr;
    }
  }
}

// 基本的なテストその2
void
TvFuncTest::check_base2(const TvFunc& func)
{
  size_t ni = func.ni();
  size_t ni_pow = 1UL << ni;

  // コピーコンストラクタのテスト
  {
    TvFunc func2(func);
    for (size_t i = 0; i < ni_pow; ++ i) {
      if ( func2.value(i) != func.value(i) ) {
	mOut << "Error: TvFunc::TvFunc(const TvFunc& src)" << endl
	     << "  func:  " << func << endl
	     << "  func2: " << func2 << endl;
	++ mNerr;
      }
    }
  }

  // 代入演算子のテスト
  {
    TvFunc func2;
    func2 = func;
    for (size_t i = 0; i < ni_pow; ++ i) {
      if ( func2.value(i) != func.value(i) ) {
	mOut << "Error: operator=(const TvFunc& src)" << endl
	     << "  func:  " << func << endl
	     << "  func2: " << func2 << endl;
	++ mNerr;
      }
    }
  }

  // negate() のテスト
  {
    TvFunc func2(func);
    func2.negate();
    for (size_t i = 0; i < ni_pow; ++ i) {
      if ( func2.value(i) != (1 - func.value(i)) ) {
	mOut << "Error: TvFunc::negate()" << endl
	     << "  func:  " << func << endl
	     << "  func2: " << func2 << endl;
	++ mNerr;
      }
    }
  }
}

// count_one/count_zero のテスト
void
TvFuncTest::check_count_one(const TvFunc& func)
{
  size_t ni = func.ni();
  size_t ni_pow = 1UL << ni;

  size_t c0_true = 0;
  for (size_t i = 0; i < ni_pow; ++ i) {
    if ( func.value(i) == 0 ) {
      ++ c0_true;
    }
  }
  size_t c1_true = ni_pow - c0_true;
  
  // count_zero() のテスト
  {
    size_t c0 = func.count_zero();
    if ( c0 != c0_true ) {
      mOut << "Error: TvFunc::count_zero()" << endl
	   << "  func: " << func << endl
	   << "  func.count_zero() = " << c0 << endl
	   << "  true c0 = " << c0_true << endl;
      ++ mNerr;
    }
  }

  // count_one() のテスト
  {
    size_t c1 = func.count_one();
    if ( c1 != c1_true ) {
      mOut << "Error: TvFunc::count_one()" << endl
	   << "  func: " << func << endl
	   << "  func.count_one() = " << c1 << endl
	   << "  true c1 = " << c1_true << endl;
      ++ mNerr;
    }
  }
}

// walsh_0 のテスト
void
TvFuncTest::check_walsh_0(const TvFunc& func)
{
  int w0 = func.walsh_0();
  walsh_0(func);
  if ( w0 != mW0Cache ) {
    mOut << "Error: TvFunc::walsh_0()" << endl
	 << "  func: " << func << endl
	 << "  func.walsh_0() = " << w0 << endl
	 << "  true w0 = " << mW0Cache << endl;
    ++ mNerr;
  }
}

// walsh_1 のテスト
void
TvFuncTest::check_walsh_1(const TvFunc& func,
			  size_t pos)
{
  int w1 = func.walsh_1(pos);
  walsh_1(func);
  int w1_true = mW1Cache[pos];
  if ( w1 != w1_true ) {
    mOut << "Error: TvFunc::walsh_1(size_t pos)" << endl
	 << "  func: " << func << endl
	 << "  func.walsh_1(" << pos << ") = " << w1 << endl
	 << "  true w1 = " << w1_true << endl;
    ++ mNerr;
  }
}

// walsh_2 のテスト
void
TvFuncTest::check_walsh_2(const TvFunc& func,
			  size_t pos1,
			  size_t pos2)
{
  if ( pos1 == pos2 ) {
    return;
  }
  int w2 = func.walsh_2(pos1, pos2);
  walsh_2(func);
  int w2_true = mW2Cache[pos1 * func.ni() + pos2];
  if ( w2 != w2_true ) {
    mOut << "Error: TvFunc::walsh_2(size_t i, size_t j)" << endl
	 << "  func: " << func << endl
	 << "  func.walsh_2(" << pos1 << ", " << pos2 << ") = "
	 << w2 << endl
	 << "  true w2 = " << w2_true << endl;
    ++ mNerr;
  }
}

// walsh_01 のテスト
void
TvFuncTest::check_walsh_01(const TvFunc& func)
{
  size_t ni = func.ni();

  int* w1_vec = new int[ni];
  int w0 = func.walsh_01(w1_vec);
  walsh_0(func);
  int w0_true = mW0Cache;
  if ( w0 != w0_true ) {
    mOut << "Error: TvFunc::walsh_01()" << endl
	 << "  func: " << func << endl
	 << "  func.walsh_01()'s w0 = " << w0 << endl
	 << "  true w0 = " << w0_true << endl;
    ++ mNerr;
  }

  walsh_1(func);
  for (size_t i = 0; i < ni; ++ i) {
    int w1_true = mW1Cache[i];
    if ( w1_vec[i] != w1_true ) {
      mOut << "Error: TvFunc::walsh_01()" << endl
	   << "  func: " << func << endl
	   << "  func.walsh_01()'s w1[" << i << "] = " << w1_vec[i] << endl
	   << "  true w1 = " << w1_true << endl;
      ++ mNerr;
    }
  }
  delete [] w1_vec;
}

// walsh_012 のテスト
void
TvFuncTest::check_walsh_012(const TvFunc& func)
{
  size_t ni = func.ni();

  int* w1_vec = new int[ni];
  int* w2_vec = new int[ni * ni];

  int w0 = func.walsh_012(w1_vec, w2_vec);
  walsh_0(func);
  int w0_true = mW0Cache;
  if ( w0 != w0_true ) {
    mOut << "Error: TvFunc::walsh_012()" << endl
	 << "  func: " << func << endl
	 << "  func.walsh_012()'s w0 = " << w0 << endl
	 << "  true w0 = " << w0_true << endl;
    ++ mNerr;
  }

  walsh_1(func);
  for (size_t i = 0; i < ni; ++ i) {
    int w1_true = mW1Cache[i];
    if ( w1_vec[i] != w1_true ) {
      mOut << "Error: TvFunc::walsh_012()" << endl
	   << "  func: " << func << endl
	   << "  func.walsh_012()'s w1[" << i << "] = " << w1_vec[i] << endl
	   << "  true w1 = " << w1_true << endl;
      ++ mNerr;
    }
  }
  for (size_t i = 0; i < ni; ++ i) {
    for (size_t j = 0; j < ni; ++ j) {
      if ( i == j ) {
	continue;
      }
      int w2_true = mW2Cache[i * ni + j];
      if ( w2_vec[i * ni + j] != w2_true ) {
	mOut << "Error: TvFunc::walsh_012()" << endl
	     << "  func: " << func << endl
	     << "  func.walsh_012()'s w2[" << i << ", " << j
	     << "] = " << w2_vec[i * ni + j] << endl
	     << "  true w2 = " << w2_true << endl;
	++ mNerr;
      }
    }
  }
  delete [] w1_vec;
  delete [] w2_vec;
}

// check_sup のテスト
void
TvFuncTest::check_check_sup(const TvFunc& func,
			    size_t pos)
{
  size_t ni = func.ni();
  size_t ni_pow = 1 << ni;

  bool stat = func.check_sup(pos);
  size_t check = 1 << pos;
  bool stat_true = false;
  for (size_t j = 0; j < ni_pow; ++ j) {
    if ( j & check ) {
      if ( func.value(j) != func.value(j ^ check) ) {
	stat_true = true;
	break;
      }
    }
  }
  if ( stat != stat_true ) {
    mOut << "Error: TvFunc::check_sup()" << endl
	 << "  func: " << func << endl
	 << "  func.check_sup(" << pos << ") = " << stat << endl
	 << "  true answer = " << stat_true << endl;
    ++ mNerr;
  }
}

// check_sym のテスト
void
TvFuncTest::check_check_sym(const TvFunc& func,
			    size_t pos1,
			    size_t pos2)
{
  if ( pos1 == pos2 ) return;

  size_t ni = func.ni();
  size_t ni_pow = 1 << ni;

  bool stat_p = func.check_sym(pos1, pos2, kPolPosi);
  bool stat_n = func.check_sym(pos1, pos2, kPolNega);
  size_t check1 = 1 << pos1;
  size_t check2 = 1 << pos2;
  size_t mask_00 = 0;
  size_t mask_01 = check1;
  size_t mask_11 = check1 | check2;
  bool stat_p_true = true;
  bool stat_n_true = true;
  for (size_t k = 0; k < ni_pow; ++ k) {
    if ( (k & mask_11) == mask_01 ) {
      if ( func.value(k) != func.value(k ^ mask_11) ) {
	stat_p_true = false;
	break;
      }
    }
  }
  for (size_t k = 0; k < ni_pow; ++ k) {
    if ( (k & mask_11) == mask_00 ) {
      if ( func.value(k) != func.value(k ^ mask_11) ) {
	stat_n_true = false;
	break;
      }
    }
  }
  if ( stat_p != stat_p_true ) {
    mOut << "Error: TvFunc::check_sym()" << endl
	 << "  func: " << func << endl
	 << "  func.check_sym(" << pos1 << ", " << pos2 << ", kPolPosi) = "
	 << stat_p << endl
	 << "  true answer = " << stat_p_true << endl;
    ++ mNerr;
  }
  if ( stat_n != stat_n_true ) {
    mOut << "Error: TvFunc::check_sym()" << endl
	 << "  func: " << func << endl
	 << "  func.check_sym(" << pos1 << ", " << pos2 << ", kPolNega) = "
	 << stat_n << endl
	 << "  true answer = " << stat_n_true << endl;
    ++ mNerr;
  }
}

// walsh_w0() のテスト
void
TvFuncTest::check_walsh_w0(const TvFunc& func,
			   size_t ibits)
{
  size_t ni = func.ni();
  size_t ni_pow = 1 << ni;

  for (size_t w = 0; w <= ni; ++ w) {
    int w0_p = func.walsh_w0(w, kPolPosi, ibits);
    int w0_n = func.walsh_w0(w, kPolNega, ibits);

    int ans1 = 0;
    for (size_t i = 0; i < ni_pow; ++ i) {
      if ( count_onebits(i) == w ) {
	if ( func.value(i ^ ibits) ) {
	  -- ans1;
	}
	else {
	  ++ ans1;
	}
      }
    }
    if ( w0_p != ans1 ) {
      mOut << "Error: TvFunc::walsh_w0()" << endl
	   << "  func: " << func << endl
	   << "  func.walsh_w0(" << w << ", kPolPosi, ";
      for (size_t i = 0; i < ni; ++ i) {
	if ( ibits & (1 << (ni - 1 - i)) ) {
	  mOut << "1";
	}
	else {
	  mOut << "0";
	}
      }
      mOut << ") = " << w0_p << endl
	   << "  true answer = " << ans1 << endl;
      ++ mNerr;
    }
    if ( w0_n != -ans1 ) {
      mOut << "Error: TvFunc::walsh_w0()" << endl
	   << "  func: " << func << endl
	   << "  func.walsh_w0(" << w << ", kPolNega, ";
      for (size_t i = 0; i < ni; ++ i) {
	if ( ibits & (1 << (ni - 1 - i)) ) {
	  mOut << "1";
	}
	else {
	  mOut << "0";
	}
      }
      mOut << ") = " << w0_n << endl
	   << "  true answer = " << -ans1 << endl;
      ++ mNerr;
    }
  }
}

// walsh_w1() のテスト
void
TvFuncTest::check_walsh_w1(const TvFunc& func,
			   size_t pos,
			   size_t ibits)
{
  size_t ni = func.ni();
  size_t ni_pow = 1 << ni;

  for (size_t w = 0; w <= ni; ++ w) {
    int w1_p = func.walsh_w1(pos, w, kPolPosi, ibits);
    int w1_n = func.walsh_w1(pos, w, kPolNega, ibits);
    int ans1 = 0;
    size_t check = 1 << pos;
    for (size_t b = 0; b < ni_pow; ++ b) {
      if ( count_onebits(b) == w ) {
	if ( b & check ) {
	  if ( func.value(b ^ ibits) ) {
	    ++ ans1;
	  }
	  else {
	    -- ans1;
	  }
	}
	else {
	  if ( func.value(b ^ ibits) ) {
	    -- ans1;
	  }
	  else {
	    ++ ans1;
	  }
	}
      }
    }
    if ( w1_p != ans1 ) {
      mOut << "Error: TvFunc::walsh_w1()" << endl
	   << "  func: " << func << endl
	   << "  func.walsh_w1(" << w << ", " << pos << ", kPolPosi, ";
      for (size_t j = 0; j < ni; ++ j) {
	if ( ibits & (1 << (ni - 1 - j)) ) {
	  mOut << "1";
	}
	else {
	  mOut << "0";
	}
      }
      mOut << ") = " << w1_p << endl
	   << "  true answer = " << ans1 << endl;
      ++ mNerr;
    }
    if ( w1_n != -ans1 ) {
      mOut << "Error: TvFunc::walsh_w1()" << endl
	   << "  func: " << func << endl
	   << "  func.walsh_w1(" << w << ", " << pos << ", kPolNega, ";
      for (size_t j = 0; j < ni; ++ j) {
	if ( ibits & (1 << (ni - 1 - j)) ) {
	  mOut << "1";
	}
	else {
	  mOut << "0";
	}
      }
      mOut << ") = " << w1_n << endl
	   << "  true answer = " << -ans1 << endl;
      ++ mNerr;
    }
  }
}

// walsh_0 の真値を計算する．
// 結果は mW0Cache に格納される．
// func.value() を正しいと仮定している．
void
TvFuncTest::walsh_0(const TvFunc& func)
{
  if ( func != mW0Func ) {
    size_t ni_pow = 1UL << func.ni();
    int w0 = 0;
    for (size_t i = 0; i < ni_pow; ++ i) {
      if ( func.value(i) == 0 ) {
	++ w0;
      }
      else {
	-- w0;
      }
    }
    mW0Cache = w0;
    mW0Func = func;
  }
}

// walsh_1 の真値を計算する．
// 結果は mW1Cache に格納される．
// func.value() を正しいと仮定している．
void
TvFuncTest::walsh_1(const TvFunc& func)
{
  if ( func != mW1Func ) {
    size_t ni = func.ni();
    size_t ni_pow = 1 << ni;

    if ( ni > mW1CacheSize ) {
      delete [] mW1Cache;
      mW1Cache = new int[ni];
      mW1CacheSize = ni;
    }
    
    for (size_t i = 0; i < ni; ++ i) {
      int w1 = 0;
      for (size_t j = 0; j < ni_pow; ++ j) {
	if ( j & (1 << i) ) {
	  if ( func.value(j) ) {
	    ++ w1;
	  }
	  else {
	    -- w1;
	  }
	}
	else {
	  if ( func.value(j) ) {
	    -- w1;
	  }
	  else {
	    ++ w1;
	  }
	}
      }
      mW1Cache[i] = w1;
    }
    mW1Func = func;
  }
}

// walsh_2 の真値を計算する．
// 結果は mW2Cache に格納される．
// func.value() を正しいと仮定している．
void
TvFuncTest::walsh_2(const TvFunc& func)
{
  if ( func != mW2Func ) {
    size_t ni = func.ni();
    size_t ni_pow = 1 << ni;

    size_t new_size = ni * ni;
    if ( new_size > mW2CacheSize ) {
      delete [] mW2Cache;
      mW2Cache = new int[new_size];
      mW2CacheSize = new_size;
    }

    for (size_t i = 0; i < ni; ++ i) {
      for (size_t j = 0; j < ni; ++ j) {
	if ( i == j ) {
	  mW2Cache[i * ni + j] = 0;
	  continue;
	}
	
	int w2 = 0;
	for (size_t k = 0; k < ni_pow; ++ k) {
	  int flag = 0;
	  if ( k & (1 << i) ) {
	    flag ^= 1;
	  }
	  if ( k & (1 << j) ) {
	    flag ^= 1;
	  }
	  if ( flag ) {
	    if ( func.value(k) ) {
	      ++ w2;
	    }
	    else {
	      -- w2;
	    }
	  }
	  else {
	    if ( func.value(k) ) {
	      -- w2;
	    }
	    else {
	      ++ w2;
	    }
	  }
	}
	mW2Cache[i * ni + j] = w2;
	mW2Cache[j * ni + i] = w2;
      }
    }
    mW2Func = func;
  }
}

// @brief エラー回数を得る．
// @return エラー回数を返す．
int
TvFuncTest::nerr() const
{
  return mNerr;
}

END_NAMESPACE_YM
