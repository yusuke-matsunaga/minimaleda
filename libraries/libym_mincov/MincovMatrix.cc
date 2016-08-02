
/// @file libym_mincov/MincovMatrix.cc
/// @brief MincovMatrix の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: MincovMatrix.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mincov/MincovMatrix.h"
#include "ym_mincov/MincovCost.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
// クラス MincovCell
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MincovCell::MincovCell()
{
}

// @brief デストラクタ
MincovCell::~MincovCell()
{
}


//////////////////////////////////////////////////////////////////////
// クラス MincovRowHead
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MincovRowHead::MincovRowHead()
{
}

/// @brief デストラクタ
MincovRowHead::~MincovRowHead()
{
}

// @brief 要素を追加する位置を探す．
// @param[in] cell 追加する要素
// @retval true 追加が成功した．
// @retval false 同じ要素がすでに存在した．
// @note 結果は cell の mLeftLink, mRightLink に設定される．
bool
MincovRowHead::search_insert_pos(MincovCell* cell)
{
  ymuint32 col_pos = cell->col_pos();
  MincovCell* pcell;
  MincovCell* ncell;
  if ( num() == 0 || back()->col_pos() < col_pos ) {
    // 末尾への追加
    ncell = &mDummy;
    pcell = ncell->mLeftLink;
  }
  else {
    // 追加位置を探索
    // この時点で back->col_pos() >= col_pos が成り立っている．
    for (pcell = &mDummy; ; pcell = ncell) {
      ncell = pcell->mRightLink;
      if ( ncell->col_pos() == col_pos ) {
	// 列番号が重複しているので無視する．
	return false;
      }
      if ( ncell->col_pos() > col_pos ) {
	// pcell と ncell の間に cell を挿入する．
	break;
      }
      assert_cond(!is_end(ncell), __FILE__, __LINE__);
    }
  }
  cell->mLeftLink = pcell;
  cell->mRightLink = ncell;
  
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス MincovColHead
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MincovColHead::MincovColHead() :
  mNum(0)
{
}

/// @brief デストラクタ
MincovColHead::~MincovColHead()
{
}

// @brief 要素を追加する位置を探す．
// @param[in] cell 追加する要素
// @retval true 追加が成功した．
// @retval false 同じ要素がすでに存在した．
// @note 結果は cell の mUpLink, mDownLink に設定される．
bool
MincovColHead::search_insert_pos(MincovCell* cell)
{
  ymuint32 row_pos = cell->row_pos();
  MincovCell* pcell;
  MincovCell* ncell;
  if ( num() == 0 || back()->row_pos() < row_pos ) {
    // 末尾への追加
    ncell = &mDummy;
    pcell = ncell->mUpLink;
  }
  else {
    // 追加位置を探索
    // この時点で back->row_pos() >= row_pos が成り立っている．
    for (pcell = &mDummy; ; pcell = ncell) {
      ncell = pcell->mDownLink;
      if ( ncell->row_pos() == row_pos ) {
	// 列番号が重複しているので無視する．
	return false;
      }
      if ( ncell->row_pos() > row_pos ) {
	// pcell と ncell の間に cell を挿入する．
	break;
      }
      assert_cond(!is_end(ncell), __FILE__, __LINE__);
    }
  }
  cell->mUpLink = pcell;
  cell->mDownLink = ncell;
  
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス MincovMatrix
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] row_size 行数
// @param[in] col_size 列数
// @param[in] cost_size コストの次元
MincovMatrix::MincovMatrix(ymuint32 row_size,
			   ymuint32 col_size,
			   ymuint32 cost_size) :
  mCellAlloc(sizeof(MincovCell), 1024),
  mRowSize(0),
  mColSize(0),
  mRowArray(NULL),
  mColArray(NULL),
  mColCostArray(NULL)
{
  resize(row_size, col_size, cost_size);
}
  
// @brief コピーコンストラクタ
// @param[in] src コピー元のオブジェクト
MincovMatrix::MincovMatrix(const MincovMatrix& src) :
  mCellAlloc(sizeof(MincovCell), 1024),
  mRowSize(0),
  mColSize(0),
  mRowArray(NULL),
  mColArray(NULL),
  mColCostArray(NULL)
{
  resize(src.row_size(), src.col_size(), src.cost_size());
  copy(src);
}
  
// @brief 代入演算子
// @param[in] src コピー元のオブジェクト
const MincovMatrix&
MincovMatrix::operator=(const MincovMatrix& src)
{
  if ( this != &src ) {
    clear();
    resize(src.row_size(), src.col_size(), src.cost_size());
    copy(src);
  }
  return *this;
}

// @brief デストラクタ
MincovMatrix::~MincovMatrix()
{
  clear();
  resize(0, 0, 0);
}

// @brief 内容をクリアする．
void
MincovMatrix::clear()
{
  mCellAlloc.destroy();

  for (ymuint i = 0; i < row_size(); ++ i) {
    mRowArray[i].clear();
  }
  for (ymuint i = 0; i < col_size(); ++ i) {
    mColArray[i].clear();
  }
}

// @brief サイズを変更する．
// @param[in] row_size 行数
// @param[in] col_size 列数
// @param[in] cost_size コストの次元
void
MincovMatrix::resize(ymuint32 row_size,
		     ymuint32 col_size,
		     ymuint32 cost_size)
{
  if ( mRowSize != row_size || mColSize != col_size ) {
    delete [] mRowArray;
    delete [] mColArray;
    for (ymuint i = 0; i < mColSize; ++ i) {
      delete mColCostArray[i];
    }
    delete [] mColCostArray;
    
    mRowSize = row_size;
    mColSize = col_size;
    mCostSize = cost_size;
    mRowArray = new MincovRowHead[mRowSize];
    mColArray = new MincovColHead[mColSize];
    mColCostArray = new MincovCost*[mColSize];
    
    mRowHead.mNext = &mRowHead;
    mRowHead.mPrev = &mRowHead;
    mRowHead.clear();
    for (ymuint i = 0; i < mRowSize; ++ i) {
      MincovRowHead* row = &mRowArray[i];
      row->clear();
      row->mPos = i;
      row->clear();
    }
    
    mColHead.mNext = &mColHead;
    mColHead.mPrev = &mColHead;
    mColHead.clear();
    for (ymuint i = 0; i < mColSize; ++ i) {
      MincovColHead* col = &mColArray[i];
      col->clear();
      col->mPos = i;
      col->clear();
#ifdef USE_MINCOVCOST
      mColCostArray[i] = new MincovCost(mCostSize);
#else
      mColCostArray[i] = new MincovCost();
#endif
    }

    mDelStack.reserve(row_size + col_size);
  }
}

// @brief 内容をコピーする．
void
MincovMatrix::copy(const MincovMatrix& src)
{
  assert_cond(row_size() == src.row_size(), __FILE__, __LINE__);
  assert_cond(col_size() == src.col_size(), __FILE__, __LINE__);
  assert_cond(cost_size() == src.cost_size(), __FILE__, __LINE__);
  
  for (ymuint i = 0; i < row_size(); ++ i) {
    const MincovRowHead* src_row = src.row(i);
    for (const MincovCell* src_cell = src_row->front();
	 !src_row->is_end(src_cell); src_cell = src_cell->row_next()) {
      insert_elem(src_cell->row_pos(), src_cell->col_pos());
    }
  }
  for (ymuint i = 0; i < col_size(); ++ i) {
    *mColCostArray[i] = src.col_cost(i);
  }
}
  
// @brief 要素を追加する．
// @param[in] row_pos 追加する要素の行番号
// @param[in] col_pos 追加する要素の列番号
// @return 追加された要素を返す．
MincovCell*
MincovMatrix::insert_elem(ymuint32 row_pos,
			  ymuint32 col_pos)
{
  MincovCell* cell = alloc_cell();
  cell->mRowPos = row_pos;
  cell->mColPos = col_pos;
  
  MincovRowHead* row = &mRowArray[row_pos];
  bool stat1 = row->search_insert_pos(cell);
  if ( !stat1 ) {
    // 列番号が重複しているので無視する．
    free_cell(cell);
    return NULL;
  }

  MincovColHead* col = &mColArray[col_pos];
  bool stat2 = col->search_insert_pos(cell);
  assert_cond(stat2, __FILE__, __LINE__);

  row->insert_elem(cell);
  if ( row->num() == 1 ) {
    MincovRowHead* prev = mRowHead.mPrev; // 末尾
    MincovRowHead* next = &mRowHead;
    if ( prev == next || prev->pos() < row_pos ) {
      // 末尾に追加
      ;
    }
    else {
      for (prev = &mRowHead; ; prev = next) {
	next = prev->mNext;
	assert_cond(next->pos() != row_pos, __FILE__, __LINE__);
	if ( next->pos() > row_pos ) {
	  // prev と next の間に挿入する．
	  break;
	}
	assert_cond(next != &mRowHead, __FILE__, __LINE__);
      }
    }
    prev->mNext = row;
    row->mPrev = prev;
    row->mNext = next;
    next->mPrev = row;
  }
  
  col->insert_elem(cell);
  if ( col->num() == 1 ) {
    MincovColHead* prev = mColHead.mPrev; // 末尾
    MincovColHead* next = &mColHead;
    if ( prev == next || prev->pos() < col_pos ) {
      // 末尾に追加
      ;
    }
    else {
      for (prev = &mColHead; ; prev = next) {
	next = prev->mNext;
	assert_cond(next->pos() != col_pos, __FILE__, __LINE__);
	if ( next->pos() > col_pos ) {
	  // prev と next の間に挿入する．
	  break;
	}
	assert_cond(next != &mColHead, __FILE__, __LINE__);
      }
    }
    prev->mNext = col;
    col->mPrev = prev;
    col->mNext = next;
    next->mPrev = col;
  }
  
  return cell;
}
  
// @brief 列のコストを設定する．
// @param[in] col_pos 列番号
// @param[in] val_pos 値の位置番号
// @param[in] value 設定する値
void
MincovMatrix::set_col_cost(ymuint32 col_pos,
			   ymuint32 val_pos,
			   double value)
{
#ifdef USE_MINCOVCOST1
  mColCostArray[col_pos]->value() = value;
#else
  mColCostArray[col_pos]->operator[](val_pos) = value;
#endif
}
  
// @brief 列を選択し，被覆される行を削除する．
void
MincovMatrix::select_col(ymuint32 col_pos)
{
  MincovColHead* col = &mColArray[col_pos];
  list<ymuint32> row_list;
  for (const MincovCell* cell = col->front();
       !col->is_end(cell); cell = cell->col_next()) {
    row_list.push_back(cell->row_pos());
  }
  for (list<ymuint32>::iterator p = row_list.begin();
       p != row_list.end(); ++ p) {
    delete_row(*p);
  }
  delete_col(col_pos);
}
    
// @brief 削除スタックにマーカーを書き込む．
void
MincovMatrix::backup()
{
  mDelStack.push_back(0U);
}

// @brief 直前のマーカーまで処理を戻す．
void
MincovMatrix::restore()
{
  while ( !mDelStack.empty() ) {
    ymuint32 tmp = mDelStack.back();
    mDelStack.pop_back();
    if ( tmp == 0U ) {
      break;
    }
    if ( tmp & 2U ) {
      ymuint32 col_pos = tmp >> 2;
      // col_pos の列を元に戻す．
      restore_col(col_pos);
    }
    else {
      ymuint32 row_pos = tmp >> 2;
      // row_pos の行を元に戻す．
      restore_row(row_pos);
    }
  }
}

// @brief 行を削除する．
void
MincovMatrix::delete_row(ymuint32 row_pos)
{
  MincovRowHead* row = &mRowArray[row_pos];
  MincovRowHead* prev = row->mPrev;
  MincovRowHead* next = row->mNext;
  prev->mNext = next;
  next->mPrev = prev;

  for (MincovCell* cell = row->mDummy.mRightLink;
       !row->is_end(cell); cell = cell->mRightLink) {
    ymuint col_pos = cell->col_pos();
    MincovColHead* col = &mColArray[col_pos];
    col->delete_elem(cell);
    if ( col->num() == 0 ) {
      delete_col(col_pos);
    }
  }

  mDelStack.push_back((row_pos << 2) | 1U);
}
  
// @brief 行を復元する．
void
MincovMatrix::restore_row(ymuint32 row_pos)
{
  MincovRowHead* row = &mRowArray[row_pos];
  MincovRowHead* prev = row->mPrev;
  MincovRowHead* next = row->mNext;
  prev->mNext = row;
  next->mPrev = row;
  
  for (MincovCell* cell = row->mDummy.mRightLink;
       !row->is_end(cell); cell = cell->mRightLink) {
    mColArray[cell->col_pos()].insert_elem(cell);
  }
}

// @brief 列を削除する．
void
MincovMatrix::delete_col(ymuint32 col_pos)
{
  MincovColHead* col = &mColArray[col_pos];
  MincovColHead* prev = col->mPrev;
  MincovColHead* next = col->mNext;
  prev->mNext = next;
  next->mPrev = prev;

  for (MincovCell* cell = col->mDummy.mDownLink;
       !col->is_end(cell); cell = cell->mDownLink) {
    ymuint row_pos = cell->row_pos();
    MincovRowHead* row = &mRowArray[row_pos];
    row->delete_elem(cell);
    if ( row->num() == 0 ) {
      delete_row(row_pos);
    }
  }
  
  mDelStack.push_back((col_pos << 2) | 3U);
}

// @brief 列を復元する．
void
MincovMatrix::restore_col(ymuint32 col_pos)
{
  MincovColHead* col = &mColArray[col_pos];
  MincovColHead* prev = col->mPrev;
  MincovColHead* next = col->mNext;
  prev->mNext = col;
  next->mPrev = col;

  for (MincovCell* cell = col->mDummy.mDownLink;
       !col->is_end(cell); cell = cell->mDownLink) {
    mRowArray[cell->row_pos()].insert_elem(cell);
  }
}

// @brief セルの生成
MincovCell*
MincovMatrix::alloc_cell()
{
  void* p = mCellAlloc.get_memory(sizeof(MincovCell));
  return new (p) MincovCell;
}

// @brief セルの解放
void
MincovMatrix::free_cell(MincovCell* cell)
{
  mCellAlloc.put_memory(sizeof(MincovCell), cell);
}

END_NAMESPACE_YM_MINCOV
