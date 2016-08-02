#ifndef LIBYM_VERILOG_SCANNER_LEXCONDSTATE_H
#define LIBYM_VERILOG_SCANNER_LEXCONDSTATE_H

/// @file libym_verilog/scanner/LexCondState.h
/// @brief LexCondState のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: LexCondState.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "LexState.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class LexCondState
/// @ingroup VlParser
/// @brief 条件コンパイル関係の compiler directive 用状態を表すクラス
//////////////////////////////////////////////////////////////////////
class LexCondState :
  public LexState
{
  friend class LexCondPlugin;

public:

  /// @brief コンストラクタ
  LexCondState(RawLex& lex);
  
  /// @brief デストラクタ
  virtual
  ~LexCondState();


public:
  //////////////////////////////////////////////////////////////////////
  // 状態を返す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 条件が成り立っているかを示す．
  bool
  cond() const;


public:
  //////////////////////////////////////////////////////////////////////
  // LexState の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 状態を初期化する．
  virtual
  void
  resetall(const FileRegion& file_region);

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 現在の条件
  bool mCurCond;

  // 条件が成り立っていない `ifdef/`ifndef のネストレベル
  ymuint32 mFalseNestLevel;

  // 条件が成り立っている `ifdef/`ifndef のネストレベル
  ymuint32 mTrueNestLevel;

  // `else 節の中にいるとき true にするフラグ
  // 実際にはスタック
  vector<bool> mElseFlag;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_SCANNER_LEXCONDSTATE_H
