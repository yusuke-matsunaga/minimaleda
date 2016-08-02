#ifndef YM_BNET_BNETWORK_H
#define YM_BNET_BNETWORK_H

/// @file ym_bnet/BNetwork.h
/// @brief BNetwork のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BNetwork.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_bnet/bnet_nsdef.h"
#include "ym_utils/DlList.h"
#include "ym_utils/ItvlMgr.h"
#include "ym_utils/NameMgr.h"
#include "ym_utils/Binder.h"
#include "ym_lexp/LogExpr.h"

// Glossary
//   ネットワーク: 複数の入力，複数の出力を持つ論理ネットワーク
//   (BNetwork)    内部に組合せ論理のノードを持つ．
//   ノード:       入力ノード，出力ノード，組合せ論理ノードの
//   (BNode)       の総称．ネットワークの構成要素
//   入力ノード:   外部入力に対応するノード
//   出力ノード:   外部出力に対応するノード
//   組合せノード: 組合せ論理を表す中間ノード logic ノードと呼ぶことにする．
//   記憶ノード:   記憶素子を表すノード latch ノードと呼ぶことにする．


BEGIN_NAMESPACE_YM_BNET

class StrBNodeMap;

//////////////////////////////////////////////////////////////////////
/// @class BNodeEdge BNetwork.h <ym_bnet/BNetwork.h>
/// @ingroup BnetGroup
/// @brief ブーリアン・ネットワークの節点間を結ぶ枝を表すクラス
/// @sa BNode BNetwork
//////////////////////////////////////////////////////////////////////
class BNodeEdge :
  public DlElem
{
  friend class BNetwork;
  friend class BNodeMgr;
public:
  //////////////////////////////////////////////////////////////////////
  // public メソッドはすべて読みだし専用
  //////////////////////////////////////////////////////////////////////

  /// @brief ソースの節点を返す．
  BNode*
  from() const;
  
  /// @brief シンクの節点を返す．
  BNode*
  to() const;
  
  /// @brief シンクの何番目のファンインとなっているかを返す．
  ymuint
  pos() const;
  

private:
  //////////////////////////////////////////////////////////////////////
  // プライベートメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  BNodeEdge();
  
  /// @brief デストラクタ
  ~BNodeEdge();

  // コピーコンストラクタは使用禁止
  BNodeEdge(const BNodeEdge& src);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // シンクノードを指すポインタ
  BNode* mTo;
  
  // シンクノードの何番目のファンインかを示す番号
  ymuint32 mPos;

  // ソースノードを指すポインタ
  BNode* mFrom;

};


//////////////////////////////////////////////////////////////////////
/// @class BNodeList BNetwork.h <ym_bnet/BNetwork.h>
/// @ingroup BnetGroup
/// @brief BNode のリストを表すクラス
/// @sa BNode
//////////////////////////////////////////////////////////////////////
class BNodeList
{
  friend class BNetwork;
public:

  //////////////////////////////////////////////////////////////////////
  /// @class BNodeListConstIter BNetwork.h <ym_bnet/BNetwork.h>
  /// @ingroup BnetGroup
  /// @brief BNodeList の反復子
  /// @sa BNodeList
  //////////////////////////////////////////////////////////////////////
  class const_iterator
  {
    friend class BNodeList;
  public:

    /// @brief コンストラクタ
    const_iterator();

    /// @brief デストラクタ
    ~const_iterator();


  public:

    /// @brief 指している BNode を取り出す．
    BNode*
    operator*() const;

    /// @brief 次の要素を指す
    void
    operator++();

    friend
    bool
    operator==(const BNodeList::const_iterator& a,
	       const BNodeList::const_iterator& b);


  private:

    /// @brief BNodeList のみが用いるコンストラクタ
    const_iterator(ymuint which,
		   BNode* node);


  private:
    //////////////////////////////////////////////////////////////////////
    // データメンバ
    //////////////////////////////////////////////////////////////////////

    // BNode::mLink のどちらを使うかを示す．
    ymuint32 mWhich;

    // 現在指しているノード
    BNode* mNode;

  };
  
private:

  /// @brief コンストラクタ
  BNodeList(ymuint which);

  /// @brief デストラクタ
  ~BNodeList();


public:

  /// @brief 要素を追加する．
  void
  push_back(BNode* node);

  /// @brief 要素を削除する．
  void
  remove(BNode* node);

  /// @brief 先頭の反復子を得る．
  const_iterator
  begin() const;

  /// @brief 末尾の反復子を得る．
  const_iterator
  end() const;

  /// @brief 要素数を返す．
  ymuint
  size() const;


private:

  /// @brief クリアする．
  /// @note 危険なのでむやみに使ってはいけない．
  void
  clear();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // BNode::mLink のどちらを使うかを示す．
  ymuint32 mWhich;

  // ダミーの BNode
  BNode* mDummyHead;

  // 要素数
  ymuint32 mNum;

};


