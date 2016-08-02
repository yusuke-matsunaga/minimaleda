#ifndef LIBYM_VERILOG_SCANNER_LEXMODULESTATE_H
#define LIBYM_VERILOG_SCANNER_LEXMODULESTATE_H

/// @file libym_verilog/scanner/LexModuleState.h
/// @brief 標準の compiler directive 用状態のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: LexModuleState.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "LexState.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class LexModuleState
/// @ingroup VlParser
/// @brief モジュール関係の compiler directive 用状態を表すクラス
//////////////////////////////////////////////////////////////////////
class LexModuleState :
  public LexState
{
public:

  /// @brief コンストラクタ
  LexModuleState(RawLex& lex);
  
  /// @brief デストラクタ
  virtual
  ~LexModuleState();


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用のメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief cell_define の状態を設定する．
  void
  set_cell_define(bool val,
		  const FileRegion& file_region);

  /// @brief net_type を設定する．
  void
  set_default_nettype(tVpiNetType val,
		      const FileRegion& file_region);

  /// @brief time_unit の設定
  void
  set_time_unit(int unit,
		const FileRegion& file_region);

  /// @brief time_precision の設定
  void
  set_time_precision(int precision,
		     const FileRegion& file_region);

  /// @brief unconnected_drive の値を設定する．
  void
  set_unconnected_drive(tVpiUnconnDrive val,
			const FileRegion& file_region);

  /// @brief decay_time の値を設定する．
  void
  set_default_decay_time(int val,
			 const FileRegion& file_region);

  /// @brief trireg_strength の値を設定する．
  void
  set_default_trireg_strength(int val,
			      const FileRegion& file_region);

  /// @brief delay_mode の値を設定する．
  void
  set_delay_mode(tVpiDefDelayMode val,
		 const FileRegion& file_region);


public:
  //////////////////////////////////////////////////////////////////////
  // 状態取得用のメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief cell_define の状態を取得する
  /// @retval true cell_define されている
  /// @retval false cell_define されていない
  bool
  cell_define() const;

  /// @brief net_type を取得する．
  tVpiNetType
  default_nettype() const;
  
  /// @brief time_unit の取得
  int
  time_unit() const;

  /// @brief time_precision の取得
  int
  time_precision() const;

  /// @brief unconnected_drive の値を取得する．
  tVpiUnconnDrive
  unconnected_drive() const;

  /// @brief decay_time の値を取得する．
  int
  default_decay_time() const;

  /// @brief trireg_strength の値を取得する．
  int
  default_trireg_strength() const;

  /// @brief delay_mode の値を取得する．
  tVpiDefDelayMode
  delay_mode() const;


public:
  //////////////////////////////////////////////////////////////////////
  // LexState の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 状態を初期化する．
  virtual
  void
  resetall(const FileRegion& file_region);


private:
  //////////////////////////////////////////////////////////////////////
  // プライベートメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief nettype の値から対応する文字列を取得する
  static
  const char*
  nettype2str(tVpiNetType nettype);
  
  /// @brief time unit を表す整数から文字列を得る．
  /// @param[in] id time unit を表す整数値(2 〜 -15)
  /// @return time unit を表す文字列
  static
  string
  unit2str(int id);

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // cell_define の状態
  // true で cell_define されていることを表す
  bool mCellDefine;

  // default_nettype の値
  tVpiNetType mDefaultNetType;

  // 現在の time unit の値
  // 正常な値は 2 〜 -15
  // -16 で未定義
  int mTimeUnit;

  // 現在の time precision の値
  // 正常な値は 2 〜 -15
  // -16 で未定義
  int mTimePrecision;

  // unconnected_drive の値
  tVpiUnconnDrive mUnconnDrive;

  // decay_time の値
  int mDecayTime;

  // trireg_strength の値
  int mTriregStrength;

  // delay_mode の値
  tVpiDefDelayMode mDelayMode;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_SCANNER_LEXMODULESTATE_H
