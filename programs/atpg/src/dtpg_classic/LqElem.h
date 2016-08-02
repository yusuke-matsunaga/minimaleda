#ifndef DTPG_CLASSIC_LQELEM_H
#define DTPG_CLASSIC_LQELEM_H

/// @file src/dtpg_classic/LqElem.h
/// @brief LqElem のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: LqElem.h 2128 2009-03-12 07:25:33Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "dtpgcl_nsdef.h"


BEGIN_NAMESPACE_YM_ATPG_DTPGCL

class LvlQ;
class Gate;

//////////////////////////////////////////////////////////////////////
/// @class LqElem LqElem.h "LqElem.h"
/// @brief LvlQ の要素の基底クラス
//////////////////////////////////////////////////////////////////////
class LqElem
{
  friend class LvlQ;

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 次の要素を指すリンク
  Gate* mNext;

};

END_NAMESPACE_YM_ATPG_DTPGCL

#endif
