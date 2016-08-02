
/// @file libym_verilog/VlMgr.cc
/// @brief VlMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlMgr.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/VlMgr.h"

#include "Parser.h"
#include "PtMgr.h"
#include "parser/cpt/CptFactory.h"

#include "Elaborator.h"
#include "elaborator/ei/EiFactory.h"

#include "ElbMgr.h"
#include "ElbUdp.h"
#include "ElbUserSystf.h"
#include "ElbAttribute.h"

BEGIN_NAMESPACE_YM_VERILOG

// @brief コンストラクタ
// @param[in] msg_mgr メッセージマネージャ
// @param[in] pt_mgr パース木を保持するクラス
VlMgr::VlMgr(MsgMgr& msg_mgr) :
  mAlloc(4096),
  mMsgMgr(msg_mgr),
  mPtMgr(new PtMgr),
  mPtiFactory(new CptFactory(mAlloc)),
  mElbMgr(new ElbMgr(mAlloc)),
  mElbFactory(new EiFactory(mAlloc))
{
}

// @brief デストラクタ
VlMgr::~VlMgr()
{
  delete mPtMgr;
  delete mPtiFactory;
  delete mElbMgr;
  delete mElbFactory;
}

// @brief 内容をクリアする．
void
VlMgr::clear()
{
  mPtMgr->clear();
  mElbMgr->clear();
  mAlloc.destroy();
}

// @brief ファイルを読み込む．
// @param[in] filename 読み込むファイル名
// @param[in] searchpath サーチパス
// @param[in] watcher_list 行番号ウォッチャーのリスト
// @retval true 正常に終了した．
// @retval false エラーが起こった．
bool
VlMgr::read_file(const string& filename,
		 const SearchPathList& searchpath,
		 const list<VlLineWatcher*> watcher_list)
{
  Parser parser(mMsgMgr, *mPtMgr, mAlloc, *mPtiFactory);

  return parser.read_file(filename, searchpath, watcher_list);
}

// @brief 登録されているモジュールのリストを返す．
// @return 登録されているモジュールのリスト
const list<const PtModule*>&
VlMgr::pt_module_list() const
{
  return mPtMgr->pt_module_list();
}

// @brief 登録されている UDP のリストを返す．
// @return 登録されている UDP のリスト
const list<const PtUdp*>&
VlMgr::pt_udp_list() const
{
  return mPtMgr->pt_udp_list();
}

// @brief エラボレーションを行う．
// @param[in] msg_mgr メッセージマネージャ
// @param[in] エラー数を返す．
size_t
VlMgr::elaborate()
{
  Elaborator elab(mMsgMgr, *mElbMgr, *mElbFactory);

  return elab(*mPtMgr);
}

// @brief UDP 定義のリストを返す．
const list<const VlUdpDefn*>&
VlMgr::udp_list() const
{
  return mElbMgr->udp_list();
}

// @brief 名前から UDP 定義を取出す．
// @param[in] name 名前
// @return name という名の UDP を返す．
// @return なければ NULL を返す．
const VlUdpDefn*
VlMgr::find_udp(const char* name) const
{
  return mElbMgr->find_udp(name);
}

// @brief topmodule のリストを返す．
const list<const VlModule*>&
VlMgr::topmodule_list() const
{
  return mElbMgr->topmodule_list();
}

// @brief 名前から UserSystf を取出す．
// @param[in] name 名前
// @return name という名のユーザー定義関数を返す．
// @return なければ NULL を返す．
const VlUserSystf*
VlMgr::find_user_systf(const char* name) const
{
  return mElbMgr->find_user_systf(name);
}

// @brief スコープと名前から名前付き要素を取り出す．
// @param[in] parent 検索対象のスコープ
// @param[in] name 名前
// @return parent というスコープ内の name という要素を返す．
// @return なければ NULL を返す．
const VlNamedObj*
VlMgr::find_obj(const VlNamedObj* parent,
		const char* name) const
{
  ElbObjHandle* handle = mElbMgr->find_obj(parent, name);
  if ( handle ) {
    return handle->obj();
  }
  return NULL;
}

// @brief スコープに属する generate block のリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @param[out] scope_list 結果を格納するリスト
// @retval true 該当する要素が1つ以上あった．
// @retval false 該当する要素がなかった．
bool
VlMgr::find_genblock_list(const VlNamedObj* parent,
			  vector<const VlNamedObj*>& scope_list) const
{
  return mElbMgr->find_genblock_list(parent, scope_list);
}

// @brief スコープとタグから宣言要素を取り出す．
// @param[in] parent 検索対象のスコープ
// @param[in] tag タグ
// @param[out] decl_list 結果を格納するリスト
// @retval true 該当する要素が1つ以上あった．
// @retval false 該当する要素がなかった．
// @note scope というスコープ内の tag というタグを持つ宣言要素を
// decl_list に入れる．
bool
VlMgr::find_decl_list(const VlNamedObj* parent,
		      int tag,
		      vector<const VlDecl*>& decl_list) const
{
  return mElbMgr->find_decl_list(parent, tag, decl_list);
}

