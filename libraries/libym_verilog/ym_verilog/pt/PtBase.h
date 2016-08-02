#ifndef YM_VERILOG_PT_PTBASE_H
#define YM_VERILOG_PT_PTBASE_H

/// @file ym_verilog/pt/PtBase.h
/// @brief Pt の基底クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: PtBase.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/verilog.h"
#include "ym_verilog/pt/PtP.h"
#include "ym_utils/FileRegion.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtBase PtBase.h <ym_verilog/pt/PtBase.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief 全ての Pt クラスの基底クラス
//////////////////////////////////////////////////////////////////////
class PtBase
{
public:

  /// @brief デストラクタ
  virtual
  ~PtBase() { }


public:
  //////////////////////////////////////////////////////////////////////
  // PtBase の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置の取得
  /// @return ファイル位置
  virtual
  FileRegion
  file_region() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class PtNamedBase PtBase.h <ym_verilog/pt/PtBase.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief 名前を持つ PtBase
//////////////////////////////////////////////////////////////////////
class PtNamedBase :
  public PtBase
{
public:

  /// @brief デストラクタ
  virtual
  ~PtNamedBase() { }


public:
  //////////////////////////////////////////////////////////////////////
  // PtNamedBase の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  /// @return 名前
  virtual
  const char*
  name() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_PT_PTBASE_H
