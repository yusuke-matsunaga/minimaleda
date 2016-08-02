#ifndef YM_BDD_BDD_H
#define YM_BDD_BDD_H

/// @file ym_bdd/Bdd.h
/// @brief BDD に共通な定義ファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Bdd.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_bdd/bdd_nsdef.h"
#include "ym_lexp/LogExpr.h"
#include "gmpxx.h"
#include "ym_utils/Binder.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
/// @name 定数の定義
/// @{

/// @brief 葉のノードの仮想的なレベル
/// @ingroup Bdd
const tLevel kLevelMax = 0xFFFFFFFF;

/// @brief 終端の0を指す枝
/// @ingroup Bdd
const tBddEdge kEdge0 = 1UL;

/// @brief 終端の1を指す枝
/// @ingroup Bdd
const tBddEdge kEdge1 = 0UL;

/// @brief 不正な値を表す枝
/// @ingroup Bdd
const tBddEdge kEdgeInvalid = 2UL;

/// @brief オーバーフローを表す枝(invalidと同値)
/// @ingroup Bdd
const tBddEdge kEdgeOverflow = 2UL;

/// @brief エラーを表す枝
/// @ingroup Bdd
const tBddEdge kEdgeError = 3UL;

/// @}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
/// @name tBddEdge に関係する関数の宣言
/// @{

/// @brief 枝に極性を付加する
/// @ingroup Bdd
/// @param[in] e 操作対象の枝
/// @param[in] p 極性
/// @return e に極性 p を付加したもの
tBddEdge
addpol(tBddEdge e,
       tPol p);

/// @brief e が不正な値でなければ枝に極性を付加する
/// @ingroup Bdd
/// @param[in] e 操作対象の枝
/// @param[in] p 極性
/// @return e に極性 p を付加したもの
tBddEdge
addpol_ifvalid(tBddEdge e,
	       tPol p);

/// @brief 枝の極性を反転させる
/// @ingroup Bdd
/// @param[in] e 操作対象の枝
/// @return e の極性を反転させたもの
tBddEdge
negate(tBddEdge e);

/// @brief e が不正な値でなければ枝の極性を反転させる
/// @ingroup Bdd
/// @param[in] e 操作対象の枝
/// @return e の極性を反転させたもの
tBddEdge
negate_ifvalid(tBddEdge e);

/// @brief 正極性の枝に変換する．
/// @ingroup Bdd
/// @param[in] e 操作対象の枝
/// @return e の極性を正極性にしたもの
tBddEdge
normalize(tBddEdge e);

/// @brief 枝の極性をとり出す
/// @ingroup Bdd
/// @param[in] e 操作対象の枝
/// @return e の極性
tPol
get_pol(tBddEdge e);

/// @brief 0の終端ノードのチェック
/// @ingroup Bdd
/// @param[in] e 操作対象の枝
/// @retval true e が0の終端ノードを指していた
/// @retval false 上記以外
bool
check_zero(tBddEdge e);

/// @brief 1の終端ノードを指す枝の時，trueを返す．
/// @ingroup Bdd
/// @param[in] e 操作対象の枝
/// @retval true e が1の終端ノードを指していた
/// @retval false 上記以外
bool
check_one(tBddEdge e);

/// @brief 定数ノード(0か1)の時, true を返す．
/// @ingroup Bdd
/// @param[in] e 操作対象の枝
/// @retval true e が0か1の終端ノードを指していた
/// @retval false 上記以外
bool
check_const(tBddEdge e);

/// @brief エラーを表す枝の時,true を返す．
/// @ingroup Bdd
/// @param[in] e 操作対象の枝
/// @retval true e がエラーを表す枝の場合
/// @retval false 上記以外
bool
check_error(tBddEdge e);

/// @brief オーバーフローを表す枝の時, true を返す．
/// @ingroup Bdd
/// @param[in] e 操作対象の枝
/// @retval true e がオーバーフローを表す枝の場合
/// @retval false 上記以外
bool
check_overflow(tBddEdge e);

/// @brief エラーかオーバーフローを表す枝の時, true を返す．
/// @ingroup Bdd
/// @param[in] e 操作対象の枝
/// @retval true e がエラーかオーバーフローを表す枝の場合
/// @retval false 上記以外
bool
check_invalid(tBddEdge e);

/// @brief 終端ノードを指す枝の時，true を返す．
/// @ingroup Bdd
/// @param[in] e 操作対象の枝
/// @retval true e が終端ノードを指していた
/// @retval false 上記以外
bool
check_leaf(tBddEdge e);

/// @brief 2つの枝が極性違いの時にtrueを返す．
/// @ingroup Bdd
/// @param[in] e1, e2 操作対象の枝
/// @retval true e1 と e2 が極性違いで同じノードを指している場合
/// @retval false 上記以外
bool
check_reverse(tBddEdge e1,
	      tBddEdge e2);

/// @}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
/// @class Bdd Bdd.h <ym_utils/Bdd.h>
/// @ingroup Bdd
/// @brief BDDを表すクラス
/// @sa BddMgr
//////////////////////////////////////////////////////////////////////
class Bdd
{
  friend class BddMgrRef;
  friend class DgMgr;

public:

  /// @brief デフォルトのコンストラクタ
  /// @note デフォルトの BDD マネージャの定数0となる
  Bdd();
  
  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  Bdd(const Bdd& src);
  
  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 自分自身
  const Bdd&
  operator=(const Bdd& src);
  
  /// @brief デストラクタ
  /// @note 根の枝のリンク数を一つ減らして，0になった時には実際に
  /// ノードの解放を行なう．
  /// @note エラーは起こらない．
  ~Bdd();
  
  
  //////////////////////////////////////////////////////////////////////
  /// @name 特別な値を持つときのチェック
  /// @{

  /// @brief 定数0のチェック
  /// @return 定数0の時 true を返す．
  bool
  is_zero() const;

  /// @brief 定数1のチェック
  /// @return 定数1の時 true を返す．
  bool
  is_one() const;

  /// @brief 定数のチェック
  /// @return 定数の時 true を返す．
  bool
  is_const() const;

  /// @brief メモリオーバーフローのチェック
  /// @return メモリオーバーフローで処理が完了していないとき true を返す．
  bool
  is_overflow() const;

