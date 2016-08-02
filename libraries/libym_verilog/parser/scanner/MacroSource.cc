
/// @file libym_verilog/scanner/MacroSource.cc
/// @brief MacroSource の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MacroSource.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MacroSource.h"
#include "TokenList.h"
#include "TokenInfo.h"

#include "print_token.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// define macro の入力切り替えのためのクラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MacroSource::MacroSource() :
  mParamArray(NULL)
{
}

// @brief デストラクタ
MacroSource::~MacroSource()
{
  delete [] mParamArray;
}

// @brief 内容をセットする．
// @param[in] name マクロ名
// @param[in] top トークンリストの先頭
// @param[in] param_list パラメータリスト
void
MacroSource::set(const char* name,
		 const TokenInfo* top,
		 TokenList* param_array)
{
  mName = name;
  mNextToken1 = top;
  mNextToken2 = NULL;
  mParamArray = param_array;
}

// @brief 内容を取り出す
// @return トークンの内容
const TokenInfo*
MacroSource::get()
{
  const TokenInfo* ans = mNextToken2;
  if ( ans ) {
    mNextToken2 = ans->next();
    return ans;
  }
  ans = mNextToken1;
  if ( ans ) {
    mNextToken1 = ans->next();
    int pos = ans->pos();
    if ( pos >= 0 ) {
      // 置き換えパラメータだった
      ans = mParamArray[pos].top();
      mNextToken2 = ans->next();
    }
  }
  return ans;
}

END_NAMESPACE_YM_VERILOG
