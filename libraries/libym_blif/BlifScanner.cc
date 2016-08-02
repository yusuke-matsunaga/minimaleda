
/// @file libym_bnet/BlifScanner.cc
/// @brief BlibScanner の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BlifScanner.cc 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "BlifScanner.h"


BEGIN_NAMESPACE_YM_BLIF

// get_token() の動作をデバッグするときに true にする．
static
const bool debug_get_token = false;


//////////////////////////////////////////////////////////////////////
// blif 用の字句解析器
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BlifScanner::BlifScanner() :
  mInput(NULL)
{
}

// @brief デストラクタ
BlifScanner::~BlifScanner()
{
}

// @brief 入力ストリームを設定する．
void
BlifScanner::init(istream& istr,
		  const FileDesc* file_desc)
{
  mInput = &istr;
  mFileDesc = file_desc;
  mCR = false;
  mCurChar = -1;
  mCurLineNo = 1;
  mCurColumn = 0;
  mUngetToken = kTokenEOF;
}

// @brief トークンを一つ読み出す．
tToken
BlifScanner::get_token()
{
  if ( debug_get_token ) {
    cerr << "get_token()" << " --> ";
  }
  if ( mUngetToken != kTokenEOF ) {
    // トークンバッファに値がある場合にはそれを返す．
    tToken tk = mUngetToken;
    mUngetToken = kTokenEOF;
    if ( debug_get_token ) {
      cerr << "(token_buf) " << tk << endl;
    }
    return tk;
  }

  mCurString = "";
  bool StartWithDot = false;
  int start = 0;
  int c;

  // 状態遷移を goto 文で表現したもの
  // 効率はよい．

 ST_INIT:
  c = cur_char();
  start = mCurColumn;
  switch ( c ) {
  case EOF:
    if ( debug_get_token ) {
      cerr << kTokenEOF << endl;
    }
    return kTokenEOF;

  case ' ':
  case '\t':
    // ホワイトスペースは読み飛ばす．
    goto ST_INIT;
	
  case '\n':
    if ( debug_get_token ) {
      cerr << kTokenNL << endl;
    }
    // ファイル位置を1つ進める．
    {
      const FileDesc* fd = mCurLoc.file_desc();
      int line = mCurLoc.start_line();
      int column = mCurLoc.end_column();
      mCurLoc = FileRegion(fd, line, column + 1, line, column + 1);
    }
    return kTokenNL;

  case '=':
    if ( debug_get_token ) {
      cerr << kTokenEQ << endl;
    }
    mCurLoc = FileRegion(mFileDesc, mCurLineNo, start, mCurLineNo, mCurColumn);
    return kTokenEQ;

  case '.':
    StartWithDot = true;
    goto ST_STR;

  case '#':
    goto ST_SHARP;

  case '/':
    if ( peek_next() != '*' ) {
      mCurString.put_char('/');
      goto ST_STR;
    }
    // peek_next() を確定させるための空読み
    cur_char();
    // ここまでで "/*" を読んでいる．
    goto ST_CM1;

  case '\\':
    goto ST_ESC;

  default:
    mCurString.put_char(c);
    goto ST_STR;
  }

 ST_SHARP:
  c = cur_char();
  if ( c == '\n' ) {
    mCurLoc = FileRegion(mFileDesc, mCurLineNo, start, mCurLineNo, mCurColumn);
    return kTokenNL;
  }
  else if ( c == EOF ) {
    return kTokenEOF;
  }
  else {
    // 改行までは読み飛ばす．
    goto ST_SHARP;
  }

 ST_CM1:
  c = cur_char();
  if ( c == '*' ) {
    goto ST_CM2;
  }
  else if ( c == EOF ) {
    return kTokenEOF;
  }
  else {
    // '*' までは読み飛ばす．
    goto ST_CM1;
  }

 ST_CM2:
  c = cur_char();
  if ( c == '/' ) {
    // コメントは空白扱いにする．
    goto ST_INIT;
  }
  else if ( c == EOF ) {
    return kTokenEOF;
  }
  else {
    goto ST_CM1;
  }

 ST_ESC:
  c = cur_char();
  if ( c == '\n' ) {
    // エスケープされた改行は空白扱いにする．
    goto ST_INIT;
  }
  else if ( c == EOF ) {
    // これはおかしいけど無視する．
    mCurLoc = FileRegion(mFileDesc, mCurLineNo, start, mCurLineNo, mCurColumn);
    if ( StartWithDot ) {
      // 予約後の検索
      tToken token = mDic.get_token(mCurString.c_str());
      if ( token != kTokenEOF ) {
	if ( debug_get_token ) {
	  cerr << token << endl;
	}
	return token;
      }
    }
    if ( debug_get_token ) {
      cerr << kTokenSTRING << "(\'" << mCurString << "\')" << endl;
    }
    return kTokenSTRING;
  }
  else {
    // それ以外は普通の文字として扱う．
    mCurString.put_char(c);
    goto ST_STR;
  }
  
 ST_STR:
  c = peek_next();
  switch ( c ) {
  case ' ':
  case '\t':
  case '\n':
  case '=':
  case '#':
  case '\\':
  case EOF:
    // 文字列の終わり
    mCurLoc = FileRegion(mFileDesc, mCurLineNo, start, mCurLineNo, mCurColumn);
    if ( StartWithDot ) {
      // 予約後の検索
      tToken token = mDic.get_token(mCurString.c_str());
      if ( token != kTokenEOF ) {
	if ( debug_get_token ) {
	  cerr << token << endl;
	}
	return token;
      }
    }
    if ( debug_get_token ) {
      cerr << kTokenSTRING << "(\'" << mCurString << "\')" << endl;
    }
    return kTokenSTRING;

  default:
    // peek_next() を確定させるための空読み
    cur_char();
    mCurString.put_char(c);
    goto ST_STR;
  }
}

// 現在の文字を読み出す．
int
BlifScanner::cur_char()
{
  int ans;
  if ( mCurChar != -1 ) {
    ans = mCurChar;
    mCurChar = -1;
  }
  else {
    ans = read_char();
  }
  if ( ans == '\n' ) {
    ++ mCurLineNo;
    mCurColumn = 0;
  }
  else {
    ++ mCurColumn;
  }
  return ans;
}

// 実際に文字を読み出す
int
BlifScanner::read_char()
{
  for ( ; ; ) {
    char c;
    if ( !mInput->get(c) ) {
      return EOF;
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
      return '\n';
    }
    if ( c == '\n' ) {
      if ( mCR ) {
	// 直前に '\r' を読んで '\n' を返していたので今の '\n' を
	// 無視する．これが唯一ループを回る条件
	mCR = false;
	continue;
      }
      return c;
    }
    // 普通の文字の時はそのまま返す．
    mCR = false;
    return c;
  }
}

END_NAMESPACE_YM_BLIF
