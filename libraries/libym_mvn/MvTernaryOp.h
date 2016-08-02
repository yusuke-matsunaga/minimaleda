#ifndef LIBYM_MVN_MVTERNARYOP_H
#define LIBYM_MVN_MVTERNARYOP_H

/// @file libym_mvn/MvTernaryOp.h
/// @brief MvTernaryOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/MvNode.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvTernaryOp MvTernaryOp.h "MvTernaryOp.h"
/// @brief 3項演算を表すノード
//////////////////////////////////////////////////////////////////////
class MvTernaryOp :
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
  /// @param[in] bit_width3 入力3のビット幅
  /// @param[in] bit_width4 出力のビット幅
  MvTernaryOp(MvModule* module,
	      ymuint bit_width1,
	      ymuint bit_width2,
	      ymuint bit_width3,
	      ymuint bit_width4);

  /// @brief デストラクタ
  ~MvTernaryOp();

};

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_MVTERNARYOP_H
