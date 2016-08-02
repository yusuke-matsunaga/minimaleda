
/// @file libym_tgnet/TgNetwork.cc
/// @brief TgNetwork の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TgNetwork.cc 1920 2008-12-20 15:52:42Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_tgnet/TgNetwork.h"
#include "ym_tgnet/TgNode.h"
#include "NameHash.h"
#include "LogicMgr.h"


BEGIN_NAMESPACE_YM_TGNET

//////////////////////////////////////////////////////////////////////
// クラス TgNetwork
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
TgNetwork::TgNetwork() :
  mNodeAlloc(sizeof(TgNode), 1024),
  mFaninAlloc(4096),
  mEdgeAlloc(4096),
  mNameHash(new NameHash),
  mLogicMgr(new LogicMgr)
{
}

// @brief デストラクタ
TgNetwork::~TgNetwork()
{
  clear();
  delete mNameHash;
  delete mLogicMgr;
}

// @brief 名前からノードを検索する
// @param[in] name 検索対象の名前
// @param[in] create_flag true なら存在しない時に新規に生成する．
// @return name という名前のノードを返す．
const TgNode*
TgNetwork::find_node(const char* name) const
{
  TgNode* node = mNameHash->find_node(name);
  return node;
}

// @brief 内容をクリアする．
void
TgNetwork::clear()
{
  mNodeArray.clear();
  mInputArray.clear();
  mOutputArray.clear();
  mLogicArray.clear();
  mFFOutArray.clear();
  mFFInArray.clear();
  
  mNameHash->clear();
  mLogicMgr->clear();
  
  mNodeAlloc.destroy();
  mFaninAlloc.destroy();
  mEdgeAlloc.destroy();
}
  
// @brief node の論理式を取り出す．
LogExpr
TgNetwork::get_lexp(const TgNode* node) const
{
  TgGateTemplate gt_id(node->type(), node->ni());
  return mLogicMgr->get(gt_id);
}

// @brief 新しいノードを生成する．
TgNode*
TgNetwork::new_node()
{
  ymuint32 gid = mNodeArray.size();
  void* p = mNodeAlloc.get_memory(sizeof(TgNode));
  TgNode* node = new (p) TgNode(gid);
  mNodeArray.push_back(node);
  return node;
}

// @brief 名前からノードを検索する
// @param[in] name 検索対象の名前
// @param[in] create_flag true なら存在しない時に新規に生成する．
// @return name という名前のノードを返す．
TgNode*
TgNetwork::find_node(const char* name,
		     bool create_flag)
{
  TgNode* node = mNameHash->find_node(name);
  if ( !create_flag || node ) {
    return node;
  }
  node = new_node();
  const char* str = mNameHash->reg_node(name, node);
  node->mName = str;
  return node;
}

// @brief ノードを外部入力に設定する．
// @param[in] node 対象のノード
void
TgNetwork::set_to_input(TgNode* node)
{
  ymuint32 lid = mInputArray.size();
  node->set_type(lid, kTgInput, 0, NULL);
  mInputArray.push_back(node);
}

// @brief ノードを外部出力に設定する．
// @param[in] node 対象のノード
void
TgNetwork::set_to_output(TgNode* node)
{
  ymuint32 lid = mOutputArray.size();
  void* p = mFaninAlloc.get_memory(sizeof(TgNode*));
  node->set_type(lid, kTgOutput, 1, p);
  mOutputArray.push_back(node);
}

// @brief ノードを論理ノードに設定する．
// @param[in] node 対象のノード
// @param[in] gt_id 論理式ID
void
TgNetwork::set_to_logic(TgNode* node,
			TgGateTemplate gt_id)
{
  ymuint32 lid = mLogicArray.size();
  tTgGateType type = gt_id.type();
  size_t ni = gt_id.ni();
  void* p = mFaninAlloc.get_memory(sizeof(TgNode*) * ni);
  node->set_type(lid, type, ni, p);
  mLogicArray.push_back(node);
}

