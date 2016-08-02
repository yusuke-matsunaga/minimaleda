
/// @file magus/logbase/MagCmd.cc
/// @brief MagCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MagCmd.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MagCmd.h"
#include "MagMgr.h"
#include "NetHandle.h"
#include "BNetHandle.h"
#include "BdnHandle.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// Magus のコマンドオブジェクトの基底クラス MagCmd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MagCmd::MagCmd(MagMgr* mgr) :
  mMagMgr(mgr)
{
}

// @brief デストラクタ
MagCmd::~MagCmd()
{
}

// マネージャを取得する．
MagMgr*
MagCmd::mgr() const
{
  return mMagMgr;
}


//////////////////////////////////////////////////////////////////////
// ネットワーク操作関係のMagCmdのメソッド
// 中身を見れば分かるけどほとんどが MagMgr を呼び出している．
//////////////////////////////////////////////////////////////////////

// name という名のネットワークが登録されているか調べる．
// 登録されていれば true を返す．
bool
MagCmd::check_network_name(const string& name) const
{
  NetHandle* neth = mMagMgr->find_nethandle(name, false);
  return (neth != NULL) ? true : false;
}

// @brief name という名の新たなネットワークエントリを作成して登録する．
// @param[in] name 名前
// @note 同名のネットワークが既に存在していた場合にはエラーとなる．
// @note また，名前が不適切な場合にもエラーとなる．
// @note エラーが起きた場合にはインタプリタに然るべきメッセージをセットして
// NULL を返す．
NetHandle*
MagCmd::new_bnethandle(const string& name) const
{
  ostringstream buf;
  NetHandle* neth = mMagMgr->new_bnethandle(name, &buf);
  if ( neth == NULL ) {
    set_result(buf.str());
  }
  return neth;
}

// @brief 新たな BdNetwork を作成して登録する．
// @param[in] name 名前
// @return 作成したネットハンドル
// @note 同名のネットワークが既に存在していた場合にはエラーとなる．
// @note また，名前が不適切な場合にもエラーとなる．
// @note エラーが起きた場合にはインタプリタに然るべきメッセージをセットして
// NULL を返す．
NetHandle*
MagCmd::new_bdnhandle(const string& name) const
{
  ostringstream buf;
  NetHandle* neth = mMagMgr->new_bdnhandle(name, &buf);
  if ( neth == NULL ) {
    set_result(buf.str());
  }
  return neth;
}

// @brief 新たな MvNetwork を作成して登録する．
// @param[in] name 名前
// @return 作成したネットハンドル
// @note 同名のネットワークが既に存在していた場合にはエラーとなる．
// @note また，名前が不適切な場合にもエラーとなる．
// @note エラーが起きた場合にはインタプリタに然るべきメッセージをセットして
// NULL を返す．
NetHandle*
MagCmd::new_mvnhandle(const string& name) const
{
  ostringstream buf;
  NetHandle* neth = mMagMgr->new_mvnhandle(name, &buf);
  if ( neth == NULL ) {
    set_result(buf.str());
  }
  return neth;
}

// name という名のネットワークを削除する．
// ただし，カレントネットワークは削除できない．
// また，名前が不適切な場合や存在しない場合にはエラーとなる．
// エラーの場合にはインタプリタに然るべきメッセージをセットしてfalse
// を返す．
bool
MagCmd::delete_nethandle(const string& name) const
{
  ostringstream buf;
  bool stat = mMagMgr->delete_nethandle(name, &buf);
  if ( !stat ) {
    set_result(buf.str());
  }
  return stat;
}

// name という名のネットワークを見付ける．
// "_" の場合にはカレントネットワークをとって来る．
// 名前が不適切な場合やその名前のネットワークが存在しない場合にはエラーと
// なる．
NetHandle*
MagCmd::find_nethandle(const string& name) const
{
  ostringstream buf;
  NetHandle* neth = mMagMgr->find_nethandle(name, &buf);
  if ( neth == NULL ) {
    set_result(buf.str());
  }
  return neth;
}

// name という名のナットワークをとって来る．
// 存在しない場合には新たに生成する．
NetHandle*
MagCmd::find_or_new_nethandle(const string& name,
			      NetHandle::tType type) const
{
  NetHandle* neth = mMagMgr->find_nethandle(name);
  if ( !neth ) {
    switch ( type ) {
    case NetHandle::kMagBNet:
      neth = new_bnethandle(name);
      break;

    case NetHandle::kMagBdn:
      neth = new_bdnhandle(name);
      break;

    default:
      assert_not_reached(__FILE__, __LINE__);
      break;
    }
  }
  return neth;
}

// @brief 操作対象のネットワークを変更し，スタックに積む．
// @param[in] name ネットワーク名
// @return 変更が成功したら true を返す．
// @note ネットワーク名が適切でない時にはインタプリタにメッセージを
// 記録して false を返す．
bool
MagCmd::push_cur_nethandle(const string& name) const
{
  ostringstream buf;
  bool stat = mMagMgr->push_cur_nethandle(name, &buf);
  if ( stat ) {
    return true;
  }
  set_result(buf.str());
  return false;
}

// @brief スタックからネットワークを復帰させる．
// @return 処理が成功すれば true を返す．
// @note スタックが空の場合にはエラーとなる．
bool
MagCmd::pop_cur_nethandle() const
{
  ostringstream buf;
  bool stat = mMagMgr->pop_cur_nethandle(&buf);
  if ( stat ) {
    return true;
  }
  set_result(buf.str());
  return false;
}

// 操作対象のネットワークを新たに指定する．
// スタックは変更しない．
// ネットワーク名が適切でない時にはインタプリタにメッセージを
// 記録して false を返す．
// この時，カレントネットワークは変更されない．
bool
MagCmd::change_cur_nethandle(const string& name) const
{
  ostringstream buf;
  bool stat = mMagMgr->change_cur_nethandle(name, &buf);
  if ( stat ) {
    return true;
  }
  set_result(buf.str());
  return false;
}

// カレントネットワークを取り出す．スタックは不変．
NetHandle*
MagCmd::cur_nethandle() const
{
  NetHandle* neth = mMagMgr->cur_nethandle();
  assert_cond(neth != NULL, __FILE__, __LINE__);
  return neth;
}

#if 0
// name という名前のノードが存在しないか確かめる．
// 存在していた場合には interp にエラーメッセージをセットして
// false を返す．
bool
MagCmd::check_node_name(const string& name) const
{
  if ( name == string() ) {
    return true;
  }

  BNode* node = cur_network()->find_node(name);
  if ( node != NULL ) {
    // 指定された名前のノードが既に存在している．
    TclObj emsg;
    emsg << name << " : Already exists.";
    set_result(emsg);
    return false;
  }

  return true;
}

// ノード名からノードを取り出す
BNode*
MagCmd::find_node(TclObj& obj)
{
  string node_str = obj.get_string(NULL);
  BNode* node = cur_network()->find_node(node_str);
  if ( node == NULL ) {
    // ノード名が不正
    TclObj emsg;
    emsg << node_str << " : does not exist";
    set_result(emsg);
  }
  return node;
}
#endif

END_NAMESPACE_MAGUS
