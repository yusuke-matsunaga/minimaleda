
/// @file src/dtpg_classic/DtpgCl.cc
/// @brief DtpgCl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id$
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgCl.h"
#include "Network.h"
#include "GateSa.h"
#include "SaFault.h"
#include "TestVector.h"
#include "SearchMgr.h"
#include "Cfna.h"


BEGIN_NAMESPACE_YM_ATPG_DTPGCL

int verbose_level = 0;
FILE* ver_fp = stderr;


// @brief コンストラクタ
DtpgCl::DtpgCl()
{
}

// @brief デストラクタ
DtpgCl::~DtpgCl()
{
}
 
// @brief ネットワーク全体をアクティブにする．
void
DtpgCl::activate_all()
{
  gn.activate_all();
}
  
// @brief f に関わる部分のみをアクティブにする．
// @param[in] f 対象の故障
// @return アクティブにできる範囲があれば true を返す．
bool
DtpgCl::activate_1fault(SaFault* f)
{
  bool stat = gn.activate_tfitfo(gn.gate(f->source_node()));
  return stat;
}

// @brief ネットワーク全体をアクティブでなくす．
void
DtpgCl::deactivate()
{
  gn.deactivate();
}
  
// @brief f がアクティブな範囲に入っていれば true を返す．
bool
DtpgCl::check_fault(SaFault* f)
{
  Gate* gate = gn.gate(f->node());
  return gate->chk_scope();
}

// @brief 故障 f に対するテストパタン生成を行う．
// @param[in] f 対象の故障
// @param[in] flist その他の未検出故障のリスト
// @param[in] dyn_mode dynamic implication のモード
// @param[out] tv 生成したパタンを入れるベクタ
// @param[out] tv で検出できる故障のリスト
// @retval true パタン生成が成功した．
// @retval false f が検出不能故障かパタン生成の途中でアボートした．
tStat
DtpgCl::run(SaFault* f,
	    const vector<SaFault*>& flist,
	    tDynMode dyn_mode,
	    TestVector* tv,
	    vector<SaFault*>& det_faults)
{
  return sm.run(f, flist, dyn_mode, tv, det_faults);
}

// @brief 現在のバックトラック制限数を得る．
ymuint
DtpgCl::backtrack_limit() const
{
  return sm.backtrack_limit();
}

// @brief バックトラック制限数を設定する．
// @param[in] limit 設定する制限数
void
DtpgCl::set_backtrack_limit(ymuint limit)
{
  sm.set_backtrack_limit(limit);
}

// @brief デフォルトのバックトラック制限数を設定する．
void
DtpgCl::set_default_backtrack_limit()
{
  sm.set_default_backtrack_limit();
}

// @brief Unique Sensitization モードを得る．
int
DtpgCl::us_mode() const
{
  return 0;
}

// @brief Unique Sensitization モードを設定する．
// @param[in] us_mode 設定する Unique Sensitization モードの値
void
DtpgCl::set_us_mode(ymuint us_mode)
{
}
  
// @brief バックトラック数のカウンタをクリアする．
void
DtpgCl::clear_backtrack_count()
{
  sm.clear_backtrack_count();
}

// @brief バックトラック数を得る．
ymuint
DtpgCl::total_backtrack_count() const
{
  return sm.backtrack_count();
}
  
// @brief verbose level を設定する．
void
DtpgCl::set_verbose_level(ymuint vlevel)
{
  nsDtpgCl::verbose_level = vlevel;
}

// @brief ネットワークをセットした後に呼ばれる関数
void
DtpgCl::after_set_network(const TgNetwork& network,
			  TvMgr& tvmgr)
{
  gn.set(network);
  
  // CFNA の計算
  gn.activate_all();
  cfna.init(gn, tvmgr);
  gn.deactivate();
}

// @brief 故障リストの内容が変わった後に呼ばれる関数
// @note デフォルトの実装ではなにもしない．
void
DtpgCl::after_update_faults(const vector<SaFault*>& flist)
{
}

END_NAMESPACE_YM_ATPG_DTPGCL
