#ifndef LIBYM_SBJ_SMDNODE_H
#define LIBYM_SBJ_SMDNODE_H

/// @file libym_techmap/SmdNode.h
/// @brief SmdNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_sbj/sbj_nsdef.h"


BEGIN_NAMESPACE_YM_SBJ

//////////////////////////////////////////////////////////////////////
/// @class SmdEdge SmdNode.h "SmdNode.h"
/// @brief SmdNode をつなぐ枝を表すクラス
//////////////////////////////////////////////////////////////////////
class SmdEdge
{
  friend class SmdNode;

public:
  //////////////////////////////////////////////////////////////////////
  // 構造に関する情報を取得す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力側のノードを得る．
  SmdNode*
  from();

  /// @brief 出力側のノードを得る．
  SmdNode*
  to();

  /// @brief 出力側のノードのファンイン番号 ( 0 or 1 ) を得る．
  ymuint
  pos();


public:
  //////////////////////////////////////////////////////////////////////
  // get_min_depth() で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief flow が流れている時 true を返す．
  bool
  flow() const;

  /// @brief flow フラグをセットする．
  void
  set_flow();

  /// @brief flow フラグをクリアする．
  void
  clear_flow();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力側のノード
  SmdNode* mFrom;

  // 出力側のノード
  SmdNode* mTo;

  // ファンイン番号＋ flow フラグ
  ymuint32 mFlags;

};


//////////////////////////////////////////////////////////////////////
/// @class SmdNode SmdNode.h "SmdNode.h"
/// @brief SbjMinDepth 用の作業領域
//////////////////////////////////////////////////////////////////////
class SmdNode
{
public:

  /// @brief コンストラクタ
  SmdNode();

  /// @brief デストラクタ
  ~SmdNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 構造に関する情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号の取得
  ymuint
  id() const;

  /// @brief 入力ノードの時に true を返す．
  bool
  is_input() const;

  /// @brief 論理ノードの時に true を返す．
  bool
  is_logic() const;

  /// @brief 1つ目のファンインを得る．
  SmdNode*
  fanin0();

  /// @brief 2つ目のファンインを得る．
  SmdNode*
  fanin1();

  /// @brief 1つ目のファンインの枝を得る．
  SmdEdge*
  fanin0_edge();

  /// @brief 2つ目のファンインの枝を得る．
  SmdEdge*
  fanin1_edge();

  /// @brief ファンアウト数を得る．
  ymuint
  fanout_num();

  /// @brief pos 番目のファンアウトの枝を取り出す．
  SmdEdge*
  fanout_edge(ymuint pos);

  /// @brief 深さを得る．
  ymuint
  depth() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を設定する．
  /// @param[in] id ID番号
  /// @param[in] input 入力ノードの時 true にセットするフラグ
  void
  set_id(ymuint id,
	 bool input = false);

  /// @brief 1つ目のファンインを設定する．
  void
  set_fanin0(SmdNode* from);

  /// @brief 2つ目のファンインを設定する．
  void
  set_fanin1(SmdNode* from);

  /// @brief ファンアウト数を設定する．
  void
  set_fanout_num(ymuint n,
		 void* p);

  /// @brief pos 番目のファンアウトを設定する．
  void
  set_fanout(ymuint pos,
	     SmdEdge* edge);


public:
  //////////////////////////////////////////////////////////////////////
  // get_min_depth() で用いる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief range マークを調べる．
  /// @note 副作用として range マークを付ける．
  bool
  check_rmark();

  /// @brief range マークを返す．
  bool
  rmark();

  /// @brief target マークを返す．
  bool
  tmark();

  /// @brief target マークを付ける．
  void
  set_tmark();

  /// @brief flow 用のマークを返す．
  bool
  fmark();

  /// @brief flow 用のマークを付ける．
  void
  set_fmark();

  /// @brief range/target/flow マークを消す．
  void
  clear_rtfmark();

  /// @brief 入力側の visit フラグを調べる．
  /// @note 副作用として v1 マークを付ける．
  bool
  check_vmark1();

  /// @brief 出力側の visit フラグを調べる．
  /// @note 副作用として v2 マークを付ける．
  bool
  check_vmark2();

  /// @brief visit フラグを消す．
  void
  clear_vmark();

  /// @brief 深さを設定する．
  void
  set_depth(ymuint depth);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号 + 入力/論理フラグ
  ymuint32 mId;

  // 1つ目のファンインの枝
  SmdEdge mFanin0;

  // 2つ目のファンインの枝
  SmdEdge mFanin1;

  // ファンアウト数
  ymuint32 mFanoutNum;

  // ファンアウトの枝のポインタの配列
  SmdEdge** mFanoutArray;

  // get_min_depth() 用の作業領域
  ymuint32 mMark;

  // 深さ
  ymuint32 mDepth;


private:
  //////////////////////////////////////////////////////////////////////
  // 定数の定義
  //////////////////////////////////////////////////////////////////////

