#ifndef YM_VERILOG_VPI_VPITASKFUNC_H
#define YM_VERILOG_VPI_VPITASKFUNC_H

/// @file ym_verilog/vpi/VpiTaskFunc.h
/// @brief Task/Function を表すクラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VpiTaskFunc.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.

// IEEE Std 1364-2001 26.6.18 Task, function declaration


#include <ym_utils/ShString.h>
#include <ym_verilog/vpi/VpiScope.h>


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VpiTaskFunc VpiTaskFunc.h <ym_verilog/vpi/VpiTaskFunc.h>
/// @brief task/function を表すクラス
/// IEEE Std 1364-2001 26.6.18 Task, function declaration
//////////////////////////////////////////////////////////////////////
class VpiTaskFunc :
  public VpiScope
{
public:
  /// @brief 親クラスのエイリアス
  typedef VpiScope Parent;


public:
  //////////////////////////////////////////////////////////////////////
  // VpiTaskFunc に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 本体のステートメントを設定する．
  virtual
  void
  set_stmt(VpiStmt* body) = 0;

  /// @brief automatic 宣言されていたら true を返す．
  virtual
  bool
  is_automatic() const = 0;

  /// @brief 入出力数を返す．
  virtual
  ymuint32
  io_size() const = 0;

  /// @brief pos 番めの入出力を返す．
  virtual
  VpiIODecl*
  io(ymuint32 pos) const = 0;
  
  /// @brief 本体を返す．
  virtual
  VpiStmt*
  stmt() const = 0;
  
  /// @brief IO 宣言のリストを返す．
  virtual
  VpiIODeclIteratorImpl*
  io_decl_iterator_obj() = 0;
  
  /// @brief IO 宣言のリストを返す．
  virtual
  VpiIODeclIterator
  io_decl_iterator() = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class VpiFunction VpiTaskFunc.h <ym_verilog/vpi/VpiTaskFunc.h>
/// @brief function declaration の基底クラス
/// IEEE Std 1364-2001 26.6.18 Task, function declaration
//////////////////////////////////////////////////////////////////////
class VpiFunction :
  public VpiTaskFunc
{
public:
  /// @brief 親クラスのエイリアス
  typedef VpiTaskFunc Parent;


public:
  //////////////////////////////////////////////////////////////////////
  // VpiFunction に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 出力変数(を表す式)を登録する．
  virtual
  void
  set_ovar(VpiExpr* ovar) = 0;

  /// @brief constant function の時 true を返す．
  virtual
  bool
  is_constant_function() const = 0;

  /// @brief 出力変数を返す．
  virtual
  VpiExpr*
  ovar() = 0;

  /// @brief 関数値の評価を行う．
  virtual
  VpiExpr*
  evaluate(const vector<VpiExpr*>& arg_list) = 0;
  
  /// @brief function type を返す．
  virtual
  tVpiFuncType
  func_type() const = 0;
  
  /// @brief 出力のビット幅を返す．
  virtual
  int
  size() const = 0;
  
  /// @brief 符号付きの時 true を返す．
  virtual
  bool
  is_signed() const = 0;
  
  /// @brief 範囲のMSBを返す．
  virtual
  VpiExpr*
  left_range() const = 0;
  
  /// @brief 範囲のLSBを返す．
  virtual
  VpiExpr*
  right_range() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class VpiTask VpiTaskFunc.h <ym_verilog/vpi/VpiTaskFunc.h>
/// @brief task を表すクラス
/// IEEE Std 1364-2001 26.6.18 Task, function declaration
//////////////////////////////////////////////////////////////////////
class VpiTask :
  public VpiTaskFunc
{
};

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_VPI_VPITASKFUNC_H
