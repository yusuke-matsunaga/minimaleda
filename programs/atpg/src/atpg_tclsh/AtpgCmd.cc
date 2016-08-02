
/// @file src/AtpgCmd.cc
/// @brief AtpgCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: AtpgCmd.cc 2128 2009-03-12 07:25:33Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "AtpgCmd.h"
#include "AtpgMgr.h"
#include "ym_tgnet/TgNetwork.h"
#include "ym_tgnet/TgNode.h"
#include "SaFault.h"


BEGIN_NAMESPACE_YM_ATPG

//////////////////////////////////////////////////////////////////////
// AtpgCmd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
AtpgCmd::AtpgCmd(AtpgMgr* mgr) :
  mMgr(mgr)
{
}

// @brief デストラクタ
AtpgCmd::~AtpgCmd()
{
}

// @brief ネットワークをセットした後に呼ぶ関数
void
AtpgCmd::after_set_network()
{
  // 諸元を TCL 変数にセットしておく
  TgNetwork& network = _network();
  size_t nlo = network.logic_num();
  size_t n_buff = 0;
  size_t n_not = 0;
  size_t n_and = 0;
  size_t n_and2 = 0;
  size_t n_and3 = 0;
  size_t n_and4 = 0;
  size_t n_nand = 0;
  size_t n_nand2 = 0;
  size_t n_nand3 = 0;
  size_t n_nand4 = 0;
  size_t n_or = 0;
  size_t n_or2 = 0;
  size_t n_or3 = 0;
  size_t n_or4 = 0;
  size_t n_nor = 0;
  size_t n_nor2 = 0;
  size_t n_nor3 = 0;
  size_t n_nor4 = 0;
  size_t n_xor = 0;
  size_t n_xor2 = 0;
  size_t n_xnor = 0;
  size_t n_xnor2 = 0;
  size_t n_cplx = 0;
  for (size_t i = 0; i < nlo; ++ i) {
    const TgNode* node = network.logic(i);
    switch ( node->type() ) {
    case kTgUndef:
    case kTgInput:
    case kTgOutput:
      break;
      
    case kTgBuff:
      ++ n_buff;
      break;
      
    case kTgNot:
      ++ n_not;
      break;
      
    case kTgAnd:
      ++ n_and;
      switch ( node->ni() ) {
      case 2: ++ n_and2; break;
      case 3: ++ n_and3; break;
      case 4: ++ n_and4; break;
      }
      break;
      
    case kTgNand:
      ++ n_nand;
      switch ( node->ni() ) {
      case 2: ++ n_nand2; break;
      case 3: ++ n_nand3; break;
      case 4: ++ n_nand4; break;
      }
      break;
      
    case kTgOr:
      ++ n_or;
      switch ( node->ni() ) {
      case 2: ++ n_or2; break;
      case 3: ++ n_or3; break;
      case 4: ++ n_or4; break;
      }
      break;
      
    case kTgNor:
      ++ n_nor;
      switch ( node->ni() ) {
      case 2: ++ n_nor2; break;
      case 3: ++ n_nor3; break;
      case 4: ++ n_nor4; break;
      }
      break;
      
    case kTgXor:
      ++ n_xor;
      switch ( node->ni() ) {
      case 2: ++ n_xor2; break;
      }
      break;
      
    case kTgXnor:
      ++ n_xnor;
      switch ( node->ni() ) {
      case 2: ++ n_xnor2; break;
      }
      break;

    default:
      ++ n_cplx;
      break;
    }
  }

  TclObj varname = "::atpg::info";
  int varflag = 0;
  set_var(varname, "input_num", network.input_num1(), varflag);
  set_var(varname, "output_num", network.output_num1(), varflag);
  set_var(varname, "ff_num", network.ff_num(), varflag);
  set_var(varname, "logic_num", network.logic_num(), varflag);
  set_var(varname, "buff_num", n_buff, varflag);
  set_var(varname, "not_num", n_not, varflag);
  set_var(varname, "and_num", n_and, varflag);
  set_var(varname, "and2_num", n_and2, varflag);
  set_var(varname, "and3_num", n_and3, varflag);
  set_var(varname, "and4_num", n_and4, varflag);
  set_var(varname, "nand_num", n_nand, varflag);
  set_var(varname, "nand2_num", n_nand2, varflag);
  set_var(varname, "nand3_num", n_nand3, varflag);
  set_var(varname, "nand4_num", n_nand4, varflag);
  set_var(varname, "or_num", n_or, varflag);
  set_var(varname, "or2_num", n_or2, varflag);
  set_var(varname, "or3_num", n_or3, varflag);
  set_var(varname, "or4_num", n_or4, varflag);
  set_var(varname, "nor_num", n_nor, varflag);
  set_var(varname, "nor2_num", n_nor2, varflag);
  set_var(varname, "nor3_num", n_nor3, varflag);
  set_var(varname, "nor4_num", n_nor4, varflag);
  set_var(varname, "xor_num", n_xor, varflag);
  set_var(varname, "xor2_num", n_xor2, varflag);
  set_var(varname, "xnor_num", n_xnor, varflag);
  set_var(varname, "xnor2_num", n_xnor2, varflag);
  set_var(varname, "cplx_num", n_cplx, varflag);
}

