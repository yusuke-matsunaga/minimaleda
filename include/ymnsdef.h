#ifndef YMNSDEF_H
#define YMNSDEF_H

/// @file ymnsdef.h
/// @brief 名前空間の定義に関するヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: ymnsdef.h 1669 2008-08-04 13:06:17Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


//////////////////////////////////////////////////////////////////////
// 名前空間の定義
//////////////////////////////////////////////////////////////////////

/// @brief 名前空間の開始
///
/// @param[in] nm 開始する名前空間名
///
/// namespace でインデントするのがいやなので define マクロでごまかす．
#define BEGIN_NAMESPACE(nm) \
namespace nm {

/// @brief 名前空間の終了
///
/// @param[in] nm 終了する名前空間名
///
/// 実は nm は使っていない．プログラマの確認用
#define END_NAMESPACE(nm) \
}

/// @brief 無名の名前空間の開始
#define BEGIN_NONAMESPACE \
namespace {

/// @brief 無名の名前空間の終了
#define END_NONAMESPACE \
}

/// @brief 標準(std)の名前空間の開始
#define BEGIN_NAMESPACE_STD \
BEGIN_NAMESPACE(std)

/// @brief 標準(std)の名前空間の終了
#define END_NAMESPACE_STD \
END_NAMESPACE(std)

/// @brief hash 用の名前空間の開始
#define BEGIN_NAMESPACE_HASH \
BEGIN_NAMESPACE(HASH_NAMESPACE)

/// @brief hash 用の名前空間の終了
#define END_NAMESPACE_HASH \
END_NAMESPACE(HASH_NAMESPACE)

/// @brief ymtools 用の名前空間の開始
#define BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsYm)

/// @brief ymtools 用の名前空間の終了
#define END_NAMESPACE_YM \
END_NAMESPACE(nsYm)

/// @namespace nsYm
/// @brief ymtools の全ての要素が属する名前空間

/// @defgroup YmUtils ユーティリティモジュール


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// std:: にある汎用的な名前を nsYym の名前空間にインポートする
//////////////////////////////////////////////////////////////////////

using std::istream;
using std::ifstream;
using std::istringstream;
using std::ostream;
using std::ofstream;
using std::ostringstream;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::ios;
using std::setw;
using std::setprecision;
using std::setfill;
using std::hex;
using std::oct;
using std::dec;
using std::fixed;
using std::scientific;
using std::exception;
using std::bad_alloc;
using std::pair;
using std::make_pair;
using std::string;
using std::vector;
using std::list;
using HASH_NAMESPACE::hash;
using HASH_NAMESPACE::hash_set;
using HASH_NAMESPACE::hash_map;

END_NAMESPACE_YM

#endif // YMNSDEF_H
