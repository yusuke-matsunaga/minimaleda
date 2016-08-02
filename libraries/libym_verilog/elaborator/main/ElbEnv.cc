
/// @file libym_verilog/elaborator/ElbEnv.cc
/// @brief ElbEnv の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ElbEnv.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ElbEnv.h"


BEGIN_NAMESPACE_YM_VERILOG

BEGIN_NONAMESPACE

const int SFT_CONSTANT = 0;
const int SFT_FUNCTION = 1;
const int SFT_CF = 2;
const int SFT_TASK = 3;
const int SFT_ARG = 4;
const int SFT_EVENT = 5;
const int SFT_LHS = 6;
const int SFT_NET = 7;
const int SFT_VAR = 8;
const int SFT_PCA = 9;
const int SFT_FORCE = 10;

const ymuint32 MASK_CONSTANT = 1U << SFT_CONSTANT;
const ymuint32 MASK_FUNCTION = 1U << SFT_FUNCTION;
const ymuint32 MASK_CF       = 1U << SFT_CF;
const ymuint32 MASK_TASK     = 1U << SFT_TASK;
const ymuint32 MASK_ARG      = 1U << SFT_ARG;
const ymuint32 MASK_EVENT    = 1U << SFT_EVENT;
const ymuint32 MASK_LHS      = 1U << SFT_LHS;
const ymuint32 MASK_NET      = 1U << SFT_NET;
const ymuint32 MASK_VAR      = 1U << SFT_VAR;
const ymuint32 MASK_PCA      = 1U << SFT_PCA;
const ymuint32 MASK_FORCE    = 1U << SFT_FORCE;

END_NONAMESPACE


// @brief コンストラクタ
ElbEnv::ElbEnv() :
  mFlags(0U),
  mCf(NULL)
{
}

// @brief コピーコンストラクタ
ElbEnv::ElbEnv(const ElbEnv& src) :
  mFlags(src.mFlags),
  mCf(src.mCf)
{
}

// @brief 代入演算子
const ElbEnv&
ElbEnv::operator=(const ElbEnv& src)
{
  mFlags = src.mFlags;
  mCf = src.mCf;
  return *this;
}

// @brief デストラクタ
ElbEnv::~ElbEnv()
{
}

// @brief 定数式を要求する．
void
ElbEnv::set_constant()
{
  mFlags |= MASK_CONSTANT;
}

// @brief function を設定する．
void
ElbEnv::set_function()
{
  mFlags |= MASK_FUNCTION;
}

// @brief 親の constant function を設定する．
// @param[in] function 設定する function
void
ElbEnv::set_constant_function(const VlNamedObj* function)
{
  mFlags |= MASK_CF;
  mFlags |= MASK_FUNCTION;
  mCf = function;
}

// @brief system task/system function の引数の印をつける．
void
ElbEnv::set_system_tf_arg()
{
  mFlags |= MASK_ARG;
}

// @brief イベント式の印をつける．
void
ElbEnv::set_event_expr()
{
  mFlags |= MASK_EVENT;
}

// @brief net 型の左辺式の印をつける．
void
ElbEnv::set_net_lhs()
{
  mFlags |= MASK_NET | MASK_LHS;
}

// @brief reg/var 型の左辺式の印をつける．
void
ElbEnv::set_var_lhs()
{
  mFlags |= MASK_VAR | MASK_LHS;
}

// @brief pca 代入文の左辺式の印をつける．
void
ElbEnv::set_pca_lhs()
{
  mFlags |= MASK_PCA | MASK_LHS;
}

// @brief force 代入文の左辺式の印をつける．
void
ElbEnv::set_force_lhs()
{
  mFlags |= MASK_FORCE | MASK_LHS;
}

// @brief 定数式が要求されている時に true を返す．
bool
ElbEnv::is_constant() const
{
  return static_cast<bool>((mFlags >> SFT_CONSTANT) & 1U);
}

// @brief constant function 内の生成の時に親の function を返す．
const VlNamedObj*
ElbEnv::constant_function() const
{
  return mCf;
}

