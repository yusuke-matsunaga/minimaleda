#ifndef LIBYM_MVN_MVNOT_H
#define LIBYM_MVN_MVNOT_H

/// @file libym_mvn/MvNot.h
/// @brief MvNot のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvUnaryOp.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvNot MvNot.h "MvNot.h"
/// @brief through (buffer) ノードを表すクラス
//////////////////////////////////////////////////////////////////////
class MvNot :
  public MvUnaryOp
{
  friend class MvMgr;

public:

  /// @brief ノードの種類を得る．
  virtual
  tType
  type() const;


private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] bit_width ビット幅
  MvNot(MvModule* module,
	ymuint bit_width);

  /// @brief デストラクタ
  ~MvNot();

};

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_MVNOT_H
