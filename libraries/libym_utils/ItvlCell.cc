
/// @file libym_utils/ItvlCell.cc
/// @brief ItvlCell の実装ファイル
/// @author Yusuke Matsunaga
///
/// $Id: ItvlCell.cc 958 2007-08-28 05:38:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ItvlCell.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス ItvlCell の実装
//////////////////////////////////////////////////////////////////////

UnitAlloc ItvlCell::mCellAlloc(sizeof(ItvlCell), 1024);

// コンストラクタ
ItvlCell::ItvlCell() :
  mLchd(NULL),
  mRchd(NULL)
{
}

ItvlCell::ItvlCell(int start,
		   int end) :
  mStart(start),
  mEnd(end),
  mLchd(NULL),
  mRchd(NULL)
{
}

// デストラクタ
ItvlCell::~ItvlCell()
{
  delete mLchd;
  delete mRchd;
}

// root_ptr 以下の木を削除する
void
ItvlCell::make_empty(ItvlCell*& root_ptr)
{
  delete root_ptr;
  root_ptr = NULL;
}

// 全区間を表すセルを追加する．
void
ItvlCell::add_allcell(ItvlCell*& root_ptr)
{
  make_empty(root_ptr);
  ItvlCell* cell = new ItvlCell(0, INT_MAX);
  add_cell(cell, root_ptr);
}

// 使用可能な数字を得る．
// 内容は変化しない．
// 使用可能な区間がない場合(!!!)，-1を返す．
int
ItvlCell::avail_num(ItvlCell* root_ptr)
{
  ItvlCell* cell = root_ptr;
  if ( !cell ) {
    return -1;
  }

  while ( cell->mLchd ) {
    cell = cell->mLchd;
  }
  return cell->mStart;
}

// [d1, d2]という区間が使用可能などうか調べる．
bool
ItvlCell::check(int d1,
		int d2,
		ItvlCell* root_ptr)
{
  // d1 <= d2 でなければ入れ換える．
  if ( d1 > d2 ) {
    int tmp = d1;
    d1 = d2;
    d2 = tmp;
  }

  // d1 を含む区間を探す．
  ItvlCell* cell = find(d1, root_ptr);
  if ( !cell ) {
    // だめ．
    return false;
  }
  // この区間が d2 を含んでいるか調べる．
  if ( cell->mEnd >= d2 ) {
    return true;
  }
  return false;
}

// 使用されている区間の最小値を求める．
// 全区間が未使用の場合は -1 を返す．
int
ItvlCell::min_id(ItvlCell* root_ptr)
{
  ItvlCell* cell;
  for (cell = root_ptr; cell->mLchd; cell = cell->mLchd) { }
  // cell は最左節点
  if ( cell->mStart > 0 ) {
    return 0;
  }
  else if ( cell->mEnd < INT_MAX ) {
    return cell->mEnd + 1;
  }
  else {
    return -1;
  }
}

// 使用されている区間の最大値を求める．
// 全区間が未使用の場合は -1 を返す．
int
ItvlCell::max_id(ItvlCell* root_ptr)
{
  ItvlCell* cell;
  for (cell = root_ptr; cell->mRchd; cell = cell->mRchd) { }
  // cell は最右節点
  if ( cell->mEnd < INT_MAX ) {
    // これはあり得ないと思うけど
    return INT_MAX;
  }
  else if ( cell->mStart > 0 ) {
    return cell->mStart - 1;;
  }
  else {
    return -1;
  }
}

// d を含む区間を求める．
ItvlCell*
ItvlCell::find(int d,
	       ItvlCell* ptr)
{
  while ( ptr ) {
    if ( ptr->mStart > d ) {
      ptr = ptr->mLchd;
    }
    else if ( ptr->mEnd < d ) {
      ptr = ptr->mRchd;
    }
    else {
      break;
    }
  }
  return ptr;
}

// d よりも小さくもっとも右側にある区間を求める．
ItvlCell*
ItvlCell::find_left(int d,
		    ItvlCell* ptr)
{
  while ( ptr && ptr->mStart > d ) {
    ptr = ptr->mLchd;
  }
  if ( !ptr ) {
    return 0;
  }
  if ( ptr->mEnd < d ) {
    while ( ptr->mRchd && ptr->mRchd->mEnd < d ) {
      ptr = ptr->mRchd;
    }
  }
  return ptr;
}

