
/// @file libym_lexp/LexpNode.cc
/// @brief LexpNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: LexpNode.cc 1143 2008-01-04 07:27:17Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "LexpNode.h"
#include "LexpMgr.h"
#include "SopLit.h"


BEGIN_NAMESPACE_YM_LEXP

//////////////////////////////////////////////////////////////////////
// クラス LexpNode
//////////////////////////////////////////////////////////////////////

// 同一の式を表していたら true を返す．
bool
posi_equiv(const LexpNode* node0,
	   const LexpNode* node1)
{
  if ( node0->type() != node1->type() ||
       node0->child_num() != node1->child_num() ) {
    return false;
  }

  size_t n = node0->child_num();
  for (size_t i = 0; i < n; i ++) {
    const LexpNode* chd0 = node0->child(i);
    const LexpNode* chd1 = node1->child(i);
    if ( !posi_equiv(chd0, chd1) ) {
      return false;
    }
  }

  if ( node0->is_literal() ) {
    if ( node0->varid() != node1->varid() ) {
      return false;
    }
  }
  return true;
}

// 互いに否定の関係にある式を表していたら true を返す．
bool
nega_equiv(const LexpNode* node0,
	   const LexpNode* node1)
{
  if ( node0->is_one() ) {
    return node0->is_zero();
  }
  if ( node0->is_zero() ) {
    return node1->is_one();
  }
  if ( node0->is_posiliteral() ) {
    return node1->is_negaliteral() && node0->varid() == node1->varid();
  }
  if ( node0->is_negaliteral() ) {
    return node1->is_posiliteral() && node0->varid() == node1->varid();
  }

  if ( node0->is_and() ) {
    if ( !node1->is_or() ) {
      return false;
    }
    size_t n = node0->child_num();
    for (size_t i = 0; i < n; i ++) {
      const LexpNode* chd0 = node0->child(i);
      const LexpNode* chd1 = node1->child(i);
      if ( !nega_equiv(chd0, chd1) ) {
	return false;
      }
    }
    return true;
  }
  if ( node0->is_or() ) {
    if ( !node1->is_and() ) {
      return false;
    }
    size_t n = node0->child_num();
    for (size_t i = 0; i < n; i ++) {
      const LexpNode* chd0 = node0->child(i);
      const LexpNode* chd1 = node1->child(i);
      if ( !nega_equiv(chd0, chd1) ) {
	return false;
      }
    }
    return true;
  }
  if ( node0->is_xor() ) {
    if ( !node1->is_xor() ) {
      return false;
    }
    size_t n = node0->child_num();
    tPol pol = kPolPosi;
    for (size_t i = 0; i < n; i ++) {
      const LexpNode* chd0 = node0->child(i);
      const LexpNode* chd1 = node1->child(i);
      if ( !nega_equiv(chd0, chd1) ) {
	pol = pol * kPolNega;
      }
      else if ( !posi_equiv(chd0, chd1) ) {
	return false;
      }
    }
    return pol == kPolNega;
  }

  assert_not_reached(__FILE__, __LINE__);

  return false;
}

// @brief vals の値にしたがった評価を行う．
ymulong
LexpNode::eval(const vector<ymulong>& vals,
	       ymulong mask) const
{
  if ( is_zero() ) {
    return 0UL;
  }
  if ( is_one() ) {
    return ~0UL & mask;
  }
  if ( is_posiliteral() ) {
    return vals[varid()];
  }
  if ( is_negaliteral() ) {
    return ~vals[varid()] & mask;
  }

  size_t ni = child_num();
  if ( is_and() ) {
    ymulong ans = child(0)->eval(vals, mask);
    for (size_t i = 1; i < ni; ++ i) {
      ans &= child(i)->eval(vals, mask);
    }
    return ans;
  }
  if ( is_or() ) {
    ymulong ans = child(0)->eval(vals, mask);
    for (size_t i = 1; i < ni; ++ i) {
      ans |= child(i)->eval(vals, mask);
    }
    return ans;
  }
  if ( is_xor() ) {
    ymulong ans = child(0)->eval(vals, mask);
    for (size_t i = 1; i < ni; ++ i) {
      ans ^= child(i)->eval(vals, mask);
    }
    return ans;
  }
  assert_not_reached(__FILE__, __LINE__);
  return 0UL;
}

