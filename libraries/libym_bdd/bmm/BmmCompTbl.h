#ifndef LIBYM_BDD_BMM_BMMCOMPTBL_H
#define LIBYM_BDD_BMM_BMMCOMPTBL_H

/// @file libym_bdd/bmm/BmmCompTbl.h
/// @brief BmmCompTbl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BmmCompTbl.h 693 2007-05-28 09:13:57Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#include "BddMgrModern.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
// 演算結果テーブルの基本クラス
// 実際にはこれの継承クラスを定義する必要がある
//////////////////////////////////////////////////////////////////////
class BmmCompTbl
{
  friend class BddMgrModern;
public:
  
  // 使用されているセル数を返す．
  size_t
  used_num() const;
  
  // テーブルサイズを返す．
  size_t
  table_size() const;
  
  // load_limit を設定する．
  void
  load_limit(double load_limit);
  
  // 最大のテーブルサイズを設定する．
  void
  max_size(size_t max_size);
  

protected:

  // コンストラクタ
  BmmCompTbl(BddMgrModern* mgr,
	     const char* name);
  
  // デストラクタ
  virtual
  ~BmmCompTbl();
  
  // ロックされていないノードに関係したセルをきれいにする．
  virtual
  void
  sweep() = 0;

  // 内容をクリアする．
  virtual
  void
  clear() = 0;
  
  // next_limitを更新する
  void
  update_next_limit();
  
  // テーブルを拡張すべき時には true を返す．
  bool
  check_tablesize() const;
  
  // BddMgr からメモリを確保する．
  void*
  allocate(size_t size);
  
  // BddMgr にメモリを返す．
  void
  deallocate(void* ptr,
	     size_t size);
  
  // ログ出力用のストリームを得る．
  ostream&
  logstream() const;

  // e の参照回数が0なら true を返す．
  static
  bool
  check_noref(tBddEdge e);
  

protected:

  // 使用されているセルの数
  size_t mUsedNum;
  
  // テーブルサイズ
  // セルの個数．バイト数ではないので注意
  size_t mTableSize;
  
  // テーブルサイズ - 1 の値，ハッシュのマスクに用いる．
  size_t mTableSize_1;
  
  // ほとんどデバッグ用の名前
  string mName;
  
  // テーブルの使用率がこの値を越えたらサイズの拡張を行なう．
  double mLoadLimit;
  
  // ただし，テーブルのサイズはこれ以上大きくしない．
  size_t mMaxSize;
  
  // mUsedがこの値を越えたらテーブルを拡張する
  size_t mNextLimit;

  // 親の BddMgr
  BddMgrModern* mMgr;

  // 同じ BddMgr に属している次のテーブル
  BmmCompTbl* mNext;

};


//////////////////////////////////////////////////////////////////////
// 1つの枝をキーとして結果の枝を格納するテーブル
//////////////////////////////////////////////////////////////////////
class BmmCompTbl1 :
  public BmmCompTbl
{
  friend class BddMgrModern;

  // キーが1つのセル
  struct Cell
  {
    tBddEdge mKey1;
    tBddEdge mAns;
  };
public:
  
  // id1をキーとして検索を行なう
  tBddEdge
  get(tBddEdge id1);
  
  // 結果を登録する
  void
  put(tBddEdge id1,
      tBddEdge ans);


private:

  // コンストラクタ
  BmmCompTbl1(BddMgrModern* mgr,
	      const char* name = 0);
  
  // デストラクタ
  virtual
  ~BmmCompTbl1();

  // ハッシュ関数
  size_t
  hash_func(tBddEdge id1);
  
  // テーブルサイズを変更する．
  void
  resize(size_t new_size);
  
  // ガーベージコレクションが起きた時の処理を行なう．
  virtual
  void
  sweep();

  // 内容をクリアする．
  virtual
  void
  clear();

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ハッシュテーブル
  Cell* mTable;

};


//////////////////////////////////////////////////////////////////////
// 2つの枝をキーとして結果の枝を格納するテーブル
//////////////////////////////////////////////////////////////////////
class BmmCompTbl2 :
  public BmmCompTbl
{
  friend class BddMgrModern;

  // キーが2つのセル
  struct Cell {
    tBddEdge mKey1;
    tBddEdge mKey2;
    tBddEdge mAns;
  };
public:
  
  // id1, id2をキーとして検索を行なう
  tBddEdge
  get(tBddEdge id1,
      tBddEdge id2);
  
  // 結果を登録する
  void
  put(tBddEdge id1,
      tBddEdge id2,
      tBddEdge ans);


private:

  // コンストラクタ
  BmmCompTbl2(BddMgrModern* mgr,
	      const char* name = 0);
  
  // デストラクタ
  virtual
  ~BmmCompTbl2();

  // ハッシュ関数
  size_t
  hash_func(tBddEdge id1,
	    tBddEdge id2);
  
  // テーブルサイズを変更する．
  void
  resize(size_t new_size);
  
  // ガーベージコレクションが起きた時の処理を行なう．
  virtual
  void
  sweep();

  // 内容をクリアする．
  virtual
  void
  clear();
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ハッシュテーブル
  Cell* mTable;

};