  /// @brief エラーのチェック
  /// @return 演算結果がエラーになったとき true を返す．
  bool
  is_error() const;

  /// @brief オーバーフローかエラーのチェック
  /// @return オーバーフローかエラーの時 true を返す．
  bool
  is_invalid() const;

  /// @brief 終端ノードのチェック
  /// @return 終端ノード (定数0, 定数1, オーバーフロー, エラー)
  /// のとき true を返す．
  bool
  is_leaf() const;
  
  /// @brief 正リテラルのみのキューブのチェック
  /// @return 表している関数が正リテラルのみのキューブ (積項)
  /// の場合に true を返す． 
  bool
  is_posi_cube() const;

  /// @brief キューブのチェック
  /// @return 表している関数がキューブ (積項) の場合に true を返す．
  bool
  is_cube() const;

  /// @brief 対称性のチェック
  /// @param[in] x, y 変数番号
  /// @param[in] pol 極性
  /// @return 変数xとyが対称(交換可能)な時に true を返す．
  /// ただし pol = kPolNega の時には x(または y) を反転して
  /// 交換したときに等しくなるかどうかチェックする．
  bool
  check_symmetry(tVarId x,
		 tVarId y,
		 tPol pol = kPolPosi) const;

  /// @}
  //////////////////////////////////////////////////////////////////////

  
  //////////////////////////////////////////////////////////////////////
  /// @name 論理演算＋代入演算子
  /// @{
  
  /// @brief 否定の論理を計算する
  /// @return 否定した論理
  Bdd
  operator~() const;

  /// @brief 否定を計算して代入する．
  /// @return 自分自身
  const Bdd&
  negate();
  
  /// @brief AND 付き代入
  /// @param[in] src オペランド
  /// @return 自分自身
  const Bdd&
  operator&=(const Bdd& src);
  
  /// @brief OR 付き代入
  /// @param[in] src オペランド
  /// @return 自分自身
  const Bdd&
  operator|=(const Bdd& src);
  
  /// @brief XOR 付き代入
  /// @param[in] src オペランド
  /// @return 自分自身
  const Bdd&
  operator^=(const Bdd& src);

  /// @brief 変数の拘束付き代入
  /// @param[in] r 固定する変数を表したリテラルの集合
  /// @return 自分自身
  const Bdd&
  operator/=(const BddLitSet& r);
  
  /// @brief generalized cofactor 付き代入
  /// @param[in] c 制約を表す BDD
  /// @return 自分自身
  /// @warning c を定数0にしてはいけない
  const Bdd&
  operator/=(const Bdd& c);

  /// @}
  //////////////////////////////////////////////////////////////////////


  //////////////////////////////////////////////////////////////////////
  /// @name BDD の構造に関係したメソッド
  /// @{
  
  /// @brief 根の節点の変数に基づいてShannon展開を行なう．
  /// @param[out] f0 0枝のコファクターを入れる BDD
  /// @param[out] f1 1枝のコファクターを入れる BDD
  /// @return 根の節点の変数番号を返す．
  /// @note もともと定数値(葉)のBDDの場合，kVarIdMax を返し，
  /// f0, f1 には自分自身を代入する．
  tVarId
  root_decomp(Bdd& f0,
	      Bdd& f1) const;

  /// @brief 根の変数番号の取得
  /// @retval 根の変数番号 内部節点の場合
  /// @retval kVarIdMax 終端節点の場合
  tVarId
  root_var() const;
  
  /// @brief 0枝の指しているコファクターの取得
  /// @retval 0枝の指しているコファクター 内部節点の場合
  /// @retval 自分自身 終端節点の場合
  Bdd
  edge0() const;
  
  /// @brief 1枝の指しているコファクターの取得
  /// @retval 1枝の指しているコファクター 内部節点の場合
  /// @retval 自分自身 終端節点の場合
  Bdd
  edge1() const;

  /// @brief 親の BddMgrRef の取得
  /// @return 親の BddMgrRef を返す．
  BddMgrRef
  mgr() const;

  /// @}
  //////////////////////////////////////////////////////////////////////

  
  //////////////////////////////////////////////////////////////////////
  /// @name その他の演算
  /// @{
  
  /// @brief コファクター演算
  /// @param[in] var 変数番号
  /// @param[in] pol 極性
  /// @return 変数 var の極性 pol 側のコファクターを返す．
  Bdd
  cofactor(tVarId var,
	   tPol pol) const;

  /// @brief コファクター演算
  /// @param[in] literal リテラル
  /// @return literal によるコファクターを返す．
  Bdd
  cofactor(const Literal& literal) const;
  
  /// @brief Davio展開のモーメント項の計算
  /// @param[in] idx 変数番号
  /// @return Davio 展開のモーメント項 (\f$f_{\overline{x}} \oplus f_x\f$)
  /// を返す．
  Bdd
  xor_moment(tVarId idx) const;
  
  /// @brief Smallest Cube Containg F を求める．
  /// @return SCC を返す．
  Bdd
  SCC() const;

  /// @brief compose 演算
  /// @param[in] var 置き換える対象の変数
  /// @param[in] g 置き換え先の BDD
  /// @return 演算結果
  Bdd
  compose(tVarId var,
	  const Bdd& g) const;

  /// @brief 複数変数の compose 演算
  /// @param[in] comp_map 置き換える対象の変数から置き換え先の BDD への写像
  /// @return 演算結果
  Bdd
  compose(const VarBddMap& comp_map) const;

  /// @brief 変数インデックスの置き換えを行う．
  /// @param[in] var_map 置き換え元の変数から置き換え先の変数への写像
  /// @return 演算結果
  Bdd
  remap_var(const VarVarMap& var_map) const;

  /// @brief e-smoothing(elimination)
  /// @param[in] svars 消去する変数の集合
  /// @return 演算結果
  Bdd
  esmooth(const BddVarSet& svars) const;

  /// @brief a-smoothing(elimination)
  /// @param[in] svars 消去する変数の集合
  /// @return 演算結果
  Bdd
  asmooth(const BddVarSet& svars) const;

