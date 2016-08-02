#ifndef LIBYM_VERILOG_ELABORATOR_INCLUDE_ELBSCOPE_H
#define LIBYM_VERILOG_ELABORATOR_INCLUDE__ELBSCOPE_H

/// @file libym_verilog/elaborator/include/ElbScope.h
/// @brief ElbScope のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ElbScope.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/vl/VlNamedObj.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbScope ElbScope.h "ElbScope.h"
/// @brief generate block を表すクラス
//////////////////////////////////////////////////////////////////////
class ElbScope :
  public VlNamedObj
{
  friend class CellScope;
  
protected:

  /// @brief コンストラクタ
  ElbScope() { }
  
  /// @brief デストラクタ
  virtual
  ~ElbScope() { }


public:
  //////////////////////////////////////////////////////////////////////
  // ElbScope の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素を返す．
  const ElbScope*
  next() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 次の要素を指すポインタ
  ElbScope* mNext;
  
};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 次の要素を返す．
inline
const ElbScope*
ElbScope::next() const
{
  return mNext;
}

END_NAMESPACE_YM_VERILOG

#endif // LiBYM_VERILOG_ELABORATOR_INCLUDE_ELBSCOPE_H
