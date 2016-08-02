%{

/// @file libym_verilog/parser/verilog_parser.yy
/// @brief Verilog-HDL (IEEE1364-2001) パーサ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: verilog_grammer.yy 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/FileRegion.h"
#include "ym_verilog/pt/PtP.h"

#include "Parser.h"
#include "Parser_port.h"
#include "Parser_decl.h"
#include "Parser_item.h"
#include "Parser_stmt.h"
#include "Parser_expr.h"
#include "Parser_misc.h"

#include "PuHierName.h"
#include "PtrList.h"


// より詳細なエラー情報を出力させる．
#define YYERROR_VERBOSE 1

// 位置を表す型
// (yylloc の型)
#define YYLTYPE FileRegion

// YYLTYPE を書き換えたので以下のマクロも書き換えが必要
#define YYLLOC_DEFAULT(Current, Rhs, N) Current = fr_merge(Rhs, N);


BEGIN_NAMESPACE_YM_VERILOG

// yacc/bison が生成したヘッダファイル
#include "verilog_grammer.h"


// 字句解析関数
int
yylex(YYSTYPE*,
      YYLTYPE*,
      Parser&);

// エラー報告関数
int
yyerror(YYLTYPE*,
	Parser&,
	const char*);


// fr_array 全体のファイル領域を求める．
// 直感的には FileRegion(fr_array[1], fr_array[n])
// だが(先頭が 0 でなく 1 であることに注意),
// 場合によっては空のトークンで位置がないばあいがあるので
// それをスキップしなければならない．
inline
FileRegion
fr_merge(const FileRegion fr_array[],
	 size_t n)
{
  if ( n == 0 ) {
    // なんでこんなことがあるのか不明
    return FileRegion();
  }

  // 真の先頭を求める．
  size_t i;
  for (i = 1; i <= n && !fr_array[i].is_valid(); ++ i) ;
  const FileRegion& first = fr_array[i];

  // 真の末尾を求める．
  size_t j;
  for (j = n; i >= i && !fr_array[j].is_valid(); -- j) ;
  const FileRegion& last = fr_array[j];

  return FileRegion(first, last);
}

%}

// "pure" parser にする．
%define api.pure

// 位置のトラッキングを行う．
%locations

// yyparse の引数
%parse-param {Parser& parser}

// yylex の引数
%lex-param {Parser& parser}


// トークンの型
%union {
  int inttype;
  double dbltype;
  unsigned long uinttype;
  const char* strtype;

  tVpiVarType vartype;
  tVpiNetType nettype;
  tVpiPrimType primtype;
  tVpiOpType optype;
  tVpiConstType consttype;
  tVpiStrength strengthtype;
  tVpiRangeMode rangemode;
  tVpiVsType vstype;
  tVpiUdpVal udpsymbol;

  PuHierName* hiername;

  PtGenCaseItem* gencaseitem;
  PtPathDecl* pathdecl;
  PtPathDelay* pathdelay;

  PtStmt* stmt;
  PtCaseItem* caseitem;

  PtExpr* expr;

  PtStrength* strength;
  PtDelay* delay;
  PtControl* control;
  PtConnection* connection;

  PtAttrInst* attrinst;
  PtAttrSpec* attrspec;

  PtrList<PtGenCaseItem>* gcilist;
  PtrList<PtStmt>* stmtlist;
  PtrList<PtCaseItem>* cilist;
  PtrList<PtRange>* rangelist;
  PtrList<PtExpr>* exprlist;
  PtrList<PtConnection>* conlist;
  PtrList<PtAttrInst>* ailist;
  PtrList<PtAttrSpec>* aslist;
}

// Lex 内部のみで用いられるトークン
%token ERROR                // エラーを表す．
%token SPACE                // ' ', '\t', eof
%token NL                   // '\r', '\n' or "\r\n"
%token CD_SYMBOL            // `ではじまる文字列
%token COMMENT1             // "//" 〜 改行
%token COMMENT2             // "/*" 〜 "*/"

// 値を持つ終端記号
%token <strtype> IDENTIFIER          // 識別子
%token <strtype> SYS_IDENTIFIER      // '$'で始まる文字列
%token <strtype> STRING              // 文字列
%token <strtype> UNUMBER             // unsigned number (in string)
%token <strtype> UNUM_BIG            // big unsigned number (in string)
%token <uinttype> UNUM_INT           // unsigned number
%token <dbltype> RNUMBER             // real number

// 値を持たない終端記号
%token BASE_B               // 'b
%token BASE_SB              // 'sb
%token BASE_O               // 'o
%token BASE_SO              // 'so
%token BASE_D               // 'd
%token BASE_SD              // 'sd
%token BASE_H               // 'h
%token BASE_SH              // 'sh
%token TILDEAND             // ~&
%token TILDEOR              // ~|
%token TILDEXOR             // ~^
%token LTLT                 // <<
%token LTLTLT               // <<<
%token GTGT                 // >>
%token GTGTGT               // >>>
%token LTEQ                 // <=
%token GTEQ                 // >=
%token EQEQ                 // ==
%token NOTEQ                // !=
%token EQEQEQ               // ===
%token NOTEQEQ              // !==
%token ANDAND               // &&
%token ANDANDAND            // &&&
%token OROR                 // ||
%token STARSTAR             // **
%token EQGT                 // =>
%token STARGT               // *>
%token MINUSGT              // ->
%token PRSTAR               // (*
%token STARPR               // *)
%token PLUSCOLON            // +:
%token MINUSCOLON           // -:
%token ';'
%token ':'
%token '['
%token ']'
%token '{'
%token '}'
%token '('
%token ')'
%token ','
%token '.'
%token '@'
%token '#'
%token '%'
%token '?'
%token '+'
%token '-'
%token '*'
%token '/'
%token '^'
%token '!'
%token '&'
%token '|'
%token '~'
%token '<'
%token '>'
%token '='
%token '$'
%token '0'
%token '1'
%token 'x'
%token 'b'
%token 'r'
%token 'f'
%token 'n'
%token 'p'

// 予約語
%token ALWAYS               // "always"
%token AND                  // "and"
%token ASSIGN               // "assign"
%token AUTOMATIC            // "automatic"
%token BEGIN                // "begin"
%token BUF                  // "buf"
%token BUFIF0               // "bufif0"
%token BUFIF1               // "bufif1"
%token CASE                 // "case"
%token CASEX                // "casex"
%token CASEZ                // "casez"
%token CELL                 // "cell"
%token CMOS                 // "cmos"
%token CONFIG               // "config"
%token DEASSIGN             // "deassign"
%token DEFAULT              // "default"
%token DEFPARAM             // "defparam"
%token DESIGN               // "design"
%token DISABLE              // "disable"
%token EDGE                 // "edge"
%token ELSE                 // "else"
%token END                  // "end"
%token ENDCASE              // "endcase"
%token ENDCONFIG            // "endconfig"
%token ENDFUNCTION          // "endfunction"
%token ENDGENERATE          // "endgenerate"
%token ENDMODULE            // "endmodule"
%token ENDPRIMITIVE         // "endprimitive"
%token ENDSPECIFY           // "endspecify"
%token ENDTABLE             // "endtable"
%token ENDTASK              // "endtask"
%token EVENT                // "event"
%token FOR                  // "for"
%token FORCE                // "force"
%token FOREVER              // "forever"
%token FORK                 // "fork"
%token FUNCTION             // "function"
%token GENERATE             // "generate"
%token GENVAR               // "genvar"
%token HIGHZ0               // "highz0"
%token HIGHZ1               // "highz1"
%token IF                   // "if"
%token INITIAL              // "initial"
%token IFNONE               // "ifnone"
%token INCDIR               // "incdir"
%token INCLUDE              // "include"
%token INOUT                // "inout"
%token INPUT                // "input"
%token INSTANCE             // "instance"
%token INTEGER              // "integer"
%token JOIN                 // "join"
%token LARGE                // "large"
%token LIBLIST              // "liblist"
%token LIBRARY              // "library"
%token LOCALPARAM           // "localparam"
%token MACROMODULE          // "macromodule"
%token MEDIUM               // "medium"
%token MODULE               // "module"
%token NAND                 // "nand"
%token NEGEDGE              // "negedge"
%token NMOS                 // "nmos"
%token NONE                 // "none" `default_nettype のみで用いる．
%token NOR                  // "nor"
%token NOSHOWCANCELLED      // "noshowcancelled"
%token NOT                  // "not"
%token NOTIF0               // "notif0"
%token NOTIF1               // "notif1"
%token OR                   // "or"
%token OUTPUT               // "output"
%token PARAMETER            // "parameter"
%token PMOS                 // "pmos"
%token POSEDGE              // "posedge"
%token PRIMITIVE            // "primitive"
%token PULL0                // "pull0"
%token PULL1                // "pull1"
%token PULLUP               // "pullup"
%token PULLDOWN             // "pulldown"
%token PULSESTYLE_ONEVENT   // "pulsestyle_onevent"
%token PULSESTYLE_ONDETECT  // "pulsestyle_ondetect"
%token RCMOS                // "rcmos"
%token REAL                 // "real"
%token REALTIME             // "realtime"
%token REG                  // "reg"
%token RELEASE              // "release"
%token REPEAT               // "repeat"
%token RNMOS                // "rnmos"
%token RPMOS                // "rpmos"
%token RTRAN                // "rtran"
%token RTRANIF0             // "rtranif0"
%token RTRANIF1             // "rtranif1"
%token SCALARED             // "scalared"
%token SHOWCANCELLED        // "showcancelled"
%token SIGNED               // "signed"
%token SMALL                // "small"
%token SPECIFY              // "specify"
%token SPECPARAM            // "specparam"
%token STRONG0              // "strong0"
%token STRONG1              // "strong1"
%token SUPPLY0              // "supply0"
%token SUPPLY1              // "supply1"
%token TABLE                // "table"
%token TASK                 // "task"
%token TIME                 // "time"
%token TRAN                 // "tran"
%token TRANIF0              // "tranif0"
%token TRANIF1              // "tranif1"
%token TRI                  // "tri"
%token TRI0                 // "tri0"
%token TRI1                 // "tri1"
%token TRIAND               // "triand"
%token TRIOR                // "trior"
%token TRIREG               // "trireg"
%token UNSIGNED             // "unsigned"
%token USE                  // "use"
%token VECTORED             // "vectored"
%token WAIT                 // "wait"
%token WAND                 // "wand"
%token WEAK0                // "weak0"
%token WEAK1                // "weak1"
%token WHILE                // "while"
%token WIRE                 // "wire"
%token WOR                  // "wor"
%token XOR                  // "xor"
%token XNOR                 // "xnor"

%token SETUP                // "$setup"
%token HOLD                 // "$hold"
%token SETUPHOLD            // "$setuphold"
%token RECOVERY             // "$recovery"
%token REMOVAL              // "$removal"
%token RECREM               // "$recrem"
%token SKEW                 // "$skew"
%token TIMESKEW             // "$timeskew"
%token FULLSKEW             // "$fullskew"
%token PERIOD               // "$period"
%token WIDTH                // "$width"
%token NOCHANGE             // "$nochange"

%token PATHPULSE            // "PATHPULSE$"

// 演算子の定義
%left  COND                         // ? :
%left  OROR                         // ||
%left  ANDAND                       // &&
%left  '|' TILDEOR                  // |, ~|
%left  '&' TILDEAND '^' TILDEXOR    // &, ~&, ^, ~^(^~)
%left  EQEQ NOTEQ EQEQEQ NOTEQEQ    // ==, !=, ===, !==
%left  '<' LTEQ '>' GTEQ            // <, <=, >, >=
%left  LTLT GTGT LTLTLT GTGTGT      // <<, >>, <<<, >>>
%left  '+' '-'                      // +, -
%left  '*' '/' '%'                  // *, /, %
%right STARSTAR                     // **
%right '!' '~' UOP                  // !, ~, (単項演算子)

// dangling else 解消のための優先順位
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

// 非終端ノードの型定義
// 何かもっといい方法があるような気がする．
%type <inttype> module_keyword

%type <inttype> opt_auto

%type <strtype> mu_head

%type <stmtlist> nzlist_of_stmt

%type <stmt> blocking_assignment nonblocking_assignment
%type <stmt> procedural_continuous_assignments
%type <stmt> variable_assignment
%type <stmt> par_block seq_block
%type <stmt> statement statement_or_null
%type <stmt> disable_statement event_trigger
%type <stmt> procedural_timing_control_statement
%type <stmt> wait_statement conditional_statement case_statement
%type <stmt> loop_statement system_task_enable task_enable

%type <exprlist> nzlist_of_expressions
%type <exprlist> nzlist_of_lvalues
%type <conlist> n_input_gate_terminals n_output_gate_terminals
%type <exprlist> nzlist_of_terminals
%type <exprlist> nzlist_of_arguments

%type <expr> concatenation
%type <expr> multiple_concatenation
%type <expr> function_call
%type <expr> system_function_call
%type <expr> expression expr1 primary
%type <expr> module_path_expression
%type <expr> net_lvalue variable_lvalue lvalue
%type <expr> number unumber rnumber
%type <expr> argument

%type <rangelist> nzlist_of_dimensions

%type <exprlist> nzlist_of_index

%type <expr> index

%type <nettype> net_type
%type <vstype> vstype
%type <inttype> sign

%type <gcilist> list_of_gencaseitem
%type <gencaseitem> genvar_case_item

%type <strength> drive_strength charge_strength
%type <strength> pulldown_strength pullup_strength
%type <strengthtype> strength0 strength1

%type <delay> delay3 delay2
%type <expr> delay_value delay_value_x delay_value_y
%type <expr> path_delay_expression
%type <expr> mintypmax_expression

%type <vartype> task_port_type
%type <vartype> data_type
%type <primtype> cmos_switchtype enable_gatetype mos_switchtype
%type <primtype> n_input_gatetype n_output_gatetype pass_en_switchtype
%type <primtype> pass_switchtype

%type <conlist> list_of_ordered_param_assign
%type <conlist> list_of_ordered_port_connections
%type <conlist> list_of_named_param_assign
%type <conlist> list_of_named_port_connections
%type <connection> named_parameter_assignment
%type <connection> named_port_connection
%type <connection> ordered_port_connection

%type <expr> init_val

%type <udpsymbol> next_state output_symbol level_symbol edge_symbol

%type <control> delay_control
%type <control> event_control
%type <control> repeat_control
%type <exprlist> event_expression
%type <expr> event_primary
%type <cilist> list_of_case_items
%type <caseitem> case_item
%type <pathdecl> path_declaration simple_path_declaration
                 edge_sensitive_path_declaration
%type <expr> specify_terminal
%type <pathdelay> path_delay_value list_of_path_delay_expressions
%type <inttype> pol_op pol_colon
%type <inttype> edge

/* まだできていない */
%type <inttype> system_timing_check
%type <inttype> timing_check_options
%type <inttype> skew_timing_check_options
%type <inttype> opt_notify_reg
%type <inttype> delayed_data
%type <inttype> timing_check_event
%type <inttype> controlled_timing_check_event
%type <inttype> opt_timing_check_event_control
%type <inttype> timing_check_event_control
%type <inttype> edge_descriptor
%type <inttype> opt_timing_check_condition
%type <inttype> timing_check_condition
/* end-of まだできていない */

%type <consttype> numbase
%type <ailist> ai_list nz_ai_list
%type <attrinst> attr_inst
%type <aslist> nzlist_of_attr_spec
%type <attrspec> attr_spec
%type <hiername> hierarchical_identifier

%%

/*
//////////////////////////////////////////////////////////////////////
// A.1.1 Library source text
//////////////////////////////////////////////////////////////////////
library_text
: // 空もありうる
| library_text library_description
;

library_description
: library_declaration
| include_statement
| config_declaration
;

library_declaration
: LIBRARY NAME file_path_list incdir_list ';'
| LIBRARY error ';'
;

file_path_list
: file_path
| file_path_list ',' file_path
;

incdir_list
: // 空もありうる
| INCDIR file_path_list
;

include_statement
: INCLUDE '<' file_path '>'
;


//////////////////////////////////////////////////////////////////////
// A.1.2 Configuration source text
//////////////////////////////////////////////////////////////////////
config_declaration
: CONFIG NAME ';'
design_statement
'{' config_rule_statement '}'
ENDCONFIG
;

design_statement
: DESIGN '{' cell_descriptor '}' ';'
;

config_rule_statement
: DEFAULT liblist_clause
| inst_clause liblist_clause
| inst_clause use_cluase
| cell_clause liblist_clause
| cell_clause use_clause
;

inst_clause
: INSTANCE inst_name
;

inst_name
: NAME
| inst_name '.' NAME
;

cell_clause
: CELL cell_descriptor
;

liblist_clause
: LIBLIST library_list
;

library_list
: // 空もありうる
| library_list NAME
;

use_clause
: USE cell_descriptor
| USE cell_descriptor ':' config
;

cell_descriptor
: NAME
| NAME '.' NAME
;
*/


//////////////////////////////////////////////////////////////////////
// A.1.3 Module and primitive source text
//////////////////////////////////////////////////////////////////////

// [SPEC] source_text ::= { description }
source_text
: // 空もありうる．
| source_text description
;

// [SPEC] description ::=
//            module_declaration
//           |udp_declaration
description
: module_declaration
| udp_declaration
;

// [SPEC] module_declaration ::=
//            { attribute_instance } module_keyword module_identifier
//                [ module_parameter_port_list ] [ list_of_ports ] ';'
//                { module_item }
//            "endmodule"
//           |{ attribute_instance } module_keyword module_identifier
//                [ module_parameter_port_list ]
//                [ list_of_port_declarations ] ';'
//                { non_port_module_item }
//            "endmodule"
// [SPEC] module_identifier ::= identifier
// 問題は list_of_ports と list_of_port_declarations が共に空の時に
// 区別がつかなくなってしまうということ．
// そのため，list_of_ports も list_of_port_declarations も空ではなくし，
// 代わりに第3番目の形式を追加している．
// もともと port は空もありうるので nzlist_of_ports が名前とは裏腹に
// 空のリストも表している．ただし括弧は含まれる．
// 3番目の形式は括弧も含まない．
// non_port_module_item は module_item でパーズしておいて
// 中でチェックした方がエラーメッセージが分かりやすい．
// yacc の構文にしてしまうとただの syntax error となってしまう．
module_declaration
: ai_list module_keyword IDENTIFIER
  module_parameter_port_list
  '(' list_of_ports ')' ';'
  list_of_module_items
  ENDMODULE
{
  parser.new_Module1995(@$, $2, $3, $1);
}
| ai_list module_keyword IDENTIFIER
  module_parameter_port_list module_portdecl_list ';'
  list_of_module_items2
  ENDMODULE
{
  parser.new_Module2001(@$, $2, $3, $1);
}
| ai_list module_keyword error ENDMODULE
{
  parser.end_module();
  yyerrok;
}
;

// [SPEC] module_keyword ::= "module" | "macromodule"
module_keyword
: MODULE
{
  // ちょっとした hack!
  parser.init_module();
  $$ = false;
}
| MACROMODULE
{
  // ちょっとした hack!
  parser.init_module();
  $$ = true;
}
;

module_portdecl_list
: // 空もあり
| '(' list_of_port_declarations ')'
{
  parser.end_io();
}


//////////////////////////////////////////////////////////////////////
// A.1.4 Module parameters and ports
//////////////////////////////////////////////////////////////////////

// [SPEC] module_parameter_port_list ::=
//            '#' '(' parameter_declaration {',' parameter_declaration} ')'
// ほとんどそのままの翻訳．ただし [ module_parameter_port_list ]
// という形で使われるので空の場合も含めている．
// しかしこれ仕様書通りだと parameter_declaration の末尾はセミコロン
// なんですけど,,,,,
module_parameter_port_list
: // 空
| '#' '(' list_of_paramport_decl ')'
{
  parser.end_paramport();
}
;

// [SPEC*] list_of_param_decl ::=
//           parameter_declaration { ',' parameter_declaration }
list_of_paramport_decl
: paramport_head paramport_assignment
| list_of_paramport_decl ',' { parser.end_paramport(); }
  paramport_head paramport_assignment
| list_of_paramport_decl ',' paramport_assignment
;

paramport_head
: ai_list PARAMETER
{
  parser.new_ParamH(@$, $1, true);
}
| ai_list PARAMETER sign '[' expression ':' expression ']'
{
  parser.new_ParamH(@$, $3, $5, $7, $1, true);
}
| ai_list PARAMETER data_type
{
  parser.new_ParamH(@$, $3, $1, true);
}
;

// [SPEC] param_assignment ::= parameter_identifier '=' constant_expression
// [SPEC] parameter_identifier ::= identifier
paramport_assignment
: IDENTIFIER '=' expression
{
  parser.new_DeclItem(@$, $1, $3);
}
;

// [SPEC] list_of_ports ::= '(' port {',' port} ')'
list_of_ports
:                   { parser.init_portref_list(); } port
| list_of_ports ',' { parser.init_portref_list(); } port
;

// [SPEC] port ::=
//             [port_expression]
//            |'.' port_identifier '(' [port_expression] ')'
// [SPEC] port_identifier ::= identifier
port
: // 空
{
  // でも NULL を返さない．
  parser.new_Port();
}
| port_expression
{
  // 内側の式のみ指定するタイプ
  parser.new_Port1(@$);
}
| '.' IDENTIFIER '('                 ')'
{
  // 外側の名前のみ指定するタイプ
  parser.new_Port2(@$, $2);
}
| '.' IDENTIFIER '(' port_expression ')'
{
  // 外側の名前と内側の式を指定するタイプ
  parser.new_Port3(@$, $2);
}
;

// [SPEC] port_expression ::=
//            port_reference
//           |'{' port_reference { ',' port_reference } '}'
port_expression
: port_reference
| '{' nzlist_of_port_references '}'
;

// [SPEC*] nzlist_of_port_references ::=
//              port_reference { ',' port_reference }
nzlist_of_port_references
: port_reference
| nzlist_of_port_references ',' port_reference
;


// [SPEC] port_reference ::= port_identifier
//            port_identifier '[' constant_expression ']'
//           |port_identifier '[' range_expression ']'
// [SPEC] port_identifier ::= identifier
// オリジナルは range_expression だけどこれは
// constant_range_expression の誤りでしょ．
// さらにオリジナルの (constant_)range_expression は (constant_)expression
// を含むので reduce/reduce conflict が生じる．
// 解決策は (constant_)range_expression から (constant_)expression を取り
// 除くこと．この expression と range の話は他の所でも出てくる
// (cf. dimension と index)
// また port_identifier は実はただの identifier
// どうも Verilog の BNF を書いた人は意味解析と構文解析の違いをわかって
// いないようである．構文解析中に識別子テーブルでもつくって別のトークンを
// 返すようにしろとでも言うのだろうか？
// たしかに C言語の構造体名はそういう工夫が必要だが．
port_reference
: IDENTIFIER
{
  parser.new_PortRef(@$, $1);
}
| IDENTIFIER index
{
  parser.new_PortRef(@$, $1, $2);
}
| IDENTIFIER '[' expression ':' expression ']'
{
  parser.new_PortRef(@$, $1, kVpiConstRange, $3, $5);
}
| IDENTIFIER '[' expression PLUSCOLON expression ']'
{
  parser.new_PortRef(@$, $1, kVpiPlusRange, $3, $5);
}
| IDENTIFIER '[' expression MINUSCOLON expression ']'
{
  parser.new_PortRef(@$, $1, kVpiMinusRange, $3, $5);
}
;


// [SPEC] list_of_port_declarations ::=
//            '(' port_declaration {',' port_declaration} ')'
//           |'(' ')'
// [SPEC] port_declaration ::=
//            { attribute_instance } inout_declaration
//           |{ attribute_instance } input_declaration
//           |{ attribute_instance } output_declaration
// input_declaration などの詳細は後に出てくる．
// 上で述べたように () のケースは nzlist_of_ports で特別に扱うこととし,
// list_of_port_declarations は非ゼロ要素のリストとする．
// 2重のリストを同じ ',' で区切っているので IEEE1364 の BNF 通りにすると
// 曖昧な文法となってしまうので1重のリストにして ',' の後に
// INPUT/INOUT/OUTPUT が来るか port_identifier が来るかで区別する様に
// 変更した．何にも考えていない IEEE1364-2001 の拡張の仕方が悪い．
// 詳細は A.2.1.2 Port declarations を参照のこと．
list_of_port_declarations
: portdecl_head variable_port_identifier_item
| list_of_port_declarations ',' variable_port_identifier_item
| list_of_port_declarations ','
  { parser.end_io(); } portdecl_head variable_port_identifier_item
;

// [SPEC*] portdecl_head ::=
//            "inout" [nettype] ["signed"] [range]
//           |"input" [nettype] ["signed"] [range]
//           |"output" [nettype] ["signed"] [range]
//           |"output" "reg"  ["signed"][range]
//           |"output" "reg"  ["signed"][range]
//           |"output" output_variable_type
//           |"output" output_variable_type
portdecl_head
: ai_list INOUT          sign
{
  parser.new_IOHead(@$, kPtIO_Inout, $3, $1);
}
| ai_list INOUT net_type sign
{
  parser.new_NetIOHead(@$, kPtIO_Inout, $3, $4, $1);
}
| ai_list INOUT          sign '[' expression ':' expression ']'
{
  parser.new_IOHead(@$, kPtIO_Inout, $3, $5, $7, $1);
}
| ai_list INOUT net_type sign '[' expression ':' expression ']'
{
  parser.new_NetIOHead(@$, kPtIO_Inout, $3, $4, $6, $8, $1);
}
| ai_list INPUT          sign
{
  parser.new_IOHead(@$, kPtIO_Input, $3, $1);
}
| ai_list INPUT net_type sign
{
  parser.new_NetIOHead(@$, kPtIO_Input, $3, $4, $1);
}
| ai_list INPUT          sign '[' expression ':' expression ']'
{
  parser.new_IOHead(@$, kPtIO_Input, $3, $5, $7, $1);
}
| ai_list INPUT net_type sign '[' expression ':' expression ']'
{
  parser.new_NetIOHead(@$, kPtIO_Input, $3, $4, $6, $8, $1);
}
| ai_list OUTPUT          sign
{
  parser.new_IOHead(@$, kPtIO_Output, $3, $1);
}
| ai_list OUTPUT net_type sign
{
  parser.new_NetIOHead(@$, kPtIO_Output, $3, $4, $1);
}
| ai_list OUTPUT          sign '[' expression ':' expression ']'
{
  parser.new_IOHead(@$, kPtIO_Output, $3, $5, $7, $1);
}
| ai_list OUTPUT net_type sign '[' expression ':' expression ']'
{
  parser.new_NetIOHead(@$, kPtIO_Output, $3, $4, $6, $8, $1);
}
| ai_list OUTPUT REG sign
{
  parser.new_RegIOHead(@$, kPtIO_Output, $4, $1);
}
| ai_list OUTPUT REG sign '[' expression ':' expression ']'
{
  parser.new_RegIOHead(@$, kPtIO_Output, $4, $6, $8, $1);
}
| ai_list OUTPUT INTEGER
{
  parser.new_VarIOHead(@$, kPtIO_Output, kVpiVarInteger, $1);
}
| ai_list OUTPUT TIME
{
  parser.new_VarIOHead(@$, kPtIO_Output, kVpiVarTime, $1);
}
;


