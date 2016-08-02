
/// @file libym_verilog/elb_impl/EiFuncCall.cc
/// @brief EiFuncCall の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiFuncCall.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiFuncCall.h"
#include "ElbTaskFunc.h"
#include "ElbUserSystf.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief 関数呼び出し式を生成する．
// @param[in] pt_obj パース木の定義要素
// @param[in] func 関数
// @param[in] arg_size 引数の数
// @param[in] arg_list 引数のリスト
ElbExpr*
EiFactory::new_FuncCall(const PtBase* pt_obj,
			const ElbTaskFunc* func,
			ymuint32 arg_size,
			ElbExpr** arg_list)
{
  void* p = mAlloc.get_memory(sizeof(EiFuncCall));
  EiFuncCall* expr = new (p) EiFuncCall(pt_obj, func,
					arg_size, arg_list);

  return expr;
}

// @brief システム関数呼び出し式を生成する．
// @param[in] pt_obj パース木の定義要素
// @param[in] user_systf システム関数
// @param[in] arg_size 引数の数
// @param[in] arg_list 引数のリスト
ElbExpr*
EiFactory::new_SysFuncCall(const PtBase* pt_obj,
			   const ElbUserSystf* user_systf,
			   ymuint32 arg_size,
			   ElbExpr** arg_list)
{
  void* p = mAlloc.get_memory(sizeof(EiSysFuncCall));
  EiSysFuncCall* expr = new (p) EiSysFuncCall(pt_obj, user_systf,
					      arg_size, arg_list);

  return expr;
}


//////////////////////////////////////////////////////////////////////
// クラス EiFcBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_obj パース木の定義要素
// @param[in] arg_size 引数の数
// @param[in] arg_list 引数のリスト
EiFcBase::EiFcBase(const PtBase* pt_obj,
		   ymuint32 arg_size,
		   ElbExpr** arg_list) :
  EiExprBase1(pt_obj),
  mArgNum(arg_size),
  mArgList(arg_list)
{
}
  
// @brief デストラクタ
EiFcBase::~EiFcBase()
{
}
    
// @brief 引数の数を返す．
ymuint32
EiFcBase::argument_num() const
{
  return mArgNum;
}

// @brief 引数の取得
// @param[in] pos 位置番号 ( 0 <= pos < argument_num() )
ElbExpr*
EiFcBase::argument(ymuint32 pos) const
{
  return mArgList[pos];
}


//////////////////////////////////////////////////////////////////////
// クラス EiFuncCall
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_obj パース木の定義要素
// @param[in] func 関数
// @param[in] arg_size 引数の数
// @param[in] arg_list 引数のリスト
EiFuncCall::EiFuncCall(const PtBase* pt_obj,
		       const ElbTaskFunc* func,
		       ymuint32 arg_size,
		       ElbExpr** arg_list) :
  EiFcBase(pt_obj, arg_size, arg_list),
  mFunc(func)
{
}

// @brief デストラクタ
EiFuncCall::~EiFuncCall()
{
}

// @brief 型の取得
tVpiObjType
EiFuncCall::type() const
{
  return kVpiFuncCall;
}

// @brief 式のタイプを返す．
tVpiValueType
EiFuncCall::value_type() const
{
  switch ( mFunc->func_type() ) {
  case kVpiIntFunc:
    return kVpiValueInteger;

  case kVpiRealFunc:
    return kVpiValueReal;

  case kVpiTimeFunc:
    return kVpiValueTime;

  case kVpiSizedFunc:
    return pack(kVpiValueUS, mFunc->bit_size());

  case kVpiSizedSignedFunc:
    return pack(kVpiValueSS, mFunc->bit_size());

  default:
    assert_not_reached(__FILE__, __LINE__);
  }
  return kVpiValueNone;
}

// @brief 定数の時 true を返す．
bool
EiFuncCall::is_const() const
{
  if ( mFunc->is_constant_function() ) {
    ymuint32 n = argument_num();
    for (ymuint32 i = 0; i < n; ++ i) {
      if ( !argument(i)->is_const() ) {
	return false;
      }
    }
    return true;
  }
  return false;
}

// @brief 関数呼び出しの時に true を返す．
bool
EiFuncCall::is_funccall() const
{
  return true;
}

// @brief 対象の関数を返す．
// @note kVpiFuncCall の時，意味を持つ．
const VlTaskFunc*
EiFuncCall::function() const
{
  return mFunc;
}

// @brief スカラー値を返す．
tVpiScalarVal
EiFuncCall::eval_scalar() const
{
  ymuint32 n = argument_num();
  vector<ElbExpr*> arg_list(n);
  for (ymuint32 i = 0; i < n; ++ i) {
    arg_list[i] = argument(i);
  }
  return mFunc->eval_scalar(arg_list);
}

// @brief 論理値を返す．
tVpiScalarVal
EiFuncCall::eval_logic() const
{
  ymuint32 n = argument_num();
  vector<ElbExpr*> arg_list(n);
  for (ymuint32 i = 0; i < n; ++ i) {
    arg_list[i] = argument(i);
  }
  return mFunc->eval_logic(arg_list);
}
  
