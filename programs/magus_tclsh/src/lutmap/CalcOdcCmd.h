#ifndef MAGUS_LUTMAP_CALCODCCMD_H
#define MAGUS_LUTMAP_CALCODCCMD_H

/// @file magus/lutmap/tcl_bindings/CalcOdcCmd.h
/// @brief CalcOdcCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CalcOdcCmd.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MagBNetCmd.h"
#include "CalcOdc.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class CalcOdcCmd CalcOdcCmd.h "CalcOdcCmd.h"
/// @brief ODC の計算を行うテストコマンドのクラス
//////////////////////////////////////////////////////////////////////
class CalcOdcCmd :
  public MagBNetCmd
{
public:
  
  /// @brief コンストラクタ
  CalcOdcCmd(MagMgr* mgr);
  
  /// @brief デストラクタ
  virtual
  ~CalcOdcCmd();


protected:
  
  /// @brief コマンドを実行する仮想関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノードごとの ODC を計算するクラス
  CalcOdc mCaclOdc;
  
};

END_NAMESPACE_MAGUS

#endif // MAGUS_LUTMAP_CALCODCCMD_H
