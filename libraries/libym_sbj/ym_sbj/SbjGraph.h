#ifndef YM_SBJ_SBJGRAPH_H
#define YM_SBJ_SBJGRAPH_H

/// @file ym_sbj/SbjGraph.h
/// @brief SbjGraph のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SbjGraph.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_sbj/sbj_nsdef.h"

#include "ym_utils/Alloc.h"
#include "ym_utils/DlList.h"
#include "ym_utils/ItvlMgr.h"


BEGIN_NAMESPACE_YM_SBJ

//////////////////////////////////////////////////////////////////////
/// @class SbjEdge SbjGraph.h "SbjGraph.h"
/// @brief サブジェクトグラフの枝を表すクラス
///
/// - 入力側のノード
/// - 出力側のノード
/// - 出力側のノードの何番目(0 or 1)の入力か
/// の情報を持つ．
/// @sa SbjNode SbjGraph
//////////////////////////////////////////////////////////////////////
class SbjEdge :
  public DlElem
{
  friend class SbjNode;
  friend class SbjGraph;

public:

  /// @brief 入力側のノードを得る．
  /// @retval 入力側のノード 通常の枝の場合
  /// @retval NULL 定数0に接続している枝の場合
  const SbjNode*
  from() const;

  /// @brief 出力側のノードを得る．
  /// @return 出力側のノードを返す．
  const SbjNode*
  to() const;

  /// @brief 入力側のノードを得る．
  /// @retval 入力側のノード 通常の枝の場合
  /// @retval NULL 定数0に接続している枝の場合
  SbjNode*
  from();

  /// @brief 出力側のノードを得る．
  /// @return 出力側のノードを返す．
  SbjNode*
  to();

  /// @brief 出力側のノードの何番目の入力かを示す．
  ymuint
  pos() const;

  /// @brief 出力ノードに接続している時 true を返す．
  bool
  is_poedge() const;


private:
  //////////////////////////////////////////////////////////////////////
  // プライベートメンバ関数
  // これらは friend class からしか呼ぶことができない．
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  SbjEdge();

  /// @brief デストラクタ
  ~SbjEdge();

  /// @brief from ノードをセットする．
  void
  set_from(SbjNode* from);

  /// @brief to ノードをセットする．
  void
  set_to(SbjNode* to,
	 ymuint pos);


private:
  //////////////////////////////////////////////////////////////////////
  // プライベートメンバ変数
  //////////////////////////////////////////////////////////////////////

  // 入力側のノード
  SbjNode* mFrom;

  // 出力側のノード
  SbjNode* mTo;

  // 入力位置
  ymuint32 mIpos;

};


