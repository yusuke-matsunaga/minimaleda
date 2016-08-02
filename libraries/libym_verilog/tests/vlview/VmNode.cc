
/// @file libym_verilog/tests/vlview/VmNode.cc
/// @brief VmNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VmNode.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include "VmNode.h"
#include "ym_verilog/VlMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス VmNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
VmNode::VmNode() :
  mExpanded(false)
{
}

// @brief デストラクタ
VmNode::~VmNode()
{
  for (vector<VmNode*>::iterator p = mChildren.begin();
       p != mChildren.end(); ++ p) {
    delete *p;
  }
}

// @brief 子供を追加する．
void
VmNode::add_child(VmNode* node) const
{
  mChildren.push_back(node);
}


//////////////////////////////////////////////////////////////////////
// クラス VmNode1
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
VmNode1::VmNode1(const VlMgr& mgr) :
  mMgr(mgr)
{
}

// @brief デストラクタ
VmNode1::~VmNode1()
{
}

// @brief VlMgr を取り出す．
const VlMgr&
VmNode1::vl_mgr() const
{
  return mMgr;
}

// @brief スコープに属する要素を追加する．
// @param[in] scope スコープ
void
VmNode1::add_scope_item(const VlNamedObj* scope) const
{
  {
    vector<const VlDecl*> decl_list;
    if ( vl_mgr().find_decl_list(scope, vpiParameter, decl_list) ) {
      add_decllist("vpiParameter", decl_list);
    }
  }
  {
    vector<const VlParamAssign*> paramassign_list;
    if ( vl_mgr().find_paramassign_list(scope, paramassign_list) ) {
      add_paramassignlist(paramassign_list);
    }
  }
  {
    vector<const VlDefParam*> defparam_list;
    if ( vl_mgr().find_defparam_list(scope, defparam_list) ) {
      add_defparamlist(defparam_list);
    }
  }
  {
    vector<const VlDecl*> decl_list;
    if ( vl_mgr().find_decl_list(scope, vpiSpecParam, decl_list) ) {
      add_decllist("vpiSepcParam", decl_list);
    }
  }
  {
    vector<const VlDecl*> decl_list;
    if ( vl_mgr().find_decl_list(scope, vpiNet, decl_list) ) {
      add_decllist("vpiNet", decl_list);
    }
  }
  {
    vector<const VlDecl*> decl_list;
    if ( vl_mgr().find_decl_list(scope, vpiNetArray, decl_list) ) {
      add_decllist("vpiNetArray", decl_list);
    }
  }
  {
    vector<const VlDecl*> decl_list;
    if ( vl_mgr().find_decl_list(scope, vpiReg, decl_list) ) {
      add_decllist("vpiReg", decl_list);
    }
  }
  {
    vector<const VlDecl*> decl_list;
    if ( vl_mgr().find_decl_list(scope, vpiRegArray, decl_list) ) {
      add_decllist("vpiRegArray", decl_list);
    }
  }
  {
    vector<const VlDecl*> decl_list;
    if ( vl_mgr().find_decl_list(scope, vpiVariables, decl_list) ) {
      add_decllist("vpiVariables", decl_list);
    }
  }
  {
    vector<const VlDecl*> decl_list;
    if ( vl_mgr().find_decl_list(scope, vpiVariables, decl_list) ) {
      add_decllist("vpiVariables", decl_list);
    }
  }
  {
    vector<const VlDecl*> decl_list;
    if ( vl_mgr().find_decl_list(scope, vpiNamedEvent, decl_list) ) {
      add_decllist("vpiNamedEvent", decl_list);
    }
  }
  {
    vector<const VlDecl*> decl_list;
    if ( vl_mgr().find_decl_list(scope, vpiNamedEventArray, decl_list) ) {
      add_decllist("vpiNamedEventArray", decl_list);
    }
  }
  {
    vector<const VlNamedObj*> scope_list;
    if ( vl_mgr().find_genblock_list(scope, scope_list) ) {
      add_scopelist(scope_list);
    }
  }
  {
    vector<const VlTaskFunc*> task_list;
    if ( vl_mgr().find_task_list(scope, task_list) ) {
      add_tasklist(task_list);
    }
  }
  {
    vector<const VlTaskFunc*> function_list;
    if ( vl_mgr().find_function_list(scope, function_list) ) {
      add_functionlist(function_list);
    }
  }
  {
    vector<const VlContAssign*> contassign_list;
    if ( vl_mgr().find_contassign_list(scope, contassign_list) ) {
      add_contassignlist(contassign_list);
    }
  }
  {
    vector<const VlModule*> module_list;
    if ( vl_mgr().find_module_list(scope, module_list) ) {
      add_moduleinstlist(module_list);
    }
  }
  {
    vector<const VlModuleArray*> modulearray_list;
    if ( vl_mgr().find_modulearray_list(scope, modulearray_list) ) {
      add_modulearraylist(modulearray_list);
    }
  }
  {
    vector<const VlPrimitive*> primitive_list;
    if ( vl_mgr().find_primitive_list(scope, primitive_list) ) {
      add_primitivelist(primitive_list);
    }
  }
  {
    vector<const VlPrimArray*> primarray_list;
    if ( vl_mgr().find_primarray_list(scope, primarray_list) ) {
      add_primarraylist(primarray_list);
    }
  }
}

END_NAMESPACE_YM_VERILOG
