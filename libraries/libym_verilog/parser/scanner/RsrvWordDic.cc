
/// @file libym_verilog/scanner/RsrvWordDic.cc
/// @brief RsrvWordDic の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: RsrvWordDic.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "RsrvWordDic.h"
#include "print_token.h"

#include "parser.h"


BEGIN_NAMESPACE_YM_VERILOG

BEGIN_NONAMESPACE

// 表を作るためのデータ構造
struct STpair {
  const char* mStr;
  int mTok;
  bool mReg;
};

// 予約語のデータの配列
static STpair init_data[] = {
  { "always",                   ALWAYS,              true },
  { "and",                      AND,                 true },
  { "assign",                   ASSIGN,              true },
  { "automatic",                AUTOMATIC,           true },
  { "begin",                    BEGIN,               true },
  { "buf",                      BUF,                 true },
  { "bufif0",                   BUFIF0,              true },
  { "bufif1",                   BUFIF1,              true },
  { "case",                     CASE,                true },
  { "casex",                    CASEX,               true },
  { "casez",                    CASEZ,               true },
  { "cell",                     CELL,                true },
  { "cmos",                     CMOS,                true },
  { "config",                   CONFIG,              true },
  { "deassign",                 DEASSIGN,            true },
  { "default",                  DEFAULT,             true },
  { "defparam",                 DEFPARAM,            true },
  { "design",                   DESIGN,              true },
  { "disable",                  DISABLE,             true },
  { "edge",                     EDGE,                true },
  { "else",                     ELSE,                true },
  { "end",                      END,                 true },
  { "endcase",                  ENDCASE,             true },
  { "endconfig",                ENDCONFIG,           true },
  { "endfunction",              ENDFUNCTION,         true },
  { "endgenerate",              ENDGENERATE,         true },
  { "endmodule",                ENDMODULE,           true },
  { "endprimitive",             ENDPRIMITIVE,        true },
  { "endspecify",               ENDSPECIFY,          true },
  { "endtable",                 ENDTABLE,            true },
  { "endtask",                  ENDTASK,             true },
  { "event",                    EVENT,               true },
  { "for",                      FOR,                 true },
  { "force",                    FORCE,               true },
  { "forever",                  FOREVER,             true },
  { "fork",                     FORK,                true },
  { "function",                 FUNCTION,            true },
  { "generate",                 GENERATE,            true },
  { "genvar",                   GENVAR,              true },
  { "highz0",                   HIGHZ0,              true },
  { "highz1",                   HIGHZ1,              true },
  { "if",                       IF,                  true },
  { "initial",                  INITIAL,             true },
  { "ifnone",                   IFNONE,              true },
  { "incdir",                   INCDIR,              true },
  { "include",                  INCLUDE,             true },
  { "inout",                    INOUT,               true },
  { "input",                    INPUT,               true },
  { "instance",                 INSTANCE,            true },
  { "integer",                  INTEGER,             true },
  { "join",                     JOIN,                true },
  { "large",                    LARGE,               true },
  { "liblist",                  LIBLIST,             true },
  { "library",                  LIBRARY,             true },
  { "localparam",               LOCALPARAM,          true },
  { "macromodule",              MACROMODULE,         true },
  { "medium",                   MEDIUM,              true },
  { "module",                   MODULE,              true },
  { "nand",                     NAND,                true },
  { "negedge",                  NEGEDGE,             true },
  { "nmos",                     NMOS,                true },
  { "nor",                      NOR,                 true },
  { "noshowcancelled",          NOSHOWCANCELLED,     true },
  { "not",                      NOT,                 true },
  { "notif0",                   NOTIF0,              true },
  { "notif1",                   NOTIF1,              true },
  { "or",                       OR,                  true },
  { "output",                   OUTPUT,              true },
  { "parameter",                PARAMETER,           true },
  { "pmos",                     PMOS,                true },
  { "posedge",                  POSEDGE,             true },
  { "primitive",                PRIMITIVE,           true },
  { "pull0",                    PULL0,               true },
  { "pull1",                    PULL1,               true },
  { "pullup",                   PULLUP,              true },
  { "pulldown",                 PULLDOWN,            true },
  { "pulsestyle_onevent",       PULSESTYLE_ONEVENT,  true },
  { "pulsestyle_ondetect",      PULSESTYLE_ONDETECT, true },
  { "rcmos",                    RCMOS,               true },
  { "real",                     REAL,                true },
  { "realtime",                 REALTIME,            true },
  { "reg",                      REG,                 true },
  { "release",                  RELEASE,             true },
  { "repeat",                   REPEAT,              true },
  { "rnmos",                    RNMOS,               true },
  { "rpmos",                    RPMOS,               true },
  { "rtran",                    RTRAN,               true },
  { "rtranif0",                 RTRANIF0,            true },
  { "rtranif1",                 RTRANIF1,            true },
  { "scalared",                 SCALARED,            true },
  { "showcancelled",            SHOWCANCELLED,       true },
  { "signed",                   SIGNED,              true },
  { "small",                    SMALL,               true },
  { "specify",                  SPECIFY,             true },
  { "specparam",                SPECPARAM,           true },
  { "strong0",                  STRONG0,             true },
  { "strong1",                  STRONG1,             true },
  { "supply0",                  SUPPLY0,             true },
  { "supply1",                  SUPPLY1,             true },
  { "table",                    TABLE,               true },
  { "task",                     TASK,                true },
  { "time",                     TIME,                true },
  { "tran",                     TRAN,                true },
  { "tranif0",                  TRANIF0,             true },
  { "tranif1",                  TRANIF1,             true },
  { "tri",                      TRI,                 true },
  { "tri0",                     TRI0,                true },
  { "tri1",                     TRI1,                true },
  { "triand",                   TRIAND,              true },
  { "trior",                    TRIOR,               true },
  { "trireg",                   TRIREG,              true },
  { "unsigned",                 UNSIGNED,            true },
  { "use",                      USE,                 true },
  { "vectored",                 VECTORED,            true },
  { "wait",                     WAIT,                true },
  { "wand",                     WAND,                true },
  { "weak0",                    WEAK0,               true },
  { "weak1",                    WEAK1,               true },
  { "while",                    WHILE,               true },
  { "wire",                     WIRE,                true },
  { "wor",                      WOR,                 true },
  { "xor",                      XOR,                 true },
  { "xnor",                     XNOR,                true },
  
  { "$setup",                   SETUP,               true },
  { "$hold",                    HOLD,                true },
  { "$setuphold",               SETUPHOLD,           true },
  { "$recovery",                RECOVERY,            true },
  { "$removal",                 REMOVAL,             true },
  { "$recrem",                  RECREM,              true },
  { "$skew",                    SKEW,                true },
  { "$timeskew",                TIMESKEW,            true },
  { "$fullskew",                FULLSKEW,            true },
  { "$period",                  PERIOD,              true },
  { "$width",                   WIDTH,               true },
  { "$nochange",                NOCHANGE,            true },
  
  { "PATHPULSE$",               PATHPULSE,           true },
  
  { "'b",                       BASE_B,              false },
  { "'sb",                      BASE_SB,             false },
  { "'o",                       BASE_O,              false },
  { "'so",                      BASE_SO,             false },
  { "'d",                       BASE_D,              false },
  { "'sd",                      BASE_SD,             false },
  { "'h",                       BASE_H,              false },
  { "'sh",                      BASE_SH,             false },
  { "~&",                       TILDEAND,            false },
  { "~|",                       TILDEOR,             false },
  { "~^",                       TILDEXOR,            false },
  { "<<",                       LTLT,                false },
  { "<<<",                      LTLTLT,              false },
  { ">>",                       GTGT,                false },
  { ">>>",                      GTGTGT,              false },
  { "<=",                       LTEQ,                false },
  { ">=",                       GTEQ,                false },
  { "==",                       EQEQ,                false },
  { "!=",                       NOTEQ,               false },
  { "===",                      EQEQEQ,              false },
  { "!==",                      NOTEQEQ,             false },
  { "&&",                       ANDAND,              false },
  { "&&&",                      ANDANDAND,           false },
  { "||",                       OROR,                false },
  { "**",                       STARSTAR,            false },
  { "=>",                       EQGT,                false },
  { "*>",                       STARGT,              false },
  { "->",                       MINUSGT,             false },
  { "(*",                       PRSTAR,              false },
  { "*)",                       STARPR,              false },
  { "+:",                       PLUSCOLON,           false },
  { "-:",                       MINUSCOLON,          false },
  
  { "';'",                      ';',                 false },
  { "':'",                      ':',                 false },
  { "'['",                      '[',                 false },
  { "']'",                      ']',                 false },
  { "'{'",                      '{',                 false },
  { "'}'",                      '}',                 false },
  { "'('",                      '(',                 false },
  { "')'",                      ')',                 false },
  { "','",                      ',',                 false },
  { "'.'",                      '.',                 false },
  { "'@'",                      '@',                 false },
  { "'#'",                      '#',                 false },
  { "'%'",                      '%',                 false },
  { "'?'",                      '?',                 false },
  { "'+'",                      '+',                 false },
  { "'-'",                      '-',                 false },
  { "'*'",                      '*',                 false },
  { "'/'",                      '/',                 false },
  { "'^'",                      '^',                 false },
  { "'!'",                      '!',                 false },
  { "'&'",                      '&',                 false },
  { "'|'",                      '|',                 false },
  { "'~'",                      '~',                 false },
  { "'<'",                      '<',                 false },
  { "'>'",                      '>',                 false },
  { "'='",                      '=',                 false },
  
  { "'0'",                      '0',                 false },
  { "'1'",                      '1',                 false },
  { "'X'",                      'x',                 false },
  { "'B'",                      'b',                 false },
  { "'R'",                      'r',                 false },
  { "'F'",                      'f',                 false },
  { "'N'",                      'n',                 false },
  { "'P'",                      'p',                 false }
};

