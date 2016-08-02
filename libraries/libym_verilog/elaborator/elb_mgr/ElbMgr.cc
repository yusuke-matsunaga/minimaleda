
/// @file libym_verilog/elb/ElbMgr.cc
/// @brief ElbMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ElbMgr.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ElbMgr.h"

#include "ym_verilog/pt/PtMisc.h"
#include "ym_verilog/pt/PtArray.h"

#include "ElbUdp.h"
#include "ElbModule.h"
#include "ElbTaskFunc.h"
#include "ElbScope.h"
#include "ElbGfRoot.h"
#include "ElbDecl.h"
#include "ElbParameter.h"
#include "ElbParamAssign.h"
#include "ElbPrimitive.h"
#include "ElbContAssign.h"
#include "ElbProcess.h"
#include "ElbUserSystf.h"
#include "ElbGenvar.h"


#define dout cerr

const ymuint debug_none       = 0x00000000;
const ymuint debug_objdict    = 0x00000001;
const ymuint debug_find_scope = 0x00000010;
const ymuint debug_all        = 0xFFFFFFFF;

#if 1
const ymuint debug = debug_none;
#else
const ymuint debug = debug_all;
#endif


BEGIN_NAMESPACE_YM_VERILOG

// @brief コンストラクタ
// @param[in] alloc メモリ確保用のオブジェクト
ElbMgr::ElbMgr(AllocBase& alloc) :
  mAlloc(alloc),
  mObjDict(alloc),
  mTagDict(alloc),
  mModInstDict(alloc),
  mAttrHash(alloc)
{
}

// @brief デストラクタ
ElbMgr::~ElbMgr()
{
}

// @brief 内容をクリアする．
void
ElbMgr::clear()
{
  mUdpList.clear();
  mUdpHash.clear();
  mTopmoduleList.clear();
  mSystfHash.clear();
  mTagDict.clear();
  mObjDict.clear();
  mModInstDict.clear();
  mAttrHash.clear();
  mTopLevel = NULL;
}

// @brief UDP 定義のリストを返す．
const list<const VlUdpDefn*>&
ElbMgr::udp_list() const
{
  return mUdpList;
}

// @brief 名前から UDP 定義を取出す．
// @param[in] name 名前
// @return name という名の UDP を返す．
// @return なければ NULL を返す．
const ElbUdpDefn*
ElbMgr::find_udp(const char* name) const
{
  hash_map<string, const ElbUdpDefn*>::const_iterator p;
  p = mUdpHash.find(name);
  if ( p == mUdpHash.end() ) {
    return NULL;
  }
  return p->second;
}

// @brief UDP を登録する．
// @param[in] def_name 定義名
// @param[in] udp 登録する UDP
void
ElbMgr::reg_udp(const char* def_name,
		const ElbUdpDefn* udp)
{
  mUdpList.push_back(udp);
  mUdpHash.insert(make_pair(def_name, udp));
}

// @brief グローバルスコープを登録する．
void
ElbMgr::reg_toplevel(const VlNamedObj* toplevel)
{
  mTopLevel = toplevel;
}

// @brief topmodule のリストを返す．
const list<const VlModule*>&
ElbMgr::topmodule_list() const
{
  return mTopmoduleList;
}

// @brief 名前から UserSystf を取出す．
// @param[in] name 名前
// @return name という名のユーザー定義関数を返す．
// @return なければ NULL を返す．
const ElbUserSystf*
ElbMgr::find_user_systf(const char* name) const
{
  hash_map<string, const ElbUserSystf*>::const_iterator p;
  p = mSystfHash.find(name);
  if ( p == mSystfHash.end() ) {
    return NULL;
  }
  return p->second;
}

// @brief システムタスク/システム関数を登録する．
void
ElbMgr::reg_user_systf(const ElbUserSystf* systf)
{
  mSystfHash.insert(make_pair(systf->_name(), systf));
}

// @brief generate block を登録する．
// @param[in] obj 登録するオブジェクト
void
ElbMgr::reg_genblock(ElbScope* obj)
{
  if ( debug & debug_objdict ) {
    dout << "reg_genblock( " << obj->name() << " @ "
	 << obj->parent()->full_name()
	 << " ["
	 << hex << reinterpret_cast<ympuint>(obj->parent()) << dec
	 << "] )" << endl
	 << endl;
  }
  mObjDict.add(obj);
  mTagDict.add_genblock(obj);
}

