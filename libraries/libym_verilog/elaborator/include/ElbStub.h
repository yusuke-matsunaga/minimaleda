#ifndef LIBYM_VERILOG_ELABORATOR_ELBSTUB_H
#define LIBYM_VERILOG_ELABORATOR_ELBSTUB_H

/// @file libym_verilog/elaborator/ElbStub.h
/// @brief ElbStub のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ElbStub.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/verilog.h"


BEGIN_NAMESPACE_YM_VERILOG

class ElbStubList;

//////////////////////////////////////////////////////////////////////
/// @class ElbStub ElbStub.h "ElbStub.h"
/// @brief インスタンス化で用いる stub クラス
//////////////////////////////////////////////////////////////////////
class ElbStub
{
  friend class ElbStubList;

public:

  /// @brief コンストラクタ
  ElbStub();

  /// @brief 仮想デストラクタ
  virtual
  ~ElbStub();

  /// @brief 処理を行う．
  virtual
  void
  eval() = 0;
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 次の要素を指すリンク
  ElbStub* mNextLink;
  
};


//////////////////////////////////////////////////////////////////////
/// @class ElbStubList ElbStub.h "ElbStub.h"
/// @brief ElbStub のリストを表すクラス
//////////////////////////////////////////////////////////////////////
class ElbStubList
{
public:

  /// @brief コンストラクタ
  ElbStubList();

  /// @brief デストラクタ
  ~ElbStubList();
  

public:
  
  /// @brief 末尾に要素を追加する．
  void
  push_back(ElbStub* elem);

  /// @brief 空の時 true を返す．
  bool
  empty() const;

  /// @brief リストの移し替えを行なう．
  /// @note 結果として src_list は空になる．
  void
  move(ElbStubList& src_list);

  /// @brief 要素の stub を評価する．
  /// @note 結果としてリストは空になる．
  void
  eval();

  /// @brief 内容を空にする．
  /// @note 中の要素のメモリは開放されない．
  void
  clear();
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の要素
  ElbStub* mTop;

  // 末尾の要素
  ElbStub* mTail;

};


//////////////////////////////////////////////////////////////////////
/// @name 個々のクラスごとの ElbStub クラスを作るためのテンプレートクラス
//////////////////////////////////////////////////////////////////////
//@{

/// @brief 引数が1つのタイプ
template<typename T,
	 typename A>
class ElbStubT1 :
  public ElbStub
{
public:

  /// @brief 引数が1つの ElbMgrImpl のメンバ関数
  typedef void (T::*ElbFunc)(A);

public:

  /// @brief コンストラクタ
  /// @param[in] obj 対象のオブジェクト
  /// @param[in] memfunc メンバ関数へのポインタ
  /// @param[in] a memfunc を起動するときの引数
  ElbStubT1(T* obj,
	    ElbFunc memfunc,
	    A a);

  /// @brief デストラクタ
  virtual
  ~ElbStubT1();

  /// @brief 処理を行う．
  /// @note obj->memfunc(a) を実行する．
  virtual
  void
  eval();
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // T のオブジェクト
  T* mObj;

  // T のメンバ関数
  ElbFunc mMemFunc;
  
  // mMemFunc に渡される引数
  A mA;

};


/// @brief 引数が2つのタイプ
template<typename T,
	 typename A,
	 typename B>
class ElbStubT2 :
  public ElbStub
{
public:

  /// @brief 引数が2つの ElbMgrImpl のメンバ関数
  typedef void (T::*ElbFunc)(A, B);

public:

  /// @brief コンストラクタ
  /// @param[in] obj 対象のオブジェクト
  /// @param[in] memfunc メンバ関数へのポインタ
  /// @param[in] a, b memfunc を起動するときの引数
  ElbStubT2(T* obj,
	    ElbFunc memfunc,
	    A a,
	    B b);

  /// @brief デストラクタ
  virtual
  ~ElbStubT2();

  /// @brief 処理を行う．
  /// @note obj->memfunc(a, b) を実行する．
  virtual
  void
  eval();
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // T のオブジェクト
  T* mObj;

  // ElbMgrImpl のメンバ関数
  ElbFunc mMemFunc;
  
  // mMemFunc に渡される引数
  A mA;
  B mB;
};


