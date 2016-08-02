#ifndef YM_BNET_BNETEQUWRITER_H
#define YM_BNET_BNETEQUWRITER_H

/// @file ym_bnet/BNetEquWriter.h
/// @brief BNetEquWriter のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BNetEquWriter.h 1920 2008-12-20 15:52:42Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_bnet/bnet_nsdef.h"


BEGIN_NAMESPACE_YM_BNET

//////////////////////////////////////////////////////////////////////
/// @class BNetEquWriter BNetEquWriter.h <ym_bnet/BNetEquWriter.h>
/// @ingroup BnetGroup
/// @brief BNetwork の内容を equ 形式で出力するクラス
/// @sa BNetwork
//////////////////////////////////////////////////////////////////////
class BNetEquWriter
{
public:

  /// @brief コンストラクタ
  BNetEquWriter();

  /// @brief デストラクタ
  ~BNetEquWriter();


public:

  /// @brief 出力する．
  void
  dump(ostream& s,
       const BNetwork& network) const;

};

END_NAMESPACE_YM_BNET

#endif // YM_BNET_BNETEQUWRITER_H