  // mMarkArray の演算で用いる定数
  static
  const int kRangeShift = 0;
  static
  const int kTargetShift = 1;
  static
  const int kFlowShift = 2;
  static
  const int kPoShift = 3;
  static
  const int kV1Shift = 4;
  static
  const int kV2Shift = 5;

  static
  const ymuint kRangeMask = 1U << kRangeShift;
  static
  const ymuint kTargetMask = 1U << kTargetShift;
  static
  const ymuint kFlowMask = 1U << kFlowShift;
  static
  const ymuint kPoMask = 1U << kPoShift;
  static
  const ymuint kV1Mask = 1U << kV1Shift;
  static
  const ymuint kV2Mask = 1U << kV2Shift;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 入力側のノードを得る．
inline
SmdNode*
SmdEdge::from()
{
  return mFrom;
}

// @brief 出力側のノードを得る．
inline
SmdNode*
SmdEdge::to()
{
  return mTo;
}

// @brief 出力側のノードのファンイン番号 ( 0 or 1 ) を得る．
inline
ymuint
SmdEdge::pos()
{
  return (mFlags & 1U);
}

// @brief flow が流れている時 true を返す．
inline
bool
SmdEdge::flow() const
{
  return static_cast<bool>((mFlags >> 1) & 1U);
}

// @brief flow フラグをセットする．
inline
void
SmdEdge::set_flow()
{
  mFlags |= 2U;
}

// @brief flow フラグをクリアする．
inline
void
SmdEdge::clear_flow()
{
  mFlags &= ~2U;
}

// @brief ID番号の取得
inline
ymuint
SmdNode::id() const
{
  return (mId >> 1);
}

// @brief 入力ノードの時に true を返す．
inline
bool
SmdNode::is_input() const
{
  return !is_logic();
}

// @brief 論理ノードの時に true を返す．
inline
bool
SmdNode::is_logic() const
{
  return static_cast<bool>(mId & 1U);
}

// @brief 1つ目のファンインを得る．
inline
SmdNode*
SmdNode::fanin0()
{
  return mFanin0.from();
}

// @brief 2つ目のファンインを得る．
inline
SmdNode*
SmdNode::fanin1()
{
  return mFanin1.from();
}

// @brief 1つ目のファンインの枝を得る．
inline
SmdEdge*
SmdNode::fanin0_edge()
{
  return &mFanin0;
}

// @brief 2つ目のファンインの枝を得る．
inline
SmdEdge*
SmdNode::fanin1_edge()
{
  return &mFanin1;
}

// @brief ファンアウト数を得る．
inline
ymuint
SmdNode::fanout_num()
{
  return mFanoutNum;
}

// @brief pos 番目のファンアウトの枝を取り出す．
inline
SmdEdge*
SmdNode::fanout_edge(ymuint pos)
{
  return mFanoutArray[pos];
}

// @brief 深さを得る．
inline
ymuint
SmdNode::depth() const
{
  return mDepth;
}

// @brief range マークを調べる．
// @note 副作用として range マークを付ける．
inline
bool
SmdNode::check_rmark()
{
  ymuint old_mark = mMark;
  mMark |= kRangeMask;
  return static_cast<bool>((old_mark >> kRangeShift) & 1U);
}

// @brief range マークを返す．
inline
bool
SmdNode::rmark()
{
  return static_cast<bool>((mMark >> kRangeShift) & 1U);
}

// @brief target マークを返す．
inline
bool
SmdNode::tmark()
{
  return static_cast<bool>((mMark >> kTargetShift) & 1U);
}

// @brief target マークを付ける．
inline
void
SmdNode::set_tmark()
{
  mMark |= kTargetMask;
}

// @brief flow 用のマークを返す．
inline
bool
SmdNode::fmark()
{
  return static_cast<bool>((mMark >> kFlowShift) & 1U);
}

// @brief flow 用のマークを付ける．
inline
void
SmdNode::set_fmark()
{
  mMark |= kFlowMask;
}

// @brief range/target/flow マークを消す．
inline
void
SmdNode::clear_rtfmark()
{
  mMark &= ~(kRangeMask | kTargetMask | kFlowMask);
}

// @brief 入力側の visit フラグを調べる．
// @note 副作用として v1 マークを付ける．
inline
bool
SmdNode::check_vmark1()
{
  ymuint32 old_mark = mMark;
  mMark |= kV1Mask;
  return static_cast<bool>((old_mark >> kV1Shift) & 1U);
}

// @brief 出力側の visit フラグを調べる．
// @note 副作用として v2 マークを付ける．
inline
bool
SmdNode::check_vmark2()
{
  ymuint32 old_mark = mMark;
  mMark |= kV2Mask;
  return static_cast<bool>((old_mark >> kV2Shift) & 1U);
}

// @brief visit フラグを消す．
inline
void
SmdNode::clear_vmark()
{
  mMark &= ~(kV1Mask | kV2Mask);
}

// @brief 深さを設定する．
inline
void
SmdNode::set_depth(ymuint depth)
{
  mDepth = depth;
}

END_NAMESPACE_YM_SBJ

#endif // LIBYM_SBJ_SMDNODE_H
