
/// @file libym_verilog/parser/scanner/InputFile.cc
/// @brief InputFile の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: InputFile.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "InputFile.h"
#include <fcntl.h>

#include "ym_utils/FileDesc.h"

#include "parser.h"


// ファイル末尾に改行がなくても warning としない時に 1
#define ALLOW_EOF_WITHOUT_NL 1


BEGIN_NAMESPACE_YM_VERILOG

BEGIN_NONAMESPACE

// @brief c が識別子の先頭で用いられる文字種の時に true を返す．
inline
bool
is_strchar1(int c)
{
  if ( isalpha(c) || c == '_' || c == '$' ) return true;
  return false;
}

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// 入力ファイルを表すためのクラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] lex 親の Lex
// @param[in] fd UNIX のファイル記述子
// @param[in] file_desc ファイル記述子
InputFile::InputFile(RawLex* lex,
		     int fd,
		     const FileDesc* file_desc) :
  mLex(lex),
  mFd(fd),
  mFileDesc(file_desc),
  mReadPos(0),
  mEndPos(0),
  mCurLine(1),
  mCurColumn(1),
  mNL(false)
{
}

// @brief デストラクタ
InputFile::~InputFile()
{
}

// @brief ファイルをクローズする．
void
InputFile::close()
{
  ::close(mFd);
  mFd = -1;
}

