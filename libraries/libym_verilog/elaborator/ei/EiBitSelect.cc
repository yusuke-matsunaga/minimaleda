
/// @file libym_verilog/elb_impl/EiBitSelect.cc
/// @brief EiBitSelect の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiBitSelect.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiBitSelect.h"
#include "ElbDecl.h"

#include "ym_verilog/BitVector.h"



BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief ビット選択式を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
// @param[in] bit_index ビット選択式
ElbExpr*
EiFactory::new_BitSelect(const PtBase* pt_expr,
			 ElbDecl* obj,
			 ElbExpr* bit_index)
{
  void* p = mAlloc.get_memory(sizeof(EiDeclBitSelect));
  ElbExpr* expr = new (p) EiDeclBitSelect(pt_expr, obj, bit_index);

  return expr;
}

// @brief ビット選択式を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] expr 本体の式
// @param[in] bit_index ビット位置
ElbExpr*
EiFactory::new_BitSelect(const PtBase* pt_expr,
			 ElbExpr* expr,
			 int bit_index)
{
  void* p = mAlloc.get_memory(sizeof(EiExprBitSelect));
  EiExprBitSelect* expr1 = new (p) EiExprBitSelect(pt_expr, expr, bit_index);

  return expr1;
}


//////////////////////////////////////////////////////////////////////
// クラス EiBitSelect
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] bit_index ビット選択式
EiBitSelect::EiBitSelect(const PtBase* pt_expr,
			 ElbExpr* bit_index) :
  EiExprBase1(pt_expr),
  mIndex(bit_index)
{
}

// @brief デストラクタ
EiBitSelect::~EiBitSelect()
{
}

// @brief 型の取得
tVpiObjType
EiBitSelect::type() const
{
  switch ( decl_obj()->type() ) {
  case kVpiNet: return kVpiNetBit;
  case kVpiReg: return kVpiRegBit;
  default: break;
  }
  return kVpiBitSelect;
}

// @brief 式のタイプを返す．
tVpiValueType
EiBitSelect::value_type() const
{
  return pack(kVpiValueUS, 1);
}

// @brief 定数の時 true を返す．
// @note 参照している要素の型によって決まる．
bool
EiBitSelect::is_const() const
{
  return decl_obj()->is_consttype() && mIndex->is_const();
}

// @brief 固定選択子の時 true を返す．
// @note ビット選択，部分選択の時，意味を持つ．
bool
EiBitSelect::is_constant_select() const
{
  return mIndex->is_const();
}

// @brief ビット指定の時に true を返す．
bool
EiBitSelect::is_bitselect() const
{
  return true;
}

// @brief インデックス式を返す．
const VlExpr*
EiBitSelect::index() const
{
  return mIndex;
}

// @brief インデックス値を返す．
// @note 式に対するビット選択の時，意味を持つ．
int
EiBitSelect::index_val() const
{
  int ans;
  if ( mIndex->eval_int(ans) ) {
    return ans;
  }
  return 0;
}

// @brief 論理値を返す．
tVpiScalarVal
EiBitSelect::eval_logic() const
{
  return eval_scalar();
}

// @brief real 型の値を返す．
double
EiBitSelect::eval_real() const
{
  return conv_to_real(eval_scalar());
}

// @brief bitvector 型の値を返す．
void
EiBitSelect::eval_bitvector(BitVector& bitvector,
			    tVpiValueType req_type) const
{
  bitvector = eval_scalar();
  bitvector.coerce(req_type);
}

// @brief decompile() の実装関数
// @param[in] pprim 親の演算子の優先順位
string
EiBitSelect::decompile_impl(int ppri) const
{
  string ans = decl_obj()->name();
  ans += "[" + mIndex->decompile() + "]";
  return ans;
}

