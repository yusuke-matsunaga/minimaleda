
/// @file libym_sat/SatSolverImpl.cc
/// @brief SatSolverImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SatSolverImpl.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "SatSolverImpl.h"
#include "SatAnalyzer.h"
#include "SatClause.h"


BEGIN_NAMESPACE_YM_SAT

BEGIN_NONAMESPACE

const ymuint debug_none        = 0x00;

const ymuint debug_implication = 0x01;

const ymuint debug_analyze     = 0x02;

const ymuint debug_assign      = 0x04;

const ymuint debug_decision    = 0x08;

const ymuint debug_solve       = 0x10;

const ymuint debug_all         = 0xffffffff;

//const ymuint debug = debug_decision | debug_analyze | debug_assign;
//const ymuint debug = debug_assign;
//const ymuint debug = debug_assign | debug_implication;
//const ymuint debug = debug_assign | debug_analyze;
//const ymuint debug = debug_solve | debug_decision;
//const ymuint debug = debug_solve | debug_assign;
//const ymuint debug = debug_all;
const ymuint debug = debug_none;

END_NONAMESPACE


const
Params kDefaultParams(0.95, 0.02, 0.999);


//////////////////////////////////////////////////////////////////////
// SatSolverImpl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SatSolverImpl::SatSolverImpl(SatAnalyzer* analyzer) :
  mAnalyzer(analyzer),
  mSane(true),
  mAlloc(4096),
  mWatcherAlloc(sizeof(Watcher), 1024),
  mVarNum(0),
  mOldVarNum(0),
  mVarSize(0),
  mVal(NULL),
  mDecisionLevel(NULL),
  mReason(NULL),
  mHeapPos(NULL),
  mActivity(NULL),
  mWatcherList(NULL),
  mHeap(NULL),
  mHeapNum(0),
  mRootLevel(0),
  mVarBump(1.0),
  mVarDecay(1.0),
  mClauseBump(1.0),
  mClauseDecay(1.0),
  mParams(kDefaultParams),
  mConflictNum(0),
  mDecisionNum(0),
  mPropagationNum(0),
  mConstrBinNum(0),
  mConstrLitNum(0),
  mLearntBinNum(0),
  mLearntLitNum(0),
  mConflictLimit(0),
  mLearntLimit(0),
  mMaxConflict(1024 * 10)
{
  mAnalyzer->set_solver(this);

  mTmpBinClause = new_clause(vector<Literal>(2));
  
  mRestart = 0;
  
  mTimerOn = false;
}

// @brief デストラクタ
SatSolverImpl::~SatSolverImpl()
{
  delete mAnalyzer;
  delete [] mVal;
  delete [] mDecisionLevel;
  delete [] mReason;
  delete [] mHeapPos;
  delete [] mActivity;
  delete [] mWatcherList;
  delete [] mHeap;
}
  
// @brief 正しい状態のときに true を返す．
bool
SatSolverImpl::sane() const
{
  return mSane;
}

// @brief 変数を追加する．
// @return 新しい変数番号を返す．
// @note 変数番号は 0 から始まる．
tVarId
SatSolverImpl::new_var()
{
  if ( decision_level() != 0 ) {
    // エラー
    cout << "Error!: decision_level() != 0" << endl;
    return 0;
  }
  
  // ここではカウンタを増やすだけ
  // 実際の処理は alloc_var() でまとめて行う．
  tVarId n = mVarNum;
  ++ mVarNum;
  return n;
}
  
