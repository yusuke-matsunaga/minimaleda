#ifndef YM_LUTMAP_LN2BNET_H
#define YM_LUTMAP_LN2BNET_H

/// @file ym_lutmap/Ln2BNet.h 
/// @brief Ln2BNet のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: LutNetwork.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_lutmap/lutmap_nsdef.h"
#include "ym_bnet/bnet_nsdef.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class Ln2BNet Ln2BNet.h "ym_lutmap/Ln2BNet.h"
/// @brief LnGraph を BNetwork に変換するクラス
//////////////////////////////////////////////////////////////////////
class Ln2BNet
{
public:

  /// @brief LnGraph を BNetwork に変換する．
  /// @param[in] src_network 変換元のネットワーク
  /// @param[out] dst_network 変換先のネットワーク
  void
  operator()(const LnGraph& src_network,
	     BNetwork& dst_network);
  
};

END_NAMESPACE_YM

#endif // YM_LUTMAP_LN2BNET_H
