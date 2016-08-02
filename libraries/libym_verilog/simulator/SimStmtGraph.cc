// $Id: SimStmtGraph.cc 700 2007-05-31 00:41:30Z matsunaga $

// シミュレーション用のコントロールフローグラフ

#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include "SimStmtGraph.h"


BEGIN_NAMESPACE_YM_VERILOG
  
//////////////////////////////////////////////////////////////////////
// VpiStmt に対応するグラフ
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SimStmtGraph::SimStmtGraph(SimNode* start,
			   SimNode* end) :
  mStart(start),
  mEnd(end)
{
}

// デストラクタ
SimStmtGraph::~SimStmtGraph()
{
}

// 開始点を得る．
SimNode*
SimStmtGraph::start() const
{
  return mStart;
}

// 終了点を得る．
SimNode*
SimStmtGraph::end() const
{
  return mEnd;
}

END_NAMESPACE_YM_VERILOG
