#ifndef LIBYM_UTILS_ITVLCELL_H
#define LIBYM_UTILS_ITVLCELL_H

/// @file libym_utils/ItvlCell.h
/// @brief ItvlCell のヘッダファイル
/// @author Yusuke Matsunaga
///
/// $Id: ItvlCell.h 958 2007-08-28 05:38:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/Alloc.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// インターバルを表す要素クラス
//////////////////////////////////////////////////////////////////////
class ItvlCell
{
private:

  // 空のコンストラクタ
  ItvlCell();

  // 開始点と終了点を指定したコンストラクタ
  ItvlCell(int start,
	   int end);
  
  // デストラクタ
  // 子供も削除する．
  ~ItvlCell();


public:

  // root_ptr 以下の木を削除する
  static
  void
  make_empty(ItvlCell*& root_ptr);

  // 全区間を表すセルを追加する．
  static
  void
  add_allcell(ItvlCell*& root_ptr);

  // 使用可能な数字を得る．
  // 内容は変化しない．
  // 使用可能な区間がない場合(!!!)，-1を返す．
  static
  int
  avail_num(ItvlCell* root_ptr);

  // [d1, d2]という区間が使用可能などうか調べる．
  static
  bool
  check(int d1,
	int d2,
	ItvlCell* root_ptr);

  // 使用されている区間の最小値を求める．
  // 全区間が未使用の場合は -1 を返す．
  static
  int
  min_id(ItvlCell* root_ptr);

  // 使用されている区間の最大値を求める．
  // 全区間が未使用の場合は -1 を返す．
  static
  int
  max_id(ItvlCell* root_ptr);

  // d を使用可能な区間から削除する．
  static
  void
  erase(int d,
	ItvlCell*& root_ptr);

  // [d1, d2] を使用可能な区間から削除する．
  static
  void
  erase(int d1,
	int d2,
	ItvlCell*& root_ptr);

  // d を使用可能区間に追加する．
  static
  void
  add(int d,
      ItvlCell*& root_ptr);

  // [d1, d2] を使用可能区間に追加する．
  static
  void
  add(int d1,
      int d2,
      ItvlCell*& root_ptr);


public:

  // 正しい構造かチェックする．
  int
  check_internal(int& l,
		 int& r) const;

  // ItvlMgr::dump のためのサブルーティン
  void
  dump_sub(ostream& s) const;

  // ItvlMgr::disp_tree() のためのサブルーティン
  void
  disp_tree_sub(ostream& s,
		int level) const;


private:

  // d を含む区間を求める．
  static
  ItvlCell* find(int d,
		 ItvlCell* ptr);

  // d よりも小さくもっとも右側にある区間を求める．
  static
  ItvlCell*
  find_left(int d,
	    ItvlCell* ptr);

  // d よりも大きくもっとも左側にある区間を求める．
  static
  ItvlCell*
  find_right(int d,
	     ItvlCell* ptr);

  // セルを追加するためのサブルーティン．
  static
  bool
  add_cell(ItvlCell* cell,
	   ItvlCell*& ptr);

  // 左の部分木の高さが減少したときの処理
  // 自分自身の高さも減少する時に true を返す．
  static
  bool
  balance_left(ItvlCell*& ptr);

  // 右の部分木の高さが減少したときの処理
  // 自分自身の高さも減少する時に true を返す．
  static
  bool
  balance_right(ItvlCell*& ptr);

  // もっとも右にある節点の内容を cell にコピーして削除する．
  // 木の高さが変化した時には true を返す．
  static
  bool
  delete_right(ItvlCell* cell,
	       ItvlCell*& ptr);

  // delete のためのサブルーティン
  // ptr を根とする部分木から cell を削除する．
  // この部分木の高さが変わった時には true を返す．
  static
  bool
  delete_cell(ItvlCell* cell,
	      ItvlCell*& ptr);

  static
  void*
  operator new(size_t size);

  static
  void
  operator delete(void* ptr,
		  size_t size);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 開始点
  int mStart;

  // 終了点
  int mEnd;

  // 平衡度
  int mBalance;

  // 左の子供
  ItvlCell* mLchd;

  // 右の子供
  ItvlCell* mRchd;


private:
  //////////////////////////////////////////////////////////////////////
  // クラスメンバ
  //////////////////////////////////////////////////////////////////////

  static
  UnitAlloc mCellAlloc;
  
};

END_NAMESPACE_YM

#endif // LIBYM_UTILS_ITVLCELL_H
