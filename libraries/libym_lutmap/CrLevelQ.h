#ifndef LIBYM_LUTMAP_CRLEVELQ_H
#define LIBYM_LUTMAP_CRLEVELQ_H

/// @file libym_lutmap/CrLevelQ.h
/// @brief CrLevelQ のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CrLevelQ.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

#include "ym_lutmap/lutmap_nsdef.h"


BEGIN_NAMESPACE_YM_LUTMAP

class CrNode;

//////////////////////////////////////////////////////////////////////
/// @class CrLevelQ CutResub.h "CutResub.h"
/// @brief CrNode のレベル付きキュー
//////////////////////////////////////////////////////////////////////
class CrLevelQ
{
public:

  /// @brief コンストラクタ
  CrLevelQ();

  /// @brief デストラクタ
  ~CrLevelQ();


public:

  /// @brief 作業領域の初期化を行う．
  /// @param[in] max_level 最大レベル
  void
  init(ymuint max_level);

  /// @brief 内容をクリアする．
  void
  clear();

  /// @brief ノードを追加する．
  void
  put(CrNode* node);

  /// @brief 要素数を得る．
  ymuint
  num() const { return mNum; }

  /// @brief 最小レベルの要素を取り出す．
  CrNode*
  getmin();

  /// @brief 最大レベルの要素を取り出す．
  CrNode*
  getmax();
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // レベルごとに分けられたキューの配列
  vector<list<CrNode*> > mLevelQ;

  // mLevelQ に入っている全要素数
  ymuint32 mNum;

  // 現在要素が入っている最小レベル
  ymuint32 mMinLevel;

  // 現在要素が入っている最大レベル
  ymuint32 mMaxLevel;

};

END_NAMESPACE_YM_LUTMAP

#endif // LIBYM_LUTMAP_CRLEVELQ_H
