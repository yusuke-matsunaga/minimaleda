
/// @file libym_aig/tests/AigTemplate.cc
/// @brief AigTemplate の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: AigTemplate.cc 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "AigTemplate.h"
#include "ym_aig/AigMgr.h"


BEGIN_NAMESPACE_YM

// コンストラクタ
AigTemplate::AigTemplate()
{
}
  
// コピーコンストラクタ
AigTemplate::AigTemplate(const AigTemplate& src)
{
  set_copy(src, hash_map<size_t, size_t>());
}
  
// コピーコンストラクタ
AigTemplate::AigTemplate(const AigTemplate& src,
			 const hash_map<size_t, size_t>& vmap)
{
  set_copy(src, vmap);
}

// 代入演算子
const AigTemplate&
AigTemplate::operator=(const AigTemplate& src)
{
  if ( this != &src ) {
    set_copy(src, hash_map<size_t, size_t>());
  }
  return *this;
}

// デストラクタ
AigTemplate::~AigTemplate()
{
  clear();
}

// 内容を空にする．
void
AigTemplate::clear()
{
  for (vector<AtNode*>::iterator p = mNodes.begin();
       p != mNodes.end(); ++ p) {
    delete *p;
  }
  mNodes.clear();
  mRoot = NULL;
}

// AIG を作る．
AigHandle
AigTemplate::make_aig(AigMgr& mgr,
		      const vector<AigHandle>& inputs)
{
  return make_aig_sub(mRoot, mgr, inputs);
}
  
// make_aig() の下請け関数
AigHandle
AigTemplate::make_aig_sub(AtNode* node,
			  AigMgr& mgr,
			  const vector<AigHandle>& inputs)
{
  if ( node->mType == kAtConst0 ) {
    return mgr.make_zero();
  }
  if ( node->mType == kAtConst1 ) {
    return mgr.make_one();
  }
  if ( node->mType == kAtLiteral ) {
    AigHandle ans = inputs[node->mId];
    if ( node->mInv1 ) {
      ans = ~ans;
    }
    return ans;
  }
  
  // ここまで来たら mType は AND/OR/XOR のいずれか
  AigHandle input0 = make_aig_sub(node->mInputs[0], mgr, inputs);
  if ( node->mInv1 ) {
    input0 = ~input0;
  }
  
  AigHandle input1 = make_aig_sub(node->mInputs[1], mgr, inputs);
  if ( node->mInv2 ) {
    input1 = ~input1;
  }

  if ( node->mType == kAtAnd ) {
    return mgr.make_and(input0, input1);
  }
  if ( node->mType == kAtOr ) {
    return mgr.make_or(input0, input1);
  }
  if ( node->mType == kAtXor ) {
    return mgr.make_xor(input0, input1);
  }
  
  assert_not_reached(__FILE__, __LINE__);
  return mgr.make_zero();
}
  
// 内容がセットされていたら true を返す．
bool
AigTemplate::set() const
{
  return !mNodes.empty();
}
    
// コストを返す．
int
AigTemplate::cost() const
{
  int ans = 0;
  for (vector<AtNode*>::const_iterator p = mNodes.begin();
       p != mNodes.end(); ++ p) {
    AtNode* node = *p;
    if ( node->mType == kAtAnd || node->mType == kAtOr ) {
      ans += 1;
    }
    else if ( node->mType == kAtXor ) {
      ans += 3;
    }
  }
  return ans;
}
  
