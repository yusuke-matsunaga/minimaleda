#ifndef DTPG_CLASSIC_SEARCHMGR_H
#define DTPG_CLASSIC_SEARCHMGR_H

/// @file src/dtpg_classic/SearchMgr.h
/// @brief SearchMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SearchMgr.h 2128 2009-03-12 07:25:33Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "Gate.h"


BEGIN_NAMESPACE_YM_ATPG

class SaFault;
class TvMgr;
class TestVector;

END_NAMESPACE_YM_ATPG


BEGIN_NAMESPACE_YM_ATPG_DTPGCL

//////////////////////////////////////////////////////////////////////
/// @class SearchMgr SearchMgr.h "SearchMgr.h"
/// @brief 含意を行うクラス
//////////////////////////////////////////////////////////////////////
class SearchMgr
{
public:

  /// @brief コンストラクタ
  SearchMgr();

  /// @brief デストラクタ
  ~SearchMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 探索関係の関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 故障 f に対するテストパタン生成を行う．
  /// @param[in] f 対象の故障
  /// @param[in] flist その他の未検出故障のリスト
  /// @param[in] dyn_mode dynamic implication のモード
  /// @param[out] tv 生成したパタンを入れるベクタ
  /// @param[out] tv で検出できる故障のリスト
  /// @retval kDetect 検出した．
  /// @retval kUntest 検出不能と判定した．
  /// @retval kAbort アボートした．
  tStat
  run(SaFault* f,
      const vector<SaFault*>& flist,
      tDynMode dyn_mode,
      TestVector* tv,
      vector<SaFault*>& det_faults);
    
  /// @brief jnode を val にするパタンを求める．
  bool
  basis_just(Gate* jnode,
	     Val3 val);

  /// @brief 値の割り当てを行う．
  void
  mand_assign(Gate* gate,
	      Val3 gval,
	      Val3 fval);
  
  /// @brief 値をセーブする．
  void
  save_value(Gate* gate);

  /// @brief 直前の implication() がアボートしていたら true を返す．
  bool
  is_aborted();

  /// @brief 直前の implication() で矛盾が起こっていたら true を返す．
  bool
  has_conflict();

  /// @brief 終端リストを得る．
  const vector<pair<Gate*, Val3> >&
  term_list() const;
  
  /// @brief 定数ノードのリストをクリアする．
  void
  clear_const();

  /// @brief 定数0ノードを追加する．
  void
  add_const0(Gate* gate);

  /// @brief 定数1ノードを追加する．
  void
  add_const1(Gate* gate);
  
  /// @brief イベントキューを初期化する．
  /// @note 定数ノードのイベントが自動的に挿入される．
  void
  eventq_clear();
  
  /// @brief イベントキューをフラッシュする．
  void
  eventq_flush();

  /// @brief イベントキューに追加する．
  void
  eventq_add(Gate* gate,
	     Val3 gval,
	     Val3 fval);

  
public:
  //////////////////////////////////////////////////////////////////////
  // 探索用のパラメータのアクセス関数
  //////////////////////////////////////////////////////////////////////


  /// @brief デフォルトのバックトラック制限をセットする．
  void
  set_default_backtrack_limit();

  /// @brief バックトラック制限を得る．
  ymuint
  backtrack_limit();

  /// @brief バックトラック制限をセットする．
  void
  set_backtrack_limit(ymuint limit);

  /// @brief バックトラック数を得る．
  ymuint
  backtrack_count();

  /// @brief バックトラック数のカウンタをクリアする．
  void
  clear_backtrack_count();
  

private:
  //////////////////////////////////////////////////////////////////////
  // run() の下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化する．
  void
  init();
  
  /// @brief 値の割り当てを行う．
  void
  opt_assign(Gate* gate,
	     Val3 gval,
	     Val3 fval);

  /// @brief 含意を行う．
  bool
  implication();

  /// @brief 含意を行う．
  bool
  implication2();
  
  /// @brief バックトラックを行う．
  bool
  back_track();

  /// @brief 値をもとに戻す．
  void
  restore_value();

