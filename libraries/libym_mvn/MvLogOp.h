#ifndef LIBYM_MVN_MVLOGOP_H
#define LIBYM_MVN_MVLOGOP_H

/// @file libym_mvn/MvLogOp.h
/// @brief MvLogOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvBinaryOp.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvAnd MvLogOp.h "MvLogOp.h"
/// @brief AND 演算を表すノード
//////////////////////////////////////////////////////////////////////
class MvAnd :
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
  MvAnd(MvModule* module,
	ymuint bit_width);

  /// @brief デストラクタ
  ~MvAnd();

};


//////////////////////////////////////////////////////////////////////
/// @class MvOr MvLogOp.h "MvLogOp.h"
/// @brief OR 演算を表すノード
//////////////////////////////////////////////////////////////////////
class MvOr :
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
  MvOr(MvModule* module,
       ymuint bit_width);

  /// @brief デストラクタ
  ~MvOr();

};


//////////////////////////////////////////////////////////////////////
/// @class MvXor MvLogOp.h "MvLogOp.h"
/// @brief XOR 演算を表すノード
//////////////////////////////////////////////////////////////////////
class MvXor :
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
  MvXor(MvModule* module,
	ymuint bit_width);

  /// @brief デストラクタ
  ~MvXor();

};

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_MVLOGOP_H
