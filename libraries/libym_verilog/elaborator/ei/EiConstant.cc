
/// @file libym_verilog/elb_impl/EiConstant.cc
/// @brief EiExpr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiConstant.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiConstant.h"

#include "ym_verilog/pt/PtExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief 定数式を生成する．
// @param[in] pt_expr パース木の定義要素
ElbExpr*
EiFactory::new_Constant(const PtExpr* pt_expr)
{
  tVpiConstType const_type = pt_expr->const_type();
  ymuint32 size = pt_expr->const_size();
  bool is_signed = false;
  ymuint32 base = 0;
  void* p;
  switch ( const_type ) {
  case kVpiIntConst:
    if ( pt_expr->const_str() == NULL ) {
      p = mAlloc.get_memory(sizeof(EiIntConst));
      return new (p) EiIntConst(pt_expr, pt_expr->const_uint());
    }
    break;

  case kVpiSignedBinaryConst:
    is_signed = true;
  case kVpiBinaryConst:
    base = 2;
    break;

  case kVpiSignedOctConst:
    is_signed = true;
  case kVpiOctConst:
    base = 8;
    break;

  case kVpiSignedDecConst:
    is_signed = true;
  case kVpiDecConst:
    base = 10;
    break;

  case kVpiSignedHexConst:
    is_signed = true;
  case kVpiHexConst:
    base = 16;
    break;

  case kVpiRealConst:
    p = mAlloc.get_memory(sizeof(EiRealConst));
    return new (p) EiRealConst(pt_expr, pt_expr->const_real());

  case kVpiStringConst:
    p = mAlloc.get_memory(sizeof(EiStringConst));
    return new (p) EiStringConst(pt_expr, pt_expr->const_str());

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }

  // ここに来たということはビットベクタ型
  p = mAlloc.get_memory(sizeof(EiBitVectorConst));
  return new (p) EiBitVectorConst(pt_expr, const_type,
				  BitVector(size, is_signed, base,
					    pt_expr->const_str()));
}

// @brief genvar 起因の定数式を生成する．
// @param[in] pt_item パース木の定義要素
// @param[in] val 値
ElbExpr*
EiFactory::new_GenvarConstant(const PtExpr* pt_primary,
			      int val)
{
  void* p = mAlloc.get_memory(sizeof(EiIntConst));
  return new (p) EiIntConst(pt_primary, val);
}


//////////////////////////////////////////////////////////////////////
// クラス EiConstant
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
EiConstant::EiConstant(const PtBase* pt_expr) :
  EiExprBase1(pt_expr)
{
}

// @brief デストラクタ
EiConstant::~EiConstant()
{
}

// @brief 型の取得
tVpiObjType
EiConstant::type() const
{
  return kVpiConstant;
}

// @brief 定数の時 true を返す．
/// @note このクラスは常に true を返す．
bool
EiConstant::is_const() const
{
  return true;
}

// @brief 要求される式の型を計算してセットする．
// @param[in] type 要求される式の型
// @note 必要であればオペランドに対して再帰的に処理を行なう．
void
EiConstant::set_reqsize(tVpiValueType type)
{
  // なにもしない．
}

// @brief スカラー値を書き込む．
// @param[in] v 書き込む値
// @note 左辺式の時のみ意味を持つ．
void
EiConstant::set_scalar(tVpiScalarVal v)
{
  assert_not_reached(__FILE__, __LINE__);
}

// @brief 実数値を書き込む．
// @param[in] v 書き込む値
// @note 左辺式の時のみ意味を持つ．
void
EiConstant::set_real(double v)
{
  assert_not_reached(__FILE__, __LINE__);
}

// @brief ビットベクタを書き込む．
// @param[in] v 書き込む値
// @note 左辺式の時のみ意味を持つ．
void
EiConstant::set_bitvector(const BitVector& v)
{
  assert_not_reached(__FILE__, __LINE__);
}


//////////////////////////////////////////////////////////////////////
// クラス EiIntConst
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] value 値
EiIntConst::EiIntConst(const PtBase* pt_expr,
		       int value) :
  EiConstant(pt_expr),
  mValue(value)
{
}

// @brief デストラクタ
EiIntConst::~EiIntConst()
{
}

// @brief 式のタイプを返す．
tVpiValueType
EiIntConst::value_type() const
{
  return kVpiValueInteger;
}

// @brief 定数の型を返す．
// @note 定数の時，意味を持つ．
tVpiConstType
EiIntConst::constant_type() const
{
  return kVpiIntConst;
}

// @brief int 型の値を返す．
bool
EiIntConst::eval_int(int& val) const
{
  val = mValue;
  return true;
}

// @brief スカラー値を返す．
tVpiScalarVal
EiIntConst::eval_scalar() const
{
  return conv_to_scalar(mValue);
}

// @brief 論理値を返す．
tVpiScalarVal
EiIntConst::eval_logic() const
{
  return conv_to_scalar(mValue);
}

// @brief real 型の値を返す．
double
EiIntConst::eval_real() const
{
  return static_cast<double>(mValue);
}

// @brief bitvector 型の値を返す．
void
EiIntConst::eval_bitvector(BitVector& bitvector,
			   tVpiValueType req_type) const
{
  BitVector tmp(mValue);
  bitvector = mValue;
  bitvector.coerce(req_type);
}

// @brief decompile() の実装関数
// @param[in] pprim 親の演算子の優先順位
string
EiIntConst::decompile_impl(int ppri) const
{
  ostringstream buf;
  buf << mValue;
  return buf.str();
}


