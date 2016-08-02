#ifndef YM_BDD_DG_H
#define YM_BDD_DG_H

/// @file ym_bdd/Dg.h
/// @brief 直交分解を表すグラフ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Dg.h 1343 2008-03-25 17:15:35Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_bdd/Bdd.h"
#include "ym_bdd/BddLitSet.h"
#include "ym_bdd/BddVarSet.h"


BEGIN_NAMESPACE_YM_BDD

// 分解グラフの節点のクラス
class DgNode;

/// @brief DgNode へのポインタ＋極性
typedef unsigned long tDgEdge;

class DgMgr;


//////////////////////////////////////////////////////////////////////
/// @class PrimDec Dg.h <ym_bdd/Dg.h>
/// @brief 関数分解の基本要素: bound-set, free-set を表すクラス
///
/// ただし，複数の関数(同じ関数を異なる場合分けでコファクター
/// したもの)の同一の bound-set, free-set による関数分解をひとつに
/// 表す目的でも用いられる．そのため bound func はベクタで格納する．
//////////////////////////////////////////////////////////////////////
class PrimDec
{
public:
  /// @brief コンストラクタ
  /// @param[in] bset 束縛変数を表す変数集合
  /// @param[in] fset 自由変数を表す変数集合
  /// @param[in] bfunc 束縛関数
  PrimDec(const BddVarSet& bset,
	  const BddVarSet& fset,
	  const Bdd& bfunc);

  /// @brief コンストラクタ
  /// @param[in] bset 束縛変数を表す変数集合
  /// @param[in] fset 自由変数を表す変数集合
  /// @param[in] bfunc 束縛関数のベクタ
  PrimDec(const BddVarSet& bset,
	  const BddVarSet& fset,
	  const BddVector& bfunc);
  
  /// @brief bound-set を得る．
  BddVarSet
  bound_set() const
  {
    return mBoundSet;
  }
  
  /// @brief free-set を得る．
  BddVarSet
  free_set() const
  {
    return mFreeSet;
  }
  
  /// @brief bound-function を得る．
  Bdd
  bound_func(size_t pos = 0) const
  {
    return mBoundFunc[pos];
  }
  
  /// @brief デバッグ用の出力
  /// @param[in] s 出力ストリーム
  void
  dump(ostream& s) const;
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // bound-set を表すサポート
  BddVarSet mBoundSet;

  // free-set を表すサポート
  BddVarSet mFreeSet;

  // bound function を表すBDDのベクタ
  BddVector mBoundFunc;

};

/// @brief PrimDec のリスト
typedef list<PrimDec> PrimDecList;



//////////////////////////////////////////////////////////////////////
/// @class Bidec Dg.h <ym_bdd/Dg.h>
/// @brief 二項分解を表すクラス
//////////////////////////////////////////////////////////////////////
class Bidec
{
public:

  /// @brief 根の分解のタイプを表すビットパタン
  ///
  /// 実際にはこの値のいくつかのビットワイズORをとった値が
  /// 使われる．
  enum {
    kConst0 = 1,  //< 定数0
    kConst1 = 2,  //< 定数1
    kLit0   = 4,  //< 負のリテラル
    kLit1   = 8,  //< 正のリテラル
    kAnd    = 16, //< AND
    kOr     = 32, //< OR
    kXor    = 64  //< XOR
  };

public:

  /// @brief 空のコンストラクタ
  Bidec()
  {
  }
  
  /// @brief コンストラクタ
  /// @param[in] f global function
  /// @param[in] g1, g2 decomposition function
  Bidec(const Bdd& f,
	const Bdd& g1,
	const Bdd& g2)
  {
    set(f, g1, g2);
  }
  
  /// @brief global function と2つの decomposition function をセットする．
  /// @param[in] f global function
  /// @param[in] g1, g2 decomposition function
  /// @note 根のタイプ(複数の候補があり得る)を求めてセットする．
  void
  set(const Bdd& f,
      const Bdd& g1,
      const Bdd& g2);
  
  /// @brief 全体の関数を得る．
  Bdd
  global_func() const
  {
    return mGlobalFunc;
  }
  
  /// @brief 根の分解のタイプを得る．
  int
  root_type() const
  {
    return mType;
  }
  
  /// @brief pos番目(iは0か1)の入力関数を得る．
  Bdd
  input_func(size_t pos) const
  {
    return mInputFunc[pos];
  }
  