// 変数に関する配列を拡張する．
void
SatSolverImpl::expand_var()
{
  ymuint old_size = mVarSize;
  Bool3* old_val = mVal;
  int* old_decision_level = mDecisionLevel;
  SatReason* old_reason = mReason;
  ymint32* old_heap_pos = mHeapPos;
  double* old_activity = mActivity;
  WatcherList* old_watcher_list = mWatcherList;
  ymuint32* old_heap = mHeap;
  if ( mVarSize == 0 ) {
    mVarSize = 1024;
  }
  while ( mVarSize < mVarNum ) {
    mVarSize <<= 1;
  }
  mVal = new Bool3[mVarSize];
  mDecisionLevel = new int[mVarSize];
  mReason = new SatReason[mVarSize];
  mHeapPos = new ymint32[mVarSize];
  mActivity = new double[mVarSize];
  mWatcherList = new WatcherList[mVarSize * 2];
  mHeap = new ymuint32[mVarSize];
  for (ymuint i = 0; i < mOldVarNum; ++ i) {
    mVal[i] = old_val[i];
    mDecisionLevel[i] = old_decision_level[i];
    mReason[i] = old_reason[i];
    mHeapPos[i] = old_heap_pos[i];
    mActivity[i] = old_activity[i];
  }
  ymuint n2 = mOldVarNum * 2;
  for (ymuint i = 0; i < n2; ++ i) {
    mWatcherList[i].move(old_watcher_list[i]);
  }
  for (ymuint i = 0; i < mHeapNum; ++ i) {
    mHeap[i] = old_heap[i];
  }
  if ( old_size > 0 ) {
    delete [] old_val;
    delete [] old_decision_level;
    delete [] old_reason;
    delete [] old_heap_pos;
    delete [] old_activity;
    delete [] old_watcher_list;
    delete [] old_heap;
  }
  mAssignList.reserve(mVarSize);
  mAnalyzer->alloc_var(mVarSize);
}

// @brief 節を追加する．
// @param[in] lits リテラルのベクタ
void
SatSolverImpl::add_clause(const vector<Literal>& lits)
{
  if ( decision_level() != 0 ) {
    // エラー
    cout << "Error![SatSolverImpl]: decision_level() != 0" << endl;
    return;
  }
  
  if ( !mSane ) {
    cout << "Error![SatSolverImpl]: mSane == false" << endl;
    return;
  }

  // operator<(Literal, Literal) を使ったソート
  mAcTmp = lits;
  sort(mAcTmp.begin(), mAcTmp.end());

  alloc_var();
  
  // - 重複したリテラルの除去
  // - false literal の除去
  // - true literal を持つかどうかのチェック
  vector<Literal>::iterator wp = mAcTmp.begin();
  for (vector<Literal>::const_iterator rp = mAcTmp.begin();
       rp != mAcTmp.end(); ++ rp) {
    Literal l = *rp;
    if ( wp != mAcTmp.begin() && *(wp - 1) == l ) {
      // 重複している．
      continue;
    }
    Bool3 v = eval(l);
    if ( v == kB3False ) {
      // false literal は追加しない．
      continue;
    }
    if ( v == kB3True ) {
      // true literal があったら既に充足している
      return;
    }
    if ( l.varid() >= mVarNum ) {
      // 範囲外
      cout << "Error![SatSolverImpl]: literal(" << l << "): out of range"
	   << endl;
      return;
    }
    // 追加する．
    *wp = l;
    ++ wp;
  }
  mAcTmp.erase(wp, mAcTmp.end());

  ymuint n = mAcTmp.size();
  mConstrLitNum += n;
  
  if ( n == 0 ) {
    // empty clause があったら unsat
    mSane = false;
    return;
  }

  Literal l0 = mAcTmp[0];
  if ( n == 1 ) {
    // unit clause があったら値の割り当てを行う．
    bool stat = check_and_assign(l0);
    if ( debug & debug_assign ) {
      cout << "\tassign " << l0 << " @" << decision_level()
	   << endl;
      if ( !stat )  {
	cout << "\t--> conflict with previous assignment" << endl
	     << "\t    " << ~l0 << " was assigned at level "
	     << decision_level(l0.varid()) << endl;
      }
    }
    if ( !stat ) {
      mSane = false;
    }
    return;
  }
  
  Literal l1 = mAcTmp[1];
  
  if ( n == 2 ) {
    // watcher-list の設定
    add_watcher(~l0, SatReason(l1));
    add_watcher(~l1, SatReason(l0));
    ++ mConstrBinNum;
  }
  else {
    // 節の生成
    SatClause* clause = new_clause(mAcTmp);
    mConstrClause.push_back(clause);

    // watcher-list の設定
    add_watcher(~l0, SatReason(clause));
    add_watcher(~l1, SatReason(clause));
  }
}

