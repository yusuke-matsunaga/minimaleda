#ifndef LIBYM_SEAL_MATRIX_H
#define LIBYM_SEAL_MATRIX_H

/// @file libym_seal/Matrix.h
/// @brief Matrix のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Matrix.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_seal/seal_nsdef.h"


BEGIN_NAMESPACE_YM_SEAL

//////////////////////////////////////////////////////////////////////
/// @class Matrix Matrix.h "Matrix.h"
/// @brief 行列を表すクラス
//////////////////////////////////////////////////////////////////////
class Matrix
{
public:

  /// @brief コンストラクタ
  /// @param[in] row_num 行数
  /// @param[in] col_num 列数
  /// @note 内容は不定
  Matrix(ymuint32 row_num,
	 ymuint32 col_num);
  
  /// @brief コピーコンストラクタ
  Matrix(const Matrix& src);

  /// @brief 代入演算子
  const Matrix&
  operator=(const Matrix& src);
  
  /// @brief デストラクタ
  ~Matrix();

  
public:

  /// @brief 行数を返す．
  ymuint32
  row_size() const;

  /// @brief 列数を返す．
  ymuint32
  col_size() const;
  
  /// @brief 要素に対するアクセス関数
  /// @param[in] r_pos 行位置
  /// @param[in] c_pos 列位置
  double&
  elem(ymuint32 r_pos,
       ymuint32 c_pos);
  
  /// @brief 要素に対するアクセス関数
  /// @param[in] r_pos 行位置
  /// @param[in] c_pos 列位置
  double
  elem(ymuint32 r_pos,
       ymuint32 c_pos) const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 行数
  ymuint32 mRowNum;

  // 列数
  ymuint32 mColNum;

  // 本体
  double* mBody;
  
};


/// @brief 行列の内容を出力する．
/// @param[in] s 出力先のストリーム
/// @param[in] m 対象の行列
void
display(ostream& s,
	const Matrix& m);

/// @brief ガウスの消去法を行って連立方程式の解を得る．
/// @param[in] src_matrix 対象の行列
/// @param[in] solution 解を格納するベクタ
/// @return 解が得られたら true を返す．
bool
gaussian_elimination(const Matrix& src_matrix,
		     vector<double>& solution);

/// @brief LU分解を行って連立方程式の解を得る．
/// @param[in] src_matrix 対象の行列
/// @param[in] solution 解を格納するベクタ
/// @return 解が得られたら true を返す．
bool
lu_decomp(const Matrix& src_matrix,
	  vector<double>& solution);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 行数を返す．
inline
ymuint32
Matrix::row_size() const
{
  return mRowNum;
}

// @brief 列数を返す．
inline
ymuint32
Matrix::col_size() const
{
  return mColNum;
}

// @brief 要素に対するアクセス関数
// @param[in] r_pos 行位置
// @param[in] c_pos 列位置
inline
double&
Matrix::elem(ymuint32 r_pos,
	     ymuint32 c_pos)
{
  return mBody[r_pos * mColNum + c_pos];
}
  
// @brief 要素に対するアクセス関数
// @param[in] r_pos 行位置
// @param[in] c_pos 列位置
inline
double
Matrix::elem(ymuint32 r_pos,
	     ymuint32 c_pos) const
{
  return mBody[r_pos * mColNum + c_pos];
}

END_NAMESPACE_YM_SEAL

#endif // LIBYM_SEAL_MATRIX_H