#if 0
// @brief スコープに属する parameter 宣言を取り出す．
// @param[in] parent 検索対象のスコープ
// @param[out] param_list 結果を格納するリスト
// @retval true 該当する要素が1つ以上あった．
// @retval false 該当する要素がなかった．
// @note scope というスコープ内の paramter 宣言を
// param_list に入れる．
bool
VlMgr::find_param_list(const VlNamedObj* parent,
		       vector<const VlDecl*>& param_list) const
{
  return mElbMgr->find_param_list(parent, param_list);
}
#endif

// @brief スコープに属する defparam のリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @param[out] defparam_list 結果を格納するリスト
// @retval true 該当する要素が1つ以上あった．
// @retval false 該当する要素がなかった．
bool
VlMgr::find_defparam_list(const VlNamedObj* parent,
			  vector<const VlDefParam*>& defparam_list) const
{
  return mElbMgr->find_defparam_list(parent, defparam_list);
}

// @brief スコープに属する param assign のリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @param[out] paramassign_list 結果を格納するリスト
// @retval true 該当する要素が1つ以上あった．
// @retval false 該当する要素がなかった．
bool
VlMgr::find_paramassign_list(const VlNamedObj* parent,
			     vector<const VlParamAssign*>& paramassign_list) const
{
  return mElbMgr->find_paramassign_list(parent, paramassign_list);
}

// @brief スコープに属する module のリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @param[out] module_list 結果を格納するリスト
// @retval true 該当する要素が1つ以上あった．
// @retval false 該当する要素がなかった．
bool
VlMgr::find_module_list(const VlNamedObj* parent,
			vector<const VlModule*>& module_list) const
{
  return mElbMgr->find_module_list(parent, module_list);
}

// @brief スコープに属する module arrayのリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @param[out] modulearray_list 結果を格納するリスト
// @retval true 該当する要素が1つ以上あった．
// @retval false 該当する要素がなかった．
bool
VlMgr::find_modulearray_list(const VlNamedObj* parent,
			     vector<const VlModuleArray*>& modulearray_list) const
{
  return mElbMgr->find_modulearray_list(parent, modulearray_list);
}

// @brief スコープに属する primitive のリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @param[out] primtive_list 結果を格納するリスト
// @retval true 該当する要素が1つ以上あった．
// @retval false 該当する要素がなかった．
bool
VlMgr::find_primitive_list(const VlNamedObj* parent,
			   vector<const VlPrimitive*>& primitive_list) const
{
  return mElbMgr->find_primitive_list(parent, primitive_list);
}

// @brief スコープに属する primitive array のリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @param[out] primarray_list 結果を格納するリスト
// @retval true 該当する要素が1つ以上あった．
// @retval false 該当する要素がなかった．
bool
VlMgr::find_primarray_list(const VlNamedObj* parent,
			   vector<const VlPrimArray*>& primarray_list) const
{
  return mElbMgr->find_primarray_list(parent, primarray_list);
}

// @brief スコープに属するタスクのリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @param[out] task_list 結果を格納するリスト
// @retval true 該当する要素が1つ以上あった．
// @retval false 該当する要素がなかった．
bool
VlMgr::find_task_list(const VlNamedObj* parent,
		      vector<const VlTaskFunc*>& task_list) const
{
  return mElbMgr->find_task_list(parent, task_list);
}

// @brief スコープに属する関数のリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @param[out] func_list 結果を格納するリスト
// @retval true 該当する要素が1つ以上あった．
// @retval false 該当する要素がなかった．
bool
VlMgr::find_function_list(const VlNamedObj* parent,
			  vector<const VlTaskFunc*>& func_list) const
{
  return mElbMgr->find_function_list(parent, func_list);
}

// @brief スコープに属する continuous assignment のリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @param[out] contassign_list 結果を格納するリスト
// @retval true 該当する要素が1つ以上あった．
// @retval false 該当する要素がなかった．
bool
VlMgr::find_contassign_list(const VlNamedObj* parent,
			    vector<const VlContAssign*>& contassign_list) const
{
  return mElbMgr->find_contassign_list(parent, contassign_list);
}

// @brief スコープに属する process のリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @param[out] process_list 結果を格納するリスト
// @retval true 該当する要素が1つ以上あった．
// @retval false 該当する要素がなかった．
bool
VlMgr::find_process_list(const VlNamedObj* parent,
			 vector<const VlProcess*>& process_list) const
{
  return mElbMgr->find_process_list(parent, process_list);
}

// @brief 属性リストを得る．
// @param[in] obj 対象のオブジェクト
// @param[in] def 定義側の属性の時 true とするフラグ
VlAttrList*
VlMgr::find_attr(const VlObj* obj,
		 bool def) const
{
  return mElbMgr->find_attr(obj, def);
}

// @brief このオブジェクトが確保したメモリの総量を返す．
size_t
VlMgr::allocated_size() const
{
  return mAlloc.allocated_size();
}

END_NAMESPACE_YM_VERILOG
