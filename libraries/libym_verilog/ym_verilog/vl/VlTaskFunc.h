#ifndef YM_VERILOG_VL_VLTASKFUNC_H
#define YM_VERILOG_VL_VLTASKFUNC_H

/// @file ym_verilog/vl/VlTaskFunc.h
/// @brief VlTaskFunc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlTaskFunc.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/vl/VlNamedObj.h"
#include "ym_verilog/vl/VlObj.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VlTaskFunc VlTaskFunc.h "VlTaskFunc.h"
/// @brief elaboration 中の task/function を表すクラス
/// IEEE Std 1364-2001 26.6.18 Task, function declaration
//////////////////////////////////////////////////////////////////////
class VlTaskFunc :
  public VlNamedObj
{
protected:
	      
  /// @brief デストラクタ
  virtual
  ~VlTaskFunc() { }


public:
  //////////////////////////////////////////////////////////////////////
  // Task/Function に共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief automatic 宣言されていたら true を返す．
  virtual
  bool
  automatic() const = 0;

  /// @brief 入出力数を得る．
  virtual
  ymuint32
  io_num() const = 0;

  /// @brief 入出力の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < io_num() )
  virtual
  const VlIODecl*
  io(ymuint32 pos) const = 0;

  /// @brief 本体のステートメントを得る．
  virtual
  const VlStmt*
  stmt() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // Function の仮想関数
  // Task の場合には意味を持たない．
  //////////////////////////////////////////////////////////////////////

  /// @brief function type を返す．
  virtual
  tVpiFuncType
  func_type() const = 0;

  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  virtual
  bool
  is_signed() const = 0;

  /// @brief 範囲のMSBの取得
  /// @retval 範囲のMSB 範囲を持つとき
  /// @retval NULL 範囲を持たないとき
  virtual
  const VlExpr*
  left_range() const = 0;

  /// @brief 範囲のLSBの取得
  /// @retval 範囲のLSB 範囲を持つとき
  /// @retval NULL 範囲を持たないとき
  virtual
  const VlExpr*
  right_range() const = 0;

  /// @brief 範囲の MSB の値を返す．
  /// @retval 範囲のMSBの値 範囲指定を持つとき
  /// @retval -1 範囲指定を持たないとき
  virtual
  int
  left_range_const() const = 0;

  /// @brief 範囲の LSB の値を返す．
  /// @retval 範囲のLSBの値 範囲指定を持つとき
  /// @retval -1 範囲指定を持たないとき
  virtual
  int
  right_range_const() const = 0;

  /// @brief 出力のビット幅を返す．
  virtual
  ymuint32
  bit_size() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // LiBYM_VERILOG_ELABORATOR_VLTASKFUNC_H
