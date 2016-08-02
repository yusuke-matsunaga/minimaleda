#ifndef LIBYM_BDD_BMM_BMMVAR_H
#define LIBYM_BDD_BMM_BMMVAR_H

/// @file libym_bdd/bmm/BmmVar.h
/// @brief BmmVar のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BmmVar.h 33 2006-07-16 14:58:17Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#include <ym_bdd/Bdd.h>


BEGIN_NAMESPACE_YM_BDD

class BddMgrModern;
class BmmNode;

//////////////////////////////////////////////////////////////////////
// 変数の情報を格納しておくクラス
//////////////////////////////////////////////////////////////////////
class BmmVar
{
  friend class BddMgrModern;
public:
  typedef BmmNode Node;

public:

  // 変数番号を得る．
  tVarId varid() const
  {
    return mId;
  }
  
  // レベルを得る．
  tLevel level() const
  {
    return mLevel;
  }

private:
  // コンストラクタ
  BmmVar(BddMgrModern* mgr, tVarId id);
  
  // デストラクタ
  ~BmmVar();

  // gc 用の sweep 処理
  void sweep();

  // 節点テーブルが縮小可能なら縮小する．
  void shrink(double load_limit);

  // ノードを登録する．
  void reg_node(size_t pos, BmmNode* node);

  // 節点テーブルを拡張する
  // メモリアロケーションに失敗したら false を返す．
  bool resize(size_t new_size);
  
  // 次のリミット値を計算する
  void set_next_limit_size(double load_limit);
  
  // ログ出力用のストリームを得る．
  ostream& logstream() const;

  // 節点テーブル用のメモリを確保する．
  // size はバイト単位ではなくエントリ数
  BmmNode** alloc_nodetable(size_t size);

  // 節点テーブル用のメモリを解放する．
  // size はバイト単位ではなくエントリ数
  void dealloc_nodetable(BmmNode** table, size_t size);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親の BddMgr
  BddMgrModern* mMgr;

  // 変数番号
  // これは不変
  tVarId mId;
  
  // レベル
  // これは可変
  tLevel mLevel;
  
  // 作業用のマーク
  int mMark;

  // compose用にBDDの枝を入れておくメンバ
  tBddEdge mCompEdge;

  // ハッシュ表中の次の要素を指すポインタ
  BmmVar* mLink;

  //////////////////////////////////////////////////////////////////////
  // 節点テーブル関係のメンバ
  //////////////////////////////////////////////////////////////////////

  // テーブルサイズ
  size_t mTableSize;

  // テーブルサイズ - 1
  size_t mTableSize_1;

  // ノード数がこの数を越えたらテーブルサイズを拡張する．
  size_t mNextLimit;

  // テーブル本体
  BmmNode** mNodeTable;

  // ノード数
  size_t mNodeNum;

};

END_NAMESPACE_YM_BDD

#endif // LIBYM_BDD_BMM_BMMVAR_H
