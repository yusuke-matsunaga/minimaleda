#ifndef YM_BDD_BDDVARSET_H
#define YM_BDD_BDDVARSET_H

/// @file ym_bdd/BddVarSet.h
/// @brief BddVarSet のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BddVarSet.h 693 2007-05-28 09:13:57Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_bdd/Bdd.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
/// @class BddVarSet BddVarSet.h <ym_bdd/BddVarSet.h>
/// @ingroup YmBdds
/// @brief 変数集合を表すクラス
///
/// 実は BDD を用いて実装している．
//////////////////////////////////////////////////////////////////////
class BddVarSet
{
  friend class Bdd;
public:
  // 反復子
  class iterator
  {
    friend class BddVarSet;
  public:
    // コンストラクタ
    iterator();
    
    // 現在指している要素を返す．
    // 末尾かどうかのチェックは行わない．
    tVarId
    operator*() const;
    
    // ひとつ先に進める．
    // 自分自身を返す．
    iterator&
    operator++();
    
    // ひとつ先に進める．
    // 進める前の値を返す．
    iterator
    operator++(int);
    
    // 等価比較
    bool
    operator==(const iterator& src) const;

    // == の逆
    bool
    operator!=(const iterator& src) const;
    

  private:

    // BddVarSet 用のコンストラクタ
    iterator(const Bdd& bdd);


  private:
    // 現在指している節点
    Bdd mCur;

  };
  // 実はこの反復子は定数反復子である．
  typedef iterator const_iterator;


public:

  // コンストラクタ
  // 空集合となるが，明示的に使う場合には下の make_empty() を使うべき
  BddVarSet(BddMgrRef mgr);
  
  // コピーコンストラクタ
  BddVarSet(const BddVarSet& src);
  
  // 代入演算子
  const BddVarSet&
  operator=(const BddVarSet& src);
  
  // 変数一つだけを要素とする集合を作るコンストラクタ
  BddVarSet(BddMgrRef mgr,
	    tVarId varid);
  
  // vector からの変換用コンストラクタ
  BddVarSet(BddMgrRef mgr,
	    const VarVector& src);
  
  // list からの変換用コンストラクタ
  BddVarSet(BddMgrRef mgr,
	    const VarList& src);
  
  // 空集合を返すクラスメソッド
  static
  BddVarSet
  make_empty(BddMgrRef mgr);
  
  // 空の時, 真となる．
  bool
  empty() const;
  
  // 要素数を返す．
  size_t
  size() const;
  
  // この変数集合の表す関数を返す．
  Bdd
  function() const;
  
  // 先頭の反復子を返す．
  iterator
  begin() const;
  
  // 末尾の反復子を返す．
  iterator
  end() const;
  
  // 変数番号の vector に変換する．
  // 変数番号の順番は内部の実装に依存する．
  // 集合のサイズを返す．
  size_t
  to_vector(VarVector& dst) const;
  
  // 変数番号のリストに変換する．
  // 変数番号の順番は内部の実装に依存する．
  // 集合のサイズを返す．
  size_t
  to_list(VarList& dst) const;

  // 集合和を計算する．
  BddVarSet
  operator+(const BddVarSet& set2) const;
  
  // 集合和を計算して代入する．
  const BddVarSet&
  operator+=(const BddVarSet& set2);

  // 集合積を計算する．
  BddVarSet
  operator*(const BddVarSet& set2) const;
  
  // 集合積を計算して代入する．
  const BddVarSet&
  operator*=(const BddVarSet& set2);

  // 集合差を計算する．
  BddVarSet
  operator-(const BddVarSet& set2) const;

  // 集合差を計算して代入する．
  const BddVarSet&
  operator-=(const BddVarSet& set2);

  // この関数は BddVarSet(const Bdd& bdd) を使う．
  friend
  BddVarSet
  support(const BddVector& bdd_array);
  friend
  BddVarSet
  support(const BddList& bdd_array);


private:

  // BDD を指定するコンストラクタ
  explicit
  BddVarSet(const Bdd& bdd);

  // サイズを設定する．
  void
  set_size(size_t size) const;

  // サイズを無効化する．
  void
  invalidate_size();
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 本体
  Bdd mBody;
  
  // 要素数を入れておくキャッシュ
  // 最下位1ビットが1の時のみ valid な値を持つ．
  mutable
  size_t mSize;

};


//////////////////////////////////////////////////////////////////////
// BddVarSet 関係の non-member 関数の宣言
//////////////////////////////////////////////////////////////////////

// 等価比較演算子
bool
operator==(const BddVarSet& set1,
	   const BddVarSet& set2);

// 非等価比較演算子
bool
operator!=(const BddVarSet& set1,
	   const BddVarSet& set2);

// set1 と set2 が集合として交わっていたら true を返す．
bool
operator&&(const BddVarSet& set1,
	   const BddVarSet& set2);

// set1 が set2 を含んでいたら true を返す．
bool
operator>=(const BddVarSet& set1,
	   const BddVarSet& set2);

// set1 が set2 に含まれていたら true を返す．
bool
operator<=(const BddVarSet& set1,
	   const BddVarSet& set2);

// set1 が真に set2 を含んでいたら true を返す．
bool
operator>(const BddVarSet& set1,
	  const BddVarSet& set2);

// set1 が真に set2 に含まれていたら true を返す．
bool
operator<(const BddVarSet& set1,
	  const BddVarSet& set2);

// ストリームに出力する．
ostream&
operator<<(ostream& s,
	   const BddVarSet& varset);


//////////////////////////////////////////////////////////////////////
// BddVarSet のインライン関数定義
//////////////////////////////////////////////////////////////////////

// この変数集合の表す関数を返す．
inline
Bdd
BddVarSet::function() const
{
  return mBody;
}

// 等価比較演算子
inline
bool
operator==(const BddVarSet& set1,
	   const BddVarSet& set2)
{
  return set1.function() == set2.function();
}

// 非等価比較演算子
inline
bool
operator!=(const BddVarSet& set1,
	   const BddVarSet& set2)
{
  return ! operator==(set1, set2);
}

// set1 と set2 が集合として交わっていたら true を返す．
inline
bool
operator&&(const BddVarSet& set1,
	   const BddVarSet& set2)
{
  return vsintersect(set1.function(), set2.function());
}

// 集合として set1 が set2 を含んでいたら true を返す．
inline
bool
operator>=(const BddVarSet& set1,
	   const BddVarSet& set2)
{
  // 実は BDD の <= 演算となる．
  return set1.function() <= set2.function();
}

// >= の逆の演算子
inline
bool
operator<=(const BddVarSet& set1,
	   const BddVarSet& set2)
{
  return set2 >= set1;
}

// 集合として set1 が strict に set2 を含んでいたら true を返す．
inline
bool
operator>(const BddVarSet& set1,
	  const BddVarSet& set2)
{
  return set1.function() < set2.function();
}

// > の逆の演算子
inline
bool
operator<(const BddVarSet& set1,
	  const BddVarSet& set2)
{
  return set2 > set1;
}

END_NAMESPACE_YM_BDD

#endif // YM_BDD_BDDVARSET_H
