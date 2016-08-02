
/// @file libym_blif/BlifNetwork.cc
/// @brief BlifNetwork の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BlifNetworkReader.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_blif/BlifNetwork.h"


BEGIN_NAMESPACE_YM_BLIF

//////////////////////////////////////////////////////////////////////
// BlifNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID 番号
BlifNode::BlifNode(ymuint32 id) :
  mId(id),
  mType(kUndef),
  mNi(0),
  mFanins(NULL),
  mNc(0),
  mCover(NULL)
{
}

// @brief デストラクタ
BlifNode::~BlifNode()
{
}


//////////////////////////////////////////////////////////////////////
// BlifNetwork
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BlifNetwork::BlifNetwork() :
  mAlloc(4096)
{
}

// @brief デストラクタ
BlifNetwork::~BlifNetwork()
{
}

// @brief 内容をクリアする．
void
BlifNetwork::clear()
{
  mName = string();
  mNodeArray.clear();
  mPIArray.clear();
  mPOArray.clear();
  mFFArray.clear();
  mLogicArray.clear();
}
  
// @brief ノードを取り出す．
// @param[in] id ID 番号
// @note なければ作成する．
BlifNode*
BlifNetwork::get_node(ymuint32 id)
{
  ymuint32 n = mNodeArray.size();
  while ( n <= id ) {
    mNodeArray.push_back(NULL);
    ++ n;
  }
  
  BlifNode* node = mNodeArray[id];
  if ( node == NULL ) {
    void* p = mAlloc.get_memory(sizeof(BlifNode));
    node = new (p) BlifNode(id);
    mNodeArray[id] = node;
  }
  return node;
}
  
// @brief ノード名を設定する．
// @param[in] node 対象のノード
// @param[in] name 名前
void
BlifNetwork::set_node_name(BlifNode* node,
			   const char* name)
{
  ymuint32 l = strlen(name);
  void* p = mAlloc.get_memory(l + 1);
  char* buff = new (p) char[l + 1];
  node->mName = buff;
  while ( (*buff ++ = *name ++) ) ;
}

// @brief ノードを外部入力に設定する．
// @note 既に他の型に設定されていたら false を返す．
bool
BlifNetwork::set_input_type(BlifNode* node)
{
  if ( node->mType != BlifNode::kUndef ) {
    return false;
  }
  node->mType = BlifNode::kInput;
  mPIArray.push_back(node);
  return true;
}

// @brief ノードを論理ノードに設定する．
// @param[in] ni ファンイン数
// @param[in] nc キューブ数
// @param[in] cover_pat 全キューブのパタンをつなげたもの
// @param[in] opat 出力のパタン
// @note 既に他の型に設定されていたら false を返す．
bool
BlifNetwork::set_logic_type(BlifNode* node,
			    ymuint32 ni,
			    ymuint32 nc,
			    const char* cover_pat,
			    char opat)
{
  if ( node->mType != BlifNode::kUndef ) {
    return false;
  }
  node->mType = BlifNode::kLogic;
  alloc_fanin(node, ni);
  node->mNc = nc;
  
  if ( ni > 0 ) {
    ymuint32 size = nc * ni;
    void* p = mAlloc.get_memory(size);
    char* buff = new (p) char[size];
    node->mCover = buff;
    while ( size -- > 0 ) {
      *buff ++ = *cover_pat ++;
    }
  }
  else {
    node->mCover = NULL;
  }
  node->mOpat = opat;
  
  mLogicArray.push_back(node);
  return true;
}

// @brief ノードをラッチノードに設定する．
// @param[in] rval リセット値 ( '0', '1', ' ' のいづれか )
// @note 既に他の型に設定されていたら false を返す．
bool
BlifNetwork::set_latch_type(BlifNode* node,
			    char rval)
{
  if ( node->mType != BlifNode::kUndef ) {
    return false;
  }
  node->mType = BlifNode::kLatch;
  alloc_fanin(node, 1);

  node->mOpat = rval;
  
  mFFArray.push_back(node);
  return true;
}

// @brief BlifNode のファンイン領域を確保する．
// @param[in] node 対象のノード
// @param[in] ni ファンイン数
void
BlifNetwork::alloc_fanin(BlifNode* node,
			 ymuint32 ni)
{
  node->mNi = ni;
  void* q = mAlloc.get_memory(sizeof(BlifNode*) * ni);
  node->mFanins = new (q) BlifNode*[ni];
}

// @brief 内容を blif 形式で出力する．
// @param[in] s 出力先のストリーム
void
BlifNetwork::write_blif(ostream& s) const
{
  s << ".model " << name() << endl;
  for (vector<BlifNode*>::const_iterator p = mPIArray.begin();
       p != mPIArray.end(); ++ p) {
    const BlifNode* node = *p;
    s << ".inputs " << node->name() << endl;
  }
  for (vector<BlifNode*>::const_iterator p = mPOArray.begin();
       p != mPOArray.end(); ++ p) {
    const BlifNode* node = *p;
    s << ".outputs " << node->name() << endl;
  }
  for (vector<BlifNode*>::const_iterator p = mFFArray.begin();
       p != mFFArray.end(); ++ p) {
    const BlifNode* node = *p;
    const BlifNode* inode = node->fanin(0);
    s << ".latch " << inode->name() << " " << node->name();
    if ( node->opat() != ' ' ) {
      s << node->opat();
    }
    s << endl;
  }
  for (vector<BlifNode*>::const_iterator p = mLogicArray.begin();
       p != mLogicArray.end(); ++ p) {
    const BlifNode* node = *p;
    s << ".names";
    ymuint32 ni = node->ni();
    for (ymuint32 i = 0; i < ni; ++ i) {
      const BlifNode* inode = node->fanin(i);
      s << " " << inode->name();
    }
    s << " " << node->name() << endl;
    ymuint32 nc = node->nc();
    if ( ni > 0 ) {
      for (ymuint32 c = 0; c < nc; ++ c) {
	for (ymuint32 i = 0; i < ni; ++ i) {
	  s << node->cube_pat(c, i);
	}
	s << " " << node->opat() << endl;
      }
    }
    else if ( nc == 1 ) {
      s << node->opat();
    }
  }
  s << ".end" << endl;
}

END_NAMESPACE_YM_BLIF
