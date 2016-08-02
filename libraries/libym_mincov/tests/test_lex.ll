%{

/// @file libym_mincov/tests/test_lex.ll
/// @brief Mincov のテスト用shellの字句解析用ソース
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: test_lex.ll 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mincov/mincov_nsdef.h"

  
BEGIN_NAMESPACE_YM_MINCOV
  
#include "test_parser.h"
  
END_NAMESPACE_YM_MINCOV
  
%}

N        [0-9]


%%

"init"       return INIT;
"clear"      return CLEAR;
"set"        return SET;
"set_cost"   return SET_COST;
"select_col" return SELECT_COL;
"delete_row" return DELETE_ROW;
"delete_col" return DELETE_COL;
"backup"     return BACKUP;
"restore"    return RESTORE;
"display"    return DISPLAY;
"exit"       return EXIT;

{N}* {
  nsYm::nsMincov::yylval = atoi(yytext);
  return NUMBER;
}

[ \t]                 ;
\r\n         return NL;
\r           return NL;
\n           return NL;

.            return yytext[0];
%%

int
yywrap()
{
  return 1;
}
