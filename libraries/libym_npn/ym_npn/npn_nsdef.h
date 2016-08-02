#ifndef YM_NPN_NPN_NSDEF_H
#define YM_NPN_NPN_NSDEF_H

/// @file ym_npn/npn_nsdef.h
/// @brief ym_npn パッケージに共通な定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: npn_nsdef.h 1508 2008-06-30 04:55:42Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

/// @defgroup NpnGroup NPN 同値類判定パッケージ
///
/// NPN 同値類の判定を行うためのクラス


#include "ymtools.h"


//////////////////////////////////////////////////////////////////////
// 名前空間を定義するためのマクロ
//////////////////////////////////////////////////////////////////////

/// @brief NPN 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_NPN \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsNpn)

/// @brief NPN 用の名前空間の終了
#define END_NAMESPACE_YM_NPN \
END_NAMESPACE(nsNpn) \
END_NAMESPACE_YM

/// @namespace nsYm::nsNpn
/// @brief NPN同値類判定パッケージ関係のクラスが属する名前空間
///
/// ただし，主要なものは nsYm にインポートされている．
/// 実装用のローカルな要素だけがこの名前空間に存在する．


BEGIN_NAMESPACE_YM_NPN

//////////////////////////////////////////////////////////////////////
// クラスの前方参照
//////////////////////////////////////////////////////////////////////

class TvFunc;
class NpnMap;
class NpnMgr;


//////////////////////////////////////////////////////////////////////
// 定数
//////////////////////////////////////////////////////////////////////

/// @brief 扱う最大の入力数
/// 特に根拠はないが，これなら Walsh 係数が 32 ビット整数で収まる．
/// あと真理値表ベースの手法ではこれくらいが限度
static
const ymuint kNpnMaxNi = 20;

END_NAMESPACE_YM_NPN

BEGIN_NAMESPACE_YM

using nsNpn::TvFunc;
using nsNpn::NpnMap;
using nsNpn::NpnMgr;
using nsNpn::kNpnMaxNi;

END_NAMESPACE_YM

#endif // YM_NPN_NPN_NSDEF_H
