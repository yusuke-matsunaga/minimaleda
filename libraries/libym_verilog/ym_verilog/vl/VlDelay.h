#ifndef YM_VERILOG_VL_VLDELAY_H
#define YM_VERILOG_VL_VLDELAY_H

/// @file ym_verilog/vl/VlDelay.h
/// @brief VlDelay のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlDelay.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/vl/VlObj.h"


BEGIN_NAMESPACE_YM_VERILOG

class VlExpr;

//////////////////////////////////////////////////////////////////////
/// @class VlDelay VlDelay.h <ym_verilog/vl/VlDelay.h>
/// @brief 遅延式を表す基底クラス
//////////////////////////////////////////////////////////////////////
class VlDelay :
  public VlObj
{
protected:

  /// @brief デストラクタ
  virtual
  ~VlDelay() { }

  
public:
  //////////////////////////////////////////////////////////////////////
  // VlDelay の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 要素数を返す．
  virtual
  ymuint32
  elem_num() const = 0;

  /// @brief 値を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < elem_num() )
  virtual
  const VlExpr*
  expr(ymuint32 pos) const = 0;
  
  /// @brief Verilog-HDL の文字列を得る．
  virtual
  string
  decompile() const = 0;
  
};

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_VL_VLDELAY_H