// 定数,リテラルもしくは子供がリテラルのノードの時に true を返す．
bool
LexpNode::is_simple() const
{
  if ( !is_op() ) {
    return true;
  }

  size_t n = child_num();
  for (size_t i = 0; i < n; ++ i) {
    if ( !child(i)->is_literal() ) {
      return false;
    }
  }
  return true;
}

// 子供がすべてリテラルの AND ノードの時に true を返す．
bool
LexpNode::is_simple_and() const
{
  if ( !is_and() ) {
    return false;
  }

  size_t n = child_num();
  for (size_t i = 0; i < n; ++ i) {
    if ( !child(i)->is_literal() ) {
      return false;
    }
  }
  return true;
}

// 子供がすべてリテラルの OR ノードの時に true を返す．
bool
LexpNode::is_simple_or() const
{
  if ( !is_or() ) {
    return false;
  }

  size_t n = child_num();
  for (size_t i = 0; i < n; ++ i) {
    if ( !child(i)->is_literal() ) {
      return false;
    }
  }
  return true;
}

// 子供がすべてリテラルの XOR ノードの時に true を返す．
bool
LexpNode::is_simple_xor() const
{
  if ( !is_xor() ) {
    return false;
  }

  size_t n = child_num();
  for (size_t i = 0; i < n; ++ i) {
    if ( !child(i)->is_literal() ) {
      return false;
    }
  }
  return true;
}

// SOP 形式の時 true を返す．
bool
LexpNode::is_sop() const
{
  if ( is_xor() ) {
    return false;
  }
  if ( is_simple() ) {
    return true;
  }
  if ( !is_or() ) {
    return false;
  }

  size_t n = child_num();
  for (size_t i = 0; i < n; ++ i) {
    const LexpNode* chd = child(i);
    if ( !chd->is_literal() && !chd->is_simple_and() ) {
      return false;
    }
  }
  return true;
}

// リテラル数を返す．
size_t
LexpNode::litnum() const
{
  if ( is_literal() ) {
    // リテラルならリテラル数は1
    return 1;
  }

  if ( is_op() ) {
    // AND/OR/XOR ノードなら子供のリテラル数の和を返す．
    size_t num = 0;
    size_t n = child_num();
    for (size_t i = 0; i < n; ++ i) {
      num += child(i)->litnum();
    }
    return num;
  }

  // それ以外は0
  return 0;
}

// 特定の変数のリテラル数を返す．
size_t
LexpNode::litnum(tVarId id) const
{
  if ( is_literal() && varid() == id ) {
    // リテラルならリテラル数は1
    return 1;
  }

  if ( is_op() ) {
    // AND/OR/XOR ノードなら子供のリテラル数の和を返す．
    size_t num = 0;
    size_t n = child_num();
    for (size_t i = 0; i < n; ++ i) {
      num += child(i)->litnum(id);
    }
    return num;
  }

  // それ以外は0
  return 0;
}

// 特定の変数の特定の極性のリテラル数を返す．
size_t
LexpNode::litnum(tVarId id,
		 tPol pol) const
{
  if ( is_literal(pol) && varid() == id ) {
    // リテラルならリテラル数は1
    return 1;
  }

  if ( is_op() ) {
    // AND/OR/XOR ノードなら子供のリテラル数の和を返す．
    size_t num = 0;
    size_t n = child_num();
    for (size_t i = 0; i < n; ++ i) {
      num += child(i)->litnum(id, pol);
    }
    return num;
  }

  // それ以外は0
  return 0;
}

// @brief 使われている変数の最大の番号 + 1を得る．
size_t
LexpNode::input_size() const
{
  if ( is_literal() ) {
    return varid() + 1;
  }

  if ( is_op() ) {
    size_t ans = 0;
    size_t n = child_num();
    for (size_t i = 0; i < n; ++ i) {
      size_t ans1 = child(i)->input_size();
      if ( ans < ans1 ) {
	ans = ans1;
      }
    }
    return ans;
  }

  // それ以外は 0
  return 0;
}