//////////////////////////////////////////////////////////////////////
// A.1.5 Module items
//////////////////////////////////////////////////////////////////////

// [SPEC] module_item ::=
//             module_or_generate_item
//            |port_declaration ';'
//            |{ attribute_instance } generated_instantiation
//            |{ attribute_instance } local_paramter_declaration
//            |{ attribute_instance } parameter_declaration
//            |{ attribute_instance } specify_block
//            |{ attribute_instance } specparam_declaration
// [SPEC] port_declaration ::=
//             { attribute_instance } inout_declaration
//            |{ attribute_instance } input_declaration
//            |{ attribute_instance } output_declaration
// 実は port_declarations 以外は non_port_module_item と同一なので
// そちらを使うと簡単に書ける．
// [SPEC] non_port_module_item ::=
//             { attribute_instance } generated_instantiation
//            |{ attribute_instance } module_or_generate_item
//            |{ attribute_instance } local_paramter_declaration
//            |{ attribute_instance } parameter_declaration
//            |{ attribute_instance } specify_block
//            |{ attribute_instance } specparam_declaration
// おいおい，module_or_generate_item はその定義の中に
// { attribute_instance }を含んでいるのでここでは不要でしょ．
// 文法規則に一貫性がないからこんな些細なミスが入り込むことになるのだよ．
// ところでここで言う item とは declaration も含んでおり, あとで処理
// を区別したいので item という形で返すことが難しい．
// そこで list_of_module_items という非終端節点をつくって，そこを
// 多重(並列)のリストにして item と declaration を区別している．
// また inout_declaration などはポート宣言で使うためにセミコロンを含まない
// 定義になっているのでここでセミコロンをつけなければいけないのに忘れている．
// とはいえ二重リストのときには inout_declhead という形で処理するので
// inout_declaration の最後にセミコロンを含めておいた方が分かりやすいので
// そうしている．

// [SPEC*] list_of_module_items ::= { module_item }
// 各要素の先頭の { attribute_instance } はここで処理する．
list_of_module_items
: // 空もありうる．
| list_of_module_items module_or_generate_decl
| list_of_module_items module_or_generate_item
| list_of_module_items inout_declaration
| list_of_module_items input_declaration
| list_of_module_items output_declaration
| list_of_module_items generated_instantiation
| list_of_module_items local_parameter_declaration
| list_of_module_items parameter_declaration
| list_of_module_items specify_block
| list_of_module_items specparam_declaration
;

// [SPEC*] list_of_module_items ::= { module_item }
// 各要素の先頭の { attribute_instance } はここで処理する．
// こちらは port_declaration を含まない．
list_of_module_items2
: // 空もありうる．
| list_of_module_items module_or_generate_decl
| list_of_module_items module_or_generate_item
| list_of_module_items generated_instantiation
| list_of_module_items local_parameter_declaration
| list_of_module_items parameter_declaration
| list_of_module_items specify_block
| list_of_module_items specparam_declaration
;

// [SPEC] module_or_generate_item ::=
//             { attribute_instance } module_or_generate_item_declaration
//            |{ attribute_instance } parameter_override
//            |{ attribute_instance } continuous_assign
//            |{ attribute_instance } gate_instantiation
//            |{ attribute_instance } udp_instantiation
//            |{ attribute_instance } module_instantiation
//            |{ attribute_instance } initial_construct
//            |{ attribute_instance } always_construct
// [SPEC] module_or_generate_item_declaration ::=
//             net_declaration
//            |reg_declaration
//            |integer_declaration
//            |real_declaration
//            |time_declaration
//            |realtime_declaration
//            |event_declaration
//            |genvar_declaration
//            |task_declaration
//            |function_declaration
// みての通り module_or_generate_item_declaration という非終端ノードを
// 展開している．
// 先頭の { attribute_instance } は module_or_generate_item の外側で処理する．
// 具体的には list_of_module_items と generate_item
// 展開しておいてから言うのも何だけど PtDeclHead 系と PtItem 系の2つに
// 分類している．ここでいう declaration は上と微妙に異なる．
module_or_generate_decl
: net_declaration
| reg_declaration
| integer_declaration
| real_declaration
| time_declaration
| realtime_declaration
| event_declaration
| genvar_declaration
;

module_or_generate_item
: task_declaration
| function_declaration
| parameter_override
| continuous_assign
| gate_instantiation
| module_instantiation
| initial_construct
| always_construct
;

// [SPEC] parameter_override ::= "defparam" list_of_param_assignments ';'
// list_of_param_assignments の parameter は階層なしだが defparam 文では
// 階層付きの名前を使ってこそ意味があるのでこれは仕様の明らかな誤り
// そのため defparam_assignment という非終端節点を用意している．
parameter_override
: ai_list defparam_head nzlist_of_defparam_assignment ';'
{
  parser.new_DefParamH(@$, $1);
}
| ai_list defparam_head error ';'
{
  yyerrok;
}
;

defparam_head
: DEFPARAM
{
  parser.init_defparam();
}

// [SPEC*] nzlist_of_defparam_assignment ::=
//             defparam_assignment { ',' defparam_assignment }
nzlist_of_defparam_assignment
: defparam_assignment
| nzlist_of_defparam_assignment ',' defparam_assignment
;

// [SPEC*] defparam_assignment ::=
//             hierarchical_identifier '=' constant_expression
defparam_assignment
: hierarchical_identifier '=' expression
{
  parser.new_DefParam(@$, $1, $3);
}
| IDENTIFIER '=' expression
{
  parser.new_DefParam(@$, $1, $3);
}
;


//////////////////////////////////////////////////////////////////////
// A.2 Declarations
//////////////////////////////////////////////////////////////////////
// A.2.1 Declaration types
//////////////////////////////////////////////////////////////////////
// A.2.1.1 Module parameter declarations
//////////////////////////////////////////////////////////////////////

// [SPEC] local_parameter_declaration ::=
//             "localparam" ["signed"] [range] list_of_param_assignments ';'
//            |"localparam" "integer"          list_of_param_assignments ';'
//            |"localparam" "real"             list_of_param_assignments ';'
//            |"localparam" "realtime"         list_of_param_assignments ';'
//            |"localparam" "time"             list_of_param_assignments ';'
local_parameter_declaration
: localparam_declhead list_of_localparam_assignments ';'
{
  parser.end_localparam();
}
| localparam_declhead error ';'
{
  parser.end_localparam();
  yyerrok;
}
;

localparam_declhead
: ai_list LOCALPARAM
{
  parser.new_LocalParamH(@$, $1);
}
| ai_list LOCALPARAM sign '[' expression ':' expression ']'
{
  parser.new_LocalParamH(@$, $3, $5, $7, $1);
}
| ai_list LOCALPARAM data_type
{
  parser.new_LocalParamH(@$, $3, $1);
}
;

// [SPEC] parameter_declaration ::=
//             "parameter" ["signed"] [range] list_of_param_assignments ';'
//            |"parameter" "integer"          list_of_param_assignments ';'
//            |"parameter" "real"             list_of_param_assignments ';'
//            |"parameter" "realtime"         list_of_param_assignments ';'
//            |"parameter" "time"             list_of_param_assignments ';'
parameter_declaration
: parameter_declhead list_of_param_assignments ';'
{
  parser.end_param();
}
| parameter_declhead error ';'
{
  parser.end_param();
  yyerrok;
}
;

parameter_declhead
: ai_list PARAMETER
{
  parser.new_ParamH(@$, $1);
}
| ai_list PARAMETER sign '[' expression ':' expression ']'
{
  parser.new_ParamH(@$, $3, $5, $7, $1);
}
| ai_list PARAMETER data_type
{
  parser.new_ParamH(@$, $3, $1);
}
;

// [SPEC] specparam_declaration ::= specparam ["range"]
//             list_of_specparam_assignments ';'
specparam_declaration
: specparam_declhead list_of_specparam_assignments ';'
{
  parser.end_decl();
}
| specparam_declhead error ';'
{
  parser.end_decl();
  yyerrok;
}
;

specparam_declhead
: ai_list SPECPARAM
{
  parser.new_SpecParamH(@$, $1);
}
| ai_list SPECPARAM '[' expression ':' expression ']'
{
  parser.new_SpecParamH(@$, $4, $6, $1);
}
;

//////////////////////////////////////////////////////////////////////
// A.2.1.2 Port declarations
//////////////////////////////////////////////////////////////////////

// [SPEC] inout_declaration ::=
//            "inout" [nettype] ["signed"] [range] list_of_port_identifiers ';'
// 末尾のセミコロンは IEEE1364-2001 の仕様書にはない．
inout_declaration
: inout_declhead list_of_port_identifiers ';'
{
  parser.end_io();
}
| inout_declhead error ';'
{
  parser.end_io();
  yyerrok;
}
;

inout_declhead
: ai_list INOUT          sign
{
  parser.new_IOHead(@$, kPtIO_Inout, $3, $1);
}
| ai_list INOUT net_type sign
{
  parser.new_NetIOHead(@$, kPtIO_Inout, $3, $4, $1);
}
| ai_list INOUT          sign '[' expression ':' expression ']'
{
  parser.new_IOHead(@$, kPtIO_Inout, $3, $5, $7, $1);
}
| ai_list INOUT net_type sign '[' expression ':' expression ']'
{
  parser.new_NetIOHead(@$, kPtIO_Inout, $3, $4, $6, $8, $1);
}
;

// [SPEC] input_declaration ::=
//            "input" [nettype]["signed"][range] list_of_port_identifiers ';'
// 末尾のセミコロンは IEEE1364-2001 の仕様書にはない．
input_declaration
: input_declhead list_of_port_identifiers ';'
{
  parser.end_io();
}
| input_declhead error ';'
{
  parser.end_io();
  yyerrok;
}
;

input_declhead
: ai_list INPUT          sign
{
  parser.new_IOHead(@$, kPtIO_Input, $3, $1);
}
| ai_list INPUT net_type sign
{
  parser.new_NetIOHead(@$, kPtIO_Input, $3, $4, $1);
}
| ai_list INPUT          sign '[' expression ':' expression ']'
{
  parser.new_IOHead(@$, kPtIO_Input, $3, $5, $7, $1);
}
| ai_list INPUT net_type sign '[' expression ':' expression ']'
{
  parser.new_NetIOHead(@$, kPtIO_Input, $3, $4, $6, $8, $1);
}
;

// [SPEC] output_declaration ::=
//    "output" [nettype]["signed"][range] list_of_port_identifiers
//   |"output" ["reg"]  ["signed"][range] list_of_port_identifiers
//   |"output" "reg"    ["signed"][range] list_of_variable_port_identifiers
//   |"output" [output_variable_type]     list_of_port_identifiers
//   |"output" output_variable_type       list_of_variable_port_identifiers
// [SPEC] list_of_port_identifiers ::= port_identifier {',' port_identifier}
// [SPEC] output_variable_type ::= "integer"|"time"
// [SPEC] list_of_variable_port_identifiers ::=
//    port_identifier ['=' constant_expression]
//    {',' port_identifier ['=' constant_expression]}
// [SPEC] port_identifier ::= identifier
// inout,input と異なりちょっと複雑．
// まず [nettype] ではじまる形式と ["reg"]で始まる形式は省略時の区別が
// つかないので "reg" のほうは省略なしとする．
// 同様に [output_variable_type] も省略なしとする．
// 残りはリストが identifier のみの形式と
// identifier '=' constant_expression の形式の区別だが，
// "reg" および output_variable_type で始まった場合には両方がありうることが
// わかる．そのため，list_of_port_identifiers のみをとる形式と両方をとる形式
// にわける．
output_declaration
: output_declhead1 list_of_port_identifiers ';'
{
  parser.end_io();
}
| output_declhead2 list_of_variable_port_identifiers ';'
{
  parser.end_io();
}
| output_declhead1 error ';'
{
  parser.end_io();
  yyerrok;
}
| output_declhead2 error ';'
{
  parser.end_io();
  yyerrok;
}
;

// [SPEC*] output_declhead1 ::=
//    "output" [nettype]["signed"][range]
// 末尾のセミコロンは IEEE1364-2001 の仕様書にはない．
output_declhead1
: ai_list OUTPUT          sign
{
  parser.new_IOHead(@$, kPtIO_Output, $3, $1);
}
| ai_list OUTPUT net_type sign
{
  parser.new_NetIOHead(@$, kPtIO_Output, $3, $4, $1);
}
| ai_list OUTPUT          sign '[' expression ':' expression ']'
{
  parser.new_IOHead(@$, kPtIO_Output, $3, $5, $7, $1);
}
| ai_list OUTPUT net_type sign '[' expression ':' expression ']'
{
  parser.new_NetIOHead(@$, kPtIO_Output, $3, $4, $6, $8, $1);
}
;

// [SPEC] output_declaration2 ::=
//    "output" "reg"  ["signed"][range] list_of_port_identifiers
//   |"output" "reg"  ["signed"][range] list_of_variable_port_identifiers
//   |"output" output_variable_type     list_of_port_identifiers
//   |"output" output_variable_type     list_of_variable_port_identifiers
// [SPEC] list_of_port_identifiers ::= port_identifier {',' port_identifier}
// [SPEC] output_variable_type ::= "integer"|"time"
// [SPEC] list_of_variable_port_identifiers ::=
//    port_identifier ['=' constant_expression]
//    {',' port_identifier ['=' constant_expression]}
// [SPEC] port_identifier ::= identifier
// これを見ると list_of_port_identifiers と list_of_variable_port_identifiers
// を区別しなければいけないように見えるが実は前者は後者に含まれているので
// 実は一種類のリストを用意すれば良い．
// 末尾のセミコロンは IEEE1364-2001 の仕様書にはない．
output_declhead2
: ai_list OUTPUT REG sign
{
  parser.new_RegIOHead(@$, kPtIO_Output, $4, $1);
}
| ai_list OUTPUT REG sign '[' expression ':' expression ']'
{
  parser.new_RegIOHead(@$, kPtIO_Output, $4, $6, $8, $1);
}
| ai_list OUTPUT INTEGER
{
  parser.new_VarIOHead(@$, kPtIO_Output, kVpiVarInteger, $1);
}
| ai_list OUTPUT TIME
{
  parser.new_VarIOHead(@$, kPtIO_Output, kVpiVarTime, $1);
}
;

//////////////////////////////////////////////////////////////////////
// A.2.1.3 Type declarations
//////////////////////////////////////////////////////////////////////

// [SPEC] event_declaration ::= "event" list_of_event_identifiers ';'
event_declaration
: event_declhead list_of_event_identifiers ';'
{
  parser.end_decl();
}
| event_declhead  error ';'
{
  parser.end_decl();
  yyerrok;
}
;

event_declhead
: ai_list EVENT
{
  parser.new_EventH(@$, $1);
}
;

// [SPEC] genvar_declaration ::= "genvar" list_of_genvar_identifiers ';'
genvar_declaration
: genvar_declhead list_of_genvar_identifiers ';'
{
  parser.end_decl();
}
| genvar_declhead error ';'
{
  parser.end_decl();
  yyerrok;
}
;

genvar_declhead
: ai_list GENVAR
{
  parser.new_GenvarH(@$, $1);
}
;

// [SPEC] integer_declaration ::= "integer" list_of_variable_identifiers ';'
integer_declaration
: integer_declhead list_of_variable_identifiers ';'
{
  parser.end_decl();
}
| integer_declhead error ';'
{
  parser.end_decl();
  yyerrok;
}
;

integer_declhead
: ai_list INTEGER
{
  parser.new_VarH(@$, kVpiVarInteger, $1);
}
;

// [SPEC] net_declaration ::=
//    net_type                                           ["signed"]
//             [delay3] list_of_net_identifiers ';'
//   |net_type [drive_strength]                          ["signed"]
//             [delay3] list_of_net_decl_assignments ';'
//   |net_type                   ["vectored"|"scalared"] ["signed"] range
//             [delay3] list_of_net_identifiers ';'
//   |net_type [drive_strength]  ["vectored"|"scalared"] ["signed"] range
//             [delay3] list_of_net_decl_assignments ';'
//   |"trireg" [charge_strength]                         ["signed"]
//             [delay3] list_of_net_identifiers ';'
//   |"trireg" [drive_strength]                          ["signed"]
//             [delay3] list_of_net_decl_assignments ';'
//   |"trireg" [charge_strength] ["vectored"|"scalared"] ["signed"] range
//             [delay3] list_of_net_identifiers ';'
//   |"trireg" [drive_strength]  ["vectored"|"scalared"] ["signed"] range
//             [delay3] list_of_net_decl_assignments ';'
// これは難解．まず，最後が list_of_net_identifiers のパタンと
// list_of_net_decl_assignments のパタンに分ける．
// 良く見てみると drive_strength が省略されたパタンは両方にある．
// また charge_strength があるパタンは list_of_net_identifier のみである．
// そこで，list_of_net_identifiers と list_of_net_decl_assignments の両方
// がありうるパタンを net_declaration1 とし，list_of_net_identifiers のみ
// がありうるパタンを net_declaration2 とし，list_of_net_decl_assignments
// のみがありうるパタンを net_declaration3 とする．
net_declaration
: net_declhead1 list_of_net_decls ';'
{
  parser.end_decl();
}
| net_declhead2 list_of_net_identifiers ';'
{
  parser.end_decl();
}
| net_declhead3 list_of_net_decl_assignments ';'
{
  parser.end_decl();
}
| net_declhead1 error ';'
{
  parser.end_decl();
  yyerrok;
}
| net_declhead2 error ';'
{
  parser.end_decl();
  yyerrok;
}
| net_declhead3 error ';'
{
  parser.end_decl();
  yyerrok;
}
;

// [SPEC*] net_declaration1_head ::=
//    net_type                         ["signed"]       [delay3]
//   |net_type ["vectored"|"scalared"] ["signed"] range [delay3]
//   |"trireg"                         ["signed"]       [delay3]
//   |"trireg" ["vectored"|"scalared"] ["signed"] range [delay3]
net_declhead1
: ai_list net_type sign
{
  // net のデフォルトは1ビット
  parser.new_NetH(@$, $2, $3, $1);
}
| ai_list net_type sign        delay3
{
  // net のデフォルトは1ビット
  parser.new_NetH(@$, $2, $3, $4, $1);
}
| ai_list net_type        sign '[' expression ':' expression ']'
{
  parser.new_NetH(@$, $2, kVpiVsNone, $3, $5, $7, $1);
}
| ai_list net_type vstype sign '[' expression ':' expression ']'
{
  parser.new_NetH(@$, $2, $3, $4, $6, $8, $1);
}
| ai_list net_type        sign '[' expression ':' expression ']' delay3
{
  parser.new_NetH(@$, $2, kVpiVsNone, $3, $5, $7, $9, $1);
}
| ai_list net_type vstype sign '[' expression ':' expression ']' delay3
{
  parser.new_NetH(@$, $2, $3, $4, $6, $8, $10, $1);
}
| ai_list TRIREG sign
{
  // 1ビット
  parser.new_NetH(@$, kVpiTriReg, $3, $1);
}
| ai_list TRIREG sign        delay3
{
  // 1ビット
  parser.new_NetH(@$, kVpiTriReg, $3, $4, $1);
}
| ai_list TRIREG        sign '[' expression ':' expression ']'
{
  parser.new_NetH(@$, kVpiTriReg, kVpiVsNone, $3, $5, $7, $1);
}
| ai_list TRIREG vstype sign '[' expression ':' expression ']'
{
  parser.new_NetH(@$, kVpiTriReg, $3, $4, $6, $8, $1);
}
| ai_list TRIREG        sign '[' expression ':' expression ']' delay3
{
  parser.new_NetH(@$, kVpiTriReg, kVpiVsNone, $3, $5, $7, $9, $1);
}
| ai_list TRIREG vstype sign '[' expression ':' expression ']' delay3
{
  parser.new_NetH(@$, kVpiTriReg, $3, $4, $6, $8, $10, $1);
}
;

// [SPEC*] net_declaration2_head ::=
//  |"trireg" charge_strength                         ["signed"]       [delay3]
//  |"trireg" charge_strength ["vectored"|"scalared"] ["signed"] range [delay3]
net_declhead2
: ai_list TRIREG charge_strength sign
{
  // 1ビット
  parser.new_NetH(@$, kVpiTriReg, $4, $3, $1);
}
| ai_list TRIREG charge_strength sign         delay3
{
  // 1ビット
  parser.new_NetH(@$, kVpiTriReg, $4, $3, $5, $1);
}
| ai_list TRIREG charge_strength        sign '[' expression ':' expression ']'
{
  parser.new_NetH(@$, kVpiTriReg, kVpiVsNone, $4, $6, $8, $3, $1);
}
| ai_list TRIREG charge_strength vstype sign '[' expression ':' expression ']'
{
  parser.new_NetH(@$, kVpiTriReg, $4, $5, $7, $9, $3, $1);
}
| ai_list TRIREG charge_strength        sign '[' expression ':' expression ']'
    delay3
{
  parser.new_NetH(@$, kVpiTriReg, kVpiVsNone, $4, $6, $8, $3, $10, $1);
}
| ai_list TRIREG charge_strength vstype sign '[' expression ':' expression ']'
    delay3
{
  parser.new_NetH(@$, kVpiTriReg, $4, $5, $7, $9, $3, $11, $1);
}
;

// [SPEC*] net_declaration3_head ::=
//  |net_type drive_strength                          ["signed"]       [delay3]
//  |net_type drive_strength  ["vectored"|"scalared"] ["signed"] range [delay3]
//  |"trireg" drive_strength                          ["signed"]       [delay3]
//  |"trireg" drive_strength  ["vectored"|"scalared"] ["signed"] range [delay3]
net_declhead3
: ai_list net_type drive_strength sign
{
  // 1ビット
  parser.new_NetH(@$, $2, $4, $3, $1);
}
| ai_list net_type drive_strength sign        delay3
{
  // 1ビット
  parser.new_NetH(@$, $2, $4, $3, $5, $1);
}
| ai_list net_type drive_strength        sign '[' expression ':' expression ']'
{
  parser.new_NetH(@$, $2, kVpiVsNone, $4, $6, $8, $3, $1);
}
| ai_list net_type drive_strength vstype sign '[' expression ':' expression ']'
{
  parser.new_NetH(@$, $2, $4, $5, $7, $9, $3, $1);
}
| ai_list net_type drive_strength        sign '[' expression ':' expression ']'
    delay3
{
  parser.new_NetH(@$, $2, kVpiVsNone, $4, $6, $8, $3, $10, $1);
}
| ai_list net_type drive_strength vstype sign '[' expression ':' expression ']'
    delay3
{
  parser.new_NetH(@$, $2, $4, $5, $7, $9, $3, $11, $1);
}

| ai_list TRIREG drive_strength sign
{
  // 1ビット
  parser.new_NetH(@$, kVpiTriReg, $4, $3, $1);
}
| ai_list TRIREG drive_strength sign        delay3
{
  // 1ビット
  parser.new_NetH(@$, kVpiTriReg, $4, $3, $5, $1);
}
| ai_list TRIREG drive_strength        sign '[' expression ':' expression ']'
{
  parser.new_NetH(@$, kVpiTriReg, kVpiVsNone, $4, $6, $8, $3, $1);
}
| ai_list TRIREG drive_strength vstype sign '[' expression ':' expression ']'
{
  parser.new_NetH(@$, kVpiTriReg, $4, $5, $7, $9, $3, $1);
}
| ai_list TRIREG drive_strength        sign '[' expression ':' expression ']'
    delay3
{
  parser.new_NetH(@$, kVpiTriReg, kVpiVsNone, $4, $6, $8, $3, $10, $1);
}
| ai_list TRIREG drive_strength vstype sign '[' expression ':' expression ']'
    delay3
{
  parser.new_NetH(@$, kVpiTriReg, $4, $5, $7, $9, $3, $11, $1);
}
;

// [SPEC] real_declaration ::= "real" list_of_real_identifiers ';'
real_declaration
: real_declhead list_of_real_identifiers ';'
{
  parser.end_decl();
}
| real_declhead error ';'
{
  parser.end_decl();
  yyerrok;
}
;

real_declhead
: ai_list REAL
{
  parser.new_VarH(@$, kVpiVarReal, $1);
}
;

// [SPEC] realtime_declaration ::= "realtime" list_of_real_identifiers ';'
realtime_declaration
: realtime_declhead list_of_real_identifiers ';'
{
  parser.end_decl();
}
| realtime_declhead error ';'
{
  parser.end_decl();
  yyerrok;
}
;

realtime_declhead
: ai_list REALTIME
{
  parser.new_VarH(@$, kVpiVarRealtime, $1);
}
;

// [SPEC] reg_declaration ::= "reg" ["signed"] [range]
//             list_of_variable_identifiers ';'
reg_declaration
: reg_declhead list_of_variable_identifiers ';'
{
  parser.end_decl();
}
| reg_declhead error ';'
{
  parser.end_decl();
  yyerrok;
}
;

