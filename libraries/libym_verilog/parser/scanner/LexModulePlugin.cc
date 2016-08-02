
/// @file libym_verilog/scanner/LexModulePlugin.cc
/// @brief LexModulePlugin の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: LexModulePlugin.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "LexModulePlugin.h"
#include "LexModuleState.h"

#include "parser.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// @class LpCellDefine
// @ingroup VlParser
// @brief `celldefine 用プラグイン
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] lex 親の Lex オブジェクト
// @param[in] name compiler directive 名
// @param[in] state 状態を保持するオブジェクト
LpCellDefine::LpCellDefine(RawLex& lex,
			   const char* name,
			   LexModuleState* state) :
  LexPlugin(lex, name),
  mState(state)
{
}

// @brief デストラクタ
LpCellDefine::~LpCellDefine()
{
}

// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
// @return エラーが起きたら false を返す．
bool
LpCellDefine::parse()
{
  if ( is_in_module() ) {
    put_msg(__FILE__, __LINE__,
	    cur_token_loc(),
	    kMsgWarning,
	    "LEX",
	    "`celldefine in module definition.");
  }

  FileRegion loc = cur_token_loc();

  if ( !expect_nl() ) {
    put_msg(__FILE__, __LINE__,
	    cur_token_loc(),
	    kMsgError,
	    "LEX",
	    "Syntax error: "
	    "expecting new-line after `celldefine.");
    return false;
  }
  
  mState->set_cell_define(true, loc);

  return true;
}


//////////////////////////////////////////////////////////////////////
// @class LpEndCellDefine
// @ingroup VlParser
// @brief `endcelldefine 用プラグイン
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] lex 親の Lex オブジェクト
// @param[in] name compiler directive 名
// @param[in] state 状態を保持するオブジェクト
LpEndCellDefine::LpEndCellDefine(RawLex& lex,
				 const char* name,
				 LexModuleState* state) :
  LexPlugin(lex, name),
  mState(state)
{
}

// @brief デストラクタ
LpEndCellDefine::~LpEndCellDefine()
{
}

// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
// @return エラーが起きたら false を返す．
bool
LpEndCellDefine::parse()
{
  if ( is_in_module() ) {
    put_msg(__FILE__, __LINE__,
	    cur_token_loc(),
	    kMsgWarning,
	    "LEX",
	    "`endcelldefine in module definition.");
  }

  FileRegion loc = cur_token_loc();

  if ( !expect_nl() ) {
    put_msg(__FILE__, __LINE__,
	    cur_token_loc(),
	    kMsgError,
	    "LEX",
	    "Syntax error: "
	    "expecting new-line after `endcelldefine.");
    return false;
  }

  mState->set_cell_define(false, loc);

  return true;
}


//////////////////////////////////////////////////////////////////////
// @class LpNetType
// @ingroup VlParser
// @brief `default_nettype 用プラグイン
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] lex 親の Lex オブジェクト
// @param[in] name compiler directive 名
// @param[in] state 状態を保持するオブジェクト
LpNetType::LpNetType(RawLex& lex,
		     const char* name,
		     LexModuleState* nettype) :
  LexPlugin(lex, name),
  mState(nettype)
{
}
  
// @brief デストラクタ
LpNetType::~LpNetType()
{
}


// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
// @return エラーが起きたら false を返す．
bool
LpNetType::parse()
{
  if ( is_in_module() ) {
    put_msg(__FILE__, __LINE__,
	    cur_token_loc(),
	    kMsgError,
	    "LEX",
	    "`default_nettype in module definition.");
    return false;
  }

  FileRegion file_region = cur_token_loc();
  
  int id = get_nospace_token();
  tVpiNetType val = kVpiNone;
  switch ( id ) {
  case WIRE:   val = kVpiWire; break;
  case TRI:    val = kVpiTri; break;
  case TRI0:   val = kVpiTri0; break;
  case TRI1:   val = kVpiTri1; break;
  case WAND:   val = kVpiWand; break;
  case TRIAND: val = kVpiTriAnd; break;
  case WOR:    val = kVpiWor; break;
  case TRIOR:  val = kVpiTriOr; break;
  case TRIREG: val = kVpiTriReg; break;
  case IDENTIFIER:
    // "none" は Verilog-HDL の予約語ではない．
    // `default_nettype の後に来たときだけ特別な意味を持つ．
    if ( strcmp(cur_string(), "none") == 0 ) {
      val = kVpiNone;
      break;
    }
    // わざと default に続く
  default:
    put_msg(__FILE__, __LINE__,
	    cur_token_loc(),
	    kMsgError,
	    "LEX",
	    "Syntax error: "
	    "expecting net-type value after `default_net_type.");
    return false;
  }

  if ( !expect_nl() ) {
    put_msg(__FILE__, __LINE__,
	    cur_token_loc(),
	    kMsgError,
	    "LEX",
	    "Syntax error: "
	    "new-line is expected.");
    return false;
  }

  mState->set_default_nettype(val, file_region);
  
  return true;
}


