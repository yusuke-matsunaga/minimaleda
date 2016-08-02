
/// @file libym_verilog/elaborator/ei/EiGenvar.cc
/// @brief EiGenvar の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiGenvar.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiGenvar.h"
#include "ym_verilog/pt/PtDecl.h"
#include "ym_verilog/BitVector.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief genvar を生成する．
// @param[in] parent 親のスコープ環境
// @param[in] pt_item 対応するパース木の要素
// @param[in] val 初期値
ElbGenvar*
EiFactory::new_Genvar(const VlNamedObj* parent,
		      const PtDeclItem* pt_item,
		      int val)
{
  void* p = mAlloc.get_memory(sizeof(EiGenvar));
  EiGenvar* genvar = new (p) EiGenvar(parent, pt_item, val);

  return genvar;
}


//////////////////////////////////////////////////////////////////////
// genvar を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] pt_item パース木の定義要素
// @param[in] name 名前
// @param[in] val 初期値
EiGenvar::EiGenvar(const VlNamedObj* parent,
		   const PtDeclItem* pt_item,
		   int val) :
  mParent(parent),
  mPtItem(pt_item),
  mInUse(false),
  mValue(val)
{
}

// デストラクタ
EiGenvar::~EiGenvar()
{
}

// @brief 型の取得
tVpiObjType
EiGenvar::type() const
{
  return kVpiConstant;
}

// @brief ファイル位置の取得
FileRegion
EiGenvar::file_region() const
{
  return mPtItem->file_region();
}

// @brief このオブジェクトの属しているスコープを返す．
const VlNamedObj*
EiGenvar::parent() const
{
  return mParent;
}

// @brief 名前の取得
const char*
EiGenvar::name() const
{
  return mPtItem->name();
}

// 使用中の時 true を返す．
bool
EiGenvar::is_inuse() const
{
  return mInUse;
}

// 使用中にする．
void
EiGenvar::set_inuse()
{
  mInUse = true;
}

// 使用を終える．
void
EiGenvar::reset_inuse()
{
  mInUse = false;
}

// 現在の値を返す．
int
EiGenvar::value() const
{
  return mValue;
}

// 値を設定する．
void
EiGenvar::set_value(int value)
{
  mValue = value;
}

// @brief 元となったパース木の定義要素を返す．
const PtDeclItem*
EiGenvar::pt_item() const
{
  return mPtItem;
}

END_NAMESPACE_YM_VERILOG