reg_declhead
: ai_list REG sign
{
  parser.new_RegH(@$, $3, $1);
}
| ai_list REG sign '[' expression ':' expression ']'
{
  parser.new_RegH(@$, $3, $5, $7, $1);
}
;

// [SPEC] time_declaration ::= "time" list_of_variable_identifiers ';'
time_declaration
: time_declhead list_of_variable_identifiers ';'
{
  parser.end_decl();
}
| time_declhead error ';'
{
  parser.end_decl();
  yyerrok;
}
;

time_declhead
: ai_list TIME
{
  parser.new_VarH(@$, kVpiVarTime, $1);
}
;


//////////////////////////////////////////////////////////////////////
// A.2.2 Declaration data types
//////////////////////////////////////////////////////////////////////
// A.2.2.1 Net and variable types
//////////////////////////////////////////////////////////////////////

// [SPEC] net_type ::=
//             "supply0"|"supply1"
//            |"tri"|"triand"|"trior"|"tri0"|"tri1"
//            |"wire"|"wand"|"wor"
net_type
: SUPPLY0
{
  $$ = kVpiSupply0;
}
| SUPPLY1
{
  $$ = kVpiSupply1;
}
| TRI
{
  $$ = kVpiTri;
}
| TRIAND
{
  $$ = kVpiTriAnd;
}
| TRIOR
{
  $$ = kVpiTriOr;
}
| TRI0
{
  $$ = kVpiTri0;
}
| TRI1
{
  $$ = kVpiTri1;
}
| WIRE
{
  $$ = kVpiWire;
}
| WAND
{
  $$ = kVpiWand;
}
| WOR
{
  $$ = kVpiWor;
}
;

// [SPEC*] sign ::= "signed"|null
sign
: SIGNED
{
  $$ = true;
}
| // 空
{
  $$ = false;
}
;

// [SPEC*] vstype ::= "vectored"|"scalared"
vstype
: VECTORED
{
  $$ = kVpiVectored;
}
| SCALARED
{
  $$ = kVpiScalared;
}
;

// [SPEC] output_variable_type ::= "integer"|"time"

// [SPEC] real_type ::=
//             real_identifier ['=' constant_expression]
//            |real_identifier dimension { dimension }
// 1番目の形式で '=' 以下が省略されたときが含まれていないように見えるが,
// 実は2番目の形式が dimension が無い場合を含んでいる．この場合,
// identifier のみになるのでこちらでカバーしている．
// [SPEC] real_identifier ::= identifier
real_type
: IDENTIFIER '=' expression
{
  parser.new_DeclItem(@$, $1, $3);
}
| identifier_with_range
;

// [SPEC] variable_type ::=
//   variable_identifier ['=' constant_expression]
//  |variable_identifier dimension { dimension }
// [SPEC] variable_identifier ::= identifier
// 1番目の形式で '=' 以下が省略されたときが含まれていないように見えるが,
// 実は2番目の形式が dimension が無い場合を含んでいる．この場合,
// identifier のみになるのでこちらでカバーしている．
variable_type
: IDENTIFIER '=' expression
{
  parser.new_DeclItem(@$, $1, $3);
}
| identifier_with_range
;


//////////////////////////////////////////////////////////////////////
// A.2.2.2 Strength
//////////////////////////////////////////////////////////////////////

// [SPEC] drive_strength ::=
//             '(' strength0 ',' strength1 ')'
//            |'(' strength1 ',' strength0 ')'
//            |'(' strength0 ',' "highz1"')'
//            |'(' strength1 ',' "highz0" ')'
//            |'(' "highz1" ',' strength0 ')'
//            |'(' "highz0" ',' strength1 ')'
drive_strength
: '(' strength0 ',' strength1 ')'
{
  $$ = parser.new_Strength(@$, $2, $4);
}
| '(' strength1 ',' strength0 ')'
{
  // 順序を入れ替える．
  $$ = parser.new_Strength(@$, $4, $2);
}
| '(' strength0 ',' HIGHZ1 ')'
{
  $$ = parser.new_Strength(@$, $2, kVpiHiZ);
}
| '(' strength1 ',' HIGHZ0 ')'
{
  // 順序を入れ替える．
  $$ = parser.new_Strength(@$, kVpiHiZ, $2);
}
| '(' HIGHZ1    ',' strength0 ')'
{
  // 順序を入れ替える．
  $$ = parser.new_Strength(@$, $4, kVpiHiZ);
}
| '(' HIGHZ0    ',' strength1 ')'
{
  $$ = parser.new_Strength(@$, kVpiHiZ, $4);
}
;

// [SPEC] strength0 ::= "supply0"|"strong0"|"pull0"|"weak0"
strength0
: SUPPLY0
{
  $$ = kVpiSupplyDrive;
}
| STRONG0
{
  $$ = kVpiStrongDrive;
}
| PULL0
{
  $$ = kVpiPullDrive;
}
| WEAK0
{
  $$ = kVpiWeakDrive;
}
;

// [SPEC] strength1 ::= "supply1"|"strong1"|"pull1"|"weak1"
strength1
: SUPPLY1
{
  $$ = kVpiSupplyDrive;
}
| STRONG1
{
  $$ = kVpiStrongDrive;
}
| PULL1
{
  $$ = kVpiPullDrive;
}
| WEAK1
{
  $$ = kVpiWeakDrive;
}
;

// [SPEC] charge_strength ::= '(' "small" ')'|'(' "medium" ')'|'(' "large" ')'
charge_strength
: '(' SMALL ')'
{
  $$ = parser.new_Strength(@$, kVpiSmallCharge);
}
| '(' MEDIUM ')'
{
  $$ = parser.new_Strength(@$, kVpiMediumCharge);
}
| '(' LARGE ')'
{
  $$ = parser.new_Strength(@$, kVpiLargeCharge);
}
;


//////////////////////////////////////////////////////////////////////
// A.2.2.3 Delays
//////////////////////////////////////////////////////////////////////

// [SPEC] delay3 ::= '#'delay_value
//                  |'#''('delay_value[',' delay_value[',' delay_value]]')'
// [SPEC] delay2 ::= '#'delay_value
//                  |'#''('delay_value[',' delay_value]')'
// ちなみに delay2 と delay3 は共通部分があるので delay2 を使って delay3 を
// 簡潔に表している．
// ひとつの問題は '(' delay_value ')' の形は delay_value(実は expression)
// 自体が括弧を含むので適切に還元できない．
// そこでここでは '(' delay_value ')' の形は取り除いている．
delay3
: delay2
{
  $$ = $1;
}
| '#' '(' delay_value_y ',' delay_value_y ',' delay_value_y ')'
{
  $$ = parser.new_Delay(@$, $3, $5, $7);
}
;

delay2
: '#' delay_value
{
  $$ = parser.new_Delay(@$, $2);
}
| '#' '(' delay_value_y ',' delay_value_y ')'
{
  $$ = parser.new_Delay(@$, $3, $5);
}
;

delay_value_y
: delay_value
{
  $$ = $1;
}
| '(' mintypmax_expression ')'
{
  $$ = $2;
}
;

// [SPEC] delay_value ::=
//             unsigned number
//            |parameter_identifier
//            |specparam_identifier
//            |mintypmax_expression
// [SPEC] parameter_identifier ::= identifier
// [SPEC] specparam_identifier ::= identifier
// 最もやっかいな構文規則の一つ．
// parameter_identifier と specparam_identifier は構文規則上は同じトークン
// なのだがこれくらいは笑って許すとしても, そもそも mintypmax_expression に
// unsigned number や identifier は含まれてしまうので冗長である．
// また，意味的にはこれは constant_mintypmax_expression ではないかと思われる．
// 実際, IEEE1364(2001ではなく)では constant_mintypmax_expression になって
// いた．
// さらにさらに IEEE1364-2001 では constant_function が constant_expression
// に追加されたおかげで，
// gatetype '#' delay_value '(' nzlist_of_expressions ')'
// の delay_value とつぎの '(' の切目が曖昧になってしまった．
// つまり， delay_value の最後が identifier の時に次の '(' を読んで
// constant_function だと思ってシフトするのかそれとも identifier
// で還元して delay_value を終わらせるのかが分からない．
// これは明らかに文法上のミスである．
// とりあえず，括弧無しの形式は UNUMBER/RNUMBER と identifier しか認めない
// 様にした．括弧をつければ全部うまく行く．
delay_value
: delay_value_x
{
  $$ = $1;
}
| delay_value_x ':' delay_value_x ':' delay_value_x
{
  $$ = parser.new_MinTypMax(@$, $1, $3, $5);
}
| '(' expression ')'
{
  $$ = parser.new_Opr(@$, kVpiNullOp, $2, NULL);
}
;
/*
| '(' mintypmax_expression ')'

*/

delay_value_x
: unumber
{
  $$ = $1;
}
| rnumber
{
  $$ = $1;
}
| IDENTIFIER
{
  $$ = parser.new_Primary(@1, $1);
}
;


//////////////////////////////////////////////////////////////////////
// A.2.3 Declaration lists
//////////////////////////////////////////////////////////////////////

// [SPEC] list_of_event_identifiers ::=
//             event_identifier [dimension {dimension}]
//                 {',' event_identifier [dimension {dimension}]}
// [SPEC] event_identifier ::= identifier
// event_identifier [dimension {dimension}]
// は identifier_with_range という非終端節点で代用できる．
// しかしなんで event_identifier {dimension} と書かないのかね？
// これも Verilog1995 -> Verilog2001 の修正が不十分な証拠
list_of_event_identifiers
: identifier_with_range
| list_of_event_identifiers ','identifier_with_range
;

// [SPEC] list_of_genvar_identifiers ::=
//             genvar_identifier {',' genvar_identifier}
list_of_genvar_identifiers
: IDENTIFIER
{
  parser.new_DeclItem(@1, $1);
}
| list_of_genvar_identifiers ',' IDENTIFIER
{
  parser.new_DeclItem(@3, $3);
}
;

// [SPEC*] list_of_net_decls ::=
//             list_of_net_identifiers|list_of_net_decl_assignments
list_of_net_decls
: list_of_net_identifiers
| list_of_net_decl_assignments
;

// [SPEC] list_of_net_decl_assignments ::=
//             net_decl_assignment {',' net_decl_assignment}
list_of_net_decl_assignments
: net_decl_assignment
| list_of_net_decl_assignments ',' net_decl_assignment
;

// [SPEC] list_of_net_identifiers ::=
//             net_identifier [dimension {dimension}]
//                 {',' net_identifier [dimension {dimension}]}
// [SPEC] net_identifier ::= identifier
// net_identifier [dimension {dimension}]
// は identifier_with_range という非終端節点で代用できる．
// しかしなんで net_identifier {dimension} と書かないのかね？
// これも Verilog1995 -> Verilog2001 の修正が不十分な証拠
list_of_net_identifiers
: identifier_with_range
| list_of_net_identifiers ',' identifier_with_range
;

// [SPEC] list_of_param_assignments ::= param_assignment {',' param_assignment}
list_of_param_assignments
: param_assignment
| list_of_param_assignments ',' param_assignment
;

// [SPEC] list_of_param_assignments ::= param_assignment {',' param_assignment}
list_of_localparam_assignments
: localparam_assignment
| list_of_localparam_assignments ',' localparam_assignment
;

// [SPEC] list_of_port_identifiers ::= port_identifier {',' port_identifier}
// [SPEC] port_identifier ::= identifier
list_of_port_identifiers
: port_identifier_item
| list_of_port_identifiers ',' port_identifier_item
;

// [SPEC] list_of_real_identifiers ::= real_type {',' real_type}
list_of_real_identifiers
: real_type
| list_of_real_identifiers ',' real_type
;

// [SPEC] list_of_specparam_assignments ::=
//             specparam_assignment {',' specparam_assignment}
list_of_specparam_assignments
: specparam_assignment
| list_of_specparam_assignments ',' specparam_assignment
;

// [SPEC] list_of_variable_identifiers ::=
//             variable_type {',' variable_type}
list_of_variable_identifiers
: variable_type
| list_of_variable_identifiers ',' variable_type
;

// [SPEC] list_of_variable_port_identifiers ::=
//             port_identifier ['=' constant_expression]
//                 {',' port_identifier ['=' constant_expression]}
list_of_variable_port_identifiers
: variable_port_identifier_item
| list_of_variable_port_identifiers ',' variable_port_identifier_item
;

// [SPEC*] port_identifier_item ::= port_identifier
// [SPEC] port_identifier ::= identifier
// これは構文規則よりもアクションが重要
port_identifier_item
: IDENTIFIER
{
  parser.new_IOItem(@1, $1);
}
;

// [SPEC*] variable_port_identifier_item ::=
//              port_identifier ['=' constant_expression]
// [SPEC] port_identifier ::= identifier
variable_port_identifier_item
: IDENTIFIER
{
  parser.new_IOItem(@1, $1);
}
| IDENTIFIER '=' expression
{
  parser.new_IOItem(@$, $1, $3);
}
;


//////////////////////////////////////////////////////////////////////
// A.2.4 Declaration assignments
//////////////////////////////////////////////////////////////////////

// [SPEC] net_decl_assignment ::= net_identifier '=' expression
// [SPEC] net_identifier ::= identifier
net_decl_assignment
: IDENTIFIER '=' expression
{
  parser.new_DeclItem(@$, $1, $3);
}
;

// [SPEC] param_assignment ::= parameter_identifier '=' constant_expression
// [SPEC] parameter_identifier ::= identifier
param_assignment
: IDENTIFIER '=' expression
{
  parser.new_DeclItem(@$, $1, $3);
}
;

// [SPEC] param_assignment ::= parameter_identifier '=' constant_expression
// [SPEC] parameter_identifier ::= identifier
localparam_assignment
: IDENTIFIER '=' expression
{
  parser.new_DeclItem(@$, $1, $3);
}
;

// [SPEC] specparam_assignment ::=
//             specparam_identifier '=' constant_mintypmax_expression
//            |pulse_control_specparam
// [SPEC] specparam_identifier ::= identifier
// [SPEC] pulse_control_specparam ::=
//    "PATHPULSE$" '=' '(' reject_limit_value [',' error_limit_value] ')'';'
//   |"PATHPULSE$"specify_input_terminal_descriptor
//     '$'specify_output_terminal_descriptor
//     '=' '(' reject_limit_value [','error_limit_value] ')'';'
// [SPEC] error_limit_value ::= limit_value
// [SPEC] reject_limit_value ::= limit_value
// [SPEC] limit_value ::= constant_mintypmax_expression
// あれ？ pulse_control_specparam の末尾のセミコロンは不要じゃないの？
// mintypmax_expression のところで書いてあるように mintypmax_expression
// に expression は含まれないので expression を明示的に加えている．
specparam_assignment
: IDENTIFIER '=' expression
{
  parser.new_DeclItem(@$, $1, $3);
}
| IDENTIFIER '=' mintypmax_expression
{
  parser.new_DeclItem(@$, $1, $3);
}
| PATHPULSE '=' '(' mintypmax_expression ')'
{
  //$$ = parser.new_specparam(@$, NULL, NULL, $4, NULL);
}
| PATHPULSE '=' '(' mintypmax_expression ','
                    mintypmax_expression ')'
{
  //$$ = parser.new_specparam(@$, NULL, NULL, $4, $6);
}
| PATHPULSE specify_terminal '$' specify_terminal '='
                '(' mintypmax_expression ')'
{
  //$$ = parser.new_specparam(@$, $2, $4, $7, NULL);
}
| PATHPULSE specify_terminal '$' specify_terminal '='
                '(' mintypmax_expression ','
                    mintypmax_expression ')'
{
  //$$ = parser.new_specparam(@$, $2, $4, $7, $9);
}
;


//////////////////////////////////////////////////////////////////////
// A.2.5 Declaration ranges
//////////////////////////////////////////////////////////////////////

// [SPEC] dimension ::= '[' dimension_constant_expression ':'
//                  dimension_constant_expression ']'
// [SPEC] dimension_constant_expression ::= constant_expression
// dimension_constant_expression などという非終端ノードは不必要

// [SPEC*] nzlist_of_dimensions ::= dimension { dimension }
nzlist_of_dimensions
: '[' expression ':' expression ']'
{
  $$ = parser.new_list<PtRange>();
  $$->push_back(parser.new_Range(@$, $2, $4));
}
| nzlist_of_dimensions '[' expression ':' expression ']'
{
  $$ = $1;
  $$->push_back(parser.new_Range(FileRegion(@2, @6), $3, $5));
}
;

// [SPEC] range ::= '[' msb_constant_expression ':' lsb_constant_expression ']'
// [SPEC] msb_constant_expression ::= constant_expression
// [SPEC] lsb_constant_expression ::= constant_expression
// この msb_constant_expression/lsb_constant_expression などは構文規則と
// 意味づけを混同している良い(悪い)例である．


//////////////////////////////////////////////////////////////////////
// A.2.6 Function declarations
//////////////////////////////////////////////////////////////////////

// [SPEC] function_declaration ::=
//             "function" ["automatic"] ["signed"] [range_or_type]
//                 function_identifier ';'
//                 function_item_declaration { function_item_declaration }
//                 function_statement
//             "endfunction"
//            |"function" ["automatic"] ["signed"] [range_or_type]
//                 function_identifier '(' function_port_list ')'';'
//                 block_item_declaration { block_item_declaration }
//                function_statement
//             "endfunction"
// [SPEC] range_or_type ::= range|"integer"|"real"|"realtime"|"time"
// IEEE1364-2001 にはこう書いてあるけど "signed" をつけて意味のあるのは
// range だけなので実際には
// ["signed"] [range] と
// "integer"|"real"|"realtime"|"time"
// の2種類なんじゃないかと思う．
// ここでは忠実に記録しておく．
// function_statement は statement で代用する．
// 読み込んだあとでチェックする．
function_declaration
: ai_list function_head opt_auto sign IDENTIFIER ';'
  nzlist_of_fitem_decl
  statement
  ENDFUNCTION
{
  parser.new_Function(@$, $5, $3, $4, $8, $1);
}
| ai_list function_head opt_auto sign '[' expression ':' expression ']' IDENTIFIER ';'
  nzlist_of_fitem_decl
  statement
  ENDFUNCTION
{
  parser.new_SizedFunc(@$, $10, $3, $4, $6, $8, $13, $1);
}
| ai_list function_head opt_auto sign data_type IDENTIFIER ';'
  nzlist_of_fitem_decl
  statement
  ENDFUNCTION
{
  parser.new_TypedFunc(@$, $6, $3, $4, $5, $9, $1);
}
| ai_list function_head opt_auto sign IDENTIFIER function_port_block ';'
  list_of_bitem_decl
  statement
  ENDFUNCTION
{
  parser.new_Function(@$, $5, $3, $4, $9, $1);
}
| ai_list function_head opt_auto sign '[' expression ':' expression ']'
  IDENTIFIER function_port_block ';'
  list_of_bitem_decl
  statement
  ENDFUNCTION
{
  parser.new_SizedFunc(@$, $10, $3, $4, $6, $8, $14, $1);
}
| ai_list function_head opt_auto sign data_type IDENTIFIER function_port_block ';'
  list_of_bitem_decl
  statement
  ENDFUNCTION
{
  parser.new_TypedFunc(@$, $6, $3, $4, $5, $10, $1);
}
| ai_list function_head error ENDFUNCTION
{
  parser.end_tf();
  yyerrok;
}
;

function_head
: FUNCTION
{
  parser.init_tf();
}
;

function_port_block
: '(' function_port_list ')'
{
  parser.end_io();
}
;

// [SPEC*] nzlist_of_fitem_decl ::=
//             ai_list function_item_declaration
//               { ai_list function_item_declaration}
// [SPEC] function_item_declaration ::=
//             block_item_declaration
//            |tf_input_declaration ';'
// たぶん tf_input_declaration の前に { attribute_instance } がつくと思う．
// これ 2001 の拡張なのでどの位置に書くのか統一がとれていない．
// セミコロンは tf_input_declaration 中で処理する．
nzlist_of_fitem_decl
: block_item_declaration
| tf_input_declaration
| nzlist_of_fitem_decl block_item_declaration
| nzlist_of_fitem_decl tf_input_declaration
;

// [SPEC*] list_of_bitem_decl ::= { block_item_declaration }
list_of_bitem_decl
: // 空
| list_of_bitem_decl block_item_declaration
;

// [SPEC] function_item_declaration ::=
//             block_item_declaration
//            |tf_input_declaration ';'
// たぶん tf_input_declaration の前に { attribute_instance } がつくと思う．
// これ 2001 の拡張なのでどの位置に書くのか統一がとれていない．

// [SPEC] function_port_list ::=
//             { attribute_instance } tf_input_declaration
//                 {',' { attribute_instance } tf_input_declaration }
function_port_list
:                                             tf_input_declhead port_identifier_item
| function_port_list ',' { parser.end_io(); } tf_input_declhead port_identifier_item
| function_port_list ',' port_identifier_item
;

// [SPEC*] data_type ::=
//             |"integer"
//             |"real"
//             |"realtime"
//             |"time"
data_type
: INTEGER
{
  $$ = kVpiVarInteger;
}
| REAL
{
  $$ = kVpiVarReal;
}
| REALTIME
{
  $$ = kVpiVarRealtime;
}
| TIME
{
  $$ = kVpiVarTime;
}
;


//////////////////////////////////////////////////////////////////////
// A.2.7 Task declarations
//////////////////////////////////////////////////////////////////////

// [SPEC] task_declaration ::=
//             "task" ["automatic"] task_identifier ';'
//                 { task_item_declaration }
//                 statement
//             "endtask"
//            |"task" ["automatic"] task_identifier '(' task_port_list ')' ';'
//                 { block_item_declaration }
//                 statement
//             "endtask"
// この statement は statement_or_null でよいそうだ．
// Verilog-XL もそうなっている．
task_declaration
: ai_list task_head opt_auto IDENTIFIER ';'
  list_of_titem_decl
  statement_or_null
  ENDTASK
{
  parser.new_Task(@$, $4, $3, $7, $1);
}
| ai_list task_head opt_auto IDENTIFIER task_port_block ';'
  list_of_bitem_decl
  statement_or_null
  ENDTASK
{
  parser.new_Task(@$, $4, $3, $8, $1);
}
| ai_list task_head error ENDTASK
{
  parser.end_tf();
  yyerrok;
}
;

task_head
: TASK
{
  parser.init_tf();
}
;

task_port_block
: '(' task_port_list ')'
{
  parser.end_io();
}
;

// [SPEC*] opt_auto ::=
//            /* 空 */
//           | "automatic"
opt_auto
:
{
  $$ = false;
}
| AUTOMATIC
{
  $$ = true;
}
;

// [SPEC*] list_of_task_item_declarations ::=
//             { task_item_declaration }
// セミコロンは tf_XXX_declaration 中で処理する．
list_of_titem_decl
: // 空
| list_of_titem_decl block_item_declaration
| list_of_titem_decl tf_input_declaration
| list_of_titem_decl tf_output_declaration
| list_of_titem_decl tf_inout_declaration
;

// [SPEC] task_item_declaration ::=
//             block_item_declaration
//            |{ attribute_instance } tf_input_declaration ';'
//            |{ attribute_instance } tf_output_declaration ';'
//            |{ attribute_instance } tf_inout_declaration ';'
// block_item_declaration の先頭にも { attribute_instance } がつくので
// まとめて list_of_titem_decl で処理する．

// [SPEC] task_port_list ::= task_port_item {',' task_port_item }
// [SPEC] task_port_item ::=
//             { attribute_instance } tf_input_declaration
//            |{ attribute_instance } tf_output_declaration
//            |{ attribute_instance } tf_inout_declaration
task_port_list
: tf_input_declhead port_identifier_item
| tf_output_declhead port_identifier_item
| tf_inout_declhead port_identifier_item
| task_port_list tf_ioitem_end tf_input_declhead port_identifier_item
| task_port_list tf_ioitem_end tf_output_declhead port_identifier_item
| task_port_list tf_ioitem_end tf_inout_declhead port_identifier_item
| task_port_list ',' port_identifier_item
;

tf_ioitem_end
: ','
{
  parser.end_io();
}
;

// [SPEC] tf_input_declaration ::=
//             "input" ["reg"] ["signed"] [range] list_of_port_identifiers
//            |"input" [task_port_type]           list_of_port_identifiers
// 実際には tf_input_declhead という非終端節点で処理する．
tf_input_declaration
: tf_input_declhead list_of_port_identifiers ';'
{
  parser.end_io();
}
| tf_input_declhead error ';'
{
  parser.end_io();
  yyerrok;
}
;

// [SPEC*] tf_input_declhead ::=
//              "input" ["reg"] ["signed"] [range]
//             |"input" [task_port_type]
// reg, signed, range が省略された場合と task_port_type が省略された
// 場合の区別がつかないので2番めの形は task_port_type の省略無しとする．
tf_input_declhead
: ai_list INPUT     sign
{
  parser.new_IOHead(@$, kPtIO_Input, $3, $1);
}
| ai_list INPUT REG sign
{
  parser.new_RegIOHead(@$, kPtIO_Input, $4, $1);
}
| ai_list INPUT     sign '[' expression ':' expression ']'
{
  parser.new_IOHead(@$, kPtIO_Input, $3, $5, $7, $1);
}
| ai_list INPUT REG sign '[' expression ':' expression ']'
{
  parser.new_RegIOHead(@$, kPtIO_Input, $4, $6, $8, $1);
}
| ai_list INPUT task_port_type
{
  parser.new_VarIOHead(@$, kPtIO_Input, $3, $1);
}
;

// [SPEC] tf_output_declaration ::=
//             "output" ["reg"] ["signed"] [range] list_of_port_identifiers
//            |"output" [task_port_type]           list_of_port_identifiers
// 実際には tf_output_declhead という非終端節点で処理する．
tf_output_declaration
: tf_output_declhead list_of_port_identifiers ';'
{
  parser.end_io();
}
| tf_output_declhead error ';'
{
  parser.end_io();
  yyerrok;
}
;