// @brief ノードを FF ノードに設定する．
// @param[in] ffin FF-in に設定するノード
// @param[in] ffout FF-out に設定するノード
void
TgNetwork::set_to_ff(TgNode* ffin,
		     TgNode* ffout)
{
  ymuint32 lid = mFFOutArray.size();
  void* p = mFaninAlloc.get_memory(sizeof(TgNode*));
  ffin->set_type(lid, kTgOutput, 1, p);
  ffin->mAltNode = ffout;
  mFFInArray.push_back(ffin);
  ffout->set_type(lid, kTgInput, 0, NULL);
  ffout->mAltNode = ffin;
  mFFOutArray.push_back(ffout);
}

// @brief ノード間の接続を行う．
// @param[in] from_node ファンイン元のノード
// @param[in] to_node ファンアウト先のノード
// @param[in] to_ipos ファンアウト先のノードの入力ピン番号
void
TgNetwork::connect(TgNode* from_node,
		   TgNode* to_node,
		   size_t to_ipos)
{
  to_node->mFanins[to_ipos] = from_node;
}

// @brief 論理式の登録を行う．
// @param[in] lexp 論理式
TgGateTemplate
TgNetwork::reg_lexp(const LogExpr& lexp)
{
  return mLogicMgr->reg_logic(lexp);
}


// @brief ネットワークの設定後の処理を行う．
void
TgNetwork::wrap_up()
{
  size_t n = node_num();
  size_t ni = input_num1();
  size_t no = output_num1();
  size_t nl = logic_num();
  size_t nq = ff_num();

  // ファンアウトリストの設定を行う．
  // まず，ファンアウト数を数える．
  for (size_t i = 0; i < nl; ++ i) {
    TgNode* node = mLogicArray[i];
    size_t ni = node->ni();
    for (size_t j = 0; j < ni; ++ j) {
      TgNode* inode = node->mFanins[j];
      ++ inode->mFanoutNum;
    }
  }
  for (size_t i = 0; i < no; ++ i) {
    TgNode* node = mOutputArray[i];
    TgNode* inode = node->mFanins[0];
    ++ inode->mFanoutNum;
  }
  for (size_t i = 0; i < nq; ++ i) {
    TgNode* node = mFFInArray[i];
    TgNode* inode = node->mFanins[0];
    ++ inode->mFanoutNum;
  }
  // ファンアウト数分の配列を確保する．
  for (size_t i = 0; i < n; ++ i) {
    TgNode* node = mNodeArray[i];
    size_t& nfo = node->mFanoutNum;
    if ( nfo > 0 ) {
      void* p = mEdgeAlloc.get_memory(sizeof(TgEdge) * nfo);
      node->mFanouts = new (p) TgEdge[nfo];
    }
    nfo = 0;
  }
  // ファンアウトの設定を行う．
  for (size_t i = 0; i < nl; ++ i) {
    TgNode* node = mLogicArray[i];
    size_t ni = node->ni();
    for (size_t j = 0; j < ni; ++ j) {
      TgNode* inode = node->mFanins[j];
      size_t& pos = inode->mFanoutNum;
      inode->mFanouts[pos].mTo = node;
      inode->mFanouts[pos].mIpos = j;
      ++ pos;
    }
  }
  for (size_t i = 0; i < no; ++ i) {
    TgNode* node = mOutputArray[i];
    TgNode* inode = node->mFanins[0];
    size_t& pos = inode->mFanoutNum;
    inode->mFanouts[pos].mTo = node;
    inode->mFanouts[pos].mIpos = 0;
    ++ pos;
  }
  for (size_t i = 0; i < nq; ++ i) {
    TgNode* node = mFFInArray[i];
    TgNode* inode = node->mFanins[0];
    size_t& pos = inode->mFanoutNum;
    inode->mFanouts[pos].mTo = node;
    inode->mFanouts[pos].mIpos = 0;
    ++ pos;
  }
  
  // ソーティングを行う．
  vector<bool> visited(n, false);
  vector<TgNode*> queue(n);
  size_t rpos = 0;
  size_t wpos = 0;
  mSortedArray.clear();
  mSortedArray.reserve(nl);
  for (size_t i = 0; i < ni; ++ i) {
    TgNode* node = mInputArray[i];
    visited[node->gid()] = true;
    queue[wpos] = node;
    ++ wpos;
  }
  for (size_t i = 0; i < nq; ++ i) {
    TgNode* node = mFFOutArray[i];
    visited[node->gid()] = true;
    queue[wpos] = node;
    ++ wpos;
  }
  while ( rpos < wpos ) {
    TgNode* node = queue[rpos];
    ++ rpos;
    if ( node->is_logic() ) {
      mSortedArray.push_back(node);
    }
    size_t nfo = node->fanout_num();
    for (size_t i = 0; i < nfo; ++ i) {
      TgNode* fonode = node->fanout_edge(i)->mTo;
      if ( !fonode->is_logic() ) continue;
      if ( visited[fonode->gid()] ) continue;
      size_t ni = fonode->ni();
      bool ok = true;
      for (size_t i = 0; i < ni; ++ i) {
	const TgNode* finode = fonode->fanin(i);
	if ( !visited[finode->gid()] ) {
	  ok = false;
	  break;
	}
      }
      if ( ok ) {
	visited[fonode->gid()] = true;
	queue[wpos] = fonode;
	++ wpos;
      }
    }
  }
  assert_cond(mSortedArray.size() == nl, __FILE__, __LINE__);
}


