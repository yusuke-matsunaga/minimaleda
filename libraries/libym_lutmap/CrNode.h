#ifndef LIBYM_LUTMAP_CRNODE_H
#define LIBYM_LUTMAP_CRNODE_H

/// @file libym_lutmap/CrNode.h
/// @brief CrNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CrNode.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_lutmap/lutmap_nsdef.h"
#include "ym_sbj/sbj_nsdef.h"


BEGIN_NAMESPACE_YM_LUTMAP

class Cut;

//////////////////////////////////////////////////////////////////////
/// @class CrNode CrNode.h "CrNode.h"
/// @brief CutResub で用いる作業領域のクラス
//////////////////////////////////////////////////////////////////////
class CrNode
{
  friend class CrHeap;

public:

  /// @brief コンストラクタ
  CrNode();

  /// @brief デストラクタ
  ~CrNode();


public:

  /// @brief 対応する SbjNode を設定する．
  void
  set_sbjnode(const SbjNode* node);

  /// @brief 対応する SbjNode を返す．
  const SbjNode*
  sbjnode() const;

  /// @brief 外部入力ノードのとき true を返す．
  bool
  is_input() const;

  /// @brief 外部出力ノードにファンアウトしているとき true を返す．
  bool
  is_output() const;

  /// @brief 削除済みのマークをつける．
  void
  set_deleted();

  /// @brief 削除済みのとき true を返す．
  bool
  deleted() const;

  /// @brief GQ マークをつける．
  void
  set_GQ();

  /// @brief GQ マークを消す．
  void
  clear_GQ();

  /// @brief GQ に入っているとき true を返す．
  bool
  in_GQ() const;

  /// @brief LQ マークをつける．
  void
  set_LQ();

  /// @brief LQ マークを消す．
  void
  clear_LQ();

  /// @brief LQ に入っているとき true を返す．
  bool
  in_LQ() const;

  /// @brief RQ マークをつける．
  void
  set_RQ();

  /// @brief RQ マークを消す．
  void
  clear_RQ();

  /// @brief RQ に入っているとき true を返す．
  bool
  in_RQ() const;

  /// @brief カットをセットする．
  void
  set_cut(const Cut* cut);

  /// @brief 現在のカットを返す．
  const Cut*
  cut() const;

  /// @brief ファンアウトリストに追加する．
  /// @note 重複していたら追加せず false を返す．
  /// 通常は追加して true を返す．
  bool
  add_fanout(CrNode* add_node);

  /// @brief ファンアウトリストから削除する．
  void
  delete_fanout(CrNode* del_node);

  /// @brief ファンアウトリストを得る．
  const vector<CrNode*>&
  fanout_list();

  /// @brief ゲインをセットする．
  void
  set_gain(ymuint gain);

  /// @brief ゲインを得る．
  ymuint
  gain() const;

  /// @brief レベルをセットする．
  void
  set_level(ymuint level);

  /// @brief レベルを返す．
  ymuint
  level() const;

  /// @brief 要求レベルをセットする．
  void
  set_req_level(ymuint req_Level);

  /// @brief 要求レベルを返す．
  ymuint
  req_level() const;

  /// @brief lock マークをつける．
  void
  lock();

  /// @brief lock マークを消す．
  void
  unlock();

  /// @brief lock マークを返す．
  bool
  is_locked() const;

  /// @brief old マークをつける．
  void
  set_oldmark();

  /// @brief old マークを消す．
  void
  clear_oldmark();

  /// @brief old マークを返す．
  bool
  oldmark();

  /// @brief new マークをつける．
  void
  set_newmark();

  /// @brief new マークを消す．
  void
  clear_newmark();

  /// @brief new マークを返す．
  bool
  newmark();


public:

  // 代役候補のカットのリスト
  vector<const Cut*> mAltCutList;

  ymuint mTmpLevel;


private:

  // 根のノード
  const SbjNode* mNode;

  // このノードを根とする最適カット
  const Cut* mCurCut;

  // ファンアウト先のカットの根のノードのリスト
  vector<CrNode*> mFanoutList;

  // 種々のフラグ
  ymuint32 mFlags;

  // 現在のゲイン
  ymuint32 mCurGain;

  // ヒープ上のインデックス
  // ヒープに入っていない時は -1
  int mHeapIndex;

  // CrNode のネットワーク中の段数
  ymuint32 mLevel;

  // CrNode のネットワーク中の要求段数
  ymuint32 mReqLevel;


private:

  static
  const int kInputShift = 0;
  static
  const int kOutputShift = 1;
  static
  const int kDeletedShift = 2;
  static
  const int kGQShift = 3;
  static
  const int kLQShift = 4;
  static
  const int kRQShift = 5;
  static
  const int kLockShift = 6;
  static
  const int kOldShift = 7;
  static
  const int kNewShift = 8;

