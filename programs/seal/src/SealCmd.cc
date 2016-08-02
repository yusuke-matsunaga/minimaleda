
/// @file src/SealCmd.cc
/// @brief SealCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SealCmd.cc 1920 2008-12-20 15:52:42Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include "seal_config.h"
#endif

#include "SealCmd.h"
#include "SealMgr.h"
#include <ym_tgnet/TgNetwork.h>
#include <ym_tgnet/TgNode.h>


BEGIN_NAMESPACE_YM_SEAL

//////////////////////////////////////////////////////////////////////
// SealCmd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SealCmd::SealCmd(SealMgr* mgr) :
  mMgr(mgr)
{
}

// @brief デストラクタ
SealCmd::~SealCmd()
{
}

// @brief ネットワークをセットした後に呼ぶ関数
void
SealCmd::after_set_network()
{
  TgNetwork& network = _network();
  size_t ni = network.input_num2();

  // 諸元を TCL 変数にセットしておく
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

  TclObj varname = "::seal::info";
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

// @brief TgNetwork を取り出す．
TgNetwork&
SealCmd::_network()
{
  return mgr()._network();
}

END_NAMESPACE_YM_SEAL
