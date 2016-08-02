
/// @file src/dtpg_classic/SearchMgr.cc
/// @brief SearchMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SearchMgr.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "SearchMgr.h"
#include "SaFault.h"
#include "TestVector.h"
#include "Gate.h"
#include "Network.h"
#include "LvlQ.h"
#include "PGraph.h"
#include "Cfna.h"
#include "backtrace.h"
#include "sense.h"


BEGIN_NAMESPACE_YM_ATPG_DTPGCL

bool debug = false;

SearchMgr sm;

// @brief コンストラクタ
SearchMgr::SearchMgr()
{
  set_default_backtrack_limit();
  clear_backtrack_count();
}

// @brief デストラクタ
SearchMgr::~SearchMgr()
{
}
struct Comp
{
  bool
  operator()(const pair<Gate*, Val3>& a,
	     const pair<Gate*, Val3>& b) const
  {
    return strcmp(a.first->name(), b.first->name()) >= 0;
  }
};

// @brief 故障 f に対するテストパタン生成を行う．
// @param[in] f 対象の故障
// @param[in] flist その他の未検出故障のリスト
// @param[in] dyn_mode dynamic implication のモード
// @param[out] tv 生成したパタンを入れるベクタ
// @param[out] tv で検出できる故障のリスト
// @retval kDetect 検出した．
// @retval kUntest 検出不能と判定した．
// @retval kAbort アボートした．
tStat
SearchMgr::run(SaFault* f,
	       const vector<SaFault*>& flist,
	       tDynMode dyn_mode,
	       TestVector* tv,
	       vector<SaFault*>& det_faults)
{
  if ( get_verbose_level() > 0 ) {
    fprintf(ver_fp, "Target fault: %s\n", f->str().c_str());
  }
  
  Gate* i_gate = gn.gate(f->source_node());
  
  bool dyn_flag = false;
  if ( dyn_mode != kDynNone ) {
    dyn_flag = true;
    set_ex_cands(i_gate);
  }
  
  gn.inject_fault(f);
  
  int strategy = 0;
  bool retry = false;
  bool found = false;
  init();
  mand_assign(i_gate, ~(f->val3()), kValX);
  pgraph.construct(i_gate);
  for ( ; ; ) {
    if ( implication() ) {
	
      // unique sensitization
      if ( sensitize(i_gate) ) {
	continue;
      }
    
      // dynamic implication
      if ( dyn_flag && examine_dyn_imp() ) {
	continue;
      }

      // kDynOnce の時は一回 dynamic implication で有益な情報が
      // 得られなくなったら二度と dynamic implication を起動しない．
      if ( dyn_mode == kDynOnce ) {
	dyn_flag = false;
      }
    
      if ( strategy == 0 ) {
	Gate* o_gate;
	Val3 o_gval;
	Val3 o_fval;
	if ( C_decision(i_gate, o_gate, o_gval, o_fval) ) {
	  opt_assign(o_gate, o_gval, o_fval);
	  continue;
	}
	if ( O_decision(i_gate, o_gate, o_gval, o_fval) ) {
	  opt_assign(o_gate, o_gval, o_fval);
	  continue;
	}
	// 割り当てる変数がなくなったということは
	// パタンが見つかったということ
	found = true;
	break;
      }
      else {
	Gate* o_gate;
	Val3 o_gval;
	Val3 o_fval;
	if ( O_decision(i_gate, o_gate, o_gval, o_fval) ) {
	  opt_assign(o_gate, o_gval, o_fval);
	  continue;
	}
	if ( C_decision(i_gate, o_gate, o_gval, o_fval) ) {
	  opt_assign(o_gate, o_gval, o_fval);
	  continue;
	}
	// 割り当てる変数がなくなったということは
	// パタンが見つかったということ
	found = true;
	break;
      }
    }
    else {
      // インプリケーション失敗
    
      if ( back_track() ) {
	continue;
      }
    
      if ( has_conflict() || retry ) {
	break;
      }
      // strategy を変えてもう一度ためす．
      mAborted = false;
      mAccBtNum += mBtNum;
      mBtNum = 0;
      strategy ^= 1;
      retry = true;
    }
  }
  if ( found ) {
    wrap_up(i_gate);
    cfna.record(mTermList, tv);
    //drop_fault(f, flist, det_faults);
  }
  restore_value();
  pgraph.destruct();
  gn.remove_fault(f);
  mAccBtNum += mBtNum;
  
  if ( has_conflict() ) {
    if ( get_verbose_level() > 0 ) {
      fprintf(ver_fp, "... redundant\n\n");
    }
    return kUntest;
  }
  
  if ( is_aborted() ) {
    if ( get_verbose_level() > 0 ) {
      fprintf(ver_fp, "... aborted\n\n");
    }
    return kAbort;
  }
  
  if ( get_verbose_level() > 0 ) {
    fprintf(ver_fp, "... detected\n\n");
  }
  return kDetect;
}

