#ifndef ATPG_DTPG_OPT_H
#define ATPG_DTPG_OPT_H

/// @file include/dtpg_opt.h
/// @brief DtpgCl のオプション用の名前空間の定義
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: dtpg_opt.h 2128 2009-03-12 07:25:33Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "atpg_nsdef.h"

#define BEGIN_NAMESPACE_YM_ATPG_DTPGOPT	\
BEGIN_NAMESPACE_YM_ATPG \
BEGIN_NAMESPACE(nsDtpgOpt)

#define END_NAMESPACE_YM_ATPG_DTPGOPT \
END_NAMESPACE(nsDtpgOpt) \
END_NAMESPACE_YM_ATPG


BEGIN_NAMESPACE_YM_ATPG_DTPGOPT

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

END_NAMESPACE_YM_ATPG_DTPGOPT

#endif // ATPG_DTPG_OPT_H
