#ifndef YM_BNET_BNETVERILOGWRITER_H
#define YM_BNET_BNETVERILOGWRITER_H

/// @file ym_bnet/BNetVerilogWriter.h
/// @brief BNetVerilogWriter のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BNetVerilogWriter.h 1920 2008-12-20 15:52:42Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_bnet/bnet_nsdef.h"


BEGIN_NAMESPACE_YM_BNET

//////////////////////////////////////////////////////////////////////
/// @class BNetVerilogWriter BNetVerilogWriter.h <ym_bnet/BNetVerilogWriter.h>
/// @ingroup BnetGroup
/// @brief BNetwork の内容を verilog 形式で出力するクラス
/// @sa BNetwork
//////////////////////////////////////////////////////////////////////
class BNetVerilogWriter
{
public:

  /// @brief コンストラクタ
  BNetVerilogWriter();

  /// @brief デストラクタ
  ~BNetVerilogWriter();


public:

  /// @brief 出力する．
  void
  dump(ostream& s,
       const BNetwork& network) const;

};

END_NAMESPACE_YM_BNET

#endif // YM_BNET_BNETVERILOGWRITER_H