//////////////////////////////////////////////////////////////////////
// 2つのBDDの枝をキーとしてBDDの枝とLogExprを結果として格納する
// ハッシュ表
//////////////////////////////////////////////////////////////////////
class BmmIsopTbl :
  public BmmCompTbl
{
  friend class BddMgrModern;

  struct Cell
  {
    tBddEdge mKey1;
    tBddEdge mKey2;
    tBddEdge mAnsBdd;
    LogExpr* mAnsCov;
  };
public:

  // id1, id2をキーとして検索を行なう
  tBddEdge
  get(tBddEdge id1,
      tBddEdge id2,
      LogExpr& ans_cov);
  
  // 結果を登録する
  void
  put(tBddEdge id1,
      tBddEdge id2,
      tBddEdge ans_bdd,
      const LogExpr& ans_cov);
  

private:

  // コンストラクタ
  BmmIsopTbl(BddMgrModern* mgr,
	     const char* name = 0);
  
  // デストラクタ
  virtual
  ~BmmIsopTbl();

  // ハッシュ関数
  size_t
  hash_func(tBddEdge id1,
	    tBddEdge id2);
  
  // テーブルサイズを変更する．
  void
  resize(size_t new_size);
  
  // ガーベージコレクションが起きた時の処理を行なう．
  virtual
  void
  sweep();

  // クリアする．
  virtual
  void
  clear();
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ハッシュテーブル
  Cell* mTable;

};


