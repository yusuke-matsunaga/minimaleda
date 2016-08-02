
/// @file libym_seal/SMatrix.cc
/// @brief SMatrix の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SMatrix.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "SMatrix.h"


BEGIN_NAMESPACE_YM_SEAL

// @brief コンストラクタ
// @param[in] size 変数の数
SMatrix::SMatrix(ymuint size) :
  mAlloc(sizeof(SmCell), 1024),
  mSize(size)
{
  mRowArray = new SmCell[mSize];
  mConstArray = new double[mSize];
  for (ymuint i = 0; i < mSize; ++ i) {
    SmCell& cell = mRowArray[i];
    cell.mColPos = 0;
    cell.mVal = 0.0;
    cell.mRightLink = &cell;
  }
  for (ymuint i = 0; i < mSize; ++ i) {
    mConstArray[i] = 0.0;
  }
  mCellNum = 0;
  
#ifdef SANITY_CHECK
  mShadowArray = new double[mSize * mSize];
  for (ymuint i = 0; i < mSize * mSize; ++ i) {
    mShadowArray[i] = 0.0;
  }
  sanity_check("constructor");
#endif
}

#if 0
// @brief コピーコンストラクタ
SMatrix::SMatrix(const SMatrix& src) :
  mAlloc(1024, sizeof(SmCell)),
  mSize(src.mSize),
  mSize(src.mSize),
  mBody(new double[mSize * mSize])
{
  ymuint32 n = mSize * mSize;
  for (ymuint32 i = 0; i < n; ++ i) {
    mBody[i] = src.mBody[i];
  }
}

// @brief 代入演算子
const SMatrix&
SMatrix::operator=(const SMatrix& src)
{
  if ( &src != this ) {
    ymuint32 n = src.mSize * src.mSize;
    if ( mSize != src.mSize ||
	 mSize != src.mSize ) {
      delete [] mBody;
      mBody = new double[n];
    }
    for (ymuint32 i = 0; i < n; ++ i) {
      mBody[i] = src.mBody[i];
    }
  }
  return *this;
}
#endif

// @brief デストラクタ
SMatrix::~SMatrix()
{
  delete [] mRowArray;
  delete [] mConstArray;
}

// @brief 要素を取り出す．
// @param[in] row 行番号
// @param[in] col 列番号
// @note 要素がない場合には NULL を返す．
SmCell*
SMatrix::find_elem(ymuint row,
		   ymuint col) const
{
  SmCell* end = row_end(row);
  for (SmCell* cur = row_top(row); cur != end; cur = cur->right()) {
    ymuint c = cur->col_pos();
    if ( c == col ) {
      return cur;
    }
    else if ( c > col ) {
      return NULL;
    }
  }
  return NULL;
}

// @brief 要素の値を設定する．
// @param[in] row 行番号
// @param[in] col 列番号
// @param[in] val 値
void
SMatrix::set_value(ymuint row,
		   ymuint col,
		   double val)
{
  if ( val == 0.0 ) {
    SmCell* end = row_end(row);
    SmCell* prev = end;
    for (SmCell* cur = prev->right(); cur != end; ) {
      ymuint c = cur->col_pos();
      if ( c == col ) {
	// cur を削除する．
	delete_cell(prev, cur);
	return;
      }
      else if ( c > col ) {
	return;
      }
      prev = cur;
      cur = prev->right();
    }
  }
  else {
    SmCell* top = &mRowArray[row];
    SmCell* prev = top;
    SmCell* cur = prev->right();
    bool found = false;
    while ( cur != top ) {
      ymuint pos = cur->col_pos();
      if ( pos == col ) {
	cur->mVal = val;
	found = true;
	break;
      }
      if ( pos > col ) {
	break;
      }
      prev = cur;
      cur = prev->right();
    }
    if ( !found ) {
      // prev と cur の間に新しいセルを挿入する．
      SmCell* cell = new_cell(row, col, val);
      prev->mRightLink = cell;
      cell->mRightLink = cur;
    }
  }

#ifdef SANITY_CHECK
  mShadowArray[row * mSize + col] = val;
  sanity_check("set_value");
#endif
}

