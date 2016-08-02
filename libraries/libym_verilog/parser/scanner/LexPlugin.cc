
/// @file libym_verilog/scanner/LexPlugin.cc
/// @brief LexPlugin の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: LexPlugin.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "LexPlugin.h"
#include "RawLex.h"

#include "parser.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// Lex に付随するプラグインの基底クラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] lex 親の Lex オブジェクト
// @param[in] name compiler directive 名
LexPlugin::LexPlugin(RawLex& lex,
		     const char* name) :
  mLex(lex),
  mName(name),
  mLink(NULL)
{
  mLex.add_plugin(name, this);
}
  
// @brief デストラクタ
LexPlugin::~LexPlugin()
{
}

// @brief 条件コンパイル用のプラグインの時 true を返す仮想関数
// @note デフォルトの実装では false を返す．
bool
LexPlugin::is_cond_plugin()
{
  return false;
}

// @brief マクロの時 true を返す仮想関数
// @note デフォルトの実装では false を返す．
bool
LexPlugin::is_macro()
{
  return false;
}

// @brief 指定されたトークンを期待して待つ．
// tok というトークンが読み出されるまで空白文字を読み飛ばす
// @return tok 以外の非空白トークンが読み出された場合には false を返す．
bool
LexPlugin::expect(int tok)
{
  int id = 0;
  do {
    id = mLex.get_raw_token();
  }
  while ( id == SPACE || id == COMMENT1 || id == COMMENT2 );
  return id == tok;
}

// @brief 空白文字以外の次のトークンが改行なら true を返す．
// ただし EOF でも true を返す．
// それ以外のトークンが来たら false を返す．
bool
LexPlugin::expect_nl()
{
  int id = 0;
  do {
    id = mLex.get_raw_token();
  }
  while ( id == SPACE || id == COMMENT1 || id == COMMENT2 );
  return id == NL;
}

// @brief 空白以外の次のトークンの取得
// @return 空白以外の次のトークンの id
int
LexPlugin::get_nospace_token()
{
  int id = 0;
  do {
    id = mLex.get_raw_token();
  }
  while ( id == SPACE || id == COMMENT1 || id == COMMENT2 ) ;
  return id;
}

END_NAMESPACE_YM_VERILOG
