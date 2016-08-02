
/// @file libym_verilog/tests/ptview/VerilogSyntaxHighlighter.cc
/// @brief VerilogSyntaxHighlighter の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "VerilogSyntaxHighlighter.h"


// @brief コンストラクタ
// @param[in] parent 親の TextEdit
VerilogSyntaxHighlighter::VerilogSyntaxHighlighter(QTextEdit* parent) :
  QSyntaxHighlighter(parent)
{
  mFont = QFont("curier", 10);

  mNormalColor = Qt::black;
  mIdentifierColor = Qt::blue;
  mKeywordColor = Qt::darkBlue;
  mStringColor = Qt::darkGreen;
  mConstantColor = Qt::darkCyan;
  mCommentColor = Qt::darkGray;
  mDirectiveColor = Qt::darkYellow;
  mErrorColor = Qt::red;

  set_format();
  
  // 予約語の登録
  mKeywordHash.insert("always", 1);
  mKeywordHash.insert("and", 1);
  mKeywordHash.insert("assign", 1);
  mKeywordHash.insert("automatic", 1);
  mKeywordHash.insert("begin", 1);
  mKeywordHash.insert("buf", 1);
  mKeywordHash.insert("bufif0", 1);
  mKeywordHash.insert("bufif1", 1);
  mKeywordHash.insert("case", 1);
  mKeywordHash.insert("casex", 1);
  mKeywordHash.insert("casez", 1);
  mKeywordHash.insert("cell", 1);
  mKeywordHash.insert("cmos", 1);
  mKeywordHash.insert("config", 1);
  mKeywordHash.insert("deassign", 1);
  mKeywordHash.insert("default", 1);
  mKeywordHash.insert("defparam", 1);
  mKeywordHash.insert("design", 1);
  mKeywordHash.insert("disable", 1);
  mKeywordHash.insert("edge", 1);
  mKeywordHash.insert("else", 1);
  mKeywordHash.insert("end", 1);
  mKeywordHash.insert("endcase", 1);
  mKeywordHash.insert("endconfig", 1);
  mKeywordHash.insert("endfunction", 1);
  mKeywordHash.insert("endgenerate", 1);
  mKeywordHash.insert("endmodule", 1);
  mKeywordHash.insert("endprimitive", 1);
  mKeywordHash.insert("endspecify", 1);
  mKeywordHash.insert("endtable", 1);
  mKeywordHash.insert("endtask", 1);
  mKeywordHash.insert("event", 1);
  mKeywordHash.insert("for", 1);
  mKeywordHash.insert("force", 1);
  mKeywordHash.insert("forever", 1);
  mKeywordHash.insert("fork", 1);
  mKeywordHash.insert("function", 1);
  mKeywordHash.insert("generate", 1);
  mKeywordHash.insert("genvar", 1);
  mKeywordHash.insert("highz0", 1);
  mKeywordHash.insert("highz1", 1);
  mKeywordHash.insert("if", 1);
  mKeywordHash.insert("initial", 1);
  mKeywordHash.insert("ifnone", 1);
  mKeywordHash.insert("incdir", 1);
  mKeywordHash.insert("include", 1);
  mKeywordHash.insert("inout", 1);
  mKeywordHash.insert("input", 1);
  mKeywordHash.insert("instance", 1);
  mKeywordHash.insert("integer", 1);
  mKeywordHash.insert("join", 1);
  mKeywordHash.insert("large", 1);
  mKeywordHash.insert("liblist", 1);
  mKeywordHash.insert("library", 1);
  mKeywordHash.insert("localparam", 1);
  mKeywordHash.insert("macromodule", 1);
  mKeywordHash.insert("medium", 1);
  mKeywordHash.insert("module", 1);
  mKeywordHash.insert("nand", 1);
  mKeywordHash.insert("negedge", 1);
  mKeywordHash.insert("nmos", 1);
  mKeywordHash.insert("nor", 1);
  mKeywordHash.insert("noshowcancelled", 1);
  mKeywordHash.insert("not", 1);
  mKeywordHash.insert("notif0", 1);
  mKeywordHash.insert("notif1", 1);
  mKeywordHash.insert("or", 1);
  mKeywordHash.insert("output", 1);
  mKeywordHash.insert("parameter", 1);
  mKeywordHash.insert("pmos", 1);
  mKeywordHash.insert("posedge", 1);
  mKeywordHash.insert("primitive", 1);
  mKeywordHash.insert("pull0", 1);
  mKeywordHash.insert("pull1", 1);
  mKeywordHash.insert("pullup", 1);
  mKeywordHash.insert("pulldown", 1);
  mKeywordHash.insert("pulsestyle_onevent", 1);
  mKeywordHash.insert("pulsestyle_ondetect", 1);
  mKeywordHash.insert("rcmos", 1);
  mKeywordHash.insert("real", 1);
  mKeywordHash.insert("realtime", 1);
  mKeywordHash.insert("reg", 1);
  mKeywordHash.insert("release", 1);
  mKeywordHash.insert("repeat", 1);
  mKeywordHash.insert("rnmos", 1);
  mKeywordHash.insert("rpmos", 1);
  mKeywordHash.insert("rtran", 1);
  mKeywordHash.insert("rtranif0", 1);
  mKeywordHash.insert("rtranif1", 1);
  mKeywordHash.insert("scalared", 1);
  mKeywordHash.insert("showcancelled", 1);
  mKeywordHash.insert("signed", 1);
  mKeywordHash.insert("small", 1);
  mKeywordHash.insert("specify", 1);
  mKeywordHash.insert("specparam", 1);
  mKeywordHash.insert("strong0", 1);
  mKeywordHash.insert("strong1", 1);
  mKeywordHash.insert("supply0", 1);
  mKeywordHash.insert("supply1", 1);
  mKeywordHash.insert("table", 1);
  mKeywordHash.insert("task", 1);
  mKeywordHash.insert("time", 1);
  mKeywordHash.insert("tran", 1);
  mKeywordHash.insert("tranif0", 1);
  mKeywordHash.insert("tranif1", 1);
  mKeywordHash.insert("tri", 1);
  mKeywordHash.insert("tri0", 1);
  mKeywordHash.insert("tri1", 1);
  mKeywordHash.insert("triand", 1);
  mKeywordHash.insert("trior", 1);
  mKeywordHash.insert("trireg", 1);
  mKeywordHash.insert("unsigned", 1);
  mKeywordHash.insert("use", 1);
  mKeywordHash.insert("vectored", 1);
  mKeywordHash.insert("wait", 1);
  mKeywordHash.insert("wand", 1);
  mKeywordHash.insert("weak0", 1);
  mKeywordHash.insert("weak1", 1);
  mKeywordHash.insert("while", 1);
  mKeywordHash.insert("wire", 1);
  mKeywordHash.insert("wor", 1);
  mKeywordHash.insert("xor", 1);
  mKeywordHash.insert("xnor", 1);
  
  mKeywordHash.insert("$setup", 1);
  mKeywordHash.insert("$hold", 1);
  mKeywordHash.insert("$setuphold", 1);
  mKeywordHash.insert("$recovery", 1);
  mKeywordHash.insert("$removal", 1);
  mKeywordHash.insert("$recrem", 1);
  mKeywordHash.insert("$skew", 1);
  mKeywordHash.insert("$timeskew", 1);
  mKeywordHash.insert("$fullskew", 1);
  mKeywordHash.insert("$period", 1);
  mKeywordHash.insert("$width", 1);
  mKeywordHash.insert("$nochange", 1);
}

