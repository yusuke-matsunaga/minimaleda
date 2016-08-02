#ifndef YM_VERILOG_VPI_VPIFRAME_H
#define YM_VERILOG_VPI_VPIFRAME_H

/// @file ym_verilog/vpi/VpiFrame.h
/// @brief VpiFrame のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VpiFrame.h 400 2007-03-06 17:58:07Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#include <ym_verilog/vpi/VpiBase.h>
#include <ym_verilog/vpi/VpiIterator.h>


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VpiFrame VpiFrame.h <ym_verilog/vpi/VpiFrame.h>
/// @brief frame を表すクラス
/// IEEE Std 1364-2001 26.6.20 Frames
//////////////////////////////////////////////////////////////////////
class VpiFrame :
  public VpiBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // VPI 関数のための仮想関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief このクラスの型を表す整数値を返す．
  /// @return vpiFrame を返す．
  virtual
  int
  type() const;
  
  /// @brief このクラスの型を表す文字列を返す．
  /// @return "vpiFrame" を返す．
  virtual
  string
  s_type() const;
  
  /// @brief 親の task call を返す．
  virtual
  VpiTaskCall*
  parent_taskcall() const = 0;
  
  /// @brief 親の function call を返す．
  virtual
  VpiFuncCall*
  parent_funccall() const = 0;
  
  /// @brief 親の frame を返す．
  virtual
  VpiFrame*
  parent_frame() const = 0;
  
  /// @brief 現在アクティブな statement を返す．
  virtual
  VpiStmt*
  stmt() const = 0;
  
  /// @brief automatic に定義された要素のリストの反復子を返す．
  virtual
  VpiIterator<VpiObj>
  automatic_iterator() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_VPI_VPIFRAME_H
