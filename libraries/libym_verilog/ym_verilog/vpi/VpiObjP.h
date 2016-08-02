#ifndef YM_VERILOG_VPI_VPIOBJP_H
#define YM_VERILOG_VPI_VPIOBJP_H

/// @file ym_verilog/vpi/VpiObjP.h
/// @brief VpiObj の派生クラスの先行宣言
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VpiObjP.h 159 2006-08-20 17:32:49Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#include <ym_verilog/verilog.h>


BEGIN_NAMESPACE_YM_VERILOG

// in VpiFactory.h
class VpiFactory;

// in VpiHandle.h
class VpiHandle;

// in VpiObj.h
class VpiObj;

// in VpiBase.h
class VpiBase;

// in VpiNamedObj.h
class VpiNamedObj;

// in VpiIteratorObj.h
class VpiIteratorObj;

// in VpiScope.h
class VpiScope;

// in VpiModule.h
class VpiModuleArray;
class VpiModule;
class VpiPorts;
class VpiPort;
class VpiPortBit;

// in VpiModPath.h
class VpiModPath;
class VpiPathTerm;
class VpiInterModPath;

// in VpiUdp.h
class VpiUdpDefn;
class VpiTableEntry;
class VpiUdpInitial;
class VpiUdpAssign;

// in VpiDecl.h
class VpiIODecl;
class VpiNetArray;
class VpiNets;
class VpiNet;
class VpiNetBit;
class VpiRegArray;
class VpiRegs;
class VpiReg;
class VpiRegBit;
class VpiVariables;
class VpiVarArray;
class VpiVariable;
class VpiVarSelect;
class VpiNeArray;
class VpiNamedEvent;
class VpiParameter;
class VpiSpecParam;
class VpiDefParam;
class VpiParamAssign;

// in VpiItem.h
class VpiPrimArray;
class VpiPrimitive;
class VpiPrimTerm;
class VpiTchk;
class VpiTchkTerm;
class VpiTaskFunc;
class VpiFunction;
class VpiTask;
class VpiDelayDevice;
class VpiDelayTerm;
class VpiCaCommon;
class VpiContAssign;
class VpiContAssignBit;
class VpiProcess;

// in VpiStmt.h
class VpiStmt;
class VpiCaseItem;

// in VpiExpr
class VpiExpr;

// in VpiControl
class VpiDelayControl;
class VpiEventControl;
class VpiRepeatControl;

// in VpiFrame
class VpiFrame;

// in VpiRange
class VpiRange;

// in VpiUserSystf
class VpiUserSystf;

// in VpiAttribute
class VpiAttribute;

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_VPI_VPIOBJP_H