// @brief デストラクタ
VerilogSyntaxHighlighter::~VerilogSyntaxHighlighter()
{
}

// フォーマットをセットする．
void
VerilogSyntaxHighlighter::set_format()
{
  mNormalFormat.setFont(mFont);
  mNormalFormat.setForeground(mNormalColor);
  mNormalFormat.setFontFixedPitch(true);
  
  mIdentifierFormat.setFont(mFont);
  mIdentifierFormat.setForeground(mIdentifierColor);
  mIdentifierFormat.setFontFixedPitch(true);
  
  mKeywordFormat.setFont(mFont);
  mKeywordFormat.setForeground(mKeywordColor);
  mKeywordFormat.setFontFixedPitch(true);
  
  mStringFormat.setFont(mFont);
  mStringFormat.setForeground(mStringColor);
  mStringFormat.setFontFixedPitch(true);

  mConstantFormat.setFont(mFont);
  mConstantFormat.setForeground(mConstantColor);
  mConstantFormat.setFontFixedPitch(true);

  mCommentFormat.setFont(mFont);
  mCommentFormat.setForeground(mCommentColor);
  mCommentFormat.setFontFixedPitch(true);

  mDirectiveFormat.setFont(mFont);
  mDirectiveFormat.setForeground(mDirectiveColor);
  mDirectiveFormat.setFontFixedPitch(true);

  mErrorFormat.setFont(mFont);
  mErrorFormat.setForeground(mErrorColor);
  mErrorFormat.setFontFixedPitch(true);
}


