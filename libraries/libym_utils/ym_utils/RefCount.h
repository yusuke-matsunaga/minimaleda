#ifndef YM_UTILS_REFCOUNT_H
#define YM_UTILS_REFCOUNT_H

/// @file ym_utils/RefCount.h
/// @brief RefCount のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: RefCount.h 1143 2008-01-04 07:27:17Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class RefCount RefCount.h <ym_utils/RefCount>
/// @ingroup YmUtils
/// @brief 参照回数を持つオブジェクトの基底クラス
/// @note
/// - inc(), dec()によってその参照回数が増減される．
/// - 参照回数が0になったらそのオブジェクトは delete される．
/// - もちろん，自動変数のオブジェクトは delete できないので，自動
/// 変数に対して dec() してはいけない．
/// - ということは自動変数でこのクラスのオブジェクトを作っても
/// 使い道がない．
//////////////////////////////////////////////////////////////////////
class RefCount
{
public:

  /// @brief 空のコンストラクタ
  RefCount();
  
  /// @brief コピーコンストラクタ
  /// @note とはいってもやることは上と同じ．
  /// 参照回数はコピーしないことに注意
  RefCount(const RefCount&);
  
  /// @brief デストラクタ
  /// @note このクラスは継承されることを前提としているので
  /// 必ず仮想関数にしておかなければならない．
  virtual
  ~RefCount();
  
  /// @brief 参照回数を得る．
  /// @return 参照回数
  int
  refcount() const;
  

protected:
  
  /// @brief 参照回数が 0 になったときの処理関数
  /// @note デフォルトでは delete this;
  virtual
  void
  suicide();
  
  /// @brief 参照回数を増やす．
  void
  inc();
  
  /// @brief 参照回数を減らす．0になったら削除する．
  void
  dec();
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 参照回数
  int mCount;

};


//////////////////////////////////////////////////////////////////////
/// @class SmartPtr RefCount.h <ym_utils/RefCount.h>
/// @ingroup YmUtils
/// @brief RefCountから派生したクラスのオブジェクトを指す
/// スマートポインタクラスのテンプレート定義
/// @note テンプレート引数 T で与えられるクラスが RefCount の派生
/// クラスでなかった場合にはコンパイル時にエラーとなる．
//////////////////////////////////////////////////////////////////////
template <typename T>
class SmartPtr
{
public:

  /// @brief コンストラクタ
  /// @param[in] ptr ポインタ
  /// @note と同時にキャスト演算子でもある．
  SmartPtr(T* ptr = 0);
  
  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  SmartPtr(const SmartPtr& src);
  
  /// @brief デストラクタ
  ~SmartPtr();
  
  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 自分自身
  const SmartPtr&
  operator=(const SmartPtr& src);
  
  /// @brief NULL ポインタとの比較
  /// @return NULL ポインタと等しい時に true を返す．
  bool
  null() const;
  
  /// @brief null() と同義の演算子
  /// @return NULL ポインタと等しい時に true を返す．
  bool
  operator!() const;

  /// @brief ポインタを取出す演算子
  operator
  T*() const;

  /// @brief T*() と同義の演算子
  T*
  operator->() const;


private:
  
  // ポインタをセットする．
  // オブジェクトの参照回数を増やす．
  void
  set(T* ptr);
  
  // 今までのポインタを外す．
  // オブジェクトの参照回数が0になったら削除する．
  void
  unset();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実際のポインタ
  T* mPtr;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// 空のコンストラクタ
inline
RefCount::RefCount() :
  mCount(0)
{
}
  
// コピーコンストラクタ
inline
RefCount::RefCount(const RefCount&) :
  mCount(0)
{
}
  
// 参照回数を得る．
inline
int
RefCount::refcount() const
{
  return mCount;
}
  
// 参照回数を増やす．
inline
void
RefCount::inc()
{
  ++ mCount;
}
  
// 参照回数を減らす．0になったら削除する．
inline
void
RefCount::dec()
{
  if ( -- mCount == 0 )
    suicide();
}

// @brief コンストラクタ
// @param[in] ptr ポインタ
// @note と同時にキャスト演算子でもある．
template<typename T>
inline
SmartPtr<T>::SmartPtr(T* ptr)
{
  set(ptr);
}
  
// @brief コピーコンストラクタ
// @param[in] src コピー元のオブジェクト
template<typename T>
inline
SmartPtr<T>::SmartPtr(const SmartPtr& src)
{
  set(src.mPtr);
}
  
// @brief デストラクタ
template<typename T>
inline
SmartPtr<T>::~SmartPtr()
{
  unset();
}
  
// @brief 代入演算子
// @param[in] src コピー元のオブジェクト
// @return 自分自身
template<typename T>
inline
const SmartPtr<T>&
SmartPtr<T>::operator=(const SmartPtr& src)
{
  // この順番なら自分自身を代入してもOK
  if ( src.mPtr ) {
    src.mPtr->inc();
  }
  if ( mPtr ) {
    mPtr->dec();
  }
  mPtr = src.mPtr;
  return *this;
}
  
// @brief NULL ポインタとの比較
// @return NULL ポインタと等しい時に true を返す．
template<typename T>
inline
bool
SmartPtr<T>::null() const
{
  return mPtr == 0;
}
  
// @brief null() と同義の演算子
// @return NULL ポインタと等しい時に true を返す．
template<typename T>
inline
bool
SmartPtr<T>::operator!() const
{
  return mPtr == 0;
}

// @brief ポインタを取出す演算子
template<typename T>
inline
SmartPtr<T>::operator T*() const
{
  return mPtr;
}

// @brief T*() と同義の演算子
template<typename T>
inline
T*
SmartPtr<T>::operator->() const
{
  return mPtr;
}
  
// ポインタをセットする．
// オブジェクトの参照回数を増やす．
template<typename T>
inline
void
SmartPtr<T>::set(T* ptr)
{
  mPtr = ptr;
  if ( mPtr ) {
    mPtr->inc();
  }
}
  
// 今までのポインタを外す．
// オブジェクトの参照回数が0になったら削除する．
template<typename T>
inline
void
SmartPtr<T>::unset()
{
  if ( mPtr ) {
    mPtr->dec();
    mPtr = 0;
  }
}

END_NAMESPACE_YM

#endif // YM_UTILS_REFCOUNT_H
