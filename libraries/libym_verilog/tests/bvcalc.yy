%{

/// @file libym_verilog/tests/bvcal.yy
/// @brief BitVector のテスト用簡易電卓の構文解析用ソース
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: bvcalc.yy 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include <ym_verilog/BitVector.h>

using namespace std;
using HASH_NAMESPACE::hash_map;
using nsYm::nsVerilog::BitVector;

extern int yylex();
int yyerror(const char*);
void print_value(const BitVector& v);

extern string cur_text;

#define YYSTYPE BitVector

hash_map<string, BitVector> id_table;
string lname;
%}

%token NUMBER
%token ID
%token SEMI
%token EQ
%token LP
%token RP
%token LB
%token RB
%token COLON
%token ID
%token ITE
%token EQX
%token EQXZ
%token STR
%token COMMA
%token END

%left OROR
%left ANDAND
%left BOR
%left BAND BXOR
%left EQEQ NOTEQ
%left LT LTEQ GT GTEQ
%left LTLT GTGT LTLTLT GTGTGT
%left ADD SUB
%left MUL DIV MOD
%right POW
%nonassoc UMINUS TILDE NOT

%%

s
: list
;

list
:
| list line
;

line
: expr SEMI
{
  print_value($1);
  cout << "$ " << flush;
}
| ID { lname = cur_text; } EQ expr SEMI
{
  id_table[lname] = $4;
  cout << $4 << " is stored in " << lname << endl
       << "$ " << flush;
}
| END
{
  cout << "bye-bye" << endl;
  YYACCEPT;
}
| error SEMI
{
  yyerrok;
  cout << "$ " << flush;
}
;

expr
: expr ADD expr
{
  $$ = $1 + $3;
}
| expr SUB expr
{
  $$ = $1 - $3;
}
| expr MUL expr
{
  $$ = $1 * $3;
}
| expr DIV expr
{
  $$ = $1 / $3;
}
| expr MOD expr
{
  $$ = $1 % $3;
}
| expr BAND expr
{
  $$ = $1 & $3;
}
| expr BOR expr
{
  $$ = $1 | $3;
}
| expr BXOR expr
{
  $$ = $1 ^ $3;
}
| TILDE expr
{
  $$ = ~$2;
}
| LP expr RP
{
  $$ = $2;
}
| SUB expr %prec UMINUS
{
  $$ = -$2;
}
| BAND expr %prec UMINUS
{
  $$ = $2.reduction_and();
}
| BOR expr %prec UMINUS
{
  $$ = $2.reduction_or();
}
| BXOR expr %prec UMINUS
{
  $$ = $2.reduction_xor();
}
| expr EQEQ expr
{
  $$ = $1 == $3;
}
| expr NOTEQ expr
{
  $$ = $1 != $3;
}
| expr LT expr
{
  $$ = $1 < $3;
}
| expr GT expr
{
  $$ = $1 > $3;
}
| expr LTEQ expr
{
  $$ = $1 <= $3;
}
| expr GTEQ expr
{
  $$ = $1 >= $3;
}
| expr LTLT expr
{
  $$ = $1 << $3;
}
| expr GTGT expr
{
  $$ = $1 >> $3;
}
| expr LTLTLT expr
{
  $$ = alshift($1, $3);
}
| expr GTGTGT expr
{
  $$ = arshift($1, $3);
}
| NOT expr
{
  $$ = !$2;
}
| expr ANDAND expr
{
  $$ = $1 && $3;
}
| expr OROR expr
{
  $$ = $1 || $3;
}
| ITE LP expr COMMA expr COMMA expr RP
{
  $$ = ite($3, $5, $7);
}
| EQX LP expr COMMA expr RP
{
  $$ = eq_with_x($3, $5);
}
| EQXZ LP expr COMMA expr RP
{
  $$ = eq_with_xz($3, $5);
}
| NUMBER
{
  $$ = $1;
}
| variable
{
  $$ = $1;
}
| variable LB NUMBER RB
{
  $$ = $1.bit_select($3.to_int());
}
| variable LB NUMBER COLON NUMBER RB
{
  $$ = $1.part_select($3.to_int(), $5.to_int());
}
| STR LP expr RP
{
  cout << $3.to_string() << endl;
  $$ = $3;
}
;

variable
: ID
{
  hash_map<string, BitVector>::iterator p = id_table.find(cur_text);
  if ( p == id_table.end() ) {
    cerr << cur_text << " : not defined" << endl;
    YYERROR;
  }
  $$ = p->second;
}
;

%%

int
yyerror(const char* s)
{
  cerr << s << endl;
  return 1;
}

int
main(int argc,
     char** argv)
{
  cout << "$ " << flush;
  yyparse();

  return 0;
}

void
print_value(const BitVector& v)
{
  cout << v.verilog_string() << "\t\t";
  if ( v.is_signed() ) {
    cout << "  signed  ";
  }
  else {
    cout << " unsigned ";
  }
  if ( v.is_sized() ) {
    cout << "size = " << v.size();
  }
  else {
    cout << "  unsized  ";
  }
  cout << " : "
       << v.bin_str(false)
       << endl;
}
