#ifndef LIBYM_VERILOG_SCANNER_TOKENLIST_H
#define LIBYM_VERILOG_SCANNER_TOKENLIST_H

/// @file libym_verilog/scanner/TokenList.h
/// @brief TokenList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TokenList.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

#include "ym_verilog/verilog.h"
#include "ym_utils/FileRegion.h"


BEGIN_NAMESPACE_YM_VERILOG

class TokenInfo;

//////////////////////////////////////////////////////////////////////
/// @class TokenList TokenList.h "TokenList.h"
/// @ingroup VlParser
/// @brief トークンのリストを表すクラス
//////////////////////////////////////////////////////////////////////
class TokenList
{
public:

  /// @brief コンストラクタ
  TokenList();

  /// @brief コピーコンストラクタ
  /// @note 実際にはコピーしない．
  TokenList(const TokenList& src);

  /// @brief デストラクタ
  ~TokenList();


public:

  /// @brief 要素を追加する (位置パラメータ)
  /// @param[in] pos 位置パラメータ
  void
  add(int pos);

  /// @brief 要素を追加する (トークン)
  /// @param[in] loc ファイル上の位置
  /// @param[in] id トークン番号
  /// @param[in] str 文字列
  void
  add(const FileRegion& loc,
      int id,
      const char* str);
  
  /// @brief 先頭のトークンを取り出す．
  const TokenInfo*
  top() const;


private:

  /// @brief add() の下請
  void
  add(TokenInfo* token);
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の要素
  TokenInfo* mTop;

  // 末尾の要素
  TokenInfo* mEnd;
  
};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_SCANNER_TOKENLIST_H
