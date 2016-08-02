
/// @file libym_verilog/elb/VlDumperImpl.cc
/// @brief VlDumperImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlDumperImpl.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include "VlDumperImpl.h"

#include <ym_utils/FileRegion.h>

#include <ym_verilog/VlMgr.h>
#include <ym_verilog/vl/VlModule.h>
#include <ym_verilog/vl/VlUdp.h>


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// VlObj の出力用クラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] s 出力ストリーム
VlDumperImpl::VlDumperImpl(ostream& s) :
  mStream(s),
  mIndent(0),
  mFileLocMode(false),
  mNullptrSuppressMode(true),
  mBitExpandMode(false)
{
}
  
// @brief デストラクタ
VlDumperImpl::~VlDumperImpl()
{
}

// @brief ファイル位置情報を出力するモードにする
void
VlDumperImpl::enable_file_loc_mode()
{
  mFileLocMode = true;
}

// @brief ファイル位置情報の出力を抑制するモードにする．
void
VlDumperImpl::disable_file_loc_mode()
{
  mFileLocMode = false;
}

// @brief ファイル位置情報を出力するモードのとき true を返す．
bool
VlDumperImpl::file_loc_mode() const
{
  return mFileLocMode;
}

// @brief NULL ポインタの出力を抑止する．
void
VlDumperImpl::enable_nullptr_suppress_mode()
{
  mNullptrSuppressMode = true;
}

// @brief NULL ポインタの出力を行う．
void
VlDumperImpl::disable_nullptr_suppress_mode()
{
  mNullptrSuppressMode = false;
}

// @brief NULL ポインタの出力抑止モードの状態を得る．
bool
VlDumperImpl::nullptr_suppress_mode() const
{
  return mNullptrSuppressMode;
}

// @brief bit 展開した内容を出力する．
void
VlDumperImpl::enable_bitexpand_mode()
{
  mBitExpandMode = true;
}

// @brief bit 展開しない．
void
VlDumperImpl::disable_bitexpand_mode()
{
  mBitExpandMode = false;
}

// @brief bit 展開モードの時 true を返す．
bool
VlDumperImpl::bitexpand_mode() const
{
  return mBitExpandMode;
}

// @brief VlMgr の持っている内容を出力する
// @param[in] mgr VlMgr
void
VlDumperImpl::put(const VlMgr& mgr)
{
  // UDP を出力する．
  const list<const VlUdpDefn*>& udp_list = mgr.udp_list();
  for (list<const VlUdpDefn*>::const_iterator p = udp_list.begin();
       p != udp_list.end(); ++ p) {
    put_udp_defn("UDP", mgr, *p);
  }

  // トップモジュールから順にモジュールを出力する．
  list<const VlModule*> tmp_list(mgr.topmodule_list());
  while ( !tmp_list.empty() ) {
    const VlModule* module = tmp_list.front();
    tmp_list.pop_front();
    put_module("MODULE", mgr, module);
    
    vector<const VlModule*> module_list;
    if ( mgr.find_module_list(module, module_list) ) {
      for (vector<const VlModule*>::iterator p = module_list.begin();
	   p != module_list.end(); ++ p) {
	const VlModule* module1 = *p;
	tmp_list.push_back(module1);
      }
    }
    vector<const VlModuleArray*> modulearray_list;
    if ( mgr.find_modulearray_list(module, modulearray_list) ) {
      for (vector<const VlModuleArray*>::iterator p = modulearray_list.begin();
	   p != modulearray_list.end(); ++ p) {
	const VlModuleArray* module_array = *p;
	ymuint32 n = module_array->elem_num();
	for (ymuint32 i = 0; i < n; ++ i) {
	  const VlModule* module1 = module_array->elem_by_offset(i);
	  tmp_list.push_back(module1);
	}
      }
    }
  }
}

