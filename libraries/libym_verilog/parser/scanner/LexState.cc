
/// @file libym_verilog/scanner/LexState.cc
/// @brief LexState の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: LexState.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "LexState.h"
#include "RawLex.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// Lex に付随した状態を保持する基底クラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LexState::LexState(RawLex& lex) :
  mLex(lex)
{
  mLex.add_state(this);
}
  
// @brief デストラクタ
LexState::~LexState()
{
}

// @brief 状態を初期化する．
// @note デフォルトの実装はなにもしない．
void
LexState::resetall(const FileRegion& file_region)
{
}

END_NAMESPACE_YM_VERILOG