// 学習節を追加する．
void
SatSolverImpl::add_learnt_clause(const vector<Literal>& lits)
{
  ymuint n = lits.size();
  mLearntLitNum += n;

  if ( n == 0 ) {
    // empty clause があったら unsat
    mSane = false;
    return;
  }

  Literal l0 = lits[0];
  if ( n == 1 ) {
    // unit clause があったら値の割り当てを行う．
    bool stat = check_and_assign(l0);
    if ( debug & debug_assign ) {
      cout << "\tassign " << l0 << " @" << decision_level()
	   << endl;
      if ( !stat )  {
	cout << "\t--> conflict with previous assignment" << endl
	     << "\t    " << ~l0 << " was assigned at level "
	     << decision_level(l0.varid()) << endl;
      }
    }
    if ( !stat ) {
      mSane = false;
    }
    return;
  }

  SatReason reason;
  Literal l1 = lits[1];
  if ( n == 2 ) {
    reason = SatReason(l1);

    // watcher-list の設定
    add_watcher(~l0, SatReason(l1));
    add_watcher(~l1, SatReason(l0));
    ++ mLearntBinNum;
  }
  else {
    // 節の生成
    SatClause* clause = new_clause(lits, true);
    mLearntClause.push_back(clause);
    reason = SatReason(clause);
    
    // watcher-list の設定
    add_watcher(~l0, reason);
    add_watcher(~l1, reason);
  }
  
  // learnt clause の場合には必ず unit clause になっているはず．
  assert_cond(eval(l0) != kB3False, __FILE__, __LINE__);
  if ( debug & debug_assign ) {
    cout << "\tassign " << l0 << " @" << decision_level()
	 << " from " << reason << endl;
  }

  assign(l0, reason);
}

// Watcher を追加する．
void
SatSolverImpl::add_watcher(Literal watch_lit,
			   SatReason reason)
{
  watcher_list(watch_lit).add(Watcher(reason));
}


// @brief SAT 問題を解く．
// @param[in] assumptions あらかじめ仮定する変数の値割り当てリスト
// @param[out] model 充足するときの値の割り当てを格納する配列．
// @retval kB3True 充足した．
// @retval kB3False 充足不能が判明した．
// @retval kB3X わからなかった．
// @note i 番めの変数の割り当て結果は model[i] に入る．
Bool3
SatSolverImpl::solve(const vector<Literal>& assumptions,
		     vector<Bool3>& model)
{
  if ( debug & debug_solve ) {
    cout << "SatSolverImpl::solve starts" << endl;
    cout << " Assumptions: ";
    const char* and_str = "";
    for (vector<Literal>::const_iterator p = assumptions.begin();
	 p != assumptions.end(); ++ p) {
      cout << and_str << *p;
      and_str = " & ";
    }
    cout << endl;
    cout << " Clauses:" << endl;
    for (vector<SatClause*>::const_iterator p = mConstrClause.begin();
	 p != mConstrClause.end(); ++ p) {
      cout << "  " << *(*p) << endl;
    }
  }

  if ( mTimerOn ) {
    mTimer.stop();
    mTimer.reset();
    mTimer.start();
  }
  
  alloc_var();
  
  simplifyDB();
  if ( !mSane ) {
    return kB3False;
  }

  double confl_limit = 100;
  double learnt_limit = mConstrClause.size() / 3;
  mVarDecay = mParams.mVarDecay;
  mClauseDecay = mParams.mClauseDecay;

  assert_cond(decision_level() == 0, __FILE__, __LINE__);

  // assumption の割り当てを行う．
  for (vector<Literal>::const_iterator p = assumptions.begin();
       p != assumptions.end(); ++ p) {
    Literal lit = *p;

    mAssignList.set_marker();
    bool stat = check_and_assign(lit);

    if ( debug & (debug_assign | debug_decision) ) {
      cout << endl
	   << "assume " << lit << " @" << decision_level()
	   << endl;
      if ( !stat )  {
	cout << "\t--> conflict with previous assignment" << endl
	     << "\t    " << ~lit << " was assigned at level "
	     << decision_level(lit.varid()) << endl;
      }
    }

    if ( !stat || implication() != kNullSatReason ) {
      backtrack(0);
      if ( debug & debug_solve ) {
	cout << "UNSAT" << endl;
      }
      return kB3False;
    }
  }
  // 以降，現在のレベルが基底レベルとなる．
  mRootLevel = decision_level();
  if ( debug & (debug_assign | debug_decision) ) {
    cout << "RootLevel = " << mRootLevel << endl;
  }

  Bool3 stat = kB3X;
  for ( ; ; ) {
    mConflictLimit = static_cast<ymuint64>(confl_limit);
    if ( mConflictLimit > mMaxConflict ) {
      mConflictLimit = mMaxConflict;
    }
    mLearntLimit = static_cast<ymuint64>(learnt_limit);
    SatStats stats;
    get_stats(stats);
    for (list<SatMsgHandler*>::iterator p = mMsgHandlerList.begin();
	 p != mMsgHandlerList.end(); ++ p) {
      SatMsgHandler& handler = *(*p);
      handler(stats);
    }
    ++ mRestart;
    stat = search();
    if ( stat != kB3X ) {
      break;
    }
    if ( mConflictLimit == mMaxConflict ) {
      break;
    }
    decay_var_activity2();
    confl_limit = confl_limit * 1.5;
    learnt_limit = learnt_limit * 1.1;
  }
  if ( stat == kB3True ) {
    // SAT ならモデル(充足させる変数割り当てのリスト)を作る．
    model.resize(mVarNum);
    for (ymuint i = 0; i < mVarNum; ++ i) {
      Bool3 val = eval(i);
      assert_cond(val != kB3X, __FILE__, __LINE__);
      model[i] = val;
    }
  }
  backtrack(0);

  if ( mTimerOn ) {
    mTimer.stop();
  }

  if ( debug & debug_solve ) {
    switch ( stat ) {
    case kB3True:  cout << "SAT" << endl; break;
    case kB3False: cout << "UNSAT" << endl; break;
    case kB3X:     cout << "UNKNOWN" << endl; break;
    default: assert_not_reached(__FILE__, __LINE__);
    }
  }

  return stat;
}

