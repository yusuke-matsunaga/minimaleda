#ifndef YM_SAT_SAT_NSDEF_H
#define YM_SAT_SAT_NSDEF_H

/// @file ym_sat/sat_nsdef.h
/// @brief SAT Solver 用の名前空間の定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: sat_nsdef.h 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

/// @defgroup SatGroup SAT ソルバ
///
/// 中身はフルスクラッチで API もオリジナルだが，実装のアイデアの多くは
/// MiniSat-1.14 を参考にしている．ちなみに MiniSat そのものを ymsat の
/// API で使うこともできる．


#include "ymtools.h"


/// @brief ym_sat 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_SAT \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsSat)

/// @brief ym_sat 用の名前空間の終了
#define END_NAMESPACE_YM_SAT \
END_NAMESPACE(nsSat) \
END_NAMESPACE_YM

/// @namespace nsYm::nsSat
/// @brief SAT ソルバ関係のクラスが属する名前空間
///
/// ただし，主要なものは nsYm にインポートされている．
/// 実装用のローカルな要素だけがこの名前空間に存在する．

BEGIN_NAMESPACE_YM_SAT

class SatSolver;
class SatSolverFactory;
class SatStats;
class SatMsgHandler;

END_NAMESPACE_YM_SAT

BEGIN_NAMESPACE_YM

using nsSat::SatSolver;
using nsSat::SatSolverFactory;
using nsSat::SatStats;
using nsSat::SatMsgHandler;

END_NAMESPACE_YM

#endif // YM_SAT_SAT_NSDEF_H
