#ifndef YM_MINCOV_MINCOVMATRIX_H
#define YM_MINCOV_MINCOVMATRIX_H

/// @file ym_mincov/MincovMatrix.h
/// @brief MincovMatrix のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: MincovMatrix.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mincov/mincov_nsdef.h"
#include "ym_utils/Alloc.h"


BEGIN_NAMESPACE_YM_MINCOV

class MincovCost;

//////////////////////////////////////////////////////////////////////
/// @class MincovCell MincovMatrix.h <ym_mincov/MincovMatrix.h>
/// @brief Mincov 用の行列要素を表すクラス
//////////////////////////////////////////////////////////////////////
class MincovCell
{
  friend class MincovRowHead;
  friend class MincovColHead;
  friend class MincovMatrix;
  
private:

  /// @brief コンストラクタ
  MincovCell();

  /// @brief デストラクタ
  ~MincovCell();

  
public:

  /// @brief 行番号を返す．
  ymuint32
  row_pos() const;

  /// @brief 列番号を返す．
  ymuint32
  col_pos() const;
  
  /// @brief 同じ行の直前の要素を返す．
  const MincovCell*
  row_prev() const;

  /// @brief 同じ行の直後の要素を返す．
  const MincovCell*
  row_next() const;

  /// @brief 同じ列の直前の要素を返す．
  const MincovCell*
  col_prev() const;

  /// @brief 同じ列の直後の要素を返す．
  const MincovCell*
  col_next() const;
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 行番号
  ymuint32 mRowPos;

  // 列番号
  ymuint32 mColPos;

  // 左隣の要素
  MincovCell* mLeftLink;

  // 右隣の要素
  MincovCell* mRightLink;

  // 上の要素
  MincovCell* mUpLink;

  // 下の要素
  MincovCell* mDownLink;
  
};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 行番号を返す．
inline
ymuint32
MincovCell::row_pos() const
{
  return mRowPos;
}

// @brief 列番号を返す．
inline
ymuint32
MincovCell::col_pos() const
{
  return mColPos;
}
  
// @brief 同じ行の直前の要素を返す．
inline
const MincovCell*
MincovCell::row_prev() const
{
  return mLeftLink;
}

// @brief 同じ行の直後の要素を返す．
inline
const MincovCell*
MincovCell::row_next() const
{
  return mRightLink;
}

// @brief 同じ列の直前の要素を返す．
inline
const MincovCell*
MincovCell::col_prev() const
{
  return mUpLink;
}

// @brief 同じ列の直後の要素を返す．
inline
const MincovCell*
MincovCell::col_next() const
{
  return mDownLink;
}


//////////////////////////////////////////////////////////////////////
/// @class MincovRowHead MincovMatrix.h <ym_mincov/MincovMatrix.h>
/// @brief Mincov 用の行の先頭要素を表すクラス
//////////////////////////////////////////////////////////////////////
class MincovRowHead
{
  friend class MincovMatrix;

private:

  /// @brief コンストラクタ
  MincovRowHead();

  /// @brief デストラクタ
  ~MincovRowHead();


public:

  /// @brief 行番号を返す．
  ymuint32
  pos() const;
  
  /// @brief 要素数を返す．
  ymuint32
  num() const;

  /// @brief 先頭の要素を返す．
  const MincovCell*
  front() const;

  /// @brief 末尾の要素を返す．
  const MincovCell*
  back() const;

  /// @brief cell が終端かどうか調べる．
  /// @param[in] cell 対象の要素
  /// @return cell が終端の時 true を返す．
  bool
  is_end(const MincovCell* cell) const;

  /// @brief 直前の行を返す．
  const MincovRowHead*
  prev() const;

  /// @brief 直後の行を返す．
  const MincovRowHead*
  next() const;
  
  
private:

  /// @brief 接続している要素をクリアする．
  void
  clear();
  
  /// @brief 追加する要素の両脇を求める．
  /// @param[in] cell 対象の要素
  /// @retval true 正しく処理された．
  /// @retval false 同一の要素が存在した．
  /// @note cell の mRightLink, mLeftLink に結果がセットされる．
  bool
  search_insert_pos(MincovCell* cell);
  
  /// @brief 要素を追加する．
  /// @param[in] cell 追加する要素
  /// @note cell の mRightLink, mLeftLink に適切な値がセットされているものとする．
  void
  insert_elem(MincovCell* cell);

  /// @brief 要素を削除する．
  /// @param[in] cell 削除する要素
  void
  delete_elem(MincovCell* cell);
  
  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 行番号
  ymuint32 mPos;

  // 要素数
  ymuint32 mNum;

