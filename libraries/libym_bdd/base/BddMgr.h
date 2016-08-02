#ifndef LIBYM_BDD_BDDMGR_H
#define LIBYM_BDD_BDDMGR_H

/// @file libym_bdd/base/BddMgr.h
/// @brief BddMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BddMgr.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#include <ym_bdd/Bdd.h>


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
// BDDの動きを管理するクラス．
//////////////////////////////////////////////////////////////////////
class BddMgr
{
  friend class Bdd;
public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタとデストラクタ
  //////////////////////////////////////////////////////////////////////

  // デフォルトマネージャを返す．
  static
  BddMgr* default_mgr();

  // コンストラクタ
  BddMgr();

  // デストラクタ
  virtual
  ~BddMgr();


  //////////////////////////////////////////////////////////////////////
  // 自分自身の参照回数関係
  //////////////////////////////////////////////////////////////////////
  
  // 参照回数を1増やす．
  void
  inc_ref();

  // 参照回数を1減らす．
  // 参照回数が0になったら自分自身を破壊する．
  void
  dec_ref();

  
  //////////////////////////////////////////////////////////////////////
  // BDDの根の枝の参照回数関数
  //////////////////////////////////////////////////////////////////////

  // e の参照回数を増やす．
  virtual
  void
  inc_rootref(tBddEdge e) = 0;

  // e の参照回数を減らす．
  virtual
  void
  dec_rootref(tBddEdge e) = 0;

  
  //////////////////////////////////////////////////////////////////////
  // 変数番号とレベルの対応づけ
  //////////////////////////////////////////////////////////////////////

  // 変数を確保する．
  // 確保に失敗したら false を返す．
  // 最後の変数の後ろに挿入される．
  virtual
  bool
  new_var(tVarId varid) = 0;

  // 現在登録されている変数をそのレベルの昇順で返す．
  virtual
  tVarSize
  var_list(list<tVarId>& vlist) const = 0;

  // 変数番号からレベルを得る．
  // もしもレベルが割り当てられていない場合にはエラーとなる．
  virtual
  tLevel
  level(tVarId varid) const = 0;
  
  // レベルから変数番号を得る．
  virtual
  tVarId
  varid(tLevel level) const = 0;
  
  // 動的変数順変更を許可する．
  virtual
  void
  enable_DVO() = 0;
  
  // 動的変数順変更を禁止する．
  virtual
  void
  disable_DVO() = 0;

  
  //////////////////////////////////////////////////////////////////////
  // BDD 生成用関数
  //////////////////////////////////////////////////////////////////////

  // 肯定のリテラル関数を作る
  virtual
  tBddEdge
  make_posiliteral(tVarId varid) = 0;

  // リテラル関数を表すBDDを作る．
  tBddEdge
  make_literal(tVarId index,
	       tPol pol);

  // リテラル関数を表すBDDを作る
  tBddEdge
  make_literal(const Literal& lit);
  
  // 否定のリテラル関数を作る．
  tBddEdge
  make_negaliteral(tVarId varid);
  
  // インデックスと左右の子供を指定してBDDを作る．
  tBddEdge
  make_bdd(tVarId varid,
	   tBddEdge chd_0,
	   tBddEdge chd_1);

  // Expr から BDD を作るためのサブルーティン
  tBddEdge
  expr_to_bdd(const LogExpr& expr,
	      const hash_map<tVarId,
	      tBddEdge>& vemap);

  // ベクタを真理値表と見なしてBDDを作る．
  // 個々の変数を vars で指定する．
  // ベクタの値は非ゼロを true とみなす．
  // v の大きさは 2^(vars.size()) に等しくなければならない．
  tBddEdge
  tvec_to_bdd(const vector<int>& v,
	      const VarVector& vars);

  
  //////////////////////////////////////////////////////////////////////
  // built-in タイプの論理演算
  //////////////////////////////////////////////////////////////////////

  // src1 & src2 を計算する．
  virtual
  tBddEdge
  and_op(tBddEdge e1,
	 tBddEdge e2) = 0;

  // src1 | src2 を計算する．
  virtual
  tBddEdge
  or_op(tBddEdge e1,
	tBddEdge e2) = 0;

  // src1 ^ src2 を計算する．
  virtual
  tBddEdge
  xor_op(tBddEdge e1,
	 tBddEdge e2) = 0;

  // edge_list に登録されたBDDのANDを計算する．
  tBddEdge
  and_op(const list<tBddEdge>& edge_list);

  // edge_list に登録されたBDDのORを計算する．
  tBddEdge
  or_op(const list<tBddEdge>& edge_list);

  // edge_list に登録されたBDDのXORを計算する．
  tBddEdge
  xor_op(const list<tBddEdge>& edge_list);

