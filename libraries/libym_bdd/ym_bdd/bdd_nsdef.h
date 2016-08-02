#ifndef YM_BDD_BDD_NSDEF_H
#define YM_BDD_BDD_NSDEF_H

/// @file ym_bdd/bdd_nsdef.h
/// @brief BDD に共通な定義ファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: bdd_nsdef.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

/// @defgroup Bdd BDD パッケージ
///
///

#include "ymtools.h"


//////////////////////////////////////////////////////////////////////
// 名前空間を定義するためのマクロ
//////////////////////////////////////////////////////////////////////

/// @brief BDD 用の名前空間の開始
/// @ingroup Bdd
#define BEGIN_NAMESPACE_YM_BDD \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsBdd)

/// @brief BDD 用の名前空間の終了
/// @ingroup Bdd
#define END_NAMESPACE_YM_BDD \
END_NAMESPACE(nsBdd) \
END_NAMESPACE_YM

/// @namespace nsYm::nsBdd
/// @brief BDD 関係のクラスが属する名前空間
///
/// ただし，主要なものは nsYm にインポートされている．
/// 実装用のローカルな要素だけがこの名前空間に存在する．


BEGIN_NAMESPACE_YM_BDD


//////////////////////////////////////////////////////////////////////
// クラス名の前方参照用の宣言
//////////////////////////////////////////////////////////////////////

class Bdd;
class BddMgrRef;
class BddMgr;
class BddMgrFactory;
class BddVarSet;
class BddLitSet;


//////////////////////////////////////////////////////////////////////
/// @name 型名の定義
/// @{

/// @brief BDD の節点を指す枝のタイプ
/// @ingroup Bdd
typedef ympuint tBddEdge;

/// @brief BDD の変数番号
/// @ingroup Bdd
typedef ymuint32 tVarId;

/// @brief BDD の変数の数
/// @ingroup Bdd
typedef ymuint32 tVarSize;

/// @brief BDD の節点のレベル
/// @ingroup Bdd
typedef ymuint32 tLevel;

/// @brief BDD の節点のレベルの数
/// @ingroup Bdd
typedef ymuint32 tLevelSize;

/// @brief BDD のベクタ
/// @ingroup Bdd
typedef vector<Bdd> BddVector;

/// @brief BDD のリスト
/// @ingroup Bdd
typedef list<Bdd> BddList;

/// @brief 変数番号をキートして BDD を値とする連想配列
/// @ingroup Bdd
typedef hash_map<tVarId, Bdd> VarBddMap;

/// @}
//////////////////////////////////////////////////////////////////////

END_NAMESPACE_YM_BDD

BEGIN_NAMESPACE_YM

using nsBdd::Bdd;
using nsBdd::BddMgrRef;
using nsBdd::BddMgr;
using nsBdd::BddMgrFactory;
using nsBdd::BddVarSet;
using nsBdd::BddLitSet;
using nsBdd::BddVector;
using nsBdd::BddList;
using nsBdd::VarBddMap;
using nsBdd::tBddEdge;

END_NAMESPACE_YM

#endif // YM_BDD_BDD_NSDEF_H
