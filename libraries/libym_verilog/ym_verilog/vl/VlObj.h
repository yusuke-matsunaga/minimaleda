#ifndef YM_VERILOG_VL_VLDOBJ_H
#define YM_VERILOG_VL_VLDOBJ_H

/// @file ym_verilog/vl/VlObj.h
/// @brief VlObj のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlObj.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/verilog.h"
#include "ym_utils/FileRegion.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VlObj VlObj.h <ym_verilog/vl/VlObj.h>
/// @brief エラボレーション中のオブジェクトを表す基底クラス
//////////////////////////////////////////////////////////////////////
class VlObj
{
protected:

  /// @brief デストラクタ
  virtual
  ~VlObj() { }


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const = 0;

  /// @brief ファイル位置の取得
  virtual
  FileRegion
  file_region() const = 0;
  
};

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_VL_VLOBJ_H