  // 二重連結リストのダミーヘッダ
  MincovCell mDummy;

  // 直前の行を指すリンク
  MincovRowHead* mPrev;

  // 直後の列を指すリンク
  MincovRowHead* mNext;
  
};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 行番号を返す．
inline
ymuint32
MincovRowHead::pos() const
{
  return mPos;
}
  
// @brief 要素数を返す．
inline
ymuint32
MincovRowHead::num() const
{
  return mNum;
}

// @brief 先頭の要素を返す．
inline
const MincovCell*
MincovRowHead::front() const
{
  return mDummy.mRightLink;
}

// @brief 末尾の要素を返す．
inline
const MincovCell*
MincovRowHead::back() const
{
  return mDummy.mLeftLink;
}

// @brief cell が終端かどうか調べる．
// @param[in] cell 対象の要素
// @return cell が終端の時 true を返す．
inline
bool
MincovRowHead::is_end(const MincovCell* cell) const
{
  return cell == &mDummy;
}

// @brief 直前の行を返す．
inline
const MincovRowHead*
MincovRowHead::prev() const
{
  return mPrev;
}

// @brief 直後の行を返す．
inline
const MincovRowHead*
MincovRowHead::next() const
{
  return mNext;
}

// @brief 接続している要素をクリアする．
inline
void
MincovRowHead::clear()
{
  mDummy.mLeftLink = &mDummy;
  mDummy.mRightLink = &mDummy;
  mNum = 0;
}

// @brief 要素を追加する．
// @param[in] cell 追加する要素
// @note cell の mRightLink, mLeftLink に適切な値がセットされているものとする．
inline
void
MincovRowHead::insert_elem(MincovCell* cell)
{
  MincovCell* prev = cell->mLeftLink;
  MincovCell* next = cell->mRightLink;

  prev->mRightLink = cell;
  next->mLeftLink = cell;
  ++ mNum;
}

// @brief 要素を削除する．
// @param[in] cell 削除する要素
inline
void
MincovRowHead::delete_elem(MincovCell* cell)
{
  MincovCell* prev = cell->mLeftLink;
  MincovCell* next = cell->mRightLink;

  prev->mRightLink = next;
  next->mLeftLink = prev;
  -- mNum;
}


//////////////////////////////////////////////////////////////////////
/// @class MincovColHead MincovMatrix.h <ym_mincov/MincovMatrix.h>
/// @brief Mincov 用の列の先頭要素を表すクラス
//////////////////////////////////////////////////////////////////////
class MincovColHead
{
  friend class MincovMatrix;

private:

  /// @brief コンストラクタ
  MincovColHead();

  /// @brief デストラクタ
  ~MincovColHead();


public:

  /// @brief 列番号を返す．
  ymuint32
  pos() const;
  
  /// @brief 要素数を返す．
  ymuint32
  num() const;

  /// @brief 先頭の要素を返す．
  const MincovCell*
  front() const;

  /// @brief 末尾の要素を返す．
  const MincovCell*
  back() const;

  /// @brief cell が終端かどうか調べる．
  /// @param[in] cell 対象の要素
  /// @return cell が終端の時 true を返す．
  bool
  is_end(const MincovCell* cell) const;

  /// @brief 直前の列を返す．
  const MincovColHead*
  prev() const;

  /// @brief 直後の列を返す．
  const MincovColHead*
  next() const;

  
private:

  /// @brief 接続している要素をクリアする．
  void
  clear();
  
  /// @brief 追加する要素の両脇を求める．
  /// @param[in] cell 対象の要素
  /// @retval true 正しく処理された．
  /// @retval false 同一の要素が存在した．
  /// @note cell の mDownLink, mUpLink に結果がセットされる．
  bool
  search_insert_pos(MincovCell* cell);
  
  /// @brief 要素を追加する．
  /// @param[in] cell 追加する要素
  /// @note cell の mDownLink, mUpLink に適切な値がセットされているものとする．
  void
  insert_elem(MincovCell* cell);

  /// @brief 要素を削除する．
  /// @param[in] cell 削除する要素
  void
  delete_elem(MincovCell* cell);

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 列番号
  ymuint32 mPos;

  // 要素数
  ymuint32 mNum;

  // 二重連結リストのダミーヘッダ
  MincovCell mDummy;

  // 直前の列を指すリンク
  MincovColHead* mPrev;

  // 直後の列を指すリンク
  MincovColHead* mNext;
  
};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 列番号を返す．
inline
ymuint32
MincovColHead::pos() const
{
  return mPos;
}
  
// @brief 要素数を返す．
inline
ymuint32
MincovColHead::num() const
{
  return mNum;
}

