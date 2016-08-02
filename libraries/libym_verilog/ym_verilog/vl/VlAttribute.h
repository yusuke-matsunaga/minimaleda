#ifndef YM_VERILOG_VL_VLATTRIBUTE_H
#define YM_VERILOG_VL_VLATTRIBUTE_H

/// @file ym_verilog/vl/VlAttribute.h
/// @brief VlAttribute のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlAttribute.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/vl/VlObj.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VlAttribute VlAttribute.h <ym_verilog/vl/VlAttribute.h>
/// @brief attribute instance を表すクラス
//////////////////////////////////////////////////////////////////////
class VlAttribute :
  public VlObj
{
protected:

  /// @brief デストラクタ
  virtual
  ~VlAttribute() { }


public:
  //////////////////////////////////////////////////////////////////////
  // VlAttribute の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 属性名を返す．
  virtual
  const char*
  name() const = 0;
  
  /// @brief def_attribute なら true を返す．
  virtual
  bool
  def_attribute() const = 0;

  /// @brief 値を表す式を返す．
  virtual
  VlExpr*
  expr() const = 0;
  
};


//////////////////////////////////////////////////////////////////////
/// @class VlAttrList VlAttribute.h <ym_verilog/vl/VlAttribute.h>
/// @breif VlAttribute のリスト
//////////////////////////////////////////////////////////////////////
class VlAttrList
{
protected:

  /// @brief デストラクタ
  virtual
  ~VlAttrList() { }

  
public:
  //////////////////////////////////////////////////////////////////////
  // VlAttrList の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素数を返す．
  virtual
  ymuint32
  size() const = 0;

  /// @brief 内容を返す．
  /// @param[in] pos 位置番号 (0 <= pos < size() )
  virtual
  VlAttribute*
  elem(ymuint32 pos) const = 0;
  
};

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_VL_VLATTRIBUTE_H