  /// @brief 必要な割り当てを求める．
  void
  wrap_up(Gate* i_gate);

  /// @brief 必要な割り当てを求める．
  void
  wrap_up2(Gate* i_gate);

  /// @brief 直前に求めたテストベクタで flist が検出可能かどうか調べる．
  void
  drop_fault(SaFault* f0,
	     const vector<SaFault*>& flist,
	     vector<SaFault*>& det_faults);


private:
  //////////////////////////////////////////////////////////////////////
  // dynamic implication 関係の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief Dynamic Implication の候補リストを作る．
  /// @param[in] gate 対象のゲート
  void
  set_ex_cands(Gate* gate);
  
  /// @brief Dynamic Implication を行う．
  /// @return 割り当てが行われたら true を返す．
  bool
  examine_dyn_imp();

  /// @brief dynamic implication を行う．
  bool
  dyn_imp(Gate* gate,
	  Val3 gval,
	  Val3 fval);
  
  /// @brief gate の TFO とそれらの TFI にマークをつける．
  void
  mark_tfi_tfo(Gate* gate);

  /// @brief gate の TFI にマークをつける．
  void
  mark_tfi(Gate* gate);
  
  
private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造の定義
  //////////////////////////////////////////////////////////////////////

  struct StackElem
  {
    StackElem(Gate* gate,
	      ymuint32 state) :
      mGate(gate),
      mState(state) { }
	     
    Gate* mGate;
    ymuint32 mState;
  };

  struct EventElem
  {
    EventElem(Gate* gate,
	      Val3 gval,
	      Val3 fval) :
      mGate(gate),
      mGval(gval),
      mFval(fval) { }

    Gate* mGate;
    Val3 mGval;
    Val3 mFval;
  };
  
  struct AltElem
  {
    AltElem(Gate* gate,
	    Val3 gval,
	    Val3 fval,
	    ymuint32 bottom) :
      mGate(gate),
      mGval(gval),
      mFval(fval),
      mBottom(bottom) { }

    Gate* mGate;
    Val3 mGval;
    Val3 mFval;
    ymuint32 mBottom;
  };

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 値を保存しておくスタック
  vector<StackElem> mSaveStack;
  
  // イベントキュー
  vector<EventElem> mEventQ;
  
  // イベントキューの読み出し位置
  ymuint32 mEqPos;
  
  // alternative を覚えておくスタック
  vector<AltElem> mAltStack;
  
  // 総バックトラック数
  ymuint32 mAccBtNum;

  // 今のバックトラック数
  ymuint32 mBtNum;

  // バックトラック制限
  ymuint32 mBtLimit;

  // アボートフラグ
  bool mAborted;

  // コンフリクトフラグ
  bool mConflict;
  
  // 割り当てられた終端ノードのリスト
  vector<pair<Gate*, Val3> > mTermList;
  
  // Dynamic Implication の候補リスト
  vector<Gate*> mExCands;

  // Dynamic Implication を試した位置
  ymuint32 mCurPos;
  
  // 定数0に縮退しているゲートのリスト
  vector<Gate*> mConst0List;

  // 定数1に縮退しているゲートのリスト
  vector<Gate*> mConst1List;
  

private:
  //////////////////////////////////////////////////////////////////////
  // 定数
  //////////////////////////////////////////////////////////////////////

  // デフォルトのバックトラック制限
  static
  const ymuint32 DEFAULT_BACKTRACK_LIMIT = 16;
  
};


extern
SearchMgr sm;


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 終端リストを得る．
inline
const vector<pair<Gate*, Val3> >&
SearchMgr::term_list() const
{
  return mTermList;
}

// @brief イベントキューに追加する．
inline
void
SearchMgr::eventq_add(Gate* gate,
		      Val3 gval,
		      Val3 fval)
{
  mEventQ.push_back(EventElem(gate, gval, fval));
}

inline
void
SearchMgr::save_value(Gate* gate)
{
  mSaveStack.push_back(StackElem(gate, gate->mState));
}

END_NAMESPACE_YM_ATPG_DTPGCL

#endif