// @brief real 型の値を返す．
double
EiFuncCall::eval_real() const
{
  ymuint32 n = argument_num();
  vector<ElbExpr*> arg_list(n);
  for (ymuint32 i = 0; i < n; ++ i) {
    arg_list[i] = argument(i);
  }
  return mFunc->eval_real(arg_list);
}

// @brief bitvector 型の値を返す．
void
EiFuncCall::eval_bitvector(BitVector& bitvector,
			   tVpiValueType req_type) const
{
  ymuint32 n = argument_num();
  vector<ElbExpr*> arg_list(n);
  for (ymuint32 i = 0; i < n; ++ i) {
    arg_list[i] = argument(i);
  }
  mFunc->eval_bitvector(arg_list, bitvector, req_type);
}
  
// @brief decompile() の実装関数
// @param[in] pprim 親の演算子の優先順位
string
EiFuncCall::decompile_impl(int ppri) const
{
  string ans = mFunc->name();
  ans += "(";
  ymuint32 n = argument_num();
  const char* comma = "";
  for (ymuint32 i = 0; i < n; ++ i) {
    ans += comma + argument(i)->decompile();
    comma = ", ";
  }
  ans += ")";
  return ans;
}

// @brief 要求される式の型を計算してセットする．
// @param[in] type 要求される式の型
// @note 必要であればオペランドに対して再帰的に処理を行なう．
void
EiFuncCall::set_reqsize(tVpiValueType type)
{
  // なにもしない．
}


//////////////////////////////////////////////////////////////////////
// クラス EiSysFuncCall
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_obj パース木の定義要素
// @param[in] user_systf システム関数
// @param[in] arg_size 引数の数
// @param[in] arg_list 引数のリスト
EiSysFuncCall::EiSysFuncCall(const PtBase* pt_obj,
			     const ElbUserSystf* user_systf,
			     ymuint32 arg_size,
			     ElbExpr** arg_list) :
  EiFcBase(pt_obj, arg_size, arg_list),
  mUserSystf(user_systf)
{
}

// @brief デストラクタ
EiSysFuncCall::~EiSysFuncCall()
{
}

// @brief 型の取得
tVpiObjType
EiSysFuncCall::type() const
{
  return kVpiSysFuncCall;
}

// @brief 式のタイプを返す．
tVpiValueType
EiSysFuncCall::value_type() const
{
  assert_cond(mUserSystf->system_function(), __FILE__, __LINE__);

  switch ( mUserSystf->function_type() ) {
  case vpiIntFunc:
    return kVpiValueInteger;
    break;

  case vpiRealFunc:
    return kVpiValueReal;
    break;

  case vpiTimeFunc:
    return kVpiValueTime;
    break;

  case vpiSizedFunc:
    return pack(kVpiValueUS, mUserSystf->size());
    break;

  case vpiSizedSignedFunc:
    return pack(kVpiValueSS, mUserSystf->size());
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
  }
  return kVpiValueNone;
}

// @brief 定数の時 true を返す．
// @note このクラスは false を返す．
bool
EiSysFuncCall::is_const() const
{
  return false;
}

// @brief システム関数よびあどい時に true を返す．
bool
EiSysFuncCall::is_sysfunccall() const
{
  return true;
}

// @brief 対象のシステム関数を返す．
// @note kVpiSysFuncCall の時，意味を持つ．
const VlUserSystf*
EiSysFuncCall::user_systf() const
{
  return mUserSystf;
}

// @brief スカラー値を返す．
tVpiScalarVal
EiSysFuncCall::eval_scalar() const
{
# warning "TODO: 未完"
  return kVpiScalarX;
}

// @brief 論理値を返す．
tVpiScalarVal
EiSysFuncCall::eval_logic() const
{
# warning "TODO: 未完"
  return kVpiScalarX;
}
  
// @brief real 型の値を返す．
double
EiSysFuncCall::eval_real() const
{
# warning "TODO: 未完"
  return 0.0;
}

// @brief bitvector 型の値を返す．
void
EiSysFuncCall::eval_bitvector(BitVector& bitvector,
			      tVpiValueType req_type) const
{
# warning "TODO: 未完"
}

// @brief decompile した文字列を返す．
string
EiSysFuncCall::decompile_impl(int ppri) const
{
  string ans = mUserSystf->name();
  ans += "(";
  ymuint32 n = argument_num();
  const char* comma = "";
  for (ymuint32 i = 0; i < n; ++ i) {
    ans += comma + argument(i)->decompile();
    comma = ", ";
  }
  ans += ")";
  return ans;
}

// @brief 要求される式の型を計算してセットする．
// @param[in] type 要求される式の型
// @note 必要であればオペランドに対して再帰的に処理を行なう．
void
EiSysFuncCall::set_reqsize(tVpiValueType type)
{
  // なにもしない．
}

END_NAMESPACE_YM_VERILOG
