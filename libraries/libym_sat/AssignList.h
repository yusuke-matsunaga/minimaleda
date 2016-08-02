#ifndef LIBYM_SAT_ASSIGNLIST_H
#define LIBYM_SAT_ASSIGNLIST_H

/// @file libym_sat/AssignList.h
/// @brief AssignList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: AssignList.h 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_sat/SatSolver.h"


BEGIN_NAMESPACE_YM_SAT

//////////////////////////////////////////////////////////////////////
/// @class AssignList AssignList.h "AssignList.h"
/// @brief 割り当ての記録を保持するクラス
///
/// 基本的にはスタックとして機能するが，一つ一つもどるのではなく，
/// 直前の決定(他の割り当てからの含意ではない値の割り当て)までを
/// 単位として戻るので，その位置を覚えておく補助的な配列を用意する．
/// ここではそれを marker と呼んでいる．
/// set_marker() を呼ぶと今のスタックの位置を marker として記憶し，
/// 現在の decision_level を一つ増やす．
/// marker(level) で level に対応した(正確には level の割り当てを行う
/// 直前の)スタックの位置を返す．level 以降の割り当てを無効にする場合
/// には marker(level) までの割り当てを取り消せばよい．
/// ただし marker は AssignList の外部からはアクセス不可となっている．
/// 可能な操作としては backtrack(level) で指定したレベルまでスタックの
/// 底を移動させ，get_prev() でスタックの上から順に割り当てを取り出す
/// 処理を has_elem() が false になるまで繰り返すことである．
///
/// 一方，含意操作中には AssignList は未処理の割り当てを入れておくキュー
/// として機能する．そのため，読み出し位置を記録する mListHead という
/// メンバ変数を持っている．ただし，この変数は外部からはアクセス不可で
/// get_next()(次に進める), skip_all()(末尾に進める), backtrack()(指定
/// したレベルまで戻る) の3つの関数でのみ値が更新される．
///
/// 最後に，AssignList の任意の位置にアクセスするための関数として，
/// get(pos) が用意されている．この関数を使うときは事前に size() で
/// アクセス可能なサイズを得ておくこと．
/// AssignList には一つの変数につき高々一回しか積まれることはないので，
/// AssignList の最大サイズは変数の数となる．また，問題が SAT の時，
/// 全ての変数に値が割り当てられるので，その時の AssignList のサイズは
/// 変数の数となる．よって，新しい変数を確保する度に reserve() を呼んで
/// 充分な大きさに拡張しておく．そうしておくことで put() などの処理中
/// に配列のサイズを気にする必要がなくなる．
//////////////////////////////////////////////////////////////////////
class AssignList
{
public:

  /// @brief コンストラクタ
  AssignList();

  /// @brief デストラクタ
  ~AssignList();


public:
  //////////////////////////////////////////////////////////////////////
  // 領域確保に関する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 必要なサイズを指定する．
  void
  reserve(ymuint req_size);


public:
  //////////////////////////////////////////////////////////////////////
  // 割り当ての追加/取り出しに関する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 割り当てを追加する．
  /// @note ここでは領域の拡張を行わない．それ以前に reserve() で適切な
  /// サイズが指定されている必要がある．
  void
  put(Literal lit);

  /// @brief 記録されている割り当ての要素数を得る．
  ymuint
  size() const;
  
  /// @brief 読み出す要素があるとき true を返す．
  bool
  has_elem() const;

  /// @brief 次の要素を返す．
  /// @return 今の読み出し位置の要素を返す．
  /// @note 読み出し位置は一つ進む
  Literal
  get_next();

  /// @brief 前の要素を返す．
  /// @return 書き込み位置の一つ手前の要素を返す．
  /// @note 書き込み位置は一つもどる．
  Literal
  get_prev();

  /// @brief 全てを読み出したことにする．
  /// @note 読み出し位置を書き込み位置にする．
  void
  skip_all();
  
  /// @brief pos 番目の要素を得る．
  Literal
  get(ymuint pos) const;


public:
  //////////////////////////////////////////////////////////////////////
  // decision level と marker の操作に関する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 現在のレベルを返す．
  int
  cur_level() const;
  
  /// @brief 現在の位置をマーカーに登録する．
  void
  set_marker();

  /// @brief level の割り当てを行う直前の状態にもどす．
  /// @param[in] level 対象のレベル
  /// @note 具体的には読み出し位置が level のマーカとなり，現在のレベルが
  /// level となる．
  void
  backtrack(int level);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // リストの実際の容量
  ymuint32 mSize;
  
  // 値割り当てを保持するリスト(配列)
  Literal* mList;

  // 書き込み位置
  ymuint32 mTail;

  // 読み出し位置
  ymuint32 mHead;
  
  // 各 decision-level ごとの marker の配列
  ymuint32* mMarker;

  // 現在の decision_level
  ymuint32 mCurLevel;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 割り当てを追加する．
inline
void
AssignList::put(Literal lit)
{
  mList[mTail ++] = lit;
}

// @brief 記録されている割り当ての要素数を得る．
inline
ymuint
AssignList::size() const
{
  return mTail;
}

// @brief 読み出す要素があるとき true を返す．
inline
bool
AssignList::has_elem() const
{
  return mHead < mTail;
}

// @brief 次の要素を返す．
inline
Literal
AssignList::get_next()
{
  return get(mHead ++);
}

// @brief 前の要素を返す．
inline
Literal
AssignList::get_prev()
{
  return get(-- mTail);
}

// @brief 全てを読み出したことにする．
inline
void
AssignList::skip_all()
{
  mHead = mTail;
}

// @brief pos 番目の要素を得る．
inline
Literal
AssignList::get(ymuint pos) const
{
  return mList[pos];
}

// @brief 現在のレベルを返す．
inline
int
AssignList::cur_level() const
{
  return mCurLevel;
}

// @brief 現在の位置をマーカーに登録する．
inline
void
AssignList::set_marker()
{
  mMarker[mCurLevel ++] = mTail;
}

// @brief level の割り当てを行う直前の状態にもどす．
inline
void
AssignList::backtrack(int level)
{
  mHead = mMarker[level];
  mCurLevel = level;
}

END_NAMESPACE_YM_SAT

#endif // LIBYM_SAT_ASSIGNLIST_H