//////////////////////////////////////////////////////////////////////
/// @class BNode BNetwork.h <ym_bnet/BNetwork.h>
/// @ingroup BnetGroup
/// @brief ブーリアン・ネットワークの節点のクラス
///
/// 節点には，
/// - 外部入力
/// - 外部出力
/// - 論理ノード
/// - latchノード
///
/// の4種類がある．
/// 本当は継承を使って4つのクラスを作ってもいいのかも知れないが，
/// それほどのことをするまでもないので，必要な情報のユニオンをとって
/// メンバにしておく．
/// mFlags の下位の値によって種類を区別する．
/// 具体的には，mod 4の値で，
///   0: 外部入力
///   1: 外部出力
///   2: 論理ノード
///   3: latch ノード
/// としておく．
///
/// 外部入力ノードは名前のみの情報を持つ．
///
/// 外部出力ノードは名前とファンインのノードの情報を持つ．
/// 外部出力ノードの名前空間はその他のノードの名前空間とは別に考える．
/// 場合によってはファンインのノードと同じ名前の場合もありうる．
///
/// 論理ノードは名前とファンインのノード配列と論理式の情報を持つ．
///
/// latchノードは名前とファンインのノードとリセット値の情報を持つ．
/// リセット値は 0, 1, 2(不定) のいずれかの値をとる．
///
/// @sa BNetwork BNodeEdge
//////////////////////////////////////////////////////////////////////
class BNode
{
  friend class BNetwork;
  friend class BNodeList;
  friend class BNodeList::const_iterator;
  friend class BNodeMgr;
public:

  /// @brief ノードの型
  enum tType {
    /// @brief 外部入力
    kPI = 0,
    /// @brief 外部出力
    kPO = 1,
    /// @brief 論理ノード
    kLO = 2,
    /// @brief latch ノード
    kFF = 3
  };

  typedef ymuint32 tId;


private:

  /// @brief コンストラクタ
  BNode();
  
  /// @brief デストラクタ
  ~BNode();


public:
  //////////////////////////////////////////////////////////////////////
  /// @name ノードの型や属性を得る関数
  /// @{
  
  /// @brief ノードの型を取り出す．
  tType
  type() const;
  
  /// @brief 外部入力の時に真を返す．
  bool
  is_input() const;
  
  /// @brief 外部出力の時に真を返す．
  bool
  is_output() const;
  
  /// @brief 中間ノードの時に真を返す．
  bool
  is_logic() const;

  /// @brief 記憶ノードの時に真を返す．
  bool
  is_latch() const;

  /// @brief 定数0の論理を持つ時，真を返す．
  bool
  is_zero() const;
  
  /// @brief 定数1の論理を持つ時，真を返す．
  bool
  is_one() const;
  
  /// @brief バッファの論理を持つ時，真を返す．
  bool
  is_buffer() const;
  
  /// @brief インバータの論理を持つ時，真を返す．
  bool
  is_inverter() const;
  
  /// @brief ANDゲートの時, 真を返す．
  /// @note 入力が否定されている場合も含む．
  bool
  is_and() const;
  
  /// @brief ORゲートの時, 真を返す．
  /// @note 入力が否定されている場合も含む．
  bool
  is_or() const;
  
  /// @brief XORゲートの時, 真を返す．
  /// @note 入力が否定されている場合も含む．
  bool
  is_xor() const;
  
  /// @brief SOP 形式の論理式の時 true を返す．
  bool
  is_sop() const;

  /// @}
  //////////////////////////////////////////////////////////////////////


  //////////////////////////////////////////////////////////////////////
  /// @name ノードの情報を得る関数
  /// @{

  /// @brief ID番号を取出す．
  tId
  id() const;
  
  /// @brief 名前を取り出す．
  const char*
  name() const;
  
  /// @brief ファンイン数を取り出す．
  ymuint
  ni() const;
  
  /// @brief i番目のファンインを取り出す．
  /// @note 範囲外の場合には 0 を返す．
  BNode*
  fanin(ymuint pos) const;

  /// @brief i番めのファンインの枝を取り出す．
  /// @note 範囲外の場合には 0 を返す．
  BNodeEdge*
  fanin_edge(ymuint pos) const;
  
  /// @brief 引数の節点が何番目のファンインか調べる．
  /// @note ファンインではない場合には-1を返す．
  int
  fanin_pos(BNode* node) const;
  
  /// @brief ファンアウト数を取り出す．
  ymuint
  fanout_num() const;
  
  /// @brief ファンアウトリストを得る．
  /// @note この関数は呼ばれるたびにリストの複製を作る．
  BNodeEdgeList
  fanouts() const;
  
  /// @brief ファンアウトリストの先頭の反復子を返す．
  BNodeFoList::const_iterator
  fanouts_begin() const;
  
  /// @brief ファンアウトリストの末尾の反復子を返す．
  BNodeFoList::const_iterator
  fanouts_end() const;
  
  /// @brief 論理式を取り出す．
  LogExpr
  func() const;

  /// @brief latch ノードの時の初期値を取り出す．
  /// @retval 0 0
  /// @retval 1 1
  /// @retval 2 不定
  int
  reset_value() const;

  /// @brief ファクタードフォームのリテラル数を取り出す．
  ymuint
  litnum() const;
  
  /// @brief SOP形式のリテラル数の見積りを得る．
  /// @note Boolean factored formの場合には実際よりも
  /// 多めの値になる場合がある．
  ymuint
  sop_litnum() const;
  
  /// @brief ファクタードフォームに基づいて自分の価値を計算する．
  /// @note この値が低いほど，このノードの存在意義は薄い．
  int
  value() const;
  
  /// @brief 親のネットワークを返す．
  BNetwork*
  parent() const;

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // プライベートメンバ関数
  //////////////////////////////////////////////////////////////////////
  
  // コピーコンストラクタは使用禁止
  BNode(const BNode& src);
  

  //////////////////////////////////////////////////////////////////////
  // ノードの型，論理式，ファンイン,ファンアウトを
  // 操作するプリミティブ関数
  // これらの関数は上書きできない．
  // 代りに変更情報を持って ChgProc() が呼ばれるの
  // でそちらを上書きすること．
  //////////////////////////////////////////////////////////////////////

  // ノードタイプをセットする．
  void
  set_type(tType type);

  
private:
  //////////////////////////////////////////////////////////////////////
  // mFlags の操作だけをおこなう関数
  //////////////////////////////////////////////////////////////////////

