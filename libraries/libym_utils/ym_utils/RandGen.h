#ifndef YM_UTILS_RANDGEN_H
#define YM_UTILS_RANDGEN_H

/// @file ym_utils/RandGen.h
/// @brief RandGen のヘッダファイル
/// @author Yusuke Matsunaga
///
/// $Id: RandGen.h 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"


BEGIN_NAMESPACE_YM


//////////////////////////////////////////////////////////////////////
/// @class RandGen RandGen.h <ym_utils/RandGen.h>
/// @ingroup YmUtils
/// @brief MT19937 を利用した乱数生成器
/// 
/// @note もとは MT19937 のコードを流用している．
/// 以下，原文
///
/// A C-program for MT19937, with initialization improved 2002/1/26.
/// Coded by Takuji Nishimura and Makoto Matsumoto.
///
/// Before using, initialize the state by using init_genrand(seed)  
/// or init_by_array(init_key, key_length).
///
/// Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
/// All rights reserved.
/// Copyright (C) 2005, Mutsuo Saito
/// All rights reserved.
///
/// Redistribution and use in source and binary forms, with or without
/// modification, are permitted provided that the following conditions
/// are met:
///
///   1. Redistributions of source code must retain the above copyright
///      notice, this list of conditions and the following disclaimer.
///
///   2. Redistributions in binary form must reproduce the above copyright
///      notice, this list of conditions and the following disclaimer in the
///      documentation and/or other materials provided with the distribution.
///
///   3. The names of its contributors may not be used to endorse or promote 
///      products derived from this software without specific prior written 
///      permission.
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
/// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
/// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
/// A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
/// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
/// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
/// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
/// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
/// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
/// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
/// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///
///
/// Any feedback is very welcome.
/// http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
/// email: m-mat @ math.sci.hiroshima-u.ac.jp (remove space)
//////////////////////////////////////////////////////////////////////
class RandGen
{
public:

  /// @brief コンストラクタ
  RandGen();

  /// @brief デストラクタ
  ~RandGen();

  /// @brief シードで初期化する．
  /// @param[in] s シード
  /// @note 以下，原文\n
  /// initializes mt[N] with a seed
  void
  init(ymuint32 s);

  /// @brief キーの配列で初期化する．
  /// @param[in] init_key キーの配列
  /// @param[in] key_length init_key の要素数
  /// @note 以下，原文\n
  /// initialize by an array with array-length
  /// init_key is the array for initializing keys
  /// key_length is its length
  /// slight change for C++, 2004/2/26
  void
  init_by_array(ymuint32 init_key[],
		size_t key_length);
  
  /// @brief 32ビット符号無し整数の乱数を発生させる．
  /// @note 以下，原文\n
  /// generates a random number on [0,0xffffffff]-interval
  ymuint32
  int32();

  /// @brief 31ビット整数の乱数を発生させる．
  /// @note 以下，原文\n
  /// generates a random number on [0,0x7fffffff]-interval
  ymint32
  int31();

  /// @brief unsigned long の整数の乱数を発生させる．
  ymulong
  ulong();
  
  /// @brief [0,1]の区間の実数の乱数を発生させる．
  /// @note 以下，原文\n
  /// These real versions are due to Isaku Wada, 2002/01/09 added
  /// generates a random number on [0,1]-real-interval
  double
  real1();

  /// @brief [0,1)の区間の実数の乱数を発生させる．
  /// @note 以下，原文\n
  /// generates a random number on [0,1)-real-interval
  double
  real2();

  /// @brief (0,1)の区間の実数の乱数を発生させる．
  /// @note 以下，原文\n
  /// generates a random number on (0,1)-real-interval
  double
  real3();

  /// @brief [0,1)の区間の53ビットの実数の乱数を発生させる．
  /// @note 以下，原文\n
  /// generates a random number on [0,1) with 53-bit resolution
  double
  res53();


private:
  //////////////////////////////////////////////////////////////////////
  // 定数
  //////////////////////////////////////////////////////////////////////

