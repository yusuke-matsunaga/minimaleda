
/// @file libym_lutmap/EnumCut.cc
/// @brief EnumCut の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EnumCut.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_sbj/SbjGraph.h"
#include "EnumCut.h"

//#define DEBUG_ENUM_RECUR


BEGIN_NAMESPACE_YM_LUTMAP

//////////////////////////////////////////////////////////////////////
// クラス EnumCut
//////////////////////////////////////////////////////////////////////

// コンストラクタ
EnumCut::EnumCut()
{
  mFsSize = 1024;
  mFrontierStack = new const SbjNode*[mFsSize];
  mFsPos = &mFrontierStack[0];
}

// デストラクタ
EnumCut::~EnumCut()
{
  delete [] mFrontierStack;
}

// 入力数が limit 以下のクラスタを列挙する．
ymuint
EnumCut::operator()(const SbjGraph& sbjgraph,
		    ymuint limit,
		    EnumCutOp* op)
{
  ymuint n = sbjgraph.max_node_id();
  mNodeTemp.clear();
  mNodeTemp.resize(n);

  mMarkedNodes.resize(n);

  mLimit = limit;
  mOp = op;

  mOp->all_init(sbjgraph, limit);

  mInputs = new const SbjNode*[limit];

  mInodeStack = new ymuint32[sbjgraph.lnode_num()];
  mIsPos = &mInodeStack[0];

  // 外部入力用の(ダミーの)クラスタを作る．
  ymuint ni = sbjgraph.input_num();
  ymuint nf = sbjgraph.dff_num();
  mNall = ni + nf + sbjgraph.lnode_num();
  mNcAll = 0;
  mCurPos = 0;
  for (ymuint i = 0; i < ni; ++ i) {
    const SbjNode* node = sbjgraph.input(i);

    mNcCur = 0;

    mOp->node_init(node, mCurPos);

    // 自分自身のみからなるクラスタを登録する．
    mOp->found(node);
    ++ mNcAll;
    ++ mNcCur;

    // 今の列挙で用いたノードを cut_node_list に格納しておく
    cnode_list(node).push_back(node);

    mOp->node_end(node, mCurPos, mNcCur);

    ++ mCurPos;
  }
  const SbjNodeList& dff_list = sbjgraph.dff_list();
  for (SbjNodeList::const_iterator p = dff_list.begin();
       p != dff_list.end(); ++ p) {
    const SbjNode* node = *p;

    mNcCur = 0;

    mOp->node_init(node, mCurPos);

    // 自分自身のみからなるクラスタを登録する．
    mOp->found(node);
    ++ mNcAll;
    ++ mNcCur;

    // 今の列挙で用いたノードを cut_node_list に格納しておく
    cnode_list(node).push_back(node);

    mOp->node_end(node, mCurPos, mNcCur);

    ++ mCurPos;
  }

  // 入力側から内部ノード用のクラスタを作る．
  vector<const SbjNode*> node_list;
  sbjgraph.sort(node_list);
  for (vector<const SbjNode*>::const_iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    const SbjNode* node = *p;
    assert_cond( node->is_logic(), __FILE__, __LINE__);

    mMarkedNodesLast = 0;

    for (ymuint i = 0; i < 2; ++ i) {
      // ファンインの cut に含まれるノードに c1mark をつける．
      const SbjNode* inode = node->fanin(i);
      mark_cnode(inode);
    }

    // 自分に c1mark がついており，ファンインには c1mark がついていない
    // ノードに c2mark をつける．
    // c2mark のついたノードが境界ノードとなる．
    for (ymuint i = 0; i < mMarkedNodesLast; ++ i) {
      const SbjNode* node = mMarkedNodes[i];
      if ( temp1mark(node) ) {
	if ( node->is_logic() ) {
	  for (ymuint i = 0; i < 2; ++ i) {
	    const SbjNode* inode = node->fanin(i);
	    if ( !temp1mark(inode) ) {
	      set_temp2mark(node);
	      break;
	    }
	  }
	}
	else { // is_ppi() == true
	  set_temp2mark(node);
	}
      }
    }

    // クラスタの列挙を行う．
    // ただし c2mark にぶつかったらそれ以上，入力側にはいかない．
    mNcCur = 0;

    mOp->node_init(node, mCurPos);

    mInputPos = 0;
    mRoot = node;

    push_node(node);
    enum_recur();
    pop_node();
    clear_state(node);
    assert_cond(frontier_is_empty(), __FILE__, __LINE__);
    assert_cond(mInputPos == 0, __FILE__, __LINE__);

    // 今の列挙で用いたノードを cut_node_list に格納しておく
    vector<const SbjNode*>& clist = cnode_list(node);
    clist.reserve(mMarkedNodesLast);
    set_cut_node_list_recur(node, clist);

    mOp->node_end(node, mCurPos, mNcCur);

    ++ mCurPos;

    // マークを消しておく
    for (ymuint i = 0; i < mMarkedNodesLast; ++ i) {
      const SbjNode* node = mMarkedNodes[i];
      clear_tempmark(node);
    }
  }

  mOp->all_end(sbjgraph, limit);

  delete [] mInputs;
  delete [] mInodeStack;

  return mNcAll;
}

