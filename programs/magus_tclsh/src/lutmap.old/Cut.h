#ifndef MAGUS_LUTMAP_CUT_H
#define MAGUS_LUTMAP_CUT_H

/// @file magus/lutmap/Cut.h 
/// @brief Cut のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Cut.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#include <ym_lexp/LogExpr.h>
#include "lutmap.h"


BEGIN_NAMESPACE_MAGUS_LUTMAP

class SbjNode;

//////////////////////////////////////////////////////////////////////
/// @class Cut Cut.h "Cut.h"
/// @brief カットを表すクラス
//////////////////////////////////////////////////////////////////////
class Cut
{
  friend class CutHolder;
  friend class CutList;
  friend class CutListIterator;

private:

  /// @brief 空のコンストラクタ
  /// @note 内容は正しくない．
  Cut();
  
  /// @brief デストラクタ
  ~Cut();

  
public:

  /// @brief 根のノードを得る．
  SbjNode*
  root() const;
  
  /// @brief 入力のサイズを得る．
  ymuint
  ni() const;

  /// @brief pos 番目の入力を得る．
  SbjNode*
  input(ymuint pos) const;

  /// @brief 内部に fo-mark の付いたノードを含んでいたら true を返す．
  bool
  fo_check() const;

  /// @brief 内容を表す論理式を得る．
  LogExpr
  expr() const;

  /// @brief デバッグ用の表示ルーティン
  /// @param[in] s 出力先のストリーム
  void
  dump(ostream& s) const;


private:

  // fo_check のサブルーティン
  bool
  fo_check_sub(SbjNode* node) const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 根のノード
  SbjNode* mRoot;

  // 次のカットを指すポインタ
  Cut* mLink;
  
  // 入力数
  ymuint32 mNi;

  // 入力のノード配列
  SbjNode* mInputs[1];

};


//////////////////////////////////////////////////////////////////////
/// @class CutListIterator Cut.h "Cut.h"
/// @brief Cut のリストをたどるための反復子
//////////////////////////////////////////////////////////////////////
class CutListIterator
{
  friend class CutList;
public:

  /// @brief コンストラクタ
  CutListIterator();

  /// @brief デストラクタ
  ~CutListIterator();


private:

  /// @brief 内容を指定したコンストラクタ
  CutListIterator(Cut* cut);


public:

  /// @brief カットを得る．
  const Cut*
  operator*() const;

  /// @brief 次の要素に移動する．
  void
  operator++();

  /// @brief 等価比較演算子
  friend
  bool
  operator==(CutListIterator a,
	     CutListIterator b);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のカット
  Cut* mCut;

};

/// @relates CutListIterator
/// @brief 非等価比較演算子
bool
operator!=(CutListIterator a,
	   CutListIterator b);


//////////////////////////////////////////////////////////////////////
/// @class CutList Cut.h "Cut.h"
/// @brief Cut のリストを表すクラス
//////////////////////////////////////////////////////////////////////
class CutList
{
public:

  /// @brief コンストラクタ
  CutList();

  /// @brief デストラクタ
  ~CutList();


public:

  /// @brief 内容をクリアする．
  void
  clear();

  /// @brief カットを末尾に追加する．
  void
  push_back(Cut* cut);

  /// @brief 先頭を表す反復子を返す．
  CutListIterator
  begin() const;

  /// @brief 末尾を表す反復子を返す．
  CutListIterator
  end() const;

  /// @brief 要素数を返す．
  ymuint
  size() const;

  /// @brief 空のとき true を返す．
  bool
  empty() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の要素
  Cut* mTop;

  // 末尾の要素
  Cut* mTail;

  // 要素数
  ymuint32 mNum;
  
};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// 根のノードを得る．
inline
SbjNode*
Cut::root() const
{
  return mRoot;
}

// 入力のサイズを得る．
inline
ymuint
Cut::ni() const
{
  return mNi;
}

// pos 番目の入力を得る．
inline
SbjNode*
Cut::input(ymuint pos) const
{
  return mInputs[pos];
}

// @brief コンストラクタ
inline
CutListIterator::CutListIterator() :
  mCut(NULL)
{
}

// @brief デストラクタ
inline
CutListIterator::~CutListIterator()
{
}

// @brief 内容を指定したコンストラクタ
inline
CutListIterator::CutListIterator(Cut* cut) :
  mCut(cut)
{
}

// @brief カットを得る．
inline
const Cut*
CutListIterator::operator*() const
{
  return mCut;
}

// @brief 次の要素に移動する．
inline
void
CutListIterator::operator++()
{
  if ( mCut ) {
    mCut = mCut->mLink;
  }
}

// @brief 等価比較演算子
inline
bool
operator==(CutListIterator a,
	   CutListIterator b)
{
  return a.mCut == b.mCut;
}

// @relates CutListIterator
// @brief 非等価比較演算子
inline
bool
operator!=(CutListIterator a,
	   CutListIterator b)
{
  return !operator==(a, b);
}

// @brief コンストラクタ
inline
CutList::CutList() :
  mTop(NULL),
  mTail(NULL),
  mNum(0)
{
}

// @brief デストラクタ
inline
CutList::~CutList()
{
}

// @brief 内容をクリアする．
inline
void
CutList::clear()
{
  mTop = NULL;
  mTail = NULL;
  mNum = 0;
}

// @brief カットを末尾に追加する．
inline
void
CutList::push_back(Cut* cut)
{
  if ( mTail ) {
    mTail->mLink = cut;
    mTail = cut;
  }
  else {
    mTop = mTail = cut;
  }
  ++ mNum;
  // 念のため
  cut->mLink = NULL;
}

// @brief 先頭を表す反復子を返す．
inline
CutListIterator
CutList::begin() const
{
  return CutListIterator(mTop);
}

// @brief 末尾を表す反復子を返す．
inline
CutListIterator
CutList::end() const
{
  return CutListIterator(NULL);
}

// @brief 要素数を返す．
inline
ymuint
CutList::size() const
{
  return mNum;
}

// @brief 空のとき true を返す．
inline
bool
CutList::empty() const
{
  return mTop == NULL;
}

END_NAMESPACE_MAGUS_LUTMAP

#endif // MAGUS_LUTMAP_CUT_H
