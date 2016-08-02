#ifndef LIBYM_UTILS_HEAPTREE_H
#define LIBYM_UTILS_HEAPTREE_H

/// @file ym_utils/HeapTree.h
/// @brief HeapTree のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: HeapTree.h 1343 2008-03-25 17:15:35Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class HeapTree HeapTree.h <ym_utils/HeapTree.h>
/// @ingroup YmUtils
/// @brief ベーシックなヒープ木のテンプレートクラス
/// @note テンプレート引数で要素の型と要素の比較関数の型を指定する．
/// 比較関数クラスは int operator()(Node a, Node b);
/// の形の関数を定義している必要がある．
//////////////////////////////////////////////////////////////////////
template <typename Node,
	  typename CompFunc>
class HeapTree
{
public:

  /// @brief コンストラクタ
  /// @param[in] size 予想される入力の個数
  /// @note 実際に size 以上の入力があっても正しく動く
  HeapTree(size_t size = 16) :
    mSize(size),
    mNum(0),
    mArray(new Node[size])
  {
  }
  
  /// @brief デストラクタ
  ~HeapTree()
  {
    delete [] mArray;
  }
  
  /// @brief 空にする．
  void
  clear()
  {
    mNum = 0;
  }
  
  /// @brief 要素が空の時 true を返す．
  bool
  empty() const
  {
    return mNum == 0;
  }
  
  /// @brief 要素を追加する．
  void
  put(Node t)
  {
    push_back(t);
    size_t pos = mNum - 1;
    while ( pos > 0 ) {
      Node& val = mArray[pos];
      size_t parent = (pos - 1) >> 1;
      Node& val_p = mArray[parent];
      if ( mCompFunc(val_p, val) > 0 ) {
	Node tmp = val_p;
	val_p = val;
	val = tmp;
	pos = parent;
      }
      else {
	pos = 0;
      }
    }
  }
  
  /// @brief 最小要素を返す．
  Node
  getmin() const
  {
    return mArray[0];
  }
  
  /// @brief 最小要素をとりのぞく．
  void
  popmin()
  {
    if ( mNum > 1 ) {
      -- mNum;
      mArray[0] = mArray[mNum];
      pushdown(0);
    }
    else {
      // mNum == 0 の時は無駄な処理だが if 文を入れるよりまし
      mNum = 0;
    }
  }
  
  /// @brief 最小要素の代わりに src を追加する．
  /// @return 新しい最小要素を返す．
  Node
  xchgmin(Node t)
  {
    mArray[0] = t;
    pushdown(0);
    Node ans = mArray[0];
    -- mNum;
    mArray[0] = mArray[mNum];
    pushdown(0);
    return ans;
  }
  

private:
  //////////////////////////////////////////////////////////////////////
  // 内部でのみ用いられる関数
  //////////////////////////////////////////////////////////////////////

  // 要素をひとつ追加する
  void
  push_back(Node t)
  {
    if ( mSize <= mNum ) {
      size_t old_size = mSize;
      Node* old_array = mArray;
      mSize <<= 1;
      mArray = new Node[mSize];
      for (size_t i = 0; i < old_size; ++ i) {
	mArray[i] = old_array[i];
      }
      delete [] old_array;
    }
    mArray[mNum] = t;
    ++ mNum;
  }
  
  // 引数の要素を適当な位置まで沈めてゆく再帰的関数
  void
  pushdown(size_t first)
  {
    size_t parent = first;
    for ( ; ; ) {
      // ヒープ木の性質から親から子の位置がわかる
      size_t left = parent + parent + 1;
      size_t right = left + 1;
      if ( right > mNum ) {
	// 左右の子どもを持たない場合
	break;
      }
      Node& val_p = mArray[parent];
      Node& val_l = mArray[left];
      if ( right == mNum ) {
	// 右の子どもを持たない場合
	if ( mCompFunc(val_p, val_l) > 0 ) {
	  // 逆転
	  Node tmp = val_p;
	  val_p = val_l;
	  val_l = tmp;
	}
	break;
      }
      else {
	Node& val_r = mArray[right];
	if ( mCompFunc(val_p, val_l) > 0 &&
	     mCompFunc(val_l, val_r) <= 0 ) {
	  // 左の子どもと取り替える．次は左の子で同じ処理をする
	  Node tmp = val_p;
	  val_p = val_l;
	  val_l = tmp;
	  parent = left;
	}
	else if ( mCompFunc(val_p, val_r) > 0 &&
		  mCompFunc(val_r, val_l) < 0 ) {
	  // 右の子どもと取り替える．次は右の子で同じ処理をする
	  Node tmp = val_p;	
	  val_p = val_r;
	  val_r = tmp;
	  parent = right;
	}
	else {
	  break;
	}
      }
    }
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 比較関数オブジェクト
  CompFunc mCompFunc;

  // 配列のサイズ
  size_t mSize;

  // 要素数
  size_t mNum;

  //　配列本体
  Node* mArray;

};


//////////////////////////////////////////////////////////////////////
/// @class SimpleHeapTree HeapTree.h <ym_utils/HeapTree.h>
/// @ingroup YmUtils
/// @brief 簡単に使えるヒープ木のテンプレートクラス
/// @note ヒープ木に要素と比較用の値を保持する．
//////////////////////////////////////////////////////////////////////
template <typename T>
class SimpleHeapTree
{
private:

  // ノードの定義
  struct Node
  {
    T mBody;
    int mVal;
    
    Node() { }
    Node(T t,
	 int val) :
      mBody(t),
      mVal(val) { }
  };

  // 比較関数の定義
  struct CompFunc
  {
    int
    operator()(Node n1,
	       Node n2)
    {
      return n1.mVal - n2.mVal;
    }
  };
  

public:

  /// @brief コンストラクタ
  /// @param[in] size 予想される入力の個数
  /// @note 実際に size 以上の入力があっても正しく動く
  SimpleHeapTree(size_t size = 16) :
    mBody(size)
  {
  }
  
  /// @brief デストラクタ
  ~SimpleHeapTree()
  {
  }
  
  /// @brief 空にする．
  void
  clear()
  {
    mBody.clear();
  }
  
  /// @brief 要素が空の時 true を返す．
  bool
  empty() const
  {
    return mBody.empty();
  }
  
  /// @brief 要素を追加する．
  void
  put(T t,
      int val)
  {
    mBody.put(Node(t, val));
  }
  
  /// @brief 最小要素を返す．
  T
  getmin() const
  {
    return mBody.getmin().mBody;
  }
  
  /// @brief 最小要素をとりのぞく．
  void
  popmin()
  {
    mBody.popmin();
  }
  
  /// @brief 最小要素の代わりに src を追加する．
  /// @return 新しい最小要素を返す．
  T
  xchgmin(T t,
	  int val)
  {
    Node ans = mBody.xchgmin(Node(t, val));
    return ans.mBody;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 本当のヒープ木
  HeapTree<Node, CompFunc> mBody;

};

END_NAMESPACE_YM

#endif
