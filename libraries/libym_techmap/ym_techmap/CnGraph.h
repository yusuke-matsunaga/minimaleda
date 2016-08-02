#ifndef YM_TECHMAP_CNGRAPH_H
#define YM_TECHMAP_CNGRAPH_H

/// @file ym_techmap/CnGraph.h
/// @brief LutNetwork のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: LutNetwork.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_techmap/techmap_nsdef.h"
#include "ym_cell/cell_nsdef.h"

#include "ym_utils/Alloc.h"
#include "ym_utils/DlList.h"
#include "ym_utils/ItvlMgr.h"


BEGIN_NAMESPACE_YM_TECHMAP

//////////////////////////////////////////////////////////////////////
/// @class CnEdge CnGraph.h "CnGraph.h"
/// @brief CELLネットワークの枝を表すクラス
///
/// - 入力側のノード
/// - 出力側のノード
/// - 出力側のノードの何番目の入力か
/// の情報を持つ．
/// @sa CnNode CnGraph
//////////////////////////////////////////////////////////////////////
class CnEdge :
  public DlElem
{
  friend class CnNode;
  friend class CnGraph;

public:

  //////////////////////////////////////////////////////////////////////
  /// @name 関数処理に関する情報にアクセスする関数
  /// @{

  /// @brief 入力側のノードを得る．(const 版)
  /// @retval 入力側のノード 通常の枝の場合
  /// @retval NULL 定数0に接続している枝の場合
  const CnNode*
  from() const;

  /// @brief 出力側のノードを得る．(const 版)
  /// @return 出力側のノードを返す．
  const CnNode*
  to() const;

  /// @brief 入力側のノードを得る．
  /// @retval 入力側のノード 通常の枝の場合
  /// @retval NULL 定数0に接続している枝の場合
  CnNode*
  from();

  /// @brief 出力側のノードを得る．
  /// @return 出力側のノードを返す．
  CnNode*
  to();

  /// @brief 出力側のノードの何番目の入力かを示す．
  ymuint
  pos() const;

  /// @brief 出力ノードに接続している時 true を返す．
  bool
  is_poedge() const;

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // プライベートメンバ関数
  // これらは friend class からしか呼ぶことができない．
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  CnEdge();

  /// @brief デストラクタ
  ~CnEdge();

  /// @brief from ノードをセットする．
  void
  set_from(CnNode* from);

  /// @brief to ノードをセットする．
  void
  set_to(CnNode* to,
	 ymuint pos);


private:
  //////////////////////////////////////////////////////////////////////
  // プライベートメンバ変数
  //////////////////////////////////////////////////////////////////////

  // 入力側のノード
  CnNode* mFrom;

  // 出力側のノード
  CnNode* mTo;

  // 入力位置
  ymuint32 mIpos;

};


