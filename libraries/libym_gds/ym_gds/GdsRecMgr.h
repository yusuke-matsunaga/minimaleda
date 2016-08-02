#ifndef YM_GDS_GDSRECMGR_H
#define YM_GDS_GDSRECMGR_H

/// @file ym_gds/GdsRecMgr.h
/// @brief GdsRecMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: GdsRecMgr.h 1343 2008-03-25 17:15:35Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#include <ym_gds/gds_nsdef.h>
#include <ym_utils/Alloc.h>


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
/// @class GdsRecMgr GdsRecMgr.h "ym_gds/GdsRecMgr.h"
/// @brief GdsRecord の生成/破壊を管理するクラス
//////////////////////////////////////////////////////////////////////
class GdsRecMgr
{
public:

  /// @brief コンストラクタ
  GdsRecMgr();

  /// @brief デストラクタ
  ~GdsRecMgr();


public:

  /// @brief レコードの生成
  /// @param[in] dsize データサイズ
  GdsRecord*
  alloc_rec(size_t dsize);

  /// @brief レコードの破壊
  /// @param[in] rec 破壊するレコード
  void
  free_rec(GdsRecord* rec);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // レコード用のアロケータ
  FragAlloc mAlloc;

};

END_NAMESPACE_YM_GDS

#endif
