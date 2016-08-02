#ifndef YM_VERILOG_VL_VLDECL_H
#define YM_VERILOG_VL_VLDECL_H

/// @file ym_verilog/vl/VlNamedObj.h
/// @brief VlNamedObj のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlDecl.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/vl/VlNamedObj.h"
#include "ym_verilog/vl/VlFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VlDecl VlDecl.h <ym_verilog/vl/VlDecl.h>
/// @brief エラボレーション中の名前付きオブジェクトを表す基底クラス
//////////////////////////////////////////////////////////////////////
class VlDecl :
  public VlNamedObj
{
protected:

  /// @brief デストラクタ
  virtual
  ~VlDecl() { }


public:
  //////////////////////////////////////////////////////////////////////
  // VlDecl の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @breif 値の型を返す．
  /// @note 値を持たないオブジェクトの場合には kVpiValueNone を返す．
  virtual
  tVpiValueType
  value_type() const = 0;

  /// @brief 定数値を持つ型のときに true を返す．
  virtual
  bool
  is_consttype() const = 0;

  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  virtual
  bool
  is_signed() const = 0;

  /// @brief 範囲のMSBの取得
  /// @retval 範囲のMSB 範囲指定を持つとき
  /// @retval NULL 範囲指定を持たないとき
  virtual
  const VlExpr*
  left_range() const = 0;

  /// @brief 範囲のLSBの取得
  /// @retval 範囲のLSB 範囲指定を持つとき
  /// @retval NULL 範囲指定を持たないとき
  virtual
  const VlExpr*
  right_range() const = 0;

  /// @brief MSB の値を返す．
  /// @retval 範囲のMSBの値 範囲指定を持つとき
  /// @retval -1 範囲指定を持たないとき
  virtual
  int
  left_range_const() const = 0;

  /// @brief LSB の値を返す．
  /// @retval 範囲のLSBの値 範囲指定を持つとき
  /// @retval -1 範囲指定を持たないとき
  virtual
  int
  right_range_const() const = 0;

  /// @brief ビット幅を返す．
  virtual
  ymuint
  bit_size() const = 0;

  /// @brief オフセット値の取得
  /// @param[in] index インデックス
  /// @retval index に対するオフセット値 index が範囲内に入っている時．
  /// @retval -1 index が範囲外の時
  virtual
  int
  bit_offset(int index) const = 0;

  /// @brief ビット要素を返す．
  /// @param[in] index インデックス
  /// @retval ビット要素 index が範囲内の場合
  /// @retval NULL index が範囲外の場合．またはビット展開されていない場合
  virtual
  const VlDecl*
  elem_by_index(int index) const = 0;

  /// @brief ビット要素を返す．
  /// @param[in] offset オフセット
  /// @retval ビット要素 offset が範囲内の場合
  /// @retval NULL offset が範囲外の場合．またはビット展開されていない場合
  virtual
  const VlDecl*
  elem_by_offset(ymuint offset) const = 0;

  /// @brief データ型の取得
  /// @retval データ型 パラメータや変数の場合
  /// @retval kVpiVarNone 上記以外
  virtual
  tVpiVarType
  data_type() const = 0;

  /// @brief net 型の取得
  /// @retval net 型 net 型の要素の場合
  /// @retval kVpiNone net 型の要素でない場合
  virtual
  tVpiNetType
  net_type() const = 0;

  /// @brief vectored|scalared 属性の取得
  /// @retval kVpiVsNone vectored|scalared 指定なし
  /// @retval kVpiVectored vectored 指定あり
  /// @retval kVpiScalared scalared 指定あり
  virtual
  tVpiVsType
  vs_type() const = 0;

  /// @brief drive0 strength の取得
  /// @retval 0 の強度
  /// @retval kVpiNoStrength strength の指定なし
  virtual
  tVpiStrength
  drive0() const = 0;

  /// @brief drive1 strength の取得
  /// @retval 1 の強度
  /// @retval kVpiNoStrength strength の指定なし
  virtual
  tVpiStrength
  drive1() const = 0;

  /// @brief charge strength の取得
  /// @retval 電荷の強度
  /// @retval kVpiNoStrength strength の指定なし
  virtual
  tVpiStrength
  charge() const = 0;

  /// @brief delay の取得
  /// @retval delay
  /// @retval NULL delay の指定なし
  virtual
  const VlDelay*
  delay() const = 0;

  /// @brief 初期値の取得
  /// @retval 初期値
  /// @retval NULL 設定がない場合
  virtual
  const VlExpr*
  init_value() const = 0;

  /// @brief localparam のときに true 返す．
  virtual
  bool
  is_local_param() const = 0;

  /// @brief 配列型オブジェクトの時に true を返す．
  virtual
  bool
  is_array() const = 0;

  /// @brief 多次元の配列型オブジェクトの時に true を返す．
  virtual
  bool
  is_multi_array() const = 0;

  /// @brief 配列型オブジェクトの場合の次元数の取得
  virtual
  ymuint
  dimension() const = 0;

  /// @brief 範囲の取得(配列型オブジェクト用)
  /// @param[in] pos 位置 ( 0 <= pos < dimension() )
  virtual
  const VlRange*
  range(ymuint pos) const = 0;

  /// @brief 配列要素の時に true を返す．
  virtual
  bool
  is_array_member() const = 0;

  /// @brief 多次元の配列要素の時に true を返す．
  virtual
  bool
  is_multi_array_member() const = 0;

  /// @brief 配列要素の時に親の配列を返す．
  virtual
  const VlDecl*
  parent_array() const = 0;

  /// @brief 1次元配列要素の時にインデックスを返す．
  virtual
  const VlExpr*
  index() const = 0;

  /// @brief 多次元配列要素の時にインデックスのリストを返す．
  /// @param[out] index_list インデックスのリストを格納する変数
  virtual
  void
  index(vector<const VlExpr*>& index_list) const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_VL_VLDECL_H