// 文字列からのハッシュ関数
ymuint32
hash_func1(const char* str)
{
  ymuint32 h = 0;
  ymuint32 c;
  for ( ; (c = static_cast<ymuint32>(*str)); ++ str) {
    h = h * 37 + c;
  }
  return h;
}

END_NONAMESPACE


// 唯一のインスタンスを取ってくる関数
const RsrvWordDic&
RsrvWordDic::the_dic()
{
  // 唯一のオブジェクト
  static RsrvWordDic theDic;
  return theDic;
}

// コンストラクタ
RsrvWordDic::RsrvWordDic()
{
  mSize = sizeof(init_data) / sizeof(STpair);
  mCellArray = new Cell[mSize];
  mTable1 = new Cell*[mSize];
  mTable2 = new Cell*[mSize];
  for (ymuint32 i = 0; i < mSize; ++ i) {
    mTable1[i] = NULL;
    mTable2[i] = NULL;
  }
  for (ymuint32 i = 0; i < mSize; ++ i) {
    STpair& p = init_data[i];
    Cell* cell = &mCellArray[i];
    cell->mStr = p.mStr;
    cell->mTok = p.mTok;
    if ( p.mReg ) {
      ymuint32 pos1 = hash_func1(p.mStr) % mSize;
      cell->mLink1 = mTable1[pos1];
      mTable1[pos1] = cell;
    }
    ymuint32 pos2 = p.mTok % mSize;
    cell->mLink2 = mTable2[pos2];
    mTable2[pos2] = cell;
  }
}

