#ifndef LIBYM_VERILOG_SCANNER_STDLEXPLUGIN_H
#define LIBYM_VERILOG_SCANNER_STDLEXPLUGIN_H

/// @file libym_verilog/scanner/StdLexPlugin.h
/// @brief 標準のプラグインのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: StdLexPlugin.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "LexPlugin.h"

/// 仕様書覚書
///
/// 19.5 `include
///   - ネスト可能
///   - ただし無限(間接的/直接的に自分自身を読む)ループは不可
///
/// 19.6 `resetall
///   - コンパイラディレクティブの影響をリセットする．
///
/// 19.7 `line
///   - ファイル名と行番号の情報を上書きする．
///   - `resetall の影響を受けない．


BEGIN_NAMESPACE_YM_VERILOG

class InputMgr;

//////////////////////////////////////////////////////////////////////
/// @class LpInclude
/// @ingroup VlParser
/// @brief `include 用プラグイン
//////////////////////////////////////////////////////////////////////
class LpInclude :
  public LexPlugin
{
public:

  /// @brief コンストラクタ
  /// @param[in] lex 親の Lex オブジェクト
  /// @param[in] name compiler directive 名
  /// @param[in] input_mgr InputMgr
  LpInclude(RawLex& lex,
	    const char* name,
	    InputMgr* input_mgr);

  /// @brief デストラクタ
  virtual
  ~LpInclude();


public:

  /// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  parse();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力ファイルを管理するオブジェクト
  InputMgr* mInputMgr;

};


//////////////////////////////////////////////////////////////////////
/// @class LpLine
/// @ingroup VlParser
/// @brief `line 用プラグイン
//////////////////////////////////////////////////////////////////////
class LpLine :
  public LexPlugin
{
public:

  /// @brief コンストラクタ
  /// @param[in] lex 親の Lex オブジェクト
  /// @param[in] name compiler directive 名
  /// @param[in] input_mgr InputMgr
  LpLine(RawLex& lex,
	 const char* name,
	 InputMgr* input_mgr);

  /// @brief デストラクタ
  virtual
  ~LpLine();


public:

  /// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  parse();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力ファイルを管理するオブジェクト
  InputMgr* mInputMgr;
  
};


//////////////////////////////////////////////////////////////////////
/// @class LpResetAll
/// @ingroup VlParser
/// @brief `resetall 用プラグイン
//////////////////////////////////////////////////////////////////////
class LpResetAll :
  public LexPlugin
{
public:

  /// @brief コンストラクタ
  /// @param[in] lex 親の Lex オブジェクト
  /// @param[in] name compiler directive 名
  LpResetAll(RawLex& lex,
	     const char* name);

  /// @brief デストラクタ
  virtual
  ~LpResetAll();


public:

  /// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  parse();
  
};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_SCANNER_STDLEXPLUGIN_H
