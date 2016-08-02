%{ // C++ 宣言部

/// @file libym_mislib/mislib_grammer.yy
/// @brief MIS-II/SIS用セルライブラリのパーザ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: mislib_grammer.yy 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.

#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include <ym_mislib/mislib_nsdef.h>
  
#include <ym_utils/ShString.h>
#include <ym_utils/FileRegion.h>

#include "MislibParserImpl.h"
#include "MislibLex.h"
#include "MislibPt.h"
  

// より詳細なエラー情報を出力させる．
#define YYERROR_VERBOSE 1
  
// 解析結果の型
#define YYSTYPE MislibPt*

// 位置を表す型
// (yylloc の型)
#define YYLTYPE FileRegion

// YYLTYPE を書き換えたので以下のマクロも書き換えが必要
#define YYLLOC_DEFAULT(Current, Rhs, N) Current = loc_merge(Rhs, N);

  
BEGIN_NAMESPACE_YM_MISLIB

// 字句解析関数の宣言
int
yylex(YYSTYPE*,
      YYLTYPE*,
      MislibParserImpl&);
 
// エラー報告関数の宣言
int
yyerror(YYLTYPE*,
	MislibParserImpl&,
	const char*);
 
BEGIN_NONAMESPACE

// loc_array 全体のファイル領域を求める．
inline
FileRegion
loc_merge(const FileRegion loc_array[],
	  size_t n)
{
  const FileRegion& first = loc_array[1];
  const FileRegion& last = loc_array[n];
  return FileRegion(first.start_loc(), last.end_loc());
}

END_NONAMESPACE

%} // C++ 宣言部おわり

// "pure" parser にする．
%pure-parser

// 位置のトラッキングを行う．
%locations

// yyparse の引数
%parse-param {MislibParserImpl& parser}

// yylex の引数
%lex-param {MislibParserImpl& parser}

// トークンの定義
%token STR
%token NUM

%token LP RP
%token SEMI
%token EQ

%token GATE
%token PIN

%token NONINV;
%token INV;
%token UNKNOWN;

%token CONST0
%token CONST1

%token ERROR

// 論理式の演算子は優先度に注意
%left PLUS
%left STAR
%nonassoc NOT

%start file

%%

//////////////////////////////////////////////////////////////////////
// YACC 本体定義
//////////////////////////////////////////////////////////////////////

// genlib(mislib) ファイルはゲートの集まり
file
: gate
| file gate
;

// ゲート
gate
: GATE STR NUM STR EQ expr SEMI pin_list
{ // 通常のパタン
  parser.new_gate1(@$, $2, $3, $4, $6, $8);
}
| GATE STR NUM STR EQ expr SEMI star_pin
{ // ワイルドカード '*' を使ったピン定義
  parser.new_gate2(@$, $2, $3, $4, $6, $8);
}
| GATE STR NUM STR EQ expr SEMI
{ // 入力ピンのないパタン (定数セル)
  parser.new_gate3(@$, $2, $3, $4, $6);
}
| GATE error
{
  yyclearin;
  yyerrok;
}
;

// 出力を表す論理式
expr
: LP expr RP
{
  $$ = $2;
}
| STR
{
  $$ = $1;
}
| NOT expr
{
  $$ = parser.new_not(@$, $2);
} 
| expr STAR expr
{
  $$ = parser.new_and(@$, $1, $3);
}
| expr PLUS expr
{
  $$ = parser.new_or(@$, $1, $3);
}
| CONST0
{
  $$ = $1;
}
| CONST1
{
  $$ = $1;
}
;

// ピンリスト
pin_list
: pin
{
  $$ = parser.new_pinlist();
  $$->push_back($1);
}
| pin_list pin
{
  ($$ = $1)->push_back($2);
}
;

// 入力ピン
pin
: PIN STR phase NUM NUM NUM NUM NUM NUM
{
  $$ = parser.new_pin(@$, $2, $3, $4, $5, $6, $7, $8, $9);
}
;

// ワイルドカードを使ったピン定義
star_pin
: PIN STAR phase NUM NUM NUM NUM NUM NUM
{
  // STAR(*) はワイルドカード
  $$ = parser.new_pin(@$, NULL, $3, $4, $5, $6, $7, $8, $9);
}

// 極性情報
phase
: NONINV
{
  $$ = $1;
}
| INV
{
  $$ = $1;
}
| UNKNOWN
{
  $$ = $1;
}
;

%%


//////////////////////////////////////////////////////////////////////
// 関連する C++ のコード
//////////////////////////////////////////////////////////////////////

// 字句解析器
// 実際には MislibLex が仕事をしている．
int
yylex(YYSTYPE* lvalp,
      YYLTYPE* llocp,
      MislibParserImpl& parser)
{
  return parser.scan(*lvalp, *llocp);
}

/// エラー出力関数
int
yyerror(YYLTYPE* llocp,
	MislibParserImpl& parser,
	const char* msg)
{
  string buff;
  const char* msg2;
  // 好みの問題だけど "parse error" よりは "syntax error" の方が好き．
  if ( !strncmp(msg, "parse error", 11) ) {
    buff ="syntax error";
    buff += (msg + 11);
    msg2 = buff.c_str();
  }
  else {
    msg2 = msg;
  }
  
  parser.msg_mgr().put_msg(__FILE__, __LINE__, *llocp,
			   kMsgError, "MISLIB_PARSER", msg2);
  
  return 1;
}

END_NAMESPACE_YM_MISLIB
