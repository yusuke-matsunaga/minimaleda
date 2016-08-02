
/// @file libym_aig/AigTemplate.cc
/// @brief AigTemplate の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: AigTemplate.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "AigTemplate.h"


BEGIN_NAMESPACE_YM_AIG

// コンストラクタ
AigTemplate::AigTemplate() :
  mLeafNum(0),
  mAndNum(0),
  mNodeArray(NULL)
{
}

// デストラクタ
AigTemplate::~AigTemplate()
{
  delete [] mNodeArray;
}
  
// @brief 初期化する．
// @param[in] leaf_num 入力数
// @param[in] and_num AND ノード数
inline
void
AigTemplate::init(ymuint leaf_num,
		  ymuint and_num)
{
  delete [] mNodeArray;
  mLeafNum = (leaf_num << 1);
  mAndNum = and_num;
  mNodeArray = new ymuint8[mAndNum];
}

// @brief AND ノードの内容をセットする．
// @param[in] pos ノード番号
// @param[in] fanin0 ファンイン0の情報
// @param[in] fanin1 ファンイン1の情報
void
AigTemplate::set_and(ymuint pos,
		     ymuint fanin0,
		     ymuint fanin1)
{
  if ( pos > leaf_num() && pos <= leaf_num() + and_num() ) {
    ymuint base = pos * 2;
    mNodeArray[base + 0] = fanin0;
    mNodeArray[base + 1] = fanin1;
  }
}
  
// @brief 出力の極性をセットする．
void
AigTemplate::set_opol(ymuint pol)
{
  if ( pol ) {
    mLeafNum |= 1U;
  }
  else {
    mLeafNum &= ~1U;
  }
}

#if 0
// AIG を作る．
AigHandle
AigTemplate::make_aig(AigManip& manip,
		      const vector<AigHandle>& inputs) const
{
  vector<AigHandle> node_map;
  node_map.reserve(mNodes.size());
  for (vector<AtNode*>::const_iterator p = mNodes.begin();
       p != mNodes.end(); ++ p) {
    AtNode* node = *p;
    AigHandle ans;
    if ( node->type() == kAtConst0 ) {
      ans = manip.make_zero();
    }
    else if ( node->type() == kAtConst1 ) {
      ans = manip.make_one();
    }
    else if ( node->type() == kAtLiteral ) {
      tNpnImap imap = mNpnMap.imap(node->input0());
      ans = inputs[npnimap_pos(imap)];
      if ( npnimap_pol(imap) == kPolNega ) {
	ans = ~ans;
      }
      if ( node->inv0() ) {
	ans = ~ans;
      }
    }
    else {
      // ここまで来たら mType は AND/OR/XOR のいずれか
      AigHandle input0 = node_map[node->input0()];
      if ( node->inv0() ) {
	input0 = ~input0;
      }
  
      AigHandle input1 = node_map[node->input1()];
      if ( node->inv1() ) {
	input1 = ~input1;
      }

      if ( node->type() == kAtAnd ) {
	ans = manip.make_and(input0, input1);
      }
      else if ( node->type() == kAtOr ) {
	ans = manip.make_or(input0, input1);
      }
      else if ( node->type() == kAtXor ) {
	ans = manip.make_xor(input0, input1);
      }
      else {
	assert_not_reached(__FILE__, __LINE__);
      }
    }
    node_map[node->id()] = ans;
  }
  AigHandle ans = node_map[mRoot->id()];
  if ( mNpnMap.opol() == kPolNega ) {
    ans = ~ans;
  }
  return ans;
}

// 内容をダンプする．
void
AigTemplate::dump(ostream& s) const
{
  s << "COST: " << cost() << endl;
  s << "ROOT: " << mRoot->id() << endl;
  s << "MAP:  " << mNpnMap << endl;
  for (vector<AtNode*>::const_iterator p = mNodes.begin();
       p != mNodes.end(); ++ p) {
    AtNode* node = *p;
    s << "  " << node->id() << ": ";
    switch ( node->type() ) {
    case kAtConst0:
      s << "Const0";
      break;

    case kAtConst1:
      s << "Const1";
      break;

    case kAtLiteral:
      s << "Literal(" << node->input0() << ", ";
      if ( node->inv0() ) {
	s << "N";
      }
      else {
	s << "P";
      }
      s << ")";
      break;

    case kAtAnd:
      s << "And(";
      if ( node->inv0() ) {
	s << "~";
      }
      s << node->input0();
      s << ", ";
      if ( node->inv1() ) {
	s << "~";
      }
      s << node->input1();
      s << ")";
      break;

    case kAtOr:
      s << "Or(";
      if ( node->inv0() ) {
	s << "~";
      }
      s << node->input0();
      s << ", ";
      if ( node->inv1() ) {
	s << "~";
      }
      s << node->input1();
      s << ")";
      break;

    case kAtXor:
      s << "Xor(";
      if ( node->inv0() ) {
	s << "~";
      }
      s << node->input0();
      s << ", ";
      if ( node->inv1() ) {
	s << "~";
      }
      s << node->input1();
      s << ")";
      break;
    }
    s << endl;
  }
}
#endif

END_NAMESPACE_YM_AIG