  /// @brief x_level の変数を y_level まで「押し込む」
  /// @param[in] x_level 元の変数のレベル
  /// @param[in] y_level 移動先の変数のレベル
  /// @param[in] pol 極性
  /// @return 演算結果
  /// @note pol が kPolNega の場合には 0-枝と 1-枝を取り替える．
  /// @note 他の関数と異なり変数番号ではなくレベルで指定する．
  /// @warning x_level < y_level でないときはエラーとなる．
  Bdd
  push_down(tLevel x_level,
	    tLevel y_level,
	    tPol pol = kPolPosi) const;
  
  /// @brief BDD の内容を積和形論理式に変換する．
  /// @return 等価な積和形論理式
  LogExpr
  sop() const;

  /// @}
  //////////////////////////////////////////////////////////////////////

  
  //////////////////////////////////////////////////////////////////////
  /// @name ハッシュ用の関数
  /// @{

  /// @brief ハッシュ値の計算
  /// @return ハッシュ値を返す．
  ymuint
  hash() const;

  /// @}
  //////////////////////////////////////////////////////////////////////

  
  //////////////////////////////////////////////////////////////////////
  /// @name 表示/ノード数の計数など
  /// @{
  
  /// @brief 内容を書き出す
  /// @param[in] s 出力ストリーム
  /// @return ノード数を返す．
  size_t
  display(ostream& s) const;
  
  /// @brief BDD が表す関数のカルノー図を表示する
  /// @param[in] s 主力ストリーム
  /// @warning ただし4変数以内
  void
  display_map(ostream& s) const;
  
  /// @brief BDD の内容を積和形論理式の形で出力する．
  /// @param[in] s 出力ストリーム
  void
  display_sop(ostream& s) const;

  /// @brief 内容のダンプ
  /// @param[in] s 出力ストリーム
  void
  dump(ostream& s) const;

  /// @brief BDD が使っているノード数を数える．
  /// @return BDD が使っているノード数
  size_t
  size() const;
  
  /// @brief 真理値表密度の計算
  /// @param[in] n 入力数
  double
  density(tVarSize n) const;
  
  /// @brief 最小項の数の計算
  /// @param[in] n 入力数
  /// @return 最小項の数
  mpz_class
  minterm_count(tVarSize n) const;
  
  /// @brief Walsh変換の 0次係数の計算
  /// @param[in] n 入力数
  /// @return Walsh変換の 0次係数
  mpz_class
  walsh0(tVarSize n) const;
  
  /// @brief Walsh変換の 1次係数の計算
  /// @param[in] var 変数番号
  /// @param[in] n 入力数
  /// @return 変数 var の 1次係数
  mpz_class
  walsh1(tVarId var,
	 tVarSize n) const;
  
  /// @brief サポート変数集合の計算 (VarVector)
  /// @param[out] support サポート変数集合を格納するベクタ
  /// @return サポートの要素数
  tVarSize
  support(VarVector& support) const;
  
  /// @brief サポート変数集合の計算 (VarList)
  /// @param[out] support サポート変数集合を格納するリスト
  /// @return サポートの要素数
  tVarSize
  support(VarList& support) const;
  
  /// @brief サポート変数集合の計算 (BddVarSet)
  /// @return サポート変数集合
  BddVarSet
  support() const;
  
  /// @brief サポート変数集合の要素数の計算
  /// @return サポート変数集合の要素数
  tVarSize
  support_size() const;
  
  /// @brief 1パスの探索
  /// @return 1パス ('1' の終端へ至る経路) を表すリテラル集合
  BddLitSet
  onepath() const;
  
  /// @brief 最短の 1パスの探索
  /// @return 最短の 1パス ('1' の終端へ至る経路) を表すリテラル集合
  BddLitSet
  shortest_onepath() const;
  
  /// @brief 最短の 1パスの長さの取得
  /// @return 最短の 1パスの長さ
  tVarSize
  shortest_onepath_len() const;

  /// @}
  //////////////////////////////////////////////////////////////////////

  
  //////////////////////////////////////////////////////////////////////
  /// @name 比較演算
  /// @{
  
  /// @brief 等価比較演算
  /// @param[in] src2 比較対象の BDD
  /// @retval true 自分自身と src2 が等価な論理関数を表している場合
  /// @retval false 上記以外
  bool
  operator==(const Bdd& src2) const;

  /// @brief 2つの関数の交叉チェック
  /// @param[in] src2 比較対象の BDD
  /// @retval true 自分自身と src2 が交わっている時
  /// @retval false 上記以外
  bool
  operator&&(const Bdd& src2) const;
  
  /// @brief 包含関係のチェック
  /// @param[in] src2 比較対象の BDD
  /// @retval true 自分自身が minterms の集合として src2 を含む時
  /// @retval false 上記以外
  bool
  operator>=(const Bdd& src2) const;

  /// @}
  //////////////////////////////////////////////////////////////////////

  
  //////////////////////////////////////////////////////////////////////
  /// @name リテラル集合関係の関数
  /// @{

  /// @brief BddLitSet からリテラルベクタへの変換
  /// @param[out] dst リテラルを格納するベクタ
  /// @return 要素数
  /// @note 自分自身がリテラル集合を表している場合に内容をベクタに変換する．
  tVarSize
  to_literalvector(LiteralVector& dst) const;

  /// @brief BddLitSet からリテラルリストへの変換
  /// @param[out] dst リテラルを格納するリスト
  /// @return 要素数
  /// @note 自分自身がリテラル集合を表している場合に内容をリストに変換する．
  tVarSize
  to_literallist(LiteralList& dst) const;

  /// @}
  //////////////////////////////////////////////////////////////////////


  //////////////////////////////////////////////////////////////////////
  /// @name NPN Matcher 関係の関数
  /// @{

  /// @brief 節点に n-mark を付け，各変数ごとにノード数を数える．
  void
  scan(hash_map<tVarId, size_t>& node_counts) const;

  /// @brief レベル level のノード数を数える．
  /// @note ただし n-mark が付いていないノードがあったら UINT_MAX を返す．
  size_t
  count_at(tLevel level) const;

  /// @brief scan で付けた n-mark を消す．
  void
  clear_scanmark() const;

