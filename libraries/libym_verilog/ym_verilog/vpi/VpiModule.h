#ifndef YM_VERILOG_VPI_VPIMODULE_H
#define YM_VERILOG_VPI_VPIMODULE_H

/// @file ym_verilog/vpi/VpiModule.h
/// @brief VpiModule のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VpiModule.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#include <ym_utils/ShString.h>
#include <ym_verilog/vpi/VpiNamedObj.h>
#include <ym_verilog/vpi/VpiIterator.h>
#include <ym_verilog/vpi/VpiScope.h>


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VpiModuleArray VpiModule.h <ym_verilog/vpi/VpiModule.h>
/// @brief module の配列を表すクラス
/// IEEE Std 1364-2001 26.6.1
//////////////////////////////////////////////////////////////////////
class VpiModuleArray :
  public VpiNamedObj
{
public:
  //////////////////////////////////////////////////////////////////////
  // VpiModuleArray に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素の取得
  /// @param[in] index 取得する要素のインデックス
  virtual
  VpiModule*
  elem(int index) const = 0;
  
  /// @brief 要素数を返す．
  virtual
  int
  size() const = 0;
  
  /// @brief 範囲のMSBを返す．
  virtual
  VpiExpr*
  left_range() const = 0;
  
  /// @brief 範囲のLSBを返す．
  virtual
  VpiExpr*
  right_range() const = 0;

  /// @brief 要素のリストを表す VpiExpr を返す．
  /// @note 変な仕様
  virtual
  VpiExpr*
  expr() const = 0;

  /// @brief 配列の先頭の反復子を得る．
  virtual
  VpiModuleIteratorImpl*
  module_iterator_obj() = 0;

  /// @brief 配列の先頭の反復子を得る．
  virtual
  VpiModuleIterator
  module_iterator() = 0;

  /// @brief parameter assignment のリストの反復子を得る．
  virtual
  VpiParamAssignIteratorImpl*
  param_assign_iterator_obj() = 0;

  /// @brief parameter assignment のリストの反復子を得る．
  virtual
  VpiParamAssignIterator
  param_assign_iterator() = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class VpiModule VpiModule.h <ym_verilog/vpi/VpiModule.h>
/// @brief Module の基底クラス
/// IEEE Std 1364-2001 26.6.1
//////////////////////////////////////////////////////////////////////
class VpiModule :
  public VpiScope
{
public:
  //////////////////////////////////////////////////////////////////////
  // VpiModule に固有の仮想関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief ポートを生成する．
  /// @param[in] file_region ソースファイル上の位置
  /// @param[in] name 名前
  /// @param[in] index ポートリスト中の位置
  virtual
  VpiPort*
  new_Port(const FileRegion& file_region,
	   const ShString& name,
	   int index) = 0;

  /// @brief ポート数を返す．
  virtual
  ymuint32
  port_num() const = 0;

  /// @brief ポートの取得
  /// @param[in] pos 取得するポートの位置
  /// @return pos 番目のポートを返す．
  virtual
  VpiPort*
  port(ymuint32 pos) const = 0;
  
  /// @brief definition location を返す．
  /// @return 定義側のファイル位置の情報を返す．
  virtual
  FileRegion
  def_file_region() const = 0;

  /// @brief 定義側のファイル名の取得
  virtual
  string
  def_filename() const = 0;

  /// @brief 定義側の行番号の取得
  virtual
  int
  def_line_no() const = 0;
  
  /// @brief definition name を返す．
  /// @return 定義名を返す．
  virtual
  string
  def_name() const = 0;
  
  /// @brief cell instance のチェック
  /// @return cell instance の場合に true を返す．
  virtual
  bool
  is_cell_instance() const = 0;
  
  /// @brief protect のチェック
  /// @return されていたら true を返す．
  virtual
  bool
  is_protected() const = 0;
  
  /// @brief 配列要素の時 true を返す．
  virtual
  bool
  is_array() const = 0;

  /// @brief top module の時 true を返す．
  virtual
  bool
  is_top_module() const = 0;
  
  /// @brief time unit を返す．
  /// @return 結果は 2 〜 -15 の整数
  /// @return もしくは未定義を表す -16
  virtual
  int
  time_unit() const = 0;
  
  /// @brief time precision を返す．
  /// @return 結果は 2 〜 -15 の整数
  /// @return もしくは未定義を表す -16
  virtual
  int
  time_precision() const = 0;
  
  /// @brief default net type を返す．
  virtual
  tVpiNetType
  def_net_type() const = 0;
  
  /// @brief unconnected drive を返す．
  virtual
  tVpiUnconnDrive
  unconn_drive() const = 0;
  
  /// @brief default delay mode を返す．
  virtual
  tVpiDefDelayMode
  def_delay_mode() const = 0;
  
  /// @brief default decay time を返す．
  virtual
  int
  def_decay_time() const = 0;
  
  /// @brief config 情報を返す．
  virtual
  string
  config() const = 0;
  
  /// @brief library 情報を返す．
  virtual
  string
  library() const = 0;
  
  /// @brief cell 情報を返す．
  virtual
  string
  cell() const = 0;

  /// @brief インデックスの値を返す．
  virtual
  int
  index_val() const = 0;

  /// @brief インデックスを返す．
  virtual
  VpiExpr*
  index() const = 0;

  /// @brief 配列要素の時, 親の配列を返す．
  virtual
  VpiModuleArray*
  module_array() const = 0;

  /// @brief ポートのリストの反復子を返す．
  virtual
  VpiPortIteratorImpl*
  port_iterator_obj() = 0;

  /// @brief ポートのリストの反復子を返す．
  virtual
  VpiPortIterator
  port_iterator() = 0;

  /// @brief 入出力のリストの反復子を返す．
  virtual
  VpiIODeclIteratorImpl*
  io_decl_iterator_obj() = 0;

  /// @brief 入出力のリストの反復子を返す．
  virtual
  VpiIODeclIterator
  io_decl_iterator() = 0;

  /// @brief モジュールのリストの反復子を返す．
  virtual
  VpiModuleIteratorImpl*
  module_iterator_obj() = 0;

  /// @brief モジュールのリストの反復子を返す．
  virtual
  VpiModuleIterator
  module_iterator() = 0;

  /// @brief モジュール配列のリストの反復子を返す．
  virtual
  VpiModuleArrayIteratorImpl*
  module_array_iterator_obj() = 0;

  /// @brief モジュール配列のリストの反復子を返す．
  virtual
  VpiModuleArrayIterator
  module_array_iterator() = 0;

  /// @brief プロセス(initial/always)のリストの反復子を返す．
  virtual
  VpiProcessIteratorImpl*
  process_iterator_obj() = 0;

  /// @brief プロセス(initial/always)のリストの反復子を返す．
  virtual
  VpiProcessIterator
  process_iterator() = 0;
  
  /// @brief modpath のリストの反復子を返す．
  virtual
  VpiModPathIteratorImpl*
  mod_path_iterator_obj() = 0;
  
  /// @brief modpath のリストの反復子を返す．
  virtual
  VpiModPathIterator
  mod_path_iterator() = 0;
  
  /// @brief タイミングチェックのリストの反復子を返す．
  virtual
  VpiTchkIteratorImpl*
  tchk_iterator_obj() = 0;
  
  /// @brief タイミングチェックのリストの反復子を返す．
  virtual
  VpiTchkIterator
  tchk_iterator() = 0;
  
  /// @brief specparm のリストの反復子を返す．
  virtual
  VpiSpecParamIteratorImpl*
  spec_param_iterator_obj() = 0;
  
  /// @brief specparm のリストの反復子を返す．
  virtual
  VpiSpecParamIterator
  spec_param_iterator() = 0;
  
  /// @brief defparam のリストの反復子を返す．
  virtual
  VpiDefParamIteratorImpl*
  def_param_iterator_obj() = 0;
  
  /// @brief defparam のリストの反復子を返す．
  virtual
  VpiDefParamIterator
  def_param_iterator() = 0;
  
  /// @brief パラメータ代入文のリストの反復子を返す．
  virtual
  VpiParamAssignIteratorImpl*
  param_assign_iterator_obj() = 0;
  
  /// @brief パラメータ代入文のリストの反復子を返す．
  virtual
  VpiParamAssignIterator
  param_assign_iterator() = 0;

#if 0 // VpiInserter
  /// @brief モジュールのリストの挿入反復子を返す．
  virtual
  VpiInserter<VpiModule>
  module_inserter() = 0;

  /// @brief モジュール配列のリストの挿入反復子を返す．
  virtual
  VpiInserter<VpiModuleArray>
  module_array_inserter() = 0;

  /// @brief プロセス(initial/always)のリストの挿入反復子を返す．
  virtual
  VpiInserter<VpiProcess>
  process_inserter() = 0;
  
  /// @brief modpath のリストの挿入反復子を返す．
  virtual
  VpiInserter<VpiModPath>
  mod_path_inserter() = 0;
  
  /// @brief タイミングチェックのリストの挿入反復子を返す．
  virtual
  VpiInserter<VpiTchk>
  tchk_inserter() = 0;
  
  /// @brief specparm のリストの挿入反復子を返す．
  virtual
  VpiInserter<VpiSpecParam>
  spec_param_inserter() = 0;
  
  /// @brief defparam のリストの挿入反復子を返す．
  virtual
  VpiInserter<VpiDefParam>
  def_param_inserter() = 0;
  
  /// @brief パラメータ代入文のリストの挿入反復子を返す．
  virtual
  VpiInserter<VpiParamAssign>
  param_assign_inserter() = 0;
#endif // VpiInserter

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_VPI_VPIMODULE_H
