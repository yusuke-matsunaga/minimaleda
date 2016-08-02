#ifndef YM_BDD_BMMFACTORY_H
#define YM_BDD_BMMFACTORY_H

/// @file ym_bdd/BmmFactory.h
/// @brief BmmFactory のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BmmFactory.h 497 2007-03-26 15:22:14Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_bdd/Bdd.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
/// @class BmmFactory BmmFactory.h <ym_bdd/BmmFactory.h>
/// @brief BddMgrModern を生成するクラス
//////////////////////////////////////////////////////////////////////
class BmmFactory :
  public BddMgrFactory
{
public:

  /// @brief コンストラクタ
  /// @param[in] reorder reordering をするとき true にするフラグ
  /// @param[in] name マネージャに付ける名前（オプショナル）
  explicit
  BmmFactory(bool reorder,
	     const string& name = string());

  /// @brief デストラクタ
  virtual
  ~BmmFactory();

  /// @brief BddMgrModern を生成する．
  virtual
  BddMgr*
  operator()() const;
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 生成時に必要となるパラメータ
  bool mReorder;

  const string mName;

};

END_NAMESPACE_YM_BDD

#endif // YM_BDD_BMMFACTORY
