
/// @file magus/lutmap/XDagCover.cc
/// @brief eXact DAG covering algorithm
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: XDagCover.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2007 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include "magus_config.h"
#endif

#include "XDagCover.h"
#include "CutHolder.h"
#include "DagACover.h"


BEGIN_NAMESPACE_MAGUS_LUTMAP


// コンストラクタ
XDagCover::XDagCover(const string& dcfargs,
		     bool verbose) :
  mClimit(0),
  mSlimit(0),
  mTlimit(0.0),
  mVerbose(verbose)
{
  DagACoverFactory dcfactory;
  mDagCover = dcfactory(dcfargs);
}

// デストラクタ
XDagCover::~XDagCover()
{
  delete mDagCover;
}

// fanout node を選択する時の候補数の上限を設定する．
// 0 で上限無し
void
XDagCover::set_climit(ymuint n)
{
  mClimit = n;
}

// 探索回数の上限を設定する．
// 0 で上限無し
void
XDagCover::set_slimit(ymuint n)
{
  mSlimit = n;
}

// 制限時間を設定する．
// 0.0 で制限なし
void
XDagCover::set_tlimit(double t)
{
  mTlimit = t;
}

// @brief best cut の記録を行う．
// @param[in] sbjgraph サブジェクトグラフ
// @param[in] cut_holder カットを保持するオブジェクト
// @param[out] maprec マッピング結果を記録するオブジェクト
void
XDagCover::record_cuts(const SbjGraph& sbjgraph,
		       const CutHolder& cut_holder,
		       MapRecord& maprec)
{
  mStopWatch.reset();
  mStopWatch.start();
  mStatus = kXdcComplete;
  mScount = 0;

  mTmpMap.init(sbjgraph);
  mBest = sbjgraph.n_lnodes() + 1;
  
  search(sbjgraph, cut_holder, 0);

  cout << "Final cost = " << mBest << endl;
  
  maprec.copy(mBestMap);

  mStopWatch.stop();
}

