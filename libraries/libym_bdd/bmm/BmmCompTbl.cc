
/// @file libym_bdd/bmm/BmmCompTbl.cc
/// @brief BmmCompTbl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BmmCompTbl.cc 700 2007-05-31 00:41:30Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include "BmmCompTbl.h"

BEGIN_NONAMESPACE
const size_t kInitSize = (1 << 10);
const double kLoadLimit = 0.8;
const size_t kMaxSize = (1 << 20);
END_NONAMESPACE


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
// BmmCompTbl
//////////////////////////////////////////////////////////////////////

// コンストラクタ
BmmCompTbl::BmmCompTbl(BddMgrModern* mgr,
		       const char* name) :
  mMgr(mgr)
{
  if ( name == 0 ) {
    // 適当な名前を付ける．
    static int num = 1;
    ostringstream s;
    s << "table#" << num ++;
    mName = s.str();
  }
  else {
    mName = name;
  }

  mLoadLimit = kLoadLimit;
  mMaxSize = kMaxSize;
  mTableSize = 0;

  // リストに追加する．
  mMgr->add_table(this);
}

// デストラクタ
BmmCompTbl::~BmmCompTbl()
{
}

// テーブルサイズを取り出す．
size_t
BmmCompTbl::table_size() const
{
  return mTableSize;
}

// next_limit を再計算する．
void
BmmCompTbl::update_next_limit()
{
  mNextLimit = size_t(double(mTableSize) * mLoadLimit);
}

// 使用されているセル数を取り出す．
size_t
BmmCompTbl::used_num() const
{
  return mUsedNum;
}

// load_limitの(再)設定を行なう．
void
BmmCompTbl::load_limit(double load_limit)
{
  mLoadLimit = load_limit;
}

// 最大のテーブルサイズの設定を行なう．
// 現在，このサイズを越えているテーブルがあっても縮小するような
// ことはしない．
void
BmmCompTbl::max_size(size_t max_size)
{
  mMaxSize = max_size;
}

// BddMgrModern からメモリを獲得する．
void*
BmmCompTbl::allocate(size_t size)
{
  return mMgr->allocate(size);
}

// BddMgrModern にメモリを返す．
void
BmmCompTbl::deallocate(void* ptr,
		       size_t size)
{
  mMgr->deallocate(ptr, size);
}

// ログ出力用のストリームを得る．
ostream&
BmmCompTbl::logstream() const
{
  return mMgr->logstream();
}


//////////////////////////////////////////////////////////////////////
// BmmCompTbl1
//////////////////////////////////////////////////////////////////////

// コンストラクタ
BmmCompTbl1::BmmCompTbl1(BddMgrModern* mgr,
			 const char* name) :
  BmmCompTbl(mgr, name)
{
  resize(kInitSize);
}

// デストラクタ
BmmCompTbl1::~BmmCompTbl1()
{
  deallocate((void*)mTable, mTableSize * sizeof(Cell));
}

// テーブルサイズを変更する．
void
BmmCompTbl1::resize(size_t new_size)
{
  // ログの出力
  logstream() << "BmmCompTbl1[" << mName << "]::resize(" << new_size << ")"
	      << endl;

  // 昔の値を保存する．
  size_t old_size = mTableSize;
  Cell* old_table = mTable;

  // 新たなサイズを設定し，テーブルを確保する．
  mTable = (Cell*)allocate(new_size * sizeof(Cell));
  mTableSize = new_size;
  mTableSize_1 = mTableSize - 1;
  update_next_limit();

  // 中身をクリアしておく．
  Cell* top = mTable;
  Cell* end = top + mTableSize;
  do {
    top->mKey1 = kEdgeInvalid;
    top ++;
  } while ( top != end );

  // ハッシュし直して新しいテーブルに登録する．
  mUsedNum = 0;
  if ( old_size > 0 ) {
    Cell* top = old_table;
    Cell* end = top + old_size;
    do {
      if ( top->mKey1 != kEdgeInvalid ) {
	size_t pos = hash_func(top->mKey1);
	Cell* temp = mTable + pos;
	if ( temp->mKey1 == kEdgeInvalid ) mUsedNum ++;
	temp->mKey1 = top->mKey1;
	temp->mAns = top->mAns;
      }
      top ++;
    } while ( top != end );
    deallocate((void*)old_table, old_size * sizeof(Cell));
  }
}

// BddMgrModern::GC()に対応する．
// 具体的には GC で削除されるノードに関連したセルをクリアする．
void
BmmCompTbl1::sweep()
{
  // ログを出力
  logstream() << "BmmCompTbl1[" << mName << "]::sweep()" << endl;

  // 削除されるノードに関連したセルをクリアする．
  Cell* cell = mTable;
  Cell* end = cell + mTableSize;
  for ( ; cell != end; cell ++) {
    if ( cell->mKey1 != kEdgeInvalid &&
	 (check_noref(cell->mKey1) ||
	  check_noref(cell->mAns)) ) {
      cell->mKey1 = kEdgeInvalid;
      mUsedNum --;
    }
  }
}

