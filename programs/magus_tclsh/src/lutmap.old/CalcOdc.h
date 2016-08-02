#ifndef MAGUS_LUTMAP_CALCODC_H
#define MAGUS_LUTMAP_CALCODC_H

/// @file magus/src/lutmap/CalcOdc.h
/// @brief CalcOdc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CalcOdc.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include "magus_config.h"
#endif

#include "lutmap.h"
#include <ym_lexp/LogExpr.h>
#include <ym_bdd/Bdd.h>
#include <ym_utils/Alloc.h>


BEGIN_NAMESPACE_MAGUS_LUTMAP

//////////////////////////////////////////////////////////////////////
/// @class CalcOdc CalcOdc.h "CalcOdc.h"
/// @brief 論理式の葉の ODC を計算するクラス
///
/// 実はクラスにするほどのことはないが，内部で new/delete を
/// 繰り返すので効率化のために専用のメモリアロケータを持つ．
//////////////////////////////////////////////////////////////////////
class CalcOdc
{
public:

  /// @brief コンストラクタ
  CalcOdc();

  /// @brief デストラクタ
  ~CalcOdc();
  

public:

  /// @brief 論理式の葉の ODC を計算する．
  /// @param[in] expr 論理式
  /// @param[in] root_odc 根の部分の ODC
  /// @param[in] leaf_gf 葉の global function の配列
  /// @param[out] leaf_odc 葉の ODC
  void
  operator()(const LogExpr& expr,
	     Bdd root_odc,
	     const vector<Bdd>& leaf_gf,
	     vector<Bdd>& leaf_odc);

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 内部のデータ構造用のメモリアロケータ
  UnitAlloc mAlloc;
  
};

END_NAMESPACE_MAGUS_LUTMAP

#endif // MAGUS_LUSTMAP_CALCODC_H