#if 0
// @brief VlObj* を出力する関数
// @param[in] label ラベル
// @param[in] obj オブジェクト
void
VlDumperImpl::put(const char* label,
		  const VlMgr& mgr,
		  const VlObj* obj)
{
  if ( obj == NULL ) {
    if ( !nullptr_suppress_mode() ) {
      VlDumpHeader x(*this, label, "null-pointer");
    }
    return;
  }

  switch ( obj->type() ) {
  case kVpiModule:          put_module(label, mgr, obj); break;
  case kVpiModuleArray:     put_module_array(label, mgr, obj); break;
  case kVpiScope:           put_scope(label, mgr, obj); break;
    
  case kVpiGateArray:
  case kVpiSwitchArray:
  case kVpiUdpArray:        put_prim_array(label, mgr, obj); break;
    
  case kVpiNet:             put_decl(label, mgr, obj); break;
  case kVpiNetArray:        put_decl(label, mgr, obj); break;
  case kVpiReg:             put_decl(label, mgr, obj); break;
  case kVpiRegArray:        put_decl(label, mgr, obj); break;
  case kVpiIntegerVar:
  case kVpiRealVar:
  case kVpiTimeVar:
  case kVpiVarSelect:       put_decl(label, mgr, obj); break;
  case kVpiNamedEvent:      put_decl(label, mgr, obj); break;
  case kVpiNamedEventArray: put_decl(label, mgr, obj); break;
  case kVpiParameter:       put_decl(label, mgr, obj); break;
  case kVpiSpecParam:       put_decl(label, mgr, obj); break;
  case kVpiDefParam:        put_def_param(label, mgr, obj); break;
  case kVpiParamAssign:     put_param_assign(label, mgr, obj); break;
    
  case kVpiRange:           put_range(label, mgr, obj); break;
    
  case kVpiGate:
  case kVpiSwitch:
  case kVpiUdp:             put_primitive(label, mgr, obj); break;
  case kVpiPrimTerm:        put_prim_term(label, mgr, obj); break;
    
  case kVpiUdpDefn:         put_udp_defn(label, mgr, obj); break;
    
  case kVpiTask:            put_task(label, mgr, obj); break;
  case kVpiFunction:        put_function(label, mgr, obj); break;
    
#if 0
  case kVpiFrame:           put_frame(label, mgr, obj); break;
#endif
  case kVpiContAssign:      put_cont_assign(label, mgr, obj); break;
#if 0
  case kVpiContAssignBit:   put_cont_assign_bit(label, mgr, obj); break;
#endif
    
  case kVpiInitial:         put_process(label, mgr, obj); break;
  case kVpiAlways:          put_process(label, mgr, obj); break;
    
  case kVpiBegin:           put_stmt(label, mgr, obj); break;
  case kVpiFork:            put_stmt(label, mgr, obj); break;
  case kVpiNamedBegin:      put_stmt(label, mgr, obj); break;
  case kVpiNamedFork:       put_stmt(label, mgr, obj); break;
  case kVpiNullStmt:        put_stmt(label, mgr, obj); break;
  case kVpiEventStmt:       put_stmt(label, mgr, obj); break;
  case kVpiAssignment:      put_stmt(label, mgr, obj); break;
  case kVpiWhile:           put_stmt(label, mgr, obj); break;
  case kVpiRepeat:          put_stmt(label, mgr, obj); break;
  case kVpiWait:            put_stmt(label, mgr, obj); break;
  case kVpiFor:             put_stmt(label, mgr, obj); break;
  case kVpiForever:         put_stmt(label, mgr, obj); break;
  case kVpiIf:              put_stmt(label, mgr, obj); break;
  case kVpiIfElse:          put_stmt(label, mgr, obj); break;
  case kVpiCase:            put_stmt(label, mgr, obj); break;
  case kVpiForce:           put_stmt(label, mgr, obj); break;
  case kVpiAssignStmt:      put_stmt(label, mgr, obj); break;
  case kVpiDeassign:        put_stmt(label, mgr, obj); break;
  case kVpiRelease:         put_stmt(label, mgr, obj); break;
  case kVpiTaskCall:        put_stmt(label, mgr, obj); break;
  case kVpiSysTaskCall:     put_stmt(label, mgr, obj); break;
  case kVpiDisable:         put_stmt(label, mgr, obj); break;
    
#if 0
  case kVpiDelayControl:    put_control(label, mgr, obj); break;
  case kVpiEventControl:    put_control(label, mgr, obj); break;
  case kVpiRepeatControl:   put_control(label, mgr, obj); break;
#endif
    
  case kVpiFuncCall:
  case kVpiSysFuncCall:
  case kVpiBitSelect:
  case kVpiPartSelect:
  case kVpiOperation:
  case kVpiConstant:
    {
      const VlExpr* expr = dynamic_cast<const VlExpr*>(obj);
      assert_cond( expr , __FILE__, __LINE__);
      put_expr(label, mgr, expr);
    }
    break;

  default: put("illegal type", obj->type() );
  }
}
#endif


//////////////////////////////////////////////////////////////////////
// その他のデータ型の出力
//////////////////////////////////////////////////////////////////////

