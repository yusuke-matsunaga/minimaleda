#ifndef LIBYM_VERILOG_SCANNER_LEX_H
#define LIBYM_VERILOG_SCANNER_LEX_H

/// @file libym_verilog/scanner/Lex.h
/// @brief Lex のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Lex.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/verilog.h"
#include "RawLex.h"


BEGIN_NAMESPACE_YM_VERILOG

class LexModuleState;

//////////////////////////////////////////////////////////////////////
/// @class Lex Lex.h "Lex.h"
/// @ingroup VlParser
/// @brief Verilog 用の字句解析器のインターフェイス
//////////////////////////////////////////////////////////////////////
class Lex :
  public RawLex
{
public:
  
  /// @brief コンストラクタ
  /// @param[in] msg_mgr メッセージマネージャ
  /// @param[in] fd_mgr ファイル記述子を管理するクラス
  Lex(MsgMgr& msg_mgr,
      FileDescMgr& fd_mgr);

  /// @brief デストラクタ
  ~Lex();


public:

  /// @brief モジュールの状態を取得する．
  LexModuleState*
  module_state();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // モジュール関係のLEX状態
  LexModuleState* mModuleState;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_SCANNER_LEX_H