// @brief conflict_limit の最大値
// @param[in] val 設定する値
// @return 以前の設定値を返す．
ymuint64
SatSolverImpl::set_max_conflict(ymuint64 val)
{
  ymuint64 old_val = mMaxConflict;
  mMaxConflict = val;
  return old_val;
}

// @brief 現在の内部状態を得る．
// @param[out] stats 状態を格納する構造体
void
SatSolverImpl::get_stats(SatStats& stats) const
{
  stats.mRestart = mRestart;
  stats.mVarNum = mVarNum;
  stats.mConstrClauseNum = clause_num();
  stats.mConstrLitNum = mConstrLitNum;
  stats.mLearntClauseNum = mLearntClause.size() + mLearntBinNum;
  stats.mLearntLitNum = mLearntLitNum;
  stats.mConflictNum = mConflictNum;
  stats.mDecisionNum = mDecisionNum;
  stats.mPropagationNum = mPropagationNum;
  stats.mConflictLimit = mConflictLimit;
  stats.mLearntLimit = mLearntLimit;
  stats.mTime = mTimer.time();
}

// @brief solve() 中のリスタートのたびに呼び出されるメッセージハンドラの登録
// @param[in] msg_handler 登録するメッセージハンドラ
void
SatSolverImpl::reg_msg_handler(SatMsgHandler* msg_handler)
{
  mMsgHandlerList.push_back(msg_handler);
}

// 探索を行う本体の関数
Bool3
SatSolverImpl::search()
{
  // コンフリクトの起こった回数
  ymuint n_confl = 0;
  for ( ; ; ) {
    // キューにつまれている割り当てから含意される値の割り当てを行う．
    SatReason conflict = implication();
    if ( conflict != kNullSatReason ) {
      // 矛盾が生じた．
      ++ n_confl;
      ++ mConflictNum;
      if ( decision_level() == mRootLevel ) {
	// トップレベルで矛盾が起きたら充足不可能
	return kB3False;
      }
      if ( n_confl > mConflictLimit ) {
	// 矛盾の回数が制限値を越えた．
	backtrack(mRootLevel);
	return kB3X;
      }

      // 今の矛盾の解消に必要な条件を「学習」する．
      vector<Literal> learnt;
      int bt_level = mAnalyzer->analyze(conflict, learnt);
  
      if ( debug & debug_analyze ) {
	cout << endl
	     << "analyze for " << conflict << endl
	     << "learnt clause is ";
	const char* plus = "";
	for (ymuint i = 0; i < learnt.size(); ++ i) {
	  Literal l = learnt[i];
	  cout << plus << l << " @" << decision_level(l.varid());
	  plus = " + ";
	}
	cout << endl;
      }

      // バックトラック
      if ( bt_level < mRootLevel ) {
	bt_level = mRootLevel;
      }
      backtrack(bt_level);

      // 学習節の生成
      add_learnt_clause(learnt);

      decay_var_activity();
      decay_clause_activity();
    }
    else {
      if ( decision_level() == 0 ) {
	simplifyDB();
      }
      if ( mLearntClause.size() > mLearntLimit ) {
	reduceDB();
      }

      // 次の割り当てを選ぶ．
      Literal lit = next_decision();
      if ( lit == kLiteralX ) {
	// すべての変数を割り当てた．
	// ということは充足しているはず．
	return kB3True;
      }
      ++ mDecisionNum;

      // バックトラックポイントを記録
      mAssignList.set_marker();

      // 未割り当ての変数を選んでいるのでエラーになるはずはない．
      if ( debug & (debug_assign | debug_decision) ) {
	cout << endl
	     << "choose " << lit << " @" << decision_level() << endl;
      }
      assign(lit);
    }
  }
}

