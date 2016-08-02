#ifndef LIBYM_SEAL_SMATRIX_H
#define LIBYM_SEAL_SMATRIX_H

/// @file libym_seal/SMatrix.h
/// @brief SMatrix のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Matrix.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_seal/seal_nsdef.h"
#include "ym_utils/Alloc.h"


BEGIN_NAMESPACE_YM_SEAL


//////////////////////////////////////////////////////////////////////
/// @class SmCell SMatrix.h "SMatrix.h"
/// @brief SMatrix の要素を表すクラス
//////////////////////////////////////////////////////////////////////
class SmCell
{
  friend class SMatrix;
public:

  /// @brief 列番号を得る．
  ymuint
  col_pos() const;

  /// @brief 値を得る．
  double
  value() const;

  /// @brief 値を設定する．
  void
  set_value(double val);

  /// @brief 右に隣接したセルを得る．
  SmCell*
  right() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 列番号
  ymuint32 mColPos;

  // 値
  double mVal;

  // 右方向のリンク
  SmCell* mRightLink;
  
};


//////////////////////////////////////////////////////////////////////
/// @class SMatrix SMatrix.h "SMatrix.h"
/// @brief 行列を表すクラス
//////////////////////////////////////////////////////////////////////
class SMatrix
{
public:

  /// @brief コンストラクタ
  /// @param[in] size 変数の数
  /// @note 内容は空
  SMatrix(ymuint size);

#if 0
  /// @brief コピーコンストラクタ
  SMatrix(const SMatrix& src);

  /// @brief 代入演算子
  const SMatrix&
  operator=(const SMatrix& src);
#endif

  /// @brief デストラクタ
  ~SMatrix();

  
public:

  /// @brief 変数の数を返す．
  ymuint32
  size() const;

  /// @brief 行の先頭を得る．
  /// @param[in] pos 行番号 ( 0 <= pos < size() )
  SmCell*
  row_top(ymuint pos) const;

  /// @brief 行の末尾を得る．
  /// @param[in] pos 行番号 ( 0 <= pos < size() )
  /// @note このセルはダミー
  SmCell*
  row_end(ymuint pos) const;

  /// @brief 要素を取り出す．
  /// @param[in] row 行番号
  /// @param[in] col 列番号
  /// @note 要素がない場合には NULL を返す．
  SmCell*
  find_elem(ymuint row,
	    ymuint col) const;

  /// @brief 定数項を取り出す．
  /// @param[in] row 行番号
  double
  const_elem(ymuint row) const;


public:

  /// @brief 要素の値を設定する．
  /// @param[in] row 行番号
  /// @param[in] col 列番号
  /// @param[in] val 値
  void
  set_value(ymuint row,
	    ymuint col,
	    double val);

  /// @brief 定数項を設定する．
  /// @param[in] row 行番号
  /// @param[in] val 値
  void
  set_const(ymuint row,
	    double value);

  /// @brief ピボット演算を行う．
  /// @param[in] src_row 参照元の行
  /// @param[in] dst_row 対象の行
  /// @param[in] denom 係数
  double
  pivot(ymuint src_row,
	ymuint dst_row,
	double denom);


#ifdef SANITY_CHECK
  void
  sanity_check(const char* msg);
#endif

  /// @brief 使用中のセル数を得る．
  size_t
  cell_num() const;

  /// @brief 使用中のメモリ量を得る．
  size_t
  used_mem() const;

  /// @brief 実際に確保されたメモリ量を得る．
  size_t
  allocated_mem() const;

  
private:

  /// @brief セルを確保する．
  SmCell*
  new_cell(ymuint row,
	   ymuint col,
	   double val);

  /// @brief セルを削除する．
  void
  delete_cell(SmCell* left,
	      SmCell* cell);
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // セルの確保用アロケータ
  UnitAlloc mAlloc;
  
  // サイズ
  ymuint32 mSize;

  // 行の先頭の配列
  SmCell* mRowArray;
  
  // 定数項の配列
  double* mConstArray;

  // 使用中のセル数
  ymuint64 mCellNum;
  
#ifdef SANITY_CHECK
  double* mShadowArray;
#endif
  
};


/// @brief 行列の内容を出力する．
/// @param[in] s 出力先のストリーム
/// @param[in] m 対象の行列
void
display(ostream& s,
	const SMatrix& m);

/// @brief ガウスの消去法を行って連立方程式の解を得る．
/// @param[in] src_matrix 対象の行列
/// @param[in] solution 解を格納するベクタ
/// @return 解が得られたら true を返す．
bool
gaussian_elimination(const SMatrix& src_matrix,
		     vector<double>& solution);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 列番号を得る．
inline
ymuint
SmCell::col_pos() const
{
  return mColPos;
}

// @brief 値を得る．
inline
double
SmCell::value() const
{
  return mVal;
}

// @brief 値を設定する．
inline
void
SmCell::set_value(double val)
{
  mVal = val;
}

// @brief 右に隣接したセルを得る．
inline
SmCell*
SmCell::right() const
{
  return mRightLink;
}

// @brief 行数を返す．
inline
ymuint32
SMatrix::size() const
{
  return mSize;
}

// @brief 行の先頭を得る．
// @param[in] pos 行番号 ( 0 <= pos < size() )
inline
SmCell*
SMatrix::row_top(ymuint pos) const
{
  return mRowArray[pos].right();
}

// @brief 行の末尾を得る．
// @param[in] pos 行番号 ( 0 <= pos < size() )
// @note このセルはダミー
inline
SmCell*
SMatrix::row_end(ymuint pos) const
{
  return &mRowArray[pos];
}

// @brief 定数項を取り出す．
// @param[in] row 行番号
inline
double
SMatrix::const_elem(ymuint row) const
{
  return mConstArray[row];
}

// @brief 定数項を設定する．
// @param[in] row 行番号
// @param[in] val 値
inline
void
SMatrix::set_const(ymuint row,
		   double value)
{
  mConstArray[row] = value;
}

END_NAMESPACE_YM_SEAL

#endif // LIBYM_SEAL_SMATRIX_H
