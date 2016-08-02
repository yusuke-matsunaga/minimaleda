
/// @file libym_verilog/elb_impl/EiVarPartSelect.cc
/// @brief EiVarPartSelect の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiVarPartSelect.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiVarPartSelect.h"
#include "ElbDecl.h"

#include "ym_verilog/BitVector.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief 可変部分選択式を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
// @param[in] base 範囲のベースアドレスを表す式
// @param[in] range 範囲を表す式
// @param[in] range_val 範囲の値
ElbExpr*
EiFactory::new_PlusPartSelect(const PtBase* pt_expr,
			      ElbDecl* obj,
			      ElbExpr* base,
			      ElbExpr* range,
			      int range_val)
{
  void* p = mAlloc.get_memory(sizeof(EiDeclPlusPartSelect));
  ElbExpr* expr = new (p) EiDeclPlusPartSelect(pt_expr, obj,
					       base, range, range_val);

  return expr;
}

// @brief 可変部分選択式を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
// @param[in] base 範囲のベースアドレスを表す式
// @param[in] range 範囲を表す式
// @param[in] range_val 範囲の値
ElbExpr*
EiFactory::new_MinusPartSelect(const PtBase* pt_expr,
			      ElbDecl* obj,
			      ElbExpr* base,
			      ElbExpr* range,
			      int range_val)
{
  void* p = mAlloc.get_memory(sizeof(EiDeclMinusPartSelect));
  ElbExpr* expr = new (p) EiDeclMinusPartSelect(pt_expr, obj,
						base, range, range_val);

  return expr;
}


//////////////////////////////////////////////////////////////////////
// クラス EiVarPartSelect
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] base 範囲のベースアドレスを表す式
// @param[in] range 範囲を表す式
// @param[in] range_val 範囲の値
EiVarPartSelect::EiVarPartSelect(const PtBase* pt_expr,
				 ElbExpr* base,
				 ElbExpr* range,
				 int range_val) :
  EiExprBase1(pt_expr),
  mBase(base),
  mRange(range),
  mRangeVal(range_val)
{
}

// @brief デストラクタ
EiVarPartSelect::~EiVarPartSelect()
{
}

// @brief 型の取得
tVpiObjType
EiVarPartSelect::type() const
{
  return kVpiPartSelect;
}

// @brief 式のタイプを返す．
tVpiValueType
EiVarPartSelect::value_type() const
{
  return pack(kVpiValueUS, mRangeVal);
}

// @brief 定数の時 true を返す．
bool
EiVarPartSelect::is_const() const
{
  return decl_obj()->is_consttype() && is_constant_select();
}

// @brief 固定選択子の時 true を返す．
// @note ビット選択，部分選択の時，意味を持つ．
bool
EiVarPartSelect::is_constant_select() const
{
  return mBase->is_const();
}

// @brief 範囲指定の時に true を返す．
bool
EiVarPartSelect::is_partselect() const
{
  return true;
}

// @brief 範囲の LSB の式を返す．
// @note 通常の範囲選択の時，意味を持つ．
// @note それ以外では NULL を返す．
const VlExpr*
EiVarPartSelect::right_range() const
{
  return mBase;
}

// @brief 範囲の LSB の値を返す．
// @note 式に対する範囲選択の時，意味を持つ．
// @note それ以外では 0 を返す．
int
EiVarPartSelect::right_range_val() const
{
  int ans;
  if ( mBase->eval_int(ans) ) {
    return ans;
  }
  return 0;
}

// @brief 範囲のビット幅を表す式を返す．
// @note 可変範囲選択の時，意味を持つ．
// @note それ以外では NULL を返す．
const VlExpr*
EiVarPartSelect::range_width() const
{
  return mRange;
}

// @brief 範囲のビット幅を返す．
// @note 可変範囲選択の時，意味を持つ．
// @note それ以外では 0 を返す．
int
EiVarPartSelect::range_width_val() const
{
  return mRangeVal;
}

// @brief スカラー値を返す．
tVpiScalarVal
EiVarPartSelect::eval_scalar() const
{
  BitVector tmp;
  eval_bitvector(tmp);
  return tmp.bit_select(0);
}

// @brief 論理値を返す．
tVpiScalarVal
EiVarPartSelect::eval_logic() const
{
  BitVector tmp;
  eval_bitvector(tmp);
  return tmp.to_logic();
}

// @brief real 型の値を返す．
double
EiVarPartSelect::eval_real() const
{
  BitVector tmp;
  eval_bitvector(tmp);
  return tmp.to_real();
}

