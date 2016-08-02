%{

/// @file trans.y
/// @brief .bench(iscas89) フォーマット用の YACC ソース
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id:
///
/// Copyright (C) 2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_iscas89/iscas89_nsdef.h"
#include "ym_iscas89/Iscas89Handler.h"
  
#include "Iscas89ParserImpl.h"
#include "Iscas89Scanner.h"
  

// より詳細なエラー情報を出力させる．
#define YYERROR_VERBOSE 1

// トークンの形
#define YYSTYPE ymuint32

// 位置を表す型
// (yylloc の型)
#define YYLTYPE FileRegion

// YYLTYPE を書き換えたので以下のマクロも書き換えが必要
#define YYLLOC_DEFAULT(Current, Rhs, N)	Current = loc_merge(Rhs, N);
  
  
BEGIN_NAMESPACE_YM_ISCAS89

// yacc/bison が生成するヘッダファイル
#include "iscas89_grammer.h"

// 字句解析関数
int
yylex(YYSTYPE*,
      YYLTYPE*,
      Iscas89ParserImpl&);

// エラー報告関数
int
yyerror(YYLTYPE*,
	Iscas89ParserImpl&,
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
  return FileRegion(first, last);
}

END_NONAMESPACE
  
%}

// "pure" parser にする．
%pure-parser

// 位置のトラッキングを行う．
%locations

// yyparse の引数
%parse-param {Iscas89ParserImpl& parser}

// yylex の引数
%lex-param {Iscas89ParserImpl& parser}

%start  file

%token  INPUT
%token  OUTPUT
%token  BUFF
%token	NOT
%token  AND
%token  NAND
%token  OR
%token  NOR
%token  XOR
%token  XNOR
%token  DFF
%token  NAME

%%

file
: lines
;

lines
: line
| lines  line
;

line
: INPUT '(' NAME ')'
{
  if ( !parser.read_input(@$, $3) ) {
    YYERROR;
  }
}
| OUTPUT '(' NAME ')'
{
  if ( !parser.read_output(@$, $3) ) {
    YYERROR;
  }
}
| NAME '=' DFF '(' name1 ')'
{
  if ( !parser.read_gate(@$, $1, kGt89DFF) ) {
    YYERROR;
  }
}
| NAME '=' NOT '(' name1 ')'
{
  if ( !parser.read_gate(@$, $1, kGt89NOT) ) {
    YYERROR;
  }
}
| NAME '=' BUFF '(' name1 ')'
{
  if ( !parser.read_gate(@$, $1, kGt89BUFF) ) {
    YYERROR;
  }
}
| NAME '=' AND '(' name_list ')'
{
  if ( !parser.read_gate(@$, $1, kGt89AND) ) {
    YYERROR;
  }
}
| NAME '=' NAND '(' name_list ')'
{
  if ( !parser.read_gate(@$, $1, kGt89NAND) ) {
    YYERROR;
  }
}
| NAME '=' OR '(' name_list ')'
{
  if ( !parser.read_gate(@$, $1, kGt89OR) ) {
    YYERROR;
  }
}
| NAME '=' NOR '(' name_list ')'
{
  if ( !parser.read_gate(@$, $1, kGt89NOR) ) {
    YYERROR;
  }
}
| NAME '=' XOR '(' name_list ')'
{
  if ( !parser.read_gate(@$, $1, kGt89XOR) ) {
    YYERROR;
  }
}
| NAME '=' XNOR '(' name_list ')'
{
  if ( !parser.read_gate(@$, $1, kGt89XNOR) ) {
    YYERROR;
  }
}
| error ')'
{
  yyerrok;
}
;

name1
: NAME
{
  parser.push_str($1);
}
;

name_list
: NAME ',' NAME
{
  parser.push_str($1);
  parser.push_str($3);
}
| name_list ',' NAME
{
  parser.push_str($3);
}
;


%%

//////////////////////////////////////////////////////////////////////
// 関連する C++ のコード
//////////////////////////////////////////////////////////////////////

// loc_array 全体のファイル領域を求める．
FileRegion
loc_merge(const FileRegion loc_array[],
	  size_t n)
{
  const FileRegion& first = loc_array[1];
  const FileRegion& last = loc_array[n];
  return FileRegion(first.file_desc(),
		    first.start_line(), first.start_column(),
		    last.end_line(), last.end_column());
}

// yacc パーサーが内部で呼び出す関数
// トークンを一つ読み込みトークン番号を返す．
// 同時に意味値(semantic value)を yylval にセットする．
// また位置を yylloc にセットする．
int
yylex(YYSTYPE *lvalp,
      YYLTYPE *llocp,
      Iscas89ParserImpl& parser)
{
  return parser.scan(*lvalp, *llocp);
}

// yacc パーサーが内部で呼び出す関数
// エラーメッセージを出力する．
int
yyerror(YYLTYPE *llocp,
	Iscas89ParserImpl& parser,
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
			   kMsgError, "ER_SYN01", msg);

  return 1;
}

END_NAMESPACE_YM_ISCAS89
