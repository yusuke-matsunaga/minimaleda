#ifndef YM_VERILOG_VPI_VPIUSERSYSTF_H
#define YM_VERILOG_VPI_VPIUSERSYSTF_H

/// @file ym_verilog/vpi/VpiUserSystf.h
/// @brief VpiUserSystf のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VpiUserSystf.h 400 2007-03-06 17:58:07Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#include <ym_verilog/vpi/VpiBase.h>


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VpiUserSystf VpiUserSystf.h <ym_verilog/vpi/VpiUserSystf.h>
/// @brief user systf を表すクラス
/// IEEE Std 1364-2001 26.6.19 Task and function call
//////////////////////////////////////////////////////////////////////
class VpiUserSystf :
  public VpiBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // VpiUserSystf に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  virtual
  string
  name() const = 0;

  /// @brief user-defined system task/function 情報を返す．
  virtual
  void
  get_systf_info(p_vpi_systf_data systf_data_p) const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_VPI_VPIUSERSYSTF_H