//////////////////////////////////////////////////////////////////////
// 3つの枝をキーとする演算結果テーブル．
// if-then-else演算で用いられる
//////////////////////////////////////////////////////////////////////
class BmmCompTbl3 :
  public BmmCompTbl
{
  friend class BddMgrModern;

  // キーが3つのセル
  struct Cell
  {
    tBddEdge mKey1;
    tBddEdge mKey2;
    tBddEdge mKey3;
    tBddEdge mAns;
  };
public:
  
  // 検索を行なう．
  tBddEdge
  get(tBddEdge id1,
      tBddEdge id2,
      tBddEdge id3);
  
  // 登録を行なう．
  void
  put(tBddEdge id1,
      tBddEdge id2,
      tBddEdge id3,
      tBddEdge ans);
  

private:

  // コンストラクタ
  BmmCompTbl3(BddMgrModern* mgr,
	      const char* name = 0);
  
  // デストラクタ
  virtual
  ~BmmCompTbl3();

  // ハッシュ関数
  size_t
  hash_func(tBddEdge id1,
	    tBddEdge id2,
	    tBddEdge id3);
  
  // テーブルサイズを変更する．
  void
  resize(size_t new_size);
  
  // ガーベージコレクションが起きた時の処理を行なう．
  virtual
  void
  sweep();

  // クリアする．
  virtual
  void
  clear();
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ハッシュテーブル
  Cell* mTable;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// テーブルを拡張すべき時には true を返す．
inline
bool
BmmCompTbl::check_tablesize() const
{
  return mUsedNum > mNextLimit && mTableSize < mMaxSize;
}

// e の参照回数が0なら true を返す．
inline
bool
BmmCompTbl::check_noref(tBddEdge e)
{
  BmmNode* vp = BddMgrModern::get_node(e);
  return vp && vp->noref();
}

// ハッシュ関数
inline
size_t
BmmCompTbl1::hash_func(tBddEdge id1)
{
  return size_t((id1 * id1) >> 8) & mTableSize_1;
}

// id1をキーとして検索を行なう
inline
tBddEdge
BmmCompTbl1::get(tBddEdge id1)
{
  Cell* tmp = mTable + hash_func(id1);
  if ( tmp->mKey1 != id1 ) {
    return kEdgeInvalid;
  }
  else {
    return tmp->mAns;
  }
}

// 結果を登録する
inline
void
BmmCompTbl1::put(tBddEdge id1,
		 tBddEdge ans)
{
  if ( check_invalid(ans) ) {
    return;
  }
  if ( check_tablesize() ) {
    resize(mTableSize << 1);
  }
  Cell* tmp = mTable + hash_func(id1);
  if ( tmp->mKey1 == kEdgeInvalid ) mUsedNum ++;
  tmp->mKey1 = id1;
  tmp->mAns = ans;
}

// ハッシュ関数
inline
size_t
BmmCompTbl2::hash_func(tBddEdge id1,
		       tBddEdge id2)
{
  return size_t(id1 + id2 + id2 + (id1 >> 2) + (id2 >> 4)) & mTableSize_1;
  //    return size_t((id1 * (id2 + 2)) >> 2) & mTableSize_1;
}

// id1, id2をキーとして検索を行なう
inline
tBddEdge
BmmCompTbl2::get(tBddEdge id1,
		 tBddEdge id2)
{
  Cell* tmp = mTable + hash_func(id1, id2);
  if ( tmp->mKey1 != id1 || tmp->mKey2 != id2 ) {
    return kEdgeInvalid;
  }
  else {
    return tmp->mAns;
  }
}

// 結果を登録する
inline
void
BmmCompTbl2::put(tBddEdge id1,
		 tBddEdge id2,
		 tBddEdge ans)
{
  if ( check_invalid(ans) ) {
    return;
  }
  if ( check_tablesize() ) {
    resize(mTableSize << 1);
  }
  Cell* tmp = mTable + hash_func(id1, id2);
  if ( tmp->mKey1 == kEdgeInvalid ) mUsedNum ++;
  tmp->mKey1 = id1;
  tmp->mKey2 = id2;
  tmp->mAns = ans;
}

// ハッシュ関数
inline
size_t
BmmIsopTbl::hash_func(tBddEdge id1,
		      tBddEdge id2)
{
  return size_t(id1 + id2 + id2 + (id1 >> 2) + (id2 >> 4)) & mTableSize_1;
  //    return size_t((id1 * (id2 + 2)) >> 2) & mTableSize_1;
}

// id1, id2をキーとして検索を行なう
inline
tBddEdge
BmmIsopTbl::get(tBddEdge id1,
		tBddEdge id2,
		LogExpr& ans_cov)
{
  Cell* tmp = mTable + hash_func(id1, id2);
  if ( tmp->mKey1 != id1 || tmp->mKey2 != id2 ) {
    return kEdgeInvalid;
  }
  else {
    ans_cov = *(tmp->mAnsCov);
    return tmp->mAnsBdd;
  }
}

// 結果を登録する
inline
void
BmmIsopTbl::put(tBddEdge id1,
		tBddEdge id2,
		tBddEdge ans_bdd,
		const LogExpr& ans_cov)
{
  if ( check_invalid(ans_bdd) ) {
    return;
  }
  if ( check_tablesize() ) {
    resize(mTableSize << 1);
  }
  Cell* tmp = mTable + hash_func(id1, id2);
  if ( tmp->mKey1 == kEdgeInvalid ) mUsedNum ++;
  tmp->mKey1 = id1;
  tmp->mKey2 = id2;
  tmp->mAnsBdd = ans_bdd;
  delete tmp->mAnsCov;
  tmp->mAnsCov = new LogExpr(ans_cov);
}

// ハッシュ関数
inline
size_t
BmmCompTbl3::hash_func(tBddEdge id1,
		       tBddEdge id2,
		       tBddEdge id3)
{
  return size_t(id1 + id2 + id3 + (id1 >> 2) + (id2 >> 4) + (id3 >> 6))
    & mTableSize_1;
}

// 検索を行なう．
inline
tBddEdge
BmmCompTbl3::get(tBddEdge id1,
		 tBddEdge id2,
		 tBddEdge id3)
{
  Cell* tmp = mTable + hash_func(id1, id2, id3);
  if ( tmp->mKey1 != id1 || tmp->mKey2 != id2 || tmp->mKey3 != id3 ) {
    return kEdgeInvalid;
  }
  else {
    return tmp->mAns;
  }
}

// 登録を行なう．
inline
void
BmmCompTbl3::put(tBddEdge id1,
		 tBddEdge id2,
		 tBddEdge id3,
		 tBddEdge ans)
{
  if ( check_invalid(ans) ) {
    return;
  }
  if ( check_tablesize() ) {
    resize(mTableSize << 1);
  }
  Cell* tmp = mTable + hash_func(id1, id2, id3);
  if ( tmp->mKey1 == kEdgeInvalid ) mUsedNum ++;
  tmp->mKey1 = id1;
  tmp->mKey2 = id2;
  tmp->mKey3 = id3;
  tmp->mAns = ans;
}

END_NAMESPACE_YM_BDD

#endif // LIBYM_BDD_BMM_BMMCOMPTBL_H