  // src1 と src2 の共通部分があれば kEdge1 を返す．
  virtual
  tBddEdge
  check_intersect(tBddEdge e1,
		  tBddEdge e2) = 0;
  
  // Davio展開のモーメント項($f_{\overline{x}} \oplus f_x$)を
  // 求める処理
  virtual
  tBddEdge
  xor_moment(tBddEdge e,
	     tVarId idx) = 0;
  
  // bdd がキューブの時 true を返す．
  virtual
  bool
  check_cube(tBddEdge e) = 0;
  
  // bdd が肯定リテラルのみからなるキューブの時 true を返す．
  virtual
  bool
  check_posi_cube(tBddEdge e) = 0;

  // 変数xとyが対称(交換可能)な時にtrueを返す．
  virtual
  bool
  check_symmetry(tBddEdge e,
		 tVarId x,
		 tVarId y,
		 tPol pol) = 0;

  // 一つの変数に対する cofactor を計算する．
  virtual
  tBddEdge
  scofactor(tBddEdge e1,
	    tVarId id,
	    tPol pol) = 0;

  // generalized cofactor を計算する．
  virtual
  tBddEdge
  gcofactor(tBddEdge e1,
	    tBddEdge e2) = 0;

  // if-then-else 演算を計算する．
  virtual
  tBddEdge
  ite_op(tBddEdge e1,
	 tBddEdge e2,
	 tBddEdge e3) = 0;
  
  // multiple compose 演算を行うために最初に呼ばれる関数．
  virtual
  void
  compose_start() = 0;

  // multiple compose 演算を行う変数と置き換え関数を登録する関数
  virtual
  void
  compose_reg(tVarId id,
	      tBddEdge e) = 0;

  // multiple compose 演算の本体
  virtual
  tBddEdge
  compose(tBddEdge e) = 0;

  // x_level の変数を y_level まで「押し込む」．
  // pol が kPolNega の時は 0-枝と 1-枝を取り替える．
  virtual
  tBddEdge
  push_down(tBddEdge e,
	    tLevel x_level,
	    tLevel y_level,
	    tPol pol = kPolPosi) = 0;
  
  // smoothing(elimination)
  // svars に含まれる変数を消去する．
  virtual
  tBddEdge
  esmooth(tBddEdge e1,
	  tBddEdge e2) = 0;

  // src1 と src2 の論理積を計算して src3 の変数を消去する．
  virtual
  tBddEdge
  and_exist(tBddEdge e1,
	    tBddEdge e2,
	    tBddEdge e3) = 0;

  // lower と upper で指定された不完全指定論理関数の非冗長積和形を求める．
  virtual
  tBddEdge
  isop(tBddEdge l,
       tBddEdge u,
       LogExpr& cover) = 0;

  // lower と upper で指定された不完全指定論理関数の主項カバーを求める．
  virtual
  LogExpr
  prime_cover(tBddEdge l,
	      tBddEdge u) = 0;

  // lower と upper で指定された不完全指定論理関数の極小サポート集合をすべて
  // 列挙する．解は論理関数の形で返される．そのなかの主項がサポート集合に
  // 対応している．
  virtual
  tBddEdge
  minimal_support(tBddEdge l,
		  tBddEdge u) = 0;
  
  // smallest cube containing F 演算
  virtual
  tBddEdge
  SCC(tBddEdge e) = 0;

  
  //////////////////////////////////////////////////////////////////////
  // BDD の構造に関係したメソッド
  //////////////////////////////////////////////////////////////////////
  
  // 根の節点の変数に基づいてShannon展開を行なう．
  // 戻り値として根の節点の変数番号を返し，その変数を0/1に固定した
  // 時の cofactor をそれぞれ f0, f1 に入れる．
  // もともと定数値(葉)のBDDの場合，kVarIdMax を返し，
  // f0, f1 には自分自身を代入する．
  virtual
  tVarId
  root_decomp(tBddEdge e,
	      tBddEdge& e0,
	      tBddEdge& e1) = 0;
  
  // 根の変数番号インデックスを取り出す．
  // 定数節点の場合には kVarIdMax を返す．
  virtual
  tVarId
  root_var(tBddEdge e) = 0;
  
  // 0枝の指している cofactor を返す．
  // 定数節点の場合には自分自身を返す．
  virtual
  tBddEdge
  edge0(tBddEdge e) = 0;
  
  // 1枝の指している cofactor を返す．
  // 定数節点の場合には自分自身を返す．
  virtual
  tBddEdge
  edge1(tBddEdge e) = 0;