// @brief トークンの読み出しを行う．
// @param[out] buff 結果の文字列を格納するバッファ
// @param[out] token_loc トークンの位置情報
int
InputFile::_read_token(StrBuff& buff)
{
  buff.clear();

  int c = peek();
  if ( c == EOF ) {
    return EOF;
  }
  if ( c == '\n' || c == '\r' ) {
    accept();
    return NL;
  }
  
  _accept();
  if ( c == ' ' || c == '\t' ) {
    read_space();
    return SPACE;
  }

  switch ( mLex->context() ) {
  case RawLex::kBin:
    // 2進数モード
    return read_bin_str(c, buff);
      
  case RawLex::kOct:
    // 8進数モード
    return read_oct_str(c, buff);
    
  case RawLex::kDec:
    // 10進数モード
    return read_dec_str(c, buff);

  case RawLex::kHex:
    // 16進数モード
    return read_hex_str(c, buff);

  case RawLex::kUdp:
    switch ( c ) {
    case '-': return '-';
    case '*': return '*';
    case '0': return '0';
    case '1': return '1';
    case 'X':
    case 'x': return 'x';
    case '?': return '?';
    case 'B':
    case 'b': return 'b';
    case 'R':
    case 'r': return 'r';
    case 'F':
    case 'f': return 'f';
    case 'P':
    case 'p': return 'p';
    case 'N':
    case 'n': return 'n';
    default: break;
    }

  case RawLex::kNormal:
    break;
  }

  // ここに来るのは kUdp か kNormal
  switch ( c ) {
  case ';':
  case ':':
  case '[':
  case ']':
  case '{':
  case '}':
  case ')':
  case ',':
  case '.':
  case '@':
  case '#':
  case '%':
  case '?':
    // これらの文字は単独のトークンとなる．
    return c;
      
  case '\'':
    // 定数
    buff.put_char(c);
    c = get();
    if ( c == 's' || c == 'S' ) {
      buff.put_char(c);
      c = get();
    }
    if ( c == 'B' || c == 'b' ) {
      return BASE_B;
    }
    if ( c == 'O' || c == 'o' ) {
      return BASE_O;
    }
    if ( c == 'D' || c == 'd' ) {
      return BASE_D;
    }
    if ( c == 'H' || c == 'h' ) {
      return BASE_H;
    }
    {
      ostringstream buf;
      buf << "illegal character \'" << static_cast<char>(c) << "\',"
	  << "only B|b|O|o|D|d|H|h is allowed here.";
      put_msg(__FILE__, __LINE__,
	      cur_file_region(),
	      kMsgError,
	      "LEX",
	      buf.str());
    }
    return ERROR;
      
  case '(':
    c = peek();
    if ( c == '*' ) {
      _accept();
      return PRSTAR;
    }
    return '(';
    
  case '+':
    c = peek();
    if ( c == ':' ) {
      _accept();
      return PLUSCOLON;
    }
    return '+';
      
  case '-':
    c = peek();
    if ( c == ':' ) {
      _accept();
      return MINUSCOLON;
    }
    if ( c == '>' ) {
      _accept();
      return MINUSGT;
    }
    return '-';

  case '^':
    c = peek();
    if ( c == '~' ) {
      _accept();
      return TILDEXOR;
    }
    return '^';

  case '*':
    c = peek();
    if ( c == '>' ) {
      _accept();
      return STARGT;
    }
    if ( c == '*' ) {
      _accept();
      return STARSTAR;
    }
    if ( c == ')' ) {
      _accept();
      return STARPR;
    }
    return '*';

  case '!':
    c = peek();
    if ( c == '=' ) {
      _accept();
      c = peek();
      if ( c == '=' ) {
	_accept();
	return NOTEQEQ;
      }
      return NOTEQ;
    }
    return '!';

  case '&':
    c = peek();
    if ( c == '&' ) {
      _accept();
      c = peek();
      if ( c == '&' ) {
	_accept();
	return ANDANDAND;
      }
      return ANDAND;
    }
    return '&';

  case '|':
    c = peek();
    if ( c == '|' ) {
      _accept();
      return OROR;
    }
    return '|';

  case '~':
    c = peek();
    if ( c == '&' ) {
      _accept();
      return TILDEAND;
    }
    if ( c == '|' ) {
      _accept();
      return TILDEOR;
    }
    if ( c == '^' ) {
      _accept();
      return TILDEXOR;
    }
    return '~';

  case '<':
    c = peek();
    if ( c == '<' ) {
      _accept();
      c = peek();
      if ( c == '<' ) {
	_accept();
	return LTLTLT;
      }
      return LTLT;
    }
    if ( c == '=' ) {
      _accept();
      return LTEQ;
    }
    return '<';

  case '>':
    c = peek();
    if ( c == '>' ) {
      _accept();
      c = peek();
      if ( c == '>' ) {
	_accept();
	return GTGTGT;
      }
      return GTGT;
    }
    if ( c == '=' ) {
      _accept();
      return GTEQ;
    }
    return '>';
      
  case '=':
    c = peek();
    if ( c == '=' ) {
      _accept();
      c = peek();
      if ( c == '=' ) {
	_accept();
	return EQEQEQ;
      }
      return EQEQ;
    }
    if ( c == '>' ) {
      _accept();
      return EQGT;
    }
    return '=';

  case '\"':
    return read_dq_str(buff);

  case '\\':
    return read_esc_str(buff);
    
  case '`':
    buff.put_char(c);
    c = get();
    if ( is_strchar1(c) ) {
      buff.put_char(c);
      get_str(buff);
      return CD_SYMBOL;
    }
    else {
      ostringstream buf;
      buf << "illegal charactor \'" << static_cast<char>(c)
	  << "\' [" << c << " in digit code].";
      put_msg(__FILE__, __LINE__,
	      cur_file_region(),
	      kMsgError,
	      "LEX",
	      buf.str());
      return ERROR;
    }
      
  case '/':
    return read_comment(buff);

  default:
    break;
  }
  
  if ( is_strchar1(c) ) {
    // 通常の識別子
    buff.put_char(c);
    // 文字列要素が表れる限り mStringBuff につむ．
    get_str(buff);

    // buff の内容をチェックする．
    // 特例: 単独の $
    if ( strcmp(buff.c_str(), "$") == 0 ) {
      return '$';
    }

    // 取り合えずここでは予約語の検査はせずに IDENTIFIER としておく
    return IDENTIFIER;
  }
  
  if ( isdigit(c) ) {
    // 数字
    buff.put_char(c);
    return read_num(buff);
  }
  
  ostringstream buf;
  buf << "illegal charactor \'" << static_cast<char>(c)
      << "\' [" << c << " in digit code].";
  put_msg(__FILE__, __LINE__,
	  cur_file_region(),
	  kMsgError,
	  "LEX",
	  buf.str());
  return ERROR;
}


