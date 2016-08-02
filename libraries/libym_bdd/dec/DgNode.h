#ifndef LIBYM_BDD_DEC_DGNODE_H
#define LIBYM_BDD_DEC_DGNODE_H

/// @file libym_bdd/dec/DgNode.h
/// @brief DgNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DgNode.h 693 2007-05-28 09:13:57Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#include <ym_bdd/Dg.h>


BEGIN_NAMESPACE_YM_BDD

// 分解グラフの節点のクラス
class DgNode;

/// @brief 0を指す枝
const tDgEdge kDgEdge0 = 0UL;

/// @brief 1を指す枝
const tDgEdge kDgEdge1 = 1UL;


//////////////////////////////////////////////////////////////////////
/// @brief 分解グラフの一つのノードを表すクラス
//////////////////////////////////////////////////////////////////////
class DgNode
{
  friend class DgMgr;
public:

  // ノードタイプを得る．
  Dg::tType
  type() const;
  
  // bi-decomposition タイプの時に true を返す．
  bool
  is_bidecomp() const;
  
  // 入力がすべてリテラルの時に真となる．
  bool
  is_all_literal() const;
  
  // グローバル関数を得る．
  Bdd
  global_func() const;
  
  // 入力の部分集合のみを考えたときの関数を計算する．
  // 例外的に空集合で全集合を表す．
  Bdd
  partial_func(const vector<size_t>& iset) const;
  
  // サポートリストを得る．
  BddVarSet
  support() const;
  
  // サポートの要素数を得る．
  size_t
  support_size() const;
  
  // グローバル関数が1となるパタンを得る．
  BddLitSet
  pat1() const;
  
  // グローバル関数が0となるパタンを得る．
  BddLitSet
  pat0() const;
  
  // top_index を得る．
  tVarId
  top_var() const;
  
  // 入力数を得る．
  size_t
  ni() const;
  
  // pos 番目の入力を得る．
  tDgEdge
  input(size_t pos) const;
  
  // pos 番目の入力のノードを得る．
  DgNode*
  input_node(size_t pos) const;
  
  // pos 番目の入力の極性を得る．
  tPol
  input_pol(size_t pos) const;
  
  // リテラル番号を得る．
  tVarId
  literal_var() const;
  
  // マークを付ける再帰処理
  void
  mark_recur(int mark);
  
  // 子供のマークを付ける再帰処理
  int
  chdmark_recur();
  
  // 境界の印を付ける再帰処理
  int
  get_boundary(vector<DgNode*>& or_list,
	       vector<DgNode*>& xor_list);
  
  // mWork を初期化する再帰処理
  void
  clear_mark();
  

private:
  //////////////////////////////////////////////////////////////////////
  // プライベートメンバ関数
  // 内容の設定は Mgr が行う．
  //////////////////////////////////////////////////////////////////////

  // コンストラクタ
  // グローバル関数，サポート，ノードタイプ，入力数を指定する．
  DgNode(const Bdd& f,
	 const BddVarSet& support,
	 Dg::tType type,
	 size_t ni);
  
  // デストラクタ
  ~DgNode();
  
  // マーク(1か2)を付ける．
  void
  mark(int mark);
  
  // マークを取り出す．
  int
  mark() const;
  
  // 子供のマークの最大値を設定する．
  void
  chdmark(int mark);
  
  // 子供のマークの最大値を得る．
  int
  chdmark() const;
  
  // 自分のマークと子供のマークの最大値が等しいときに真を返す．
  bool
  frontier() const;
  
  // 境界の印を付ける．
  void
  bmark(int val);
  
  // 境界の印を得る．
  int
  bmark() const;
  
  // 入力の枝に印を付ける．
  void
  input_flag(size_t pos,
	     int mark);
  
  // 入力の枝の印を取り出す．
  int
  input_flag(size_t pos) const;
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 参照回数
  int mRefCount;

  // ノードタイプ
  Dg::tType mType;

  // 入力がすべてリテラルの時に真となる．
  bool mAllLit;

  // グローバル関数
  Bdd mGlobalFunc;

  // サポートリスト
  BddVarSet mSupport;

  // オンセットの1パタン
  BddLitSet mPat1;

  // オフセットの1パタン
  BddLitSet mPat0;

  // サポートの top variable のインデックス
  // 入力の順序付けに用いる．
  tVarId mTopVarId;

  // 入力数
  size_t mNi;

  // 入力枝の配列
  // ORタイプの時のみ否定属性が付くことがある．
  vector<tDgEdge> mInputs;

  // 入力の枝に付ける作業用のフラグ
  vector<int> mInputFlags;

  // 2つのノードの共通な子供を見つけるための作業領域
  int mWork;

};


//////////////////////////////////////////////////////////////////////
/// @brief tEdge の出力用補助クラス
//////////////////////////////////////////////////////////////////////
class DispEdge
{
public:

  /// @brief コンストラクタ
  DispEdge(tDgEdge edge)
  {
    mEdge = edge;
  }

  /// @brief 出力関数
  friend
  ostream& operator<<(ostream& s,
		      const DispEdge& src)
  {
    ios::fmtflags save = s.flags();
    s << setw(8) << hex << (unsigned int) src.mEdge;
    s.flags(save);
    return s;
  }

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  tDgEdge mEdge;

};

// ポインタ＋極性を枝にする．
tDgEdge
make_edge(DgNode* node,
	  tPol pol = kPolPosi);

