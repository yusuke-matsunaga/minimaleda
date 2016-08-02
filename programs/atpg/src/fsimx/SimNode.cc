
/// @file fsim/SimNode.cc
/// @brief SimNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: SimNode.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "SimNode.h"
#include "SimFFR.h"
#include "SnInput.h"
#include "SnAnd.h"
#include "SnOr.h"
#include "SnXor.h"
#include "SnCplx.h"


BEGIN_NAMESPACE_YM_ATPG_FSIMX

//////////////////////////////////////////////////////////////////////
// 故障シミュレーション用のノードを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SimNode::SimNode(ymuint32 id) :
  mId(id),
  mNfo(0),
  mFanouts(NULL),
  mFanoutIpos(0),
  mFFR(NULL),
  mLevel(0)
{
}

// デストラクタ
SimNode::~SimNode()
{
  delete [] mFanouts;
}

// @brief ノードを生成するクラスメソッド
SimNode*
SimNode::new_node(ymuint32 id,
		  tTgGateType type,
		  const LogExpr& lexp,
		  const vector<SimNode*>& inputs)
{
  SimNode* node = NULL;
  ymuint ni = inputs.size();
  switch ( type ) {
  case kTgInput:
    assert_cond(ni == 0, __FILE__, __LINE__);
    node = new SnInput(id);
    break;
    
  case kTgBuff:
    assert_cond(ni == 1, __FILE__, __LINE__);
    node = new SnBuff(id, inputs);
    break;

  case kTgNot:
    assert_cond(ni == 1, __FILE__, __LINE__);
    node = new SnNot(id, inputs);
    break;

  case kTgAnd:
    switch ( ni ) {
    case 2:  node = new SnAnd2(id, inputs); break;
    case 3:  node = new SnAnd3(id, inputs); break;
    case 4:  node = new SnAnd4(id, inputs); break;
    default: node = new SnAnd(id, inputs);  break;
    }
    break;

  case kTgNand:
    switch ( ni ) {
    case 2:  node = new SnNand2(id, inputs); break;
    case 3:  node = new SnNand3(id, inputs); break;
    case 4:  node = new SnNand4(id, inputs); break;
    default: node = new SnNand(id, inputs);  break;
    }
    break;

  case kTgOr:
    switch ( ni ) {
    case 2:  node = new SnOr2(id, inputs); break;
    case 3:  node = new SnOr3(id, inputs); break;
    case 4:  node = new SnOr4(id, inputs); break;
    default: node = new SnOr(id, inputs);  break;
    }
    break;

  case kTgNor:
    switch ( ni ) {
    case 2:  node = new SnNor2(id, inputs); break;
    case 3:  node = new SnNor3(id, inputs); break;
    case 4:  node = new SnNor4(id, inputs); break;
    default: node = new SnNor(id, inputs);  break;
    }
    break;

  case kTgXor:
    switch ( ni ) {
    case 2:  node = new SnXor2(id, inputs); break;
    default: node = new SnXor(id, inputs);  break;
    }
    break;

  case kTgXnor:
    switch ( ni ) {
    case 2:  node = new SnXnor2(id, inputs); break;
    default: node = new SnXnor(id, inputs);  break;
    }
    break;

  default:
    node = new SnCplx(id, lexp, inputs);
    break;
  }
  return node;
}

// @brief FFR の根のノードの時 true を返す．
bool
SimNode::is_ffr_root() const
{
  return mFFR->root() == this;
}

// @brief レベルを設定する．
void
SimNode::set_level(ymuint level)
{
  mLevel = level;
}

// @brief ファンアウトリストを作成する．
void
SimNode::set_fanout_list(const vector<SimNode*>& fo_list,
			 ymuint ipos)
{
  mNfo = fo_list.size();
  mFanouts = new SimNode*[mNfo];
  ymuint i = 0;
  for (vector<SimNode*>::const_iterator p = fo_list.begin();
       p != fo_list.end(); ++ p, ++ i) {
    mFanouts[i] = *p;
  }
  mFanoutIpos |= (ipos << 2);
}

// @brief ローカルな obs の計算を行う．
PackedVal
SimNode::calc_lobs()
{
  if ( is_ffr_root() ) {
    return kPvAll1;
  }
  if ( !check_lobs() ) {
    SimNode* onode = fanout(0);
    ymuint pos = fanout_ipos();
    mLobs = onode->calc_lobs() & onode->calc_gobs3(pos);
    set_lobs();
  }
  return mLobs;
}

END_NAMESPACE_YM_ATPG_FSIMX