// jnode を val にするパタンを求める．
bool
SearchMgr::basis_just(Gate* jnode,
		      Val3 val)
{
  // jnode のファンアウトも一時的に切っておく
  int jnode_no = jnode->act_nfo();
  jnode->set_act_no(0);

  // 一連の初期化
  init();
  
  // 要求値の設定
  mand_assign(jnode, val, kValX);

  // main loop
  bool flag = false;
  for (; ; ) {
    if ( !implication2() ) {
      if ( back_track() ) {
	continue;
      }
      if ( is_aborted() ) {
	if ( get_verbose_level() > 1 ) {
	  fprintf(ver_fp, "justificatoin aborted\n");
	}
      }
      else {
	if ( get_verbose_level() > 1 ) {
	  fprintf(ver_fp, "%s %s never occurs\n",
		  jnode->name(), value_name(val, val));
	}
	if ( val == kVal0 ) {
	  add_const1(jnode);
	}
	else {
	  add_const0(jnode);
	}
	flag = false;
      }
      break;
    }
    Gate* o_gate;
    Val3 o_gval;
    Val3 o_fval;
    if ( C_decision(jnode, o_gate, o_gval, o_fval) ) {
      opt_assign(o_gate, o_gval, o_fval);
    }
    else {
      // justified
      wrap_up2(jnode);
      flag = true;
      break;
    }
  }

  // post-processing
  restore_value();
  jnode->set_act_no(jnode_no);
  
  return flag;
}
  
// @brief 初期化する．
void
SearchMgr::init()
{
  mSaveStack.clear();
  mAltStack.clear();
  mBtNum = 0;
  mAborted = false;
  mConflict = false;
  eventq_clear();
}

// @brief 値の割り当てを行う．
void
SearchMgr::opt_assign(Gate* gate,
		      Val3 gval,
		      Val3 fval)
{
  if ( get_verbose_level() > 1 ) {
    fprintf(ver_fp, "OPTIONAL_ASSIGN: %s (%s)\n", gate->name(),
	    value_name(gval, fval));
  }
  
  eventq_add(gate, gval, fval);
  
  mAltStack.push_back(AltElem(gate, ~gval, ~fval,
			      mSaveStack.size()));
}

// @brief 値の割り当てを行う．
void
SearchMgr::mand_assign(Gate* gate,
		       Val3 gval,
		       Val3 fval)
{
  if ( get_verbose_level() > 1 ) {
    fprintf(ver_fp, "MANDATORY_ASSIGN: %s (%s)\n", gate->name(),
	    value_name(gval, fval));
  }
  
  eventq_add(gate, gval, fval);
}

// @brief 含意を行う．
bool
SearchMgr::implication()
{
  if ( mEqPos >= mEventQ.size() ) {
    FATAL("NULL implication");
  }
  
  pgraph.clear_indif();
  while ( mEqPos < mEventQ.size() ) {
    EventElem& tmp = mEventQ[mEqPos];
    ++ mEqPos;
    Gate* gate = tmp.mGate;
    if ( gate->chk_fcone() ) {
      Val3 gval = tmp.mGval;
      if ( gval == kVal0 ) {
	if ( !gate->imp0_gb() ) {
	  return false;
	}
      }
      else if ( gval == kVal1 ) {
	if ( !gate->imp1_gb() ) {
	  return false;
	}
      }
      Val3 fval = tmp.mFval;
      if ( fval == kVal0 ) {
	if ( !gate->imp0_fb() ) {
	  return false;
	}
      }
      else if ( fval == kVal1 ) {
	if ( !gate->imp1_fb() ) {
	  return false;
	}
      }
    }
    else {
      Val3 gval = tmp.mGval;
      if ( gval == kVal0 ) {
	if ( !gate->imp0_hb() ) {
	  return false;
	}
      }
      else if ( gval == kVal1 ) {
	if ( !gate->imp1_hb() ) {
	  return false;
	}
      }
    }
  }
  mEventQ.clear();
  mEqPos = 0;
  return pgraph.update();
}

