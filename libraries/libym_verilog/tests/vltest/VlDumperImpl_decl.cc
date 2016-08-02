
/// @file libym_verilog/elb/VlDumperImpl_decl.cc
/// @brief VlDumper の実装ファイル (decl担当)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlDumperImpl_decl.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include "VlDumperImpl.h"

#include <ym_verilog/VlMgr.h>
#include <ym_verilog/vl/VlIODecl.h>
#include <ym_verilog/vl/VlDecl.h>
#include <ym_verilog/vl/VlParamAssign.h>
#include <ym_verilog/vl/VlModule.h>
#include <ym_verilog/vl/VlUdp.h>
#include <ym_verilog/vl/VlTaskFunc.h>


BEGIN_NAMESPACE_YM_VERILOG

// io decl の内容を出力する関数
// IEEE 1364-2001 p.636
void
VlDumperImpl::put_iodecl(const char* label,
			 const VlMgr& mgr,
			 const VlIODecl* iodecl)
{
  if ( iodecl == NULL ) {
    if ( !nullptr_suppress_mode() ) {
      VlDumpHeader x(this, label, "null-pointer");
    }
    return;
  }

  const char* nm = NULL;
  switch ( iodecl->direction() ) {
  case kVpiInput:  nm = "Input"; break;
  case kVpiOutput: nm = "Output"; break;
  case kVpiInout:  nm = "Inout"; break;
  default: assert_not_reached(__FILE__, __LINE__);
  }
  VlDumpHeader x(this, label, nm);

  put("FileRegion", iodecl->file_region() );
  put("vpiName", iodecl->name() );
  put("vpiSigned", iodecl->is_signed() );
#if 0
  put("vpiScalar", iodecl->is_scalar() );
  put("vpiVector", iodecl->is_vector() );
#else
#warning "TODO: IODecl の scalar/vector"
#endif
  put("vpiSize",   iodecl->bit_size() );

  const VlModule* module = iodecl->module();
  if ( module ) {
    put("vpiModule", module->full_name() );
  }
  const VlUdpDefn* udp = iodecl->udp_defn();
  if ( udp ) {
    put("vpiUdpDefn", udp->def_name() );
  }
  const VlTaskFunc* task = iodecl->task();
  if ( task ) {
    put("vpiTask", task->full_name() );
  }
  const VlTaskFunc* func = iodecl->function();
  if ( func ) {
    put("vpiFunction", func->full_name() );
  }
  put_expr("vpiLeftRange", mgr, iodecl->left_range() );
  put_expr("vpiRightRange", mgr, iodecl->right_range() );

#if 0
  put("vpiExpr", handle.get_handle(vpiExpr), vpiDecompile);
#else
#warning "TODO: IODecl の expr()"
#endif
}