  // タイプを得る．
  tType
  flags_get_type() const;
  
  // タイプをセットする．
  void
  flags_set_type(tType type);

  // latch ノードの場合の初期値を得る．
  int
  flags_get_reset_value() const;
  
  // latch ノードの場合の初期値を設定する．
  void
  flags_set_reset_value(int value);

  // temporary マークを得る．
  bool
  flags_get_temp() const;

  // temporary マークをセットする．
  void
  flags_set_temp();

  // temporary マークをクリアする．
  void
  flags_clr_temp();
  
  // 入力数を得る．
  ymuint
  flags_get_ni() const;

  // 入力数をセットする．
  void
  flags_set_ni(ymuint ni);

  
private:
  //////////////////////////////////////////////////////////////////////
  // プライベートなデータメンバ
  //////////////////////////////////////////////////////////////////////

  // mFlags のための定義
  // 最下位2ビットでノードの型を表す．
  // 3ビットめ以降は別の用途で用いられる．
  static
  const int kShiftType  = 0;
  static
  const int kShiftReset = 2;
  static
  const int kShiftTemp  = 4;
  static
  const int kShiftNI    = 5;
  static
  const ymuint32 kMaskType   = (3 << kShiftType);
  static
  const ymuint32 kMaskReset  = (3 << kShiftReset);
  static
  const ymuint32 kMaskTemp   = (1 << kShiftTemp);
  static
  const ymuint32 kMaskNI     = ((1 << kShiftNI) - 1);
  
  // 節点の種類を表す番号 ＋ いくつかのフラグ．
  ymuint32 mFlags;
  
  // ID番号
  tId mId;
  
  // ファンインの枝の配列
  BNodeEdge* mFaninEdgeArray;
  
  // ファンアウトの枝を入れるリスト
  DlList<BNodeEdge> mFanouts;
  
  // 機能を表す論理式
  LogExpr mFunc;
  
  // この節点の属しているネットワーク
  BNetwork* mParent;
  
  // ネットワーク中のリスト用のリンク
  struct {
    BNode* mPrev;
    BNode* mNext;
  } mLink[2];

};


//////////////////////////////////////////////////////////////////////
/// @class BNetChg BNetwork.h <ym_bnet/BNetwork.h>
/// @ingroup BnetGroup
/// @brief ネットワーク上の変更を表すクラス
/// @sa BNetwork
//////////////////////////////////////////////////////////////////////
class BNetChg
{
public:

  /// @brief 変更の種類を表す列挙型
  enum tType {
    /// @brief すべてクリア
    kAllClear,
    /// @brief 新規ノードの追加
    kNodeAdd,
    /// @brief ノードの削除
    kNodeDel,
    /// @brief ノードのファンインの変更
    kNodeFanin,
    /// @brief ノードの関数の変更
    kNodeFunc,
    /// @brief ノードの名前の変更
    kNodeName,
  };


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 生成関数
  /// @{

  /// @brief クリアイベントを生成するクラスメソッド
  static
  BNetChg
  all_clear();

  /// @brief 新規ノードの追加イベントを生成するクラスメソッド
  static
  BNetChg
  node_add(BNode::tId id);

  /// @brief ノードの削除イベントを生成するクラスメソッド
  static
  BNetChg
  node_delete(BNode::tId id);

  /// @brief ノードのファンインの変更イベントを生成するクラスメソッド
  static
  BNetChg
  node_fanin(BNode::tId id);

  /// @brief ノードの関数の変更イベントを生成するクラスメソッド
  static
  BNetChg
  node_func(BNode::tId id);

  /// @brief ノードの名前の変更イベントを生成するクラスメソッド
  static
  BNetChg
  node_name(BNode::tId id);

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 内容にアクセスするための関数
  /// @{

  /// @brief イベントタイプを返す．
  tType
  type() const;

  /// @brief ノードIDを返す．
  BNode::tId
  id() const;

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // プライベートメンバ関数
  //////////////////////////////////////////////////////////////////////

  // コンストラクタ
  BNetChg(tType type,
	  BNode::tId nodeid = 0);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変更の種類
  tType mType;

  // 該当ノードのID番号(意味を持たない場合もある．)
  BNode::tId mId;

};


/// @relates BNodeList::const_iterator
/// @brief 等価比較演算子
bool
operator==(const BNodeList::const_iterator& a,
	   const BNodeList::const_iterator& b);

/// @relates BNodeList::const_iterator
/// @brief 非等価比較演算子
bool
operator!=(const BNodeList::const_iterator& a,
	   const BNodeList::const_iterator& b);


//////////////////////////////////////////////////////////////////////
/// @class BNetwork BNetwork.h <ym_bnet/BNetwork.h>
/// @ingroup BnetGroup
/// @brief Boolean Network全体を表すクラス
/// @sa BNode BNodeEdge BNetManip
//////////////////////////////////////////////////////////////////////
class BNetwork
{
  friend class BNode;
  friend class BNetManip;
  friend class BNetworkTrace;
public:
  //////////////////////////////////////////////////////////////////////
  /// @name コンストラクタ/デストラクタ/初期化
  /// @{

  /// @brief 普通のコンストラクタ．
  /// @note 空のネットワークを作る．
  BNetwork();
  
  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のネットワーク
  BNetwork(const BNetwork& src);
  
  /// @brief デストラクタ
  ~BNetwork();
  
  /// @brief 代入演算子
  /// @param[in] src コピー元のネットワーク
  /// @return 自分自身への定数参照を返す．
  /// @note もちろん，古い内容はちゃんと捨てる．
  const BNetwork&
  operator=(const BNetwork& src);

