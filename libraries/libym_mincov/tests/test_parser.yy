%{

/// @file libym_mincov/tests/test_parser.yy
/// @brief Mincov のテスト用shellの構文解析用ソース
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: test_parser.yy 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mincov/mincov_nsdef.h"
#include "TestMatrix.h"


using namespace std;

extern
int yylex();
int yyerror(const char*);
 
  
BEGIN_NAMESPACE_YM_MINCOV
  
#include "test_parser.h"

#define YYSTYPE int
  
bool interactive = false;

void
prompt();
 
TestMatrix test_matrix(0, 0, 0);
 
%}

%token NUMBER
%token INIT
%token CLEAR
%token SET
%token SET_COST
%token SELECT_COL
%token DELETE_ROW
%token DELETE_COL
%token BACKUP
%token RESTORE
%token DISPLAY
%token EXIT
%token NL

%%

s
: list
;

list
:
| list line
;

line
: INIT NUMBER NUMBER NUMBER NL
{
  test_matrix.resize($2, $3, $4);
  prompt();
}
| CLEAR NL
{
  test_matrix.clear();
  prompt();
}
| SET NUMBER NUMBER NL
{
  test_matrix.set($2, $3);
  prompt();
}
| SET_COST NUMBER NUMBER NUMBER NL
{
  test_matrix.set_cost($2, $3, static_cast<double>($4));
  prompt();
}
| SELECT_COL NUMBER NL
{
  test_matrix.select_col($2);
  prompt();
}
| DELETE_ROW NUMBER NL
{
  test_matrix.delete_row($2);
  prompt();
}
| DELETE_COL NUMBER NL
{
  test_matrix.delete_col($2);
  prompt();
}
| BACKUP NL
{
  test_matrix.backup();
  prompt();
}
| RESTORE NL
{
  test_matrix.restore();
  prompt();
}
| DISPLAY NL
{
  test_matrix.display(cout);
  prompt();
}
| EXIT NL
{
  cout << "bye-bye" << endl;
  YYACCEPT;
}
;

%%

void
prompt()
{
  if ( interactive ) {
    cout << "$ " << flush;
  }
}

END_NAMESPACE_YM_MINCOV

int
yyerror(const char* s)
{
  using namespace std;
  
  cerr << s << endl;
  return 1;
}


int
main(int argc,
     char** argv)
{
  using namespace nsYm::nsMincov;
  
  extern FILE* yyin;
  if ( argc == 2 ) {
    yyin = fopen(argv[1], "r");
    if ( yyin == 0 ) {
      cerr << argv[1] << ": Could not open" << endl;
      return 2;
    }
    interactive = false;
  }
  else if ( argc == 1 ) {
    yyin = stdin;
    interactive = true;
  }
  else {
    cerr << "USAGE: " << argv[0] << " [command file]" << endl;
    return 1;
  }
  prompt();
  yyparse();
}