// [SPEC*] tf_output_declhead ::=
//              "output" ["reg"] ["signed"] [range]
//             |"output" [task_port_type]
// reg, signed, range が省略された場合と task_port_type が省略された
// 場合の区別がつかないので2番めの形は task_port_type の省略無しとする．
tf_output_declhead
: ai_list OUTPUT     sign
{
  parser.new_IOHead(@$, kPtIO_Output, $3, $1);
}
| ai_list OUTPUT REG sign
{
  parser.new_RegIOHead(@$, kPtIO_Output, $4, $1);
}
| ai_list OUTPUT     sign '[' expression ':' expression ']'
{
  parser.new_IOHead(@$, kPtIO_Output, $3, $5, $7, $1);
}
| ai_list OUTPUT REG sign '[' expression ':' expression ']'
{
  parser.new_RegIOHead(@$, kPtIO_Output, $4, $6, $8, $1);
}
| ai_list OUTPUT task_port_type
{
  parser.new_VarIOHead(@$, kPtIO_Output, $3, $1);
}
;

// [SPEC] tf_inout_declaration ::=
//             inout ["reg"] ["signed"] [range] list_of_port_identifiers
//            |inout [task_port_type]           list_of_port_identifiers
// 実際には tf_inout_declhead という非終端節点で処理する．
tf_inout_declaration
: tf_inout_declhead list_of_port_identifiers ';'
{
  parser.end_io();
}
| tf_inout_declhead error ';'
{
  parser.end_io();
  yyerrok;
}
;

// [SPEC*] tf_inout_declhead ::=
//              inout ["reg"] ["signed"] [range]
//             |inout [task_port_type]
// reg, signed, range が省略された場合と task_port_type が省略された
// 場合の区別がつかないので2番めの形は task_port_type の省略無しとする．
tf_inout_declhead
: ai_list INOUT     sign
{
  parser.new_IOHead(@$, kPtIO_Inout, $3, $1);
}
| ai_list INOUT REG sign
{
  parser.new_RegIOHead(@$, kPtIO_Inout, $4, $1);
}
| ai_list INOUT     sign '[' expression ':' expression ']'
{
  parser.new_IOHead(@$, kPtIO_Inout, $3, $5, $7, $1);
}
| ai_list INOUT REG sign '[' expression ':' expression ']'
{
  parser.new_RegIOHead(@$, kPtIO_Inout, $4, $6, $8, $1);
}
| ai_list INOUT task_port_type
{
  parser.new_VarIOHead(@$, kPtIO_Inout, $3, $1);
}
;

// [SPEC] task_port_type ::= "time"|"real"|"realtime"|"integer"
// じつは data_type と同じ
task_port_type
: TIME
{
  $$ = kVpiVarTime;
}
| REAL
{
  $$ = kVpiVarReal;
}
| REALTIME
{
  $$ = kVpiVarRealtime;
}
| INTEGER
{
  $$ = kVpiVarInteger;
}
;


//////////////////////////////////////////////////////////////////////
// A.2.8 Block item declaration
//////////////////////////////////////////////////////////////////////

// [SPEC] block_item_declaration ::=
//             { attribute_instance } block_reg_declaration
//            |{ attribute_instance } event_declaration
//            |{ attribute_instance } integer_declaration
//            |{ attribute_instance } local_parameter_declaration
//            |{ attribute_instance } parameter_declaration
//            |{ attribute_instance } real_declaration
//            |{ attribute_instance } realtime_declaration
//            |{ attribute_instance } time_declaration
// 先頭の { attribute_instance } は block_item_declaration を
// 使っているところで処理する．
// 具体的には nzlist_of_fitem_decl, list_of_bitem_decl, list_of_titem_decl
block_item_declaration
: block_reg_declaration
| event_declaration
| integer_declaration
| local_parameter_declaration
| parameter_declaration
| real_declaration
| realtime_declaration
| time_declaration
;

// [SPEC] block_reg_declaration ::= "reg" ["signed"] [range]
//                 list_of_block_variable_identifiers ';'
block_reg_declaration
: block_reg_declhead list_of_block_variable_identifiers ';'
{
  parser.end_decl();
}
| block_reg_declhead error ';'
{
  parser.end_decl();
  yyerrok;
}
;

block_reg_declhead
: ai_list REG sign
{
  parser.new_RegH(@$, $3, $1);
}
| ai_list REG sign '[' expression ':' expression ']'
{
  parser.new_RegH(@$, $3, $5, $7, $1);
}
;

// [SPEC] list_of_block_variable_identifiers ::=
//             block_variable_type {',' block_variable_type }
list_of_block_variable_identifiers
: block_variable_type
| list_of_block_variable_identifiers ',' block_variable_type
;

// [SPEC] block_variable_type ::=
//             variable_identifier
//            |variable_identifier dimension {dimension}
// [SPEC] variable_identifier ::= identifier
// なんども出てきているように identifier_with_range は空も含むので
// 一通りでよい．
block_variable_type
: identifier_with_range
;

// [SPEC*] identifier_with_range ::= identifier {dimension}
identifier_with_range
: IDENTIFIER
{
  parser.new_DeclItem(@1, $1);
}
| IDENTIFIER nzlist_of_dimensions
{
  parser.new_DeclItem(@$, $1, $2);
}
;


//////////////////////////////////////////////////////////////////////
// A.3 Primitive instances
// A.3.1 Primitive instantiation and instances
//////////////////////////////////////////////////////////////////////

// [SPEC] gate_instantiation ::=
//             cmos_switchtype [delay3] cmos_switch_instance
//                 {',' cmos_switch_instance } ';'
//            |enable_gatetype [drive_strength] [delay3] enable_gate_instance
//                 {',' enble_gate_instance } ';'
//            |mos_switchtype [delay3] mos_switch_instance
//                 {',' mos_switch_instance } ';'
//            |n_input_gatetype [drive_strength] [delay2] n_input_gate_instance
//                 {',' n_input_gate_instance } ';'
//            |n_output_gatetype [drive_strength] [delay2]
//                 n_output_gate_instance {',' n_output_gate_instance } ';'
//            |pass_en_switchtype [delay3] pass_enable_switch_instance
//                 {',' pass_enable_switch_instance } ';'
//            |pass_switchtype pass_switch_instance
//                 {',' pass_switch_instance } ';'
//            |"pulldown" [pulldown_strength] pull_gate_instance
//                 {',' pull_gate_instance } ';'
//            |"pullup" [pullup_strength] pull_gate_instance
//                 {',' pull_gate_instance } ';'
gate_instantiation
: ai_list cmos_switchtype                        nzlist_of_cmos_switch_inst ';'
{
  parser.new_GateH(@$, $2, $1);
}
| ai_list cmos_switchtype                 delay3 nzlist_of_cmos_switch_inst ';'
{
  parser.new_GateH(@$, $2, $3, $1);
}
| ai_list enable_gatetype                        nzlist_of_enable_gate_inst ';'
{
  parser.new_GateH(@$, $2, $1);
}
| ai_list enable_gatetype                 delay3 nzlist_of_enable_gate_inst ';'
{
  parser.new_GateH(@$, $2, $3, $1);
}
| ai_list enable_gatetype  drive_strength        nzlist_of_enable_gate_inst ';'
{
  parser.new_GateH(@$, $2, $3, $1);
}
| ai_list enable_gatetype  drive_strength delay3 nzlist_of_enable_gate_inst ';'
{
  parser.new_GateH(@$, $2, $3, $4, $1);
}
| ai_list mos_switchtype                         nzlist_of_mos_switch_inst ';'
{
  parser.new_GateH(@$, $2, $1);
}
| ai_list mos_switchtype                  delay3 nzlist_of_mos_switch_inst ';'
{
  parser.new_GateH(@$, $2, $3, $1);
}
| ai_list n_input_gatetype                       nzlist_of_n_input_gate_inst ';'
{
  parser.new_GateH(@$, $2, $1);
}
| ai_list n_input_gatetype                delay2 nzlist_of_n_input_gate_inst ';'
{
  parser.new_GateH(@$, $2, $3, $1);
}
| ai_list n_input_gatetype drive_strength        nzlist_of_n_input_gate_inst ';'
{
  parser.new_GateH(@$, $2, $3, $1);
}
| ai_list n_input_gatetype drive_strength delay2 nzlist_of_n_input_gate_inst ';'
{
  parser.new_GateH(@$, $2, $3, $4, $1);
}
| ai_list n_output_gatetype                       nzlist_of_n_output_gate_inst ';'
{
  parser.new_GateH(@$, $2, $1);
}
| ai_list n_output_gatetype                delay2 nzlist_of_n_output_gate_inst ';'
{
  parser.new_GateH(@$, $2, $3, $1);
}
| ai_list n_output_gatetype drive_strength        nzlist_of_n_output_gate_inst ';'
{
  parser.new_GateH(@$, $2, $3, $1);
}
| ai_list n_output_gatetype drive_strength delay2 nzlist_of_n_output_gate_inst ';'
{
  parser.new_GateH(@$, $2, $3, $4, $1);
}
| ai_list pass_en_switchtype                      nzlist_of_pass_en_switch_inst ';'
{
  parser.new_GateH(@$, $2, $1);
}
| ai_list pass_en_switchtype               delay3 nzlist_of_pass_en_switch_inst ';'
{
  parser.new_GateH(@$, $2, $3, $1);
}
| ai_list pass_switchtype                         nzlist_of_pass_switch_inst ';'
{
  parser.new_GateH(@$, $2, $1);
}
| ai_list PULLDOWN                                nzlist_of_pull_inst ';'
{
  parser.new_GateH(@$, kVpiPulldownPrim, $1);
}
| ai_list PULLDOWN pulldown_strength              nzlist_of_pull_inst ';'
{
  parser.new_GateH(@$, kVpiPulldownPrim, $3, $1);
}
| ai_list PULLUP                                  nzlist_of_pull_inst ';'
{
  parser.new_GateH(@$, kVpiPullupPrim, $1);
}
| ai_list PULLUP pullup_strength                  nzlist_of_pull_inst ';'
{
  parser.new_GateH(@$, kVpiPullupPrim, $3, $1);
}
;

// [SPEC*] nzlist_of_cmos_switch_inst ::=
//                  cmos_siwtch_instance {',' cmos_switch_instance}
nzlist_of_cmos_switch_inst
: cmos_switch_instance
| nzlist_of_cmos_switch_inst ',' cmos_switch_instance
;

// [SPEC*] nzlist_of_enable_gate_inst ::=
//                  enable_gate_instance {',' enble_gate_instance }
nzlist_of_enable_gate_inst
: enable_gate_instance
| nzlist_of_enable_gate_inst ',' enable_gate_instance
;

// [SPEC*] nzlist_of_mos_switch_inst ::=
//                   mos_switch_instance {',' mos_switch_instance }
nzlist_of_mos_switch_inst
: mos_switch_instance
| nzlist_of_mos_switch_inst ',' mos_switch_instance
;

// [SPEC*] nzlist_of_n_input_gate_inst ::=
//                  n_input_gate_instance {',' n_input_gate_instance }
nzlist_of_n_input_gate_inst
: n_input_gate_instance
| nzlist_of_n_input_gate_inst ',' n_input_gate_instance
;

// [SPEC*] nzlist_of_n_output_gate_inst ::=
//                   n_output_gate_instance {',' n_output_gate_instance }
nzlist_of_n_output_gate_inst
: n_output_gate_instance
| nzlist_of_n_output_gate_inst ',' n_output_gate_instance
;

// [SPEC*] nzlist_of_pass_en_switch_inst ::=
//                  pass_enable_switch_instance
//                  {',' pass_enable_switch_instance }
nzlist_of_pass_en_switch_inst
: pass_enable_switch_instance
| nzlist_of_pass_en_switch_inst ',' pass_enable_switch_instance
;

// [SPEC*] nzlist_of_pass_switch_inst ::=
//                   pass_switch_instance {',' pass_switch_instance }
nzlist_of_pass_switch_inst
: pass_switch_instance
| nzlist_of_pass_switch_inst ',' pass_switch_instance
;

// [SPEC*] nzlist_of_pull_inst ::=
//                  pull_gate_instance {',' pull_gate_instance }
nzlist_of_pull_inst
: pull_gate_instance
| nzlist_of_pull_inst ',' pull_gate_instance
;

// [SPEC] cmos_switch_instance ::= [name_of_gate_instance]
//                  '(' output_terminal ',' input_terminal ','
//                      ncontrol_terminal ',' pcontrol_terminal ')'
// [SPEC] name_of_gate_instance ::= gate_instance_identifier [ range ]
// [SPEC] input_terminal ::= expression
// [SPEC] ncontrol_terminal ::= expression
// [SPEC] output_terminal ::= net_lvalue
// [SPEC] pcontrol_terminal ::= expression
cmos_switch_instance
: '(' net_lvalue ',' expression ',' expression ',' expression ')'
{
  parser.new_Inst(@$, $2, $4, $6, $6);
}
| IDENTIFIER
  '(' net_lvalue ',' expression ',' expression ',' expression ')'
{
  parser.new_InstN(@$, $1, $3, $5, $7, $9);
}
| IDENTIFIER '[' expression ':' expression ']'
  '(' net_lvalue ',' expression ',' expression ',' expression ')'
{
  parser.new_InstV(@$, $1, $3, $5, $8, $10, $12, $14);
}
;

// [SPEC] enable_gate_instance ::= [name_of_gate_instance]
//                  '(' output_terminal ',' input_terminal ','
//                      enable_terminal ')'
// [SPEC] name_of_gate_instance ::= gate_instance_identifier [ range ]
// [SPEC] enable_terminal ::= expression
// [SPEC] input_terminal ::= expression
// [SPEC] output_terminal ::= net_lvalue
enable_gate_instance
:                  '(' net_lvalue ',' expression ',' expression ')'
{
  parser.new_Inst(@$, $2, $4, $6);
}
| IDENTIFIER       '(' net_lvalue ',' expression ',' expression ')'
{
  parser.new_InstN(@$, $1, $3, $5, $7);
}
| IDENTIFIER '[' expression ':' expression ']'
  '(' net_lvalue ',' expression ',' expression ')'
{
  parser.new_InstV(@$, $1, $3, $5, $8, $10, $12);
}
;

// [SPEC] mos_switch_instance ::= [name_of_gate_instance]
//                 '(' output_terminal ',' input_terminal ','
//                     enable_terminal ')'
// [SPEC] name_of_gate_instance ::= gate_instance_identifier [ range ]
// [SPEC] enable_terminal ::= expression
// [SPEC] input_terminal ::= expression
// [SPEC] output_terminal ::= net_lvalue
mos_switch_instance
:                  '(' net_lvalue ',' expression ',' expression ')'
{
  parser.new_Inst(@$, $2, $4, $6);
}
| IDENTIFIER       '(' net_lvalue ',' expression ',' expression ')'
{
  parser.new_InstN(@$, $1, $3, $5, $7);
}
| IDENTIFIER '[' expression ':' expression ']'
  '(' net_lvalue ',' expression ',' expression ')'
{
  parser.new_InstV(@$, $1, $3, $5, $8, $10, $12);
}
;

// [SPEC] n_input_gate_instance ::= [name_of_gate_instance]
//                 '(' output_terminal ',' input_terminal
//                     {',' input_terminal} ')'
// [SPEC] name_of_gate_instance ::= gate_instance_identifier [ range ]
n_input_gate_instance
:                  '(' n_input_gate_terminals ')'
{
  parser.new_Inst(@$, $2);
}
| IDENTIFIER       '(' n_input_gate_terminals ')'
{
  parser.new_InstN(@$, $1, $3);
}
| IDENTIFIER '[' expression ':' expression ']' '(' n_input_gate_terminals ')'
{
  parser.new_InstV(@$, $1, $3, $5, $8);
}
;

// [SPEC] n_input_gate_terminals ::=
//             output_terminal ',' input_terminal {',' input_terminal}
// [SPEC] input_terminal ::= expression
// [SPEC] output_terminal ::= net_lvalue
n_input_gate_terminals
: net_lvalue ',' expression
{
  $$ = parser.new_list<PtConnection>();
  $$->push_back(parser.new_OrderedCon($1));
  $$->push_back(parser.new_OrderedCon($3));
}
| n_input_gate_terminals ',' expression
{
  $$ = $1;
  $$->push_back(parser.new_OrderedCon($3));
}
;

// [SPEC] n_output_gate_instance ::=
//             [name_of_gate_instance]
//                 '(' output_terminal {',' output_terminal}
//                   ',' input_terminal ')'
// [SPEC] name_of_gate_instance ::= gate_instance_identifier [ range ]
// [SPEC] input_terminal ::= expression
// [SPEC] output_terminal ::= net_lvalue
n_output_gate_instance
:                  '(' n_output_gate_terminals ')'
{
  parser.new_Inst(@$, $2);
}
| IDENTIFIER       '(' n_output_gate_terminals ')'
{
  parser.new_InstN(@$, $1, $3);
}
| IDENTIFIER '[' expression ':' expression ']' '(' n_output_gate_terminals ')'
{
  parser.new_InstV(@$, $1, $3, $5, $8);
}
;

// [SPEC] n_output_gate_terminals ::=
//             output_terminal {',' output_terminal} ', ' input_terminal
// [SPEC] input_terminal ::= expression
// [SPEC] output_terminal ::= net_lvalue
// ちょっと手抜きをしてすべて expression のリストと見なす．
n_output_gate_terminals
: expression ',' expression
{
  $$ = parser.new_list<PtConnection>();
  $$->push_back(parser.new_OrderedCon($1));
  $$->push_back(parser.new_OrderedCon($3));
}
| n_output_gate_terminals ',' expression
{
  $$ = $1;
  $$->push_back(parser.new_OrderedCon($3));
}
;

// [SPEC] pass_switch_instance ::=
//            [name_of_gate_instance] '(' inout_terminal ',' inout_terminal ')'
// [SPEC] name_of_gate_instance ::= gate_instance_identifier [ range ]
// [SPEC] inout_terminal ::= net_lvalue
pass_switch_instance
:                  '(' net_lvalue ',' net_lvalue ')'
{
  parser.new_Inst(@$, $2, $4);
}
| IDENTIFIER       '(' net_lvalue ',' net_lvalue ')'
{
  parser.new_InstN(@$, $1, $3, $5);
}
| IDENTIFIER '[' expression ':' expression ']' '(' net_lvalue ',' net_lvalue ')'
{
  parser.new_InstV(@$, $1, $3, $5, $8, $10);
}
;

// [SPEC] pass_enable_switch_instance ::=
//             [name_of_gate_instance] '(' inout_terminal ',' inout_terminal
//                 ',' enable_terminal ')'
// [SPEC] name_of_gate_instance ::= gate_instance_identifier [ range ]
// [SPEC] enable_terminal ::= expression
// [SPEC] inout_terminal ::= net_lvalue
pass_enable_switch_instance
:                  '(' net_lvalue ',' net_lvalue ',' expression ')'
{
  parser.new_Inst(@$, $2, $4, $6);
}
| IDENTIFIER       '(' net_lvalue ',' net_lvalue ',' expression ')'
{
  parser.new_InstN(@$, $1, $3, $5, $7);
}
| IDENTIFIER '[' expression ':' expression ']'
  '(' net_lvalue ',' net_lvalue ',' expression ')'
{
  parser.new_InstV(@$, $1, $3, $5, $8, $10, $12);
}
;

// [SPEC] pull_gate_instance ::=
//             [name_of_gate_instance] '(' output_terminal ')'
// [SPEC] name_of_gate_instance ::= gate_instance_identifier [ range ]
// [SPEC] output_terminal ::= net_lvalue
pull_gate_instance
:                  '(' net_lvalue ')'
{
  parser.new_Inst(@$, $2);
}
| IDENTIFIER       '(' net_lvalue ')'
{
  parser.new_InstN(@$, $1, $3);
}
| IDENTIFIER '[' expression ':' expression ']' '(' net_lvalue ')'
{
  parser.new_InstV(@$, $1, $3, $5, $8);
}
;


//////////////////////////////////////////////////////////////////////
// A.3.2 Primitive strengths
//////////////////////////////////////////////////////////////////////

// [SPEC] pulldown_strength ::=
//             '(' strength0 ',' strength1 ')'
//            |'(' strength1 ',' strength0 ')'
//            |'(' strength0 ')'
pulldown_strength
: '(' strength0 ',' strength1 ')'
{
  $$ = parser.new_Strength(@$, $2, $4);
}
| '(' strength1 ',' strength0 ')'
{
  $$ = parser.new_Strength(@$, $2, $4);
}
| '(' strength0 ')'
{
  $$ = parser.new_Strength(@$, $2);
}
;

// [SPEC] pullup_strength ::=
//             '(' strength0 ',' strength1 ')'
//            |'(' strength1 ',' strength0 ')'
//            |'(' strength1 ')'
pullup_strength
: '(' strength0 ',' strength1 ')'
{
  $$ = parser.new_Strength(@$, $2, $4);
}
| '(' strength1 ',' strength0 ')'
{
  $$ = parser.new_Strength(@$, $2, $4);
}
| '(' strength1 ')'
{
  $$ = parser.new_Strength(@$, $2);
}
;


//////////////////////////////////////////////////////////////////////
// A.3.3 Primitive terminals
//////////////////////////////////////////////////////////////////////

// [SPEC] enable_terminal ::= expression

// [SPEC] inout_terminal ::= net_lvalue

// [SPEC] input_terminal ::= expression

// [SPEC] ncontrol_terminal ::= expression

// [SPEC] output_terminal ::= net_lvalue

// [SPEC] pcontrol_terminal ::= expression



//////////////////////////////////////////////////////////////////////
// A.3.4 Primitive gate and swith types
//////////////////////////////////////////////////////////////////////

// [SPEC] cmos_switchtype ::= "cmos"|"rcmos"
cmos_switchtype
: CMOS
{
  $$ = kVpiCmosPrim;
}
| RCMOS
{
  $$ = kVpiRcmosPrim;
}
;

// [SPEC] enable_gatetype ::= "bufif0"|"bufif1"|"notif0"|"notif1"
enable_gatetype
: BUFIF0
{
  $$ = kVpiBufif0Prim;
}
| BUFIF1
{
  $$ = kVpiBufif1Prim;
}
| NOTIF0
{
  $$ = kVpiNotif0Prim;
}
| NOTIF1
{
  $$ = kVpiNotif1Prim;
}
;

// [SPEC] mos_switchtype ::= "nmos"|"pmos"|"rnmos"|"rpmos"
mos_switchtype
: NMOS
{
  $$ = kVpiNmosPrim;
}
| PMOS
{
  $$ = kVpiPmosPrim;
}
| RNMOS
{
  $$ = kVpiRnmosPrim;
}
| RPMOS
{
  $$ = kVpiRpmosPrim;
}
;

// [SPEC] n_input_gatetype ::= "and"|"nand"|"or"|"nor"|"xor"|"xnor"
n_input_gatetype
: AND
{
  $$ = kVpiAndPrim;
}
| NAND
{
  $$ = kVpiNandPrim;
}
| OR
{
  $$ = kVpiOrPrim;
}
| NOR
{
  $$ = kVpiNorPrim;
}
| XOR
{
  $$ = kVpiXorPrim;
}
| XNOR
{
  $$ = kVpiXnorPrim;
}
;

// [SPEC] n_output_gatetype ::= "buf"|"not"
n_output_gatetype
: BUF
{
  $$ = kVpiBufPrim;
}
| NOT
{
  $$ = kVpiNotPrim;
}
;

// [SPEC] pass_en_switchtype ::= "tranif0"|"tranif1"|"rtranif0"|"rtranif1"
pass_en_switchtype
: TRANIF0
{
  $$ = kVpiTranif0Prim;
}
| TRANIF1
{
  $$ = kVpiTranif1Prim;
}
| RTRANIF0
{
  $$ = kVpiRtranif0Prim;
}
| RTRANIF1
{
  $$ = kVpiRtranif1Prim;
}
;

// [SPEC] pass_switchtype ::= "tran"|"rtran"
pass_switchtype
: TRAN
{
  $$ = kVpiTranPrim;
}
| RTRAN
{
  $$ = kVpiRtranPrim;
}
;



//////////////////////////////////////////////////////////////////////
// A.4 Module and generated instantiation
//////////////////////////////////////////////////////////////////////
// A.4.1 Module instantiation
//////////////////////////////////////////////////////////////////////

// [SPEC] module_instantiation ::=
//             module_identifier [parameter_value_assignment]
//                 module_instance {',' module_instance } ';'
// [SPEC] udp_instantiation ::= udp_identifier [drive_strength] [delay2]
//             udp_instance {',' udp_instance} ';'
// [SPEC] module_identifier ::= identifier
// [SPEC] udp_identifier ::= identifier
// 最大の頭痛の種
// module_identifier と udp_identifier は区別不能
// なので両方の最大公約数的な仕様にしておく．
// [SPEC*] mu_instantiation ::=
//             identifier [parameter_value_assignment]
//                 mu_instance {',' mu_instance } ';'
//            |identifier [drive_strength] [delay2]
//                 mu_instance {',' mu_instance } ';'
// [SPEC] parameter_value_assignment ::=
//             '#' '(' list_of_parameter_assignments ')'
// [SPEC] list_of_parameter_assignments ::=
//             ordered_parameter_assignment {',' ordered_parameter_assignment }
//            |named_parameter_assignment {',' named_parameter_assignment }
// [SPEC] ordered_parameter_assignment ::= expression
// 最悪なことに delay2 とは '#' '(' expression ',' ,,,
// なので parameter_value_assignment と重なる．
// とりあえず delay2 と ordered_parameter_assignment は区別できない．
module_instantiation
: ai_list mu_head                                          nzlist_of_mu_inst ';'
{
  parser.new_MuH(@$, $2, $1);
}
| ai_list mu_head '#' '(' list_of_ordered_param_assign ')' nzlist_of_mu_inst ';'
{
  parser.new_MuH(@$, $2, $5, $1);
}
| ai_list mu_head '#' '(' list_of_named_param_assign ')'   nzlist_of_mu_inst ';'
{
  parser.new_MuH(@$, $2, $5, $1);
}
| ai_list mu_head '#' unumber                              nzlist_of_mu_inst ';'
             // これは delay2 の特殊形
{
  PtDelay* delay = parser.new_Delay(FileRegion(@3, @4), $4);
  parser.new_MuH(@$, $2, delay, $1);
}
| ai_list mu_head '#' rnumber                              nzlist_of_mu_inst ';'
             // これは delay2 の特殊形
{
  PtDelay* delay = parser.new_Delay(FileRegion(@3, @4), $4);
  parser.new_MuH(@$, $2, delay, $1);
}
| ai_list mu_head '#' IDENTIFIER                           nzlist_of_mu_inst ';'
             // これは delay2 の特殊形
{
  PtExpr* prim = parser.new_Primary(@4, $4);
  PtDelay* delay = parser.new_Delay(FileRegion(@3, @4), prim);
  parser.new_MuH(@$, $2, delay, $1);
}
| ai_list mu_head drive_strength                           nzlist_of_mu_inst ';'
{
  parser.new_MuH(@$, $2, $3, $1);
}
| ai_list mu_head drive_strength delay2                    nzlist_of_mu_inst ';'
{
  parser.new_MuH(@$, $2, $3, $4, $1);
}
| ai_list mu_head error ';'
{
  yyerrok;
}
;