// SOP形式の積項数とリテラル数を計算する．
SopLit
LexpNode::soplit(bool inverted) const
{
  if ( is_literal() ) {
    return SopLit(1, 1);
  }

  if ( type() == kAnd && ! inverted ||
       type() == kOr && inverted ) {
    SopLit l(1, 0);
    size_t n = child_num();
    for (size_t i = 0; i < n; ++ i) {
      SopLit l1 = child(i)->soplit(inverted);
      l *= l1;
    }
    return l;
  }

  if ( type() == kOr && ! inverted ||
       type() == kAnd && inverted ) {
    SopLit l(0, 0);
    size_t n = child_num();
    for (size_t i = 0; i < n; ++ i) {
      SopLit l1 = child(i)->soplit(inverted);
      l += l1;
    }
    return l;
  }

  if ( type() == kXor ) {
    size_t n = child_num();
    const LexpNode* chd = child(0);
    SopLit lp = chd->soplit(inverted);
    SopLit ln = chd->soplit(inverted);
    for (size_t i = 1; i < n; ++ i) {
      const LexpNode* chd = child(i);
      SopLit l1p = lp;
      SopLit l1n = ln;
      SopLit l2p = chd->soplit(false);
      SopLit l2n = chd->soplit(true);
      lp = l1p * l2n + l1n * l2p;
      ln = l1p * l2p + l1n * l2n;
    }
    return lp;
  }

  return SopLit(0, 0);
}

// SOP形式の積項数とリテラル数を計算する．
SopLit
LexpNode::soplit(bool inverted,
		 tVarId id) const
{
  if ( is_literal() ) {
    if ( varid() == id ) {
      return SopLit(1, 1);
    }
    else {
      return SopLit(1, 0);
    }
  }

  if ( type() == kAnd && ! inverted ||
       type() == kOr  && inverted ) {
    SopLit l(1, 0);
    size_t n = child_num();
    for (size_t i = 0; i < n; ++ i) {
      SopLit l1 = child(i)->soplit(inverted, id);
      l *= l1;
    }
    return l;
  }

  if ( type() == kOr && ! inverted ||
       type() == kAnd && inverted ) {
    SopLit l(0, 0);
    size_t n = child_num();
    for (size_t i = 0; i < n; ++ i) {
      SopLit l1 = child(i)->soplit(inverted, id);
      l += l1;
    }
    return l;
  }

  if ( type() == kXor ) {
    size_t n = child_num();
    const LexpNode* chd = child(0);
    SopLit lp = chd->soplit(inverted);
    SopLit ln = chd->soplit(inverted);
    for (size_t i = 1; i < n; ++ i) {
      const LexpNode* chd = child(i);
      SopLit l1p = lp;
      SopLit l1n = ln;
      SopLit l2p = chd->soplit(false, id);
      SopLit l2n = chd->soplit(true, id);
      lp = l1p * l2n + l1n * l2p;
      ln = l1p * l2p + l1n * l2n;
    }
    return lp;
  }

  return SopLit(0, 0);
}

// SOP形式の積項数とリテラル数を計算する．
SopLit
LexpNode::soplit(bool inverted,
		 tVarId id,
		 tPol pol) const
{
  if ( is_literal() ) {
    if ( varid() == id && is_literal(pol) ) {
      return SopLit(1, 1);
    }
    else {
      return SopLit(1, 0);
    }
  }

  if ( type() == kAnd && ! inverted ||
       type() == kOr && inverted ) {
    SopLit l(1, 0);
    size_t n = child_num();
    for (size_t i = 0; i < n; ++ i) {
      SopLit l1 = child(i)->soplit(inverted, id, pol);
      l *= l1;
    }
    return l;
  }

  if ( type() == kOr && ! inverted ||
       type() == kAnd && inverted ) {
    SopLit l(0, 0);
    size_t n = child_num();
    for (size_t i = 0; i < n; ++ i) {
      SopLit l1 = child(i)->soplit(inverted, id, pol);
      l += l1;
    }
    return l;
  }

  if ( type() == kXor ) {
    size_t n = child_num();
    const LexpNode* chd = child(0);
    SopLit lp = chd->soplit(inverted);
    SopLit ln = chd->soplit(inverted);
    for (size_t i = 1; i < n; ++ i) {
      const LexpNode* chd = child(i);
      SopLit l1p = lp;
      SopLit l1n = ln;
      SopLit l2p = chd->soplit(false, id);
      SopLit l2n = chd->soplit(true, id);
      lp = l1p * l2n + l1n * l2p;
      ln = l1p * l2p + l1n * l2n;
    }
    return lp;
  }

  return SopLit(0, 0);
}

// 自殺する．
void
LexpNode::suicide()
{
  // なんでこれだけのコードを別の関数にするかというと，
  // これを LexpNode.h に書くと LexpMgr.h をインクルードしなければ
  // ならなくなるので．
  LexpMgr::the_obj().free_node(this);
}

END_NAMESPACE_YM_LEXP
