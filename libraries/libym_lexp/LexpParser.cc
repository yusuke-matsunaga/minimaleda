
/// @file libym_lexp/LexpParser.cc
/// @brief LexpParser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: LexpParser.cc 700 2007-05-31 00:41:30Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "LexpParser.h"


BEGIN_NAMESPACE_YM_LEXP

// 字句解析ルール
// "(double-quote) で囲まれた文字列はいかなるコードであろうと
// string と見なす．
// また，直前に\(backslash)がついているコードも string と見なされる．
  

// @brief コンストラクタ
// @param[in] input 入力ファイルストリーム
LexpParser::LexpParser(istream* input) :
  mInput(input)
{
}

// @brief デストラクタ
LexpParser::~LexpParser()
{
}

// str からリテラル番号を得る．
tVarId
LexpParser::str_to_literal(const string& str)
{
  tVarId id = 0;
  for (string::const_iterator p = str.begin(); p != str.end(); ++p) {
    char c = *p;
    id *= 10;
    id += tVarId(c - '0');
  }
  return id;
}

// トークンを一つ読み出す．
tToken
LexpParser::get_token(tVarId& lit_id)
{
  string str;
  char c;

  // 汚いのは承知で goto 文で状態遷移を書きます．
 state0:
  if ( !mInput->get(c) ) return kTokenEND;
  switch (c) {
  case '\0': return kTokenEND;
  case '*':  // 次のケースとおなじ
  case '&':  return kTokenAND;
  case '+':  // 次のケースとおなじ
  case '|':  return kTokenOR;
  case '^':  return kTokenXOR;
  case '(':  return kTokenLP;
  case ')':  return kTokenRP;
  case '~':  // 次のケースとおなじ
  case '!':  return kTokenNOT;
  case ' ':  // 次のケースとおなじ
  case '\t': // 次のケースとおなじ
  case '\n': goto state0;
  case '0':  // 次のケースとおなじ
  case '1':  // 次のケースとおなじ
  case '2':  // 次のケースとおなじ
  case '3':  // 次のケースとおなじ
  case '4':  // 次のケースとおなじ
  case '5':  // 次のケースとおなじ
  case '6':  // 次のケースとおなじ
  case '7':  // 次のケースとおなじ
  case '8':  // 次のケースとおなじ
  case '9':  str.append(1, c); goto state1;
  case 'z':  // 次のケースと同じ
  case 'Z':  goto stateZ;
  case 'o':  // 次のケースと同じ
  case 'O':  goto stateO;
  default:
    throw SyntaxError("syntax error");
  }

 state1:
  if ( !mInput->get(c) ) {
    lit_id = str_to_literal(str);
    return kTokenNUM;
  }
  switch (c) {
  case '\0': // 次のケースとおなじ
  case '*':  // 次のケースとおなじ
  case '&':  // 次のケースとおなじ
  case '+':  // 次のケースとおなじ
  case '|':  // 次のケースとおなじ
  case '^':  // 次のケースとおなじ
  case '(':  // 次のケースとおなじ
  case ')':  // 次のケースとおなじ
  case '~':  // 次のケースとおなじ
  case '!':  // 次のケースとおなじ
  case ' ':  // 次のケースとおなじ
  case '\t': // 次のケースとおなじ
  case '\n': // 次のケースとおなじ
    mInput->putback(c);
    lit_id = str_to_literal(str);
    return kTokenNUM;
  case '0':  // 次のケースとおなじ
  case '1':  // 次のケースとおなじ
  case '2':  // 次のケースとおなじ
  case '3':  // 次のケースとおなじ
  case '4':  // 次のケースとおなじ
  case '5':  // 次のケースとおなじ
  case '6':  // 次のケースとおなじ
  case '7':  // 次のケースとおなじ
  case '8':  // 次のケースとおなじ
  case '9':  str.append(1, c); goto state1;
  default:
    throw SyntaxError("syntax error");
  }

 stateZ:
  if ( !mInput->get(c) || c != 'e' && c != 'E' ) {
    throw SyntaxError("syntax error");
  }
  if ( !mInput->get(c) || c != 'r' && c != 'R' ) {
    throw SyntaxError("syntax error");
  }
  if ( !mInput->get(c) || c != 'o' && c != 'O' ) {
    throw SyntaxError("syntax error");
  }
  return kTokenZERO;

 stateO:
  if ( !mInput->get(c) || c != 'n' && c != 'N' ) {
    throw SyntaxError("syntax error");
  }
  if ( !mInput->get(c) || c != 'e' && c != 'E' ) {
    throw SyntaxError("syntax error");
  }
  return kTokenONE;
}

