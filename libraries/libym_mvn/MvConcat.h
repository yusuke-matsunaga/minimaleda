#ifndef LIBYM_MVN_MVCONCAT_H
#define LIBYM_MVN_MVCONCAT_H

/// @file libym_mvn/MvConcat.h
/// @brief MvConcat のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/MvNode.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvConcat MvConcat.h "MvConcat.h"
/// @brief 連結演算を表すノード
//////////////////////////////////////////////////////////////////////
class MvConcat :
  public MvNode
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
  /// @param[in] ibitwidth_array 入力のビット幅の配列
  MvConcat(MvModule* module,
	   const vector<ymuint>& ibitwidth_array);

  /// @brief デストラクタ
  ~MvConcat();

};

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_MVCONCAT_H
