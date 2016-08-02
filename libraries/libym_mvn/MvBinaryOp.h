#ifndef LIBYM_MVN_MVBINARYOP_H
#define LIBYM_MVN_MVBINARYOP_H

/// @file libym_mvn/MvBinaryOp.h
/// @brief MvBinaryOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/MvNode.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvBinaryOp MvBinaryOp.h "MvBinaryOp.h"
/// @brief 1入力1出力ノードの基底クラス
//////////////////////////////////////////////////////////////////////
class MvBinaryOp :
  public MvNode
{
protected:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] bit_width1 入力1のビット幅
  /// @param[in] bit_width2 入力2のビット幅
  /// @param[in] bit_width3 出力のビット幅
  MvBinaryOp(MvModule* module,
	     ymuint bit_width1,
	     ymuint bit_width2,
	     ymuint bit_width3);

  /// @brief デストラクタ
  ~MvBinaryOp();

};

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_MVBINARYOP_H