  /// @brief 内容を s に書き出す．
  /// デバッグ用の関数
  void
  display(ostream& s) const;
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力全体の関数
  Bdd mGlobalFunc;

  // 二項分解のタイプ
  int mType;

  // 入力の関数
  Bdd mInputFunc[2];

};

/// @brief Bidec のベクタ
typedef vector<Bidec> BidecVector;



//////////////////////////////////////////////////////////////////////
/// @class Dg Dg.h <ym_bdd/Dg.h>
/// @brief 関数分解を表すグラフを表すクラス
///
/// ユーザがこのクラスを直接生成／削除することはできない．
/// DgMgr を用いること．
/// @sa DgMgr
//////////////////////////////////////////////////////////////////////
class Dg
{
  friend class DgMgr;
public:

  /// @brief ノードのタイプを表す列挙型
  enum tType {
    kC0   = 0, //< 定数0
    kC1   = 1, //< 定数1
    kLitP = 2, //< 肯定のリテラル
    kLitN = 3, //< 否定のリテラル
    kOr   = 4, //< OR
    kAnd  = 5, //< AND
    kXor  = 6, //< XOR
    kXnor = 7, //< XNOR
    kCplx = 8  //< それ以外
  };

public:
  /// @brief 空のコンストラクタ
  Dg();
  
  /// @brief コピーコンストラクタ
  Dg(const Dg& src);
  
  /// @brief 代入演算子
  const Dg&
  operator=(const Dg& src);
  
  /// @brief デストラクタ
  ~Dg();
  
  /// @brief 根のノードが分解を持つとき，true を返す．
  /// @note 具体的には入力がすべてリテラルの CPLX ノード以外は真を返す．
  bool
  has_decomp() const;
  
  /// @brief root で表される分解グラフが CBF のとき，true を返す．
  bool
  is_CBF() const;
  
  /// @brief 根の分解のタイプを返す．
  tType
  type() const;
  
  /// @brief 根の分解のタイプが二項分解のときに true を返す．
  bool
  is_bidecomp() const;
  
  /// @brief 根の分解の入力数を返す．
  size_t
  ni() const;
  
  /// @brief 根の分解の pos 番目の入力の分解を得る．
  Dg
  input(size_t pos) const;
  
  /// @brief global function を得る．
  Bdd
  global_func() const;
  
  /// @brief 入力を iset のみに制限した場合の global function を求める．
  Bdd
  partial_func(const vector<size_t>& iset) const;
  
  /// @brief サポートを得る．
  BddVarSet
  support() const;
  
  /// @brief サポートのサイズを得る．
  size_t
  support_size() const;

  /// @brief 直交関数分解の列挙
  /// @note うち，bound-set の入力数が llimit 以上 ulimit 以下
  /// のものを列挙する．
  void
  enum_djdec(size_t llimit,
	     size_t ulimit,
	     PrimDecList& dec_list) const;

  /// @brief 内容を出力する
  void
  display(ostream& s) const;
  

private:

  // 枝を指定したコンストラクタ
  Dg(tDgEdge root,
     DgMgr* mgr);

  // enum_dec のための再帰関数
  void
  ed_recur(size_t llimit,
	   size_t ulimit,
	   const BddVarSet& all_sup,
	   PrimDecList& dec_list) const;
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 根の枝
  tDgEdge mRoot;

  // このグラフを管理するオブジェクト
  DgMgr* mMgr;

};



//////////////////////////////////////////////////////////////////////
/// @class DgMgr Dg.h <ym_bdd/Dg.h>
/// @brief 分解グラフを管理するクラス
/// @sa Dg
//////////////////////////////////////////////////////////////////////
class DgMgr
{
  friend class Dg;
public:
  /// @brief プロファイル情報
  struct Profile {
    /// @brief Case1Or の呼ばれた回数
    int mCase1OrNum;
    
    /// @brief Case1Xor の呼ばれた回数
    int mCase1XorNum;
    
    /// @brief Case1Cplxの呼ばれた回数
    int mCase1CplxNum;
    
    /// @brief Case1Cplx2の呼ばれた回数
    int mCase1Cplx2Num;
    
    /// @brief Case2Or の呼ばれた回数
    int mCase2OrNum;
    
    /// @brief Case2Xor の呼ばれた回数
    int mCase2XorNum;
    
    /// @brief Case2Cplx の呼ばれた回数
    int mCase2CplxNum;
    
    /// @brief コンストラクタ
    Profile();
    
