#ifndef YM_VERILOG_VPI_VPISTMT_H
#define YM_VERILOG_VPI_VPISTMT_H

/// @file ym_verilog/vpi/VpiStmt.h
/// @brief ステートメントを表すクラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VpiStmt.h 400 2007-03-06 17:58:07Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.

// IEEE Std 1364-2001 26.6.27 Process, block, statement, event statement
// IEEE Std 1364-2001 26.6.28 Assignment
// IEEE Std 1364-2001 26.6.29 Delay control
// IEEE Std 1364-2001 26.6.30 Event control
// IEEE Std 1364-2001 26.6.31 Repeat control
// IEEE Std 1364-2001 26.6.32 While, repeat, wait
// IEEE Std 1364-2001 26.6.33 For
// IEEE Std 1364-2001 26.6.34 Forever
// IEEE Std 1364-2001 26.6.35 If, if-else
// IEEE Std 1364-2001 26.6.36 Case
// IEEE Std 1364-2001 26.6.37 Assign statement, deassign, force, release
// IEEE Std 1364-2001 26.6.38 Disable

#include <ym_verilog/vpi/VpiBase.h>
#include <ym_verilog/vpi/VpiIterator.h>


BEGIN_NAMESPACE_YM_VERILOG

class VpiScope;
class VpiProcess;
class VpiExpr;

//////////////////////////////////////////////////////////////////////
/// @class VpiStmt VpiStmt.h <ym_verilog/vpi/VpiStmt.h>
/// @brief statement の基底クラス
/// IEEE Std 1364-2001 26.6.27 Process, block, statement, event statement
//////////////////////////////////////////////////////////////////////
class VpiStmt :
  public VpiBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // VpiStmt に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のスコープを返す．
  virtual
  VpiScope*
  scope() const = 0;

  /// @brief 親の process を返す．
  virtual
  VpiProcess*
  process() const = 0;

  /// @brief function 中の実行を行う．
  /// @param[in] constant_function constant_function の評価の時 true
  /// @retval NULL 通常の実行
  /// @retval scope disable 文が実行されたときの disable 対象のスコープ
  virtual
  VpiScope*
  func_exec(bool constant_function) = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class VpiCaseItem VpiStmt.h "VpiStmt.h"
/// @brief caes item を表すクラス
/// IEEE Std 1364-2001 26.6.36 Case
//////////////////////////////////////////////////////////////////////
class VpiCaseItem :
  public VpiBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // VpiCaseItem に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 条件式のリストの反復子を返す．
  virtual
  VpiExprIteratorImpl*
  expr_iterator_obj() = 0;

  /// @brief 条件式のリストの反復子を返す．
  virtual
  VpiExprIterator
  expr_iterator() = 0;
  
  /// @brief 本体のステートメントを返す．
  virtual
  VpiStmt*
  stmt() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_VPI_VPISTMT_H