// 識別子の先頭に使える文字かどうか調べる．
bool
is_idchar1(QChar c)
{
  if ( c.isLetter() ||
       c == '_'  ||
       c == '$' ) {
    return true;
  }
  return false;
}

// 識別子に使える文字かどうか調べる．
bool
is_idchar2(QChar c)
{
  if ( c.isLetterOrNumber() ||
       c == '_'  ||
       c == '$' ) {
    return true;
  }
  return false;
}

// 2進定数に使える文字かどうか調べる．
bool
is_binchar(QChar c)
{
  c = c.toUpper();
  if ( c == '0' ||
       c == '1' ||
       c == 'X' ||
       c == 'Z' ||
       c == '_' ||
       c == '?' ) {
    return true;
  }
  return false;
}

// 8進定数に使える文字かどうか調べる．
bool
is_octchar(QChar c)
{
  c = c.toUpper();
  if ( c == '0' ||
       c == '1' ||
       c == '2' ||
       c == '3' ||
       c == '4' ||
       c == '5' ||
       c == '6' ||
       c == '7' ||
       c == 'X' ||
       c == 'Z' ||
       c == '_' ||
       c == '?' ) {
    return true;
  }
  return false;
}

// 10進定数に使える文字かどうか調べる．
bool
is_decchar(QChar c)
{
  c = c.toUpper();
  if ( c.isDigit() ||
       c == 'X' ||
       c == 'Z' ||
       c == '_' ||
       c == '?' ) {
    return true;
  }
  return false;
}

// 16進定数に使える文字かどうか調べる．
bool
is_hexchar(QChar c)
{
  c = c.toUpper();
  if ( c.isDigit() ||
       c == 'A' ||
       c == 'B' ||
       c == 'C' ||
       c == 'D' ||
       c == 'E' ||
       c == 'F' ||
       c == 'X' ||
       c == 'Z' ||
       c == '_' ||
       c == '?' ) {
    return true;
  }
  return false;
}

// UDP のテーブルに使える文字かどうか調べる．
bool
is_udpchar(QChar c)
{
  c = c.toUpper();
  if ( c == '-' ||
       c == '*' ||
       c == '0' ||
       c == '1' ||
       c == 'X' ||
       c == '?' ||
       c == 'B' ||
       c == 'R' ||
       c == 'F' ||
       c == 'P' ||
       c == 'N' ||
       c == '(' ||
       c == ')' ||
       c == ':' ||
       c == ';' ) {
    return true;
  }
  return false;
}