// @brief 先頭の要素を返す．
inline
const MincovCell*
MincovColHead::front() const
{
  return mDummy.mDownLink;
}

// @brief 末尾の要素を返す．
inline
const MincovCell*
MincovColHead::back() const
{
  return mDummy.mUpLink;
}

// @brief cell が終端かどうか調べる．
// @param[in] cell 対象の要素
// @return cell が終端の時 true を返す．
inline
bool
MincovColHead::is_end(const MincovCell* cell) const
{
  return cell == &mDummy;
}

// @brief 直前の列を返す．
inline
const MincovColHead*
MincovColHead::prev() const
{
  return mPrev;
}

// @brief 直後の列を返す．
inline
const MincovColHead*
MincovColHead::next() const
{
  return mNext;
}

// @brief 接続している要素をクリアする．
inline
void
MincovColHead::clear()
{
  mDummy.mUpLink = &mDummy;
  mDummy.mDownLink = &mDummy;
  mNum = 0;
}

// @brief 要素を追加する．
// @param[in] cell 追加する要素
// @note cell の mDownLink, mUpLink に適切な値がセットされているものとする．
inline
void
MincovColHead::insert_elem(MincovCell* cell)
{
  MincovCell* prev = cell->mUpLink;
  MincovCell* next = cell->mDownLink;

  prev->mDownLink = cell;
  next->mUpLink = cell;
  ++ mNum;
}

// @brief 要素を削除する．
// @param[in] cell 削除する要素
inline
void
MincovColHead::delete_elem(MincovCell* cell)
{
  MincovCell* prev = cell->mUpLink;
  MincovCell* next = cell->mDownLink;

  prev->mDownLink = next;
  next->mUpLink = prev;
  -- mNum;
}


//////////////////////////////////////////////////////////////////////
/// @class MincovMatrix MincovMatrix.h <ym_mincov/MincovMatrix.h>
/// @brief Mincov 用の行列を表すクラス
//////////////////////////////////////////////////////////////////////
class MincovMatrix
{
public:

  /// @brief コンストラクタ
  /// @param[in] row_size 行数
  /// @param[in] col_size 列数
  /// @param[in] cost_size コストの次元
  /// @note 要素を持たない行列となる．
  MincovMatrix(ymuint32 row_size,
	       ymuint32 col_size,
	       ymuint32 cost_size);
  
  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  MincovMatrix(const MincovMatrix& src);
  
  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  const MincovMatrix&
  operator=(const MincovMatrix& src);
  
  /// @brief デストラクタ
  ~MincovMatrix();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 行数を返す．
  ymuint32
  row_size() const;

  /// @brief 列数を返す．
  ymuint32
  col_size() const;
  
  /// @brief コストの次元を返す．
  ymuint32
  cost_size() const;
  
  /// @brief 行を取り出す．
  /// @param[in] row_pos 行位置 ( 0 <= row_pos < row_size() )
  const MincovRowHead*
  row(ymuint32 row_pos) const;

  /// @brief 行の先頭を取り出す．
  const MincovRowHead*
  row_front() const;
  
  /// @brief 行の末尾を取り出す．
  const MincovRowHead*
  row_back() const;

  /// @brief row が終端かどうか調べる．
  /// @param[in] row 対象の行
  /// @return row が終端の時 true を返す．
  bool
  is_row_end(const MincovRowHead* row) const;
  
  /// @brief 列を取り出す．
  /// @param[in] col_pos 列位置 ( 0 <= col_pos < col_size() )
  const MincovColHead*
  col(ymuint32 col_pos) const;

  /// @brief 列の先頭を取り出す．
  const MincovColHead*
  col_front() const;
  
  /// @brief 列の末尾を取り出す．
  const MincovColHead*
  col_back() const;

  /// @brief col が終端かどうか調べる．
  /// @param[in] col 対象の行
  /// @return col が終端の時 true を返す．
  bool
  is_col_end(const MincovColHead* col) const;
  
  /// @brief 列のコストを取り出す．
  /// @param[in] col_pos 列位置 ( 0 <= col_pos < col_size() )
  const MincovCost&
  col_cost(ymuint32 col_pos) const;
  

public:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 内容をクリアする．
  /// @note 行/列のサイズは不変
  void
  clear();
  
  /// @brief サイズを変更する．
  /// @param[in] row_size 行数
  /// @param[in] col_size 列数
  /// @param[in] cost_size コストの次元
  /// @note 内容はクリアされる．
  void
  resize(ymuint32 row_size,
	 ymuint32 col_size,
	 ymuint32 cost_size);
  
