
/// @file magus/lutmap/NodeSubstCmd.cc
/// @brief NodeSubstCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: NodeSubstCmd.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "LutmapCmd.h"
#include "ym_lutmap/LutNetwork.h"
#include "BoolResub.h"
#include "ym_tclpp/TclPopt.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// ノードの置き換えを行うコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
NodeSubstCmd::NodeSubstCmd(MagMgr* mgr,
			   LutmapData* data) :
  LutmapCmd(mgr, data)
{
  mPoptNpat = new TclPoptInt(this, "npat",
			     "specify pattern vector size");
  
  set_usage_string("");
}

// @brief デストラクタ
NodeSubstCmd::~NodeSubstCmd()
{
}
  
// @brief コマンドを実行する仮想関数
int
NodeSubstCmd::cmd_proc(TclObjVector& objv)
{
  ymuint objc = objv.size();
  if ( objc != 1 ) {
    print_usage();
    return TCL_ERROR;
  }

  try {
    int npat = 1;
    if ( mPoptNpat->is_specified() ) {
      npat = mPoptNpat->val();
    }
    BoolResub resub;
    resub.node_subst(lutnetwork(), npat);
    return TCL_OK;
  }
  catch ( AssertError x ) {
    cerr << x << endl;
    TclObj emsg;
    emsg << "Assertion Error";
    set_result(emsg);
    return TCL_ERROR;
  }
  
  return TCL_OK;
}


//////////////////////////////////////////////////////////////////////
// ノードの併合を行うコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
NodeMergeCmd::NodeMergeCmd(MagMgr* mgr,
			   LutmapData* data) :
  LutmapCmd(mgr, data)
{
  mPoptNpat = new TclPoptInt(this, "npat",
			     "specify pattern vector size");
  
  set_usage_string("<#inputs>[=INT]");
}

// @brief デストラクタ
NodeMergeCmd::~NodeMergeCmd()
{
}
  
// @brief コマンドを実行する仮想関数
int
NodeMergeCmd::cmd_proc(TclObjVector& objv)
{
  ymuint objc = objv.size();
  if ( objc != 2 ) {
    print_usage();
    return TCL_ERROR;
  }
  
#if 0
  try {
#endif
    int limit;
    int code = int_conv(objv[1], limit);
    if ( code != TCL_OK ) {
      return code;
    }
    int npat = 1;
    if ( mPoptNpat->is_specified() ) {
      npat = mPoptNpat->val();
    }
    BoolResub resub;
    resub.node_merge(lutnetwork(), limit, npat);
    return TCL_OK;
#if 0
  }
  catch ( AssertError x ) {
    cerr << x << endl;
    TclObj emsg;
    emsg << "Assertion Error";
    set_result(emsg);
    return TCL_ERROR;
  }
#endif
  
  return TCL_OK;
}

END_NAMESPACE_MAGUS
