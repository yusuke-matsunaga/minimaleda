
/// @file libym_verilog/elb/ObjDict.cc
/// @brief ObjDict の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ObjDict.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ObjDict.h"

#include "ym_verilog/vl/VlNamedObj.h"

#include "ElbDecl.h"
#include "ElbModule.h"
#include "ElbTaskFunc.h"
#include "ElbPrimitive.h"
#include "ElbGfRoot.h"
#include "ElbGenvar.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス ElbObjHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbObjHandle::ElbObjHandle() :
  mLink(NULL)
{
}

// @brief デストラクタ
ElbObjHandle::~ElbObjHandle()
{
}

// @brief オブジェクトの型を返す．
tVpiObjType
ElbObjHandle::type()
{
  return obj()->type();
}

// @brief ファイル位置の取得
FileRegion
ElbObjHandle::file_region()
{
  return obj()->file_region();
}

// @brief オブジェクトの階層付き名前を返す．
string
ElbObjHandle::full_name()
{
  return obj()->full_name();
}

// @brief 配列要素の VlNamedObj を返す．
// @note このクラスでは NULL を返す．
const VlNamedObj*
ElbObjHandle::array_elem(int index)
{
  return NULL;
}

// @brief ElbDecl を返す．
// @note このクラスでは NULL を返す．
ElbDecl*
ElbObjHandle::decl()
{
  return NULL;
}

// @brief ElbDeclArray を返す．
// @note このクラスでは NULL を返す．
ElbDeclArray*
ElbObjHandle::decl_array()
{
  return NULL;
}

// @brief ElbTaskFunc を返す．
// @note このクラスでは NULL を返す．
ElbTaskFunc*
ElbObjHandle::taskfunc()
{
  return NULL;
}

// @brief ElbModuleArray を返す．
// @note このクラスでは NULL を返す．
ElbModuleArray*
ElbObjHandle::module_array()
{
  return NULL;
}

// @brief ElbPrimArray を返す．
// @note このクラスでは NULL を返す．
ElbPrimArray*
ElbObjHandle::prim_array()
{
  return NULL;
}

// @brief ElbPrimitive を返す．
// @note このクラスでは NULL を返す．
ElbPrimitive*
ElbObjHandle::primitive()
{
  return NULL;
}

// @brief ElbGfRoot を返す．
// @note このクラスでは NULL を返す．
ElbGfRoot*
ElbObjHandle::gfroot()
{
  return NULL;
}

