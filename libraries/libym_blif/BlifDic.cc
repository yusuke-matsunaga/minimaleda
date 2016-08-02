
/// @file libym_bnet/BlifDic.cc
/// @brief BlibDic の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BlifDic.cc 1293 2008-02-21 02:25:52Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "BlifScanner.h"


BEGIN_NAMESPACE_YM_BLIF

BEGIN_NONAMESPACE

size_t
hash_func(const char* str)
{
  size_t h = 0;
  size_t c;
  for ( ; (c = static_cast<size_t>(*str)); ++ str) {
    h = h * 37 + c;
  }
  return h;
}

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// blif 用の予約語辞書
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BlifDic::BlifDic()
{
  // 予約語テーブルの生成
  struct InitData {
    const char* mKey;
    tToken mVal;
  };
  static InitData init_data[] = {
    {"model", kTokenMODEL},
    {"inputs", kTokenINPUTS},
    {"outputs", kTokenOUTPUTS},
    {"clock", kTokenCLOCK},
    {"end", kTokenEND},
    {"names", kTokenNAMES},
    {"exdc", kTokenEXDC},
    {"latch", kTokenLATCH},
    {"gate", kTokenGATE},
    {"mlatch", kTokenMLATCH},
    {"subckt", kTokenSUBCKT},
    {"search", kTokenSEARCH},
    {"start_kiss", kTokenSTART_KISS},
    {"i", kTokenI},
    {"o", kTokenO},
    {"p", kTokenP},
    {"r", kTokenR},
    {"end_kiss", kTokenEND_KISS},
    {"latch_order", kTokenLATCH_ORDER},
    {"code", kTokenCODE},
    {"cycle", kTokenCYCLE},
    {"clock_event", kTokenCLOCK_EVENT},
    {"area", kTokenAREA},
    {"delay", kTokenDELAY},
    {"wire_load_slope", kTokenWIRE_LOAD_SLOPE},
    {"wire", kTokenWIRE},
    {"input_arrival", kTokenINPUT_ARRIVAL},
    {"default_input_arrival", kTokenDEFAULT_INPUT_ARRIVAL},
    {"output_required", kTokenOUTPUT_REQUIRED},
    {"default_output_required", kTokenDEFAULT_OUTPUT_REQUIRED},
    {"input_drive", kTokenINPUT_DRIVE},
    {"default_input_drive", kTokenDEFAULT_INPUT_DRIVE},
    {"output_load", kTokenOUTPUT_LOAD},
    {"default_output_load", kTokenDEFAULT_OUTPUT_LOAD}
  };

  size_t n = sizeof(init_data) / sizeof(Cell);
  mTableSize = n;
  mHashTable = new Cell*[mTableSize];
  for (size_t i = 0; i < n; ++ i) {
    mHashTable[i] = NULL;
  }
  mCellBlock = new Cell[n];
  for (size_t i = 0; i < n; ++ i) {
    Cell* cell = &mCellBlock[i];
    cell->mStr = init_data[i].mKey;
    cell->mToken = init_data[i].mVal;
    size_t pos = hash_func(cell->mStr) % mTableSize;
    cell->mLink = mHashTable[pos];
    mHashTable[pos] = cell;
  }
}

// @brief デストラクタ
BlifDic::~BlifDic()
{
  delete [] mHashTable;
  delete [] mCellBlock;
}

/// @brief str に対応したトークンを返す．
tToken
BlifDic::get_token(const char* str)
{
  size_t pos = hash_func(str) % mTableSize;
  for (Cell* cell = mHashTable[pos]; cell; cell = cell->mLink) {
    if ( strcmp(cell->mStr, str) == 0 ) {
      return cell->mToken;
    }
  }
  return kTokenEOF;
}

// @brief トークンの内容を出力する．
void
BlifDic::dump_token(ostream& s,
		    tToken token)
{
  switch (token) {
  case kTokenEOF    : s << "EOF"; break;
  case kTokenNL     : s << "NL"; break;
  case kTokenSTRING : s << "STRING"; break;
  case kTokenEQ     : s << "EQ"; break;
  case kTokenMODEL  : s << "MODEL"; break;
  case kTokenINPUTS : s << "INPUTS"; break;
  case kTokenOUTPUTS: s << "OUTPUTS"; break;
  case kTokenCLOCK  : s << "CLOCK"; break;
  case kTokenEND    : s << "END"; break;
  case kTokenNAMES  : s << "NAMES"; break;
  case kTokenEXDC   : s << "EXDC"; break;
  case kTokenLATCH  : s << "LATCH"; break;
  case kTokenGATE   : s << "GATE"; break;
  case kTokenMLATCH: s << "MLATCH"; break;
  case kTokenSUBCKT: s << "SUBCKT"; break;
  case kTokenSEARCH: s << "SEARCH"; break;
  case kTokenSTART_KISS: s << "START_KISS"; break;
  case kTokenI: s << "I"; break;
  case kTokenO: s << "O"; break;
  case kTokenP: s << "P"; break;
  case kTokenR: s << "R"; break;
  case kTokenEND_KISS: s << "END_KISS"; break;
  case kTokenLATCH_ORDER: s << "LATCH_ORDER"; break;
  case kTokenCODE: s << "CODE"; break;
  case kTokenCYCLE: s << "CYCLE"; break;
  case kTokenCLOCK_EVENT: s << "CLOCK_EVENT"; break;
  case kTokenAREA: s << "AREA"; break;
  case kTokenDELAY: s << "DELAY"; break;
  case kTokenWIRE_LOAD_SLOPE: s << "WIRE_LOAD_SLOPE"; break;
  case kTokenWIRE : s << "WIRE"; break;
  case kTokenINPUT_ARRIVAL: s << "INPUT_ARRIVAL"; break;
  case kTokenDEFAULT_INPUT_ARRIVAL: s << "DEFAULT_INPUT_ARRIVAL"; break;
  case kTokenOUTPUT_REQUIRED: s << "OUTPUT_REQUIRED"; break;
  case kTokenDEFAULT_OUTPUT_REQUIRED: s << "DEFAULT_OUTPUT_REQUIRED"; break;
  case kTokenINPUT_DRIVE: s << "INPUT_DRIVE"; break;
  case kTokenDEFAULT_INPUT_DRIVE: s << "DEFAULT_INPUT_DRIVE"; break;
  case kTokenOUTPUT_LOAD: s << "OUTPUT_LOAD"; break;
  case kTokenDEFAULT_OUTPUT_LOAD: s << "DEFAULT_OUTPUT_LOAD"; break;
  default: break;
  }
}

// トークンの出力関数
ostream&
operator<<(ostream& s,
	   tToken token)
{
  BlifDic::dump_token(s, token);
  return s;
}

END_NAMESPACE_YM_BLIF