//////////////////////////////////////////////////////////////////////
/// @class SbjNode SbjGraph.h "SbjGraph.h"
/// @brief サブジェクトグラフを構成するプリミティブノードを表すクラス
///
/// ノードの種類は
/// - 入力ノード
/// - 出力ノード
/// - 論理ノード
/// - DFFノード
/// の 4種類がある．
/// 論理ノードの場合，ノードタイプが AND と XOR の2種類があり，
/// さらに2つの入力の極性がある．
/// ただし，XOR タイプの場合には入力は反転させない．
/// @sa SbjEdge SbjGraph
//////////////////////////////////////////////////////////////////////
class SbjNode :
  public DlElem
{
  friend class SbjGraph;

public:
  /// @brief ノードの型
  enum tType {
    /// @brief 入力ノード
    kINPUT  = 0,
    /// @brief 出力ノード
    kOUTPUT = 1,
    /// @brief 論理ノード
    kLOGIC  = 2,
    /// @brief DFFノード
    kDFF    = 3
  };


private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  // SbjGraph のみが呼び出せる．
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  SbjNode();

  /// @brief デストラクタ
  ~SbjNode();


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 構造に関する情報を取り出す関数
  /// @{

  /// @brief ID 番号の取得
  /// @return ID 番号を返す．
  /// @note ID 番号はノードの生成時に SbjGraph により自動的に割り振られる．
  /// @sa SbjGraph
  ymuint
  id() const;

  /// @brief ID を表す文字列の取得
  /// @note デバッグ時にしか意味を持たない
  string
  id_str() const;

  /// @brief タイプを得る．
  tType
  type() const;

  /// @brief 入力ノードの時に true を返す．
  bool
  is_input() const;

  /// @brief 出力ノードの時に true を返す．
  bool
  is_output() const;

  /// @brief 論理ノードの時に true を返す．
  bool
  is_logic() const;

  /// @brief AND ノードの時に true を返す．
  bool
  is_and() const;

  /// @brief XOR ノードの時に true を返す．
  bool
  is_xor() const;

  /// @brief DFFノードの時に true を返す．
  bool
  is_dff() const;

  /// @brief 入力ノードか DFF ノードの時に true を返す．
  bool
  is_ppi() const;

  /// @brief 出力ノードか DFF ノードの時に true を返す．
  bool
  is_ppo() const;

  /// @brief サブID (入力／出力番号)を得る．
  /// @note 入力ノード/出力ノードの場合のみ意味を持つ．
  ymuint
  subid() const;

  /// @brief 出力ノードの極性を返す．
  /// @return 反転していたら true を返す．
  bool
  output_inv() const;

  /// @brief 機能コードを得る．
  /// @note 論理ノードの場合のみ意味を持つ．
  /// @note 機能コードの意味は以下の通り
  ///  - 000 : a & b
  ///  - 001 : a' & b
  ///  - 010 : a & b'
  ///  - 011 : a' & b'
  ///  - 100 : a ^ b
  /// このうち，000 〜 011 が is_and() で true を返す．
  /// 100 が is_xor() で true を返す．
  ymuint
  fcode() const;

  /// @brief ファンインのノードを得る．
  /// @param[in] pos 入力番号(0 or 1)
  /// @return pos 番めのファンインのノード
  /// @note 該当するファンインがなければ NULL を返す．
  const SbjNode*
  fanin(ymuint pos) const;

  /// @brief ファンインの反転属性を得る．
  /// @param[in] pos 入力番号 (0 or 1)
  bool
  fanin_inv(ymuint pos) const;

  /// @brief ファンインの枝を得る．
  /// @param[in] pos 入力番号(0 or 1)
  /// @return pos 番目の入力の枝
  /// @note 該当するファンインの枝がなければ NULL を返す．
  const SbjEdge*
  fanin_edge(ymuint pos) const;

  /// @brief ファンアウト数を得る．
  ymuint
  fanout_num() const;

  /// @brief ファンアウトリストを得る．
  const SbjEdgeList&
  fanout_list() const;

  /// @brief 出力ノードにファンアウトしているとき true を返す．
  bool
  pomark() const;

  /// @brief DFFノードの場合のデータ入力を得る．
  const SbjNode*
  fanin_data() const;

  /// @brief DFFノードの場合のデータ入力の反転属性を得る．
  bool
  fanin_data_inv() const;

  /// @brief DFFノードの場合のクロック入力を得る．
  const SbjNode*
  fanin_clock() const;

  /// @brief DFFノードの場合のクロック入力の反転属性を得る．
  bool
  fanin_clock_inv() const;

  /// @brief DFFノードの場合のセット入力を得る．
  const SbjNode*
  fanin_set() const;

  /// @brief DFFノードの場合のセット入力の反転属性を得る．
  bool
  fanin_set_inv() const;

  /// @brief DFFノードの場合のリセット入力を得る．
  const SbjNode*
  fanin_rst() const;

  /// @brief DFFノードの場合のリセット入力の反転属性を得る．
  bool
  fanin_rst_inv() const;

  /// @brief レベルを得る．
  ymuint
  level() const;

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // 内容の設定に用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファンインのノードを得る．
  /// @param[in] pos 入力番号(0 or 1)
  /// @return pos 番めのファンインのノード
  /// @note 該当するファンインがなければ NULL を返す．
  SbjNode*
  fanin(ymuint pos);

  /// @brief ファンインの枝を得る．
  /// @param[in] pos 入力番号(0 or 1)
  /// @return pos 番目の入力の枝
  /// @note 該当するファンインの枝がなければ NULL を返す．
  SbjEdge*
  fanin_edge(ymuint pos);

  /// @brief DFFノードの場合のデータ入力を得る．
  SbjNode*
  fanin_data();

  /// @brief DFFノードの場合のクロック入力を得る．
  SbjNode*
  fanin_clock();

  /// @brief DFFノードの場合のセット入力を得る．
  SbjNode*
  fanin_set();

  /// @brief DFFノードの場合のリセット入力を得る．
  SbjNode*
  fanin_rst();

  /// @brief タイプを入力に設定する．
  void
  set_input(ymuint subid);

  /// @brief タイプを出力に設定する．
  /// @param[in] inv 反転属性
  void
  set_output(ymuint subid,
	     bool inv);

  /// @brief タイプを論理に設定する．
  void
  set_logic(ymuint fcode);

  /// @brief タイプをDFFに設定する．
  /// @param[in] inv データ入力の反転属性
  void
  set_dff(bool inv);

  /// @brief DFFノードのクロック極性を設定する．
  void
  set_dff_clock(bool inv);

  /// @brief DFFノードのセット極性を設定する．
  void
  set_dff_set(bool inv);

  /// @brief DFFノードのリセット極性を設定する．
  void
  set_dff_rst(bool inv);

  /// @brief ファンアウトに出力が含まれているか調べ pomark をセットする．
  void
  scan_po();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID 番号
  ymuint32 mId;

  // タイプ (+ 入力/出力番号/機能コード)
  ymuint32 mFlags;

  // ファンインの枝(そのもの)の配列
  // 要素数は通常2だが DFF の場合は4
  SbjEdge* mFanins;

  // ファンアウトの枝のリスト
  SbjEdgeList mFanoutList;

  // 作業用のマーク
  ymuint32 mMark;

  // レベル
  ymuint32 mLevel;


private:
  //////////////////////////////////////////////////////////////////////
  // 定数の定義
  //////////////////////////////////////////////////////////////////////

  // mFlags の演算で用いる定数
  static
  const int kTypeShift = 0;
  static
  const int kOinvShift = 2;
  static
  const int kSubidShift = 3;
  static
  const int kFcodeShift = 2;
  static
  const int kDDinvShift = 2;
  static
  const int kDCinvShift = 3;
  static
  const int kDSinvShift = 4;
  static
  const int kDRinvShift = 5;

  static
  const ymuint kTypeMask = 3U;
  static
  const ymuint kFcodeMask = 7U;

  // mMark の演算で用いる定数
  static
  const int kPoShift = 0;

  static
  const ymuint kPoMask = (1U << kPoShift);

};


