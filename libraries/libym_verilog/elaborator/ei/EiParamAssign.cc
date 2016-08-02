
/// @file libym_verilog/elb_impl/EiParamAssign.cc
/// @brief EiParamAssign の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiParamAssign.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiParamAssign.h"
#include "ElbParameter.h"
#include "ElbExpr.h"

#include "ym_verilog/pt/PtItem.h"
#include "ym_verilog/pt/PtMisc.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief パラメータ割り当て文を生成する．
// @param[in] module 親のモジュール
// @param[in] param 対象の parameter
// @param[in] rhs 割り当て式の右辺
// @param[in] named_con 名前による割り当ての時 true
ElbParamAssign*
EiFactory::new_ParamAssign(const VlModule* module,
			   const PtBase* pt_obj,
			   ElbDecl* param,
			   ElbExpr* rhs,
			   bool named_con)
{
  EiParamAssign* pa = NULL;
  if ( named_con ) {
    void* p = mAlloc.get_memory(sizeof(EiParamAssign));
    pa = new (p) EiParamAssign(module, pt_obj, param, rhs);
  }
  else {
    void* p = mAlloc.get_memory(sizeof(EiParamAssign2));
    pa = new (p) EiParamAssign2(module, pt_obj, param, rhs);
  }
  return pa;
}

// @brief defparam 文を生成する．
// @param[in] module 親のモジュール
// @param[in] pt_header パース木の defparam ヘッダ
// @param[in] pt_item パース木の defparam 定義
// @param[in] param 対象の parameter
// @param[in] rhs 割り当て式の右辺
ElbDefParam*
EiFactory::new_DefParam(const VlModule* module,
			const PtItem* pt_header,
			const PtDefParam* pt_defparam,
			ElbDecl* param,
			ElbExpr* rhs)
{
  void* p = mAlloc.get_memory(sizeof(EiDefParam));
  EiDefParam* defparam = new (p) EiDefParam(module, pt_header, pt_defparam,
					    param, rhs);

  return defparam;
}


//////////////////////////////////////////////////////////////////////
// クラス EiParamAssign
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のモジュール
// @param[in] pt_obj パース木中の対応する要素
// @param[in] param 対象の parameter
// @param[in] rhs 割り当て式の右辺
EiParamAssign::EiParamAssign(const VlModule* parent,
			     const PtBase* pt_obj,
			     ElbDecl* param,
			     ElbExpr* rhs) :
  mModule(parent),
  mPtObj(pt_obj),
  mLhs(param),
  mRhs(rhs)
{
}

// @brief デストラクタ
EiParamAssign::~EiParamAssign()
{
}

// @brief 型の取得
tVpiObjType
EiParamAssign::type() const
{
  return kVpiParamAssign;
}

// @brief ファイル位置を返す．
FileRegion
EiParamAssign::file_region() const
{
  return mPtObj->file_region();
}

// @brief 親のモジュールを返す．
const VlModule*
EiParamAssign::parent() const
{
  return mModule;
}

// @brief 左辺値を返す．
const VlDecl*
EiParamAssign::lhs() const
{
  return mLhs;
}

// @brief 右辺値を返す．
const VlExpr*
EiParamAssign::rhs() const
{
  return mRhs;
}

// @brief 名前による接続の場合に true を返す．
bool
EiParamAssign::is_conn_by_name() const
{
  return false;
}


//////////////////////////////////////////////////////////////////////
// クラス EiParamAssign2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のモジュール
// @param[in] pt_obj パース木中の対応する要素
// @param[in] param 対象の parameter
// @param[in] rhs 割り当て式の右辺
EiParamAssign2::EiParamAssign2(const VlModule* parent,
			       const PtBase* pt_obj,
			       ElbDecl* param,
			       ElbExpr* rhs) :
  EiParamAssign(parent, pt_obj, param, rhs)
{
}

// @brief デストラクタ
EiParamAssign2::~EiParamAssign2()
{
}

// @brief 名前による接続の場合に true を返す．
bool
EiParamAssign2::is_conn_by_name() const
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス EiDefParam
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のモジュール
// @param[in] pt_header パース木の defparam ヘッダ
// @param[in] pt_item パース木の defparam 定義
// @param[in] param 対象の parameter
// @param[in] rhs 割り当て式の右辺
EiDefParam::EiDefParam(const VlModule* parent,
		       const PtItem* pt_header,
		       const PtDefParam* pt_defparam,
		       ElbDecl* param,
		       ElbExpr* rhs) :
  mModule(parent),
  mPtHead(pt_header),
  mPtDefParam(pt_defparam),
  mLhs(param),
  mRhs(rhs)
{
}

// @brief デストラクタ
EiDefParam::~EiDefParam()
{
}

// @brief 型の取得
tVpiObjType
EiDefParam::type() const
{
  return kVpiDefParam;
}

// @brief ファイル位置を返す．
FileRegion
EiDefParam::file_region() const
{
  return mPtDefParam->file_region();
}

// @brief 親のモジュールを返す．
const VlModule*
EiDefParam::parent() const
{
  return mModule;
}

// @brief 左辺値を返す．
const VlDecl*
EiDefParam::lhs() const
{
  return mLhs;
}

// @brief 右辺値を返す．
const VlExpr*
EiDefParam::rhs() const
{
  return mRhs;
}


END_NAMESPACE_YM_VERILOG
