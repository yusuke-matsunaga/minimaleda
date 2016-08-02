#ifndef YM_MINCOV_MINCOV_NSDEF_H
#define YM_MINCOV_MINCOV_NSDEF_H

/// @file ym_mincov/mincov_nsdef.h
/// @brief Mincov Solver 用の名前空間の定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: mincov_nsdef.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

/// @defgroup MincovGroup Mincov Solver
///
/// 最小被覆問題を解くためのライブラリ


#include "ymtools.h"


/// @brief ym_mincov 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_MINCOV \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsMincov)

/// @brief ym_mincov 用の名前空間の終了
#define END_NAMESPACE_YM_MINCOV \
END_NAMESPACE(nsMincov) \
END_NAMESPACE_YM

/// @namespace nsYm::nsMincov
/// @brief MINCOV ソルバ関係のクラスが属する名前空間
///
/// ただし，主要なものは nsYm にインポートされている．
/// 実装用のローカルな要素だけがこの名前空間に存在する．

BEGIN_NAMESPACE_YM_MINCOV

class MincovSolver;
class MincovSolverFactory;
class MincovMatrix;
class MincovMsgHandler;

END_NAMESPACE_YM_MINCOV

BEGIN_NAMESPACE_YM

using nsMincov::MincovSolver;
using nsMincov::MincovSolverFactory;
using nsMincov::MincovMatrix;
using nsMincov::MincovMsgHandler;

END_NAMESPACE_YM

#endif // YM_MINCOV_MINCOV_NSDEF_H