mu_head
: IDENTIFIER
{
  $$ = $1;
  parser.init_inst();
}
;

// [SPEC*] list_of_ordered_param_assign ::=
//             expression { ',' expression }
list_of_ordered_param_assign
: expression
{
  $$ = parser.new_list<PtConnection>();
  $$->push_back(parser.new_OrderedCon($1));
}
| list_of_ordered_param_assign ',' expression
{
  $$ = $1;
  $$->push_back(parser.new_OrderedCon($3));
}
;

// [SPEC*] list_of_named_param_assign ::=
//             named_parameter_assignment { ',' named_parameter_assignment }
list_of_named_param_assign
: named_parameter_assignment
{
  $$ = parser.new_list<PtConnection>();
  $$->push_back($1);
}
| list_of_named_param_assign ',' named_parameter_assignment
{
  $$ = $1;
  $$->push_back($3);
}
;

// [SPEC] named_parameter_assignment ::=
//             '.'parameter_identifier'('[expression]')'
// [SPEC] parameter_identifier ::= identifier
named_parameter_assignment
: '.' IDENTIFIER '(' ')'
{
  $$ = parser.new_NamedCon(@$, $2);
}
| '.' IDENTIFIER '(' expression ')'
{
  $$ = parser.new_NamedCon(@$, $2, $4);
}
;

// [SPEC*] nzlist_of_module_inst ::=
//             module_instance { ',' module_instance }
nzlist_of_mu_inst
: mu_instance
| nzlist_of_mu_inst ',' mu_instance
;

// [SPEC] module_instance ::=
//             name_of_instance '(' [list_of_port_connections] ')'
// [SPEC] name_of_instance ::= module_instance_identifier [ range ]
// [SPEC] list_of_port_connections ::=
//             orderd_port_connection {',' ordered_port_connection }
//            |named_port_connection {',' named_port_connection }
// [SPEC] udp_instance ::= [name_of_udp_instance] '(' output_terminal ','
//             input_terminal {',' input_terminal} ')'
// [SPEC] name_of_udp_instance ::= udp_instance_identifier [range]
// [SPEC] input_terminal ::= expression
// [SPEC] output_terminal ::= net_lvalue
//
// 嘘つき！ udp_instance_identifier は arrayed_identifier なので中に
// [range] を含んでいる．この仕様書をみてプログラム作ってない証拠
//
// 微妙に異なるので以下のようにする．
// [SPEC*] mu_instance ::=
//             [name_of_instance] '(' [list_of_port_connections] ')'
// [SPEC*] name_of_instance ::= identifier [range]
// ただし udp_instance の場合には named_port_connection はない．
// これはここでは判定できないのであとで調べる．
// 空のリスト '(' ')' の場合も 1 つの ai_list (ordered_port_connection)
// を要素として持つリストと認識されるのであとで特別に扱う必要がある．
mu_instance
:                  '(' list_of_ordered_port_connections ')'
{
  parser.new_Inst(@$, $2);
}
|                  '(' list_of_named_port_connections ')'
{
  parser.new_Inst(@$, $2);
}
| IDENTIFIER       '(' list_of_ordered_port_connections ')'
{
  parser.new_InstN(@$, $1, $3);
}
| IDENTIFIER       '(' list_of_named_port_connections ')'
{
  parser.new_InstN(@$, $1, $3);
}
| IDENTIFIER '[' expression ':' expression ']'
  '(' list_of_ordered_port_connections ')'
{
  parser.new_InstV(@$, $1, $3, $5, $8);
}
| IDENTIFIER '[' expression ':' expression ']'
  '(' list_of_named_port_connections ')'
{
  parser.new_InstV(@$, $1, $3, $5, $8);
}
;

// [SPEC*] list_of_ordered_port_connection ::=
//            [ ordered_port_connection { ',' ordered_port_connection }]
// ordered_port_connection は空も含める．
list_of_ordered_port_connections
: ordered_port_connection
{
  $$ = parser.new_list<PtConnection>();
  $$->push_back($1);
}
| list_of_ordered_port_connections ',' ordered_port_connection
{
  $$ = $1;
  $$->push_back($3);
}
;

// [SPEC] ordered_port_connection ::= { attribute_instance } [expression]
ordered_port_connection
: // 空
{
  // でも NULL は返さない．
  $$ = parser.new_OrderedCon(NULL);
}
| expression
{
  $$ = parser.new_OrderedCon($1);
}
| nz_ai_list
{
  $$ = parser.new_OrderedCon(@1, NULL, $1);
}
| nz_ai_list expression
{
  $$ = parser.new_OrderedCon(@$, $2, $1);
}
;

// [SPEC*] list_of_named_port_connections ::=
//            named_port_connection { ',' named_port_connection }
list_of_named_port_connections
: named_port_connection
{
  $$ = parser.new_list<PtConnection>();
  $$->push_back($1);
}
| list_of_named_port_connections ',' named_port_connection
{
  $$ = $1;
  $$->push_back($3);
}
;

// [SPEC] named_port_connection ::=
//            { attribute_instance } '.' port_identifier '(' [ expression ] ')'
// [SPEC] port_identifier ::= identifier
named_port_connection
: '.' IDENTIFIER '(' ')'
{
  $$ = parser.new_NamedCon(@$, $2);
}
| '.' IDENTIFIER '(' expression ')'
{
  $$ = parser.new_NamedCon(@$, $2, $4);
}
| nz_ai_list '.' IDENTIFIER '(' ')'
{
  $$ = parser.new_NamedCon(@$, $3, NULL, $1);
}
| nz_ai_list '.' IDENTIFIER '(' expression ')'
{
  $$ = parser.new_NamedCon(@$, $3, $5, $1);
}
;


//////////////////////////////////////////////////////////////////////
// A.4.2 Generated instantiation
//////////////////////////////////////////////////////////////////////

// [SPEC] generated_instantiation ::=
//             "generate" { generate_item } "endgenerate"
generated_instantiation
: ai_list generate_head list_of_generate_items ENDGENERATE
{
  parser.new_Generate(@$, $1);
}
| ai_list generate_head error ENDGENERATE
{
  // generate 中のエラーは endgenerate まで読み飛ばす
  parser.end_generate();
  yyerrok;
}
;

generate_head
: GENERATE
{
  parser.init_generate();
}
;

// [SPEC*] list_of_generate_items ::= { generate_item }
list_of_generate_items
: // 空
| list_of_generate_items generate_item
| list_of_generate_items module_or_generate_decl
;

// [SPEC] generate_item_or_null ::= [ generate_item ]
generate_item_or_null
: ';'
| generate_item
| module_or_generate_decl
;

// [SPEC] generate_item ::=
//             generate_conditional_statement
//            |generate_case_statement
//            |generate_loop_statement
//            |generate_block
//            |module_or_generate_item
// [SPEC] generate_conditional_statement ::=
//             "if" '(' constant_expression ')' generate_item_or_null
//                 [ "else" generate_item_or_null ]
// [SPEC] generate_case_statement ::=
//             "case" '(' constant_expression ')'
//                 genvar_case_item { genvar_case_item } "endcase"
// [SPEC] generate_loop_statement ::=
//             "for" '(' genvar_assignment ';' constant_expression ';'
//                 genvar_assignment ')'
//                 "begin" ';' generate_block_identifier { generate_item }
//                 "end"
// [SPEC] generate_block ::=
//             "begin" [ ':' generate_block_identifier ] { generate_item }
//             "end"
// [SPEC] genvar_assignment ::= genvar_identifier '=' constant_expression
// [SPEC] genvar_identifier ::= identifier
// generate_item_or_null の場合実際には module_or_generate_decl の場合が
// あることを忘れてはならない．
generate_item
: gen_if '(' expression ')' generate_item_or_null %prec LOWER_THAN_ELSE
{
  parser.new_GenIf(@$, $3);
}
| gen_if '(' expression ')' generate_item_or_null
  gen_else generate_item_or_null
{
  parser.new_GenIfElse(@$, $3);
}
| gen_if error gen_else { yyerrok; } generate_item_or_null
{
  parser.end_genif();
}
| CASE '(' expression ')' list_of_gencaseitem ENDCASE
{
  parser.new_GenCase(@$, $3, $5);

}
| CASE error ENDCASE
{
  yyerrok;
}
| FOR '(' IDENTIFIER '=' expression ';' expression ';'
          IDENTIFIER '=' expression ')'
  gen_begin ':' IDENTIFIER
  list_of_generate_items
  END
{
  parser.new_GenFor(@$, $3, $5, $7, $9, $11, $15);
}
| FOR '(' IDENTIFIER '=' expression ';' expression ';'
          IDENTIFIER '=' expression ')'
  gen_begin error
  END
{
  parser.end_generate();
  yyerrok;
}
| FOR error END
{
  yyerrok;
}
| gen_begin list_of_generate_items END
{
  parser.new_GenBlock(@$);
}
| gen_begin ':' IDENTIFIER list_of_generate_items END
{
  parser.new_GenBlock(@$, $3);
}
| gen_begin error END
{
  parser.end_generate();
  yyerrok;
}
| module_or_generate_item
;

gen_if
: IF
{
  parser.init_genif();
}
;

gen_else
: ELSE
{
  parser.init_else();
}
;

gen_begin
: BEGIN
{
  parser.init_generate();
}
;

// [SPEC*] list_of_gencaseitem ::=
//             genvar_caseitem { genvar_caseitem }
list_of_gencaseitem
: genvar_case_item
{
  $$ = parser.new_list<PtGenCaseItem>();
  $$->push_back($1);
}
| list_of_gencaseitem genvar_case_item
{
  $$ = $1;
  $$->push_back($2);
}
;

// [SPEC] genvar_case_item ::=
//             constant_expression {',' constant_expression } ':'
//                 generate_item_or_null
//            |"default" [':'] generate_item_or_null
genvar_case_item
: nzlist_of_expressions ':' { parser.init_generate(); } generate_item_or_null
{
  $$ = parser.new_GenCaseItem(@$, $1);
}
| DEFAULT                   { parser.init_generate(); } generate_item_or_null
{
  $$ = parser.new_GenCaseItem(@$, NULL);
}
| DEFAULT               ':' { parser.init_generate(); } generate_item_or_null
{
  $$ = parser.new_GenCaseItem(@$, NULL);
}
;


//////////////////////////////////////////////////////////////////////
// A.5.1 UDP declaration
//////////////////////////////////////////////////////////////////////

// [SPEC] udp_declaration ::=
//             { attribute_instance } "primitive" udp_identifier
//                 '(' udp_port_list ')' ';'
//                 udp_port_declaration { udp_port_declaration }
//                 udp_body
//             "endprimitive"
//            |{ attribute_instance } "primitive" udp_identifier
//                 '(' udp_declaration_port_list ')' ';'
//                 udp_body
//             "endprimitive"
// 実はもとの仕様とかなり違う文法で読み込んでいる．
udp_declaration
: ai_list udp_head IDENTIFIER '(' udp_port_list ')' ';'
  nzlist_of_uport_decl
  TABLE
  list_of_udp_entries
  ENDTABLE
  ENDPRIMITIVE
{
  parser.new_Udp1995(@$, $3, NULL, FileRegion(), NULL, $1);
}
| ai_list udp_head IDENTIFIER '(' udp_port_list ')' ';'
  nzlist_of_uport_decl
  INITIAL IDENTIFIER '=' init_val ';'
  TABLE
  list_of_udp_entries
  ENDTABLE
  ENDPRIMITIVE
{
  parser.new_Udp1995(@$, $3, $10, @10, $12, $1);
}
| ai_list udp_head IDENTIFIER
  '(' udp_declaration_port_list ')' ';' { parser.end_io(); }
  TABLE
  list_of_udp_entries
  ENDTABLE
  ENDPRIMITIVE
{
  parser.new_Udp2001(@$, $3, NULL, FileRegion(), NULL, $1);
}
| ai_list udp_head IDENTIFIER
  '(' udp_declaration_port_list ')' ';' { parser.end_io(); }
  INITIAL IDENTIFIER '=' init_val ';'
  TABLE
  list_of_udp_entries
  ENDTABLE
  ENDPRIMITIVE
{
  parser.new_Udp2001(@$, $3, $10, @10, $12, $1);
}
| ai_list udp_head error ENDPRIMITIVE
{
  parser.end_udp();
  yyerrok;
}
;

// ちょっとした Hack
udp_head
: PRIMITIVE
{
  parser.init_udp();
}
;


//////////////////////////////////////////////////////////////////////
// A.5.2 UDP ports
//////////////////////////////////////////////////////////////////////

// [SPEC] udp_port_list ::= output_port_identifier ','
//             input_port_identifier {',' input_port_identifier }
// [SPEC] output_port_identifier ::= identifier
// [SPEC] input_port_identifier ::= identifier
// 相変わらずアホな構文規則
// 要は udp_port_list ::= identifier ',' identifier {',' identifier}
// でしょ．
udp_port_list
: udp_port      ',' udp_port // 最低2つはある．
| udp_port_list ',' udp_port
;

udp_port
: IDENTIFIER
{
  parser.new_Port2(@1, $1);
}
;

// [SPEC] udp_declaration_port_list ::= udp_output_declaration ','
//             udp_input_declaration {',' udp_input_declaration}
// [SPEC] udp_input_declaration ::=
//             { attribute_instance } "input" list_of_port_identifiers
// [SPEC] list_of_port_identifiers ::= port_identifier {',' port_identifier}
// もういい加減この2重リストの問題はうんざり．
// 他のIO宣言と違ってここでは XXX_declhead の中で { attribute_instance }
// の処理をする．
udp_declaration_port_list
: udp_output_declaration udp_comma udp_input_declhead port_identifier_item
| udp_declaration_port_list udp_comma udp_input_declhead port_identifier_item
| udp_declaration_port_list udp_comma port_identifier_item
;

udp_comma
: ','
{
  parser.end_io();
}
;

// [SPEC*] nzlist_of_uport_decl ::=
//            udp_port_declaration { udp_port_declaration }
nzlist_of_uport_decl
: udp_output_declaration
| udp_input_declaration
| udp_reg_declaration
| nzlist_of_uport_decl udp_output_declaration
| nzlist_of_uport_decl udp_input_declaration
| nzlist_of_uport_decl udp_reg_declaration
;

// [SPEC] udp_port_declaration ::=
//             udp_output_declaration ';'
//            |udp_input_declaration ';'
//            |udp_reg_declaration ';'

// [SPEC] udp_output_declaration ::=
//             { attribute_instance } "output"       port_identifier
//            |{ attribute_instance } "output" "reg" port_identifier
//                 [ '=' constant_expression]
// 最後の port_identifier [ '=' constant_expression ] は
// variable_port_identifier として処理する．
udp_output_declaration
: udp_output_declhead1 port_identifier_item ';'
{
  parser.end_io();
}
| udp_output_declhead2 variable_port_identifier_item ';'
{
  parser.end_io();
}
;

udp_output_declhead1
: ai_list OUTPUT
{
  // UDP は1ビット符号なしのみ
  parser.new_IOHead(@$, kPtIO_Output, false, $1);
}
;

udp_output_declhead2
: ai_list OUTPUT REG
{
  // UDP は1ビット符号なしのみ
  parser.new_RegIOHead(@$, kPtIO_Output, false, $1);
}
;

// [SPEC] udp_input_declaration ::=
//             { attribute_instance } "input" list_of_port_identifiers
// [SPEC] list_of_port_identifiers ::= port_identifier {',' port_identifier}
udp_input_declaration
: udp_input_declhead list_of_port_identifiers ';'
{
  parser.end_io();
}
;

// [SPEC*] udp_input_declhead ::= { attribute_instance } "input"
udp_input_declhead
: ai_list INPUT
{
  // UDP は1ビット符号無しのみ
  parser.new_IOHead(@$, kPtIO_Input, false, $1);
}
;

// [SPEC] udp_reg_declaration ::=
//             { attribute_instance } "reg" variable_identifier
udp_reg_declaration
: ai_list REG IDENTIFIER ';'
{
  parser.new_RegH(@$, false, $1);
  parser.new_DeclItem(@3, $3);
  parser.end_decl();
}
;


//////////////////////////////////////////////////////////////////////
// A.5.3 UDP body
//////////////////////////////////////////////////////////////////////

// [SPEC] udp_body ::=
//             combinational_body
//            |sequential_body
// [SPEC] combinational_body ::=
//             "table"
//               combinational_entry { combinational_entry }
//             "endtable"
// [SPEC] sequential_body ::=
//             [ udp_initial_statement ]
//             "table"
//               sequential_entry { sequential_entry }
//             "endtable"
// [SPEC] udp_initial_statement ::=
//             "initial" output_port_identifier '=' init_val ';'
// [SPEC] output_port_identifier ::= identifier
// [SPEC] init_val ::= 1'b0|1'b1|1'bx|1'bX|1'B0|1'B1|1'Bx|1'BX|1|0
// 本当は combinational_entry と sequential_entry が混在しているのは
// なしだが，構文的にチェックするのは面倒くさいので，パーズしておいてから
// あとでチェックする．
init_val
: UNUM_INT
{ // 実際には {1|0} のみ
  if ( $1 != 0 && $1 != 1 ) {
    parser.put_msg(__FILE__, __LINE__,
		   @1,
		   kMsgError,
		   "PARSER",
		   "expectiong 1 or 0.");
    YYERROR;
  }
  else {
    $$ = parser.new_IntConst(@1, $1);
  }
}
| UNUM_INT BASE_B UNUMBER
{ // 実際には 1'b{0|1|x} のみ
  if ( $1 != 1 ) {
    parser.put_msg(__FILE__, __LINE__,
		   @1,
		   kMsgError,
		   "PARSER",
		   "UDP's initial constant size should be 1.");
    YYERROR;
  }
  else if ( strcmp($3, "z") == 0 ||
	    strcmp($3, "Z") == 0 ) {
    parser.put_msg(__FILE__, __LINE__,
		   @3,
		   kMsgError,
		   "PARSER",
		   "Z value is not allowd here.");
    YYERROR;
  }
  else {
    $$ = parser.new_IntConst(@$, 1, kVpiBinaryConst, $3);
  }
}
| UNUM_INT wrong_base UNUMBER
{
  parser.put_msg(__FILE__, __LINE__,
		 @1,
		 kMsgError,
		 "PARSER",
		 "illegal base: only 'b is allowed here.");
  YYERROR;
}
| UNUM_BIG BASE_B UNUMBER
{
  parser.put_msg(__FILE__, __LINE__,
		 @1,
		 kMsgError,
		 "PARS",
		 "UDP's initial constant size should be 1.");
  YYERROR;
}
| numbase UNUMBER
{
  parser.put_msg(__FILE__, __LINE__,
		 @1,
		 kMsgError,
		 "PARSER",
		 "UDP's initial constant size should be 1.");
  YYERROR;
}
;

wrong_base
: BASE_SB
| BASE_O
| BASE_SO
| BASE_D
| BASE_SD
| BASE_H
| BASE_SH
;

// [SPEC*] list_of_udp_entries ::=
//             udp_entry { udp_entry }
list_of_udp_entries
:                     { parser.init_udp_value_list(); } udp_entry
| list_of_udp_entries { parser.init_udp_value_list(); } udp_entry
;

// [SPEC*] udp_entry ::= combinational_entry | sequential_entry
// [SPEC] combinational_entry ::= level_input_list ':' output_symbol ';'
// [SPEC] sequential_entry ::=
//             seq_input_list ':' current_state ':' next_state ';'
// [SPEC] seq_input_list ::= level_input_list | edge_input_list
// [SPEC] current_state ::= level_symbol
udp_entry
: seq_input_list ':' output_symbol ';'
{
  parser.new_UdpEntry(@$, @3, $3);
}
| seq_input_list ':' level_symbol ':' next_state ';'
{
  parser.new_UdpEntry(@$, @3, $3, @5, $5);
}
| error ';'
{
  yyerrok;
}
;

// [SPEC] seq_input_list ::= level_input_list | edge_input_list
// [SPEC] level_input_list ::= level_symbol { level_symbol }
// [SPEC] edge_input_list ::= { level_symbol } edge_indicator { level_symbol }
// これをこのまま書くとコンフリクトが起きる．
// 良くみてみると, seq_input_list とは
// level_symbol と edge_indicator の空でないリストのうち，
// edge_indicator を2つ以上含まないものだということがわかる．
// edge_indicator をいくつ含んでいるかのチェックはあとで行う．
seq_input_list
: level_symbol_value
| edge_indicator
| seq_input_list level_symbol_value
| seq_input_list edge_indicator
;

// [SPEC] edge_indicator ::= '(' level_symbol level_symbol ')' | edge_symbol
edge_indicator
: '(' level_symbol level_symbol ')'
{
  parser.new_UdpValue(@$, merge_udp_value($2, $3));
}
| edge_symbol
{
  parser.new_UdpValue(@1, $1);
}
;

// [SPEC*] level_symbol_value ::= level_symbol
level_symbol_value
: level_symbol
{
  parser.new_UdpValue(@1, $1);
}
;

// [SPEC] next_state ::= output_symbol | '-'
next_state
: output_symbol
{
  $$ = $1;
}
| '-'
{
  $$ = kVpiUdpValNC;
}
;

// [SPEC] output_symbol ::= '0' | '1' | 'x' | 'X'
output_symbol
: '0'
{
  $$ = kVpiUdpVal0;
}
| '1'
{
  $$ = kVpiUdpVal1;
}
| 'x'
{
  $$ = kVpiUdpValX;
}
;

// [SPEC] level_symbol ::= '0' | '1' | 'x' | 'X' | '?' | 'b' | 'B'
level_symbol
: output_symbol
{
  $$ = $1;
}
| '?'
{
  $$ = kVpiUdpValQ;
}
| 'b'
{
  $$ = kVpiUdpValB;
}
;

// [SPEC] edge_symbol ::= 'r' | 'R' | 'f' | 'F' | 'p' | 'P' | 'n' | 'N' | '*'
edge_symbol
: 'r'
{
  $$ = kVpiUdpValR;
}
| 'f'
{
  $$ = kVpiUdpValF;
}
| 'p'
{
  $$ = kVpiUdpValP;
}
| 'n'
{
  $$ = kVpiUdpValN;
}
| '*'
{
  $$ = kVpiUdpValQQ;
}
;


//////////////////////////////////////////////////////////////////////
// A.5.4 UDP instantiation
//////////////////////////////////////////////////////////////////////

// 前述の様に UDP instantiation は module instantiation と区別できないので
// いっしょに扱う．

// [SPEC] udp_instantiation ::= udp_identifier [drive_strength] [delay2]
//             udp_instance {',' udp_instance} ';'
// [SPEC] udp_instance ::= [name_of_udp_instance] '(' output_terminal ','
//             input_terminal {',' input_terminal} ')'
// [SPEC] name_of_udp_instance ::= udp_instance_identifier [range]
// [SPEC] input_terminal ::= expression
// [SPEC] output_terminal ::= net_lvalue


//////////////////////////////////////////////////////////////////////
// A.6 Behavioral statements
//////////////////////////////////////////////////////////////////////
// A.6.1 Contiuous assignment statements
//////////////////////////////////////////////////////////////////////

// [SPEC] continuous_assign ::=
//             "assign" [drive_stregth] [delay3] list_of_net_assignments ';'
continuous_assign
: ai_list assign_head                       list_of_net_assignments ';'
{
  parser.new_ContAssignH(@$, $1);
}
| ai_list assign_head                delay3 list_of_net_assignments ';'
{
  parser.new_ContAssignH(@$, $3, $1);
}
| ai_list assign_head drive_strength        list_of_net_assignments ';'
{
  parser.new_ContAssignH(@$, $3, $1);
}
| ai_list assign_head drive_strength delay3 list_of_net_assignments ';'
{
  parser.new_ContAssignH(@$, $3, $4, $1);
}
| ai_list assign_head error ';'
{
  yyerrok;
}
;

assign_head
: ASSIGN
{
  parser.init_contassign();
}
;

// [SPEC] list_of_net_assignments ::= net_assignment {',' net_assignments }
list_of_net_assignments
: net_assignment
| list_of_net_assignments ',' net_assignment
;

// [SPEC] net_assignment ::= net_lvalue '=' expression
net_assignment
: net_lvalue '=' expression
{
  parser.new_ContAssign(@$, $1, $3);
}
;


//////////////////////////////////////////////////////////////////////
// A.6.2 Procedural blocks and assignments
//////////////////////////////////////////////////////////////////////

// [SPEC] initial_construct ::= "initial" statement
initial_construct
: ai_list INITIAL statement
{
  parser.new_Initial(@$, $3, $1);
}
;

// [SPEC] always_construct ::= "always" statement
always_construct
: ai_list ALWAYS statement
{
  parser.new_Always(@$, $3, $1);
}
;

// [SPEC] blocking_assignment ::=
//             variable_lvalue '=' [ delay_or_event_control ] expression
// delay_control/event_control/repeat_control を区別する．
// 末尾のセミコロンはこちらで処理する．
blocking_assignment
: ai_list variable_lvalue '='               expression ';'
{
  $$ = parser.new_Assign(@$, $2, $4, $1);
}
| ai_list variable_lvalue '=' delay_control expression ';'
{
  $$ = parser.new_Assign(@$, $2, $5, $4, $1);
}
| ai_list variable_lvalue '=' event_control expression ';'
{
  $$ = parser.new_Assign(@$, $2, $5, $4, $1);
}
| ai_list variable_lvalue '=' repeat_control expression ';'
{
  $$ = parser.new_Assign(@$, $2, $5, $4, $1);
}
;

