
/// @file libym_verilog/elb_impl/EiAssignment.cc
/// @brief EiAssignment の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiAssignment.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiAssignment.h"
#include "ElbControl.h"
#include "ElbExpr.h"

#include "ym_verilog/BitVector.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////
  
// @brief 代入文を生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] lhs 左辺の式
// @param[in] rhs 右辺の式
// @param[in] block ブロッキング代入の時 true
// @param[in] control コントロール
ElbStmt*
EiFactory::new_Assignment(const VlNamedObj* parent,
			  ElbProcess* process,
			  const PtStmt* pt_stmt,
			  ElbExpr* lhs,
			  ElbExpr* rhs,
			  bool block,
			  ElbControl* control)
{
  ElbStmt* stmt = NULL;
  if ( block ) {
    void* p = mAlloc.get_memory(sizeof(EiAssignment));
    stmt = new (p) EiAssignment(parent, process, pt_stmt,
				lhs, rhs, control);
  }
  else {
    void* p = mAlloc.get_memory(sizeof(EiNbAssignment));
    stmt = new (p) EiNbAssignment(parent, process, pt_stmt,
				  lhs, rhs, control);
  }
  return stmt;
}

// @brief assign ステートメントを生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] lhs 左辺の式
// @param[in] rhs 右辺の式
ElbStmt*
EiFactory::new_AssignStmt(const VlNamedObj* parent,
			  ElbProcess* process,
			  const PtStmt* pt_stmt,
			  ElbExpr* lhs,
			  ElbExpr* rhs)
{
  void* p = mAlloc.get_memory(sizeof(EiAssignStmt));
  ElbStmt* stmt = new (p) EiAssignStmt(parent, process, pt_stmt,
				      lhs, rhs);

  return stmt;
}
  
// @brief deassign ステートメントを生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] lhs 左辺の式
ElbStmt*
EiFactory::new_DeassignStmt(const VlNamedObj* parent,
			    ElbProcess* process,
			    const PtStmt* pt_stmt,
			    ElbExpr* lhs)
{
  void* p = mAlloc.get_memory(sizeof(EiAssignStmt));
  ElbStmt* stmt = new (p) EiDeassignStmt(parent, process, pt_stmt,
					lhs);

  return stmt;
}

// @brief force ステートメントを生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] lhs 左辺の式
// @param[in] rhs 右辺の式
ElbStmt*
EiFactory::new_ForceStmt(const VlNamedObj* parent,
			 ElbProcess* process,
			 const PtStmt* pt_stmt,
			 ElbExpr* lhs,
			 ElbExpr* rhs)
{
  void* p = mAlloc.get_memory(sizeof(EiForceStmt));
  ElbStmt* stmt = new (p) EiForceStmt(parent, process, pt_stmt,
				     lhs, rhs);

  return stmt;
}
  
// @brief release ステートメントを生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] lhs 左辺の式
ElbStmt*
EiFactory::new_ReleaseStmt(const VlNamedObj* parent,
			   ElbProcess* process,
			   const PtStmt* pt_stmt,
			   ElbExpr* lhs)
{
  void* p = mAlloc.get_memory(sizeof(EiReleaseStmt));
  ElbStmt* stmt = new (p) EiReleaseStmt(parent, process, pt_stmt,
				       lhs);

  return stmt;
}


//////////////////////////////////////////////////////////////////////
// クラス EiAssignBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] lhs 左辺の式
// @param[in] rhs 右辺の式
EiAssignBase::EiAssignBase(const VlNamedObj* parent,
			   ElbProcess* process,
			   const PtStmt* pt_stmt,
			   ElbExpr* lhs,
			   ElbExpr* rhs) :
  EiStmtBase(parent, process, pt_stmt),
  mLhs(lhs),
  mRhs(rhs)
{
}

// @brief デストラクタ
EiAssignBase::~EiAssignBase()
{
}
  
// @brief 左辺を返す．
const VlExpr*
EiAssignBase::lhs() const
{
  return mLhs;
}
  
// @brief 右辺を返す．
const VlExpr*
EiAssignBase::rhs() const
{
  return mRhs;
}
  
// @brief 左辺を返す．
ElbExpr*
EiAssignBase::_lhs() const
{
  return mLhs;
}
  
// @brief 右辺を返す．
ElbExpr*
EiAssignBase::_rhs() const
{
  return mRhs;
}


//////////////////////////////////////////////////////////////////////
// クラス EiNbAssignment
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] lhs 左辺の式
// @param[in] rhs 右辺の式
// @param[in] control コントロール
EiNbAssignment::EiNbAssignment(const VlNamedObj* parent,
			       ElbProcess* process,
			       const PtStmt* pt_stmt,
			       ElbExpr* lhs,
			       ElbExpr* rhs,
			       ElbControl* control) :
  EiAssignBase(parent, process, pt_stmt, lhs, rhs),
  mControl(control)
{
}

// @brief デストラクタ
EiNbAssignment::~EiNbAssignment()
{
}

// @brief クラスの型の取得
tVpiObjType
EiNbAssignment::type() const
{
  return kVpiAssignment;
}
  
