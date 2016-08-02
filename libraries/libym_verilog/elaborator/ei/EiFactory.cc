
/// @file libym_verilog/elb_impl/EiFactory.cc
/// @brief EiFactory の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiFactory.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiExpr.h"

#include "ym_verilog/pt/PtUdp.h"
#include "ym_verilog/pt/PtDecl.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス EiFactory
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] alloc メモリ確保用のアロケータ
EiFactory::EiFactory(AllocBase& alloc) :
  mAlloc(alloc)
{
}

// @brief デストラクタ
EiFactory::~EiFactory()
{
}

// @brief 内訳を表示する．
void
EiFactory::dump_prof(ostream& s)
{
}

// @brief ステートメントの配列を生成する．
// @param[in] stmt_num 要素数
ElbStmt**
EiFactory::new_StmtList(ymuint32 stmt_num)
{
  void* q = mAlloc.get_memory(sizeof(ElbStmt*) * stmt_num);
  ElbStmt** array = new (q) ElbStmt*[stmt_num];

  return array;
}

// @brief 式の配列を生成する．
// @param[in] elem_num 要素数
ElbExpr**
EiFactory::new_ExprList(ymuint32 elem_num)
{
  void* p = mAlloc.get_memory(sizeof(ElbExpr*) * elem_num);
  ElbExpr** expr_array = new (p) ElbExpr*[elem_num];

  return expr_array;
}

END_NAMESPACE_YM_VERILOG
