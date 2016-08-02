
/// @file bddsim/SimNode.cc
/// @brief SimNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: SimNode.cc 1920 2008-12-20 15:52:42Z matsunaga $
///
/// Copyright (C) 2005-2007 Yusuke Matsunaga
/// All rights reserved.

#if HAVE_CONFIG_H
#include "seal_config.h"
#endif


#include "SimNode.h"


BEGIN_NAMESPACE_YM_SEAL_BDDSIM

//////////////////////////////////////////////////////////////////////
// 故障シミュレーション用のノードを表すクラス
//////////////////////////////////////////////////////////////////////

// 入力ノード用のコンストラクタ
SimNode::SimNode(ymuint32 id) :
  mId(id),
  mGateType(kTgInput),
  mNfi(0),
  mFanins(NULL),
  mNfo(0),
  mFanouts(NULL),
  mFfrRoot(NULL),
  mLevel(0),
  mFlags(0UL),
  mLink(NULL)
{
}

// コンストラクタ
// この状態ではファンアウトになにもつながっていない．
SimNode::SimNode(ymuint32 id,
		 tTgGateType type,
		 const vector<SimNode*>& inputs) :
  mId(id),
  mGateType(type),
  mNfi(inputs.size()),
  mFanins(new SimNode*[mNfi]),
  mNfo(0),
  mFanouts(NULL),
  mFfrRoot(NULL),
  mLevel(0),
  mFlags(0UL),
  mLink(NULL)
{
  size_t max_level = 0;
  for (size_t i = 0; i < mNfi; ++ i) {
    SimNode* input = inputs[i];
    assert_cond(input, __FILE__, __LINE__);
    mFanins[i] = input;
    if ( max_level < input->level() ) {
      max_level = input->level();
    }
  }

  set_level(max_level + 1);
}

// コンストラクタ
// この状態ではファンアウトになにもつながっていない．
SimNode::SimNode(ymuint32 id,
		 tTgGateType type,
		 const LogExpr& lexp,
		 const vector<SimNode*>& inputs) :
  mId(id),
  mGateType(type),
  mNfi(inputs.size()),
  mFanins(new SimNode*[mNfi]),
  mNfo(0),
  mFanouts(NULL),
  mFfrRoot(NULL),
  mLevel(0),
  mFlags(0UL),
  mLink(NULL)
{
  assert_cond(type >= kTgUsrDef, __FILE__, __LINE__);
  mLexp = lexp;
  
  size_t max_level = 0;
  for (size_t i = 0; i < mNfi; ++ i) {
    SimNode* input = inputs[i];
    assert_cond(input, __FILE__, __LINE__);
    mFanins[i] = input;
    if ( max_level < input->level() ) {
      max_level = input->level();
    }
  }

  set_level(max_level + 1);
}

// デストラクタ
SimNode::~SimNode()
{
  delete [] mFanins;
  delete [] mFanouts;
}

// @brief レベルを設定する．
inline
void
SimNode::set_level(size_t level)
{
  // キューフラグはクリアされる．
  mLevel = level;
  clear_queue();
}

// @brief 出力マークをつける．
void
SimNode::set_output()
{
  mFlags |= (1UL << 1);
}

// @brief ファンアウトリストを作成する．
void
SimNode::set_fanout_list(const vector<SimNode*>& fo_list)
{
  mNfo = fo_list.size();
  mFanouts = new SimNode*[mNfo];
  size_t i = 0;
  for (vector<SimNode*>::const_iterator p = fo_list.begin();
       p != fo_list.end(); ++ p, ++ i) {
    mFanouts[i] = *p;
  }
}

