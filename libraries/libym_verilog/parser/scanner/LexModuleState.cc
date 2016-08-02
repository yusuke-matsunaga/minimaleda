
/// @file libym_verilog/scanner/LexModuleState.cc
/// @brief LexModuleState の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: LexModuleState.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "LexModuleState.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// @class LexModuleState
// @ingroup VlParser
// @brief 標準の compiler directive 用状態を表すクラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LexModuleState::LexModuleState(RawLex& lex) :
  LexState(lex)
{
  resetall(FileRegion());
}
  
// @brief デストラクタ
LexModuleState::~LexModuleState()
{
}

// @brief cell_define の状態を設定する．
void
LexModuleState::set_cell_define(bool val,
				const FileRegion& file_region)
{
  mCellDefine = val;

  if ( debug() ) {
    ostringstream buf;
    buf << "celldefine is ";
    if ( val ) {
      buf << "ON";
    }
    else {
      buf << "OFF";
    }
    put_msg(__FILE__, __LINE__,
	    file_region,
	    kMsgDebug,
	    "LEX",
	    buf.str());
  }
}

// @brief cell_define の状態を取得する
// @retval true cell_define されている
// @retval false cell_define されていない
bool
LexModuleState::cell_define() const
{
  return mCellDefine;
}

// @brief net_type を設定する．
void
LexModuleState::set_default_nettype(tVpiNetType val,
				    const FileRegion& file_region)
{
  mDefaultNetType = val;

  if ( debug() ) {
    ostringstream buf;
    buf << "default_nettype is set to "
	<< nettype2str(val) << ".";
    put_msg(__FILE__, __LINE__,
	    file_region,
	    kMsgDebug,
	    "LEX",
	    buf.str());
  }
}

// @brief net_type を取得する．
tVpiNetType
LexModuleState::default_nettype() const
{
  return mDefaultNetType;
}

// @brief time_unit の設定
void
LexModuleState::set_time_unit(int unit,
			      const FileRegion& file_region)
{
  mTimeUnit = unit;

  if ( debug() ) {
    ostringstream buf;
    buf << "time_unit is set to " << unit2str(unit) << ".";
    put_msg(__FILE__, __LINE__,
	    file_region,
	    kMsgDebug,
	    "LEX",
	    buf.str());
  }
}

// @brief time_precision の設定
void
LexModuleState::set_time_precision(int precision,
				   const FileRegion& file_region)
{
  mTimePrecision = precision;

  if ( debug() ) {
    ostringstream buf;
    buf << "time_precision is set to " << unit2str(precision) << ".";
    put_msg(__FILE__, __LINE__,
	    file_region,
	    kMsgDebug,
	    "LEX",
	    buf.str());
  }
}

// @brief time_unit の取得
int
LexModuleState::time_unit() const
{
  return mTimeUnit;
}

// @brief time_precision の取得
int
LexModuleState::time_precision() const
{
  return mTimePrecision;
}

// @brief unconnected_drive の値を設定する．
void
LexModuleState::set_unconnected_drive(tVpiUnconnDrive val,
				      const FileRegion& file_region)
{
  mUnconnDrive = val;

  if ( debug() ) {
    ostringstream buf;
    buf << "unconnected_drive is set to ";
    switch ( val ) {
    case kVpiHighZ: buf << "none"; break;
    case kVpiPull0: buf << "pull0"; break;
    case kVpiPull1: buf << "pull1"; break;
    }
    put_msg(__FILE__, __LINE__,
	    file_region,
	    kMsgDebug,
	    "LEX",
	    buf.str());
  }
}

// @brief unconnected_drive の値を取得する．
tVpiUnconnDrive
LexModuleState::unconnected_drive() const
{
  return mUnconnDrive;
}

// @brief decay_time の値を設定する．
void
LexModuleState::set_default_decay_time(int val,
				       const FileRegion& file_region)
{
  mDecayTime = val;

  if ( debug() ) {
    ostringstream buf;
    buf << "default_decay_time is set to ";
    if ( val != -1 ) {
      buf << val;
    }
    else {
      buf << "\"infinite\"";
    }
    put_msg(__FILE__, __LINE__,
	    file_region,
	    kMsgDebug,
	    "LEX",
	    buf.str());
  }
}

