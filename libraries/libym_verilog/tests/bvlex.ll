%{

/// @file libym_verilog/tests/bvlex.ll
/// @brief BitVector のテスト用簡易電卓の字句解析用ソース
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: bvlex.ll 700 2007-05-31 00:41:30Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include <ym_verilog/BitVector.h>

using namespace std;
using nsYm::nsVerilog::BitVector;

#define YYSTYPE BitVector

#include "bvcalc.h"

string cur_text;
 
%}

A        [_a-zA-Z]
AN       [_a-zA-Z0-9]

D        [0-9xXzZ?]
N        [1-9xXzZ?]
O        [0-7xXzZ?]
H        [0-9a-fA-FxXzZ?]
B        [01xXzZ?]

SZ       {N}{D}*

DB       {SZ}?'[sS]?[dD]
BB       {SZ}?'[sS]?[bB]
OB       {SZ}?'[sS]?[oO]
HB       {SZ}?'[sS]?[hH]

DV       {D}({D}|_)*
BV       {B}({B}|_)*
OV       {O}({O}|_)*
HV       {H}({H}|_)*

%%

"+"      return ADD;
"-"      return SUB;
"*"      return MUL;
"/"      return DIV;
"%"      return MOD;
"&"      return BAND;
"|"      return BOR;
"^"      return BXOR;
"!"      return NOT;
"&&"     return ANDAND;
"||"     return OROR;
"<<"     return LTLT;
">>"     return GTGT;
"<<<"    return LTLTLT;
">>>"    return GTGTGT;
"=="     return EQEQ;
"!="     return NOTEQ;
"<"      return LT;
">"      return GT;
"<="     return LTEQ;
">="     return GTEQ;
"("      return LP;
")"      return RP;
"["      return LB;
"]"      return RB;
":"      return COLON;
"ite"    return ITE;
"eqx"    return EQX;
"eqxz"   return EQXZ;
","      return COMMA;
"="      return EQ;
"str"    return STR;
"end"    return END;
"exit"   return END;
"quit"   return END;
";"      return SEMI;

{A}{AN}* {
  cur_text = string(yytext);
  return ID;
}

{D}*  |
{BB}{BV} |
{OB}{OV} |
{DB}{DV} |
{HB}{HV} {
  yylval.set_from_verilog_string(yytext);
  return NUMBER;
}

\".*\"    {
  cur_text = string(yytext);
  /* 最初と最後の " を取り除く */
  yylval = cur_text.substr(1, cur_text.size() - 2);
  return NUMBER;
}

[ \t]    ;
\r\n     ;
\r       ;
\n       ;

.        return yytext[0];
%%

int
yywrap()
{
  return 1;
}
