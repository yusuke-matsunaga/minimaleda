#ifndef LIBYM_VERILOG_ELB_OBJDICT_H
#define LIBYM_VERILOG_ELB_OBJDICT_H

/// @file libym_verilog/elaborator/include/ObjDict.h
/// @brief ObjDict のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ObjDict.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/verilog.h"
#include "ym_utils/FileRegion.h"
#include "ym_utils/Alloc.h"

#include "ym_verilog/vl/VlFwd.h"
#include "ElbFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbObjHandle ObjDict.h "ObjDict.h"
//////////////////////////////////////////////////////////////////////
class ElbObjHandle
{
  friend class ObjDict;

protected:

  /// @brief コンストラクタ
  ElbObjHandle();

  /// @brief デストラクタ
  virtual
  ~ElbObjHandle();


public:

  /// @brief VlNamedObj を返す．
  virtual
  const VlNamedObj*
  obj() = 0;

  /// @brief オブジェクトの型を返す．
  tVpiObjType
  type();

  /// @brief ファイル位置の取得
  virtual
  FileRegion
  file_region();

  /// @brief オブジェクトの階層付き名前を返す．
  string
  full_name();

  /// @brief 配列要素を返す．
  /// @note このクラスでは NULL を返す．
  virtual
  const VlNamedObj*
  array_elem(int index);

  /// @brief ElbDecl を返す．
  /// @note このクラスでは NULL を返す．
  virtual
  ElbDecl*
  decl();

  /// @brief ElbDeclArray を返す．
  /// @note このクラスでは NULL を返す．
  virtual
  ElbDeclArray*
  decl_array();

  /// @brief ElbTaskFunc を返す．
  /// @note このクラスでは NULL を返す．
  virtual
  ElbTaskFunc*
  taskfunc();

  /// @brief ElbModuleArray を返す．
  /// @note このクラスでは NULL を返す．
  virtual
  ElbModuleArray*
  module_array();

  /// @brief ElbPrimArray を返す．
  /// @note このクラスでは NULL を返す．
  virtual
  ElbPrimArray*
  prim_array();

  /// @brief ElbPrimitive を返す．
  /// @note このクラスでは NULL を返す．
  virtual
  ElbPrimitive*
  primitive();

  /// @brief ElbGfRoot を返す．
  /// @note このクラスでは NULL を返す．
  virtual
  ElbGfRoot*
  gfroot();

  /// @brief ElbGenvar を返す．
  /// @note このクラスでは NULL を返す．
  virtual
  ElbGenvar*
  genvar();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 次の要素を指すリンク
  ElbObjHandle* mLink;

};


//////////////////////////////////////////////////////////////////////
/// @class ElbTaskFuncHandle
//////////////////////////////////////////////////////////////////////
class ElbTaskFuncHandle :
  public ElbObjHandle
{
  friend class ObjDict;

private:

  /// @brief コンストラクタ
  ElbTaskFuncHandle(ElbTaskFunc* obj);

  /// @brief デストラクタ
  virtual
  ~ElbTaskFuncHandle();


public:

  /// @brief VlNamedObj を返す．
  virtual
  const VlNamedObj*
  obj();

  /// @brief ElbTaskFunc を返す．
  virtual
  ElbTaskFunc*
  taskfunc();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のオブジェクト
  ElbTaskFunc* mObj;

};


//////////////////////////////////////////////////////////////////////
/// @class ElbDeclHandle
//////////////////////////////////////////////////////////////////////
class ElbDeclHandle :
  public ElbObjHandle
{
  friend class ObjDict;

private:

  /// @brief コンストラクタ
  ElbDeclHandle(ElbDecl* obj);

  /// @brief デストラクタ
  virtual
  ~ElbDeclHandle();


public:

  /// @brief VlNamedObj を返す．
  virtual
  const VlNamedObj*
  obj();

  /// @brief ElbDecl を返す．
  virtual
  ElbDecl*
  decl();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のオブジェクト
  ElbDecl* mObj;

};


//////////////////////////////////////////////////////////////////////
/// @class ElbDeclArrayHandle
//////////////////////////////////////////////////////////////////////
class ElbDeclArrayHandle :
  public ElbObjHandle
{
  friend class ObjDict;

private:

  /// @brief コンストラクタ
  ElbDeclArrayHandle(ElbDeclArray* obj);

  /// @brief デストラクタ
  virtual
  ~ElbDeclArrayHandle();


public:

  /// @brief VlNamedObj を返す．
  virtual
  const VlNamedObj*
  obj();

  /// @brief ElbDeclArray を返す．
  virtual
  ElbDeclArray*
  decl_array();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のオブジェクト
  ElbDeclArray* mObj;

};


//////////////////////////////////////////////////////////////////////
/// @class ElbModuleArrayHandle
//////////////////////////////////////////////////////////////////////
class ElbModuleArrayHandle :
  public ElbObjHandle
{
  friend class ObjDict;

private:

  /// @brief コンストラクタ
  ElbModuleArrayHandle(ElbModuleArray* obj);

  /// @brief デストラクタ
  virtual
  ~ElbModuleArrayHandle();


public:

  /// @brief VlNamedObj を返す．
  virtual
  const VlNamedObj*
  obj();

  /// @brief 配列要素を返す．
  virtual
  const VlNamedObj*
  array_elem(int index);

  /// @brief ElbModuleArray を返す．
  virtual
  ElbModuleArray*
  module_array();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のオブジェクト
  ElbModuleArray* mObj;

};