//////////////////////////////////////////////////////////////////////
/// @class SbjPort SbjGraph.h "SbjGraph.h"
/// @brief ポートを表すクラス
///
/// ポートは通常，名前を持ち，1つもしくは複数の入出力ノード
/// と対応づけられる．
//////////////////////////////////////////////////////////////////////
class SbjPort
{
  friend class SbjGraph;

private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] name 名前
  /// @param[in] body 入出力ノードのベクタ
  SbjPort(const string& name,
	  const vector<SbjNode*>& body);

  /// @brief デストラクタ
  ~SbjPort();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を得る．
  string
  name() const;

  /// @brief ビット数を得る．
  ymuint
  bit_width() const;

  /// @brief pos ビット目の内容を得る．
  /// @param[in] pos ビット位置 ( 0 <= pos < bit_width() )
  const SbjNode*
  bit(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  string mName;

  // 入出力ノードのベクタ
  vector<SbjNode*> mBody;

};


//////////////////////////////////////////////////////////////////////
/// @class SbjGraph SbjGraph.h "SbjGraph.h"
/// @brief サブジェクトグラフを表すクラス
///
/// 入力ノード，出力ノード，論理ノード，DFFノードを分けてリストで持つ
/// ( @sa input_list(), output_list(), lnode_list(), dff_list() )．
/// また，すべてのノードに唯一な ID 番号を割り振っており，その ID 番号
/// からノードを取り出すこともできる( @sa node(ymuint id) )．
/// 入力ノード，出力ノードは ID 番号とは別に入力 ID 番号，および出力
/// ID 番号を持っており，それらの ID 番号からノードを取り出すこともできる．
/// ( @sa input(ymuint id), output(ymuint id) )
/// 論理ノードを入力からのトポロジカル順で処理するためには sort()
/// を用いてソートされたノードのベクタを得る．
/// @sa SbjEdge SbjNode
//////////////////////////////////////////////////////////////////////
class SbjGraph
{
public:

  /// @brief コンストラクタ
  SbjGraph();

  /// @brief コピーコンストラクタ
  SbjGraph(const SbjGraph& src);

  /// @brief 代入演算子
  const SbjGraph&
  operator=(const SbjGraph& src);

  /// @brief デストラクタ
  ~SbjGraph();


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 外部インターフェイス情報の取得
  /// @{

  /// @brief モジュール名を得る．
  string
  name() const;

  /// @brief ポート数を得る．
  ymuint
  port_num() const;

  /// @brief ポートを得る．
  /// @param[in] pos 位置 ( 0 <= pos < port_num() )
  const SbjPort*
  port(ymuint pos) const;

  /// @brief 入出力ノードに関連づけられたポートを得る．
  /// @param[in] node 入出力ノード
  const SbjPort*
  port(const SbjNode* node) const;

  /// @brief 入出力ノードのポートにおけるビット位置を得る．
  /// @param[in] node 入出力ノード
  ymuint
  port_pos(const SbjNode* node) const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 外部インターフェイス情報の設定
  /// @{