// 値の評価を行う．
ymulong
AigTemplate::eval(const vector<ymulong>& ivals) const
{
  for (vector<AtNode*>::const_iterator p = mNodes.begin();
       p != mNodes.end(); ++ p) {
    AtNode* node = *p;
    ymulong ival0;
    ymulong ival1;
    switch ( node->mType ) {
    case kAtConst0:
      node->mVal = 0UL;
      break;
      
    case kAtConst1:
      node->mVal = 1UL;
      break;

    case kAtLiteral:
      if ( node->mInv1 ) {
	node->mVal = ~ivals[node->mId];
      }
      else {
	node->mVal = ivals[node->mId];
      }
      break;

    case kAtAnd:
      ival0 = node->mInputs[0]->mVal;
      if ( node->mInv1 ) {
	ival0 = ~ival0;
      }
      ival1 = node->mInputs[1]->mVal;
      if ( node->mInv2 ) {
	ival1 = ~ival1;
      }
      node->mVal = ival0 & ival1;
      break;

    case kAtOr:
      ival0 = node->mInputs[0]->mVal;
      if ( node->mInv1 ) {
	ival0 = ~ival0;
      }
      ival1 = node->mInputs[1]->mVal;
      if ( node->mInv2 ) {
	ival1 = ~ival1;
      }
      node->mVal = ival0 | ival1;
      break;

    case kAtXor:
      ival0 = node->mInputs[0]->mVal;
      if ( node->mInv1 ) {
	ival0 = ~ival0;
      }
      ival1 = node->mInputs[1]->mVal;
      if ( node->mInv2 ) {
	ival1 = ~ival1;
      }
      node->mVal = ival0 ^ ival1;
      break;
    }
  }
  return mRoot->mVal;
}

// 自分の複製を生成するコードを出力する．
void
AigTemplate::dump_code(ostream& s,
		       const string& target) const
{
  s << "  {" << endl;
  for (vector<AtNode*>::const_iterator p = mNodes.begin();
       p != mNodes.end(); ++ p) {
    AtNode* node = *p;
    ostringstream buf;
    buf << "node" << node->mPos;
    string var_name = buf.str();
    switch ( node->mType ) {
    case kAtConst0:
      s << "    size_t " << var_name << " = " << target
	<< ".make_const0();" << endl;
      break;
      
    case kAtConst1:
      s << "    size_t " << var_name << " = " << target
	<< ".make_const1();" << endl;
      break;

    case kAtLiteral:
      s << "    size_t " << var_name << " = " << target
	<< ".make_literal(" << node->mId << ", ";
      if ( node->mInv1 ) {
	s << "true";
      }
      else {
	s << "false";
      }
      s << ");" << endl;
      break;

    case kAtAnd:
      s << "    size_t " << var_name << " = " << target
	<< ".make_and(node" << node->mInputs[0]->mPos << ", ";
      if ( node->mInv1 ) {
	s << "true";
      }
      else {
	s << "false";
      }
      s << ", "
	<< "node" << node->mInputs[1]->mPos << ", ";
      if ( node->mInv2 ) {
	s << "true";
      }
      else {
	s << "false";
      }
      s << ");" << endl;
      break;

    case kAtOr:
      s << "    size_t " << var_name << " = " << target
	<< ".make_or(node" << node->mInputs[0]->mPos << ", ";
      if ( node->mInv1 ) {
	s << "true";
      }
      else {
	s << "false";
      }
      s << ", "
	<< "node" << node->mInputs[1]->mPos << ", ";
      if ( node->mInv2 ) {
	s << "true";
      }
      else {
	s << "false";
      }
      s << ");" << endl;
      break;

    case kAtXor:
      s << "    size_t " << var_name << " = " << target
	<< ".make_xor(node" << node->mInputs[0]->mPos << ", ";
      if ( node->mInv1 ) {
	s << "true";
      }
      else {
	s << "false";
      }
      s << ", "
	<< "node" << node->mInputs[1]->mPos << ", ";
      if ( node->mInv2 ) {
	s << "true";
      }
      else {
	s << "false";
      }
      s << ");" << endl;
      break;
    }
  }
  s << "    " << target << ".set_root(node" << mRoot->mPos << ");" << endl;
  s << "  }" << endl;
}

