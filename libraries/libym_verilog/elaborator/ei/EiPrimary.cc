
/// @file libym_verilog/elb_impl/EiPrimary.cc
/// @brief EiPrimary の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiPrimary.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiPrimary.h"
#include "ElbDecl.h"
#include "ElbPrimitive.h"

#include "ym_verilog/BitVector.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief プライマリ式を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
ElbExpr*
EiFactory::new_Primary(const PtBase* pt_expr,
		       ElbDecl* obj)
{
  void* p = mAlloc.get_memory(sizeof(EiPrimary));
  EiPrimary* expr = new (p) EiPrimary(pt_expr, obj);

  return expr;
}

// @brief システム関数/システムタスクの引数を生成する．
// @param[in] pt_expr パース木中で参照している要素
// @param[in] arg 引数本体
ElbExpr*
EiFactory::new_ArgHandle(const PtBase* pt_expr,
			 const VlNamedObj* arg)
{
  void* p = mAlloc.get_memory(sizeof(EiScopePrimary));
  EiScopePrimary* expr = new (p) EiScopePrimary(pt_expr, arg);

  return expr;
}

// @brief システム関数/システムタスクの引数を生成する．
// @param[in] pt_expr パース木中で参照している要素
// @param[in] arg 引数本体
ElbExpr*
EiFactory::new_ArgHandle(const PtBase* pt_expr,
			 ElbPrimitive* arg)
{
  void* p = mAlloc.get_memory(sizeof(EiPrimitivePrimary));
  EiPrimitivePrimary* expr = new (p) EiPrimitivePrimary(pt_expr, arg);

  return expr;
}


//////////////////////////////////////////////////////////////////////
// クラス EiPrimary
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
EiPrimary::EiPrimary(const PtBase* pt_obj,
		     ElbDecl* obj) :
  EiExprBase1(pt_obj),
  mObj(obj)
{
}

// @brief デストラクタ
EiPrimary::~EiPrimary()
{
}

// @brief 型の取得
tVpiObjType
EiPrimary::type() const
{
  return mObj->type();
}

// @brief 式のタイプを返す．
tVpiValueType
EiPrimary::value_type() const
{
  return mObj->value_type();
}

// @brief 定数の時 true を返す．
// @note 参照している要素の型によって決まる．
bool
EiPrimary::is_const() const
{
  return mObj->is_consttype();
}

// @brief プライマリ(net/reg/variables/parameter)の時に true を返す．
bool
EiPrimary::is_primary() const
{
  return true;
}

// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
// @note 宣言要素に対するビット選択，部分選択の場合にも意味を持つ．
const VlDecl*
EiPrimary::decl_obj() const
{
  return mObj;
}

// @brief スカラー値を返す．
tVpiScalarVal
EiPrimary::eval_scalar() const
{
  return mObj->get_scalar();
}

// @brief 論理値を返す．
tVpiScalarVal
EiPrimary::eval_logic() const
{
  return mObj->get_scalar();
}

// @brief real 型の値を返す．
double
EiPrimary::eval_real() const
{
  return mObj->get_real();
}

// @brief bitvector 型の値を返す．
void
EiPrimary::eval_bitvector(BitVector& bitvector,
			  tVpiValueType req_type) const
{
  mObj->get_bitvector(bitvector, req_type);
}

// @brief decompile() の実装関数
// @param[in] pprim 親の演算子の優先順位
string
EiPrimary::decompile_impl(int ppri) const
{
  return mObj->name();
}

// @brief 要求される式の型を計算してセットする．
// @param[in] type 要求される式の型
// @note 必要であればオペランドに対して再帰的に処理を行なう．
void
EiPrimary::set_reqsize(tVpiValueType type)
{
  // なにもしない．
}

// @brief スカラー値を書き込む．
// @param[in] v 書き込む値
// @note 左辺式の時のみ意味を持つ．
void
EiPrimary::set_scalar(tVpiScalarVal v)
{
  mObj->set_scalar(v);
}

// @brief 実数値を書き込む．
// @param[in] v 書き込む値
// @note 左辺式の時のみ意味を持つ．
void
EiPrimary::set_real(double v)
{
  mObj->set_real(v);
}

// @brief ビットベクタを書き込む．
// @param[in] v 書き込む値
// @note 左辺式の時のみ意味を持つ．
void
EiPrimary::set_bitvector(const BitVector& v)
{
  mObj->set_bitvector(v);
}


//////////////////////////////////////////////////////////////////////
// クラス EiScopePrimary
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
EiScopePrimary::EiScopePrimary(const PtBase* pt_obj,
			       const VlNamedObj* obj) :
  EiExprBase1(pt_obj),
  mObj(obj)
{
}

// @brief デストラクタ
EiScopePrimary::~EiScopePrimary()
{
}

// @brief 型の取得
tVpiObjType
EiScopePrimary::type() const
{
  return mObj->type();
}

// @brief 式のタイプを返す．
tVpiValueType
EiScopePrimary::value_type() const
{
  return kVpiValueNone;
}

