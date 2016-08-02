
/// @file libym_verilog/tests/vlview/VmItermNode.cc
/// @brief VmItemNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlPtNode_item.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include "VmItemNode.h"
#include "VmMiscNode.h"
#include "ym_verilog/vl/VlParamAssign.h"
#include "ym_verilog/vl/VlDecl.h"
#include "ym_verilog/vl/VlTaskFunc.h"
#include "ym_verilog/vl/VlPrimitive.h"
#include "ym_verilog/vl/VlContAssign.h"
#include "ym_verilog/vl/VlModule.h"
#include "ym_verilog/vl/VlUdp.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス VmNode1 の関数
//////////////////////////////////////////////////////////////////////

// @brief ParamAssignListNode を追加する．
// @param[in] pa_list parameter代入文のリスト
void
VmNode1::add_paramassignlist(const vector<const VlParamAssign*>& pa_list) const
{
  add_child( new VmParamAssignListNode(pa_list) );
}

// @brief DefParamListNode を追加する．
// @param[in] defparam_list defparam 文のリスト
void
VmNode1::add_defparamlist(const vector<const VlDefParam*>& defparam_list) const
{
  add_child( new VmDefParamListNode(defparam_list) );
}

// @brief ScopeListNode を追加する．
// @param[in] scope_list 内部スコープのリスト
void
VmNode1::add_scopelist(const vector<const VlNamedObj*>& scope_list) const
{
  add_child( new VmScopeListNode(vl_mgr(), scope_list) );
}

// @brief TaskListNode を追加する．
// @param[in] task_list タスクのリスト
void
VmNode1::add_tasklist(const vector<const VlTaskFunc*>& task_list) const
{
  add_child( new VmTaskListNode(vl_mgr(), task_list) );
}

// @brief FuncListNode を追加する．
// @param[in] function_list 関数のリスト
void
VmNode1::add_functionlist(const vector<const VlTaskFunc*>& function_list) const
{
  add_child( new VmFunctionListNode(vl_mgr(), function_list) );
}

// @brief ContAssignListNode を追加する．
// @param[in] ca_list 継続的代入文のリスト
void
VmNode1::add_contassignlist(const vector<const VlContAssign*>& ca_list) const
{
  add_child( new VmContAssignListNode(ca_list) );
}

// @brief PrimitiveListNode を追加する．
// @param[in] primitive_list プリミティブのリスト
void
VmNode1::add_primitivelist(const vector<const VlPrimitive*>& primitive_list) const
{
  add_child( new VmPrimitiveListNode(primitive_list) );
}

// @brief PrimArrayListNode を追加する．
// @param[in] primarray_list プリミティブ配列のリスト
void
VmNode1::add_primarraylist(const vector<const VlPrimArray*>& primarray_list) const
{
  add_child( new VmPrimArrayListNode(primarray_list) );
}


//////////////////////////////////////////////////////////////////////
// クラス VmParamAssignListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pa_list パラメータ代入文のリスト
VmParamAssignListNode::VmParamAssignListNode(const vector<const VlParamAssign*>& pa_list) :
  mParamAssignArray(pa_list)
{
}

