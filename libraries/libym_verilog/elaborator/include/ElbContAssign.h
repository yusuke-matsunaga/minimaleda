#ifndef LIBYM_VERILOG_ELB_ELBCONTASSIGN_H
#define LIBYM_VERILOG_ELB_ELBCONTASSIGN_H

/// @file libym_verilog/elaborator/include/ElbContAssign.h
/// @brief ElbContAssign のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ElbContAssign.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/vl/VlContAssign.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbCaHead ElbContAssign.h "ElbContAssign.h"
/// @brief 継続的代入文のヘッダ
//////////////////////////////////////////////////////////////////////
class ElbCaHead
{
protected:
  
  /// @brief コンストラクタ
  ElbCaHead();
  
  /// @brief デストラクタ
  virtual
  ~ElbCaHead();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbCaHead の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のモジュールを返す．
  virtual
  const VlModule*
  module() const = 0;
  
  /// @brief 0の強さを返す．
  virtual
  tVpiStrength
  drive0() const = 0;
  
  /// @brief 1の強さを返す．
  virtual
  tVpiStrength
  drive1() const = 0;
  
  /// @brief 遅延を表す式を返す．
  virtual
  ElbDelay*
  delay() const = 0;
  
};


//////////////////////////////////////////////////////////////////////
/// @class ElbContAssign ElbContAssign.h "ElbContAssign.h"
/// @brief 継続的代入文を表すクラス
/// IEEE Std 1364-2001 26.6.24 Continuous assignment
//////////////////////////////////////////////////////////////////////
class ElbContAssign :
  public VlContAssign
{
  friend class CellContAssign;
  
protected:
  
  /// @brief コンストラクタ
  ElbContAssign();
  
  /// @brief デストラクタ
  virtual
  ~ElbContAssign();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbContAssign の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素を得る．
  const ElbContAssign*
  next() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 次の要素を指すポインタ
  ElbContAssign* mNext;
  
};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
ElbCaHead::ElbCaHead()
{
}

// @brief デストラクタ
inline
ElbCaHead::~ElbCaHead()
{
}

// @brief コンストラクタ
inline
ElbContAssign::ElbContAssign() :
  mNext(NULL)
{
}

// @brief デストラクタ
inline
ElbContAssign::~ElbContAssign()
{
}

// @brief 次の要素を得る．
inline
const ElbContAssign*
ElbContAssign::next() const
{
  return mNext;
}

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELB_ELBCONTASSIGN_H