  /// @}
  //////////////////////////////////////////////////////////////////////

  
  //////////////////////////////////////////////////////////////////////
  //  friend 関数の宣言
  //////////////////////////////////////////////////////////////////////
  friend
  Bdd
  ite_op(const Bdd& cond,
	 const Bdd& s,
	 const Bdd& t);

  friend
  Bdd
  and_exist(const Bdd& src1,
	    const Bdd& src2,
	    const BddVarSet& svars);

  friend
  Bdd
  isop(const Bdd& lower,
       const Bdd& upper,
       LogExpr& cover);

  friend
  LogExpr
  prime_cover(const Bdd& lower,
	      const Bdd& upper);

  friend
  Bdd
  minimal_support(const Bdd& lower,
		  const Bdd& upper);

  friend
  size_t
  display(const BddVector& array,
	  ostream& s);

  friend
  size_t
  display(const BddList& array,
	  ostream& s);

  friend
  void
  dump(const BddVector& array,
       ostream& s);

  friend
  void
  dump(const BddList& array,
       ostream& s);

  friend
  size_t
  size(const BddVector& array);

  friend
  size_t
  size(const BddList& array);

  friend
  tVarSize
  support(const BddVector& bdd_array,
	  VarVector& sup);

  friend
  tVarSize
  support(const BddVector& bdd_array,
	  VarList& sup);

  friend
  BddVarSet
  support(const BddVector& bdd_array);

  friend
  tVarSize
  support_size(const BddVector& bdd_array);

  friend
  tVarSize
  support(const BddList& bdd_array,
	  VarVector& sup);

  friend
  tVarSize
  support(const BddList& bdd_array,
	  VarList& sup);

  friend
  BddVarSet
  support(const BddList& bdd_array);

  friend
  tVarSize
  support_size(const BddList& bdd_array);

  friend
  Bdd
  vscap(const Bdd& src1,
	const Bdd& src2);

  friend
  Bdd
  vsdiff(const Bdd& src1,
	 const Bdd& src2);

  friend
  bool
  vsintersect(const Bdd& src1,
	      const Bdd& src2);

  friend
  Bdd
  lscap(const Bdd& src1,
	const Bdd& src2);

  friend
  Bdd
  lsdiff(const Bdd& src1,
	 const Bdd& src2);

  friend
  bool
  lsintersect(const Bdd& src1,
	      const Bdd& src2);

public:
  //////////////////////////////////////////////////////////////////////
  /// @name static メンバ
  /// @{

  /// @brief default_mgr に養子縁組を行う
  ///
  /// @param[in] top_bdd BDD リストの先頭
  /// @param[in] default_mgr 付け替える先の BDD マネージャ
  static
  void
  change_mgr(Bdd* top_bdd,
	     BddMgr* default_mgr);

  /// @}
  //////////////////////////////////////////////////////////////////////


private:

  /// @brief BDD マネージャと根の枝を引数とするコンストラクタ
  /// @param[in] pmgr BDD マネージャ
  /// @param[in] e 根の枝
  Bdd(BddMgr* pmgr,
      tBddEdge root);
  
  // 根の枝をとり出す
  tBddEdge
  root() const;

  /// @brief mgr, root をセットする時に呼ばれる関数
  /// @param[in] mgr BDD マネージャ
  /// @param[in] root 根の枝
  void
  set(BddMgr* mgr,
      tBddEdge root);

  /// @brief (mMgr, mRoot) への参照をなくす時に呼ばれる関数
  void
  reset();

  /// @brief 根の枝を new_e に置き換える．
  /// @param[in] new_e 新しい枝
  /// @warning new_e も同一の BddMgr に属していると仮定する．
  void
  assign(tBddEdge new_e);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親の BddMgr を指すポインタ
  BddMgr* mMgr;

  // 根のノードを指す枝
  tBddEdge mRoot;

  // おなじ BddMgr の BDD をつなぐためのリンクポインタ
  Bdd* mPrev;
  Bdd* mNext;

};


//////////////////////////////////////////////////////////////////////
/// @class BddMgrParam Bdd.h <ym_bdd/Bdd.h>
/// @ingroup Bdd
/// @brief BddMgrにパラメータをセットするための構造体．
///
/// 全てのメンバを設定する必要はなく，設定したいメンバに対応した
/// マスクビットを1にすればよい
/// @sa BddMgr
//////////////////////////////////////////////////////////////////////
struct BddMgrParam
{
  static
  const ymuint32 GC_THRESHOLD  =  1U;
  static
  const ymuint32 GC_NODE_LIMIT =  2U;
  static
  const ymuint32 NT_LOAD_LIMIT =  4U;
  static
  const ymuint32 RT_LOAD_LIMIT =  8U;
  static
  const ymuint32 MEM_LIMIT     = 16U;

  double mGcThreshold;
  size_t mGcNodeLimit;
  double mNtLoadLimit;
  double mRtLoadLimit;
  size_t mMemLimit;
};


//////////////////////////////////////////////////////////////////////
/// @class BddMgrFactory Bdd.h <ym_bdd/Bdd.h>
/// @ingroup Bdd
/// @brief BddMgr(の派生クラス)を生成するファクトリクラス
///
/// このクラスの派生クラスの operator() 関数を使って BddMgr を生成する
/// @sa BddMgr
//////////////////////////////////////////////////////////////////////
class BddMgrFactory
{
public:
  /// @brief コンストラクタ
  BddMgrFactory();

  /// @brief デストラクタ
  virtual
  ~BddMgrFactory();

  /// @brief BddMgr のインスタンスを生成する．
  virtual
  BddMgr*
  operator()() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class BddMgrRef Bdd.h <ym_bdd/Bdd.h>
/// @ingroup Bdd
/// @brief BDDの動きを管理するクラス．
///
/// 実際には BddMgr へのスマートポインタとなっている．
/// @sa BddMgr
//////////////////////////////////////////////////////////////////////
class BddMgrRef
{
  friend class Bdd;
  friend class DgMgr;
public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタとデストラクタ
  // いちおう誰でも生成/破壊ができる．
  //////////////////////////////////////////////////////////////////////

  /// @brief 空のコンストラクタ
  /// @note デフォルトのマネージャを取り出す．
  BddMgrRef();

