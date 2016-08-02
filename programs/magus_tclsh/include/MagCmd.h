#ifndef MAGUS_MAGCMD_H
#define MAGUS_MAGCMD_H

/// @file MagCmd.h
/// @brief MagCmd のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MagCmd.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "magus_nsdef.h"
#include "ym_tclpp/TclCmd.h"
#include "NetHandle.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class MagCmd MagCmd.h "MagCmd.h"
/// @ingroup MagusGroup
/// @brief Magus のコマンドオブジェクトの基底クラス
///
/// おもな仕事はエラーが起きた時にメッセージをセット
/// するような関数の殻をかぶせること．
/// あとは変換などの便利関数を入れてある．
//////////////////////////////////////////////////////////////////////
class MagCmd :
  public TclCmd
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  MagCmd(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~MagCmd();


protected:
  //////////////////////////////////////////////////////////////////////
  // ネットワークの検索，登録，削除関係の処理
  //////////////////////////////////////////////////////////////////////

  /// @brief ネットワーク名の重複チェック
  /// @param[in] name 対象のネットワーク名
  /// @return 登録されていれば true を返す．
  /// @note エラーメッセージはセットされない．
  bool
  check_network_name(const string& name) const;

  /// @brief 新たな BNetwork を作成して登録する．
  /// @param[in] name 名前
  /// @return 作成したネットハンドル
  /// @note 同名のネットワークが既に存在していた場合にはエラーとなる．
  /// @note また，名前が不適切な場合にもエラーとなる．
  /// @note エラーが起きた場合にはインタプリタに然るべきメッセージをセットして
  /// NULL を返す．
  NetHandle*
  new_bnethandle(const string& name) const;

  /// @brief 新たな BdNetwork を作成して登録する．
  /// @param[in] name 名前
  /// @return 作成したネットハンドル
  /// @note 同名のネットワークが既に存在していた場合にはエラーとなる．
  /// @note また，名前が不適切な場合にもエラーとなる．
  /// @note エラーが起きた場合にはインタプリタに然るべきメッセージをセットして
  /// NULL を返す．
  NetHandle*
  new_bdnhandle(const string& name) const;

  /// @brief 新たな MvNetwork を作成して登録する．
  /// @param[in] name 名前
  /// @return 作成したネットハンドル
  /// @note 同名のネットワークが既に存在していた場合にはエラーとなる．
  /// @note また，名前が不適切な場合にもエラーとなる．
  /// @note エラーが起きた場合にはインタプリタに然るべきメッセージをセットして
  /// NULL を返す．
  NetHandle*
  new_mvnhandle(const string& name) const;

  /// @brief ネットワークの削除
  /// @param[in] name 名前
  /// @return 削除が成功したら true を返す．
  /// @note 名前が不正なときや使用中のときにはエラーとなる．
  bool
  delete_nethandle(const string& name) const;

  /// @brief ネットワークの検索
  /// @param[in] name 名前
  /// @return name という名のネットワークを返す．
  /// @note "_" の場合にはカレントネットワークをとって来る．
  /// @note 名前が不適切な場合やその名前のネットワークが存在しない場合には
  /// エラーとなる．
  /// @note エラーが起きた場合にはインタプリタに然るべきメッセージをセット
  /// して NULL を返す．
  NetHandle*
  find_nethandle(const string& name) const;

  /// @brief name という名のネットワークをなんとしてもとって来る．
  /// @param[in] name ネットワーク名
  /// @param[in] type ネットワークの型
  /// @return name という名のネットワークを返す．
  /// @note 要するに無ければ作る．
  NetHandle*
  find_or_new_nethandle(const string& name,
			NetHandle::tType type) const;



protected:
  //////////////////////////////////////////////////////////////////////
  // カレントネットワーク関係の処理
  //////////////////////////////////////////////////////////////////////

  /// @brief 操作対象のネットワークを変更し，スタックに積む．
  /// @param[in] name ネットワーク名
  /// @return 変更が成功したら true を返す．
  /// @note ネットワーク名が適切でない時にはインタプリタにメッセージを
  /// 記録して false を返す．
  bool
  push_cur_nethandle(const string& name) const;

  /// @brief スタックからネットワークを復帰させる．
  /// @return 処理が成功すれば true を返す．
  /// @note スタックが空の場合にはエラーとなる．
  bool
  pop_cur_nethandle() const;

  /// @brief 操作対象のネットワークを変更する．
  /// @param[in] name ネットワーク名
  /// @return 変更が成功したら true を返す．
  /// @note ネットワーク名が適切でない時にはインタプリタにメッセージを
  /// 記録して false を返す．
  /// @note 常にスタックは変更しない．
  bool
  change_cur_nethandle(const string& name) const;

  /// @brief 現在，操作対象となっているネットワークを返す．
  /// @note 常にカレントネットワークは存在するはず．
  /// (と言いつつ中ではちゃんとチェックはしてますよ．)
  NetHandle*
  cur_nethandle() const;

  // マネージャを取得する．
  MagMgr*
  mgr() const;


private:
  //////////////////////////////////////////////////////////////////////
  // メンバ変数
  //////////////////////////////////////////////////////////////////////

  // ネットワークやセルライブラリ等のデータを管理するオブジェクト
  MagMgr* mMagMgr;

};

END_NAMESPACE_MAGUS

#endif // MAGUS_MAGCMD_H
