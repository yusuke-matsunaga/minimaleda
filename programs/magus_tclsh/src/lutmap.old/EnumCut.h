#ifndef MAGUS_LUTMAP_ENUMCUT_H
#define MAGUS_LUTMAP_ENUMCUT_H

/// @file magus/lutmap/EnumCut.h 
/// @brief EnumCut のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EnumCut.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#include "lutmap.h"
#include "EnumCutOp.h"


BEGIN_NAMESPACE_MAGUS_LUTMAP


//////////////////////////////////////////////////////////////////////
/// @class EnumCut EnumCut.h "EnumCut.h"
/// @brief cut 列挙用のクラス
//////////////////////////////////////////////////////////////////////
class EnumCut
{
  class NodeTemp;
  
public:

  /// @brief コンストラクタ
  EnumCut();

  /// @brief デストラクタ
  virtual
  ~EnumCut();

  /// @brief 入力数が limit 以下のクラスタを列挙する．
  /// @param[in] sbjgraph 対象のサブジェクトグラフ
  /// @param[in] limit 入力数の制限
  /// @param[in] mode cut 列挙モード
  /// @param[in] op カットが列挙される時に呼ばれるクラス
  /// @return 全 cut 数を返す．
  ymuint
  operator()(const SbjGraph& sbjgraph,
	     ymuint limit,
	     tCutMode mode,
	     EnumCutOp* op);


private:
  //////////////////////////////////////////////////////////////////////
  // プライベートメンバ関数
  //////////////////////////////////////////////////////////////////////
  
  // cut の列挙を行なうための再帰関数
  bool
  enum_recur();

  // node のカットになったノードに c1mark を付け，mMarkedNodes に入れる．
  void
  mark_cnode(SbjNode* node);

  // node の TFI に c1mark を付ける．
  void
  mark_cnode2(SbjNode* node);

  // node のカットになったノードに c1mark を付け，marked_nodes に入れる．
  void
  mark_cnode3(SbjNode* node);

  // root_depth よりも小さな depth を持つノードを frontier stack に積む．
  void
  get_frontier(SbjNode* node,
	       ymuint root_depth);

  // get_frontier で付けた印を消す．
  void
  clear_frontier(SbjNode* node);

  // cmark の付いているノードを cnode_list に入れて
  // cmark を消す．
  void
  set_cut_node_list_recur(SbjNode* node,
			  vector<SbjNode*>& cnode_list);
  
  // frontier stack にノードをプッシュする
  void
  push_node(SbjNode* node);

  // frontier stack からノードをポップする
  SbjNode*
  pop_node();

  // frontier stack が空のとき true を返す．
  bool
  frontier_is_empty() const;

  const NodeTemp&
  node_temp(SbjNode* node) const;

  NodeTemp&
  node_temp(SbjNode* node);

  int
  state(SbjNode* node);

  void
  set_state1(SbjNode* node);

  void
  set_state2(SbjNode* node);

  void
  clear_state(SbjNode* node);

  bool
  cmark(SbjNode* node);

  void
  set_cmark(SbjNode* node);

  void
  clear_cmark(SbjNode* node);
  
  bool
  temp1mark(SbjNode* node);
  
  void
  set_temp1mark(SbjNode* node);

  bool
  temp2mark(SbjNode* node);
  
  void
  set_temp2mark(SbjNode* node);

  void
  clear_tempmark(SbjNode* node);

  bool
  edge_mark(SbjNode* node,
	    ymuint pos);

  void
  set_edge_mark(SbjNode* node,
		ymuint pos);

  void
  clear_edge_mark(SbjNode* node,
		  ymuint pos);
  
  vector<SbjNode*>&
  cnode_list(SbjNode* node);

  const vector<SbjNode*>&
  cnode_list(SbjNode* node) const;

  
private:

  // 各ノードごとの作業領域
  class NodeTemp
  {
  public:

    /// @brief コンストラクタ
    NodeTemp();

    /// @brief 状態を取り出す．
    /// - 0: 初期状態
    /// - 1: cut の内部に含まれている
    /// - 2: cut の入力になっている
    int
    state() const;

    /// @brief 内部に含まれた．
    void
    set_state1();

    /// @brief 入力となった．
    void
    set_state2();
  
    /// @brief 初期状態に戻す．
    void
    clear_state();
  
    /// @brief クラスタノードの印を返す．
    bool
    cmark() const;
  