    /// @brief プロファイル情報を出力する．
    void display(ostream& s) const;
  };
  
public:

  /// @brief コンストラクタ
  explicit
  DgMgr(BddMgrRef mgr);
  
  /// @brief デストラクタ
  ~DgMgr();
  
  /// @brief BDD から DG を作る．
  /// @return 根の枝を返す．
  Dg
  decomp(const Bdd& F);
  
  /// @brief BDD から BDG を作る．
  /// @return 根の枝を返す．
  Dg
  bidecomp(const Bdd& F);
  
  /// @brief 現在持っている Decomposition Graph ハッシュ表をクリアする．
  void
  clear();
  
  /// @brief 参照回数が0のBDDのエントリを削除する．
  void
  sweep();
  
  /// @brief プロファイル情報を得る．
  void
  get_profile(Profile& prof) const;
  

private:
  
  // 新たなノードを作り，それに極性を加えて枝を返す．
  tDgEdge
  new_edge(const Bdd& f,
	   const BddVarSet& sup,
	   Dg::tType type,
	   const vector<tDgEdge>& inputs,
	   tPol opol = kPolPosi);
  
  // 定数ノード(枝)を作る．
  // val == 0 の時0 それ以外の時1となる．
  tDgEdge
  make_const(int val);
  
  // リテラルを作る．
  // リテラル番号および極性を指定する．
  tDgEdge
  make_literal(tVarId varid,
	       tPol pol);
  
  // 複数の入力を持つORノードを作る．
  // 入力は inputs を参照する．
  // inputs[i] も OR ノードの場合には併合する．
  tDgEdge
  make_or(const vector<tDgEdge>& inputs,
	  tPol opol = kPolPosi);
  
  // ノード v の部分的な入力を持つ OR ノードを作る．
  // InputFlag が flag に一致するもののみを使う．
  tDgEdge
  make_or(DgNode* v,
	  int flag,
	  tPol opol = kPolPosi);
  
  // 複数の入力を持つXORノードを作る．
  // 入力は inputs を参照する．
  // inputs[i] も OR ノードの場合には併合する．
  tDgEdge
  make_xor(const vector<tDgEdge>& inputs,
	   tPol opol = kPolPosi);
  
  // ノード v の部分的な入力を持つ XOR ノードを作る．
  // InputFlag が flag に一致するもののみを使う．
  tDgEdge
  make_xor(DgNode* v,
	   int flag,
	   tPol opol = kPolPosi);
  
  // Cplx ノードを作る．
  // 入力は inputs を参照する．
  tDgEdge
  make_cplx(const Bdd& f,
	    const vector<DgNode*>& inputs,
	    tPol opol = kPolPosi);
  
  // Cplx ノードを作る．
  // こちらは入力がリテラルの時．ノード配列は必要ない．
  tDgEdge
  make_cplx(const Bdd& f,
	    const BddVarSet& support,
	    tPol opol = kPolPosi);
  
  // 2つの入力を持つORノードを作る．
  tDgEdge
  make_or(tDgEdge src1,
	  tDgEdge src2,
	  tPol opol = kPolPosi);
  
  // 2つの入力を持つXORノードを作る．
  tDgEdge
  make_xor(tDgEdge src1,
	   tDgEdge src2,
	   tPol opol = kPolPosi);
  
  // 2つの入力を持つANDノード(実際にはORノードの否定)を作る．
  tDgEdge
  make_and(tDgEdge src1,
	   tDgEdge src2,
	   tPol opol = kPolPosi);
  
  // 複数の入力を持つノードを作る．
  // type には kDgOr か kDgXor を指定する．
  tDgEdge
  make_node(Dg::tType type,
	    const vector<tDgEdge>& inputs,
	    tPol opol = kPolPosi);
  
  // リテラルとのORノードをつくる．
  tDgEdge
  make_litOr(tVarId varid,
	     tPol lit_pol,
	     tDgEdge src2,
	     tPol opol = kPolPosi);
  
  // リテラルとのXORノードをつくる．
  tDgEdge
  make_litXor(tVarId varid,
	      tPol lit_pol,
	      tDgEdge src2,
	      tPol opol = kPolPosi);
  
  // リテラルとのANDノードをつくる．
  tDgEdge
  make_litAnd(tVarId varid,
	      tPol lit_pol,
	      tDgEdge src2,
	      tPol opol = kPolPosi);
  

  
  //////////////////////////////////////////////////////////////////////
  // 以下は disjunctive decomposition 用のサブルーティン
  //////////////////////////////////////////////////////////////////////