BEGIN_NONAMESPACE

// 故障を表す TclObj を作る
TclObj
f2obj(SaFault* f)
{
  TclObjVector tmp(3);
  tmp[0] = f->node()->name();
  if ( f->is_output_fault() ) {
    tmp[1] = "O";
  }
  else {
    ostringstream buf;
    buf << "I" << f->pos();
    tmp[1] = buf.str();
  }
  if ( f->val() ) {
    tmp[2] = "SA1";
  }
  else {
    tmp[2] = "SA0";
  }
  // ここでTclObjVector から TclObj へ変換を行っている．
  return tmp;
}

END_NONAMESPACE

// @brief 故障リストを更新した後に呼ぶ関数
void
AtpgCmd::after_update_faults()
{
  FaultMgr& fault_mgr = _fault_mgr();
  
  // 諸元を TCL 変数にセットしておく
  const vector<SaFault*>& all_list = fault_mgr.all_list();
  const vector<SaFault*>& rep_list = fault_mgr.all_rep_list();
  const vector<SaFault*>& remain_list = fault_mgr.remain_list();
  const vector<SaFault*>& untest_list = fault_mgr.untest_list();
  size_t n_all = all_list.size();
  size_t n_rep = rep_list.size();
  size_t n_remain = remain_list.size();
  size_t n_untest = untest_list.size();
  size_t n_det = n_rep - n_remain - n_untest;

  TclObj varname = "::atpg::info";
  int varflag = 0;
  set_var(varname, "all_fault_num", n_all, varflag);
  set_var(varname, "rep_fault_num", n_rep, varflag);
  set_var(varname, "det_fault_num", n_det, varflag);
  set_var(varname, "remain_fault_num", n_remain, varflag);
  set_var(varname, "untestt_fault_num", n_untest, varflag);
#if 0
  {
    TclObjVector tmp_list;
    for (vector<SaFault*>::const_iterator p = all_list.begin();
	 p != all_list.end(); ++ p) {
      SaFault* f = *p;
      tmp_list.push_back(f2obj(f));
    }
    set_var(varname, "all_fault_list", tmp_list, varflag);
  }
  {
    TclObjVector tmp_list;
    for (vector<SaFault*>::const_iterator p = rep_list.begin();
	 p != rep_list.end(); ++ p) {
      SaFault* f = *p;
      tmp_list.push_back(f2obj(f));
    }
    set_var(varname, "rep_fault_list", tmp_list, varflag);
  }
  {
    TclObjVector tmp_list;
    for (vector<SaFault*>::const_iterator p = remain_list.begin();
	 p != remain_list.end(); ++ p) {
      SaFault* f = *p;
      tmp_list.push_back(f2obj(f));
    }
    set_var(varname, "remain_fault_list", tmp_list, varflag);
  }
  {
    TclObjVector tmp_list;
    for (vector<SaFault*>::const_iterator p = untest_list.begin();
	 p != untest_list.end(); ++ p) {
      SaFault* f = *p;
      tmp_list.push_back(f2obj(f));
    }
    set_var(varname, "untest_fault_list", tmp_list, varflag);
  }
#endif
}
  
// @brief TgNetwork を取り出す．
TgNetwork&
AtpgCmd::_network()
{
  return mgr()._network();
}

// @brief FaultMgr を取り出す．
FaultMgr&
AtpgCmd::_fault_mgr()
{
  return mgr()._fault_mgr();
}

// @brief TvMgr を取り出す．
TvMgr&
AtpgCmd::_tv_mgr()
{
  return mgr()._tv_mgr();
}

END_NAMESPACE_YM_ATPG