// @brief control を返す．NULL の場合もありうる．
const VlControl*
EiNbAssignment::control() const
{
  return mControl;
}

// @brief function 中の実行を行う．
const VlNamedObj*
EiNbAssignment::func_exec(bool constant_function) const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス EiAssignment
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] lhs 左辺の式
// @param[in] rhs 右辺の式
// @param[in] control コントロール
EiAssignment::EiAssignment(const VlNamedObj* parent,
			   ElbProcess* process,
			   const PtStmt* pt_stmt,
			   ElbExpr* lhs,
			   ElbExpr* rhs,
			   ElbControl* control) :
  EiNbAssignment(parent, process, pt_stmt, lhs, rhs, control)
{
}

// @brief デストラクタ
EiAssignment::~EiAssignment()
{
}

// @brief 代入のブロッキング/ノンブロッキングの区別の取得
// @note このクラスでは true を返す．
bool
EiAssignment::is_blocking() const
{
  return true;
}

// @brief function 中の実行を行う．
const VlNamedObj*
EiAssignment::func_exec(bool constant_function) const
{
  assert_cond(control() == NULL, __FILE__, __LINE__);
  if ( _lhs()->bit_size() == 1 ) {
    tVpiScalarVal v = _rhs()->eval_scalar();
    _lhs()->set_scalar(v);
  }
  else if ( _lhs()->value_type() == kVpiValueReal ) {
    double v = _rhs()->eval_real();
    _lhs()->set_real(v);
  }
  else {
    BitVector v;
    _rhs()->eval_bitvector(v, _lhs()->value_type());
    _lhs()->set_bitvector(v);
  }
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス EiAssignStmt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] lhs 左辺の式
// @param[in] rhs 右辺の式
EiAssignStmt::EiAssignStmt(const VlNamedObj* parent,
			   ElbProcess* process,
			   const PtStmt* pt_stmt,
			   ElbExpr* lhs,
			   ElbExpr* rhs) :
  EiAssignBase(parent, process, pt_stmt, lhs, rhs)
{
}

// @brief デストラクタ
EiAssignStmt::~EiAssignStmt()
{
}

// @brief このクラスの型を返す．
tVpiObjType
EiAssignStmt::type() const
{
  return kVpiAssignStmt;
}

// @brief function 中の実行を行う．
// @note このクラスは function 中では使えない．
const VlNamedObj*
EiAssignStmt::func_exec(bool constant_function) const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス EiForceStmt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] lhs 左辺の式
// @param[in] rhs 右辺の式
EiForceStmt::EiForceStmt(const VlNamedObj* parent,
			 ElbProcess* process,
			 const PtStmt* pt_stmt,
			 ElbExpr* lhs,
			 ElbExpr* rhs) :
  EiAssignBase(parent, process, pt_stmt, lhs, rhs)
{
}

// @brief デストラクタ
EiForceStmt::~EiForceStmt()
{
}
  
// @brief このクラスの型を返す．
tVpiObjType
EiForceStmt::type() const
{
  return kVpiForce;
}
  
// @brief function 中の実行を行う．
// @note このクラスは function 中では使えない．
const VlNamedObj*
EiForceStmt::func_exec(bool constant_function) const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeassignBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] lhs 左辺の式
EiDeassignBase::EiDeassignBase(const VlNamedObj* parent,
			       ElbProcess* process,
			       const PtStmt* pt_stmt,
			       ElbExpr* lhs) :
  EiStmtBase(parent, process, pt_stmt),
  mLhs(lhs)
{
}

// @brief デストラクタ
EiDeassignBase::~EiDeassignBase()
{
}
  
// @brief 左辺を返す．
const VlExpr*
EiDeassignBase::lhs() const
{
  return mLhs;
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeassignStmt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] lhs 左辺の式
EiDeassignStmt::EiDeassignStmt(const VlNamedObj* parent,
			       ElbProcess* process,
			       const PtStmt* pt_stmt,
			       ElbExpr* lhs) :
  EiDeassignBase(parent, process, pt_stmt, lhs)
{
}

// @brief デストラクタ
EiDeassignStmt::~EiDeassignStmt()
{
}
  
// @brief このクラスの型を返す．
tVpiObjType
EiDeassignStmt::type() const
{
  return kVpiDeassign;
}

// @brief function 中の実行を行う．
// @note このクラスは function 中では使えない．
const VlNamedObj*
EiDeassignStmt::func_exec(bool constant_function) const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス EiReleaseStmt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] lhs 左辺の式
EiReleaseStmt::EiReleaseStmt(const VlNamedObj* parent,
			     ElbProcess* process,
			     const PtStmt* pt_stmt,
			     ElbExpr* lhs) :
  EiDeassignBase(parent, process, pt_stmt, lhs)
{
}

// @brief デストラクタ
EiReleaseStmt::~EiReleaseStmt()
{
}
  
// @brief このクラスの型を返す．
tVpiObjType
EiReleaseStmt::type() const
{
  return kVpiRelease;
}

// @brief function 中の実行を行う．
// @note このクラスは function 中では使えない．
const VlNamedObj*
EiReleaseStmt::func_exec(bool constant_function) const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

END_NAMESPACE_YM_VERILOG
