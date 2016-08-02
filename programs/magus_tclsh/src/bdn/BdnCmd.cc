
/// @file magus/logbase/MagBdnCmd.cc
/// @brief MagBdnCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MagBdnCmd.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MagBdnCmd.h"
#include "ym_tclpp/TclPopt.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// Magus のコマンドオブジェクトの基底クラス MagBdnCmd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MagBdnCmd::MagBdnCmd(MagMgr* mgr) :
  MagCmd(mgr),
  mNetworkSpecified(false)
{
  mPoptNtwk = new TclPoptObj(this, "network",
			     "specify target network",
			     "<network-name>");
  mPoptNewNtwk = new TclPoptObj(this, "new_network",
				"specify target new network",
				"<network-name>");
  new_popt_group(mPoptNtwk, mPoptNewNtwk);
}

// @brief デストラクタ
MagBdnCmd::~MagBdnCmd()
{
}

// @brief カレントネットワークの取得
// @note カレントネットワークが BNetwork でない場合には NULL を返す．
BdNetwork*
MagBdnCmd::cur_network() const
{
  return cur_nethandle()->_bdn();
}

// コマンド行の引数を解析しネットワークとライブラリをセットする．
// エラーが起きた場合にはインタプリタにメッセージをセットし，
// false を返す．
// ネットワークおよびライブラリが指定されていた場合には，
// それぞれ mNetworkSpecified, mLibrarySpecified に true を入れる．
int
MagBdnCmd::before_cmd_proc(TclObjVector& objv)
{
  mNetworkSpecified = false;

  MagCmd::before_cmd_proc(objv);

  // ネットワークを指定したときに true とするフラグ
  bool ntwk_flag = false;

  // 新規作成の時 true とするフラグ
  bool new_flag = false;

  // ネットワーク名
  TclObj name;
  if ( mPoptNtwk->is_specified() ) {
    name = mPoptNtwk->val();
    ntwk_flag = true;
  }
  else if ( mPoptNewNtwk->is_specified() ) {
    new_flag = true;
    name = mPoptNewNtwk->val();
    ntwk_flag = true;
  }

  if ( ntwk_flag ) {
    // name をネットワーク名と見なしてスタックに入れる．
    // 上書きを考慮して関数を直接呼ばずに Tcl スクリプト
    // を評価させる．
    TclObj script;
    if ( script.append_element("::magus::push_current_network") != TCL_OK ) {
      return TCL_ERROR;
    }
    if ( new_flag ) {
      if ( script.append_element("-new_bdn") != TCL_OK ) {
	return TCL_ERROR;
      }
    }
    // name は {--new <name>}の場合があるので単一の要素とは限らない．
    if ( script.append_list(name) != TCL_OK ) {
      return TCL_ERROR;
    }
    if ( eval(script) != TCL_OK ) {
      // (たぶん)ネットワークが存在しなかった．
      return TCL_ERROR;
    }
    mNetworkSpecified = true;
  }

  return TCL_OK;
}

void
MagBdnCmd::after_cmd_proc()
{
  if ( mNetworkSpecified ) {
    // カレントネットワークをもとに戻す．
    // 上書きを考慮して関数を直接呼ばずに Tcl スクリプト
    // を評価させる．
    int stat = eval("::magus::pop_current_network");
    assert_cond( stat == TCL_OK, __FILE__, __LINE__);
    mNetworkSpecified = false;
  }
}

END_NAMESPACE_MAGUS