    /// @brief クラスタノードの印をつける．
    void
    set_cmark();
  
    /// @brief クラスタノードの印を消す．
    void
    clear_cmark();
  
    /// @brief temporary 用のマーク1を取り出す．
    bool
    temp1mark() const;
    
    /// @brief temporary 用のマーク1をつける．
    void
    set_temp1mark();
    
    /// @brief temporary 用のマーク2を取り出す．
    bool
    temp2mark() const;
  
    /// @brief temporary 用のマーク2をつける．
    void
    set_temp2mark();
    
    /// @brief temporary 用のマーク1/2を消す．
    void
    clear_tempmark();

    /// @brief pos 番めの枝のマークを取り出す．
    /// @note pos < 20
    bool
    edge_mark(ymuint pos) const;

    /// @brief pos 番めの枝のマークを付ける．
    void
    set_edge_mark(ymuint pos);

    /// @brief pos 番めの枝のマークを消す．
    void
    clear_edge_mark(ymuint pos);
    
    /// @brief クラスタノードのリストを返す．
    vector<SbjNode*>&
    cnode_list();

    /// @brief クラスタノードのリストを返す．
    const vector<SbjNode*>&
    cnode_list() const;


  private:
    //////////////////////////////////////////////////////////////////////
    // データメンバ
    //////////////////////////////////////////////////////////////////////

    // このノードを根とする全カットのフットプリント
    vector<SbjNode*> mCnodeList;

    // 種々のマーク
    ymuint32 mMarks;

  };

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力数の最大値
  ymuint32 mLimit;

  // cut 列挙モード
  tCutMode mMode;

  // 全ノード数
  ymuint32 mNall;

  // 現在処理中のノード番号
  ymuint32 mCurPos;

  // 全 cut 数
  ymuint32 mNcAll;

  // 現在処理中のノードの cut 数
  ymuint32 mNcCur;
  
  // 根のノード
  SbjNode* mRoot;

  // mFrontierStack のサイズ
  ymuint32 mFsSize;

  // 最前線ノードのスタック
  SbjNode** mFrontierStack;

  // mFrontierStack の先頭を指すポインタ
  SbjNode** mFsPos;
  
  // 確定した境界ノードを入れるベクタ
  SbjNode** mInputs;

  // mInputs の次の書き込み位置
  ymuint32 mInputPos;

  // 内部ノードのスタック
  ymuint32* mInodeStack;

  // 内部ノードスタックのスタックポインタ
  ymuint32* mIsPos;

  // 各ノードごとの作業領域
  vector<NodeTemp> mNodeTemp;

  // マークの付いたノードを入れておく配列
  vector<SbjNode*> mMarkedNodes;

  // mMarkedNodes の末尾
  ymuint32 mMarkedNodesLast;
  
  // 現在の cut が対象外のとき正の値を持つカウンタ
  ymuint32 mNgCount;

  // カットが列挙されたときに呼ばれるクラス
  EnumCutOp* mOp;
  
};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// コンストラクタ
inline
EnumCut::NodeTemp::NodeTemp() :
  mMarks(0U)
{
}

// @brief 状態を取り出す．
// - 0: 初期状態
// - 1: cut の内部に含まれている
// - 2: cut の入力になっている
inline
int
EnumCut::NodeTemp::state() const
{
  return mMarks & 3U;
}

// @brief 内部に含まれた．
inline
void
EnumCut::NodeTemp::set_state1()
{
  mMarks &= ~3U;
  mMarks |= 1U;
}

// @brief 入力となった．
inline
void
EnumCut::NodeTemp::set_state2()
{
  mMarks &= ~3U;
  mMarks |= 2U;
}
  
// @brief 初期状態に戻す．
inline
void
EnumCut::NodeTemp::clear_state()
{
  mMarks &= ~3U;
}
  
// @brief クラスタノードの印を返す．
inline
bool
EnumCut::NodeTemp::cmark() const
{
  return static_cast<bool>((mMarks >> 2) & 1U);
}
  
// @brief クラスタノードの印をつける．
inline
void
EnumCut::NodeTemp::set_cmark()
{
  mMarks |= 4U;
}
  
// @brief クラスタノードの印を消す．
inline
void
EnumCut::NodeTemp::clear_cmark()
{
  mMarks &= ~4U;
}
  