// 宣言要素の内容を出力する関数
// IEEE 1364-2001 p.638
void
VlDumperImpl::put_decl(const char* label,
		       const VlMgr& mgr,
		       const VlDecl* decl)
{
  if ( decl == NULL ) {
    if ( !nullptr_suppress_mode() ) {
      VlDumpHeader x(this, label, "null-pointer");
    }
    return;
  }

  const char* nm = NULL;
  switch ( decl->type() ) {
  case kVpiNet:             nm = "Net"; break;
  case kVpiNetArray:        nm = "NetArray"; break;
  case kVpiReg:             nm = "Reg"; break;
  case kVpiRegArray:        nm = "RegArray"; break;
  case kVpiIntegerVar:      nm = "IntegerVar"; break;
  case kVpiRealVar:         nm = "RealVar"; break;
  case kVpiTimeVar:         nm = "TimeVar"; break;
  case kVpiVarSelect:       nm = "VarSelect"; break;
  case kVpiNamedEvent:      nm = "NamedEvent"; break;
  case kVpiNamedEventArray: nm = "NamedEventArray"; break;
  case kVpiParameter:       nm = "Parameter"; break;
  case kVpiSpecParam:       nm = "SpecParam"; break;
  default: assert_not_reached( __FILE__, __LINE__ );
  }
  VlDumpHeader x(this, label, nm);

  put("FileRegion", decl->file_region() );
  put("vpiFullName", decl->full_name() );
#if 0
  put("vpiExpanded", decl->is_expanded() );
#else
#warning "TODO: Decl の expanded"
#endif
#if 0
  put("vpiImplicitDecl", handle.get_bool(vpiImplicitDecl));
#else
#warning "TODO: Decl の implicit decl"
#endif
#if 0
  put("vpiNetDeclAssign", handle.get_bool(vpiNetDeclAssign));
#else
#warning "TODO: Decl の net decl assign"
#endif
#if 0
  put("vpiScalar", handle.get_bool(vpiScalar));
  put("vpiVector", handle.get_bool(vpiVector));
  put("vpiExplicitScalar", handle.get_bool(vpiExplicitScalared));
  put("vpiExplicitVector", handle.get_bool(vpiExplicitVectored));
#else
#warning "TODO: Decl の scalar/vector, explicit scalar/explicit vector"
#endif
  put("vpiSigned", decl->is_signed() );
  put("vpiSize", decl->bit_size() );

  put("vpiModule", decl->parent_module()->full_name() );
  put("vpiScope", decl->parent()->full_name() );

  put("vpiArray", decl->is_array());
  put("vpiMultiArray", decl->is_multi_array());
  if ( decl->is_array_member() ) {
    put("vpiParent", decl->parent_array()->full_name());
  }

  if ( decl->is_array() ) {
    ymuint32 n = decl->dimension();
    for (ymuint32 i = 0; i < n; ++ i) {
      put_range("vpiRange", mgr, decl->range(i) );
    }
  }

  if ( decl->type() == kVpiNet ) {
    put("vpiStrength0", decl->drive0() );
    put("vpiStrength1", decl->drive1() );
    put("vpiChargeStrength",decl->charge() );
    put("vpiDelay", decl->delay() );
  }

  if ( decl->type() == kVpiReg ) {
#if 0
    put("vpiExpr", handle.get_handle(vpiExpr));
#else
#warning "TODO: Reg の expr"
#endif
  }

  if ( decl->is_multi_array_member() ) {
    vector<const VlExpr*> index_list;
    decl->index(index_list);
    for (ymuint i = 0; i < index_list.size(); ++ i) {
      put("vpiIndex", index_list[i]);
    }
  }
  else if ( decl->is_array_member() ) {
    put("vpiIndex", decl->index());
  }

  if ( decl->type() == kVpiReg ||
       decl->type() == kVpiIntegerVar ||
       decl->type() == kVpiTimeVar ) {
#if 0
    put("vpiPortInst", handle.get_iterate(vpiPortInst));
    put("vpiPorts", handle.get_iterate(vpiPorts));
#else
#warning "TODO: Decl の port inst/ports"
#endif
  }

  put_expr("vpiLeftRange", mgr, decl->left_range());
  put_expr("vpiRightRange", mgr, decl->right_range());

  if ( decl->type() == kVpiNet ) {
#if 0
    put("vpiDriver", handle.get_iterate(vpiDriver));
    put("vpiLocalDrver", handle.get_iterate(vpiLocalDriver));
    put("vpiLoad", handle.get_iterate(vpiLoad));
    put("vpiLocalLoad", handle.get_iterate(vpiLocalLoad));
    put("vpiContAssign", handle.get_iterate(vpiContAssign));
    put("vpiPrimTerm", handle.get_iterate(vpiPrimTerm));
    put("vpiPathTerm", handle.get_iterate(vpiPathTerm));
    put("vpiTchkTerm", handle.get_iterate(vpiTchkTerm));
    put("vpiUse", handle.get_iterate(vpiUse));

    put("vpiSimNet", handle.get_handle(vpiSimNet));

    if ( mBitExpandMode ) {
      put("vpiBit", handle.get_iterate(vpiBit));
    }
#else
#warning "TODO: Net のもろもろ"
#endif
  }
  if ( decl->type() == kVpiReg ) {
#if 0
    put("vpiDriver", handle.get_iterate(vpiDriver));
    put("vpiLoad", handle.get_iterate(vpiLoad));
    put("vpiContAssign", handle.get_iterate(vpiContAssign));
    put("vpiPrimTerm", handle.get_iterate(vpiPrimTerm));
    put("vpiPathTerm", handle.get_iterate(vpiPathTerm));
    put("vpiTchkTerm", handle.get_iterate(vpiTchkTerm));
    put("vpiUse", handle.get_iterate(vpiUse));

    if ( mBitExpandMode ) {
      put("vpiBit", handle.get_iterate(vpiBit));
    }
#else
#warning "TODO: Reg のもろもろ"
#endif
  }
  if ( decl->type() == kVpiIntegerVar ||
       decl->type() == kVpiTimeVar ||
       decl->type() == kVpiRealVar ) {
#if 0
  put("vpiUse", handle.get_iterate(vpiUse));
  put("vpiVarSelect", handle.get_iterate(vpiVarSelect));
#else
#warning "TODO: Variables の use"
#warning "TODO: Variables の VarSelect"
#endif
  }
  if ( decl->type() == kVpiParameter ||
       decl->type() == kVpiSpecParam ) {
    put("vpiLocalParam", decl->is_local_param() );
    put("vpiConstType", decl->is_consttype() );
#if 0
    put("vpiExpr", handle.get_handle(vpiExpr));
    put("vpiUse", handle.get_iterate(vpiUse));
#else
#warning "TODO: Parameter の Expr"
#warning "TODO: Parameter の Use"
#endif
  }
}

