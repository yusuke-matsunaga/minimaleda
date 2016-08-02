#ifndef LIBYM_SAT_WATCHER_H
#define LIBYM_SAT_WATCHER_H

/// @file libym_sat/Watcher.h
/// @brief Watcher のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Watcher.h 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_sat/sat_nsdef.h"
#include "SatReason.h"


BEGIN_NAMESPACE_YM_SAT

//////////////////////////////////////////////////////////////////////
/// @class Watcher ImplInk.h "Watcher.h"
/// @brief watch literal に関連付けられた節のリストを表すクラス
///
/// (a1 + a2 + a3 + ... an) という節があるとする．
/// この時， a1 と a2 が watch literal ならば，~a1 または ~a2 
/// の割り当てが起こったときに，この節の watch literal の更新を行う
/// 必要がある．
/// そのような節のリストを作るためのクラス
//////////////////////////////////////////////////////////////////////
class Watcher :
  public SatReason
{
public:

  /// @brief コンストラクタ
  Watcher();

  /// @brief コンストラクタ
  /// @param[in] src もととなる SatReason
  explicit
  Watcher(SatReason src);
  
};


//////////////////////////////////////////////////////////////////////
/// @class WatcherList Watcher.h "Watcher.h"
/// @brief Watcher のリストを表すクラス
//////////////////////////////////////////////////////////////////////
class WatcherList
{
public:

  /// @brief コンストラクタ
  WatcherList();

  /// @brief デストラクタ
  ~WatcherList();


public:

  /// @brief 要素を追加する．
  void
  add(Watcher elem);
  
  /// @brief 要素数を返す．
  ymuint
  num() const;

  /// @brief pos 番目の要素を返す．
  Watcher
  elem(ymuint pos) const;
  
  /// @brief pos 番目の要素を設定する．
  void
  set_elem(ymuint pos,
	   Watcher elem);

  /// @brief 要素を切り詰める．
  void
  erase(ymuint num);
  
  /// @brief from の内容を移す．
  /// @note from は空になる．
  void
  move(WatcherList& from);
  

private:

  /// @brief 配列を拡張する．
  void
  expand(ymuint req_size);

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 配列のサイズ
  ymuint32 mSize;

  // 要素数
  ymuint32 mNum;

  // 配列
  Watcher* mArray;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
Watcher::Watcher()
{
}

// @brief コンストラクタ
// @param[in] src もととなる SatReason
inline
Watcher::Watcher(SatReason src) :
  SatReason(src)
{
}

// @brief コンストラクタ
inline
WatcherList::WatcherList() :
  mSize(0),
  mNum(0),
  mArray(NULL)
{
}

// @brief デストラクタ
inline
WatcherList::~WatcherList()
{
  delete [] mArray;
}

// @brief 要素を追加する．
inline
void
WatcherList::add(Watcher elem)
{
  if ( mNum >= mSize ) {
    expand(mNum + 1);
  }
  set_elem(mNum, elem);
  ++ mNum;
}
  
// @brief 要素数を返す．
inline
ymuint
WatcherList::num() const
{
  return mNum;
}

// @brief pos 番目の要素を返す．
inline
Watcher
WatcherList::elem(ymuint pos) const
{
  return mArray[pos];
}
  
// @brief pos 番目の要素を設定する．
inline
void
WatcherList::set_elem(ymuint pos,
		      Watcher elem)
{
  mArray[pos] = elem;
}

// @brief 要素を切り詰める．
inline
void
WatcherList::erase(ymuint num)
{
  mNum = num;
}
  
// @brief from の内容を移す．
// @note from は空になる．
inline
void
WatcherList::move(WatcherList& from)
{
  mSize = from.mSize;
  mNum = from.mNum;
  mArray = from.mArray;
  from.mSize = 0;
  from.mNum = 0;
  from.mArray = NULL;
}

// @brief 配列を拡張する．
inline
void
WatcherList::expand(ymuint req_size)
{
  if ( mSize < req_size ) {
    ymuint old_size = mSize;
    Watcher* old_array = mArray;
    if ( mSize == 0 ) {
      mSize = 4;
    }
    while ( mSize < req_size ) {
      mSize <<= 1;
    }
    mArray = new Watcher[mSize];
    for (ymuint i = 0; i < old_size; ++ i) {
      mArray[i] = old_array[i];
    }
    delete [] old_array;
  }
}

END_NAMESPACE_YM_SAT

#endif // LIBYM_SAT_SATREASON_H
