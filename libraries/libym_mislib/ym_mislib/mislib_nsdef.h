#ifndef YM_MISLIB_MISLIB_NSDEF_H
#define YM_MISLIB_MISLIB_NSDEF_H

/// @file ym_cell/cell_nsdef.h
/// @brief libym_cell 用の名前空間の定義
///
/// @author Yusuke Matsunaga
///
/// $Id: mislib_nsdef.h 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

/// @defgroup CellGroup セルライブラリ
///
/// セルライブラリを扱うためのクラスライブラリ

#include "ymtools.h"


//////////////////////////////////////////////////////////////////////
// 名前空間を定義するためのマクロ
//////////////////////////////////////////////////////////////////////

/// @brief ym_cell の名前空間の開始
#define BEGIN_NAMESPACE_YM_MISLIB \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsMislib)

/// @brief ym_cell の名前空間の終了
#define END_NAMESPACE_YM_MISLIB \
END_NAMESPACE(nsMislib) \
END_NAMESPACE_YM

/// @namespace nsYm::nsMislib
/// @brief mislib ファイル用パーサークラスが属する名前空間
///
/// ただし，主要なものは nsYm にインポートされている．
/// 実装用のローカルな要素だけがこの名前空間に存在する．


BEGIN_NAMESPACE_YM_MISLIB

//////////////////////////////////////////////////////////////////////
// クラスの前方参照
//////////////////////////////////////////////////////////////////////

class MislibParser;
class MislibParserImpl;
class MislibPin;
class MislibHandler;

END_NAMESPACE_YM_MISLIB

BEGIN_NAMESPACE_YM

using nsMislib::MislibParser;
using nsMislib::MislibPin;
using nsMislib::MislibHandler;

END_NAMESPACE_YM

#endif // YM_MISLIB_MISLIB_NSDEF_H
