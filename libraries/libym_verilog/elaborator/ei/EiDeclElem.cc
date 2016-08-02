
/// @file libym_verilog/elaborator/ei/EiDeclElem.cc
/// @brief EiDeclElem の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiPrimary.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiDeclElem.h"
#include "ElbExpr.h"

#include "ym_verilog/pt/PtBase.h"

#include "ym_verilog/BitVector.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief 宣言要素を生成する(配列の要素)．
// @param[in] pt_expr パース木の定義要素
// @param[in] parent_array 親の配列
// @param[in] index_list インデックスのリスト
ElbDecl*
EiFactory::new_DeclElem(const PtBase* pt_expr,
			ElbDeclArray* parent_array,
			const vector<ElbExpr*>& index_list)
{
  void* p = mAlloc.get_memory(sizeof(EiDeclElem));
  ElbDecl* decl = new (p) EiDeclElem(pt_expr, parent_array, index_list);

  return decl;
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeclElem
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] parent_array 親の配列
// @param[in] index_list インデックスのリスト
EiDeclElem::EiDeclElem(const PtBase* pt_expr,
		       ElbDeclArray* parent_array,
		       const vector<ElbExpr*>& index_list) :
  mPtExpr(pt_expr),
  mArray(parent_array),
  mIndexList(index_list)
{
}

// @brief デストラクタ
EiDeclElem::~EiDeclElem()
{
}

// @brief 型の取得
tVpiObjType
EiDeclElem::type() const
{
  return mArray->elem_type();
}

// @brief ファイル位置を返す．
FileRegion
EiDeclElem::file_region() const
{
  return mPtExpr->file_region();
}

// @brief このオブジェクトの属しているスコープを返す．
const VlNamedObj*
EiDeclElem::parent() const
{
  return mArray->parent();
}

// @brief 名前の取得
const char*
EiDeclElem::name() const
{
  return mArray->name();
}

// @brief 式のタイプを返す．
tVpiValueType
EiDeclElem::value_type() const
{
  return mArray->value_type();
}

// @brief 符号の取得
// @retval true 符号つき
// @retval false 符号なし
bool
EiDeclElem::is_signed() const
{
  return mArray->is_signed();
}

// @brief MSB の値を返す．
// @retval 範囲のMSBの値 範囲指定を持つとき
// @retval -1 範囲指定を持たないとき
int
EiDeclElem::left_range_const() const
{
  return mArray->left_range_const();
}

// @brief LSB の値を返す．
// @retval 範囲のLSBの値 範囲指定を持つとき
// @retval -1 範囲指定を持たないとき
int
EiDeclElem::right_range_const() const
{
  return mArray->right_range_const();
}

// @brief ビット幅を返す．
ymuint32
EiDeclElem::bit_size() const
{
  return mArray->bit_size();
}

// @brief オフセット値の取得
// @param[in] index インデックス
// @retval index に対するオフセット値 index が範囲内に入っている時．
// @retval -1 index が範囲外の時
int
EiDeclElem::bit_offset(int index) const
{
  return mArray->bit_offset(index);
}

// @brief データ型の取得
// @retval データ型 kParam, kLocalParam, kVar の場合
// @retval kVpiVarNone 上記以外
tVpiVarType
EiDeclElem::data_type() const
{
  return mArray->data_type();
}

// @brief net 型の取得
// @retval net 型 net 型の要素の場合
// @retval kVpiNone net 型の要素でない場合
tVpiNetType
EiDeclElem::net_type() const
{
  return mArray->net_type();
}

// @brief vectored|scalared 属性の取得
// @retval kVpiVsNone vectored|scalared 指定なし
// @retval kVpiVectored vectored 指定あり
// @retval kVpiScalared scalared 指定あり
tVpiVsType
EiDeclElem::vs_type() const
{
  return mArray->vs_type();
}

// @brief drive0 strength の取得
// @retval 0 の強度
// @retval kVpiNoStrength strength の指定なし
tVpiStrength
EiDeclElem::drive0() const
{
  return mArray->drive0();
}

// @brief drive1 strength の取得
// @retval 1 の強度
// @retval kVpiNoStrength strength の指定なし
tVpiStrength
EiDeclElem::drive1() const
{
  return mArray->drive1();
}

// @brief charge strength の取得
// @retval 電荷の強度
// @retval kVpiNoStrength strength の指定なし
tVpiStrength
EiDeclElem::charge() const
{
  return mArray->charge();
}

// @brief delay の取得
// @retval delay
// @retval NULL delay の指定なし
const VlDelay*
EiDeclElem::delay() const
{
  return mArray->delay();
}