// @brief 含意を行う．
bool
SearchMgr::implication2()
{
  while ( mEqPos < mEventQ.size() ) {
    EventElem& tmp = mEventQ[mEqPos];
    ++ mEqPos;
    Gate* gate = tmp.mGate;
    Val3 gval = tmp.mGval;
    if ( gval == kVal0 ) {
      if ( !gate->imp0_hb() ) {
	return false;
      }
    }
    else if ( gval == kVal1 ) {
      if ( !gate->imp1_hb() ) {
	return false;
      }
    }
  }
  mEventQ.clear();
  mEqPos = 0;
  return true;
}

// @brief バックトラックを行う．
bool
SearchMgr::back_track()
{
  if ( get_verbose_level() > 1 ) {
    fprintf(ver_fp, "BACK_TRACK\n");
  }

  eventq_flush();

  if ( mAltStack.empty() ) {
    mConflict = true;
    return false;
  }
  
  AltElem& ae = mAltStack.back();
  eventq_add(ae.mGate, ae.mGval, ae.mFval);
  ymuint bottom = ae.mBottom;
  mAltStack.pop_back();
  for (ymuint i = mSaveStack.size(); i > bottom; ) {
    -- i;
    StackElem& se = mSaveStack[i];
    Gate* gate = se.mGate;
    gate->mState = se.mState;
  }
  mSaveStack.erase(mSaveStack.begin() + bottom, mSaveStack.end());
  ++ mBtNum;
  if ( mBtNum >= mBtLimit ) {
    mAborted = true;
    return false;
  }
  return true;
}

// @brief 値をもとに戻す．
void
SearchMgr::restore_value()
{
  for (ymuint i = 0; i < mSaveStack.size(); ++ i) {
    StackElem& se = mSaveStack[i];
    Gate* gate = se.mGate;
    gate->mState = 0;
  }
  mSaveStack.clear();
}

