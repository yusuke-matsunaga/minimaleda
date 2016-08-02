
/// @file libym_verilog/elb_impl/EiPartSelect.cc
/// @brief EiPartSelect の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiPartSelect.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiPartSelect.h"
#include "ElbDecl.h"
#include "ElbPrimitive.h"

#include "ym_verilog/BitVector.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief 部分選択式を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
// @param[in] index1, inde2 パート選択式
// @param[in] index1_val, index2_val パート選択式の値
ElbExpr*
EiFactory::new_PartSelect(const PtBase* pt_expr,
			  ElbDecl* obj,
			  ElbExpr* index1,
			  ElbExpr* index2,
			  int index1_val,
			  int index2_val)
{
  void* p = mAlloc.get_memory(sizeof(EiDeclPartSelect));
  ElbExpr* expr = new (p) EiDeclPartSelect(pt_expr, obj,
					   index1, index2,
					   index1_val, index2_val);

  return expr;
}

// @brief 部分選択式を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] expr 本体の式
// @param[in] index1, inde2 パート選択式
ElbExpr*
EiFactory::new_PartSelect(const PtBase* pt_expr,
			  ElbExpr* expr,
			  int index1,
			  int index2)
{
  void* p = mAlloc.get_memory(sizeof(EiExprPartSelect));
  ElbExpr* expr1 = new (p) EiExprPartSelect(pt_expr, expr,
					    index1, index2);

  return expr1;
}


//////////////////////////////////////////////////////////////////////
// クラス EiPartSelect
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] index1, inde2 パート選択式
// @param[in] index1_val, index2_val パート選択式の値
EiPartSelect::EiPartSelect(const PtBase* pt_expr,
			   ElbExpr* index1,
			   ElbExpr* index2,
			   int index1_val,
			   int index2_val) :
  EiExprBase1(pt_expr),
  mIndex1(index1),
  mIndex2(index2),
  mIndex1Val(index1_val),
  mIndex2Val(index2_val)
{
}

// @brief デストラクタ
EiPartSelect::~EiPartSelect()
{
}

// @brief 型の取得
tVpiObjType
EiPartSelect::type() const
{
  return kVpiPartSelect;
}

// @brief 式のタイプを返す．
tVpiValueType
EiPartSelect::value_type() const
{
  int w = 0;
  if ( mIndex1Val > mIndex2Val ) {
    w = mIndex1Val - mIndex2Val + 1;
  }
  else {
    w = mIndex2Val - mIndex1Val + 1;
  }
  return pack(kVpiValueUS, w);
}

// @brief 定数の時 true を返す．
// @note 参照している要素の型によって決まる．
bool
EiPartSelect::is_const() const
{
  return decl_obj()->is_consttype();
}

// @brief 固定選択子の時 true を返す．
// @note ビット選択，部分選択の時，意味を持つ．
bool
EiPartSelect::is_constant_select() const
{
  return true;
}

// @brief 範囲指定の時に true を返す．
bool
EiPartSelect::is_partselect() const
{
  return true;
}

// @brief 範囲指定のモードを返す．
tVpiRangeMode
EiPartSelect::range_mode() const
{
  return kVpiConstRange;
}

// @brief 範囲の MSB の式を返す．
// @note 通常の範囲選択の時，意味を持つ．
const VlExpr*
EiPartSelect::left_range() const
{
  return mIndex1;
}

// @brief 範囲の LSB の式を返す．
// @note 通常の範囲選択の時，意味を持つ．
const VlExpr*
EiPartSelect::right_range() const
{
  return mIndex2;
}

// @brief 範囲の MSB の値を返す．
// @note 式に対する範囲選択の時，意味を持つ．
int
EiPartSelect::left_range_val() const
{
  return mIndex1Val;
}

// @brief 範囲の LSB の値を返す．
// @note 式に対する範囲選択の時，意味を持つ．
int
EiPartSelect::right_range_val() const
{
  return mIndex2Val;
}

// @brief スカラー値を返す．
tVpiScalarVal
EiPartSelect::eval_scalar() const
{
  BitVector tmp;
  eval_bitvector(tmp);
  return tmp.bit_select(0);
}

// @brief 論理値を返す．
tVpiScalarVal
EiPartSelect::eval_logic() const
{
  BitVector tmp;
  eval_bitvector(tmp);
  return tmp.to_logic();
}

// @brief real 型の値を返す．
double
EiPartSelect::eval_real() const
{
  BitVector tmp;
  eval_bitvector(tmp);
  return tmp.to_real();
}

// @brief decompile() の実装関数
// @param[in] pprim 親の演算子の優先順位
string
EiPartSelect::decompile_impl(int ppri) const
{
  string ans = decl_obj()->name();
  ans += "[" + mIndex1->decompile() + ":"
    + mIndex2->decompile() + "]";
  return ans;
}

