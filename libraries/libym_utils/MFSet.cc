
/// @file libym_utils/MFSet.cc
/// @brief MFSet の実装ファイル
/// @author Yusuke Matsunaga
///
/// $Id: MFSet.cc 958 2007-08-28 05:38:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/MFSet.h"


BEGIN_NAMESPACE_YM

/// @class MFSetCell MFSet.cc
/// @brief MFSet 内部で用いられるクラス
class MFSetCell
{
  friend class MFSet;
  typedef MFSet::id_type id_type;
private:

  // コンストラクタ
  MFSetCell();
  
  // この要素の属する集合の代表元を返す．
  MFSetCell*
  find();
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 一つの母集合の中でユニークなID番号
  id_type mId;

  // 親を指すポインタ
  MFSetCell* mParent;

  // 根本までのレベル
  int mRank;

};


// コンストラクタ
MFSetCell::MFSetCell()
{
  mParent = this;
  mRank = 0;
}

// 検索処理
MFSetCell*
MFSetCell::find()
{
  MFSetCell* tmp = this;

  // 検索の途中で道の圧縮を行なう．
  MFSetCell* parent = tmp->mParent;
  while ( parent->mParent != parent ) {
    tmp = tmp->mParent = parent->mParent;
    parent = tmp->mParent;
  }

  return parent;
}

// x 番めのセルを取り出す．
MFSetCell*
MFSet::get(id_type id)
{
  if ( id >= mNum ) {
    // 範囲外の場合はNULLを返す．
    return NULL;
  }
  return &mCellArray[id];
}

// n 個の要素を持つ集合を作るコンストラクタ．
MFSet::MFSet(size_t n) :
  mNum(n),
  mCellArray(new MFSetCell[mNum])
{
  for (size_t i = 0; i < n; ++ i) {
    mCellArray[i].mId = i;
  }
}

// デストラクタ
MFSet::~MFSet()
{
  delete [] mCellArray;
}

// x を含む集合の代表元を返す．
// x が存在しない時には 0 を返す．
MFSet::id_type 
MFSet::find(id_type id)
{
  MFSetCell* x = get(id);
  if ( x ) {
    x = x->find();
    return x->mId;
  }
  else {
    return 0;
  }
}

// x_id を含む集合と y_id を含む集合を併合し，新しい集合の代表元を返す．
// x_id や y_id が存在しない時には 0 を返す．
MFSet::id_type
MFSet::merge(id_type x_id,
	     id_type y_id)
{
  MFSetCell* x = get(x_id);
  if ( !x ) return 0;

  MFSetCell* y = get(y_id);
  if ( !y ) return 0;

  if ( x->mParent != x ) {
    // xは代表点ではない．
    x = x->find(); 
  }

  if ( y->mParent != y ) {
    // yは代表点ではない．
    y = y->find();
  }

  if ( x == y ) {
    // 同一の集合のマージはできない．
    // ただし，違反ではないのでその代表元を返す．
    return x->mId;
  }

  // ランクによるマージ
  if ( x->mRank > y->mRank ) {
    y->mParent = x;
    return x->mId;
  }
  else if ( x->mRank == y->mRank ) {
    ++ x->mRank;
    y->mParent = x;
    return x->mId;
  }
  else {
    x->mParent = y;
    return y->mId;
  }
}

END_NAMESPACE_YM
