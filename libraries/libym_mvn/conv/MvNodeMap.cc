
/// @file libym_mvn/conv/MvNodeMap.cc
/// @brief MvNodeMap の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/MvNodeMap.h"
#include "ym_sbj/SbjGraph.h"
#include "ym_mvn/MvMgr.h"
#include "ym_mvn/MvModule.h"
#include "ym_mvn/MvNode.h"


BEGIN_NAMESPACE_YM_MVN

// @brief コンストラクタ
MvNodeMap::MvNodeMap(ymuint n) :
  mArray(n)
{
}

// @brief デストラクタ
MvNodeMap::~MvNodeMap()
{
}

// @brief 登録する．(1ビット版)
// @param[in] mvnode MvNode
// @param[in] sbjnode SbjNode
// @param[in] inv 反転属性
void
MvNodeMap::put(const MvNode* mvnode,
	       SbjNode* sbjnode,
	       bool inv)
{
  put(mvnode, 0, sbjnode, inv);
}

// @brief 登録する．(ベクタ版)
// @param[in] mvnode MvNode
// @param[in] index ビット位置
// @param[in] sbjnode SbjNode
// @param[in] inv 反転属性
void
MvNodeMap::put(const MvNode* mvnode,
	       ymuint index,
	       SbjNode* sbjnode,
	       bool inv)
{
  assert_cond( mArray.size() > mvnode->id(), __FILE__, __LINE__);
  vector<ympuint>& array = mArray[mvnode->id()];
  if ( array.size() != mvnode->output(0)->bit_width() ) {
    array.resize(mvnode->output(0)->bit_width());
  }
  array[index] = reinterpret_cast<ympuint>(sbjnode) | (inv & 1U);
}

// @brief 探す．(1ビット版)
bool
MvNodeMap::get(const MvNode* mvnode,
	       SbjNode*& sbjnode,
	       bool& inv) const
{
  return get(mvnode, 0, sbjnode, inv);
}

// @brief 探す．(ベクタ版)
bool
MvNodeMap::get(const MvNode* mvnode,
	       ymuint index,
	       SbjNode*& sbjnode,
	       bool& inv) const
{
  assert_cond( mArray.size() > mvnode->id(), __FILE__, __LINE__);
  const vector<ympuint>& array = mArray[mvnode->id()];
  if ( array.empty() ) {
    return false;
  }
  assert_cond( array.size() == mvnode->output(0)->bit_width(),
	       __FILE__, __LINE__);
  ympuint tmp = array[index];
  sbjnode = reinterpret_cast<SbjNode*>(tmp & ~1UL);
  inv = static_cast<bool>(tmp & 1U);
  return true;
}


BEGIN_NONAMESPACE
void
dump_sbjnode(ostream& s,
	     SbjNode* sbjnode,
	     bool inv)
{
  if ( sbjnode ) {
    if ( inv ) {
      s << "~";
    }
    s << sbjnode->id_str();
  }
  else {
    if ( inv ) {
      s << "1";
    }
    else {
      s << "0";
    }
  }
}

END_NONAMESPACE

void
dump_mvnode_map(ostream& s,
		const MvMgr& mvmgr,
		const MvNodeMap& mvnode_map)
{
  ymuint n = mvmgr.max_node_id();
  for (ymuint i = 0; i < n; ++ i) {
    const MvNode* node = mvmgr.node(i);
    if ( node == NULL ) continue;
    const MvOutputPin* opin = node->output(0);
    ymuint bw = opin->bit_width();
    if ( bw == 1 ) {
      SbjNode* sbjnode;
      bool inv;
      bool stat = mvnode_map.get(node, sbjnode, inv);
      assert_cond( stat , __FILE__, __LINE__);
      s << "// node" << node->id() << " : ";
      dump_sbjnode(s, sbjnode, inv);
      s << endl;
    }
    else {
      for (ymuint i = 0; i < bw; ++ i) {
	SbjNode* sbjnode;
	bool inv;
	bool stat = mvnode_map.get(node, i, sbjnode, inv);
	assert_cond( stat , __FILE__, __LINE__);
	s << "// node" << node->id() << " [" << i << "] : ";
	dump_sbjnode(s, sbjnode, inv);
	s << endl;
      }
    }
  }
}

END_NAMESPACE_YM_MVN
