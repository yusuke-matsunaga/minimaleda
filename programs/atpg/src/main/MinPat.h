#ifndef MAIN_MINPAT_H
#define MAIN_MINPAT_H

/// @file src/main/MinPat.h
/// @brief MinPat のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MinPat.h 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "AtpgFunctor.h"


BEGIN_NAMESPACE_YM_ATPG

//////////////////////////////////////////////////////////////////////
/// @class MinPatStats MinPat.h "MinPat.h"
/// @brief MinPat の statistics data を表すクラス
//////////////////////////////////////////////////////////////////////
class MinPatStats
{
  friend class MinPat;

public:

  /// @brief コンストラクタ
  MinPatStats();

  /// @brief デストラクタ
  ~MinPatStats();

  
public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 元のパタン数を得る．
  ymuint
  original_patterns() const;

  /// @brief 最小化後のパタン数を得る．
  ymuint
  minimized_patterns() const;

  /// @brief 計算時間を得る．
  USTime
  time() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 元のパタン数
  ymuint mPatNum1;

  // 最小化後のパタン数
  ymuint mPatNum2;

  // 計算時間
  USTime mTime;
  
};


//////////////////////////////////////////////////////////////////////
/// @class MinPat MinPat.h "MinPat.h"
/// @brief テストベクタの最小化を行うクラス
//////////////////////////////////////////////////////////////////////
class MinPat :
  public AtpgFunctor
{
public:

  /// @brief コンストラクタ
  MinPat(AtpgMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~MinPat();
  
  
public:
  
  /// @brief テストベクタの最小化を行う．
  /// @param[in] mode 最小化モード
  void
  operator()(ymuint mode);
  
  /// @brief 直前の実行結果を得る．
  const MinPatStats&
  stats() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 実行結果
  MinPatStats mStats;
  
};

END_NAMESPACE_YM_ATPG

#endif // MAIN_MINPAT_H
