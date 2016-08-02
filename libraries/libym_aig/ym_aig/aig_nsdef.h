#ifndef YM_AIG_AIG_NSDEF_H
#define YM_AIG_AIG_NSDEF_H

/// @file ym_aig/aig_nsdef.h
/// @brief AIG パッケージ用の名前空間の定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: aig_nsdef.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"


/// @brief ym_aig 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_AIG \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsAig)

/// @brief ym_aig 用の名前空間の終了
#define END_NAMESPACE_YM_AIG \
END_NAMESPACE(nsAig) \
END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_AIG

class AigMgr;
class AigHandle;
class AigNode;

class FraigMgr;
class FraigHandle;

END_NAMESPACE_YM_AIG


BEGIN_NAMESPACE_YM

using nsAig::AigMgr;
using nsAig::AigHandle;
using nsAig::AigNode;

using nsAig::FraigMgr;
using nsAig::FraigHandle;

END_NAMESPACE_YM

#endif // YM_AIG_AIG_NSDEF_H
