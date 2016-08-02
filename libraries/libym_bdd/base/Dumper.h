#ifndef LIBYM_BDD_DUMPER_H
#define LIBYM_BDD_DUMPER_H

/// @file libym_bdd/base/Dumper.h
/// @brief Dumper のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Dumper.h 693 2007-05-28 09:13:57Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#include "BddMgr.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
// BDD の節点に番号を割り振るためのクラス
//////////////////////////////////////////////////////////////////////
class IdMgr
{
public:
  // コンストラクタ
  IdMgr();

  // デストラクタ
  ~IdMgr();

  // e がすでに ID 番号を持っていたら true を返す．
  bool
  has_id(tBddEdge e) const;

  // e の ID 番号を返す．
  // 未登録ならば新しい番号を割り振る．
  size_t
  id(tBddEdge e);

  // 登録された節点数を返す．
  size_t
  num() const;

  // クリアする．
  void
  clear();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // tBddEdge と ID 番号の対応表
  hash_map<tBddEdge, size_t> mMap;

  // 次に割り当てるID番号
  size_t mNext;

};


//////////////////////////////////////////////////////////////////////
// BDD の内容を出力するためのクラス
//////////////////////////////////////////////////////////////////////
class Displayer
{

public:

  // コンストラクタ
  Displayer(BddMgr* mgr,
	    ostream& s);

  // デストラクタ
  ~Displayer();

  // 登録された節点数を返す．
  size_t
  num() const;

  // e を根とするBDDの内容を出力する．
  void
  display_root(tBddEdge e);


private:

  // e の ID 番号を出力する．
  void
  display_id(tBddEdge e);

  // e の内容を出力する．
  void
  display_name(tBddEdge e);

  // display_root の下請関数
  void
  display_step(tBddEdge e);
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // BddMgr へのポインタ
  BddMgr* mMgr;

  // 出力用のストリーム
  ostream& mStream;

  // 処理済の tBddEdge に印を付けておくためのハッシュ表
  hash_set<tBddEdge> mMark;

  // ID 番号を管理するマネージャ
  IdMgr mIdMgr;

};


//////////////////////////////////////////////////////////////////////
// BDD の内容を保存するためのクラス
//////////////////////////////////////////////////////////////////////
class Dumper
{
public:

  // コンストラクタ
  Dumper(BddMgr* mgr,
	 ostream& s);

  // デストラクタ
  ~Dumper();

  // e を根とするBDDの内容を出力する．
  void
  dump(tBddEdge e);

  // e の内容を出力する．
  void
  dump_edge(tBddEdge e);
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // BddMgr へのポインタ
  BddMgr* mMgr;

  // 出力用のストリーム
  ostream& mStream;

  // ID 番号を管理するマネージャ
  IdMgr mIdMgr;

};


//////////////////////////////////////////////////////////////////////
// ダンプされた内容を読み込むためのクラス
//////////////////////////////////////////////////////////////////////
class Restorer
{
public:

  // コンストラクタ
  Restorer(BddMgr* mgr,
	   istream& s);

  // デストラクタ
  ~Restorer();

  // 読み込む．
  // 読み込んだBDDの根の枝の数を返す．
  // エラーが起きたら 0 を返す．
  size_t
  read();

  // pos 番目の枝を返す．
  tBddEdge
  root(size_t pos);


private:

  // 内部の枝を指す文字列から枝を得る．
  // 見つからなければ kEdgeError を返す．
  tBddEdge
  find_edge(const char* str) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // BddMgr へのポインタ
  BddMgr* mMgr;

  // 入力用のストリーム
  istream& mStream;

  // 根の枝を格納しておくベクタ
  vector<tBddEdge> mRootVector;

  // 内部の枝を格納しておくベクタ
  vector<tBddEdge> mEdgeVector;

};

END_NAMESPACE_YM_BDD

#endif
