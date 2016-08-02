
/// @file libym_mvn/MvPort.cc
/// @brief MvPort の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/MvPort.h"
#include "ym_mvn/MvNode.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
// クラス MvPortRef
//////////////////////////////////////////////////////////////////////


// @brief この実体のビット幅を返す．
ymuint
MvPortRef::bit_width() const
{
  if ( has_bitselect() ) {
    return 1;
  }
  else if ( has_partselect() ) {
    return msb() - lsb() + 1;
  }
  else {
    const MvNode* n = node();
    if ( n->type() == MvNode::kInput ) {
      return n->output(0)->bit_width();
    }
    else if ( n->type() == MvNode::kOutput ) {
      return n->input(0)->bit_width();
    }
    else if ( n->type() == MvNode::kInout ) {
      return n->input(0)->bit_width();
    }
  }
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}


//////////////////////////////////////////////////////////////////////
// クラス MvPort
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
// @param[in] np portref 数
MvPort::MvPort(const char* name,
	       ymuint np) :
  mName(name),
  mPortRefNum(np),
  mPortRefArray(new MvPortRef[np])
{
}

// @brief デストラクタ
MvPort::~MvPort()
{
  delete [] mPortRefArray;
}

// @brief ビット幅を得る．
ymuint
MvPort::bit_width() const
{
  ymuint ans = 0;
  for (ymuint i = 0; i < mPortRefNum; ++ i) {
    MvPortRef& pr = mPortRefArray[i];
    ans += pr.bit_width();
  }
  return ans;
}

END_NAMESPACE_YM_MVN
