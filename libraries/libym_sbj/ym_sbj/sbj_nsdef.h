#ifndef YM_SBJ_SBJ_NSDEF_H
#define YM_SBJ_SBJ_NSDEF_H

/// @file ym_sbj/sbj_nsdef.h
/// @brief sbj パッケージに共通な定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: lutmap.h 555 2007-04-08 07:20:30Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"
#include "ym_utils/DlList.h"


/// @brief sbj 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_SBJ \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsSbj)

/// @brief sbj 用の名前空間の終了
#define END_NAMESPACE_YM_SBJ \
END_NAMESPACE(nsSbj) \
END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_SBJ

class SbjGraph;
class SbjPort;
class SbjEdge;
class SbjNode;
class SbjDumper;

typedef DlList<SbjEdge> SbjEdgeList;
typedef DlList<SbjNode> SbjNodeList;

END_NAMESPACE_YM_SBJ

BEGIN_NAMESPACE_YM

using nsSbj::SbjGraph;
using nsSbj::SbjPort;
using nsSbj::SbjEdge;
using nsSbj::SbjNode;
using nsSbj::SbjDumper;

using nsSbj::SbjEdgeList;
using nsSbj::SbjNodeList;

END_NAMESPACE_YM

#endif // YM_SBJ_SBJ_NSDEF_H