// 枝の極性を反転する．
tDgEdge
inv_pol(tDgEdge edge,
	tPol pol = kPolNega);

// 枝からポインタを取り出す．
DgNode*
edge2node(tDgEdge e);

// 枝から極性を取り出す．
tPol
edge2pol(tDgEdge e);

// 枝の極性を反転させる．
void
inv_edge(tDgEdge& e,
	 tPol pol = kPolNega);

// edge の内容を出力する．
void
display(tDgEdge edge,
	ostream& s,
	size_t col = 0);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// マーク(1か2)を付ける．
inline
void
DgNode::mark(int mark)
{
  mWork |= mark;
}

// マークを取り出す．
inline
int
DgNode::mark() const
{
  return mWork & 3;
}

// 子供のマークの最大値を設定する．
inline
void
DgNode::chdmark(int mark)
{
  mWork |= (mark << 2);
}

// 子供のマークの最大値を得る．
inline
int
DgNode::chdmark() const
{
  return (mWork >> 2) & 3;
}

// 自分のマークと子供のマークの最大値が等しいときに真を返す．
inline
bool
DgNode::frontier() const
{
  if ( (mWork ^ (mWork >> 2)) & 3 ) {
    return false;
  }
  else {
    return true;
  }
}

// 境界の印を付ける．
inline
void
DgNode::bmark(int val)
{
  mWork &= 15;
  mWork |= (val << 4);
}

// 境界の印を得る．
inline
int
DgNode::bmark() const
{
  return (mWork >> 4);
}

// 入力の枝に印を付ける．
inline
void
DgNode::input_flag(size_t pos,
		   int mark)
{
  mInputFlags[pos] = mark;
}

// 入力の枝の印を取り出す．
inline
int
DgNode::input_flag(size_t pos) const
{
  return mInputFlags[pos];
}

// ノードタイプを得る．
inline
Dg::tType
DgNode::type() const
{
  return mType;
}

// bi-decomposition タイプの時に true を返す．
inline
bool
DgNode::is_bidecomp() const
{
  return mType == Dg::kOr || mType == Dg::kXor;
}
  
// 入力がすべてリテラルの時に真となる．
inline
bool
DgNode::is_all_literal() const
{
  return mAllLit;
}

// グローバル関数を得る．
inline
Bdd
DgNode::global_func() const
{
  return mGlobalFunc;
}

// サポートリストを得る．
inline
BddVarSet
DgNode::support() const
{
  return mSupport;
}

// サポートの要素数を得る．
inline
size_t
DgNode::support_size() const
{
  return mSupport.size();
}

// グローバル関数が1となるパタンを得る．
inline
BddLitSet
DgNode::pat1() const
{
  return mPat1;
}

// グローバル関数が0となるパタンを得る．
inline
BddLitSet
DgNode::pat0() const
{
  return mPat0;
}

// top_index を得る．
inline
tVarId
DgNode::top_var() const
{
  return mTopVarId;
}

// 入力数を得る．
inline
size_t
DgNode::ni() const
{
  return mNi;
}

// pos 番目の入力を得る．
inline
tDgEdge
DgNode::input(size_t pos) const
{
  return mInputs[pos];
}

// pos 番目の入力のノードを得る．
inline
DgNode*
DgNode::input_node(size_t pos) const
{
  return edge2node(mInputs[pos]);
}

// pos 番目の入力の極性を得る．
inline
tPol
DgNode::input_pol(size_t pos) const
{
  return edge2pol(mInputs[pos]);
}

// リテラル番号を得る．
inline
tVarId
DgNode::literal_var() const
{
  assert_cond(mType == Dg::kLitP, __FILE__, __LINE__);
  return mTopVarId;
}

// プロファイル情報を得る．
inline
void
DgMgr::get_profile(Profile& prof) const
{
  prof = mProf;
}

// ポインタ＋極性を枝にする．
inline
tDgEdge
make_edge(DgNode* node,
	  tPol pol)
{
  return (long) node | (long) pol;
}

// 枝の極性を反転する．
inline
tDgEdge
inv_pol(tDgEdge edge,
	tPol pol)
{
  return (tDgEdge)((long)edge ^ (long)pol);
}

// 枝からポインタを取り出す．
inline
DgNode*
edge2node(tDgEdge e)
{
  return (DgNode*)(e & ~1L);
}

// 枝から極性を取り出す．
inline
tPol
edge2pol(tDgEdge e)
{
  return (tPol)(e & 1L);
}

// 枝の極性を反転させる．
inline
void
inv_edge(tDgEdge& e,
	 tPol pol)
{
  e ^= (long)pol;
}

END_NAMESPACE_YM_BDD


BEGIN_NAMESPACE_HASH

// DgNode* をキーとした hash_{set,map}を作るための定義
template <>
struct hash<nsYm::nsBdd::DgNode*>
{
  size_t operator()(nsYm::nsBdd::DgNode* node) const
  {
    return reinterpret_cast<size_t>(node)/sizeof(void*);
  }
};

// const DgNode* をキーとした hash_{set,map}を作るための定義
template <>
struct hash<const nsYm::nsBdd::DgNode*>
{
  size_t operator()(const nsYm::nsBdd::DgNode* node) const
  {
    return reinterpret_cast<size_t>(node)/sizeof(void*);
  }
};

END_NAMESPACE_HASH

#endif // LIBYM_BDD_DEC_DGNODE_H