// node のカットになったノードに c1mark を付け，mMarkedNodes に入れる．
void
EnumCut::mark_cnode(const SbjNode* node)
{
  const vector<const SbjNode*>& cnode_list1 = cnode_list(node);
  for (vector<const SbjNode*>::const_iterator p = cnode_list1.begin();
       p != cnode_list1.end(); ++ p) {
    const SbjNode* node = *p;
    if ( !temp1mark(node) ) {
      set_temp1mark(node);
      mMarkedNodes[mMarkedNodesLast] = node;
      ++ mMarkedNodesLast;
    }
  }
}

// node の TFI に c1mark を付ける．
void
EnumCut::mark_cnode2(const SbjNode* node)
{
  if ( !temp1mark(node) ) {
    set_temp1mark(node);
    mMarkedNodes[mMarkedNodesLast] = node;
    ++ mMarkedNodesLast;
    if ( node->is_logic() ) {
      mark_cnode2(node->fanin(0));
      mark_cnode2(node->fanin(1));
    }
  }
}

// node のカットになったノードに c1mark を付け，marked_nodes に入れる．
void
EnumCut::mark_cnode3(const SbjNode* node)
{
  const vector<const SbjNode*>& cnode_list1 = cnode_list(node);
  for (vector<const SbjNode*>::const_iterator p = cnode_list1.begin();
       p != cnode_list1.end(); ++ p) {
    const SbjNode* node = *p;
    if ( !temp1mark(node) ) {
      set_temp1mark(node);
      mMarkedNodes[mMarkedNodesLast] = node;
      ++ mMarkedNodesLast;
      if ( node->fanout_num() > 1 ) {
	set_temp2mark(node);
      }
    }
  }
}

#if 0
// root_depth よりも小さな depth を持つノードを frontier stack に積む．
void
EnumCut::get_frontier(const SbjNode* node,
		      ymuint root_depth)
{
  if ( state(node) ) {
    return;
  }
  if ( node->depth() < root_depth ) {
    if ( temp2mark(node) ) {
      // 境界ノードなので入力とする．
      assert_cond(mInputPos < mLimit, __FILE__, __LINE__);
      set_state2(node);
      mInputs[mInputPos] = node;
      ++ mInputPos;
    }
    else {
      // スタックに積む．
      push_node(node);
    }
  }
  else {
    // node に内部ノードの印をつけて子供のノードに再帰する．
    set_state1(node);
    set_cmark(node);
    get_frontier(node->fanin(0), root_depth);
    get_frontier(node->fanin(1), root_depth);
  }
}

// get_frontier でつけた印を消す．
void
EnumCut::clear_frontier(const SbjNode* node)
{
  if ( state(node) ) {
    clear_state(node);
    if ( node->is_logic() ) {
      clear_frontier(node->fanin(0));
      clear_frontier(node->fanin(1));
    }
  }
}
#endif

// frontier stack にノードをプッシュする
inline
void
EnumCut::push_node(const SbjNode* node)
{
  if ( mFsPos == mFrontierStack + mFsSize ) {
    const SbjNode** old_stack = mFrontierStack;
    mFsSize <<= 1;
    mFrontierStack = new const SbjNode*[mFsSize];
    const SbjNode** sp = old_stack;
    const SbjNode** dp = mFrontierStack;
    for ( ; dp != mFsPos; ++ sp, ++ dp) {
      *dp = *sp;
    }
    delete [] old_stack;
  }
  *mFsPos = node;
  ++ mFsPos;
  set_state1(node);
}

// frontier stack からノードをポップする
inline
const SbjNode*
EnumCut::pop_node()
{
  -- mFsPos;
  return *mFsPos;
}

// frontier stack が空のとき true を返す．
inline
bool
EnumCut::frontier_is_empty() const
{
  return mFsPos == mFrontierStack;
}

