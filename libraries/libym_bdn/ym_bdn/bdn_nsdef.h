#ifndef YM_BDN_BDN_NSDEF_H
#define YM_BDN_BDN_NSDEF_H

/// @file ym_bdn/bdn_nsdef.h
/// @brief ym_bdn の共通ヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: bdn_nsdef.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

/// @defgroup BdnGroup Binary Decomposed Network パッケージ
///
/// ネットワーク全体として以下の情報(データ)を持つ．
///
/// - 外部入力ノード
///   各々が外部入力IDを持つ．外部入力IDは0から始まる連続した整数．
///   入力ノードは一度生成されたら削除されない．
///
/// - 外部出力ノード
///   各々が外部出力IDを持つ．外部出力IDは0から始まる連続した整数．
///   出力ノードは一度生成されたら削除されない．
///   ファンイン(or NULL)と反転属性を持つ．
///
/// - ラッチノード
///   ラッチに相当するノード．
///   リセット値(0, 1, 2)を持つ．2 の場合にはリセット値は不定
///
/// - 論理ノード
///   2つのファンイン(常に != NULL)とローカルな論理関数
///   (真理値表の形で4ビット)を持つ．ただし，真理値表ベクタ
///   の最初のビットは常に0 (ということは省略可)．
///   ファンアウトの枝のリストも持つ．
///   論理ノードも明示的には削除されないが，ファンアウトを持たないノード
///   が削除されることはある．
///
/// すべてのノードはノードIDを持つ．ノードIDは 0 から始まる整数だが，ノードが
/// 削除された場合には歯抜けの状態になる．
///
/// 基本的には論理回路の構造を表すブーリアンネットワークだが，
/// 論理ノードは2入力のみという制限を設けている．
///
/// そのためいくつか境界条件として特別な扱いが必要となる．
/// - 定数関数の表現
///   明かに内部の論理ノードが定数関数を表している場合には sweep
///   してしまえばよいので，考慮すべきは外部出力が定数の場合のみ
///   となる．
/// - インバーターの表現
///   内部の論理ノードがファンインのノードの否定の論理を利用している
///   時には自分のローカルな論理関数の該当の入力の極性を反転すれば良い．
///   また，外部出力が論理ノードの否定となっている場合も，その論理ノード
///   のローカルな論理関数を反転すればよい．ただし，その論理ノードが複数
///   の外部出力にファンアウトしており，両方の極性で参照されている場合
///   には対応できない．
/// - structural hashing
///   同じファンインを持つ AND と NAND のノードがあった場合，どちらかを
///   反転すれば共有できるが，正規化のルールがなければ一意には定まらない．
///
/// ということで以下のような制約を設ける．
/// - 論理ノードの論理関数
///   2つの入力がともに 0 のときの出力が 0 になるようにする．
/// - 外部出力ノードの属性
///   ファンインの論理ノード
///   反転属性 (bool: true で反転)
/// - 定数出力の表し方
///   ファンインを NULL
///   反転属性なしで 0，反転属性ありで 1 を表す．


#include "ymtools.h"
#include "ym_utils/DlList.h"


/// @brief ym_bdn 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_BDN \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsBdn)

/// @brief ym_bdn 用の名前空間の終了
#define END_NAMESPACE_YM_BDN \
END_NAMESPACE(nsBdn) \
END_NAMESPACE_YM

/// @namespace nsYm::nsBdn
/// @brief Bdn 関係のクラスが属する名前空間
///
/// ただし，主要なものは nsYm にインポートされている．
/// 実装用のローカルな要素だけがこの名前空間に存在する．

BEGIN_NAMESPACE_YM_BDN

class BdNetwork;
class BdnBlifWriter;
class BlifBdnConv;

class BdnNode;
class BdnNodeHandle;

class BdnEdge;


/// @brief 枝のリスト
/// @ingroup BdnGroup
typedef list<BdnEdge*> BdnEdgeList;

/// @brief ファンアウトリスト
/// @ingroup BdnGroup
typedef DlList<BdnEdge> BdnFanoutList;

/// @brief 節点のベクタ
/// @ingroup BdnGroup
typedef vector<BdnNode*> BdnNodeVector;

/// @brief 節点のリスト
/// @ingroup BdnGroup
typedef DlList<BdnNode> BdnNodeList;

END_NAMESPACE_YM_BDN

BEGIN_NAMESPACE_YM

using nsBdn::BdNetwork;
using nsBdn::BdnBlifWriter;
using nsBdn::BlifBdnConv;

using nsBdn::BdnNode;
using nsBdn::BdnNodeHandle;
using nsBdn::BdnNodeVector;
using nsBdn::BdnNodeList;

using nsBdn::BdnEdge;
using nsBdn::BdnEdgeList;

using nsBdn::BdnFanoutList;

END_NAMESPACE_YM

#endif // YM_BDN_BDN_NSDEF_H
