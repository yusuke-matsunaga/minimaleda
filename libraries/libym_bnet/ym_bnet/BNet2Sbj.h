#ifndef YM_BNET_BNET2SBJ_H
#define YM_BNET_BNET2SBJ_H

/// @file ym_bnet/BNet2Sbj.h
/// @brief BNet2Sbj のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SbjGraph.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_bnet/bnet_nsdef.h"
#include "ym_sbj/sbj_nsdef.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class BNet2Sbj BNet2Sbj.h "ym_lutmap/BNet2Sbj.h"
/// @brief BNetwork を SbjGraph へ変換するクラス
//////////////////////////////////////////////////////////////////////
class BNet2Sbj
{
public:

  /// @brief BNetwork を SbjGraph へ変換する
  /// @param[in] network 変換元のネットワーク
  /// @param[out] sbjgraph 変換先のネットワーク
  /// @param[in] err_out エラーメッセージの出力先
  /// @note network はすでに 2-feasible ネットワークに分解されているとする．
  /// @return 2-feasible でない場合には false を返す．
  bool
  operator()(const BNetwork& network,
	     SbjGraph& sbjgraph,
	     ostream& err_out);

};

END_NAMESPACE_YM

#endif // YM_BNET_BNET2SBJ_H
