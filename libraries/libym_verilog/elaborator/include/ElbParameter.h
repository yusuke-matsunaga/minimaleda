#ifndef LIBYM_VERILOG_ELB_ELBPAREMETER_H
#define LIBYM_VERILOG_ELB_ELBPARAMETER_H

/// @file libym_verilog/elaborator/include/ElbParameter.h
/// @brief ElbParameter のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ElbParameter.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ElbDecl.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbParamHead.h ElbParameter.h "ElbParameter.h"
/// @brief 宣言要素のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class ElbParamHead
{
protected:

  /// @brief コンストラクタ
  ElbParamHead();

  /// @brief デストラクタ
  virtual
  ~ElbParamHead();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbParamHead の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const = 0;

  /// @brief このオブジェクトの属しているスコープを返す．
  virtual
  const VlNamedObj*
  parent() const = 0;

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
  ElbExpr*
  left_range() const = 0;

  /// @brief 範囲のLSBの取得
  /// @retval 範囲のLSB 範囲を持つとき
  /// @retval NULL 範囲を持たないとき
  virtual
  ElbExpr*
  right_range() const = 0;

  /// @brief MSB の値を返す．
  virtual
  int
  left_range_const() const = 0;

  /// @brief LSB の値を返す．
  virtual
  int
  right_range_const() const = 0;

  /// @brief ビット幅を返す．
  /// @note このクラスでは 1 を返す．
  virtual
  ymuint32
  bit_size() const = 0;

  /// @brief LSB からのオフセット値の取得
  /// @param[in] index インデックス
  /// @retval index の LSB からのオフセット index が範囲内に入っている．
  /// @retval -1 index が範囲外
  virtual
  int
  bit_offset(int index) const = 0;

  /// @breif 値の型を返す．
  /// @note 値を持たないオブジェクトの場合には kVpiValueNone を返す．
  virtual
  tVpiValueType
  value_type() const = 0;

  /// @brief データ型の取得
  virtual
  tVpiVarType
  data_type() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class ElbParameter ElbParameter.h "ElbParameter.h"
/// @brief parameter を表すクラス
/// IEEE Std 1364-2001 26.6.12 Parameter, specparam
//////////////////////////////////////////////////////////////////////
class ElbParameter :
  public ElbDecl
{
  friend class CellParam;

protected:

  /// @brief コンストラクタ
  ElbParameter();

  /// @brief デストラクタ
  virtual
  ~ElbParameter();


public:
  //////////////////////////////////////////////////////////////////////
  // VlDecl の関数
  //////////////////////////////////////////////////////////////////////

  /// @breif 値の型を返す．
  /// @note 値を持たないオブジェクトの場合には kVpiValueNone を返す．
  virtual
  tVpiValueType
  value_type() const = 0;

  /// @brief 定数値を持つ型のときに true を返す．
  /// @note このクラスは true を返す．
  virtual
  bool
  is_consttype() const;

  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  virtual
  bool
  is_signed() const = 0;

  /// @brief ビット要素を返す．
  /// @param[in] index インデックス
  /// @retval ビット要素 index が範囲内の場合
  /// @retval NULL index が範囲外の場合．またはビット展開されていない場合
  /// @note このクラスでは NULL を返す．
  virtual
  const VlDecl*
  elem_by_index(int index) const;

  /// @brief ビット要素を返す．
  /// @param[in] offset オフセット
  /// @retval ビット要素 offset が範囲内の場合
  /// @retval NULL offset が範囲外の場合．またはビット展開されていない場合
  /// @note このクラスでは NULL を返す．
  virtual
  const VlDecl*
  elem_by_offset(ymuint offset) const;

  /// @brief データ型の取得
  /// @retval データ型 パラメータや変数の場合
  /// @retval kVpiVarNone 上記以外
  virtual
  tVpiVarType
  data_type() const = 0;

  /// @brief net 型の取得
  /// @retval net 型 net 型の要素の場合
  /// @retval kVpiNone net 型の要素でない場合
  /// @note このクラスでは kVpiNone を返す．
  virtual
  tVpiNetType
  net_type() const;

  /// @brief vectored|scalared 属性の取得
  /// @retval kVpiVsNone vectored|scalared 指定なし
  /// @retval kVpiVectored vectored 指定あり
  /// @retval kVpiScalared scalared 指定あり
  /// @note このクラスでは kVpiVsNone を返す．
  virtual
  tVpiVsType
  vs_type() const;

  /// @brief drive0 strength の取得
  /// @retval 0 の強度
  /// @retval kVpiNoStrength strength の指定なし
  /// @note このクラスでは kVpiNoStrength を返す．
  virtual
  tVpiStrength
  drive0() const;

  /// @brief drive1 strength の取得
  /// @retval 1 の強度
  /// @retval kVpiNoStrength strength の指定なし
  /// @note kVpiNoStrength を返す．
  virtual
  tVpiStrength
  drive1() const;

  /// @brief charge strength の取得
  /// @retval 電荷の強度
  /// @retval kVpiNoStrength strength の指定なし
  /// @note このクラスでは kVpiNoStrength を返す．
  virtual
  tVpiStrength
  charge() const;

  /// @brief delay の取得
  /// @retval delay
  /// @retval NULL delay の指定なし
  /// @note このクラスでは NULL を返す．
  virtual
  const VlDelay*
  delay() const;

  /// @brief 配列型オブジェクトの時に true を返す．
  /// @note このクラスでは NULL を返す．
  virtual
  bool
  is_array() const;

  /// @brief 多次元の配列型オブジェクトの時に true を返す．
  /// @note このクラスでは NULL を返す．
  virtual
  bool
  is_multi_array() const;

  /// @brief 配列型オブジェクトの場合の次元数の取得
  /// @note このクラスでは 0 を返す．
  virtual
  ymuint32
  dimension() const;

  /// @brief 範囲の取得
  /// @param[in] pos 位置 ( 0 <= pos < dimension() )
  /// @note このクラスでは NULL を返す．
  virtual
  const VlRange*
  range(ymuint32 pos) const;

  /// @brief 配列要素の時に true を返す．
  /// @note このクラスでは false を返す．
  virtual
  bool
  is_array_member() const;

  /// @brief 多次元の配列要素の時に true を返す．
  /// @note このクラスでは false を返す．
  virtual
  bool
  is_multi_array_member() const;

  /// @brief 配列要素の時に親の配列を返す．
  /// @note このクラスでは NULL を返す．
  virtual
  VlDecl*
  parent_array() const;

  /// @brief 1次元配列要素の時にインデックスを返す．
  /// @note このクラスでは NULL を返す．
  virtual
  const VlExpr*
  index() const;

  /// @brief 多次元配列要素の時にインデックスのリストを返す．
  /// @param[out] index_list インデックスのリストを格納する変数
  virtual
  void
  index(vector<const VlExpr*>& index_list) const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 符号付きに補正する．
  /// @note このクラスでは何もしない．
  virtual
  void
  set_signed();

  /// @brief スカラー値を設定する．
  /// @param[in] val 値
  /// @note このクラスでは何もしない．
  virtual
  void
  set_scalar(tVpiScalarVal val);

  /// @brief real 型の値を設定する．
  /// @param[in] val 値
  /// @note このクラスでは何もしない．
  virtual
  void
  set_real(double val);

  /// @brief bitvector 型の値を設定する．
  /// @param[in] val 値
  /// @note このクラスでは何もしない．
  virtual
  void
  set_bitvector(const BitVector& val);

  /// @brief ビット値を設定する．
  /// @param[in] index ビット位置
  /// @param[in] val 値
  /// @note このクラスでは何もしない．
  virtual
  void
  set_bitselect(int index,
		tVpiScalarVal val);

  /// @brief 範囲値を設定する．
  /// @param[in] left 範囲の MSB
  /// @param[in] right 範囲の LSB
  /// @param[in] val 値
  /// @note このクラスでは何もしない．
  virtual
  void
  set_partselect(int left,
		 int right,
		 const BitVector& val);

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
ElbParamHead::ElbParamHead()
{
}

// @brief デストラクタ
inline
ElbParamHead::~ElbParamHead()
{
}

// @brief コンストラクタ
inline
ElbParameter::ElbParameter()
{
}

// @brief デストラクタ
inline
ElbParameter::~ElbParameter()
{
}

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELB_ELBPARAMETER_H
