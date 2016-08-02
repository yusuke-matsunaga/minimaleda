#ifndef YM_VERILOG_VPI_VPIATTRIBUTE_H
#define YM_VERILOG_VPI_VPIATTRIBUTE_H

/// @file ym_verilog/vpi/VpiAttribute.h
/// @brief VpiAttribute のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VpiAttribute.h 400 2007-03-06 17:58:07Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#include <ym_verilog/vpi/VpiBase.h>


BEGIN_NAMESPACE_YM_VERILOG

class VpiExpr;

//////////////////////////////////////////////////////////////////////
/// @class VpiAttribute VpiAttribute.h <ym_verilog/vpi/VpiAttribute.h>
/// @brief attribute instance を表すクラス
/// IEEE Std 1364-2001 26.6.42
//////////////////////////////////////////////////////////////////////
class VpiAttribute :
  public VpiBase
{
public:
  /// @brief 親クラスのエイリアス
  typedef VpiBase Parent;


public:
  //////////////////////////////////////////////////////////////////////
  // VpiAttribute に固有の仮想関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 定義中の attribute の時 true を返す．
  virtual
  bool
  is_def_attribute() const = 0;
  
  /// @brief 名前を返す．
  virtual
  string
  name() const = 0;

  /// @brief 値を返す．(IEEE 1364-2001非対応)
  virtual
  VpiExpr*
  expr() const = 0;

  /// @brief 親のオブジェクトを返す．
  virtual
  VpiObj*
  parent() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_VPI_VPIATTRIBUTE_H