BEGIN_NONAMESPACE

// 2進数モードで使える文字
inline
bool
is_binchar(int c)
{
  if ( c == '0' || c == '1' ||
       c == 'x' || c == 'X' ||
       c == 'z' || c == 'Z' ||
       c == '?' ) {
    return true;
  }
  else {
    return false;
  }
}

END_NONAMESPACE

// @brief 2進数モードの読み込みを行う．
// @param[in] c 最初の文字
// @param[out] buff 結果を格納するバッファ
// @return トークンを返す．
int
InputFile::read_bin_str(int c,
			StrBuff& buff)
{
  if ( is_binchar(c) ) {
    buff.put_char(c);
    for ( ; ; ) {
      if ( mReadPos >= mEndPos ) {
	if ( fill_buff() == 0 ) {
	  break;
	}
      }
      int c = mBuff[mReadPos];
      if ( is_binchar(c) ) {
	_accept();
	buff.put_char(c);
      }
      else if ( c == '_' ) {
	_accept();
	// ただ読み飛ばす
      }
      else {
	break;
      }
    }
    return UNUMBER;
  }

  ostringstream msg_buf;
  msg_buf << "illegal charactor \'" << static_cast<char>(c) << "\',"
	  << "only \'01xXzZ?\' are allowed here.";
  put_msg(__FILE__, __LINE__,
	  cur_file_region(),
	  kMsgError,
	  "LEX",
	  msg_buf.str());
  return ERROR;
}


BEGIN_NONAMESPACE

// 8進数モードで使える文字
inline
bool
is_octchar(int c)
{
  if ( '0' <= c && c <= '7' ||
       c == 'x' || c == 'X' ||
       c == 'z' || c == 'Z' ||
       c == '?' ) {
    return true;
  }
  else {
    return false;
  }
}

END_NONAMESPACE
  
// @brief 8進数モードの読み込みを行う．
// @param[in] c 最初の文字
// @param[out] buff 結果を格納するバッファ
// @return トークンを返す．
int
InputFile::read_oct_str(int c,
			StrBuff& buff)
{
  if ( is_octchar(c) ) {
    buff.put_char(c);
    for ( ; ; ) {
      if ( mReadPos >= mEndPos ) {
	if ( fill_buff() == 0 ) {
	  break;
	}
      }
      int c = mBuff[mReadPos];
      if ( is_octchar(c) ) {
	_accept();
	buff.put_char(c);
      }
      else if ( c == '_' ) {
	_accept();
	// ただ読み飛ばす．
      }
      else {
	break;
      }
    }
    return UNUMBER;
  }
  
  ostringstream msg_buf;
  msg_buf << "illegal charactor \'" << static_cast<char>(c) << "\',"
	  << "only \'0-7xXzZ?\' are allowed here.";
  put_msg(__FILE__, __LINE__,
	  cur_file_region(),
	  kMsgError,
	  "LEX",
	  msg_buf.str());
  return ERROR;
}


BEGIN_NONAMESPACE

// 10進数モードで使える文字
inline
bool
is_decchar(int c)
{
  if ( '0' <= c && c <= '9' ) {
    return true;
  }
  else {
    return false;
  }
}

END_NONAMESPACE
  