  /// @}
  //////////////////////////////////////////////////////////////////////
  

public:
  //////////////////////////////////////////////////////////////////////
  /// @name 内部情報を得るためのメンバ関数
  /// @{
  
  /// @brief モデル名を得る．
  const char*
  model_name() const;
  
  /// @brief 入力数を得る．
  ymuint
  input_num() const;
  
  /// @brief 入力ノードのリストを返す．
  const BNodeList&
  inputs() const;
  
  /// @brief 入力ノードのリストの先頭の反復子を返す．
  BNodeList::const_iterator
  inputs_begin() const;
  
  /// @brief 入力ノードのリストの末尾の反復子を返す．
  BNodeList::const_iterator
  inputs_end() const;
  
  /// @brief 出力数を得る．
  ymuint
  output_num() const;
  
  /// @brief 出力ノードのリストを返す．
  const BNodeList&
  outputs() const;
  
  /// @brief 出力ノードのリストの先頭の反復子を返す．
  BNodeList::const_iterator
  outputs_begin() const;
  
  /// @brief 出力ノードのリストの先頭の反復子を返す．
  BNodeList::const_iterator
  outputs_end() const;
  
  /// @brief 中間節点の数を得る．
  ymuint
  logic_node_num() const;
  
  /// @brief 中間節点のリストを返す．
  const BNodeList&
  logic_nodes() const;
  
  /// @brief 中間節点のリストの先頭の反復子を返す．
  BNodeList::const_iterator
  logic_nodes_begin() const;
  
  /// @brief 中間節点のリストの末尾の反復子を返す．
  BNodeList::const_iterator
  logic_nodes_end() const;

  /// @brief latch ノードの数を得る．
  ymuint
  latch_node_num() const;

  /// @brief latch ノードのリストを返す．
  const BNodeList&
  latch_nodes() const;

  /// @brief latch ノードのリストの先頭の反復子を返す．
  BNodeList::const_iterator
  latch_nodes_begin() const;

  /// @brief latch ノードのリストの末尾の反復子を返す．
  BNodeList::const_iterator
  latch_nodes_end() const;
  
  /// @brief nameという名前をもつノードを探す．
  /// @param[in] name 探索対象の名前
  /// @retval name という名前を持つノード
  /// @retval NULL 該当するノードがない場合
  BNode*
  find_node(const char* name) const;
  
  /// @brief nameという名前をもつノードを探す．
  /// @param[in] name 探索対象の名前
  /// @retval name という名前を持つノード
  /// @retval NULL 該当するノードがない場合
  BNode*
  find_node(const string& name) const;
  
  /// @brief name という名前を持つ外部出力節点を取出す．
  /// @param[in] name 探索対象の名前
  /// @retval name という名前を持つ外部出力ノード
  /// @retval NULL 該当する節点が無い場合
  BNode*
  find_ponode(const char* name) const;
  
  /// @brief name という名前を持つ外部出力節点を取出す．
  /// @param[in] name 探索対象の名前
  /// @retval name という名前を持つ外部出力ノード
  /// @retval NULL 該当する節点が無い場合
  BNode*
  find_ponode(const string& name) const;
  
  /// @brief すべてのノード数を得る．
  ymuint
  node_num() const;
  
  /// @brief ノードの id 番号の最大値 + 1 を得る．
  ymuint
  max_node_id() const;
  
  /// @brief すべてのノードのリストを返す．
  const BNodeList&
  nodes() const;
  
  /// @brief すべてのノードのリストの先頭の反復子を返す．
  BNodeList::const_iterator
  nodes_begin() const;
  
  /// @brief すべてのノードのリストの末尾の反復子を返す．
  BNodeList::const_iterator
  nodes_end() const;
  
  /// @brief ID番号からノードを取出す．
  BNode*
  node(BNode::tId id) const;

  /// @brief 入力からのトポロジカルソート順を求める．
  /// @param[out] node_list 結果のノード配列
  /// @param[in] reverse true の時は出力からのトポロジカルソートを行う．
  void
  tsort(BNodeVector& node_list,
	bool reverse = false) const;

  /// @brief ノード名を取り出す．
  const char*
  node_name(const BNode* node) const;
  
  /// @brief 全部のリテラル数を得る．
  ymuint
  litnum() const;
  
  /// @brief SOP形式のリテラル数の見積りを得る．
  /// @note Boolean factored formの場合には実際よりも
  /// 多めの値になる場合がある．
  ymuint
  sop_litnum() const;

  /// @}
  //////////////////////////////////////////////////////////////////////

  
  //////////////////////////////////////////////////////////////////////
  /// @name TFI/TFO 操作関係の関数
  /// @{

  /// @brief 組み合わせ回路のループがないかチェックする．
  /// @return ループがあったら false を返す．
  bool
  acyclic_check() const;

  /// @}
  //////////////////////////////////////////////////////////////////////
  
  //////////////////////////////////////////////////////////////////////
  /// @name ネットワーク名/モデル名を設定する関数
  /// @{
  
  /// @brief モデル名を設定する．
  /// @param[in] name 設定するモデル名
  void
  set_model_name(const char* name);
  
  /// @brief モデル名を設定する．
  /// @param[in] name 設定するモデル名
  void
  set_model_name(const string& name);

  /// @}
  //////////////////////////////////////////////////////////////////////
  

  //////////////////////////////////////////////////////////////////////
  /// @name 自動生成名規則に関する関数
  /// @{

