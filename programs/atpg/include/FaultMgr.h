#ifndef BASE_FAULTMGR_H
#define BASE_FAULTMGR_H

/// @file src/base/FaultMgr.h
/// @brief FaultMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: FaultMgr.h 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.

#include "atpg_nsdef.h"
#include "SaFault.h"
#include <ym_tgnet/tgnet_nsdef.h>
#include <ym_utils/Alloc.h>


BEGIN_NAMESPACE_YM_ATPG

class TestVector;

//////////////////////////////////////////////////////////////////////
/// @class FaultMgr FaultMgr.h "FaultMgr.h"
/// @brief 故障リストを表すクラス
//////////////////////////////////////////////////////////////////////
class FaultMgr
{
public:

  /// @brief コンストラクタ
  FaultMgr();

  /// @brief デストラクタ
  ~FaultMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // read-only のメソッド
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 全ての故障のリストを得る．
  const vector<SaFault*>&
  all_list() const;
  
  /// @brief すべての故障数を得る．
  ymuint
  all_num() const;
  
  /// @brief 全ての代表故障のリストを得る．
  const vector<SaFault*>&
  all_rep_list() const;
  
  /// @brief すべての代表故障数を得る．
  ymuint
  all_rep_num() const;
  
  /// @brief 検出済みの代表故障のリストを得る．
  const vector<SaFault*>&
  det_list() const;

  /// @brief 検出済みの代表故障数を得る．
  ymuint
  det_num() const;
  
  /// @brief 未検出の代表故障のリストを得る．
  const vector<SaFault*>&
  remain_list() const;
  
  /// @brief 未検出の代表故障数を得る．
  ymuint
  remain_num() const;
  
  /// @brief 検出不能故障のリストを得る．
  const vector<SaFault*>&
  untest_list() const;
  
  /// @brief 検出不能故障数を得る．
  ymuint
  untest_num() const;

  
public:
  //////////////////////////////////////////////////////////////////////
  // 内容を書き換えるメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief クリアする．
  void
  clear();

  /// @brief network の全ての単一縮退故障を設定する．
  void
  set_ssa_fault(const TgNetwork& network);
  
  /// @brief 出力の故障を追加する．
  /// @param[in] node 対象のノード
  /// @param[in] val 縮退している値
  /// @param[in] rep 代表故障
  SaFault*
  add_ofault(const TgNode* node,
	     int val,
	     SaFault* rep = NULL);
  
  /// @brief 入力の故障を追加する．
  /// @param[in] node 対象のノード
  /// @param[in] pos 入力の故障の時に入力番号を表す
  /// @param[in] val 縮退している値
  /// @param[in] rep 代表故障
  SaFault*
  add_ifault(const TgNode* node,
	     ymuint pos,
	     int val,
	     SaFault* rep = NULL);
  
  /// @brief fault の状態を変更する．
  void
  set_status(SaFault* fault,
	     FaultStatus stat);
  
  /// @brief 故障リストをスキャンして未検出リストを更新する．
  void
  update();


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief node に関する故障を登録する．
  void
  reg_faults(const TgNode* node);
  
  /// @brief 故障を生成する．
  /// @param[in] node 対象のノード
  /// @param[in] is_output 出力の故障のときに true とするフラグ
  /// @param[in] pos 入力の故障の時に入力番号を表す
  /// @param[in] val 縮退している値
  SaFault*
  new_fault(const TgNode* node,
	    bool is_output,
	    ymuint pos,
	    int val);
  
  /// @brief 出力の故障を取り出す．
  /// @param[in] node 対象のノード
  /// @param[in] val 縮退している値
  SaFault*
  find_ofault(const TgNode* node,
	      int val);

  /// @brief 入力の故障を取り出す．
  /// @param[in] node 対象のノード
  /// @param[in] pos 入力の故障の時に入力番号を表す
  /// @param[in] val 縮退している値
  SaFault*
  find_ifault(const TgNode* node,
	      ymuint pos,
	      int val);
  
