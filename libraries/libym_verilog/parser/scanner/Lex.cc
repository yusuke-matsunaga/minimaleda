
/// @file libym_verilog/scanner/Lex.cc
/// @brief Lex の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Lex.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "Lex.h"
#include "LexModulePlugin.h"
#include "LexModuleState.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// Verilog 用の LEX クラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] msg_mgr メッセージマネージャ
// @param[in] fd_mgr ファイル記述子を管理するクラス
Lex::Lex(MsgMgr& msg_mgr,
	 FileDescMgr& fd_mgr) :
  RawLex(msg_mgr, fd_mgr)
{
  mModuleState = new LexModuleState(*this);
  
  // 生成されたプラグインは Lex の管理下におかれるのでここでポインタを
  // 保持しておく必要はない．
  new LpNetType(*this, "default_nettype", mModuleState);
  new LpTimeScale(*this, "timescale", mModuleState);
  new LpCellDefine(*this, "celldefine", mModuleState);
  new LpEndCellDefine(*this, "endcelldefine", mModuleState);
  new LpUnconnDrive(*this, "unconnected_drive", mModuleState);
  new LpNounconnDrive(*this, "nounconnected_drive", mModuleState);
  new LpDecayTime(*this, "default_decay_time", mModuleState);
  new LpTriregStrength(*this, "default_trireg_strength", mModuleState);
  new LpDelayMode(*this, "delay_mode_distribute", mModuleState, kVpiDelayModeDistrib);
  new LpDelayMode(*this, "delay_mode_path", mModuleState, kVpiDelayModePath);
  new LpDelayMode(*this, "delay_mode_unit", mModuleState, kVpiDelayModeUnit);
  new LpDelayMode(*this, "delay_mode_zero", mModuleState, kVpiDelayModeZero);
}

// @brief デストラクタ
Lex::~Lex()
{
}

// @brief モジュールの状態を取得する．
LexModuleState*
Lex::module_state()
{
  return mModuleState;
}

END_NAMESPACE_YM_VERILOG
