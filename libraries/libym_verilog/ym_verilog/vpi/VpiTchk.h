#ifndef YM_VERILOG_VPI_VPITCHK_H
#define YM_VERILOG_VPI_VPITCHK_H

/// @file ym_verilog/vpi/VpiTchk.h
/// @brief timing check を表すクラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VpiTchk.h 400 2007-03-06 17:58:07Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.

// IEEE Std 1364-2001 26.6.17 Timing check


#include <ym_verilog/vpi/VpiBase.h>
#include <ym_verilog/vpi/VpiIterator.h>


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VpiTchk VpiTchk.h <ym_verilog/vpi/VpiTchk.h>
/// @brief timing check を表すクラス
/// IEEE Std 1364-2001 26.6.17 Timing check
//////////////////////////////////////////////////////////////////////
class VpiTchk :
  public VpiBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // VpiTchk に固有の仮想関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief tchk type を返す．
  virtual
  int
  tchk_type() const = 0;
    
  /// @brief 親のモジュールを取出す
  virtual
  VpiModule*
  module() const = 0;

  /// @brief 遅延を表す式を得る．
  virtual
  VpiExpr*
  delay() const = 0;
  
  /// @brief notifier reg を得る．
  virtual
  VpiRegs*
  tchk_notifier() const = 0;
  
  /// @brief reference term を得る．
  virtual
  VpiTchkTerm*
  tchk_ref_term() const = 0;
  
  /// @brief data term を得る．
  virtual
  VpiTchkTerm*
  tchk_data_term() const = 0;
  
  /// @brief 式のリストの反復子を得る．
  virtual
  VpiExprIteratorImpl*
  expr_iterator_obj() = 0;
  
  /// @brief 式のリストの反復子を得る．
  virtual
  VpiExprIterator
  expr_iterator() = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class VpiTchkTerm VpiTchk.h <ym_verilog/vpi/VpiTchk.h>
/// @brief timing check の端子
/// IEEE Std 1364-2001 26.6.17 Timing check
//////////////////////////////////////////////////////////////////////
class VpiTchkTerm :
  public VpiBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // VpiTchkTerm に固有の仮想関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief edge type を返す．
  virtual
  int
  edge() const = 0;
  
  /// @brief 親の timing check を返す．
  virtual
  VpiTchk*
  tchk() const = 0;
  
  /// @brief 式を返す．
  virtual
  VpiExpr*
  expr() const = 0;
  
  /// @brief 条件を返す．
  virtual
  VpiExpr*
  condition() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_VPI_VPITCHK_H