// @brief 宣言要素のリストの内容を出力する関数
void
VlDumperImpl::put_decl_list(const char* label,
			    const VlMgr& mgr,
			    const vector<const VlDecl*>& decl_list)
{
  VlDumpHeader x(this, label, "DeclList");

  for (vector<const VlDecl*>::const_iterator p = decl_list.begin();
       p != decl_list.end(); ++ p) {
    put_decl(label, mgr, *p);
  }
}

// @brief def param のリストの内容を出力する関数
void
VlDumperImpl::put_defparam_list(const char* label,
				const VlMgr& mgr,
				const vector<const VlDefParam*>& defparam_list)
{
  VlDumpHeader x(this, label, "DefParamList");

  for (vector<const VlDefParam*>::const_iterator p = defparam_list.begin();
       p != defparam_list.end(); ++ p) {
    const VlDefParam* defparam = *p;

    VlDumpHeader x(this, label, "DefParam");

    put("FileRegion", defparam->file_region() );
    put("vpiModule", defparam->parent()->full_name() );
    put("vpiLhs", defparam->lhs()->full_name() );
    put_expr("vpiRhs", mgr, defparam->rhs() );
  }
}

// @brief param assign のリストの内容を出力する関数
void
VlDumperImpl::put_paramassign_list(const char* label,
				   const VlMgr& mgr,
				   const vector<const VlParamAssign*>& pa_list)
{
  VlDumpHeader x(this, label, "ParamAssignList");

  for (vector<const VlParamAssign*>::const_iterator p = pa_list.begin();
       p != pa_list.end(); ++ p) {
    const VlParamAssign* paramassign = *p;

    VlDumpHeader x(this, label, "ParamAssign");

    put("FileRegion", paramassign->file_region() );
    put("vpiModule", paramassign->parent()->full_name() );
    put("vpiConnByName", paramassign->is_conn_by_name() );
    put("vpiLhs", paramassign->lhs()->full_name() );
    put_expr("vpiRhs", mgr, paramassign->rhs() );
  }
}

