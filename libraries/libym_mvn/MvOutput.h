#ifndef LIBYM_MVN_MVOUTPUT_H
#define LIBYM_MVN_MVOUTPUT_H

/// @file libym_mvn/MvOutput.h
/// @brief MvOutput のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvUnaryOp.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvOutput MvOutput.h "MvOutput.h"
/// @brief 出力ノードを表すクラス
//////////////////////////////////////////////////////////////////////
class MvOutput :
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
  MvOutput(MvModule* module,
	   ymuint bit_width);

  /// @brief デストラクタ
  ~MvOutput();

};

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_MVOUTPUT_H