// @brief block scope を登録する．
// @param[in] obj 登録するオブジェクト
void
ElbMgr::reg_blockscope(ElbScope* obj)
{
  if ( debug & debug_objdict ) {
    dout << "reg_blockscope( " << obj->name() << " @ "
	 << obj->parent()->full_name()
	 << " ["
	 << hex << reinterpret_cast<ympuint>(obj->parent()) << dec
	 << "] )" << endl
	 << endl;
  }
  mObjDict.add(obj);
#if 0 // TagDict::add_blockscope() は作ってない．
  mTagDict.add_blockscope(obj);
#endif
}

// @brief 宣言要素を登録する．
// @param[in] tag タグ
// @param[in] obj 登録するオブジェクト
void
ElbMgr::reg_decl(int tag,
		 ElbDecl* obj)
{
  if ( debug & debug_objdict ) {
    dout << "reg_decl( " << obj->name() << " @ "
	 << obj->parent()->full_name()
	 << " ["
	 << hex << reinterpret_cast<ympuint>(obj->parent()) << dec
	 << "] )" << endl << endl;
  }
  mObjDict.add(obj);
  if ( tag ) {
    mTagDict.add_decl(tag, obj);
  }
}

// @brief 配列型の宣言要素を登録する．
// @param[in] tag タグ
// @param[in] obj 登録するオブジェクト
void
ElbMgr::reg_declarray(int tag,
		      ElbDeclArray* obj)
{
  if ( debug & debug_objdict ) {
    dout << "reg_declarray( " << obj->name() << " @ "
	 << obj->parent()->full_name()
	 << " ["
	 << hex << reinterpret_cast<ympuint>(obj->parent()) << dec
	 << "] )" << endl << endl;
  }
  mObjDict.add(obj);
  if ( tag ) {
    mTagDict.add_decl(tag, obj);
  }
}

#if 0
// @brief parameter 宣言を登録する．
// @param[in] obj 登録するオブジェクト
void
ElbMgr::reg_parameter(ElbParameter* obj)
{
  if ( debug & debug_objdict ) {
    dout << "reg_parameter( " << obj->name() << " @ "
	 << obj->parent()->full_name()
	 << " ["
	 << hex << reinterpret_cast<ympuint>(obj->parent()) << dec
	 << "] )" << endl << endl;
  }
  mObjDict.add(obj);
  mTagDict.add_parameter(obj);
}
#endif

// @brief defparam を登録する．
// @param[in] obj 登録するオブジェクト
void
ElbMgr::reg_defparam(ElbDefParam* obj)
{
  mTagDict.add_defparam(obj);
}

// @brief paramassign を登録する．
// @param[in] obj 登録するオブジェクト
void
ElbMgr::reg_paramassign(ElbParamAssign* obj)
{
  mTagDict.add_paramassign(obj);
}

// @brief モジュール配列を登録する．
// @param[in] obj 登録するオブジェクト
void
ElbMgr::reg_modulearray(ElbModuleArray* obj)
{
  if ( debug & debug_objdict ) {
    dout << "reg_modulearray( " << obj->name() << " @ "
	 << obj->parent()->full_name()
	 << " ["
	 << hex << reinterpret_cast<ympuint>(obj->parent()) << dec
	 << "] )" << endl << endl;
  }
  mObjDict.add(obj);
  mTagDict.add_modulearray(obj);
}

// @brief モジュールを登録する．
void
ElbMgr::reg_module(ElbModule* obj)
{
  if ( debug & debug_objdict ) {
    dout << "reg_module( " << obj->name() << " @ "
	 << obj->parent()->full_name()
	 << " ["
	 << hex << reinterpret_cast<ympuint>(obj->parent()) << dec
	 << "] )" << endl << endl;
  }
  mObjDict.add(obj);
  mModInstDict.add(obj);
  mTagDict.add_module(obj);
  if ( obj->parent() == mTopLevel ) {
    mTopmoduleList.push_back(obj);
  }
}

// @brief プリミティブ配列を登録する．
// @param[in] obj 登録するオブジェクト
void
ElbMgr::reg_primarray(ElbPrimArray* obj)
{
  if ( obj->name() != NULL ) {
    if ( debug & debug_objdict ) {
      dout << "reg_primarray( " << obj->name() << " @ "
	   << obj->parent()->full_name()
	 << " ["
	 << hex << reinterpret_cast<ympuint>(obj->parent()) << dec
	 << "] )" << endl << endl;
    }
    mObjDict.add(obj);
  }
  mTagDict.add_primarray(obj);
}