#if 0
// net bit の内容を出力する関数
// IEEE 1364-2001 p.638
void
VlDumperImpl::put_net_bit(const char* label,
		      const VpiHandle& handle)
{
  VlDumpHeader x(this, label, "NetBit");

  put("FileRegion", handle.file_region());

  put("vpiFullName", handle.get_str(vpiFullName));
  put("vpiArray", handle.get_bool(vpiArray));
  put("vpiMultiArray", handle.get_bool(vpiMultiArray));
  put("vpiExpanded", handle.get_bool(vpiExpanded));
  put("vpiImplicitDecl", handle.get_bool(vpiImplicitDecl));
  put("vpiNetDeclAssign", handle.get_bool(vpiNetDeclAssign));
  put("vpiScalar", handle.get_bool(vpiScalar));
  put("vpiVector", handle.get_bool(vpiVector));
  put("vpiExplicitScalar", handle.get_bool(vpiExplicitScalared));
  put("vpiExplicitVector", handle.get_bool(vpiExplicitVectored));
  put("vpiSigned", handle.get_bool(vpiSigned));
  put("vpiSize", handle.get_int(vpiSize));

  put("vpiParent", handle.get_handle(vpiParent), vpiFullName);
  put("vpiConstantSelect", handle.get_bool(vpiConstantSelect));

  put("vpiStrength0",
      static_cast<tVpiStrength>(handle.get_int(vpiStrength0)));
  put("vpiStrength1",
      static_cast<tVpiStrength>(handle.get_int(vpiStrength1)));
  put("vpiChargeStrength",
      static_cast<tVpiStrength>(handle.get_int(vpiChargeStrength)));

  put("vpiIndex", handle.get_handle(vpiIndex));
  put("vpiIndex", handle.get_iterate(vpiIndex));

  put("vpiPortInst", handle.get_iterate(vpiPortInst));
  put("vpiPorts", handle.get_iterate(vpiPorts));

  put("vpiLeftRange", handle.get_handle(vpiLeftRange));
  put("vpiRightRange", handle.get_handle(vpiRightRange));

  put("vpiDriver", handle.get_iterate(vpiDriver));
  put("vpiLocalDrver", handle.get_iterate(vpiLocalDriver));
  put("vpiLoad", handle.get_iterate(vpiLoad));
  put("vpiLocalLoad", handle.get_iterate(vpiLocalLoad));

  put("vpiContAssign", handle.get_iterate(vpiContAssign));
  put("vpiPrimTerm", handle.get_iterate(vpiPrimTerm));
  put("vpiPathTerm", handle.get_iterate(vpiPathTerm));
  put("vpiTchkTerm", handle.get_iterate(vpiTchkTerm));
  put("vpiUse", handle.get_iterate(vpiUse));

  put("vpiSimNet", handle.get_handle(vpiSimNet));
}
#endif

#if 0
// net array の内容を出力する関数
// IEEE 1364-2001 p.638
void
VlDumperImpl::put_net_array(const char* label,
			const VlMgr& mgr,
			cosnt VlObj* obj)
{
  VlDecl* net_array = dynamic_cast<const VlDecl*>(obj);
  assert_cond( net_array , __FILE__, __LINE__);

  VlDumpHeader x(this, label, "NetArray");

  put("FileRegion", net_array->file_region() );

  put("vpiFullName", net_array->full_name() );
#if 0
  put("vpiScalar", handle.get_bool(vpiScalar));
  put("vpiVector", handle.get_bool(vpiVector));
#else
#warning "TODO: NetArray の vector/scalar"
#endif

  put("vpiSize", net_array->bit_size() );

  put("vpiModule", net_array->module()->full_name() );
  put("vpiScope",  net_array->scope()->full_name() );

  ymuint32 n = net_array->dimension();
  for (ymuint32 i = 0; i < n; ++ i) {
    put("vpiRange", mgr, net_array->range(i) );
  }

#if 0
  put("vpiNet", handle.get_iterate(vpiNet));
#else
#warning "TODO: NetArray の Net"
#endif
}
#endif

