#ifndef LIBYM_VERILOG_ELB_ELBUSERSYSTF_H
#define LIBYM_VERILOG_ELB_ELBUSERSYSTF_H

/// @file libym_verilog/elaborator/include/ElbUserSystf.h
/// @brief ElbUserSystf のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ElbUserSystf.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/vl/VlUserSystf.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbUserSystf ElbUserSystf.h "ElbUserSystf.h"
/// @brief システムタスク/関数の定義を表すクラス
/// IEEE Std 1364-2001 26.6.14 UDP
//////////////////////////////////////////////////////////////////////
class ElbUserSystf :
  public VlUserSystf
{
public:
  
  /// @brief コンストラクタ
  ElbUserSystf() { }
  
  /// @brief デストラクタ
  virtual
  ~ElbUserSystf() { }


public:
  //////////////////////////////////////////////////////////////////////
  // VlUserSystf に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  virtual
  string
  name() const;


public:
  //////////////////////////////////////////////////////////////////////
  // VlUserSystf に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  virtual
  const char*
  _name() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELB_ELBUSERSYSTF_H
