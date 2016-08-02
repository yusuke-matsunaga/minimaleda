
/// @file libym_lexp/LexpMgr.cc
/// @brief LexpMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: LexpMgr.cc 958 2007-08-28 05:38:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "LexpMgr.h"
#include "LexpNode.h"


BEGIN_NAMESPACE_YM_LEXP

//////////////////////////////////////////////////////////////////////
// クラス LexpMgr
//////////////////////////////////////////////////////////////////////


// @brief 唯一のインスタンスを返す．
LexpMgr&
LexpMgr::the_obj()
{
  // 唯一のオブジェクト
  static LexpMgr mTheObj;
  return mTheObj;
}

// @brief コンストラクタ
LexpMgr::LexpMgr() :
  mNodeAlloc(4096),
  mNodeNum(0),
  mMaxNodeNum(0),
  mStuckNodeNum(0)
{
  mConst0 = alloc_node(kConst0);
  ++ mStuckNodeNum;
  mConst1 = alloc_node(kConst1);
  ++ mStuckNodeNum;
}

// デストラクタ
LexpMgr::~LexpMgr()
{
}

// 恒偽関数を作る．
LexpNodePtr
LexpMgr::make_zero()
{
  return mConst0;
}

// 恒真関数を作る．
LexpNodePtr
LexpMgr::make_one()
{
  return mConst1;
}

// 肯定のリテラルを作る．
LexpNodePtr
LexpMgr::make_posiliteral(tVarId id)
{
  make_literals(id);
  size_t pos = id << 1;
  assert_cond(pos < mLiteralArray.size(), __FILE__, __LINE__);
  return mLiteralArray[pos];
}

// 否定のリテラルを作る．
LexpNodePtr
LexpMgr::make_negaliteral(tVarId id)
{
  make_literals(id);
  size_t pos = (id << 1) + 1;
  assert_cond(pos < mLiteralArray.size(), __FILE__, __LINE__);
  return mLiteralArray[pos];
}

// chd_list の要素を子供とする AND ノードを作る．
// 子供も AND ノードの場合にはマージする．
// 子供が定数ノードの場合には値に応じた簡単化を行う．
LexpNodePtr
LexpMgr::make_and(size_t begin)
{
  bool const0 = false;
  size_t end = nodestack_top();
  mTmpNodeList.clear();
  mTmpNodeList.reserve(end - begin);
  for (size_t i = begin; i < end; ++ i) {
    const LexpNode* node = mNodeStack[i];
    tType type = node->type();
    if ( type == kConst0 ) {
      const0 = true;
      break;
    }
    if ( type == kAnd ) {
      size_t ni = node->child_num();
      for (size_t j = 0; j < ni; ++ j) {
	const LexpNode* node1 = node->child(j);
	if ( check_node(node1) ) {
	  // 逆相の入力があったら答は0
	  const0 = true;
	  break;
	}
      }
      if ( const0 ) {
	break;
      }
    }
    else if ( type == kConst1 ) {
      ; // 無視
    }
    else {
      if ( check_node(node) ) {
	// 逆相の入力があったら答は0
	const0 = true;
	break;
      }
    }
  }
  nodestack_pop(begin);

  size_t n = mTmpNodeList.size();
  LexpNodePtr node;
  if ( const0 ) {
    node = make_zero();
  }
  else if ( n == 0 ) {
    node = make_one();
  }
  else if ( n == 1 ) {
    node = mTmpNodeList.front();
  }
  else {
    node = alloc_node(kAnd);
  }
  return node;
}

// chd_list の要素を子供に持つ OR を表す式を作る．
// 子供も OR ノードの場合にはマージする．
// 子供が定数ノードの場合には値に応じた簡単化を行う．
LexpNodePtr
LexpMgr::make_or(size_t begin)
{
  bool const1 = false;
  size_t end = nodestack_top();
  mTmpNodeList.clear();
  mTmpNodeList.reserve(end - begin);
  for (size_t i = begin; i < end; ++ i) {
    LexpNodePtr node = mNodeStack[i];
    tType type = node->type();
    if ( type == kConst1 ) {
      const1 = true;
      break;
    }
    if ( type == kOr ) {
      size_t ni = node->child_num();
      for (size_t j = 0; j < ni; ++ j) {
	const LexpNode* node1 = node->child(j);
	if ( check_node(node1) ) {
	  // 逆相の入力があったら答は1
	  const1 = true;
	  break;
	}
      }
      if ( const1 ) {
	break;
      }
    }
    else if ( type == kConst0 ) {
      ; // 無視
    }
    else {
      if ( check_node(node) ) {
	// 逆相の入力があったら答は1
	const1 = true;
	break;
      }
    }
  }
  nodestack_pop(begin);

  LexpNodePtr node;
  size_t n = mTmpNodeList.size();
  if ( const1 ) {
    node = make_one();
  }
  else if ( n == 0 ) {
    node = make_zero();
  }
  else if ( n == 1 ) {
    node = mTmpNodeList.front();
  }
  else {
    node = alloc_node(kOr);
  }
  return node;
}