  /// @brief ファクトリオブジェクトを指定するコンストラクタ
  /// @note 新しいインスタンスを生成する．
  BddMgrRef(const BddMgrFactory& factory);

  /// @brief コピーコンストラクタ
  /// @note 実際には同じ BddMgr を指すだけで BddMgr はコピーしない．
  BddMgrRef(const BddMgrRef& src);

  /// @brief 代入演算子
  /// @note コピーコンストラクタと同様
  const BddMgrRef&
  operator=(const BddMgrRef& src);

  /// @brief デストラクタ
  ~BddMgrRef();

  
  //////////////////////////////////////////////////////////////////////
  /// @name 等価比較演算子
  /// @{

  /// @brief 2つのマネージャが等価なとき true を返す．
  bool
  operator==(const BddMgrRef& src2) const;

  /// @brief 2つのマネージャが等価でないとき true を返す．
  bool
  operator!=(const BddMgrRef& src2) const;

  /// @}
  //////////////////////////////////////////////////////////////////////

  
  //////////////////////////////////////////////////////////////////////
  /// @name BDD 生成用関数
  /// @{
  
  /// @brief 定数0関数を作る．
  /// @return 生成された BDD
  Bdd
  make_zero();
  
  /// @brief 定数1関数を作る．
  /// @return 生成された BDD
  Bdd
  make_one();
  
  /// @brief オバーフローBDDを明示的に生成する．
  /// @return 生成された BDD
  Bdd
  make_overflow();
  
  /// @brief エラーBDDを明示的に生成する．
  /// @return 生成された BDD
  Bdd
  make_error();
  
  /// @brief リテラル関数を表すBDDを作る
  /// @param[in] varid 変数番号
  /// @param[in] pol 極性
  /// @return 生成された BDD
  Bdd
  make_literal(tVarId varid,
	       tPol pol);

  /// @brief リテラル関数を表すBDDを作る
  /// @param[in] lit リテラル
  /// @return 生成された BDD
  /// @sa Literal
  Bdd
  make_literal(const Literal& lit);
  
  /// @brief 肯定のリテラル関数を作る
  /// @param[in] varid 変数番号
  /// @return 生成された BDD
  Bdd
  make_posiliteral(tVarId varid);
  
  /// @brief 否定のリテラル関数を作る．
  /// @param[in] varid 変数番号
  /// @return 生成された BDD
  Bdd
  make_negaliteral(tVarId varid);
  
  /// @brief インデックスと左右の子供を指定してBDDを作る．
  /// @param[in] varid 変数番号
  /// @param[in] chd_0 0枝の指す BDD
  /// @param[in] chd_1 1枝の指す BDD
  /// @return 生成された BDD
  Bdd
  make_bdd(tVarId varid,
	   const Bdd& chd_0,
	   const Bdd& chd_1);

  /// @brief ベクタを真理値表と見なしてBDDを作る．
  /// @param[in] v 真理値表ベクタ
  /// @param[in] ni 入力数
  /// @return 生成された BDD
  /// @note 変数は 0 〜 (ni - 1) とみなす．
  /// @note ベクタの値は非ゼロを true とみなす．
  /// @warning v の大きさは 2^ni に等しくなければならない．
  Bdd
  tvec_to_bdd(const vector<int>& v,
	      tVarSize ni);

  /// @brief ベクタを真理値表と見なしてBDDを作る(変数割当て付き)．
  /// @param[in] v 真理値表ベクタ
  /// @param[in] vars 変数番号のベクタ
  /// @return 生成された BDD
  /// @note こちらは個々の変数を vars で指定したもの．
  /// @note ベクタの値は非ゼロを true とみなす．
  /// @warning v の大きさは 2^(vars.size()) に等しくなければならない．
  Bdd
  tvec_to_bdd(const vector<int>& v,
	      const VarVector& vars);
  
  /// @brief 論理式からBDDを作る．
  /// @param[in] expr 論理式
  /// @param[in] varmap 変数番号の割当てを表す連想配列
  /// @return 生成された BDD
  /// @note varmapは論理式中にあらわれる変数から BDD 用の変数
  /// への写像を表す．
  /// varmap に登録されていない場合には恒等変換だと思う．
  Bdd
  expr_to_bdd(const LogExpr& expr,
	      const VarVarMap& varmap = VarVarMap());

  /// @brief 論理式からBDDを作る(compose版)．
  /// @param[in] expr 論理式
  /// @param[in] varmap 変数番号とBDDの割当てを表す連想配列
  /// @return 生成された BDD
  /// @note varmapは論理式中にあらわれる変数から論理関数への
  /// 写像を表す．
  /// map に登録されていない場合には恒等変換だと思う．
  Bdd
  expr_to_bdd(const LogExpr& expr,
	      const VarBddMap& varmap);
  
  /// @brief ストリームを論理式と見なしてBDDを作る．
  /// @param[in] in 入力ストリーム
  /// @param[out] err_msg エラーメッセージを格納する文字列
  /// @return 生成された BDD
  /// @note ストリームが正しくないときにはエラーBDDを作り，
  /// err_msg にエラーメッセージをセットする．
  Bdd
  expr_to_bdd(istream& in,
	      string& err_msg);
  
  /// @brief 文字列を読んで論理式を作り,そこからBDDを作る．
  /// @param[in] s 論理式を表す文字列
  /// @param[out] err_msg エラーメッセージを格納する文字列
  /// @return 生成された BDD
  /// @note 文字列が正しくないときにはエラーBDDを作り，
  /// err_msg にエラーメッセージをセットする．
  Bdd
  expr_to_bdd(const char* s,
	      string& err_msg);
  
  /// @brief 文字列を読んで論理式を作り,そこからBDDを作る．
  /// @param[in] s 論理式を表す文字列
  /// @param[out] err_msg エラーメッセージを格納する文字列
  /// @return 生成された BDD
  /// @note 文字列が正しくないときにはエラーBDDを作り，
  /// err_msg にエラーメッセージをセットする．
  Bdd
  expr_to_bdd(const string& s,
	      string& err_msg);
  