  // v0, v1 がともにORノードで共通の入力を持つ場合の処理．
  // (nc, common), (n0, inputs0), (n1, input1) がそれぞれ
  // 共通，v0のみ，v1のみの入力番号を持つ．
  // common は v0 の入力番号
  tDgEdge
  case1or(tVarId varid,
	  DgNode* v0,
	  DgNode* v1,
	  tPol pol0,
	  size_t nc);
  
  // v0, v1 がともにXORノードで共通の入力を持つ場合の処理．
  // (nc, common), (n0, inputs0), (n1, input1) がそれぞれ
  // 共通，v0のみ，v1のみの入力番号を持つ．
  // common は v0 の入力番号
  tDgEdge
  case1xor(tVarId varid,
	   DgNode* v0,
	   DgNode* v1,
	   tPol pol0,
	   tPol pol1,
	   size_t nc);
  
  // v0 が OR で，r1 を入力として持つ．
  tDgEdge
  case2or(tVarId varid,
	  DgNode* v0,
	  tPol pol0,
	  size_t pos,
	  tDgEdge r1,
	  tPol ipol);
  
  // v0 が XR で，r1 を入力として持つ．
  tDgEdge
  case2xor(tVarId varid,
	   DgNode* v0,
	   tPol pol0,
	   size_t pos,
	   DgNode* v1,
	   tPol pol1,
	   tPol ipol);
  
  // v0, v1 がともにCPLXノードでただ1組の入力を除いて共通な入力
  // を持つ場合の処理．
  // d0, d1 は異なっている入力番号
  tDgEdge
  case1cplx(const Bdd& F,
	    tVarId varid,
	    DgNode* v0,
	    DgNode* v1,
	    size_t d0,
	    size_t d1,
	    tPol xpol);
  
  // v0, v1 がともにCPLXノードですべての入力が共通な場合．
  tDgEdge
  case1cplx2(const Bdd& F,
	     tVarId varid,
	     DgNode* v0,
	     DgNode* v1,
	     size_t pos);
  
  // v0 の一部を0/1に固定してv1に一致できる場合の処理
  tDgEdge
  case2cplx(const Bdd& F,
	    tVarId varid,
	    DgNode* c_node,
	    tPol lit_pol,
	    int or_and,
	    DgNode* v);
  
  // CplxCase3 のサブルーティン
  // node もしくは node の TFI のうち，sup と disjoint なものを
  // inputs に入れる．OR/XORノードの場合には自動的に分割する．
  void
  find_disjoint_node(DgNode* node,
		     const BddVarSet& support,
		     vector<DgNode*>& inputs);
  
  // CplxCase3 のサブルーティン
  // 共通でない入力を2つ以上持つノードを探して括り出す．
  void
  find_uncommon_inputs(const vector<DgNode*>& list1,
		       int mark,
		       vector<DgNode*>& inputs);
  
  // CplxCase3 のサブルーティン
  // 共通な入力を2つ以上持つノードを探して，共通部分を括り出す．
  // 共通部分は inputs に追加する．
  void
  find_common_inputs(const vector<DgNode*>& list0,
		     const vector<DgNode*>& list1,
		     vector<DgNode*>& inputs);
  
  // リストの内容を配列に入れて，TopVarId() の昇順にソートする．
  // ただし，0番目の要素は後から入れる．
  size_t
  blist2barray(const vector<DgNode*>& b_list);
  
  // 境界のノードを inputs にいれる再帰処理
  void
  list_boundary(DgNode* node,
		vector<DgNode*>& inputs);
  
  // index の2つのコファクターをマージする．
  tDgEdge
  merge(tVarId varid,
	tDgEdge r0,
	tDgEdge r1);
  
  // BDD から DG を作るサブルーティン
  tDgEdge
  decomp_step(tBddEdge e);
  
  // 作業領域のサイズを設定する．
  // 今から扱う論理関数の入力数を入れる．
  void
  resize_buffer(size_t ni);
  
  // inputs に枝を追加する．
  void
  insert2(vector<tDgEdge>& inputs,
	       tDgEdge edge);
  
  // inputs にノードを追加する．
  void
  insert3(vector<DgNode*>& inputs,
	  DgNode* node);
  
  // 枝の先のノードの参照回数を一つ増やす
  // ただし定数枝の場合にはノードは空なのでチェックする
  void
  inc_ref(tDgEdge root);
  
