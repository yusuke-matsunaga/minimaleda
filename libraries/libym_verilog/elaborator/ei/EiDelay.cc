
/// @file libym_verilog/elb_impl/EiDelay.cc
/// @brief EiDelay の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiDelay.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiDelay.h"
#include "ElbExpr.h"

#include "ym_verilog/pt/PtBase.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief 遅延値を生成する．
// @param[in] pt_obj パース木の定義要素
// @param[in] elem_num 要素数
// @param[in] expr_list 式の配列
ElbDelay*
EiFactory::new_Delay(const PtBase* pt_obj,
		     ymuint32 elem_num,
		     ElbExpr** expr_list)
{
  void* p = mAlloc.get_memory(sizeof(EiDelay));
  EiDelay* delay = new (p) EiDelay(pt_obj, elem_num, expr_list);

  return delay;
}


//////////////////////////////////////////////////////////////////////
// クラス EiDelay
//////////////////////////////////////////////////////////////////////
  
// @brief コンストラクタ
// @param[in] pt_obj パース木の定義要素
// @param[in] elem_num 要素数
// @param[in] expr_list 式の配列
EiDelay::EiDelay(const PtBase* pt_obj,
		 ymuint32 elem_num,
		 ElbExpr** expr_list) :
  mPtObj(pt_obj),
  mElemNum(elem_num),
  mElemArray(expr_list)
{
}
  
// @brief デストラクタ
EiDelay::~EiDelay()
{
}

// @brief 型の取得
tVpiObjType
EiDelay::type() const
{
  // 嘘
  return kVpiOperation;
}

// @brief ファイル位置を返す．
FileRegion
EiDelay::file_region() const
{
  return mPtObj->file_region();
}
  
// @brief 要素数を返す．
ymuint32
EiDelay::elem_num() const
{
  return mElemNum;
}

// @brief 値を返す．
// @param[in] pos 位置番号 ( 0 <= pos < elem_num() )
const VlExpr*
EiDelay::expr(ymuint32 pos) const
{
  return mElemArray[pos];
}
  
// @brief Verilog-HDL の文字列を得る．
string
EiDelay::decompile() const
{
  string ans = "(";
  string comma = "";
  for (ymuint32 i = 0; i < elem_num(); ++ i) {
    ans += comma;
    ans += expr(i)->decompile();
    comma = ", ";
  }
  ans += ")";
  return ans;
}

END_NAMESPACE_YM_VERILOG
