#ifndef YM_VERILOG_VPI_VPIPORT_H
#define YM_VERILOG_VPI_VPIPORT_H

/// @file ym_verilog/vpi/VpiPort.h
/// @brief VpiPort のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VpiPort.h 1343 2008-03-25 17:15:35Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#include <ym_verilog/vpi/VpiBase.h>
#include <ym_verilog/vpi/VpiIterator.h>


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VpiPorts VpiPort.h <ym_verilog/vpi/VpiPort.h>
/// @brief Port/PortBit に共通な基底クラス
/// IEEE Std 1364-2001 26.6.5 Ports
//////////////////////////////////////////////////////////////////////
class VpiPorts :
  public VpiBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // VpiPorts に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 遅延情報を返す．
  virtual
  void
  get_delays(p_vpi_delay delay_p) const = 0;
  
  /// @brief 遅延情報をセットする．
  virtual
  void
  put_delays(p_vpi_delay delay_p) = 0;
  
  /// @brief 入出力の区別を得る．
  virtual
  tVpiDirection
  direction() const = 0;
  
  /// @brief ビット幅を返す．
  virtual
  int
  size() const = 0;
  
  /// @brief 名前による接続を持つとき true を返す．
  virtual
  bool
  is_conn_by_name() const = 0;
  
  /// @brief 明示的に名前がついているとき true を返す．
  virtual
  bool
  is_explicit_name() const = 0;
  
  /// @brief スカラーの時 true を返す．
  virtual
  bool
  is_scalar() const = 0;
  
  /// @brief ベクターの時 true を返す．
  virtual
  bool
  is_vector() const = 0;

  /// @brief 名前を返す．
  virtual
  string
  name() const = 0;

  /// @brief 親のモジュールを取出す
  virtual
  VpiModule*
  module() const = 0;
  
  /// @brief 上位の接続先を返す．
  virtual
  VpiExpr*
  high_conn() const = 0;
  
  /// @brief 下位の接続先を返す．
  virtual
  VpiExpr*
  low_conn() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class VpiPort VpiPort.h <ym_verilog/vpi/VpiPort.h>
/// @brief ポートのベクタを表すタイプ
/// IEEE Std 1364-2001 26.6.5 Ports
//////////////////////////////////////////////////////////////////////
class VpiPort :
  public VpiPorts
{
public:
  //////////////////////////////////////////////////////////////////////
  // VpiPort に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief low_conn を接続する．
  virtual
  void
  set_low_conn(VpiExpr* low_conn,
	       tVpiDirection dir,
	       const vector<VpiExpr*>& low_conn_bits,
	       const vector<tVpiDirection>& dir_bits) = 0;

  /// @brief high_conn を接続する．
  virtual
  void
  set_high_conn(VpiExpr* high_conn,
		const vector<VpiExpr*>& high_conn_bits,
		bool conn_by_name) = 0;
  
  /// @brief ポートリストの何番目のポートかを表すインデックスを返す．
  virtual
  int
  port_index() const = 0;
  
  /// @brief 子供の PortBit のリストの反復子を得る．
  virtual
  VpiPortBitIteratorImpl*
  bit_iterator_obj() = 0;
  
  /// @brief 子供の PortBit のリストの反復子を得る．
  virtual
  VpiPortBitIterator
  bit_iterator() = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class VpiPortBit VpiPort.h <ym_verilog/vpi/VpiPort.h>
/// @brief ポートの1ビット分を表すタイプ
/// IEEE Std 1364-2001 26.6.5 Ports
//////////////////////////////////////////////////////////////////////
class VpiPortBit :
  public VpiPorts
{
public:
  //////////////////////////////////////////////////////////////////////
  // VpiPortBit に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief オフセットを返す．(VPI未対応)
  /// @note LSB が 0
  virtual
  int
  offset() const = 0;

  /// @brief 親のPortを返す．
  virtual
  VpiPort*
  parent() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_VPI_VPIPORT_H