// @brief 要求される式の型を計算してセットする．
// @param[in] type 要求される式の型
// @note 必要であればオペランドに対して再帰的に処理を行なう．
void
EiPartSelect::set_reqsize(tVpiValueType type)
{
  // なにもしない．
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeclPartSelect
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
// @param[in] index1, inde2 パート選択式
// @param[in] index1_val, index2_val パート選択式の値
EiDeclPartSelect::EiDeclPartSelect(const PtBase* pt_expr,
				   ElbDecl* obj,
				   ElbExpr* index1,
				   ElbExpr* index2,
				   int index1_val,
				   int index2_val) :
  EiPartSelect(pt_expr, index1, index2, index1_val, index2_val),
  mObj(obj)
{
}

// @brief デストラクタ
EiDeclPartSelect::~EiDeclPartSelect()
{
}

// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
const VlDecl*
EiDeclPartSelect::decl_obj() const
{
  return mObj;
}

// @brief bitvector 型の値を返す．
void
EiDeclPartSelect::eval_bitvector(BitVector& bitvector,
				 tVpiValueType req_type) const
{
  mObj->get_partselect(left_range_val(), right_range_val(), bitvector);
  bitvector.coerce(req_type);
}

// @brief スカラー値を書き込む．
// @param[in] v 書き込む値
// @note 左辺式の時のみ意味を持つ．
void
EiDeclPartSelect::set_scalar(tVpiScalarVal v)
{
  int msb = left_range_val();
  assert_cond( msb == right_range_val(), __FILE__, __LINE__);
  mObj->set_bitselect(msb, v);
}

// @brief ビットベクタを書き込む．
// @param[in] v 書き込む値
// @note 左辺式の時のみ意味を持つ．
void
EiDeclPartSelect::set_bitvector(const BitVector& v)
{
  int msb = left_range_val();
  int lsb = right_range_val();
  mObj->set_partselect(lsb, msb, v);
}


//////////////////////////////////////////////////////////////////////
// クラス EiExprPartSelect
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] expr 本体の式
// @param[in] index1, index2 パート選択式
EiExprPartSelect::EiExprPartSelect(const PtBase* pt_expr,
				   ElbExpr* expr,
				   int index1,
				   int index2) :
  EiExprBase1(pt_expr),
  mExpr(expr),
  mIndex1(index1),
  mIndex2(index2)
{
}

// @brief デストラクタ
EiExprPartSelect::~EiExprPartSelect()
{
}

// @brief 型の取得
tVpiObjType
EiExprPartSelect::type() const
{
  return kVpiPartSelect;
}

// @brief 式のタイプを返す．
tVpiValueType
EiExprPartSelect::value_type() const
{
  int w = 0;
  if ( mIndex1 > mIndex2 ) {
    w = mIndex1 - mIndex2 + 1;
  }
  else {
    w = mIndex2 - mIndex1 + 1;
  }
  return pack(kVpiValueUS, w);
}

// @brief 定数の時 true を返す．
// @note 参照している要素の型によって決まる．
bool
EiExprPartSelect::is_const() const
{
  return mExpr->is_const();
}

// @brief 固定選択子の時 true を返す．
// @note ビット選択，部分選択の時，意味を持つ．
bool
EiExprPartSelect::is_constant_select() const
{
  return true;
}

// @brief 範囲指定の時に true を返す．
bool
EiExprPartSelect::is_partselect() const
{
  return true;
}

// @brief 範囲指定のモードを返す．
tVpiRangeMode
EiExprPartSelect::range_mode() const
{
  return kVpiConstRange;
}

// @brief 親の式を返す．
// @note 式に対するビット選択/範囲選択の時，意味を持つ．
const VlExpr*
EiExprPartSelect::parent_expr() const
{
  return mExpr;
}

// @brief 範囲の MSB の値を返す．
// @note 式に対する範囲選択の時，意味を持つ．
int
EiExprPartSelect::left_range_val() const
{
  return mIndex1;
}

// @brief 範囲の LSB の値を返す．
// @note 式に対する範囲選択の時，意味を持つ．
int
EiExprPartSelect::right_range_val() const
{
  return mIndex2;
}

// @brief スカラー値を返す．
tVpiScalarVal
EiExprPartSelect::eval_scalar() const
{
  BitVector tmp;
  eval_bitvector(tmp);
  return tmp.bit_select(0);
}

// @brief 論理値を返す．
tVpiScalarVal
EiExprPartSelect::eval_logic() const
{
  BitVector tmp;
  eval_bitvector(tmp);
  return tmp.to_logic();
}

// @brief real 型の値を返す．
double
EiExprPartSelect::eval_real() const
{
  BitVector tmp;
  eval_bitvector(tmp);
  return tmp.to_real();
}

// @brief bitvector 型の値を返す．
void
EiExprPartSelect::eval_bitvector(BitVector& bitvector,
				 tVpiValueType req_type) const
{
  BitVector tmp;
  mExpr->eval_bitvector(tmp);
  tmp.part_select(mIndex1, mIndex2, bitvector);
  bitvector.coerce(req_type);
}

// @brief decompile() の実装関数
// @param[in] pprim 親の演算子の優先順位
string
EiExprPartSelect::decompile_impl(int ppri) const
{
  ostringstream buf;
  buf << "(" << mExpr->decompile() << ")"
      << "[" << mIndex1 << ":" << mIndex2 << "]";
  return buf.str();
}

// @brief 要求される式の型を計算してセットする．
// @param[in] type 要求される式の型
// @note 必要であればオペランドに対して再帰的に処理を行なう．
void
EiExprPartSelect::set_reqsize(tVpiValueType type)
{
  // なにもしない．
}

END_NAMESPACE_YM_VERILOG
