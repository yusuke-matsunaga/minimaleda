
/// @file libym_bdd/dec/DgNode.cc
/// @brief DgNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DgNode.cc 700 2007-05-31 00:41:30Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include "DgNode.h"


BEGIN_NAMESPACE_YM_BDD

BEGIN_NONAMESPACE

// DgNode の集合
typedef hash_set<DgNode*> DgNodeSet;

// displayのためのサブルーティン
void
disp_sub(tDgEdge edge,
	 ostream& s,
	 size_t col,
	 DgNodeSet& mark)
{
  if ( edge == kDgEdge0 ) {
    s << "Constant 0" << endl;
    return;
  }
  else if ( edge == kDgEdge1 ) {
    s << "Constant 1" << endl;
    return;
  }

  tPol pol = edge2pol(edge);
  DgNode* node = edge2node(edge);

  if ( mark.count(node) == 0 ) {
    mark.insert(node);
    Bdd F = node->global_func();
    size_t ni = node->ni();
    for (size_t i = 0; i < col; ++ i) {
      s << " ";
    }
    if ( pol == kPolPosi ) {
      s << "-P- ";
    }
    else {
      s << "-N- ";
    }
    switch ( node->type() ) {
    case Dg::kC0:
      s << "CONST0" << endl;
      break;
    case Dg::kLitP:
      s << "LITERAL(" << node->literal_var() << ")" << endl;
      break;
    case Dg::kOr:
      s << "OR(" << ni << ")" << endl;
      for (size_t i = 0; i < ni; ++ i) {
	tDgEdge iedge = node->input(i);
	disp_sub(iedge, s, col + 4, mark);
      }
      break;
    case Dg::kXor:
      s << "XOR(" << ni << ")" << endl;
      for (size_t i = 0; i < ni; ++ i) {
	tDgEdge iedge = node->input(i);
	disp_sub(iedge, s, col + 4, mark);
      }
      break;
    case Dg::kCplx:
      s << "COMPLEX(" << ni << ")\t" << F.sop() << endl;
      for (size_t i = 0; i < ni; ++ i) {
	tDgEdge iedge = node->input(i);
	disp_sub(iedge, s, col + 4, mark);
      }
      break;
    default:
      break;
    }
  }
}

END_NONAMESPACE


// コンストラクタ
// グローバル関数，サポート関数，ノードタイプ，入力数を指定する．
DgNode::DgNode(const Bdd& f,
	       const BddVarSet& sup,
	       Dg::tType type, size_t ni) :
  mType(type),
  mGlobalFunc(f),
  mSupport(sup),
  mPat1(f.onepath()),
  mPat0((~f).onepath()),
  mNi(ni),
  mInputs(mNi),
  mInputFlags(mNi)
{
  mRefCount = 0;
  mTopVarId = f.root_var();
  mWork = 0;
}

// デストラクタ
DgNode::~DgNode()
{
}

// 入力の部分集合のみを考えたときの関数を計算する．
// 例外的に空集合の時は全入力とみなす．
Bdd
DgNode::partial_func(const vector<size_t>& iset) const
{
  if ( iset.empty() ) {
    return global_func();
  }
  Bdd ans = global_func().mgr().make_zero();
  if ( type() == Dg::kOr ) {
    for (vector<size_t>::const_iterator p = iset.begin();
	 p != iset.end(); ++ p) {
      tDgEdge e = input(*p);
      DgNode* inode = edge2node(e);
      tPol pol = edge2pol(e);
      Bdd ifunc = inode->global_func();
      if ( pol == kPolNega ) {
	ifunc.negate();
      }
      ans |= ifunc;
    }
  }
  else if ( type() == Dg::kXor ) {
    for (vector<size_t>::const_iterator p = iset.begin();
	 p != iset.end(); ++ p) {
      tDgEdge e = input(*p);
      DgNode* inode = edge2node(e);
      tPol pol = edge2pol(e);
      Bdd ifunc = inode->global_func();
      if ( pol == kPolNega ) {
	ifunc.negate();
      }
      ans ^= ifunc;
    }
  }
  else {
    cerr << "Error in partial_func(): illegal node type" << endl;
    display(make_edge((DgNode*)this, kPolPosi), cerr);
    for (vector<size_t>::const_iterator p = iset.begin();
	 p != iset.end(); ++ p) {
      cerr << *p;
    }
    cerr << endl;
    abort();
  }
  return ans;
}

// 自分とそのTFIにラベルをつける．
void
DgNode::mark_recur(int label_val)
{
  if ( (mark() & label_val) == 0 ) {
    mark(label_val);
    for (size_t i = 0; i < ni(); ++ i) {
      DgNode* inode = input_node(i);
      inode->mark_recur(label_val);
    }
  }
}

// 自分のTFIのラベルの最大値を求める．
int
DgNode::chdmark_recur()
{
  int val = chdmark();
  if ( val == 0 ) {
    val = mark();
    for (size_t i = 0; i < ni(); ++ i) {
      DgNode* inode = input_node(i);
      val |= inode->chdmark_recur();
    }
    chdmark(val);
  }
  return val;
}

// mark() と chdmark() が同じ値のノードを境界ノードとする．
// 境界ノードの場合には BMark に 1 を付ける．
// それ以外の処理済のノードには 2 を入れる．
// また，境界ノードを2つ以上入力に持つORノード，XORノードを
// それぞれ or_list, xor_list に入れる．
int
DgNode::get_boundary(vector<DgNode*>& or_list,
		     vector<DgNode*>& xor_list)
{
  if ( bmark() == 0 ) {
    if ( frontier() ) {
      bmark(1);
    }
    else {
      bmark(2);
      // 境界ノードの数．
      size_t c = 0;
      for (size_t i = 0; i < ni(); ++ i) {
	DgNode* inode = input_node(i);
	int val1 = inode->get_boundary(or_list, xor_list);
	if ( val1 == 1 ) {
	  ++ c;
	}
      }
      if ( c >= 2 ) {
	if ( type() == Dg::kOr ) {
	  or_list.push_back(this);
	}
	else if ( type() == Dg::kXor ) {
	  xor_list.push_back(this);
	}
      }
    }
  }
  return bmark();
}

// 自分とそのTFIのラベルを消す．
void
DgNode::clear_mark()
{
  if ( mWork != 0 ) {
    mWork = 0;
    for (size_t i = 0; i < ni(); ++ i) {
      DgNode* inode = input_node(i);
      inode->clear_mark();
    }
  }
}

// 内容をファイルに出力する．
void
display(tDgEdge edge,
	ostream& s,
	size_t col)
{
  DgNodeSet mark;
  disp_sub(edge, s, col, mark);
}

END_NAMESPACE_YM_BDD