/// @brief 引数が3つのタイプ
template<typename T,
	 typename A,
	 typename B,
	 typename C>
class ElbStubT3 :
  public ElbStub
{
public:

  /// @brief 引数が3つの ElbMgrImpl のメンバ関数
  typedef void (T::*ElbFunc)(A, B, C);

public:

  /// @brief コンストラクタ
  /// @param[in] obj 対象のオブジェクト
  /// @param[in] memfunc メンバ関数へのポインタ
  /// @param[in] a, b, c memfunc を起動するときの引数
  ElbStubT3(T* obj,
	    ElbFunc memfunc,
	    A a,
	    B b,
	    C c);

  /// @brief デストラクタ
  virtual
  ~ElbStubT3();

  /// @brief 処理を行う．
  /// @note obj->memfunc(a, b, c) を実行する．
  virtual
  void
  eval();
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // T のオブジェクト
  T* mObj;

  // ElbMgrImpl のメンバ関数
  ElbFunc mMemFunc;
  
  // mMemFunc に渡される引数
  A mA;
  B mB;
  C mC;
};


/// @brief 引数が4つのタイプ
template<typename T,
	 typename A,
	 typename B,
	 typename C,
	 typename D>
class ElbStubT4 :
  public ElbStub
{
public:

  /// @brief 引数が4つの ElbMgrImpl のメンバ関数
  typedef void (T::*ElbFunc)(A, B, C, D);

public:

  /// @brief コンストラクタ
  /// @param[in] obj 対象のオブジェクト
  /// @param[in] memfunc メンバ関数へのポインタ
  /// @param[in] a, b, c, d memfunc を起動するときの引数
  ElbStubT4(T* obj,
	    ElbFunc memfunc,
	    A a,
	    B b,
	    C c,
	    D d);

  /// @brief デストラクタ
  virtual
  ~ElbStubT4();

  /// @brief 処理を行う．
  /// @note obj->memfunc(a, b, c, d) を実行する．
  virtual
  void
  eval();
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // T のオブジェクト
  T* mObj;

  // ElbMgrImpl のメンバ関数
  ElbFunc mMemFunc;
  
  // mMemFunc に渡される引数
  A mA;
  B mB;
  C mC;
  D mD;
};

//@}


//////////////////////////////////////////////////////////////////////
// 実装
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
ElbStub::ElbStub() :
  mNextLink(NULL)
{
}

// @brief 仮想デストラクタ
inline
ElbStub::~ElbStub()
{
}

// @brief コンストラクタ
inline
ElbStubList::ElbStubList() :
  mTop(NULL),
  mTail(NULL)
{
}

// @brief デストラクタ
inline
ElbStubList::~ElbStubList()
{
}

// @brief 末尾に要素を追加する．
inline
void
ElbStubList::push_back(ElbStub* elem)
{
  if ( mTail ) {
    mTail->mNextLink = elem;
    mTail = elem;
  }
  else {
    mTop = elem;
    mTail = elem;
  }
}

// @brief 空の時 true を返す．
inline
bool
ElbStubList::empty() const
{
  return mTop == NULL;
}

// @brief リストの移し替えを行なう．
// @note 結果として src_list は空になる．
inline
void
ElbStubList::move(ElbStubList& src_list)
{
  mTop = src_list.mTop;
  mTail = src_list.mTail;
  src_list.mTop = NULL;
  src_list.mTail = NULL;
}

// @brief 要素の stub を評価する．
// @note 結果としてリストは空になる．
inline
void
ElbStubList::eval()
{
  for (ElbStub* stub = mTop; stub; ) {
    ElbStub* next = stub->mNextLink;
    stub->eval();
    stub = next;
  }
  mTop = NULL;
  mTail = NULL;
}