  static
  const ymuint kInputMask = 1U << kInputShift;
  static
  const ymuint kOutputMask = 1U << kOutputShift;
  static
  const ymuint kDeletedMask = 1U << kDeletedShift;
  static
  const ymuint kGQMask = 1U << kGQShift;
  static
  const ymuint kLQMask = 1U << kLQShift;
  static
  const ymuint kRQMask = 1U << kRQShift;
  static
  const ymuint kLockMask = 1U << kLockShift;
  static
  const ymuint kOldMask = 1U << kOldShift;
  static
  const ymuint kNewMask = 1U << kNewShift;

};


/// @relates CrNode
/// @brief CrHeap のための比較関数
bool
compare(CrNode* node1,
	CrNode* node2);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 対応する SbjNode を返す．
inline
const SbjNode*
CrNode::sbjnode() const
{
  return mNode;
}

// @brief カットをセットする．
inline
void
CrNode::set_cut(const Cut* cut)
{
  mCurCut = cut;
}

// @brief 現在のカットを返す．
inline
const Cut*
CrNode::cut() const
{
  return mCurCut;
}

// @brief 外部入力ノードのとき true を返す．
inline
bool
CrNode::is_input() const
{
  return static_cast<bool>((mFlags >> kInputShift) & 1U);
}

// @brief 外部出力ノードにファンアウトしているとき true を返す．
inline
bool
CrNode::is_output() const
{
  return static_cast<bool>((mFlags >> kOutputShift) & 1U);
}

// @brief 削除済みのマークをつける．
inline
void
CrNode::set_deleted()
{
  mFlags |= kDeletedMask;
}

// @brief 削除済みのとき true を返す．
inline
bool
CrNode::deleted() const
{
  return static_cast<bool>((mFlags >> kDeletedShift) & 1U);
}

// @brief GQ マークをつける．
inline
void
CrNode::set_GQ()
{
  mFlags |= (1U << kGQShift);
}

// @brief GQ マークを消す．
inline
void
CrNode::clear_GQ()
{
  mFlags &= ~(1U << kGQShift);
}

// @brief GQ に入っているとき true を返す．
inline
bool
CrNode::in_GQ() const
{
  return static_cast<bool>((mFlags >> kGQShift) & 1U);
}

// @brief LQ マークをつける．
inline
void
CrNode::set_LQ()
{
  mFlags |= (1U << kLQShift);
}

// @brief LQ マークを消す．
inline
void
CrNode::clear_LQ()
{
  mFlags &= ~(1U << kLQShift);
}

// @brief LQ に入っているとき true を返す．
inline
bool
CrNode::in_LQ() const
{
  return static_cast<bool>((mFlags >> kLQShift) & 1U);
}

// @brief RQ マークをつける．
inline
void
CrNode::set_RQ()
{
  mFlags |= (1U << kRQShift);
}

// @brief RQ マークを消す．
inline
void
CrNode::clear_RQ()
{
  mFlags &= ~(1U << kRQShift);
}

// @brief RQ に入っているとき true を返す．
inline
bool
CrNode::in_RQ() const
{
  return static_cast<bool>((mFlags >> kRQShift) & 1U);
}

// @brief ファンアウトリストを得る．
inline
const vector<CrNode*>&
CrNode::fanout_list()
{
  return mFanoutList;
}

// @brief ゲインをセットする．
inline
void
CrNode::set_gain(ymuint gain)
{
  mCurGain = gain;
}

// @brief ゲインを得る．
inline
ymuint
CrNode::gain() const
{
  return mCurGain;
}

// @brief レベルをセットする．
inline
void
CrNode::set_level(ymuint level)
{
  mLevel = level;
}

// @brief レベルを返す．
inline
ymuint
CrNode::level() const
{
  return mLevel;
}

// @brief 要求レベルをセットする．
inline
void
CrNode::set_req_level(ymuint req_level)
{
  mReqLevel = req_level;
}

// @brief 要求レベルを返す．
inline
ymuint
CrNode::req_level() const
{
  return mReqLevel;
}

// @brief lock マークをつける．
inline
void
CrNode::lock()
{
  mFlags |= kLockMask;
}

// @brief lock マークを消す．
inline
void
CrNode::unlock()
{
  mFlags &= ~kLockMask;
}

// @brief lock マークを返す．
inline
bool
CrNode::is_locked() const
{
  return static_cast<bool>((mFlags >> kLockShift) & 1U);
}

// @brief old マークをつける．
inline
void
CrNode::set_oldmark()
{
  mFlags |= kOldMask;
}

// @brief old マークを消す．
inline
void
CrNode::clear_oldmark()
{
  mFlags &= ~kOldMask;
}

// @brief old マークを返す．
inline
bool
CrNode::oldmark()
{
  return static_cast<bool>((mFlags >> kOldShift) & 1U);
}

// @brief new マークをつける．
inline
void
CrNode::set_newmark()
{
  mFlags |= kNewMask;
}

// @brief new マークを消す．
inline
void
CrNode::clear_newmark()
{
  mFlags &= ~kNewMask;
}

// @brief new マークを返す．
inline
bool
CrNode::newmark()
{
  return static_cast<bool>((mFlags >> kNewShift) & 1U);
}

END_NAMESPACE_YM_LUTMAP

#endif // LIBYM_LUTMAP_CRNODE_H