  /// @brief 自動に生成する名前の prefix と suffix を変更する．
  /// @param[in] prefix 新しいプリフィクス
  /// @param[in] suffix 新しいサフィックス
  /// @note 生成名は sprintf("%s%d%s", prefix, id, suffix)
  /// となる．
  void
  change_name_rule(const char* prefix,
		   const char* suffix);

  /// @brief 自動に生成する名前の prefix と suffix を変更する．
  /// @param[in] prefix 新しいプリフィクス
  /// @param[in] suffix 新しいサフィックス
  /// @note 生成名は sprintf("%s%d%s", prefix, id, suffix)
  /// となる．
  void
  change_name_rule(const string& prefix,
		   const string& suffix);
  
  /// @brief 自動生成名規則の prefix と suffix を得る．
  /// @param[out] prefix 現在のプリフィクス
  /// @param[out] suffix 現在のサフィックス
  void
  name_rule(string& prefix,
	    string& suffix) const;

  /// @}
  //////////////////////////////////////////////////////////////////////
  

public:
  //////////////////////////////////////////////////////////////////////
  /// @name ネットワークの構造を変更する関数
  /// @{
  
  /// @brief 今までの内容を捨てて空のネットワークにする．
  void
  clear();
  
  /// @brief 全てのノードの論理式を簡単化する．
  void
  lexp_simplify();

  /// @brief ノードの論理式を簡単化する．
  /// @param[in] node 対象のノード
  void
  lexp_simplify_node(BNode* node);
  
  /// @brief どこにもファンアウトしていないノードを削除する．
  void
  clean_up();
  
  /// @brief 定数ノード/バッファ/インバータ/使われていないノードの消去を行う．
  /// @note 副作用として clean_up() が呼び出される．
  void
  sweep();
  
  /// @brief ファンアウト数が0の外部入力ノードを削除する．
  void
  delete_unused_input();
  
  /// @brief 「価値」がしきい値以下のノードを削除する．
  /// @param[in] threshold しきい値
  /// @param[in] sop_limit SOP のリテラル数の増分の上限
  /// @param[in] auto_limit 上限を自動計算するとき true
  /// @note sop_limit が 0 のとき，上限なし
  /// @note sop_limit と auto_limit がともに指定されたら小さい方を用いる．
  void
  eliminate(int threshold,
	    ymuint sop_limit,
	    bool auto_limit = true);
  
  /// @}
  //////////////////////////////////////////////////////////////////////
  

protected:
  
  /// @brief 構造が変化した事を記録しておく関数
  void
  structure_changed();


private:

  /// @brief コピーを行なう
  void
  copy(const BNetwork& src);
  
  /// @brief 型と名前を指定してノードを作成し，それを登録する．
  /// @param[in] type 型
  /// @param[in] name 名前
  /// @retval 生成したノード
  /// @retval NULL 何らかのエラーが起きた場合
  /// @note 名前が NULL の場合には，既存の名前と重複しない名前を適当に
  /// つける．
  BNode*
  new_node(BNode::tType type,
	   const char* name = NULL);
  
  // @brief ノードの削除を行う．
  // @param[in] node 削除対象のノード
  // @retval true 削除が成功した．
  // @retval false 削除が失敗した．
  // @note 削除が失敗するのは以下の原因
  //  - nodeがこのネットワークに属していない場合
  //  - nodeが他の節点にファンアウトしている場合(外部出力節点から参照されて
  //    いる場合も含む)
  bool
  delete_node(BNode* node);

  // ノードの型を変える．
  void
  set_node_type(BNode* node,
		BNode::tType type);
		
  // ノードに名前を設定して登録する
  // name が ""(空) の場合には名前を自動生成する．
  // 名前が重複していた場合には false を返す．
  bool
  set_node_name(BNode* node,
		const char* name = NULL);
  
  // ノードの名前をクリアする．
  // 実際には名前をキーにした連想配列から取り除く
  void
  reset_node_name(BNode* node);

  // @brief ノード名の変更を行う．
  // @param[in] node 設定対象のノード
  // @param[in] name 新しい名前
  // @retval true 設定変更が成功した．
  // @retval false 名前が重複しており設定変更が失敗した．
  // @note name が空の場合には自動的に新しい名前が生成される．
  // (これはかなり特殊な使い方)
  bool
  rename_node(BNode* node,
	      const char* name);

  // ノードのファンインをセットする．
  void
  set_node_fanins(BNode* node,
		  const BNodeVector& fanins);

  // 枝のファンイン元のノードを切り替える．
  void
  change_edge_fanin(BNodeEdge* edge,
		    BNode* old_node,
		    BNode* new_node);

  // ノードの論理式をセットする．
  void
  set_node_func(BNode* node,
		const LogExpr& lexp);

  // ノードのリセット値をセットする．
  // latch ノードの場合のみ意味を持つ．
  void
  set_node_reset_value(BNode* node,
		       int reset_value);

  // tsortのためのサブルーティン
  void
  tsort_sub(BNode* node,
	    BNodeVector& node_list) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // blif ファイルに書き出すときのモデル名
  StrBuff mModelName;

  // ノードの名前の配列
  // node->id() をインデックスとする．
  vector<const char*> mNodeName;
  
  // 外部入力節点のリスト
  BNodeList mPiList;
  
  // 外部出力節点のリスト
  BNodeList mPoList;
  
  // 中間節点のリスト
  BNodeList mLogicList;

  // latch 節点のリスト
  BNodeList mLatchList;
  
  // すべての節点のリスト
  BNodeList mNodeList;
  
  // 名前から入力/中間節点を得る連想配列
  StrBNodeMap* mNameMap;
  
  // 名前から外部出力節点を取出すための連想配列
  StrBNodeMap* mPoMap;
  
