#ifndef LIBYM_NPN_TESTS_TVFUNCTEST_H
#define LIBYM_NPN_TESTS_TVFUNCTEST_H

/// @file libym_npn/tests/TvFuncTest.h
/// @brief TvFuncTest のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TvFuncTest.h 1594 2008-07-18 10:26:12Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_npn/TvFunc.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class TvFuncTest TvFuncTest.h "TvFuncTest.h"
/// @brief TvFunc のメンバ関数をテストするためのクラス
//////////////////////////////////////////////////////////////////////
class TvFuncTest
{
public:

  /// @brief コンストラクタ
  /// @param[in] out 出力用ストリーム
  TvFuncTest(ostream& out);

  /// @brief デストラクタ
  ~TvFuncTest();

  /// @brief 個別の関数とは無関係のテスト
  /// @param[in] ni 入力数
  ///
  /// TvFunc::const_zero(size_t ni)
  /// TvFunc::const_one(size_t ni)
  /// TvFunc::posi_literal(size_t ni, tVarId pos)
  /// TvFunc::nega_literal(size_t ni, tVarId pos)
  /// のテストを行う．
  void check0(size_t ni);

  /// @brief 基本的なテスト
  /// @param[in] ni 入力数
  /// @param[in] vect テストする論理関数のベクタ
  ///
  /// TvFunc::TvFunc(size_t ni, const vector<int>& vect)
  /// TvFunc::ni()
  /// TvFunc::value(size_t pos)
  /// のテストを行う．
  void check_base(size_t ni,
		  const vector<int>& vect);

  /// @brief 基本的なテストその2
  /// @param[in] func 検査対象の論理関数
  ///
  /// TvFunc::TvFunc(const TvFunc& func)
  /// TvFunc::operator=(const TvFunc& func)
  /// TvFunc::negate()
  /// のテストを行う．
  void check_base2(const TvFunc& func);

  /// @brief count_one/count_zero のテスト
  /// @param[in] func 検査対象の論理関数
  ///
  /// TvFunc::count_one()
  /// TvFunc::count_zero()
  /// のテストを行う．
  void check_count_one(const TvFunc& func);

  /// @brief walsh_0 のテスト
  /// @param[in] func 検査対象の論理関数
  ///
  /// TvFunc::walsh_0()
  /// のテストを行う．
  void check_walsh_0(const TvFunc& func);

  /// @brief walsh_1 のテスト
  /// @param[in] func 検査対象の論理関数
  /// @param[in] pos 入力番号
  ///
  /// TvFunc::walsh_1(size_t pos)
  /// のテストを行う．
  void check_walsh_1(const TvFunc& func,
		     size_t pos);

  /// @brief walsh_2 のテスト
  /// @param[in] func 検査対象の論理関数
  /// @param[in] pos1, pos2 入力番号
  ///
  /// TvFunc::walsh_2(size_t pos1, size_t pos2)
  /// のテストを行う．
  void check_walsh_2(const TvFunc& func,
		     size_t pos1,
		     size_t pos2);

  /// @brief walsh_01 のテスト
  /// @param[in] func 検査対象の論理関数
  ///
  /// TvFunc::walsh_01(int w1_vec[])
  /// のテストを行う．
  void check_walsh_01(const TvFunc& func);

  /// @brief walsh_012 のテスト
  /// @param[in] func 検査対象の論理関数
  ///
  /// TvFunc::walsh_01(int w1_vec[], int w2_vec[])
  /// のテストを行う．
  void check_walsh_012(const TvFunc& func);

  /// @brief check_sup のテスト
  /// @param[in] func 検査対象の論理関数
  /// @param[in] pos 入力番号
  ///
  /// TvFunc::check_sup(size_t pos)
  /// のテストを行う．
  void check_check_sup(const TvFunc& func,
		       size_t pos);

  /// @brief check_sym のテスト
  /// @param[in] func 検査対象の論理関数
  /// @param[in] pos1, pos2 入力番号
  ///
  /// TvFunc::check_sym(size_t pos1, size_t pos2)
  /// のテストを行う．
  void check_check_sym(const TvFunc& func,
		       size_t pos1,
		       size_t pos2);

  /// @brief walsh_w0() のテスト
  /// @param[in] func 検査対象の論理関数
  /// @param[in] ibits 入力の極性反転パタン
  ///
  /// TvFunc::walsh_w0(size_t w, tPol opol, size_t ibits)
  /// のテストを行う．
  void check_walsh_w0(const TvFunc& func,
		      size_t ibits);

  /// @brief walsh_w1() のテスト
  /// @param[in] func 検査対象の論理関数
  /// @param[in] pos 入力番号
  /// @param[in] ibits 入力の極性反転パタン
  ///
  /// TvFunc::walsh_w1(size_t pos, size_t w, tPol opol, size_t ibits)
  /// のテストを行う．
  void check_walsh_w1(const TvFunc& func,
		      size_t pos,
		      size_t ibits);

  /// @brief エラー回数を得る．
  /// @return エラー回数を返す．
  int nerr() const;


private:

  // walsh_0 の真値を計算する．
  // 結果は mW0Cache に格納される．
  // func.value() を正しいと仮定している．
  void walsh_0(const TvFunc& func);

  // walsh_1 の真値を計算する．
  // 結果は mW1Cache に格納される．
  // func.value() を正しいと仮定している．
  void walsh_1(const TvFunc& func);

  // walsh_2 の真値を計算する．
  // 結果は mW2Cache に格納される．
  // func.value() を正しいと仮定している．
  void walsh_2(const TvFunc& func);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // エラーメッセージを出力するストリーム
  ostream& mOut;

  // エラーの数
  int mNerr;

  // w0_cache 対象の論理関数
  TvFunc mW0Func;

  // walsh_0 の真値
  int mW0Cache;
  
  // w1_cache 対象の論理関数
  TvFunc mW1Func;

  // walsh_1 の真値
  int* mW1Cache;

  // mW1Cache のサイズ
  size_t mW1CacheSize;
  
  // w2_cache 対象の論理関数
  TvFunc mW2Func;

  // walsh_2 の真値
  int* mW2Cache;

  // mW2Cache のサイズ
  size_t mW2CacheSize;

};


END_NAMESPACE_YM

#endif // LIBYM_NPN_TESTS_TVFUNCTESTCMD_H
