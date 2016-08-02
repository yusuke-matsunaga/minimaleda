#ifndef YM_VERILOG_VPI_VPINAMEDEVENT_H
#define YM_VERILOG_VPI_VPINAMEDEVENT_H

/// @file include/ym_verilog/vpi/VpiNamedEvent.h
/// @brief NamedEvent 宣言要素のクラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VpiNamedEvent.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#include <ym_verilog/vpi/VpiNamedObj.h>
#include <ym_verilog/vpi/VpiIterator.h>


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VpiNeArray VpiNamedEvent.h <ym_verilog/vpi/VpiNamedEvent.h>
/// @brief 名前つきイベントの配列を表すクラス
/// IEEE Std 1364-2001 26.6.11 Named event
//////////////////////////////////////////////////////////////////////
class VpiNeArray :
  public VpiNamedObj
{
public:
  //////////////////////////////////////////////////////////////////////
  // VpiNeArray に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 配列要素を生成する．
  virtual
  void
  instantiate_elem(VpiFactory* factory) = 0;

  /// @brief 配列の次元数を返す．
  virtual
  ymuint32
  array_dim() const = 0;
  
  /// @brief automatic に定義されたとき true を返す．
  virtual
  bool
  is_automatic() const = 0;
  
  /// @brief valid の時 true を返す．
  virtual
  bool
  is_valid() const = 0;
  
  /// @brief この要素を含む frame を返す．
  virtual
  VpiFrame*
  frame() const = 0;

  /// @brief 範囲のリストの反復子を返す．
  virtual
  VpiRangeIteratorImpl*
  range_iterator_obj() = 0;

  /// @brief 範囲のリストの反復子を返す．
  virtual
  VpiRangeIterator
  range_iterator() = 0;

  /// @brief 要素のリストの反復子を返す．
  virtual
  VpiNamedEventIteratorImpl*
  named_event_iterator_obj() = 0;

  /// @brief 要素のリストの反復子を返す．
  virtual
  VpiNamedEventIterator
  named_event_iterator() = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class VpiNamedEvent VpiNamedEvent.h <ym_verilog/vpi/VpiNamedEvent.h>
/// @brief 名前つきイベントの基底クラス
/// IEEE Std 1364-2001 26.6.11 Named event
//////////////////////////////////////////////////////////////////////
class VpiNamedEvent :
  public VpiNamedObj
{
public:
  //////////////////////////////////////////////////////////////////////
  // VpiNamedEvent に固有の仮想関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief index_list が定数の時 true を返す．
  virtual
  bool
  is_constant_select() const = 0;

  /// @brief 配列要素の時 true を返す．
  virtual
  bool
  is_array() const = 0;
  
  /// @brief 多次元配列の要素の時 true を返す．
  virtual
  bool
  is_multi_array() const = 0;

  /// @brief automatic に定義されたとき true を返す．
  virtual
  bool
  is_automatic() const = 0;
  
  /// @brief valid の時 true を返す．
  virtual
  bool
  is_valid() const = 0;
  
  /// @brief この要素を含む frame を返す．
  virtual
  VpiFrame*
  frame() const = 0;

  /// @brief 配列の要素の場合に親の配列を返す．
  virtual
  VpiNeArray*
  parent() const = 0;

  /// @brief 配列要素の場合にインデックスリストの反復子を返す．
  virtual
  VpiIndexIteratorImpl*
  index_iterator_obj() = 0;

  /// @brief 配列要素の場合にインデックスリストの反復子を返す．
  virtual
  VpiIndexIterator
  index_iterator() = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_VPI_VPINAMEDEVENT_H
