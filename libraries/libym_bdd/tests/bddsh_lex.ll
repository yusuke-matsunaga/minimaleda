%{

/// @file libym_bdd/tests/bddsh_lex.ll
/// @brief Bdd のテスト用shellの字句解析用ソース
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: bddsh_lex.ll 700 2007-05-31 00:41:30Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#if defined(HAVE_CONFIG_H)
#include <ymconfig.h>
#endif

#include <ym_bdd/Bdd.h>
#include <ym_bdd/BddVarSet.h>
#include <ym_bdd/BddLitSet.h>

using namespace std;
using namespace nsYm::nsBdd;

#define YYSTYPE Bdd

#include "bddsh_parser.h"

string cur_text;
int cur_num; 
 
%}

A        [_a-zA-Z]
AN       [_a-zA-Z0-9]

N        [0-9]


%%

"true"   return TRUE;
"TRUE"   return TRUE;
"false"  return FALSE;
"FALSE"  return FALSE;
"~"      return TILDE;
"+"      return ADD;
"-"      return SUB;
"*"      return MUL;
"/"      return DIV;
"&"      return BAND;
"|"      return BOR;
"^"      return BXOR;
"&&"     return ANDAND;
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
"?"      return QMARK;
":"      return COLON;
"ite"    return ITE_OP;
"esm"    return ESM_OP;
"asm"    return ASM_OP;
","      return COMMA;
"="      return EQ;
"end"    return END;
"exit"   return END;
"quit"   return END;
";"      return SEMI;

{A}{AN}* {
  cur_text = string(yytext);
  return ID;
}

{N}* {
  cur_num = atoi(yytext);
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
