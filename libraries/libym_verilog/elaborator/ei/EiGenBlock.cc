
/// @file libym_verilog/elb_impl/EiGenBlock.cc
/// @brief EiGenBlock の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiGenBlock.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiGenBlock.h"
#include "ym_verilog/pt/PtItem.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////
  
// @brief generate block を生成する．
// @param[in] parent 親のスコープ環境
// @param[in] pt_item 対応するパース木の要素
ElbScope*
EiFactory::new_GenBlock(const VlNamedObj* parent,
			const PtItem* pt_item)
{
  void* p = mAlloc.get_memory(sizeof(EiGenBlock));
  EiGenBlock* scope = new (p) EiGenBlock(parent, pt_item);

  return scope;
}

// @brief GfBlock 検索用の親の名前付きオブジェクトを作る．
ElbGfRoot*
EiFactory::new_GfRoot(const VlNamedObj* parent,
		      const PtItem* pt_item)
{
  void* p = mAlloc.get_memory(sizeof(EiGfRoot));
  EiGfRoot* gfroot = new (p) EiGfRoot(parent, pt_item);

  return gfroot;
}
  
// @brief generate for block を生成する．
// @param[in] parent 親のスコープ環境
// @param[in] pt_item 対応するパース木の要素
// @param[in] int gvi 対応する genvar の値
ElbScope*
EiFactory::new_GfBlock(const VlNamedObj* parent,
		       const PtItem* pt_item,
		       int gvi)
{
  void* p = mAlloc.get_memory(sizeof(EiGfBlock));
  EiGfBlock* scope = new (p) EiGfBlock(parent, pt_item, gvi);

  return scope;
}


//////////////////////////////////////////////////////////////////////
// クラス EiGenBlock
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ環境
// @param[in] pt_item 対応するパース木の要素
EiGenBlock::EiGenBlock(const VlNamedObj* parent,
		       const PtItem* pt_item) :
  mParent(parent),
  mPtItem(pt_item)
{
}

// @brief デストラクタ
EiGenBlock::~EiGenBlock()
{
}

// @brief 型の取得
tVpiObjType
EiGenBlock::type() const
{
  return kVpiScope;
}

// @brief ファイル位置の取得
FileRegion
EiGenBlock::file_region() const
{
  return mPtItem->file_region();
}

// @brief このオブジェクトの属しているスコープを返す．
const VlNamedObj*
EiGenBlock::parent() const
{
  return mParent;
}
  
// @brief 名前の取得
const char*
EiGenBlock::name() const
{
  return mPtItem->name();
}


//////////////////////////////////////////////////////////////////////
// クラス EiGfRoot
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ環境
// @param[in] pt_item 対応するパース木の要素
EiGfRoot::EiGfRoot(const VlNamedObj* parent,
		   const PtItem* pt_item) :
  mParent(parent),
  mPtItem(pt_item)
{
}

// @brief デストラクタ
EiGfRoot::~EiGfRoot()
{
}

// @brief 型の取得
tVpiObjType
EiGfRoot::type() const
{
  return kVpiScope;
}

// @brief ファイル位置の取得
FileRegion
EiGfRoot::file_region() const
{
  return mPtItem->file_region();
}

// @brief このオブジェクトの属しているスコープを返す．
const VlNamedObj*
EiGfRoot::parent() const
{
  return mParent;
}
  
// @brief 名前の取得
const char*
EiGfRoot::name() const
{
  return mPtItem->name();
}

// @brief 子供のスコープを追加する．
void
EiGfRoot::add(int index,
	      const VlNamedObj* block)
{
  mTable.insert(make_pair(index, block));
}


// @brief 子供のスコープを取り出す．
const VlNamedObj*
EiGfRoot::elem_by_index(int index)
{
  hash_map<int, const VlNamedObj*>::iterator p = mTable.find(index);
  if ( p != mTable.end() ) {
    return p->second;
  }
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス EiGfBlock
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ環境
// @param[in] pt_item 対応するパース木の要素
// @param[in] index 対応する genvar の値
EiGfBlock::EiGfBlock(const VlNamedObj* parent,
		     const PtItem* pt_item,
		     int index) :
  EiGenBlock(parent, pt_item)
{
  ostringstream buf;
  buf << pt_item->name() << "[" << index << "]";
  
  mName = buf.str();
}

// @brief デストラクタ
EiGfBlock::~EiGfBlock()
{
}
  
// @brief 名前の取得
const char*
EiGfBlock::name() const
{
  return mName.c_str();
}

END_NAMESPACE_YM_VERILOG
