
/// @file libym_utils/RandGen.cc
/// @brief RandGen の実装ファイル
/// @author Yusuke Matsunaga
///
/// $Id: Binder.cc 33 2006-07-16 14:58:17Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

// もとは MT19937 のコードを流用している．
// 以下はオリジナルの copyright notice
/* 
   A C-program for MT19937, with initialization improved 2002/1/26.
   Coded by Takuji Nishimura and Makoto Matsumoto.

   Before using, initialize the state by using init_genrand(seed)  
   or init_by_array(init_key, key_length).

   Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.                          
   Copyright (C) 2005, Mutsuo Saito,
   All rights reserved.                          

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

     3. The names of its contributors may not be used to endorse or promote 
        products derived from this software without specific prior written 
        permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


   Any feedback is very welcome.
   http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
   email: m-mat @ math.sci.hiroshima-u.ac.jp (remove space)
*/


#include "ym_utils/RandGen.h"


BEGIN_NAMESPACE_YM

// @brief コンストラクタ
RandGen::RandGen() :
  mIdx(N + 1)
{
  init(19650218UL);
}

// @brief デストラクタ
RandGen::~RandGen()
{
}

/* initializes mt[N] with a seed */
void
RandGen::init(ymuint32 s)
{
  mV[0]= s & 0xffffffffUL;
  for (mIdx = 1; mIdx < N; ++ mIdx) {
    mV[mIdx] = 
      (1812433253UL * (mV[mIdx - 1] ^ (mV[mIdx - 1] >> 30)) + mIdx); 
    /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
    /* In the previous versions, MSBs of the seed affect   */
    /* only MSBs of the array mt[].                        */
    /* 2002/01/09 modified by Makoto Matsumoto             */
    mV[mIdx] &= 0xffffffffUL;
    /* for >32 bit machines */
  }
}