  // ID番号をキーにしたノードのベクタ
  BNodeVector mNodeVector;
  
  // ノードのID番号管理用オブジェクト
  ItvlMgr mItvlMgr;
  
  // 名前の自動生成用オブジェクト
  NameMgr mNameMgr;
  
  // network trace を管理するオブジェクト
  T1BindMgr<BNetChg> mTraceMgr;

};


//////////////////////////////////////////////////////////////////////
/// @class BNetworkTrace BNetwork.h <ym_bnet/BNetwork.h>
/// @ingroup BnetGroup
/// @brief BNetwork に変更があったことを通知するラス
/// @sa BNetwork T1Binder
//////////////////////////////////////////////////////////////////////
class BNetworkTrace :
  public T1Binder<BNetChg>
{
public:

  /// @brief コンストラクタ
  /// @note この時点ではネットワークに結び付いていない．
  BNetworkTrace();
  
  /// @brief デストラクタ
  /// @note ネットワークにバインドしていたら解除する．
  virtual
  ~BNetworkTrace();
  
  /// @brief ネットワークとのバインドを解除する．
  void
  unbind();
  
  /// @brief ネットワークとバインドする．
  /// @param[in] network バインドするネットワーク
  void
  bind(BNetwork* network);
  
  /// @brief ネットワークを取り出す．
  BNetwork*
  network() const;
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のネットワーク
  BNetwork* mNetwork;
};



//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// コンストラクタ
inline
BNodeEdge::BNodeEdge()
{
}

// デストラクタ
inline
BNodeEdge::~BNodeEdge()
{
}

// ソースの節点を返す．
inline
BNode*
BNodeEdge::from() const
{
  return mFrom;
}

// シンクの節点を返す．
inline
BNode*
BNodeEdge::to() const
{
  return mTo;
}

// シンクの何番目のファンインとなっているかを返す．
inline
ymuint
BNodeEdge::pos() const
{
  return mPos;
}

// ノードの型を取り出す．
inline
BNode::tType
BNode::type() const
{
  return flags_get_type();
}

// ID番号を取出す．
inline
BNode::tId
BNode::id() const
{
  return mId;
}

// 外部入力の時に真を返す．
inline
bool
BNode::is_input() const
{
  return type() == kPI;
}

// 外部出力の時に真を返す．
inline
bool
BNode::is_output() const
{
  return type() == kPO;
}

// 中間ノードの時に真を返す．
inline
bool
BNode::is_logic() const
{
  return type() == kLO;
}

// latch ノードの時に真を返す．
inline
bool
BNode::is_latch() const
{
  return type() == kFF;
}

// 定数0の論理を持つ時，真を返す．
inline
bool
BNode::is_zero() const
{
  return mFunc.is_zero();
}

// 定数1の論理を持つ時，真を返す．
inline
bool
BNode::is_one() const
{
  return mFunc.is_one();
}

// バッファの論理を持つ時，真を返す．
inline
bool
BNode::is_buffer() const
{
  return mFunc.is_posiliteral();
}

// インバータの論理を持つ時，真を返す．
inline
bool
BNode::is_inverter() const
{
  return mFunc.is_negaliteral();
}

// ANDゲートの時, 真を返す．
// 入力が否定されている場合も含む．
inline
bool
BNode::is_and() const
{
  return mFunc.is_simple_and();
}

// ORゲートの時, 真を返す．
// 入力が否定されている場合も含む．
inline
bool
BNode::is_or() const
{
  return mFunc.is_simple_or();
}

// XORゲートの時, 真を返す．
// 入力が否定されている場合も含む．
inline
bool
BNode::is_xor() const
{
  return mFunc.is_simple_xor();
}

// SOP 形式の論理式の時, true を返す．
inline
bool
BNode::is_sop() const
{
  return mFunc.is_sop();
}

// 名前を取り出す．
inline
const char*
BNode::name() const
{
  return mParent->node_name(this);
}

// ファンイン数を取り出す．
inline
ymuint
BNode::ni() const
{
  return flags_get_ni();
}

// i番目のファンインを取り出す．範囲外の場合には 0 を返す．
inline
BNode*
BNode::fanin(ymuint pos) const
{
  return (pos < ni()) ? mFaninEdgeArray[pos].from() : 0;
}

// i番めのファンインの枝を取り出す．範囲外の場合には 0 を返す．
inline
BNodeEdge*
BNode::fanin_edge(ymuint pos) const
{
  return (pos < ni()) ? &mFaninEdgeArray[pos] : 0;
}

// ファンアウト数を取り出す．
inline
ymuint
BNode::fanout_num() const
{
  return mFanouts.size();
}

// ファンアウトリストを得る．
inline
BNodeEdgeList
BNode::fanouts() const
{
  return BNodeEdgeList(fanouts_begin(), fanouts_end());
}

// ファンアウトリストの先頭の反復子を返す．
inline
BNodeFoList::const_iterator
BNode::fanouts_begin() const
{
  return mFanouts.begin();
}

// ファンアウトリストの末尾の反復子を返す．
inline
BNodeFoList::const_iterator
BNode::fanouts_end() const
{
  return mFanouts.end();
}

// 論理式を取り出す．
inline
LogExpr
BNode::func() const
{
  return mFunc;
}

// @brief latch ノードの時の初期値を取り出す．
inline
int
BNode::reset_value() const
{
  return flags_get_reset_value();
}

// ファクタードフォームのリテラル数を取り出す．
inline
ymuint
BNode::litnum() const
{
  return mFunc.litnum();
}

// 親のネットワークを返す．
inline
BNetwork*
BNode::parent() const
{
  return mParent;
}

