
/// @file src/base/SaFault.cc
/// @brief SaFault の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: SaFault.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2007 Yusuke Matsunaga
/// All rights reserved.

#if HAVE_CONFIG_H
#include "atpg_config.h"
#endif


#include "SaFault.h"
#include <ym_tgnet/TgNetwork.h>
#include <ym_tgnet/TgNode.h>
#include <sstream>


BEGIN_NAMESPACE_YM_ATPG


//////////////////////////////////////////////////////////////////////
// クラス SaFault
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] node 対象のゲート
// @param[in] output 入力の故障のとき false, 出力の故障のとき true を与える．
// @param[in] pos 故障位置
// @param[in] val 故障値 0 か非0 で 0/1 を区別する．
SaFault::SaFault(const TgNode* node,
		 bool output,
		 ymuint pos,
		 int val) :
  mNode(node),
  mPosVal(0UL),
  mFinfo(NULL)
{
  if ( val ) {
    mPosVal |= 1U;
  }
  if ( output ) {
    mPosVal |= 2U;
  }
  mPosVal |= (pos << 3);
}

// @brief デストラクタ
SaFault::~SaFault()
{
}
  
// @brief 故障の入力側のゲートを返す．
const TgNode*
SaFault::source_node() const
{
  if ( is_output_fault() ) {
    return node();
  }
  else {
    return node()->fanin(pos());
  }
}

// @brief 故障の内容を表す文字列を返す．
string
SaFault::str() const
{
  ostringstream ans;
  ans << node()->name() << ":";
  if ( is_input_fault() ) {
    ans << "I" << pos();
  }
  else {
    ans << "O";
  }
  ans << ":";
  if ( val() ) {
    ans <<"SA1";
  }
  else {
    ans <<"SA0";
  }
  return ans.str();
}

// @brief ストリーム出力演算子
// @param[in] s 出力先のストリーム
// @param[in] f 故障
ostream&
operator<<(ostream& s,
	   const SaFault* f)
{
  return s << f->str();
}

END_NAMESPACE_YM_ATPG
