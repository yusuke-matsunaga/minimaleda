
/// @file libym_cell/mislib/MislibLex.cc
/// @brief MislibLex の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MislibLex.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MislibLex.h"


BEGIN_NAMESPACE_YM_CELL

#include "mislib_grammer.h"


// コンストラクタ
MislibLex::MislibLex(MsgMgr& msg_mgr) :
  mMsgMgr(msg_mgr)
{
}

// デストラクタ
MislibLex::~MislibLex()
{
}

// @brief 入力ストリームを設定する．
// @param[in] istr 入力ストリーム
// @param[in] file_desc ファイル記述子
void
MislibLex::init(istream& istr,
		const FileDesc* file_desc)
{
  mInput = &istr;
  mFileDesc = file_desc;
  mUngetChar = 0;
  mCR = false;
  mCurLine = 1;
  mCurColumn = 0;
}

// トークンを一つとってくる．
int
MislibLex::read_token()
{
  int c;

  mCurString.clear();

 state1:
  c = get();
  mFirstColumn = mCurColumn;
  if ( isalpha(c) || (c == '_') ) {
    mCurString.put_char(c);
    goto state2;
  }
  if ( isdigit(c) || (c == '-') ) {
    mCurString.put_char(c);
    goto state3;
  }
  switch (c) {
  case EOF: return EOF;
  case ' ':
  case '\t': goto state1;
  case '\n': ++ mCurLine; mCurColumn = 0; goto state1;
  case '.': goto state4;
  case ';': return SEMI;
  case '=': return EQ;
  case '(': return LP;
  case ')': return RP;
  case '!': return NOT;
  case '*': return STAR;
  case '+': return PLUS;
  case '#': goto state6;
  case '\"': goto state7;
  }
  goto error;

  // 一文字目が[a-zA-Z_]の時
 state2:
  c = get();
  if ( isalpha(c) || isdigit(c) || (c == '_') ) {
    mCurString.put_char(c);
    goto state2;
  }
  unget();

  // 予約語（？）の検査
  // 数が少ないのでナイーブな方法をとっている．
  if ( mCurString == "GATE" ) {
    return GATE;
  }
  if ( mCurString == "PIN" ) {
    return PIN;
  }
  if ( mCurString == "NONINV" ) {
    return NONINV;
  }
  if ( mCurString == "INV" ) {
    return INV;
  }
  if ( mCurString == "UNKNOWN" ) {
    return UNKNOWN;
  }
  if ( mCurString == "CONST0" ) {
    return CONST0;
  }
  if ( mCurString == "CONST1" ) {
    return CONST1;
  }
  return STR;

  // 一文字目が[0-9]の時
 state3:
  c = get();
  if ( isdigit(c) ) {
    mCurString.put_char(c);
    goto state3;
  }
  if ( c == '.' ) {
    mCurString.put_char(c);
    goto state5;
  }
  if ( c == 'E' || c == 'e' ) {
    mCurString.put_char(c);
    goto state8;
  }
  goto state_NUM;

  // 一文字目が"."の時
 state4:
  mCurString = ".";
  mCurString.put_char(c = get());
  if ( isdigit(c) ) goto state5;
  goto error;

  // [0-9]*"."を読み終えたところ
 state5:
  c = get();
  if ( isdigit(c) ) {
    mCurString.put_char(c);
    goto state5;
  }
  if ( c == 'E' || c == 'e' ) {
    mCurString.put_char(c);
    goto state8;
  }
  goto state_NUM;

  // [0-9]*"."[Ee]を読み終えたところ
 state8:
  c = get();
  if ( c == '-' ) {
    mCurString.put_char(c);
    goto state9;
  }
  if ( isdigit(c) ) {
    mCurString.put_char(c);
    goto state9;
  }
  goto state_NUM;

  // [0-9]*"."[Ee]-?を読み終えたところ
 state9:
  c = get();
  if ( isdigit(c) ) {
    mCurString.put_char(c);
    goto state9;
  }

 state_NUM:
  unget();
  return NUM;

  // '#'があったら改行までループする．
 state6:
  c = get();
  if ( c == '\n' ) {
    unget();
    goto state1;
  }
  goto state6;

  // "があったら次の"までを強制的に文字列だと思う．
 state7:
  c = get();
  if ( c == '\"' ) {
    return STR;
  }
  mCurString.put_char(c);
  goto state7;

 error:
  {
    ostringstream buf;
    buf << "Syntax error: illegal input (" << mCurString << ").";
    mMsgMgr.put_msg(__FILE__, __LINE__,
		    cur_loc(), kMsgError,
		    "MISLIB_PARSE", buf.str().c_str());
  }
  return ERROR;
}

// 一文字読み出す．
int
MislibLex::get()
{
  int c = 0;

  if ( mUngetChar != 0 ) {
    // 戻された文字があったらそれを返す．
    c = mUngetChar;
    mUngetChar = 0;
  }
  else {
    for ( ; ; ) {
      c = mInput->get();
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
MislibLex::unget()
{
  mUngetChar = mLastChar;
  -- mCurColumn;
}

// @brief 現在のトークンの位置情報を返す．
FileRegion
MislibLex::cur_loc() const
{
  return FileRegion(mFileDesc,
		    mCurLine, mFirstColumn,
		    mCurLine, mCurColumn);
}

END_NAMESPACE_YM_CELL