// 内容をダンプする．
void
AigTemplate::dump(ostream& s) const
{
  s << "COST: " << cost() << endl;
  s << "ROOT: " << mRoot->mPos << endl;
  for (vector<AtNode*>::const_iterator p = mNodes.begin();
       p != mNodes.end(); ++ p) {
    AtNode* node = *p;
    s << "  " << node->mPos << ": ";
    switch ( node->mType ) {
    case kAtConst0:
      s << "Const0";
      break;
    case kAtConst1:
      s << "Const1";
      break;
    case kAtLiteral:
      s << "Literal(" << node->mId << ", ";
      if ( node->mInv1 ) {
	s << "N";
      }
      else {
	s << "P";
      }
      s << ")";
      break;
    case kAtAnd:
      s << "And(";
      if ( node->mInv1 ) {
	s << "~";
      }
      s << node->mInputs[0]->mPos;
      s << ", ";
      if ( node->mInv2 ) {
	s << "~";
      }
      s << node->mInputs[1]->mPos;
      s << ")";
      break;
    case kAtOr:
      s << "Or(";
      if ( node->mInv1 ) {
	s << "~";
      }
      s << node->mInputs[0]->mPos;
      s << ", ";
      if ( node->mInv2 ) {
	s << "~";
      }
      s << node->mInputs[1]->mPos;
      s << ")";
      break;
    case kAtXor:
      s << "Xor(";
      if ( node->mInv1 ) {
	s << "~";
      }
      s << node->mInputs[0]->mPos;
      s << ", ";
      if ( node->mInv2 ) {
	s << "~";
      }
      s << node->mInputs[1]->mPos;
      s << ")";
      break;
    }
    s << endl;
  }
}

// 定数0にセットする．
void
AigTemplate::set_const0()
{
  clear();
  AtNode* node = get_cnode(kAtConst0);
  mRoot = node;
}

// 定数1にセットする．
void
AigTemplate::set_const1()
{
  clear();
  AtNode* node = get_cnode(kAtConst1);
  mRoot = node;
}

// リテラルにセットする．
void
AigTemplate::set_literal(tVarId id,
			 bool inv)
{
  clear();
  AtNode* node = get_lnode(id, inv);
  mRoot = node;
}

// and にセットする．
void
AigTemplate::set_and(const AigTemplate& input0,
		     bool inv0,
		     const AigTemplate& input1,
		     bool inv1)
{
  clear();
  AtNode* node1 = dup_node(input0, hash_map<size_t, size_t>());
  AtNode* node2 = dup_node(input1, hash_map<size_t, size_t>());
  AtNode* node3 = get_anode(kAtAnd, node1, inv0, node2, inv1);
  mRoot = node3;
}

// or にセットする．
void
AigTemplate::set_or(const AigTemplate& input0,
		    bool inv0,
		    const AigTemplate& input1,
		    bool inv1)
{
  clear();
  AtNode* node1 = dup_node(input0, hash_map<size_t, size_t>());
  AtNode* node2 = dup_node(input1, hash_map<size_t, size_t>());
  AtNode* node3 = get_anode(kAtOr, node1, inv0, node2, inv1);
  mRoot = node3;
}

// xor にセットする．
void
AigTemplate::set_xor(const AigTemplate& input0,
		     bool inv0,
		     const AigTemplate& input1,
		     bool inv1)
{
  clear();
  AtNode* node1 = dup_node(input0, hash_map<size_t, size_t>());
  AtNode* node2 = dup_node(input1, hash_map<size_t, size_t>());
  AtNode* node3 = get_anode(kAtXor, node1, inv0, node2, inv1);
  mRoot = node3;
}

// 変数の付け替えを行ってコピーする．
void
AigTemplate::set_copy(const AigTemplate& src,
		      const hash_map<size_t, size_t>& vmap)
{
  clear();
  mRoot = dup_node(src, vmap);
}

// 変数の付け替えを行ってコピーする．
void
AigTemplate::set_copy(const AigTemplate& src,
		      const NpnMap& vmap)
{
  clear();
  mRoot = set_copy_sub(src.mRoot, vmap);
}
  
// 双対形をセットする．
void
AigTemplate::set_dual(const AigTemplate& src)
{
  clear();
  mRoot = set_dual_sub(src.mRoot);
}
  