// @brief FileLoc の情報を出力する．
// @param[in] label ラベル
// @param[in] file_loc ファイルの位置情報
void
VlDumperImpl::put(const char* label,
		  const FileLoc& file_loc)
{
  if ( !file_loc_mode() ) {
    return;
  }

  VlDumpHeader x(this, label, "FileLoc");
  if ( file_loc.is_valid() ) {
    put_parent_file(file_loc);

    VlDumpHeader x(this, "mPosition", "FilePos", false);
    mStream << "name = " << file_loc.filename()
	    << ", line " << file_loc.line()
	    << ", column " << file_loc.column();
  }
  else {
    VlDumpHeader x(this, "mPosition", "FilePos", false);
    mStream << "invalid file_loc";
  }
}

// @brief FileRegion の情報をに出力する．
// @param[in] label ラベル
// @param[in] file_region ファイル領域の情報
void
VlDumperImpl::put(const char* label,
		  const FileRegion& file_region)
{
  if ( !file_loc_mode() ) {
    return;
  }

  VlDumpHeader x1(this, label, "FileRegion");

  const FileLoc& first = file_region.start_loc();
  const FileLoc& last = file_region.end_loc();

  if ( first.file_desc() == last.file_desc() ) {
    if ( first.is_valid() ) {
      // 同じファイル
      put_parent_file(first);

      VlDumpHeader x(this, "mRegion", "FilePos", false);
      mStream << "name = " << first.filename();
      if ( first.line() == last.line() ) {
	// 同じ行番号
	mStream << ", line = " << first.line();
	if ( first.column() == last.column() ) {
	  // 同じコラム位置
	  mStream << ", column = " << first.column();
	}
	else {
	  // 異なるコラム
	  mStream << ", column = " << first.column()
		  << " - " << last.column();
	}
      }
      else {
	// 異なる行
	mStream << ", line = " << first.line()
		<< ", column = " << first.column()
		<< " - line = " << last.line()
		<< ", column = " << last.column();
      }
    }
    else {
      VlDumpHeader x(this, "mRegion", "FilePos", false);
      mStream << "---";
    }
  }
  else {
    // 異なるファイル
    put("mFirstLoc", first);
    put("mLastLoc", last);
  }
}

// @brief file_loc の親のファイル情報を出力する．
// @param[in] file_loc ファイル位置の情報
void
VlDumperImpl::put_parent_file(const FileLoc& file_loc)
{
  list<const FileLoc*> file_list;
  file_loc.parent_file_list(file_list);
  for (list<const FileLoc*>::const_iterator p = file_list.begin();
       p != file_list.end(); ++ p) {
    const FileLoc* tmp = *p;
    VlDumpHeader x(this, "mParentFile", "IncFile", false);
    mStream << "name = " << tmp->filename()
	    << ", line " << tmp->line();
  }
}

// @brief direction を出力する関数
// @param[in] label ラベル
// @param[in] direction 向き
void
VlDumperImpl::put(const char* label,
		  tVpiDirection direction)
{
  VlDumpHeader x(this, label, "vpiDirection", false);
  switch ( direction ) {
  case kVpiInput:       mStream << "vpiInput"; break;
  case kVpiOutput:      mStream << "vpiOutput"; break;
  case kVpiInout:       mStream << "vpiInout"; break;
  case kVpiMixedIO:     mStream << "vpiMixedIO"; break;
  case kVpiNoDirection: mStream << "vpiNoDirection"; break;
  default: mStream << "illegal value: " << direction;
  }
}

// @brief const type を出力する関数
// @param[in] label ラベル
// @param[in] const_type 定数の型
void
VlDumperImpl::put(const char* label,
		  tVpiConstType const_type)
{
  VlDumpHeader x(this, label, "vpiConstType", false);
  switch ( const_type ) {
  case kVpiDecConst:    mStream << "vpiDecConst"; break;
  case kVpiRealConst:   mStream << "vpiRealConst"; break;
  case kVpiBinaryConst: mStream << "vpiBinaryConst"; break;
  case kVpiOctConst:    mStream << "vpiOctConst"; break;
  case kVpiHexConst:    mStream << "vpiHexConst"; break;
  case kVpiStringConst: mStream << "vpiStringConst"; break;
  case kVpiIntConst:    mStream << "vpiIntConst"; break;
  default:   mStream << "illegal value: " << const_type;
  }
}

