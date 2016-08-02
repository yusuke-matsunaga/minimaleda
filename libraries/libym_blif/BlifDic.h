#ifndef LIBYM_BLIF_BLIFDIC_H
#define LIBYM_BLIF_BLIFDIC_H

/// @file libym_blif/BlifDic.h
/// @brief BlibDic のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BlifDic.h 958 2007-08-28 05:38:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_blif/blif_nsdef.h"


BEGIN_NAMESPACE_YM_BLIF

/// @brief blif 形式のトークン
enum tToken {
  kTokenEOF,
  kTokenNL,
  kTokenSTRING,
  kTokenEQ,
  kTokenMODEL,
  kTokenINPUTS,
  kTokenOUTPUTS,
  kTokenCLOCK,
  kTokenEND,
  kTokenNAMES,
  kTokenEXDC,
  kTokenLATCH,
  kTokenGATE,
  kTokenMLATCH,
  kTokenSUBCKT,
  kTokenSEARCH,
  kTokenSTART_KISS,
  kTokenI,
  kTokenO,
  kTokenP,
  kTokenR,
  kTokenEND_KISS,
  kTokenLATCH_ORDER,
  kTokenCODE,
  kTokenCYCLE,
  kTokenCLOCK_EVENT,
  kTokenAREA,
  kTokenDELAY,
  kTokenWIRE_LOAD_SLOPE,
  kTokenWIRE,
  kTokenINPUT_ARRIVAL,
  kTokenDEFAULT_INPUT_ARRIVAL,
  kTokenOUTPUT_REQUIRED,
  kTokenDEFAULT_OUTPUT_REQUIRED,
  kTokenINPUT_DRIVE,
  kTokenDEFAULT_INPUT_DRIVE,
  kTokenOUTPUT_LOAD,
  kTokenDEFAULT_OUTPUT_LOAD
};


//////////////////////////////////////////////////////////////////////
/// @class BlifDic BlifDic.h "BlifDic.h"
/// @brief blif 用の予約語辞書
//////////////////////////////////////////////////////////////////////
class BlifDic
{
public:

  /// @brief コンストラクタ
  BlifDic();

  /// @brief デストラクタ
  ~BlifDic();


public:

  /// @brief str に対応したトークンを返す．
  tToken
  get_token(const char* str);

  /// @brief トークンの内容を出力する．
  static
  void
  dump_token(ostream& s,
	     tToken token);


private:

  struct Cell {

    // 予約語の文字列
    const char* mStr;

    // 予約語のトークン
    tToken mToken;

    // ハッシュ表の次の要素を指すポインタ
    Cell* mLink;

  };
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ハッシュ表のサイズ
  size_t mTableSize;

  // ハッシュ表
  Cell** mHashTable;

  // 全セルの先頭
  Cell* mCellBlock;

};

/// @brief トークンを出力する．
ostream&
operator<<(ostream& s,
	   tToken token);


END_NAMESPACE_YM_BLIF

#endif // LIBYM_BLIF_BLIFDIC_H