// @brief ピボット演算を行う．
double
SMatrix::pivot(ymuint src_row,
	       ymuint dst_row,
	       double denom)
{
  SmCell* src_top = row_top(src_row);
  SmCell* src_end = row_end(src_row);
  SmCell* dst_end = row_end(dst_row);

  double max_value = 0.0;
  SmCell* src_cell = src_top;
  SmCell* dst_prev = dst_end;
  SmCell* dst_cell = dst_prev->right();
  ymuint src_col = src_cell->col_pos();
  ymuint dst_col = dst_cell->col_pos();
  while ( src_cell != src_end ) {
    if ( src_col == dst_col ) {
      dst_cell->mVal -= src_cell->value() * denom;
      if ( dst_cell->mVal == 0.0 ) {
	// dst_cell を削除
	delete_cell(dst_prev, dst_cell);
	// dst_prev はそのままでよい．
      }
      else {
	double v = fabs(dst_cell->value());
	if ( max_value < v ) {
	  max_value = v;
	}
	dst_prev = dst_cell;
      }
      src_cell = src_cell->right();
      src_col = src_cell->col_pos();
      dst_cell = dst_prev->right();
      if ( dst_cell == dst_end ) {
	dst_col = mSize;
      }
      else {
	dst_col = dst_cell->col_pos();
      }
    }
    else if ( src_col < dst_col ) {
      double v = src_cell->value() * denom;
      if ( v != 0.0 ) {
	SmCell* cell = new_cell(dst_row, src_col, - v);
	dst_prev->mRightLink = cell;
	cell->mRightLink = dst_cell;
	dst_prev = cell;
	v = fabs(v);
	if ( max_value < v ) {
	  max_value = v;
	}
      }
      src_cell = src_cell->right();
      src_col = src_cell->col_pos();
    }
    else { // src_col > dst_col
      double v = fabs(dst_cell->value());
      if ( max_value < v ) {
	max_value = v;
      }
      dst_prev = dst_cell;
      dst_cell = dst_prev->right();
      if ( dst_cell == dst_end ) {
	dst_col = mSize;
      }
      else {
	dst_col = dst_cell->col_pos();
      }
    }
  }
  for ( ; dst_cell != dst_end; dst_cell = dst_cell->right()) {
    double v = fabs(dst_cell->value());
    if ( max_value < v ) {
      max_value = v;
    }
  }
  mConstArray[dst_row] -= mConstArray[src_row] * denom;

#ifdef SANITY_CHECK
  {
    double v0 = mShadowArray[src_row * mSize + src_col];
    double d = mShadowArray[dst_row * mSize + src_col] / v0;
    for (ymuint i = 0; i < mSize; ++ i) {
      mShadowArray[dst_row * mSize + i] -= mShadowArray[src_row * mSize + i] * d;
    }
  }
  sanity_check("pivot");
#endif

  return max_value;
}

// @brief 使用中のセル数を得る．
size_t
SMatrix::cell_num() const
{
  return mCellNum;
}

// @brief 使用中のメモリ量を得る．
size_t
SMatrix::used_mem() const
{
  return mAlloc.used_size();
}

// @brief 実際に確保されたメモリ量を得る．
size_t
SMatrix::allocated_mem() const
{
  return mAlloc.allocated_size();
}

// @brief セルを確保する．
SmCell*
SMatrix::new_cell(ymuint row,
		  ymuint col,
		  double val)
{
  void* p = mAlloc.get_memory(sizeof(SmCell));
  SmCell* cell = new (p) SmCell();
  cell->mColPos = col;
  cell->mVal = val;
  ++ mCellNum;
  return cell;
}

// @brief セルを削除する．
void
SMatrix::delete_cell(SmCell* left,
		     SmCell* cell)
{
  SmCell* right = cell->right();
  left->mRightLink = right;
  mAlloc.put_memory(sizeof(SmCell), cell);
  -- mCellNum;
}

// @brief 行列の内容を出力する．
void
display(ostream& s,
	const SMatrix& m)
{
  ymuint32 nv = m.size();

  for (ymuint32 i = 0; i < nv; ++ i) {
    cout << setw(7) << i << ":";
    for (SmCell* cell = m.row_top(i); cell != m.row_end(i); cell = cell->right()) {
      cout << " " << setw(7) << cell->col_pos() << ", "
	   << setw(7) << setprecision(4) << cell->value();
    }
    cout << endl;
  }
}

#ifdef SANITY_CHECK

void
SMatrix::sanity_check(const char* msg)
{
  bool error = false;
  for (ymuint i = 0; i < mSize; ++ i) {
    ymuint last_col = 0;
    for (SmCell* cell = row_top(i); cell != row_end(i); cell = cell->right()) {
      SmCell* up = cell->up();
      SmCell* down = cell->down();
      SmCell* left = cell->left();
      SmCell* right = cell->right();
      if ( up && up->down() != cell ) {
	cout << "Up link mismatch at (" << cell->row_pos() << ", "
	     << cell->col_pos() << ")" << endl;
	error = true;
      }
      if ( down && down->up() != cell ) {
	cout << "Down link mismatch at (" << cell->row_pos() << ", "
	     << cell->col_pos() << ")" << endl;
	error = true;
      }
      if ( left && left->right() != cell ) {
	cout << "Left link mismatch at (" << cell->row_pos() << ", "
	     << cell->col_pos() << ")" << endl;
	error = true;
      }
      if ( right && right->left() != cell ) {
	cout << "Right link mismatch at (" << cell->row_pos() << ", "
	     << cell->col_pos() << ")" << endl;
	error = true;
      }
      for (ymuint j = last_col; j < cell->col_pos(); ++ j) {
	if ( mShadowArray[i * mSize + j] != 0.0 ) {
	  cout << "Value mismatch at (" << i << ", " << j << ")" << endl;
	  error = true;
	}
      }
      if ( mShadowArray[i * mSize + cell->col_pos()] != cell->value() ) {
	cout << "Value mismatch at (" << i << ", "
	     << cell->col_pos() << ")" << endl;
	error = true;
      }
      last_col = cell->col_pos() + 1;
    }
  }
  if ( error ) {
    cout << "error occured in " << msg << endl;
    display(cout, *this);
    for (ymuint i = 0; i < mSize; ++ i) {
      cout << "R#" << i << ": ";
      for (ymuint j = 0;j < mSize; ++ j) {
	if ( mShadowArray[i * mSize + j] != 0.0 ) {
	  cout << " " << j << ", " << mShadowArray[i * mSize + j];
	}
      }
      cout << endl;
    }
    abort();
  }
}

#endif

END_NAMESPACE_YM_SEAL
