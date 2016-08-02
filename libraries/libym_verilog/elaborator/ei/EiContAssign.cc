
/// @file libym_verilog/elb_impl/EiContAssign.cc
/// @brief EiContAssign の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiContAssign.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiContAssign.h"
#include "ElbExpr.h"
#include "ElbDelay.h"

#include "ym_verilog/pt/PtItem.h"
#include "ym_verilog/pt/PtMisc.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief continuous assignment のヘッダを生成する．
// @param[in] module 親のモジュール
// @param[in] pt_head パース木のヘッダ定義
// @param[in] delay 遅延値
// @note 遅延を持たないときは NULL を与える．
ElbCaHead*
EiFactory::new_CaHead(const VlModule* module,
		      const PtItem* pt_head,
		      ElbDelay* delay)
{
  EiCaHead* head = NULL;
  if ( delay ) {
    void* p = mAlloc.get_memory(sizeof(EiCaHeadD));
    head = new (p) EiCaHeadD(module, pt_head, delay);
  }
  else {
    void* p = mAlloc.get_memory(sizeof(EiCaHead));
    head = new (p) EiCaHead(module, pt_head);
  }
  return head;
}

// @brief continuous assignment を生成する．
// @param[in] head ヘッダ
// @param[in] pt_obj 対応するパース木中の定義要素
// @param[in] lhs 左辺式
// @param[in] rhs 右辺式
ElbContAssign*
EiFactory::new_ContAssign(ElbCaHead* head,
			  const PtBase* pt_obj,
			  ElbExpr* lhs,
			  ElbExpr* rhs)
{
  void* p = mAlloc.get_memory(sizeof(EiContAssign1));
  EiContAssign* cont_assign = new (p) EiContAssign1(head, pt_obj, lhs, rhs);
  
  return cont_assign;
}

// @brief net 宣言中の continuous assignment を生成する．
// @param[in] module 親のモジュール
// @param[in] pt_obj パース木の定義要素
// @param[in] lhs 左辺式
// @param[in] rhs 右辺式
ElbContAssign*
EiFactory::new_ContAssign(const VlModule* module,
			  const PtBase* pt_obj,
			  ElbExpr* lhs,
			  ElbExpr* rhs)
{
  void* p = mAlloc.get_memory(sizeof(EiContAssign2));
  EiContAssign* cont_assign = new (p) EiContAssign2(module, pt_obj, lhs, rhs);

  return cont_assign;
}


//////////////////////////////////////////////////////////////////////
// クラス EiCaHead
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] pt_obj パース木のヘッダ定義
EiCaHead::EiCaHead(const VlModule* module,
		   const PtItem* pt_head) :
  mModule(module),
  mPtHead(pt_head)
{
}

// @brief デストラクタ
EiCaHead::~EiCaHead()
{
}

// @brief 親のスコープを返す．
const VlModule*
EiCaHead::module() const
{
  return mModule;
}
  
// @brief 0の強さを返す．
tVpiStrength
EiCaHead::drive0() const
{
  if ( mPtHead->strength() ) {
    return mPtHead->strength()->drive0();
  }
  return kVpiNoStrength;
}
  
// @brief 1の強さを返す．
tVpiStrength
EiCaHead::drive1() const
{
  if ( mPtHead->strength() ) {
    return mPtHead->strength()->drive0();
  }
  return kVpiNoStrength;
}
  
// @brief 遅延を表す式を返す．
// @note このクラスでは NULL を返す．
ElbDelay*
EiCaHead::delay() const
{
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス EiCaHeadD
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] pt_head パース木のヘッダ定義
// @param[in] delay 遅延値
EiCaHeadD::EiCaHeadD(const VlModule* module,
		     const PtItem* pt_head,
		     ElbDelay* delay) :
  EiCaHead(module, pt_head),
  mDelay(delay)
{
}

// @brief デストラクタ
EiCaHeadD::~EiCaHeadD()
{
}
  
// @brief 遅延を表す式を返す．
ElbDelay*
EiCaHeadD::delay() const
{
  return mDelay;
}


//////////////////////////////////////////////////////////////////////
// クラス EiContAssign
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_obj 対応するパース木中の定義要素
// @param[in] lhs 左辺式
// @param[in] rhs 右辺式
EiContAssign::EiContAssign(const PtBase* pt_obj,
			   ElbExpr* lhs,
			   ElbExpr* rhs) :
  mPtObj(pt_obj),
  mLhs(lhs),
  mRhs(rhs)
{
}

// @brief デストラクタ
EiContAssign::~EiContAssign()
{
}

// @brief 型の取得
tVpiObjType
EiContAssign::type() const
{
  return kVpiContAssign;
}

// @brief ファイル位置を返す．
FileRegion
EiContAssign::file_region() const
{
  return mPtObj->file_region();
}

// @brief ビット幅を返す．
ymuint32
EiContAssign::bit_size() const
{
  return mLhs->bit_size();
}

// @brief 左辺を返す．
const VlExpr*
EiContAssign::lhs() const
{
  return mLhs;
}
  
// @brief 右辺を返す．
const VlExpr*
EiContAssign::rhs() const
{
  return mRhs;
}


//////////////////////////////////////////////////////////////////////
// クラス EiContAssign1
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] head ヘッダ
// @param[in] pt_obj 対応するパース木中の定義要素
// @param[in] lhs 左辺式
// @param[in] rhs 右辺式
EiContAssign1::EiContAssign1(ElbCaHead* head,
			     const PtBase* pt_obj,
			     ElbExpr* lhs,
			     ElbExpr* rhs) :
  EiContAssign(pt_obj, lhs, rhs),
  mHead(head)
{
}

// @brief デストラクタ
EiContAssign1::~EiContAssign1()
{
}

// @brief 親のスコープを返す．
const VlModule*
EiContAssign1::module() const
{
  return mHead->module();
}
  
// @brief 0の強さを返す．
tVpiStrength
EiContAssign1::drive0() const
{
  return mHead->drive0();
}
  
// @brief 1の強さを返す．
tVpiStrength
EiContAssign1::drive1() const
{
  return mHead->drive1();
}

// @brief 遅延を表す式を返す．
const VlDelay*
EiContAssign1::delay() const
{
  return mHead->delay();
}

// @brief ネット宣言中の assignment の時に true を返す．
bool
EiContAssign1::has_net_decl_assign() const
{
  return false;
}


//////////////////////////////////////////////////////////////////////
// クラス EiContAssign2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] pt_obj パース木の定義要素
// @param[in] lhs 左辺式
// @param[in] rhs 右辺式
EiContAssign2::EiContAssign2(const VlModule* module,
			     const PtBase* pt_obj,
			     ElbExpr* lhs,
			     ElbExpr* rhs) :
  EiContAssign(pt_obj, lhs, rhs),
  mModule(module)
{
}

// @brief デストラクタ
EiContAssign2::~EiContAssign2()
{
}

// @brief 親のスコープを返す．
const VlModule*
EiContAssign2::module() const
{
  return mModule;
}
  
// @brief 0の強さを返す．
tVpiStrength
EiContAssign2::drive0() const
{
  return kVpiNoStrength;
}
  
// @brief 1の強さを返す．
tVpiStrength
EiContAssign2::drive1() const
{
  return kVpiNoStrength;
}

// @brief 遅延を表す式を返す．
const VlDelay*
EiContAssign2::delay() const
{
  return NULL;
}

// @brief ネット宣言中の assignment の時に true を返す．
bool
EiContAssign2::has_net_decl_assign() const
{
  return true;
}


END_NAMESPACE_YM_VERILOG