//////////////////////////////////////////////////////////////////////
/// @class CnNode CnGraph.h "CnGraph.h"
/// @brief セルネットワークを構成するセルノードを表すクラス
///
/// ノードの種類は
/// - 入力ノード
/// - 出力ノード
/// - セルノード
/// - DFFノード
/// の 4種類がある．
/// @sa CnEdge CnGraph
//////////////////////////////////////////////////////////////////////
class CnNode :
  public DlElem
{
  friend class CnGraph;

public:
  /// @brief ノードの型
  enum tType {
    /// @brief 入力ノード
    kINPUT  = 0,
    /// @brief 出力ノード
    kOUTPUT = 1,
    /// @brief セルノード
    kCELL = 2,
    /// @brief DFFノード
    kDFF = 3
  };


private:

  /// @brief コンストラクタ
  CnNode();

  /// @brief デストラクタ
  ~CnNode();


public:

  //////////////////////////////////////////////////////////////////////
  /// @name 構造に関する情報を取り出す関数
  /// @{

  /// @brief ID 番号の取得
  /// @return ID 番号を返す．
  /// @note ID 番号はノードの生成時に CnGraph により自動的に割り振られる．
  /// @sa CnGraph
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

  /// @brief セルノードの時に true を返す．
  bool
  is_cellnode() const;

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

  /// @brief 入力数を得る．
  ymuint
  ni() const;

  /// @brief ファンインのノードを得る．
  /// @param[in] pos 入力番号
  /// @return pos 番めのファンインのノード
  /// @note 該当するファンインがなければ NULL を返す．
  const CnNode*
  fanin(ymuint pos) const;

  /// @brief ファンインのノードを得る．
  /// @param[in] pos 入力番号
  /// @return pos 番めのファンインのノード
  /// @note 該当するファンインがなければ NULL を返す．
  CnNode*
  fanin(ymuint pos);

  /// @brief ファンインの枝を得る．(const 版)
  /// @param[in] pos 入力番号
  /// @return pos 番目の入力の枝
  /// @note 該当するファンインの枝がなければ NULL を返す．
  const CnEdge*
  fanin_edge(ymuint pos) const;

  /// @brief ファンインの枝を得る．
  /// @param[in] pos 入力番号
  /// @return pos 番目の入力の枝
  /// @note 該当するファンインの枝がなければ NULL を返す．
  CnEdge*
  fanin_edge(ymuint pos);

  /// @brief ファンアウト数を得る．
  ymuint
  fanout_num() const;

  /// @brief ファンアウトリストを得る．
  const CnEdgeList&
  fanout_list() const;

  /// @brief 出力ノードにファンアウトしているとき true を返す．
  bool
  pomark() const;

  /// @brief CELLノードの場合にセルを得る．
  const Cell*
  cell() const;

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // プライベートメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief タイプを入力に設定する．
  /// @param[in] subid 入力番号
  void
  set_input(ymuint subid);

  /// @brief タイプを出力に設定する．
  /// @param[in] subid 出力番号
  void
  set_output(ymuint subid);

  /// @brief タイプをCELLに設定する．
  void
  set_cell(const Cell* cell);

  /// @brief タイプをDFFに設定する．
  void
  set_dff();

  /// @brief ファンアウトに出力が含まれているか調べ pomark をセットする．
  void
  scan_po();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID 番号
  ymuint32 mId;

  // タイプ (+ 入力／出力番号/PoMask)
  ymuint32 mFlags;

  // 入力数
  ymuint32 mNi;

  // ファンインの枝(そのもの)の配列
  CnEdge* mFanins;

  // ファンアウトの枝のリスト
  CnEdgeList mFanoutList;

  // セル
  const Cell* mCell;


private:
  //////////////////////////////////////////////////////////////////////
  // mFlags 用の定数
  //////////////////////////////////////////////////////////////////////

  static
  const int kTypeShift  = 0;
  static
  const int kPoShift    = 3;
  static
  const int kSubidShift = 4;

  static
  const ymuint32 kTypeMask  = 3U << kTypeShift;
  static
  const ymuint32 kPoMask    = 1U << kPoShift;

};


//////////////////////////////////////////////////////////////////////
/// @class CnPort CnGraph.h "CnGraph.h"
/// @brief ポートを表すクラス
//////////////////////////////////////////////////////////////////////
class CnPort
{
  friend class CnGraph;

private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] name 名前
  /// @param[in] io_node_vec 対応する入出力ノードのベクタ
  CnPort(const string& name,
	 const vector<CnNode*>& io_node_vec);

  /// @brief デストラクタ
  ~CnPort();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を得る．
  string
  name() const;

  /// @brief ビット数を得る．
  ymuint
  bit_width() const;

  /// @brief pos ビット目の内容を得る．
  /// @param[in] pos ビット位置 ( 0 <= pos < bit_width() )
  const CnNode*
  bit(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  string mName;

  // 入出力ノードの配列
  vector<CnNode*> mBody;

};


//////////////////////////////////////////////////////////////////////
/// @class CnGraph CnGraph.h "CnGraph.h"
/// @brief セルネットワークを表すクラス
///
/// 入力ノード，出力ノード，セルノード，DFF ノードを分けてリストで持つ
/// ( @sa input_list(), output_list(), cellnode_list(), dff_list() )．
/// また，すべてのノードに唯一な ID 番号を割り振っており，その ID 番号
/// からノードを取り出すこともできる( @sa node(ymuint id) )．
/// 入力ノード，出力ノードは ID 番号とは別に入力 ID 番号，および出力
/// ID 番号を持っており，それらの ID 番号からノードを取り出すこともできる．
/// ( @sa input(ymuint id), output(ymuint id) )
/// セルノードを入力からのトポロジカル順で処理するためには sort()
/// を用いてソートされたノードのベクタを得る．
/// @sa CnEdge CnNode
//////////////////////////////////////////////////////////////////////
class CnGraph
{
public:

