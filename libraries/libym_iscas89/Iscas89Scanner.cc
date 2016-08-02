
/// @file libym_bnet/Iscas89Scanner.cc
/// @brief Iscas89Scanner の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Iscas89Scanner.cc 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "Iscas89Scanner.h"


BEGIN_NAMESPACE_YM_ISCAS89

#include "iscas89_grammer.h"

// get_token() の動作をデバッグするときに true にする．
static
const bool debug_get_token = false;


//////////////////////////////////////////////////////////////////////
// iscas89 用の字句解析器
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Iscas89Scanner::Iscas89Scanner() :
  mInput(NULL)
{
}

// @brief デストラクタ
Iscas89Scanner::~Iscas89Scanner()
{
}

// @brief 入力ストリームを設定する．
void
Iscas89Scanner::init(istream& istr,
		     const FileDesc* file_desc)
{
  mInput = &istr;
  mFileDesc = file_desc;
  mCR = false;
  mCurChar = -1;
  mCurLineNo = 1;
  mCurColumn = 0;
}

// @brief トークンを一つ読み出す．
int
Iscas89Scanner::get_token()
{
  if ( debug_get_token ) {
    cerr << "get_token()" << " --> ";
  }

  mCurString = "";
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
      cerr << "EOF" << endl;
    }
    return EOF;

  case ' ':
  case '\t':
  case '\n':
    // ホワイトスペースは読み飛ばす．
    goto ST_INIT;

  case '#':
    goto ST_SHARP;

  case '=':
    if ( debug_get_token ) {
      cerr << "=" << endl;
    }
    return '=';

  case '(':
    if ( debug_get_token ) {
      cerr << "(" << endl;
    }
    return '(';

  case ')':
    if ( debug_get_token ) {
      cerr << ")" << endl;
    }
    return ')';

  case ',':
    if ( debug_get_token ) {
      cerr << "," << endl;
    }
    return ',';
    
  default:
    mCurString.put_char(c);
    goto ST_STR;
  }

 ST_SHARP: // 1行コメントの始まり
  c = cur_char();
  if ( c == '\n' ) {
    goto ST_INIT;
  }
  if ( c == EOF ) {
    return EOF;
  }
  // 改行までは読み飛ばす．
  goto ST_SHARP;
  
 ST_STR:
  c = peek_next();
  switch ( c ) {
  case ' ':
  case '\t':
  case '\n':
  case '#':
  case '=':
  case '(':
  case ')':
  case ',':
  case EOF:
    // 文字列の終わり
    mCurTokenLoc = FileRegion(mFileDesc,
			      mCurLineNo, start,
			      mCurLineNo, mCurColumn);
    // 予約後の検索
    if ( mCurString == "INPUT" ) {
      if ( debug_get_token ) {
	cerr << "INPUT" << endl;
      }
      return INPUT;
    }
    else if ( mCurString == "OUTPUT" ) {
      if ( debug_get_token ) {
	cerr << "OUTPUT" << endl;
      }
      return OUTPUT;
    }
    else if ( mCurString == "BUFF" ) {
      if ( debug_get_token ) {
	cerr << "BUFF" << endl;
      }
      return BUFF;
    }
    else if ( mCurString == "NOT" ) {
      if ( debug_get_token ) {
	cerr << "NOT" << endl;
      }
      return NOT;
    }
    else if ( mCurString == "INV" ) {
      if ( debug_get_token ) {
	cerr << "INV" << endl;
      }
      return NOT;
    }
    else if ( mCurString == "AND" ) {
      if ( debug_get_token ) {
	cerr << "AND" << endl;
      }
      return AND;
    }
    else if ( mCurString == "NAND" ) {
      if ( debug_get_token ) {
	cerr << "NAND" << endl;
      }
      return NAND;
    }
    else if ( mCurString == "OR" ) {
      if ( debug_get_token ) {
	cerr << "OR" << endl;
      }
      return OR;
    }
    else if ( mCurString == "NOR" ) {
      if ( debug_get_token ) {
	cerr << "NOR" << endl;
      }
      return NOR;
    }
    else if ( mCurString == "XOR" ) {
      if ( debug_get_token ) {
	cerr << "XOR" << endl;
      }
      return XOR;
    }
    else if ( mCurString == "XNOR" ) {
      if ( debug_get_token ) {
	cerr << "XNOR" << endl;
      }
      return XNOR;
    }
    else if ( mCurString == "DFF" ) {
      if ( debug_get_token ) {
	cerr << "DFF" << endl;
      }
      return DFF;
    }
    if ( debug_get_token ) {
      cerr << "NAME(\'" << mCurString << "\')" << endl;
    }
    return NAME;

  default:
    // peek_next() を確定させるための空読み
    cur_char();
    mCurString.put_char(c);
    goto ST_STR;
  }
}

// 現在の文字を読み出す．
int
Iscas89Scanner::cur_char()
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
Iscas89Scanner::read_char()
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

END_NAMESPACE_YM_ISCAS89
