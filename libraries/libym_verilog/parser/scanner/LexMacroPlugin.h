#ifndef LIBYM_VERILOG_SCANNER_LEXMACROPLUGIN_H
#define LIBYM_VERILOG_SCANNER_LEXMACROPLUGIN_H

/// @file libym_verilog/scanner/LexMacroPlugin.h
/// @brief 標準のプラグインのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: LexMacroPlugin.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "LexPlugin.h"
#include "TokenList.h"

/// 仕様書覚書
///
/// 19.3 `define and `undef
///   - マクロ置換は `resetall の影響を受けない
/// 19.3.1 `define
///   - `define はモジュール定義の外側でも内側でも使える．
///   - 定義されたマクロ名の先頭に '`' をつけたものが置換の対象となる．
///   - コンパイラディレクティブは「定義済みマクロ」とみなすことができる．
///   - コンパイラディレクティブと同名のマクロを定義することはできない
///   - 一般のマクロを多重定義することはできる．最後の定義が使用される．
///   - あるマクロが他のマクロを参照することは可能．その場合，参照される
///     マクロは参照時に置換される．定義時ではない．
///   - 自分自身への参照を含むマクロを定義することはできない．
/// 19.3.2 `undef
///   - 定義されていないマクロを `undef すると警告となる．


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class LpDefine
/// @ingroup VlParser
/// @brief `define 用プラグイン
//////////////////////////////////////////////////////////////////////
class LpDefine :
  public LexPlugin
{
public:

  /// @brief コンストラクタ
  /// @param[in] lex 親の Lex オブジェクト
  /// @param[in] name compiler directive 名
  LpDefine(RawLex& lex,
	   const char* name);

  /// @brief デストラクタ
  virtual
  ~LpDefine();


public:

  /// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  parse();
  
};


//////////////////////////////////////////////////////////////////////
/// @class LpUndef
/// @ingroup VlParser
/// @brief `undef 用プラグイン
//////////////////////////////////////////////////////////////////////
class LpUndef :
  public LexPlugin
{
public:

  /// @brief コンストラクタ
  /// @param[in] lex 親の Lex オブジェクト
  /// @param[in] name compiler directive 名
  LpUndef(RawLex& lex,
	  const char* name);

  /// @brief デストラクタ
  virtual
  ~LpUndef();


public:

  /// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  parse();
  
};


//////////////////////////////////////////////////////////////////////
/// @class LpMacro
/// @ingroup VlParser
/// @brief マクロ置換用プラグイン
//////////////////////////////////////////////////////////////////////
class LpMacro :
  public LexPlugin
{
  friend class LpDefine;
  
public:

  /// @brief コンストラクタ
  /// @param[in] lex 親の Lex オブジェクト
  /// @param[in] name compiler directive 名(マクロ名)
  /// @param[in] num_param パラメータ数
  LpMacro(RawLex& lex,
	  const char* name,
	  ymuint32 num_param);

  /// @brief デストラクタ
  virtual
  ~LpMacro();


public:

  /// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  parse();

  /// @brief マクロの時 true を返す仮想関数
  virtual
  bool
  is_macro();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // マクロ名
  StrBuff mName;

  // パラメータ数
  ymuint32 mNumParam;

  // 置き換えテンプレート
  TokenList mTokenList;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_SCANNER_LEXMACROPLUGIN_H
