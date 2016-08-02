
/// @file calc_svf/SnOr.cc
/// @brief SnOr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: SnOr.cc 1920 2008-12-20 15:52:42Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.

#if HAVE_CONFIG_H
#include "seal_config.h"
#endif


#include "SnOr.h"


BEGIN_NAMESPACE_YM_SEAL_SVF

//////////////////////////////////////////////////////////////////////
// SnOr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnOr::SnOr(ymuint32 id,
	   const vector<SimNode*>& inputs) :
  SnGate(id, inputs)
{
}

// @brief デストラクタ
SnOr::~SnOr()
{
}

// @brief 正常値の計算を行う．
tPackedVal
SnOr::_calc_gval()
{
  size_t n = mNfi;
  tPackedVal new_val = mFanins[0]->get_gval();
  for (size_t i = 1; i < n; ++ i) {
    new_val |= mFanins[i]->get_gval();
  }
  return new_val;
}

// @brief 故障値の計算を行う．
tPackedVal
SnOr::_calc_fval()
{
  size_t n = mNfi;
  tPackedVal new_val = mFanins[0]->get_fval();
  for (size_t i = 1; i < n; ++ i) {
    new_val |= mFanins[i]->get_fval();
  }
  return new_val;
}

// @brief 入力の擬似最小 obs を計算する．
void
SnOr::calc_pseudo_min_iobs()
{
  size_t ni = mNfi;
  tPackedVal tmp0 = kPvAll1;
  for (size_t i = 1; i <= ni; ++ i) {
    size_t j = ni - i;
    mTmp[j] = tmp0;
    tmp0 &= ~mFanins[j]->get_gval();
  }
  tmp0 = get_obs();
  for (size_t i = 0; i < ni; ++ i) {
    SimNode* inode = mFanins[i];
    inode->or_obs(tmp0 & mTmp[i]);
    tmp0 &= ~inode->get_gval();
  }
}

// @brief 入力の最大 obs を計算する．
void
SnOr::calc_max_iobs()
{
  size_t ni = mNfi;
  tPackedVal tmp0 = kPvAll1;
  for (size_t i = 1; i <= ni; ++ i) {
    size_t j = ni - i;
    mTmp[j] = tmp0;
    tmp0 &= ~mFanins[j]->get_gval();
  }
  tmp0 = get_obs();
  for (size_t i = 0; i < ni; ++ i) {
    SimNode* inode = mFanins[i];
    tPackedVal val1 = inode->get_gval();
    inode->or_obs(tmp0 & (mTmp[i] | val1));
    tmp0 &= ~val1;
  }
}

// @brief 入力の obs を計算する．
void
SnOr::_calc_iobs(tPackedVal obs)
{
  size_t ni = mNfi;
  tPackedVal tmp0 = kPvAll1;
  for (size_t i = 1; i <= ni; ++ i) {
    size_t j = ni - i;
    mTmp[j] = tmp0;
    tmp0 &= ~mFanins[j]->get_gval();
  }
  tmp0 = obs;
  for (size_t i = 0; i < ni; ++ i) {
    SimNode* inode = mFanins[i];
    inode->calc_iobs(tmp0 & mTmp[i]);
    tmp0 &= ~inode->get_gval();
  }
}

