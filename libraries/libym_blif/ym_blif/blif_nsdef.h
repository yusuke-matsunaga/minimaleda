#ifndef YM_BLIF_BLIF_NSDEF_H
#define YM_BLIF_BLIF_NSDEF_H

/// @file ym_blif/blif_nsdef.h
/// @brief nsBlif の名前空間の定義
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: blif_nsdef.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

/// @defgroup BlifGroup BLIF パーサ
///
/// UCB で開発された mis, sis の標準入出力フォーマット
/// BLIF(Berkeley Logic Interchange Format) 用のパーサ


#include "ymtools.h"


/// @brief ym_bli 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_BLIF \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsBlif)

/// @brief ym_blif 用の名前空間の終了
#define END_NAMESPACE_YM_BLIF \
END_NAMESPACE(nsBlif) \
END_NAMESPACE_YM

/// @namespace nsYm::nsBlif
/// @brief BlifParser 関係のクラスが属する名前空間
///
/// ただし，主要なものは nsYm にインポートされている．
/// 実装用のローカルな要素だけがこの名前空間に存在する．

BEGIN_NAMESPACE_YM_BLIF

// クラスの前方宣言
class BlifHandler;
class BlifParser;
class BlifParserImpl;
class BlifNetwork;
class BlifNode;
class BlifNetworkReader;

END_NAMESPACE_YM_BLIF

BEGIN_NAMESPACE_YM

using nsBlif::BlifHandler;
using nsBlif::BlifParser;
using nsBlif::BlifNetwork;
using nsBlif::BlifNode;
using nsBlif::BlifNetworkReader;

END_NAMESPACE_YM

#endif // YM_BLIF_BLIF_NSDEF_H