  /// @brief しきい値関数を表すBDDを返す．
  /// @param[in] n 入力数
  /// @param[in] th しきい値
  /// @return 生成された BDD
  /// @note しきい値関数とは n 入力で th 個以上の入力が1のときに1
  /// となる関数のこと．
  /// @note 関数処理で作るよりも効率的．
  Bdd
  make_thfunc(tVarSize n,
	      tVarSize th);

  /// @brief 複数のBDDの論理積を求める．
  /// @param[in] bdds BDD のベクタ
  /// @return 生成された BDD
  Bdd
  and_op(const BddVector& bdds);

  /// @brief 複数のBDDの論理積を求める．
  /// @param[in] bdds BDD のリスト
  /// @return 生成された BDD
  Bdd
  and_op(const BddList& bdds);

  /// @brief 複数のBDDの論理和を求める．
  /// @param[in] bdds BDD のベクタ
  /// @return 生成された BDD
  Bdd
  or_op(const BddVector& bdds);

  /// @brief 複数のBDDの論理和を求める．
  /// @param[in] bdds BDD のリスト
  /// @return 生成された BDD
  Bdd
  or_op(const BddList& bdds);

  /// @brief 複数のBDDの排他的論理和を求める．
  /// @param[in] bdds BDD のベクタ
  /// @return 生成された BDD
  Bdd
  xor_op(const BddVector& bdds);

  /// @brief 複数のBDDの排他的論理和を求める．
  /// @param[in] bdds BDD のリスト
  /// @return 生成された BDD
  Bdd
  xor_op(const BddList& bdds);

  /// @brief 保存されたBDDを読み込む．
  /// @param[in] s 入力ストリーム
  /// @return 生成された BDD
  Bdd
  restore(istream& s);

  /// @brief 保存されたBDDをベクタに読み込む．
  /// @param[in] s 入力ストリーム
  /// @param[out] array 生成された BDD を格納するベクタ
  void
  restore(istream& s,
	  BddVector& array);

  /// @brief 保存されたBDDをリストに読み込む．
  /// @param[in] s 入力ストリーム
  /// @param[out] array 生成された BDD を格納するリスト
  void
  restore(istream& s,
	  BddList& array);

  /// @}
  //////////////////////////////////////////////////////////////////////

  
  //////////////////////////////////////////////////////////////////////
  /// @name 変数番号とレベルの対応づけ
  /// @{

  /// @brief 変数を確保する．
  /// @param[in] varid 変数番号
  /// @retval true 変数が確保できた
  /// @retval false 確保に失敗した
  /// @note 新しい変数は最後の変数の後ろに挿入される．
  bool
  new_var(tVarId varid);

  /// @brief 現在登録されている変数をそのレベルの昇順で返す．
  /// @param[out] vlist 変数を格納するリスト
  /// @return 変数の要素数
  tVarSize
  var_list(list<tVarId>& vlist) const;

  /// @brief 変数番号からレベルを得る．
  /// @param[in] varid 変数番号
  /// @return varid に対応するレベル
  /// @note もしもレベルが割り当てられていない場合にはエラーとなる．
  tLevel
  level(tVarId varid) const;
  
  /// @brief レベルから変数番号を得る．
  /// @param[in] level レベル
  /// @return level に対応する変数番号
  tVarId
  varid(tLevel level) const;
  
  /// @brief 動的変数順変更を許可する．
  /// @note BddMgr の実装によっては動的変数順の変更をサポートして
  /// いない場合がある
  void
  enable_DVO();
  
  /// @brief 動的変数順変更を禁止する．
  /// @note BddMgr の実装によっては動的変数順の変更をサポートして
  /// いない場合がある
  void
  disable_DVO();

  /// @}
  //////////////////////////////////////////////////////////////////////

  
  //////////////////////////////////////////////////////////////////////
  /// @name 内部動作の設定を行う関数
  /// @{
  
  /// @brief ガーベージコレクションを行なう．
  /// @param[in] shrink_nodetable この値が true の時,
  /// 可能なら節点テーブルのサイズを縮小する．
  void
  gc(bool shrink_nodetable);

  /// @brief GC 前の sweep 処理を行うためのバインダーを登録する．
  /// @param[in] binder
  /// @sa EventBinder
  void
  reg_sweep_binder(EventBinder* binder);
  
  /// @brief ログ出力用のストリームを設定する．
  /// @param[in] s 出力ストリーム
  void
  set_logstream(ostream& s);

  /// @brief ログ出力用のストリームを解除する．
  void
  unset_logstream();
  
  /// @brief ログ出力用のストリームを得る．
  /// @return ログ出力用のストリーム
  ostream&
  logstream() const;

  /// @brief パラメータを設定する．
  /// @param[in] param 設定用パラメータ
  /// @param[in] mask 設定したい項目を指定するマスク
  /// @note mask のビットが 1 となっている項目のみ設定する．
  /// @sa BddMgrParam
  void
  param(const BddMgrParam& param,
	ymuint32 mask);
  
  /// @brief パラメータを取得する．
  /// @param[out] param 取得されたパラメータを格納する変数
  void
  param(BddMgrParam& param) const;

  /// @brief 名前を得る．
  const string&
  name() const;

  /// @brief 使用メモリ量(in bytes)を得る．
  size_t
  used_mem() const;
  
  /// @brief 節点テーブルに登録されているノードの数を得る．
  size_t
  node_num() const;
  
  /// @brief GC で回収される(フリーになる)ノード数を得る．
  size_t
  garbage_num() const;
  
  /// @brief 利用可能なフリーノード数を得る．
  size_t
  avail_num() const;
  
