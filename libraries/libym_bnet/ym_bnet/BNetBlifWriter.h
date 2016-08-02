#ifndef YM_BNET_BNETBLIFWRITER_H
#define YM_BNET_BNETBLIFWRITER_H

/// @file ym_bnet/BNetBlifWriter.h
/// @brief BNetBlifWriter のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BNetBlifWriter.h 1920 2008-12-20 15:52:42Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_bnet/bnet_nsdef.h"


BEGIN_NAMESPACE_YM_BNET

//////////////////////////////////////////////////////////////////////
/// @class BNetBlifWriter BNetBlifWriter.h <ym_bnet/BNetBlifWriter.h>
/// @ingroup BnetGroup
/// @brief BNetwork の内容を blif 形式で出力するクラス
/// @sa BNetwork
//////////////////////////////////////////////////////////////////////
class BNetBlifWriter
{
public:

  /// @brief コンストラクタ
  BNetBlifWriter();

  /// @brief デストラクタ
  ~BNetBlifWriter();


public:

  /// @brief 出力する．
  void
  dump(ostream& s,
       const BNetwork& network) const;

};

END_NAMESPACE_YM_BNET

#endif // YM_BNET_BNETBLIFWRITER_H
