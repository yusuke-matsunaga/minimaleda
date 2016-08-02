#ifndef LIBYM_VERILOG_SCANNER_MACROSOURCE_H
#define LIBYM_VERILOG_SCANNER_MACROSOURCE_H

/// @file libym_verilog/scanner/MacroSource.h
/// @brief MacroSource のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MacroSource.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/verilog.h"
#include "ym_utils/StrBuff.h"


BEGIN_NAMESPACE_YM_VERILOG

class TokenList;
class TokenInfo;

//////////////////////////////////////////////////////////////////////
/// @class MacroSource MacroSource.h "MacroSource.h"
/// @ingroup VlParser
/// @brief define macro の入力切り替えのためのクラス
//////////////////////////////////////////////////////////////////////
class MacroSource
{
public:

  /// @brief コンストラクタ
  MacroSource();
  
  /// @brief デストラクタ
  ~MacroSource();


public:

  /// @brief 内容をセットする．
  /// @param[in] name マクロ名
  /// @param[in] top トークンリストの先頭
  /// @param[in] param_list パラメータリスト
  void
  set(const char* name,
      const TokenInfo* top,
      TokenList* param_array);
  
  /// @brief マクロ名のチェック
  /// @param[in] name チェックするマクロ名
  /// @return マクロ名が name に等しいとき true を返す．
  bool
  check_name(const char* name) const;

  /// @brief 内容を取り出す．
  const TokenInfo*
  get();

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // マクロ名
  StrBuff mName;

  // 次に読み出すトークンその1
  const TokenInfo* mNextToken1;

  // 次に読み出すトークンその2
  const TokenInfo* mNextToken2;

  // 置き換え用のトークンリストの配列
  TokenList* mParamArray;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief マクロ名のチェック
// @param[in] name 検査対象の名前
// @return マクロ名が name に等しいとき true を返す．
inline
bool
MacroSource::check_name(const char* name) const
{
  return strcmp(mName.c_str(), name) == 0;
}


END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_SCANNER_MACROSOURCE_H
