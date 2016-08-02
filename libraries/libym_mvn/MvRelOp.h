#ifndef LIBYM_MVN_MVRELOP_H
#define LIBYM_MVN_MVRELOP_H

/// @file libym_mvn/MvRelOp.h
/// @brief MvRelOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvBinaryOp.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvEq MvRelOp.h "MvRelOp.h"
/// @brief equal 演算を表すノード
//////////////////////////////////////////////////////////////////////
class MvEq :
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
  /// @param[in] bit_width ビット幅
  MvEq(MvModule* module,
       ymuint bit_width);

  /// @brief デストラクタ
  ~MvEq();

};


//////////////////////////////////////////////////////////////////////
/// @class MvLt MvRelOp.h "MvRelOp.h"
/// @brief less than 比較演算を表すノード
//////////////////////////////////////////////////////////////////////
class MvLt :
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
  /// @param[in] bit_width ビット幅
  MvLt(MvModule* module,
       ymuint bit_width);

  /// @brief デストラクタ
  ~MvLt();

};

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_MVRELOP_H
