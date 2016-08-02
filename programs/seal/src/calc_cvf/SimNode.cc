
/// @file calc_cvf/SimNode.cc
/// @brief SimNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: SimNode.cc 1920 2008-12-20 15:52:42Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.

#if HAVE_CONFIG_H
#include "seal_config.h"
#endif


#include "SimNode.h"
#include "SimFFR.h"


BEGIN_NAMESPACE_YM_SEAL_CVF

//////////////////////////////////////////////////////////////////////
// 故障シミュレーション用のノードを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SimNode::SimNode(ymuint32 id) :
  mId(id),
  mNfo(0),
  mFanouts(NULL),
  mFFR(NULL),
  mLevel(0),
  mFlags(0UL)
{
}

// デストラクタ
SimNode::~SimNode()
{
}

// @brief FFR の根のノードの時 true を返す．
bool
SimNode::is_ffr_root() const
{
  return mFFR->root() == this;
}

// @brief レベルを設定する．
void
SimNode::set_level(size_t level)
{
  mLevel = level;
}

END_NAMESPACE_YM_SEAL_CVF
