#ifndef LIBYM_VERILOG_ELB_ELBMGR_H
#define LIBYM_VERILOG_ELB_ELBMGR_H

/// @file libym_verilog/elaborator/include/ElbMgr.h
/// @brief ElbMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ElbMgr.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/pt/PtP.h"
#include "ym_verilog/vl/VlFwd.h"
#include "ym_utils/Alloc.h"

#include "TagDict.h"
#include "ObjDict.h"
#include "ModuleHash.h"
#include "AttrHash.h"

#include "ElbFwd.h"
#include "ElbModule.h"
#include "ElbDecl.h"
#include "ElbPrimitive.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbMgr ElbMgr.h "ElbMgr.h"
/// @brief ElbMgr の実装クラス
//////////////////////////////////////////////////////////////////////
class ElbMgr
{
public:

  /// @brief コンストラクタ
  /// @param[in] alloc メモリ確保用のオブジェクト
  ElbMgr(AllocBase& alloc);

  /// @brief デストラクタ
  virtual
  ~ElbMgr();


public:

  /// @brief 内容をクリアする．
  void
  clear();


public:
  //////////////////////////////////////////////////////////////////////
  // 要素を検索する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief UDP 定義のリストを返す．
  const list<const VlUdpDefn*>&
  udp_list() const;

  /// @brief 名前から UDP 定義を取出す．
  /// @param[in] name 名前
  /// @return name という名の UDP を返す．
  /// @return なければ NULL を返す．
  const ElbUdpDefn*
  find_udp(const char* name) const;

  /// @brief topmodule のリストを返す．
  const list<const VlModule*>&
  topmodule_list() const;

  /// @brief 名前から UserSystf を取出す．
  /// @param[in] name 名前
  /// @return name という名のユーザー定義関数を返す．
  /// @return なければ NULL を返す．
  const ElbUserSystf*
  find_user_systf(const char* name) const;

  /// @brief スコープに属する generate block のリストを取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @param[out] scope_list 結果を格納するリスト
  /// @retval true 該当する要素が1つ以上あった．
  /// @retval false 該当する要素がなかった．
  bool
  find_genblock_list(const VlNamedObj* parent,
		     vector<const VlNamedObj*>& scope_list) const;

  /// @brief スコープとタグから宣言要素を取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @param[in] tag タグ
  /// @param[out] decl_list 結果を格納するリスト
  /// @retval true 該当する要素が1つ以上あった．
  /// @retval false 該当する要素がなかった．
  /// @note scope というスコープ内の tag というタグを持つ宣言要素を
  /// decl_list に入れる．
  bool
  find_decl_list(const VlNamedObj* parent,
		 int tag,
		 vector<const VlDecl*>& decl_list) const;

  /// @brief スコープに属する defparam のリストを取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @param[out] defparam_list 結果を格納するリスト
  /// @retval true 該当する要素が1つ以上あった．
  /// @retval false 該当する要素がなかった．
  bool
  find_defparam_list(const VlNamedObj* parent,
		     vector<const VlDefParam*>& defparam_list) const;

  /// @brief スコープに属する param assign のリストを取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @param[out] paramassign_list 結果を格納するリスト
  /// @retval true 該当する要素が1つ以上あった．
  /// @retval false 該当する要素がなかった．
  bool
  find_paramassign_list(const VlNamedObj* parent,
			vector<const VlParamAssign*>& paramassign_list) const;

  /// @brief スコープに属する module のリストを取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @param[out] module_list 結果を格納するリスト
  /// @retval true 該当する要素が1つ以上あった．
  /// @retval false 該当する要素がなかった．
  bool
  find_module_list(const VlNamedObj* parent,
		   vector<const VlModule*>& module_list) const;

  /// @brief スコープに属する module arrayのリストを取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @param[out] modulearray_list 結果を格納するリスト
  /// @retval true 該当する要素が1つ以上あった．
  /// @retval false 該当する要素がなかった．
  bool
  find_modulearray_list(const VlNamedObj* parent,
			vector<const VlModuleArray*>& modulearray_list) const;

