#ifndef LIBYM_VERILOG_SIMULATOR_SIMSTMTGRAPH_H
#define LIBYM_VERILOG_SIMULATOR_SIMSTMTGRAPH_H

// $Id: SimStmtGraph.h 232 2006-09-19 08:45:43Z matsunaga $

// 一つの statement に対応したコントロールフローグラフ

#include <ym_verilog/verilog.h>


BEGIN_NAMESPACE_YM_VERILOG

class SimNode;

//////////////////////////////////////////////////////////////////////
// VpiStmt に対応するグラフ
//////////////////////////////////////////////////////////////////////
class SimStmtGraph
{
public:

  // コンストラクタ
  SimStmtGraph(SimNode* start,
	       SimNode* end);

  // デストラクタ
  ~SimStmtGraph();

  // 開始点を得る．
  SimNode* start() const;

  // 終了点を得る．
  SimNode* end() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 開始点
  SimNode* mStart;

  // 終了点
  SimNode* mEnd;
  
};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_SIMULATOR_SIMSTMTGRAPH_H