// @brief 10進数モードの読み込みを行う．
// @param[in] c 最初の文字
// @param[out] buff 結果を格納するバッファ
// @return トークンを返す．
int
InputFile::read_dec_str(int c,
			StrBuff& buff)
{
  if ( is_decchar(c) ) {
    buff.put_char(c);
    for ( ; ; ) {
      if ( mReadPos >= mEndPos ) {
	if ( fill_buff() == 0 ) {
	  break;
	}
      }
      int c = mBuff[mReadPos];
      if ( is_decchar(c) ) {
	_accept();
	buff.put_char(c);
      }
      else if ( c == '_' ) {
	_accept();
	// ただ読み飛ばす．
      }
      else {
	break;
      }
    }
    return UNUMBER;
  }
  else if ( c == 'x' || c == 'X' ||
	    c == 'Z' || c == 'Z' ||
	    c == '?' ) {
    buff.put_char(c);
    for ( ; ; ) {
      if ( mReadPos >= mEndPos ) {
	if ( fill_buff() == 0 ) {
	  break;
	}
      }
      int c = mBuff[mReadPos];
      if ( c == '_' ) {
	_accept();
	// ただ読み飛ばす．
      }
      else {
	break;
      }
    }
    return UNUMBER;
  }
  
  ostringstream msg_buf;
  msg_buf << "illegal charactor \'" << static_cast<char>(c) << "\',"
	  << "only \'0-9xXzZ?\' are allowed here.";
  put_msg(__FILE__, __LINE__,
	  cur_file_region(),
	  kMsgError,
	  "LEX",
	  msg_buf.str());
  return ERROR;
}


BEGIN_NONAMESPACE

// 16進数モードで使える文字
inline
bool
is_hexchar(int c)
{
  if ( '0' <= c && c <= '9' ||
       'a' <= c && c <= 'f' ||
       'A' <= c && c <= 'F' ||
       c == 'x' || c == 'X' ||
       c == 'z' || c == 'Z' ||
       c == '?' ) {
    return true;
  }
  else {
    return false;
  }
}

END_NONAMESPACE
  
// @brief 16進数モードの読み込みを行う．
// @param[in] c 最初の文字
// @param[out] buff 結果を格納するバッファ
// @return トークンを返す．
int
InputFile::read_hex_str(int c,
			StrBuff& buff)
{
  if ( is_hexchar(c) ) {
    buff.put_char(c);
    for ( ; ; ) {
      if ( mReadPos >= mEndPos ) {
	if ( fill_buff() == 0 ) {
	  break;
	}
      }
      int c = mBuff[mReadPos];
      if ( is_hexchar(c) ) {
	_accept();
	buff.put_char(c);
      }
      else if ( c == '_' ) {
	_accept();
	// ただ読み飛ばす．
      }
      else {
	break;
      }
    }
    return UNUMBER;
  }
  
  ostringstream msg_buf;
  msg_buf << "illegal charactor \'" << static_cast<char>(c) << "\',"
	  << "only \'0-9a-ha-HxXzZ?\' are allowed here.";
  put_msg(__FILE__, __LINE__,
	  cur_file_region(),
	  kMsgError,
	  "LEX",
	  msg_buf.str());
  return ERROR;
}

// @brief 識別子に用いられる文字([a-zA-Z0-9_$])が続く限り読みつづける．
// @param[out] buf 結果を格納する文字列バッファ
void
InputFile::get_str(StrBuff& buff)
{
  do {
    while ( mReadPos < mEndPos ) {
      int c = mBuff[mReadPos];
      if ( isalnum(c) || c == '_' || c == '$' ) {
	_accept();
	buff.put_char(c);
      }
      else {
	// [a-zA-Z0-9_$] でなければ終わり
	return;
      }
    }
  }
  while ( fill_buff() > 0 );
}
  
