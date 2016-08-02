
/// @file libym_verilog/elb/VlDumperImpl_scope.cc
/// @brief VlDumperImpl の実装ファイル (scope担当)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlDumperImpl_scope.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include "VlDumperImpl.h"

#include <ym_verilog/VlMgr.h>
#include <ym_verilog/vl/VlModule.h>
#include <ym_verilog/vl/VlTaskFunc.h>


BEGIN_NAMESPACE_YM_VERILOG

// scope の内容を出力する関数
// IEEE1364-2001 p.634
void
VlDumperImpl::put_scope(const char* label,
			const VlMgr& mgr,
			const VlNamedObj* scope)
{
  VlDumpHeader x(this, label, "Scope");

  put("mFileRegion", scope->file_region() );
  put("vpiFullName", scope->full_name() );

  put_scope_sub(mgr, scope);
}

void
VlDumperImpl::put_scope_sub(const VlMgr& mgr,
			    const VlNamedObj* scope)
{
  vector<const VlDecl*> decl_list;
  if ( mgr.find_decl_list(scope, vpiParameter, decl_list) ) {
    put_decl_list("vpiParameter", mgr, decl_list);
  }
  vector<const VlParamAssign*> paramassign_list;
  if ( mgr.find_paramassign_list(scope, paramassign_list) ) {
    put_paramassign_list("vpiParamAssign", mgr, paramassign_list);
  }
  vector<const VlDefParam*> defparam_list;
  if ( mgr.find_defparam_list(scope, defparam_list) ) {
    put_defparam_list("vpiDefParam", mgr, defparam_list);
  }
  if ( mgr.find_decl_list(scope, vpiSpecParam, decl_list) ) {
    put_decl_list("vpiSpecParam", mgr, decl_list);
  }
  if ( mgr.find_decl_list(scope, vpiNet, decl_list) ) {
    put_decl_list("vpiNet", mgr, decl_list);
  }
  if ( mgr.find_decl_list(scope, vpiNetArray, decl_list) ) {
    put_decl_list("vpiNetArray", mgr, decl_list);
  }
  if ( mgr.find_decl_list(scope, vpiReg, decl_list) ) {
    put_decl_list("vpiReg", mgr, decl_list);
  }
  if ( mgr.find_decl_list(scope, vpiRegArray, decl_list) ) {
    put_decl_list("vpiRegArray", mgr, decl_list);
  }
  if ( mgr.find_decl_list(scope, vpiVariables, decl_list) ) {
    put_decl_list("vpiVariables", mgr, decl_list);
  }
  if ( mgr.find_decl_list(scope, kVpiNamedEvent, decl_list) ) {
    put_decl_list("vpiNamedEvent", mgr, decl_list);
  }
  if ( mgr.find_decl_list(scope, kVpiNamedEventArray, decl_list) ) {
    put_decl_list("vpiNamedEventArray", mgr, decl_list);
  }

  vector<const VlNamedObj*> scope_list;
  if ( mgr.find_genblock_list(scope, scope_list) ) {
    VlDumpHeader x(this, "vpiInternalScope", "ScopeList");
    for (vector<const VlNamedObj*>::iterator p = scope_list.begin();
	 p != scope_list.end(); ++ p) {
      put_scope("vpiInternalScope", mgr, *p);
    }
  }

  vector<const VlTaskFunc*> task_list;
  if ( mgr.find_task_list(scope, task_list) ) {
    for (vector<const VlTaskFunc*>::const_iterator p = task_list.begin();
	 p != task_list.end(); ++ p) {
      put_task("vpiTask", mgr, *p);
    }
  }
  vector<const VlTaskFunc*> func_list;
  if ( mgr.find_function_list(scope, func_list) ) {
    for (vector<const VlTaskFunc*>::const_iterator p = func_list.begin();
	 p != func_list.end(); ++ p) {
      put_function("vpiFunction", mgr, *p);
    }
  }

  vector<const VlContAssign*> contassign_list;
  if ( mgr.find_contassign_list(scope, contassign_list) ) {
    put_contassign_list("vpiContAssign", mgr, contassign_list);
  }

  vector<const VlModule*> module_list;
  if ( mgr.find_module_list(scope, module_list) ) {
    // module instance 名前だけ
    for (vector<const VlModule*>::iterator p = module_list.begin();
	 p != module_list.end(); ++ p) {
      const VlModule* module = *p;
      put("vpiModule", module->full_name());
    }
  }
  vector<const VlModuleArray*> modulearray_list;
  if ( mgr.find_modulearray_list(scope, modulearray_list) ) {
    put_modulearray_list("vpiModuleArray", mgr, modulearray_list);
  }
  vector<const VlPrimitive*> primitive_list;
  if ( mgr.find_primitive_list(scope, primitive_list) ) {
    put_primitive_list("vpiPrimitive", mgr, primitive_list);
  }
  vector<const VlPrimArray*> primarray_list;
  if ( mgr.find_primarray_list(scope, primarray_list) ) {
    put_primarray_list("vpiPrimitiveArray", mgr, primarray_list);
  }
}

END_NAMESPACE_YM_VERILOG