  static
  const size_t N = 624;
  static
  const size_t M = 397;
  static
  const ymuint32 MATRIX_A = 0x9908b0dfUL;   // constant vector a
  static
  const ymuint32 UPPER_MASK = 0x80000000UL; // most significant w-r bits
  static
  const ymuint32 LOWER_MASK = 0x7fffffffUL; // least significant r bits


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 状態ベクタの配列
  ymuint32 mV[N];

  // 状態ベクタのインデックス
  size_t mIdx;

};


//////////////////////////////////////////////////////////////////////
/// @class RandPermGen RandGen.h <ym_utils/RandGen.h>
/// @brief ランダムな順列を作るクラス
//////////////////////////////////////////////////////////////////////
class RandPermGen
{
public:

  /// @brief コンストラクタ
  /// @param[in] n 要素数
  RandPermGen(size_t n);

  /// @brief デストラクタ
  ~RandPermGen();


public:
  
  /// @brief 要素数を返す．
  size_t
  num() const;
  
  /// @brief ランダムな順列を生成する．
  /// @param[in] randgen 乱数発生器
  void
  generate(RandGen& randgen);

  /// @brief 順列の要素を取り出す．
  /// @param[in] pos 要素の位置番号 ( 0 <= pos < num() )
  ymuint32
  elem(size_t pos) const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 要素数
  size_t mNum;
  
  // 現在の順列
  ymuint32* mArray;
  
};


//////////////////////////////////////////////////////////////////////
/// @class RandCombiGen RandGen.h <ym_utils/RandGen.h>
/// @brief ランダムな組み合わせを作るクラス
//////////////////////////////////////////////////////////////////////
class RandCombiGen
{
public:

  /// @brief コンストラクタ
  /// @param[in] n 全要素数
  /// @param[in] k 組み合わせの要素数
  RandCombiGen(size_t n,
	       size_t k);

  /// @brief デストラクタ
  ~RandCombiGen();


public:
  
  /// @brief 全要素数を返す．
  size_t
  num() const;
  
  /// @brief 組み合わせの要素数を返す．
  size_t
  combi_num() const;
  
  /// @brief ランダムな組み合わせを生成する．
  /// @param[in] randgen 乱数発生器
  void
  generate(RandGen& randgen);

  /// @brief 組み合わせの要素を取り出す．
  /// @param[in] pos 要素の位置番号 ( 0 <= pos < combi_num() )
  ymuint32
  elem(size_t pos) const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 要素数
  size_t mNum;
  
  // 組み合わせの要素数
  size_t mCombiNum;
  
  // 現在の順列
  ymuint32* mArray;
  
};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 31ビット整数の乱数を発生させる．
inline
ymint32
RandGen::int31()
{
  return static_cast<ymint32>(int32() >> 1);
}

// @brief [0,1]の区間の実数の乱数を発生させる．
inline
double
RandGen::real1()
{
  return int32() * (1.0 / 4294967295.0); 
  /* divided by 2^32-1 */ 
}

// @brief [0,1)の区間の実数の乱数を発生させる．
inline
double
RandGen::real2()
{
  return int32() * (1.0 / 4294967296.0); 
  /* divided by 2^32 */
}

// @brief (0,1)の区間の実数の乱数を発生させる．
inline
double
RandGen::real3()
{
  return ((static_cast<double>(int32())) + 0.5) * (1.0 / 4294967296.0); 
  /* divided by 2^32 */
}

// @brief [0,1)の区間の53ビットの実数の乱数を発生させる．
inline
double
RandGen::res53() 
{ 
  unsigned long a = int32() >> 5;
  unsigned long b = int32() >> 6; 
  return (a * 67108864.0 + b) * (1.0 / 9007199254740992.0); 
} 

END_NAMESPACE_YM

#endif // YM_UTILS_RANDGEN_H
