#ifndef LIBYM_VERILOG_ELABORATOR_EI_EIOPERATION_H
#define LIBYM_VERILOG_ELABORATOR_EI_EIOPERATION_H

/// @file libym_verilog/elaborator/ei/EiOperation.h
/// @brief EiExpr の演算子の派生クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiOperation.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "EiExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiOperation EiOperation.h "EiOperation.h"
/// @brief 演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class EiOperation :
  public EiExprBase1
{
protected:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  EiOperation(const PtBase* pt_expr);

  /// @brief デストラクタ
  virtual
  ~EiOperation();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子の時に true を返す．
  virtual
  bool
  is_operation() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief decompile() の実装関数
  /// @param[in] pprim 親の演算子の優先順位
  virtual
  string
  decompile_impl(int ppri) const;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELABORATOR_EI_EIOPERATION_H
