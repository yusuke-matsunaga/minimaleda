#ifndef LIBYM_MVN_MVDFF_H
#define LIBYM_MVN_MVDFF_H

/// @file libym_mvn/MvDff.h
/// @brief MvDff のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/MvNode.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvDff MvDff.h "MvDff.h"
/// @brief D-FF を表すクラス
//////////////////////////////////////////////////////////////////////
class MvDff :
  public MvNode
{
protected:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] bit_width ビット幅
  MvDff(MvModule* module,
	ymuint bit_width);

  /// @brief デストラクタ
  ~MvDff();

};


//////////////////////////////////////////////////////////////////////
/// @class MvDff1 MvDff.h "MvDff.h"
/// @brief 非同期セット / リセットタイプの D-FF を表すクラス
//////////////////////////////////////////////////////////////////////
class MvDff1 :
  public MvDff
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
  MvDff1(MvModule* module,
	 ymuint bit_width);

  /// @brief デストラクタ
  ~MvDff1();

};


//////////////////////////////////////////////////////////////////////
/// @class MvDff2 MvDff.h "MvDff.h"
/// @brief 同期セット / リセットタイプの D-FF を表すクラス
//////////////////////////////////////////////////////////////////////
class MvDff2 :
  public MvDff
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
  MvDff2(MvModule* module,
	 ymuint bit_width);

  /// @brief デストラクタ
  ~MvDff2();

};

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_MVDFF_H
