
/// @file OptTestCmd.cc
/// @brief OptTestCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: OptTestCmd.cc 1417 2008-05-06 18:12:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "OptTestCmd.h"
#include "ym_tclpp/TclPopt.h"
#include <sstream>


BEGIN_NAMESPACE_YM_TCLPP

//////////////////////////////////////////////////////////////////////
// オプション解析のテスト用コマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
OptTestCmd::OptTestCmd()
{
  mOptSwitch1 = new TclPopt(this, "switch",
			    "specify option switch");
  mOptStr1 = new TclPoptStr(this, "str",
			    "specify option string");
  mOptBool1 = new TclPoptBool(this, "bool",
			      "specify option switch");
  mOptInt1 = new TclPoptInt(this, "int",
			    "specify option number");
  mOptDouble1 = new TclPoptDouble(this, "double",
				  "specify option floating number");

  mOptA = new TclPopt(this, "A",
		      "option-A");
  mOptB = new TclPopt(this, "B",
		      "option-B");
  mOptC = new TclPopt(this, "C",
		      "option-C");
  new_popt_group(mOptA, mOptB, mOptC);
}

// @brief デストラクタ
OptTestCmd::~OptTestCmd()
{
}

// 乱数初期化コマンド
int
OptTestCmd::cmd_proc(TclObjVector& objv)
{
  ostringstream buf;
  buf << "Options:" << endl;
  if ( mOptSwitch1->is_specified() ) {
    buf << mOptSwitch1->opt_str() << " is specified" << endl;
  }
  if ( mOptStr1->is_specified() ) {
    buf << mOptStr1->opt_str() << ": "
	<< mOptStr1->val() << endl;
  }
  if ( mOptBool1->is_specified() ) {
    buf << mOptBool1->opt_str() << ": "
	<< mOptBool1->val() << endl;
  }
  if ( mOptInt1->is_specified() ) {
    buf << mOptInt1->opt_str() << ": "
	<< mOptInt1->val() << endl;
  }
  if ( mOptDouble1->is_specified() ) {
    buf << mOptDouble1->opt_str() << ": "
	<< mOptDouble1->val() << endl;
  }
  if ( mOptA->is_specified() ) {
    buf << mOptA->opt_str() << " is specified" << endl;
  }
  if ( mOptB->is_specified() ) {
    buf << mOptB->opt_str() << " is specified" << endl;
  }
  if ( mOptC->is_specified() ) {
    buf << mOptC->opt_str() << " is specified" << endl;
  }

  buf << endl
      << "Arguments:" << endl;
  size_t objc = objv.size();
  buf << "objc: " << objc << endl;
  for (size_t i = 0; i < objc; ++ i) {
    string obj_str = objv[i];
    buf << "  objv[" << i << "]: " << obj_str << endl;
  }
  TclObj result(buf.str());
  set_result(result);

  return TCL_OK;
}

END_NAMESPACE_YM_TCLPP