// @brief ElbGenvar を返す．
// @note このクラスでは NULL を返す．
ElbGenvar*
ElbObjHandle::genvar()
{
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス ElbTaskFuncHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbTaskFuncHandle::ElbTaskFuncHandle(ElbTaskFunc* obj) :
  mObj(obj)
{
}

// @brief デストラクタ
ElbTaskFuncHandle::~ElbTaskFuncHandle()
{
}

// @brief VlNamedObj を返す．
const VlNamedObj*
ElbTaskFuncHandle::obj()
{
  return mObj;
}

// @brief ElbTaskFunc を返す．
ElbTaskFunc*
ElbTaskFuncHandle::taskfunc()
{
  return mObj;
}


//////////////////////////////////////////////////////////////////////
// クラス ElbDeclHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbDeclHandle::ElbDeclHandle(ElbDecl* obj) :
  mObj(obj)
{
}

// @brief デストラクタ
ElbDeclHandle::~ElbDeclHandle()
{
}

// @brief VlNamedObj を返す．
const VlNamedObj*
ElbDeclHandle::obj()
{
  return mObj;
}

// @brief ElbDecl を返す．
ElbDecl*
ElbDeclHandle::decl()
{
  return mObj;
}


//////////////////////////////////////////////////////////////////////
// クラス ElbDeclArrayHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbDeclArrayHandle::ElbDeclArrayHandle(ElbDeclArray* obj) :
  mObj(obj)
{
}

// @brief デストラクタ
ElbDeclArrayHandle::~ElbDeclArrayHandle()
{
}

// @brief VlNamedObj を返す．
const VlNamedObj*
ElbDeclArrayHandle::obj()
{
  return mObj;
}

// @brief ElbDeclArray を返す．
ElbDeclArray*
ElbDeclArrayHandle::decl_array()
{
  return mObj;
}


//////////////////////////////////////////////////////////////////////
// クラス ElbModuleArrayHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbModuleArrayHandle::ElbModuleArrayHandle(ElbModuleArray* obj) :
  mObj(obj)
{
}

// @brief デストラクタ
ElbModuleArrayHandle::~ElbModuleArrayHandle()
{
}

// @brief VlNamedObj を返す．
const VlNamedObj*
ElbModuleArrayHandle::obj()
{
  return mObj;
}

// @brief 配列要素の VlNamedObj を返す．
const VlNamedObj*
ElbModuleArrayHandle::array_elem(int index)
{
  return mObj->elem_by_index(index);
}

// @brief ElbModuleArray を返す．
ElbModuleArray*
ElbModuleArrayHandle::module_array()
{
  return mObj;
}


//////////////////////////////////////////////////////////////////////
// クラス ElbPrimArrayHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbPrimArrayHandle::ElbPrimArrayHandle(ElbPrimArray* obj) :
  mObj(obj)
{
}

// @brief デストラクタ
ElbPrimArrayHandle::~ElbPrimArrayHandle()
{
}

// @brief VlNamedObj を返す．
const VlNamedObj*
ElbPrimArrayHandle::obj()
{
  return mObj;
}

// @brief ElbPrimArray を返す．
ElbPrimArray*
ElbPrimArrayHandle::prim_array()
{
  return mObj;
}


//////////////////////////////////////////////////////////////////////
// クラス VlPrimitive
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbPrimitiveHandle::ElbPrimitiveHandle(ElbPrimitive* obj) :
  mObj(obj)
{
}

// @brief デストラクタ
ElbPrimitiveHandle::~ElbPrimitiveHandle()
{
}

// @brief VlNamedObj を返す．
const VlNamedObj*
ElbPrimitiveHandle::obj()
{
  return mObj;
}

// @brief ElbPrimitive を返す．
ElbPrimitive*
ElbPrimitiveHandle::primitive()
{
  return mObj;
}


//////////////////////////////////////////////////////////////////////
// クラス ElbScopeHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbScopeHandle::ElbScopeHandle(const VlNamedObj* obj) :
  mObj(obj)
{
}

// @brief デストラクタ
ElbScopeHandle::~ElbScopeHandle()
{
}

// @brief VlNamedObj を返す．
const VlNamedObj*
ElbScopeHandle::obj()
{
  return mObj;
}


//////////////////////////////////////////////////////////////////////
// クラス ElbGfRootHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbGfRootHandle::ElbGfRootHandle(ElbGfRoot* obj) :
  mObj(obj)
{
}

// @brief デストラクタ
ElbGfRootHandle::~ElbGfRootHandle()
{
}

// @brief VlNamedObj を返す．
const VlNamedObj*
ElbGfRootHandle::obj()
{
  return mObj;
}

// @brief 配列要素の VlNamedObj を返す．
const VlNamedObj*
ElbGfRootHandle::array_elem(int index)
{
  return mObj->elem_by_index(index);
}

// @brief ElbGfRoot を返す．
ElbGfRoot*
ElbGfRootHandle::gfroot()
{
  return mObj;
}


//////////////////////////////////////////////////////////////////////
// クラス ElbGenvarHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbGenvarHandle::ElbGenvarHandle(ElbGenvar* obj) :
  mObj(obj)
{
}

// @brief デストラクタ
ElbGenvarHandle::~ElbGenvarHandle()
{
}

// @brief VlNamedObj を返す．
const VlNamedObj*
ElbGenvarHandle::obj()
{
  return mObj;
}

// @brief ElbGenvar を返す．
ElbGenvar*
ElbGenvarHandle::genvar()
{
  return mObj;
}


//////////////////////////////////////////////////////////////////////
// VlNamedObj を格納するハッシュ表
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ObjDict::ObjDict(AllocBase& alloc) :
  mAlloc(sizeof(ElbScopeHandle), 1024),
  mNum(0)
{
  alloc_table(1024);
}

// @brief デストラクタ
ObjDict::~ObjDict()
{
  delete [] mTable;
}

// @brief 内容を空にする．
void
ObjDict::clear()
{
  for (ymuint32 i = 0; i < mSize; ++ i) {
    mTable[i] = NULL;
  }
  mNum = 0;
}

// @brief 要素を追加する．
void
ObjDict::add(const VlNamedObj* obj)
{
  void* p = mAlloc.get_memory(sizeof(ElbScopeHandle));
  ElbScopeHandle* handle = new (p) ElbScopeHandle(obj);
  add_handle(obj->parent(), obj->name(), handle);
}

// @brief 要素を追加する．
void
ObjDict::add(ElbTaskFunc* obj)
{
  void* p = mAlloc.get_memory(sizeof(ElbTaskFuncHandle));
  ElbTaskFuncHandle* handle = new (p) ElbTaskFuncHandle(obj);
  add_handle(obj->parent(), obj->name(), handle);
}

// @brief 要素を追加する．
void
ObjDict::add(ElbDecl* obj)
{
  void* p = mAlloc.get_memory(sizeof(ElbDeclHandle));
  ElbDeclHandle* handle = new (p) ElbDeclHandle(obj);
  add_handle(obj->parent(), obj->name(), handle);
}

// @brief 要素を追加する．
void
ObjDict::add(ElbDeclArray* obj)
{
  void* p = mAlloc.get_memory(sizeof(ElbDeclArrayHandle));
  ElbDeclArrayHandle* handle = new (p) ElbDeclArrayHandle(obj);
  add_handle(obj->parent(), obj->name(), handle);
}

// @brief 要素を追加する．
void
ObjDict::add(ElbModuleArray* obj)
{
  void* p = mAlloc.get_memory(sizeof(ElbModuleArrayHandle));
  ElbModuleArrayHandle* handle = new (p) ElbModuleArrayHandle(obj);
  add_handle(obj->parent(), obj->name(), handle);
}

// @brief 要素を追加する．
void
ObjDict::add(ElbPrimArray* obj)
{
  void* p = mAlloc.get_memory(sizeof(ElbPrimArrayHandle));
  ElbPrimArrayHandle* handle = new (p) ElbPrimArrayHandle(obj);
  add_handle(obj->parent(), obj->name(), handle);
}

// @brief 要素を追加する．
void
ObjDict::add(ElbPrimitive* obj)
{
  void* p = mAlloc.get_memory(sizeof(ElbPrimitiveHandle));
  ElbPrimitiveHandle* handle = new (p) ElbPrimitiveHandle(obj);
  add_handle(obj->parent(), obj->name(), handle);
}

// @brief 要素を追加する．
void
ObjDict::add(ElbGfRoot* obj)
{
  void* p = mAlloc.get_memory(sizeof(ElbGfRootHandle));
  ElbGfRootHandle* handle = new (p) ElbGfRootHandle(obj);
  add_handle(obj->parent(), obj->name(), handle);
}

// @brief 要素を追加する．
void
ObjDict::add(ElbGenvar* obj)
{
  void* p = mAlloc.get_memory(sizeof(ElbGenvarHandle));
  ElbGenvarHandle* handle = new (p) ElbGenvarHandle(obj);
  add_handle(obj->parent(), obj->name(), handle);
}

// @brief セルを追加する．
void
ObjDict::add_handle(const VlNamedObj* parent,
		    const char* name,
		    ElbObjHandle* handle)
{
  if ( mNum >= mLimit ) {
    // テーブルを拡張する．
    ymuint32 old_size = mSize;
    ElbObjHandle** old_table = mTable;
    alloc_table(old_size << 1);
    for (ymuint32 i = 0; i < old_size; ++ i) {
      for (ElbObjHandle* handle = old_table[i]; handle; ) {
	ElbObjHandle* next = handle->mLink;
	const VlNamedObj* obj = handle->obj();
	ymuint32 pos = hash_func(obj->parent(), obj->name());
	handle->mLink = mTable[pos];
	mTable[pos] = handle;
	handle = next;
      }
    }
    delete [] old_table;
  }
  ymuint32 pos = hash_func(parent, name);
  handle->mLink = mTable[pos];
  mTable[pos] = handle;
  ++ mNum;
}

// @brief 名前から該当する要素を検索する．
ElbObjHandle*
ObjDict::find(const VlNamedObj* parent,
	      const char* name) const
{
  ymuint32 pos = hash_func(parent, name);
  for (ElbObjHandle* handle = mTable[pos]; handle; handle = handle->mLink) {
    const VlNamedObj* obj = handle->obj();
    if ( obj->parent() == parent && strcmp(obj->name(), name) == 0 ) {
      return handle;
    }
  }
  return NULL;
}

// @brief このオブジェクトが使用しているメモリ量を返す．
size_t
ObjDict::allocated_size() const
{
  return sizeof(ElbObjHandle*) * mSize + mAlloc.allocated_size();
}

// @brief テーブルの領域を確保する．
void
ObjDict::alloc_table(ymuint32 size)
{
  mSize = size;
  mLimit = static_cast<ymuint32>(mSize * 1.8);
  mTable = new ElbObjHandle*[mSize];
  for (ymuint32 i = 0; i < mSize; ++ i) {
    mTable[i] = NULL;
  }
}

// @brief ハッシュ値を計算する．
ymuint32
ObjDict::hash_func(const VlNamedObj* parent,
		   const char* name) const
{
  ymuint32 h = 0;
  ymuint32 c;
  for ( ; (c = static_cast<ymuint32>(*name)); ++ name) {
    h = h * 37 + c;
  }
  return ((reinterpret_cast<ympuint>(parent) * h) >> 8) % mSize;
}


END_NAMESPACE_YM_VERILOG