// @brief 二重引用符用の読み込み
// @param[out] buf 結果を格納する文字列バッファ
// @return トークン番号を返す．
// @note 可能性のあるトークンは
//  - STRING
//  - ERROR
int
InputFile::read_dq_str(StrBuff& buff)
{
  // goto 文の嵐だが，全体の構造はいくつかの無限ループのブロック間の
  // 移動に goto 文を用いているだけ．
  // 各々の無限ループを状態ととらえて状態遷移だと思えば良い．
  
  // 8進数モードの値
  int cur_val = 0;

 INIT:
  for ( ; ; ) {
    if ( mReadPos >= mEndPos ) {
      if ( fill_buff() == 0 ) {
	// 文字列が終わらないうちに改行が来てしまった．
	goto ERR_END;
      }
    }
    int c = mBuff[mReadPos];
    if ( c == '\"' ) {
      _accept();
      // 終わり
      return STRING;
    }
    else if ( c == '\n' || c == '\r' ) {
      accept();
      // 文字列が終わらないうちに改行が来てしまった．
      goto ERR_END;
    }
    else if ( c == '\\' ) {
      _accept();
      goto BSLASH;
    }
    else {
      _accept();
      buff.put_char(c);
    }
  }

 BSLASH:
  for ( ; ; ) {
    if ( mReadPos >= mEndPos ) {
      if ( fill_buff() == 0 ) {
	goto ERR_END;
      }
    }
    int c = mBuff[mReadPos];
    if ( c == 'n' || c == 't' ) {
      _accept();
      buff.put_char('\\');
      buff.put_char(c);
      goto INIT;
    }
    else if ( c == '\n' ) {
      accept();
      // これでいいのか良くわからないけどスペースに変える．
      buff.put_char(' ');
      goto INIT;
    }
    else if ( c >= '0' && c <= '7' ) {
      _accept();
      cur_val = c - '0';
      goto BSLASH1;
    }
    else {
      _accept();
      // 上記以外の文字ならバックスラッシュの意味はない
      buff.put_char(c);
      goto INIT;
    }
  }
  
 BSLASH1:
  for ( ; ; ) {
    if ( mReadPos >= mEndPos ) {
      if ( fill_buff() == 0 ) {
	goto ERR_END;
      }
    }
    int c = mBuff[mReadPos];
    if ( c >= '0' && c <= '7' ) {
      _accept();
      cur_val = cur_val * 8 + (c - '0');
      goto BSLASH2;
    }
    else {
      buff.put_char(static_cast<char>(cur_val));
      goto INIT;
    }
  }

 BSLASH2:
  for ( ; ; ) {
    if ( mReadPos >= mEndPos ) {
      if ( fill_buff() == 0 ) {
	goto ERR_END;
      }
    }
    int c = mBuff[mReadPos];
    if ( c >= '0' && c <= '7' ) {
      _accept();
      cur_val = cur_val * 8 + (c - '0');
    }
    buff.put_char(static_cast<char>(cur_val));
    goto INIT;
  }

 ERR_END:
  // 文字列が終わらないうちに改行が来てしまった．
  put_msg(__FILE__, __LINE__,
	  cur_file_region(),
	  kMsgError,
	  "LEX",
	  "new line in quoted string.");
  return ERROR;
}

// @brief escaped identifier 用の読み込み
// @param[out] buf 結果を格納する文字列バッファ
// @param[out] buf 結果を格納する文字列バッファ
// @return トークン番号を返す．
// @note 可能性のあるトークンは
//  - SPACE
//  - IDENTIFIER
//  - ERROR
int
InputFile::read_esc_str(StrBuff& buff)
{
  int c = peek();
  if ( c == ' ' || c == '\t' || c == '\n' ) {
    accept();
    // ただの空白に置き換える．
    return SPACE;
  }
  if ( !isascii(c) ) {
    // escaped identifier でも非 ascii 文字は違反
    put_msg(__FILE__, __LINE__,
	    cur_file_region(),
	    kMsgError,
	    "LEX",
	    "non-ascii character in escaped string.");
    return ERROR;
  }
      
  // escaped identifier モード
  _accept();
  buff.put_char(c);
  for ( ; ; ) {
    if ( mReadPos >= mEndPos ) {
      if ( fill_buff() == 0 ) {
	break;
      }
    }
    int c = mBuff[mReadPos];
    if ( c == ' ' || c == '\t' || c == '\n' || c == '\r' ) {
      // 空白改行文字なら終わり
      return IDENTIFIER;
    }
    else if ( isascii(c) ) {
      _accept();
      buff.put_char(c);
    }
    else {
      // 非 ascii 文字はエラー
      // ただしその直前までを escaped identifier とする．
      // 次の read_token() でエラーとなるはず．
      break;
    }
  }
  
  return IDENTIFIER;
}
  
