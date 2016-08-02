
/// @file libym_aig/AigNode.cc
/// @brief AigNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: AigNode.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_aig/AigNode.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
// クラス AigHandle
//////////////////////////////////////////////////////////////////////
  
// @brief ノードの通し番号を得る．
ymuint
AigHandle::node_id() const
{
  return node()->node_id();
}


/// @relates AigHandle
/// @brief 内容を出力する関数
ostream&
operator<<(ostream& s,
	   AigHandle src)
{
  bool inv = src.inv();
  AigNode* node = src.node();
  if ( node ) {
    if ( inv ) {
      s << "~";
    }
    s << node->node_id();
  }
  else {
    if ( inv ) {
      s << "CONST1";
    }
    else {
      s << "CONST0";
    }
  }
  return s;
}


//////////////////////////////////////////////////////////////////////
// AIG(And-Inverter Graph) のノードを表すクラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] input 外部入力のときに true とするフラグ
// @param[in] id ノード番号
AigNode::AigNode(bool input,
		 ymuint id) :
  mFlags(id << kSftN)
{
  if ( input ) {
    mFlags |= (1U << kSftT);
  }
}

// デストラクタ
AigNode::~AigNode()
{
}

END_NAMESPACE_YM_AIG
