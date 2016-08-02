
/// @file libym_sat/DimacsParser.cc
/// @brief DimacsParser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DimacsParser.cc 750 2007-06-30 09:09:58Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_sat/DimacsParser.h"


BEGIN_NAMESPACE_YM_SAT

BEGIN_NONAMESPACE
const bool debug_read_token = false;
END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// DimacsHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DimacsHandler::DimacsHandler() :
  mParser(NULL)
{
}

// @brief デストラクタ
DimacsHandler::~DimacsHandler()
{
}

// パーサーを得る．
DimacsParser*
DimacsHandler::parser() const
{
  return mParser;
}

// @brief 初期化
bool
DimacsHandler::init()
{
  return true;
}

// @brief p 行の読込み
// @param[in] lineno 行番号
// @param[in] nv 変数の数
// @param[in] nc 節の数
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
DimacsHandler::read_p(int lineno,
		      size_t nv,
		      size_t nc)
{
  return true;
}

// @brief clause 行の読込み
// @param[in] lineno 行番号
// @param[in] lits リテラルの配列．最後の0は含まない
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
DimacsHandler::read_clause(int lineno,
			   const vector<int>& lits)
{
  return true;
}

// @brief 終了処理
// @param[in] loc 位置情報
bool
DimacsHandler::end()
{
  return true;
}

// @brief エラー終了時の処理
void
DimacsHandler::error_exit()
{
}


//////////////////////////////////////////////////////////////////////
// DimacsMsgHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DimacsMsgHandler::DimacsMsgHandler() :
  mParser(NULL)
{
}

// @brief デストラクタ
DimacsMsgHandler::~DimacsMsgHandler()
{
}


//////////////////////////////////////////////////////////////////////
// DimacsParser
//////////////////////////////////////////////////////////////////////

// コンストラクタ
DimacsParser::DimacsParser()
{
}

// デストラクタ
DimacsParser::~DimacsParser()
{
}

