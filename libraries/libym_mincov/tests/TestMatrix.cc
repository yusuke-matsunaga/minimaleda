
/// @file libym_mincov/tests/TestMatrix.cc
/// @brief TestMatrix の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TestMatrix.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "TestMatrix.h"


BEGIN_NAMESPACE_YM_MINCOV
  
// @brief コンストラクタ
// @param[in] row_size 行数
// @param[in] col_size 列数
// @param[in] cost_size コストの次元
TestMatrix::TestMatrix(ymuint32 row_size,
		       ymuint32 col_size,
		       ymuint32 cost_size) :
  mRowSize(row_size),
  mColSize(col_size),
  mCostSize(cost_size),
  mBody(row_size * col_size, 0),
  mMatrix(row_size, col_size, cost_size)
{
}
  
// @brief コピーコンストラクタ
// @param[in] src コピー元の行列
TestMatrix::TestMatrix(const TestMatrix& src) :
  mRowSize(src.row_size()),
  mColSize(src.col_size()),
  mCostSize(src.cost_size()),
  mBody(src.mBody),
  mMatrix(src.mMatrix)
{
}

// @brief 代入演算子
// @param[in] src コピー元の行列
const TestMatrix&
TestMatrix::operator=(const TestMatrix& src)
{
  if ( this != &src) {
    mBody.clear();
    mRowSize = src.row_size();
    mColSize = src.col_size();
    mCostSize = src.cost_size();
    ymuint n = mRowSize * mColSize;
    mBody.resize(n);
    for (ymuint i = 0; i < n; ++ i) {
      mBody[i] = src.mBody[i];
    }
    mMatrix = src.mMatrix;
  }
  return *this;
}

// @brief デストラクタ
TestMatrix::~TestMatrix()
{
}
  
// @brief 内容をクリアする．
// @note 行/列のサイズは不変
void
TestMatrix::clear()
{
  ymuint n = mBody.size();
  for (ymuint i = 0; i < n; ++ i) {
    mBody[i] = 0;
  }
  mMatrix.clear();
  
  assert_cond(sanity_check(), __FILE__, __LINE__);
}

// @brief サイズを変更する．
// @param[in] row_size 行数
// @param[in] col_size 列数
// @param[in] cost_size コストの次元
// @note 内容はクリアされる．
void
TestMatrix::resize(ymuint32 row_size,
		   ymuint32 col_size,
		   ymuint32 cost_size)
{
  if ( row_size != mRowSize || col_size != mColSize || cost_size != mCostSize ) {
    clear();
    mRowSize = row_size;
    mColSize = col_size;
    mCostSize = cost_size;
    mBody.resize(row_size * col_size, 0);
    mMatrix.resize(row_size, col_size, cost_size);
  }
  
  assert_cond(sanity_check(), __FILE__, __LINE__);
}

// @brief 要素を追加する．
// @param[in] row_pos 追加する要素の行番号
// @param[in] col_pos 追加する要素の列番号
void
TestMatrix::set(ymuint32 row_pos,
		ymuint32 col_pos)
{
  set_elem(row_pos, col_pos, 1);
  mMatrix.insert_elem(row_pos, col_pos);
  
  assert_cond(sanity_check(), __FILE__, __LINE__);
}

// @brief コストを設定する．
// @param[in] col_pos 列番号
// @param[in] val_pos 値の位置番号
// @param[in] value 設定する値
void
TestMatrix::set_cost(ymuint32 col_pos,
		     ymuint32 val_pos,
		     double value)
{
  mMatrix.set_col_cost(col_pos, val_pos, value);
}

// @brief 列を選択し，被覆される行を削除する．
// @param[in] col_pos 選択した列
void
TestMatrix::select_col(ymuint32 col_pos)
{
  for (ymuint row_pos = 0; row_pos < row_size(); ++ row_pos) {
    if ( elem(row_pos, col_pos) ) {
      // row_pos の行を削除する．
      delete_row(row_pos);
    }
  }
  // col_pos の行は消えているはず．
  
  mMatrix.select_col(col_pos);
  
  assert_cond(sanity_check(), __FILE__, __LINE__);
}

// @brief 行を削除する．
void
TestMatrix::delete_row(ymuint32 row_pos)
{
  for (ymuint c = 0; c < col_size(); ++ c) {
    set_elem(row_pos, c, 0);
  }
  mMatrix.delete_row(row_pos);
  
  assert_cond(sanity_check(), __FILE__, __LINE__);
}

// @brief 列を削除する．
void
TestMatrix::delete_col(ymuint32 col_pos)
{
  for (ymuint r = 0; r < row_size(); ++ r) {
    set_elem(r, col_pos, 0);
  }
  mMatrix.delete_col(col_pos);

  assert_cond(sanity_check(), __FILE__, __LINE__);
}

// @brief 削除スタックにマーカーを書き込む．
void
TestMatrix::backup()
{
  mStack.push_back(new TestMatrix(*this));
  mMatrix.backup();
}

// @brief 直前のマーカーまで処理を戻す．
void
TestMatrix::restore()
{
  TestMatrix* src = mStack.back();
  mStack.pop_back();

  *this = *src;
  delete src;

  mMatrix.restore();

  assert_cond(sanity_check(), __FILE__, __LINE__);
}

// @brief 内容を出力する．
void
TestMatrix::display(ostream& s) const
{
  for (ymuint r = 0; r < row_size(); ++ r) {
    vector<ymuint32> cols;
    row_vector(r, cols);
    if ( !cols.empty() ) {
      s << r << ":";
      for (vector<ymuint32>::iterator p = cols.begin();
	   p != cols.end(); ++ p) {
	s << " " << *p;
      }
      s << endl;
    }
  }
}