  /// @brief モジュール名を設定する．
  void
  set_name(const string& name);

  /// @brief ポートを追加する(1ビット版)．
  /// @param[in] name ポート名
  /// @param[in] io_node 対応付ける入出力ノード
  void
  add_port(const string& name,
	   SbjNode* io_node);

  /// @brief ポートを追加する(ベクタ版)．
  /// @param[in] name ポート名
  /// @param[in] body 対応付ける入出力ノードのベクタ
  void
  add_port(const string& name,
	   const vector<SbjNode*>& body);

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name ノード関連の情報の取得
  /// @{

  /// @brief ノードIDの最大値 + 1 の取得
  /// @return ノードIDの最大値 + 1 を返す．
  ymuint
  max_node_id() const;

  /// @brief ID 番号によるノードの取得
  /// @param[in] id ID 番号 ( 0 <= id < max_node_id() )
  /// @return ID 番号が id のノードを返す．
  /// @retrun 該当するノードが無い場合には NULL を返す．
  const SbjNode*
  node(ymuint id) const;

  /// @brief 入力ノード数の取得
  /// @return 入力ノード数を返す．
  ymuint
  input_num() const;

  /// @brief 入力 ID 番号による入力ノードの取得
  /// @param[in] id 入力 ID 番号 ( 0 <= id < n_inputs() )
  /// @return 入力 ID 番号が id のノードを返す．
  SbjNode*
  input(ymuint id) const;

  /// @brief 入力ノードのリストを得る．
  const SbjNodeList&
  input_list() const;

  /// @brief 入力ノードと DFF ノードのリストを返す．
  /// @param[out] node_list ノードを格納するリスト
  /// @return 要素数を返す．
  ymuint
  ppi_list(vector<const SbjNode*>& node_list) const;

  /// @brief 出力のノード数を得る．
  ymuint
  output_num() const;

  /// @brief 出力 ID 番号による出力ノードの取得
  /// @param[in] id 出力 ID 番号 ( 0 <= id < n_outputs() )
  /// @return 出力 ID 番号が id のノードを返す．
  SbjNode*
  output(ymuint id) const;

  /// @brief 出力ノードのリストを得る．
  const SbjNodeList&
  output_list() const;

  /// @brief 出力ノードと DFF ノードのリストを返す．
  /// @param[out] node_list ノードを格納するリスト
  /// @return 要素数を返す．
  ymuint
  ppo_list(vector<const SbjNode*>& node_list) const;

  /// @brief 論理ノード数を得る．
  ymuint
  lnode_num() const;

  /// @brief 論理ノードのリストを得る．
  const SbjNodeList&
  lnode_list() const;

  /// @brief DFFノード数を得る．
  ymuint
  dff_num() const;

  /// @brief DFFノードのリストを得る．
  const SbjNodeList&
  dff_list() const;

  /// @brief ソートされた論理ノードのリストを得る．
  void
  sort(vector<const SbjNode*>& node_list) const;

  /// @brief 逆順で論理ソートされたノードのリストを得る．
  void
  rsort(vector<const SbjNode*>& node_list) const;

  /// @brief 段数を求める．
  /// @note 副作用として各 SbjNode のレベルが設定される．
  ymuint
  level() const;

  /// @brief 各ノードの minimum depth を求める．
  /// @param[in] k LUT の最大入力数
  /// @param[out] depth_array 各ノードの深さを収める配列
  /// @return 出力の最大深さを返す．
  ymuint
  get_min_depth(ymuint k,
		vector<ymuint>& depth_array) const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name ノードの生成／変更
  /// @{

  /// @brief 入力ノードを作る．
  /// @return 作成したノードを返す．
  SbjNode*
  new_input();

  /// @brief 出力ノードを作る．
  /// @param[in] inode 入力のノード
  /// @param[in] inv 極性
  /// @return 作成したノードを返す．
  SbjNode*
  new_output(SbjNode* inode,
	     bool inv);

  /// @brief 論理ノードを作る．
  /// @param[in] fcode 機能コード
  /// @param[in] inode1 1番めの入力ノード
  /// @param[in] inode2 2番めの入力ノード
  /// @return 作成したノードを返す．
  SbjNode*
  new_logic(ymuint fcode,
	    SbjNode* inode1,
	    SbjNode* inode2);

  /// @brief ANDノードを作る．
  /// @param[in] inode1 1番めの入力ノード
  /// @param[in] inode2 2番めの入力ノード
  /// @param[in] inv1 1番めの枝の反転属性
  /// @param[in] inv2 2番めの枝の反転属性
  /// @return 作成したノードを返す．
  SbjNode*
  new_and(SbjNode* inode1,
	  SbjNode* inode2,
	  bool inv1,
	  bool inv2);