// @brief temporary 用のマーク1を取り出す．
inline
bool
EnumCut::NodeTemp::temp1mark() const
{
  return static_cast<bool>((mMarks >> 3) & 1U);
}

// @brief temporary 用のマーク1をつける．
inline
void
EnumCut::NodeTemp::set_temp1mark()
{
  mMarks |= 8U;
}
    
// @brief temporary 用のマーク2を取り出す．
inline
bool
EnumCut::NodeTemp::temp2mark() const
{
  return static_cast<bool>((mMarks >> 4) & 1U);
}
  
// @brief temporary 用のマーク2をつける．
inline
void
EnumCut::NodeTemp::set_temp2mark()
{
  mMarks |= 16U;
}

// @brief temporary 用のマーク1/2を消す．
inline
void
EnumCut::NodeTemp::clear_tempmark()
{
  mMarks &= ~(8U | 16U);
}

// @brief pos 番めの枝のマークを取り出す．
// @note pos < 20
inline
bool
EnumCut::NodeTemp::edge_mark(ymuint pos) const
{
  return static_cast<bool>((mMarks >> (pos + 5)) & 1U);
}

// @brief pos 番めの枝のマークを付ける．
inline
void
EnumCut::NodeTemp::set_edge_mark(ymuint pos)
{
  mMarks |= (32U << pos);
}

// @brief pos 番めの枝のマークを消す．
inline
void
EnumCut::NodeTemp::clear_edge_mark(ymuint pos)
{
  mMarks &= ~(32U << pos);
}

// @brief クラスタノードのリストを返す．
inline
vector<SbjNode*>&
EnumCut::NodeTemp::cnode_list()
{
  return mCnodeList;
}

// @brief クラスタノードのリストを返す．
inline
const vector<SbjNode*>&
EnumCut::NodeTemp::cnode_list() const
{
  return mCnodeList;
}

inline
const EnumCut::NodeTemp&
EnumCut::node_temp(SbjNode* node) const
{
  return mNodeTemp[node->id()];
}

inline
EnumCut::NodeTemp&
EnumCut::node_temp(SbjNode* node)
{
  return mNodeTemp[node->id()];
}

inline
int
EnumCut::state(SbjNode* node)
{
  return node_temp(node).state();
}

inline
void
EnumCut::set_state1(SbjNode* node)
{
  node_temp(node).set_state1();
}

inline
void
EnumCut::set_state2(SbjNode* node)
{
  node_temp(node).set_state2();
}

inline
void
EnumCut::clear_state(SbjNode* node)
{
  node_temp(node).clear_state();
}

inline
bool
EnumCut::cmark(SbjNode* node)
{
  return node_temp(node).cmark();
}

inline
void
EnumCut::set_cmark(SbjNode* node)
{
  node_temp(node).set_cmark();
}

inline
void
EnumCut::clear_cmark(SbjNode* node)
{
  node_temp(node).clear_cmark();
}

inline
bool
EnumCut::temp1mark(SbjNode* node)
{
  return node_temp(node).temp1mark();
}

inline
void
EnumCut::set_temp1mark(SbjNode* node)
{
  node_temp(node).set_temp1mark();
}

inline
bool
EnumCut::temp2mark(SbjNode* node)
{
  return node_temp(node).temp2mark();
}

inline
void
EnumCut::set_temp2mark(SbjNode* node)
{
  node_temp(node).set_temp2mark();
}

inline
void
EnumCut::clear_tempmark(SbjNode* node)
{
  node_temp(node).clear_tempmark();
}

inline
bool
EnumCut::edge_mark(SbjNode* node,
		   ymuint pos)
{
  return node_temp(node).edge_mark(pos);
}

inline
void
EnumCut::set_edge_mark(SbjNode* node,
		       ymuint pos)
{
  node_temp(node).set_edge_mark(pos);
}

inline
void
EnumCut::clear_edge_mark(SbjNode* node,
			 ymuint pos)
{
  node_temp(node).clear_edge_mark(pos);
}

inline
vector<SbjNode*>&
EnumCut::cnode_list(SbjNode* node)
{
  return node_temp(node).cnode_list();
}

inline
const vector<SbjNode*>&
EnumCut::cnode_list(SbjNode* node) const
{
  return node_temp(node).cnode_list();
}

END_NAMESPACE_MAGUS_LUTMAP

#endif // MAGUS_LUTMAP_ENUMCUT_H
