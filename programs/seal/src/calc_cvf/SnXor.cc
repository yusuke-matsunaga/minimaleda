
/// @file calc_cvf/SnXor.cc
/// @brief SnXor の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: SnXor.cc 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.

#if HAVE_CONFIG_H
#include "seal_config.h"
#endif


#include "SnXor.h"


BEGIN_NAMESPACE_YM_SEAL_CVF

//////////////////////////////////////////////////////////////////////
// SnXor
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnXor::SnXor(ymuint32 id,
	     const vector<SimNode*>& inputs) :
  SnGate(id, inputs)
{
}

// @brief デストラクタ
SnXor::~SnXor()
{
}

// @brief 正常値の計算を行う．
tPackedVal
SnXor::_calc_gval()
{
  size_t n = mNfi;
  tPackedVal new_val = mFanins[0]->get_gval();
  for (size_t i = 1; i < n; ++ i) {
    new_val ^= mFanins[i]->get_gval();
  }
  return new_val;
}

// @brief 故障値の計算を行う．
tPackedVal
SnXor::_calc_fval()
{
  size_t n = mNfi;
  tPackedVal new_val = mFanins[0]->get_fval();
  for (size_t i = 1; i < n; ++ i) {
    new_val ^= mFanins[i]->get_fval();
  }
  return new_val;
}

// @brief 入力の擬似最小 obs を計算する．
void
SnXor::calc_pseudo_min_iobs()
{
  tPackedVal obs = get_obs();
  for (size_t i = 0; i < mNfi; ++ i) {
    mFanins[i]->or_obs(obs);
  }
}

// @brief 入力の最大 obs を計算する．
void
SnXor::calc_max_iobs(RandGen& randgen)
{
  tPackedVal obs = get_obs2();
  for (size_t i = 0; i < mNfi; ++ i) {
    mFanins[i]->or_obs2(obs);
  }
}

// @brief 入力の obs を計算する．
void
SnXor::_calc_iobs(tPackedVal obs)
{
  for (size_t i = 0; i < mNfi; ++ i) {
    mFanins[i]->calc_iobs(obs);
  }
}

// @brief 正常値の計算を行う．
Bdd
SnXor::_calc_gfunc()
{
  size_t n = mNfi;
  Bdd new_func = mFanins[0]->get_gfunc();
  for (size_t i = 1; i < n; ++ i) {
    new_func ^= mFanins[i]->get_gfunc();
  }
  return new_func;
}

// @brief 故障値の計算を行う．
Bdd
SnXor::_calc_ffunc()
{
  size_t n = mNfi;
  Bdd new_func = mFanins[0]->get_ffunc();
  for (size_t i = 1; i < n; ++ i) {
    new_func ^= mFanins[i]->get_ffunc();
  }
  return new_func;
}

// @brief 入力の擬似最小 obs を計算する．
void
SnXor::calc_pseudo_min_iobsfunc()
{
  Bdd obs = get_obsfunc();
  for (size_t i = 0; i < mNfi; ++ i) {
    mFanins[i]->or_obsfunc(obs);
  }
}
  
// @brief 入力の最大 obs を計算する．
void
SnXor::calc_max_iobsfunc()
{
  Bdd obs = get_obsfunc();
  for (size_t i = 0; i < mNfi; ++ i) {
    mFanins[i]->or_obsfunc(obs);
  }
}
  
// @brief 入力の obs を計算する．
void
SnXor::_calc_iobsfunc(const Bdd& obs)
{
  for (size_t i = 0; i < mNfi; ++ i) {
    mFanins[i]->calc_iobsfunc(obs);
  }
}