// @brief 数字を読み込む．
// @param[out] buf 結果を格納する文字列バッファ
// @return トークン番号を返す．
// @note 可能性のあるトークンは
//  - UNUM_INT
//  - RNUMBER
//  - ERROR
int
InputFile::read_num(StrBuff& buff)
{
  for ( ; ; ) {
    if ( mReadPos >= mEndPos ) {
      if ( fill_buff() == 0 ) {
	break;
      }
    }
    int c = mBuff[mReadPos];
    if ( c == '_' ) {
      _accept();
      // ただ読み飛ばす．
    }
    else if ( isdigit(c) ) {
      _accept();
      buff.put_char(c);
    }
    else if ( c == '.' ) {
      _accept();
      buff.put_char(c);
      goto AFTER_DOT;
    }
    else if ( c == 'e' || c == 'E' ) {
      _accept();
      buff.put_char(c);
      goto AFTER_EXP;
    }
    else {
      break;
    }
  }
  return UNUM_INT;

 AFTER_DOT:
  for ( ; ; ) {
    if ( mReadPos >= mEndPos ) {
      if ( fill_buff() == 0 ) {
	break;
      }
    }
    int c = mBuff[mReadPos];
    if ( c == '_' ) {
      _accept();
      // ただ読み飛ばす．
    }
    else if ( isdigit(c) ) {
      _accept();
      buff.put_char(c);
      goto AFTER_DOT_NUM;
    }
    else {
      break;
    }
  }
  return ERROR;

 AFTER_DOT_NUM:
  for ( ; ; ) {
    if ( mReadPos >= mEndPos ) {
      if ( fill_buff() == 0 ) {
	break;
      }
    }
    int c = mBuff[mReadPos];
    if ( c == '_' ) {
      _accept();
      // ただ読み飛ばす．
    }
    else if ( c == 'e' || c == 'E' ) {
      _accept();
      buff.put_char(c);
      goto AFTER_EXP;
    }
    else if ( isdigit(c) ) {
      _accept();
      buff.put_char(c);
    }
    else {
      break;
    }
  }
  return RNUMBER;
  
 AFTER_EXP:
  for ( ; ; ) {
    if ( mReadPos >= mEndPos ) {
      if ( fill_buff() == 0 ) {
	break;
      }
    }
    int c = mBuff[mReadPos];
    if ( c == '_' ) {
      _accept();
      // ただ読み飛ばす．
    }
    else if ( c == '+' || c == '-' ) {
      _accept();
      buff.put_char(c);
      goto AFTER_EXP_SIGN;
    }
    else if ( isdigit(c) ) {
      buff.put_char('+');
      _accept();
      buff.put_char(c);
      goto AFTER_EXP_NUM;
    }
    else {
      break;
    }
  }
  return ERROR;
  
 AFTER_EXP_SIGN:
  for ( ; ; ) {
    if ( mReadPos >= mEndPos ) {
      if ( fill_buff() == 0 ) {
	break;
      }
    }
    int c = mBuff[mReadPos];
    if ( c == '_' ) {
      _accept();
      // ただ読み飛ばす．
    }
    else if ( isdigit(c) ) {
      _accept();
      buff.put_char(c);
      goto AFTER_EXP_NUM;
    }
    else {
      break;
    }
  }
  return ERROR;
  
 AFTER_EXP_NUM:
  for ( ; ; ) {
    if ( mReadPos >= mEndPos ) {
      if ( fill_buff() == 0 ) {
	break;
      }
    }
    int c = mBuff[mReadPos];
    if ( c == '_' ) {
      _accept();
      // ただ読み飛ばす．
    }
    else if ( isdigit(c) ) {
      _accept();
      buff.put_char(c);
    }
    else {
      break;
    }
  }
  return RNUMBER;
}

// @brief 空白文字を読み飛ばす
void
InputFile::read_space()
{
  for ( ; ; ) {
    if ( mReadPos >= mEndPos ) {
      if ( fill_buff() == 0 ) {
	break;
      }
    }
    int c = mBuff[mReadPos];
    if ( c == ' ' || c == '\t' ) {
      _accept();
    }
    else {
      break;
    }
  }
}
  
