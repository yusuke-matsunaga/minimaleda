#ifndef YM_VERILOG_PT_PTMODULE_H
#define YM_VERILOG_PT_PTMODULE_H

/// @file ym_verilog/pt/PtModule.h
/// @brief PtModule のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: PtModule.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/pt/PtBase.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtModule PtModule.h <ym_verilog/pt/PtModule.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief module を表すノード
//////////////////////////////////////////////////////////////////////
class PtModule :
  public PtNamedBase
{
public:

  /// @brief 仮想デストラクタ
  virtual
  ~PtModule() { }


public:
  //////////////////////////////////////////////////////////////////////
  // PtModule の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief macromodule 情報の取得
  /// @retval true macromodule の場合
  /// @retval false module の場合
  virtual
  bool
  is_macromodule() const = 0;

  /// @brief cell 情報の取得
  /// @retval true `celldefine --- `endcelldefine に挟まれたモジュールの場合
  /// @retval false 上記以外
  virtual
  bool
  is_cell() const = 0;

  /// @brief protect 情報の取得
  /// @return プロテクトされていたら true を返す．
  virtual
  bool
  is_protected() const = 0;

  /// @brief time unit の取得
  /// @return 時間の単位を表す 2 〜 -15 の整数\n
  /// もしくは未定義を表す -16
  virtual
  int
  time_unit() const = 0;

  /// @brief time precision の取得
  /// @return 時間の精度を表す 2 〜 -15 の整数\n
  /// もしくは未定義を表す -16
  virtual
  int
  time_precision() const = 0;

  /// @brief default net type の取得
  /// @return default net type
  virtual
  tVpiNetType
  nettype() const = 0;

  /// @brief unconnected drive の取得
  /// @return unconnected drive
  virtual
  tVpiUnconnDrive
  unconn_drive() const = 0;

  /// @brief default delay mode の取得
  /// @return default delay mode
  virtual
  tVpiDefDelayMode
  delay_mode() const = 0;

  /// @brief default decay time の取得
  /// @return default decay time
  virtual
  int
  decay_time() const = 0;

  /// @brief portfaults 情報の取得
  /// @return true で enable_portfaults を表す．
  virtual
  bool
  portfaults() const = 0;

  /// @brief suppress_faults 情報の取得
  /// @return true で suppress_faults が効いていることを表す．
  virtual
  bool
  suppress_faults() const = 0;

  /// @brief config 情報の取得
  /// @return config 情報
  virtual
  const string&
  config() const = 0;

  /// @brief library 情報の取得
  /// @return library 情報
  virtual
  const string&
  library() const = 0;

  /// @brief cell 情報の取得
  /// @return cell 情報
  virtual
  const string&
  cell() const = 0;

  /// @brief パラメータポート宣言配列の取得
  virtual
  PtDeclHeadArray
  paramport_array() const = 0;

  /// @brief ポート数を取り出す．
  virtual
  ymuint32
  port_num() const = 0;

  /// @brief ポートを取り出す．
  /// @param[in] pos 位置番号 ( 0 <= pos < port_num() )
  virtual
  const PtPort*
  port(ymuint32 pos) const = 0;

  /// @brief 入出力宣言ヘッダ配列の取得
  virtual
  PtIOHeadArray
  iohead_array() const = 0;

  /// @brief 入出力宣言の要素数の取得
  /// @note 個々のヘッダが持つ要素数の総和を計算する．
  virtual
  ymuint32
  iodecl_num() const = 0;

  /// @brief parameter 宣言ヘッダ配列の取得
  virtual
  PtDeclHeadArray
  paramhead_array() const = 0;

  /// @brief localparam 宣言ヘッダ配列の取得
  virtual
  PtDeclHeadArray
  localparamhead_array() const = 0;

  /// @brief 宣言ヘッダ配列の取得
  virtual
  PtDeclHeadArray
  declhead_array() const = 0;

  /// @brief item 配列の取得
  virtual
  PtItemArray
  item_array() const = 0;

  /// @brief 関数名から関数の検索
  /// @param[in] name 検索対象の関数名
  /// @return 該当する関数
  /// @return なければ NULL を返す．
  virtual
  const PtItem*
  find_function(const char* name) const = 0;

  /// @brief top_module フラグを下ろす
  virtual
  void
  clear_topmodule() const = 0;

  /// @brief top module のチェック
  /// @return top module の場合に true を返す．
  virtual
  bool
  is_topmodule() const = 0;

  /// @brief in_use フラグの設定
  virtual
  void
  set_in_use() const = 0;

  /// @brief in_use フラグの解除
  virtual
  void
  reset_in_use() const = 0;

  /// @brief in_use フラグの取得
  virtual
  bool
  is_in_use() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_PT_PTMODULE_H
