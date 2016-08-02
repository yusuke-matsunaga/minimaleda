#ifndef YM_NPN_TVFUNC_H
#define YM_NPN_TVFUNC_H

/// @file ym_npn/TvFunc.h
/// @brief TvFunc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TvFunc.h 1343 2008-03-25 17:15:35Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_npn/npn_nsdef.h"
#include "ym_npn/NpnMap.h"


BEGIN_NAMESPACE_YM_NPN

//////////////////////////////////////////////////////////////////////
/// @class TvFunc TvFunc.h <ym_npn/TvFunc.h>
/// @ingroup NpnGroup
/// @brief 論理関数を表すクラス
//////////////////////////////////////////////////////////////////////
class TvFunc
{
public:

  /// @brief 入力数のみ指定したコンストラクタ
  /// @param[in] ni 入力数
  /// 中身は恒偽関数
  explicit
  TvFunc(ymuint ni = 0);

  /// @brief 入力数と真理値を指定したコンストラクタ
  /// @param[in] 入力数
  /// @param[in] values 真理値のベクタ
  TvFunc(ymuint ni,
	 const vector<int>& values);

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のソースオブジェクト
  TvFunc(const TvFunc& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のソースオブジェクト
  /// @return 自分自身への参照を返す．
  const TvFunc&
  operator=(const TvFunc& src);

  /// @brief デストラクタ
  ~TvFunc();


public:
  //////////////////////////////////////////////////////////////////////
  // オブジェクト生成用のクラスメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief 恒偽関数を作る．
  /// @param[in] ni 入力数
  /// @return 生成したオブジェクトを返す．
  static
  TvFunc
  const_zero(ymuint ni);

  /// @brief 恒真関数を作る．
  /// @param[in] ni 入力数
  /// @return 生成したオブジェクトを返す．
  static
  TvFunc
  const_one(ymuint ni);

  /// @brief 肯定のリテラル関数を作る．
  /// @param[in] ni 入力数
  /// @param[in] pos リテラルの変数番号
  /// @return 生成したオブジェクトを返す．
  static
  TvFunc
  posi_literal(ymuint ni,
	       ymuint pos);

  /// @brief 否定のリテラル関数を作る．
  /// @param[in] ni 入力数
  /// @param[in] pos リテラルの変数番号
  /// @return 生成したオブジェクトを返す．
  static
  TvFunc
  nega_literal(ymuint ni,
	       ymuint pos);


public:

  /// @brief 自分自身を否定する．
  /// @return 自身への参照を返す．
  const TvFunc&
  negate();

  /// @brief src1 との論理積を計算し自分に代入する．
  /// @param[in] src1 演算対象のオブジェクト
  /// @return 自身への参照を返す．
  const TvFunc&
  operator&=(const TvFunc& src1);

  /// @brief src1 との論理和を計算し自分に代入する．
  /// @param[in] src1 演算対象のオブジェクト
  /// @return 自身への参照を返す．
  const TvFunc&
  operator|=(const TvFunc& src1);

  /// @brief src1 との排他的論理和を計算し自分に代入する．
  /// @param[in] src1 演算対象のオブジェクト
  /// @return 自身への参照を返す．
  const TvFunc&
  operator^=(const TvFunc& src1);


public:

  /// @brief 入力数を得る．
  ymuint
  ni() const;

  /// @brief 入力値を2進数と見なしたときの pos 番目の値を得る．
  /// @param[in] pos 変数番号
  /// 答は 0 か 1 だが int 型
  int
  value(ymuint pos) const;

  /// @brief 0 の数を数える．
  ymuint
  count_zero() const;

  /// @brief 1 の数を数える．
  ymuint
  count_one() const;

  /// @brief 0次の Walsh 係数を求める．
  ymint
  walsh_0() const;

  /// @brief 1次の Walsh 係数を求める．
  /// @param[in] pos 変数番号
  ymint
  walsh_1(ymuint pos) const;

  /// @brief 2次の Walsh 係数を求める．
  /// @param[in] pos1, pos2 変数番号
  int
  walsh_2(ymuint pos1,
	  ymuint pos2) const;

  /// @brief 0次と 1次の Walsh 係数を求める．
  /// @param[out] vec 値を格納する配列
  int
  walsh_01(int vec[]) const;

  /// @brief 0次と 1次と 2次の Walsh 係数を求める．
  /// @param[out] vec1, vec2 値を格納する配列
  int
  walsh_012(int vec1[],
	    int vec2[]) const;

  /// @brief 重み別の 0 次の Walsh 係数を求める．
  /// @param[in] w
  /// @param[in] opol
  /// @param[in] ibits
  int
  walsh_w0(ymuint w,
	   tPol opol,
	   ymuint ibits) const;

  /// @brief 重み別の 1 次の Walsh 係数を求める．
  int
  walsh_w1(ymuint i,
	   ymuint w,
	   tPol opol,
	   ymuint ibits) const;

  /// @brief pos 番目の変数がサポートの時 true を返す．
  /// @param[in] pos 変数番号
  bool
  check_sup(tVarId pos) const;

  /// @brief pos1 番目と pos2 番目の変数が対称のとき true を返す．
  /// @param[in] pos1, pos2 変数番号
  /// @param[in] pol 極性
  bool
  check_sym(tVarId pos1,
	    tVarId pos2,
	    tPol pol = kPolPosi) const;

  /// @brief npnmap に従った変換を行う．
  /// @param[in] npnmap 変換マップ
  /// @return 変換した関数を返す．
  TvFunc
  xform(const NpnMap& npnmap) const;

  /// @brief ハッシュ値を返す．
  ymuint
  hash() const;

  /// @brief 内容の出力
  /// @param[in] s 出力先のストリーム
  /// @param[in] mode 出力モード
  /// @note mode は 2 か 16
  void
  dump(ostream& s,
       int mode = 2) const;


public:

  /// @brief ブロック数を得る．
  ymuint
  nblk() const;

  /// @brief 生のデータを得る．
  ymulong
  raw_data(ymuint blk) const;


public:

  friend
  bool
  operator==(const TvFunc& func1,
	     const TvFunc& func2);

  friend
  bool
  operator<(const TvFunc& func1,
	    const TvFunc& func2);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で使われる関数
  //////////////////////////////////////////////////////////////////////

  // 恒真関数を作るコンストラクタ
  // 2番目の引数はダミー
  TvFunc(ymuint ni,
	 int dummy);

  // リテラル関数を作るコンストラクタ
  TvFunc(ymuint ni,
	 ymuint pos,
	 tPol pol);

  // 入力数 ni のベクタを納めるのに必要なブロック数を計算する．
  static
  ymuint
  nblock(ymuint ni);

  // pos 番目の要素のブロック位置を計算する．
  static
  ymuint
  block(ymuint pos);

  // pos 番目の要素のシフト量を計算する．
  static
  ymuint
  shift(ymuint pos);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力数
  ymuint32 mNi;

  // ブロック数
  ymuint32 mNblk;

  // パックされた真理値ベクトル
  ymulong* mVector;

};

/// @relates TvFunc
/// @brief 否定を求める．
TvFunc
operator~(const TvFunc& src);

/// @relates TvFunc
/// @brief 論理積を求める．
TvFunc
operator&(const TvFunc& src1,
	  const TvFunc& src2);

/// @relates TvFunc
/// @brief 論理和を求める．
TvFunc
operator|(const TvFunc& src1,
	  const TvFunc& src2);

/// @relates TvFunc
/// @brief 排他的論理和を求める．
TvFunc
operator^(const TvFunc& src1,
	  const TvFunc& src2);

/// @relates TvFunc
/// @brief 等価比較
bool
operator==(const TvFunc& func1,
	   const TvFunc& func2);

/// @relates TvFunc
/// @brief 非等価比較
bool
operator!=(const TvFunc& src1,
	   const TvFunc& src2);

/// @relates TvFunc
/// @brief 大小比較(小なり)
bool
operator<(const TvFunc& func1,
	  const TvFunc& func2);

/// @relates TvFunc
/// @brief 大小比較(大なり)
bool
operator>(const TvFunc& src1,
	  const TvFunc& src2);

/// @relates TvFunc
/// @brief 大小比較(小なりイコール)
bool
operator<=(const TvFunc& src1,
	   const TvFunc& src2);

/// @relates TvFunc
/// @brief 大小比較(大なりイコール)
bool
operator>=(const TvFunc& src1,
	   const TvFunc& src2);

/// @relates TvFunc
/// @brief ストリームに対する出力
ostream&
operator<<(ostream& s,
	   const TvFunc& func);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// 入力数を得る．
inline
ymuint
TvFunc::ni() const
{
  return mNi;
}

// 入力値を2進数と見なしたときの pos 番目の値を得る．
// 答は 0 か 1 だが int 型
inline
int
TvFunc::value(ymuint pos) const
{
  return (mVector[block(pos)] >> shift(pos)) & 1;
}

// ブロック数を得る．
inline
ymuint
TvFunc::nblk() const
{
  return mNblk;
}

// 生のデータを得る．
inline
ymulong
TvFunc::raw_data(ymuint blk) const
{
  return mVector[blk];
}

// 入力数 ni のベクタを納めるのに必要なブロック数を計算する．
inline
ymuint
TvFunc::nblock(ymuint ni)
{
  const ymuint wsize = sizeof(ymulong) * 8;
  return ((1 << ni) + wsize - 1) / wsize;
}

// pos 番目の要素のブロック位置を計算する．
inline
ymuint
TvFunc::block(ymuint pos)
{
  const ymuint wsize = sizeof(size_t) * 8;
  return pos / wsize;
}

// pos 番目の要素のシフト量を計算する．
inline
ymuint
TvFunc::shift(ymuint pos)
{
  const ymuint wsize = sizeof(size_t) * 8;
  return pos % wsize;
}

// 否定を求める．
inline
TvFunc
operator~(const TvFunc& src)
{
  return TvFunc(src).negate();
}

// 論理積を求める．
inline
TvFunc
operator&(const TvFunc& src1,
	  const TvFunc& src2)
{
  return TvFunc(src1).operator&=(src2);
}

// 論理和を求める．
inline
TvFunc
operator|(const TvFunc& src1,
	  const TvFunc& src2)
{
  return TvFunc(src1).operator|=(src2);
}

// 排他的論理和を求める．
inline
TvFunc
operator^(const TvFunc& src1,
	  const TvFunc& src2)
{
  return TvFunc(src1).operator^=(src2);
}

// 等価比較
inline
bool
operator!=(const TvFunc& src1,
	   const TvFunc& src2)
{
  return !operator==(src1, src2);
}

// 大小比較のバリエーション
inline
bool
operator>(const TvFunc& src1,
	  const TvFunc& src2)
{
  return operator<(src2, src1);
}

inline
bool
operator<=(const TvFunc& src1,
	   const TvFunc& src2)
{
  return !operator<(src2, src1);
}

inline
bool
operator>=(const TvFunc& src1,
	   const TvFunc& src2)
{
  return !operator<(src1, src2);
}

// ストリームに対する出力
inline
ostream&
operator<<(ostream& s,
	   const TvFunc& func)
{
  func.dump(s, 2);
  return s;
}

END_NAMESPACE_YM_NPN

BEGIN_NAMESPACE_HASH
// TvFunc をキーにしたハッシュ関数クラスの定義
template <>
struct hash<nsYm::nsNpn::TvFunc>
{
  ymuint
  operator()(const nsYm::nsNpn::TvFunc& f) const
  {
    return f.hash();
  }
};
END_NAMESPACE_HASH

#endif // YM_NPN_TVFUNC_H
