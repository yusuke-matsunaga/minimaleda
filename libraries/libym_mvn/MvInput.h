#ifndef LIBYM_MVN_MVINPUT_H
#define LIBYM_MVN_MVINPUT_H

/// @file libym_mvn/MvInput.h
/// @brief MvInput のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvUnaryOp.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvInput MvInput.h "MvInput.h"
/// @brief 入力ノードを表すクラス
//////////////////////////////////////////////////////////////////////
class MvInput :
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
  MvInput(MvModule* module,
	  ymuint bit_width);

  /// @brief デストラクタ
  ~MvInput();

};

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_MVINPUT_H