// d よりも大きくもっとも左側にある区間を求める．
ItvlCell*
ItvlCell::find_right(int d,
		     ItvlCell* ptr)
{
  while ( ptr && ptr->mEnd < d ) {
    ptr = ptr->mRchd;
  }
  if ( !ptr ) {
    return 0;
  }
  if ( ptr->mStart > d ) {
    while ( ptr->mLchd && ptr->mLchd->mStart > d ) {
      ptr = ptr->mLchd;
    }
  }
  return ptr;
}

// セルを追加するためのサブルーティン．
bool
ItvlCell::add_cell(ItvlCell* cell,
		   ItvlCell*& ptr)
{
  if ( ptr == NULL ) {
    ptr = cell;
    ptr->mBalance = 0;
    return true;
  }
  if ( ptr->mStart > cell->mEnd ) {
    bool chg = add_cell(cell, ptr->mLchd);
    if ( chg ) {
      // 左の部分木が高くなった．
      switch ( ptr->mBalance ) {
      case 1: // 今まで左が低かったのでこれでバランスした．
	ptr->mBalance = 0;
	return false;
      case 0: // 左が高くなった．
	ptr->mBalance = -1;
	return true;
      case -1: // 再平衡化が必要
	{
	  ItvlCell* left = ptr->mLchd;
	  if ( left->mBalance == -1 ) {
	    // 単LL回転
	    ptr->mLchd = left->mRchd;
	    left->mRchd = ptr;
	    ptr->mBalance = 0;
	    ptr = left;
	  }
	  else {
	    // 二重LR回転
	    ItvlCell* right = left->mRchd;
	    left->mRchd = right->mLchd;
	    right->mLchd = left;
	    ptr->mLchd = right->mRchd;
	    right->mRchd = ptr;
	    if ( right->mBalance == -1 ) {
	      ptr->mBalance = 1;
	    }
	    else {
	      ptr->mBalance = 0;
	    }
	    if ( right->mBalance == 1 ) {
	      left->mBalance = -1;
	    }
	    else {
	      left->mBalance = 0;
	    }
	    ptr = right;
	  }
	  ptr->mBalance = 0;
	  return false;
	}
      }
    }
  }
  else if ( ptr->mEnd < cell->mStart ) {
    bool chg = add_cell(cell, ptr->mRchd);
    if ( chg ) {
      // 右の部分木が高くなった．
      switch ( ptr->mBalance ) {
      case -1: // 今まで右が低かったのでバランスした．
	ptr->mBalance = 0;
	return false;
      case 0: // 右が高くなった．(全体も高くなった．)
	ptr->mBalance = 1;
	return true;
      case 1: // 再平衡化が必要
	{
	  ItvlCell* right = ptr->mRchd;
	  if ( right->mBalance == 1 ) {
	    // 単RR回転
	    ptr->mRchd = right->mLchd;
	    right->mLchd = ptr;
	    ptr->mBalance = 0;
	    ptr = right;
	  }
	  else {
	    // 二重RL回転
	    ItvlCell* left = right->mLchd;
	    right->mLchd = left->mRchd;
	    left->mRchd = right;
	    ptr->mRchd = left->mLchd;
	    left->mLchd = ptr;
	    if ( left->mBalance == 1 ) {
	      ptr->mBalance = -1;
	    }
	    else {
	      ptr->mBalance = 0;
	    }
	    if ( left->mBalance == -1 ) {
	      right->mBalance = 1;
	    }
	    else {
	      right->mBalance = 0;
	    }
	    ptr = left;
	  }
	  ptr->mBalance = 0;
	  return false;
	}
      }
    }
  }
  else {
    // 重複した追加は違反
    assert_not_reached(__FILE__, __LINE__);
  }
  return false;
}

