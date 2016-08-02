
/// @file calc_svf/SnGate.cc
/// @brief SnGate の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: SnGate.cc 1920 2008-12-20 15:52:42Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.

#if HAVE_CONFIG_H
#include "seal_config.h"
#endif


#include "SnGate.h"


BEGIN_NAMESPACE_YM_SEAL_SVF

//////////////////////////////////////////////////////////////////////
// @class SnGate SimNode.h
// @brief 多入力ゲートの基底クラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnGate::SnGate(ymuint32 id,
	       const vector<SimNode*>& inputs) :
  SimNode(id),
  mNfi(inputs.size()),
  mFanins(new SimNode*[mNfi]),
  mTmp(new tPackedVal[mNfi])
{
  size_t max_level = 0;
  for (size_t i = 0; i < mNfi; ++ i) {
    SimNode* input = inputs[i];
    assert_cond(input, __FILE__, __LINE__);
    mFanins[i] = input;
    size_t level = input->level() + 1;
    if ( max_level < level ) {
      max_level = level;
    }
  }
  set_level(max_level);
}

// @brief デストラクタ
SnGate::~SnGate()
{
  delete [] mFanins;
  delete [] mTmp;
}

// @brief ファンイン数を得る．
size_t
SnGate::nfi() const
{
  return mNfi;
}

// @brief pos 番めのファンインを得る．
SimNode*
SnGate::fanin(size_t pos) const
{
  return mFanins[pos];
}


//////////////////////////////////////////////////////////////////////
// @class SnGate1 SimNode.h
// @brief 1入力ゲートの基底クラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnGate1::SnGate1(ymuint32 id,
		 const vector<SimNode*>& inputs) :
  SimNode(id)
{
  assert_cond(inputs.size() == 1, __FILE__, __LINE__);
  assert_cond(inputs[0], __FILE__, __LINE__);
  mFanin = inputs[0];
  set_level(mFanin->level() + 1);
}

// @brief デストラクタ
SnGate1::~SnGate1()
{
}

// @brief ファンイン数を得る．
size_t
SnGate1::nfi() const
{
  return 1;
}

// @brief pos 番めのファンインを得る．
SimNode*
SnGate1::fanin(size_t pos) const
{
  return mFanin;
}


//////////////////////////////////////////////////////////////////////
// @class SnGate2 SimNode.h
// @brief 2入力ゲートの基底クラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnGate2::SnGate2(ymuint32 id,
		 const vector<SimNode*>& inputs) :
  SimNode(id)
{
  assert_cond(inputs.size() == 2, __FILE__, __LINE__);
  assert_cond(inputs[0], __FILE__, __LINE__);
  assert_cond(inputs[1], __FILE__, __LINE__);
  mFanins[0] = inputs[0];
  mFanins[1] = inputs[1];
  size_t level = mFanins[0]->level();
  if ( level < mFanins[1]->level() ) {
    level = mFanins[1]->level();
  }
  set_level(level + 1);
}

// @brief デストラクタ
SnGate2::~SnGate2()
{
}

// @brief ファンイン数を得る．
size_t
SnGate2::nfi() const
{
  return 2;
}

// @brief pos 番めのファンインを得る．
SimNode*
SnGate2::fanin(size_t pos) const
{
  return mFanins[pos];
}


//////////////////////////////////////////////////////////////////////
// @class SnGate3 SimNode.h
// @brief 3入力ゲートの基底クラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnGate3::SnGate3(ymuint32 id,
		 const vector<SimNode*>& inputs) :
  SimNode(id)
{
  assert_cond(inputs.size() == 3, __FILE__, __LINE__);
  assert_cond(inputs[0], __FILE__, __LINE__);
  assert_cond(inputs[1], __FILE__, __LINE__);
  assert_cond(inputs[2], __FILE__, __LINE__);
  mFanins[0] = inputs[0];
  mFanins[1] = inputs[1];
  mFanins[2] = inputs[2];
  size_t level = mFanins[0]->level();
  if ( level < mFanins[1]->level() ) {
    level = mFanins[1]->level();
  }
  if ( level < mFanins[2]->level() ) {
    level = mFanins[2]->level();
  }
  set_level(level + 1);
}

// @brief デストラクタ
SnGate3::~SnGate3()
{
}

// @brief ファンイン数を得る．
size_t
SnGate3::nfi() const
{
  return 3;
}

// @brief pos 番めのファンインを得る．
SimNode*
SnGate3::fanin(size_t pos) const
{
  return mFanins[pos];
}


//////////////////////////////////////////////////////////////////////
// @class SnGate4 SimNode.h
// @brief 4入力ゲートの基底クラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnGate4::SnGate4(ymuint32 id,
		 const vector<SimNode*>& inputs) :
  SimNode(id)
{
  assert_cond(inputs.size() == 4, __FILE__, __LINE__);
  assert_cond(inputs[0], __FILE__, __LINE__);
  assert_cond(inputs[1], __FILE__, __LINE__);
  assert_cond(inputs[2], __FILE__, __LINE__);
  assert_cond(inputs[3], __FILE__, __LINE__);
  mFanins[0] = inputs[0];
  mFanins[1] = inputs[1];
  mFanins[2] = inputs[2];
  mFanins[3] = inputs[3];
  size_t level = mFanins[0]->level();
  if ( level < mFanins[1]->level() ) {
    level = mFanins[1]->level();
  }
  if ( level < mFanins[2]->level() ) {
    level = mFanins[2]->level();
  }
  if ( level < mFanins[3]->level() ) {
    level = mFanins[3]->level();
  }
  set_level(level + 1);
}

// @brief デストラクタ
SnGate4::~SnGate4()
{
}

// @brief ファンイン数を得る．
size_t
SnGate4::nfi() const
{
  return 4;
}

// @brief pos 番めのファンインを得る．
SimNode*
SnGate4::fanin(size_t pos) const
{
  return mFanins[pos];
}

END_NAMESPACE_YM_SEAL_SVF
