#ifndef YM_VERILOG_VPI_VPISCOPE_H
#define YM_VERILOG_VPI_VPISCOPE_H

/// @file ym_verilog/vpi/VpiScope.h
/// @brief VpiScope のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VpiScope.h 1052 2007-10-24 11:08:51Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#include <ym_verilog/vpi/VpiNamedObj.h>
#include <ym_verilog/vpi/VpiIterator.h>


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VpiScope VpiScope.h <ym_verilog/vpi/VpiScope.h>
/// @brief scope を表す基底クラス
/// IEEE Std 1364-2001 26.6.3 Scope
//////////////////////////////////////////////////////////////////////
class VpiScope :
  public VpiNamedObj
{
public:
  /// @brief 親クラスのエイリアス
  typedef VpiNamedObj Parent;


public:
  //////////////////////////////////////////////////////////////////////
  // VpiScope に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief プリミティブのリストの反復子を返す．
  virtual
  VpiPrimitiveIteratorImpl*
  primitive_iterator_obj() = 0;

  /// @brief プリミティブのリストの反復子を返す．
  virtual
  VpiPrimitiveIterator
  primitive_iterator() = 0;

  /// @brief プリミティブ配列のリストの反復子を返す．
  virtual
  VpiPrimitiveArrayIteratorImpl*
  primitive_array_iterator_obj() = 0;

  /// @brief プリミティブ配列のリストの反復子を返す．
  virtual
  VpiPrimitiveArrayIterator
  primitive_array_iterator() = 0;

  /// @brief ネットのリストの反復子を返す．
  virtual
  VpiNetIteratorImpl*
  net_iterator_obj() = 0;

  /// @brief ネットのリストの反復子を返す．
  virtual
  VpiNetIterator
  net_iterator() = 0;
  
  /// @brief ネット配列のリストの反復子を返す．
  virtual
  VpiNetArrayIteratorImpl*
  net_array_iterator_obj() = 0;
  
  /// @brief ネット配列のリストの反復子を返す．
  virtual
  VpiNetArrayIterator
  net_array_iterator() = 0;

  /// @brief REG のリストの反復子を返す．
  virtual
  VpiRegIteratorImpl*
  reg_iterator_obj() = 0;

  /// @brief REG のリストの反復子を返す．
  virtual
  VpiRegIterator
  reg_iterator() = 0;

  /// @brief REG 配列のリストの反復子を返す．
  virtual
  VpiRegArrayIteratorImpl*
  reg_array_iterator_obj() = 0;

  /// @brief REG 配列のリストの反復子を返す．
  virtual
  VpiRegArrayIterator
  reg_array_iterator() = 0;

  /// @brief variables のリストの反復子を返す．
  virtual
  VpiVariablesIteratorImpl*
  variables_iterator_obj() = 0;

  /// @brief variables のリストの反復子を返す．
  virtual
  VpiVariablesIterator
  variables_iterator() = 0;
  
  /// @brief parameter のリストの反復子を返す．
  virtual
  VpiParameterIteratorImpl*
  parameter_iterator_obj() = 0;
  
  /// @brief parameter のリストの反復子を返す．
  virtual
  VpiParameterIterator
  parameter_iterator() = 0;
  
  /// @brief 内部に含まれるスコープのリストの反復子を返す．
  virtual
  VpiInternalScopeIteratorImpl*
  internal_scope_iterator_obj() = 0;
  
  /// @brief 内部に含まれるスコープのリストの反復子を返す．
  virtual
  VpiInternalScopeIterator
  internal_scope_iterator() = 0;

  /// @brief イベントのリストの反復子を返す．
  virtual
  VpiNamedEventIteratorImpl*
  named_event_iterator_obj() = 0;

  /// @brief イベントのリストの反復子を返す．
  virtual
  VpiNamedEventIterator
  named_event_iterator() = 0;

  /// @brief イベント配列のリストの反復子を返す．
  virtual
  VpiNamedEventArrayIteratorImpl*
  named_event_array_iterator_obj() = 0;

  /// @brief イベント配列のリストの反復子を返す．
  virtual
  VpiNamedEventArrayIterator
  named_event_array_iterator() = 0;
  
  /// @brief 継続的代入文のリストの反復子を返す．
  virtual
  VpiContAssignIteratorImpl*
  cont_assign_iterator_obj() = 0;
  
  /// @brief 継続的代入文のリストの反復子を返す．
  virtual
  VpiContAssignIterator
  cont_assign_iterator() = 0;

#if 0 // VpiInserter
  /// @brief プリミティブのリストの挿入反復子を返す．
  virtual
  VpiInserter<VpiPrimitive>
  primitive_inserter() = 0;

  /// @brief プリミティブ配列のリストの挿入反復子を返す．
  virtual
  VpiInserter<VpiPrimArray>
  primitive_array_inserter() = 0;

  /// @brief ネットのリストの挿入反復子を返す．
  virtual
  VpiInserter<VpiNet>
  net_inserter() = 0;
  
  /// @brief ネット配列のリストの挿入反復子を返す．
  virtual
  VpiInserter<VpiNetArray>
  net_array_inserter() = 0;

  /// @brief REG のリストの挿入反復子を返す．
  virtual
  VpiInserter<VpiReg>
  reg_inserter() = 0;

  /// @brief REG 配列のリストの挿入反復子を返す．
  virtual
  VpiInserter<VpiRegArray>
  reg_array_inserter() = 0;

  /// @brief variables のリストの挿入反復子を返す．
  virtual
  VpiInserter<VpiVariables>
  variables_inserter() = 0;
  
  /// @brief parameter のリストの挿入反復子を返す．
  virtual
  VpiInserter<VpiParameter>
  parameter_inserter() = 0;
  
  /// @brief 内部に含まれるスコープのリストの挿入反復子を返す．
  virtual
  VpiInserter<VpiScope>
  internal_scope_inserter() = 0;

  /// @brief イベントのリストの挿入反復子を返す．
  virtual
  VpiInserter<VpiNamedEvent>
  named_event_inserter() = 0;

  /// @brief イベント配列のリストの挿入反復子を返す．
  virtual
  VpiInserter<VpiNeArray>
  named_event_array_inserter() = 0;
  
  /// @brief 継続的代入文のリストの挿入反復子を返す．
  virtual
  VpiInserter<VpiContAssign>
  cont_assign_inserter() = 0;
#endif // VpiInserter
  
  /// @brief 自分が属するモジュールを返す．
  /// @note 自分自身がモジュールの場合には自分自身を返す．
  virtual
  VpiModule*
  scope_to_module() = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_VPI_VPISCOPE_H