// 左の部分木の高さが減少したときの処理
// 自分自身の高さも減少する時に true を返す．
bool
ItvlCell::balance_left(ItvlCell*& ptr)
{
  bool chg = true;

  switch ( ptr->mBalance ) {
  case -1: // もともと左が高かったのでバランスする．
    ptr->mBalance = 0;
    // でも高さは減少する．
    chg = true;
    break;
  case 0: // もともとバランスしていたので左が低くなる．
    ptr->mBalance = 1;
    // 高さは変わらないので false を返す．
    chg = false;
    break;
  case 1: // 再平衡化を行う．
    {
      ItvlCell* right = ptr->mRchd;
      int br = right->mBalance;
      if ( br >= 0 ) {
	// 単RR回転
	ptr->mRchd = right->mLchd;
	right->mLchd = ptr;
	if ( br == 0 ) {
	  ptr->mBalance = 1;
	  right->mBalance = -1;
	  chg = false;
	}
	else {
	  ptr->mBalance = 0;
	  right->mBalance = 0;
	}
	ptr = right;
      }
      else {
	// 二重RL回転
	ItvlCell* left = right->mLchd;
	int bl = left->mBalance;
	right->mLchd = left->mRchd;
	left->mRchd = right;
	ptr->mRchd = left->mLchd;
	left->mLchd = ptr;
	if ( bl == 1 ) {
	  ptr->mBalance = -1;
	}
	else {
	  ptr->mBalance = 0;
	}
	if ( bl == -1 ) {
	  right->mBalance = 1;
	}
	else {
	  right->mBalance = 0;
	}
	ptr = left;
	ptr->mBalance = 0;
      }
    }
  }
  return chg;
}

// 右の部分木の高さが減少したときの処理
// 自分自身の高さも減少する時に true を返す．
bool
ItvlCell::balance_right(ItvlCell*& ptr)
{
  bool chg = true;

  switch ( ptr->mBalance ) {
  case 1: // もともと右が高かったのでバランスする．
    ptr->mBalance = 0;
    // でも高さは減少する．
    break;
  case 0: // もともとバランスしていたので右が低くなる．
    ptr->mBalance = -1;
    // 高さは変わらないので chg フラグを降ろす．
    chg = false;
    break;
  case -1: // 再平衡化を行う．
    {
      ItvlCell* left = ptr->mLchd;
      int bl = left->mBalance;
      if ( bl <= 0 ) {
	// 単LL回転
	ptr->mLchd = left->mRchd;
	left->mRchd = ptr;
	if ( bl == 0 ) {
	  ptr->mBalance = -1;
	  left->mBalance = 1;
	  chg = false;
	}
	else {
	  ptr->mBalance = 0;
	  left->mBalance = 0;
	}
	ptr = left;
      }
      else {
	// 二重LR回転
	ItvlCell* right = left->mRchd;
	int br = right->mBalance;
	left->mRchd = right->mLchd;
	right->mLchd = left;
	ptr->mLchd = right->mRchd;
	right->mRchd = ptr;
	if ( br == -1 ) {
	  ptr->mBalance = 1;
	}
	else {
	  ptr->mBalance = 0;
	}
	if ( br == 1 ) {
	  left->mBalance = -1;
	}
	else {
	  left->mBalance = 0;
	}
	ptr = right;
	ptr->mBalance = 0;
      }
    }
  }
  return chg;
}

// もっとも右にある節点の内容を cell にコピーして削除する．
// 木の高さが変化した時には true を返す．
bool
ItvlCell::delete_right(ItvlCell* cell,
		       ItvlCell*& ptr)
{
  if ( ptr->mRchd ) {
    // まだ右に子供がいる
    bool chg = delete_right(cell, ptr->mRchd);
    if ( chg ) {
      chg = balance_right(ptr);
    }
    return chg;
  }

  // ptr が最右端の節点を指している．
  cell->mStart = ptr->mStart;
  cell->mEnd = ptr->mEnd;
  ItvlCell* dcell = ptr;
  ptr = ptr->mLchd;
  dcell->mLchd = 0;
  delete dcell;
  return true;
}

