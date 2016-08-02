
/// @file libym_verilog/elb/VlNamedObj.cc
/// @brief VlNamedObj の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlNamedObj.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/vl/VlNamedObj.h"
#include "ym_verilog/vl/VlModule.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス VlNamedObj
//////////////////////////////////////////////////////////////////////

// @brief このオブジェクトの属している module を返す．
const VlModule*
VlNamedObj::parent_module() const
{
  // デフォルトは parent() に同じ質問をする．
  return parent()->parent_module();
}
  
// @brief fullname の取得
string
VlNamedObj::full_name() const
{
  string tmp;
  if ( name() ) {
    tmp = name();
  }
  else {
    tmp = "<anonymous>";
  }
  if ( parent() ) {
    return parent()->full_name() + "." + tmp;
  }
  return tmp;
}


//////////////////////////////////////////////////////////////////////
// クラス VlModule
//////////////////////////////////////////////////////////////////////

// @brief 自分の属する module を得る．
// @note このクラスでは this を返す．
const VlModule*
VlModule::parent_module() const
{
  return this;
}


END_NAMESPACE_YM_VERILOG
