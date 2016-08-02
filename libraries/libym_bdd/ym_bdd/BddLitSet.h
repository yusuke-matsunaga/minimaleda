#ifndef YM_BDD_BDDLITSET_H
#define YM_BDD_BDDLITSET_H

/// @file ym_bdd/BddLitSet.h
/// @brief BddLitSet のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BddLitSet.h 693 2007-05-28 09:13:57Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_bdd/Bdd.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
/// @class BddLitSet BddLitSet.h <ym_bdd/BddLitSet.h>
/// @ingroup YmBdds
/// @brief リテラル集合( = cube)を表すクラス
///
/// 実は BDD を用いて実装している．
//////////////////////////////////////////////////////////////////////
class BddLitSet
{
  friend class Bdd;
public:
  // 反復子
  class iterator
  {
    friend class BddLitSet;
  public:
    // コンストラクタ
    iterator();
    
    // 現在指している要素を返す．
    // 末尾かどうかのチェックは行わない．
    Literal
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
    bool
    operator!=(const iterator& src) const;
    

  private:

    // BddLitSet 用のコンストラクタ
    iterator(const Bdd& bdd);
    
    // ひとつ先に進める．
    void
    next();


  private:
    //////////////////////////////////////////////////////////////////////
    // データメンバ
    //////////////////////////////////////////////////////////////////////

    // 現在指している節点
    Bdd mCur;

  };
  

public:

  // コンストラクタ
  // 空集合となるが，明示的に使う場合には下の make_empty() を使うべき
  BddLitSet(BddMgrRef mgr);
  
  // コピーコンストラクタ
  BddLitSet(const BddLitSet& src);
  
  // 代入演算子
  const BddLitSet&
  operator=(const BddLitSet& src);
  
  // リテラル1つだけを要素とする集合を作るコンストラクタ
  BddLitSet(BddMgrRef mgr,
	    tVarId varid,
	    tPol pol);
  BddLitSet(BddMgrRef mgr,
	    const Literal& lit);
  
  // vector からの変換用コンストラクタ
  BddLitSet(BddMgrRef mgr,
	    const LiteralVector& src);
  
  // list からの変換用コンストラクタ
  BddLitSet(BddMgrRef mgr,
	    const LiteralList& src);
  
  // 空集合を返すクラスメソッド
  static
  BddLitSet
  make_empty(BddMgrRef mgr);
  
  // 空の時に真となる
  bool
  empty() const;
  
  // 要素数を返す．
  size_t
  size() const;
  
  // リテラル関数を返す．
  Bdd
  function() const;
  
  // 先頭の反復子を返す．
  iterator
  begin() const;
  
  // 末尾の反復子を返す．
  iterator
  end() const;

  // リテラルの vector に変換する．
  // 順番は内部の実装に依存する．
  // 集合のサイズを返す．
  size_t
  to_vector(LiteralVector& dst) const;
  
  // リテラルのリストに変換する．
  // 順番は内部の実装に依存する．
  // 集合のサイズを返す．
  size_t
  to_list(LiteralList& dst) const;

  // 集合和を計算する．
  // 肯定のリテラルと否定のリテラルが両方含まれていた場合には
  // 空集合となる．
  BddLitSet
  operator+(const BddLitSet& set2) const;

  // 集合和を計算して代入する．
  // 肯定のリテラルと否定のリテラルが両方含まれていた場合には
  // 空集合となる．
  const BddLitSet&
  operator+=(const BddLitSet& set2);
  
  // 集合積を計算する．
  BddLitSet
  operator*(const BddLitSet& set2) const;
  
  // 集合積を計算して代入する．
  const BddLitSet&
  operator*=(const BddLitSet& set2);

  // 集合差を計算する．
  BddLitSet
  operator-(const BddLitSet& set2) const;

  // 集合差を計算して代入する．
  const BddLitSet&
  operator-=(const BddLitSet& set2);


private:

  // BDD を指定するコンストラクタ
  explicit
  BddLitSet(const Bdd& bdd);

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

  // リテラル集合を表すBDD
  Bdd mBody;
  
  // 要素数を入れておくキャッシュ
  // 最下位1ビットが1の時のみ valid な値を持つ．
  mutable
  size_t mSize;

};


//////////////////////////////////////////////////////////////////////
// BddLitSet 関係の non-member 関数の宣言
//////////////////////////////////////////////////////////////////////

// 等価比較演算子
bool
operator==(const BddLitSet& set1,
	   const BddLitSet& set2);

// 非等価比較演算子
bool
operator!=(const BddLitSet& set1,
	   const BddLitSet& set2);

// set1 と set2 が集合として交わっていたら true を返す．
bool
operator&&(const BddLitSet& set1,
	   const BddLitSet& set2);

// set1 が set2 を含んでいたら true を返す．
bool
operator>=(const BddLitSet& set1,
	   const BddLitSet& set2);

// set1 が set2 に含まれていたら true を返す．
bool
operator<=(const BddLitSet& set1,
	   const BddLitSet& set2);

// set1 が真に set2 を含んでいたら true を返す．
bool
operator>(const BddLitSet& set1,
	  const BddLitSet& set2);

// set1 が真に set2 に含まれていたら true を返す．
bool
operator<(const BddLitSet& set1,
	  const BddLitSet& set2);

// ストリームに出力する．
ostream&
operator<<(ostream& s,
	   const BddLitSet& litset);


//////////////////////////////////////////////////////////////////////
// BddLitSet のインライン関数定義
//////////////////////////////////////////////////////////////////////

// リテラル関数を返す．
inline
Bdd
BddLitSet::function() const
{
  return mBody;
}

// 等価比較演算子
inline
bool
operator==(const BddLitSet& set1,
	   const BddLitSet& set2)
{
  return set1.function() == set2.function();
}

// 非等価比較演算子
inline
bool
operator!=(const BddLitSet& set1,
	   const BddLitSet& set2)
{
  return ! operator==(set1, set2);
}

// set1 と set2 が集合として交わっていたら true を返す．
inline
bool
operator&&(const BddLitSet& set1,
	   const BddLitSet& set2)
{
  return lsintersect(set1.function(), set2.function());
}

// 集合として set1 が set2 を含んでいたら true を返す．
inline
bool
operator>=(const BddLitSet& set1,
	   const BddLitSet& set2)
{
  // 実は BDD の <= 演算となる．
  return set1.function() <= set2.function();
}

// >= の逆の演算子
inline
bool
operator<=(const BddLitSet& set1,
	   const BddLitSet& set2)
{
  return set2 >= set1;
}

// 集合として set1 が strict に set2 を含んでいたら true を返す．
inline
bool
operator>(const BddLitSet& set1,
	  const BddLitSet& set2)
{
  return set1.function() > set2.function();
}

// > の逆の演算子
inline
bool
operator<(const BddLitSet& set1,
	  const BddLitSet& set2)
{
  return set2 > set1;
}

END_NAMESPACE_YM_BDD

#endif // YM_BDD_BDDLITSET_H