// compose を行う．
void
AigTemplate::set_compose(const AigTemplate& src1,
			 const vector<AigTemplate>& tmap)
{
  clear();
  vector<AtNode*> nmap;
  for (vector<AigTemplate>::const_iterator p = tmap.begin();
       p != tmap.end(); ++ p) {
    const AigTemplate& templ = *p;
    AtNode* cnode = dup_node(templ, hash_map<size_t, size_t>());
    nmap.push_back(cnode);
  }
  mRoot = set_compose_sub(src1.mRoot, nmap);
}


// src の複製を作る．
AtNode*
AigTemplate::dup_node(const AigTemplate& src,
		      const hash_map<size_t, size_t>& vmap)
{
  return dup_node_sub(src.mRoot, vmap);
}

// dup_node の下請け関数
AtNode*
AigTemplate::dup_node_sub(AtNode* node,
			  const hash_map<size_t, size_t>& vmap)
{
  if ( node->mType == kAtConst0 || node->mType == kAtConst1 ) {
    return get_cnode(node->mType);
  }
  if ( node->mType == kAtLiteral ) {
    hash_map<size_t, size_t>::const_iterator p = vmap.find(node->mId);
    size_t new_id = node->mId;
    if ( p != vmap.end() ) {
      new_id = p->second;
    }
    return get_lnode(new_id, node->mInv1);
  }
  AtNode* node0 = dup_node_sub(node->mInputs[0], vmap);
  AtNode* node1 = dup_node_sub(node->mInputs[1], vmap);
  return get_anode(node->mType, node0, node->mInv1, node1, node->mInv2);
}
 
// set_copy の下請け関数
AtNode*
AigTemplate::set_copy_sub(AtNode* node,
			  const NpnMap& vmap)
{
  if ( vmap.opol() == kPolPosi ) {
    if ( node->mType == kAtConst0 || node->mType == kAtConst1 ) {
      return get_cnode(node->mType);
    }
    if ( node->mType == kAtLiteral ) {
      tNpnImap imap = vmap.imap(node->mId);
      tVarId pos = npnimap_pos(imap);
      tPol pol = npnimap_pol(imap);
      bool inv = node->mInv1;
      if ( pol == kPolNega ) {
	inv = !inv;
      }
      return get_lnode(pos, inv);
    }
    AtNode* node0 = set_copy_sub(node->mInputs[0], vmap);
    AtNode* node1 = set_copy_sub(node->mInputs[1], vmap);
    return get_anode(node->mType, node0, node->mInv1, node1, node->mInv2);
  }
  else {
    if ( node->mType == kAtConst0 ) {
      return get_cnode(kAtConst1);
    }
    if ( node->mType == kAtConst1 ) {
      return get_cnode(kAtConst0);
    }
    if ( node->mType == kAtLiteral ) {
      tNpnImap imap = vmap.imap(node->mId);
      tVarId pos = npnimap_pos(imap);
      tPol pol = npnimap_pol(imap);
      bool inv = node->mInv1;
      if ( pol == kPolPosi ) {
	inv = !inv;
      }
      return get_lnode(pos, inv);
    }
    AtNode* node0 = set_copy_sub(node->mInputs[0], vmap);
    AtNode* node1 = set_copy_sub(node->mInputs[1], vmap);
    if ( node->mType == kAtAnd ) {
      return get_anode(kAtOr, node0, node->mInv1, node1, node->mInv2);
    }
    if ( node->mType == kAtOr ) {
      return get_anode(kAtAnd, node0, node->mInv1, node1, node->mInv2);
    }
    if ( node->mType == kAtXor ) {
      return get_anode(kAtXor, node0, !node->mInv1, node1, node->mInv2);
    }
    assert_not_reached(__FILE__, __LINE__);
    return NULL;
  }
}
  
