#ifndef LIBYM_CELL_CI_CIBUS_H
#define LIBYM_CELL_CI_CIBUS_H

/// @file libym_cell/ci/CiBus.h
/// @brief CiBus のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/CellBus.h"
#include "ym_utils/ShString.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CiBusType CiBus.h "ci/CiBus.h"
/// @brief CellBusType の実装クラス
//////////////////////////////////////////////////////////////////////
class CiBusType :
  public CellBusType
{
  friend class CiLibrary;

private:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  /// @param[in] bit_from 開始位置
  /// @param[in] bit_to 終了位置
  CiBusType(ShString name,
	      ymint bit_from,
	      ymint bit_to);

  /// @brief デストラクタ
  virtual
  ~CiBusType();


public:
  //////////////////////////////////////////////////////////////////////
  // 属性の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  virtual
  string
  name() const;

  /// @brief base_type の取得
  virtual
  tBaseType
  base_type() const;

  /// @brief data_type の取得
  virtual
  tDataType
  data_type() const;

  /// @brief ビット幅の取得
  virtual
  ymuint
  bit_width() const;

  /// @brief 開始ビットの取得
  virtual
  ymint
  bit_from() const;

  /// @brief 終了ビットの取得
  virtual
  ymint
  bit_to() const;

  /// @brief 向きの取得
  /// @note true の時，降順を表す．
  virtual
  bool
  downto() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  ShString mName;

  // ビット幅
  ymuint32 mBitWidth;

  // 開始ビット
  ymint32 mBitFrom;

  // 終了ビット
  ymint32 mBitTo;

  // 向き
  bool mDownTo;

};


//////////////////////////////////////////////////////////////////////
/// @class CiBus CiBus.h "ci/CiBus.h"
/// @brief CellBus の実装クラス
//////////////////////////////////////////////////////////////////////
class CiBus :
  public CellBus
{
  friend class CiLibrary;

private:

  /// @brief コンストラクタ
  CiBus();

  /// @brief デストラクタ
  ~CiBus();


public:
  //////////////////////////////////////////////////////////////////////
  // 属性の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  virtual
  string
  name() const;

  /// @brief バスの型の取得
  virtual
  const CellBusType*
  bus_type() const;

  /// @brief ピン数の取得
  virtual
  ymuint
  pin_num() const;

  /// @brief ピンの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < pin_num() )
  virtual
  const CellPin*
  pin(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  ShString mName;

  // バスの型
  const CellBusType* mBusType;

  // ピン数
  ymuint32 mPinNum;

  // ピンの配列
  CellPin** mPinList;

};

END_NAMESPACE_YM_CELL

#endif // LIBYM_CELL_CI_CIBUS_H