#if 0
// reg の内容を出力する関数
// IEEE 1364-2001 p.640
void
VlDumperImpl::put_reg(const char* label,
		      const VlMgr& mgr,
		      const VlObj* obj)
{
  VlDecl* reg = dynamic_cast<const VlDecl*>(obj);
  assert_cond( reg , __FILE__, __LINE__);

  VlDumpHeader x(this, label, "Reg");

  put("FileRegion", reg->file_region() );

  put("vpiFullName", reg->full_name() );
#if 0
  put("vpiScalar", handle.get_bool(vpiScalar));
  put("vpiVector", handle.get_bool(vpiVector));
#else
#warning "TODO: Reg の scalar/vector"
#endif
  put("vpiSigned", reg->is_signed() );
  put("vpiSize", reg->bit_size() );

  put("vpiModule", reg->module()->full_name() );
  put("vpiScope", reg->scope()->full_name() );

#if 0
  put("vpiArray", handle.get_bool(vpiArray));
  put("vpiMultiArray", handle.get_bool(vpiMultiArray));
  if ( handle.get_bool(vpiArray) ) {
    put_str("vpiParent", handle.get_handle(vpiParent), vpiFullName);
  }
#else
#warning "TODO: Reg の array/multi array/parent"
#endif

#if 0
  put("vpiExpr", handle.get_handle(vpiExpr));
#else
#warning "TODO: Reg の expr"
#endif

#if 0
  put("vpiIndex", handle.get_iterate(vpiIndex));
#else
#warning "TODO: Reg の index"
#endif

#if 0
  put("vpiPortInst", handle.get_iterate(vpiPortInst));
  put("vpiPorts", handle.get_iterate(vpiPorts));
#else
#warning "TODO: Reg の port inst/ports"
#endif

  put("vpiLeftRange", mgr, reg->left_range() );
  put("vpiRightRange", mgr, reg->right_range() );

#if 0
  put("vpiDriver", handle.get_iterate(vpiDriver));
  put("vpiLoad", handle.get_iterate(vpiLoad));
  put("vpiContAssign", handle.get_iterate(vpiContAssign));
  put("vpiPrimTerm", handle.get_iterate(vpiPrimTerm));
  put("vpiPathTerm", handle.get_iterate(vpiPathTerm));
  put("vpiTchkTerm", handle.get_iterate(vpiTchkTerm));
  put("vpiUse", handle.get_iterate(vpiUse));
#else
#warning "TODO: Reg のもろもろ"
#endif

#if 0
  if ( mBitExpandMode ) {
    put("vpiBit", handle.get_iterate(vpiBit));
  }
#else
#warning "TODO: Reg のビット展開"
#endif
}
#endif

#if 0
// reg bit の内容を出力する関数
// IEEE 1364-2001 p.640
void
VlDumperImpl::put_reg_bit(const char* label,
		       const VpiHandle& handle)
{
  VlDumpHeader x(this, label, "RegBit");

  put("FileRegion", handle.file_region());

  put("vpiFullName", handle.get_str(vpiFullName));
  put("vpiArray", handle.get_bool(vpiArray));
  put("vpiMultiArray", handle.get_bool(vpiMultiArray));
  put("vpiScalar", handle.get_bool(vpiScalar));
  put("vpiVector", handle.get_bool(vpiVector));
  put("vpiSigned", handle.get_bool(vpiSigned));
  put("vpiSize", handle.get_int(vpiSize));

  put("vpiExpr", handle.get_handle(vpiExpr));

  put_str("vpiParent", handle.get_handle(vpiParent), vpiFullName);
  put("vpiConstantSelect", handle.get_bool(vpiConstantSelect));

  put("vpiIndex", handle.get_handle(vpiIndex));
  put("vpiIndex", handle.get_iterate(vpiIndex));

  put("vpiPortInst", handle.get_iterate(vpiPortInst));
  put("vpiPorts", handle.get_iterate(vpiPorts));

  put("vpiLeftRange", handle.get_handle(vpiLeftRange));
  put("vpiRightRange", handle.get_handle(vpiRightRange));

  put("vpiDriver", handle.get_iterate(vpiDriver));
  put("vpiLoad", handle.get_iterate(vpiLoad));
  put("vpiContAssign", handle.get_iterate(vpiContAssign));
  put("vpiPrimTerm", handle.get_iterate(vpiPrimTerm));
  put("vpiPathTerm", handle.get_iterate(vpiPathTerm));
  put("vpiTchkTerm", handle.get_iterate(vpiTchkTerm));
  put("vpiUse", handle.get_iterate(vpiUse));
}
#endif

