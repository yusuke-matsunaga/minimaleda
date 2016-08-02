#ifndef SEALMGR_H
#define SEALMGR_H

/// @file include/SealMgr.h
/// @brief SealMgr のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SealMgr.h 1920 2008-12-20 15:52:42Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.


#include "seal_nsdef.h"
#include <ym_tgnet/tgnet_nsdef.h>


BEGIN_NAMESPACE_YM_SEAL

//////////////////////////////////////////////////////////////////////
/// @class SealMgr SealMgr.h "SealMgr.h"
/// @brief SEAL に共通のデータを保持しておくためのクラス
//////////////////////////////////////////////////////////////////////
class SealMgr
{
public:

  /// @brief コンストラクタ
  SealMgr();

  /// @brief デストラクタ
  ~SealMgr();


public:
  
  /// @brief TgNetwork を取り出す．
  TgNetwork&
  _network();
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のネットワーク
  TgNetwork* mNetwork;
  
};

END_NAMESPACE_YM_SEAL

#endif // SEALMGR_H
