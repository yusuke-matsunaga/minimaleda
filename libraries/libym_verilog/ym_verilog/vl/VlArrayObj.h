#ifndef YM_VERILOG_VL_VLARRAYOBJ_H
#define YM_VERILOG_VL_VLARRAYOBJ_H

/// @file ym_verilog/vl/VlArrayObj.h
/// @brief VlArrayObj のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlArrayObj.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/vl/VlNamedObj.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VlArrayObj VlArrayObj.h <ym_verilog/vl/VlArrayObj.h>
/// @brief 配列型オブジェクトの基底クラス
//////////////////////////////////////////////////////////////////////
class VlArrayObj :
  public VlNamedObj
{
protected:

  /// @brief デストラクタ
  virtual
  ~VlArrayObj() { }


public:
  //////////////////////////////////////////////////////////////////////
  // VlArrayObj の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 配列型オブジェクトの場合の次元数の取得
  /// @note このクラスでは 0 を返す．
  virtual
  ymuint32
  dimension_list_size() const = 0;

  /// @brief 自身が配列型の時の要素の探索
  /// @param[in] index_list インデックスのリスト
  virtual
  const VlDecl*
  array_elem(const vector<int>& index_list) = 0;

  /// @brief 自身が配列型の時の要素の探索
  /// @param[in] index_list インデックスのリスト
  virtual
  const VlDecl*
  array_elem(const vector<VlExpr*>& index_list) = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_VL_VLARRAYOBJ_H
