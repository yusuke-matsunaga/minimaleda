#ifndef YM_BNET_BNET_NSDEF_H
#define YM_BNET_BNET_NSDEF_H

/// @file ym_bnet/bnet_nsdef.h
/// @brief ym_bnet の共通ヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: bnet_nsdef.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

/// @defgroup BnetGroup Boolean Network パッケージ
///
/// Boolean Network を扱うためのクラス群
/// UCB の mis/sis と大きく異なるのはノードを論理関数を常に論理式
/// で表しているということ


#include "ymtools.h"
#include "ym_utils/DlList.h"


/// @brief ymbnet 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_BNET \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsBnet)

/// @brief ymbnet 用の名前空間の終了
#define END_NAMESPACE_YM_BNET \
END_NAMESPACE(nsBnet) \
END_NAMESPACE_YM

/// @namespace nsYm::nsBnet
/// @brief Boolean Network 関係のクラスが属する名前空間
///
/// ただし，主要なものは nsYm にインポートされている．
/// 実装用のローカルな要素だけがこの名前空間に存在する．


// エラー処理が必要な箇所を示すための人畜無害なマーカー
#if 0
#define BNET_ERROR(msg)
#else
#define BNET_ERROR(msg) cerr << msg << endl
#endif


BEGIN_NAMESPACE_YM_BNET

class BNetwork;
class BNetManip;
class BNetDecomp;
class BNetGateDecomp;
class BNetSopDecomp;

class BNode;
class BNodeEdge;

class BNetChg;
class BNetworkTrace;

class BNodeList;

class BNetBlifWriter;
class BNetEquWriter;
class BNetVerilogWriter;

class BNet2Sbj;


/// @brief 枝のリスト
/// @ingroup BnetGroup
typedef list<BNodeEdge*> BNodeEdgeList;

/// @brief ファンアウトリスト
/// @ingroup BnetGroup
typedef DlList<BNodeEdge> BNodeFoList;

/// @brief 節点のベクタ
/// @ingroup BnetGroup
typedef vector<BNode*> BNodeVector;

END_NAMESPACE_YM_BNET

BEGIN_NAMESPACE_YM

using nsBnet::BNetwork;
using nsBnet::BNetManip;
using nsBnet::BNetDecomp;
using nsBnet::BNetGateDecomp;
using nsBnet::BNetSopDecomp;

using nsBnet::BNode;
using nsBnet::BNodeVector;
using nsBnet::BNodeList;

using nsBnet::BNodeEdge;
using nsBnet::BNodeEdgeList;

using nsBnet::BNodeFoList;

using nsBnet::BNetChg;
using nsBnet::BNetworkTrace;

using nsBnet::BNetBlifWriter;
using nsBnet::BNetEquWriter;
using nsBnet::BNetVerilogWriter;

using nsBnet::BNet2Sbj;

using nsBnet::BNodeEdgeList;
using nsBnet::BNodeFoList;
using nsBnet::BNodeVector;

END_NAMESPACE_YM

#endif // YM_BNET_BNET_NSDEF_H
