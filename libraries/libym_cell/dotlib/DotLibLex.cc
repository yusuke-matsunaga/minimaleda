
/// @file libym_cell/dotlib/DotLibLex.cc
/// @brief DotLibLex の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DotLibLex.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "DotLibLex.h"


BEGIN_NAMESPACE_YM_CELL

//#include "dotlib_grammer.h"

#define INT_NUM 301
#define FLOAT_NUM 302
#define STR 303
#define NL 400
#define ERROR 500


// コンストラクタ
DotLibLex::DotLibLex(MsgMgr& msg_mgr) :
  mMsgMgr(msg_mgr)
{
  init();
}

// デストラクタ
DotLibLex::~DotLibLex()
{
  close_file();
}

// ファイルをセットする．
// 自動的にに clear() を呼ぶ．
// ファイルのオープンに失敗したら false を返す．
bool
DotLibLex::open_file(const string& filename)
{
  init();
  mInput.open(filename.c_str());
  if ( !mInput ) {
    return false;
  }
  mCurFileDesc = mFileDescMgr.new_file_desc(filename);
  return true;
}

// ファイルをクローズする．
void
DotLibLex::close_file()
{
  mInput.close();
  mCurFileDesc = NULL;
}

// 初期化
void
DotLibLex::init()
{
  mUngetChar = 0;
  mCR = false;
  mCurLine = 1;
  mCurColumn = 0;
}

// トークンを一つとってくる．
// 仕様
// - [0-9]* からなる文字列を整数値として認識する．
// - [0-9]*.[0-9]+(e|E)(+|-)?[0-9]+ を浮動小数点値として認識する．
// - [a-zA-Z_][a-zA-Z0-9_]* からなる文字列を識別子として認識する．
// - "..." の中身を文字列として認識する．
// - 識別子が予約語かどうかはここでは判定しない．
// - /* 〜 */ で囲まれた部分は読み飛ばす．
// - // から改行までを読みとばす．(本当の仕様ではない)
// - 改行は改行として扱う．
// - 空白，タブ，バックスラッシュ＋改行は区切り文字とみなす．
// - それ以外の文字はそのまま返す．
int
DotLibLex::read_token()
{
  int c;

  mCurString.clear();

 ST_INIT: // 初期状態
  c = get();
  mFirstColumn = mCurColumn;
  if ( isdigit(c) ) {
    mCurString.put_char(c);
    goto ST_NUM1;
  }
  if ( isalpha(c) || (c == '_') ) {
    mCurString.put_char(c);
    goto ST_ID;
  }

  switch (c) {
  case '.':
    mCurString.put_char(c);
    goto ST_DOT;

  case EOF:
    return EOF;

  case ' ':
  case '\t':
    goto ST_INIT; // 最初の空白は読み飛ばす．

  case '\n':
    nl();
    return NL;

  case '\"':
    goto ST_DQ;

  case '\\':
    c = get();
    if ( c == '\n' ) {
      // 無視する．
      nl();
      goto ST_INIT;
    }
    // それ以外はバックスラッシュがなかったことにする．
    unget();
    goto ST_INIT;

  case '/':
    goto ST_COMMENT1;

  default:
    break;
  }
  return c;

 ST_NUM1: // 一文字目が[0-9]の時
  c = get();
  if ( isdigit(c) ) {
    mCurString.put_char(c);
    goto ST_NUM1;
  }
  if ( c == '.' ) {
    mCurString.put_char(c);
    goto ST_DOT;
  }
  unget();
  return INT_NUM;

 ST_DOT: // [0-9]*'.' を読み込んだ時
  c = get();
  if ( isdigit(c) ) {
    mCurString.put_char(c);
    goto ST_NUM2;
  }
  { // '.' の直後はかならず数字
    ostringstream buf;
    buf << "digit number expected after dot";
    mMsgMgr.put_msg(__FILE__, __LINE__, cur_file_region(),
		    kMsgError,
		    "DOTLIB_LEX",
		    buf.str());
    return ERROR;
  }

 ST_NUM2: // [0-9]*'.'[0-9]* を読み込んだ時
  c = get();
  if ( isdigit(c) ) {
    mCurString.put_char(c);
    goto ST_NUM2;
  }
  if ( c == 'e' || c == 'E' ) {
    mCurString.put_char(c);
    goto ST_NUM3;
  }
  unget();
  return FLOAT_NUM;

 ST_NUM3: // [0-9]*'.'[0-9]*(e|E)を読み込んだ時
  c = get();
  if ( isdigit(c) ) {
    mCurString.put_char(c);
    goto ST_NUM4;
  }
  if ( c == '+' || c == '-' ) {
    mCurString.put_char(c);
    goto ST_NUM4;
  }
  { // (e|E) の直後はかならず数字か符号
    ostringstream buf;
    buf << "exponent value expected";
    mMsgMgr.put_msg(__FILE__, __LINE__, cur_file_region(),
		    kMsgError,
		    "DOTLIB_LEX",
		    buf.str());
    return ERROR;
  }

 ST_NUM4: // [0-9]*'.'[0-9]*(e|E)(+|-)?[0-9]*を読み込んだ直後
  c = get();
  if ( isdigit(c) ) {
    mCurString.put_char(c);
    goto ST_NUM4;
  }
  unget();
  return FLOAT_NUM;

 ST_ID: // 一文字目が[a-zA-Z_]の時
  c = get();
  if ( isalnum(c) || (c == '_') ) {
    mCurString.put_char(c);
    goto ST_ID;
  }
  unget();
  return STR;

 ST_DQ: // "があったら次の"までを強制的に文字列だと思う．
  c = get();
  if ( c == '\"' ) {
    return STR;
  }
  if ( c == '\n' ) {
    ostringstream buf;
    buf << "unexpected newline in quoted string.";
    mMsgMgr.put_msg(__FILE__, __LINE__, cur_file_region(),
		    kMsgError,
		    "DOTLIB_LEX",
		    buf.str());
    return ERROR;
  }
  if ( c == EOF ) {
    ostringstream buf;
    buf << "unexpected end-of-file in quoted string.";
    mMsgMgr.put_msg(__FILE__, __LINE__, cur_file_region(),
		    kMsgError,
		    "DOTLIB_LEX",
		    buf.str());
    return ERROR;
  }
  mCurString.put_char(c);
  goto ST_DQ;

 ST_COMMENT1: // '/' を読み込んだ直後
  c = get();
  if ( c == '/' ) { // C++ スタイルのコメント
    goto ST_COMMENT2;
  }
  if ( c == '*' ) { // C スタイルのコメント
    goto ST_COMMENT3;
  }
  unget();
  return '/';

 ST_COMMENT2: // 改行まで読み飛ばす．
  c = get();
  if ( c == '\n' ) {
    nl();
    goto ST_INIT;
  }
  if ( c == EOF ) {
    return EOF;
  }
  goto ST_COMMENT2;

 ST_COMMENT3: // "/*" を読み込んだ直後
  c = get();
  if ( c == EOF ) {
    goto ST_EOF;
  }
  if ( c == '*' ) {
    goto ST_COMMENT4;
  }
  goto ST_COMMENT3;

 ST_COMMENT4: // "/* 〜 *" まで読み込んだ直後
  c = get();
  if ( c == EOF ) {
    goto ST_EOF;
  }
  if ( c == '/' ) {
    goto ST_INIT;
  }
  if ( c == '*' ) {
    goto ST_COMMENT4;
  }
  if ( c == '\n' ) {
    nl();
  }
  goto ST_COMMENT3;

 ST_EOF:
  {
    ostringstream buf;
    buf << "Unexpected end-of-file in comment block.";
    mMsgMgr.put_msg(__FILE__, __LINE__, cur_file_region(),
		    kMsgError,
		    "DOTLIB_LEX",
		    buf.str());
  }
  return ERROR;
}

