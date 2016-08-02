
/// @file libym_verilog/elb/VlDumper.cc
/// @brief VlDumper の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlDumper.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include "VlDumper.h"
#include "VlDumperImpl.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// VlObj の出力用クラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] s 出力ストリーム
VlDumper::VlDumper(ostream& s) :
  mImpl(new VlDumperImpl(s))
{
}
  
/// @brief デストラクタ
VlDumper::~VlDumper()
{
  delete mImpl;
}

// @brief ファイル位置情報を出力するモードにする
void
VlDumper::enable_file_loc_mode()
{
  mImpl->enable_file_loc_mode();
}

// @brief ファイル位置情報の出力を抑制するモードにする．
void
VlDumper::disable_file_loc_mode()
{
  mImpl->disable_file_loc_mode();
}

// @brief ファイル位置情報を出力するモードのとき true を返す．
bool
VlDumper::file_loc_mode() const
{
  return mImpl->file_loc_mode();
}

// @brief NULL ポインタの出力を抑止する．
void
VlDumper::enable_nullptr_suppress_mode()
{
  mImpl->enable_nullptr_suppress_mode();
}

// @brief NULL ポインタの出力を行う．
void
VlDumper::disable_nullptr_suppress_mode()
{
  mImpl->disable_nullptr_suppress_mode();
}

// @brief NULL ポインタの出力抑止モードの状態を得る．
bool
VlDumper::nullptr_suppress_mode() const
{
  return mImpl->nullptr_suppress_mode();
}

// @brief bit 展開した内容を出力する．
void
VlDumper::enable_bitexpand_mode()
{
  mImpl->enable_bitexpand_mode();
}

// @brief bit 展開しない．
void
VlDumper::disable_bitexpand_mode()
{
  mImpl->disable_bitexpand_mode();
}

// @brief bit 展開モードの時 true を返す．
bool
VlDumper::bitexpand_mode() const
{
  return mImpl->bitexpand_mode();
}

// @brief VlMgr の持っている内容を出力する
// @param[in] mgr VlMgr
void
VlDumper::operator()(const VlMgr& mgr)
{
  mImpl->put(mgr);
}

END_NAMESPACE_YM_VERILOG
