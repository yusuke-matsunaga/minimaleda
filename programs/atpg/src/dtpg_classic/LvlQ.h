#ifndef DTPG_CLASSIC_LVLQ_H
#define DTPG_CLASSIC_LVLQ_H

/// @file src/dtpg_classic/LvlQ.h
/// @brief LvlQ のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: LvlQ.h 2128 2009-03-12 07:25:33Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "dtpgcl_nsdef.h"
#include "Gate.h"


BEGIN_NAMESPACE_YM_ATPG_DTPGCL

//////////////////////////////////////////////////////////////////////
/// @class LvlQ LvlQ.h "LvlQ.h"
/// @brief レベル付けされたキューを表すクラス
//////////////////////////////////////////////////////////////////////
class LvlQ
{
public:

  /// @brief コンストラクタ
  LvlQ();

  /// @brief デストラクタ
  ~LvlQ();


public:

  /// @brief サイズを初期化する．
  void
  resize(ymuint req_size);

  /// @brief 内容をクリアする．
  void
  clear();

  /// @brief ゲートを追加する．
  void
  put(Gate* gate);

  /// @brief 外部出力に近い順に取り出す．
  /// @note 空なら NULL を返す．
  Gate*
  get_from_top();

  /// @brief 外部入力に近い順に取り出す．
  /// @note 空なら NULL を返す．
  Gate*
  get_from_bottom();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // キュー本体
  vector<Gate*> mArray;

  // 要素数
  ymuint32 mNum;

  // 最小のレベル
  ymuint32 mMinLevel;

  // 最大のレベル
  ymuint32 mMaxLevel;

};

extern
LvlQ lvlq;

//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

inline
void
LvlQ::put(Gate* gate)
{
  if ( !gate->chk_lvlq() ) {
    gate->set_lvlq();
    ymuint level = gate->level();
    gate->mNext = mArray[level];
    mArray[level] = gate;
    if ( mMinLevel > level ) {
      mMinLevel = level;
    }
    if ( mMaxLevel < level ) {
      mMaxLevel = level;
    }
    ++ mNum;
  }
}

END_NAMESPACE_YM_ATPG_DTPGCL

#endif