// @brief プリミティブを登録する．
// @param[in] obj 登録するオブジェクト
void
ElbMgr::reg_primitive(ElbPrimitive* obj)
{
  if ( obj->name() != NULL ) {
    if ( debug & debug_objdict ) {
      dout << "reg_primitive( " << obj->name() << " @ "
	   << obj->parent()->full_name()
	   << " ["
	   << hex << reinterpret_cast<ympuint>(obj->parent()) << dec
	   << "] )" << endl << endl;
    }
    mObjDict.add(obj);
  }
  mTagDict.add_primitive(obj);
}

// @brief タスクを登録する．
// @param[in] obj 登録するオブジェクト
void
ElbMgr::reg_task(ElbTaskFunc* obj)
{
  if ( debug & debug_objdict ) {
    dout << "reg_task( " << obj->name() << " @ "
	 << obj->parent()->full_name()
	 << " ["
	 << hex << reinterpret_cast<ympuint>(obj->parent()) << dec
	 << "] )" << endl << endl;
  }
  mObjDict.add(obj);
  mTagDict.add_task(obj);
}

// @brief 関数を登録する．
// @param[in] obj 登録するオブジェクト
void
ElbMgr::reg_function(ElbTaskFunc* obj)
{
  if ( debug & debug_objdict ) {
    dout << "reg_function( " << obj->name() << " @ "
	 << obj->parent()->full_name()
	 << " ["
	 << hex << reinterpret_cast<ympuint>(obj->parent()) << dec
	 << "] )" << endl << endl;
  }
  mObjDict.add(obj);
  mTagDict.add_function(obj);
}

// @brief continuous assignment を登録する．
// @param[in] obj 登録するオブジェクト
void
ElbMgr::reg_contassign(ElbContAssign* obj)
{
  mTagDict.add_contassign(obj);
}

// @brief process を登録する．
// @param[in] obj 登録するオブジェクト
void
ElbMgr::reg_process(ElbProcess* obj)
{
  mTagDict.add_process(obj);
}

// @brief gfroot を登録する．
// @param[in] obj 登録するオブジェクト
void
ElbMgr::reg_gfroot(ElbGfRoot* obj)
{
  if ( debug & debug_objdict ) {
    dout << "reg_gfroot( " << obj->name() << " @ "
	 << obj->parent()->full_name()
	 << " ["
	 << hex << reinterpret_cast<ympuint>(obj->parent()) << dec
	 << "] )" << endl << endl;
  }
  mObjDict.add(obj);
}

// @brief genvar を登録する．
// @param[in] obj 登録するオブジェクト
void
ElbMgr::reg_genvar(ElbGenvar* obj)
{
  if ( debug & debug_objdict ) {
    dout << "reg_genvar( " << obj->name() << " @ "
	 << obj->parent()->full_name()
	 << " ["
	 << hex << reinterpret_cast<ympuint>(obj->parent()) << dec
	 << "] )" << endl << endl;
  }
  mObjDict.add(obj);
}

// @brief 属性リストを登録する．
// @param[in] obj 対象のオブジェクト
// @param[in] def 定義側の属性の時 true とするフラグ
// @param[in] attr_list 属性リスト
void
ElbMgr::reg_attr(const VlObj* obj,
		 bool def,
		 ElbAttrList* attr_list)
{
  mAttrHash.add(obj, def, attr_list);
}

// @brief スコープと名前から名前付き要素を取り出す．
// @param[in] parent 検索対象のスコープ
// @param[in] name 名前
// @return parent というスコープ内の name という要素を返す．
// @return なければ NULL を返す．
ElbObjHandle*
ElbMgr::find_obj(const VlNamedObj* scope,
		 const char* name) const
{
  if ( debug & debug_find_scope ) {
    dout << "find_obj( " << name << ", @ "
	 << scope->full_name()
	 << " ["
	 << hex << reinterpret_cast<ympuint>(scope) << dec
	 << "] )" << endl << endl;
  }

  ElbObjHandle* handle = mObjDict.find(scope, name);

  if ( handle == NULL ) {
    if ( debug & debug_find_scope ) {
      dout << "--> Not Found"
	   << endl << endl;
    }
    // なければ NULL を返す。
    return NULL;
  }

  if ( debug & debug_find_scope ) {
    dout << "--> Found"
	 << endl << endl;
  }

  return handle;
}