bool
EnumCut::enum_recur()
{
  if ( frontier_is_empty() ) {
#if defined(DEBUG_ENUM_RECUR)
    cout << "FRONTIER IS EMPTY" << endl;
#endif

    for (ymuint i = 0; i < mInputPos; ++ i) {
      set_cmark(mInputs[i]);
    }
    if ( mInputPos > 1 ) {
      mOp->found(mRoot, mInputPos, mInputs);
    }
    else {
      mOp->found(mRoot);
    }
    ++ mNcAll;
    ++ mNcCur;
    return true;
  }
  else {
    const SbjNode* node = pop_node();

#if defined(DEBUG_ENUM_RECUR)
    cout << "POP[1] " << node->id_str() << endl;
#endif

    // 速度に効くのでコメントアウトしている
    //assert_cond(node->bmark() == 1, __FILE__, __LINE__);
    //assert_cond(node->is_logic(), __FILE__, __LINE__);

    bool has_cuts = false;

    if ( mInputPos < mLimit ) {
      // node を入力にして再帰を続ける．
      set_state2(node);
      mInputs[mInputPos] = node;
      ++ mInputPos;

#if defined(DEBUG_ENUM_RECUR)
      cout << "MARK[1] " << node->id_str() << endl;
#endif

      has_cuts = enum_recur();
      set_state1(node);
      -- mInputPos;

#if defined(DEBUG_ENUM_RECUR)
      cout << "UNMARK[1] " << node->id_str() << endl;
#endif
    }

    const SbjNode** old_fs_pos = mFsPos;
    ymuint old_input_pos = mInputPos;
    bool recur = true;
    bool inode0_stat = false;
    const SbjNode* inode0 = node->fanin(0);
    if ( state(inode0) == 0 ) {
      if ( !temp2mark(inode0) ) {
	push_node(inode0);
	inode0_stat = true;
	set_edge_mark(node, 0);

#if defined(DEBUG_ENUM_RECUR)
	cout << "PUSH[2] " << inode0->id_str() << endl;
#endif
      }
      else if ( mInputPos < mLimit ) {
	set_state2(inode0);
	mInputs[mInputPos] = inode0;
	++ mInputPos;
	inode0_stat = true;
	set_edge_mark(node, 0);

#if defined(DEBUG_ENUM_RECUR)
	cout << "MARK[2] " << inode0->id_str() << endl;
#endif
      }
      else {
	recur = false;
      }
    }
    if ( recur ) {
      bool inode1_stat = false;
      const SbjNode* inode1 = node->fanin(1);
      if ( state(inode1) == 0 ) {
	if ( !temp2mark(inode1) ) {
	  push_node(inode1);
	  inode1_stat = true;
	  set_edge_mark(node, 1);

#if defined(DEBUG_ENUM_RECUR)
	  cout << "PUSH[3] " << inode1->id_str() << endl;
#endif
	}
	else if ( mInputPos < mLimit ) {
	  set_state2(inode1);
	  mInputs[mInputPos] = inode1;
	  ++ mInputPos;
	  inode1_stat = true;
	  set_edge_mark(node, 1);

#if defined(DEBUG_ENUM_RECUR)
	  cout << "MARK[3] " << inode0->id_str() << endl;
#endif
	}
	else {
	  recur = false;
	}
      }
      if ( recur ) {
	// node を内部ノードにして再帰を続ける．
	bool has_cuts1 = enum_recur();
	if ( has_cuts1 ) {
	  set_cmark(node);
	  has_cuts = true;
	}
      }
      if ( inode1_stat ) {
	clear_state(inode1);
	clear_edge_mark(node, 1);

#if defined(DEBUG_ENUM_RECUR)
	cout << "UNMARK[3] " << inode1->id_str() << endl;
#endif
      }
    }
    if ( inode0_stat ) {
      clear_state(inode0);
      clear_edge_mark(node, 0);

#if defined(DEBUG_ENUM_RECUR)
      cout << "UNMARK[2] " << inode0->id_str() << endl;
#endif
    }
    mFsPos = old_fs_pos;
    mInputPos = old_input_pos;

    push_node(node);

#if defined(DEBUG_ENUM_RECUR)
    cout << "PUSH[4] " << node->id_str() << endl;
#endif

    return has_cuts;
  }
}

// cmark の付いているノードを cnode_list に入れて cmark を消す．
void
EnumCut::set_cut_node_list_recur(const SbjNode* node,
				 vector<const SbjNode*>& cnode_list)
{
  if ( !cmark(node) ) {
    return;
  }
  clear_cmark(node);
  cnode_list.push_back(node);
  if ( node->is_logic() ) {
    set_cut_node_list_recur(node->fanin(0), cnode_list);
    set_cut_node_list_recur(node->fanin(1), cnode_list);
  }
}

END_NAMESPACE_YM_LUTMAP