  /// @brief XORノードを作る．
  /// @param[in] inode1 1番めの入力ノード
  /// @param[in] inode2 2番めの入力ノード
  /// @return 作成したノードを返す．
  SbjNode*
  new_xor(SbjNode* inode1,
	  SbjNode* inode2);

  /// @brief DFFノードを作る．
  /// @param[in] inode 入力のノード
  /// @param[in] inv 極性
  /// @return 作成したノードを返す．
  SbjNode*
  new_dff(SbjNode* inode = NULL,
	  bool inv = false);

  /// @brief 出力ノードの内容を変更する
  /// @param[in] 変更対象の出力ノード
  /// @param[in] inode 入力のノード
  /// @param[in] inv 極性
  void
  change_output(SbjNode* node,
		SbjNode* inode,
		bool inv);

  /// @brief 論理ノードの内容を再設定する．
  /// @param[in] node 変更対象の論理ノード
  /// @param[in] fcode 機能コード
  /// @param[in] inode1 1番めの入力ノード
  /// @param[in] inode2 2番めの入力ノード
  void
  change_logic(SbjNode* node,
	       ymuint fcode,
	       SbjNode* inode1,
	       SbjNode* inode2);

  /// @brief ANDノードの内容を再設定する．
  /// @param[in] node 変更対象の論理ノード
  /// @param[in] inode1 1番めの入力ノード
  /// @param[in] inode2 2番めの入力ノード
  /// @param[in] inv1 1番めの枝の反転属性
  /// @param[in] inv2 2番めの枝の反転属性
  void
  change_and(SbjNode* node,
	     SbjNode* inode1,
	     SbjNode* inode2,
	     bool inv1,
	     bool inv2);

  /// @brief XORノードの内容を再設定する．
  /// @param[in] node 変更対象の論理ノード
  /// @param[in] inode1 1番めの入力ノード
  /// @param[in] inode2 2番めの入力ノード
  void
  change_xor(SbjNode* node,
	     SbjNode* inode1,
	     SbjNode* inode2);

  /// @brief DFFノードの内容を変更する
  /// @param[in] 変更対象のDFFノード
  /// @param[in] inode 入力のノード
  /// @param[in] inv 極性
  void
  set_dff_data(SbjNode* node,
	       SbjNode* inode,
	       bool inv);

  /// @brief DFFノードのクロック入力を設定する．
  /// @param[in] 変更対象のDFFノード
  /// @param[in] inode 入力のノード
  /// @param[in] inv 極性
  void
  set_dff_clock(SbjNode* node,
		SbjNode* inode,
		bool inv);

  /// @brief DFFノードのセット入力を設定する．
  /// @param[in] 変更対象のDFFノード
  /// @param[in] inode 入力のノード
  /// @param[in] inv 極性
  void
  set_dff_set(SbjNode* node,
	      SbjNode* inode,
	      bool inv);

  /// @brief DFFノードのリセット入力を設定する．
  /// @param[in] 変更対象のDFFノード
  /// @param[in] inode 入力のノード
  /// @param[in] inv 極性
  void
  set_dff_rst(SbjNode* node,
	      SbjNode* inode,
	      bool inv);

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name その他の関数
  /// @{

  /// @brief 空にする．
  void
  clear();

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // プライベートメンバ関数
  //////////////////////////////////////////////////////////////////////

  // 複製する．
  void
  copy(const SbjGraph& src,
       vector<SbjNode*>& nodemap);

  // 新しいノードを作成し mNodeList に登録する．
  // 作成されたノードを返す．
  SbjNode*
  new_node(ymuint ni);

  // 入力ノードの削除
  void
  delete_input(SbjNode* node);

  // 出力ノードの削除
  void
  delete_output(SbjNode* node);

  // 論理ノードの削除
  void
  delete_logic(SbjNode* node);

  // DFFノードの削除
  void
  delete_dff(SbjNode* node);

  // node を削除する．
  void
  delete_node(SbjNode* node,
	      ymuint ni);

  // from を to の pos 番目のファンインとする．
  // to の pos 番目にすでに接続があった場合には自動的に削除される．
  void
  connect(SbjNode* from,
	  SbjNode* to,
	  ymuint pos);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  /// @brief 入出力ノードに関係するポートの情報
  struct PortInfo
  {
    /// @brief ポート
    SbjPort* mPort;
    /// @brief ビット位置
    ymuint32 mPos;
  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノードを確保するためのアロケータ
  SimpleAlloc mAlloc;