// クリアする．
void
BmmCompTbl1::clear()
{
  Cell* cell = mTable;
  Cell* end = cell + mTableSize;
  for ( ; cell != end; cell ++) {
    cell->mKey1 = kEdgeInvalid;
    mUsedNum --;
  }
}


//////////////////////////////////////////////////////////////////////
// BmmCompTbl2
//////////////////////////////////////////////////////////////////////

// コンストラクタ
BmmCompTbl2::BmmCompTbl2(BddMgrModern* mgr,
			 const char* name) :
  BmmCompTbl(mgr, name)
{
  resize(kInitSize);
}

// デストラクタ
BmmCompTbl2::~BmmCompTbl2()
{
  deallocate((void*)mTable, mTableSize * sizeof(Cell));
}

// テーブルサイズを変更する．
void
BmmCompTbl2::resize(size_t new_size)
{
  // ログの出力
  logstream() << "BmmCompTbl2[" << mName << "]::resize(" << new_size << ")"
	      << endl;

  // 昔の値を保存する．
  size_t old_size = mTableSize;
  Cell* old_table = mTable;

  // 新たなサイズを設定し，テーブルを確保する．
  mTable = (Cell*)allocate(new_size * sizeof(Cell));
  mTableSize = new_size;
  mTableSize_1 = mTableSize - 1;
  update_next_limit();

  // 中身をクリアしておく．
  Cell* top = mTable;
  Cell* end = top + mTableSize;
  do {
    top->mKey1 = kEdgeInvalid;
    top ++;
  } while ( top != end );

  // ハッシュし直して新しいテーブルに登録する．
  mUsedNum = 0;
  if ( old_size > 0 ) {
    Cell* top = old_table;
    Cell* end = top + old_size;
    do {
      if ( top->mKey1 != kEdgeInvalid ) {
	size_t pos = hash_func(top->mKey1, top->mKey2);
	Cell* temp = mTable + pos;
	if ( temp->mKey1 == kEdgeInvalid ) mUsedNum ++;
	temp->mKey1 = top->mKey1;
	temp->mKey2 = top->mKey2;
	temp->mAns = top->mAns;
      }
      top ++;
    } while ( top != end );
    deallocate((void*)old_table, old_size * sizeof(Cell));
  }
}

// BddMgrModern::GC()に対応する．
// 具体的には GC で削除されるノードに関連したセルをクリアする．
void
BmmCompTbl2::sweep()
{
  // ログを出力
  logstream() << "BmmCompTbl2[" << mName << "]::sweep()" << endl;

  // 削除されるノードに関連したセルをクリアする．
  Cell* cell = mTable;
  Cell* end = cell + mTableSize;
  for ( ; cell != end; cell ++) {
    if ( cell->mKey1 != kEdgeInvalid &&
	 (check_noref(cell->mKey1) ||
	  check_noref(cell->mKey2) ||
	  check_noref(cell->mAns)) ) {
      cell->mKey1 = kEdgeInvalid;
      mUsedNum --;
    }
  }
}

// クリアする．
void
BmmCompTbl2::clear()
{
  Cell* cell = mTable;
  Cell* end = cell + mTableSize;
  for ( ; cell != end; cell ++) {
    cell->mKey1 = kEdgeInvalid;
    mUsedNum --;
  }
}


//////////////////////////////////////////////////////////////////////
// BmmCompTbl3
//////////////////////////////////////////////////////////////////////

// コンストラクタ
BmmCompTbl3::BmmCompTbl3(BddMgrModern* mgr,
			 const char* name) :
  BmmCompTbl(mgr, name)
{
  resize(kInitSize);
}

// デストラクタ
BmmCompTbl3::~BmmCompTbl3()
{
  deallocate((void*)mTable, mTableSize * sizeof(Cell));
}

// テーブルサイズを変更する．
void
BmmCompTbl3::resize(size_t new_size)
{
  // ログの出力
  logstream() << "BmmCompTbl3[" << mName << "]::resize(" << new_size << ")"
	      << endl;

  // 昔の値を保存する．
  size_t old_size = mTableSize;
  Cell* old_table = mTable;

  // 新たなサイズを設定し，テーブルを確保する．
  mTable = (Cell*)allocate(new_size * sizeof(Cell));
  mTableSize = new_size;
  mTableSize_1 = mTableSize - 1;
  update_next_limit();

  // 中身をクリアしておく．
  Cell* top = mTable;
  Cell* end = top + mTableSize;
  do {
    top->mKey1 = kEdgeInvalid;
    top ++;
  } while ( top != end );

  // ハッシュし直して新しいテーブルに登録する．
  mUsedNum = 0;
  if ( old_size > 0 ) {
    Cell* top = old_table;
    Cell* end = top + old_size;
    do {
      if ( top->mKey1 != kEdgeInvalid ) {
	size_t pos = hash_func(top->mKey1, top->mKey2, top->mKey3);
	Cell* temp = mTable + pos;
	if ( temp->mKey1 == kEdgeInvalid ) mUsedNum ++;
	temp->mKey1 = top->mKey1;
	temp->mKey2 = top->mKey2;
	temp->mKey3 = top->mKey3;
	temp->mAns = top->mAns;
      }
      top ++;
    } while ( top != end );
    deallocate((void*)old_table, old_size * sizeof(Cell));
  }
}