//////////////////////////////////////////////////////////////////////
// クラス EiBitVectorConst
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] const_type 定数型
// @param[in] value 値
EiBitVectorConst::EiBitVectorConst(const PtBase* pt_expr,
				   tVpiConstType const_type,
				   const BitVector& value) :
  EiConstant(pt_expr),
  mConstType(const_type),
  mValue(value)
{
}

// @brief デストラクタ
EiBitVectorConst::~EiBitVectorConst()
{
}

// @brief 式のタイプを返す．
tVpiValueType
EiBitVectorConst::value_type() const
{
  ymuint32 size = mValue.size();
  if ( static_cast<int>(mConstType) & 8 ) {
    return pack(kVpiValueSS, size);
  }
  else {
    return pack(kVpiValueUS, size);
  }
}

// @brief 定数の型を返す．
// @note 定数の時，意味を持つ．
tVpiConstType
EiBitVectorConst::constant_type() const
{
  return mConstType;
}

// @brief スカラー値を返す．
tVpiScalarVal
EiBitVectorConst::eval_scalar() const
{
  return mValue.to_scalar();
}

// @brief 論理値を返す．
tVpiScalarVal
EiBitVectorConst::eval_logic() const
{
  return mValue.to_logic();
}

// @brief real 型の値を返す．
double
EiBitVectorConst::eval_real() const
{
  return mValue.to_real();
}

// @brief bitvector 型の値を返す．
void
EiBitVectorConst::eval_bitvector(BitVector& bitvector,
				 tVpiValueType req_type) const
{
  bitvector = mValue;
  bitvector.coerce(req_type);
}

// @brief decompile() の実装関数
// @param[in] pprim 親の演算子の優先順位
string
EiBitVectorConst::decompile_impl(int ppri) const
{
  switch ( mConstType ) {
  case kVpiSignedDecConst:
  case kVpiDecConst:
    return mValue.verilog_string(10);

  case kVpiSignedBinaryConst:
  case kVpiBinaryConst:
    return mValue.verilog_string(2);

  case kVpiSignedOctConst:
  case kVpiOctConst:
    return mValue.verilog_string(8);

  case kVpiSignedHexConst:
  case kVpiHexConst:
    return mValue.verilog_string(16);

  default:
    ;
  }
  return "Type error"; // dummy
}


//////////////////////////////////////////////////////////////////////
// クラス EiRealConst
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] value 値
EiRealConst::EiRealConst(const PtBase* pt_expr,
			 double value) :
  EiConstant(pt_expr),
  mValue(value)
{
}

// @brief デストラクタ
EiRealConst::~EiRealConst()
{
}

// @brief 式のタイプを返す．
tVpiValueType
EiRealConst::value_type() const
{
  return kVpiValueReal;
}

// @brief 定数の型を返す．
// @note 定数の時，意味を持つ．
tVpiConstType
EiRealConst::constant_type() const
{
  return kVpiRealConst;
}

// @brief スカラー値を返す．
tVpiScalarVal
EiRealConst::eval_scalar() const
{
#warning "TODO: 仕様を確認"
  return kVpiScalarX;
}

// @brief 論理値を返す．
tVpiScalarVal
EiRealConst::eval_logic() const
{
  return conv_to_scalar(eval_real());
}

// @brief real 型の値を返す．
double
EiRealConst::eval_real() const
{
  return mValue;
}

// @brief bitvector 型の値を返す．
void
EiRealConst::eval_bitvector(BitVector& bitvector,
			    tVpiValueType req_type) const
{
  bitvector = eval_real();
  bitvector.coerce(req_type);
}

// @brief decompile() の実装関数
// @param[in] pprim 親の演算子の優先順位
string
EiRealConst::decompile_impl(int ppri) const
{
  ostringstream buf;
  buf << eval_real();
  return buf.str();
}


//////////////////////////////////////////////////////////////////////
// クラス EiStringConst
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] value 値
EiStringConst::EiStringConst(const PtBase* pt_expr,
			     const string& value) :
  EiConstant(pt_expr),
  mValue(value)
{
}

// @brief デストラクタ
EiStringConst::~EiStringConst()
{
}

// @brief 式のタイプを返す．
tVpiValueType
EiStringConst::value_type() const
{
  ymuint32 size = mValue.size();
  return pack(kVpiValueUS, size);
}

// @brief 定数の型を返す．
// @note 定数の時，意味を持つ．
tVpiConstType
EiStringConst::constant_type() const
{
  return kVpiStringConst;
}

// @brief スカラー値を返す．
tVpiScalarVal
EiStringConst::eval_scalar() const
{
  return mValue.to_scalar();
}

// @brief 論理値を返す．
tVpiScalarVal
EiStringConst::eval_logic() const
{
  return mValue.to_logic();
}

// @brief real 型の値を返す．
double
EiStringConst::eval_real() const
{
  return mValue.to_real();
}

// @brief bitvector 型の値を返す．
void
EiStringConst::eval_bitvector(BitVector& bitvector,
			      tVpiValueType req_type) const
{
  bitvector = mValue;
  bitvector.coerce(req_type);
}

// @brief decompile() の実装関数
// @param[in] pprim 親の演算子の優先順位
string
EiStringConst::decompile_impl(int ppri) const
{
  return mValue.to_string();
}

END_NAMESPACE_YM_VERILOG