// chd_list の要素を子供に持つ XOR を表す式を作る．
// 子供も XOR ノードの場合にはマージする．
// 子供が定数ノードの場合には値に応じた簡単化を行う．
LexpNodePtr
LexpMgr::make_xor(size_t begin)
{
  tPol pol = kPolPosi;
  size_t end = nodestack_top();
  mTmpNodeList.clear();
  mTmpNodeList.reserve(end - begin);
  for (size_t i = begin; i < end; ++ i) {
    LexpNodePtr node = mNodeStack[i];
    tType type = node->type();
    if ( type == kConst1 ) {
      pol = ~pol;
    }
    else if ( type == kXor ) {
      size_t ni = node->child_num();
      for (size_t j = 0; j < ni; ++ j) {
	const LexpNode* node1 = node->child(j);
	if ( check_node2(node1) ) {
	  pol = ~pol;
	}
      }
    }
    else if ( type == kConst0 ) {
      ; // 無視
    } else {
      if ( check_node2(node) ) {
	pol = ~pol;
      }
    }
  }
  nodestack_pop(begin);

  LexpNodePtr node;
  size_t n = mTmpNodeList.size();
  if ( n == 0 ) {
    node = make_zero();
  }
  else {
    if ( n == 1 ) {
      node = mTmpNodeList.front();
    }
    else {
      node = alloc_node(kXor);
    }
  }
  if ( pol == kPolPosi ) {
    return node;
  }
  else {
    return complement(node);
  }
}

// node が node_list の中に含まれていないか調べる．
// 同相で同じものがふくまれていたらなにもせず false を返す．
// 逆相で同じものがふくまれていたらなにもせず true を返す．
// 同じものがふくまれていなかったら node を node_list に追加して
// false を返す．
bool
LexpMgr::check_node(const LexpNode* node)
{
  for (LexpNodeList::iterator p = mTmpNodeList.begin();
       p != mTmpNodeList.end(); ++ p) {
    const LexpNode* node1 = *p;
    if ( posi_equiv(node, node1) ) {
      // 同相で重なっていた場合
      return false;
    }
    if ( nega_equiv(node, node1) ) {
      // 逆相で重なっていた場合
      return true;
    }
  }
  mTmpNodeList.push_back(node);
  return false;
}

// node が node_list の中に含まれていないか調べる(XOR用)．
// 同相で同じものが含まれていたら重複したものを node_list から削除して
// false を返す．
// 逆相で同じものが含まれていたら重複したものを node_list から削除して
// true を返す．
// 同じものが含まれていなかったら node を node_list に追加して
// false を返す．
bool
LexpMgr::check_node2(const LexpNode* node)
{
  for (LexpNodeList::iterator p = mTmpNodeList.begin();
       p != mTmpNodeList.end(); ) {
    const LexpNode* node1 = *p;
    // ループ中で削除する場合があるので反復子をコピーしてから進めておく
    LexpNodeList::iterator q = p;
    ++ p;
    
    if ( posi_equiv(node, node1) ) {
      // 同相で重なっていたら両方取り除く
      mTmpNodeList.erase(q);
      return false;
    }
    if ( nega_equiv(node, node1) ) {
      // 逆相で重なっていたら両方取り除く
      mTmpNodeList.erase(q);
      return true;
    }
  }
  mTmpNodeList.push_back(node);
  return false;
}

// @brief ノードスタックにノードを入れる．
void
LexpMgr::nodestack_push(const LexpNode* node)
{
  mNodeStack.push_back(node);
}

// @brief ノードスタックの先頭位置を返す．
size_t
LexpMgr::nodestack_top()
{
  return mNodeStack.size();
}

// @brief ノードスタックの先頭位置を戻す．
void
LexpMgr::nodestack_pop(size_t oldtop)
{
  assert_cond(oldtop < mNodeStack.size(), __FILE__, __LINE__);
  mNodeStack.erase(mNodeStack.begin() + oldtop, mNodeStack.end());
}

