#ifndef YM_UTILS_BINDER_H
#define YM_UTILS_BINDER_H

/// @file ym_utils/Binder.h
/// @brief Binder 関連クラスのヘッダファイル
/// @author Yusuke Matsunaga
///
/// $Id: Binder.h 1343 2008-03-25 17:15:35Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

/// @defgroup BinderGroup 汎用バインダ
/// @ingroup YmUtils
///
/// オブジェクトの動作を他のオブジェクトに関連づけるメカニズム
///
/// よくあるデザインパターンの一種で，クラス A のオブジェクトのある動作
/// にクラス B のオブジェクトのある動作を関連づけたいが，
/// クラス A のコード中に直接 クラス B のメンバ関数の呼び出しを書いて
/// しまうと依存関係が生じてしまうのでやりたくない，という時に用いる．
/// 具体的にはクラス A 側に BindMgr (の派生クラス) を用意しておいて
/// クラス A から呼び出してもらいたいクラスはバインダーをこの BindMgr
/// に登録する．クラス A の BindMgr はイベントを起動するタイミングで
/// 登録されているバインダーに対してイベントの通知を行う．
///
/// ここではイベントのみを伝える EventBinder とn個(nは 1 ... 6)の
/// データを伝えるテンプレートクラス T<n>Binder を用意している．


#include "ymtools.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラスの先行宣言
//////////////////////////////////////////////////////////////////////

class BindMgr;

class EventBindMgr;

template <typename T>
class T1BindMgr;

template <typename T1,
	  typename T2>
class T2BindMgr;

template <typename T1,
	  typename T2,
	  typename T3>
class T3BindMgr;

template <typename T1,
	  typename T2,
	  typename T3,
	  typename T4>
class T4BindMgr;

template <typename T1,
	  typename T2,
	  typename T3,
	  typename T4,
	  typename T5>
class T5BindMgr;

template <typename T1,
	  typename T2,
	  typename T3,
	  typename T4,
	  typename T5,
	  typename T6>
class T6BindMgr;


//////////////////////////////////////////////////////////////////////
/// @class Binder Binder.h <ym_utils/Binder.h>
/// @ingroup BinderGroup
/// @brief 他のオブジェクトの動作に関連づけるための基底クラス
/// @sa BindMgr
//////////////////////////////////////////////////////////////////////
class Binder
{
  friend class BindMgr;
public:

  /// @brief コンストラクタ
  /// @note この時点では特定の BindMgr には結び付いていない．
  Binder();
  
  /// @brief デストラクタ
  /// @note BindMgr にバインドしていたらバインドを解除する．
  virtual
  ~Binder();
  
  /// @brief BindMgr の取得
  /// @retval バインドしている BindMgr　バインドしている場合
  /// @retval NULL バインドしていない場合
  BindMgr*
  mgr() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // リストの削除に用いる反復子
  list<Binder*>::iterator mItForList;

  // このオブジェクトを持っているクラス
  BindMgr* mMgr;

};


//////////////////////////////////////////////////////////////////////
/// @class BindMgr Binder.h <ym_utils/Binder.h>
/// @ingroup BinderGroup
/// @brief Binder を起動するクラスの基底クラス
/// @sa Binder
//////////////////////////////////////////////////////////////////////
class BindMgr
{
  friend class Binder;

public:

  /// @brief コンストラクタ
  BindMgr();
  
  /// @brief デストラクタ
  /// @note バインド中のすべての Binder オブジェクトとのバインドを
  /// 解除する．
  /// @note Binder オブジェクトそのものは削除しない．
  virtual
  ~BindMgr();


protected:
  
  /// @brief binder の登録
  /// @param[in] binder 登録する Binder
  void
  _reg_binder(Binder* binder);
  
  /// @brief binder の登録の解除
  /// @param[in] binder 登録を解除する Binder
  void
  _unreg_binder(Binder* binder);

  
protected:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  /// @brief バインドされた binder のリスト
  list<Binder*> mList;

};