//////////////////////////////////////////////////////////////////////
// @class LpTimeScale
// @ingroup VlParser
// @brief `timescale のパース
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] lex 親の Lex オブジェクト
// @param[in] name compiler directive 名
// @param[in] state 状態を保持するオブジェクト
LpTimeScale::LpTimeScale(RawLex& lex,
			 const char* name,
			 LexModuleState* state) :
  LexPlugin(lex, name),
  mState(state)
{
}

// @brief デストラクタ
LpTimeScale::~LpTimeScale()
{
}

// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
// @return エラーが起きたら false を返す．
bool
LpTimeScale::parse()
{
  if ( is_in_module() ) {
    put_msg(__FILE__, __LINE__,
	    cur_token_loc(),
	    kMsgError,
	    "LEX",
	    "`timescale in module definition.");
    return false;
  }

  FileRegion file_region1 = cur_token_loc();
  int unit = -1;
  if ( !parse_unit(unit) ) {
    // エラーメッセージは parse_unit() 内でセットされている．
    return false;
  }

  if ( !expect('/') ) {
    put_msg(__FILE__, __LINE__,
	    cur_token_loc(),
	    kMsgError,
	    "LEX",
	    "Syntax error: "
	    "expecting '/' after time-unit value.");
    return false;
  }

  FileRegion file_region2 = cur_token_loc();
  int precision = -1;
  if ( !parse_unit(precision) ) {
    // エラーメッセージは parse_unit() 内でセットされている．
    return false;
  }

  if ( unit < precision ) {
    put_msg(__FILE__, __LINE__,
	    cur_token_loc(),
	    kMsgError,
	    "LEX",
	    "time precision is coarser than time unit.");
    return false;
  }

  if ( !expect_nl() ) {
    put_msg(__FILE__, __LINE__,
	    cur_token_loc(),
	    kMsgError,
	    "LEX",
	    "Syntax error: "
	    "expecting new-line after time-precision value.");
    return false;
  }

  mState->set_time_unit(unit, file_region1);
  mState->set_time_precision(precision, file_region2);

  return true;
}

// @brief `timescale 内の時間単位を読む関数
// @param[in] unit 読み込まれた値を格納する変数
// @retval true 正しいフォーマットで値が読み込まれた．
// @retval false 読み込み中にエラーが起きた．
// 値は 100s 〜 1fs で，2 〜 -15 に対応する．
bool
LpTimeScale::parse_unit(int& unit)
{
  int mag = 0;
  int uni = 0;

  // 実際にはループを回ることはないが break を使って
  // 実行制御を行うために for-loop を使っている．
  // ここでは成功した場合のみ break で抜けている．
  for ( ; ; ) {
    if ( expect(UNUM_INT) ) {
      const char* number = cur_string();
      if ( strcmp(number, "1") == 0 ) {
	mag = 0;
	break;
      }
      if ( strcmp(number, "10") == 0 ) {
	mag = 1;
	break;
      }
      if ( strcmp(number, "100") == 0 ) {
	mag = 2;
	break;
      }
    }
    put_msg(__FILE__, __LINE__,
	    cur_token_loc(),
	    kMsgError,
	    "LEX",
	    "Syntax error: "
	    "expecting 1/10/100.");
    return false;
  }

  // 上と同様の疑似ループ
  for ( ; ; ) {
    if ( expect(IDENTIFIER) ) {
      const char* unit_str = cur_string();
      if ( strcmp(unit_str, "s") == 0 ) {
	uni = 0;
	break;
      }
      if ( strcmp(unit_str, "ms") == 0 ) {
	uni = -3;
	break;
      }
      if ( strcmp(unit_str, "us") == 0 ) {
	uni = -6;
	break;
      }
      if ( strcmp(unit_str, "ns") == 0 ) {
	uni = -9;
	break;
      }
      if ( strcmp(unit_str, "ps") == 0 ) {
	uni = -12;
	break;
      }
      if ( strcmp(unit_str, "fs") == 0 ) {
	uni = -15;
	break;
      }
    }
    put_msg(__FILE__, __LINE__,
	    cur_token_loc(),
	    kMsgError,
	    "LEX",
	    "Syntax error: "
	    "expecting s/ms/us/ns/ps/fs.");
    return false;
  }

  unit = uni + mag;

  return true;
}