// ノードの型を取り出す．
inline
BNode::tType
BNode::flags_get_type() const
{
  return tType((mFlags & kMaskType) >> kShiftType);
}

// タイプをセットする．
inline
void
BNode::flags_set_type(tType type)
{
  mFlags &= ~kMaskType;
  mFlags |= (type << kShiftType);
}

// @brief latch ノードの時の初期値を取り出す．
inline
int
BNode::flags_get_reset_value() const
{
  return (mFlags & kMaskReset) >> kShiftReset;
}

// latch ノードの場合の初期値を設定する．
inline
void
BNode::flags_set_reset_value(int value)
{
  mFlags &= ~kMaskReset;
  mFlags |= (value << kShiftReset) & kMaskReset;
}

// temporary マークを得る．
inline
bool
BNode::flags_get_temp() const
{
  return static_cast<bool>((mFlags >> kShiftTemp) & 1);
}

// temporary マークをセットする．
inline
void
BNode::flags_set_temp()
{
  mFlags |= kMaskTemp;
}

// temporary マークをクリアする．
inline
void
BNode::flags_clr_temp()
{
  mFlags &= ~kMaskTemp;
}

// ファンイン数を取り出す．
inline
ymuint
BNode::flags_get_ni() const
{
  return (mFlags >> kShiftNI);
}

// 入力数をセットする．
inline
void
BNode::flags_set_ni(ymuint ni)
{
  mFlags &= kMaskNI;
  mFlags |= (ni << kShiftNI);
}

// イベントタイプを返す．
inline
BNetChg::tType
BNetChg::type() const
{
  return mType;
}

// ノードIDを返す．
inline
BNode::tId
BNetChg::id() const
{
  return mId;
}
  
// クリアイベントを生成するクラスメソッド
inline
BNetChg
BNetChg::all_clear()
{
  return BNetChg(kAllClear);
}

// 新規ノードの追加イベントを生成するクラスメソッド
inline
BNetChg
BNetChg::node_add(BNode::tId id)
{
  return BNetChg(kNodeAdd, id);
}

// ノードの削除イベントを生成するクラスメソッド
inline
BNetChg
BNetChg::node_delete(BNode::tId id)
{
  return BNetChg(kNodeDel, id);
}

// ノードのファンインの変更イベントを生成するクラスメソッド
inline
BNetChg
BNetChg::node_fanin(BNode::tId id)
{
  return BNetChg(kNodeFanin, id);
}

// ノードの関数の変更イベントを生成するクラスメソッド
inline
BNetChg
BNetChg::node_func(BNode::tId id)
{
  return BNetChg(kNodeFunc, id);
}

// ノードの名前の変更イベントを生成するクラスメソッド
inline
BNetChg
BNetChg::node_name(BNode::tId id)
{
  return BNetChg(kNodeName, id);
}

// コンストラクタ
inline
BNetChg::BNetChg(tType type,
		 BNode::tId nodeid) :
  mType(type),
  mId(nodeid)
{
}

// @brief コンストラクタ
inline
BNodeList::BNodeList(ymuint which) :
  mWhich(which),
  mDummyHead(new BNode())
{
  clear();
}
  
// @brief デストラクタ
inline
BNodeList::~BNodeList()
{
  delete mDummyHead;
}

// @brief 要素を追加する．
inline
void
BNodeList::push_back(BNode* node)
{
  BNode* prev = mDummyHead->mLink[mWhich].mPrev;
  prev->mLink[mWhich].mNext = node;
  node->mLink[mWhich].mPrev = prev;
  node->mLink[mWhich].mNext = mDummyHead;
  mDummyHead->mLink[mWhich].mPrev = node;
  ++ mNum;
}

// @brief 要素を削除する．
inline
void
BNodeList::remove(BNode* node)
{
  BNode* prev = node->mLink[mWhich].mPrev;
  BNode* next = node->mLink[mWhich].mNext;
  prev->mLink[mWhich].mNext = next;
  next->mLink[mWhich].mPrev = prev;
  -- mNum;
}

// @brief クリアする．
inline
void
BNodeList::clear()
{
  mDummyHead->mLink[mWhich].mPrev = mDummyHead;
  mDummyHead->mLink[mWhich].mNext = mDummyHead;
  mNum = 0;
}

// @brief 先頭の反復子を得る．
inline
BNodeList::const_iterator
BNodeList::begin() const
{
  return BNodeList::const_iterator(mWhich, mDummyHead->mLink[mWhich].mNext);
}

// @brief 末尾の反復子を得る．
inline
BNodeList::const_iterator
BNodeList::end() const
{
  return BNodeList::const_iterator(mWhich, mDummyHead);
}

// @brief 要素数を返す．
inline
ymuint
BNodeList::size() const
{
  return mNum;
}

// @brief コンストラクタ
inline
BNodeList::const_iterator::const_iterator() :
  mWhich(0),
  mNode(NULL)
{
}

// @brief BNodeList のみが用いるコンストラクタ
inline
BNodeList::const_iterator::const_iterator(ymuint which,
					  BNode* node) :
  mWhich(which),
  mNode(node)
{
}

// @brief デストラクタ
inline
BNodeList::const_iterator::~const_iterator()
{
}

// @brief 指している BNode を取り出す．
inline
BNode*
BNodeList::const_iterator::operator*() const
{
  return mNode;
}

// @brief 次の要素を指す
inline
void
BNodeList::const_iterator::operator++()
{
  if ( mNode ) {
    mNode = mNode->mLink[mWhich].mNext;
  }
}

// @brief 等価比較演算子
inline
bool
operator==(const BNodeList::const_iterator& a,
	   const BNodeList::const_iterator& b)
{
  return a.mWhich == b.mWhich && a.mNode == b.mNode;
}

