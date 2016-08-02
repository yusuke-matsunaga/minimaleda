
/// @file magus/techmap/TechmapCmd.cc
/// @brief TechmapCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TechmapCmd.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "TechmapCmd.h"
#include "LoadPatCmd.h"
#include "Conv2SbjCmd.h"
#include "AreaMapCmd.h"
#include "DumpSbjCmd.h"
#include "DumpCnCmd.h"


BEGIN_NAMESPACE_MAGUS_TECHMAP

//////////////////////////////////////////////////////////////////////
// LUT mapping の基底クラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
TechmapCmd::TechmapCmd(MagMgr* mgr,
		       TechmapData* data) :
  MagCmd(mgr),
  mData(data)
{
}

// @brief デストラクタ
TechmapCmd::~TechmapCmd()
{
}

// @brief PatMgr を得る．
PatMgr&
TechmapCmd::pat_mgr()
{
  return mData->mPatMgr;
}

// @brief SbjGraph を得る．
SbjGraph&
TechmapCmd::sbjgraph()
{
  return mData->mSbjGraph;
}

// @brief セルネットワークを得る．
CnGraph&
TechmapCmd::cngraph()
{
  return mData->mCnGraph;
}

END_NAMESPACE_MAGUS_TECHMAP

BEGIN_NAMESPACE_MAGUS

int
techmap_init(Tcl_Interp* interp,
	     MagMgr* mgr)
{
  using namespace nsTechmap;

  TechmapData* data = new TechmapData;

  TclCmdBinder2<LoadPatCmd, MagMgr*, TechmapData*>::reg(interp, mgr, data,
							"magus::techmap::load_pat");
  TclCmdBinder2<Conv2SbjCmd, MagMgr*, TechmapData*>::reg(interp, mgr, data,
							"magus::techmap::conv2sbj");
  TclCmdBinder2<DumpSbjCmd, MagMgr*, TechmapData*>::reg(interp, mgr, data,
						       "magus::techmap::dump_sbjgraph");
  TclCmdBinder2<AreaMapCmd, MagMgr*, TechmapData*>::reg(interp, mgr, data,
						       "magus::techmap::area_map");
  TclCmdBinder2<DumpCnCmd, MagMgr*, TechmapData*>::reg(interp, mgr, data,
						       "magus::techmap::dump_cngraph");


  const char* init =
    "namespace eval tclreadline {\n"
    "namespace eval magus {\n"
    "namespace eval techmap {\n"
    "proc complete(load_pat) { text start end line pos mod } { return \"\" }\n"
    "proc complete(conv2sbj) { text start end line pos mod } { return \"\" }\n"
    "proc complete(dump_sbjgraph) { text start end line pos mod } { return \"\" }\n"
    "proc complete(area_map) { text start end line pos mod } { return \"\" }\n"
    "proc complete(dump_cngraph) { text start end line pos mod } { return \"\" }\n"
    "}\n"
    "}\n"
    "}\n";

  if ( Tcl_Eval(interp, (char*) init) == TCL_ERROR ) {
    return TCL_ERROR;
  }

  return TCL_OK;
}

END_NAMESPACE_MAGUS