// @brief 色分けを行う．
void
VerilogSyntaxHighlighter::highlightBlock(const QString& text)
{
  int state = previousBlockState();
  if ( state == -1 ) {
    state = kNormal;
  }
  
  bool in_comment = static_cast<bool>((state >> 4) & 1);
  state &= 15;
  int pos = 0;
  int start = 0;
  int l = text.length();

  while ( pos < l ) {
    if ( in_comment ) {
      if ( text.mid(pos, 2) == "*/" ) {
	pos += 2;
	setFormat(start, pos - start, mCommentFormat);
	in_comment = false;
      }
      else {
	++ pos;
      }
      continue;
    }

    start = pos;

    // 最初の空白は読み飛ばす．
    for ( ; pos < l && text[pos].isSpace(); ++ pos) ;
    setFormat(start, pos - start, mNormalFormat);
    if ( pos >= l ) break;

    start = pos;
    if ( text.mid(pos, 2) == "//" ) {
      // C++ スタイルのコメント
      setFormat(start, l - start, mCommentFormat);
      break;
    }
    if ( text.mid(pos, 2) == "/*" ) {
      // C スタイルのコメント
      in_comment = true;
      pos += 2;
      continue;
    }

    switch ( state ) {
    case kNormal:
      if ( is_idchar1(text[pos]) ) {
	// 識別子か予約語
	for ( ++ pos; pos < l; ++ pos) {
	  QChar c = text[pos];
	  if ( !is_idchar2(c) ) {
	    break;
	  }
	}
	QString str = text.mid(start, pos - start);
	if ( mKeywordHash.contains(str) ) {
	  setFormat(start, pos - start, mKeywordFormat);
	  if ( str == "table" ) {
	    state = kUdp;
	  }
	}
	else {
	  setFormat(start, pos - start, mIdentifierFormat);
	}
      }
      else if ( text[pos].isNumber() ) {
	// 数値
	for ( ++ pos; pos < l; ++ pos) {
	  QChar c = text[pos];
	  if ( !c.isNumber() && c != '_' ) {
	    break;
	  }
	}
	setFormat(start, pos - start, mConstantFormat);
      }
      else if ( text[pos] == '\"' ) {
	// 文字列
	bool found = false;
	for ( ++ pos; pos < l; ++ pos) {
	  QChar c = text[pos];
	  if ( c == '\"' ) {
	    ++ pos;
	    setFormat(start, pos - start, mStringFormat);
	    found = true;
	    break;
	  }
	}
	if ( !found ) {
	  setFormat(start, pos - start, mErrorFormat);
	}
      }
      else if ( text[pos] == '\\' ) {
	// escaped identifier
	for ( ++ pos; pos < l; ++ pos) {
	  QChar c = text[pos];
	  if ( c.isSpace() ) {
	    break;
	  }
	}
	setFormat(start, pos - start, mIdentifierFormat);
      }
      else if ( text[pos] == '`' ) {
	// compier directive
	for ( ++ pos; pos < l; ++ pos) {
	  QChar c = text[pos];
	  if ( c.isSpace() ) {
	    break;
	  }
	}
	setFormat(start, pos - start, mDirectiveFormat);
      }
      else if ( text.mid(pos, 2) == "'b" ) {
	// 2進定数
	setFormat(start, 2, mConstantFormat);
	pos += 2;
	state = kBin;
      }
      else if ( text.mid(pos, 3) == "'sb" ) {
	// 2進定数
	setFormat(start, 2, mConstantFormat);
	pos += 3;
	state = kBin;
      }
      else if ( text.mid(pos, 2) == "'o" ) {
	// 8進定数
	setFormat(start, 2, mConstantFormat);
	pos += 2;
	state = kOct;
      }
      else if ( text.mid(pos, 3) == "'so" ) {
	// 8進定数
	setFormat(start, 2, mConstantFormat);
	pos += 3;
	state = kOct;
      }
      else if ( text.mid(pos, 2) == "'d" ) {
	// 10進定数
	setFormat(start, 2, mConstantFormat);
	pos += 2;
	state = kDec;
      }
      else if ( text.mid(pos, 3) == "'sd" ) {
	// 10進定数
	setFormat(start, 2, mConstantFormat);
	pos += 3;
	state = kDec;
      }
      else if ( text.mid(pos, 2) == "'h" ) {
	// 16進定数
	setFormat(start, 2, mConstantFormat);
	pos += 2;
	state = kHex;
      }
      else if ( text.mid(pos, 3) == "'sh" ) {
	// 16進定数
	setFormat(start, 2, mConstantFormat);
	pos += 3;
	state = kHex;
      }
      else {
	// その他
	setFormat(pos, 1, mNormalFormat);
	++ pos;
      }
      break;

    case kBin:
      if ( !is_binchar(text[pos]) ) {
	// 次の区切りまでエラー
	for ( ; pos < l; ++ pos ) {
	  QChar c = text[pos].toUpper();
	  if ( c.isLetterOrNumber() ) {
	    continue;
	  }
	  break;
	}
	setFormat(start, pos - start, mErrorFormat);
      }
      else {
	// 次の区切りまで定数
	for ( ; pos < l; ++ pos) {
	  QChar c = text[pos].toUpper();
	  if ( !is_binchar(c) ) {
	    break;
	  }
	}
	setFormat(start, pos - start, mConstantFormat);
      }
      state = kNormal;
      break;
      
    case kOct:
      if ( !is_octchar(text[pos]) ) {
	// 次の区切りまでエラー
	for ( ; pos < l; ++ pos ) {
	  QChar c = text[pos].toUpper();
	  if ( c.isLetterOrNumber() ) {
	    continue;
	  }
	  break;
	}
	setFormat(start, pos - start, mErrorFormat);
      }
      else {
	// 次の区切りまで定数
	for ( ; pos < l; ++ pos) {
	  QChar c = text[pos].toUpper();
	  if ( !is_octchar(c) ) {
	    break;
	  }
	}
	setFormat(start, pos - start, mConstantFormat);
      }
      state = kNormal;
      break;
      
    case kDec:
      if ( !is_decchar(text[pos]) ) {
	// 次の区切りまでエラー
	for ( ; pos < l; ++ pos ) {
	  QChar c = text[pos].toUpper();
	  if ( c.isLetterOrNumber() ) {
	    continue;
	  }
	  break;
	}
	setFormat(start, pos - start, mErrorFormat);
      }
      else {
	// 次の区切りまで定数
	for ( ; pos < l; ++ pos) {
	  QChar c = text[pos].toUpper();
	  if ( !is_decchar(c) ) {
	    break;
	  }
	}
	setFormat(start, pos - start, mConstantFormat);
      }
      state = kNormal;
      break;
      
    case kHex:
      if ( !is_hexchar(text[pos]) ) {
	// 次の区切りまでエラー
	for ( ; pos < l; ++ pos ) {
	  QChar c = text[pos].toUpper();
	  if ( c.isLetterOrNumber() ) {
	    continue;
	  }
	  break;
	}
	setFormat(start, pos - start, mErrorFormat);
      }
      else {
	// 次の区切りまで定数
	for ( ; pos < l; ++ pos) {
	  QChar c = text[pos].toUpper();
	  if ( !is_hexchar(c) ) {
	    break;
	  }
	}
	setFormat(start, pos - start, mConstantFormat);
      }
      state = kNormal;
      break;

    case kUdp:
      if ( text[pos] == 'e' ) {
	// 識別子か予約語
	int pos1;
	for (pos1 = pos + 1; pos1 < l; ++ pos1) {
	  QChar c = text[pos1];
	  if ( !is_idchar2(c) ) {
	    break;
	  }
	}
	QString str = text.mid(start, pos1 - start);
	if ( str == "endtable" ) {
	  setFormat(start, pos1 - start, mKeywordFormat);
	  state = kNormal;
	  pos = pos1;
	  continue;
	}
      }
      if ( is_udpchar(text[pos]) ) {
	setFormat(pos, 1, mConstantFormat);
      }
      else {
	setFormat(pos, 1, mErrorFormat);
      }
      ++ pos;
      continue;

    default:
      break;
    }
  }
  if ( in_comment ) {
    // 行末までいってコメントが終わらなかった．
    setFormat(start, l - start, mCommentFormat);
    state |= 16;
  }
  setCurrentBlockState(state);
}