// 次のトークンが AND ならそれを読み出し true を返す．
// そうでなければなにもしないで false を返す．
bool
LexpParser::get_and_token()
{
  for ( ; ; ) {
    char c;
    if ( !mInput->get(c) )       return false;
    if ( c == '*' || c == '&' ) return true;
    if ( c != ' ' && c != '\t' && c != '\n' ) {
      mInput->putback(c);
      return false;
    }
  }
}

// リテラルをとって来る．
// ただし，LP expr RP もリテラルと見なす．
LogExpr
LexpParser::get_literal()
{
  // ここに来る可能性のあるのは NUM, NOT, LP のみ
  // それ以外はエラー
  tVarId id;
  tToken token = get_token(id);

  if ( token == kTokenZERO ) {
    return LogExpr::make_zero();
  }

  if ( token == kTokenONE ) {
    return LogExpr::make_one();
  }
  
  if ( token == kTokenNUM ) {
    // id 番目の肯定のリテラルを作る．
    return LogExpr::make_posiliteral(id);
  }

  if ( token == kTokenNOT ) {
    // 次のトークンをとって来る．
    token = get_token(id);
    if ( token != kTokenNUM ) {
      // これは文字列でなければならない．
      throw SyntaxError("NUMBER is expected after NOT");
    }
    // id 番目の否定のリテラルを作る．
    return LogExpr::make_negaliteral(id);
  }

  if ( token == kTokenLP ) {
    // RP で終る論理式をとって来る．
    return get_expr(kTokenRP);
  }

  // それ以外はエラー
  throw SyntaxError("syntax error");

  // ダミー
  return LogExpr::make_zero();
}

// AND でつながった積項をとって来る．
LogExpr
LexpParser::get_product()
{
  // まず第一項めを取り出す．
  LogExpr expr = get_literal();

  for ( ; ; ) {
    // 次のトークンが AND かどうかを調べる．
    if ( ! get_and_token() ) {
      // AND 以外なら終り．
      return expr;
    }
    // 次のリテラルを取り出す．
    LogExpr expr1 = get_literal();

    // 積項を作る．
    expr &= expr1;
  }
  // ここにはこない．
}

// OR もしくは XOR でつながった積項をとって来る．
// 最後は end_token で終らなければ false を返す．
LogExpr
LexpParser::get_expr(tToken end_token)
{
  // まず第一項めを取り出す．
  LogExpr expr = get_product();

  for ( ; ; ) {
    // 次のトークンを調べる．
    tVarId dummy;
    tToken token = get_token(dummy);
    if ( token == end_token ) {
      return expr;
    }
    if ( token != kTokenOR && token != kTokenXOR ) {
      // OR か XOR で無ければならない．
      cerr << "token = " << token << endl;
      throw SyntaxError("syntax error");
    }
    // 次の積項を取り出す．
    LogExpr expr1 = get_product();

    if ( token == kTokenOR ) {
      // 和項を作る．
      expr |= expr1;
    }
    else {
      // 排他的論理和項を作る．
      expr ^= expr1;
    }
  }
  // ここにはこない．
}

// @brief トークンを出力する．主にデバッグ用
ostream&
operator<<(ostream& s,
	   tToken token)
{
  switch (token) {
  case kTokenEND:  s << "TokenEND"; break;
  case kTokenZERO: s << "TokenZERO"; break;
  case kTokenONE:  s << "TokenONE"; break;
  case kTokenNUM:  s << "TokenNUM"; break;
  case kTokenLP:   s << "TokenLP "; break;
  case kTokenRP:   s << "TokenRP "; break;
  case kTokenAND:  s << "TokenAND"; break;
  case kTokenOR:   s << "TokenOR "; break;
  case kTokenXOR:  s << "TokenXOR"; break;
  case kTokenNOT:  s << "TokenNOT"; break;
  case kTokenERR:  s << "TokenERR"; break;
  }
  return s;
}

END_NAMESPACE_YM_LEXP