// @brief 必要な割り当てを求める．
void
SearchMgr::wrap_up(Gate* i_gate)
{
  if ( get_verbose_level() > 2 ) {
    cout << "wrap_up(" << i_gate->name() << ")" << endl;
  }
  lvlq.clear();
  for (Gate* gate = i_gate; ; ) {
    if ( get_verbose_level() > 2 ) {
      cout << "  gate = " << gate->name()
	   << ": " << value_name(gate->gval(), gate->fval()) << endl;
    }
    lvlq.put(gate);
    if ( gate != i_gate && gate->is_po() ) {
      if ( get_verbose_level() > 2 ) {
	cout << "  PO" << endl << endl;
      }
      break;
    }
    ymuint no = gate->act_nfo();
    bool found = false;
    for (ymuint i = 0; i < no; ++ i) {
      Gate* o_gate = gate->act_fanout(i);
      if ( o_gate->chk_pg() &&
	   o_gate->chk_gj() &&
	   o_gate->chk_fj() ) {
	lvlq.put(o_gate);
	gate = o_gate;
	found = true;
	break;
      }
    }
    assert_cond(found, __FILE__, __LINE__);
  }
  
  mTermList.clear();
  for (Gate* gate; (gate = lvlq.get_from_top()); ) {
    if ( get_verbose_level() > 2 ) {
      cout << "  gate = " << gate->description()
	   << ": " << value_name(gate->gval(), gate->fval()) << endl;
    }
    Val3 gval = gate->gval();
    Val3 fval = gate->fval();
    if ( !gate->chk_fcone() && gate->chk_cfna(gval) ) {
      mTermList.push_back(make_pair(gate, gval));
      if ( get_verbose_level() > 2 ) {
	cout << "  TERMINAL" << endl << endl;
      }
      continue;
    }
    ymuint ni = gate->nfi();
    vector<bool> inq(ni, false);
    if ( gate->chk_gj() ) {
      if ( gval == gate->o_val() ) {
	for (ymuint i = 0; i < ni; ++ i) {
	  Gate* i_gate = gate->sorted_fanin(i);
	  if ( i_gate->gval() == gate->c_val() &&
	       i_gate->chk_gj() ) {
	    lvlq.put(i_gate);
	    inq[i] = true;
	    break;
	  }
	}
      }
      else {
	for (ymuint i = 0; i < ni; ++ i) {
	  Gate* i_gate = gate->sorted_fanin(i);
	  assert_cond(i_gate->chk_gj(), __FILE__, __LINE__);
	  lvlq.put(i_gate);
	  inq[i] = true;
	}
      }
    }
    if ( gate->chk_fcone() && gate->chk_fj() ) {
      if ( fval == gate->o_val() ) {
	for (ymuint i = 0; i < ni; ++ i) {
	  Gate* i_gate = gate->sorted_fanin(i);
	  if ( i_gate->chk_fcone() ) {
	    if ( i_gate->fval() == gate->c_val() &&
		 i_gate->chk_fj() ) {
	      lvlq.put(i_gate);
	      inq[i] = true;
	      break;
	    }
	  }
	  else {
	    if ( i_gate->gval() == gate->c_val() &&
		 i_gate->chk_gj() ) {
	      lvlq.put(i_gate);
	      inq[i] = true;
	      break;
	    }
	  }
	}
      }
      else {
	for (ymuint i = 0; i < ni; ++ i) {
	  Gate* i_gate = gate->sorted_fanin(i);
	  if ( i_gate->chk_fcone() ) {
	    assert_cond(i_gate->chk_fj(), __FILE__, __LINE__);
	    }
	    else {
	    assert_cond(i_gate->chk_gj(), __FILE__, __LINE__);
	  }
	  lvlq.put(i_gate);
	  inq[i] = true;
	}
      }
    }
    if ( get_verbose_level() > 2 ) {
      for (ymuint i = 0; i < ni; ++ i) {
	Gate* i_gate = gate->sorted_fanin(i);
	cout << "    "
	     << value_name(i_gate->gval(), i_gate->fval());
	if ( inq[i] ) {
	  cout << "  ***  :";
	}
	else {
	  cout << "       :";
	}
	cout << i_gate->name() << endl;
      }
      cout << endl;
    }
  }
}

// @brief 必要な割り当てを求める．
void
SearchMgr::wrap_up2(Gate* i_gate)
{
  if ( get_verbose_level() > 2 ) {
    cout << "wrap_up2(" << i_gate->name() << ")" << endl;
  }
  
  lvlq.clear();
  lvlq.put(i_gate);
  mTermList.clear();
  for (Gate* gate; (gate = lvlq.get_from_top()); ) {
    if ( get_verbose_level() > 2 ) {
      cout << "  gate = " << gate->description()
	   << ": " << value_name(gate->gval(), gate->fval()) << endl;
    }
    Val3 gval = gate->gval();
    if ( gate->chk_cfna(gval) ) {
      mTermList.push_back(make_pair(gate, gval));
      if ( get_verbose_level() > 2 ) {
	cout << "  TERMINAL" << endl << endl;
      }
      continue;
    }
    ymuint ni = gate->nfi();
    vector<bool> inq(ni, false);
    if ( gate->chk_gj() ) {
      if ( gval == gate->o_val() ) {
	for (ymuint i = 0; i < ni; ++ i) {
	  Gate* i_gate = gate->sorted_fanin(i);
	  if ( i_gate->gval() == gate->c_val() &&
	       i_gate->chk_gj() ) {
	    lvlq.put(i_gate);
	    inq[i] = true;
	    break;
	  }
	}
      }
      else {
	for (ymuint i = 0; i < ni; ++ i) {
	  Gate* i_gate = gate->sorted_fanin(i);
	  assert_cond(i_gate->chk_gj(), __FILE__, __LINE__);
	  lvlq.put(i_gate);
	  inq[i] = true;
	}
      }
    }
    if ( get_verbose_level() > 2 ) {
      for (ymuint i = 0; i < ni; ++ i) {
	Gate* i_gate = gate->sorted_fanin(i);
	cout << "    "
	     << value_name(i_gate->gval(), i_gate->fval());
	if ( inq[i] ) {
	  cout << "  ***  :";
	}
	else {
	  cout << "       :";
	}
	cout << i_gate->name() << endl;
      }
      cout << endl;
    }
  }
}

