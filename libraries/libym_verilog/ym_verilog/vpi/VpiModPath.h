#ifndef YM_VERILOG_VPI_VPIMODPATH_H
#define YM_VERILOG_VPI_VPIMODPATH_H

/// @file ym_verilog/vpi/VpiModPath.h
/// @brief VpiModPath のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VpiModPath.h 400 2007-03-06 17:58:07Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#include <ym_verilog/vpi/VpiBase.h>
#include <ym_verilog/vpi/VpiIterator.h>


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VpiModPath VpiModPath.h <ym_verilog/vpi/VpiModPath.h>
/// @brief Module path を表すクラス
/// IEEE Std 1364-2001 26.6.15 Module path, path term
//////////////////////////////////////////////////////////////////////
class VpiModPath :
  public VpiBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // VpiModPath に固有の仮想関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief データ端子の極性を得る．
  virtual
  int
  data_polarity() const = 0;
  
  /// @brief path type を返す．
  virtual
  int
  path_type() const = 0;
  
  /// @brief 入力端子の極性を得る．
  virtual
  int
  polarity() const = 0;
  
  /// @brief "ifnone" 型の module path なら true を返す．
  virtual
  bool
  mod_path_has_if_none() const = 0;
  
  /// @brief 条件式を得る．
  virtual
  VpiExpr*
  condition() const = 0;
  
  /// @brief 遅延を表す式を得る．
  virtual
  VpiExpr*
  delay() const = 0;

  /// @brief 親のモジュールを取出す
  virtual
  VpiModule*
  module() const = 0;
  
  /// @brief 入力端子のリストの反復子を得る．
  virtual
  VpiModPathInIteratorImpl*
  mod_path_in_iterator_obj() const = 0;
  
  /// @brief 入力端子のリストの反復子を得る．
  virtual
  VpiModPathInIterator
  mod_path_in_iterator() const = 0;
  
  /// @brief 出力端子のリストの反復子を得る．
  virtual
  VpiModPathOutIteratorImpl*
  mod_path_out_iterator_obj() const = 0;
  
  /// @brief 出力端子のリストの反復子を得る．
  virtual
  VpiModPathOutIterator
  mod_path_out_iterator() const = 0;
  
  /// @brief データ端子のリストの反復子を得る．
  virtual
  VpiModDataPathInIteratorImpl*
  mod_data_path_in_iterator_obj() const = 0;
  
  /// @brief データ端子のリストの反復子を得る．
  virtual
  VpiModDataPathInIterator
  mod_data_path_in_iterator() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class VpiPathTerm VpiModPath.h <ym_verilog/vpi/VpiModPath.h>
/// @brief module path の端子
/// IEEE Std 1364-2001 26.6.15 Module path, path term
//////////////////////////////////////////////////////////////////////
class VpiPathTerm :
  public VpiBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // VpiPathTerm に固有の仮想関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 入出力の区分を返す．
  virtual
  tVpiDirection
  direction() const = 0;
  
  /// @brief edge type を返す．
  virtual
  int
  edge() const = 0;
  
  /// @brief 式を返す．
  virtual
  VpiExpr*
  expr() const = 0;
  
  /// @brief 親の module path を返す．
  virtual
  VpiModPath*
  mod_path() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class VpiInterModPath VpiModPath.h <ym_verilog/vpi/VpiModPath.h>
/// @brief intermodule path を表すクラス
/// IEEE Std 1364-2001 26.6.16 Intermodule path
//////////////////////////////////////////////////////////////////////
class VpiInterModPath :
  public VpiBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // VpiInterModPath に固有の仮想関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief ポートのリストの反復子を返す．
  virtual
  VpiPortsIteratorImpl*
  ports_iterator_obj() const = 0;
  
  /// @brief ポートのリストの反復子を返す．
  virtual
  VpiPortsIterator
  ports_iterator() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_VPI_VPIMODPATH_H