  /// @brief 故障を追加する．
  /// @param[in] node 対象のノード
  /// @param[in] is_output 出力の故障のときに true とするフラグ
  /// @param[in] pos 入力の故障の時に入力番号を表す
  /// @param[in] val 縮退している値
  /// @param[in] rep 代表故障
  SaFault*
  add_fault(const TgNode* node,
	    bool is_output,
	    ymuint pos,
	    int val,
	    SaFault* rep);
  
  
private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造の定義
  //////////////////////////////////////////////////////////////////////

  // ノード1つあたりの故障の情報を表すクラス
  struct Fnode
  {
    // 出力の故障(01/)
    SaFault* mOfault[2];

    // 入力の故障(各ファンイン x 0/1)
    SaFault** mIfaults;
  };

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // SaFault の確保用のアロケータ
  UnitAlloc mFaultAlloc;

  // Finfo の確保用アロケータ
  UnitAlloc mFinfoAlloc;

  // Fnode.mIfaults の確保用アロケータ
  SimpleAlloc mIfaultsAlloc;
  
  // 関連付けられているネットワーク
  const TgNetwork* mNetwork;
  
  // ノードごとの故障の情報
  vector<Fnode> mFnodeArray;
  
  // 全ての故障を保持しておくリスト
  vector<SaFault*> mAllList;
  
  // 全ての代表故障を保持しておくリスト
  vector<SaFault*> mAllRepList;
  
  // 検出済みの故障を保持しておくリスト
  vector<SaFault*> mDetList;
  
  // 未検出の故障を保持しておくリスト
  vector<SaFault*> mRemainList;

  // 検出不能故障を保持しておくリスト
  vector<SaFault*> mUntestList;
  
  // 故障リストに変化があったことを記録するフラグ
  bool mChanged;
  
};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 全ての故障のリストを得る．
inline
const vector<SaFault*>&
FaultMgr::all_list() const
{
  return mAllList;
}
  
// @brief すべての故障数を得る．
inline
ymuint
FaultMgr::all_num() const
{
  return mAllList.size();
}

// @brief 全ての代表故障のリストを得る．
inline
const vector<SaFault*>&
FaultMgr::all_rep_list() const
{
  return mAllRepList;
}
  
// @brief すべての代表故障数を得る．
inline
ymuint
FaultMgr::all_rep_num() const
{
  return mAllRepList.size();
}
  
// @brief 検出済みの代表故障のリストを得る．
inline
const vector<SaFault*>&
FaultMgr::det_list() const
{
  return mDetList;
}

// @brief 検出済みの代表故障数を得る．
inline
ymuint
FaultMgr::det_num() const
{
  return mDetList.size();
}

// @brief 未検出の故障のリストを得る．
inline
const vector<SaFault*>&
FaultMgr::remain_list() const
{
  return mRemainList;
}
  
// @brief 未検出の代表故障数を得る．
inline
ymuint
FaultMgr::remain_num() const
{
  return mRemainList.size();
}

// @brief 検出不能故障のリストを得る．
inline
const vector<SaFault*>&
FaultMgr::untest_list() const
{
  return mUntestList;
}
  
// @brief 検出不能故障数を得る．
inline
ymuint
FaultMgr::untest_num() const
{
  return mUntestList.size();
}

// @brief 出力の故障を追加する．
inline
SaFault*
FaultMgr::add_ofault(const TgNode* node,
		     int val,
		     SaFault* rep)
{
  return add_fault(node, true, 0, val, rep);
}

// @brief 入力の故障を追加する．
inline
SaFault*
FaultMgr::add_ifault(const TgNode* node,
		     ymuint pos,
		     int val,
		     SaFault* rep)
{
  return add_fault(node, false, pos, val, rep);
}

END_NAMESPACE_YM_ATPG

#endif // ATPG_FAULTMGR_H