// [SPEC] nonblocking_assignment ::=
//             variable_lvalue "<=" [ delay_or_event_control ] expression
// 末尾のセミコロンはこちらで処理する．
nonblocking_assignment
: ai_list variable_lvalue LTEQ               expression ';'
{
  $$ = parser.new_NbAssign(@$, $2, $4, $1);
}
| ai_list variable_lvalue LTEQ delay_control expression ';'
{
  $$ = parser.new_NbAssign(@$, $2, $5, $4, $1);
}
| ai_list variable_lvalue LTEQ event_control expression ';'
{
  $$ = parser.new_NbAssign(@$, $2, $5, $4, $1);
}
| ai_list variable_lvalue LTEQ repeat_control expression ';'
{
  $$ = parser.new_NbAssign(@$, $2, $5, $4, $1);
}
;

// [SPEC] procedural_continuous_assignments ::=
//             "assign" variable_assignment
//            |"deassign" variable_lvalue
//            |"force" variable_assignment
//            |"release" variabble_lvalue
//            |"release" net_lvalue
// [SPEC] variable_assignment ::= variable_lvalue '=' expression
// variable_lvalue と net_lvalue は区別できないので最後の形式は除く
// どうでもいいけど何でこの名前が複数形なのか不思議
// 末尾のセミコロンはこちらで処理する．
procedural_continuous_assignments
: ai_list ASSIGN variable_lvalue '=' expression ';'
{
  $$ = parser.new_PcAssign(@$, $3, $5, $1);
}
| ai_list DEASSIGN variable_lvalue ';'
{
  $$ = parser.new_Deassign(@$, $3, $1);
}
| ai_list FORCE variable_lvalue '=' expression ';'
{
  $$ = parser.new_Force(@$, $3, $5, $1);
}
| ai_list RELEASE variable_lvalue ';'
{
  $$ = parser.new_Release(@$, $3, $1);
}
;

// [SPEC] function_blocking_assignment ::= variable_lvalue '=' expression

// [SPEC] function_statement_or_null ::=
//             function_statement
//            |{ attribute_innstance } ';'

//////////////////////////////////////////////////////////////////////
// A.6.3 Parallel and sequential blocks
//////////////////////////////////////////////////////////////////////

// [SPEC] function_seq_block ::=
//             "begin" [':' block_identifer { block_item_declaration }]
//                 { function_statement }
//             "end"

// [SPEC] par_block ::=
//             "fork" [':' block_identifier { block_item_declaration }]
//                 { statement }
//             "join"

par_block
: ai_list FORK
  JOIN
{
  $$ = parser.new_ParBlock(@$, NULL, $1);
}
| ai_list FORK
    nzlist_of_stmt
  JOIN
{
  $$ = parser.new_ParBlock(@$, $3, $1);
}
| ai_list fork ':' IDENTIFIER
    list_of_bitem_decl
  join
{
  $$ = parser.new_NamedParBlock(@$, $4, NULL, $1);
}
| ai_list fork ':' IDENTIFIER
    list_of_bitem_decl
    nzlist_of_stmt
  join
{
  $$ = parser.new_NamedParBlock(@$, $4, $6, $1);
}
| ai_list FORK
    error
  JOIN
{
  $$ = NULL;
  yyerrok;
}
;

fork
: FORK
{
  parser.init_block();
}
;

join
: JOIN
{
  parser.end_block();
}
;

// [SPEC] seq_block ::=
//             "begin" [':' block_identifier { block_item_declaration }]
//                 { statement }
//             "end"

seq_block
: ai_list BEGIN
  END
{
  $$ = parser.new_SeqBlock(@$, NULL, $1);
}
| ai_list BEGIN
    nzlist_of_stmt
  END
{
  $$ = parser.new_SeqBlock(@$, $3, $1);
}
| ai_list begin ':' IDENTIFIER
    list_of_bitem_decl
  end
{
  $$ = parser.new_NamedSeqBlock(@$, $4, NULL, $1);
}
| ai_list begin ':' IDENTIFIER
    list_of_bitem_decl
    nzlist_of_stmt
  end
{
  $$ = parser.new_NamedSeqBlock(@$, $4, $6, $1);
}
| ai_list BEGIN
    error
  END
{
  $$ = NULL;
  yyerrok;
}
;

begin
: BEGIN
{
  parser.init_block();
}
;

end
: END
{
  parser.end_block();
}

// [SPEC*] list_of_stmt ::= { statement }
nzlist_of_stmt
: statement
{
  $$ = parser.new_list<PtStmt>();
  $$->push_back($1);
}
| nzlist_of_stmt statement
{
  $$ = $1;
  $$->push_back($2);
}
;

// [SPEC] variable_assignment ::= variable_lvalue '=' expression
// なぜこの定義がここにあるのか不思議
variable_assignment
: variable_lvalue '=' expression
{
  $$ = parser.new_Assign(@$, $1, $3, NULL);
}
;


//////////////////////////////////////////////////////////////////////
// A.6.5 Statements
//////////////////////////////////////////////////////////////////////

// [SPEC] statement ::=
//             { attribute_instance } blocking_assignment ';'
//            |{ attribute_instance } case_statement
//            |{ attribute_instance } conditional_statement
//            |{ attribute_instance } disable_statement
//            |{ attribute_instance } event_trigger
//            |{ attribute_instance } loop_statement
//            |{ attribute_instance } nonblocking_assignment ';'
//            |{ attribute_instance } par_block
//            |{ attribute_instance } procedural_continuous_assignments ';'
//            |{ attribute_instance } procedural_timing_control_statement
//            |{ attribute_instance } seq_block
//            |{ attribute_instance } system_task_enable
//            |{ attribute_instance } task_enable
//            |{ attribute_instance } wait_statement
// statement の頭の { attribute_instance } は個別のクラスで
// 処理する．
// blocking_assignment/nonblocking_assignment/
// procedural_continuous_assignments の末尾のセミコロンは
// blocking_assignment/nonblocking_assignment/
// procedural_continuous_assignments 側で処理する．
statement
: blocking_assignment
{
  $$ = $1;
}
| case_statement
{
  $$ = $1;
}
| conditional_statement
{
  $$ = $1;
}
| disable_statement
{
  $$ = $1;
}
| event_trigger
{
  $$ = $1;
}
| loop_statement
{
  $$ = $1;
}
| nonblocking_assignment
{
  $$ = $1;
}
| par_block
{
  $$ = $1;
}
| procedural_continuous_assignments
{
  $$ = $1;
}
| procedural_timing_control_statement
{
  $$ = $1;
}
| seq_block
{
  $$ = $1;
}
| system_task_enable
{
  $$ = $1;
}
| task_enable
{
  $$ = $1;
}
| wait_statement
{
  $$ = $1;
}
| error ';'
{
  $$ = NULL;
  yyerrok;
}
;

// [SPEC] statement_or_null ::=
//             statement
//            |{ attribute_instance } ';'
statement_or_null
: statement
{
  $$ = $1;
}
| ai_list ';'
{
  $$ = parser.new_NullStmt(@$, $1);
}
;

// [SPEC] function_statement ::=
//             { attribute_instance } function_blocking_assignment ';'
//            |{ attribute_instance } function_case_statement
//            |{ attribute_instance } function_conditional_statement
//            |{ attribute_instance } function_loop_statement
//            |{ attribute_instance } function_seq_block
//            |{ attribute_instance } disable_statement
//            |{ attribute_instance } system_task_enable
// function_statement は statement で処理しておいてあとで function
// では用いることの出来ない statement を弾くようにする．
// このほうが単なる parse error よりも分かりやすいエラーメッセージを
// 出力できる．


//////////////////////////////////////////////////////////////////////
// A.6.5 Timing control statements
//////////////////////////////////////////////////////////////////////

// [SPEC] delay_control ::=
//             '#' delay_value
//            |'#' '(' mintypmax_expression ')'
// [SPEC] delay_value ::=
//             unsigned number
//            |parameter_identifier
//            |specparam_identifier
//            |mintypmax_expression
// あれ？ delay_value に mintypmax_expression は含まれているんですけど．
// ということで2番目の形は冗長なので取り除いた．
delay_control
: '#' delay_value
{
  $$ = parser.new_DelayControl(@$, $2);
}
| '#' '(' mintypmax_expression ')'
{
  $$ = parser.new_DelayControl(@$, $3);
}
;

// [SPEC] delay_or_event_control ::=
//             delay_control
//            |event_control
//            |"repeat" '(' expression ')' event_control

// [SPEC*] repeat_control ::=
//            "repeat" '(' expression ')' event_control
repeat_control
: REPEAT '(' expression ')' '@' hierarchical_identifier
{
  $$ = parser.new_RepeatControl(@$, $3, $6, @6);
}
| REPEAT '(' expression ')' '@' IDENTIFIER
{
  $$ = parser.new_RepeatControl(@$, $3, $6, @6);
}
| REPEAT '(' expression ')' '@' '(' event_expression ')'
{
  $$ = parser.new_RepeatControl(@$, $3, $7);
}
| REPEAT '(' expression ')' any_event
{
  $$ = parser.new_RepeatControl(@$, $3);
}
;

// [SPEC] disable_statement ::=
//             "disable" hierarchical_task_identifier ';'
//            |"disable" hierarchical_block_identifier ';'
// [SPEC] hierarchical_task_identifier ::= hierarchical_identifier
// [SPEC] hierarchical_block_identifier ::= hierarchical_identifier
// こういう意味的な区別を構文規則に持ち込むなっちゅうの
disable_statement
: ai_list DISABLE hierarchical_identifier ';'
{
  $$ = parser.new_Disable(@$, $3, $1);
}
| ai_list DISABLE IDENTIFIER ';'
{
  $$ = parser.new_Disable(@$, $3, $1);
}
;

// [SPEC] event_control ::=
//             '@' event_identifier
//            |'@' '(' event_expression ')'
//            |'@' '*'
//            |'@' '(' '*' ')'
// [SPEC] event_identifier ::= identifier
// なのだが，Verilog-XL では hierarchical_identifier でも通るみたい．
event_control
: '@' hierarchical_identifier
{
  $$ = parser.new_EventControl(@$, $2, @2);
}
| '@' IDENTIFIER
{
  $$ = parser.new_EventControl(@$, $2, @2);
}
| '@' '(' event_expression ')'
{
  $$ = parser.new_EventControl(@$, $3);
}
| any_event
{
  $$ = parser.new_EventControl(@$);
}
;

// [SPEC*] any_event ::= '@' '*' | '@' '(' '*' ')'
// 実際には attribute_instance の (*, *) との混同を防ぐためにもう少し
// 複雑なルールになる．
any_event
: '@' '*'
| '@' '(' '*' ')'
| '@' PRSTAR ')'
| '@' '(' STARPR
;

// [SPEC] event_trigger ::= "->" hierarchical_event_identifer ';'
// [SPEC] hierarchical_event_identifier ::= hierarchical_identifier
// あれ？ イベントって次元をもった配列として定義できるので
// [SPEC*] hierarchical_identifier {'[' expression ']} じゃないの？
event_trigger
: ai_list MINUSGT hierarchical_identifier ';'
{
  PtExpr* primary = parser.new_Primary(@3, $3);
  $$ = parser.new_EventStmt(@$, primary, $1);
}
| ai_list MINUSGT IDENTIFIER ';'
{
  PtExpr* primary = parser.new_Primary(@3, $3);
  $$ = parser.new_EventStmt(@$, primary, $1);
}
| ai_list MINUSGT hierarchical_identifier nzlist_of_index ';'
{
  PtExpr* primary = parser.new_Primary(FileRegion(@3, @4), $3, $4);
  $$ = parser.new_EventStmt(@$, primary, $1);
}
| ai_list MINUSGT IDENTIFIER nzlist_of_index ';'
{
  PtExpr* primary = parser.new_Primary(FileRegion(@3, @4), $3, $4);
  $$ = parser.new_EventStmt(@$, primary, $1);
}
;


// [SPEC] event_expression ::=
//             expression
//            |hierarchical_identifier
//            |"posedge" expression
//            |"negedge" expression
//            |event_expression "or" event_expresison
//            |event_expression ',' event_expression
// or などのパーズをうまくやるために "or" と ',' を含まない
// event_primary という非終端ノードを定義する．
// そもそも expression のなかに hierarchical_identifier は含まれてるでしょ．
event_expression
: event_primary
{
  $$ = parser.new_list<PtExpr>();
  $$->push_back($1);
}
| event_expression OR event_primary
{
  $$ = $1;
  $$->push_back($3);
}
| event_expression ',' event_primary
{
  $$ = $1;
  $$->push_back($3);
}
;

event_primary
: expression
{
  $$ = $1;
}
| POSEDGE expression
{
  $$ = parser.new_Opr(@$, kVpiPosedgeOp, $2, NULL);
}
| NEGEDGE expression
{
  $$ = parser.new_Opr(@$, kVpiNegedgeOp, $2, NULL);
}
;

// [SPEC] procedural_timing_control_statement ::=
//             delay_or_event_control statement_or_null
// [SPEC] delay_or_event_control ::=
//             delay_control
//            |event_control
//            |"repeat" '(' expression ')' event_control
// この最後の形式を含めると loop_statement の repeat 文と区別ができなくなる．
// そもそも procedural_timing_control_statement は statement の一部であり
// loop_statement の repeat 文で
// repeat '(' expression ')' statement
// と書いたときに statement が procedural_timing_control_statement であったら
// repeat '(' expression ')' event_control statement となるので
// 最後の形式はもともと含まれているので不要である．
procedural_timing_control_statement
: ai_list delay_control statement_or_null
{
  $$ = parser.new_DcStmt(@$, $2, $3, $1);
}
| ai_list event_control statement_or_null
{
  $$ = parser.new_EcStmt(@$, $2, $3, $1);
}
;

// [SPEC] wait_statement ::=
//             "wait" '(' expression ')' statement_or_null
wait_statement
: ai_list WAIT '(' expression ')' statement_or_null
{
  $$ = parser.new_Wait(@$, $4, $6, $1);
}
;


//////////////////////////////////////////////////////////////////////
// A.6.6 Conditional statements
//////////////////////////////////////////////////////////////////////

// [SPEC] conditional_statement ::=
//             "if" '(' expression ')'
//                 statement_or_null [ "else" statement_or_null ]
//            |if_else_if_statement
// [SPEC] if_else_if_statement ::=
//             "if" '(' expresison ')' statement_or_null
//                 { "else if" '(' expresison ')' statement_or_null }
//                 [ "else" statement_or_null ]
// if も statement なので else if の記述は不要なのにね
conditional_statement
: ai_list IF '(' expression ')' statement_or_null %prec LOWER_THAN_ELSE
{
  $$ = parser.new_If(@$, $4, $6, $1);
}
| ai_list IF '(' expression ')' statement_or_null ELSE statement_or_null
{
  $$ = parser.new_If(@$, $4, $6, $8, $1);
}
;

// [SPEC] function_conditional_statement ::=
//             "if" '(' expression ')'
//                 function_statement_or_null
//                 [ "else" function_statement_or_null ]
//            |function_if_else_if_statement
// [SPEC] function_if_else_if_statement ::=
//             "if" '(' expresison ')' function_statement_or_null
//                 { "else if" '(' expresison ')' function_statement_or_null }
//                 [ "else" function_statement_or_null ]


//////////////////////////////////////////////////////////////////////
// A.6.7 Case statements
//////////////////////////////////////////////////////////////////////

// [SPEC] case_statement ::=
//             "case" '(' expression ')'
//                 case_item { case_item } "endcase"
//            |"casez" '(' expression ')'
//                 case_item { case_item } "endcase"
//            |"casex" '(' expression ')'
//                 case_item { case_item } "endcase"
case_statement
: ai_list CASE '(' expression ')'
  list_of_case_items
  ENDCASE
{
  $$ = parser.new_Case(@$, $4, $6, $1);
}
| ai_list CASEX '(' expression ')'
  list_of_case_items
  ENDCASE
{
  $$ = parser.new_CaseX(@$, $4, $6, $1);
}
| ai_list CASEZ '(' expression ')'
  list_of_case_items
  ENDCASE
{
  $$ = parser.new_CaseZ(@$, $4, $6, $1);
}
| error ENDCASE
{
  $$ = NULL;
  yyerrok;
}
;

// [SPEC*] list_of_case_items ::= case_item { case_item }
list_of_case_items
: case_item
{
  $$ = parser.new_list<PtCaseItem>();
  $$->push_back($1);
}
| list_of_case_items case_item
{
  $$ = $1;
  $$->push_back($2);
}
;

// [SPEC] case_item ::=
//             expresison {',' expression} ':' statement_or_null
//            |"default" [':'] statement_or_null
case_item
: nzlist_of_expressions ':' statement_or_null
{
  $$ = parser.new_CaseItem(@$, $1, $3);
}
| DEFAULT                   statement_or_null
{
  $$ = parser.new_CaseItem(@$, NULL, $2);
}
| DEFAULT               ':' statement_or_null
{
  $$ = parser.new_CaseItem(@$, NULL, $3);
}
;

// [SPEC] function_case_statement ::=
//             "case" '(' expression ')'
//                 function_case_item { function_case_item } "endcase"
//            |"casez" '(' expression ')'
//                 function_case_item { function_case_item } "endcase"
//            |"casex" '(' expression ')'
//                 function_case_item { function_case_item } "endcase"
// [SPEC] function_case_item ::=
//             expression {',' expression} ':' function_statement_or_null
//            |"default" [':'] function_statement_or_null


//////////////////////////////////////////////////////////////////////
// A.6.8 Looping statements
//////////////////////////////////////////////////////////////////////

// [SPEC] function_loop_statement ::=
//             "forever" function_statement
//            |"repeat" '(' expression ')' function_statement
//            |"while" '(' expression ')' function_statement
//            |"for '(' variable_assignment ';' expresson ';'
//                 variable_assignment ')' function_statement

// [SPEC] loop_statement ::=
//             "forever" statement
//            |"repeat" '(' expression ')' statement
//            |"while" '(' expression ')' statement
//            |"for '(' variable_assignment ';' expresson ';'
//                 variable_assignment ')' statement
loop_statement
: ai_list FOREVER statement
{
  $$ = parser.new_Forever(@$, $3, $1);
}
| ai_list REPEAT '(' expression ')' statement
{
  $$ = parser.new_Repeat(@$, $4, $6, $1);
}
| ai_list WHILE '(' expression ')' statement
{
  $$ = parser.new_While(@$, $4, $6, $1);
}
| ai_list FOR '(' variable_assignment ';' expression ';'
                  variable_assignment ')' statement
{
  $$ = parser.new_For(@$, $4, $6, $8, $10, $1);
}
;


//////////////////////////////////////////////////////////////////////
// A.6.9 Task enable statements
//////////////////////////////////////////////////////////////////////

// [SPEC] system_task_enable ::=
//             system_task_identifier ['(' expression {',' expresison} ')'] ';'
// どうも system function/system task 系は空の引数を許しているようなので
// nzlist_of_arguments というのを作る．
// リスト自体は空でないがが要素は空がありうる．
system_task_enable
: ai_list SYS_IDENTIFIER ';'
{
  $$ = parser.new_SysEnable(@$, $2, NULL, $1);
}
| ai_list SYS_IDENTIFIER '(' nzlist_of_arguments ')' ';'
{
  $$ = parser.new_SysEnable(@$, $2, $4, $1);
}
;

// [SPEC] task_enable ::=
//             hierarchical_task_identifier
//                 ['(' expression {',' expresison} ')'] ';'
task_enable
: ai_list hierarchical_identifier ';'
{
  $$ = parser.new_Enable(@$, $2, NULL, $1);
}
| ai_list IDENTIFIER ';'
{
  $$ = parser.new_Enable(@$, $2, NULL, $1);
}
| ai_list hierarchical_identifier '(' nzlist_of_expressions ')' ';'
{
  $$ = parser.new_Enable(@$, $2, $4, $1);
}
| ai_list IDENTIFIER '(' nzlist_of_expressions ')' ';'
{
  $$ = parser.new_Enable(@$, $2, $4, $1);
}
;


//////////////////////////////////////////////////////////////////////
// A.7 Specify section
//////////////////////////////////////////////////////////////////////
// A.7.1 Specify block declaration
//////////////////////////////////////////////////////////////////////

// [SPEC] specify_block ::= "specify" { specify_item } "endspecify"

specify_block
: ai_list SPECIFY specify_items ENDSPECIFY
;

// [SPEC*] specify_items ::= { specify_item }
// [SPEC] specify_item ::=
//             specparam_declaration
//            |pulsestyle_declaration
//            |showcancelled_declaration
//            |path_declaration
//            |system_timing_check
// [SPEC] pulsestyle_declaration ::=
//             "pulsestyle_onevent" list_of_path_outputs ';'
//            |"pulsestyle_ondetect" list_of_path_outputs ';'
// [SPEC] showcancelled_declaration ::=
//             "showcancelled" list_of_path_outputs ';'
//            |"noshowcancelled" list_of_path_outputs ';'
// [SPEC] path_declaration ::=
//             simple_path_declaration ';'
//            |edge_sensitive_path_declaration ';'
//            |state_dependent_path_declaration ';'
// [SPEC] state_dependent_path_declaration ::=
//             "if" '(' module_path_expression ')' simple_path_declaration
//            |"if" '(' module_path_expression ')'
//                 edge_sensitive_path_declaration
//            |"ifnone" simple_path_declaration
// [SPEC] list_of_path_outputs ::=
//             specify_output_terminal_descriptor
//                 {',' specify_output_terminal_descriptor }
// 下の式に現れる path_declaration は実際には
// simple_path_declaration + edge_sensitive_path_declaration である．
// state_dependent_path_declaration は含まないので個別に規則を追加している．
specify_items
: // 空もありうる
| specify_items specify_item
;

specify_item
: specparam_declaration
| PULSESTYLE_ONEVENT  nzlist_of_terminals ';'
{
  parser.new_SpecItem(@$, kVpiPulsestyleOnEvent, $2);
}
| PULSESTYLE_ONDETECT nzlist_of_terminals ';'
{
  parser.new_SpecItem(@$, kVpiPulsestyleOnDetect, $2);
}
| SHOWCANCELLED   nzlist_of_terminals ';'
{
  parser.new_SpecItem(@$, kVpiShowcancelled, $2);
}
| NOSHOWCANCELLED nzlist_of_terminals ';'
{
  parser.new_SpecItem(@$, kVpiNoshowcancelled, $2);
}
| path_declaration ';'
{
  parser.new_SpecPath(@$, kVpiSpecPathNull, NULL, $1);
}
| IF '(' module_path_expression ')' path_declaration ';'
{
  parser.new_SpecPath(@$, kVpiSpecPathIf, $3, $5);
}
| IFNONE simple_path_declaration ';'
{
  parser.new_SpecPath(@$, kVpiSpecPathIfnone, NULL, $2);
}
| system_timing_check
{
  // system_timing_check はまともにパーズしていない．
}
| error ';'
{
  yyerrok;
}
;


//////////////////////////////////////////////////////////////////////
// A.7.2 Specify path declarations
//////////////////////////////////////////////////////////////////////

// [SPEC] path_declaration ::=
//             simple_path_declaration ';'
//            |edge_sensitive_path_declaration ';'
//            |state_dependent_path_declaration ';'
// [SPEC] simple_path_declaration ::=
//             parallel_path_description '=' path_delay_value
//            |full_path_description '=' path_delay_value
// [SPEC] parallel_path_description ::=
//             '(' specify_input_terminal_descriptor [ polarity_operator ]
//                 '=>' specify_output_terminal_descriptor ')'
// [SPEC] full_path_description ::=
//             '(' list_of_path_inputs [polarity_operator ]
//                 '*>' list_of_path_outputs ')'
// [SPEC] list_of_path_inputs ::=
//             specify_input_terminal_descriptor
//                 {',' specify_input_terminal_descriptor }
// [SPEC] list_of_path_outputs ::=
//             specify_output_terminal_descriptor
//                 {',' specify_output_terminal_descriptor }
// [SPEC] polarity_operator ::= '+'|'-'
// path_declaration は似て非なる構文がたくさんあるので面倒くさい．
// simple_path_declaration と edge_sensitive_path_declaration は
// 頭が共通で後ろに ':' expression がつくかどうかだけで判断する場合が
// あるので異なる非終端ノードにして還元することができない．
// また，本当は parallel_path_description の最初は単一の terminal だが
// 要素数が1の list_of_path_inputs(nzlist_of_terminals)と区別がつかず,
// コンフリクトを起してしまうのでとりあえず nzlist_of_terminals でパーズ
// しておいて複数の terminal を含んでいたらエラーとする．
// セミコロンは上で処理している．
path_declaration
: simple_path_declaration
{
  $$ = $1;
}
| edge_sensitive_path_declaration
{
  $$ = $1;
}
;

simple_path_declaration
: '(' nzlist_of_terminals        EQGT   specify_terminal ')'
      '=' path_delay_value
{
  $$ = parser.new_PathDecl(@$, 0,
			   $2, 0,
			   EQGT,
			   $4, 0,
			   NULL, $7);
}
| '(' nzlist_of_terminals pol_op EQGT   specify_terminal ')'
      '=' path_delay_value
{
  $$ = parser.new_PathDecl(@$, 0,
			   $2, $3,
			   EQGT,
			   $5, 0,
			   NULL, $8);
}
| '(' nzlist_of_terminals        STARGT nzlist_of_terminals ')'
      '=' path_delay_value
{
  $$ = parser.new_PathDecl(@$, 0,
			   $2, 0,
			   STARGT,
			   $4, 0,
			   NULL, $7);
}
| '(' nzlist_of_terminals pol_op STARGT nzlist_of_terminals ')'
      '=' path_delay_value
{
  $$ = parser.new_PathDecl(@$, 0,
			   $2, $3,
			   STARGT,
			   $5, 0,
			   NULL, $8);
}
;