// @brief prim type を出力する関数
// @param[in] label ラベル
// @param[in] prim_type プリミティブの型
void
VlDumperImpl::put(const char* label,
		  tVpiPrimType prim_type)
{
  VlDumpHeader x(this, label, "vpiPrimiType", false);
  switch ( prim_type ) {
  case kVpiAndPrim:      mStream << "vpiAndPrim"; break;
  case kVpiNandPrim:     mStream << "vpiNandPrim"; break;
  case kVpiOrPrim:       mStream << "vpiOrPrim"; break;
  case kVpiNorPrim:      mStream << "vpiNorPrim"; break;
  case kVpiXorPrim:      mStream << "vpiXorPrim"; break;
  case kVpiXnorPrim:     mStream << "vpiXnorPrim"; break;
  case kVpiBufPrim:      mStream << "vpiBufPrim"; break;
  case kVpiNotPrim:      mStream << "vpiNotPrim"; break;
  case kVpiBufif0Prim:   mStream << "vpiBufif0Prim"; break;
  case kVpiBufif1Prim:   mStream << "vpiBufif1Prim"; break;
  case kVpiNotif0Prim:   mStream << "vpiNotif0Prim"; break;
  case kVpiNotif1Prim:   mStream << "vpiNotif1Prim"; break;
  case kVpiNmosPrim:     mStream << "vpiNmosPrim"; break;
  case kVpiPmosPrim:     mStream << "vpiPmosPrim"; break;
  case kVpiCmosPrim:     mStream << "vpiCmosPrim"; break;
  case kVpiRnmosPrim:    mStream << "vpiRnmosPrim"; break;
  case kVpiRpmosPrim:    mStream << "vpiRpmosPrim"; break;
  case kVpiRcmosPrim:    mStream << "vpiRcmosPrim"; break;
  case kVpiRtranPrim:    mStream << "vpiRtranPrim"; break;
  case kVpiRtranif0Prim: mStream << "vpiRtranif0Prim"; break;
  case kVpiRtranif1Prim: mStream << "vpiRtranif1Prim"; break;
  case kVpiTranPrim:     mStream << "vpiTranPrim"; break;
  case kVpiTranif0Prim:  mStream << "vpiTranif0Prim"; break;
  case kVpiTranif1Prim:  mStream << "vpiTranif1Prim"; break;
  case kVpiPullupPrim:   mStream << "vpiPullupPrim"; break;
  case kVpiPulldownPrim: mStream << "vpiPulldownPrim"; break;
  case kVpiSeqPrim:      mStream << "vpiSeqPrim"; break;
  case kVpiCombPrim:     mStream << "vpiCombPrim"; break;
  default: mStream << "illegal value: " << prim_type;
  }
}

// @brief func type の出力
// @param[in] label ラベル
// @param[in] func_type
void
VlDumperImpl::put(const char* label,
		  tVpiFuncType func_type)
{
  VlDumpHeader x(this, label, "vpiFuncType", false);
  switch ( func_type ) {
  case kVpiIntFunc:         mStream << "vpiIntFunc"; break;
  case kVpiRealFunc:        mStream << "vpiRealFunc"; break;
  case kVpiTimeFunc:        mStream << "vpiTimeFunc"; break;
  case kVpiSizedFunc:       mStream << "vpiSizedFunc"; break;
  case kVpiSizedSignedFunc: mStream << "vpiSizedSignedFunc"; break;
  default: mStream << "illegal value: " << func_type;
  }
}