// @brief 内容を空にする．
// @note 中の要素のメモリは開放されない．
inline
void
ElbStubList::clear()
{
  mTop = NULL;
  mTail = NULL;
}

// @brief コンストラクタ
// @param[in] obj 対象のオブジェクト
// @param[in] memfunc メンバ関数へのポインタ
// @param[in] a memfunc を起動するときの引数
template<typename T,
	 typename A>
ElbStubT1<T, A>::ElbStubT1(T* obj,
			   ElbFunc memfunc,
			   A a) :
  mObj(obj),
  mMemFunc(memfunc),
  mA(a)
{
}

// @brief デストラクタ
template<typename T,
	 typename A>
ElbStubT1<T, A>::~ElbStubT1()
{
}

// @brief 処理を行う．
template<typename T,
	 typename A>
void
ElbStubT1<T, A>::eval()
{
  (mObj->*(mMemFunc))(mA);
}

// @brief コンストラクタ
// @param[in] memfunc メンバ関数へのポインタ
// @param[in] obj 対象のオブジェクト
// @param[in] a, b memfunc を起動するときの引数
template<typename T,
	 typename A,
	 typename B>
ElbStubT2<T, A, B>::ElbStubT2(T* obj,
			      ElbFunc memfunc,
			      A a,
			      B b) :
  mObj(obj),
  mMemFunc(memfunc),
  mA(a),
  mB(b)
{
}

// @brief デストラクタ
template<typename T,
	 typename A,
	 typename B>
ElbStubT2<T, A, B>::~ElbStubT2()
{
}

// @brief 処理を行う．
template<typename T,
	 typename A,
	 typename B>
void
ElbStubT2<T, A, B>::eval()
{
  (mObj->*(mMemFunc))(mA, mB);
}

// @brief コンストラクタ
// @param[in] memfunc メンバ関数へのポインタ
// @param[in] obj 対象のオブジェクト
// @param[in] a, b, c memfunc を起動するときの引数
template<typename T,
	 typename A,
	 typename B,
	 typename C>
ElbStubT3<T, A, B, C>::ElbStubT3(T* obj,
				 ElbFunc memfunc,
				 A a,
				 B b,
				 C c) :
  mObj(obj),
  mMemFunc(memfunc),
  mA(a),
  mB(b),
  mC(c)
{
}

// @brief デストラクタ
template<typename T,
	 typename A,
	 typename B,
	 typename C>
ElbStubT3<T, A, B, C>::~ElbStubT3()
{
}

// @brief 処理を行う．
template<typename T,
	 typename A,
	 typename B,
	 typename C>
void
ElbStubT3<T, A, B, C>::eval()
{
  (mObj->*(mMemFunc))(mA, mB, mC);
}

// @brief コンストラクタ
// @param[in] memfunc メンバ関数へのポインタ
// @param[in] obj 対象のオブジェクト
// @param[in] a, b, c, d memfunc を起動するときの引数
template<typename T,
	 typename A,
	 typename B,
	 typename C,
	 typename D>
ElbStubT4<T, A, B, C, D>::ElbStubT4(T* obj,
				    ElbFunc memfunc,
				    A a,
				    B b,
				    C c,
				    D d) :
  mObj(obj),
  mMemFunc(memfunc),
  mA(a),
  mB(b),
  mC(c),
  mD(d)
{
}

// @brief デストラクタ
template<typename T,
	 typename A,
	 typename B,
	 typename C,
	 typename D>
ElbStubT4<T, A, B, C, D>::~ElbStubT4()
{
}

// @brief 処理を行う．
template<typename T,
	 typename A,
	 typename B,
	 typename C,
	 typename D>
void
ElbStubT4<T, A, B, C, D>::eval()
{
  (mObj->*(mMemFunc))(mA, mB, mC, mD);
}

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELABORATOR_ELBSTUB_H