#if 0
// reg array の内容を出力する関数
// IEEE 1364-2001 p.640
void
VlDumperImpl::put_reg_array(const char* label,
			const VlMgr& mgr,
			const VlObj* obj)
{
  VlDecl* reg_array = dynamic_cast<const VlDecl*>(obj);
  assert_cond( reg_array , __FILE__, __LINE__);

  VlDumpHeader x(this, label, "RegArray");

  put("FileRegion", reg_array->file_region() );

  put("vpiFullName", reg_array->full_name() );
#if 0
  put("vpiScalar", handle.get_bool(vpiScalar));
  put("vpiVector", handle.get_bool(vpiVector));
#else
#warning "TODO: RegArray の scalar/vector"
#endif
  put("vpiSize", reg_array->bit_size() );

  put("vpiModule", reg_array->module()->full_name() );
  put("vpiScope", reg_array->scope()->full_name() );

  ymuint32 n = net_array->dimension();
  for (ymuint32 i = 0; i < n; ++ i) {
    put("vpiRange", mgr, net_array->range(i) );
  }

#if 0
  put("vpiReg", handle.get_iterate(vpiReg));
#else
#warning "TODO: RegArray の Reg"
#endif
}
#endif

#if 0
// variables の内容を出力する関数
// IEEE 1364-2001 p.642
void
VlDumperImpl::put_variables(const char* label,
			const VlMgr& mgr,
			const VlObj* obj)
{
  VlDecl* var = dynamic_cast<const VlDecl*>(obj);
  assert_cond( var , __FILE__, __LINE__);

  const char* nm = NULL;
  switch ( var->type() ) {
  case vpiIntegerVar: nm = "integer var"; break;
  case vpiTimeVar:    nm = "time var"; break;
  case vpiRealVar:    nm = "real var"; break;
  default: assert_not_reached(__FILE__, __LINE__);
  }
  VlDumpHeader x(this, label, nm);

  put("FileRegion", var->file_region() );

  put("vpiFullName", var->full_name() );
#if 0
  put("vpiArray", handle.get_bool(vpiArray));
  put("vpiMultiArray", handle.get_bool(vpiMultiArray));
#else
#warning "TODO: Varriables の array/multi array"
#endif
  put("vpiSigned", var->is_signed() );
  put("vpiSize", var->bit_size() );

  put("vpiModule", var->module()->full_name() );
  put("vpiScope", var->scope()->full_name() );

#if 0
  if ( handle.get_bool(vpiArray) ) {
    put("vpiLeftRange", handle.get_handle(vpiLeftRange));
    put("vpiRightRange", handle.get_handle(vpiRightRange));
    put("vpiRange", handle.get_iterate(vpiRange));
  }
#else
#warning "TODO: Variables の array, left_range/right_range"
#endif

#if 0
  put("vpiPortInst", handle.get_iterate(vpiPortInst));
  put("vpiPorts", handle.get_iterate(vpiPorts));
  put("vpiUse", handle.get_iterate(vpiUse));

  put("vpiVarSelect", handle.get_iterate(vpiVarSelect));
#else
#warning "TODO: Variables の port inst/ports"
#endif
}
#endif

