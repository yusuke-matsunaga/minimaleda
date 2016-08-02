#ifndef LIBYM_MVN_MVPARTSELECT_H
#define LIBYM_MVN_MVPARTSELECT_H

/// @file libym_mvn/MvPartSelect.h
/// @brief MvPartSelect のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvBinaryOp.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvPartSelect MvPartSelect.h "MvPartSelect.h"
/// @brief 可変 part-select 演算を表すノード
//////////////////////////////////////////////////////////////////////
class MvPartSelect :
  public MvBinaryOp
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
  /// @param[in] bit_width1 入力のビット幅
  /// @param[in] bit_width2 範囲指定入力(base位置)のビット幅
  /// @param[in] bit_width3 範囲のビット幅(出力のビット幅)
  MvPartSelect(MvModule* module,
	       ymuint bit_width1,
	       ymuint bit_width2,
	       ymuint bit_width3);

  /// @brief デストラクタ
  ~MvPartSelect();

};

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_MVPARTSELECT_H