// 読み込みを行なう．
bool
DimacsParser::read(istream& in)
{
  // 読込用の内部状態
  enum {
    ST_INIT,
    ST_P1,
    ST_P2,
    ST_P3,
    ST_BODY1,
    ST_BODY2,
    ST_BODY3
  } state = ST_INIT;

  // 宣言された変数の数
  int dec_nv = 0;
  // 宣言された節の数
  int dec_nc = 0;

  // 実際に読み込んだ変数の最大値
  int max_v = 0;
  // 実際に読み込んだ節の数
  int act_nc = 0;

  vector<int> lits;

  bool stat = true;
  for (list<DimacsHandler*>::iterator p = mHandlerList.begin();
       p != mHandlerList.end(); ++ p) {
    DimacsHandler* handler = *p;
    if ( !handler->init() ) {
      stat = false;
    }
  }
  if ( !stat ) {
    goto error;
  }
  
  mLineNo = 1;
  mCR = false;
  mNL = false;
  for ( ; ; ) {
    tToken tk = read_token(in);
    if ( tk == kERR ) {
      return false;
    }
    if ( tk == kC ) {
      // コメント行なのでなにもしない．
      continue;
    }

    switch ( state ) {
    case ST_INIT:
      if ( tk == kP ) {
	state = ST_P1;
      }
      break;

    case ST_P1:
      if ( tk != kNUM ) {
	goto p_error;
      }
      dec_nv = mCurVal;
      state = ST_P2;
      break;

    case ST_P2:
      if ( tk != kNUM ) {
	goto p_error;
      }
      dec_nc = mCurVal;
      state = ST_P3;
      break;

    case ST_P3:
      if ( tk != kNL ) {
	goto p_error;
      }
      for (list<DimacsHandler*>::iterator p = mHandlerList.begin();
	   p != mHandlerList.end(); ++ p) {
	DimacsHandler* handler = *p;
	if ( !handler->read_p(mLineNo - 1, dec_nv, dec_nc) ) {
	  stat = false;
	}
      }
      if ( !stat ) {
	goto error;
      }
      state = ST_BODY1;
      break;

    case ST_BODY1:
      if ( tk == kP ) {
	add_msg(__FILE__, __LINE__, mLineNo,
		"ERR01", "duplicated 'p' lines");
	return false;
      }
      if ( tk == kEOF ) {
	goto normal_end;
      }
      if ( tk == kNL ) {
	continue;
      }
      if ( tk != kNUM ) {
	goto n_error;
      }
      
      lits.clear();
      lits.push_back(mCurVal);
      if ( mCurVal < 0 ) {
	mCurVal = - mCurVal;
      }
      if ( max_v < mCurVal ) {
	max_v = mCurVal;
      }
      state = ST_BODY2;
      break;

    case ST_BODY2:
      if ( tk == kZERO ) {
	state = ST_BODY3;
      }
      else if ( tk == kNL ) {
	continue;
      }
      else if ( tk == kNUM ) {
	lits.push_back(mCurVal);
	if ( mCurVal < 0 ) {
	  mCurVal = - mCurVal;
	}
	if ( max_v < mCurVal ) {
	  max_v = mCurVal;
	}
      }
      else {
	goto n_error;
      }
      break;

    case ST_BODY3:
      if ( tk != kNL && tk != kEOF ) {
	goto n_error;
      }
      ++ act_nc;
      for (list<DimacsHandler*>::iterator p = mHandlerList.begin();
	   p != mHandlerList.end(); ++ p) {
	DimacsHandler* handler = *p;
	if ( !handler->read_clause(mLineNo - 1, lits) ) {
	  stat = false;
	}
      }
      if ( !stat ) {
	goto error;
      }
      state = ST_BODY1;
      break;
    }
  }

  normal_end:
  if ( dec_nv == 0 ) {
    add_msg(__FILE__, __LINE__, mLineNo,
	    "ERR02", "unexpected end-of-file");
    goto error;
  }
  if ( dec_nv < max_v ) {
    add_msg(__FILE__, __LINE__, mLineNo,
	    "WRN01", "actual number of variables is more than the declared");
  }
  if ( dec_nc > act_nc ) {
    add_msg(__FILE__, __LINE__, mLineNo,
	    "WRN02", "actual number of clauses is less than the declared");
  }
  else if ( dec_nc < act_nc ) {
    add_msg(__FILE__, __LINE__, mLineNo,
	    "WRN03", "actual number of clauses is more than the declared");
  }

  for (list<DimacsHandler*>::iterator p = mHandlerList.begin();
       p != mHandlerList.end(); ++ p) {
    DimacsHandler* handler = *p;
    if ( !handler->end() ) {
	 stat = false;
    }
  }
  if ( !stat ) {
    goto error;
  }
  return true;

 p_error:
  add_msg(__FILE__, __LINE__, mLineNo,
	  "ERR03", "syntax error \"p cnf <num of vars> <num of clauses>\" expected");
  goto error;

 n_error:
  add_msg(__FILE__, __LINE__, mLineNo,
	  "ERR04", "syntax error \"<lit_1> <lit_2> ... <lit_n> 0\" expected");
  goto error;
  
 error:
  for (list<DimacsHandler*>::iterator p = mHandlerList.begin();
       p != mHandlerList.end(); ++ p) {
    DimacsHandler* handler = *p;
    handler->error_exit();
  }
  
  return false;
}

// @brief イベントハンドラの登録
void
DimacsParser::add_handler(DimacsHandler* handler)
{
  handler->mParser = this;
  mHandlerList.push_back(handler);
}

// @brief メッセージハンドラの登録
void
DimacsParser::add_msg_handler(DimacsMsgHandler* handler)
{
  handler->mParser = this;
  mMsgHandlerList.push_back(handler);
}