// @brief 正常値の計算を行う．
// @note 結果は mGfunc にセットされる．
void
SimNode::calc_gfunc()
{
  Bdd new_func = mFanins[0]->mGfunc;
  switch ( gate_type() ) {
  case kTgUndef:
  case kTgInput:
  case kTgOutput:
    assert_not_reached(__FILE__, __LINE__);
    break;

  case kTgBuff:
    break;

  case kTgNot:
    new_func = ~new_func;
    break;

  case kTgAnd:
    switch ( mNfi ) {
    case 2:
      new_func &= mFanins[1]->mGfunc;
      break;

    case 3:
      new_func &= mFanins[1]->mGfunc;
      new_func &= mFanins[2]->mGfunc;
      break;

    case 4:
      new_func &= mFanins[1]->mGfunc;
      new_func &= mFanins[2]->mGfunc;
      new_func &= mFanins[3]->mGfunc;
      break;
      
    default:
      for (size_t i = 1; i < mNfi; ++ i) {
	new_func &= mFanins[i]->mGfunc;
      }
      break;
    }
    break;

  case kTgNand:
    switch ( mNfi ) {
    case 2:
      new_func &= mFanins[1]->mGfunc;
      new_func = ~new_func;
      break;
      
    case 3:
      new_func &= mFanins[1]->mGfunc;
      new_func &= mFanins[2]->mGfunc;
      new_func = ~new_func;
      break;
      
    case 4:
      new_func &= mFanins[1]->mGfunc;
      new_func &= mFanins[2]->mGfunc;
      new_func &= mFanins[3]->mGfunc;
      new_func = ~new_func;
      break;
      
    default:
      for (size_t i = 1; i < mNfi; ++ i) {
	new_func &= mFanins[i]->mGfunc;
      }
      new_func = ~new_func;
      break;
    }
    break;
    
  case kTgOr:
    switch ( mNfi ) {
    case 2:
      new_func |= mFanins[1]->mGfunc;
      break;
      
    case 3:
      new_func |= mFanins[1]->mGfunc;
      new_func |= mFanins[2]->mGfunc;
      break;
      
    case 4:
      new_func |= mFanins[1]->mGfunc;
      new_func |= mFanins[2]->mGfunc;
      new_func |= mFanins[3]->mGfunc;
      break;
      
    default:
      for (size_t i = 1; i < mNfi; ++ i) {
	new_func |= mFanins[i]->mGfunc;
      }
      break;
    }
    break;

  case kTgNor:
    switch ( mNfi ) {
    case 2:
      new_func |= mFanins[1]->mGfunc;
      new_func = ~new_func;
      break;
      
    case 3:
      new_func |= mFanins[1]->mGfunc;
      new_func |= mFanins[2]->mGfunc;
      new_func = ~new_func;
      break;
      
    case 4:
      new_func |= mFanins[1]->mGfunc;
      new_func |= mFanins[2]->mGfunc;
      new_func |= mFanins[3]->mGfunc;
      new_func = ~new_func;
      break;
      
    default:
      for (size_t i = 1; i < mNfi; ++ i) {
	new_func |= mFanins[i]->mGfunc;
      }
      new_func = ~new_func;
      break;
    }

  case kTgXor:
    for (size_t i = 1; i < mNfi; ++ i) {
      new_func ^= mFanins[i]->mGfunc;
    }
    break;

  case kTgXnor:
    for (size_t i = 1; i < mNfi; ++ i) {
      new_func ^= mFanins[i]->mGfunc;
    }
    new_func = ~new_func;
    break;

  default: // cplx
    {
      VarBddMap varmap;
      for (size_t i = 0; i < mNfi; ++ i) {
	varmap.insert(make_pair(i, mFanins[i]->mGfunc));
      }
      BddMgrRef mgr = mFanins[0]->mGfunc.mgr();
      new_func = mgr.expr_to_bdd(mLexp, varmap);
    }
    break;
  }
  mGfunc = new_func;
  mFfunc = new_func;
}