  /// @brief コンストラクタ
  CnGraph();

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  CnGraph(const CnGraph& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 自分自身への参照を返す．
  const CnGraph&
  operator=(const CnGraph& src);

  /// @brief デストラクタ
  ~CnGraph();


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
  /// @param[in] pos ポート位置 ( 0 <= pos < port_num() )
  const CnPort*
  port(ymuint pos) const;

  /// @brief 入出力ノードに関連づけられたポートを得る．
  /// @param[in] node 入出力ノード
  const CnPort*
  port(const CnNode* node) const;

  /// @brief 入出力ノードのポートにおけるビット位置を得る．
  /// @param[in] node 入出力ノード
  ymuint
  port_pos(const CnNode* node) const;

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
  /// @param[in] io_node 対応する入出力ノード
  void
  add_port(const string& name,
	   CnNode* io_node);

  /// @brief ポートを追加する(ベクタ版)．
  /// @param[in] name ポート名
  /// @param[in] io_node_vec 対応する入出力ノードのベクタ
  void
  add_port(const string& name,
	   const vector<CnNode*>& io_node_vec);

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name ノード関連の情報の取得
  /// @{

  /// @brief ノードIDの最大値 + 1 の取得
  /// @return ノードIDの最大値 + 1 を返す．
  /// @note ノードIDは間が抜けている場合がある．
  ymuint
  max_node_id() const;

  /// @brief ID 番号によるノードの取得
  /// @param[in] id ID 番号
  /// @return ID 番号が id のノードを返す．
  /// @retrun 該当するノードが無い場合には NULL を返す．
  const CnNode*
  node(ymuint id) const;

  /// @brief 入力ノード数の取得
  /// @return 入力ノード数を返す．
  ymuint
  input_num() const;

  /// @brief 入力 ID 番号による入力ノードの取得
  /// @param[in] id 入力 ID 番号
  /// @return 入力 ID 番号が id のノードを返す．
  const CnNode*
  input(ymuint id) const;

  /// @brief 入力ノードのリストを得る．
  const CnNodeList&
  input_list() const;

  /// @brief 入力ノードと DFF ノードのリストを得る．
  /// @param[out] node_list ノードを格納するリスト
  /// @return 要素数を返す．
  ymuint
  ppi_list(list<CnNode*>& node_list) const;

  /// @brief 出力のノード数を得る．
  ymuint
  output_num() const;

  /// @brief 出力 ID 番号による出力ノードの取得
  /// @param[in] id 出力 ID 番号
  /// @return 出力 ID 番号が id のノードを返す．
  const CnNode*
  output(ymuint id) const;

  /// @brief 出力ノードのリストを得る．
  const CnNodeList&
  output_list() const;

  /// @brief 出力ノードと DFF ノードのリストを得る．
  /// @param[out] node_list ノードを格納するリスト
  /// @return 要素数を返す．
  ymuint
  ppo_list(list<CnNode*>& node_list) const;

  /// @brief セルノード数を得る．
  ymuint
  cellnode_num() const;

  /// @brief セルノードのリストを得る．
  const CnNodeList&
  cellnode_list() const;

  /// @brief ソートされたセルノードのリストを得る．
  /// @param[out] node_list
  void
  sort(vector<CnNode*>& node_list) const;

  /// @brief DFFノード数を得る．
  ymuint
  dff_num() const;

  /// @brief DFFノードのリストを得る．
  const CnNodeList&
  dff_list() const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name ノードの生成／変更
  /// @{

  /// @brief 空にする．
  void
  clear();

  /// @brief 入力ノードを作る．
  /// @return 作成したノードを返す．
  CnNode*
  new_input();

  /// @brief 出力ノードを作る．
  /// @param[in] inode 入力のノード
  /// @return 作成したノードを返す．
  CnNode*
  new_output(CnNode* inode);

  /// @brief セルノードを作る．
  /// @param[in] inodes 入力ノードのベクタ
  /// @param[in] cell セル
  /// @return 作成したノードを返す．
  CnNode*
  new_cellnode(const vector<CnNode*>& inodes,
	       const Cell* cell);

  /// @brief DFFノードを作る．
  /// @return 作成したノードを返す．
  CnNode*
  new_dff();

  /// @brief DFFノードのデータ入力を設定する．
  /// @param[in] node 対象の DFF ノード
  /// @param[in] inode 入力のノード
  void
  set_dff_input(CnNode* node,
		CnNode* inode);

  /// @brief DFFノードのクロック入力を設定する．
  /// @param[in] node 対象の DFF ノード
  /// @param[in] inode 入力のノード
  void
  set_dff_clock(CnNode* node,
		CnNode* inode);

  /// @brief DFFノードのセット入力を設定する．
  /// @param[in] node 対象の DFF ノード
  /// @param[in] inode 入力のノード
  void
  set_dff_set(CnNode* node,
	      CnNode* inode);

  /// @brief DFFノードのリセット入力を設定する．
  /// @param[in] node 対象の DFF ノード
  /// @param[in] inode 入力のノード
  void
  set_dff_rst(CnNode* node,
	      CnNode* inode);

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // プライベートメンバ関数
  //////////////////////////////////////////////////////////////////////

  // 複製する．
  void
  copy(const CnGraph& src,
       vector<CnNode*>& nodemap);

  // 新しいノードを作成し mNodeList に登録する．
  // 作成されたノードを返す．
  CnNode*
  new_node(ymuint ni);

  /// @brief 入力ノードの削除
  /// @param[in] node 対象のノード
  void
  delete_input(CnNode* node);

  /// @brief 出力ノードの削除
  /// @param[in] node 対象のノード
  void
  delete_output(CnNode* node);

  /// @brief セルノードを削除する．
  /// @param[in] node 対象のノード
  /// @note node のファンアウトは空でなければならない．
  void
  delete_cellnode(CnNode* node);

  /// @brief DFFノードを削除する．
  /// @param[in] node 対象のノード
  /// @note node のファンアウトは空でなければならない．
  void
  delete_dff(CnNode* node);

  // node を削除する．
  void
  delete_node(CnNode* node);

  // from を to の pos 番目のファンインとする．
  // to の pos 番目にすでに接続があった場合には自動的に削除される．
  void
  connect(CnNode* from,
	  CnNode* to,
	  ymuint pos);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  /// @brief 入出力ノードに関係するポートの情報
  struct PortInfo
  {
    /// @brief ポート
    CnPort* mPort;
    /// @brief ビット位置
    ymuint32 mPos;
  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノードを確保するためのアロケータ
  SimpleAlloc mAlloc;

  // ノードのファンインの枝の配列を確保するためのアロケータ
  FragAlloc mAlloc2;

  // モジュール名
  string mName;

  // ポートの配列
  vector<CnPort*> mPortArray;

  // ID 番号をキーにしたノードの配列
  // すべてのノードが格納される．
  vector<CnNode*> mNodeArray;

  // ID 番号を管理するためのオブジェクト
  ItvlMgr mItvlMgr;

  // 入力番号をキーにした入力ノードの配列
  // 穴はあいていない．
  vector<CnNode*> mInputArray;

  // 入力番号をキーにしたポート情報の配列
  vector<PortInfo> mInputPortArray;

  // 入力ノードのリスト
  CnNodeList mInputList;

  // 出力番号をキーにした出力ノードの配列
  // 穴はあいていない．
  vector<CnNode*> mOutputArray;

  // 出力番号をキーにしたポート情報の配列
  vector<PortInfo> mOutputPortArray;

  // 出力ノードのリスト
  CnNodeList mOutputList;

  // セルノードのリスト
  CnNodeList mCellnodeList;

  // DFFノードのリスト
  CnNodeList mDffList;

};

/// @relates CnGraph
/// @brief 内容を s に出力する．
/// @param[in] s 出力先のストリーム
/// @param[in] lngraph 対象のネットワーク
void
dump(ostream& s,
     const CnGraph& lngraph);

/// @relates CnGraph
/// @brief 内容をシミュレーション可能な Verilog-HDL 形式で出力する．
/// @param[in] s 出力先のストリーム
/// @param[in] cngraph 対象のネットワーク
void
dump_verilog(ostream& s,
	     const CnGraph& cngraph);


//////////////////////////////////////////////////////////////////////
// inline 関数の定義
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// クラス CnEdge
//////////////////////////////////////////////////////////////////////

// コンストラクタ
inline
CnEdge::CnEdge() :
  mFrom(NULL),
  mTo(NULL),
  mIpos(0)
{
}

// デストラクタ
inline
CnEdge::~CnEdge()
{
}

// 入力側のノードを得る．
inline
const CnNode*
CnEdge::from() const
{
  return mFrom;
}

// 出力側のノードを得る．
inline
const CnNode*
CnEdge::to() const
{
  return mTo;
}

// 入力側のノードを得る．
inline
CnNode*
CnEdge::from()
{
  return mFrom;
}

// 出力側のノードを得る．
inline
CnNode*
CnEdge::to()
{
  return mTo;
}

// 出力側のノードの何番目の入力かを示す．
inline
ymuint
CnEdge::pos() const
{
  return mIpos;
}

// @brief from ノードをセットする．
inline
void
CnEdge::set_from(CnNode* from)
{
  mFrom = from;
}

// @brief to ノードをセットする．
inline
void
CnEdge::set_to(CnNode* to,
	       ymuint pos)
{
  mTo = to;
  mIpos = pos;
}

// @brief 出力ノードに接続している時 true を返す．
inline
bool
CnEdge::is_poedge() const
{
  return to()->is_output();
}


//////////////////////////////////////////////////////////////////////
// クラス CnNode
//////////////////////////////////////////////////////////////////////

// ID 番号を得る．
inline
ymuint
CnNode::id() const
{
  return mId;
}

// タイプを得る．
inline
CnNode::tType
CnNode::type() const
{
  return static_cast<tType>((mFlags & kTypeMask) >> kTypeShift);
}

// 入力ノードの時に true を返す．
inline
bool
CnNode::is_input() const
{
  return type() == kINPUT;
}

// 出力ノードの時に true を返す．
inline
bool
CnNode::is_output() const
{
  return type() == kOUTPUT;
}

// セルノードの時に true を返す．
inline
bool
CnNode::is_cellnode() const
{
  return type() == kCELL;
}

// @brief DFFノードの時に true を返す．
inline
bool
CnNode::is_dff() const
{
  return type() == kDFF;
}

// @brief 入力ノードか DFF ノードの時に true を返す．
inline
bool
CnNode::is_ppi() const
{
  return type() == kINPUT || type() == kDFF;
}

// @brief 出力ノードか DFF ノードの時に true を返す．
inline
bool
CnNode::is_ppo() const
{
  return type() == kOUTPUT || type() == kDFF;
}

// @brief サブID (入力／出力番号)を得る．
inline
ymuint
CnNode::subid() const
{
  return (mFlags >> kSubidShift);
}

// @brief 入力数を得る．
inline
ymuint
CnNode::ni() const
{
  return mNi;
}

// @brief ファンインのノードを得る．
inline
const CnNode*
CnNode::fanin(ymuint pos) const
{
  // pos の範囲チェックはしていない！！！
  return mFanins[pos].from();
}

// @brief ファンインのノードを得る．
inline
CnNode*
CnNode::fanin(ymuint pos)
{
  // pos の範囲チェックはしていない！！！
  return mFanins[pos].from();
}

// ファンインの枝を得る．
inline
const CnEdge*
CnNode::fanin_edge(ymuint pos) const
{
  // pos の範囲チェックはしていない！！！
  return &mFanins[pos];
}

// ファンインの枝を得る．
inline
CnEdge*
CnNode::fanin_edge(ymuint pos)
{
  // pos の範囲チェックはしていない！！！
  return &mFanins[pos];
}

// ファンアウトリストを得る．
inline
const CnEdgeList&
CnNode::fanout_list() const
{
  return mFanoutList;
}

// ファンアウト数を得る．
inline
ymuint
CnNode::fanout_num() const
{
  return mFanoutList.size();
}

// @brief 出力ノードにファンアウトしているとき true を返す．
inline
bool
CnNode::pomark() const
{
  return static_cast<bool>((mFlags >> kPoShift) & 1U);
}

// @brief CELLノードの場合にセルを得る．
inline
const Cell*
CnNode::cell() const
{
  return mCell;
}

// タイプを入力に設定する．
inline
void
CnNode::set_input(ymuint subid)
{
  mFlags = static_cast<ymuint>(kINPUT) | (subid << kSubidShift);
}

// タイプを出力に設定する．
inline
void
CnNode::set_output(ymuint subid)
{
  mFlags = static_cast<ymuint>(kOUTPUT) | (subid << kSubidShift);
}

// タイプをセルに設定する．
inline
void
CnNode::set_cell(const Cell* cell)
{
  mFlags = static_cast<ymuint>(kCELL);
  mCell = cell;
}

// @brief タイプをDFFに設定する．
inline
void
CnNode::set_dff()
{
  mFlags = static_cast<ymuint>(kDFF);
}


//////////////////////////////////////////////////////////////////////
// クラス CnPort
//////////////////////////////////////////////////////////////////////

// @brief 名前を得る．
inline
string
CnPort::name() const
{
  return mName;
}

// @brief ビット数を得る．
inline
ymuint
CnPort::bit_width() const
{
  return mBody.size();
}

// @brief pos ビット目の内容を得る．
// @param[in] pos ビット位置 ( 0 <= pos < bit_width() )
inline
const CnNode*
CnPort::bit(ymuint pos) const
{
  return mBody[pos];
}


//////////////////////////////////////////////////////////////////////
// クラス CnGraph
//////////////////////////////////////////////////////////////////////

// @brief モジュール名を得る．
inline
string
CnGraph::name() const
{
  return mName;
}

// @brief ポート数を得る．
inline
ymuint
CnGraph::port_num() const
{
  return mPortArray.size();
}

// @brief ポートを得る．
// @param[in] pos ポート位置 ( 0 <= pos < port_num() )
inline
const CnPort*
CnGraph::port(ymuint pos) const
{
  return mPortArray[pos];
}

// @brief ポートを追加する(1ビット版)．
// @param[in] name ポート名
// @param[in] io_node 対応する入出力ノード
inline
void
CnGraph::add_port(const string& name,
		  CnNode* io_node)
{
  add_port(name, vector<CnNode*>(1, io_node));
}

// ノード番号の最大値 + 1 を返す．
inline
ymuint
CnGraph::max_node_id() const
{
  return mNodeArray.size();
}

// ID 番号が id のノードを取り出す．
// 該当するノードが無い場合には NULL を返す．
inline
const CnNode*
CnGraph::node(ymuint id) const
{
  return mNodeArray[id];
}

// 入力ノード数を得る．
inline
ymuint
CnGraph::input_num() const
{
  return mInputArray.size();
}

// ID 番号が id の入力ノードを取り出す．
inline
const CnNode*
CnGraph::input(ymuint id) const
{
  return mInputArray[id];
}

// @brief 入力ノードのリストを得る．
inline
const CnNodeList&
CnGraph::input_list() const
{
  return mInputList;
}

// 出力のノード数を得る．
inline
ymuint
CnGraph::output_num() const
{
  return mOutputArray.size();
}

// ID 番号が id の出力ノードを取り出す．
inline
const CnNode*
CnGraph::output(ymuint id) const
{
  return mOutputArray[id];
}

// @brief 入力ノードのリストを得る．
inline
const CnNodeList&
CnGraph::output_list() const
{
  return mOutputList;
}

// セルノード数を得る．
inline
ymuint
CnGraph::cellnode_num() const
{
  return mCellnodeList.size();
}

// セルノードのリストを得る．
inline
const CnNodeList&
CnGraph::cellnode_list() const
{
  return mCellnodeList;
}

// @brief DFFノード数を得る．
inline
ymuint
CnGraph::dff_num() const
{
  return mDffList.size();
}

// @brief DFFノードのリストを得る．
inline
const CnNodeList&
CnGraph::dff_list() const
{
  return mDffList;
}

END_NAMESPACE_YM_TECHMAP

#endif // YM_TECHMAP_LUTNETWORK_H
