
/// @file libym_utils/ItvlMgr.cc
/// @brief ItvlMgrの実装ファイル
/// @author Yusuke Matsunaga
///
/// $Id: ItvlMgr.cc 958 2007-08-28 05:38:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/ItvlMgr.h"
#include "ItvlCell.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス ItvlMgr
//////////////////////////////////////////////////////////////////////

// コンストラクタ
ItvlMgr::ItvlMgr() :
  mRoot(NULL)
{
  clear();
}

// デストラクタ
ItvlMgr::~ItvlMgr()
{
  ItvlCell::make_empty(mRoot);
}

// 内容をクリアして全区間を使用可能とする．
void
ItvlMgr::clear()
{
  ItvlCell::add_allcell(mRoot);
}

// 使用可能な数字を得る．
// 内容は変化しない．
// 使用可能な区間がない場合(!!!)，-1を返す．
int
ItvlMgr::avail_num() const
{
  return ItvlCell::avail_num(mRoot);
}

// [d1, d2]という区間が使用可能などうか調べる．
bool
ItvlMgr::check(int d1,
	       int d2) const
{
  return ItvlCell::check(d1, d2, mRoot);
}

// 使用されている区間の最小値を求める．
// 全区間が未使用の場合は -1 を返す．
int
ItvlMgr::min_id() const
{
  return ItvlCell::min_id(mRoot);
}

// 使用されている区間の最大値を求める．
// 全区間が未使用の場合は -1 を返す．
int
ItvlMgr::max_id() const
{
  return ItvlCell::max_id(mRoot);
}

// d を使用可能な区間から削除する．
void
ItvlMgr::erase(int d)
{
  ItvlCell::erase(d, mRoot);
  sanity_check();
}

// [d1, d2] を使用可能な区間から削除する．
void
ItvlMgr::erase(int d1,
	       int d2)
{
  ItvlCell::erase(d1, d2, mRoot);
  sanity_check();
}

// d を使用可能区間に追加する．
void
ItvlMgr::add(int d)
{
  ItvlCell::add(d, mRoot);
  sanity_check();
}

// [d1, d2] を使用可能区間に追加する．
void
ItvlMgr::add(int d1,
	     int d2)
{
  ItvlCell::add(d1, d2, mRoot);
  sanity_check();
}

// 正しい構造かチェックする．
void
ItvlMgr::sanity_check() const
{
#ifdef DEBUG_ITVLMGR
  int d1, d2; // ダミー変数
  mRoot->check_internal(d1, d2);
#endif
}

// 内容を表示する．
void
ItvlMgr::dump(ostream& s) const
{
  if ( mRoot ) {
    mRoot->dump_sub(s);
  }
}

// 木構造を表示する．
void
ItvlMgr::disp_tree(ostream& s) const
{
  if ( mRoot ) {
    mRoot->disp_tree_sub(s, 0);
  }
}

END_NAMESPACE_YM