// @brief decompile() の実装関数
// @param[in] ppri 親の演算子の優先順位
string
EiVarPartSelect::decompile_impl(int ppri) const
{
  ostringstream buf;
  buf << decl_obj()->name() << "[" << mBase->decompile();
  if ( range_mode() == kVpiPlusRange ) {
    buf << " +: ";
  }
  else {
    buf << " -: ";
  }
  buf << mRangeVal << "]";
  return buf.str();
}

// @brief 要求される式の型を計算してセットする．
// @param[in] type 要求される式の型
// @note 必要であればオペランドに対して再帰的に処理を行なう．
void
EiVarPartSelect::set_reqsize(tVpiValueType type)
{
  // なにもしない．
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeclVarPartSelect
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
// @param[in] base 範囲のベースアドレスを表す式
// @param[in] range 範囲を表す式
// @param[in] range_val 範囲の値
EiDeclVarPartSelect::EiDeclVarPartSelect(const PtBase* pt_expr,
					 ElbDecl* obj,
					 ElbExpr* base,
					 ElbExpr* range,
					 int range_val) :
  EiVarPartSelect(pt_expr, base, range, range_val),
  mObj(obj)
{
}

// @brief デストラクタ
EiDeclVarPartSelect::~EiDeclVarPartSelect()
{
}

// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
const VlDecl*
EiDeclVarPartSelect::decl_obj() const
{
  return mObj;
}

// @brief bitvector 型の値を返す．
void
EiDeclVarPartSelect::eval_bitvector(BitVector& bitvector,
				    tVpiValueType req_type) const
{
  int lsb = right_range_val();
  int msb = left_range_val();
  mObj->get_partselect(msb, lsb, bitvector);
  bitvector.coerce(req_type);
}

// @brief スカラー値を書き込む．
// @param[in] v 書き込む値
// @note 左辺式の時のみ意味を持つ．
void
EiDeclVarPartSelect::set_scalar(tVpiScalarVal v)
{
  assert_cond( range_width_val() == 1, __FILE__, __LINE__);
  mObj->set_bitselect(right_range_val(), v);
}

// @brief ビットベクタを書き込む．
// @param[in] v 書き込む値
// @note 左辺式の時のみ意味を持つ．
void
EiDeclVarPartSelect::set_bitvector(const BitVector& v)
{
  int lsb = right_range_val();
  int msb = left_range_val();
  mObj->set_partselect(msb, lsb, v);
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeclPlusPartSelect
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
// @param[in] base 範囲のベースアドレスを表す式
// @param[in] range 範囲を表す式
// @param[in] range_val 範囲の値
EiDeclPlusPartSelect::EiDeclPlusPartSelect(const PtBase* pt_expr,
					   ElbDecl* obj,
					   ElbExpr* base,
					   ElbExpr* range,
					   int range_val) :
  EiDeclVarPartSelect(pt_expr, obj, base, range, range_val)
{
}

// @brief デストラクタ
EiDeclPlusPartSelect::~EiDeclPlusPartSelect()
{
}

// @brief 範囲指定のモードを返す．
tVpiRangeMode
EiDeclPlusPartSelect::range_mode() const
{
  return kVpiPlusRange;
}

// @brief 範囲の MSB の値を返す．
// @note 式に対する範囲選択の時，意味を持つ．
// @note それ以外では 0 を返す．
int
EiDeclPlusPartSelect::left_range_val() const
{
  return right_range_val() + range_width_val() - 1;
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeclMinusPartSelect
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
// @param[in] base 範囲のベースアドレスを表す式
// @param[in] range 範囲を表す式
// @param[in] range_val 範囲の値
EiDeclMinusPartSelect::EiDeclMinusPartSelect(const PtBase* pt_expr,
					     ElbDecl* obj,
					     ElbExpr* base,
					     ElbExpr* range,
					     int range_val) :
  EiDeclVarPartSelect(pt_expr, obj, base, range, range_val)
{
}

// @brief デストラクタ
EiDeclMinusPartSelect::~EiDeclMinusPartSelect()
{
}

// @brief 範囲指定のモードを返す．
tVpiRangeMode
EiDeclMinusPartSelect::range_mode() const
{
  return kVpiMinusRange;
}

// @brief 範囲の MSB の値を返す．
// @note 式に対する範囲選択の時，意味を持つ．
// @note それ以外では 0 を返す．
int
EiDeclMinusPartSelect::left_range_val() const
{
  return right_range_val() - range_width_val() + 1;
}

END_NAMESPACE_YM_VERILOG