pol_op
: '+'
{
  $$ = '+';
}
| '-'
{
  $$ = '-';
}
;

//////////////////////////////////////////////////////////////////////
// A.7.3 Specify block terminals
//////////////////////////////////////////////////////////////////////

// [SPEC] specify_input_terminal_descriptor ::=
//             input_identifier
//            |input_identifier '[' constant_expression ']'
//            |input_identifier '[' range_expression ']'
// [SPEC] specify_output_terminal_descriptor ::=
//             output_identifier
//            |output_identifier '[' constant_expression ']'
//            |output_identifier '[' range_expression ']'
// [SPEC] input_identifier ::=
//             input_port_identifier
//            |inout_port_identifier
// [SPEC] output_identifier ::=
//             output_port_identifier
//            |inout_port_identifier
// [SPEC] inout_port_identifier ::= identifier
// [SPEC] input_port_identifier ::= identifier
// [SPEC] output_port_identifier ::= identifier
// お定まりの無意味な構文規則．これを整理すると以下のようになる．
//
// [SPEC*] specify_terminal ::=
//              identifier
//             |identifier '[' constant_expression ']'
//             |identifier '[' range_expression ']'
// この3番目の形式の range_expression って constant_range_expression の
// 誤りでしょ．
specify_terminal
: IDENTIFIER
{
  $$ = parser.new_Primary(@1, $1);
}
| IDENTIFIER index
{
  $$ = parser.new_CPrimary(@$, $1, $2);
}
| IDENTIFIER '[' expression ':' expression ']'
{
  $$ = parser.new_CPrimary(@$, $1, kVpiConstRange, $3, $5);
}
| IDENTIFIER '[' expression PLUSCOLON expression ']'
{
  $$ = parser.new_CPrimary(@$, $1, kVpiPlusRange, $3, $5);
}
| IDENTIFIER '[' expression MINUSCOLON expression ']'
{
  $$ = parser.new_CPrimary(@$, $1, kVpiMinusRange, $3, $5);
}
;

// [SPEC*] nzlist_of_terminals ::=
//              specify_terminal {',' specify_terminal}
nzlist_of_terminals
: specify_terminal
{
  $$ = parser.new_list<PtExpr>();
  $$->push_back($1);
}
| nzlist_of_terminals ',' specify_terminal
{
  $$ = $1;
  $$->push_back($3);
}
;


//////////////////////////////////////////////////////////////////////
// A.7.4 Specify path delays
//////////////////////////////////////////////////////////////////////

// [SPEC] path_delay_value ::=
//             list_of_path_delay_expressions
//            |'(' list_of_path_delay_expressions ')'
// list_of_path_delay_expressions が実際には path_delay_expression 単体
// の場合には '(' list_of_path_delay_expressions ')' が重複するので
// カッコは複数のpath_delay_expression を持つときのみにする．
// そのため，list_of_path_delay_expressions から path_delay_expression
// 単体の場合を取り除き path_delay_value に直接加える．
path_delay_value
: path_delay_expression
{
  $$ = parser.new_PathDelay(@1, $1);
}
| list_of_path_delay_expressions
{
  $$ = $1;
}
| '(' list_of_path_delay_expressions ')'
{
  $$ = $2;
}
;

// [SPEC] list_of_path_delay_expressions ::=
//             path_delay_expression
//            |path_delay_expression ',' path_delay_expression
//            |path_delay_expression ',' path_delay_expression ','
//                 path_delay_expression
//            |path_delay_expression ',' path_delay_expression ','
//                 path_delay_expression ',' path_delay_expression ','
//                 path_delay_expression ',' path_delay_expression
//            |path_delay_expression ',' path_delay_expression ','
//                 path_delay_expression ',' path_delay_expression ','
//                 path_delay_expression ',' path_delay_expression
//                 path_delay_expression ',' path_delay_expression ','
//                 path_delay_expression ',' path_delay_expression ','
//                 path_delay_expression ',' path_delay_expression
list_of_path_delay_expressions
: path_delay_expression ',' path_delay_expression
{
  $$ = parser.new_PathDelay(@$, $1, $3);
}
| path_delay_expression ',' path_delay_expression ','
  path_delay_expression
{
  $$ = parser.new_PathDelay(@$, $1, $3, $5);
}
| path_delay_expression ',' path_delay_expression ','
  path_delay_expression ',' path_delay_expression ','
  path_delay_expression ',' path_delay_expression
{
  $$ = parser.new_PathDelay(@$, $1, $3, $5, $7, $9, $11);
}
| path_delay_expression ',' path_delay_expression ','
  path_delay_expression ',' path_delay_expression ','
  path_delay_expression ',' path_delay_expression ','
  path_delay_expression ',' path_delay_expression ','
  path_delay_expression ',' path_delay_expression ','
  path_delay_expression ',' path_delay_expression
{
  $$ = parser.new_PathDelay(@$, $1, $3, $5, $7, $9, $11, $13, $15,
			    $17, $19, $21, $23);
}
;

// [SPEC] path_delay_expression ::= constant_mintypmax_expression
// 前述のように constant_mintypmax_expression は constant_expression
// を含まないので明示的に加える必要がある．
path_delay_expression
: expression
{
  $$ = $1;
}
| mintypmax_expression
{
  $$ = $1;
}
;

// [SPEC] edge_sensitive_path_declaration ::=
//             parallel_edge_sensitive_path_description '=' path_delay_value
//            |full_edge_sensitive_path_description '=' path_delay_value
// [SPEC] parallel_edge_sensitive_path_description ::=
//             '(' [ edge_identifier ] specify_input_terminal_descriptor
//                 '=>' specify_output_terminal_descriptor
//                 [ polarity_operator ] ':' data_source_expression ')'
// [SPEC] full_edge_sensitive_path_description ::=
//             '(' [ edge_identifier ] list_of_path_inputs "*>"
//                 list_of_path_outputs [ polarity_operator ] ':'
//                 data_source_expression ')'
// [SPEC] data_source_expression ::= expression
// [SPEC] edige_identifier ::= "posedge"|"negedge"
// [SPEC] polarity_operator ::= '+'|'-'
// 仰天！ 仕様が違う．
// 実際には specify_output_terminal_descript [polarity_identifier] ':'
//  data_source_expression を囲む '(' ')' が必要みたい．
// Verilog-XL もそうなっている．これなら reduce/reduce conflict も起きない
// ような気がする．
// parallel_edge_sensitive_path_description は実際にはリストではなく
// edge_sensitive_path_declaration で edge_identifier が省略された場合には
// simple_path_declaration と同じになるので省略はなし．
// 要素数は常に1だがパーズの都合上 nzlist_of_terminals にしている．
// あとで要素数が1であることを確認する必要がある．
//
// 2010.08.21追記
// parallel_edge_sensitive_path_description や
// full_edge_sensitive_path_description で '+'|'-'
// の直後に':'が来る場合がある．
// ところが連続した　"+:" や "-:" はそれぞれ PLUSCOLON，MINUSCOLON
// というトークンになってしまう．
// そのため，PLUSCOLON, MINUSCOLON を含んだ構文を追加する．
// 具体的には pol_colon という非終端ノードを追加している．
edge_sensitive_path_declaration
: '('      nzlist_of_terminals EQGT
    '(' specify_terminal           expression ')' ')' '=' path_delay_value
{ // parallel_edge_sensitive_path_description
  $$ = parser.new_PathDecl(@$, 0,
			   $2, 0,
			   kVpiPathParallel,
			   $5, 0,
			   $6, $10);
}
| '('      nzlist_of_terminals EQGT
    '(' specify_terminal pol_colon expression ')' ')' '=' path_delay_value
{ // parallel_edge_sensitive_path_description
  $$ = parser.new_PathDecl(@$, 0,
			   $2, 0,
			   kVpiPathParallel,
			   $5, $6,
			   $7, $11);
}
| '(' edge nzlist_of_terminals EQGT
    '(' specify_terminal          expression ')' ')' '=' path_delay_value
{ // parallel_edge_sensitive_path_description
  $$ = parser.new_PathDecl(@$, $2,
			   $3, 0,
			   kVpiPathParallel,
			   $6, 0,
			   $7, $11);
}
| '(' edge nzlist_of_terminals EQGT
    '(' specify_terminal pol_colon expression ')' ')' '=' path_delay_value
{ // parallel_edge_sensitive_path_description
  $$ = parser.new_PathDecl(@$, $2,
			   $3, 0,
			   kVpiPathParallel,
			   $6, $7,
			   $8, $12);
}
| '('      nzlist_of_terminals STARGT
     '(' nzlist_of_terminals           expression ')' ')' '=' path_delay_value
{ // full_edge_sensitive_path_description
  $$ = parser.new_PathDecl(@$, 0,
			   $2, 0,
			   kVpiPathFull,
			   $5, 0,
			   $6, $10);
}
| '('      nzlist_of_terminals STARGT
     '(' nzlist_of_terminals pol_colon expression ')' ')' '=' path_delay_value
{ // full_edge_sensitive_path_description
  $$ = parser.new_PathDecl(@$, 0,
			   $2, 0,
			   kVpiPathFull,
			   $5, $6,
			   $7, $11);
}
| '(' edge nzlist_of_terminals STARGT
    '(' nzlist_of_terminals           expression ')' ')' '=' path_delay_value
{ // full_edge_sensitive_path_description
  $$ = parser.new_PathDecl(@$, $2,
			   $3, 0,
			   kVpiPathFull,
			   $6, 0,
			   $7, $11);
}
| '(' edge nzlist_of_terminals STARGT
    '(' nzlist_of_terminals pol_colon expression ')' ')' '=' path_delay_value
{ // full_edge_sensitive_path_description
  $$ = parser.new_PathDecl(@$, $2,
			   $3, 0,
			   kVpiPathFull,
			   $6, $7,
			   $8, $12);
}
;

pol_colon
: '+' ':'
{
  $$ = '+';
}
| '-' ':'
{
  $$ = '-';
}
| PLUSCOLON
{
  // + と : がくっついていた
  $$ = '+';
}
| MINUSCOLON
{
  // - と : がくっついていた
  $$ = '-';
}
;

edge
: POSEDGE
{
  $$ = kVpiPosedgeOp;
}
| NEGEDGE
{
  $$ = kVpiNegedgeOp;
}
;


//////////////////////////////////////////////////////////////////////
// A.7.5.1 System timing check commands
//////////////////////////////////////////////////////////////////////

system_timing_check
: SETUP '(' timing_check_event ',' timing_check_event ',' expression
            opt_notify_reg ')' ';'
{
  $$ = 0; // ダミー
}
| HOLD '(' timing_check_event ',' timing_check_event ',' expression
           opt_notify_reg ')' ';'
{
  $$ = 0; // ダミー
}
| SETUPHOLD '(' timing_check_event ',' timing_check_event ','
  expression ',' expression timing_check_options ')' ';'
{
  $$ = 0; // ダミー
}
| RECOVERY '(' timing_check_event ',' timing_check_event ',' expression
               opt_notify_reg ')' ';'
{
  $$ = 0; // ダミー
}
| REMOVAL '(' timing_check_event ',' timing_check_event ',' expression
              opt_notify_reg ')' ';'
{
  $$ = 0; // ダミー
}
| RECREM '(' timing_check_event ',' timing_check_event ','
             expression ',' expression timing_check_options ')' ';'
{
  $$ = 0; // ダミー
}
| SKEW '(' timing_check_event ',' timing_check_event ',' expression
           opt_notify_reg ')' ';'
{
  $$ = 0; // ダミー
}
| TIMESKEW '(' timing_check_event ',' timing_check_event ','
               expression skew_timing_check_options ')' ';'
{
  $$ = 0; // ダミー
}
| FULLSKEW '(' timing_check_event ',' timing_check_event ','
               expression ',' expression skew_timing_check_options ')' ';'
{
  $$ = 0; // ダミー
}
| PERIOD '(' controlled_timing_check_event ',' expression
             opt_notify_reg ')' ';'
{
  $$ = 0; // ダミー
}
| WIDTH '(' controlled_timing_check_event ',' expression ','
            expression opt_notify_reg ')' ';'
{
  $$ = 0; // ダミー
}
| NOCHANGE '(' timing_check_event ',' timing_check_event ','
               mintypmax_expression ','
               mintypmax_expression opt_notify_reg ')' ';'
{
  $$ = 0; // ダミー
}
;

timing_check_options
: // 空
{
  $$ = 0; // ダミー
  @$ = FileRegion();
}
| ',' IDENTIFIER
{
  $$ = 0; // ダミー
}
| ',' IDENTIFIER ',' mintypmax_expression
{
  $$ = 0; // ダミー
}
| ',' IDENTIFIER ',' mintypmax_expression ',' mintypmax_expression
{
  $$ = 0; // ダミー
}
| ',' IDENTIFIER ',' mintypmax_expression ',' mintypmax_expression
  ',' delayed_data
{
  $$ = 0; // ダミー
}
| ',' IDENTIFIER ',' mintypmax_expression ',' mintypmax_expression
  ',' delayed_data ',' delayed_data
{
  $$ = 0; // ダミー
}
;

skew_timing_check_options
: // 空
{
  $$ = 0; // ダミー
  @$ = FileRegion();
}
| ',' IDENTIFIER
{
  $$ = 0; // ダミー
}
| ',' IDENTIFIER ',' expression
{
  $$ = 0; // ダミー
}
| ',' IDENTIFIER ',' expression ',' mintypmax_expression
{
  $$ = 0; // ダミー
}
;

opt_notify_reg
: // 空
{
  $$ = 0; // ダミー
  @$ = FileRegion();
}
| ',' IDENTIFIER
{
  $$ = 0; // ダミー
}
;


//////////////////////////////////////////////////////////////////////
// A.7.5.2 System timing check command arguments
//////////////////////////////////////////////////////////////////////

// [SPEC] delayed_data ::=
//             terminal_identifier
//            |terminal_identifier [ constant_mintypmax_expression ]
delayed_data
: IDENTIFIER
{
  $$ = 0; // ダミー
}
| IDENTIFIER '[' mintypmax_expression ']'
{
  $$ = 0; // ダミー
}
;
// CHECK_CONST($3)


//////////////////////////////////////////////////////////////////////
// A.7.5.3 System timing check event definitions
//////////////////////////////////////////////////////////////////////

timing_check_event
: opt_timing_check_event_control specify_terminal
  opt_timing_check_condition
{
  $$ = 0; // ダミー
}
;

controlled_timing_check_event
: timing_check_event_control specify_terminal
  opt_timing_check_condition
{
  $$ = 0; // ダミー
}
;

opt_timing_check_event_control
: // 空
{
  $$ = 0; // ダミー
  @$ = FileRegion();
}
| timing_check_event_control
{
  $$ = 0; // ダミー
}
;

timing_check_event_control
: POSEDGE
{
  $$ = 0; // ダミー
}
| NEGEDGE
{
  $$ = 0; // ダミー
}
| EDGE '[' edge_descriptor ']'
{
  $$ = 0; // ダミー
}
| EDGE '[' edge_descriptor ',' edge_descriptor ']'
{
  $$ = 0; // ダミー
}
;

edge_descriptor
: UNUMBER // 01, 10, z0, Z0, z1, Z1, x0, X0, x1, X1, 0z, 0Z, 1z, 1Z,
          // 0x, 0X, 1x, 1X のみが有効
{
  $$ = 0; // ダミー
}
;

opt_timing_check_condition
: // 空
{
  $$ = 0; // ダミー
  @$ = FileRegion();
}
| timing_check_condition
{
  $$ = 0; // ダミー
}
;

timing_check_condition
: expression
{
  $$ = 0; // ダミー
}
;

/*
timing_check_condition
: scalar_timing_check_condition
| '(' scalar_timing_check_condition ')'
;

scalar_timing_check_condition
: expression
| '~' expression
| expression EQEQ scalar_constant
| expression EQEQEQ scalar_constant
| expression NOTEQ scalar_constant
| expression NOTEQEQ scalar_constant
;

scalar_constant
: UNUMBER // 1ビットの01のみ
;
*/


//////////////////////////////////////////////////////////////////////
// A.8 Expressions
//////////////////////////////////////////////////////////////////////
// A.8.1 Concatenations
//////////////////////////////////////////////////////////////////////

// [SPEC] concatenation ::= '{' expression {',' expression } '}'
concatenation
: '{' nzlist_of_expressions '}'
{
  $$ = parser.new_Concat(@$, $2);
}
;

// [SPEC] constant_concatenation ::=
//             '{' constant_expression {',' constant_expression } '}'

// [SPEC] constant_multiple_concatenation ::=
//             '{' constant_expression constant_concatenation '}'

// [SPEC*] nzlist_of_constant_expressions ::=
//              constant_expression {',' constant_expression }

// [SPEC] module_path_concatenation ::=
//             '{' module_path_expression {',' module_path_expression } '}'
// [SPEC] module_path_multiple_concatenation ::=
//             '{' constant_expression module_path_concatenation '}'

// [SPEC] multiple_concatenation ::=
//             '{' constante_expression concatenation '}'
// 本当は $2 は constant_expression だが
// そうすると大量の reduce/reduce conflict が起きる
// 構文解析では expression と甘くしておいて
// あとで constant_expression かどうかチェックするしかない
multiple_concatenation
: '{' expression '{' nzlist_of_expressions '}' '}'
{
  $$ = parser.new_MultiConcat(@$, $2, $4);
}
;

// [SPEC*] nzlist_of_expression ::=
//              expression {',' expression }
nzlist_of_expressions
: expression
{
  $$ = parser.new_list<PtExpr>();
  $$->push_back($1);
}
| nzlist_of_expressions ',' expression
{
  $$ = $1;
  $$->push_back($3);
}
;

// [SPEC] net_concatenation ::=
//             '{' net_concatenation_value {',' net_concatenation_value } '}'
// [SPEC] net_concatenation_value ::=
//             hierarchical_net_identifier
//            |hierarchical_net_identifier
//                 '[' expression ']' {'[' expression ']'}
//            |hierarchical_net_identifier
//                 '[' expression ']' {'[' expression ']'} [ range_expression ]
//            |hierarchical_net_identifier [ range_expression ]
//            |net_concatenation
// オリジナルでは net_concatenation_value という非終端記号を定義しているが
// これは net_lvalue と同一のものであるので置き換えた
// [SPEC] variable_concatenation ::=
//    '{' variable_concatenation_value {',' variable_concatenation_value } '}'
// [SPEC] variable_concatenation_value ::=
//    hierarchical_variable_identifier
//   |hierarchical_variable_identifier '[' expression ']' {'[' expression ']'}
//   |hierarchical_variable_identifier '[' expression ']' {'[' expression ']'}
//      [ range_expression ]
//   |hierarchical_variable_identifier [ range_expression ]
//   |variable_concatenation
// オリジナルでは variable_concatenation_value という非終端記号を定義して
// いるがこれは variable_lvalue と同一のものであるので置き換えた
// さらに net_concatenation と variable_concatenation は区別がほとんどつか
// ないので一つにした．
// しかし良く考えてみるとこの net_concatenation_value の中の expression
// は constant_expression の間違いだよね．どっちにしろ同じだけど．


//////////////////////////////////////////////////////////////////////
// A.8.2 Function calls
//////////////////////////////////////////////////////////////////////

// [SPEC] constant_function_call ::=
//             function_identifier { attribute_instance }
//                 '(' constant_expression {',' constant_expression} ')'
// [SPEC] function_identifier ::= identifier

// [SPEC] function_call ::=
//             hierarchical_function_identifier { attribute_instance }
//                '(' expression {',' expression} ')'
// [SPEC] hierarchical_function_identifier ::= hierarchical_identifier
// 仕様によれば空の引数はないはずだが iverilog のテストスートに存在する．
// でもこれは iverilog がバカなのか？ <<== そうみたい．
// Verilog-XL でもシンタックスエラーになるので無視
function_call
: hierarchical_identifier ai_list '(' nzlist_of_expressions ')'
{
  $$ = parser.new_FuncCall(@$, $1, $4, $2);
}
| IDENTIFIER ai_list '(' nzlist_of_expressions ')'
{
  $$ = parser.new_FuncCall(@$, $1, $4, $2);
}
;

// [SPEC] genvar_function_call ::=
//             genvar_function_identifier { attribute_instance }
//                  '(' constant_expression {',' constant_expression} ')'

// [SPEC] system_function_call ::=
//             system_function_identifier
//                 ['(' expression {',' expression } ')']
// [SPEC] system_function_identifier ::= $[a-zA-Z0-9_$]{[a-zA-Z0-9_$]}
// どうも空の引数を許しているようなので nzlist_of_arguments というのを作る．
system_function_call
: SYS_IDENTIFIER
{
  $$ = parser.new_SysFuncCall(@$, $1, NULL);
}
| SYS_IDENTIFIER '(' nzlist_of_arguments ')'
{
  $$ = parser.new_SysFuncCall(@$, $1, $3);
}
;

// [SPEC*] nzlist_of_arguments ::= argument {',' argument }
nzlist_of_arguments
: argument
{
  $$ = parser.new_list<PtExpr>();
  $$->push_back($1);
}
| nzlist_of_arguments ',' argument
{
  $$ = $1;
  $$->push_back($3);
}
;

// [SPEC*] argument ::= /* 空 */ | expression
argument
: /* 空 */
{
  $$ = NULL;
  @$ = FileRegion();
}
| expression
{
  $$ = $1;
}
;


//////////////////////////////////////////////////////////////////////
// A.8.3 Expressions
//////////////////////////////////////////////////////////////////////

// expression 関係は元の構文のままではとても使えないので大幅に変えている．
// そのためオリジナルの構文規則をまとめて書いておく．
// [SPEC] base_expression ::= expression
// [SPEC] conditional_expression ::= expression1 '?' { attribute_instance }
//                 expression2 ':' expression3
// アホか！ なんで expression1, expression2, expression3 が必要なわけ？
// [SPEC] constant_base_expression ::= constant_expression
// [SPEC] constant_expression ::=
//             constant_primary
//            |unary_operator { attribute_instance } constant_primay
//            |constant_expression '?' { attribute_instance }
//                 constant_expression ':' constant_expression
//            |string
// [SPEC] constant_mintypmax_expression ::=
//             constant_expression
//            |constant_expression ':' constant_expression ':'
//                 constant_expression
// [SPEC] constant_range_expression ::=
//             constant_expression
//            |msb_constant_expression ':' lsb_constant_expression
//            |constant_base_expression "+:" width_constant_expression
//            |constant_base_expression "-:" width_constant_expression
// [SPEC] dimension_constant_expression ::= constant_expression
// [SPEC] expression1 ::= expression
// [SPEC] expression2 ::= expression
// [SPEC] expression3 ::= expression
// [SPEC] expression ::=
//             primay
//            |unary_operator { attribute_instance } primary
//            |expression binary_operator { attribute_instance } expression
//            |conditional_expression
//            |string
// [SPEC] lsb_constant_expression ::= constant_expression
// [SPEC] mintypmax_expression ::=
//             expression
//            |expression ':' expression ':' expression
// [SPEC] module_path_conditional_expression ::=
//             module_path_expression '?' { attribute_instance }
//                 module_path_expression ':' module_path_expression
// [SPEC] module_path_expression ::=
//             module_path_primary
//            |unary_module_path_operator { attribute_instance }
//                 module_path_parimary
//            |module_path_expression binary_module_path_operator
//                 module_path_expression
//            |module_path_conditional_expression
// [SPEC] module_path_mintypmax_expression ::=
//             module_path_expression
//            |module_path_expression ':' module_path_expression
//             module_path_expression
// [SPEC] msb_constant_expression ::= constant_expression
// [SPEC] range_expression ::=
//             expression
//            |msb_constant_expression ':' lsb_constant_expression
//            |base_expression "+:" width_constant_expression
//            |base_expression "-:" width_constant_expression
// [SPEC] width_constant_expression ::= constant_expression

// 要するに conditional_expression の第1項めと第2項めには expression
// が書けない．なぜかというとそこに conditional_expression があると
// どこに還元して良いのか分からなくなるので．
// 書きたければ '(' ')' でくくれば primary と見なされるので正しい文法となる．
expression
: expr1 %prec ANDAND
{
  $$ = $1;
}
| expr1 '?' ai_list expr1 ':' expression %prec COND
{
  $$ = parser.new_Opr(@$, kVpiConditionOp, $1, $4, $6, $3);
}
;