#if 0
// var select の内容を出力する関数
// IEEE 1364-2001 p.642
void
VlDumperImpl::put_var_select(const char* label,
			  const VpiHandle& handle)
{
  VlDumpHeader x(this, label, "VarSelect");

  put("FileRegion", handle.file_region());

  put("vpiFullName", handle.get_str(vpiFullName));
  put("vpiConstantSelect", handle.get_bool(vpiConstantSelect));
  put("vpiSize", handle.get_int(vpiSize));

  put("vpiIndex", handle.get_handle(vpiIndex));
  put("vpiIndex", handle.get_iterate(vpiIndex));

  put("vpiUse", handle.get_iterate(vpiUse));
}
#else
#warning "TODO: VarSelect"
#endif

#if 0
// named  event の内容を出力する関数
// IEEE 1364-2001 p.644
void
VlDumperImpl::put_named_event(const char* label,
			  const VlMgr& mgr,
			  const VlObj* obj)
{
  VlDumpHeader x(this, label, "NamedEvent");

  put("FileRegion", handle.file_region());

  put("vpiFullName", handle.get_str(vpiFullName));

  put_str("vpiModule", handle.get_handle(vpiModule), vpiFullName);
  put_str("vpiScope", handle.get_handle(vpiScope), vpiFullName);

  put("vpiArray", handle.get_bool(vpiArray));
  put("vpiMultiArray", handle.get_bool(vpiMultiArray));
  if ( handle.get_bool(vpiArray) ) {
    put_str("vpiParent", handle.get_handle(vpiParent), vpiFullName);
  }
}
#endif

#if 0
// named event array の内容を出力する関数
// IEEE 1364-2001 p.644
void
VlDumperImpl::put_named_event_array(const char* label,
				 const VpiHandle& handle)
{
  VlDumpHeader x(this, label, "NamedEventArray");

  put("FileRegion", handle.file_region());

  put("vpiFullName", handle.get_str(vpiFullName));

  put_str("vpiModule", handle.get_handle(vpiModule), vpiFullName);
  put_str("vpiScope", handle.get_handle(vpiScope), vpiFullName);

  put("vpiRange", handle.get_iterate(vpiRange));

  put("vpiNamedEvent", handle.get_iterate(vpiNamedEvent));
}
#endif

#if 0
// parameter の内容を出力する関数
// IEEE 1364-2001 p.645
void
VlDumperImpl::put_parameter(const char* label,
			 const VpiHandle& handle)
{
  VlDumpHeader x(this, label, "Parameter");

  put("FileRegion", handle.file_region());
  put("vpiFullName", handle.get_str(vpiFullName));

  put("vpiLocalParam", handle.get_bool(vpiLocalParam));
  put("vpiConstType",
      static_cast<tVpiConstType>(handle.get_int(vpiConstType)));
  put("vpiSigned", handle.get_bool(vpiSigned));
  put("vpiSize", handle.get_int(vpiSize));

  put_str("vpiModule", handle.get_handle(vpiModule), vpiFullName);
  put_str("vpiScope", handle.get_handle(vpiScope), vpiFullName);

  put("vpiLeftRange", handle.get_handle(vpiLeftRange));
  put("vpiRightRange", handle.get_handle(vpiRightRange));

  put("vpiExpr", handle.get_handle(vpiExpr));

  put("vpiUse", handle.get_iterate(vpiUse));
}
#endif

#if 0
// spec param の内容を出力する関数
// IEEE 1364-2001 p.645
void
VlDumperImpl::put_spec_param(const char* label,
			  const VpiHandle& handle)
{
  VlDumpHeader x(this, label, "SpecParam");

  put("FileRegion", handle.file_region());

  put("vpiFullName", handle.get_str(vpiFullName));

  put("vpiConstType",
      static_cast<tVpiConstType>(handle.get_int(vpiConstType)));
  put("vpiSize", handle.get_int(vpiSize));

  put_str("vpiModule", handle.get_handle(vpiModule), vpiFullName);
  put_str("vpiScope", handle.get_handle(vpiScope), vpiFullName);

  put("vpiExpr", handle.get_handle(vpiExpr));

  put("vpiUse", handle.get_iterate(vpiUse));
}
#endif

END_NAMESPACE_YM_VERILOG