// @brief 直前に求めたテストベクタで f が検出可能かどうか調べる．
void
SearchMgr::drop_fault(SaFault* f0,
		      const vector<SaFault*>& flist,
		      vector<SaFault*>& det_faults)
{
  Gate* fos = gn.gate(f0->node())->fos();
  
  det_faults.clear();

  for (vector<SaFault*>::const_iterator p = flist.begin();
       p != flist.end(); ++ p) {
    SaFault* f = *p;
    Gate* gate = gn.gate(f->node());
    if ( f == f0 || gate->fos() != fos ) {
      continue;
    }
    if ( f->val3() == kVal0 ) {
      if ( gn.gate(f->source_node())->gval() != kVal1 ) {
	continue;
      }
    }
    else {
      if ( gn.gate(f->source_node())->gval() != kVal0 ) {
	continue;
      }
    }

    if ( f->is_input_fault() ) {
      ymuint ipos = f->pos();
      if ( !gate->calc_obs(ipos) ) {
	continue;
      }
    }
    
    bool reached = false;
    for ( ; ; ) {
      if ( gate->is_fos() ) {
	reached = true;
	break;
      }
      Gate* ogate = gate->fanout(0);
      int ipos = gate->fanout_ipos(0);
      if ( !ogate->calc_obs(ipos) ) {
	break;
      }
      gate = ogate;
    }
    if ( reached ) {
      // 検出可能
      det_faults.push_back(f);
    }
  }
}

// @brief 直前の implication() がアボートしていたら true を返す．
bool
SearchMgr::is_aborted()
{
  return mAborted;
}

// @brief 直前の implication() で矛盾が起こっていたら true を返す．
bool
SearchMgr::has_conflict()
{
  return mConflict;
}

// @brief デフォルトのバックトラック制限をセットする．
void
SearchMgr::set_default_backtrack_limit()
{
  mBtLimit = DEFAULT_BACKTRACK_LIMIT;
}

// @brief バックトラック制限を得る．
ymuint
SearchMgr::backtrack_limit()
{
  return mBtLimit;
}

// @brief バックトラック制限をセットする．
void
SearchMgr::set_backtrack_limit(ymuint limit)
{
  mBtLimit = limit;
}

// @brief バックトラック数を得る．
ymuint
SearchMgr::backtrack_count()
{
  return mAccBtNum + mBtNum;
}

// @brief バックトラック数のカウンタをクリアする．
void
SearchMgr::clear_backtrack_count()
{
  mAccBtNum = 0;
  mBtNum = 0;
}

// @brief dynamic implication の候補リストを作る．
// @param[in] gate 対象のゲート
void
SearchMgr::set_ex_cands(Gate* gate)
{
  mExCands.clear();
  mExCands.reserve(gn.ngate());
  lvlq.clear();
  gn.clear_mark();
  mark_tfi_tfo(gate);
  for (Gate* g; (g = lvlq.get_from_top()); ) {
    if ( g->nfi() >= 2 ) {
      mExCands.push_back(g);
    }
  }
  mCurPos = 0;
}

void
SearchMgr::mark_tfi_tfo(Gate* gate)
{
  if ( gate->chk_mark() ) {
    return;
  }
  gate->set_mark();
  lvlq.put(gate);
  for (int i = gate->act_nfo(); -- i >= 0; ) {
    mark_tfi_tfo(gate->act_fanout(i));
  }
  for (int i = gate->nfi(); -- i >= 0; ) {
    mark_tfi(gate->fanin(i));
  }
}