/* initialize by an array with array-length */
/* init_key is the array for initializing keys */
/* key_length is its length */
/* slight change for C++, 2004/2/26 */
void
RandGen::init_by_array(ymuint32 init_key[],
		       size_t key_length)
{
  init(19650218UL);

  size_t i = 1;
  size_t j = 0;
  size_t k = (N > key_length ? N : key_length);
  for ( ; k; -- k) {
    mV[i] = (mV[i] ^ ((mV[i - 1] ^ (mV[i - 1] >> 30)) * 1664525UL))
      + init_key[j] + j; /* non linear */
    mV[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
    ++ i;
    ++ j;
    if ( i >= N ) {
      mV[0] = mV[N - 1];
      i = 1;
    }
    if ( j >= key_length ) {
      j = 0;
    }
  }
  for ( k = N - 1; k; -- k) {
    mV[i] = (mV[i] ^ ((mV[i - 1] ^ (mV[i - 1] >> 30)) * 1566083941UL))
      - i; /* non linear */
    mV[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
    ++ i;
    if ( i>= N ) {
      mV[0] = mV[N - 1];
      i = 1;
    }
  }
  
  mV[0] = 0x80000000UL; /* MSB is 1; assuring non-zero initial array */ 
}

/* generates a random number on [0,0xffffffff]-interval */
ymuint32
RandGen::int32()
{
  static ymuint32 mag01[2] = { 0x0UL, MATRIX_A };
  /* mag01[x] = x * MATRIX_A  for x=0,1 */

  if ( mIdx >= N ) { /* generate N words at one time */
    
    if ( mIdx == N + 1 )   /* if init_genrand() has not been called, */
      init(5489UL); /* a default initial seed is used */

    size_t kk;
    for ( kk = 0; kk < N - M; ++ kk) {
      unsigned long y = (mV[kk] & UPPER_MASK) | (mV[kk + 1] & LOWER_MASK);
      mV[kk] = mV[kk + M] ^ (y >> 1) ^ mag01[y & 0x1UL];
    }
    for ( ; kk < N - 1; ++ kk) {
      unsigned long y = (mV[kk] & UPPER_MASK) | (mV[kk + 1] & LOWER_MASK);
      mV[kk] = mV[kk + (M - N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
    }
    unsigned long y = (mV[N-1] & UPPER_MASK) | (mV[0] & LOWER_MASK);
    mV[N - 1] = mV[M - 1] ^ (y >> 1) ^ mag01[y & 0x1UL];
    
    mIdx = 0;
  }
  
  ymuint32 y = mV[mIdx ++];
  
  /* Tempering */
  y ^= (y >> 11);
  y ^= (y << 7) & 0x9d2c5680UL;
  y ^= (y << 15) & 0xefc60000UL;
  y ^= (y >> 18);
  
  return y;
}

// @brief unsigned long の整数の乱数を発生させる．
ymulong
RandGen::ulong()
{
#if SIZEOF_LONG == 4
  return int32();
#elif SIZEOF_LONG == 8
  ymulong tmp1 = int32();
  ymulong tmp2 = int32();
  return (tmp1 << 32) | tmp2;
#else
  ymulong ans = 0UL;
  size_t n = SIZEOF_LONG / 4;
  for (size_t i = 0; i < n; ++ i) {
    ans <<= 32;
    ans |= int32();
  }
  return ans;
#endif
}


//////////////////////////////////////////////////////////////////////
// クラス RandPermGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] n 要素数
RandPermGen::RandPermGen(size_t n) :
  mNum(n),
  mArray(new ymuint32[n])
{
}

// @brief デストラクタ
RandPermGen::~RandPermGen()
{
  delete [] mArray;
}
  
// @brief 要素数を返す．
size_t
RandPermGen::num() const
{
  return mNum;
}
  
// @brief ランダムな順列を生成する．
void
RandPermGen::generate(RandGen& randgen)
{
  for (size_t i = 0; i < mNum; ++ i) {
    mArray[i] = i;
  }
  for (size_t i = 0; i < mNum; ++ i) {
    size_t n = mNum - i;
    size_t p = (randgen.int32() % n) + i;
    if ( p != i ) {
      size_t v = mArray[p];
      mArray[p] = mArray[i];
      mArray[i] = v;
    }
  }
}

// @brief 順列の要素を取り出す．
// @param[in] pos 要素の位置番号 ( 0 <= pos < num() )
ymuint32
RandPermGen::elem(size_t pos) const
{
  return mArray[pos];
}


//////////////////////////////////////////////////////////////////////
// クラス RandCombiGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] n 全要素数
// @param[in] k 組み合わせの要素数
RandCombiGen::RandCombiGen(size_t n,
			   size_t k) :
  mNum(n),
  mCombiNum(k),
  mArray(new ymuint32[n])
{
}

// @brief デストラクタ
RandCombiGen::~RandCombiGen()
{
  delete [] mArray;
}
  
// @brief 全要素数を返す．
size_t
RandCombiGen::num() const
{
  return mNum;
}
  
// @brief 組み合わせの要素数を返す．
size_t
RandCombiGen::combi_num() const
{
  return mCombiNum;
}
  
// @brief ランダムな組み合わせを生成する．
// @param[in] randgen 乱数発生器
void
RandCombiGen::generate(RandGen& randgen)
{
  for (size_t i = 0; i < mNum; ++ i) {
    mArray[i] = i;
  }
  for (size_t i = 0; i < mCombiNum; ++ i) {
    size_t n = mNum - i;
    size_t p = (randgen.int32() % n) + i;
    if ( p != i ) {
      size_t v = mArray[p];
      mArray[p] = mArray[i];
      mArray[i] = v;
    }
  }
}

// @brief 組み合わせの要素を取り出す．
// @param[in] pos 要素の位置番号 ( 0 <= pos < combi_num() )
ymuint32
RandCombiGen::elem(size_t pos) const
{
  return mArray[pos];
}

END_NAMESPACE_YM
