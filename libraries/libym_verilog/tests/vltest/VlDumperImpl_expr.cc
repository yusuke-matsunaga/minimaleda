
/// @file libym_verilog/elb/VlDumperImpl_expr.cc
/// @brief VlDumperImpl の実装ファイル (expr担当)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlDumperImpl_expr.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include "VlDumperImpl.h"

#include <ym_verilog/VlMgr.h>
#include <ym_verilog/vl/VlRange.h>
#include <ym_verilog/vl/VlDelay.h>
#include <ym_verilog/vl/VlExpr.h>
#include <ym_verilog/vl/VlTaskFunc.h>
#include <ym_verilog/vl/VlUserSystf.h>
#include <ym_verilog/vl/VlDecl.h>
#include <ym_verilog/vl/VlPrimitive.h>


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// 式の内容の出力
//////////////////////////////////////////////////////////////////////

// @brief 式の内容を出力する関数
void
VlDumperImpl::put_expr(const char* label,
		       const VlMgr& mgr,
		       const VlExpr* expr)
{
  if ( expr == NULL ) {
    if ( !nullptr_suppress_mode() ) {
      VlDumpHeader x(this, label, "null-pointer");
    }
    return;
  }

  const char* nm = NULL;
  switch ( expr->type() ) {
  case kVpiOperation:              nm = "Operation"; break;
  case kVpiConstant:               nm = "Constant"; break;
  case kVpiFuncCall:               nm = "FuncCall"; break;
  case kVpiSysFuncCall:            nm = "SysFuncCall"; break;
  default:
    if ( expr->is_primary() ) {
      nm = "Reference";
    }
    else if ( expr->is_bitselect() ) {
      nm = "BitSelect";
    }
    else if ( expr->is_partselect() ) {
      nm = "PartSelect";
    }
    break;
  }

  VlDumpHeader x(this, label, nm);
  put("FileRegion", expr->file_region());

  switch ( expr->type() ) {
  case kVpiOperation:
    put("vpiOpType", expr->op_type());
    for (ymuint32 i = 0; i < expr->operand_num(); ++ i) {
      put_expr("vpiOperand", mgr, expr->operand(i));
    }
    break;

  case kVpiConstant:
    put("vpiConstType", expr->constant_type());
    put("vpiDecompile", expr->decompile()); // <= 手抜き
    break;

  case kVpiFuncCall:
    put("vpiFunction", expr->function()->full_name());
    for (ymuint32 i = 0; i < expr->argument_num(); ++ i) {
      put_expr("vpiArgument", mgr, expr->argument(i));
    }
    break;

  case kVpiSysFuncCall:
    put("vpiUserSysTf", expr->user_systf()->name());
    for (ymuint32 i = 0; i < expr->argument_num(); ++ i) {
      put_expr("vpiArgument", mgr, expr->argument(i));
    }
    break;

  default:
    if ( expr->is_bitselect() ) {
      put("vpiConstantSelect", expr->is_constant_select());
      if ( expr->decl_obj() ) {
	put("vpiParent", expr->decl_obj()->full_name());
	if ( expr->declarray_dimension() > 0 ) {
	  for (ymuint32 i = 0; i < expr->declarray_dimension(); ++ i) {
	    put_expr("array_index", mgr, expr->declarray_index(i));
	  }
	}
	put_expr("vpiIndex", mgr, expr->index());
      }
      else {
	put_expr("vpiParent", mgr, expr->parent_expr());
	put("vpiIndex", expr->index_val());
      }
    }
    else if ( expr->is_partselect() ) {
      put("vpiConstantSelect", expr->is_constant_select());
      if ( expr->decl_obj() ) {
	put("vpiParent", expr->decl_obj()->full_name());
	if ( expr->declarray_dimension() > 0 ) {
	  for (ymuint32 i = 0; i < expr->declarray_dimension(); ++ i) {
	    put_expr("array_index", mgr, expr->declarray_index(i));
	  }
	}
	put_expr("vpiLeftRange", mgr, expr->left_range());
	put_expr("vpiRightRange", mgr, expr->right_range());
      }
      else {
	put_expr("vpiParent", mgr, expr->parent_expr());
	put("vpiLeftRange", expr->left_range_val());
	put("vpiRightRange", expr->right_range_val());
      }
    }
    else if ( expr->is_primary() ) {
      if ( expr->decl_obj() ) {
	put("decl_obj", expr->decl_obj()->full_name());
	if ( expr->declarray_dimension() > 0 ) {
	  for (ymuint32 i = 0; i < expr->declarray_dimension(); ++ i) {
	    put_expr("array_index", mgr, expr->declarray_index(i));
	  }
	}
      }
      else if ( expr->scope_obj() ) {
	put("scope_obj", expr->scope_obj()->full_name());
      }
      else if ( expr->primitive_obj() ) {
	put("primitive_obj", expr->primitive_obj()->full_name());
      }
      else {
	assert_not_reached(__FILE__, __LINE__);
      }
    }
    else {
      assert_not_reached(__FILE__, __LINE__);
    }
    break;
  }
}