void
SearchMgr::mark_tfi(Gate* gate)
{
  if ( gate->chk_mark() ) {
    return;
  }
  gate->set_mark();
  lvlq.put(gate);
  for (int i = gate->nfi(); -- i >= 0; ) {
    mark_tfi(gate->fanin(i));
  }
}

// @brief dynamic implication を行う．
bool
SearchMgr::examine_dyn_imp()
{
  ymuint i = mCurPos;
  for ( ; ; ) {
    Gate* gate = mExCands[i];
    if ( !gate->chk_fcone() ) {
      if ( gate->gval() == kValX ) {
	if ( !dyn_imp(gate, kVal0, kValX) ) {
	  eventq_add(gate, kVal1, kValX);
	  break;
	}
	if ( !dyn_imp(gate, kVal1, kValX) ) {
	  eventq_add(gate, kVal0, kValX);
	  break;
	}
      }
    }
    else if ( !gate->chk_dif() ) {
      if ( gate->gval() == kValX ||
	   gate->fval() == kValX) {
	if ( !dyn_imp(gate, kVal0, kVal0) ) {
	  eventq_add(gate, kVal1, kVal1);
	  break;
	}
	if ( !dyn_imp(gate, kVal1, kVal1) ) {
	  eventq_add(gate, kVal0, kVal0);
	  break;
	}
      }
    }
    else {
      if ( gate->gval() == kValX ) {
	if ( !dyn_imp(gate, kVal0, kValX) ) {
	  eventq_add(gate, kVal1, kValX);
	  break;
	}
	if ( !dyn_imp(gate, kVal1, kValX) ) {
	  eventq_add(gate, kVal0, kValX);
	  break;
	}
      }
      if ( gate->fval() == kValX ) {
	if ( !dyn_imp(gate, kValX, kVal0) ) {
	  eventq_add(gate, kValX, kVal1);
	  break;
	}
	if ( !dyn_imp(gate, kValX, kVal1) ) {
	  eventq_add(gate, kValX, kVal0);
	  break;
	}
      }
    }
    ++ i;
    if ( i == mExCands.size() ) {
      i = 0;
    }
    if ( i == mCurPos ) {
      return false;
    }
  }
  mCurPos = i;
  return true;
}

// @brief dynamic implication を行う．
bool
SearchMgr::dyn_imp(Gate* gate,
		   Val3 gval,
		   Val3 fval)
{
  eventq_add(gate, gval, fval);
  ymuint old_top = mSaveStack.size();;
  bool result = implication();
  for (ymuint i = mSaveStack.size(); i > old_top; ) {
    -- i;
    StackElem& se = mSaveStack[i];
    Gate* gate = se.mGate;
    gate->mState = se.mState;
  }
  mSaveStack.erase(mSaveStack.begin() + old_top, mSaveStack.end());
  return result;
}
  
// @brief 定数ノードのリストをクリアする．
void
SearchMgr::clear_const()
{
  mConst0List.clear();
  mConst1List.clear();
}

// @brief 定数0ノードを追加する．
void
SearchMgr::add_const0(Gate* gate)
{
  mConst0List.push_back(gate);
  gate->set_const();
}

// @brief 定数1ノードを追加する．
void
SearchMgr::add_const1(Gate* gate)
{
  mConst1List.push_back(gate);
  gate->set_const();
}

// @brief イベントキューを初期化する．
// @note 定数ノードのイベントが自動的に挿入される．
void
SearchMgr::eventq_clear()
{
  eventq_flush();
  for (vector<Gate*>::iterator p = mConst0List.begin();
       p != mConst0List.end(); ++ p) {
    Gate* gate = *p;
    if ( gate->is_active() ) {
      eventq_add(gate, kVal0, kValX);
    }
  }
  for (vector<Gate*>::iterator p = mConst1List.begin();
       p != mConst1List.end(); ++ p) {
    Gate* gate = *p;
    if ( gate->is_active() ) {
      eventq_add(gate, kVal1, kValX);
    }
  }
}

// @brief イベントキューをフラッシュする．
void
SearchMgr::eventq_flush()
{
  mEventQ.clear();
  mEqPos = 0;
}

END_NAMESPACE_YM_ATPG_DTPGCL