// @brief 配列要素の時に true を返す．
bool
EiDeclElem::is_array_member() const
{
  return true;
}

// @brief 多次元の配列要素の時に true を返す．
bool
EiDeclElem::is_multi_array_member() const
{
  return mIndexList.size() > 1;
}

// @brief 配列要素の時に親の配列を返す．
VlDecl*
EiDeclElem::parent_array() const
{
  return mArray;
}

// @brief 1次元配列要素の時にインデックスを返す．
const VlExpr*
EiDeclElem::index() const
{
  return mIndexList[0];
}

// @brief 多次元配列要素の時にインデックスのリストを返す．
// @param[out] index_list インデックスのリストを格納する変数
void
EiDeclElem::index(vector<const VlExpr*>& index_list) const
{
  index_list.clear();
  ymuint n = mIndexList.size();
  index_list.resize(n);
  for (ymuint i = 0; i < n; ++ i) {
    index_list[i] = mIndexList[i];
  }
}

// @brief 符号付きに補正する．
// @note このクラスで呼ばれることはない．
void
EiDeclElem::set_signed()
{
  assert_not_reached(__FILE__, __LINE__);
}

// @brief スカラー値を返す．
tVpiScalarVal
EiDeclElem::get_scalar() const
{
  ymuint offset = calc_offset();
  return mArray->get_scalar(offset);
}

// @brief スカラー値を設定する．
// @param[in] val 値
void
EiDeclElem::set_scalar(tVpiScalarVal val)
{
  ymuint offset = calc_offset();
  mArray->set_scalar(offset, val);
}

// @brief 論理値を返す．
tVpiScalarVal
EiDeclElem::get_logic() const
{
  ymuint offset = calc_offset();
  return mArray->get_logic(offset);
}

// @brief real 型の値を返す．
double
EiDeclElem::get_real() const
{
  ymuint offset = calc_offset();
  return mArray->get_real(offset);
}

// @brief real 型の値を設定する．
// @param[in] val 値
void
EiDeclElem::set_real(double val)
{
  ymuint offset = calc_offset();
  mArray->set_real(offset, val);
}

// @brief bitvector 型の値を返す．
// @param[out] val 値
// @param[in] req_type 要求される型
void
EiDeclElem::get_bitvector(BitVector& val,
			  tVpiValueType req_type) const
{
  ymuint offset = calc_offset();
  mArray->get_bitvector(offset, val, req_type);
}

// @brief bitvector 型の値を設定する．
// @param[in] val 値
void
EiDeclElem::set_bitvector(const BitVector& val)
{
  ymuint offset = calc_offset();
  mArray->set_bitvector(offset, val);
}

// @brief ビット選択値を返す．
// @param[in] index ビット位置
tVpiScalarVal
EiDeclElem::get_bitselect(int index) const
{
  ymuint offset = calc_offset();
  return mArray->get_bitselect(offset, index);
}

// @brief ビット値を設定する．
// @param[in] index ビット位置
// @param[in] val 値
void
EiDeclElem::set_bitselect(int index,
			  tVpiScalarVal val)
{
  ymuint offset = calc_offset();
  mArray->set_bitselect(offset, index, val);
}

// @brief 範囲選択値を返す．
// @param[in] left 範囲の MSB
// @param[in] right 範囲の LSB
// @param[out] val 値
void
EiDeclElem::get_partselect(int left,
			   int right,
			   BitVector& val) const
{
  ymuint offset = calc_offset();
  mArray->get_partselect(offset, left, right, val);
}

// @brief 範囲値を設定する．
// @param[in] left 範囲の MSB
// @param[in] right 範囲の LSB
// @param[in] val 値
void
EiDeclElem::set_partselect(int left,
			   int right,
			   const BitVector& val)
{
  ymuint offset = calc_offset();
  mArray->set_partselect(offset, left, right, val);
}

// @brief 範囲のMSBの取得
// @retval 範囲のMSB 範囲を持つとき
// @retval NULL 範囲を持たないとき
ElbExpr*
EiDeclElem::_left_range() const
{
  return mArray->_left_range();
}

// @brief 範囲のLSBの取得
// @retval 範囲のLSB 範囲を持つとき
// @retval NULL 範囲を持たないとき
ElbExpr*
EiDeclElem::_right_range() const
{
  return mArray->_right_range();
}

// @brief インデックス式を評価してオフセットを計算する．
ymuint
EiDeclElem::calc_offset() const
{
  return mArray->calc_offset(mIndexList);
}

END_NAMESPACE_YM_VERILOG
