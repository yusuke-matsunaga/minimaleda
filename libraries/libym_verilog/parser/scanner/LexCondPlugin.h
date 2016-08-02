#ifndef LIBYM_VERILOG_SCANNER_LEXCONDPLUGIN_H
#define LIBYM_VERILOG_SCANNER_LEXCONDPLUGIN_H

/// @file libym_verilog/scanner/LexCondPlugin.h
/// @brief LexCondPlugin のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: LexCondPlugin.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "LexPlugin.h"

/// 仕様書覚書
///
/// 19.4 `ifdef, `else, `elsif, `endif, `ifndef
///   - ソースファイル中のどこでも用いることができる．
///   - コンパイラディレクティブ名は「定義されている」とはみなされない．
///     つまり `ifdef ifdef は「偽」となる．
///   - 条件がなりたたずにスキップされる部分も Verilog-HDL の字句解析
///     の正しい文法規則を満たしていなければならない．


BEGIN_NAMESPACE_YM_VERILOG

class LexCondState;

//////////////////////////////////////////////////////////////////////
/// @class LexCondPlugin
/// @ingroup VlParser
/// @brief 条件コンパイル関係のプラグインの基底クラス
//////////////////////////////////////////////////////////////////////
class LexCondPlugin :
  public LexPlugin
{
protected:

  /// @brief コンストラクタ
  /// @param[in] lex 親の Lex オブジェクト
  /// @param[in] name compiler directive 名
  LexCondPlugin(RawLex& lex,
		const char* name,
		LexCondState* cond_state);

  /// @brief デストラクタ
  virtual
  ~LexCondPlugin();


public:
  //////////////////////////////////////////////////////////////////////
  // LexPlugin の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 条件コンパイル用のプラグインの時 true を返す仮想関数
  virtual
  bool
  is_cond_plugin();


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスが用いる便利関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief `ifdef/`ifndef 文の現在の条件の取得
  /// @retval true 条件が成り立っている
  /// @retval false 条件が成り立っていない
  bool
  cond();
  
  /// @brief condition の書き換え
  /// @param[in] flag 設定する値
  void
  set_cond(bool flag);
  
  /// @brief condition の反転
  void
  flip_cond();

  /// @brief true-nest-level の取得
  int
  true_nest_level();

  /// @brief true-nest-level を増やす
  void
  inc_true_nest_level();

  /// @brief true-nest-level を減らす
  void
  dec_true_nest_level();

  /// @brief false-nest-level の取得
  int
  false_nest_level();

  /// @brief false-nest-level を増やす
  void
  inc_false_nest_level();

  /// @brief false-nest-level を減らす
  void
  dec_false_nest_level();

  /// @brief else-flag の取得
  bool
  else_flag();

  /// @brief else-flag の設定
  void
  set_else_flag(bool flag);

  /// @brief else-flag の退避
  void
  push_else_flag(bool flag);

  /// @brief else-flag の復帰
  void
  pop_else_flag();
  
  /// @brief マクロ定義の検査
  /// @param[in] name 名前
  /// @return name という名のマクロが定義されていたら true を返す．
  bool
  is_macro_defined(const char* name);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 条件に関する状態
  LexCondState* mCondState;

};


//////////////////////////////////////////////////////////////////////
/// @class LpIfdef
/// @ingroup VlParser
/// @brief `ifdef/`ifndef 用プラグイン
//////////////////////////////////////////////////////////////////////
class LpIfdef :
  public LexCondPlugin
{
public:

  /// @brief コンストラクタ
  /// @param[in] lex 親の Lex オブジェクト
  /// @param[in] name compiler directive 名
  LpIfdef(RawLex& lex,
	  const char* name,
	  LexCondState* cond_state);

  /// @brief デストラクタ
  virtual
  ~LpIfdef();


public:

  /// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  parse();

};


//////////////////////////////////////////////////////////////////////
/// @class LpElse
/// @ingroup VlParser
/// @brief `else 用プラグイン
//////////////////////////////////////////////////////////////////////
class LpElse :
  public LexCondPlugin
{
public:

  /// @brief コンストラクタ
  /// @param[in] lex 親の Lex オブジェクト
  /// @param[in] name compiler directive 名
  LpElse(RawLex& lex,
	 const char* name,
	 LexCondState* cond_state);

  /// @brief デストラクタ
  virtual
  ~LpElse();


public:

  /// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  parse();

};


//////////////////////////////////////////////////////////////////////
/// @class LpElsif
/// @ingroup VlParser
/// @brief `elsif 用プラグイン
//////////////////////////////////////////////////////////////////////
class LpElsif :
  public LexCondPlugin
{
public:

  /// @brief コンストラクタ
  /// @param[in] lex 親の Lex オブジェクト
  /// @param[in] name compiler directive 名
  LpElsif(RawLex& lex,
	  const char* name,
	  LexCondState* cond_state);

  /// @brief デストラクタ
  virtual
  ~LpElsif();


public:

  /// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  parse();

};


//////////////////////////////////////////////////////////////////////
/// @class LpEndif
/// @ingroup VlParser
/// @brief `endif 用プラグイン
//////////////////////////////////////////////////////////////////////
class LpEndif :
  public LexCondPlugin
{
public:

  /// @brief コンストラクタ
  /// @param[in] lex 親の Lex オブジェクト
  /// @param[in] name compiler directive 名
  LpEndif(RawLex& lex,
	  const char* name,
	  LexCondState* cond_state);

  /// @brief デストラクタ
  virtual
  ~LpEndif();


public:

  /// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  parse();

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_SCANNER_LEXCONDPLUGIN_H
