#ifndef LIBYM_CELL_CI_CIBUNDLE_H
#define LIBYM_CELL_CI_CIBUNDLE_H

/// @file libym_cell/ci/CiBundle.h
/// @brief CiBundle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/CellBundle.h"
#include "ym_utils/ShString.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CiBundle CiBundle.h "ci/CiBundle.h"
/// @brief CellBundle の実装クラス
//////////////////////////////////////////////////////////////////////
class CiBundle :
  public CellBundle
{
  friend class CiLibrary;

private:

  /// @brief コンストラクタ
  CiBundle();

  /// @brief デストラクタ
  ~CiBundle();


public:
  //////////////////////////////////////////////////////////////////////
  // 属性の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  virtual
  string
  name() const;

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

  // ピン数
  ymuint32 mPinNum;

  // ピンのリスト
  CellPin** mPinList;

};

END_NAMESPACE_YM_CELL

#endif // LIBYM_CELL_CI_CIBUNDLE_H
