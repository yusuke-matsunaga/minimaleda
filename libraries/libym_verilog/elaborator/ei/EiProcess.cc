
/// @file libym_verilog/elb_impl/EiProcess.cc
/// @brief EiProcess の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiProcess.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiProcess.h"
#include "ElbStmt.h"

#include "ym_verilog/pt/PtItem.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////
  
// @brief プロセス文を生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_item パース木の要素定義
ElbProcess*
EiFactory::new_Process(const VlNamedObj* parent,
		       const PtItem* pt_item)
{
  void* p = mAlloc.get_memory(sizeof(EiProcess));
  EiProcess* process = new (p) EiProcess(parent, pt_item);

  return process;
}


//////////////////////////////////////////////////////////////////////
// クラス EiProcess
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] pt_item パース木の要素定義
EiProcess::EiProcess(const VlNamedObj* parent,
		     const PtItem* pt_item) :
  mParent(parent),
  mPtItem(pt_item)
{
}

// @brief デストラクタ
EiProcess::~EiProcess()
{
}

// @brief クラスの型の取得
tVpiObjType
EiProcess::type() const
{
  switch ( mPtItem->type() ) {
  case kPtItem_Initial: return kVpiInitial;
  case kPtItem_Always:  return kVpiAlways;
  default: break;
  }
  assert_not_reached(__FILE__, __LINE__);
  return kVpiInitial;
}

// @brief ファイル位置を返す．
FileRegion
EiProcess::file_region() const
{
  return mPtItem->file_region();
}

// @brief 親のスコープを返す．
const VlNamedObj*
EiProcess::parent() const
{
  return mParent;
}

// @brief 本体のステートメントの取得
const VlStmt*
EiProcess::stmt() const
{
  return mStmt;
}

// @brief 本体のステートメントをセットする．
// @param[in] stmt 本体のステートメント
void
EiProcess::set_stmt(ElbStmt* stmt)
{
  mStmt = stmt;
}


END_NAMESPACE_YM_VERILOG
