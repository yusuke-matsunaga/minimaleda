
/// @file libym_mvn/MvNode.cc
/// @brief MvNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/MvNode.h"


BEGIN_NAMESPACE_YM_MVN

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] ni 入力ピン数
// @param[in] no 出力ピン数
MvNode::MvNode(MvModule* parent,
	       ymuint ni,
	       ymuint no) :
  mParent(parent),
  mNi(ni),
  mInputPins(new MvInputPin[ni]),
  mNo(no),
  mOutputPins(new MvOutputPin[no])
{
  for (ymuint i = 0; i < ni; ++ i) {
    MvInputPin* pin = &mInputPins[i];
    pin->mNode = this;
    pin->mPos = i;
    pin->mBitWidth = 1;
  }
  for (ymuint i = 0; i < no; ++ i) {
    MvOutputPin* pin = &mOutputPins[i];
    pin->mNode = this;
    pin->mPos = i;
    pin->mBitWidth = 1;
  }
}

// @brief デストラクタ
MvNode::~MvNode()
{
  delete [] mInputPins;
  delete [] mOutputPins;
}

// @brief ビット位置を得る．
// @note type() が kConstBitSelect の時のみ意味を持つ．
// @note デフォルトの実装では 0 を返す．
ymuint
MvNode::bitpos() const
{
  return 0;
}

// @brief 範囲指定の MSB を得る．
// @note type() が kConstPartSelect の時のみ意味を持つ．
// @note デフォルトの実装では 0 を返す．
ymuint
MvNode::msb() const
{
  return 0;
}

// @brief 範囲指定の LSB を得る．
// @note type() が kConstPartSelect の時のみ意味を持つ．
// @note デフォルトの実装では 0 を返す．
ymuint
MvNode::lsb() const
{
  return 0;
}

// @brief 定数値を得る．
// @param[out] val 値を格納するベクタ
// @note type() が kConst の時のみ意味を持つ．
// @note デフォルトの実装ではなにもしない．
void
MvNode::const_value(vector<ymuint32>& val) const
{
}

// @brief 入力ピンのビット幅を設定する．
// @param[in] pos 対象のピン番号
// @param[in] bit_width ビット幅
void
MvNode::set_ipin_bit_width(ymuint pos,
			   ymuint bit_width)
{
  mInputPins[pos].mBitWidth = bit_width;
}

// @brief 出力ピンのビット幅を設定する．
// @param[in] pos 対象のピン番号
// @param[in] bit_width ビット幅
void
MvNode::set_opin_bit_width(ymuint pos,
			   ymuint bit_width)
{
  mOutputPins[pos].mBitWidth = bit_width;
}

END_NAMESPACE_YM_MVN
