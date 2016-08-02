#ifndef LIBYM_MVN_MVROP_H
#define LIBYM_MVN_MVROP_H

/// @file libym_mvn/MvRop.h
/// @brief MvRop のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvUnaryOp.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvRand MvRop.h "MvRop.h"
//////////////////////////////////////////////////////////////////////
class MvRand :
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
  MvRand(MvModule* module,
	 ymuint bit_width);

  /// @brief デストラクタ
  ~MvRand();

};


//////////////////////////////////////////////////////////////////////
/// @class MvRor MvRop.h "MvRop.h"
//////////////////////////////////////////////////////////////////////
class MvRor :
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
  MvRor(MvModule* module,
	ymuint bit_width);

  /// @brief デストラクタ
  ~MvRor();

};


//////////////////////////////////////////////////////////////////////
/// @class MvRxor MvRop.h "MvRop.h"
//////////////////////////////////////////////////////////////////////
class MvRxor :
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
  MvRxor(MvModule* module,
	 ymuint bit_width);

  /// @brief デストラクタ
  ~MvRxor();

};

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_MVROP_H
