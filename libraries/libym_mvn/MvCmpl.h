#ifndef LIBYM_MVN_MVCMPL_H
#define LIBYM_MVN_MVCMPL_H

/// @file libym_mvn/MvCmpl.h
/// @brief MvCmpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvUnaryOp.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvCmpl MvCmpl.h "MvCmpl.h"
/// @brief complement(2の補数) ノードを表すクラス
//////////////////////////////////////////////////////////////////////
class MvCmpl :
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
  MvCmpl(MvModule* module,
	 ymuint bit_width);

  /// @brief デストラクタ
  ~MvCmpl();

};

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_MVCMPL_H
