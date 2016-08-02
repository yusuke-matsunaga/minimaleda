#ifndef LIBYM_MVN_MVUNARYOP_H
#define LIBYM_MVN_MVUNARYOP_H

/// @file libym_mvn/MvUnaryOp.h
/// @brief MvUnaryOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/MvNode.h"
#include "ym_mvn/MvPin.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvUnaryOp MvUnaryOp.h "MvUnaryOp.h"
/// @brief 1入力1出力ノードの基底クラス
//////////////////////////////////////////////////////////////////////
class MvUnaryOp :
  public MvNode
{
protected:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] bit_width1 入力のビット幅
  /// @param[in] bit_width2 出力のビット幅
  MvUnaryOp(MvModule* module,
	    ymuint bit_width1,
	    ymuint bit_width2);

  /// @brief デストラクタ
  ~MvUnaryOp();

};

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_MVUNARYOP_H
