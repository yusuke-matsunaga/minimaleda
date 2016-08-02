#ifndef DTPG_CLASSIC_CFNA_H
#define DTPG_CLASSIC_CFNA_H

/// @file src/dtpg_classic/Cfna.h
/// @brief Cfna のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Cfna.h 2128 2009-03-12 07:25:33Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "dtpgcl_nsdef.h"
#include "Val3.h"


BEGIN_NAMESPACE_YM_ATPG

class TvMgr;
class TestVector;

END_NAMESPACE_YM_ATPG

BEGIN_NAMESPACE_YM_ATPG_DTPGCL

class Network;
class Gate;

//////////////////////////////////////////////////////////////////////
/// @class Cfna Cfna.h "Cfna.h"
/// @brief conflict free node assignmentを行うクラス
//////////////////////////////////////////////////////////////////////
class Cfna
{
public:

  /// @brief コンストラクタ
  Cfna();

  /// @brief デストラクタ
  ~Cfna();


public:

  /// @brief 初期化する．
  void
  init(Network& network,
       TvMgr& tvmgr);

  /// @brief 現在の割り当てを記録する．
  void
  record(const vector<pair<Gate*, Val3> >& term_list,
	 TestVector* tv);
  

private:
  //////////////////////////////////////////////////////////////////////
  // 下請けの関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 内容をすべてクリアする．
  void
  clear();
  
  /// @brief テストベクタをクリアする．
  void
  clear_vector();

  /// @brief テストベクタをセットする．
  void
  set_vector(const vector<pair<Gate*, Val3> >& term_list);

  /// @brief 現在の割り当てを gate の割り当てとして記録する．
  void
  record_vector(Gate* gate,
		Val3 val);
  
  /// @brief gate の割り当てを取り出す．
  void
  get_vector(Gate* gate,
	     Val3 val);
  
  /// @brief ゲートごとの正当化を行う．
  bool
  justify_gate(Gate* gate,
	       Val3 val);

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // TvMgr
  TvMgr* mTvMgr;

  // ゲートごとの割り当てを記録しておくハッシュ
  hash_map<ymuint, TestVector*> mAssignMap;
  
  // 入力数
  ymuint32 mNpi;
  
  // 作業用のテストベクタ
  TestVector* mCurVector;
  
};

extern
Cfna cfna;

END_NAMESPACE_YM_ATPG_DTPGCL

#endif