// 分枝限定法による探索
void
XDagCover::search(const SbjGraph& sbjgraph,
		  const CutHolder& cut_holder,
		  int lb0)
{
  cout << "search(" << lb0 << ", " << mBest << ")" << endl;
  ++ mScount;
  if ( mSlimit != 0 && mScount > mSlimit ) {
    // 回数オーバー
    mStatus = kXdcSlimit;
    return;
  }
  if ( mTlimit != 0.0 && mStopWatch.time().usr_time() >= mTlimit ) {
    // 時間切れ
    mStatus = kXdcTlimit;
    return;
  }

  // 現時点での可能解と下限を求めておく．
  int lb = mDagCover->record_cuts(sbjgraph, cut_holder, mTmpMap);
  cout << "lb = " << lb << endl;
  if ( lb == -1 ) {
    return;
  }
  if ( lb < lb0 ) {
    lb = lb0;
  }

  if ( mBest <= lb ) {
    // ここを探索しても良い解は得られない．
    cout << " bounded" << endl;
    return;
  }

  int ub = mTmpMap.estimate(sbjgraph);

  cout << "ub = " << ub << endl;
  if ( ub == -1 ) {
    // マッピング不能
    return;
  }
  
  assert_cond(lb <= ub, __FILE__, __LINE__);

  if ( ub < mBest ) {
    // 最適解の更新
    mBest = ub;
    mBestMap.copy(mTmpMap);
    cout << "Best = " << ub << endl;
  }

  // 次に分岐するノードを探す．
  ymuint n = sbjgraph.max_node_id();
  int best_u_min = INT_MAX;
  int best_u_max = INT_MAX;
  SbjNode* dec_node = NULL;
  bool reverse = false;
  for (ymuint i = 0; i < n; ++ i) {
    SbjNode* node = sbjgraph.node(i);
    if ( node == NULL ) continue;
    if ( mTmpMap.get_cut(node) == NULL ) continue;
    if ( node->is_fo() || node->is_unselected() ) continue;
    if ( mTmpMap.check_fonode(node) && mTmpMap.cover_count(node) > 1 ) {
#if 1
      cout << "**** " << node->id_str() << " ****" << endl;
      node->set_fo();
      sbjgraph.set_efo();
      int f_lb = mDagCover->record_cuts(sbjgraph, cut_holder, mTmpMap2);
      int f_ub = mTmpMap2.estimate(sbjgraph);
      node->clear_fo();
      cout << "***" << endl;
      node->set_unselected();
      sbjgraph.set_efo();
      int u_lb = mDagCover->record_cuts(sbjgraph, cut_holder, mTmpMap2);
      int u_ub = mTmpMap2.estimate(sbjgraph);
      node->clear_unselected();
      sbjgraph.set_efo();

      cout << "  f_lb = " << f_lb << "\tf_ub = " << f_ub << endl;
      cout << "  u_lb = " << u_lb << "\tu_ub = " << u_ub << endl;
      
      int lb1 = f_lb;
      if ( lb1 > u_lb ) {
	lb1 = u_lb;
      }
      if ( lb1 >= mBest ) {
	// node を fanout にしても unselected にしても mBest を下回らない
	// のなら node に無関係に最適解はない．
	return;
      }
      if ( lb < lb1 ) {
	lb = lb1;
      }

      bool tmp_reverse = false;
      int u_min = 0;
      int u_max = 0;
      if ( u_ub == -1 ) {
	u_min = u_max = f_ub;
      }
      else if ( f_ub == -1 ) {
	u_min = u_max = u_ub;
	tmp_reverse = true;
      }
      else if ( u_ub < f_ub ) {
	u_min = u_ub;
	u_max = f_ub;
	tmp_reverse = true;
      }
      else {
	u_min = f_ub;
	u_max = u_ub;
      }

      if ( best_u_min > u_min ) {
	best_u_min = u_min;
	best_u_max = u_max;
	dec_node = node;
	reverse = tmp_reverse;
      }
      else if ( best_u_min == u_min && best_u_max > u_max ) {
	best_u_max = u_max;
	dec_node = node;
	reverse = tmp_reverse;
      }
#else
      dec_node = node;
      break;
#endif
    }
  }
  if ( dec_node == NULL) {
    return;
  }

  if ( reverse ) {
    // dec_node を unselected にする．
    cout << dec_node->id_str() << "->set_unselected()" << endl;
    dec_node->set_unselected();
    sbjgraph.set_efo();

    search(sbjgraph, cut_holder, lb);
    
    cout << dec_node->id_str() << "->clear_unselected()" << endl;
    dec_node->clear_unselected();

    // dec_node を fo にする．
    cout << dec_node->id_str() << "->set_fo()" << endl;
    dec_node->set_fo();
    sbjgraph.set_efo();

    search(sbjgraph, cut_holder, lb);
    
    cout << dec_node->id_str() << "->clear_fo()" << endl;
    dec_node->clear_fo();
    sbjgraph.set_efo();
  }
  else {
    // dec_node を fo にする．
    cout << dec_node->id_str() << "->set_fo()" << endl;
    dec_node->set_fo();
    sbjgraph.set_efo();

    search(sbjgraph, cut_holder, lb);
    
    cout << dec_node->id_str() << "->clear_fo()" << endl;
    dec_node->clear_fo();

    // dec_node を unselected にする．
    cout << dec_node->id_str() << "->set_unselected()" << endl;
    dec_node->set_unselected();
    sbjgraph.set_efo();

    search(sbjgraph, cut_holder, lb);
    
    cout << dec_node->id_str() << "->clear_unselected()" << endl;
    dec_node->clear_unselected();
    sbjgraph.set_efo();
  }
  cout << endl;
}

END_NAMESPACE_MAGUS_LUTMAP