// 一文字読み出す．
int
DotLibLex::get()
{
  int c = 0;

  if ( mUngetChar != 0 ) {
    // 戻された文字があったらそれを返す．
    c = mUngetChar;
    mUngetChar = 0;
  }
  else {
    for ( ; ; ) {
      c = mInput.get();
      if ( c == EOF ) {
	break;
      }
      // Windows(DOS)/Mac/UNIX の間で改行コードの扱いが異なるのでここで
      // 強制的に '\n' に書き換えてしまう．
      // Windows : '\r', '\n'
      // Mac     : '\r'
      // UNIX    : '\n'
      // なので '\r' を '\n' に書き換えてしまう．
      // ただし次に本当の '\n' が来たときには無視するために
      // mCR を true にしておく．
      if ( c == '\r' ) {
	mCR = true;
	c = '\n';
	break;
      }
      if ( c == '\n' ) {
	if ( mCR ) {
	  // 直前に '\r' を読んで '\n' を返していたので今の '\n' を
	  // 無視する．これが唯一ループを回る条件
	  mCR = false;
	  continue;
	}
	break;
      }
      // 普通の文字の時はそのまま返す．
      mCR = false;
      break;
    }
  }
  ++ mCurColumn;
  mLastChar = c;
  return c;
}

// 一文字読み戻す．
void
DotLibLex::unget()
{
  mUngetChar = mLastChar;
  -- mCurColumn;
}

// 改行を読み込んだ時の処理
void
DotLibLex::nl()
{
  ++ mCurLine;
  mCurColumn = 0;
}

END_NAMESPACE_YM_CELL