// @brief 内容をダンプする．
void
SnXor::dump(ostream& s) const
{
  size_t n = mNfi;
  s << "XOR(" << mFanins[0]->id();
  for (size_t i = 1; i < n; ++ i) {
    s << ", " << mFanins[i]->id();
  }
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnXor2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnXor2::SnXor2(ymuint32 id,
	       const vector<SimNode*>& inputs) :
  SnGate2(id, inputs)
{
}

// @brief デストラクタ
SnXor2::~SnXor2()
{
}

// @brief 正常値の計算を行う．
tPackedVal
SnXor2::_calc_gval()
{
  tPackedVal pat0 = mFanins[0]->get_gval();
  tPackedVal pat1 = mFanins[1]->get_gval();
  return pat0 ^ pat1;
}

// @brief 故障値の計算を行う．
tPackedVal
SnXor2::_calc_fval()
{
  tPackedVal pat0 = mFanins[0]->get_fval();
  tPackedVal pat1 = mFanins[1]->get_fval();
  return pat0 ^ pat1;
}

// @brief 入力の擬似最小 obs を計算する．
void
SnXor2::calc_pseudo_min_iobs()
{
  tPackedVal obs = get_obs();
  mFanins[0]->or_obs(obs);
  mFanins[1]->or_obs(obs);
}

// @brief 入力の最大 obs を計算する．
void
SnXor2::calc_max_iobs(RandGen& randgen)
{
  tPackedVal obs = get_obs2();
  mFanins[0]->or_obs2(obs);
  mFanins[1]->or_obs2(obs);
}

// @brief 入力の obs を計算する．
void
SnXor2::_calc_iobs(tPackedVal obs)
{
  mFanins[0]->calc_iobs(obs);
  mFanins[1]->calc_iobs(obs);
}

// @brief 正常値の計算を行う．
Bdd
SnXor2::_calc_gfunc()
{
  Bdd pat0 = mFanins[0]->get_gfunc();
  Bdd pat1 = mFanins[1]->get_gfunc();
  return pat0 ^ pat1;
}

// @brief 故障値の計算を行う．
Bdd
SnXor2::_calc_ffunc()
{
  Bdd pat0 = mFanins[0]->get_ffunc();
  Bdd pat1 = mFanins[1]->get_ffunc();
  return pat0 ^ pat1;
}

// @brief 入力の擬似最小 obs を計算する．
void
SnXor2::calc_pseudo_min_iobsfunc()
{
  Bdd obs = get_obsfunc();
  mFanins[0]->or_obsfunc(obs);
  mFanins[1]->or_obsfunc(obs);
}
  
// @brief 入力の最大 obs を計算する．
void
SnXor2::calc_max_iobsfunc()
{
  Bdd obs = get_obsfunc();
  mFanins[0]->or_obsfunc(obs);
  mFanins[1]->or_obsfunc(obs);
}
  
// @brief 入力の obs を計算する．
void
SnXor2::_calc_iobsfunc(const Bdd& obs)
{
  mFanins[0]->calc_iobsfunc(obs);
  mFanins[1]->calc_iobsfunc(obs);
}

// @brief 内容をダンプする．
void
SnXor2::dump(ostream& s) const
{
  s << "XOR2(" << mFanins[0]->id();
  s << ", " << mFanins[1]->id();
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnXnor
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnXnor::SnXnor(ymuint32 id,
	       const vector<SimNode*>& inputs) :
  SnXor(id, inputs)
{
}

// @brief デストラクタ
SnXnor::~SnXnor()
{
}

// @brief 正常値の計算を行う．
tPackedVal
SnXnor::_calc_gval()
{
  size_t n = mNfi;
  tPackedVal new_val = mFanins[0]->get_gval();
  for (size_t i = 1; i < n; ++ i) {
    new_val ^= mFanins[i]->get_gval();
  }
  return ~new_val;
}

// @brief 故障値の計算を行う．
tPackedVal
SnXnor::_calc_fval()
{
  size_t n = mNfi;
  tPackedVal new_val = mFanins[0]->get_fval();
  for (size_t i = 1; i < n; ++ i) {
    new_val ^= mFanins[i]->get_fval();
  }
  return ~new_val;
}

// @brief 正常値の計算を行う．
Bdd
SnXnor::_calc_gfunc()
{
  size_t n = mNfi;
  Bdd new_func = mFanins[0]->get_gfunc();
  for (size_t i = 1; i < n; ++ i) {
    new_func ^= mFanins[i]->get_gfunc();
  }
  return ~new_func;
}

// @brief 故障値の計算を行う．
Bdd
SnXnor::_calc_ffunc()
{
  size_t n = mNfi;
  Bdd new_func = mFanins[0]->get_ffunc();
  for (size_t i = 1; i < n; ++ i) {
    new_func ^= mFanins[i]->get_ffunc();
  }
  return ~new_func;
}

// @brief 内容をダンプする．
void
SnXnor::dump(ostream& s) const
{
  size_t n = mNfi;
  s << "XNOR(" << mFanins[0]->id();
  for (size_t i = 1; i < n; ++ i) {
    s << ", " << mFanins[i]->id();
  }
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnXnor2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnXnor2::SnXnor2(ymuint32 id,
		 const vector<SimNode*>& inputs) :
  SnXor2(id, inputs)
{
}

// @brief デストラクタ
SnXnor2::~SnXnor2()
{
}

// @brief 正常値の計算を行う．
tPackedVal
SnXnor2::_calc_gval()
{
  tPackedVal pat0 = mFanins[0]->get_gval();
  tPackedVal pat1 = mFanins[1]->get_gval();
  return ~(pat0 ^ pat1);
}

// @brief 故障値の計算を行う．
tPackedVal
SnXnor2::_calc_fval()
{
  tPackedVal pat0 = mFanins[0]->get_fval();
  tPackedVal pat1 = mFanins[1]->get_fval();
  return ~(pat0 ^ pat1);
}

// @brief 正常値の計算を行う．
Bdd
SnXnor2::_calc_gfunc()
{
  Bdd pat0 = mFanins[0]->get_gfunc();
  Bdd pat1 = mFanins[1]->get_gfunc();
  return ~(pat0 ^ pat1);
}

// @brief 故障値の計算を行う．
Bdd
SnXnor2::_calc_ffunc()
{
  Bdd pat0 = mFanins[0]->get_ffunc();
  Bdd pat1 = mFanins[1]->get_ffunc();
  return ~(pat0 ^ pat1);
}

// @brief 内容をダンプする．
void
SnXnor2::dump(ostream& s) const
{
  s << "XNOR2(" << mFanins[0]->id();
  s << ", " << mFanins[1]->id();
  s << ")" << endl;
}

END_NAMESPACE_YM_SEAL_CVF
