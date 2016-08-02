#ifndef YM_TCLPP_TCLPP_NSDEF_H
#define YM_TCLPP_TCLPP_NSDEF_H

/// @file ym_tclpp/tclpp_nsdef.h
/// @brief Tclpp サブモジュール関係のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: tclpp_nsdef.h 1417 2008-05-06 18:12:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

/// @defgroup Tclpp Tcl++ サブモジュール
///
/// Tcl8.X から Tcl 内部のデータを抽象化したオブジェクトとして
/// Tcl_Obj が導入されたが, 所詮, C ベースの API なので安全に
/// 参照回数を管理することもままならない．
///
/// そこで Tcl_Obj に C++ のラッパをかぶせたクラスがこれ

#include <tcl.h>
#include "ymtools.h"

// tcl8.4 で定義されている const マクロ
#if !defined(CONST84)
#define CONST84
#endif


//////////////////////////////////////////////////////////////////////
// 名前空間の定義
//////////////////////////////////////////////////////////////////////

/// @brief ym_tclpp 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_TCLPP \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsTclpp)

/// @brief ym_tclpp 用の名前空間の終了
#define END_NAMESPACE_YM_TCLPP \
END_NAMESPACE(nsTclpp) \
END_NAMESPACE_YM

/// @namespace nsYm::nsTclpp
/// @brief Tclpp 関係のクラスが属する名前空間
///
/// ただし，主要なものは nsYm にインポートされている．
/// 実装用のローカルな要素だけがこの名前空間に存在する．


BEGIN_NAMESPACE_YM_TCLPP

//////////////////////////////////////////////////////////////////////
// 前方参照のための宣言
//////////////////////////////////////////////////////////////////////

class TclBase;
class TclObj;
typedef vector<TclObj> TclObjVector;
typedef list<TclObj> TclObjList;
class TclCmd;
class TclCmdCls;
template<typename cmd>
class TclCmdBinder;
template<typename cmd,
	 typename T1>
class TclCmdBinder1;
template<typename cmd,
	 typename T1,
	 typename T2>
class TclCmdBinder2;
template<typename cmd,
	 typename T1,
	 typename T2,
	 typename T3>
class TclCmdBinder3;
template<typename cmd,
	 typename T1,
	 typename T2,
	 typename T3,
	 typename T4>
class TclCmdBinder4;
template<typename cmd,
	 typename T1,
	 typename T2,
	 typename T3,
	 typename T4,
	 typename T5>
class TclCmdBinder5;
template<typename cmd,
	 typename T1,
	 typename T2,
	 typename T3,
	 typename T4,
	 typename T5,
	 typename T6>
class TclCmdBinder6;
class TclPopt;
class TclPoptObj;
class TclPoptStr;
class TclPoptBool;
class TclPoptInt;
class TclPoptUint;
class TclPoptLong;
class TclPoptUlong;
class TclPoptDouble;
class TclPoptGroup;
class TclVarTrace;
class TclHandler;
class TclFileHandler;
class TclIdleHandler;
class TclTimerHandler;
class TclPeriodicTimerHandler;

END_NAMESPACE_YM_TCLPP


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// nsYm の名前空間にインポートしておく
//////////////////////////////////////////////////////////////////////

using nsTclpp::TclBase;
using nsTclpp::TclObj;
using nsTclpp::TclObjVector;
using nsTclpp::TclObjList;
using nsTclpp::TclCmd;
using nsTclpp::TclCmdCls;
using nsTclpp::TclCmdBinder;
using nsTclpp::TclCmdBinder1;
using nsTclpp::TclCmdBinder2;
using nsTclpp::TclCmdBinder3;
using nsTclpp::TclCmdBinder4;
using nsTclpp::TclCmdBinder5;
using nsTclpp::TclCmdBinder6;
using nsTclpp::TclPopt;
using nsTclpp::TclPoptObj;
using nsTclpp::TclPoptStr;
using nsTclpp::TclPoptBool;
using nsTclpp::TclPoptInt;
using nsTclpp::TclPoptUint;
using nsTclpp::TclPoptLong;
using nsTclpp::TclPoptUlong;
using nsTclpp::TclPoptDouble;
using nsTclpp::TclPoptGroup;
using nsTclpp::TclVarTrace;
using nsTclpp::TclHandler;
using nsTclpp::TclFileHandler;
using nsTclpp::TclIdleHandler;
using nsTclpp::TclTimerHandler;
using nsTclpp::TclPeriodicTimerHandler;

END_NAMESPACE_YM

#endif // YMTCLPP_TCLPP_NSDEF_H
