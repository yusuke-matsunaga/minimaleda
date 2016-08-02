
/// @file libym_seal/Matrix.cc
/// @brief Matrix の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Matrix.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "Matrix.h"


BEGIN_NAMESPACE_YM_SEAL

// @brief コンストラクタ
// @param[in] row_num 行数
// @param[in] col_num 列数
Matrix::Matrix(ymuint32 row_num,
	       ymuint32 col_num) :
  mRowNum(row_num),
  mColNum(col_num),
  mBody(new double[row_num * col_num])
{
  for (ymuint i = 0; i < row_num * col_num; ++ i) {
    mBody[i] = 0.0;
  }
}
  
// @brief コピーコンストラクタ
Matrix::Matrix(const Matrix& src) :
  mRowNum(src.mRowNum),
  mColNum(src.mColNum),
  mBody(new double[mRowNum * mColNum])
{
  ymuint32 n = mRowNum * mColNum;
  for (ymuint32 i = 0; i < n; ++ i) {
    mBody[i] = src.mBody[i];
  }
}

// @brief 代入演算子
const Matrix&
Matrix::operator=(const Matrix& src)
{
  if ( &src != this ) {
    ymuint32 n = src.mRowNum * src.mColNum;
    if ( mRowNum != src.mRowNum ||
	 mColNum != src.mColNum ) {
      delete [] mBody;
      mBody = new double[n];
    }
    for (ymuint32 i = 0; i < n; ++ i) {
      mBody[i] = src.mBody[i];
    }
  }
  return *this;
}

// @brief デストラクタ
Matrix::~Matrix()
{
  delete [] mBody;
}

// @brief 行列の内容を出力する．
void
display(ostream& s,
	const Matrix& m)
{
  ymuint32 nr = m.row_size();
  ymuint32 nc = m.col_size();

  for (ymuint32 i = 0; i < nr; ++ i) {
    for (ymuint32 j = 0; j < nc; ++ j) {
      cout << " " << setw(7) << setprecision(4) << m.elem(i, j);
    }
    cout << endl;
  }
}

END_NAMESPACE_YM_SEAL