// 割当てキューに基づいて implication を行う．
SatReason
SatSolverImpl::implication()
{
  SatReason conflict = kNullSatReason;
  while ( mAssignList.has_elem() ) {
    Literal l = mAssignList.get_next();
    ++ mPropagationNum;
    
    if ( debug & debug_implication ) {
      cout << "\tpick up " << l << endl;
    }
    // l の割り当てによって無効化された watcher-list の更新を行う．
    Literal nl = ~l;

    WatcherList& wlist = watcher_list(l);
    ymuint n = wlist.num();
    ymuint rpos = 0;
    ymuint wpos = 0;
    while ( rpos < n ) {
      Watcher w = wlist.elem(rpos);
      wlist.set_elem(wpos, w);
      ++ rpos;
      ++ wpos;
      if ( w.is_literal() ) {
	Literal l0 = w.literal();
	Bool3 val0 = eval(l0);
	if ( val0 == kB3X ) {
	  if ( debug & debug_assign ) {
	    cout << "\tassign " << l0 << " @" << decision_level()
		 << " from " << l << endl;
	  }
	  assign(l0, SatReason(nl));
	}
	else if ( val0 == kB3False ) {
	  // 矛盾がおこった．
	  if ( debug & debug_assign ) {
	    cout << "\t--> conflict with previous assignment" << endl
		 << "\t    " << ~l0 << " was assigned at level "
		 << decision_level(l0.varid()) << endl;
	  }
	  // ループを抜けるためにキューの末尾まで先頭を動かす．
	  mAssignList.skip_all();
	  mTmpBinClause->mLits[0] = l0;
	  mTmpBinClause->mLits[1] = nl;
	  conflict = SatReason(mTmpBinClause);
	  break;
	}
      }
      else {
	SatClause& c = w.clause();
	Literal l0 = c.lit0();
	if ( l0 == nl ) {
	  // nl を 2番めのリテラルにする．
	  l0 = c.lit1();
	  c.lit0() = l0;
	  c.lit1() = nl;
	}
	else { // l1 == nl
	  if ( debug & debug_implication ) {
	    // この assert は重いのでデバッグ時にしかオンにしない．
	    // ※ debug と debug_implication が const なので結果が0の
	    // ときにはコンパイル時に消されることに注意
	    assert_cond(c.lit1() == nl, __FILE__, __LINE__);
	  }
	}

	Bool3 val0 = eval(l0);
	if ( val0 != kB3True ) {
	  if ( debug & debug_implication ) {
	    cout << "\t\texamining watcher clause " << c << endl;
	  }

	  // nl の替わりのリテラルを見つける．
	  // この時，替わりのリテラルが未定かすでに充足しているかどうか
	  // は問題でない．
	  bool found = false;
	  int n = c.size();
	  for (int i = 2; i < n; ++ i) {
	    Literal l2 = c.lit(i);
	    if ( eval(l2) != kB3False ) {
	      c.lit(i) = nl;
	      c.lit1() = l2;
	      if ( debug & debug_implication ) {
		cout << "\t\t\tsecond watching literal becomes "
		     << l2 << endl;
	      }
	      // l の watcher list からは取り除く
	      -- wpos;
	      // ~tmp の watcher list に追加する．
	      watcher_list(~l2).add(w);
	      found = true;
	      break;
	    }
	  }
	  if ( found ) {
	    continue;
	  }

	  if ( debug & debug_implication ) {
	    cout << "\t\tno other watching literals" << endl;
	  }
	
	  // 見付からなかったので l0 に従った割り当てを行う．
	  if ( val0 == kB3X ) {
	    if ( debug & debug_assign ) {
	      cout << "\tassign " << l0 << " @" << decision_level()
		   << " from " << w << endl;
	    }
	    assign(l0, w);
	  }
	  else {
	    // 矛盾がおこった．
	    if ( debug & debug_assign ) {
	      cout << "\t--> conflict with previous assignment" << endl
		   << "\t    " << ~l0 << " was assigned at level "
		   << decision_level(l0.varid()) << endl;
	    }
	    // ループを抜けるためにキューの末尾まで先頭を動かす．
	    mAssignList.skip_all();
	    conflict = w;
	    break;
	  }
	}
      }
    }
    if ( wpos != rpos ) {
      for ( ; rpos < n; ++ rpos) {
	wlist.set_elem(wpos, wlist.elem(rpos));
	++ wpos;
      }
      wlist.erase(wpos);
    }
  }
  
  return conflict;
}