// @brief 内容をダンプする．
void
SnOr::dump(ostream& s) const
{
  size_t n = mNfi;
  s << "OR(" << mFanins[0]->id();
  for (size_t i = 1; i < n; ++ i) {
    s << ", " << mFanins[i]->id();
  }
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnOr2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnOr2::SnOr2(ymuint32 id,
	     const vector<SimNode*>& inputs) :
  SnGate2(id, inputs)
{
}

// @brief デストラクタ
SnOr2::~SnOr2()
{
}

// @brief 正常値の計算を行う．
tPackedVal
SnOr2::_calc_gval()
{
  tPackedVal pat0 = mFanins[0]->get_gval();
  tPackedVal pat1 = mFanins[1]->get_gval();
  return pat0 | pat1;
}

// @brief 故障値の計算を行う．
tPackedVal
SnOr2::_calc_fval()
{
  tPackedVal pat0 = mFanins[0]->get_fval();
  tPackedVal pat1 = mFanins[1]->get_fval();
  return pat0 | pat1;
}

// @brief 入力の擬似最小 obs を計算する．
void
SnOr2::calc_pseudo_min_iobs()
{
  tPackedVal obs = get_obs();
  mFanins[0]->or_obs(obs & ~mFanins[1]->get_gval());
  mFanins[1]->or_obs(obs & ~mFanins[0]->get_gval());
}

// @brief 入力の最大 obs を計算する．
void
SnOr2::calc_max_iobs()
{
  tPackedVal obs = get_obs();
  tPackedVal v0 = ~mFanins[0]->get_gval();
  tPackedVal v1 = ~mFanins[1]->get_gval();
  mFanins[0]->or_obs(obs & (v1 | ~v0));
  mFanins[1]->or_obs(obs & v0);
}

// @brief 入力の obs を計算する．
void
SnOr2::_calc_iobs(tPackedVal obs)
{
  mFanins[0]->calc_iobs(obs & ~mFanins[1]->get_gval());
  mFanins[1]->calc_iobs(obs & ~mFanins[0]->get_gval());
}

// @brief 内容をダンプする．
void
SnOr2::dump(ostream& s) const
{
  s << "OR2(" << mFanins[0]->id();
  s << ", " << mFanins[1]->id();
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnOr3
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnOr3::SnOr3(ymuint32 id,
	     const vector<SimNode*>& inputs) :
  SnGate3(id, inputs)
{
}

// @brief デストラクタ
SnOr3::~SnOr3()
{
}

// @brief 正常値の計算を行う．
tPackedVal
SnOr3::_calc_gval()
{
  tPackedVal pat0 = mFanins[0]->get_gval();
  tPackedVal pat1 = mFanins[1]->get_gval();
  tPackedVal pat2 = mFanins[2]->get_gval();
  return pat0 | pat1 | pat2;
}

// @brief 故障値の計算を行う．
tPackedVal
SnOr3::_calc_fval()
{
  tPackedVal pat0 = mFanins[0]->get_fval();
  tPackedVal pat1 = mFanins[1]->get_fval();
  tPackedVal pat2 = mFanins[2]->get_fval();
  return pat0 | pat1 | pat2;
}

// @brief 入力の擬似最小 obs を計算する．
void
SnOr3::calc_pseudo_min_iobs()
{
  tPackedVal obs = get_obs();
  tPackedVal v0 = ~mFanins[0]->get_gval() & obs;
  tPackedVal v1 = ~mFanins[1]->get_gval() & obs;
  tPackedVal v2 = ~mFanins[2]->get_gval();
  mFanins[0]->or_obs(v1 & v2);
  mFanins[1]->or_obs(v0 & v2);
  mFanins[2]->or_obs(v0 & v1);
}

// @brief 入力の最大 obs を計算する．
void
SnOr3::calc_max_iobs()
{
  tPackedVal obs = get_obs();
  tPackedVal v0 = ~mFanins[0]->get_gval();
  tPackedVal v1 = ~mFanins[1]->get_gval();
  tPackedVal v2 = ~mFanins[2]->get_gval();
  tPackedVal l0 = obs;
  tPackedVal l1 = l0 & v0;
  tPackedVal l2 = l1 & v1;
  tPackedVal u1 = v2;
  tPackedVal u0 = u1 & v1;
  mFanins[0]->or_obs(l0 & (u0 | ~v0));
  mFanins[1]->or_obs(l1 & (u1 | ~v1));
  mFanins[2]->or_obs(l2);
}

// @brief 入力の obs を計算する．
void
SnOr3::_calc_iobs(tPackedVal obs)
{
  tPackedVal v0 = ~mFanins[0]->get_gval() & obs;
  tPackedVal v1 = ~mFanins[1]->get_gval() & obs;
  tPackedVal v2 = ~mFanins[2]->get_gval();
  mFanins[0]->calc_iobs(v1 & v2);
  mFanins[1]->calc_iobs(v0 & v2);
  mFanins[2]->calc_iobs(v0 & v1);
}

// @brief 内容をダンプする．
void
SnOr3::dump(ostream& s) const
{
  s << "OR3(" << mFanins[0]->id();
  s << ", " << mFanins[1]->id();
  s << ", " << mFanins[2]->id();
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnOr4
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnOr4::SnOr4(ymuint32 id,
	     const vector<SimNode*>& inputs) :
  SnGate4(id, inputs)
{
}

// @brief デストラクタ
SnOr4::~SnOr4()
{
}

// @brief 正常値の計算を行う．
tPackedVal
SnOr4::_calc_gval()
{
  tPackedVal pat0 = mFanins[0]->get_gval();
  tPackedVal pat1 = mFanins[1]->get_gval();
  tPackedVal pat2 = mFanins[2]->get_gval();
  tPackedVal pat3 = mFanins[3]->get_gval();
  return pat0 | pat1 | pat2 | pat3;
}

// @brief 故障値の計算を行う．
tPackedVal
SnOr4::_calc_fval()
{
  tPackedVal pat0 = mFanins[0]->get_fval();
  tPackedVal pat1 = mFanins[1]->get_fval();
  tPackedVal pat2 = mFanins[2]->get_fval();
  tPackedVal pat3 = mFanins[3]->get_fval();
  return pat0 | pat1 | pat2 | pat3;
}

// @brief 入力の擬似最小 obs を計算する．
void
SnOr4::calc_pseudo_min_iobs()
{
  tPackedVal obs = get_obs();
  tPackedVal v0 = ~mFanins[0]->get_gval();
  tPackedVal v1 = ~mFanins[1]->get_gval();
  tPackedVal v2 = ~mFanins[2]->get_gval();
  tPackedVal v3 = ~mFanins[3]->get_gval();
  tPackedVal l0 = obs;
  tPackedVal l1 = l0 & v0;
  tPackedVal l2 = l1 & v1;
  tPackedVal l3 = l2 & v2;
  tPackedVal u2 = v3;
  tPackedVal u1 = u2 & v2;
  tPackedVal u0 = u1 & v1;
  mFanins[0]->or_obs(l0 & u0);
  mFanins[1]->or_obs(l1 & u1);
  mFanins[2]->or_obs(l2 & u2);
  mFanins[3]->or_obs(l3);
}

// @brief 入力の最大 obs を計算する．
void
SnOr4::calc_max_iobs()
{
  tPackedVal obs = get_obs();
  tPackedVal v0 = ~mFanins[0]->get_gval();
  tPackedVal v1 = ~mFanins[1]->get_gval();
  tPackedVal v2 = ~mFanins[2]->get_gval();
  tPackedVal v3 = ~mFanins[3]->get_gval();
  tPackedVal l0 = obs;
  tPackedVal l1 = l0 & v0;
  tPackedVal l2 = l1 & v1;
  tPackedVal l3 = l2 & v2;
  tPackedVal u2 = v3;
  tPackedVal u1 = u2 & v2;
  tPackedVal u0 = u1 & v1;
  mFanins[0]->or_obs(l0 & (u0 | ~v0));
  mFanins[1]->or_obs(l1 & (u1 | ~v1));
  mFanins[2]->or_obs(l2 & (u2 | ~v2));
  mFanins[3]->or_obs(l3);
}

// @brief 入力の obs を計算する．
void
SnOr4::_calc_iobs(tPackedVal obs)
{
  tPackedVal v0 = ~mFanins[0]->get_gval();
  tPackedVal v1 = ~mFanins[1]->get_gval();
  tPackedVal v2 = ~mFanins[2]->get_gval();
  tPackedVal v3 = ~mFanins[3]->get_gval();
  tPackedVal l1 = obs & v0;
  tPackedVal l2 = l1 & v1;
  tPackedVal l3 = l2 & v2;
  tPackedVal u2 = obs & v3;
  tPackedVal u1 = u2 & v2;
  tPackedVal u0 = u1 & v1;
  mFanins[0]->calc_iobs(u0);
  mFanins[1]->calc_iobs(u1 & l1);
  mFanins[2]->calc_iobs(u2 & l2);
  mFanins[3]->calc_iobs(l3);
}

// @brief 内容をダンプする．
void
SnOr4::dump(ostream& s) const
{
  s << "OR4(" << mFanins[0]->id();
  s << ", " << mFanins[1]->id();
  s << ", " << mFanins[2]->id();
  s << ", " << mFanins[3]->id();
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnNor
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnNor::SnNor(ymuint32 id,
	     const vector<SimNode*>& inputs) :
  SnOr(id, inputs)
{
}

// @brief デストラクタ
SnNor::~SnNor()
{
}

// @brief 正常値の計算を行う．
tPackedVal
SnNor::_calc_gval()
{
  size_t n = mNfi;
  tPackedVal new_val = mFanins[0]->get_gval();
  for (size_t i = 1; i < n; ++ i) {
    new_val |= mFanins[i]->get_gval();
  }
  return~new_val;
}

// @brief 故障値の計算を行う．
tPackedVal
SnNor::_calc_fval()
{
  size_t n = mNfi;
  tPackedVal new_val = mFanins[0]->get_fval();
  for (size_t i = 1; i < n; ++ i) {
    new_val |= mFanins[i]->get_fval();
  }
  return ~new_val;
}

// @brief 内容をダンプする．
void
SnNor::dump(ostream& s) const
{
  size_t n = mNfi;
  s << "NOR(" << mFanins[0]->id();
  for (size_t i = 1; i < n; ++ i) {
    s << ", " << mFanins[i]->id();
  }
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnNor2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnNor2::SnNor2(ymuint32 id,
	       const vector<SimNode*>& inputs) :
  SnOr2(id, inputs)
{
}

// @brief デストラクタ
SnNor2::~SnNor2()
{
}

// @brief 正常値の計算を行う．
tPackedVal
SnNor2::_calc_gval()
{
  tPackedVal pat0 = mFanins[0]->get_gval();
  tPackedVal pat1 = mFanins[1]->get_gval();
  return ~(pat0 | pat1);
}

// @brief 故障値の計算を行う．
tPackedVal
SnNor2::_calc_fval()
{
  tPackedVal pat0 = mFanins[0]->get_fval();
  tPackedVal pat1 = mFanins[1]->get_fval();
  return ~(pat0 | pat1);
}

// @brief 内容をダンプする．
void
SnNor2::dump(ostream& s) const
{
  s << "NOR2(" << mFanins[0]->id();
  s << ", " << mFanins[1]->id();
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnNor3
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnNor3::SnNor3(ymuint32 id,
	       const vector<SimNode*>& inputs) :
  SnOr3(id, inputs)
{
}

// @brief デストラクタ
SnNor3::~SnNor3()
{
}

// @brief 正常値の計算を行う．
tPackedVal
SnNor3::_calc_gval()
{
  tPackedVal pat0 = mFanins[0]->get_gval();
  tPackedVal pat1 = mFanins[1]->get_gval();
  tPackedVal pat2 = mFanins[2]->get_gval();
  return ~(pat0 | pat1 | pat2);
}

// @brief 故障値の計算を行う．
tPackedVal
SnNor3::_calc_fval()
{
  tPackedVal pat0 = mFanins[0]->get_fval();
  tPackedVal pat1 = mFanins[1]->get_fval();
  tPackedVal pat2 = mFanins[2]->get_fval();
  return ~(pat0 | pat1 | pat2);
}

// @brief 内容をダンプする．
void
SnNor3::dump(ostream& s) const
{
  s << "NOR3(" << mFanins[0]->id();
  s << ", " << mFanins[1]->id();
  s << ", " << mFanins[2]->id();
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnNor4
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnNor4::SnNor4(ymuint32 id,
	       const vector<SimNode*>& inputs) :
  SnOr4(id, inputs)
{
}

// @brief デストラクタ
SnNor4::~SnNor4()
{
}

// @brief 正常値の計算を行う．
tPackedVal
SnNor4::_calc_gval()
{
  tPackedVal pat0 = mFanins[0]->get_gval();
  tPackedVal pat1 = mFanins[1]->get_gval();
  tPackedVal pat2 = mFanins[2]->get_gval();
  tPackedVal pat3 = mFanins[3]->get_gval();
  return ~(pat0 | pat1 | pat2 | pat3);
}

// @brief 故障値の計算を行う．
tPackedVal
SnNor4::_calc_fval()
{
  tPackedVal pat0 = mFanins[0]->get_fval();
  tPackedVal pat1 = mFanins[1]->get_fval();
  tPackedVal pat2 = mFanins[2]->get_fval();
  tPackedVal pat3 = mFanins[3]->get_fval();
  return ~(pat0 | pat1 | pat2 | pat3);
}

// @brief 内容をダンプする．
void
SnNor4::dump(ostream& s) const
{
  s << "NOR4(" << mFanins[0]->id();
  s << ", " << mFanins[1]->id();
  s << ", " << mFanins[2]->id();
  s << ", " << mFanins[3]->id();
  s << ")" << endl;
}

END_NAMESPACE_YM_SEAL_SVF