// @brief decay_time の値を取得する．
int
LexModuleState::default_decay_time() const
{
  return mDecayTime;
}

// @brief trireg_strength の値を設定する．
void
LexModuleState::set_default_trireg_strength(int val,
					    const FileRegion& file_region)
{
  mTriregStrength = val;

  if ( debug() ) {
    ostringstream buf;
    buf << "default_trireg_strength is set to " << val << ".";
    put_msg(__FILE__, __LINE__,
	    file_region,
	    kMsgDebug,
	    "LEX",
	    buf.str());
  }
}

// @brief trireg_strength の値を取得する．
int
LexModuleState::default_trireg_strength() const
{
  return mTriregStrength;
}

// @brief delay_mode の値を設定する．
void
LexModuleState::set_delay_mode(tVpiDefDelayMode val,
			       const FileRegion& file_region)
{
  mDelayMode = val;

  if ( debug() ) {
    ostringstream buf;
    buf << "delay_mode is set to ";
    switch ( val ) {
    case kVpiDelayModeDistrib: buf << "\"distributed\""; break;
    case kVpiDelayModePath:    buf << "\"path\""; break;
    case kVpiDelayModeUnit:    buf << "\"unit\""; break;
    case kVpiDelayModeZero:    buf << "\"zero\""; break;
    case kVpiDelayModeMTM:     buf << "\"MTM\""; break;
    case kVpiDelayModeNone:    buf << "\"none\""; break;
    }
    put_msg(__FILE__, __LINE__,
	    file_region,
	    kMsgDebug,
	    "LEX",
	    buf.str());
  }
}

// @brief delay_mode の値を取得する．
tVpiDefDelayMode
LexModuleState::delay_mode() const
{
  return mDelayMode;
}

// @brief 状態を初期化する．
void
LexModuleState::resetall(const FileRegion& file_region)
{
  set_cell_define(false, file_region);
  set_default_nettype(kVpiWire, file_region);
  set_time_unit(-16, file_region);
  set_time_precision(-16, file_region);
  set_unconnected_drive(kVpiHighZ, file_region);
  set_default_decay_time(-1, file_region);
  set_default_trireg_strength(0, file_region);
  set_delay_mode(kVpiDelayModeNone, file_region);
}

const char*
LexModuleState::nettype2str(tVpiNetType nettype)
{
  switch ( nettype ) {
  case kVpiWire:    return "wire";
  case kVpiTri:     return "tri";
  case kVpiTri0:    return "tri0";
  case kVpiTri1:    return "tri1";
  case kVpiWand:    return "wand";
  case kVpiTriAnd:  return "triand";
  case kVpiWor:     return "wor";
  case kVpiTriOr:   return "trior";
  case kVpiTriReg:  return "trireg";
  case kVpiSupply0: return "supply0";
  case kVpiSupply1: return "supply1";
  case kVpiNone:    return "none";
  }
  return "-- internal error : undefined nettype --";
}

// @brief time unit を表す整数から文字列を得る．
// @param[in] unit time unit を表す整数値(2 〜 -15)
// @return time unit を表す文字列
//
// parse_unit() の逆関数
string
LexModuleState::unit2str(int unit)
{
  if ( unit > 2 || unit < -15 ) {
    return "illegal time unit";
  }

  unit += 15;
  string ans;
  switch ( unit % 3 ) {
  case 0: ans = "1"; break;
  case 1: ans = "10"; break;
  case 2: ans = "100"; break;
  }
  switch ( unit / 3 ) {
  case 5: ans += "s"; break;
  case 4: ans += "ms"; break;
  case 3: ans += "us"; break;
  case 2: ans += "ns"; break;
  case 1: ans += "ps"; break;
  case 0: ans += "fs"; break;
  }
  return ans;
}

END_NAMESPACE_YM_VERILOG