// level までバックトラックする
void
SatSolverImpl::backtrack(int level)
{
  if ( debug & (debug_assign | debug_decision) ) {
    cout << endl
	 << "backtrack until @" << level << endl;
  }
  
  if ( level < decision_level() ) {
    mAssignList.backtrack(level);
    while ( mAssignList.has_elem() ) {
      Literal p = mAssignList.get_prev();
      tVarId varid = p.varid();
      mVal[varid] = kB3X;
      heap_push(varid);
      if ( debug & debug_assign ) {
	cout << "\tdeassign " << p << endl;
      }
    }
  }

  if ( debug & (debug_assign | debug_decision) ) {
    cout << endl;
  }
}

// 次の割り当てを選ぶ
Literal
SatSolverImpl::next_decision()
{
#if 0
  if ( mRandGen.real1() < mParams.mVarFreq && !heap_empty() ) {
    ymuint pos = mRandGen.int32() % mVarNum;
    tPol pol = kPolNega;
    if ( eval(pos) == kB3X ) {
      return Literal(pos, pol);
    }
  }
#endif
  while ( !heap_empty() ) {
    tVarId var = heap_pop_top();
    if ( eval(var) == kB3X ) {
      //tPol pol = (mRandGen.int32() & 1) ? kPolPosi : kPolNega;
      tPol pol = kPolNega;
      return Literal(var, pol);
    }
  }
  return kLiteralX;
}

// CNF を簡単化する．
void
SatSolverImpl::simplifyDB()
{
  if ( !mSane ) {
    return;
  }
  assert_cond(decision_level() == 0, __FILE__, __LINE__);

  if ( implication() != kNullSatReason ) {
    mSane = false;
    return;
  }

}

BEGIN_NONAMESPACE
// reduceDB で用いる SatClause の比較関数
class SatClauseLess
{
public:
  bool
  operator()(SatClause* a,
	     SatClause* b)
  {
    return a->size() > 2 && (b->size() == 2 || a->activity() < b->activity() );
  }
};
END_NONAMESPACE

// 使われていない学習節を削除する．
void
SatSolverImpl::reduceDB()
{
  ymuint n = mLearntClause.size();
  ymuint n2 = n / 2;
  
  // 足切りのための制限値
  double abs_limit = mClauseBump / n;

  sort(mLearntClause.begin(), mLearntClause.end(), SatClauseLess());

  vector<SatClause*>::iterator wpos = mLearntClause.begin();
  for (ymuint i = 0; i < n2; ++ i) {
    SatClause* clause = mLearntClause[i];
    if ( clause->size() > 2 && !is_locked(clause) ) {
      delete_clause(clause);
    }
    else {
      *wpos = clause;
      ++ wpos;
    }
  }
  for (ymuint i = n2; i < n; ++ i) {
    SatClause* clause = mLearntClause[i];
    if ( clause->size() > 2 && !is_locked(clause) &&
	 clause->activity() < abs_limit ) {
      delete_clause(clause);
    }
    else {
      *wpos = clause;
      ++ wpos;
    }
  }
  if ( wpos != mLearntClause.end() ) {
    mLearntClause.erase(wpos, mLearntClause.end());
  }
}

// 新しい節を生成する．
SatClause*
SatSolverImpl::new_clause(const vector<Literal>& lits,
			  bool learnt)
{
  ymuint n = lits.size();
  ymuint size = sizeof(SatClause) + sizeof(Literal) * (n - 1);
  void* tmp = mAlloc.get_memory(size);
  return new (tmp) SatClause(lits, learnt);
}

