#ifndef YM_ISCAS89_ISCAS89_NSDEF_H
#define YM_ISCAS89_ISCAS89_NSDEF_H

/// @file ym_iscas89/iscas89_nsdef.h
/// @brief nsIscas89 の名前空間の定義
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: iscas89_nsdef.h 2043 2009-02-21 07:08:48Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

/// @defgroup Iscas89Group ISCAS89フォーマットパーサ
///
/// ISCAS89 ベンチマーク回路フォーマット用パーサ


#include "ymtools.h"


/// @brief ym_iscas89 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_ISCAS89 \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsIscas89)

/// @brief ym_iscas89 用の名前空間の終了
#define END_NAMESPACE_YM_ISCAS89 \
END_NAMESPACE(nsIscas89) \
END_NAMESPACE_YM

/// @namespace nsYm::nsIscas89
/// @brief Iscas89Parser 関係のクラスが属する名前空間
///
/// ただし，主要なものは nsYm にインポートされている．
/// 実装用のローカルな要素だけがこの名前空間に存在する．


BEGIN_NAMESPACE_YM_ISCAS89

// クラスの前方宣言
class Iscas89Handler;
class Iscas89Parser;
class Iscas89ParserImpl;

END_NAMESPACE_YM_ISCAS89

BEGIN_NAMESPACE_YM

using nsIscas89::Iscas89Handler;
using nsIscas89::Iscas89Parser;

END_NAMESPACE_YM

#endif // YM_ISCAS89_ISCAS89_NSDEF_H
