#ifndef YM_VERILOG_VL_VLFWD_H
#define YM_VERILOG_VL_VLFWD_H

/// @file ym_verilog/vl/VlFwd.h
/// @brief VlObj の派生クラスの前方宣言ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlFwd.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/verilog.h"


BEGIN_NAMESPACE_YM_VERILOG

// in VlObj.h
class VlObj;

// in VlNamedObj.h
class VlNamedObj;

// in VlDecl.h
class VlDecl;

// in VlModule.h
class VlModule;
class VlModuleArray;

// in VlPort.h
class VlPort;

// in VlIODecl.h
class VlIODecl;

// in VlParamAssign.h
class VlParamAssign;
class VlDefParam;

// in VlContAssign.h
class VlContAssign;

// in VlTaskFunc.h
class VlTaskFunc;

// in VlPrimitive.h
class VlPrimitive;
class VlPrimArray;
class VlPrimTerm;

// in VlProcess.h
class VlProcess;

// in VlStmt.h
class VlStmt;
class VlCaseItem;

// in VlControl.h
class VlControl;

// in VlExpr.h
class VlExpr;

// in VlDelay.h
class VlDelay;

// in VlRange.h
class VlRange;

// in VlUdp.h
class VlUdpDefn;
class VlTableEntry;

// in VlUserSystf.h
class VlUserSystf;

// in VlAttribute.h
class VlAttribute;
class VlAttrList;

END_NAMESPACE_YM_VERILOG

BEGIN_NAMESPACE_YM

using nsVerilog::VlObj;
using nsVerilog::VlNamedObj;
using nsVerilog::VlDecl;
using nsVerilog::VlModule;
using nsVerilog::VlModuleArray;
using nsVerilog::VlPort;
using nsVerilog::VlIODecl;
using nsVerilog::VlParamAssign;
using nsVerilog::VlDefParam;
using nsVerilog::VlContAssign;
using nsVerilog::VlTaskFunc;
using nsVerilog::VlPrimitive;
using nsVerilog::VlPrimArray;
using nsVerilog::VlPrimTerm;
using nsVerilog::VlProcess;
using nsVerilog::VlStmt;
using nsVerilog::VlCaseItem;
using nsVerilog::VlControl;
using nsVerilog::VlExpr;
using nsVerilog::VlDelay;
using nsVerilog::VlRange;
using nsVerilog::VlUdpDefn;
using nsVerilog::VlTableEntry;
using nsVerilog::VlUserSystf;
using nsVerilog::VlAttribute;
using nsVerilog::VlAttrList;

END_NAMESPACE_YM

#endif // YM_VERILOG_VL_VLFWD_H
