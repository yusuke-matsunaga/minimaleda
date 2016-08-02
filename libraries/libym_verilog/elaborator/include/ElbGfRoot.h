#ifndef LIBYM_VERILOG_ELB_ELBGFROOT_H
#define LIBYM_VERILOG_ELB_ELBGFROOT_H

/// @file libym_verilog/elaborator/include/ElbGfRoot.h
/// @brief ElbGfRoot のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ElbGfRoot.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/vl/VlNamedObj.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbGfRoot ElbGfRoot.h "ElbGfRoot.h"
/// @brief GfBlock の親となるクラス
/// @note スコープとしての親ではなく，名前による検索のためのオブジェクト
//////////////////////////////////////////////////////////////////////
class ElbGfRoot :
  public VlNamedObj
{
protected:

  /// @brief コンストラクタ
  ElbGfRoot() { }
  
  /// @brief デストラクタ
  virtual
  ~ElbGfRoot() { }
  

public:
  //////////////////////////////////////////////////////////////////////
  // ElbGfRoot の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 子供のスコープを追加する．
  virtual
  void
  add(int index,
      const VlNamedObj* block) = 0;

  /// @brief 子供のスコープを取り出す．
  virtual
  const VlNamedObj*
  elem_by_index(int index) = 0;
  
};

END_NAMESPACE_YM_VERILOG

#endif // LiBYM_VERILOG_ELB_ELBGFROOT_H