//////////////////////////////////////////////////////////////////////
/// @class EventBinder Binder.h <ym_utils/Binder.h>
/// @ingroup BinderGroup
/// @brief 引数なしのイベントを伝播するためのクラス
/// @note 引数がある場合には下の TBinder を使うこと.
/// 実際に使うには event_proc() を上書きした継承クラスを用意する
/// 必要がある.
/// @sa EventBindMgr
/// @sa T1Binder, T2Binder, T3Binder, T4Binder, T5Binder, T6Binder
//////////////////////////////////////////////////////////////////////
class EventBinder :
  public Binder
{
  friend class EventBindMgr;

private:

  /// @brief イベント処理関数
  /// @note 継承クラスが実装しなければならない．
  virtual
  void
  event_proc() = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class EventBindMgr Binder.h <ym_utils/Binder.h>
/// @ingroup BinderGroup
/// @brief EventBinder の継承クラスを起動するクラス
/// @note reg_binder() でバインダーオブジェクトを登録して
/// prop_event() でイベントを伝播する．
/// @sa EventBinder
//////////////////////////////////////////////////////////////////////
class EventBindMgr :
  private BindMgr
{
public:

  /// @brief コンストラクタ
  EventBindMgr();

  /// @brief デストラクタ
  virtual
  ~EventBindMgr();
  
  /// @brief EventBinder を登録する．
  /// @param[in] binder 登録する EventBinder
  void
  reg_binder(EventBinder* binder);
  
  /// @brief EventBinder の登録を削除する．
  /// @param[in] binder 登録を削除する EventBinder
  /// @note binder が登録されていない場合には何もしない．
  void
  unreg_binder(EventBinder* binder);

  /// @brief ここに登録されたすべての binder にイベントを伝える．
  void
  prop_event();

};


//////////////////////////////////////////////////////////////////////
/// @class T1Binder Binder.h <ym_utils/Binder.h>
/// @ingroup BinderGroup
/// @brief 引数を1つ持つ Binder
/// @sa T1BindMgr
//////////////////////////////////////////////////////////////////////
template<typename T1>
class T1Binder :
  public Binder
{
  friend class T1BindMgr<T1>;
private:

  /// @brief イベント処理関数
  /// @param[in] obj ブロードキャストする情報を表すオブジェクト
  virtual
  void
  event_proc(T1 obj) = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class T1BindMgr Binder.h <ym_utils/Binder.h>
/// @ingroup BinderGroup
/// @brief T1Binder を起動するクラス
/// @sa T1Binder
//////////////////////////////////////////////////////////////////////
template<typename T1>
class T1BindMgr :
  private BindMgr
{
public:

  typedef T1Binder<T1> TBinder;

public:

  /// @brief コンストラクタ
  T1BindMgr() { }

  /// @brief デストラクタ
  ~T1BindMgr() { }
  
  /// @brief TBinder の登録を削除する．
  /// @param[in] binder 登録を削除する対象
  /// @note binder が登録されていない場合には何もしない．
  void
  unreg_binder(TBinder* binder) { _unreg_binder(binder); }
  
  /// @brief TBinder を登録する．
  /// @param[in] binder 登録する対象
  void
  reg_binder(TBinder* binder) { _reg_binder(binder); }
  
  /// @brief ここに登録されたすべての binder に T1 のオブジェクトを伝える．
  /// @param[in] data ブロードキャストする内容
  void
  prop_event(T1 data)
  {
    for (list<Binder*>::iterator p = mList.begin(); p != mList.end(); ++ p) {
      // 本当はよくない static_cast だが reg_binder() で登録できるのは
      // TBinder だけなので大丈夫なはず
      TBinder* binder = static_cast<TBinder*>(*p);
      binder->event_proc(data);
    }
  }
};


//////////////////////////////////////////////////////////////////////
/// @class T2Binder Binder.h <ym_utils/Binder.h>
/// @ingroup BinderGroup
/// @brief 引数を2つ持つ Binder
/// @sa T2BindMgr
//////////////////////////////////////////////////////////////////////
template<typename T1,
	 typename T2>
class T2Binder :
  public Binder
{
  friend class T2BindMgr<T1, T2>;
private:

  /// @brief イベント処理関数
  /// @param[in] obj1, obj2 ブロードキャストする情報を表すオブジェクト
  virtual
  void
  event_proc(T1 obj1,
	     T2 obj2) = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class T2BindMgr Binder.h <ym_utils/Binder.h>
/// @ingroup BinderGroup
/// @brief T2Binder を起動するクラス
/// @sa T2Binder
//////////////////////////////////////////////////////////////////////
template<typename T1,
	 typename T2>
class T2BindMgr :
  private BindMgr
{
public:

  typedef T2Binder<T1, T2> TBinder;

public:

  /// @brief コンストラクタ
  T2BindMgr() { }

  /// @brief デストラクタ
  ~T2BindMgr() { }
  
  /// @brief TBinder の登録を削除する．
  /// @param[in] binder 登録を削除する対象
  /// @note binder が登録されていない場合には何もしない．
  void
  unreg_binder(TBinder* binder) { _unreg_binder(binder); }
  
  /// @brief TBinder を登録する．
  /// @param[in] binder 登録する対象
  void
  reg_binder(TBinder* binder) { _reg_binder(binder); }
  
  /// @brief ここに登録されたすべての binder に (T1, T2)
  /// @brief のオブジェクトを伝える．
  /// @param[in] data1, data2 ブロードキャストする内容
  void
  prop_event(T1 data1,
	     T2 data2)
  {
    for (list<Binder*>::iterator p = mList.begin(); p != mList.end(); ++ p) {
      // 本当はよくない static_cast だが reg_binder() で登録できるのは
      // TBinder だけなので大丈夫なはず
      TBinder* binder = static_cast<TBinder*>(*p);
      binder->event_proc(data1, data2);
    }
  }
};


//////////////////////////////////////////////////////////////////////
/// @class T3Binder Binder.h <ym_utils/Binder.h>
/// @ingroup BinderGroup
/// @brief 引数を3つ持つ Binder
/// @sa T3BindMgr
//////////////////////////////////////////////////////////////////////
template<typename T1,
	 typename T2,
	 typename T3>
class T3Binder :
  public Binder
{
  friend class T3BindMgr<T1, T2, T3>;
private:

  /// @brief イベント処理関数
  /// @param[in] obj1, obj2, obj3 ブロードキャストする情報を表すオブジェクト
  virtual
  void
  event_proc(T1 obj1,
	     T2 obj2,
	     T3 obj3) = 0;
  
};


//////////////////////////////////////////////////////////////////////
/// @class T3BindMgr Binder.h <ym_utils/Binder.h>
/// @ingroup BinderGroup
/// @brief T3Binder を起動するクラス
/// @sa T3Binder
//////////////////////////////////////////////////////////////////////
template<typename T1,
	 typename T2,
	 typename T3>
class T3BindMgr :
  private BindMgr
{
public:

  typedef T3Binder<T1, T2, T3> TBinder;

public:

  /// @brief コンストラクタ
  T3BindMgr() { }

  /// @brief デストラクタ
  ~T3BindMgr() { }
  
  /// @brief TBinderの登録を削除する．
  /// @param[in] binder 登録を削除する対象
  /// @note binder が登録されていない場合には何もしない．
  void
  unreg_binder(TBinder* binder) { _unreg_binder(binder); }
  
  /// @brief TBinder を登録する．
  /// @param[in] binder 登録する対象
  void
  reg_binder(TBinder* binder) { _reg_binder(binder); }
  
  /// @brief ここに登録されたすべての binder に (T1, T2, T3)
  /// @brief のオブジェクトを伝える．
  /// @param[in] data1, data2, data3 ブロードキャストする内容
  void
  prop_event(T1 data1,
	     T2 data2,
	     T3 data3)
  {
    for (list<Binder*>::iterator p = mList.begin();
	 p != mList.end(); ++ p) {
      // 本当はよくない static_cast だが reg_binder() で登録できるのは
      // TBinder だけなので大丈夫なはず
      TBinder* binder = static_cast<TBinder*>(*p);
      binder->event_proc(data1, data2, data3);
    }
  }
};


//////////////////////////////////////////////////////////////////////
/// @class T4Binder Binder.h <ym_utils/Binder.h>
/// @ingroup BinderGroup
/// @brief 引数を4つ持つ Binder
/// @sa TBindMgr
//////////////////////////////////////////////////////////////////////
template<typename T1,
	 typename T2,
	 typename T3,
	 typename T4>
class T4Binder :
  public Binder
{
  friend class T4BindMgr<T1, T2, T3, T4>;
private:

  /// @brief イベント処理関数
  /// @param[in] obj1, obj2, obj3, obj4 ブロードキャストする情報
  /// を表すオブジェクト
  virtual
  void
  event_proc(T1 obj1,
	     T2 obj2,
	     T3 obj3,
	     T4 obj4) = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class T4BindMgr Binder.h <ym_utils/Binder.h>
/// @ingroup BinderGroup
/// @brief T4Binder を起動するクラス
/// @sa T4Binder
//////////////////////////////////////////////////////////////////////
template<typename T1,
	 typename T2,
	 typename T3,
	 typename T4>
class T4BindMgr :
  private BindMgr
{
public:

  typedef T4Binder<T1, T2, T3, T4> TBinder;

public:

  /// @brief コンストラクタ
  T4BindMgr() { }

  /// @brief デストラクタ
  ~T4BindMgr() { }
  
  /// @brief TBinder の登録を削除する．
  /// @param[in] binder 登録を削除する対象
  /// @note binder が登録されていない場合には何もしない．
  void
  unreg_binder(TBinder* binder) { _unreg_binder(binder); }
  
  /// @brief TBinder を登録する．
  /// @param[in] binder 登録する対象
  void
  reg_binder(TBinder* binder) { _reg_binder(binder); }
  
  /// @brief ここに登録されたすべての binder に (T1, T2, T3, T4)
  /// @brief のオブジェクトを伝える．
  /// @param[in] data1, data2, data3, data4 ブロードキャストする内容
  void
  prop_event(T1 data1,
	     T2 data2,
	     T3 data3,
	     T4 data4)
  {
    for (list<Binder*>::iterator p = mList.begin();
	 p != mList.end(); ++ p) {
      // 本当はよくない static_cast だが reg_binder() で登録できるのは
      // TBinder だけなので大丈夫なはず
      TBinder* binder = static_cast<TBinder*>(*p);
      binder->event_proc(data1, data2, data3, data4);
    }
  }
};


//////////////////////////////////////////////////////////////////////
/// @class T5Binder Binder.h <ym_utils/Binder.h>
/// @ingroup BinderGroup
/// @brief 引数を5つ持つ Binder
/// @sa T5BindMgr
//////////////////////////////////////////////////////////////////////
template <typename T1,
	  typename T2,
	  typename T3,
	  typename T4,
	  typename T5>
class T5Binder :
  public Binder
{
  friend class T5BindMgr<T1, T2, T3, T4, T5>;
private:

  /// @brief イベント処理関数
  /// @param[in] obj1, obj2, obj3, obj4, obj5 ブロードキャストする
  /// 情報を表すオブジェクト
  virtual
  void
  event_proc(T1 obj1,
	     T2 obj2,
	     T3 obj3,
	     T4 obj4,
	     T5 obj5) = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class T5BindMgr Binder.h <ym_utils/Binder.h>
/// @ingroup BinderGroup
/// @brief T5Binder を起動するクラス
/// @sa T5Binder
//////////////////////////////////////////////////////////////////////
template <typename T1,
	  typename T2,
	  typename T3,
	  typename T4,
	  typename T5>
class T5BindMgr :
  private BindMgr
{
public:

  typedef T5Binder<T1, T2, T3, T4, T5> TBinder;

public:

  /// @brief コンストラクタ
  T5BindMgr() { }

  /// @brief デストラクタ
  ~T5BindMgr() { }
  
  /// @brief TBinder の登録を削除する．
  /// @param[in] binder 登録を削除する対象
  /// @note binder が登録されていない場合には何もしない．
  void
  unreg_binder(TBinder* binder) { _unreg_binder(binder); }
  
  /// @brief TBinder を登録する．
  /// @param[in] binder 登録する対象
  void
  reg_binder(TBinder* binder) { _reg_binder(binder); }
  
  /// @brief ここに登録されたすべての binder に (T1, T2, T3, T4, T5)
  /// @brief のオブジェクトを伝える．
  /// @param[in] data1, data2, data3, data4, data5 ブロードキャストする内容
  void
  prop_event(T1 data1,
	     T2 data2,
	     T3 data3,
	     T4 data4,
	     T5 data5)
  {
    for (list<Binder*>::iterator p = mList.begin();
	 p != mList.end(); ++ p) {
      // 本当はよくない static_cast だが reg_binder() で登録できるのは
      // TBinder だけなので大丈夫なはず
      TBinder* binder = static_cast<TBinder*>(*p);
      binder->event_proc(data1, data2, data3, data4, data5);
    }
  }
};


//////////////////////////////////////////////////////////////////////
/// @class T6Binder Binder.h <ym_utils/Binder.h>
/// @ingroup BinderGroup
/// @brief 引数を6つ持つ Binder
/// @sa T6BindMgr
//////////////////////////////////////////////////////////////////////
template <typename T1,
	  typename T2,
	  typename T3,
	  typename T4,
	  typename T5,
	  typename T6>
class T6Binder :
  public Binder
{
  friend class T6BindMgr<T1, T2, T3, T4, T5, T6>;
private:

  /// @brief イベント処理関数
  /// @param[in] obj1, obj2, obj3, obj4, obj5, obj6 ブロードキャスト
  /// する情報を表すオブジェクト
  virtual
  void
  event_proc(T1 obj1,
	     T2 obj2,
	     T3 obj3,
	     T4 obj4,
	     T5 obj5,
	     T6 obj6) = 0;
  
};


//////////////////////////////////////////////////////////////////////
/// @class T6BindMgr Binder.h <ym_utils/Binder.h>
/// @ingroup BinderGroup
/// @brief T6Binder を起動するクラス
/// @sa T6Binder
//////////////////////////////////////////////////////////////////////
template <typename T1,
	  typename T2,
	  typename T3,
	  typename T4,
	  typename T5,
	  typename T6>
class T6BindMgr :
  private BindMgr
{
public:

  typedef T6Binder<T1, T2, T3, T4, T5, T6> TBinder;

public:

  /// @brief コンストラクタ
  T6BindMgr() { }

  /// @brief デストラクタ
  ~T6BindMgr() { }
  
  /// @brief TBinder の登録を削除する．
  /// @param[in] binder 登録を削除する対象
  /// @note binder が登録されていない場合には何もしない．
  void
  unreg_binder(TBinder* binder) { _unreg_binder(binder); }
  
  /// @brief TBinder を登録する．
  /// @param[in] binder 登録する対象
  void
  reg_binder(TBinder* binder) { _reg_binder(binder); }
  
  /// @brief ここに登録されたすべての binder に (T1, T2, T3, T4, T5, T6)
  /// @brief のオブジェクトを伝える．
  /// @param[in] data1, data2, data3, data4, data5, data6
  /// ブロードキャストする内容
  void
  prop_event(T1 data1,
	     T2 data2,
	     T3 data3,
	     T4 data4,
	     T5 data5,
	     T6 data6)
  {
    for (list<Binder*>::iterator p = mList.begin();
	 p != mList.end(); ++ p) {
      // 本当はよくない static_cast だが reg_binder() で登録できるのは
      // TBinder だけなので大丈夫なはず
      TBinder* binder = static_cast<TBinder*>(*p);
      binder->event_proc(data1, data2, data3, data4, data5, data6);
    }
  }
};

END_NAMESPACE_YM

#endif // YM_UTILS_BINDER_H