  /// @brief スコープに属する primitive のリストを取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @param[out] primtive_list 結果を格納するリスト
  /// @retval true 該当する要素が1つ以上あった．
  /// @retval false 該当する要素がなかった．
  bool
  find_primitive_list(const VlNamedObj* parent,
		      vector<const VlPrimitive*>& primitive_list) const;

  /// @brief スコープに属する primitive array のリストを取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @param[out] primarray_list 結果を格納するリスト
  /// @retval true 該当する要素が1つ以上あった．
  /// @retval false 該当する要素がなかった．
  bool
  find_primarray_list(const VlNamedObj* parent,
		      vector<const VlPrimArray*>& primarray_list) const;

  /// @brief スコープに属する continuous assignment のリストを取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @param[out] contassign_list 結果を格納するリスト
  /// @retval true 該当する要素が1つ以上あった．
  /// @retval false 該当する要素がなかった．
  bool
  find_contassign_list(const VlNamedObj* parent,
		       vector<const VlContAssign*>& contassign_list) const;

  /// @brief スコープに属するタスクのリストを取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @param[out] task_list 結果を格納するリスト
  /// @retval true 該当する要素が1つ以上あった．
  /// @retval false 該当する要素がなかった．
  bool
  find_task_list(const VlNamedObj* parent,
		 vector<const VlTaskFunc*>& task_list) const;

  /// @brief スコープに属する関数のリストを取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @param[out] func_list 結果を格納するリスト
  /// @retval true 該当する要素が1つ以上あった．
  /// @retval false 該当する要素がなかった．
  bool
  find_function_list(const VlNamedObj* parent,
		     vector<const VlTaskFunc*>& func_list) const;

  /// @brief スコープに属する process のリストを取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @param[out] process_list 結果を格納するリスト
  /// @retval true 該当する要素が1つ以上あった．
  /// @retval false 該当する要素がなかった．
  bool
  find_process_list(const VlNamedObj* parent,
		    vector<const VlProcess*>& process_list) const;

  /// @brief スコープと名前から名前付き要素を取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @param[in] name 名前
  /// @return parent というスコープ内の name という要素を返す．
  /// @return なければ NULL を返す．
  ElbObjHandle*
  find_obj(const VlNamedObj* parent,
	   const char* name) const;

  /// @brief スコープと階層名から要素を取り出す．
  /// @param[in] base_scope 起点となるスコープ
  /// @param[in] nb_array 階層名の上部 (NULL の場合も有りうる)
  /// @param[in] name 末尾の名前
  /// @param[in] ulimit 探索する名前空間の上限
  /// @return 見付かったオブジェクトを返す．
  /// 見付からなかったら NULL を返す．
  ElbObjHandle*
  find_obj_up(const VlNamedObj* base_scope,
	      PtNameBranchArray nb_array,
	      const char* name,
	      const VlNamedObj* ulimit);


public:
  //////////////////////////////////////////////////////////////////////
  // 要素を登録する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief システムタスク/システム関数を登録する．
  /// @param[in] systf 登録するシステムタスク/システム関数
  void
  reg_user_systf(const ElbUserSystf* systf);

  /// @brief UDP を登録する．
  /// @param[in] def_name 定義名
  /// @param[in] udp 登録する UDP
  void
  reg_udp(const char* def_name,
	  const ElbUdpDefn* udp);

  /// @brief グローバルスコープを登録する．
  void
  reg_toplevel(const VlNamedObj* toplevel);

  /// @brief generate block を登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_genblock(ElbScope* obj);

  /// @brief block scope を登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_blockscope(ElbScope* obj);

  /// @brief 宣言要素を登録する．
  /// @param[in] tag タグ
  /// @param[in] obj 登録するオブジェクト
  void
  reg_decl(int tag,
	   ElbDecl* obj);

  /// @brief 宣言要素を登録する．
  /// @param[in] tag タグ
  /// @param[in] obj 登録するオブジェクト
  void
  reg_declarray(int tag,
		ElbDeclArray* obj);

  /// @brief defparam を登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_defparam(ElbDefParam* obj);

  /// @brief paramassign を登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_paramassign(ElbParamAssign* obj);

  /// @brief モジュール配列を登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_modulearray(ElbModuleArray* obj);