// BddMgrModern::GC()に対応する．
// 具体的には GC で削除されるノードに関連したセルをクリアする．
void
BmmCompTbl3::sweep()
{
  // ログを出力
  logstream() << "BmmCompTbl3[" << mName << "]::sweep()" << endl;

  // 削除されるノードに関連したセルをクリアする．
  Cell* cell = mTable;
  Cell* end = cell + mTableSize;
  for ( ; cell != end; cell ++) {
    if ( cell->mKey1 != kEdgeInvalid &&
	 (check_noref(cell->mKey1) ||
	  check_noref(cell->mKey2) ||
	  check_noref(cell->mKey3) ||
	  check_noref(cell->mAns)) ) {
      cell->mKey1 = kEdgeInvalid;
      mUsedNum --;
    }
  }
}

// クリアする．
void
BmmCompTbl3::clear()
{
  Cell* cell = mTable;
  Cell* end = cell + mTableSize;
  for ( ; cell != end; cell ++) {
    cell->mKey1 = kEdgeInvalid;
    mUsedNum --;
  }
}


//////////////////////////////////////////////////////////////////////
// BmmIsopTbl
//////////////////////////////////////////////////////////////////////

// コンストラクタ
BmmIsopTbl::BmmIsopTbl(BddMgrModern* mgr,
		       const char* name) :
  BmmCompTbl(mgr, name)
{
  resize(kInitSize);
}

// デストラクタ
BmmIsopTbl::~BmmIsopTbl()
{
  clear();
  deallocate((void*)mTable, mTableSize * sizeof(Cell));
}

// テーブルサイズを変更する．
void
BmmIsopTbl::resize(size_t new_size)
{
  // ログの出力
  logstream() << "BmmIsopTbl[" << mName << "]::resize(" << new_size << ")"
	      << endl;

  // 昔の値を保存する．
  size_t old_size = mTableSize;
  Cell* old_table = mTable;

  // 新たなサイズを設定し，テーブルを確保する．
  mTable = (Cell*)allocate(new_size * sizeof(Cell));
  mTableSize = new_size;
  mTableSize_1 = mTableSize - 1;
  update_next_limit();

  // 中身をクリアしておく．
  Cell* top = mTable;
  Cell* end = top + mTableSize;
  do {
    top->mKey1 = kEdgeInvalid;
    top->mAnsCov = 0;
    top ++;
  } while ( top != end );

  // ハッシュし直して新しいテーブルに登録する．
  mUsedNum = 0;
  if ( old_size > 0 ) {
    Cell* top = old_table;
    Cell* end = top + old_size;
    do {
      if ( top->mKey1 != kEdgeInvalid ) {
	size_t pos = hash_func(top->mKey1, top->mKey2);
	Cell* temp = mTable + pos;
	if ( temp->mKey1 == kEdgeInvalid ) mUsedNum ++;
	temp->mKey1 = top->mKey1;
	temp->mKey2 = top->mKey2;
	temp->mAnsBdd = top->mAnsBdd;
	delete temp->mAnsCov;
	temp->mAnsCov = top->mAnsCov;
      }
      top ++;
    } while ( top != end );
    deallocate((void*)old_table, old_size * sizeof(Cell));
  }
}

// BddMgrModern::GC()に対応する．
// 具体的には GC で削除されるノードに関連したセルをクリアする．
void
BmmIsopTbl::sweep()
{
  // ログを出力
  logstream() << "BmmIsopTbl[" << mName << "]::sweep()" << endl;

  // 削除されるノードに関連したセルをクリアする．
  Cell* cell = mTable;
  Cell* end = cell + mTableSize;
  for ( ; cell != end; cell ++) {
    if ( cell->mKey1 != kEdgeInvalid &&
	 (check_noref(cell->mKey1) ||
	  check_noref(cell->mKey2) ||
	  check_noref(cell->mAnsBdd)) ) {
      cell->mKey1 = kEdgeInvalid;
      delete cell->mAnsCov;
      cell->mAnsCov = 0;
      mUsedNum --;
    }
  }
}

// クリアする．
void
BmmIsopTbl::clear()
{
  Cell* cell = mTable;
  Cell* end = cell + mTableSize;
  for ( ; cell != end; cell ++) {
    cell->mKey1 = kEdgeInvalid;
    delete cell->mAnsCov;
    cell->mAnsCov = 0;
    mUsedNum --;
  }
}

END_NAMESPACE_YM_BDD
