#ifndef YM_CELL_CELLBUS_H
#define YM_CELL_CELLBUS_H

/// @file　ym_cell/CellBus.h
/// @brief CellBus のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CellBus.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

#include "ym_cell/cell_nsdef.h"
#include "ym_cell/cell_type.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CellBusType CellBus.h <ym_cell/CellBus.h>
//////////////////////////////////////////////////////////////////////
class CellBusType
{
public:

  /// @brief base_type を表す型
  enum tBaseType {
    kArrayType
  };

  /// @brief data_type を表す型
  enum tDataType {
    kBitType
  };


public:

  /// @brief コンストラクタ
  CellBusType() { }

  /// @brief デストラクタ
  virtual
  ~CellBusType() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 属性の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  virtual
  string
  name() const = 0;

  /// @brief base_type の取得
  virtual
  tBaseType
  base_type() const = 0;

  /// @brief data_type の取得
  virtual
  tDataType
  data_type() const = 0;

  /// @brief ビット幅の取得
  virtual
  ymuint
  bit_width() const = 0;

  /// @brief 開始ビットの取得
  virtual
  ymint
  bit_from() const = 0;

  /// @brief 終了ビットの取得
  virtual
  ymint
  bit_to() const = 0;

  /// @brief 向きの取得
  /// @note true の時，降順を表す．
  virtual
  bool
  downto() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class CellBus CellBus.h <ym_cell/CellBus.h>
/// @brief バスを表すクラス
//////////////////////////////////////////////////////////////////////
class CellBus
{
public:

  /// @brief コンストラクタ
  CellBus() { }

  /// @brief デストラクタ
  virtual
  ~CellBus() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 属性の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  virtual
  string
  name() const = 0;

  /// @brief バスの型の取得
  virtual
  const CellBusType*
  bus_type() const = 0;

  /// @brief ピン数の取得
  virtual
  ymuint
  pin_num() const = 0;

  /// @brief ピンの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < pin_num() )
  virtual
  const CellPin*
  pin(ymuint pos) const = 0;

};

END_NAMESPACE_YM_CELL

#endif // YM_CELL_CELLBUS_H
