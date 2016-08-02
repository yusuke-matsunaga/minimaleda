
/// @file libym_verilog/tests/ptview/PtNode.cc
/// @brief PtNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: PtNode.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include "PtNode.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス PtNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PtNode::PtNode() :
  mExpanded(false)
{
}

// @brief デストラクタ
PtNode::~PtNode()
{
  for (vector<PtNode*>::iterator p = mChildren.begin();
       p != mChildren.end(); ++ p) {
    delete *p;
  }
}


//////////////////////////////////////////////////////////////////////
// クラス PtScalarNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PtScalarNode::PtScalarNode()
{
}

// @brief デストラクタ
PtScalarNode::~PtScalarNode()
{
}

// @brief 対象のファイル上での位置を返す．
// @note このクラスでは空の FileRegion を返す．
FileRegion
PtScalarNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
// @note このクラスではなにもしない．
void
PtScalarNode::expand() const
{
}

END_NAMESPACE_YM_VERILOG
