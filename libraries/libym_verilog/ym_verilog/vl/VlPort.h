#ifndef YM_VERILOG_VL_VLPORT_H
#define YM_VERILOG_VL_VLPORT_H

/// @file ym_verilog/vl/VlPort.h
/// @brief VlPort のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlPort.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/vl/VlObj.h"
#include "ym_verilog/vl/VlFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VlPort VlPort.h <ym_verilog/vl/VlPort.h>
/// @brief ポートを表すクラス
/// IEEE Std 1364-2001 26.6.5 Ports
//////////////////////////////////////////////////////////////////////
class VlPort :
  public VlObj
{
protected:

  /// @brief デストラクタ
  virtual
  ~VlPort() { }
  
  
public:
  //////////////////////////////////////////////////////////////////////
  // VlPort に固有の仮想関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 入出力の区別を得る．
  virtual
  tVpiDirection
  direction() const = 0;
  
  /// @brief ビット幅を返す．
  virtual
  int
  bit_size() const = 0;
  
  /// @brief 名前による接続を持つとき true を返す．
  virtual
  bool
  is_conn_by_name() const = 0;
  
  /// @brief 明示的に名前がついているとき true を返す．
  virtual
  bool
  is_explicit_name() const = 0;

  /// @brief 名前を返す．
  virtual
  const char*
  name() const = 0;

  /// @brief 親のモジュールを取出す
  virtual
  const VlModule*
  module() const = 0;
  
  /// @brief ポートリストの何番目のポートかを表すインデックスを返す．
  virtual
  ymuint32
  port_index() const = 0;
  
  /// @brief 上位の接続先を返す．
  virtual
  const VlExpr*
  high_conn() const = 0;
  
  /// @brief 下位の接続先を返す．
  virtual
  const VlExpr*
  low_conn() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_VL_VLPORT_H