// @brief 名前によるオブジェクトの探索
// @param[in] base_scope 起点となるスコープ
// @param[in] nb_array 階層名の上部 (NULL の場合も有りうる)
// @param[in] name 末尾の名前
// @param[in] ulimit 探索する名前空間の上限
// @return 見付かったオブジェクトを返す．
// 見付からなかったら NULL を返す．
ElbObjHandle*
ElbMgr::find_obj_up(const VlNamedObj* base_scope,
		    PtNameBranchArray nb_array,
		    const char* name,
		    const VlNamedObj* ulimit)
{
  // まず nb の部分の解決を行う．
  base_scope = find_scope_up(base_scope, nb_array, ulimit);
  if ( base_scope == NULL ) {
    return NULL;
  }

  if ( debug & debug_find_scope ) {
    dout << "find_obj_up( " << name << " )@"
	 << base_scope->full_name() << endl;
  }

  // base_scope を起点として name というオブジェクトを探す．
  for ( ; base_scope; base_scope = base_scope->parent()) {
    ElbObjHandle* handle = find_obj(base_scope, name);
    if ( handle ) {
      // 見つけた
      if ( debug & debug_find_scope ) {
	dout << "--> Found: " << handle->obj()->name() << " @ "
	     << base_scope->name() << endl << endl;
      }
      return handle;
    }
    // base_scope が上限だったのでこれ以上 upward search できない．
    if ( base_scope == ulimit ) {
      if ( debug & debug_find_scope ) {
	dout << "--> Not found: reaches to ulimit" << endl << endl;
      }
      return NULL;
    }
  }
  // ダミー
  return NULL;
}

// base_scope を起点として (name_branch, "" ) という名前のスコープを探す．
// なければ親のスコープに対して同様の探索を繰り返す．
const VlNamedObj*
ElbMgr::find_scope_up(const VlNamedObj* base_scope,
		      PtNameBranchArray nb_array,
		      const VlNamedObj* ulimit)
{
  if ( debug & debug_find_scope ) {
    dout << "find_scope_up( "
	 << expand_full_name(nb_array, NULL)
	 << " ) @"
	 << base_scope->full_name() << endl;
  }

  ymuint32 n = nb_array.size();
  const VlNamedObj* cur_scope = base_scope;
  for (ymuint32 i = 0; i < n; ) {
    PtNameBranch* name_branch = nb_array[i];
    const char* top_name = name_branch->name();
    const VlNamedObj* top_scope = NULL;
    // まず普通に探す．
    ElbObjHandle* handle = find_obj(cur_scope, top_name);
    if ( handle ) {
      if ( name_branch->has_index() ) {
	top_scope = handle->array_elem(name_branch->index());
      }
      else {
	top_scope = handle->obj();
      }
    }
    else if ( !name_branch->has_index() ) {
      // モジュール定義名として探す．
      top_scope = mModInstDict.find(cur_scope, top_name);
    }
    if ( top_scope == NULL) {
      // cur_scope が上限もしくは cur_scope の親がいなければ
      // upward search できない．
      if ( cur_scope == ulimit || cur_scope->parent() == NULL ) {

	if ( debug & debug_find_scope ) {
	  dout << "--> Not Found" << endl << endl;
	}

	return NULL;
      }

      if ( debug & debug_find_scope ) {
	dout << " upward search" << endl;
      }

      cur_scope = cur_scope->parent();
    }
    else {

      if ( debug & debug_find_scope ) {
	dout << " downward search" << endl;
      }

      cur_scope = top_scope;
      ++ i;
    }
  }

  if ( debug & debug_find_scope ) {
    dout << "--> Found: " << cur_scope->full_name() << endl << endl;
  }

  return cur_scope;
}

// @brief このオブジェクトが確保したメモリの総量を返す．
size_t
ElbMgr::allocated_size() const
{
  return mTagDict.allocated_size()
    + mObjDict.allocated_size()
    + mModInstDict.allocated_size()
    + mAttrHash.allocated_size();
}

END_NAMESPACE_YM_VERILOG
