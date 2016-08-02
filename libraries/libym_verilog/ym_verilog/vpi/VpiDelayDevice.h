#ifndef YM_VERILOG_VPI_VPIDELAYDEVICE_H
#define YM_VERILOG_VPI_VPIDELAYDEVICE_H

/// @file ym_verilog/vpi/VpiDelayDevice.h
/// @brief DelayDevice を表すクラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VpiDelayDevice.h 400 2007-03-06 17:58:07Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.

// IEEE Std 1364-2001 26.6.21 Delay terminals


#include <ym_verilog/vpi/VpiBase.h>
#include <ym_verilog/vpi/VpiIterator.h>


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VpiDelayDevice VpiDelayDevice.h <ym_verilog/vpi/VpiDelayDevice.h>
/// @brief delay device ?を表すクラス
/// IEEE Std 1364-2001 26.6.21 Delay terminals
//////////////////////////////////////////////////////////////////////
class VpiDelayDevice :
  public VpiBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // VpiDelayDeive に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief delay type を返す．
  virtual
  int
  delay_type() const = 0;
  
  /// @brief 親のモジュールを取出す
  virtual
  VpiModule*
  module() const = 0;
  
  /// @brief in term を返す．
  virtual
  VpiDelayTerm*
  in_term() const = 0;
  
  /// @brief out term を返す．
  virtual
  VpiDelayTerm*
  out_term() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class VpiDelayTerm VpiDelayDevice.h <ym_verilog/vpi/VpiDelayDevice.h>
/// @brief delay term を表すクラス
/// IEEE Std 1364-2001 26.6.21 Delay terminals
//////////////////////////////////////////////////////////////////////
class VpiDelayTerm :
  public VpiBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // VpiDelayTerm に固有の仮想関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief delay type を返す．
  virtual
  int
  delay_type() const = 0;
  
  /// @brief 親の delay divice を返す．
  virtual
  VpiDelayDevice*
  delay_device() const = 0;
  
  /// @brief driver のリストの反復子を返す．
  virtual
  VpiDriverIteratorImpl*
  driver_iterator_obj() = 0;

  /// @brief driver のリストの反復子を返す．
  virtual
  VpiDriverIterator
  driver_iterator() = 0;
  
  /// @brief load のリストの反復子を返す．
  virtual
  VpiLoadIteratorImpl*
  load_iterator_obj() = 0;
  
  /// @brief load のリストの反復子を返す．
  virtual
  VpiLoadIterator
  load_iterator() = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_VPI_VPIDELAYDEVICE_H