//////////////////////////////////////////////////////////////////////
// @class LpUnconnDrive
// @ingroup VlParser
// @brief unconnected_drive のパース用プラグイン
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] lex 親の Lex オブジェクト
// @param[in] name compiler directive 名
// @param[in] state 状態を保持するオブジェクト
LpUnconnDrive::LpUnconnDrive(RawLex& lex,
			     const char* name,
			     LexModuleState* state) :
  LexPlugin(lex, name),
  mState(state)
{
}

// @brief デストラクタ
LpUnconnDrive::~LpUnconnDrive()
{
}

// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
// @return エラーが起きたら false を返す．
bool
LpUnconnDrive::parse()
{
  if ( is_in_module() ) {
    put_msg(__FILE__, __LINE__,
	    cur_token_loc(),
	    kMsgError,
	    "LEX",
	    "`unconnected_drive in module definition.");
    return false;
  }

  FileRegion loc = cur_token_loc();
  tVpiUnconnDrive ud = kVpiHighZ;
  switch ( get_nospace_token() ) {
  case PULL0: ud = kVpiPull0; break;
  case PULL1: ud = kVpiPull1; break;
  default:
    put_msg(__FILE__, __LINE__,
	    loc,
	    kMsgError,
	    "LEX",
	    "Syntax error: "
	    "expecting \"pull0\" or \"pull1\""
	    " after `unconneccted_drive.");
    return false;
  }

  if ( !expect_nl() ) {
    put_msg(__FILE__, __LINE__,
	    cur_token_loc(),
	    kMsgError,
	    "LEX",
	    "Syntax error: "
	    "expecting new-line.");
    return false;
  }

  mState->set_unconnected_drive(ud, loc);

  return true;
}


//////////////////////////////////////////////////////////////////////
// @class LpNonconnDrive
// @ingroup VlParser
// @brief unconnected_drive のパース用プラグイン
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] lex 親の Lex オブジェクト
// @param[in] name compiler directive 名
// @param[in] state 状態を保持するオブジェクト
LpNounconnDrive::LpNounconnDrive(RawLex& lex,
				 const char* name,
				 LexModuleState* state) :
  LexPlugin(lex, name),
  mState(state)
{
}

// @brief デストラクタ
LpNounconnDrive::~LpNounconnDrive()
{
}

// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
// @return エラーが起きたら false を返す．
bool
LpNounconnDrive::parse()
{
  if ( is_in_module() ) {
    put_msg(__FILE__, __LINE__,
	    cur_token_loc(),
	    kMsgError,
	    "LEX",
	    "`nounconnected_drive in module definition.");
    return false;
  }

  FileRegion loc = cur_token_loc();

  if ( !expect_nl() ) {
    put_msg(__FILE__, __LINE__,
	    cur_token_loc(),
	    kMsgError,
	    "LEX",
	    "Syntax error: "
	    "expecting new-line after `nounconnected_drive.");
    return false;
  }

  mState->set_unconnected_drive(kVpiHighZ, loc);

  return true;
}


//////////////////////////////////////////////////////////////////////
// @class LpDecayTime
// @ingroup VlParser
// @brief `decay_time のパース用プラグイン
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] lex 親の Lex オブジェクト
// @param[in] name compiler directive 名
// @param[in] state 状態を保持するオブジェクト
LpDecayTime::LpDecayTime(RawLex& lex,
			 const char* name,
			 LexModuleState* state) :
  LexPlugin(lex, name),
  mState(state)
{
}
  
// @brief デストラクタ
LpDecayTime::~LpDecayTime()
{
}

// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
// @return エラーが起きたら false を返す．
bool
LpDecayTime::parse()
{
  if ( is_in_module() ) {
    put_msg(__FILE__, __LINE__,
	    cur_token_loc(),
	    kMsgError,
	    "LEX",
	    "`default_decay_time in module definition.");
    return false;
  }

  int val = 0;
  FileRegion loc;
  for ( ; ; ) {
    // 別にループにする必要はないけど
    // これが一番簡単
    
    int id = get_nospace_token();
    loc = cur_token_loc();
    if ( id == UNUM_INT ) {
      val = atoi(cur_string());
      break;
    }
    if ( id == IDENTIFIER ) {
      if ( strcmp(cur_string(), "infinite") == 0 ) {
	val = -1;
	break;
      }
    }
    put_msg(__FILE__, __LINE__,
	    cur_token_loc(),
	    kMsgError,
	    "LEX",
	    "Syntax error: "
	    "expecting an integer or \"infinite\""
	    " after `default_decay_time.");
    return false;
  }

  if ( !expect_nl() ) {
    put_msg(__FILE__, __LINE__,
	    cur_token_loc(),
	    kMsgError,
	    "LEX",
	    "Syntax error: "
	    "new-line is expected.");
    return false;
  }

  mState->set_default_decay_time(val, loc);
  
  return true;
}


//////////////////////////////////////////////////////////////////////
// @class LpTriregStrength
// @ingroup VlParser
// @brief `trireg_strength のパース用プラグイン
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] lex 親の Lex オブジェクト
// @param[in] name compiler directive 名
// @param[in] state 状態を保持するオブジェクト
LpTriregStrength::LpTriregStrength(RawLex& lex,
				   const char* name,
				   LexModuleState* state) :
  LexPlugin(lex, name),
  mState(state)
{
}

// @brief デストラクタ
LpTriregStrength::~LpTriregStrength()
{
}

// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
// @return エラーが起きたら false を返す．
bool
LpTriregStrength::parse()
{
  if ( is_in_module() ) {
    put_msg(__FILE__, __LINE__,
	    cur_token_loc(),
	    kMsgError,
	    "LEX",
	    "`default_trireg_strength in module definition.");
    return false;
  }

  int val;
  FileRegion loc;
  for ( ; ; ) {
    if ( expect(UNUM_INT) ) {
      val = atoi(cur_string());
      loc = cur_token_loc();
      if ( val >= 0 && val <= 250 ) {
	break;
      }
    }
    put_msg(__FILE__, __LINE__,
	    cur_token_loc(),
	    kMsgError,
	    "LEX",
	    "Syntax error: "
	    "expecting an integer between 0 and 250 "
	    "after `default_trireg_strength.");
    return false;
  }

  if ( !expect_nl() ) {
    put_msg(__FILE__, __LINE__,
	    cur_token_loc(),
	    kMsgError,
	    "LEX",
	    "Syntax error: "
	    "expecting new-line.");
    return false;
  }

  mState->set_default_trireg_strength(val, loc);

  return true;
}


//////////////////////////////////////////////////////////////////////
// @class LpDelayMode
// @ingroup VlParser
// @brief `delay_mode のパース用プラグイン
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] lex 親の Lex オブジェクト
// @param[in] name compiler directive 名
// @param[in] state 状態を保持するオブジェクト
LpDelayMode::LpDelayMode(RawLex& lex,
			 const char* name,
			 LexModuleState* state,
			 tVpiDefDelayMode mode) :
  LexPlugin(lex, name),
  mState(state),
  mMode(mode)
{
}

// @brief デストラクタ
LpDelayMode::~LpDelayMode()
{
}

// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
// @return エラーが起きたら false を返す．
bool
LpDelayMode::parse()
{
  FileRegion loc = cur_token_loc();
  
  if ( is_in_module() ) {
    put_msg(__FILE__, __LINE__,
	    loc,
	    kMsgError,
	    "LEX",
	    "`delay_mode in module definition.");
    return false;
  }

  if ( !expect_nl() ) {
    put_msg(__FILE__, __LINE__,
	    cur_token_loc(),
	    kMsgError,
	    "LEX",
	    "Syntax error: "
	    "expecting new-line.");
    return false;
  }

  mState->set_delay_mode(mMode, loc);

  return true;
}

END_NAMESPACE_YM_VERILOG