// @brief 要求される式の型を計算してセットする．
// @param[in] type 要求される式の型
// @note 必要であればオペランドに対して再帰的に処理を行なう．
void
EiBitSelect::set_reqsize(tVpiValueType type)
{
  // なにもしない．
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeclBitSelect
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
// @param[in] bit_index ビット選択式
EiDeclBitSelect::EiDeclBitSelect(const PtBase* pt_expr,
				 ElbDecl* obj,
				 ElbExpr* bit_index) :
  EiBitSelect(pt_expr, bit_index),
  mObj(obj)
{
}

// @brief デストラクタ
EiDeclBitSelect::~EiDeclBitSelect()
{
}

// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
const VlDecl*
EiDeclBitSelect::decl_obj() const
{
  return mObj;
}

// @brief スカラー値を返す．
tVpiScalarVal
EiDeclBitSelect::eval_scalar() const
{
  int bpos = index_val();
  if ( index()->eval_int(bpos) ) {
    return mObj->get_bitselect(bpos);
  }
  // ビット指定が X/Z の時は X を返す．
  return kVpiScalarX;
}

// @brief スカラー値を書き込む．
// @param[in] v 書き込む値
// @note 左辺式の時のみ意味を持つ．
void
EiDeclBitSelect::set_scalar(tVpiScalarVal v)
{
  int bpos;
  if ( index()->eval_int(bpos) ) {
    mObj->set_bitselect(bpos, v);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス EiExprBitSelect
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] expr 本体の式
// @param[in] bit_index ビット位置
EiExprBitSelect::EiExprBitSelect(const PtBase* pt_expr,
				 ElbExpr* expr,
				 int bit_index) :
  EiExprBase1(pt_expr),
  mExpr(expr),
  mIndex(bit_index)
{
}

// @brief デストラクタ
EiExprBitSelect::~EiExprBitSelect()
{
}

// @brief 型の取得
tVpiObjType
EiExprBitSelect::type() const
{
  return kVpiBitSelect;
}

// @brief 式のタイプを返す．
tVpiValueType
EiExprBitSelect::value_type() const
{
  return pack(kVpiValueUS, 1);
}

// @brief 定数の時 true を返す．
// @note 参照している要素の型によって決まる．
bool
EiExprBitSelect::is_const() const
{
  return mExpr->is_const();
}

// @brief ビット指定の時に true を返す．
bool
EiExprBitSelect::is_bitselect() const
{
  return true;
}

// @brief 親の式を返す．
// @note 正確には式に対するビット選択/部分選択の時のみ意味を持つ．
const VlExpr*
EiExprBitSelect::parent_expr() const
{
  return mExpr;
}

// @brief インデックス値を返す．
// @note 式に対するビット選択の時，意味を持つ．
int
EiExprBitSelect::index_val() const
{
  return mIndex;
}

// @brief スカラー値を返す．
tVpiScalarVal
EiExprBitSelect::eval_scalar() const
{
  BitVector tmp;
  mExpr->eval_bitvector(tmp);
  return tmp.bit_select(mIndex);
}

// @brief 論理値を返す．
tVpiScalarVal
EiExprBitSelect::eval_logic() const
{
  return eval_scalar();
}

// @brief real 型の値を返す．
double
EiExprBitSelect::eval_real() const
{
  return conv_to_real(eval_scalar());
}

// @brief bitvector 型の値を返す．
void
EiExprBitSelect::eval_bitvector(BitVector& bitvector,
				tVpiValueType req_type) const
{
  bitvector = eval_scalar();
  bitvector.coerce(req_type);
}

// @brief decompile() の実装関数
// @param[in] pprim 親の演算子の優先順位
string
EiExprBitSelect::decompile_impl(int ppri) const
{
  ostringstream buf;
  buf << "(" << mExpr->decompile() << ")" << "[" << mIndex << "]";
  return buf.str();
}

// @brief 要求される式の型を計算してセットする．
// @param[in] type 要求される式の型
// @note 必要であればオペランドに対して再帰的に処理を行なう．
void
EiExprBitSelect::set_reqsize(tVpiValueType type)
{
  // なにもしない．
}

END_NAMESPACE_YM_VERILOG
