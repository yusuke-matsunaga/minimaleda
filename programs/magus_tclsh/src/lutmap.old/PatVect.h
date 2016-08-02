#ifndef MAGUS_LUTMAP_PATVECT_H
#define MAGUS_LUTMAP_PATVECT_H

/// @file magus/lutmap/PatVect.h
/// @brief PatVect のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: PatVect.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.

#include "lutmap.h"

#include <ym_utils/RandGen.h>
#include <ym_lexp/LogExpr.h>


BEGIN_NAMESPACE_MAGUS_LUTMAP

//////////////////////////////////////////////////////////////////////
/// @class PatVect PatVect.h "PatVect.h"
/// @brief パタンベクタを表すクラス
//////////////////////////////////////////////////////////////////////
class PatVect
{
public:

  /// @brief コンストラクタ
  /// @param[in] size サイズ(単位はワード)
  PatVect(ymuint size = 0);
  
  /// @brief コピーコンストラクタ
  /// @param[in] src ソース
  PatVect(const PatVect& src);

  /// @brief 代入演算子
  /// @param[in] src ソース
  const PatVect&
  operator=(const PatVect& src);
  
  /// @brief デストラクタ
  virtual
  ~PatVect();


public:
  
  /// @brief サイズを得る．
  ymuint
  size() const;
  
  /// @brief サイズを変更する．
  /// @note 現在の内容は失われる．
  void
  resize(ymuint size);
  
  /// @brief pos 番目のブロックを得る．
  /// @param[in] pos ブロック位置
  ymuint32
  block(ymuint pos) const;
  
  /// @brief pos 番目のブロックを得る．
  /// @param[in] pos ブロック位置
  ymuint32&
  block(ymuint pos);
  
  /// @brief 内容をクリアする(0に初期化する)．
  void
  clear();
  
  /// @brief 内容をセットする(1に初期化する)．
  void
  set();

  /// @brief ランダムパタンを設定する．
  /// @param[in] randgen 乱数発生器
  void
  set_random(RandGen& randgen);
  
  /// @brief 否定を計算して自分自身に代入する．
  const PatVect&
  negate();
  
  /// @brief 等価比較演算子
  bool
  operator==(const PatVect& src) const;
  
  /// @brief 論理積を計算して自分自身に代入する．
  const PatVect&
  operator&=(const PatVect& src);

  /// @brief 論理和を計算して自分自身に代入する．
  const PatVect&
  operator|=(const PatVect& src);

  /// @brief 排他的論理和を計算して自分自身に代入する．
  const PatVect&
  operator^=(const PatVect& src);
  
  
private:
  //////////////////////////////////////////////////////////////////////
  // 内部でのみ用いられる関数
  //////////////////////////////////////////////////////////////////////
  
  // コピーする．
  void
  copy(const PatVect& src);

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // サイズ
  ymuint32 mSize;

  // ベクタ本体
  ymuint32* mVect;
  
};


//////////////////////////////////////////////////////////////////////
// PatVect に関係する非メンバ関数
//////////////////////////////////////////////////////////////////////

/// @brief 非等価比較演算子
bool
operator!=(const PatVect& opr1,
	   const PatVect& opr2);

/// @brief 否定演算子
/// @param[in] opr1 引数
/// @return opr1 の否定を返す．
PatVect
operator~(const PatVect& opr1);

/// @brief AND 演算子
/// @param[in] opr1, opr2
/// @return opr1 と opr2 の論理積を返す．
PatVect
operator&(const PatVect& opr1,
	  const PatVect& opr2);

/// @brief OR 演算子
/// @param[in] opr1, opr2
/// @return opr1 と opr2 の論理和を返す．
PatVect
operator|(const PatVect& opr1,
	  const PatVect& opr2);

/// @brief XOR 演算子
/// @param[in] opr1, opr2
/// @return opr1 と opr2 の排他的論理和を返す．
PatVect
operator^(const PatVect& opr1,
	  const PatVect& opr2);

/// @brief ドントケアを考慮して両立するとき true を返す．
/// @param[in] opr1, opr2 2つのパタン
/// @param[in] dc ドントケアを表すパタン
bool
eq_with_dc(const PatVect& opr1,
	   const PatVect& opr2,
	   const PatVect& dc);

/// @brief ドントケアを考慮して両立するとき true を返す．
/// @param[in] opr1, opr2 2つのパタン
/// @param[in] dc1, dc2 ドントケアを表すパタン
bool
eq_with_dc(const PatVect& opr1,
	   const PatVect& opr2,
	   const PatVect& dc1,
	   const PatVect& dc2);

/// @brief 論理式に応じた計算を行う．
/// @param[in] expr 論理式
/// @param[in] ipat 論理式の入力変数のパタン
/// @param[out] opat 論理式の出力のパタン
void
calc_pat(const LogExpr& expr,
	 const vector<PatVect*>& ipat,
	 PatVect& opat);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief サイズを得る．
inline
ymuint
PatVect::size() const
{
  return mSize;
}

// @brief pos 番目のブロックを得る．
// @param[in] pos ブロック位置
inline
ymuint32
PatVect::block(ymuint pos) const
{
  return mVect[pos];
}
  
// @brief pos 番目のブロックを得る．
// @param[in] pos ブロック位置
inline
ymuint32&
PatVect::block(ymuint pos)
{
  return mVect[pos];
}

// @brief 非等価比較演算子
inline
bool
operator!=(const PatVect& opr1,
	   const PatVect& opr2)
{
  return !(opr1 == opr2);
}

// @brief 否定演算子
// @param[in] opr1 引数
// @return opr1 の否定を返す．
inline
PatVect
operator~(const PatVect& opr1)
{
  return PatVect(opr1).negate();
}

// @brief AND 演算子
// @param[in] opr1, opr2
// @return opr1 と opr2 の論理積を返す．
inline
PatVect
operator&(const PatVect& opr1,
	  const PatVect& opr2)
{
  return PatVect(opr1).operator&=(opr2);
}

// @brief OR 演算子
// @param[in] opr1, opr2
// @return opr1 と opr2 の論理和を返す．
inline
PatVect
operator|(const PatVect& opr1,
	  const PatVect& opr2)
{
  return PatVect(opr1).operator|=(opr2);
}

// @brief XOR 演算子
// @param[in] opr1, opr2
// @return opr1 と opr2 の排他的論理和を返す．
inline
PatVect
operator^(const PatVect& opr1,
	  const PatVect& opr2)
{
  return PatVect(opr1).operator^=(opr2);
}

END_NAMESPACE_MAGUS_LUTMAP

#endif // MAGUS_LUTMAP_PATVECT_H