// @brief デストラクタ
VmParamAssignListNode::~VmParamAssignListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmParamAssignListNode::data(int column,
			    int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiParamAssign list";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
VmParamAssignListNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
void
VmParamAssignListNode::expand() const
{
  for (vector<const VlParamAssign*>::const_iterator p = mParamAssignArray.begin();
       p != mParamAssignArray.end(); ++ p) {
    add_child( new VmParamAssignNode(*p) );
  }
}


//////////////////////////////////////////////////////////////////////
// クラス VmParamAssignNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] paramassign パラメータ代入文
VmParamAssignNode::VmParamAssignNode(const VlParamAssign* paramassign) :
  mParamAssign(paramassign)
{
}

// @brief デストラクタ
VmParamAssignNode::~VmParamAssignNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmParamAssignNode::data(int column,
			int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiParamAssign";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
VmParamAssignNode::loc() const
{
  return mParamAssign->file_region();
}

// @brief 子供の配列を作る．
void
VmParamAssignNode::expand() const
{
  add_str("vpiModule", mParamAssign->parent()->full_name());
  add_bool("vpiConnByName", mParamAssign->is_conn_by_name());
  add_str("vpiLhs", mParamAssign->lhs()->full_name());
  add_expr("vpiRhs", mParamAssign->rhs());
}


//////////////////////////////////////////////////////////////////////
// クラス VmDefParamListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] defparam_list deparam文のリスト
VmDefParamListNode::VmDefParamListNode(const vector<const VlDefParam*>& defparam_list) :
  mDefParamArray(defparam_list)
{
}

// @brief デストラクタ
VmDefParamListNode::~VmDefParamListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmDefParamListNode::data(int column,
			 int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiDefParam list";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
VmDefParamListNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
void
VmDefParamListNode::expand() const
{
  for (vector<const VlDefParam*>::const_iterator p = mDefParamArray.begin();
       p != mDefParamArray.end(); ++ p) {
    add_child( new VmDefParamNode(*p) );
  }
}


//////////////////////////////////////////////////////////////////////
// クラス VmDefParamNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] defparam deparam文
VmDefParamNode::VmDefParamNode(const VlDefParam* defparam) :
  mDefParam(defparam)
{
}

// @brief デストラクタ
VmDefParamNode::~VmDefParamNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmDefParamNode::data(int column,
		     int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiDefParam";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
VmDefParamNode::loc() const
{
  return mDefParam->file_region();
}

// @brief 子供の配列を作る．
void
VmDefParamNode::expand() const
{
  add_str("vpiModule", mDefParam->parent()->full_name());
  add_str("vpiLhs", mDefParam->lhs()->full_name());
  add_expr("vpiRhs", mDefParam->rhs());
}


//////////////////////////////////////////////////////////////////////
// クラス VmScopeListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] vl_mgr VlMgr
// @param[in] scope_list スコープのリスト
VmScopeListNode::VmScopeListNode(const VlMgr& vl_mgr,
				 const vector<const VlNamedObj*>& scope_list) :
  VmNode1(vl_mgr),
  mScopeArray(scope_list)
{
}

// @brief デストラクタ
VmScopeListNode::~VmScopeListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmScopeListNode::data(int column,
		     int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiInternalScope list";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
VmScopeListNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
void
VmScopeListNode::expand() const
{
  for (vector<const VlNamedObj*>::const_iterator p = mScopeArray.begin();
       p != mScopeArray.end(); ++ p) {
    add_child( new VmScopeNode(vl_mgr(), *p) );
  }
}


//////////////////////////////////////////////////////////////////////
// クラス VmScopeNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] vl_mgr VlMgr
// @param[in] scope スコープ
VmScopeNode::VmScopeNode(const VlMgr& vl_mgr,
			 const VlNamedObj* scope) :
  VmNode1(vl_mgr),
  mScope(scope)
{
}

// @brief デストラクタ
VmScopeNode::~VmScopeNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmScopeNode::data(int column,
		 int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiInternalScope";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
VmScopeNode::loc() const
{
  return mScope->file_region();
}

// @brief 子供の配列を作る．
void
VmScopeNode::expand() const
{
  add_str("vpiFullName", mScope->full_name());

  add_scope_item(mScope);
}


//////////////////////////////////////////////////////////////////////
// クラス VmTaskListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] vl_mgr VlMgr
// @param[in] task_list タスクのリスト
VmTaskListNode::VmTaskListNode(const VlMgr& vl_mgr,
			       const vector<const VlTaskFunc*>& task_list) :
  VmNode1(vl_mgr),
  mTaskArray(task_list)
{
}

// @brief デストラクタ
VmTaskListNode::~VmTaskListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmTaskListNode::data(int column,
		     int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiTask list";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
VmTaskListNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
void
VmTaskListNode::expand() const
{
  for (vector<const VlTaskFunc*>::const_iterator p = mTaskArray.begin();
       p != mTaskArray.end(); ++ p) {
    add_child( new VmTaskNode(vl_mgr(), *p) );
  }
}


//////////////////////////////////////////////////////////////////////
// クラス VmTaskNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] vl_mgr VlMgr
// @param[in] task タスク
VmTaskNode::VmTaskNode(const VlMgr& vl_mgr,
		       const VlTaskFunc* task) :
  VmNode1(vl_mgr),
  mTask(task)
{
}

// @brief デストラクタ
VmTaskNode::~VmTaskNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmTaskNode::data(int column,
		 int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiTask";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
VmTaskNode::loc() const
{
  return mTask->file_region();
}

// @brief 子供の配列を作る．
void
VmTaskNode::expand() const
{
  add_str("vpiFullName", mTask->full_name());
  ymuint n = mTask->io_num();
  vector<const VlIODecl*> io_array(n);
  for (ymuint i = 0; i < n; ++ i) {
    io_array[i] = mTask->io(i);
  }
  add_iolist(io_array);

  add_scope_item(mTask);

  add_stmt("vpiStmt", mTask->stmt());
}


//////////////////////////////////////////////////////////////////////
// クラス VmFunctionListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] vl_mgr VlMgr
// @param[in] func_list 関数のリスト
VmFunctionListNode::VmFunctionListNode(const VlMgr& vl_mgr,
				       const vector<const VlTaskFunc*>& func_list) :
  VmNode1(vl_mgr),
  mFunctionArray(func_list)
{
}

// @brief デストラクタ
VmFunctionListNode::~VmFunctionListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmFunctionListNode::data(int column,
			 int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiFunction list";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
VmFunctionListNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
void
VmFunctionListNode::expand() const
{
  for (vector<const VlTaskFunc*>::const_iterator p = mFunctionArray.begin();
       p != mFunctionArray.end(); ++ p) {
    add_child( new VmFunctionNode(vl_mgr(), *p) );
  }
}


//////////////////////////////////////////////////////////////////////
// クラス VmFunctionNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] vl_mgr VlMgr
// @param[in] func 関数
VmFunctionNode::VmFunctionNode(const VlMgr& vl_mgr,
		       const VlTaskFunc* func) :
  VmNode1(vl_mgr),
  mFunction(func)
{
}

// @brief デストラクタ
VmFunctionNode::~VmFunctionNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmFunctionNode::data(int column,
		     int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiFunction";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
VmFunctionNode::loc() const
{
  return mFunction->file_region();
}

BEGIN_NONAMESPACE

const char*
functype2str(tVpiFuncType type)
{
  switch ( type ) {
  case kVpiIntFunc:         return "vpiIntFunc";
  case kVpiRealFunc:        return "vpiRealFunc";
  case kVpiTimeFunc:        return "vpiTimeFunc";
  case kVpiSizedFunc:       return "vpiSizedFunc";
  case kVpiSizedSignedFunc: return "vpiSizedSignedFunc";
  default: assert_not_reached(__FILE__, __LINE__);
  }
  return NULL;
}

END_NONAMESPACE

// @brief 子供の配列を作る．
void
VmFunctionNode::expand() const
{
  add_str("vpiFullName", mFunction->full_name());
  add_str("vpiFuncType", functype2str(mFunction->func_type()));
  add_bool("vpiSigned", mFunction->is_signed());
  add_int("vpiSize", mFunction->bit_size());
  if ( mFunction->left_range() ) {
    add_expr("vpiLeftRange", mFunction->left_range());
    add_expr("vpiRightRange", mFunction->right_range());
  }
  ymuint n = mFunction->io_num();
  vector<const VlIODecl*> io_array(n);
  for (ymuint i = 0; i < n; ++ i) {
    io_array[i] = mFunction->io(i);
  }
  add_iolist(io_array);

  add_scope_item(mFunction);

  add_stmt("vpiStmt", mFunction->stmt());
}


//////////////////////////////////////////////////////////////////////
// クラス VmPrimArrayListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] primarray_list プリミティブ配列のリスト
VmPrimArrayListNode::VmPrimArrayListNode(const vector<const VlPrimArray*>& primarray_list) :
  mPrimArrayArray(primarray_list)
{
}

// @brief デストラクタ
VmPrimArrayListNode::~VmPrimArrayListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmPrimArrayListNode::data(int column,
			  int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiPrimArray list";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
VmPrimArrayListNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
void
VmPrimArrayListNode::expand() const
{
  for (vector<const VlPrimArray*>::const_iterator p = mPrimArrayArray.begin();
       p != mPrimArrayArray.end(); ++ p) {
    add_child( new VmPrimArrayNode(*p) );
  }
}


//////////////////////////////////////////////////////////////////////
// クラス VmPrimArrayNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] primitive プリミティブ
VmPrimArrayNode::VmPrimArrayNode(const VlPrimArray* primitive) :
  mPrimArray(primitive)
{
}

// @brief デストラクタ
VmPrimArrayNode::~VmPrimArrayNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmPrimArrayNode::data(int column,
		      int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiPrimArray";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
VmPrimArrayNode::loc() const
{
  return mPrimArray->file_region();
}

// @brief 子供の配列を作る．
void
VmPrimArrayNode::expand() const
{
  ymuint n = mPrimArray->elem_num();
  for (ymuint i = 0; i < n; ++ i) {
    add_child( new VmPrimitiveNode(mPrimArray->elem_by_offset(i)) );
  }
}


//////////////////////////////////////////////////////////////////////
// クラス VmPrimitiveListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] prim_list プリミティブのリスト
VmPrimitiveListNode::VmPrimitiveListNode(const vector<const VlPrimitive*>& prim_list) :
  mPrimArray(prim_list)
{
}

// @brief デストラクタ
VmPrimitiveListNode::~VmPrimitiveListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmPrimitiveListNode::data(int column,
			  int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiPrimitive list";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
VmPrimitiveListNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
void
VmPrimitiveListNode::expand() const
{
  for (vector<const VlPrimitive*>::const_iterator p = mPrimArray.begin();
       p != mPrimArray.end(); ++ p) {
    add_child( new VmPrimitiveNode(*p) );
  }
}


//////////////////////////////////////////////////////////////////////
// クラス VmPrimitiveNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] primitive プリミティブ
VmPrimitiveNode::VmPrimitiveNode(const VlPrimitive* primitive) :
  mPrimitive(primitive)
{
}

// @brief デストラクタ
VmPrimitiveNode::~VmPrimitiveNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmPrimitiveNode::data(int column,
		      int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiPrimitive";
    }
    else if ( column == 1 ) {
      const char* nm = NULL;
      switch ( mPrimitive->type() ) {
      case kVpiGate:   nm = "vpiGate"; break;
      case kVpiSwitch: nm = "vpiSwitch"; break;
      case kVpiUdp:    nm = "vpiUdp"; break;
      default: assert_not_reached(__FILE__, __LINE__);
      }
      return nm;
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
VmPrimitiveNode::loc() const
{
  return mPrimitive->file_region();
}

// @brief 子供の配列を作る．
void
VmPrimitiveNode::expand() const
{
  add_str("vpiFullName", mPrimitive->full_name());
  add_str("vpiDefName", mPrimitive->def_name());
  add_child( new VmPrimTypeNode(mPrimitive->prim_type()) );

  add_str("vpiModule", mPrimitive->parent_module()->full_name());
  add_str("vpiScope", mPrimitive->parent()->full_name());

  add_strength("vpiStrength0", mPrimitive->drive0());
  add_strength("vpiStrength1", mPrimitive->drive1());
  add_delay(mPrimitive->delay());

  if ( mPrimitive->type() == kVpiUdp ) {
    add_str("vpiUdpDefn", mPrimitive->udp_defn()->def_name());
  }
  add_int("port num", mPrimitive->port_num());

  add_child( new VmPrimTermListNode(mPrimitive) );
}


//////////////////////////////////////////////////////////////////////
// クラス VmPrimTermListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] primitive 親のプリミティブ
VmPrimTermListNode::VmPrimTermListNode(const VlPrimitive* primitive) :
  mPrimitive(primitive)
{
}

// @brief デストラクタ
VmPrimTermListNode::~VmPrimTermListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmPrimTermListNode::data(int column,
			 int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiPtimTerm list";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
VmPrimTermListNode::loc() const
{
  ymuint n = mPrimitive->port_num();
  if ( n == 0 ) {
    return FileRegion();
  }
  else {
    FileRegion first = mPrimitive->prim_term(0)->file_region();
    FileRegion last = mPrimitive->prim_term(n - 1)->file_region();
    return FileRegion(first, last);
  }
}

// @brief 子供の配列を作る．
void
VmPrimTermListNode::expand() const
{
  ymuint n = mPrimitive->port_num();
  for (ymuint i = 0; i < n; ++ i) {
    add_child( new VmPrimTermNode(mPrimitive->prim_term(i)) );
  }
}


//////////////////////////////////////////////////////////////////////
// クラス VmPrimTermNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] prim_term プリミティブの端子
VmPrimTermNode::VmPrimTermNode(const VlPrimTerm* prim_term) :
  mPrimTerm(prim_term)
{
}

// @brief デストラクタ
VmPrimTermNode::~VmPrimTermNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmPrimTermNode::data(int column,
		     int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiPtimTerm";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
VmPrimTermNode::loc() const
{
  return mPrimTerm->file_region();
}

// @brief 子供の配列を作る．
void
VmPrimTermNode::expand() const
{
  add_str("vpiPrimitive", mPrimTerm->primitive()->full_name());
  add_int("vpiTermIndex", mPrimTerm->term_index());
  add_dir(mPrimTerm->direction());
  add_expr("vpiExpr", mPrimTerm->expr());
}


//////////////////////////////////////////////////////////////////////
// クラス VmContAssignListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] ca_list 継続的代入文のリスト
VmContAssignListNode::VmContAssignListNode(const vector<const VlContAssign*>& ca_list) :
  mContAssignArray(ca_list)
{
}

// @brief デストラクタ
VmContAssignListNode::~VmContAssignListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmContAssignListNode::data(int column,
			   int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiContAssign list";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
VmContAssignListNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
void
VmContAssignListNode::expand() const
{
  for (vector<const VlContAssign*>::const_iterator p = mContAssignArray.begin();
       p != mContAssignArray.end(); ++ p) {
    add_child( new VmContAssignNode(*p) );
  }
}


//////////////////////////////////////////////////////////////////////
// クラス VmContAssignNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] cont_assign 継続的代入文
VmContAssignNode::VmContAssignNode(const VlContAssign* cont_assign) :
  mContAssign(cont_assign)
{
}

// @brief デストラクタ
VmContAssignNode::~VmContAssignNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmContAssignNode::data(int column,
		       int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiContAssign";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
VmContAssignNode::loc() const
{
  return mContAssign->file_region();
}

// @brief 子供の配列を作る．
void
VmContAssignNode::expand() const
{
  add_bool("vpiNetDeclAssign", mContAssign->has_net_decl_assign());
  add_str("vpiModule", mContAssign->module()->full_name());

  add_strength("vpiStrength0", mContAssign->drive0());
  add_strength("vpiStrength1", mContAssign->drive1());
  add_delay(mContAssign->delay());

  add_int("vpiSize", mContAssign->bit_size());

  add_expr("vpiLhs", mContAssign->lhs());
  add_expr("vpiRhs", mContAssign->rhs());
}

END_NAMESPACE_YM_VERILOG