// 否定形を返す．
LexpNodePtr
LexpMgr::complement(const LexpNode* node)
{
  switch ( node->type() ) {
  case kConst0:
    return make_one();
  case kConst1:
    return make_zero();
  case kPosiLiteral:
    return make_negaliteral(node->varid());
  case kNegaLiteral:
    return make_posiliteral(node->varid());
  case kAnd:
    {
      size_t n = node->child_num();
      size_t begin = nodestack_top();
      for (size_t i = 0; i < n; ++ i) {
	nodestack_push(complement(node->child(i)));
      }
      return make_or(begin);
    }
  case kOr:
    {
      size_t n = node->child_num();
      size_t begin = nodestack_top();
      for (size_t i = 0; i < n; ++ i) {
	nodestack_push(complement(node->child(i)));
      }
      return make_and(begin);
    }
  case kXor:
    {
      size_t n = node->child_num();
      size_t begin = nodestack_top();
      nodestack_push(complement(node->child(0)));
      for (size_t i = 1; i < n; ++ i) {
	nodestack_push(node->child(i));
      }
      return make_xor(begin);
    }
  }

  // ここに来ることはない．
  assert_not_reached(__FILE__, __LINE__);

  return 0;
}

// id 番目の変数を sub に置き換える．
LexpNodePtr
LexpMgr::compose(const LexpNode* node,
		 tVarId id,
		 const LexpNodePtr& sub)
{
  switch ( node->type() ) {
  case kConst0:
  case kConst1:
    return node;

  case kPosiLiteral:
    if ( node->varid() == id ) {
      return sub;
    }
    return node;

  case kNegaLiteral:
    if ( node->varid() == id ) {
      return complement(sub);
    }
    return node;
  default:
    break;
  }

  size_t n = node->child_num();
  size_t begin = nodestack_top();
  bool ident = true;
  for (size_t i = 0; i < n; ++ i) {
    LexpNodePtr chd = compose(node->child(i), id, sub);
    if ( chd != node->child(i) ) {
      ident = false;
    }
    nodestack_push(chd);
  }
  if ( ident ) {
    nodestack_pop(begin);
    return node;
  }

  switch ( node->type() ) {
  case kAnd: return make_and(begin);
  case kOr:  return make_or(begin);
  case kXor: return make_xor(begin);
  default:
    break;
  }

  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// comp_map にしたがってリテラルを式に置き換える．
LexpNodePtr
LexpMgr::compose(const LexpNode* node,
		 const VarLogExprMap& comp_map)
{
  switch ( node->type() ) {
  case kConst0:
  case kConst1:
    return node;
    
  case kPosiLiteral:
    {
      VarLogExprMap::const_iterator p = comp_map.find(node->varid());
      if ( p != comp_map.end() ) {
	return (p->second).root();
      }
    }
    return node;

  case kNegaLiteral:
    {
      VarLogExprMap::const_iterator p = comp_map.find(node->varid());
      if ( p != comp_map.end() ) {
	return complement((p->second).root());
      }
    }
    return node;

  default:
    break;
  }

  size_t n = node->child_num();
  size_t begin = nodestack_top();
  bool ident = true;
  for (size_t i = 0; i < n; ++ i) {
    LexpNodePtr chd = compose(node->child(i), comp_map);
    if ( chd != node->child(i) ) {
      ident = false;
    }
    nodestack_push(chd);
  }
  if ( ident ) {
    nodestack_pop(begin);
    return node;
  }

  switch ( node->type() ) {
  case kAnd: return make_and(begin);
  case kOr:  return make_or(begin);
  case kXor: return make_xor(begin);
  default:   break;
  }

  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// 変数番号をマッピングし直す
LexpNodePtr
LexpMgr::remap_var(const LexpNode* node,
		   const VarVarMap& varmap)
{
  switch ( node->type() ) {
  case kConst0:
  case kConst1:
    return node;

  case kPosiLiteral:
    {
      VarVarMap::const_iterator p = varmap.find(node->varid());
      if ( p != varmap.end() ) {
	return make_posiliteral(p->second);
      }
    }
    return node;

  case kNegaLiteral:
    {
      VarVarMap::const_iterator p = varmap.find(node->varid());
      if ( p != varmap.end() ) {
	return make_negaliteral(p->second);
      }
    }
    return node;

  default: break;
  }
    
  size_t n = node->child_num();
  size_t begin = nodestack_top();
  bool ident = true;
  for (size_t i = 0; i < n; ++ i) {
    LexpNodePtr chd = remap_var(node->child(i), varmap);
    if ( chd != node->child(i) ) {
      ident = false;
    }
    nodestack_push(chd);
  }
  if ( ident ) {
    nodestack_pop(begin);
    return node;
  }
  
  switch ( node->type() ) {
  case kAnd: return make_and(begin);
  case kOr:  return make_or(begin);
  case kXor: return make_xor(begin);
  default:   break;
  }

  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// 簡単化を行う．
LexpNodePtr
LexpMgr::simplify(const LexpNode* node)
{
  switch ( node->type() ) {
  case kConst0:
  case kConst1:
  case kPosiLiteral:
  case kNegaLiteral:
    return node;

  default:
    break;
  }
  
  size_t n = node->child_num();
  size_t begin = nodestack_top();
  bool ident = true;
  for (size_t i = 0; i < n; ++ i) {
    LexpNodePtr chd = simplify(node->child(i));
    if ( chd != node->child(i) ) {
      ident = false;
    }
    nodestack_push(chd);
  }
  if ( ident ) {
    nodestack_pop(begin);
    return node;
  }

  switch ( node->type() ) {
  case kAnd: return make_and(begin);
  case kOr:  return make_or(begin);
  case kXor: return make_xor(begin);
  default:   break;
  }

  // ここに来ることはない．
  assert_not_reached(__FILE__, __LINE__);

  return NULL;
}

// @brief 使用されているメモリ量を返す．
size_t
LexpMgr::used_size()
{
  return mNodeAlloc.used_size();
}

// @brief 使用されているノード数を返す．
size_t
LexpMgr::node_num()
{
  return mNodeNum;
}

// @brief used_size() の最大値を返す．
size_t
LexpMgr::max_used_size()
{
  return mNodeAlloc.max_used_size();
}

// @brief nodenum() の最大値を返す．
size_t
LexpMgr::max_node_num()
{
  return mMaxNodeNum;
}

// @brief 実際に確保したメモリ量を返す．
size_t
LexpMgr::allocated_size()
{
  return mNodeAlloc.allocated_size();
}

// @brief 実際に確保した回数を返す．
size_t
LexpMgr::allocated_count()
{
  return mNodeAlloc.allocated_count();
}

// @brief 内部状態を出力する．
void
LexpMgr::print_stats(ostream& s)
{
  s << "maximum used size: " << max_used_size() << endl
    << "maximum node num:  " << max_node_num() << endl
    << "current used size: " << used_size() << endl
    << "current node num:  " << node_num()
    << " ( " << mStuckNodeNum << " )" << endl
    << "allocated size:    " << allocated_size() << endl
    << "allocated count:   " << allocated_count() << endl
    << endl;
}

// id 番めまでのリテラルノードを作る．
void
LexpMgr::make_literals(tVarId id)
{
  size_t last = mLiteralArray.size() / 2;
  while ( last <= id ) {
    LexpNode* posi = alloc_node(kPosiLiteral);
    posi->mNc = last;
    mLiteralArray.push_back(posi);
    ++ mStuckNodeNum;

    LexpNode* nega = alloc_node(kNegaLiteral);
    nega->mNc = last;
    mLiteralArray.push_back(nega);
    ++ mStuckNodeNum;
    ++ last;
  }
}

// LexpNode を確保して内容を設定する．
LexpNode*
LexpMgr::alloc_node(tType type)
{
  ++ mNodeNum;
  if ( mMaxNodeNum < mNodeNum ) {
    mMaxNodeNum = mNodeNum;
  }

  size_t nc = 0;
  if ( type == kAnd || type == kOr || type == kXor ) {
    nc = mTmpNodeList.size();
  }
    
  size_t req_size = calc_size(nc);
  void* p = mNodeAlloc.get_memory(req_size);
  LexpNode* node = reinterpret_cast<LexpNode*>(p);
  node->mRefType = static_cast<ymuint32>(type);
  node->mNc = nc;
  for (size_t i = 0; i < nc; ++ i) {
    node->mChildArray[i] = mTmpNodeList[i];
    node->child(i)->inc_ref();
  }
  return node;
}

// LexpNode を削除する．
void
LexpMgr::free_node(LexpNode* node)
{
  size_t n = node->child_num();
  for (size_t i = 0; i < n; ++ i) {
    node->child(i)->dec_ref();
  }

  -- mNodeNum;

  size_t req_size = calc_size(n);
  mNodeAlloc.put_memory(req_size, node);
}

// LexpNode の入力数から必要なサイズを計算する．
size_t
LexpMgr::calc_size(size_t nc)
{
  return sizeof(LexpNode) + sizeof(LexpNode*) * (nc - 1);
}

END_NAMESPACE_YM_LEXP
