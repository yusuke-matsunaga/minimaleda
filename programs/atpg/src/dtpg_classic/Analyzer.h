#ifndef DTPG_CLASSIC_ANALYZER_H
#define DTPG_CLASSIC_ANALYZER_H

/// @file src/dtpg_classic/Analyzer.h
/// @brief Analyzer のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Analyzer.h 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "dtpgcl_nsdef.h"
#include "ym_aig/aig_nsdef.h"


BEGIN_NAMESPACE_YM_ATPG

class TvMgr;
class TestVector;

END_NAMESPACE_YM_ATPG

BEGIN_NAMESPACE_YM_ATPG_DTPGCL

class Network;
class Gate;

//////////////////////////////////////////////////////////////////////
/// @class Analyzer Analyzer.h "Analyzer.h"
/// @brief いくつかの解析を行うクラス
//////////////////////////////////////////////////////////////////////
class Analyzer
{
public:

  /// @brief コンストラクタ
  Analyzer();

  /// @brief デストラクタ
  ~Analyzer();


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
  
  // AigMgr
  AigMgr* mAigMgr;
  
};


END_NAMESPACE_YM_ATPG_DTPGCL

#endif
