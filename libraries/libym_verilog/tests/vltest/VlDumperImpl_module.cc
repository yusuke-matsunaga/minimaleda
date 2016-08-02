
/// @file libym_verilog/elb/VlDumperImpl_module.cc
/// @brief VlDumperImpl の実装ファイル (module担当)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlDumperImpl_module.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include "VlDumperImpl.h"

#include <ym_verilog/VlMgr.h>
#include <ym_verilog/vl/VlModule.h>
#include <ym_verilog/vl/VlPort.h>


BEGIN_NAMESPACE_YM_VERILOG

BEGIN_NONAMESPACE

// time unit を出力する関数
string
unit2str(int scale)
{
  string ans;
  scale += 15;
  int a = scale % 3;
  int b = scale / 3;
  switch ( a ) {
  case 2: ans = "100"; break;
  case 1: ans = "10"; break;
  case 0: ans = "1"; break;
  }
  switch ( b ) {
  case 5: ans += "s"; break;
  case 4: ans += "ms"; break;
  case 3: ans += "us"; break;
  case 2: ans += "ns"; break;
  case 1: ans += "ps"; break;
  case 0: ans += "fs"; break;
  }
  return ans;
}

END_NONAMESPACE


// module の内容を出力する関数
// IEEE1364-2001 p.634
void
VlDumperImpl::put_module(const char* label,
			 const VlMgr& mgr,
			 const VlModule* module)
{
  VlDumpHeader x(this, label, "Module");

  put("mFileRegion", module->file_region() );

  put("vpiDefName", module->def_name() );
  put("vpiFullName", module->full_name() );
  put("vpiDefFile",  module->def_file_region() );

  bool array = (module->module_array() != NULL);
  put("vpiArray", array);
  if ( array ) {
    put("vpiModuleArray", module->module_array()->full_name() );
    put("vpiIndex", module->index() );
  }
  put("vpiTopModule", module->is_top_module() );
  put("vpiCellInstance", module->is_cell_instance() );
  put("vpiProtected", module->is_protected() );
  
  if ( module->time_unit() != -16 ) {
    put("vpiTimeUnit", unit2str(module->time_unit()) );
    put("vpiTimePrecision", unit2str(module->time_precision()) );
  }
  
  put("vpiDefNetType", module->def_net_type() );
  put("vpiUnconnDrive", module->unconn_drive() );
  put("vpiDefDelayMode", module->def_delay_mode() );
  put("vpiDefDecayTime", module->def_decay_time() );
  put("vpiConfig", module->config() );
  put("vpiLibrary", module->library() );
  put("vpiCell", module->cell() );

#if 0 // VERIFAULT
  {
    VlDumpHeader x20(s, "portfaults");
    if ( handle.portfaults() ) {
      s << "enabled";
    }
    else {
      s << "disabled";
    }
  }
  {
    VlDumpHeader x21(s, "suppress_faults");
    if ( handle.suppress_faults() ) {
      s << "true";
    }
    else {
      s << "false";
    }
  }
#endif
  
  for (ymuint32 i = 0; i < module->port_num(); ++ i) {
    put_port("vpiPort", mgr, module->port(i));
  }
  for (ymuint32 i = 0; i < module->io_num(); ++ i) {
    put_iodecl("vpiIODecl", mgr, module->io(i));
  }
  
  put_scope_sub(mgr, module);
  
  vector<const VlProcess*> process_list;
  if ( mgr.find_process_list(module, process_list) ) {
    put_process_list("vpiProcess", mgr, process_list);
  }
}

// module array の内容を出力する関数
// IEEE 1364-2001 p.635
void
VlDumperImpl::put_modulearray(const char* label,
			      const VlMgr& mgr,
			      const VlModuleArray* module_array)
{
  VlDumpHeader x(this, label, "ModuleArray");
  
  put("FileRegion", module_array->file_region() );
  put("vpiFullName", module_array->full_name() );
  put("vpiSize", module_array->elem_num() );
  put_expr("vpiLeftRange", mgr, module_array->left_range() );
  put_expr("vpiRightRange", mgr, module_array->right_range() );
  
#if 0
  vector<VlObj*> obj_list;
  if ( mgr.find_obj(module_array, vpiParamAssign, obj_list) ) {
    put("vpiParamAssign", mgr, obj_list);
  }
#endif
  
  // 中身は出力しない．
}
  
// @brief module array のリストの内容を出力する関数
void
VlDumperImpl::put_modulearray_list(const char* label,
				   const VlMgr& mgr,
				   const vector<const VlModuleArray*>& ma_list)
{
  VlDumpHeader x(this, label, "ModuleArrayList");

  for (vector<const VlModuleArray*>::const_iterator p = ma_list.begin();
       p != ma_list.end(); ++ p) {
    put_modulearray(label, mgr, *p);
  }
}

// port の内容を出力する関数
// IEEE 1364-2001 p.637
void
VlDumperImpl::put_port(const char* label,
		       const VlMgr& mgr,
		       const VlPort* port)
{
  VlDumpHeader x(this, label, "Port");

  put("FileRegion", port->file_region() );

  put("vpiModule", port->module()->full_name() );
  put("vpiConnByName", port->is_conn_by_name() );
  put("vpiDirection", port->direction() );
  put("vpiExplicitName", port->is_explicit_name() );
  put("vpiPortIndex", port->port_index() );
  if ( port->name() != NULL ) {
    put("vpiName", port->name() );
  }
#if 0
  put("vpiScalar", port->is_scalar() );
  put("vpiVector", port->is_vector() );
#endif
  put("vpiSize", port->bit_size() );
  
  put_expr("vpiHighConn", mgr, port->high_conn() );
  put_expr("vpiLowConn",  mgr, port->low_conn() );
  
#if 0
  if ( mBitExpandMode ) {
    put("vpiBit", handle.get_iterate(vpiBit));
  }
#else
#warning "TODO: Port のビット要素"
#endif
}

#if 0
// port bit の内容を出力する関数
// IEEE 1364-2001 p.637
void
VlDumperImpl::put_port_bit(const char* label,
		       const VlMgr& mgr,
		       cosnt VlObj* obj)
{
  VlDumpHeader x(*this, label, "PortBit");

  put("FileRegion", handle.file_region());

  put("vpiConnByName", handle.get_bool(vpiConnByName));
  put("vpiDirection",
      static_cast<tVpiDirection>(handle.get_int(vpiDirection)));
  put("vpiExplicitName", handle.get_bool(vpiExplicitName));
  put("vpiScalar", handle.get_bool(vpiScalar));
  put("vpiVector", handle.get_bool(vpiVector));
  put("vpiSize", handle.get_int(vpiSize));

  put("vpiHighConn", handle.get_handle(vpiHighConn), vpiDecompile);
  put"vpiLowConn", handle.get_handle(vpiLowConn), vpiDecompile);
}
#endif

END_NAMESPACE_YM_VERILOG
