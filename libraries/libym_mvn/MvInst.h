#ifndef LIBYM_MVN_MVINST_H
#define LIBYM_MVN_MVINST_H

/// @file libym_mvn/MvInst.h
/// @brief MvInst のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/MvNode.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvInst MvInst.h "MvInst.h"
/// @brief モジュールインスタンスを表すノード
//////////////////////////////////////////////////////////////////////
class MvInst :
  public MvNode
{
  friend class MvMgr;
public:

  /// @brief ノードの種類を得る．
  virtual
  tType
  type() const;

  /// @brief 下位モジュールを得る．
  /// @note type() が kInst の時のみ意味を持つ．
  virtual
  const MvModule*
  module() const;


private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] submodule 子のモジュール
  MvInst(MvModule* module,
	 MvModule* submodule);

  /// @brief デストラクタ
  ~MvInst();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 子のモジュール
  MvModule* mSubmodule;
  
};

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_MVINST_H