// @brief '/' を読んだ直後の処理
// @param[out] buf コメントを格納する文字列バッファ
// @return トークン番号を返す．
// @note 可能性のあるトークンは
//  - COMMENT1
//  - COMMENT2
//  - '/'
//  - ERROR
int
InputFile::read_comment(StrBuff& buff)
{
  int c = peek();
  if ( c == '/' ) {
    _accept();

    buff.put_char('/');
    buff.put_char('/');
    
    // 意味的には以下のコードと等価な処理を行う．
    // for ( ; ; ) {
    //   int c = peek();
    //   accept();
    //   if ( c == '\n' ) {
    //     break;
    //   }
    // }
    for ( ; ; ) {
      if ( mReadPos >= mEndPos ) {
	if ( fill_buff() == 0 ) {
	  // 末尾に NL がなく EOF の場合の処理はこれでいいの？
	  return COMMENT1;
	}
      }
      int c = mBuff[mReadPos];
      if ( c == '\n' || c == '\r' ) {
	// 末尾の改行は buf に積まない．
	return COMMENT1;
      }
      else {
	_accept();
	buff.put_char(c);
      }
    }
  }
  else if ( c == '*' ) {
    _accept();

    buff.put_char('/');
    buff.put_char('*');
    
    // 直前の文字が '*' の時 true となるフラグ
    bool star = false;
    for ( ; ; ) {
      if ( mReadPos >= mEndPos ) {
	if ( fill_buff() == 0 ) {
	  // '*/' を読む前に EOF になってしまった．
	  put_msg(__FILE__, __LINE__,
		  cur_file_region(),
		  kMsgError,
		  "LEX",
		  "unexpected end-of-file in comment block(/*).");
	  return ERROR;
	}
      }
      int c = mBuff[mReadPos];
      if ( c == '\n' || c == '\r' ) {
	accept();
	buff.put_char('\n');
      }
      else if ( c == '*' ) {
	_accept();
	buff.put_char(c);
	star = true;
      }
      else if ( star && c == '/' ) {
	_accept();
	buff.put_char(c);
	// '*/' を読み込んだ
	return COMMENT2;
      }
      else {
	_accept();
	buff.put_char(c);
	star = false;
      }
    }
  }
  else {
    return '/';
  }
}

// @brief 次の一文字を試し読みする．
// @return 読み込んだ文字を返す．
int
InputFile::peek()
{
  if ( mReadPos >= mEndPos ) {
    if ( fill_buff() == 0 ) {
      if ( mNL ) {
	return EOF;
      }
      else {
	// ファイル末尾が NL でなければ NL を挿入する．
#if ALLOW_EOF_WITHOUT_NL == 0
	put_msg(__FILE__, __LINE__,
		cur_file_region(),
		kMsgWarning,
		"LEX",
		"Unexpected EOF, NL is assumed.");
#endif
	return '\n';
      }
    }
  }
  return mBuff[mReadPos];
}

// @brief 直前の peek() を確定させる．
void
InputFile::accept()
{
  mLastLine = mCurLine;
  mLastColumn = mCurColumn;
  switch ( mBuff[mReadPos] ) {
  case '\n':
    nl();
    ++ mReadPos;
    break;

  case '\r':
    // Windows(DOS)/Mac/UNIX の間で改行コードの扱いが異なるのでここで
    // 強制的に '\n' に書き換えてしまう．
    // Windows : '\r', '\n'
    // Mac     : '\r'
    // UNIX    : '\n'
    // なので '\r' を読んだら次の文字を調べ，'\n' なら一文字とみなす．
    nl();
    ++ mReadPos;
    if ( mReadPos >= mEndPos ) {
      fill_buff();
    }
    if ( mReadPos < mEndPos && mBuff[mReadPos] == '\n' ) {
      ++ mReadPos;
    }
    break;

  default:
    ++ mCurColumn;
    ++ mReadPos;
    break;
  }
}

END_NAMESPACE_YM_VERILOG
