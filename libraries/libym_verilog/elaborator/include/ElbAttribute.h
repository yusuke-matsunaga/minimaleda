#ifndef LIBYM_VERILOG_ELB_ELBATTRIBUTE_H
#define LIBYM_VERILOG_ELB_ELBATTRIBUTE_H

/// @file libym_verilog/elaborator/include/ElbAttribute.h
/// @brief ElbAttribute のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ElbAttribute.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/vl/VlAttribute.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbAttribute ElbAttribute.h "ElbAttribute.h"
/// @brief VlAttribute の実装クラス
//////////////////////////////////////////////////////////////////////
class ElbAttribute :
  public VlAttribute
{
protected:
  
  /// @brief コンストラクタ
  ElbAttribute() { }
  
  /// @brief デストラクタ
  virtual
  ~ElbAttribute() { }
  
};


//////////////////////////////////////////////////////////////////////
/// @class ElbAttrList ElbAttribute.h "ElbAttribute.h"
/// @brief ElbAttribute のリスト(配列)
//////////////////////////////////////////////////////////////////////
class ElbAttrList :
  public VlAttrList
{
protected:

  /// @brief コンストラクタ
  ElbAttrList() { }

  /// @brief デストラクタ
  ~ElbAttrList() { }


public:
  //////////////////////////////////////////////////////////////////////
  // ElbAttrList の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素を設定する．
  /// @param[in] pos 位置番号
  /// @param[in] pt_as パース木の定義要素
  /// @param[in] expr 値を表す式
  /// @param[in] def 定義側の属性のとき true とするフラグ
  virtual
  void
  set(ymuint32 pos,
      const PtAttrSpec* pt_as,
      ElbExpr* expr,
      bool def) = 0;
  
};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELB_ELBATTRIBUTE_H