// delete のためのサブルーティン
// ptr を根とする部分木から cell を削除する．
// この部分木の高さが変わった時には true を返す．
bool
ItvlCell::delete_cell(ItvlCell* cell,
		      ItvlCell*& ptr)
{
  if ( ptr == NULL ) {
    // セルが存在しない?
    // 本当はエラーだけど無視しちゃう．
    return false;
  }

  if ( ptr->mStart > cell->mEnd ) {
    bool chg = delete_cell(cell, ptr->mLchd);
    if ( chg ) {
      chg = balance_left(ptr);
    }
    return chg;
  }
  if ( ptr->mEnd < cell->mStart ) {
    bool chg = delete_cell(cell, ptr->mRchd);
    if ( chg ) {
      chg = balance_right(ptr);
    }
    return chg;
  }

  // この場合，ptr と cell の区間がオーバーラップしているので
  // ptr == cell でないとおかしい．
  assert_cond( ptr == cell, __FILE__, __LINE__);
  bool chg = false;
  if ( !ptr->mLchd ) {
    // 右の子供しか持たない場合
    ptr = cell->mRchd;
    cell->mRchd = 0;
    delete cell;
    chg = true;
  }
  else if ( !ptr->mRchd  ) {
    // 左の子供しか持たない場合
    ptr = cell->mLchd;
    cell->mLchd = 0;
    delete cell;
    chg = true;
  }
  else {
    // 二人の子供を持つ場合．
    // 左の部分木のもっとも右にある節点で置き換える．
    // この場合 cell は削除してはいけない．
    chg = delete_right(cell, ptr->mLchd);
    if ( chg ) {
      chg = balance_left(ptr);
    }
  }
  return chg;
}

// d を使用可能な区間から削除する．
void
ItvlCell::erase(int d,
		ItvlCell*& root_ptr)
{
  // d を含む区間を探す．
  ItvlCell* cell = find(d, root_ptr);
  if ( cell == NULL ) {
    // もともと入っていない？
    // ほんとはおかしいけど，無視しておこう．
    return;
  }

  if ( cell->mStart == cell->mEnd ) {
    // この場合はこの要素を削除すれば良い．
    delete_cell(cell, root_ptr);
  }
  else {
    if ( cell->mStart == d ) {
      ++ cell->mStart;
    }
    else if ( cell->mEnd == d ) {
      -- cell->mEnd;
    }
    else {
      // 2つの区間に分割
      ItvlCell* cell2 = new ItvlCell(d + 1, cell->mEnd);
      cell->mEnd = d - 1;
      add_cell(cell2, root_ptr);
    }
  }
}

// [d1, d2] を使用可能な区間から削除する．
void
ItvlCell::erase(int d1,
		int d2,
		ItvlCell*& root_ptr)
{
  // d1 <= d2 でなければ入れ換える．
  if (d1 > d2) {
    int tmp = d1;
    d1 = d2;
    d2 = tmp;
  }

  // d1 を含む区間を探す．
  ItvlCell* cell = find(d1, root_ptr);
  if ( !cell || cell->mEnd < d2 ) {
    // もともと入っていない？
    // ほんとはおかしいけど，無視しておこう．
    return;
  }

  if ( cell->mStart == d1 && cell->mEnd == d2 ) {
    // この場合はこの要素を削除すれば良い．
    delete_cell(cell, root_ptr);
  }
  else {
    if ( cell->mStart == d1 ) {
      ++ cell->mStart;
    }
    else if ( cell->mEnd == d2 ) {
      -- cell->mEnd;
    }
    else {
      // 2つの区間に分割
      ItvlCell* cell2 = new ItvlCell(d2 + 1, cell->mEnd);
      cell->mEnd = d1 - 1;
      add_cell(cell2, root_ptr);
    }
  }
}

// d を使用可能区間に追加する．
void
ItvlCell::add(int d,
	      ItvlCell*& root_ptr)
{
  // d に隣接する区間を探す．
  ItvlCell* left = find_left(d, root_ptr);
  ItvlCell* right = find_right(d, root_ptr);

  // ちょっとしたhack
  int start = d + 2;
  int end = d - 2;
  if ( left ) {
    end = left->mEnd;
  }
  if ( right ) {
    start = right->mStart;
  }

  assert_cond(end < d, __FILE__, __LINE__);
  assert_cond(start > d, __FILE__, __LINE__);

  if ( end + 2 == start ) {
    // 2つの区間が d で一つにつながる．
    int end2 = right->mEnd;
    delete_cell(right, root_ptr);
    // left の節点は置き換わっている可能性があるのでもう一度取り出す．
    left = find_left(d, root_ptr);
    left->mEnd = end2;
  }
  else if ( end + 1 == d ) {
    ++ left->mEnd;
  }
  else if ( start - 1 == d ) {
    -- right->mStart;
  }
  else {
    // 新しい区間を作る．
    ItvlCell* cell = new ItvlCell(d, d);
    add_cell(cell, root_ptr);
  }
}

