#ifndef LIBYM_MVN_MVITE_H
#define LIBYM_MVN_MVITE_H

/// @file libym_mvn/MvIte.h
/// @brief MvIte のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvTernaryOp.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvIte MvIte.h "MvIte.h"
/// @brief ITE 演算を表すノード
//////////////////////////////////////////////////////////////////////
class MvIte :
  public MvTernaryOp
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
  MvIte(MvModule* module,
	ymuint bit_width);

  /// @brief デストラクタ
  ~MvIte();

};

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_MVITE_H