// set_dual の下請け関数
AtNode*
AigTemplate::set_dual_sub(AtNode* node)
{
  if ( node->mType == kAtConst0 ) {
    return get_cnode(kAtConst1);
  }
  if ( node->mType == kAtConst1 ) {
    return get_cnode(kAtConst0);
  }
  if ( node->mType == kAtLiteral ) {
    return get_lnode(node->mId, !node->mInv1);
  }
  AtNode* node0 = set_dual_sub(node->mInputs[0]);
  AtNode* node1 = set_dual_sub(node->mInputs[1]);
  if ( node->mType == kAtAnd ) {
    return get_anode(kAtOr, node0, node->mInv1, node1, node->mInv2);
  }
  if ( node->mType == kAtOr ) {
    return get_anode(kAtAnd, node0, node->mInv1, node1, node->mInv2);
  }
  if ( node->mType == kAtXor ) {
    return get_anode(kAtXor, node0, !node->mInv1, node1, node->mInv2);
  }
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// set_compose の下請け関数
AtNode*
AigTemplate::set_compose_sub(AtNode* node,
			     const vector<AtNode*>& nmap)
{
  assert_cond( node->mType != kAtLiteral, __FILE__, __LINE__);
  
  if ( node->mType == kAtConst0 || node->mType == kAtConst1 ) {
    return get_cnode(node->mType);
  }
  
  AtNode* node0 = NULL;
  bool inv1 = node->mInv1;
  AtNode* inode0 = node->mInputs[0];
  if ( inode0->mType == kAtLiteral ) {
    if ( inode0->mInv1 ) {
      inv1 = !inv1;
    }
    node0 = nmap[inode0->mId];
  }
  else {
    node0 = set_compose_sub(inode0, nmap);
  }
  AtNode* node1 = NULL;
  bool inv2 = node->mInv2;
  AtNode* inode1 = node->mInputs[1];
  if ( inode1->mType == kAtLiteral ) {
    if ( inode1->mInv1 ) {
      inv2 = !inv2;
    }
    node1 = nmap[inode1->mId];
  }
  else {
    node1 = set_compose_sub(inode1, nmap);
  }
  return get_anode(node->mType, node0, inv1, node1, inv2);
}

// 定数ノードを作る．
AtNode*
AigTemplate::get_cnode(tAtType type)
{
  for (vector<AtNode*>::iterator p = mNodes.begin();
       p != mNodes.end(); ++ p) {
    AtNode* node = *p;
    if ( node->mType == type ) {
      return node;
    }
  }
  AtNode* node = new AtNode;
  node->mPos = mNodes.size();
  mNodes.push_back(node);
  node->mType = type;
  return node;
}
  
// リテラルノードを作る．
AtNode*
AigTemplate::get_lnode(ymuint32 id,
		       bool inv)
{
  for (vector<AtNode*>::iterator p = mNodes.begin();
       p != mNodes.end(); ++ p) {
    AtNode* node = *p;
    if ( node->mType == kAtLiteral && node->mId == id && node->mInv1 == inv ) {
      return node;
    }
  }
  AtNode* node = new AtNode;
  node->mPos = mNodes.size();
  mNodes.push_back(node);
  node->mType = kAtLiteral;
  node->mId = id;
  node->mInv1 = inv;
  return node;
}

// AND/OR/XORノードを作る．
AtNode*
AigTemplate::get_anode(tAtType type,
		       AtNode* input0,
		       bool inv0,
		       AtNode* input1,
		       bool inv1)
{
  for (vector<AtNode*>::iterator p = mNodes.begin();
       p != mNodes.end(); ++ p) {
    AtNode* node = *p;
    if ( node->mType == type &&
	 node->mInputs[0] == input0 &&
	 node->mInv1 == inv0 &&
	 node->mInputs[1] == input1 &&
	 node->mInv2 == inv1 ) {
      return node;
    }
  }
  AtNode* node = new AtNode;
  node->mPos = mNodes.size();
  mNodes.push_back(node);
  node->mType = type;
  node->mInputs[0] = input0;
  node->mInputs[1] = input1;
  node->mInv1 = inv0;
  node->mInv2 = inv1;
  return node;
}

END_NAMESPACE_YM