  // e の参照回数が0なら true を返す．
  virtual
  bool
  check_noref(tBddEdge e) = 0;
  
  
  //////////////////////////////////////////////////////////////////////
  // 1へ至るパスを求める関数
  //////////////////////////////////////////////////////////////////////
  
  // 1パスを求める．
  // 結果はその経路のみのBDDとなる．
  virtual
  tBddEdge
  onepath(tBddEdge e) = 0;
  
  // 最短の1パスを求める．
  // 結果はその経路のみのBDDとなる．
  virtual
  tBddEdge
  shortest_onepath(tBddEdge e) = 0;
  
  // 最短の1パスの長さを求める．
  virtual
  tVarSize
  shortest_onepath_len(tBddEdge e) = 0;


  //////////////////////////////////////////////////////////////////////
  // ノード数の計数や真理値表密度の計算など
  //////////////////////////////////////////////////////////////////////

  // e を根とするBDDのノード数を数える．
  virtual
  size_t
  size(tBddEdge e) = 0;

  // edge_list に登録されたBDDのノード数を数える．
  virtual
  size_t
  size(const list<tBddEdge>& edge_list) = 0;

  // BDD の表す論理関数の minterm の数を返す．
  // 無限長精度の整数(mpz_class)を用いて計算する．
  // n は論理関数の変数の数
  virtual
  mpz_class
  minterm_count(tBddEdge e,
		tVarSize n) = 0;
  
  // Walsh 変換の0次の係数を計算する．
  // n は論理関数の変数の数
  virtual
  mpz_class
  walsh0(tBddEdge e,
	 tVarSize n) = 0;
  
  // Walsh 変換の1次の係数を計算する．
  // n は論理関数の変数の数
  virtual
  mpz_class
  walsh1(tBddEdge e,
	 tVarId var,
	 tVarSize n) = 0;

  
  //////////////////////////////////////////////////////////////////////
  // サポート関係の関数
  //////////////////////////////////////////////////////////////////////

  // e を根とするBDDのサポートに印をつける．
  virtual
  tVarSize
  mark_support(tBddEdge e) = 0;

  // edge_list に登録されたBDDのサポートに印をつける．
  virtual
  tVarSize
  mark_support(const list<tBddEdge>& edge_list) = 0;

  // 印のついた変数をベクタに変換する．
  virtual
  tVarSize
  mark_to_vector(VarVector& support) = 0;

  // 印のついた変数をリストに変換する．
  virtual
  tVarSize
  mark_to_list(VarList& support) = 0;

  // 印のついた変数をBDD(キューブ)に変換する．
  virtual
  tBddEdge
  mark_to_bdd() = 0;

  
  //////////////////////////////////////////////////////////////////////
  // VarSet の内部で用いられる演算
  //////////////////////////////////////////////////////////////////////

  // src1 と src2 が変数集合の時に共通部分を求める．
  virtual
  tBddEdge
  vscap(tBddEdge e1,
	tBddEdge e2) = 0;

  // src1 と src2 が変数集合の時に集合差を求める．
  virtual
  tBddEdge
  vsdiff(tBddEdge e1,
	 tBddEdge e2) = 0;

  // src1 と src2 が変数集合の時のインターセクションチェック
  virtual
  bool
  vsintersect(tBddEdge e1,
	      tBddEdge e2) = 0;


  //////////////////////////////////////////////////////////////////////
  // LitSet の内部で用いられる演算
  //////////////////////////////////////////////////////////////////////
  
  // src1 と src2 がリテラル集合の時に共通部分を求める．
  virtual
  tBddEdge
  lscap(tBddEdge e1,
	tBddEdge e2) = 0;

  // src1 と src2 がリテラル集合の時に集合差を求める．
  virtual
  tBddEdge
  lsdiff(tBddEdge e1,
	 tBddEdge e2) = 0;

  // src1 と src2 がリテラル集合の時のインターセクションチェック
  virtual
  bool
  lsintersect(tBddEdge e1,
	      tBddEdge e2) = 0;

  // LitSet 用のBDDからリテラルのベクタを作る．
  virtual
  tVarSize
  to_literalvector(tBddEdge e,
		   LiteralVector& dst) = 0;

  // LitSet 用のBDDからリテラルのリストを作る．
  virtual
  tVarSize
  to_literallist(tBddEdge e,
		 LiteralList& dst) = 0;


  //////////////////////////////////////////////////////////////////////
  // NPN Matcher で用いられる演算
  //////////////////////////////////////////////////////////////////////

  // e を根とするBDDの節点に n-mark を付け，各変数ごとのノード数を数える．
  virtual
  void
  scan(tBddEdge e,
       hash_map<tVarId, size_t>& node_counts) = 0;

