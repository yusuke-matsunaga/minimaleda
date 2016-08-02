#ifndef MAGUS_LUTMAP_XDAGCOVER_H
#define MAGUS_LUTMAP_XDAGCOVER_H

/// @file magus/lutmap/XDagCover.h
/// @brief eXact DAG covering algorithm
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: XDagCover.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2007 Yusuke Matsunaga
/// All rights reserved.

#include <ym_utils/StopWatch.h>
#include "DagACover.h"
#include "MapRecord.h"


BEGIN_NAMESPACE_MAGUS_LUTMAP

class LBFactory;

//////////////////////////////////////////////////////////////////////
// eXact DAG covering algorithm
//////////////////////////////////////////////////////////////////////
class XDagCover
{
public:

  /// @brief 結果の状態を表す列挙型
  enum tStatus {
    /// @brief 最適解が求まった
    kXdcComplete,
    /// @brief 回数制限により中止された
    kXdcSlimit,
    /// @brief 時間制限により中止された
    kXdcTlimit
  };
  

public:

  // コンストラクタ
  XDagCover(const string& dcfargs,
	    bool verbose);

  // デストラクタ
  virtual
  ~XDagCover();


public:

  // fanout node を選択する時の候補数の上限を設定する．
  // 0 で上限無し
  void
  set_climit(ymuint n);

  // 探索回数の上限を設定する．
  // 0 で上限無し
  void
  set_slimit(ymuint n);

  // 制限時間を設定する．
  // 0.0 で制限なし
  void
  set_tlimit(double t);

  /// @brief best cut の記録を行う．
  /// @param[in] sbjgraph サブジェクトグラフ
  /// @param[in] cut_holder カットを保持するオブジェクト
  /// @param[out] maprec マッピング結果を記録するオブジェクト
  void
  record_cuts(const SbjGraph& sbjgraph,
	      const CutHolder& cut_holder,
	      MapRecord& maprec);


private:

  // 分枝限定法による探索
  void
  search(const SbjGraph& sbjgraph,
	 const CutHolder& cut_holder,
	 int lb);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 現在までのベスト
  int mBest;

  // 作業領域として用いる MapRecord
  MapRecord mTmpMap;

  // 作業領域として用いる MapRecord
  MapRecord mTmpMap2;
  
  // ベストの解
  MapRecord mBestMap;

  // dag covering を行うファンクター
  DagACover* mDagCover;
  
  // fanout node を探すときの上限
  ymuint32 mClimit;

  // search 全体の上限
  ymuint32 mSlimit;

  // search の呼ばれた回数
  ymuint32 mScount;

  // 制限時間(秒)
  double mTlimit;

  // ストップウォッチ
  StopWatch mStopWatch;
  
  bool mVerbose;

  tStatus mStatus;
  
};

END_NAMESPACE_MAGUS_LUTMAP

#endif // MAGUS_LUTMAP_XDAGCOVER_H