// @brief 故障値の計算を行う．
// @return 故障差関数を返す．
// @note 結果は mFval にセットされる．
Bdd
SimNode::calc_ffunc()
{
  Bdd new_func = mFanins[0]->mFfunc;
  switch ( gate_type() ) {
  case kTgUndef:
  case kTgInput:
  case kTgOutput:
    assert_not_reached(__FILE__, __LINE__);
    break;

  case kTgBuff:
    break;

  case kTgNot:
    new_func = ~new_func;
    break;

  case kTgAnd:
    switch ( mNfi ) {
    case 2:
      new_func &= mFanins[1]->mFfunc;
      break;
      
    case 3:
      new_func &= mFanins[1]->mFfunc;
      new_func &= mFanins[2]->mFfunc;
      break;
      
    case 4:
      new_func &= mFanins[1]->mFfunc;
      new_func &= mFanins[2]->mFfunc;
      new_func &= mFanins[3]->mFfunc;
      break;
      
    default:
      for (size_t i = 1; i < mNfi; ++ i) {
	new_func &= mFanins[i]->mFfunc;
      }
      break;
    }
    break;

  case kTgNand:
    switch ( mNfi ) {
    case 2:
      new_func &= mFanins[1]->mFfunc;
      new_func = ~new_func;
      break;
      
    case 3:
      new_func &= mFanins[1]->mFfunc;
      new_func &= mFanins[2]->mFfunc;
      new_func = ~new_func;
      break;
      
    case 4:
      new_func &= mFanins[1]->mFfunc;
      new_func &= mFanins[2]->mFfunc;
      new_func &= mFanins[3]->mFfunc;
      new_func = ~new_func;
      break;
      
    default:
      for (size_t i = 1; i < mNfi; ++ i) {
	new_func &= mFanins[i]->mFfunc;
      }
      new_func = ~new_func;
      break;
    }
    break;

  case kTgOr:
    switch ( mNfi ) {
    case 2:
      new_func |= mFanins[1]->mFfunc;
      break;
      
    case 3:
      new_func |= mFanins[1]->mFfunc;
      new_func |= mFanins[2]->mFfunc;
      break;
      
    case 4:
      new_func |= mFanins[1]->mFfunc;
      new_func |= mFanins[2]->mFfunc;
      new_func |= mFanins[3]->mFfunc;
      break;
      
    default:
      for (size_t i = 1; i < mNfi; ++ i) {
	new_func |= mFanins[i]->mFfunc;
      }
      break;
    }
    break;

  case kTgNor:
    switch ( mNfi ) {
    case 2:
      new_func |= mFanins[1]->mFfunc;
      new_func = ~new_func;
      break;
      
    case 3:
      new_func |= mFanins[1]->mFfunc;
      new_func |= mFanins[2]->mFfunc;
      new_func = ~new_func;
      break;
      
    case 4:
      new_func |= mFanins[1]->mFfunc;
      new_func |= mFanins[2]->mFfunc;
      new_func |= mFanins[3]->mFfunc;
      new_func = ~new_func;
      break;
      
    default:
      for (size_t i = 1; i < mNfi; ++ i) {
	new_func |= mFanins[i]->mFfunc;
      }
      new_func = ~new_func;
      break;
    }
    break;

  case kTgXor:
    for (size_t i = 1; i < mNfi; ++ i) {
      new_func ^= mFanins[i]->mFfunc;
    }
    break;

  case kTgXnor:
    for (size_t i = 1; i < mNfi; ++ i) {
      new_func ^= mFanins[i]->mFfunc;
    }
    new_func = ~new_func;
    break;

  default:
    {
      VarBddMap varmap;
      for (size_t i = 0; i < mNfi; ++ i) {
	varmap.insert(make_pair(i, mFanins[i]->mFfunc));
      }
      BddMgrRef mgr = mFanins[0]->mFfunc.mgr();
      new_func = mgr.expr_to_bdd(mLexp, varmap);
    }
    break;
  }
  mFfunc = new_func;
  return ( mGfunc ^ mFfunc );
}

