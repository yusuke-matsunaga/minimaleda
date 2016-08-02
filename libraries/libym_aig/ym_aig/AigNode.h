#ifndef LIBYM_AIG_AIGNODE_H
#define LIBYM_AIG_AIGNODE_H

/// @file libym_aig/AigNode.h
/// @brief AigNodeのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: AigNode.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_aig/AigHandle.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
/// @class AigNode AigNode.h "AigNode.h"
/// @brief ノードを表すクラス
//////////////////////////////////////////////////////////////////////
class AigNode
{
  friend class AigMgrImpl;
  
private:
  // このクラスは AigMgrImpl 内でしか生成／破壊できない．
  
  /// @brief コンストラクタ
  /// @param[in] input 外部入力のときに true とするフラグ
  /// @param[in] id ノード番号
  AigNode(bool input,
	  ymuint id);
  
  /// @brief デストラクタ
  ~AigNode();

  
public:
  //////////////////////////////////////////////////////////////////////
  // 型とID番号に関する情報を得る関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 外部入力ノードのとき true を返す．
  bool
  is_input() const;

  /// @brief ANDノードのとき true を返す．
  bool
  is_and() const;
  
  /// @brief 通し番号を得る．
  ymuint
  node_id() const;
  
  /// @brief 外部入力ノードのときの入力番号を返す．
  /// @note is_input() の時のみ意味を持つ．
  ymuint
  input_id() const;

  
public:
  //////////////////////////////////////////////////////////////////////
  // ファンインに関する情報を得る関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief pos で指示されたファンインのノードを得る．
  /// @note pos は 0 か 1 でなければならない．
  AigNode*
  fanin(ymuint pos) const;
  
  /// @brief fanin0 のノードを得る．
  AigNode*
  fanin0() const;

  /// @brief fanin1 のノードを得る．
  AigNode*
  fanin1() const;
  
  /// @brief pos で指示されたファンインの極性を得る．
  /// @note pos は 0 か 1 でなければならない．
  bool
  fanin_inv(ymuint pos) const;

  /// @brief fanin0 の極性を得る．
  bool
  fanin0_inv() const;

  /// @brief fanin1 の極性を得る．
  bool
  fanin1_inv() const;

  /// @brief pos で指示されたファンインのハンドルを得る．
  /// @note pos は 0 か 1 でなければならない．
  AigHandle
  fanin_handle(ymuint pos) const;

  /// @brief fanin0 のハンドルを得る．
  AigHandle
  fanin0_handle() const;

  /// @brief fanin1 のハンドルを得る．
  AigHandle
  fanin1_handle() const;
 
  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 通し番号 + ノードタイプ
  ymuint32 mFlags;
  
  // ファンインの枝の配列
  AigHandle mFanins[2];
  
  // ハッシュ用のリンクポインタ
  AigNode* mLink;
  
  
private:
  //////////////////////////////////////////////////////////////////////
  // mFlags で用いるシフト定数
  //////////////////////////////////////////////////////////////////////

  // 型
  static
  const int kSftT = 0;
  
  // id 番号
  static
  const int kSftN = 4;
  
};


//////////////////////////////////////////////////////////////////////
// AigNode のインライン関数
//////////////////////////////////////////////////////////////////////

// @brief 入力ノードのとき true を返す．
inline
bool
AigNode::is_input() const
{
  return ((mFlags >> kSftT) & 1U) == 1U;
}

// @brief ANDノードのとき true を返す．
inline
bool
AigNode::is_and() const
{
  return !is_input();
}

// 通し番号を得る．
inline
ymuint
AigNode::node_id() const
{
  return static_cast<ymuint>(mFlags >> kSftN);
}
  
// @brief 外部入力ノードのときの入力番号を返す．
// @note is_input() の時のみ意味を持つ．
inline
ymuint
AigNode::input_id() const
{
  return mFanins[0].mPackedData;
}

// pos で指示されたファンインのノードを得る．
// pos は 0 か 1 でなければならない．
inline
AigNode*
AigNode::fanin(ymuint pos) const
{
  return mFanins[pos].node();
}

// fanin0 のノードを得る．
inline
AigNode*
AigNode::fanin0() const
{
  return mFanins[0].node();
}

// fanin1 のノードを得る．
inline
AigNode*
AigNode::fanin1() const
{
  return mFanins[1].node();
}

// pos で指示されたファンインの極性を得る．
// pos は 0 か 1 でなければならない．
inline
bool
AigNode::fanin_inv(ymuint pos) const
{
  return mFanins[pos].inv();
}

// fanin0 の極性を得る．
inline
bool
AigNode::fanin0_inv() const
{
  return mFanins[0].inv();
}

// fanin1 の極性を得る．
inline
bool
AigNode::fanin1_inv() const
{
  return mFanins[1].inv();
}

// @brief pos で指示されたファンインのハンドルを得る．
// @note pos は 0 か 1 でなければならない．
inline
AigHandle
AigNode::fanin_handle(ymuint pos) const
{
  return mFanins[pos];
}

// @brief fanin0 のハンドルを得る．
inline
AigHandle
AigNode::fanin0_handle() const
{
  return mFanins[0];
}

// @brief fanin1 のハンドルを得る．
inline
AigHandle
AigNode::fanin1_handle() const
{
  return mFanins[1];
}

END_NAMESPACE_YM_AIG

#endif // LIBYM_AIG_AIGNODE_H