  // 枝の先のノードの参照回数を一つ減らす
  // ただし定数枝の場合にはノードは空なのでチェックする
  void
  dec_ref(tDgEdge root);
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親の BddMgr
  BddMgrRef mMgr;

  // 今の分解が bidecomposition の時に true となるフラグ
  bool mBidecomp;
  
  // 分解グラフのノードを持つ連想配列
  // グローバル関数をキーにする．
  hash_map<tBddEdge, Dg> mNodeHash1;
  
  // 二項分解グラフのノードを持つ連想配列
  // グローバル関数をキーにする．
  hash_map<tBddEdge, Dg> mNodeHash2;
  
  size_t mN;
  
  // 現在扱っている関数の入力数
  size_t mNi;
  
  // プロファイル情報
  Profile mProf;

};

/// @brief 直交関数分解の列挙
///
/// 直交関数分解のうち，bound-set の入力数が llimit 以上 ulimit 以下
/// のものを列挙する．
/// ただし，free-set が空のものや bound-set が空のもの, 入力数が 1 の
/// ものも含む．
/// @param[in] f 対象の論理関数
/// @param[in] llimit 束縛変数集合の要素数の下限
/// @param[in] ulimit 束縛変数集合の要素数の上限
/// @param[out] dec_list 関数分解を格納するリスト
void
enum_djdec(const Bdd& f,
	   size_t llimit,
	   size_t ulimit,
	   PrimDecList& dec_list);

/// @brief cf[] に共通な直交分解を求める．
/// @param[in] cf コファクターごとの論理関数を納めた BDD ベクタ
/// @param[in] limit 求める関数分解の入力数の上限
/// @param[out] cd_list 求まった関数分解を納めるリスト
void
common_djdec(const BddVector& cf,
	     size_t limit,
	     PrimDecList& cd_list);

/// @brief 2つの関数分解が直交しているか調べる．
///
/// たがいの bound-set が交わらない分解を直交していると
/// 定義する．
/// @param[in] dec1, dec2 2つの関数分解
/// @retval true dec1 と dec2 は直交している
/// @retval false dec1 と dec2 は直交していない
inline
bool
independent(const PrimDec& dec1,
	    const PrimDec& dec2)
{
  return !(dec1.bound_set() && dec2.bound_set());
}

/// @brief 元の関数 f と束縛関数 bound_func から出力の関数を得る．
/// @param[in] f 元の関数
/// @param[in] bound_func 束縛関数
/// @param[in] hvar 束縛関数の出力に対応する変数番号
/// @return 出力の関数
Bdd
root_func(const Bdd& f,
	  const Bdd& bound_func,
	  tVarId hvar);

/// @brief dec_list をダンプする．
/// @param[in] dec_list 基本分解のリスト
/// @param[in] s 出力ストリーム
void
dump_primdec_list(const PrimDecList& dec_list,
		  ostream& s);

/// @brief 複数の Bidec をマージする．
/// @param[in] idx_vector インデックスのリスト
/// @param[in] bidec_vector idex_vector によるコファクターの
/// 2項分解の結果のリスト
/// @param[in] m_bidec 共通な2項分解を格納する変数
/// @return 共通部分をもつとき true を返す．
/// \f$2^|\mbox{idx\_vector}|\f$個の2項分解 bidec_vector の共通部分を求める．
bool
merge_bidec(const VarVector& idx_vector,
	    const BidecVector& bidec_vector,
	    Bidec& m_bidec);

/// @brief 2^|idx| 個の関数 cf[] が同種の二項分解を持つとき true を返す．
///
/// その時のマージした二項分解を m_bidec に入れる．
/// ただし，第一項の入力数は limit1 以下, 第二項の入力数は limit2 以下
/// とする．
bool
find_common_bidec(const VarVector& idx,
		  const BddVector& cf,
		  size_t limit1,
		  size_t limit2,
		  Bidec& m_bidec);

/// @brief 関数のベクタ cf[] の両立する二項分解を求める．
///
/// 両立する分解とはサポートが同一である分解のこと
/// ただし，第一項の入力数は limit1 以下, 第二項の入力数は limit2 以下
/// とする．
void
enum_common_bidec(const BddVector& cf,
		  size_t limit1,
		  size_t limit2,
		  list<BidecVector>& bidec_list);

END_NAMESPACE_YM_BDD

#endif // YM_BDD_DG_H