#if 0
// func call の内容を式として出力する関数
// IEEE 1364-2001 p. 650
void
VlDumperImpl::put_func_call(const char* label,
			    const VpiHandle& handle)
{
  VlDumpHeader x(*this, label, "FuncCall");

  put("FileRegion", handle.file_region());

  put("vpiName", handle.get_str(vpiName));
  put_str("vpiFunction", handle.get_handle(vpiFunction), vpiFullName);
  put("vpiFuncType",
      static_cast<tVpiFuncType>(handle.get_int(vpiFuncType)));

  put("vpiArgument", handle.get_iterate(vpiArgument));
}

// system func call の内容を式として出力する関数
// IEEE 1364-2001 p.650
void
VlDumperImpl::put_sys_func_call(const char* label,
				const VpiHandle& handle)
{
  VlDumpHeader x(*this, label, "SystemFuncCall");

  put("FileRegion", handle.file_region());

  put("vpiName", handle.get_str(vpiName));
  put("vpiUserDefn", handle.get_bool(vpiUserDefn));
  put("vpiFuncType",
      static_cast<tVpiFuncType>(handle.get_int(vpiFuncType)));
  put("vpiUserSystf", handle.get_handle(vpiUserSystf));

  put("vpiArgument", handle.get_iterate(vpiArgument));
}

// bit select の内容を式として出力する関数
// IEEE 1364-2001 p.654
void
VlDumperImpl::put_bit_select(const char* label,
			  const VpiHandle& handle)
{
  VlDumpHeader x(*this, label, "bit select");

  put("FileRegion", handle.file_region());

  put("vpiConstantSelect", handle.get_bool(vpiConstantSelect));
  put_str("vpiParent", handle.get_handle(vpiParent), vpiFullName);

  put("vpiIndex", handle.get_handle(vpiIndex));

  put("vpiUse", handle.get_iterate(vpiUse));
}

// part select の内容を式として出力する関数
// IEEE 1364-2001 p.655
void
VlDumperImpl::put_part_select(const char* label,
			   const VpiHandle& handle)
{
  VlDumpHeader x(*this, label, "PartSelect");

  put("FileRegion", handle.file_region());

  put("vpiConstantSelect", handle.get_bool(vpiConstantSelect));
  put_str("vpiParent", handle.get_handle(vpiParent), vpiFullName);

  put("vpiSize", handle.get_int(vpiSize));
  put("vpiLeftRange", handle.get_handle(vpiLeftRange));
  put("vpiRightRange", handle.get_handle(vpiRightRange));
}

// operation の内容を式として出力する関数
// IEEE 1364-2001 p.655
void
VlDumperImpl::put_operation(const char* label,
			 const VpiHandle& handle)
{
  VlDumpHeader x(*this, label, "Operation");

  put("FileRegion", handle.file_region());

  put("vpiSize", handle.get_int(vpiSize));
  put("vpiOpType", static_cast<tVpiOpType>(handle.get_int(vpiOpType)));
  put("vpiOperand", handle.get_iterate(vpiOperand));
}

// constant の内容を式として出力する関数
// IEEE 1364-2001 p.655
void
VlDumperImpl::put_constant(const char* label,
			const VpiHandle& handle)
{
  VlDumpHeader x(*this, label, "Constant");

  put("FileRegion", handle.file_region());

  put("vpiSize", handle.get_int(vpiSize));
  put("vpiConstType",
      static_cast<tVpiConstType>(handle.get_int(vpiConstType)));
  put("vpiDecompile", handle.get_str(vpiDecompile));
}
#endif

// @brief 遅延値を出力する関数
void
VlDumperImpl::put_delay(const char* label,
			const VlMgr& mgr,
			const VlDelay* delay)
{
  if ( delay == NULL ) {
    if ( !nullptr_suppress_mode() ) {
      VlDumpHeader x(this, label, "null-pointer");
    }
    return;
  }

  VlDumpHeader x(this, label, "Delay");

  put("expr", delay->decompile() );
}

// range の内容を出力する関数
// IEEE 1364-2001 p.644
void
VlDumperImpl::put_range(const char* label,
			const VlMgr& mgr,
			const VlRange* range)
{
  if ( range == NULL ) {
    if ( !nullptr_suppress_mode() ) {
      VlDumpHeader x(this, label, "null-pointer");
    }
    return;
  }

  VlDumpHeader x(this, label, "Range");

  put("FileRegion", range->file_region() );

  put("vpiSize", range->size() );

  put_expr("vpiLeftRange", mgr, range->left_range() );
  put_expr("vpiRightRange", mgr, range->right_range() );
}

END_NAMESPACE_YM_VERILOG