// @brief 一貫性のチェックを行う．
bool
TestMatrix::sanity_check()
{
  ymuint nr = mMatrix.row_size();
  if ( nr != row_size() ) {
    cerr << "check_equiv: row_size() mismatch!" << endl;
    return false;
  }
  ymuint nc = mMatrix.col_size();
  if ( nc != col_size() ) {
    cerr << "check_equiv: col_size() mismatch!" << endl;
    return false;
  }
  
  if ( nr == 0 && nc == 0 ) {
    return true;
  }
	 
  // 行ごとの比較
  ymuint prev_pos = 0;
  for (const MincovRowHead* row = mMatrix.row_front();
       !mMatrix.is_row_end(row); row = row->next()) {
    ymuint row_pos = row->pos();
    vector<ymuint32> cols;
    for (ymuint row_pos1 = prev_pos; row_pos1 < row_pos; ++ row_pos1) {
      row_vector(row_pos1, cols);
      if ( cols.size() > 0 ) {
	cerr << "check_equiv: row_num(" << row_pos1 << ") mismatch!" << endl;
	cerr << " no RowHead"
	     << ", cols.size() = " << cols.size() << endl;
	return false;
      }
    }
    prev_pos = row_pos + 1;
    row_vector(row_pos, cols);
    if ( row->num() != cols.size() ) {
      cerr << "check_equiv: row_num(" << row_pos << ") mismatch!" << endl;
      cerr << " row->num() = " << row->num()
	   << ", cols.size() = " << cols.size() << endl;
      return false;
    }
    const MincovCell* cell = row->front();
    const MincovCell* prev = cell->row_prev();
    if ( prev->row_next() != cell ) {
      cerr << "check_equiv: row link inconsistency!" << endl;
      return false;
    }
    for (ymuint i = 0; !row->is_end(cell); ++ i) {
      if ( cell->col_pos() != cols[i] ) {
	cerr << "check_equiv: row_vector(" << row_pos << ") mismatch!" << endl;
	return false;
      }
      const MincovCell* next = cell->row_next();
      if ( next->row_prev() != cell ) {
	cerr << "check_equiv: row link inconsistency!" << endl;
	return false;
      }
      if ( row->is_end(next) && next != prev ) {
	cerr << "check_equiv: row link inconsistency!" << endl;
	return false;
      }
      cell = next;
    }
  }

  // 列ごとの比較
  prev_pos = 0;
  for (const MincovColHead* col = mMatrix.col_front();
       !mMatrix.is_col_end(col); col = col->next()) {
    ymuint col_pos = col->pos();
    vector<ymuint32> rows;
    for (ymuint col_pos1 = prev_pos; col_pos1 < col_pos; ++ col_pos1) {
      col_vector(col_pos1, rows);
      if ( rows.size() > 0 ) {
	cerr << "check_equiv: col_num(" << col_pos1 << ") mismatch!" << endl;
	cerr << " no ColHead"
	     << ", rows.size() = " << rows.size() << endl;
	return false;
      }
    }
    prev_pos = col_pos + 1;
    col_vector(col_pos, rows);
    if ( col->num() != rows.size() ) {
      cerr << "check_equiv: col_num(" << col_pos << ") mismatch!" << endl;
      cerr << " col->num() = " << col->num()
	   << ", rows.size() = " << rows.size() << endl;
      return false;
    }
    const MincovCell* cell = col->front();
    const MincovCell* prev = cell->col_prev();
    if ( prev->col_next() != cell ) {
      cerr << "check_equiv: col link inconsistency!" << endl;
      return false;
    }
    for (ymuint i = 0; !col->is_end(cell); ++ i) {
      if ( cell->row_pos() != rows[i] ) {
	cerr << "check_equiv: col_vector(" << col_pos << ") mismatch!" << endl;
	return false;
      }
      const MincovCell* next = cell->col_next();
      if ( next->col_prev() != cell ) {
	cerr << "check_equiv: col link inconsistency!" << endl;
	return false;
      }
      if ( col->is_end(next) && next != prev ) {
	cerr << "check_equiv: col link inconsistency!" << endl;
	return false;
      }
      cell = next;
    }
  }

  return true;
}

// @brief 指定された行の非ゼロ要素の列番号を得る．
// @param[in] row_pos 行番号 ( 0 <= row_pos < row_size() )
// @param[out] cols 列番号を納めるベクタ
void
TestMatrix::row_vector(ymuint32 row_pos,
		       vector<ymuint32>& cols) const
{
  cols.clear();
  ymuint nc = col_size();
  for (ymuint col_pos = 0; col_pos < nc; ++ col_pos) {
    if ( elem(row_pos, col_pos) ) {
      cols.push_back(col_pos);
    }
  }
}

// @brief 指定された列の非ゼロ要素の行番号を得る．
// @param[in] col_pos 列番号 ( 0 <= col_pos < col_size() )
// @param[out] rows 行番号を納めるベクタ
void
TestMatrix::col_vector(ymuint32 col_pos,
		       vector<ymuint32>& rows) const
{
  rows.clear();
  ymuint nr = row_size();
  for (ymuint row_pos = 0; row_pos < nr; ++ row_pos) {
    if ( elem(row_pos, col_pos) ) {
      rows.push_back(row_pos);
    }
  }
}

END_NAMESPACE_YM_MINCOV