// 節を捨てる．
void
SatSolverImpl::delete_clause(SatClause* clause)
{
  // 0 番目と 1 番目のリテラルに関係する watch list を更新
  for (ymuint i = 0; i < 2; ++ i) {
    Literal l = ~clause->lit(i);
    WatcherList& wlist = watcher_list(l);
    ymuint n = wlist.num();
    ymuint rpos = 0;
    ymuint wpos = 0;
    for ( ; rpos < n; ++ rpos) {
      Watcher w = wlist.elem(rpos);
      if ( w != SatReason(clause) ) {
	wlist.set_elem(wpos, w);
	++ wpos;
      }
    }
    wlist.erase(wpos);
  }
  if ( clause->is_learnt() ) {
    mLearntLitNum -= clause->size();
  }
  else {
    mConstrLitNum -= clause->size();
  }
}

// 変数のアクティビティを増加させる．
void
SatSolverImpl::bump_var_activity(tVarId varid)
{
  double& act = mActivity[varid];
  act += mVarBump;
  if ( act > 1e+100 ) {
    for (ymuint i = 0; i < mVarNum; ++ i) {
      mActivity[i] *= 1e-100;
    }
    mVarBump *= 1e-100;
  }
  ymint pos = mHeapPos[varid];
  if ( pos >= 0 ) {
    heap_move_up(pos);
  }
}

// 変数のアクティビティを定率で減少させる．
void
SatSolverImpl::decay_var_activity()
{
  mVarBump /= mVarDecay;
}

// リスタート時の変数のアクティビティの低減率
void
SatSolverImpl::decay_var_activity2()
{
#if 0
  for (ymuint i = 0; i < mVarNum; ++ i) {
    mActivity[i] /= mConflictNum;
  }
  mVarBump = 1.0;
#endif
}

// 学習節のアクティビティを増加させる．
void
SatSolverImpl::bump_clause_activity(SatClause* clause)
{
  clause->mActivity += mClauseBump;
  if ( clause->mActivity > 1e+100 ) {
    for (vector<SatClause*>::iterator p = mLearntClause.begin();
	 p != mLearntClause.end(); ++ p) {
      SatClause* clause1 = *p;
      clause1->mActivity *= 1e-100;
    }
    mClauseBump *= 1e-100;
  }
}

// 学習節のアクティビティを定率で減少させる．
void
SatSolverImpl::decay_clause_activity()
{
  mClauseBump /= mClauseDecay;
}

// 引数の位置にある要素を適当な位置まで沈めてゆく
void
SatSolverImpl::heap_move_down(ymuint pos)
{
  tVarId var_p = mHeap[pos];
  double val_p = mActivity[var_p];
  for ( ; ; ) {
    // ヒープ木の性質から親から子の位置がわかる
    ymuint pos_l = heap_left(pos);
    ymuint pos_r = pos_l + 1;
    if ( pos_r > mHeapNum ) {
      // 左右の子どもを持たない場合
      break;
    }
    ymuint pos_c = pos_l;
    tVarId var_c = mHeap[pos_c];
    double val_c = mActivity[var_c];
    if ( pos_r < mHeapNum ) {
      tVarId var_r = mHeap[pos_r];
      double val_r = mActivity[var_r];
      if ( val_c < val_r ) {
	pos_c = pos_r;
	var_c = var_r;
	val_c = val_c;
      }
    }
    if ( val_c <= val_p ) {
      break;
    }
    // 逆転
    heap_set(var_p, pos_c);
    heap_set(var_c, pos);
    pos = pos_c;
  }
}

// @brief 内容を出力する
void
SatSolverImpl::heap_dump(ostream& s) const
{
  s << "heap num = " << mHeapNum << endl;
  ymuint j = 0;
  ymuint nc = 1;
  const char* spc = "";
  for (ymuint i = 0; i < mHeapNum; ++ i) {
    tVarId var = mHeap[i];
    assert_cond(mHeapPos[var] == static_cast<ymint>(i),
		__FILE__, __LINE__);
    if ( i > 0 ) {
      ymint p = (i - 1) >> 1;
      assert_cond(mVal[mHeap[p]] >= mVal[var], __FILE__, __LINE__);
    }
    s << spc << var << "("
      << mActivity[var]
      << ")";
    ++ j;
    if ( j == nc ) {
      j = 0;
      nc <<= 1;
      s << endl;
      spc = "";
    }
    else {
      spc = " ";
    }
  }
  if ( j > 0 ) {
    s << endl;
  }
}

END_NAMESPACE_YM_SAT