// @brief 入力の可観測性を再帰的に計算する．
void
SimNode::calc_iobs(const Bdd& obs,
		   bool force)
{
#if 0
  if ( force || !(is_output() || nfo() != 1) ) {
    set_obs(obs);
    switch ( gate_type() ) {
    case kTgUndef:
    case kTgOutput:
      assert_not_reached(__FILE__, __LINE__);
      break;

    case kTgInput:
      break;

    case kTgBuff:
    case kTgNot:
      iobs[0]->calc_iobs(obs);
      break;

  case kTgAnd:
  case kTgNand:
    switch ( mNfi ) {
    case 2:
      iobs[0]->calc_iobs(obs & mFanins[1]->get_gfunc());
      iobs[1]->calc_iobs(obs & mFanins[0]->get_gfunc());
      break;

    case 3:
      {
	Bdd v0 = mFanins[0]->get_gfunc();
	Bdd v1 = mFanins[1]->get_gfunc();
	Bdd v2 = mFanins[2]->get_gfunc();
	iobs[0]->calc_iobs(obs & v1 & v2);
	iobs[1]->calc_iobs(obs & v0 & v2);
	iobs[2]->calc_iobs(obs & v0 & v1);
      }
      break;
      
    case 4:
      {
	Bdd v0 = mFanins[0]->get_gfunc();
	Bdd v1 = mFanins[1]->get_gfunc();
	Bdd v2 = mFanins[2]->get_gfunc();
	Bdd v3 = mFanins[3]->get_gfunc();
	Bdd l1 = v0;
	Bdd l2 = l1 & v1;
	Bdd l3 = l2 & v2;
	Bdd u2 = v3;
	Bdd u1 = u2 & v2;
	Bdd u0 = u1 & v1;
	iobs[0]->calc_iobs(obs & u0);
	iobs[1]->calc_iobs(obs & u1 & l1);
	iobs[2]->calc_iobs(obs & u2 & l2);
	iobs[3]->calc_iobs(obs & l3);
      }
      break;
    
    default:
      {
	// mTmp1[i] は i - 1 以下の位置のファンインによる obs マスク
	// mTmp2[i] は i + 1 以上の位置のファンインによる obs マスク
	vector<Bdd> tmp1(mNfi);
	vector<Bdd> tmp2(mNfi);
	BddMgrRef mgr = obs.mgr();
	Bdd tmp0 = mgr.make_one();
	for (size_t i = 0; i < mNfi; ++ i) {
	  tmp1[i] = tmp0;
	  tmp0 &= mFanins[i]->get_gfunc();
	}
	tmp0 = mgr.make_one();
	for (size_t i = 1; i <= mNfi; ++ i) {
	  tmp2[mNfi - i] = tmp0;
	  tmp0 &= mFanins[mNfi - i]->get_gfunc();
	}
	for (size_t i = 0; i < mNfi; ++ i) {
	  iobs[i]->calc_iobs(obs & tmp1[i] & tmp2[i]);
	}
      }
      break;
    }
    break;

  case kTgOr:
  case kTgNor:
    switch ( mNfi ) {
    case 2:
      iobs[0]->calc_iobs(obs & ~mFanins[1]->get_gfunc());
      iobs[1]->calc_iobs(obs & ~mFanins[0]->get_gfunc());
      break;
      
    case 3:
      {
	Bdd v0 = ~mFanins[0]->get_gfunc();
	Bdd v1 = ~mFanins[1]->get_gfunc();
	Bdd v2 = ~mFanins[2]->get_gfunc();
	iobs[0]->calc_iobs(obs & v1 & v2);
	iobs[1]->calc_iobs(obs & v0 & v2);
	iobs[2]->calc_iobs(obs & v0 & v1);
      }
      break;
      
    case 4:
      {
	Bdd v0 = ~mFanins[0]->get_gfunc();
	Bdd v1 = ~mFanins[1]->get_gfunc();
	Bdd v2 = ~mFanins[2]->get_gfunc();
	Bdd v3 = ~mFanins[3]->get_gfunc();
	Bdd l1 = v0;
	Bdd l2 = l1 & v1;
	Bdd l3 = l2 & v2;
	Bdd u2 = v3;
	Bdd u1 = u2 & v2;
	Bdd u0 = u1 & v1;
	iobs[0]->calc_iobs(obs & u0);
	iobs[1]->calc_iobs(obs & u1 & l1);
	iobs[2]->calc_iobs(obs & u2 & l2);
	iobs[3]->calc_iobs(obs & l3);
      }
      break;
      
    default:
      {
	// mTmp1[i] は i - 1 以下の位置のファンインによる obs マスク
	// mTmp2[i] は i + 1 以上の位置のファンインによる obs マスク
	vector<Bdd> tmp1(mNfi);
	vector<Bdd> tmp2(mNfi);
	BddMgrRef mgr = obs.mgr();
	Bdd tmp0 = mgr.make_one();
	for (size_t i = 0; i < mNfi; ++ i) {
	  tmp1[i] = tmp0;
	  tmp0 &= ~mFanins[i]->get_gfunc();
	}
	tmp0 = mgr.make_one();
	for (size_t i = 1; i <= mNfi; ++ i) {
	  tmp2[mNfi - i] = tmp0;
	  tmp0 &= ~mFanins[mNfi - i]->get_gfunc();
	}
	for (size_t i = 0; i < mNfi; ++ i) {
	  iobs[i]->calc_iobs(obs & tmp1[i] & tmp2[i]);
	}
      }
      break;
    }
    break;

  case kTgXor:
  case kTgXnor:
    for (size_t i = 0; i < mNfi; ++ i) {
      iobs[i] = obs;
    }
    break;

  default:
    {
      VarBddMap varmap;
      BddMgrRef mgr = obs.mgr();
      for (size_t i = 0; i < mNfi; ++ i) {
	varmap.insert(make_pair(i, mFanins[i]->get_gfunc()));
      }
      for (size_t i = 0; i < mNfi; ++ i) {
	varmap.insert(make_pair(i, ~mFanins[i]->get_gfunc()));
	Bdd tmp = mgr.expr_to_bdd(mLexp, varmap);
	iobs[i]->calc_iobs(obs & (tmp ^ mGfunc));
	varmap.insert(make_pair(i, mFanins[i]->get_gfunc()));
      }
    }
    break;
    }
#endif
}

END_NAMESPACE_YM_SEAL_BDDSIM