// @brief 定数の時 true を返す．
// @note 参照している要素の型によって決まる．
bool
EiScopePrimary::is_const() const
{
  return false;
}

// @brief スカラー値を返す．
tVpiScalarVal
EiScopePrimary::eval_scalar() const
{
  return kVpiScalarX;
}

// @brief 論理値を返す．
tVpiScalarVal
EiScopePrimary::eval_logic() const
{
  return kVpiScalarX;
}

// @brief real 型の値を返す．
double
EiScopePrimary::eval_real() const
{
  return 0.0;
}

// @brief bitvector 型の値を返す．
void
EiScopePrimary::eval_bitvector(BitVector& bitvector,
			       tVpiValueType req_type) const
{
}

// @brief decompile() の実装関数
// @param[in] pprim 親の演算子の優先順位
string
EiScopePrimary::decompile_impl(int ppri) const
{
  return mObj->name();
}

// @brief 対象のオブジェクトを返す．
const VlNamedObj*
EiScopePrimary::scope_obj() const
{
  return mObj;
}

// @brief 要求される式の型を計算してセットする．
// @param[in] type 要求される式の型
// @note 必要であればオペランドに対して再帰的に処理を行なう．
void
EiScopePrimary::set_reqsize(tVpiValueType type)
{
  // なにもしない．
}

// @brief スカラー値を書き込む．
// @param[in] v 書き込む値
// @note 左辺式の時のみ意味を持つ．
void
EiScopePrimary::set_scalar(tVpiScalarVal v)
{
  assert_not_reached(__FILE__, __LINE__);
}

// @brief 実数値を書き込む．
// @param[in] v 書き込む値
// @note 左辺式の時のみ意味を持つ．
void
EiScopePrimary::set_real(double v)
{
  assert_not_reached(__FILE__, __LINE__);
}

// @brief ビットベクタを書き込む．
// @param[in] v 書き込む値
// @note 左辺式の時のみ意味を持つ．
void
EiScopePrimary::set_bitvector(const BitVector& v)
{
  assert_not_reached(__FILE__, __LINE__);
}


//////////////////////////////////////////////////////////////////////
// クラス EiPrimitivePrimary
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
EiPrimitivePrimary::EiPrimitivePrimary(const PtBase* pt_obj,
				       ElbPrimitive* obj) :
  EiExprBase1(pt_obj),
  mObj(obj)
{
}

// @brief デストラクタ
EiPrimitivePrimary::~EiPrimitivePrimary()
{
}

// @brief 型の取得
tVpiObjType
EiPrimitivePrimary::type() const
{
  return mObj->type();
}

// @brief 式のタイプを返す．
tVpiValueType
EiPrimitivePrimary::value_type() const
{
  return kVpiValueNone;
}

// @brief 定数の時 true を返す．
// @note 参照している要素の型によって決まる．
bool
EiPrimitivePrimary::is_const() const
{
  return false;
}

// @brief スカラー値を返す．
tVpiScalarVal
EiPrimitivePrimary::eval_scalar() const
{
  return kVpiScalarX;
}

// @brief 論理値を返す．
tVpiScalarVal
EiPrimitivePrimary::eval_logic() const
{
  return kVpiScalarX;
}

// @brief real 型の値を返す．
double
EiPrimitivePrimary::eval_real() const
{
  return 0.0;
}

// @brief bitvector 型の値を返す．
void
EiPrimitivePrimary::eval_bitvector(BitVector& bitvector,
				   tVpiValueType req_type) const
{
}

// @brief decompile() の実装関数
// @param[in] pprim 親の演算子の優先順位
string
EiPrimitivePrimary::decompile_impl(int ppri) const
{
  return mObj->name();
}

// @brief 対象のオブジェクトを返す．
const VlPrimitive*
EiPrimitivePrimary::primitive_obj() const
{
  return mObj;
}

// @brief 要求される式の型を計算してセットする．
// @param[in] type 要求される式の型
// @note 必要であればオペランドに対して再帰的に処理を行なう．
void
EiPrimitivePrimary::set_reqsize(tVpiValueType type)
{
  // なにもしない．
}

// @brief スカラー値を書き込む．
// @param[in] v 書き込む値
// @note 左辺式の時のみ意味を持つ．
void
EiPrimitivePrimary::set_scalar(tVpiScalarVal v)
{
  assert_not_reached(__FILE__, __LINE__);
}

// @brief 実数値を書き込む．
// @param[in] v 書き込む値
// @note 左辺式の時のみ意味を持つ．
void
EiPrimitivePrimary::set_real(double v)
{
  assert_not_reached(__FILE__, __LINE__);
}

// @brief ビットベクタを書き込む．
// @param[in] v 書き込む値
// @note 左辺式の時のみ意味を持つ．
void
EiPrimitivePrimary::set_bitvector(const BitVector& v)
{
  assert_not_reached(__FILE__, __LINE__);
}

END_NAMESPACE_YM_VERILOG