  // e を根とするBDDのレベル level のノード数を数える．
  // ただし，n-mark の付いていないノードがあったら UINT_MAX を返す．
  virtual
  size_t
  count_at(tBddEdge e,
	   tLevel level) = 0;

  // scan で付けた n-mark を消す．
  virtual
  void
  clear_scanmark(tBddEdge e) = 0;

  
  //////////////////////////////////////////////////////////////////////
  // 内部動作の設定を行う関数
  //////////////////////////////////////////////////////////////////////
  
  // ガーベージコレクションを行なう．
  // shrink_nodetable = true の時, 可能なら節点テーブルのサイズを縮小する．
  virtual
  void
  gc(bool shrink_nodetable) = 0;

  // GC 前の sweep 処理を行うためのバインダーを登録する．
  virtual
  void
  reg_sweep_binder(EventBinder* binder) = 0;

  // パラメータを設定する．設定したい項目のマスクビットを1とする．
  virtual
  void
  param(const BddMgrParam& param,
	ymuint32 mask) = 0;
  
  // パラメータを取得する．
  virtual
  void
  param(BddMgrParam& param) const = 0;

  // 名前を得る．
  virtual
  const string&
  name() const = 0;

  // 使用メモリ量(in bytes)を得る．
  virtual
  size_t
  used_mem() const = 0;
  
  // 節点テーブルに登録されているノードの数を得る．
  virtual
  size_t
  node_num() const = 0;
  
  // GC で回収される(フリーになる)ノード数を得る．
  virtual
  size_t
  garbage_num() const = 0;
  
  // 利用可能なフリーノード数を得る．
  virtual
  size_t
  avail_num() const = 0;
  
  // GC の起動された回数を得る．
  virtual
  size_t
  gc_count() const = 0;

  
  //////////////////////////////////////////////////////////////////////
  // BDDの管理用関数
  //////////////////////////////////////////////////////////////////////

  // Bdd の根の枝をセットする時の関数
  void
  set_bdd(Bdd* bdd_p,
	  tBddEdge e);

  // bdd の根の枝を new_e に変更する．
  // new_e も同一の BddMgr に属していると仮定する．
  void
  assign(Bdd* bdd_p,
	 tBddEdge new_e);

  
  //////////////////////////////////////////////////////////////////////
  // ログ出力用の関数
  //////////////////////////////////////////////////////////////////////

  // ログ出力用のストリームを設定する．
  void
  set_logstream(ostream& s);

  // ログ出力用のストリームを解除する．
  void
  unset_logstream();
  
  // ログ出力用のストリームを得る．
  ostream&
  logstream() const;


private:

  // 真理値表からBDDを作るためのサブルーティン
  tBddEdge
  tvec_sub(const vector<int>& v,
	   ymuint32 top,
	   ymuint32 size,
	   const vector<tBddEdge>& var_vector,
	   tVarId var_idx);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 参照回数
  int mRefCount;

  
  //////////////////////////////////////////////////////////////////////
  // このマネージャに管理されている BDD のリスト
  //////////////////////////////////////////////////////////////////////

  // リストのためのダミーヘッダ
  // BDD としては用いない．
  Bdd* mTopBdd;

  
  //////////////////////////////////////////////////////////////////////
  // ログ出力用のメンバ
  //////////////////////////////////////////////////////////////////////

  // ログ出力用のストリーム
  ostream* mLogFp;

  // ダミーのヌルストリーム
  ofstream* mNullStream;


private:
  //////////////////////////////////////////////////////////////////////
  // スタティックメンバ
  //////////////////////////////////////////////////////////////////////

  // デフォルトのオブジェクト
  static
  BddMgr* mDefaultMgr;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の実装
//////////////////////////////////////////////////////////////////////

// リテラル関数を表すBDDを作る．
inline
tBddEdge
BddMgr::make_literal(tVarId index,
		     tPol pol)
{
  tBddEdge ans = make_posiliteral(index);
  return addpol(ans, pol);
}

// リテラル関数を表すBDDを作る
inline
tBddEdge
BddMgr::make_literal(const Literal& lit)
{
  return make_literal(lit.varid(), lit.pol());
}

// 否定のリテラル関数を作る．
inline
tBddEdge
BddMgr::make_negaliteral(tVarId varid)
{
  return negate_ifvalid(make_posiliteral(varid));
}

// インデックスと左右の子供を指定してBDDを作る．
inline
tBddEdge
BddMgr::make_bdd(tVarId varid,
		 tBddEdge chd_0,
		 tBddEdge chd_1)
{
  tBddEdge tmp = make_posiliteral(varid);
  return ite_op(tmp, chd_1, chd_0);
}

END_NAMESPACE_YM_BDD

#endif // YM_BDD_BDDMGR_H
