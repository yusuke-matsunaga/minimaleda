
/// @file libym_verilog/elb/ElbParameter.cc
/// @brief ElbParameter の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ElbParameter.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ElbParameter.h"
#include "ElbParamAssign.h"
#include "ElbExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス ElbParameter
//////////////////////////////////////////////////////////////////////

// @brief 定数値を持つ型のときに true を返す．
bool
ElbParameter::is_consttype() const
{
  return true;
}

// @brief ビット要素を返す．
// @param[in] index インデックス
// @retval ビット要素 index が範囲内の場合
// @retval NULL index が範囲外の場合．またはビット展開されていない場合
// @note このクラスでは NULL を返す．
const VlDecl*
ElbParameter::elem_by_index(int index) const
{
  return NULL;
}

// @brief ビット要素を返す．
// @param[in] offset オフセット
// @retval ビット要素 offset が範囲内の場合
// @retval NULL offset が範囲外の場合．またはビット展開されていない場合
// @note このクラスでは NULL を返す．
const VlDecl*
ElbParameter::elem_by_offset(ymuint offset) const
{
  return NULL;
}

// @brief net 型の取得
// @retval net 型 net 型の要素の場合
// @retval kVpiNone net 型の要素でない場合
tVpiNetType
ElbParameter::net_type() const
{
  return kVpiNone;
}

// @brief vectored|scalared 属性の取得
// @retval kVpiVsNone vectored|scalared 指定なし
// @retval kVpiVectored vectored 指定あり
// @retval kVpiScalared scalared 指定あり
tVpiVsType
ElbParameter::vs_type() const
{
  return kVpiVsNone;
}

// @brief drive0 strength の取得
// @retval 0 の強度
// @retval kVpiNoStrength strength の指定なし
tVpiStrength
ElbParameter::drive0() const
{
  return kVpiNoStrength;
}

// @brief drive1 strength の取得
// @retval 1 の強度
// @retval kVpiNoStrength strength の指定なし
tVpiStrength
ElbParameter::drive1() const
{
  return kVpiNoStrength;
}

// @brief charge strength の取得
// @retval 電荷の強度
// @retval kVpiNoStrength strength の指定なし
tVpiStrength
ElbParameter::charge() const
{
  return kVpiNoStrength;
}

// @brief delay の取得
// @retval delay
// @retval NULL delay の指定なし
const VlDelay*
ElbParameter::delay() const
{
  return NULL;
}

// @brief 配列型オブジェクトの時に true を返す．
bool
ElbParameter::is_array() const
{
  return false;
}

// @brief 多次元の配列型オブジェクトの時に true を返す．
bool
ElbParameter::is_multi_array() const
{
  return false;
}

// @brief 配列型オブジェクトの場合の次元数の取得
// @note このクラスでは 0 を返す．
ymuint32
ElbParameter::dimension() const
{
  return 0;
}

// @brief 範囲の取得
// @param[in] pos 位置 ( 0 <= pos < dimension() )
// @note このクラスでは NULL を返す．
const VlRange*
ElbParameter::range(ymuint32 pos) const
{
  return NULL;
}

// @brief 配列要素の時に true を返す．
// @note このクラスでは false を返す．
bool
ElbParameter::is_array_member() const
{
  return false;
}

// @brief 多次元の配列要素の時に true を返す．
bool
ElbParameter::is_multi_array_member() const
{
  return false;
}

// @brief 配列要素の時に親の配列を返す．
VlDecl*
ElbParameter::parent_array() const
{
  return NULL;
}

// @brief 1次元配列要素の時にインデックスを返す．
const VlExpr*
ElbParameter::index() const
{
  return NULL;
}

// @brief 多次元配列要素の時にインデックスのリストを返す．
// @param[out] index_list インデックスのリストを格納する変数
void
ElbParameter::index(vector<const VlExpr*>& index_list) const
{
  index_list.clear();
}

// @brief 符号付きに補正する．
// @note このクラスでは何もしない．
void
ElbParameter::set_signed()
{
}

// @brief スカラー値を設定する．
// @param[in] val 値
// @note このクラスでは何もしない．
void
ElbParameter::set_scalar(tVpiScalarVal val)
{
}

// @brief real 型の値を設定する．
// @param[in] val 値
// @note このクラスでは何もしない．
void
ElbParameter::set_real(double val)
{
}

// @brief bitvector 型の値を設定する．
// @param[in] val 値
// @note このクラスでは何もしない．
void
ElbParameter::set_bitvector(const BitVector& val)
{
}

// @brief ビット値を設定する．
// @param[in] index ビット位置
// @param[in] val 値
// @note このクラスでは何もしない．
void
ElbParameter::set_bitselect(int index,
			    tVpiScalarVal val)
{
}

// @brief 範囲値を設定する．
// @param[in] left 範囲の MSB
// @param[in] right 範囲の LSB
// @param[in] val 値
// @note このクラスでは何もしない．
void
ElbParameter::set_partselect(int left,
			     int right,
			     const BitVector& val)
{
}

END_NAMESPACE_YM_VERILOG