BEGIN_NONAMESPACE

// ノード番号を出力する．
void
dump_node(ostream& s,
	  const TgNode* node)
{
  s << node->gid();
  if ( node->is_input() ) {
    s << "[I" << node->lid() << "]";
  }
  else if ( node->is_output() ) {
    s << "[O" << node->lid() << "]";
  }
  else if ( node->is_logic() ) {
    s << "[L" << node->lid() << "]";
  }
  else if ( node->is_ffin() ) {
    s << "[D" << node->lid() << "]";
  }
  else if ( node->is_ffout() ) {
    s << "[Q" << node->lid() << "]";
  }
  else {
    assert_not_reached(__FILE__, __LINE__);
  }
}

END_NONAMESPACE

// @brief TgNetwork の内容をダンプする関数
void
dump(ostream& s,
     const TgNetwork& network)
{
  s << "=====<TgNework Dump>=====" << endl;

  s << endl
    << "Logic Nodes" << endl;
  size_t nlo = network.logic_num();
  for (size_t i = 0; i < nlo; ++ i) {
    const TgNode* node = network.logic(i);
    dump_node(s, node);
    s << ": ni = " << node->ni() << endl;
    if ( node->is_cplx_logic() ) {
      LogExpr tmp = network.get_lexp(node);
      s << "\t" << tmp << endl;
    }
    else {
      s << "\t" << node->type() << endl;
    }
  }

  s << endl
    << "Connections" << endl;
  size_t nn = network.node_num();
  for (size_t i = 0; i < nn; ++ i) {
    const TgNode* node = network.node(i);
    size_t ni = node->ni();
    for (size_t j = 0; j < ni; ++ j) {
      const TgNode* inode = node->fanin(j);
      dump_node(s, inode);
      s << " -->> "
	<< j << "@";
      dump_node(s, node);
      s << endl;
    }
  }
  s << "=====<TgNework Dump End>=====" << endl;
}


//////////////////////////////////////////////////////////////////////
// クラス TgNode
//////////////////////////////////////////////////////////////////////
  
// コンストラクタ
TgNode::TgNode(ymuint32 gid) :
  mTypeId(kTgUndef, 0),
  mGid(gid),
  mName(NULL),
  mFanins(NULL),
  mFanoutNum(0),
  mFanouts(NULL),
  mAltNode(NULL)
{
}

// デストラクタ
TgNode::~TgNode()
{
}

// タイプをセットする．
void
TgNode::set_type(ymuint32 lid,
		 tTgGateType type,
		 size_t ni,
		 void* p)
{
  mTypeId = TgGateTemplate(type, ni);;
  mLid = lid;
  if ( ni > 0 ) {
    mFanins = static_cast<TgNode**>(p);
  }
  else {
    mFanins = NULL;
  }
  for (size_t i = 0; i < ni; ++ i) {
    mFanins[i] = NULL;
  }
  mFanoutNum = 0;
}

END_NAMESPACE_YM_TGNET
