#ifndef YM_NPN_CVFUNC_H
#define YM_NPN_CVFUNC_H

/// @file ym_npn/CvFunc.h
/// @brief CvFunc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CvFunc.h 1343 2008-03-25 17:15:35Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_npn/npn_nsdef.h"
#include "ym_lexp/Literal.h"


BEGIN_NAMESPACE_YM_NPN

//////////////////////////////////////////////////////////////////////
/// @class CvFunc CvFunc.h <ym_npn/CvFunc.h>
/// @ingroup NpnGroup
/// @brief カバー形式で論理関数を表すクラス
//////////////////////////////////////////////////////////////////////
class CvFunc
{
public:

  /// @brief 入力数のみ指定したコンストラクタ
  /// @param[in] ni 入力数
  /// @note 中身は恒偽関数
  explicit
  CvFunc(size_t ni = 0);

  /// @brief 入力数とキューブ数を指定したコンストラクタ
  /// @param[in] ni 入力数
  /// @param[in] nc キューブ数
  /// @note すべて空(ユニバーサル)のキューブとなる．
  CvFunc(size_t ni,
	 size_t nc);

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  CvFunc(const CvFunc& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 自分自身への参照を返す．
  const CvFunc&
  operator=(const CvFunc& src);
  
  /// @brief デストラクタ
  ~CvFunc();


public:
  //////////////////////////////////////////////////////////////////////
  /// @name CvFunc のインスタンスを生成するクラスメソッド
  /// @{

  /// @brief 恒偽関数を作る．
  /// @param[in] ni 入力数
  /// @return 生成された CvFunc を返す．
  static
  CvFunc
  const_zero(size_t ni);
  
  /// @brief 恒真関数を作る．
  /// @param[in] ni 入力数
  /// @return 生成された CvFunc を返す．
  static
  CvFunc
  const_one(size_t ni);

  /// @brief 肯定のリテラル関数を作る．
  /// @param[in] ni 入力数
  /// @param[in] i リテラル番号
  /// @return 生成された CvFunc を返す．
  static
  CvFunc
  posi_literal(size_t ni,
	       size_t i);

  /// @brief 否定のリテラル関数を作る．
  /// @param[in] ni 入力数
  /// @param[in] i リテラル番号
  /// @return 生成された CvFunc を返す．
  static
  CvFunc
  nega_literal(size_t ni,
	       size_t i);

  /// @}
  //////////////////////////////////////////////////////////////////////


public:

#if 0
  // 自分自身を否定する．
  const CvFunc& negate();

  // src1 との論理積を計算し自分に代入する．
  const CvFunc& operator&=(const CvFunc& src1);

  // src1 との論理和を計算し自分に代入する．
  const CvFunc& operator|=(const CvFunc& src1);

  // src1 との排他的論理和を計算し自分に代入する．
  const CvFunc& operator^=(const CvFunc& src1);
#endif

  
public:

  /// @brief 入力数を得る．
  size_t
  ni() const;

  /// @brief キューブ数を得る．
  size_t
  nc() const;

  /// @brief 0 の数を数える．
  size_t
  count_zero() const;

  /// @brief 1 の数を数える．
  size_t
  count_one() const;

  /// @brief 0次の Walsh 係数を求める．
  int
  walsh_0() const;

  /// @brief 1次の Walsh 係数を求める．
  int
  walsh_1(size_t i) const;

#if 0
  // 2次の Walsh 係数を求める．
  int walsh_2(size_t i,
	      size_t j) const;
#endif

  /// @brief コファクターを返す．
  CvFunc
  cofactor(size_t pos,
	   tPol pol) const;

  /// @brief 0 次と 1 次の Walsh 係数を求める．
  int
  walsh_01(int vec[]) const;

#if 0
  // 重み別の 0 次の Walsh 係数を求める．
  int walsh_w0(size_t w,
	       tPol opol,
	       size_t ibits) const;

  // 重み別の 1 次の Walsh 係数を求める．
  int walsh_w1(size_t i,
	       size_t w,
	       tPol opol,
	       size_t ibits) const;
  
  // i 番目の変数がサポートの時 true を返す．
  bool check_sup(tVarId i) const;
  
  // i 番目と j 番目の変数が対称のとき true を返す．
  bool check_sym(tVarId i,
		 tVarId j,
		 tPol pol = kPolPosi) const;

  // npnmap に従った変換を行う．
  CvFunc xform(const NpnMap& npnmap) const;
#endif
#if 0
  // ハッシュ値を返す．
  size_t hash() const;

  // 等価比較
  friend
  bool operator==(const CvFunc& func1,
		  const CvFunc& func2);
#endif

  /// @brief i 番目のキューブの j 番目の入力のパタンを求める．
  size_t
  pat(size_t i,
      size_t j) const;

  /// @brief i 番目のキューブの j 番目を入力パタンを設定する．
  void
  set_pat(size_t i,
	  size_t j,
	  size_t pat);

public:

  /// @brief キューブ(積項)当たりのワード数を返す．
  size_t
  nbpc() const;

  /// @brief 総ブロック数を返す．
  size_t
  nblk() const;

  
private:

  // リテラルを作るコンストラクタ
  CvFunc(size_t ni,
	 size_t pos,
	 tPol pol);

  // i 番目のキューブの情報ワードのブロック番号を返す．
  size_t
  iblock(size_t i) const;

  // i 番目のキューブの j 番目の入力のブロック番号を返す．
  size_t
  block(size_t i,
	size_t j) const;

  // pos 番目の入力のシフト量を返す．
  size_t
  shift(size_t pos) const;

  // コファクターを計算し，残されたキューブにマークをつける．
  void
  set_cofactor_mark(size_t pos,
		    tPol pol) const;

  // set_cofactor_mark の逆の処理
  void
  clear_cofactor_mark(size_t pos,
		      tPol pol) const;

  size_t
  co_sub() const;

  size_t
  w01_sub(int vec[]) const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力数
  size_t mNi;

  // キューブ数
  size_t mNc;

  // 1行当たりのブロック数
  size_t mNbpc;
  
  // ブロック数
  size_t mNblk;

  // 実際に確保されているサイズ
  size_t mSize;

  // カバー全体を表す配列
  size_t* mChunk;

  // cofactor で使うマークを入れる領域
  size_t* mMark;

};

#if 0
// 否定を求める．
CvFunc operator~(const CvFunc& src);

// 論理積を求める．
CvFunc operator&(const CvFunc& src1,
		 const CvFunc& src2);

// 論理和を求める．
CvFunc operator|(const CvFunc& src1,
		 const CvFunc& src2);

// 排他的論理和を求める．
CvFunc operator^(const CvFunc& src1,
		 const CvFunc& src2);
#endif

// ストリームに対する出力
ostream&
operator<<(ostream& s,
	   const CvFunc& func);

#if 0
// 等価比較
inline
bool
operator!=(const CvFunc& src1,
	   const CvFunc& src2)
{
  return !operator==(src1, src2);
}
#endif


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// 入力数を得る．
inline
size_t
CvFunc::ni() const
{
  return mNi;
}

// キューブ数を得る．
inline
size_t
CvFunc::nc() const
{
  return mNc;
}

// キューブ(積項)当たりのワード数を返す．
inline
size_t
CvFunc::nbpc() const
{
  return mNbpc;
}

// ブロック数を得る．
inline
size_t
CvFunc::nblk() const
{
  return mNblk;
}

// i 番目のキューブの情報ワードのブロック番号を返す．
inline
size_t
CvFunc::iblock(size_t i) const
{
  return i * mNbpc;
}

// i 番目のキューブの j 番目の要素のブロック位置を計算する．
inline
size_t
CvFunc::block(size_t i,
	      size_t j) const
{
  const size_t wsize = sizeof(size_t) * 8;
  return i * mNbpc + (j * 2) / wsize + 1;
}

// pos 番目の要素のシフト量を計算する．
inline
size_t
CvFunc::shift(size_t pos) const
{
  const size_t wsize = sizeof(size_t) * 8;
  return (pos * 2) % wsize;
}

// i 番目のキューブの j 番目の入力のパタンを求める．
inline
size_t
CvFunc::pat(size_t i,
	    size_t j) const
{
  return (mChunk[block(i, j)] >> shift(j)) & 3UL;
}

// i 番目のキューブの j 番目を入力パタンを設定する．
inline
void
CvFunc::set_pat(size_t i,
		size_t j,
		size_t pat)
{
  size_t b = block(i, j);
  size_t s = shift(j);
  mChunk[b] &= ~(3UL << s);
  mChunk[b] |= pat << s;
}


#if 0
// 否定を求める．
inline
CvFunc
operator~(const CvFunc& src)
{
  return CvFunc(src).negate();
}

// 論理積を求める．
inline
CvFunc
operator&(const CvFunc& src1,
	  const CvFunc& src2)
{
  return CvFunc(src1).operator&=(src2);
}

// 論理和を求める．
inline
CvFunc
operator|(const CvFunc& src1,
	  const CvFunc& src2)
{
  return CvFunc(src1).operator|=(src2);
}

// 排他的論理和を求める．
inline
CvFunc
operator^(const CvFunc& src1,
	  const CvFunc& src2)
{
  return CvFunc(src1).operator^=(src2);
}
#endif

END_NAMESPACE_YM_NPN

#if 0
BEGIN_NAMESPACE_HASH
// CvFunc をキーにしたハッシュ関数クラスの定義
template <>
struct hash<nsYm::nsNpn::CvFunc>
{
  size_t operator()(const nsYm::nsNpn::CvFunc& f) const
  {
    return f.hash();
  }
};
END_NAMESPACE_HASH
#endif

#endif // YM_NPN_CVFUNC_H
