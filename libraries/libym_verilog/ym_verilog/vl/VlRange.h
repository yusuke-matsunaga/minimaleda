#ifndef YM_VERILOG_VL_VLRANGE_H
#define YM_VERILOG_VL_VLRANGE_H

/// @file ym_verilog/vl/VlRange.h
/// @brief VlRange のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlRange.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/vl/VlObj.h"
#include "ym_verilog/vl/VlFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VlRange VlRange.h <ym_verilog/vl/VlRange.h>
/// @brief 範囲を表すクラス
/// IEEE Std 1364-2001 26.6.10 Object range
//////////////////////////////////////////////////////////////////////
class VlRange :
  public VlObj
{
protected:

  /// @brief デストラクタ
  virtual
  ~VlRange() { }


public:
  //////////////////////////////////////////////////////////////////////
  // VlRange の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素数(ビット幅)を返す．
  virtual
  ymuint32
  size() const = 0;
  
  /// @brief MSB を返す．
  virtual
  const VlExpr*
  left_range() const = 0;
  
  /// @brief LSB を返す．
  virtual
  const VlExpr*
  right_range() const = 0;
  
  /// @brief MSB の値を返す．
  /// @retval MSB の値 値が確定しているとき
  /// @retval -1 値が確定していない
  virtual
  int
  left_range_const() const = 0;
  
  /// @brief LSB の値を返す．
  /// @retval LSB の値 値が確定しているとき
  /// @retval -1 値が確定していない
  virtual
  int
  right_range_const() const = 0;

  /// @brief 範囲のチェック
  /// @param[in] index インデックス
  /// @retval true index が範囲内に入っている．
  /// @retval false index が範囲外
  virtual
  bool
  is_in(int index) const = 0;
  
  /// @brief LSB からのオフセット値の取得
  /// @param[in] index インデックス
  /// @retval index の LSB からのオフセット index が範囲内に入っている．
  /// @retval -1 index が範囲外
  virtual
  int
  offset(int index) const = 0;

  /// @brief MSB からのオフセット値の取得
  /// @param[in] index インデックス
  /// @retval index の MSB からのオフセット index が範囲内に入っている．
  /// @retval -1 index が範囲外
  virtual
  int
  roffset(int index) const = 0;
  
  /// @brief offset の逆関数
  /// @param[in] offset LSB からのオフセット値
  /// @return offset に対応したインデックスを返す．
  virtual
  int
  index(int offset) const = 0;

  /// @brief roffset の逆関数
  /// @param[in] roffset MSB からのオフセット値
  /// @return roffset に対応したインデックスを返す．
  virtual
  int
  rindex(int roffset) const = 0;
  
};

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_VL_VLRANGE_H