// @brief op type の出力
// @param[in] label ラベル
// @param[in] op_type 演算子の種類
void
VlDumperImpl::put(const char* label,
		  tVpiOpType op_type)
{
  VlDumpHeader x(this, label, "vpiOpType", false);
  switch ( op_type ) {
  case kVpiMinusOp:       mStream << "vpiMinusOp"; break;
  case kVpiPlusOp:        mStream << "vpiPlusOp"; break;
  case kVpiNotOp:         mStream << "vpiNotOp"; break;
  case kVpiBitNegOp:      mStream << "vpiBitNegOp"; break;
  case kVpiUnaryAndOp:    mStream << "vpiUnaryAndOp"; break;
  case kVpiUnaryNandOp:   mStream << "vpiUnaryNandOp"; break;
  case kVpiUnaryOrOp:     mStream << "vpiUnaryOrOp"; break;
  case kVpiUnaryNorOp:    mStream << "vpiUnaryNorOp"; break;
  case kVpiUnaryXorOp:    mStream << "vpiUnaryXorOp"; break;
  case kVpiUnaryXNorOp:   mStream << "vpiUnaryXNorOp"; break;
  case kVpiSubOp:         mStream << "vpiSubOp"; break;
  case kVpiDivOp:         mStream << "vpiDivOp"; break;
  case kVpiModOp:         mStream << "vpiModOp"; break;
  case kVpiEqOp:          mStream << "vpiEqOp"; break;
  case kVpiNeqOp:         mStream << "vpiNeqOp"; break;
  case kVpiCaseEqOp:      mStream << "vpiCaseEqOp"; break;
  case kVpiCaseNeqOp:     mStream << "vpiCaseNeqOp"; break;
  case kVpiGtOp:          mStream << "vpiGtOp"; break;
  case kVpiGeOp:          mStream << "vpiGeOp"; break;
  case kVpiLtOp:          mStream << "vpiLtOp"; break;
  case kVpiLeOp:          mStream << "vpiLeOp"; break;
  case kVpiLShiftOp:      mStream << "vpiLShiftOp"; break;
  case kVpiRShiftOp:      mStream << "vpiRShiftOp"; break;
  case kVpiAddOp:         mStream << "vpiAddOp"; break;
  case kVpiMultOp:        mStream << "vpiMultOp"; break;
  case kVpiLogAndOp:      mStream << "vpiLogAndOp"; break;
  case kVpiLogOrOp:       mStream << "vpiLogOrOp"; break;
  case kVpiBitAndOp:      mStream << "vpiBitAndOp"; break;
  case kVpiBitOrOp:       mStream << "vpiBitOrOp"; break;
  case kVpiBitXorOp:      mStream << "vpiBitXorOp"; break;
  case kVpiBitXNorOp:     mStream << "vpiBitXNorOp"; break;
  case kVpiConditionOp:   mStream << "vpiConditionOp"; break;
  case kVpiConcatOp:      mStream << "vpiConcatOp"; break;
  case kVpiMultiConcatOp: mStream << "vpiMultiConcatOp"; break;
  case kVpiEventOrOp:     mStream << "vpiEventOrOp"; break;
  case kVpiNullOp:        mStream << "vpiNullOp"; break;
  case kVpiListOp:        mStream << "vpiListOp"; break;
  case kVpiMinTypMaxOp:   mStream << "vpiMinTypMaxOp"; break;
  case kVpiPosedgeOp:     mStream << "vpiPosedgeOp"; break;
  case kVpiNegedgeOp:     mStream << "vpiNegedgeOp"; break;
  case kVpiArithLShiftOp: mStream << "vpiArithLShiftOp"; break;
  case kVpiArithRShiftOp: mStream << "vpiArithRShiftOp"; break;
  case kVpiPowerOp:       mStream << "vpiPowerOp"; break;
  default: mStream << "illegal value: " << op_type;
  }
}

// @brief strength を出力する関数
// @param[in] label ラベル
// @param[in] str 信号強度
// @param[in] put_on_null true の時, 空でも出力する．
void
VlDumperImpl::put(const char* label,
		  tVpiStrength str,
		  bool put_on_null)
{
  if ( str == kVpiNoStrength && !put_on_null ) {
    return;
  }
  VlDumpHeader x(this, label, "vpiStrength", false);
  switch ( str ) {
  case kVpiNoStrength:   mStream << "none"; break;
  case kVpiSupplyDrive:  mStream << "supply"; break;
  case kVpiStrongDrive:  mStream << "strong"; break;
  case kVpiPullDrive:    mStream << "pull"; break;
  case kVpiWeakDrive:    mStream << "weak"; break;
  case kVpiLargeCharge:  mStream << "large"; break;
  case kVpiMediumCharge: mStream << "medium"; break;
  case kVpiSmallCharge:  mStream << "small"; break;
  case kVpiHiZ:          mStream << "high-Z"; break;
  default: mStream << "illegal strength"; break;
  }
}

// @brief unconnected drive の出力
// @param[in] label ラベル
// @param[in] ud unconnected drive
void
VlDumperImpl::put(const char* label,
		  tVpiUnconnDrive ud)
{
  VlDumpHeader x(this, label, "unconn_drive", false);
  switch ( ud ) {
  case kVpiHighZ: mStream << "high-Z"; break;
  case kVpiPull1: mStream << "pull-1"; break;
  case kVpiPull0: mStream << "pull-0"; break;
  }
}