  /// @brief モジュールを登録する．
  /// @param[in] module 登録するモジュール
  void
  reg_module(ElbModule* module);

  /// @brief プリミティブ配列を登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_primarray(ElbPrimArray* obj);

  /// @brief プリミティブを登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_primitive(ElbPrimitive* obj);

  /// @brief タスクを登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_task(ElbTaskFunc* obj);

  /// @brief 関数を登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_function(ElbTaskFunc* obj);

  /// @brief continuous assignment を登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_contassign(ElbContAssign* obj);

  /// @brief process を登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_process(ElbProcess* obj);

  /// @brief gfroot を登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_gfroot(ElbGfRoot* obj);

  /// @brief genvar を登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_genvar(ElbGenvar* obj);


public:
  //////////////////////////////////////////////////////////////////////
  // 属性の検索/登録
  //////////////////////////////////////////////////////////////////////

  /// @brief 属性リストを得る．
  /// @param[in] obj 対象のオブジェクト
  /// @param[in] def 定義側の属性の時 true とするフラグ
  ElbAttrList*
  find_attr(const VlObj* obj,
	    bool def) const;

  /// @brief 属性リストを登録する．
  /// @param[in] obj 対象のオブジェクト
  /// @param[in] def 定義側の属性の時 true とするフラグ
  /// @param[in] attr_list 属性リスト
  void
  reg_attr(const VlObj* obj,
	   bool def,
	   ElbAttrList* attr_list);


public:
  //////////////////////////////////////////////////////////////////////
  // その他の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief アロケータを取り出す．
  AllocBase&
  allocator();

  /// @brief このオブジェクトが確保したメモリの総量を返す．
  size_t
  allocated_size() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 検索の下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief base_scope を起点として (nb, "") という名前のスコープを探す．
  /// なければ親のスコープに対して同様の探索を繰り返す．
  const VlNamedObj*
  find_scope_up(const VlNamedObj* base_scope,
		PtNameBranchArray nb_array,
		const VlNamedObj* ulimit);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリ確保用のアロケータ
  AllocBase& mAlloc;

  // UDP のリスト
  list<const VlUdpDefn*> mUdpList;

  // UDP の辞書
  hash_map<string, const ElbUdpDefn*> mUdpHash;

  // topmodule のリスト
  list<const VlModule*> mTopmoduleList;

  // UserSystf の辞書
  hash_map<string, const ElbUserSystf*> mSystfHash;

  // 名前をキーにしたオブジェクトの辞書
  ObjDict mObjDict;

  // タグをキーにした各スコープごとのオブジェクトのリストの辞書
  TagDict mTagDict;

  // モジュール名をキーにしたモジュールインスタンスの辞書
  ModuleHash mModInstDict;

  // 属性リストの辞書
  AttrHash mAttrHash;

