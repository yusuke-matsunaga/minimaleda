#ifndef LIBYM_MVN_MVINOUT_H
#define LIBYM_MVN_MVINOUT_H

/// @file libym_mvn/MvInout.h
/// @brief MvInout のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvUnaryOp.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvInout MvInout.h "MvInout.h"
/// @brief 入力ノードを表すクラス
//////////////////////////////////////////////////////////////////////
class MvInout :
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
  MvInout(MvModule* module,
	  ymuint bit_width);

  /// @brief デストラクタ
  ~MvInout();

};

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_MVINOUT_H