// @brief default delay mode の出力
// @param[in] label ラベル
// @param[in] delay_mode default delay mode
void
VlDumperImpl::put(const char* label,
		  tVpiDefDelayMode delay_mode)
{
  VlDumpHeader x(this, label, "def_delay_mode", false);
  switch ( delay_mode ) {
  case kVpiDelayModeNone:    mStream << "none"; break;
  case kVpiDelayModePath:    mStream << "path"; break;
  case kVpiDelayModeDistrib: mStream << "distrib"; break;
  case kVpiDelayModeUnit:    mStream << "unit"; break;
  case kVpiDelayModeZero:    mStream << "zero"; break;
  case kVpiDelayModeMTM:     mStream << "min-typ-max"; break;
  }
}

// @brief ネット型の出力
// @param[in] label ラベル
// @param[in] net_type ネット型
void
VlDumperImpl::put(const char* label,
		  tVpiNetType net_type)
{
  VlDumpHeader x(this, label, "net_type", false);
  switch ( net_type ) {
  case kVpiSupply0: mStream << "supply0"; break;
  case kVpiSupply1: mStream << "supply1"; break;
  case kVpiTri:     mStream << "tri"; break;
  case kVpiTriAnd:  mStream << "triand"; break;
  case kVpiTriOr:   mStream << "trior"; break;
  case kVpiTriReg:  mStream << "trireg"; break;
  case kVpiTri0:    mStream << "tri0"; break;
  case kVpiTri1:    mStream << "tri1"; break;
  case kVpiWire:    mStream << "wire"; break;
  case kVpiWand:    mStream << "wand"; break;
  case kVpiWor:     mStream << "wor"; break;
  case kVpiNone:    mStream << "none"; break;
  }
}

// @brief ブール値の出力
// @param[in] label ラベル
// @param[in] b ブール値
void
VlDumperImpl::put(const char* label,
		  bool b)
{
  VlDumpHeader x(this, label, "bool", false);
  if ( b ) {
    mStream << "true";
  }
  else {
    mStream << "false";
  }
}

// @brief 整数値の出力
// @param[in] label ラベル
// @param[in] d 整数値
void
VlDumperImpl::put(const char* label,
		  int d)
{
  VlDumpHeader x(this, label, "int", false);
  mStream << d;
}

// @brief 整数値の出力
// @param[in] label ラベル
// @param[in] d 整数値
void
VlDumperImpl::put(const char* label,
		  ymuint32 d)
{
  VlDumpHeader x(this, label, "int", false);
  mStream << d;
}

// @brief 文字列の出力
// @param[in] label ラベル
// @param[in] str 文字列
void
VlDumperImpl::put(const char* label,
		  const string& str)
{
  VlDumpHeader x(this, label, "string", false);
  mStream << str;
}

// @brief 文字列の出力
// @param[in] label ラベル
// @param[in] str 文字列
void
VlDumperImpl::put(const char* label,
		  const char* str)
{
  VlDumpHeader x(this, label, "string", false);
  if ( str ) {
    mStream << str;
  }
  else {
    mStream << "<null-ptr>";
  }
}

// @brief 開始マーカを出力する．
// @param[in] label ラベル
// @param[in] type 属性
// @param[in] need_cr マーカ出力直後に改行するときに true とするフラグ
void
VlDumperImpl::begin_marker(const char* label,
			   const string& type,
			   bool need_cr)
{
  for (int i = 0; i < mIndent; ++ i) {
    mStream << "  ";
  }
  mStream << "<" << label << " type = \"" << type << "\">";
  if ( need_cr ) {
    mStream << endl;
  }
  mDoCR.push_back(need_cr);
  ++ mIndent;
}

// @brief 終了マーカを出力する．
// @param[in] label ラベル
void
VlDumperImpl::end_marker(const char* label)
{
  -- mIndent;
  if ( mDoCR.back() ) {
    for (int i = 0; i < mIndent; ++ i) {
      mStream << "  ";
    }
  }
  mDoCR.pop_back();
  mStream << "</" << label << ">" << endl;
}


//////////////////////////////////////////////////////////////////////
// マーカを出力するクラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] dumper 出力ストリーム
// @param[in] label ラベル
// @param[in] type 属性
// @param[in] need_cr true の時マーカ出力直後に改行する
VlDumpHeader::VlDumpHeader(VlDumperImpl* dumper,
			   const char* label,
			   const char* type,
			   bool need_cr) :
  mDumper(dumper),
  mName(label)
{
  mDumper->begin_marker(mName, type, need_cr);
}

// @brief デストラクタ
VlDumpHeader::~VlDumpHeader()
{
  mDumper->end_marker(mName);
}

END_NAMESPACE_YM_VERILOG