  // SbjEdge の配列を確保するためのアロケータ
  FragAlloc mAlloc2;

  // 名前
  string mName;

  // ポートの配列
  vector<SbjPort*> mPortArray;

  // ID 番号をキーにしたノードの配列
  // すべてのノードが格納される．
  vector<SbjNode*> mNodeArray;

  // ID 番号を管理するためのオブジェクト
  ItvlMgr mItvlMgr;

  // 入力番号をキーにした入力ノードの配列
  // 穴はあいていない．
  vector<SbjNode*> mInputArray;

  // 入力番号をキーにしたポート情報の配列
  vector<PortInfo> mInputPortArray;

  // 入力ノードのリスト
  SbjNodeList mInputList;

  // 出力番号をキーにした出力ノードの配列
  // 穴はあいていない．
  vector<SbjNode*> mOutputArray;

  // 出力番号をキーにしたポート情報の配列
  vector<PortInfo> mOutputPortArray;

  // 出力ノードのリスト
  SbjNodeList mOutputList;

  // 論理ノードのリスト
  SbjNodeList mLnodeList;

  // DFFノードのリスト
  SbjNodeList mDffList;

  // 最大レベル
  mutable
  ymuint32 mLevel;

  // mLevel および各 SbjNode::mLevel が正しいとき true となるフラグ
  mutable
  bool mLevelValid;

};


//////////////////////////////////////////////////////////////////////
// inline 関数の定義
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// クラス SbjEdge
//////////////////////////////////////////////////////////////////////

// コンストラクタ
inline
SbjEdge::SbjEdge() :
  mFrom(NULL),
  mTo(NULL),
  mIpos(0U)
{
}

// デストラクタ
inline
SbjEdge::~SbjEdge()
{
}

// 入力側のノードを得る．
inline
const SbjNode*
SbjEdge::from() const
{
  return mFrom;
}

// 出力側のノードを得る．
inline
const SbjNode*
SbjEdge::to() const
{
  return mTo;
}

// 入力側のノードを得る．
inline
SbjNode*
SbjEdge::from()
{
  return mFrom;
}

// 出力側のノードを得る．
inline
SbjNode*
SbjEdge::to()
{
  return mTo;
}

// 出力側のノードの何番目の入力かを示す．
inline
ymuint
SbjEdge::pos() const
{
  return mIpos;
}

// @brief from ノードをセットする．
inline
void
SbjEdge::set_from(SbjNode* from)
{
  mFrom = from;
}

// @brief to ノードをセットする．
inline
void
SbjEdge::set_to(SbjNode* to,
		ymuint pos)
{
  mTo = to;
  mIpos = pos;
}

// @brief 出力ノードに接続している時 true を返す．
inline
bool
SbjEdge::is_poedge() const
{
  return to()->is_output();
}


//////////////////////////////////////////////////////////////////////
// クラス SbjNode
//////////////////////////////////////////////////////////////////////

// ID 番号を得る．
inline
ymuint
SbjNode::id() const
{
  return mId;
}

// タイプを入力に設定する．
inline
void
SbjNode::set_input(ymuint subid)
{
  mFlags = static_cast<ymuint>(kINPUT) | (subid << kSubidShift);
}

// タイプを出力に設定する．
inline
void
SbjNode::set_output(ymuint subid,
		    bool inv)
{
  mFlags = static_cast<ymuint>(kOUTPUT) |
    (subid << kSubidShift) |
    (inv << kOinvShift);
}

// タイプを論理に設定する．
inline
void
SbjNode::set_logic(ymuint fcode)
{
  mFlags = static_cast<ymuint>(kLOGIC) | (fcode << kFcodeShift);
}

// @brief タイプをDFFに設定する．
inline
void
SbjNode::set_dff(bool inv)
{
  mFlags = static_cast<ymuint>(kDFF) | (inv << kDDinvShift);
}

// @brief DFFノードのクロック極性を設定する．
inline
void
SbjNode::set_dff_clock(bool inv)
{
  mFlags &= ~(1UL << kDCinvShift);
  mFlags |= (static_cast<ymuint>(inv & 1U) << kDCinvShift);
}

// @brief DFFノードのセット極性を設定する．
inline
void
SbjNode::set_dff_set(bool inv)
{
  mFlags &= ~(1UL << kDSinvShift);
  mFlags |= (static_cast<ymuint>(inv & 1U) << kDSinvShift);
}

// @brief DFFノードのリセット極性を設定する．
inline
void
SbjNode::set_dff_rst(bool inv)
{
  mFlags &= ~(1UL << kDRinvShift);
  mFlags |= (static_cast<ymuint>(inv & 1U) << kDRinvShift);
}

// タイプを得る．
inline
SbjNode::tType
SbjNode::type() const
{
  return static_cast<tType>(mFlags & kTypeMask);
}

// 入力ノードの時に true を返す．
inline
bool
SbjNode::is_input() const
{
  return type() == kINPUT;
}

// 出力ノードの時に true を返す．
inline
bool
SbjNode::is_output() const
{
  return type() == kOUTPUT;
}

// 論理ノードの時に true を返す．
inline
bool
SbjNode::is_logic() const
{
  return type() == kLOGIC;
}

// @brief AND ノードの時に true を返す．
inline
bool
SbjNode::is_and() const
{
  // 注意! encoding が変わったら変更すること．
  return (mFlags & 0x13U) == 0x02U;
}

// @brief XOR ノードの時に true を返す．
inline
bool
SbjNode::is_xor() const
{
  // 注意! encoding が変わったら変更すること．
  return (mFlags & 0x13U) == 0x12U;
}

// DFFノードの時に true を返す．
inline
bool
SbjNode::is_dff() const
{
  return type() == kDFF;
}

// @brief 入力ノードか DFF ノードの時に true を返す．
inline
bool
SbjNode::is_ppi() const
{
  return is_input() || is_dff();
}

// @brief 出力ノードか DFF ノードの時に true を返す．
inline
bool
SbjNode::is_ppo() const
{
  return is_output() || is_dff();
}

// @brief サブID (入力／出力番号)を得る．
inline
ymuint
SbjNode::subid() const
{
  return mFlags >> kSubidShift;
}

// @brief 出力ノードの極性を得る．
inline
bool
SbjNode::output_inv() const
{
  return static_cast<bool>((mFlags >> kOinvShift) & 1U);
}

// @brief 機能コードを得る．
inline
ymuint
SbjNode::fcode() const
{
  return (mFlags >> kFcodeShift) & kFcodeMask;
}

// @brief ファンインのノードを得る．
inline
const SbjNode*
SbjNode::fanin(ymuint pos) const
{
  // pos の範囲チェックはしていない！！！
  return mFanins[pos].from();
}

// @brief ファンインのノードを得る．
inline
SbjNode*
SbjNode::fanin(ymuint pos)
{
  // pos の範囲チェックはしていない！！！
  return mFanins[pos].from();
}

// @brief ファンインの反転属性を得る．
// @param[in] pos 入力番号 (0 or 1)
inline
bool
SbjNode::fanin_inv(ymuint pos) const
{
  return static_cast<bool>((mFlags >> (pos + kFcodeShift)) & 1U);
}

// ファンインの枝を得る．
inline
const SbjEdge*
SbjNode::fanin_edge(ymuint pos) const
{
  // pos の範囲チェックはしていない！！！
  return &mFanins[pos];
}

// ファンインの枝を得る．
inline
SbjEdge*
SbjNode::fanin_edge(ymuint pos)
{
  // pos の範囲チェックはしていない！！！
  return &mFanins[pos];
}

// ファンアウトリストを得る．
inline
const SbjEdgeList&
SbjNode::fanout_list() const
{
  return mFanoutList;
}

// ファンアウト数を得る．
inline
ymuint
SbjNode::fanout_num() const
{
  return mFanoutList.size();
}

// @brief 出力ノードにファンアウトしているとき true を返す．
inline
bool
SbjNode::pomark() const
{
  return static_cast<bool>((mMark >> kPoShift) & 1U);
}

// @brief DFFノードの場合のデータ入力を得る．
inline
const SbjNode*
SbjNode::fanin_data() const
{
  return fanin(0);
}

// @brief DFFノードの場合のデータ入力を得る．
inline
SbjNode*
SbjNode::fanin_data()
{
  return fanin(0);
}

// @brief DFFノードの場合のデータ入力の反転属性を得る．
inline
bool
SbjNode::fanin_data_inv() const
{
  return static_cast<bool>((mFlags >> kDDinvShift) & 1U);
}

// @brief DFFノードの場合のクロック入力を得る．
inline
const SbjNode*
SbjNode::fanin_clock() const
{
  return fanin(1);
}

// @brief DFFノードの場合のクロック入力を得る．
inline
SbjNode*
SbjNode::fanin_clock()
{
  return fanin(1);
}

// @brief DFFノードの場合のクロック入力の反転属性を得る．
inline
bool
SbjNode::fanin_clock_inv() const
{
  return static_cast<bool>((mFlags >> kDCinvShift) & 1U);
}

// @brief DFFノードの場合のセット入力を得る．
inline
const SbjNode*
SbjNode::fanin_set() const
{
  return fanin(2);
}

// @brief DFFノードの場合のセット入力を得る．
inline
SbjNode*
SbjNode::fanin_set()
{
  return fanin(2);
}

// @brief DFFノードの場合のセット入力の反転属性を得る．
inline
bool
SbjNode::fanin_set_inv() const
{
  return static_cast<bool>((mFlags >> kDSinvShift) & 1U);
}

// @brief DFFノードの場合のリセット入力を得る．
inline
const SbjNode*
SbjNode::fanin_rst() const
{
  return fanin(3);
}

// @brief DFFノードの場合のリセット入力を得る．
inline
SbjNode*
SbjNode::fanin_rst()
{
  return fanin(3);
}

// @brief DFFノードの場合のリセット入力の反転属性を得る．
inline
bool
SbjNode::fanin_rst_inv() const
{
  return static_cast<bool>((mFlags >> kDRinvShift) & 1U);
}

// @brief レベルを得る．
inline
ymuint
SbjNode::level() const
{
  return mLevel;
}


//////////////////////////////////////////////////////////////////////
// クラス SbjPort
//////////////////////////////////////////////////////////////////////

// @brief 名前を得る．
inline
string
SbjPort::name() const
{
  return mName;
}

// @brief ビット数を得る．
inline
ymuint
SbjPort::bit_width() const
{
  return mBody.size();
}

// @brief pos ビット目の内容を得る．
// @param[in] pos ビット位置 ( 0 <= pos < bit_width() )
inline
const SbjNode*
SbjPort::bit(ymuint pos) const
{
  return mBody[pos];
}


//////////////////////////////////////////////////////////////////////
// クラス SbjGraph
//////////////////////////////////////////////////////////////////////

// @brief モジュール名を得る．
inline
string
SbjGraph::name() const
{
  return mName;
}

// @brief モジュール名を設定する．
inline
void
SbjGraph::set_name(const string& name)
{
  mName = name;
}

// @brief ポート数を得る．
inline
ymuint
SbjGraph::port_num() const
{
  return mPortArray.size();
}

// @brief ポートを得る．
// @param[in] pos 位置 ( 0 <= pos < port_num() )
inline
const SbjPort*
SbjGraph::port(ymuint pos) const
{
  return mPortArray[pos];
}

// @brief ポートを追加する(1ビット版)．
// @param[in] name ポート名
// @param[in] io_node 対応付ける入出力ノード
inline
void
SbjGraph::add_port(const string& name,
		   SbjNode* io_node)
{
  add_port(name, vector<SbjNode*>(1, io_node));
}

// ノード番号の最大値 + 1 を返す．
inline
ymuint
SbjGraph::max_node_id() const
{
  return mNodeArray.size();
}

// ID 番号が id のノードを取り出す．
// 該当するノードが無い場合には NULL を返す．
inline
const SbjNode*
SbjGraph::node(ymuint id) const
{
  return mNodeArray[id];
}

// 入力ノード数を得る．
inline
ymuint
SbjGraph::input_num() const
{
  return mInputArray.size();
}

// ID 番号が id の入力ノードを取り出す．
inline
SbjNode*
SbjGraph::input(ymuint id) const
{
  return mInputArray[id];
}

// @brief 入力ノードのリストを得る．
inline
const SbjNodeList&
SbjGraph::input_list() const
{
  return mInputList;
}

// 出力のノード数を得る．
inline
ymuint
SbjGraph::output_num() const
{
  return mOutputArray.size();
}

// ID 番号が id の出力ノードを取り出す．
inline
SbjNode*
SbjGraph::output(ymuint id) const
{
  return mOutputArray[id];
}

// @brief 入力ノードのリストを得る．
inline
const SbjNodeList&
SbjGraph::output_list() const
{
  return mOutputList;
}

// 論理ノード数を得る．
inline
ymuint
SbjGraph::lnode_num() const
{
  return mLnodeList.size();
}

// 論理ノードのリストを得る．
inline
const SbjNodeList&
SbjGraph::lnode_list() const
{
  return mLnodeList;
}

// DFFノード数を得る．
inline
ymuint
SbjGraph::dff_num() const
{
  return mDffList.size();
}

// DFFノードのリストを得る．
inline
const SbjNodeList&
SbjGraph::dff_list() const
{
  return mDffList;
}

END_NAMESPACE_YM_SBJ

#endif // YM_SBJ_SBJGRAPH_H