//////////////////////////////////////////////////////////////////////
/// @class ElbPrimArrayHandle
//////////////////////////////////////////////////////////////////////
class ElbPrimArrayHandle :
  public ElbObjHandle
{
  friend class ObjDict;

private:

  /// @brief コンストラクタ
  ElbPrimArrayHandle(ElbPrimArray* obj);

  /// @brief デストラクタ
  virtual
  ~ElbPrimArrayHandle();


public:

  /// @brief VlNamedObj を返す．
  virtual
  const VlNamedObj*
  obj();

  /// @brief ElbPrimArray を返す．
  virtual
  ElbPrimArray*
  prim_array();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のオブジェクト
  ElbPrimArray* mObj;

};


//////////////////////////////////////////////////////////////////////
/// @class ElbPrimitiveHandle
//////////////////////////////////////////////////////////////////////
class ElbPrimitiveHandle :
  public ElbObjHandle
{
  friend class ObjDict;

private:

  /// @brief コンストラクタ
  ElbPrimitiveHandle(ElbPrimitive* obj);

  /// @brief デストラクタ
  virtual
  ~ElbPrimitiveHandle();


public:

  /// @brief VlNamedObj を返す．
  virtual
  const VlNamedObj*
  obj();

  /// @brief ElbPrimitive を返す．
  virtual
  ElbPrimitive*
  primitive();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のオブジェクト
  ElbPrimitive* mObj;

};


//////////////////////////////////////////////////////////////////////
/// @class ElbScopeHandle
//////////////////////////////////////////////////////////////////////
class ElbScopeHandle :
  public ElbObjHandle
{
  friend class ObjDict;

private:

  /// @brief コンストラクタ
  ElbScopeHandle(const VlNamedObj* obj);

  /// @brief デストラクタ
  virtual
  ~ElbScopeHandle();


public:

  /// @brief VlNamedObj を返す．
  virtual
  const VlNamedObj*
  obj();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のオブジェクト
  const VlNamedObj* mObj;

};


//////////////////////////////////////////////////////////////////////
/// @class ElbGfRootHandle
//////////////////////////////////////////////////////////////////////
class ElbGfRootHandle :
  public ElbObjHandle
{
  friend class ObjDict;

private:

  /// @brief コンストラクタ
  ElbGfRootHandle(ElbGfRoot* obj);

  /// @brief デストラクタ
  virtual
  ~ElbGfRootHandle();


public:

  /// @brief VlNamedObj を返す．
  virtual
  const VlNamedObj*
  obj();

  /// @brief 配列要素を返す．
  virtual
  const VlNamedObj*
  array_elem(int index);

  /// @brief ElbGfRoot を返す．
  virtual
  ElbGfRoot*
  gfroot();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のオブジェクト
  ElbGfRoot* mObj;

};


//////////////////////////////////////////////////////////////////////
/// @class ElbGenvarHandle
//////////////////////////////////////////////////////////////////////
class ElbGenvarHandle :
  public ElbObjHandle
{
  friend class ObjDict;

private:

  /// @brief コンストラクタ
  ElbGenvarHandle(ElbGenvar* obj);

  /// @brief デストラクタ
  virtual
  ~ElbGenvarHandle();


public:

  /// @brief VlNamedObj を返す．
  virtual
  const VlNamedObj*
  obj();

  /// @brief ElbGenvar を返す．
  virtual
  ElbGenvar*
  genvar();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のオブジェクト
  ElbGenvar* mObj;

};


//////////////////////////////////////////////////////////////////////
/// @class ObjDict ObjDict.h "ObjDict.h"
/// @brief VlNamedObj を格納するハッシュ表
//////////////////////////////////////////////////////////////////////
class ObjDict
{
public:

  /// @brief コンストラクタ
  /// @param[in] alloc メモリ確保用のアロケータ
  ObjDict(AllocBase& alloc);

  /// @brief デストラクタ
  ~ObjDict();


public:
  //////////////////////////////////////////////////////////////////////
  // ObjDict の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を空にする．
  void
  clear();

  /// @brief 要素を追加する．
  void
  add(const VlNamedObj* obj);

  /// @brief 要素を追加する．
  void
  add(ElbTaskFunc* obj);

  /// @brief 要素を追加する．
  void
  add(ElbDecl* obj);

  /// @brief 要素を追加する．
  void
  add(ElbDeclArray* obj);

  /// @brief 要素を追加する．
  void
  add(ElbModuleArray* obj);

  /// @brief 要素を追加する．
  void
  add(ElbPrimArray* obj);

  /// @brief 要素を追加する．
  void
  add(ElbPrimitive* obj);

  /// @brief 要素を追加する．
  void
  add(ElbGfRoot* obj);

  /// @brief 要素を追加する．
  void
  add(ElbGenvar* obj);

  /// @brief 名前から該当する要素を検索する．
  /// @note なければ NULL を返す．
  ElbObjHandle*
  find(const VlNamedObj* parent,
       const char* name) const;

  /// @brief このオブジェクトが使用しているメモリ量を返す．
  size_t
  allocated_size() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ハンドルを追加する．
  void
  add_handle(const VlNamedObj* parent,
	     const char* name,
	     ElbObjHandle* handle);

  /// @brief テーブルの領域を確保する．
  void
  alloc_table(ymuint32 size);

  /// @brief ハッシュ値を計算する．
  ymuint32
  hash_func(const VlNamedObj* parent,
	    const char* name) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ElbObjHandle の確保用のアロケータ
  UnitAlloc mAlloc;

  // ハッシュ表のサイズ
  ymuint32 mSize;

  // ハッシュ表
  ElbObjHandle** mTable;

  // ハッシュ表を拡大するしきい値
  ymuint32 mLimit;

  // 要素数
  ymuint32 mNum;

};


END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELB_OBJDICT_H