// デストラクタ
RsrvWordDic::~RsrvWordDic()
{
  delete [] mCellArray;
  delete [] mTable1;
  delete [] mTable2;
}

// str が予約語ならそのトークン番号を返す．
// 先頭が '$' なら SYS_IDENTIFIER を返す．
// 上記以外ならば IDENTIFIER を返す．
int
RsrvWordDic::token(const char* str) const
{
  ymuint32 pos = hash_func1(str) % mSize;
  for (Cell* cell = mTable1[pos]; cell; cell = cell->mLink1) {
    if ( strcmp(str, cell->mStr) == 0 ) {
      return cell->mTok;
    }
  }
  if ( str[0] == '$' ) {
    return SYS_IDENTIFIER;
  }
  return IDENTIFIER;
}

// トークンから文字列を取り出す．
// トークンが正しくなければ NULL を返す．
const char*
RsrvWordDic::str(int token) const
{
  ymuint32 pos = token % mSize;
  for (Cell* cell = mTable2[pos]; cell; cell = cell->mLink2) {
    if ( token == cell->mTok ) {
      return cell->mStr;
    }
  }
  return NULL;
}


// @brief トークンの内容を出力する．
// @param[in] s 出力先のストリーム
// @param[in] id トークン番号
// @param[in] str トークン文字列
void
print_token(ostream& s,
	    int id,
	    const char* str)
{
  const RsrvWordDic& dic = RsrvWordDic::the_dic();

  switch ( id ) {
  case SPACE:          s << "SPACE"; break;
  case NL:             s << "NL"; break;
  case COMMENT1:       s << "COMMENT1: " << str; break;
  case COMMENT2:       s << "COMMENT2: " << endl << str; break;
  case IDENTIFIER:     s << "IDENTIFIER(" << str << ")"; break;
  case SYS_IDENTIFIER: s << "SYS_IDENTIFIER(" << str << ")"; break;
  case UNUMBER:        s << "UNUMBER(" << str << ")"; break;
  case UNUM_INT:       s << "UNUM_INT(" << str << ")"; break;
  case RNUMBER:        s << "RNUMBER(" << str << ")"; break;
  case STRING:         s << "STRING(" << str << ")"; break;
  case CD_SYMBOL:      s << "CD_SYMBOL(" << str << ")"; break;
  case ERROR:          s << "ERROR"; break;
  default:
    s << "RSRV[" << id << "](";
    str = dic.str(id);
    if ( str != NULL ) {
      s << str;
    }
    else {
      s << "??";
    }
    s << ")";
  }
}

END_NAMESPACE_YM_VERILOG
