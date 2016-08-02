#ifndef YM_CELL_CELLLIBRARY_H
#define YM_CELL_CELLLIBRARY_H

/// @file　ym_cell/CellLibrary.h
/// @brief CellLibrary のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CellLibrary.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/cell_nsdef.h"
#include "ym_cell/cell_type.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CellLibrary CellLibrary.h <ym_cell/CellLibrary.h>
/// @brief セルライブラリを表すクラス
//////////////////////////////////////////////////////////////////////
class CellLibrary
{
public:

  /// @brief コンストラクタ
  CellLibrary() { }

  /// @brief デストラクタ
  virtual
  ~CellLibrary() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 属性の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  virtual
  string
  name() const = 0;

  /// @brief テクノロジの取得
  /// 返り値は
  /// - kTechAsic
  /// - kTechFpga
  /// のどちらか
  virtual
  tCellTechnology
  technology() const = 0;

  /// @brief 遅延モデルの取得
  /// 返り値は
  /// - kDelayGeneric
  /// - kDelayPiecewise
  /// - kDelayNonlinear
  /// のいずれか
  virtual
  tCellDelayModel
  delay_model() const = 0;

  /// @brief バス命名規則の取得
  virtual
  string
  bus_naming_style() const = 0;

  /// @brief 日付情報の取得
  virtual
  string
  date() const = 0;

  /// @brief リビジョン情報の取得
  virtual
  string
  revision() const = 0;

  /// @brief コメント情報の取得
  virtual
  string
  comment() const = 0;

  /// @brief 時間単位の取得
  virtual
  string
  time_unit() const = 0;

  /// @brief 電圧単位の取得
  virtual
  string
  voltage_unit() const = 0;

  /// @brief 電流単位の取得
  virtual
  string
  current_unit() const = 0;

  /// @brief 抵抗単位の取得
  virtual
  string
  pulling_resistance_unit() const = 0;

  /// @brief 容量単位の取得
  virtual
  string
  capacitive_load_unit() const = 0;

  /// @brief 電力単位の取得
  virtual
  string
  leakage_power_unit() const = 0;

  /// @brief ルックアップテーブルのテンプレートの取得
  /// @param[in] name テンプレート名
  /// @note なければ NULL を返す．
  virtual
  const CellLutTemplate*
  lu_table_template(const char* name) const = 0;

  /// @brief バスタイプの取得
  /// @param[in] name バスタイプ名
  /// @note なければ NULL を返す．
  virtual
  const CellBusType*
  bus_type(const char* name) const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // セル情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief このライブラリの持つセル数の取得
  virtual
  ymuint
  cell_num() const = 0;

  /// @brief セルの取得
  /// @param[in] pos 位置番号( 0 <= pos < cell_num() )
  virtual
  const Cell*
  cell(ymuint pos) const = 0;

  /// @brief 名前からのセルの取得
  virtual
  const Cell*
  cell(const char* name) const = 0;

};


/// @relates CellLibrary
/// @brief 内容をバイナリダンプする．
void
dump_library(ostream& s,
	     const CellLibrary& library);

/// @relates CellLibrary
/// @brief 内容を出力する．
void
display_library(ostream& s,
		const CellLibrary& library);

/// @relates CellLibrary
/// @brief バイナリダンプされた内容を読み込む．
const CellLibrary*
restore_library(istream& s);


END_NAMESPACE_YM_CELL

#endif // YM_CELL_CELLLIBRARY_H
