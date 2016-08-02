#ifndef ATPG_DTPGCL_NSDEF_H
#define ATPG_DTPGCL_NSDEF_H

/// @file include/dtpgcl_nsdef.h
/// @brief DtpgCl に共通な定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: dtpgcl_nsdef.h 2128 2009-03-12 07:25:33Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "atpg_nsdef.h"
#include "ym_utils/StopWatch.h"


// DtpgCl 用の名前空間の定義
#define BEGIN_NAMESPACE_YM_ATPG_DTPGCL \
BEGIN_NAMESPACE_YM_ATPG \
BEGIN_NAMESPACE(nsDtpgCl)

#define END_NAMESPACE_YM_ATPG_DTPGCL \
END_NAMESPACE(nsDtpgCl) \
END_NAMESPACE_YM_ATPG


BEGIN_NAMESPACE_YM_ATPG_DTPGCL

//////////////////////////////////////////////////////////////////////
/// @brief Dynamic Implication モード
//////////////////////////////////////////////////////////////////////
enum tDynMode {
  /// @brief なし
  kDynNone,
  /// @brief 一回のみ
  kDynOnce,
  /// @brief 毎回
  kDynAlways
};


extern
int verbose_level;

extern
FILE* ver_fp;

extern
MStopWatch sw_timer;

/// @brief verbose level を得る．
inline
int
get_verbose_level()
{
  return verbose_level;
}

/// @brief メッセージを出力して死ぬ．
inline
void
FATAL(const char* msg)
{
  cout << msg << endl;
  assert_not_reached(__FILE__, __LINE__);
}


class DtpgCl;

END_NAMESPACE_YM_ATPG_DTPGCL

BEGIN_NAMESPACE_YM_ATPG

using nsDtpgCl::DtpgCl;

END_NAMESPACE_YM_ATPG

#endif