// @brief トークンの読込み
DimacsParser::tToken
DimacsParser::read_token(istream& in)
{
  // トークンとは空白もしくは改行で区切られたもの
  // とりあえずそれ以外の文字はすべてトークンの構成要素とみなす．
  // 改行は単独のトークンとみなす．
  // EOF も単独のトークンとみなす．

  if ( mNL ) {
    mNL = false;
    ++ mLineNo;
    if ( debug_read_token ) {
      cerr << "kNL <-- read_token()" << endl;
    }
    return kNL;
  }
  
  // 最初の空白文字をスキップする．
  int c;
  for ( ; ; ) {
    c = read_char(in);
    if ( c == EOF ) {
      if ( debug_read_token ) {
	cerr << "kEOF <-- read_token()" << endl;
      }
      return kEOF;
    }
    if ( c == '\n' ) {
      ++ mLineNo;
      if ( debug_read_token ) {
	cerr << "kNL <-- read_token()" << endl;
      }
      return kNL;
    }
    if ( c != ' ' && c != '\t' ) {
      break;
    }
  }

  if ( c == 'c' ) {
    // 手抜きで次の NL まで読み飛ばす．
    while ( (c = read_char(in)) != '\n' && c != EOF ) ;
    if ( c == '\n' ) {
      ++ mLineNo;
    }
    if ( debug_read_token ) {
      cerr << "kC <-- read_token()" << endl;
    }
    return kC;
  }
  if ( c == 'p' ) {
    c = read_char(in);
    // 次は空白でなければならない．
    if ( c != ' ' && c != '\t' ) {
      goto p_error;
    }
    // 続く空白を読み飛ばす．
    for ( ; ; ) {
      c = read_char(in);
      if ( c != ' ' && c != '\t' ) {
	break;
      }
    }
    // 次は "cnf" のはず．
    if ( c != 'c' ) {
      goto p_error;
    }
    c = read_char(in);
    if ( c != 'n' ) {
      goto p_error;
    }
    c = read_char(in);
    if ( c != 'f' ) {
      goto p_error;
    }
    c = read_char(in);
    // 次は空白のはず．
    if ( c != ' ' && c != '\t' ) {
      goto p_error;
    }
    if ( debug_read_token ) {
      cerr << "kP <-- read_token()" << endl;
    }
    return kP;

  p_error:
    add_msg(__FILE__, __LINE__, mLineNo,
	    "ERR05", "syntax error");
    return kERR;
  }
  
  bool minus_flag = false;
  if ( c == '-' ) {
    minus_flag = true;
    c = read_char(in);
  }

  int val = 0;
  for ( ; ; ) {
    if ( c < '0' || '9' < c ) {
      add_msg(__FILE__, __LINE__, mLineNo,
	      "ERR06", "syntax error");
      return kERR;
    }
    val = val * 10 + (c - '0');

    c = read_char(in);
    if ( c == ' ' || c == '\t' || c == '\n' || c == EOF ) {
      if ( c == '\n' ) {
	mNL = true;
      }
      if ( minus_flag ) {
	mCurVal = - val;
      }
      else {
	mCurVal = val;
      }
      if ( mCurVal == 0 ) {
	if ( debug_read_token ) {
	  cerr << "kZERO <-- read_token()" << endl;
	}
	return kZERO;
      }
      if ( debug_read_token ) {
	cerr << "kNUM(" << mCurVal << ") <-- read_token()" << endl;
      }
      return kNUM;
    }
  }
}

// 一文字読み出す．
int
DimacsParser::read_char(istream& in)
{
  for ( ; ; ) {
    int c = in.get();
    if ( c == std::char_traits<char>::eof() ) {
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
      return '\n';
    }
    // 普通の文字の時はそのまま返す．
    mCR = false;
    return c;
  }
}

// @brief メッセージの追加
// @param[in] src_file この関数を読んでいるソースファイル名
// @param[in] src_line この関数を読んでいるソースの行番号
// @param[in] loc ファイル位置
// @param[in] label メッセージラベル
// @param[in] body メッセージ本文
void
DimacsParser::add_msg(const char* src_file,
		      int src_line,
		      int lineno,
		      const string& label,
		      const string& body)
{
  for (list<DimacsMsgHandler*>::iterator p = mMsgHandlerList.begin();
       p != mMsgHandlerList.end(); ++ p) {
    (*(*p))(src_file, src_line, lineno, label, body);
  }
}

END_NAMESPACE_YM_SAT
