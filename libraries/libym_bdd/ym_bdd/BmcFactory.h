#ifndef YM_BDD_BMCFACTORY_H
#define YM_BDD_BMCFACTORY_H

/// @file ym_bdd/BmcFactory.h
/// @brief BmcFactory のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BmcFactory.h 497 2007-03-26 15:22:14Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#include "ym_bdd/Bdd.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
/// @class BmcFactory BmcFactory.h <ym_bdd/BmcFactory.h>
/// @brief BddMgrClassic を生成するクラス
//////////////////////////////////////////////////////////////////////
class BmcFactory :
  public BddMgrFactory
{
public:

  /// @brief コンストラクタ
  /// @param[in] name マネージャに付ける名前（オプショナル）
  explicit
  BmcFactory(const string& name = string());

  /// @brief デストラクタ
  virtual
  ~BmcFactory();

  /// @brief BddMgrClassic を生成する．
  virtual
  BddMgr*
  operator()() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  string mName;
  
};

END_NAMESPACE_YM_BDD

#endif // YM_BDD_BMCFACTORY_H
