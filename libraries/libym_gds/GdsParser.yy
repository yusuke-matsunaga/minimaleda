%{

/// @file libym_gds/GdsParser.yy
/// @brief GDS-II のパーサ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: GdsParser.yy 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_gds/Msg.h"
#include "ym_gds/GdsScanner.h"
#include "ym_gds/GdsDumper.h"
#include "ym_gds/GdsRecord.h"


// より詳細なエラー情報を出力させる．
#define YYERROR_VERBOSE 1

// シンボルの値を GdsRecord へのポインタに変更
// このやり方が正しいかどうかは知らない．
#ifdef YYSTYPE
#undef YYSTYPE
#endif
#define YYSTYPE GdsRecord*

BEGIN_NAMESPACE_YM_GDS_PARSER

// yacc/bison が生成したヘッダファイル
#include "GdsParser.h"

// このファイルで定義されている関数
extern
int yylex();

extern 
int yyerror(const char* );
 
%}

// トークン
%token ERROR

%token HEADER
%token BGNLIB
%token LIBNAME
%token UNITS
%token ENDLIB
%token BGNSTR
%token STRNAME
%token ENDSTR
%token BOUNDARY
%token PATH
%token SREF
%token AREF
%token TEXT
%token LAYER
%token DATATYPE
%token WIDTH
%token XY
%token ENDEL
%token SNAME
%token COLROW
%token TEXTNODE
%token NODE
%token TEXTTYPE
%token PRESENTATION
%token SPACING
%token STRING
%token STRANS
%token MAG
%token ANGLE
%token UINTEGER
%token USTRING
%token REFLIBS
%token FONTS
%token PATHTYPE
%token GENERATIONS
%token ATTRTABLE
%token STYPTABLE
%token STRTYPE
%token ELFLAGS
%token ELKEY
%token LINKTYPE
%token LINKKEYS
%token NODETYPE
%token PROPATTR
%token PROPVALUE
%token BOX
%token BOXTYPE
%token PLEX
%token BGNEXTN
%token ENDEXTN
%token TAPENUM
%token TAPECODE
%token STRCLASS
%token RESERVED
%token FORMAT
%token MASK
%token ENDMASKS
%token LIBDIRSIZE
%token SRFNAME
%token LIBSECUR
%token BORDER
%token SOFTFENCE
%token HARDFENCE
%token SOFTWIRE
%token HARDWIRE
%token PATHPORT
%token NODEPORT
%token USERCONSTRAINT
%token SPACER_ERROR
%token CONTACT


%%

stream_format
: HEADER BGNLIB opt_LIBDIRSIZE opt_SRFNAME opt_LIBSECUR
  LIBNAME opt_REFLIBS opt_FONTS opt_ATTRTABLE opt_GENERATIONS
  opt_FormatType UNITS star_structure ENDLIB
;

opt_LIBDIRSIZE
: // null
| LIBDIRSIZE
;

opt_SRFNAME
: // null
| SRFNAME
;

opt_LIBSECUR
: // null
| LIBSECUR
;

opt_REFLIBS
: // null
| REFLIBS
;

opt_FONTS
: // null
| FONTS
;

opt_ATTRTABLE
: // null
| ATTRTABLE
;

opt_GENERATIONS
: // null
| GENERATIONS
;

opt_FormatType
: // null
| FORMAT
| FORMAT plus_MASK ENDMASKS
;

plus_MASK
: MASK
| plus_MASK MASK
;

star_structure
: // null
| star_structure structure
;

structure
: BGNSTR STRNAME          star_element ENDSTR
| BGNSTR STRNAME STRCLASS star_element ENDSTR
;

star_element
: // null
| star_element element
;

element
: elem_header star_property ENDEL
;

elem_header
: boundary
| path
| sref
| aref
| text
| node
| box
;

boundary
: BOUNDARY opt_ELFLAGS opt_PLEX LAYER DATATYPE XY
;

path
: PATH opt_ELFLAGS opt_PLEX LAYER DATATYPE opt_PATHTYPE
  opt_WIDTH opt_BGNEXTN opt_ENDEXTN XY
;

sref
: SREF opt_ELFLAGS opt_PLEX SNAME opt_strans XY
;

aref
: AREF opt_ELFLAGS opt_PLEX SNAME opt_strans COLROW XY
;

text
: TEXT opt_ELFLAGS opt_PLEX LAYER textbody
;

node
: NODE opt_ELFLAGS opt_PLEX LAYER NODETYPE XY
;

box
: BOX opt_ELFLAGS opt_PLEX LAYER BOXTYPE XY
;

textbody
: TEXTTYPE opt_PRESENTATION opt_PATHTYPE opt_WIDTH opt_strans XY STRING
;

opt_ELFLAGS
: // null
| ELFLAGS
;

opt_PLEX
: // null
| PLEX
;

opt_PATHTYPE
: // null
| PATHTYPE
;

opt_WIDTH
: // null
| WIDTH
;

opt_BGNEXTN
: // null
| BGNEXTN
;

opt_ENDEXTN
: // null
| ENDEXTN
;

opt_strans
: // null
| STRANS
| STRANS     ANGLE
| STRANS MAG
| STRANS MAG ANGLE
;

opt_PRESENTATION
: // null
| PRESENTATION
;

star_property
: // null
| star_property PROPATTR PROPVALUE
;

%%

// yylex で用いる静的メンバ
static
GdsScanner* gds_scanner = NULL;

// scannar を設定する関数
void
set_scanner(GdsScanner* scanner)
{
  gds_scanner = scanner;
}

// 一文字(ここでは 1 record)をとってくる関数
int
yylex()
{
  GdsRecord* rec = gds_scanner->read_rec();
  if ( rec ) {
    GdsDumper dumper(cout);
    dumper(*rec);
    yylval = rec;
    return rec->rtype_token();
  }
  return 0;
}

// yacc パーサが内部で呼び出す関数
// エラーメッセージを出力する．
int
yyerror(const char* s)
{
  error_header(__FILE__, __LINE__, "GdsParser",
	       gds_scanner->cur_pos(), s);
  msg_end();

  return 1;
}

END_NAMESPACE_YM_GDS_PARSER