  /// @brief 要素を追加する．
  /// @param[in] row_pos 追加する要素の行番号
  /// @param[in] col_pos 追加する要素の列番号
  /// @return 追加された要素を返す．
  MincovCell*
  insert_elem(ymuint32 row_pos,
	      ymuint32 col_pos);
  
  /// @brief 列のコストを設定する．
  /// @param[in] col_pos 列番号
  /// @param[in] val_pos 値の位置番号
  /// @param[in] value 設定する値
  void
  set_col_cost(ymuint32 col_pos,
	       ymuint32 val_pos,
	       double value);
  
  /// @brief 列を選択し，被覆される行を削除する．
  /// @param[in] col_pos 選択した列
  void
  select_col(ymuint32 col_pos);
  
  /// @brief 行を削除する．
  void
  delete_row(ymuint32 row_pos);
  
  /// @brief 列を削除する．
  void
  delete_col(ymuint32 col_pos);
  
  /// @brief 削除スタックにマーカーを書き込む．
  void
  backup();

  /// @brief 直前のマーカーまで処理を戻す．
  void
  restore();

  
private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をコピーする．
  void
  copy(const MincovMatrix& src);
  
  /// @brief 行を復元する．
  void
  restore_row(ymuint32 row_pos);

  /// @brief 列を復元する．
  void
  restore_col(ymuint32 col_pos);
  
  /// @brief セルの生成
  MincovCell*
  alloc_cell();

  /// @brief セルの解放
  void
  free_cell(MincovCell* cell);

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // セルを確保するためのアロケータ
  UnitAlloc mCellAlloc;
  
  // 行数
  ymuint32 mRowSize;

  // 列数
  ymuint32 mColSize;
  
  // コストの次元
  ymuint32 mCostSize;
  
  // 行の先頭の配列
  MincovRowHead* mRowArray;
  
  // 行の先頭をつなぐリンクトリストのダミー
  MincovRowHead mRowHead;
  
  // 列の先頭の配列
  MincovColHead* mColArray;

  // 列の先頭をつなぐリンクトリストのダミー
  MincovColHead mColHead;
  
  // 列のコストの配列
  MincovCost** mColCostArray;
  
  // 削除の履歴を覚えておくスタック
  vector<ymuint32> mDelStack;
  
};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 行数を返す．
inline
ymuint32
MincovMatrix::row_size() const
{
  return mRowSize;
}

// @brief 列数を返す．
inline
ymuint32
MincovMatrix::col_size() const
{
  return mColSize;
}
  
// @brief コストの次元を返す．
inline
ymuint32
MincovMatrix::cost_size() const
{
  return mCostSize;
}

// @brief 行の先頭を取り出す．
// @param[in] row_pos 行位置 ( 0 <= row_pos < row_size() )
inline
const MincovRowHead*
MincovMatrix::row(ymuint32 row_pos) const
{
  return &mRowArray[row_pos];
}

// @brief 行の先頭を取り出す．
inline
const MincovRowHead*
MincovMatrix::row_front() const
{
  return mRowHead.mNext;
}

// @brief 行の末尾を取り出す．
inline
const MincovRowHead*
MincovMatrix::row_back() const
{
  return mRowHead.mPrev;
}

// @brief row が終端かどうか調べる．
// @param[in] row 対象の行
// @return row が終端の時 true を返す．
inline
bool
MincovMatrix::is_row_end(const MincovRowHead* row) const
{
  return row == &mRowHead;
}

// @brief 列の先頭を取り出す．
// @param[in] col_pos 列位置 ( 0 <= col_pos < col_size() )
inline
const MincovColHead*
MincovMatrix::col(ymuint32 col_pos) const
{
  return &mColArray[col_pos];
}

// @brief 列の先頭を取り出す．
inline
const MincovColHead*
MincovMatrix::col_front() const
{
  return mColHead.mNext;
}

// @brief 列の末尾を取り出す．
inline
const MincovColHead*
MincovMatrix::col_back() const
{
  return mColHead.mPrev;
}

// @brief col が終端かどうか調べる．
// @param[in] col 対象の行
// @return col が終端の時 true を返す．
inline
bool
MincovMatrix::is_col_end(const MincovColHead* col) const
{
  return col == &mColHead;
}

// @brief 列のコストを取り出す．
// @param[in] col_pos 列位置 ( 0 <= col_pos < col_size() )
inline
const MincovCost&
MincovMatrix::col_cost(ymuint32 col_pos) const
{
  return *mColCostArray[col_pos];
}

END_NAMESPACE_YM_MINCOV

#endif // YM_MINCOV_MINCOVMATRIX_H