expr1
: primary
{
  $$ = $1;
}
| '+' ai_list primary %prec UOP
{
  $$ = parser.new_Opr(@$, kVpiPlusOp, $3, $2);
}
| '-' ai_list primary %prec UOP
{
  $$ = parser.new_Opr(@$, kVpiMinusOp, $3, $2);
}
| '!' ai_list primary %prec UOP
{
  $$ = parser.new_Opr(@$, kVpiNotOp, $3, $2);
}
| '~' ai_list primary %prec UOP
{
  $$ = parser.new_Opr(@$, kVpiBitNegOp, $3, $2);
}
| '&' ai_list primary %prec UOP
{
  $$ = parser.new_Opr(@$, kVpiUnaryAndOp, $3, $2);
}
| '|' ai_list primary %prec UOP
{
  $$ = parser.new_Opr(@$, kVpiUnaryOrOp, $3, $2);
}
| '^' ai_list primary %prec UOP
{
  $$ = parser.new_Opr(@$, kVpiUnaryXorOp, $3, $2);
}
| TILDEAND ai_list primary %prec UOP
{
  $$ = parser.new_Opr(@$, kVpiUnaryNandOp, $3, $2);
}
| TILDEOR ai_list primary %prec UOP
{
  $$ = parser.new_Opr(@$, kVpiUnaryNorOp, $3, $2);
}
| TILDEXOR ai_list primary %prec UOP
{
  $$ = parser.new_Opr(@$, kVpiUnaryXNorOp, $3, $2);
}
| expr1 STARSTAR ai_list expr1
{
  $$ = parser.new_Opr(@$, kVpiPowerOp, $1, $4, $3);
}
| expr1 '+' ai_list expr1
{
  $$ = parser.new_Opr(@$, kVpiAddOp, $1, $4, $3);
}
| expr1 '-' ai_list expr1
{
  $$ = parser.new_Opr(@$, kVpiSubOp, $1, $4, $3);
}
| expr1 '*' ai_list expr1
{
  $$ = parser.new_Opr(@$, kVpiMultOp, $1, $4, $3);
}
| expr1 '/' ai_list expr1
{
  $$ = parser.new_Opr(@$, kVpiDivOp, $1, $4, $3);
}
| expr1 '%' ai_list expr1
{
  $$ = parser.new_Opr(@$, kVpiModOp, $1, $4, $3);
}
| expr1 EQEQ ai_list expr1
{
  $$ = parser.new_Opr(@$, kVpiEqOp, $1, $4, $3);
}
| expr1 NOTEQ ai_list expr1
{
  $$ = parser.new_Opr(@$, kVpiNeqOp, $1, $4, $3);
}
| expr1 EQEQEQ ai_list expr1
{
  $$ = parser.new_Opr(@$, kVpiCaseEqOp, $1, $4, $3);
}
| expr1 NOTEQEQ ai_list expr1
{
  $$ = parser.new_Opr(@$, kVpiCaseNeqOp, $1, $4, $3);
}
| expr1 ANDAND ai_list expr1
{
  $$ = parser.new_Opr(@$, kVpiLogAndOp, $1, $4, $3);
}
| expr1 OROR ai_list expr1
{
  $$ = parser.new_Opr(@$, kVpiLogOrOp, $1, $4, $3);
}
| expr1 '<' ai_list expr1
{
  $$ = parser.new_Opr(@$, kVpiLtOp, $1, $4, $3);
}
| expr1 '>' ai_list expr1
{
  $$ = parser.new_Opr(@$, kVpiGtOp, $1, $4, $3);
}
| expr1 LTEQ ai_list expr1
{
  $$ = parser.new_Opr(@$, kVpiLeOp, $1, $4, $3);
}
| expr1 GTEQ ai_list expr1
{
  $$ = parser.new_Opr(@$, kVpiGeOp, $1, $4, $3);
}
| expr1 '&' ai_list expr1
{
  $$ = parser.new_Opr(@$, kVpiBitAndOp, $1, $4, $3);
}
| expr1 '|' ai_list expr1
{
  $$ = parser.new_Opr(@$, kVpiBitOrOp, $1, $4, $3);
}
| expr1 '^' ai_list expr1
{
  $$ = parser.new_Opr(@$, kVpiBitXorOp, $1, $4, $3);
}
| expr1 TILDEXOR ai_list expr1
{
  $$ = parser.new_Opr(@$, kVpiBitXNorOp, $1, $4, $3);
}
| expr1 LTLT ai_list expr1
{
  $$ = parser.new_Opr(@$, kVpiLShiftOp, $1, $4, $3);
}
| expr1 GTGT ai_list expr1
{
  $$ = parser.new_Opr(@$, kVpiRShiftOp, $1, $4, $3);
}
| expr1 LTLTLT ai_list expr1
{
  $$ = parser.new_Opr(@$, kVpiArithLShiftOp, $1, $4, $3);
}
| expr1 GTGTGT ai_list expr1
{
  $$ = parser.new_Opr(@$, kVpiArithRShiftOp, $1, $4, $3);
}
| STRING
{
  $$ = parser.new_StringConst(@$, $1);
}
;

module_path_expression
: expression
{
  $$ = $1;
}
;

// [SPEC] constant_mintypmax_expression ::=
//             constant_expression
//            |constant_expression ':' constant_expression ':'
//                 constant_expression
// この最初の形式を認めてしまうと通常の constant_expression にカッコを
// つけたものが constant_mintypmax_expression になってしまうので認めない．
// そのかわり constant_mintypmax_expression を用いている箇所で
// constant_expression も認めるようにする．

// [SPEC] mintypmax_expression ::=
//             expression
//            |expression ':' expression ':' expression
// この最初の形式を認めてしまうと通常の expression にカッコをつけたものが
// mintypmax_expression になってしまうので認めない．
// そのかわり mintypmax_expression を用いている箇所で
// expression も認めるようにする．
mintypmax_expression
:  expression ':' expression ':' expression
{
  $$ = parser.new_MinTypMax(@$, $1, $3, $5);
}
;

// [SPEC] range_expression ::=
//             expression
//            |msb_constant_expression ':' lsb_constant_expression
//            |base_expression "+:" width_constant_expression
//            |base_expression "-:" width_constant_expression

// [SPEC] constant_range_expression ::=
//             constant_expression
//            |msb_constant_expression ':' lsb_constant_expression
//            |constant_base_expression "+:" width_constant_expression
//            |constant_base_expression "-:" width_constant_expression

// IEEE1364-2001 には expression も含まれるが
// index で '[' expression ']' を扱っており,
// それ以外では specify_terminal に
// '[' range_expression ']' が現れるだけなので
// 個別に対応して range_expression には expression を含めない．
// 本当は $1 も constant expression だが大量の reduce/reduce conflict
// が起きるので expression にしている．
// あとで constant_expression かどうかのチェックを行う．
// 先頭と末尾に '[' と ']' を追加している．


//////////////////////////////////////////////////////////////////////
// A.8.4 Primaries
//////////////////////////////////////////////////////////////////////

// [SPEC] constant_primary ::=
//             constant_concatenation
//            |constant_function_call
//            |'(' constant_mintypmax_expression ')'
//            |constant_multiple_concatenation
//            |genvar_identifier
//            |number
//            |parameter_identifier
//            |specparam_identifier
// [SPEC] parameter_identifier ::= identifier
// [SPEC] specparam_identifier ::= identifier
// genvar_identifier, parameter_identifier, specparam_identifier
// は全部 identifier なので一つにまとめる．

// [SPEC] primary ::=
//             number
//            |hierarchical_identifier
//            |hierarchical_identifier '[' expression ']' {'[' expression ']'}
//            |hierarchical_identifier '[' expression ']' {'[' expression ']'}
//                 '[' range_expression ']'
//            |hierarchical_identifier '[' range_expression ']'
//            |concatenation
//            |multiple_concatenation
//            |function_call
//            |system_function_call
//            |constant_function_call
//            |'(' mintypmax_expression ')'
// constant_function_call は function_call に含まれるので省略する．
// mintypmax_expression から expression は取り除いてあるのでここで明示的に
// 追加する．
primary
: number
{
  $$ = $1;
}
| hierarchical_identifier
{
  $$ = parser.new_Primary(@1, $1);
}
| IDENTIFIER
{
  $$ = parser.new_Primary(@1, $1);
}
| hierarchical_identifier nzlist_of_index
{
  $$ = parser.new_Primary(@$, $1, $2);
}
| IDENTIFIER nzlist_of_index
{
  $$ = parser.new_Primary(@$, $1, $2);
}
| hierarchical_identifier                 '[' expression ':' expression ']'
{
  $$ = parser.new_Primary(@$, $1, kVpiConstRange, $3, $5);
}
| IDENTIFIER                 '[' expression ':' expression ']'
{
  $$ = parser.new_Primary(@$, $1, kVpiConstRange, $3, $5);
}
| hierarchical_identifier                 '[' expression PLUSCOLON expression ']'
{
  $$ = parser.new_Primary(@$, $1, kVpiPlusRange, $3, $5);
}
| IDENTIFIER                 '[' expression PLUSCOLON expression ']'
{
  $$ = parser.new_Primary(@$, $1, kVpiPlusRange, $3, $5);
}
| hierarchical_identifier                 '[' expression MINUSCOLON expression ']'
{
  $$ = parser.new_Primary(@$, $1, kVpiMinusRange, $3, $5);
}
| IDENTIFIER                 '[' expression MINUSCOLON expression ']'
{
  $$ = parser.new_Primary(@$, $1, kVpiMinusRange, $3, $5);
}
| hierarchical_identifier nzlist_of_index '[' expression ':' expression ']'
{
  $$ = parser.new_Primary(@$, $1, $2, kVpiConstRange, $4, $6);
}
| IDENTIFIER nzlist_of_index '[' expression ':' expression ']'
{
  $$ = parser.new_Primary(@$, $1, $2, kVpiConstRange, $4, $6);
}
| hierarchical_identifier nzlist_of_index '[' expression PLUSCOLON expression ']'
{
  $$ = parser.new_Primary(@$, $1, $2, kVpiPlusRange, $4, $6);
}
| IDENTIFIER nzlist_of_index '[' expression PLUSCOLON expression ']'
{
  $$ = parser.new_Primary(@$, $1, $2, kVpiPlusRange, $4, $6);
}
| hierarchical_identifier nzlist_of_index '[' expression MINUSCOLON expression ']'
{
  $$ = parser.new_Primary(@$, $1, $2, kVpiMinusRange, $4, $6);
}
| IDENTIFIER nzlist_of_index '[' expression MINUSCOLON expression ']'
{
  $$ = parser.new_Primary(@$, $1, $2, kVpiMinusRange, $4, $6);
}
| concatenation
{
  $$ = $1;
}
| multiple_concatenation
{
  $$ = $1;
}
| function_call
{
  $$ = $1;
}
| system_function_call
{
  $$ = $1;
}
| '(' expression ')'
{
  // 括弧の位置を保持するためのトリック
  $$ = parser.new_Opr(@$, kVpiNullOp, $2, NULL);
}
| '(' mintypmax_expression ')'
{
  // 括弧の位置を保持するためのトリック
  $$ = parser.new_Opr(@$, kVpiNullOp, $2, NULL);
}
;


//////////////////////////////////////////////////////////////////////
// A.8.5 Expression left-side values
//////////////////////////////////////////////////////////////////////

// [SPEC] net_concatenation ::=
//             '{' net_concatenation_value {',' net_concatenation_value } '}'
// [SPEC] net_concatenation_value ::=
//             hierarchical_net_identifier
//            |hierarchical_net_identifier
//                 '[' expression ']' {'[' expression ']'}
//            |hierarchical_net_identifier
//                 '[' expression ']' {'[' expression ']'} [ range_expression ]
//            |hierarchical_net_identifier [ range_expression ]
//            |net_concatenation
// オリジナルでは net_concatenation_value という非終端記号を定義しているが
// これは net_lvalue と同一のものであるので置き換えた
// [SPEC] variable_concatenation ::=
//    '{' variable_concatenation_value {',' variable_concatenation_value } '}'
// [SPEC] variable_concatenation_value ::=
//    hierarchical_variable_identifier
//   |hierarchical_variable_identifier '[' expression ']' {'[' expression ']'}
//   |hierarchical_variable_identifier '[' expression ']' {'[' expression ']'}
//      [ range_expression ]
//   |hierarchical_variable_identifier [ range_expression ]
//   |variable_concatenation
// オリジナルでは variable_concatenation_value という非終端記号を定義して
// いるがこれは variable_lvalue と同一のものであるので置き換えた
// さらに net_concatenation と variable_concatenation は区別がほとんどつか
// ないので一つにした．
// しかし良く考えてみるとこの net_concatenation_value の中の expression
// は constant_expression の間違いだよね．どっちにしろ同じだけど．

// [SPEC] net_lvalue ::=
//    hierarchical_net_identifier
//   |hierarchical_net_identifier '[' constant_expression ']'
//      {'[' constant_expression ']'}
//   |hierarchical_net_identifier '[' constant_expression ']'
//      {'[' constant_expression ']'} '[' constant_range_expression ']'
//   |hierarchical_net_identifier '[' constant_range_expression ']'
//   |net_concatenation
// [SPEC] variable_lvalue ::=
//    hierarchical_variable_identifier
//   |hierarchical_variable_identifier '[' expression ']'
//      {'[' expression ']'}
//   |hierarchical_variable_identifier '[' expression ']'
//      {'[' expression ']'} '[' range_expression ']'
//   |hierarchical_variable_identifier '[' range_expression ']'
//   |variable_concatenation
// [SPEC] hierarchical_net_identifier ::= hierarchical_identifier
// [SPEC] hierarchical_variable_identifier ::= hierarchical_identifier
// net_lvalue と variable_lvalue の違いは添字などが const か否かだが
// syntax 的に切り分けるのは難しいので両方とも const はつけていない．
// あとでチェックする必要がある．
net_lvalue
: lvalue
{
  $$ = $1;
}
;

variable_lvalue
: lvalue
{
  $$ = $1;
}
;

lvalue
: hierarchical_identifier
{
  $$ = parser.new_Primary(@1, $1);
}
| IDENTIFIER
{
  $$ = parser.new_Primary(@1, $1);
}
| hierarchical_identifier nzlist_of_index
{
  $$ = parser.new_Primary(@$, $1, $2);
}
| IDENTIFIER nzlist_of_index
{
  $$ = parser.new_Primary(@$, $1, $2);
}
| hierarchical_identifier                 '[' expression ':' expression ']'
{
  $$ = parser.new_Primary(@$, $1, kVpiConstRange, $3, $5);
}
| IDENTIFIER                 '[' expression ':' expression ']'
{
  $$ = parser.new_Primary(@$, $1, kVpiConstRange, $3, $5);
}
| hierarchical_identifier                 '[' expression PLUSCOLON expression ']'
{
  $$ = parser.new_Primary(@$, $1, kVpiPlusRange, $3, $5);
}
| IDENTIFIER                 '[' expression PLUSCOLON expression ']'
{
  $$ = parser.new_Primary(@$, $1, kVpiPlusRange, $3, $5);
}
| hierarchical_identifier                 '[' expression MINUSCOLON expression ']'
{
  $$ = parser.new_Primary(@$, $1, kVpiMinusRange, $3, $5);
}
| IDENTIFIER                 '[' expression MINUSCOLON expression ']'
{
  $$ = parser.new_Primary(@$, $1, kVpiMinusRange, $3, $5);
}
| hierarchical_identifier nzlist_of_index '[' expression ':' expression ']'
{
  $$ = parser.new_Primary(@$, $1, $2, kVpiConstRange, $4, $6);
}
| IDENTIFIER nzlist_of_index '[' expression ':' expression ']'
{
  $$ = parser.new_Primary(@$, $1, $2, kVpiConstRange, $4, $6);
}
| hierarchical_identifier nzlist_of_index '[' expression PLUSCOLON expression ']'
{
  $$ = parser.new_Primary(@$, $1, $2, kVpiPlusRange,  $4, $6);
}
| IDENTIFIER nzlist_of_index '[' expression PLUSCOLON expression ']'
{
  $$ = parser.new_Primary(@$, $1, $2, kVpiPlusRange,  $4, $6);
}
| hierarchical_identifier nzlist_of_index '[' expression MINUSCOLON expression ']'
{
  $$ = parser.new_Primary(@$, $1, $2, kVpiMinusRange, $4, $6);
}
| IDENTIFIER nzlist_of_index '[' expression MINUSCOLON expression ']'
{
  $$ = parser.new_Primary(@$, $1, $2, kVpiMinusRange, $4, $6);
}
| '{' nzlist_of_lvalues '}'
{
  $$ = parser.new_Concat(@$, $2);
}
;

// [SPEC*] nzlist_of_index ::= index { index }
nzlist_of_index
: index
{
  $$ = parser.new_list<PtExpr>();
  $$->push_back($1);
}
| nzlist_of_index index
{
  $$ = $1;
  $$->push_back($2);
}
;

// [SPEC*] index ::= '[' expression ']'
index
: '[' expression ']'
{
  $$ = $2;
}
;

// [SPEC*] nzlist_of_lvalues ::= lvalue {',' lvalue }
nzlist_of_lvalues
: lvalue
{
  $$ = parser.new_list<PtExpr>();
  $$->push_back($1);
}
| nzlist_of_lvalues ',' lvalue
{
  $$ = $1;
  $$->push_back($3);
}
;


//////////////////////////////////////////////////////////////////////
// A.8.6 Operators
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// A.8.7 Numbers
//////////////////////////////////////////////////////////////////////

number
: unumber
{
  $$ = $1;
}
| numbase UNUMBER
{
  // この時の UNUMBER は numbase に対して適切な文字から構成されている
  // はず．
  $$ = parser.new_IntConst(@$, $1, $2);
}
| UNUM_INT numbase UNUMBER
{
  // 2番目の UNUMBER は number に対して適切な文字から構成されている
  // はず．
  $$ = parser.new_IntConst(@$, $1, $2, $3);
}
| UNUM_BIG numbase UNUMBER
{
  parser.put_msg(__FILE__, __LINE__,
		 @1,
		 kMsgError,
		 "PAR",
		 "size is too big.");
  YYERROR;
}
| rnumber
{
  $$ = $1;
}
;

unumber
: UNUM_INT
{
  $$ = parser.new_IntConst(@$, $1);
}
| UNUM_BIG
{
  $$ = parser.new_IntConst(@$, $1);
}
;

rnumber
: RNUMBER
{
  $$ = parser.new_RealConst(@$, $1);
}
;

// 基数と符号の有無
numbase
: BASE_B
{
  $$ = kVpiBinaryConst;
}
| BASE_SB
{
  $$ = kVpiSignedBinaryConst;
}
| BASE_O
{
  $$ = kVpiOctConst;
}
| BASE_SO
{
  $$ = kVpiSignedOctConst;
}
| BASE_D
{
  $$ = kVpiDecConst;
}
| BASE_SD
{
  $$ = kVpiSignedDecConst;
}
| BASE_H
{
  $$ = kVpiHexConst;
}
| BASE_SH
{
  $$ = kVpiSignedHexConst;
}
;


//////////////////////////////////////////////////////////////////////
// A.8.8 Strings
//////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////
// A.9.1 Attributes
//////////////////////////////////////////////////////////////////////

// [SPEC*] ai_list ::= { attribute_instance }
// [SPEC] attribute_instance ::= "(*" attr_spec {',' attr_spec} "*)"
ai_list
: // 空もありうる
{
  $$ = NULL;
  @$ = FileRegion();
}
| nz_ai_list
{
  $$ = $1;
}
;

// [SPEC*] nz_ai_list ::= attribute_instance { attribute_instance }
nz_ai_list
: attr_inst
{
  $$ = parser.new_list<PtAttrInst>();
  $$->push_back($1);
}
| nz_ai_list attr_inst
{
  $$ = $1;
  $$->push_back($2);
}
;

attr_inst
: PRSTAR nzlist_of_attr_spec STARPR
{
  $$ = parser.new_AttrInst(@$, $2);
}
| PRSTAR error STARPR
{
  $$ = NULL;
  yyerrok;
}
;

// [SPEC*] nazlist_of_attr_spec ::= attr_sprc {',' attr_spec}
nzlist_of_attr_spec
: attr_spec
{
  $$ = parser.new_list<PtAttrSpec>();
  $$->push_back($1);
}
| nzlist_of_attr_spec ',' attr_spec
{
  $$ = $1;
  $$->push_back($3);
}
;

// [SPEC] attr_spec ::=
//    attr_name '=' constant_expression
//   |attr_name
// [SPEC] attr_name ::= identifier
attr_spec
: IDENTIFIER
{
  $$ = parser.new_AttrSpec(@1, $1, NULL);
}
| IDENTIFIER '=' expression
{
  $$ = parser.new_AttrSpec(@$, $1, $3);
}
;


//////////////////////////////////////////////////////////////////////
// A.9.2 Comments
//////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////
// A.9.3 Identifiers
//////////////////////////////////////////////////////////////////////

// [SPEC] arrayed_identifier ::=
//             simple_arrayed_identifier
//            |escaped_arrayed_identifier
// [SPEC] block_identifier ::= identifier
// [SPEC] cell_identifier ::= identifier
// [SPEC] config_identifier ::= identifier
// [SPEC] escaped_arrayed_identifier ::= escaped_identifier [ range ]
// [SPEC] escaped_hierarchical_identifier ::=
//             escaped_hierarchical_branch
//                 { '.' simple_hierarchical_branch |
//                   '.' escaped_hierarchical_branch }
// [SPEC] escaped_identifier ::=
//             \{Any_ASCII_character_except_white_space} white_space
// [SPEC] event_identifier ::= identifier
// [SPEC] function_identifier ::= identifier
// [SPEC] gate_instance_identifier ::= arrayed_identifier
// [SPEC] generate_block_identifier ::= identifier
// [SPEC] genvar_function_identifier ::= identifier /* Hierarchy disallowd */
// [SPEC] genvar_identifier ::= identifier
// [SPEC] hierarchical_block_identifier ::= hierarchical_identifier
// [SPEC] hierarchical_event_identifier ::= hierarchical_identifier
// [SPEC] hierarchical_function_identifier ::= hierarchical_identifier
// [SPEC] hierarchical_identifier ::=
//             simple_hierarchical_identifier
//            |escaped_hierarchical_identifier
// [SPEC] hierarchical_net_identifier ::= hierarchical_identifier
// [SPEC] hierarchical_variable_identifier ::= hierarchical_identifier
// [SPEC] hierarchical_task_identifier ::= hierarchical_identifier
// [SPEC] identifier ::=
//             simple_identifier
//            |escaped_identifier
// [SPEC] inout_port_identifier ::= identifier
// [SPEC] input_port_identifier ::= identifier
// [SPEC] instance_identifier ::= identifier
// [SPEC] library_identifier ::= identifier
// [SPEC] memory_identifier ::= identifier
// [SPEC] module_identifier ::= identifier
// [SPEC] module_instance_identifier ::= arrayed_identifier
// [SPEC] net_identifier ::= identifier
// [SPEC] output_port_identifier ::= identifier
// [SPEC] parameter_identifier ::= identifier
// [SPEC] port_identifier ::= identifier
// [SPEC] real_identifier ::= identifier
// [SPEC] simple_arrayed_identifier ::= simple_identifier [ range ]
// [SPEC] simple_hierarchical_indentifier ::=
//            simple_hierarchical_branch [ '.' escaped_identifier ]
// [SPEC] simple_identifier ::= [a-zA-Z_]{[a-zA-Z0-9_$]}
// [SPEC] specparam_identifier ::= identifier
// [SPEC] system_function_identifier ::= $[a-zA-Z0-9_$]{[a-zA-Z0-9_$]}
// [SPEC] system_task_identifier ::= $[a-zA-Z0-9_$]{[a-zA-Z0-9_$]}
// [SPEC] task_identifier ::= identifier
// [SPEC] terminal_identifier ::= identifier
// [SPEC] text_maxro_identifier ::= simple_identifier
// [SPEC] topmodule_identifier ::= identifier
// [SPEC] udp_identifier ::= identifier
// [SPEC] udp_instance_identifier ::= arrayed_identifier
// [SPEC] variable_identifier ::= identifier
//
// まず simple と escaped の区別をなくし hierarchical_branch を
// 展開している．そうしないと還元還元矛盾を引き起こす．
// また '[' ']' の中を UNUMBER とするとシフト還元矛盾を引き起こすので
// expression として読んでおいてあとでチェックする．
// 実際, 意味的には整数型の定数値になればいいはずである．
//
// ***注意***
// もともと hierarchical_identifier は階層のない identifier を含んでいるが
// ここでは最低でも1階層以上の階層をもつものを hierarchical_identifier と
// している．hierarchical_identifier を使うところでは IDENTIFIER も併せて
// 定義しておかないといけない．
//
// [SPEC*] hierarchical_identifier ::=
//    identifier
//  | hierarchical_identifier ['[' expression ']'] '.' identifier
hierarchical_identifier
: IDENTIFIER '.' IDENTIFIER
{
  $$ = parser.new_HierName($1, $3);
}
| IDENTIFIER '[' expression ']' '.' IDENTIFIER
{
  if ( !$3->is_index_expr() ) {
    // エラー: 添字に使える式ではなかった．
    parser.put_msg(__FILE__, __LINE__,
		   $3->file_region(),
		   kMsgError,
		   "PARS",
		   "illegal expression in hierarchical identifier.");
    $$ = NULL;
    YYERROR;
  }
  else {
    $$ = parser.new_HierName($1, $3->index_value(), $6);
  }
}
| hierarchical_identifier '.' IDENTIFIER
{
  $$ = $1;
  parser.add_HierName($1, $3);
}
| hierarchical_identifier '[' expression ']' '.' IDENTIFIER
{
  if ( !$3->is_index_expr() ) {
    // エラー: 添字に使える式ではなかった．
    parser.put_msg(__FILE__, __LINE__,
		   $3->file_region(),
		   kMsgError,
		   "PARS",
		   "illegal expression in hierarchical identifier.");
    $$ = NULL;
    YYERROR;
  }
  else {
    $$ = $1;
    parser.add_HierName($1, $3->index_value(), $6);
  }
}
;


//////////////////////////////////////////////////////////////////////
// A.9.4 Identifier branches
//////////////////////////////////////////////////////////////////////

// [SPEC] simple_hierarchical_branch ::=
//    simple_identifier ['[' unsigned_number ']']
//      [{'.' simple_identifier ['[' unsigned_number ']']
// [SPEC] escaped_hierarchical_branch ::=
//    escaped_identifier ['[' unsigned_number ']']
//      [{'.' escaped_identifier ['[' unsigned_number ']']


//////////////////////////////////////////////////////////////////////
// A.9.5 White space
//////////////////////////////////////////////////////////////////////

// [SPEC] white_space ::= space|tab|newline|eof



//////////////////////////////////////////////////////////////////////
%%

// yacc パーサーが内部で呼び出す関数
// トークンを一つ読み込みトークン番号を返す．
// 同時に意味値(semantic value)を yylval にセットする．
// また位置を yylloc にセットする．
int
yylex(YYSTYPE* lvalp,
      YYLTYPE* llocp,
      Parser& parser)
{
  return parser.yylex(*lvalp, *llocp);
}

// yacc パーサーが内部で呼び出す関数
// エラーメッセージを出力する．
int
yyerror(YYLTYPE* llocp,
	Parser& parser,
	const char* s)
{
  string s2;
  // 好みの問題だけど "parse error" よりは "syntax error" の方が好き．
  if ( !strncmp(s, "parse error", 11) ) {
    s2 ="syntax error";
    s2 += (s + 11);
  }
  else {
    s2 = s;
  }

  parser.put_msg(__FILE__, __LINE__,
		 *llocp,
		 kMsgError,
		 "PARS",
		 s2);

  return 1;
}

END_NAMESPACE_YM_VERILOG