  // トップレベルスコープ
  const VlNamedObj* mTopLevel;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief スコープに属する generate block のリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @param[out] scope_list 結果を格納するリスト
// @retval true 該当する要素が1つ以上あった．
// @retval false 該当する要素がなかった．
inline
bool
ElbMgr::find_genblock_list(const VlNamedObj* parent,
			   vector<const VlNamedObj*>& scope_list) const
{
  return mTagDict.find_genblock_list(parent, scope_list);
}

// @brief スコープとタグから宣言要素を取り出す．
// @param[in] parent 検索対象のスコープ
// @param[in] tag タグ
// @param[out] decl_list 結果を格納するリスト
// @retval true 該当する要素が1つ以上あった．
// @retval false 該当する要素がなかった．
// @note scope というスコープ内の tag というタグを持つ宣言要素を
// decl_list に入れる．
inline
bool
ElbMgr::find_decl_list(const VlNamedObj* parent,
		       int tag,
		       vector<const VlDecl*>& decl_list) const
{
  return mTagDict.find_decl_list(parent, tag, decl_list);
}

// @brief スコープに属する defparam のリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @param[out] defparam_list 結果を格納するリスト
// @retval true 該当する要素が1つ以上あった．
// @retval false 該当する要素がなかった．
inline
bool
ElbMgr::find_defparam_list(const VlNamedObj* parent,
			   vector<const VlDefParam*>& defparam_list) const
{
  return mTagDict.find_defparam_list(parent, defparam_list);
}

// @brief スコープに属する param assign のリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @param[out] paramassign_list 結果を格納するリスト
// @retval true 該当する要素が1つ以上あった．
// @retval false 該当する要素がなかった．
inline
bool
ElbMgr::find_paramassign_list(const VlNamedObj* parent,
			      vector<const VlParamAssign*>& paramassign_list) const
{
  return mTagDict.find_paramassign_list(parent, paramassign_list);
}

// @brief スコープに属する module のリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @param[out] module_list 結果を格納するリスト
// @retval true 該当する要素が1つ以上あった．
// @retval false 該当する要素がなかった．
inline
bool
ElbMgr::find_module_list(const VlNamedObj* parent,
			 vector<const VlModule*>& module_list) const
{
  return mTagDict.find_module_list(parent, module_list);
}

// @brief スコープに属する module arrayのリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @param[out] modulearray_list 結果を格納するリスト
// @retval true 該当する要素が1つ以上あった．
// @retval false 該当する要素がなかった．
inline
bool
ElbMgr::find_modulearray_list(const VlNamedObj* parent,
			      vector<const VlModuleArray*>& modulearray_list) const
{
  return mTagDict.find_modulearray_list(parent, modulearray_list);
}

// @brief スコープに属する primitive のリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @param[out] primtive_list 結果を格納するリスト
// @retval true 該当する要素が1つ以上あった．
// @retval false 該当する要素がなかった．
inline
bool
ElbMgr::find_primitive_list(const VlNamedObj* parent,
			    vector<const VlPrimitive*>& primitive_list) const
{
  return mTagDict.find_primitive_list(parent, primitive_list);
}

// @brief スコープに属する primitive array のリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @param[out] primarray_list 結果を格納するリスト
// @retval true 該当する要素が1つ以上あった．
// @retval false 該当する要素がなかった．
inline
bool
ElbMgr::find_primarray_list(const VlNamedObj* parent,
			    vector<const VlPrimArray*>& primarray_list) const
{
  return mTagDict.find_primarray_list(parent, primarray_list);
}

// @brief スコープに属する continuous assignment のリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @param[out] contassign_list 結果を格納するリスト
// @retval true 該当する要素が1つ以上あった．
// @retval false 該当する要素がなかった．
inline
bool
ElbMgr::find_contassign_list(const VlNamedObj* parent,
			     vector<const VlContAssign*>& contassign_list) const
{
  return mTagDict.find_contassign_list(parent, contassign_list);
}

// @brief スコープに属するタスクのリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @param[out] task_list 結果を格納するリスト
// @retval true 該当する要素が1つ以上あった．
// @retval false 該当する要素がなかった．
inline
bool
ElbMgr::find_task_list(const VlNamedObj* parent,
		       vector<const VlTaskFunc*>& task_list) const
{
  return mTagDict.find_task_list(parent, task_list);
}

// @brief スコープに属する関数のリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @param[out] func_list 結果を格納するリスト
// @retval true 該当する要素が1つ以上あった．
// @retval false 該当する要素がなかった．
inline
bool
ElbMgr::find_function_list(const VlNamedObj* parent,
			   vector<const VlTaskFunc*>& func_list) const
{
  return mTagDict.find_function_list(parent, func_list);
}

// @brief スコープに属する process のリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @param[out] process_list 結果を格納するリスト
// @retval true 該当する要素が1つ以上あった．
// @retval false 該当する要素がなかった．
inline
bool
ElbMgr::find_process_list(const VlNamedObj* parent,
			  vector<const VlProcess*>& process_list) const
{
  return mTagDict.find_process_list(parent, process_list);
}

// @brief 属性リストを得る．
// @param[in] obj 対象のオブジェクト
// @param[in] def 定義側の属性の時 true とするフラグ
inline
ElbAttrList*
ElbMgr::find_attr(const VlObj* obj,
		  bool def) const
{
  return mAttrHash.find(obj, def);
}

// @brief アロケータを取り出す．
inline
AllocBase&
ElbMgr::allocator()
{
  return mAlloc;
}

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELB_ELBMGR_H