// @brief function 内の生成時に true を返す．
bool
ElbEnv::inside_function() const
{
  return static_cast<bool>((mFlags >> SFT_FUNCTION) & 1U);
}

// @brief constant function 内の生成の時に true を返す．
bool
ElbEnv::inside_constant_function() const
{
  return static_cast<bool>((mFlags >> SFT_CF) & 1U);
}

// @brief system task/system function の引数の時に true を返す．
bool
ElbEnv::is_system_tf_arg() const
{
  return static_cast<bool>((mFlags >> SFT_ARG) & 1U);
}

// @brief イベント式の時に true を返す．
bool
ElbEnv::is_event_expr() const
{
  return static_cast<bool>((mFlags >> SFT_EVENT) & 1U);
}

// @brief 左辺式の時に true を返す．
bool
ElbEnv::is_lhs() const
{
  return static_cast<bool>((mFlags >> SFT_LHS) & 1U);
}

// @brief net 型の左辺式の時に true を返す．
bool
ElbEnv::is_net_lhs() const
{
  return static_cast<bool>((mFlags >> SFT_NET) & 1U);
}

// @brief reg/var 型の左辺式の時に true を返す．
bool
ElbEnv::is_var_lhs() const
{
  return static_cast<bool>((mFlags >> SFT_VAR) & 1U);
}

// @brief pca 代入文の左辺式の時に true を返す．
bool
ElbEnv::is_pca_lhs() const
{
  return static_cast<bool>((mFlags >> SFT_PCA) & 1U);
}

// @brief force 代入文の左辺式の時に true を返す．
bool
ElbEnv::is_force_lhs() const
{
  return static_cast<bool>((mFlags >> SFT_FORCE) & 1U);
}


//////////////////////////////////////////////////////////////////////
// クラス ElbConstantEnv
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbConstantEnv::ElbConstantEnv()
{
  set_constant();
}


//////////////////////////////////////////////////////////////////////
// クラス ElbConstantFunctionEnv
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] func 親の関数
ElbConstantFunctionEnv::ElbConstantFunctionEnv(const VlNamedObj* func)
{
  set_constant_function(func);
}


//////////////////////////////////////////////////////////////////////
// クラス ElbTfEnv
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] taskfunc タスクか関数のオブジェクト
ElbTfEnv::ElbTfEnv(const VlNamedObj* taskfunc)
{
  if ( taskfunc->type() == kVpiFunction ) {
    set_function();
  }
}


//////////////////////////////////////////////////////////////////////
// クラス ElbSystemTfArgEnv
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] env 親の環境
ElbSystemTfArgEnv::ElbSystemTfArgEnv(const ElbEnv& env) :
  ElbEnv(env)
{
  set_system_tf_arg();
}


//////////////////////////////////////////////////////////////////////
// クラス ElbEventExprEnv ElbEnv.h "ElbEnv.h"
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] env 親の環境
ElbEventExprEnv::ElbEventExprEnv(const ElbEnv& env) :
  ElbEnv(env)
{
  set_event_expr();
}


//////////////////////////////////////////////////////////////////////
// クラス ElbNetLhsEnv
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] env 親の環境
ElbNetLhsEnv::ElbNetLhsEnv(const ElbEnv& env) :
  ElbEnv(env)
{
  set_net_lhs();
}


//////////////////////////////////////////////////////////////////////
// クラス ElbVarLhsEnv
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] env 親の環境
ElbVarLhsEnv::ElbVarLhsEnv(const ElbEnv& env) :
  ElbEnv(env)
{
  set_var_lhs();
}


//////////////////////////////////////////////////////////////////////
// クラス ElbPcaLhsEnv
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] env 親の環境
ElbPcaLhsEnv::ElbPcaLhsEnv(const ElbEnv& env) :
  ElbEnv(env)
{
  set_pca_lhs();
}


//////////////////////////////////////////////////////////////////////
// クラス ElbForceLhsEnv
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] env 親の環境
ElbForceLhsEnv::ElbForceLhsEnv(const ElbEnv& env) :
  ElbEnv(env)
{
  set_force_lhs();
}

END_NAMESPACE_YM_VERILOG
