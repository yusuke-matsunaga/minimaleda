
/// @file libym_verilog/scanner/TokenList.cc
/// @brief TokenList の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TokenList.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "TokenList.h"
#include "TokenInfo.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// トークンのリストを表すクラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
TokenList::TokenList() :
  mTop(NULL),
  mEnd(NULL)
{
}

// @brief コピーコンストラクタ
// @note 実際にはコピーしない．
TokenList::TokenList(const TokenList& src) :
  mTop(NULL),
  mEnd(NULL)
{
}


// @brief デストラクタ
TokenList::~TokenList()
{
  for (TokenInfo* token = mTop; token; ) {
    TokenInfo* next = token->mLink;
    delete token;
    token = next;
  }
}

// @brief 要素を追加する (位置パラメータ)
// @param[in] pos 位置パラメータ
void
TokenList::add(int pos)
{
  add(new TokenInfo(pos));
}

// @brief 要素を追加する (トークン)
// @param[in] loc ファイル上の位置
// @param[in] id トークン番号
// @param[in] str 文字列
void
TokenList::add(const FileRegion& loc,
	       int id,
	       const char* str)
{
  add(new TokenInfo(loc, id, str));
}

// @brief add() の下請
void
TokenList::add(TokenInfo* token)
{
  if ( mEnd ) {
    mEnd->mLink = token;
  }
  else {
    mTop = token;
  }
  mEnd = token;
}
  
// @brief 先頭のトークンを取り出す．
const TokenInfo*
TokenList::top() const
{
  return mTop;
}

END_NAMESPACE_YM_VERILOG
