#ifndef LIBYM_MVN_MVTHROUGH_H
#define LIBYM_MVN_MVTHROUGH_H

/// @file libym_mvn/MvThrough.h
/// @brief MvThrough のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvUnaryOp.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvThrough MvThrough.h "MvThrough.h"
/// @brief through (buffer) ノードを表すクラス
//////////////////////////////////////////////////////////////////////
class MvThrough :
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
  MvThrough(MvModule* module,
	    ymuint bit_width);

  /// @brief デストラクタ
  ~MvThrough();

};

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_MVTHROUGH_H
