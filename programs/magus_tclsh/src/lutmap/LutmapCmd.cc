
/// @file magus/lutmap/LutmapCmd.cc
/// @brief LutmapCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: LutmapCmd.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "LutmapCmd.h"
#include "AreaMapCmd.h"
#include "DelayMapCmd.h"
#include "ym_lutmap/LnGraph.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// LUT mapping の基底クラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LutmapCmd::LutmapCmd(MagMgr* mgr,
		     LutmapData* data) :
  MagCmd(mgr),
  mData(data)
{
}

// @brief デストラクタ
LutmapCmd::~LutmapCmd()
{
}

// @brief SbjGraph を得る．
SbjGraph&
LutmapCmd::sbjgraph()
{
  return mData->mSbjGraph;
}

// @brief LutNetwork を得る．
LnGraph&
LutmapCmd::lutnetwork()
{
  return mData->mLutNetwork;
}


int
lutmap_init(Tcl_Interp* interp,
	    MagMgr* mgr)
{
  LutmapData* data = new LutmapData;

  TclCmdBinder2<Conv2SbjCmd, MagMgr*, LutmapData*>::reg(interp, mgr, data,
							"magus::lutmap::conv2sbj");

  TclCmdBinder2<DumpSbjCmd, MagMgr*, LutmapData*>::reg(interp, mgr, data,
						       "magus::lutmap::dump_sbjgraph");
  TclCmdBinder2<AreaMapCmd, MagMgr*, LutmapData*>::reg(interp, mgr, data,
						       "magus::lutmap::area_map");
  TclCmdBinder2<DelayMapCmd, MagMgr*, LutmapData*>::reg(interp, mgr, data,
							"magus::lutmap::delay_map");
  TclCmdBinder2<DumpLutCmd, MagMgr*, LutmapData*>::reg(interp, mgr, data,
						       "magus::lutmap::dump_lutnetwork");
  TclCmdBinder2<Conv2BNetCmd, MagMgr*, LutmapData*>::reg(interp, mgr, data,
							 "magus::lutmap::conv2bnet");
  TclCmdBinder2<WriteVqmCmd, MagMgr*, LutmapData*>::reg(interp, mgr, data,
							"magus::lutmap::write_vqm");


  const char* init =
    "namespace eval tclreadline {\n"
    "namespace eval magus {\n"
    "namespace eval lutmap {\n"
    "proc complete(conv2sbj) { text start end line pos mod } { return \"\" }\n"
    "proc complete(dump_sbjgraph) { text start end line pos mod } { return \"\" }\n"
    "proc complete(area_map) { text start end line pos mod } { return \"\" }\n"
    "proc complete(delay_map) { text start end line pos mod } { return \"\" }\n"
    "proc complete(dump_lutnetwork) { text start end line pos mod } { return \"\" }\n"
    "proc complete(conv2bnet) { text start end line pos mod } { return \"\" }\n"
    "proc complete(write_vqn) { text start end line pos mod } { return \"\" }\n"
    "}\n"
    "}\n"
    "}\n";

  if ( Tcl_Eval(interp, (char*) init) == TCL_ERROR ) {
    return TCL_ERROR;
  }

  return TCL_OK;
}

END_NAMESPACE_MAGUS