// [d1, d2] を使用可能区間に追加する．
void
ItvlCell::add(int d1,
	      int d2,
	      ItvlCell*& root_ptr)
{
  // d1 に隣接する区間を探す．
  ItvlCell* left = find_left(d1, root_ptr);
  // d2 に隣接する区間を探す．
  ItvlCell* right = find_right(d2, root_ptr);
  // left と right の間に他の区間が入っていたらおかしいので何もしないで
  // 抜ける．
  if ( find_right(d1, root_ptr) != right ) {
    // 今は何もしないで戻る．
    return;
  }

  // ちょっとしたhack
  int start = d2 + 2;
  int end = d1 - 2;
  if ( left ) {
    end = left->mEnd;
  }
  if ( right ) {
    start = right->mStart;
  }

  assert_cond(end < d1, __FILE__, __LINE__);
  assert_cond(start > d2, __FILE__, __LINE__);

  if ( end + 1 == d1 && start - 1 == d2 ) {
    // 2つの区間が [d1, d2] で一つにつながる．
    int end2 = right->mEnd;
    delete_cell(right, root_ptr);
    // left の節点は置き換わっている可能性があるのでもう一度取り出す．
    left = find_left(d1, root_ptr);
    left->mEnd = end2;
  }
  else if ( end + 1 == d1 ) {
    ++ left->mEnd;
  }
  else if ( start - 1 == d2 ) {
    -- right->mStart;
  }
  else {
    // 新しい区間を作る．
    ItvlCell* cell = new ItvlCell(d1, d2);
    add_cell(cell, root_ptr);
  }
}

void*
ItvlCell::operator new(size_t size)
{
  return mCellAlloc.get_memory(size);
}

void
ItvlCell::operator delete(void* ptr,
			  size_t size)
{
  mCellAlloc.put_memory(sizeof(ItvlCell), ptr);
}

// 正しい構造かチェックする．
int
ItvlCell::check_internal(int& l,
			 int& r) const
{
  if ( !this ) {
    return 0;
  }
  int l1, r1;
  int hl = mLchd->check_internal(l1, r1);
  int l2, r2;
  int hr = mRchd->check_internal(l2, r2);
  switch ( mBalance ) {
  case 1: // 右が高いはず
    assert_cond(hl < hr, __FILE__, __LINE__);
    break;
  case -1: // 左が高いはず
    assert_cond(hl > hr, __FILE__, __LINE__);
    break;
  case 0: // 高さは等しいはず
    assert_cond(hl == hr, __FILE__, __LINE__);
    break;
  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
  if ( mLchd ) {
    assert_cond(r1 < mStart, __FILE__, __LINE__);
    l = l1;
  }
  else {
    l = mStart;
  }
  if ( mRchd ) {
    assert_cond(l2 > mEnd, __FILE__, __LINE__);
    r = r2;
  }
  else {
    r = mEnd;
  }
  int h = hl;
  if ( h < hr ) {
    h = hr;
  }
  return h + 1;
}

// ItvlMgr::dump のためのサブルーティン
void
ItvlCell::dump_sub(ostream& s) const
{
  if ( mLchd ) {
    mLchd->dump_sub(s);
  }
  s << " " << mStart << " - " << mEnd << endl;
  if ( mRchd ) {
    mRchd->dump_sub(s);
  }
}

// ItvlMgr::disp_tree() のためのサブルーティン
void
ItvlCell::disp_tree_sub(ostream& s,
			int level) const
{
  if ( mLchd ) {
    mLchd->disp_tree_sub(s, level + 1);
  }
  for (int i = 0; i < level; i ++) {
    s << "    ";
  }
  s << " [" << mStart << " - " << mEnd << "]";
  switch ( mBalance ) {
  case -1: s << ">"; break;
  case 0:  s << "="; break;
  case 1:  s << "<"; break;
  }
  s << endl;
  if ( mRchd ) {
    mRchd->disp_tree_sub(s, level + 1);
  }
}

END_NAMESPACE_YM