// @brief 非等価比較演算子
inline
bool
operator!=(const BNodeList::const_iterator& a,
	   const BNodeList::const_iterator& b)
{
  return !operator==(a, b);
}
      
// モデル名を得る．
inline
const char*
BNetwork::model_name() const
{
  return mModelName.c_str();
}
  
// @brief nameという名前をもつノードを探す．
inline
BNode*
BNetwork::find_node(const string& name) const
{
  return find_node(name.c_str());
}
  
// @brief name という名前を持つ外部出力節点を取出す．
inline
BNode*
BNetwork::find_ponode(const string& name) const
{
  return find_ponode(name.c_str());
}
      
// 入力数を得る．
inline
ymuint
BNetwork::input_num() const
{
  return mPiList.size();
}
      
// 入力ノードのリストを返す．
inline
const BNodeList&
BNetwork::inputs() const
{
  return mPiList;
}

// 入力ノードのリストの先頭の反復子を返す．
inline
BNodeList::const_iterator
BNetwork::inputs_begin() const
{
  return mPiList.begin();
}

// 入力ノードのリストの末尾の反復子を返す．
inline
BNodeList::const_iterator
BNetwork::inputs_end() const
{
  return mPiList.end();
}
      
// 出力数を得る．
inline
ymuint
BNetwork::output_num() const
{
  return mPoList.size();
}
      
// 出力ノードのリストを返す．
inline
const BNodeList&
BNetwork::outputs() const
{
  return mPoList;
}

// 出力ノードのリストの先頭の反復子を返す．
inline
BNodeList::const_iterator
BNetwork::outputs_begin() const
{
  return mPoList.begin();
}

// 出力ノードのリストの先頭の反復子を返す．
inline
BNodeList::const_iterator
BNetwork::outputs_end() const
{
  return mPoList.end();
}
      
// 中間節点の数を得る．
inline
ymuint
BNetwork::logic_node_num() const
{
  return mLogicList.size();
}
      
// 中間節点のリストを返す．
inline
const BNodeList&
BNetwork::logic_nodes() const
{
  return mLogicList;
}

// 中間節点のリストの先頭の反復子を返す．
inline
BNodeList::const_iterator
BNetwork::logic_nodes_begin() const
{
  return mLogicList.begin();
}

// 中間節点のリストの末尾の反復子を返す．
inline
BNodeList::const_iterator
BNetwork::logic_nodes_end() const
{
  return mLogicList.end();
}
      
// latch 節点の数を得る．
inline
ymuint
BNetwork::latch_node_num() const
{
  return mLatchList.size();
}
      
// 中間節点のリストを返す．
inline
const BNodeList&
BNetwork::latch_nodes() const
{
  return mLatchList;
}

// 中間節点のリストの先頭の反復子を返す．
inline
BNodeList::const_iterator
BNetwork::latch_nodes_begin() const
{
  return mLatchList.begin();
}

// 中間節点のリストの末尾の反復子を返す．
inline
BNodeList::const_iterator
BNetwork::latch_nodes_end() const
{
  return mLatchList.end();
}

// すべてのノード数を得る．
inline
ymuint
BNetwork::node_num() const
{
  return mNodeList.size();
}

// ノードの id 番号の最大値 + 1 を得る．
inline
ymuint
BNetwork::max_node_id() const
{
  return static_cast<ymuint>(mItvlMgr.max_id() + 1);
}
      
// すべてのノードのリストを返す．
inline
const BNodeList&
BNetwork::nodes() const
{
  return mNodeList;
}

// すべてのノードのリストの先頭の反復子を返す．
inline
BNodeList::const_iterator
BNetwork::nodes_begin() const
{
  return mNodeList.begin();
}

// すべてのノードのリストの末尾の反復子を返す．
inline
BNodeList::const_iterator
BNetwork::nodes_end() const
{
  return mNodeList.end();
}

// ID番号からノードを取出す．
inline
BNode*
BNetwork::node(BNode::tId id) const
{
  return mNodeVector[id];
}

// @brief ノード名を取り出す．
inline
const char*
BNetwork::node_name(const BNode* node) const
{
  return mNodeName[node->id()];
}

// @brief 自動に生成する名前の prefix と suffix を変更する．
inline
void
BNetwork::change_name_rule(const string& prefix,
			   const string& suffix)
{
  change_name_rule(prefix.c_str(), suffix.c_str());
}

// コンストラクタ
// この時点ではネットワークに結び付いていない．
inline
BNetworkTrace::BNetworkTrace()
{
  mNetwork = NULL;
}

// ネットワークとのバインドを解除する．
inline
void
BNetworkTrace::unbind()
{
  if ( mNetwork ) {
    mNetwork->mTraceMgr.unreg_binder(this);
    mNetwork = NULL;
  }
}

// ネットワークとバインドする．
inline
void
BNetworkTrace::bind(BNetwork* network)
{
  unbind();
  mNetwork = network;
  network->mTraceMgr.reg_binder(this);
}

// ネットワークを取り出す．
inline
BNetwork*
BNetworkTrace::network() const
{
  return mNetwork;
}

END_NAMESPACE_YM_BNET

BEGIN_NAMESPACE_HASH
// BNode へのポインタをキーにしたハッシュ関数クラスの定義
template <>
struct hash<nsYm::nsBnet::BNode*>
{
  ymuint
  operator()(nsYm::nsBnet::BNode* node) const
  {
    return reinterpret_cast<ympuint>(node)/sizeof(void*);
  }
};
END_NAMESPACE_HASH

#endif // YM_BNET_BNETWORK_H