  /// @brief GC の起動された回数を得る．
  size_t
  gc_count() const;

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  // BddMgr を指定したコンストラクタ
  explicit
  BddMgrRef(BddMgr* ptr);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実際のマネージャを指すポインタ
  BddMgr* mPtr;

};


//////////////////////////////////////////////////////////////////////
/// @name BDD 関係の non-member functions
/// @{

/// @relates Bdd
/// @ingroup Bdd
/// @brief AND 演算
/// @param[in] src1, src2 オペランド
/// @return src1 と src2 の AND
Bdd
operator&(const Bdd& src1,
	  const Bdd& src2);

/// @relates Bdd
/// @ingroup Bdd
/// @brief OR 演算
/// @param[in] src1, src2 オペランド
/// @return src1 と src2 の OR
Bdd
operator|(const Bdd& src1,
	  const Bdd& src2);

/// @relates Bdd
/// @ingroup Bdd
/// @brief XOR 演算
/// @param[in] src1, src2 オペランド
/// @return src1 と src2 の XOR
Bdd
operator^(const Bdd& src1,
	  const Bdd& src2);

/// @relates Bdd
/// @ingroup Bdd
/// @brief 変数への値の代入
/// @param[in] src もとの BDD
/// @param[in] r 固定する変数を表したリテラルの集合
/// @return r に従って変数を固定した BDD
Bdd
operator/(const Bdd& src,
	  const BddLitSet& r);

/// @relates Bdd
/// @ingroup Bdd
/// @brief generalized cofactor の計算
/// @param[in] src もとの BDD
/// @param[in] c 制約を表す BDD
/// @return c に従ってコファクタリングした BDD
/// @warning c を定数0にしてはいけない
Bdd
operator/(const Bdd& src,
	  const Bdd& c);

/// src1 と src2 が等価でない場合 true を返す．
bool
operator!=(const Bdd& src1,
	   const Bdd& src2);

// src1 が minterms の集合として src2 に含まれるとき true を返す．
bool
operator<=(const Bdd& src1,
	   const Bdd& src2);

// src1 が真に src2 を含むとき true を返す．
bool
operator>(const Bdd& src1,
	  const Bdd& src2);

// src1 が真に src2 に含まれるとき true を返す．
bool
operator<(const Bdd& src1,
	  const Bdd& src2);

// if-then-else 演算．
// condが真の時はsを，偽の時はtを選ぶ
Bdd
ite_op(const Bdd& cond,
       const Bdd& s,
       const Bdd& t);

// src1 と src2 の論理積を計算しつつスムージングを行なう．
Bdd
and_exist(const Bdd& src1,
	  const Bdd& src2,
	  const BddVarSet& svars);

// ISOP の生成
// lower と upper で指定された不完全指定論理関数に含まれる関数の
// 非冗長積和形表現を一つ求め, cover にセットする．
// その論理関数を返す．
Bdd
isop(const Bdd& lower,
     const Bdd& upper,
     LogExpr& cover);

// prime-cover の生成
// lower と upper で指定された不完全指定論理関数に含まれる関数の
// すべての prime implicant からなる prime cover を求める．
LogExpr
prime_cover(const Bdd& lower,
	    const Bdd& upper);

// minimum support の生成
// lower と upper で指定された不完全指定論理関数に含まれる関数の
// 極小サポート集合をもとめる．
Bdd
minimal_support(const Bdd& lower,
		const Bdd& upper);

// BDDの配列の内容を書き出す
// と同時にノード数を返す．
size_t
display(const BddVector& array,
	ostream& s);
size_t
display(const BddList& array,
	ostream& s);

// BDDの配列の内容を保存用に書き出す
void
dump(const BddVector& array,
     ostream& s);

// BDDの配列の内容を保存用に書き出す
void
dump(const BddList& array,
     ostream& s);

// BDDの配列のノード数を数える
size_t
size(const BddVector& array);

// BDDの配列のノード数を数える
size_t
size(const BddList& array);

// BDD のベクタのサポートを求める．
tVarSize
support(const BddVector& bdd_array,
	VarVector& sup);

// BDD のベクタのサポートを求める．
tVarSize
support(const BddVector& bdd_array,
	VarList& sup);

// BDD のベクタのサポートを求める．
BddVarSet
support(const BddVector& bdd_array);

// BDD のベクタのサポートを求める．
tVarSize
support_size(const BddVector& bdd_array);

// BDD のリストのサポートを求める．
tVarSize
support(const BddList& bdd_array,
	VarVector& sup);

// BDD のリストのサポートを求める．
tVarSize
support(const BddList& bdd_array,
	VarList& sup);

// BDD のリストのサポートを求める．
BddVarSet
support(const BddList& bdd_array);

// BDD のリストのサポートを求める．
tVarSize
support_size(const BddList& bdd_array);

// src1 と src2 が変数集合の時に共通部分を求める．
Bdd
vscap(const Bdd& src1,
      const Bdd& src2);

// src1 と src2 が変数集合の時に集合差を求める．
Bdd
vsdiff(const Bdd& src1,
       const Bdd& src2);

// src1 と src2 が変数集合の時のインターセクションチェック
bool
vsintersect(const Bdd& src1,
	    const Bdd& src2);

// src1 と src2 がリテラル集合の時に共通部分を求める．
Bdd
lscap(const Bdd& src1,
      const Bdd& src2);

// src1 と src2 がリテラル集合の時に集合差を求める．
Bdd
lsdiff(const Bdd& src1,
       const Bdd& src2);

// src1 と src2 がリテラル集合の時のインターセクションチェック
bool
lsintersect(const Bdd& src1,
	    const Bdd& src2);

/// @}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 枝に極性を付加する
// @param e 枝
// @param p 極性
// @return e に極性 p を加えたもの
inline
tBddEdge
addpol(tBddEdge e,
       tPol p)
{
  return e ^ static_cast<tBddEdge>(p);
}

// @brief e が不正な値でなければ枝に極性を付加する
// @param e 枝
// @param p 極性
// @return e に極性 p を加えたもの
inline
tBddEdge
addpol_ifvalid(tBddEdge e,
	       tPol p)
{
  return (e & kEdgeInvalid) ? e : e ^ static_cast<tBddEdge>(p);
}

// @brief 枝の極性を反転させる
// @param e 枝
// @return 反転した枝
inline
tBddEdge
negate(tBddEdge e)
{
  return e ^ static_cast<tBddEdge>(kPolNega);
}

// @brief e が不正な値でなければ枝の極性を反転させる
// @param e 枝
// @return 反転した枝
inline
tBddEdge
negate_ifvalid(tBddEdge e)
{
  return (e & kEdgeInvalid) ? e : e ^ long(kPolNega);
}

// @brief 正極性の枝に変換する．
// @param e 枝
// @return 枝の極性を正極性に正規化したもの
inline
tBddEdge
normalize(tBddEdge e)
{
  return e & ~1UL;
}

// @brief 枝の極性をとり出す
// @param e 枝
// @return e の極性
inline
tPol
get_pol(tBddEdge e)
{
  return static_cast<tPol>(e & 1UL);
}

// @brief 定数 0 の終端ノードのチェック
// @param e 枝
// @return e が定数 0 の終端ノードを指す枝の時，true を返す．
inline
bool
check_zero(tBddEdge e)
{
  return (e == kEdge0);
}

// @brief 定数 1 の終端ノードのチェック
// @param e 枝
// @return e が定数 1 の終端ノードを指す枝の時，true を返す．
inline
bool
check_one(tBddEdge e)
{
  return (e == kEdge1);
}

// @brief 定数ノードのチェック
// @param e 枝
// @return e が定数ノード ( 0 か 1 ) の時, true を返す．
inline
bool
check_const(tBddEdge e)
{
  // ちょっとキタナイことをやっている．
  return ((e & ~1) == kEdge1);
}

// @brief エラーを表す枝のチェック
// @param e 枝
// @return e がエラーを表す枝の時，true を返す．
inline
bool
check_error(tBddEdge e)
{
  return (e == kEdgeError);
}

// @brief オーバーフローを表す枝のチェック
// @param e 枝
// @return e がオーバーフローを表す枝の時，true を返す．
inline
bool
check_overflow(tBddEdge e)
{
  return (e == kEdgeOverflow);
}

// @brief 不正な枝のチェック
// @param e 枝
// @return e が不正な枝 (エラーかオーバーフロー)を表す枝の時，true を返す．
inline
bool
check_invalid(tBddEdge e)
{
  return ((e & ~1UL) == kEdgeInvalid);
}

// @brief 終端ノードを指す枝のチェック
// @param e 枝
// @return e が終端ノードを指す枝 (定数かエラーかオーバーフロー) の時，
// true を返す．
inline
bool
check_leaf(tBddEdge e)
{
  return !(e & ~3UL);
}

// @brief 2つの枝が極性違いの時に true を返す．
// @param e1 1 つめの枝
// @param e2 2 つめの枝
// @return 2つの枝が極性違いの時に true を返す．
inline
bool
check_reverse(tBddEdge e1,
	      tBddEdge e2)
{
  return ((e1 ^ e2) == kPolNega);
}

// @brief 根の枝をとり出す
// @return 根の枝
inline
tBddEdge
Bdd::root() const
{
  return mRoot;
}

// @brief 親の BDD マネージャを返す．
// @return 親の BDD マネージャ
inline
BddMgrRef
Bdd::mgr() const
{
  return BddMgrRef(mMgr);
}

// @brief 定数0 のチェック
// @return 定数0の時 true を返す．
inline
bool
Bdd::is_zero() const
{
  return check_zero(root());
}

// @brief 定数1 のチェック
// @return 定数1 の時 true を返す．
inline
bool
Bdd::is_one() const
{
  return check_one(root());
}

// @brief 定数のチェック
// @return 定数の時 true を返す．
inline
bool
Bdd::is_const() const
{
  return check_const(root());
}

// @brief オーバーフローのチェック
// @return 演算結果がオーバーフローしたとき true を返す．
inline
bool
Bdd::is_overflow() const
{
  return check_overflow(root());
}

// @brief エラーのチェック
// @return 演算結果がエラーになったとき true を返す．
inline
bool
Bdd::is_error() const
{
  return check_error(root());
}

// @brief オーバーフローとエラーのチェック
// @return 演算結果がオーバーフローかエラーのとき true を返す．
inline
bool
Bdd::is_invalid() const
{
  return check_invalid(root());
}

// @brief 終端ノードのチェック
// @return 終端ノードのとき true を返す．
inline
bool
Bdd::is_leaf() const
{
  return check_leaf(root());
}

// @brief 等価比較
// @return 2つのBDDが等しいとき true を返す．
inline
bool
Bdd::operator==(const Bdd& src2) const
{
  // 実はただのポインタ（スカラ値）比較でわかる．
  return root() == src2.root();
}

// @brief 不等価比較
// @return 2つのBDDが等しくないとき true を返す．
inline
bool
operator!=(const Bdd& src1,
	   const Bdd& src2)
{
  return ! src1.operator==(src2);
}

// @brief 被包含チェック
// @return src1 が src2 に minterm 集合として包含されているときに
// true を返す．
inline
bool
operator<=(const Bdd& src1,
	   const Bdd& src2)
{
  return src2 >= src1;
}

// @brief 真包含チェック
// @param src1, src2 オペランド
// @return src1 が src2 を minterm 集合として真に包含しているときに
// true を返す．
inline
bool
operator>(const Bdd& src1,
	  const Bdd& src2)
{
  return (src1 != src2) && (src1 >= src2);
}

// @brief 真被包含チェック
// @param src1, src2 オペランド
// @return src1 が src2 を minterm 集合として真に包含されているときに
// true を返す．
inline
bool
operator<(const Bdd& src1,
	  const Bdd& src2)
{
  return src2 > src1;
}

// @brief リテラルによるコファクター演算
// @param literal コファクタリングの変数番号と極性を組にしたもの
// @return コファクター
inline
Bdd
Bdd::cofactor(const Literal& literal) const
{
  return cofactor(literal.varid(), literal.pol());
}

// @brief ハッシュ関数
// @return ハッシュ値
inline
ymuint
Bdd::hash() const
{
  tBddEdge r = root();
  return (r * r) >> 8;
}

// @brief 積和系論理式の出力
// @param s 出力ストリーム
// BDD の内容を積和形論理式の形で s に出力する．
inline
void
Bdd::display_sop(ostream& s) const
{
  s << sop() << endl;
}

END_NAMESPACE_YM_BDD

BEGIN_NAMESPACE_HASH

// BDDをキーにしたハッシュ関数クラスの定義
template <>
struct hash<nsYm::nsBdd::Bdd>
{
  ymuint operator()(const nsYm::nsBdd::Bdd& bdd) const
  {
    return bdd.hash();
  }
};

END_NAMESPACE_HASH

#endif // YM_BDD_BDD_H
