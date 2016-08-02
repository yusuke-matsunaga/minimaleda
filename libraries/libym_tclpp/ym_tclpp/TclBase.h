#ifndef YM_TCLPP_TCLBASE_H
#define YM_TCLPP_TCLBASE_H

/// @file ym_tclpp/TclBase.h
/// @brief TclBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TclBase.h 1417 2008-05-06 18:12:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_tclpp/tclpp_nsdef.h"


BEGIN_NAMESPACE_YM_TCLPP

//////////////////////////////////////////////////////////////////////
/// @class TclBase TclBase.h <ym_tclpp/TclBase.h>
/// @ingroup Tclpp
/// @brief Tcl インタープリタを持つクラス
///
/// 基本的にこのクラスのメンバ関数は自分の持っているインタプリタに
/// 仕事をさせているだけなので自分自身の状態が変わるわけではない．
/// よって，コンストラクタ，デストラクタ，および set_interp() 以外は
/// すべて const メソッドとなる．
/// 要するにインタプリタに用件を伝えるだけでインタプリタそのものでは
/// ないということ．
//////////////////////////////////////////////////////////////////////
class TclBase
{
public:

  /// @brief コンストラクタ
  /// @note 最初は特定のインタープリタには結び付いていない．
  TclBase();
  
  /// @brief デストラクタ
  virtual
  ~TclBase();
  
  /// @brief インタープリタの取得
  /// @return インタープリタを返す．
  /// NULL の場合もあり得る．
  Tcl_Interp*
  interp() const;
  
  
  //////////////////////////////////////////////////////////////////////
  /// @name Tcl スクリプトの評価関数
  /// @{

  /// @brief スクリプトの評価
  ///
  /// 結果またはエラーメッセージをインタープリタにセットする．
  /// @param[in] script 評価する Tcl スクリプト
  /// @retval TCL_OK 評価が成功した
  /// @retval TCL_ERROR エラーが起きた
  int
  eval(const TclObj& script) const;
  
  /// @brief  グローバルレベルでのスクリプトの評価
  ///
  /// 結果またはエラーメッセージをインタープリタにセットする．
  /// @param[in] script 評価する Tcl スクリプト
  /// @retval TCL_OK 評価が成功した
  /// @retval TCL_ERROR エラーが起きた
  int
  global_eval(const TclObj& script) const;
  
  /// @brief ファイルの評価
  ///
  /// 結果またはエラーメッセージをインタープリタにセットする．
  /// @param[in] file 評価するファイルのパス名
  /// @retval TCL_OK 評価が成功した
  /// @retval TCL_ERROR エラーが起きた
  int
  eval_file(const string& file) const;

  /// @brief 履歴リストへの記録を伴う評価
  ///
  /// スクリプトの内容を履歴リストにイベントとして登録する．
  /// 結果またはエラーメッセージをインタープリタにセットする．
  /// @param[in] script 評価する Tcl スクリプト
  /// @param[in] flags ダミー実行フラグ．この値が0ならば実際に評価する．
  /// この値が TCL_NO_EVALなら評価しない．
  /// @retval TCL_OK 評価が成功した
  /// @retval TCL_ERROR エラーが起きた
  int
  record_and_eval(const TclObj& script,
		  int flags) const;

  /// @}
  //////////////////////////////////////////////////////////////////////
  

  //////////////////////////////////////////////////////////////////////
  /// @name コマンド操作関数
  /// @{

  //////////////////////////////////////////////////////////////////////
  /// @warning コマンドの登録に関しては，ClientData と Tcl_CmdProc,
  /// Tcl_CmdDeleteProc をひとまとめにしたクラス TclCmd
  /// を用いる．
  ///
  /// 直接 Tcl_CreateCommand() を呼んでははいけない．
  /// @sa TclCmd
  //////////////////////////////////////////////////////////////////////
  
  /// @brief コマンドの削除
  /// @param[in] cmd_name 削除するコマンド名
  /// @retval TCL_OK 成功した
  /// @retval TCL_ERROR エラーが起きた
  int
  delete_command(const string& cmd_name) const;
  
  /// @brief token を使ったコマンドの削除
  /// @param[in] token 削除するコマンドのトークン
  /// @retval TCL_OK 成功した
  /// @retval TCL_ERROR エラーが起きた
  /// @sa TclCmd
  int
  delete_command(Tcl_Command token) const;
  
  /// @brief コマンドに関する情報の取得
  /// @param[in] cmd_name コメント名
  /// @param[out] info 得られたコマンド情報
  /// @retval 0 cmd_name という名のコマンド情報の取得に成功した
  /// @retval 1 cmd_name という名のコマンドは登録されていない
  /// @warning この関数の戻り値は TCL_OK や TCL_ERROR ではないので注意．
  int
  command_info(const string& cmd_name,
	       Tcl_CmdInfo* info) const;
  
  /// @brief コマンドに関する情報のセット
  /// @param[in] cmd_name コメント名
  /// @param[in] info セットするコマンド情報
  /// @retval TCL_OK 成功した
  /// @retval TCL_ERROR エラーが起きた
  /// cmd_name が登録されていれば info の情報を cmd_name の情報にコピーし，
  /// TCL_OK を返す．登録されていなければ TCL_ERROR を返す．
  /// この関数を使うことで，コマンド削除時に用いられる ClientData を
  /// CreateCommand() の引数の ClientData から変更することができる．
  int
  set_command_info(const string& cmd_name,
		   Tcl_CmdInfo* info) const;
  
  /// @brief コマンド名の取得
  /// @param[in] token コマンドのトークン
  /// @return コマンド名
  /// @sa TclCmd
  string
  command_name(Tcl_Command token) const;

  /// @}
  //////////////////////////////////////////////////////////////////////

  
  //////////////////////////////////////////////////////////////////////
  /// @name 結果にアクセスするための関数
  /// @{

  /// @brief 結果のセット
  /// @param[in] obj 結果にセットするオブジェクト
  void
  set_result(const TclObj& obj) const;
  
  /// @brief 結果のクリア
  void
  reset_result() const;
  
  /// @brief 結果の取得
  /// @return 結果を表すオブジェクト
  TclObj
  result() const;

  /// @}
  //////////////////////////////////////////////////////////////////////
  

  //////////////////////////////////////////////////////////////////////
  /// @name オブジェクトの型変換
  /// @{

  /// @brief オブジェクトから整数への変換
  /// @param[in] obj 変換元のオブジェクト
  /// @param[out] ans 変換結果を格納する変数
  /// @retval TCL_OK 変換が成功した
  /// @retval TCL_ERROR エラーが起こった．
  /// エラーの場合にはエラーメッセージをインタプリタにセットする．
  int
  int_conv(const TclObj& obj,
	   ymint& ans) const;
  
  /// @brief オブジェクトから unsigned int への変換
  /// @param[in] obj 変換元のオブジェクト
  /// @param[out] ans 変換結果を格納する変数
  /// @retval TCL_OK 変換が成功した
  /// @retval TCL_ERROR エラーが起こった．
  /// エラーの場合にはエラーメッセージをインタプリタにセットする．
  int
  uint_conv(const TclObj& obj,
	    ymuint& ans) const;
  
  /// @brief オブジェクトから long への変換
  /// @param[in] obj 変換元のオブジェクト
  /// @param[out] ans 変換結果を格納する変数
  /// @retval TCL_OK 変換が成功した
  /// @retval TCL_ERROR エラーが起こった．
  /// エラーの場合にはエラーメッセージをインタプリタにセットする．
  int
  long_conv(const TclObj& obj,
	    ymlong& ans) const;
  
  /// @brief オブジェクトから unsigned long への変換
  /// @param[in] obj 変換元のオブジェクト
  /// @param[out] ans 変換結果を格納する変数
  /// @retval TCL_OK 変換が成功した
  /// @retval TCL_ERROR エラーが起こった．
  /// エラーの場合にはエラーメッセージをインタプリタにセットする．
  int
  ulong_conv(const TclObj& obj,
	     ymulong& ans) const;
  
  /// @brief オブジェクトから double への変換
  /// @param[in] obj 変換元のオブジェクト
  /// @param[out] ans 変換結果を格納する変数
  /// @retval TCL_OK 変換が成功した
  /// @retval TCL_ERROR エラーが起こった．
  /// エラーの場合にはエラーメッセージをインタプリタにセットする．
  int
  double_conv(const TclObj& obj,
	      double& ans) const;
  
  /// @brief オブジェクトからブール値への変換
  /// @param[in] obj 変換元のオブジェクト
  /// @param[out] ans 変換結果を格納する変数
  /// @retval TCL_OK 変換が成功した
  /// @retval TCL_ERROR エラーが起こった．
  /// エラーの場合にはエラーメッセージをインタプリタにセットする．
  int
  bool_conv(const TclObj& obj,
	    bool& ans) const;
  
  /// @brief オブジェクトからリストへの変換
  /// @param[in] obj 変換元のオブジェクト
  /// @param[out] objv 変換結果を格納するベクタ
  /// @retval TCL_OK 変換が成功した
  /// @retval TCL_ERROR エラーが起こった．
  int
  list_conv(const TclObj& obj,
	    TclObjVector& objv) const;
  
  /// @brief オブジェクトからリストへの変換
  /// @param[in] obj 変換元のオブジェクト
  /// @param[out] objv 変換結果を格納するリスト
  /// @retval TCL_OK 変換が成功した
  /// @retval TCL_ERROR エラーが起こった．
  int
  list_conv(const TclObj& obj,
	    TclObjList& objv) const;
  
  /// @brief オブジェクトからリストへの変換
  /// @param[in] obj 変換元のオブジェクト
  /// @param[out] length 変換結果のリストの要素数を格納する変数
  /// @retval TCL_OK 変換が成功した
  /// @retval TCL_ERROR エラーが起こった．
  /// エラーの場合にはエラーメッセージをインタプリタにセットする．
  int
  list_length(const TclObj& obj,
	      size_t& length) const;
  
  /// @brief オブジェクトからリストへの変換
  /// @param[in] obj 変換元のオブジェクト
  /// @param[in] index 取り出す要素の位置 (最初の位置は 0)
  /// @param[out] elem 変換結果のリストの index 番目の要素を格納する変数
  /// @retval TCL_OK 変換が成功した
  /// @retval TCL_ERROR エラーが起こった．
  int
  list_index(const TclObj& obj,
	     size_t index,
	     TclObj& elem) const;

  /// @}
  //////////////////////////////////////////////////////////////////////

  
  //////////////////////////////////////////////////////////////////////
  /// @name 式の評価
  /// @{

  /// @brief long int 式の評価
  ///
  /// オブジェクトを long int 式として評価する．
  /// @param[in] obj 評価するオブジェクト
  /// @param[out] ans 評価結果を格納する変数
  /// @retval TCL_OK 評価が成功した
  /// @retval TCL_ERROR エラーが起こった．
  /// エラーが発生した場合にはエラーメッセージをインタプリタにセットする．
  int
  expr_long(const TclObj& obj,
	    long& ans) const;

  /// @brief double 式の評価
  ///
  /// オブジェクトを double 式として評価する．
  /// @param[in] obj 評価するオブジェクト
  /// @param[out] ans 評価結果を格納する変数
  /// @retval TCL_OK 評価が成功した
  /// @retval TCL_ERROR エラーが起こった．
  /// エラーが発生した場合にはエラーメッセージをインタプリタにセットする．
  int
  expr_double(const TclObj& obj,
	      double& ans) const;

  /// @brief bool 式の評価
  ///
  /// オブジェクトを bool 式として評価する．
  /// @param[in] obj 評価するオブジェクト
  /// @param[out] ans 評価結果を格納する変数
  /// @retval TCL_OK 評価が成功した
  /// @retval TCL_ERROR エラーが起こった．
  /// エラーが発生した場合にはエラーメッセージをインタプリタにセットする．
  int
  expr_bool(const TclObj& obj,
	    bool& ans) const;
  
  /// @brief オブジェクトの評価
  ///
  /// 結果を TclObj の形で評価する．
  /// @param[in] obj 評価するオブジェクト
  /// @param[out] ans 評価結果を格納するオブジェクト
  /// @retval TCL_OK 評価が成功した
  /// @retval TCL_ERROR エラーが起こった．
  /// エラーが発生した場合にはエラーメッセージをインタプリタにセットする．
  int
  expr(const TclObj& obj,
       TclObj& ans) const;

  /// @brief double を文字列に変換する．
  ///
  /// 精度は Tcl 変数 tcl_precision で指定する．
  /// @param[in] val 入力値
  /// @return val を文字列に変換したもの
  string
  print_double(double val) const;

  /// @}
  //////////////////////////////////////////////////////////////////////
  

  //////////////////////////////////////////////////////////////////////
  /// @name エラー情報の設定
  /// @{

  /// @brief エラー情報の追加
  /// @param[in] message 追加するエラーメッセージ
  /// message を Tcl変数 errorInfo に入っているスタックトレース
  /// に追加する．
  void
  add_errorinfo(const string& message) const;

  /// @brief エラーコードのセット
  /// @param[in] obj エラーコード
  /// obj を Tcl 変数 errorCode にセットする．
  void
  set_errorcode(const TclObj& obj) const;
  
  /// @brief バックグラウンドエラーの報告
  void
  background_error() const;

  /// @}
  //////////////////////////////////////////////////////////////////////
  

  //////////////////////////////////////////////////////////////////////
  /// @name Tcl変数の操作
  /// @{

  /// @brief 変数の値の設定
  /// @param[in] name 値を設定する対象の変数名
  /// @param[in] value 設定する値
  /// @param[in] flags 種々のフラグ
  /// @return 変数に代入された値を返す．
  /// エラーの場合には NULL を持つオブジェクトを返す．
  ///
  /// name を変数名とする変数に値として value を代入する
  /// 変数が存在しなければ新たに作られる．
  /// @note flags の値の意味については Tcl の本を参照
  /// @todo TclBase::set_var() の flags の意味のドキュメントをつくる．
  TclObj
  set_var(const TclObj& name,
	  const TclObj& value,
	  int flags) const;

  /// @brief 配列型変数の値の設定
  /// @param[in] base 値を設定する対象の変数名
  /// @param[in] idx 値を設定する対象のインデックス
  /// @param[in] value 設定する値
  /// @param[in] flags 種々のフラグ
  /// @return 変数に代入された値を返す．
  /// エラーの場合には NULL を持つオブジェクトを返す．
  ///
  /// base(idx) を変数名とする変数に値として value を代入する．
  /// 変数が存在しなければ新たに作られる．
  /// @note flags の値の意味については Tcl の本を参照
  TclObj
  set_var(const TclObj& base,
	  const TclObj& idx,
	  const TclObj& value,
	  int flags) const;
  
  /// @brief 変数の値の取得
  /// @param[in] name 対象の変数名
  /// @param[in] flags 種々のフラグ
  /// @return name の値を返す．
  /// エラーの場合には NULL を持つオブジェクトを返す．
  ///
  /// name を変数名とする変数の値を返す．
  /// @note flags については Tcl の本を参照
  TclObj
  var(const TclObj& name,
      int flags) const;

  /// @brief 配列型変数の値の取得
  /// @param[in] base 対象の変数名
  /// @param[in] idx 対象のインデックス
  /// @param[in] flags 種々のフラグ
  /// @return base(idx) の値を返す．
  /// エラーの場合には NULL を持つオブジェクトを返す．
  ///
  /// base(idx) を変数名とする変数の値を返す．
  /// @note flags については Tcl の本を参照
  TclObj
  var(const TclObj& base,
      const TclObj& idx,
      int flags) const;

  /// @brief 変数の削除
  /// @param[in] name 削除する変数の名前
  /// @param[in] flags 種々のフラグ
  /// @retval TCL_OK 削除が成功した
  /// @retval TCL_ERROR 削除が失敗した (たぶん変数が存在しなかった)
  ///
  /// Tcl 変数 name を削除する．
  /// @note flags については Tcl の本を参照
  int
  unset_var(const string& name,
	    int flags) const;

  /// @brief 配列型変数の要素の削除
  /// @param[in] base 削除する変数の名前
  /// @param[in] idx 削除する要素のインデックス
  /// @param[in] flags 種々のフラグ
  /// @retval TCL_OK 削除が成功した
  /// @retval TCL_ERROR 削除が失敗した (たぶん変数が存在しなかった)
  ///
  /// Tcl変数 base(idx) を削除する．
  /// @note flags については Tcl の本を参照
  int
  unset_var(const string& base,
	    const string& idx,
	    int flags) const;

  /// @brief C変数とのリンクの設定
  /// @param[in] name Tcl変数の名前
  /// @param[in] addr リンクさせるC変数のアドレス
  /// @param[in] type タイプ？
  /// @retval TCL_OK リンクが成功した
  /// @retval TCL_ERROR リンクが失敗した
  ///
  /// Tcl変数 name とそのアドレスが addr であるC変数をリンクさせる．
  /// @note type に関しては Tcl の本を参照
  /// @todo TclBase::link_var() の type のドキュメントを生成する
  int
  link_var(const string& name,
	   char* addr,
	   int type) const;

  /// @brief C変数とのリンクの解除
  /// @param[in] name Tcl変数の名前
  ///
  /// Tcl変数 name に対するリンクを削除する．
  void
  unlink_var(const string& name) const;

  /// @}
  //////////////////////////////////////////////////////////////////////
  

  //////////////////////////////////////////////////////////////////////
  /// @name Tcl変数の値の取得と型変換を組み合わせた関数
  /// @{

  /// @brief Tcl変数から int への変換
  /// @param[in] name Tcl変数名
  /// @param[in] flags 種々のフラグ
  /// @param[out] ans 変換結果を格納する変数
  /// @retval TCL_OK 値の取得と変換が成功した
  /// @retval TCL_ERROR 失敗した
  ///
  /// name という変数の値を整数に変換し，ans に代入する．
  /// エラーがあればエラーメッセージをインタプリタにセットする．
  /// @sa int TclBase::var(const TclObj& name, int flags) const
  int
  int_from_var(const TclObj& name,
	       int flags,
	       int& ans) const;

  /// @brief 配列型 Tcl変数から int への変換
  /// @param[in] base Tcl変数名
  /// @param[in] idx インデックス
  /// @param[in] flags 種々のフラグ
  /// @param[out] ans 変換結果を格納する変数
  /// @retval TCL_OK 値の取得と変換が成功した
  /// @retval TCL_ERROR 失敗した
  ///
  /// base(idx) という変数の値を整数に変換し，ans に代入する．
  /// エラーがあればエラーメッセージをインタプリタにセットする．
  /// @sa int TclBase::var(const TclObj& base, const TclObj& idx, int flags) const
  int
  int_from_var(const TclObj& base,
	       const TclObj& idx,
	       int flags,
	       int& ans) const;

  /// @brief Tcl変数から unsigned int への変換
  /// @param[in] name Tcl変数名
  /// @param[in] flags 種々のフラグ
  /// @param[out] ans 変換結果を格納する変数
  /// @retval TCL_OK 値の取得と変換が成功した
  /// @retval TCL_ERROR 失敗した
  ///
  /// name という変数の値を符号なし整数に変換し，ans に代入する．
  /// @sa int TclBase::var(const TclObj& name, int flags) const
  int
  uint_from_var(const TclObj& name,
		int flags,
		unsigned int& ans) const;

  /// @brief 配列型 Tcl変数から unsigned int への変換
  /// @param[in] base Tcl変数名
  /// @param[in] idx インデックス
  /// @param[in] flags 種々のフラグ
  /// @param[out] ans 変換結果を格納する変数
  /// @retval TCL_OK 値の取得と変換が成功した
  /// @retval TCL_ERROR 失敗した
  ///
  /// base(idx) という変数の値を符号なし整数に変換し，ans に代入する．
  /// @sa int TclBase::var(const TclObj& base, const TclObj& idx, int flags) const
  int
  uint_from_var(const TclObj& base,
		const TclObj& idx,
		int flags,
		unsigned int& ans) const;

  /// @brief Tcl変数から long への変換
  /// @param[in] name Tcl変数名
  /// @param[in] flags 種々のフラグ
  /// @param[out] ans 変換結果を格納する変数
  /// @retval TCL_OK 値の取得と変換が成功した
  /// @retval TCL_ERROR 失敗した
  ///
  /// name という変数の値を long整数に変換し，ans に代入する．
  /// @sa int TclBase::var(const TclObj& name, int flags) const
  int
  long_from_var(const TclObj& name,
		int flags,
		long& ans) const;

  /// @brief 配列型 Tcl変数から long への変換
  /// @param[in] base Tcl変数名
  /// @param[in] idx インデックス
  /// @param[in] flags 種々のフラグ
  /// @param[out] ans 変換結果を格納する変数
  /// @retval TCL_OK 値の取得と変換が成功した
  /// @retval TCL_ERROR 失敗した
  ///
  /// base(idx) という変数の値を long整数に変換し，ans に代入する．
  /// @sa int TclBase::var(const TclObj& base, const TclObj& idx, int flags) const
  int
  long_from_var(const TclObj& base,
		const TclObj& idx,
		int flags,
		long& ans) const;
  
  /// @brief Tcl変数から unsigned long への変換
  /// @param[in] name Tcl変数名
  /// @param[in] flags 種々のフラグ
  /// @param[out] ans 変換結果を格納する変数
  /// @retval TCL_OK 値の取得と変換が成功した
  /// @retval TCL_ERROR 失敗した
  ///
  /// name という変数の値を 符号なしlong整数に変換し，ans に代入する．
  /// @sa int TclBase::var(const TclObj& name, int flags) const
  int
  ulong_from_var(const TclObj& name,
		 int flags,
		 unsigned long& ans) const;
  
  /// @brief 配列型 Tcl変数から unsigned long への変換
  /// @param[in] base Tcl変数名
  /// @param[in] idx インデックス
  /// @param[in] flags 種々のフラグ
  /// @param[out] ans 変換結果を格納する変数
  /// @retval TCL_OK 値の取得と変換が成功した
  /// @retval TCL_ERROR 失敗した
  ///
  /// base(idx) という変数の値を 符号なしlong整数に変換し，ans に代入する．
  /// @sa int TclBase::var(const TclObj& base, const TclObj& idx, int flags) const
  int
  ulong_from_var(const TclObj& base,
		 const TclObj& idx,
		 int flags,
		 unsigned long& ans) const;
  
  /// @brief Tcl変数から double への変換
  /// @param[in] name Tcl変数名
  /// @param[in] flags 種々のフラグ
  /// @param[out] ans 変換結果を格納する変数
  /// @retval TCL_OK 値の取得と変換が成功した
  /// @retval TCL_ERROR 失敗した
  ///
  /// name という変数の値を double に変換し，ans に代入する．
  /// @sa int TclBase::var(const TclObj& name, int flags) const
  int
  double_from_var(const TclObj& name,
		  int flags,
		  double& ans) const;
  
  /// @brief 配列型 Tcl変数から double への変換
  /// @param[in] base Tcl変数名
  /// @param[in] idx インデックス
  /// @param[in] flags 種々のフラグ
  /// @param[out] ans 変換結果を格納する変数
  /// @retval TCL_OK 値の取得と変換が成功した
  /// @retval TCL_ERROR 失敗した
  ///
  /// base(idx) という変数の値を double に変換し，ans に代入する．
  /// @sa int TclBase::var(const TclObj& base, const TclObj& idx, int flags) const
  int
  double_from_var(const TclObj& base,
		  const TclObj& idx,
		  int flags,
		  double& ans) const;
  
  /// @brief Tcl変数から bool への変換
  /// @param[in] name Tcl変数名
  /// @param[in] flags 種々のフラグ
  /// @param[out] ans 変換結果を格納する変数
  /// @retval TCL_OK 値の取得と変換が成功した
  /// @retval TCL_ERROR 失敗した
  ///
  /// name という変数の値をブール値に変換し，ans に代入する．
  /// @sa int TclBase::var(const TclObj& name, int flags) const
  int
  bool_from_var(const TclObj& name,
		int flags,
		bool& ans) const;
  
  /// @brief 配列型 Tcl変数から bool への変換
  /// @param[in] base Tcl変数名
  /// @param[in] idx インデックス
  /// @param[in] flags 種々のフラグ
  /// @param[out] ans 変換結果を格納する変数
  /// @retval TCL_OK 値の取得と変換が成功した
  /// @retval TCL_ERROR 失敗した
  ///
  /// base(idx) という変数の値をブール値に変換し，ans に代入する．
  /// @sa int TclBase::var(const TclObj& base, const TclObj& idx, int flags) const
  int
  bool_from_var(const TclObj& base,
		const TclObj& idx,
		int flags,
		bool& ans) const;
  
  /// @brief Tcl変数からリストへの変換
  /// @param[in] name Tcl変数名
  /// @param[in] flags 種々のフラグ
  /// @param[out] objv 変換結果を格納するベクタ
  /// @retval TCL_OK 値の取得と変換が成功した
  /// @retval TCL_ERROR 失敗した
  ///
  /// name という変数の値をリストに変換し，objv に代入する．
  /// @sa int TclBase::var(const TclObj& name, int flags) const
  int
  list_from_var(const TclObj& name,
		int flags,
		TclObjVector& objv) const;
  
  /// @brief Tcl変数からリストへの変換
  /// @param[in] name Tcl変数名
  /// @param[in] flags 種々のフラグ
  /// @param[out] objv 変換結果を格納するリスト
  /// @retval TCL_OK 値の取得と変換が成功した
  /// @retval TCL_ERROR 失敗した
  ///
  /// name という変数の値をリストに変換し，objv に代入する．
  /// @sa int TclBase::var(const TclObj& name, int flags) const
  int
  list_from_var(const TclObj& name,
		int flags,
		TclObjList& objv) const;
  
  /// @brief 配列型 Tcl変数からリストへの変換
  /// @param[in] base Tcl変数名
  /// @param[in] idx インデックス
  /// @param[in] flags 種々のフラグ
  /// @param[out] objv 変換結果を格納するベクタ
  /// @retval TCL_OK 値の取得と変換が成功した
  /// @retval TCL_ERROR 失敗した
  ///
  /// base(idx) という変数の値をリストに変換し，objv に代入する．
  /// @sa int TclBase::var(const TclObj& base, const TclObj& idx, int flags) const
  int
  list_from_var(const TclObj& base,
		const TclObj& idx,
		int flags,
		TclObjVector& objv) const;
  
  /// @brief 配列型 Tcl変数からリストへの変換
  /// @param[in] base Tcl変数名
  /// @param[in] idx インデックス
  /// @param[in] flags 種々のフラグ
  /// @param[out] objv 変換結果を格納するリスト
  /// @retval TCL_OK 値の取得と変換が成功した
  /// @retval TCL_ERROR 失敗した
  ///
  /// base(idx) という変数の値をリストに変換し，objv に代入する．
  /// @sa int TclBase::var(const TclObj& base, const TclObj& idx, int flags) const
  int
  list_from_var(const TclObj& base,
		const TclObj& idx,
		int flags,
		TclObjList& objv) const;

  /// @}
  //////////////////////////////////////////////////////////////////////
  

  //////////////////////////////////////////////////////////////////////
  /// @name 変数トレース情報の取得
  /// @{

  //////////////////////////////////////////////////////////////////////
  /// @warning 変数トレースの生成／破壊に関しては Tcl の生のAPI
  /// を使わずに TclVarTrace を用いる．
  /// @sa TclVarTrace
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数トレース情報の取得
  /// @param[in] name Tcl変数名
  /// @param[in] flags 種々のフラグ
  /// @param[in] prev_obj 探索を開始するポインタ
  /// @return 見つかった変数トレース
  ///
  /// prev_obj が NULL ならば変数 name に設定してあるトレースのうち，
  /// flags が一致する(ただし，flags には TCL_GLOBAL_ONLY だけが
  /// 使われる)最初のトレースの TclVarTrace を返す．NULL でなければ，
  /// flags が一致するトレースのうち，TclVarTrace が prev_obj
  /// であるものから探索を開始し，次に一致したトレースの TclVarTrace
  /// を返す．もし一致するトレースがそれ以上ない場合には NULL を返す．
  TclVarTrace*
  vartrace_info(const string& name,
		int flags,
		TclVarTrace* prev_obj) const;
  
  /// @brief 変数トレース情報の取得
  /// @param[in] base Tcl変数名
  /// @param[in] idx インデックス
  /// @param[in] flags 種々のフラグ
  /// @param[in] prev_obj 探索を開始するポインタ
  /// @return 見つかった変数トレース
  ///
  /// prev_obj が NULL ならば変数 base(idx) に設定してあるトレースの
  /// うち，flags が一致する(ただし，flags には TCL_GLOBAL_ONLY だ
  /// けが使われる)最初のトレースの TclVarTrace を返す．NULL でなければ，
  /// flags が一致するトレースのうち，TclVarTrace が prev_obj
  /// であるものから探索を開始し，次に一致したトレースの TclVarTrace
  /// を返す．もし一致するトレースがそれ以上ない場合には NULL を返す．
  TclVarTrace*
  vartrace_info(const string& base,
		const string& idx,
		int flags,
		TclVarTrace* prev_obj) const;

  /// @}
  //////////////////////////////////////////////////////////////////////
  

  //////////////////////////////////////////////////////////////////////
  /// @name ユーティリティ関数
  /// @{

  /// @brief ファイル名中の '~' の展開
  /// @param[in] name 入力文字列
  /// @param[out] subst_name 置き換えられた文字列を格納する変数
  /// @retval true 置き換えが成功した
  /// @retval false 置き換えが失敗した．
  ///
  /// name をファイルのパス名と見なしての'~'展開を行ない，
  /// 結果を subst_name に入れる．
  bool
  tilde_subst(const string& name,
	      string& subst_name) const;
  
  /// @brief エラー出力関数
  /// @return エラーメッセージ文字列
  ///
  /// errno に基づいたエラーコードを Tcl変数 errorCode に入れ，
  /// エラーメッセージ文字列を返す．
  /// この関数ではエラーメッセージはインタープリタの結果には
  /// セットされない．
  string
  posix_error() const;

  /// @brief 入力ファイルのオープン
  /// @param[out] ifs 開いたファイルの入力ストリームをセットする変数
  /// @param[in] name ファイル名
  /// @retval true オープンに成功した．
  /// @retval false オープンに失敗した．
  ///
  /// ファイル名の '~' 置換を行なって入力用ファイルを開く．
  /// 失敗した場合にはメッセージをインタプリタにセットする．
  bool
  open_ifile(ifstream& ifs,
	     const string& name) const;

  /// @brief 出力ファイルのオープン
  /// @param[out] ofs 開いたファイルの出力ストリームをセットする変数
  /// @param[in] name ファイル名
  /// @retval true オープンに成功した．
  /// @retval false オープンに失敗した．
  ///
  /// ファイル名の '~' 置換を行なって出力用ファイルを開く．
  /// 失敗した場合にはメッセージをインタプリタにセットする．
  bool
  open_ofile(ofstream& ofs,
	     const string& name) const;

  /// @brief glob 形式のマッチング関数
  /// @param[in] str マッチング対象の文字列
  /// @param[in] pat glob 形式のパタン
  /// @retval 1 マッチが成功した．
  /// @retval 0 マッチが見つからなかった．
  ///
  /// Tcl コマンド string match と同様の処理を行なう．
  ///
  /// 実はインタプリタを用いないのでただの関数にできるのだが，
  /// 名前空間の問題もあるのでメンバ関数にしている．
  /// イヤなら Tcl の素の関数を呼べば良い．
  int
  string_match(const string& str,
	       const string& pat) const;

  /// @brief 正規表現のマッチング関数
  /// @param[in] str マッチング対象の文字列
  /// @param[in] pat 正規表現のパタン
  /// @retval 1 マッチが成功した．
  /// @retval 0 マッチが見つからなかった．
  /// @retval -1 パタンが正規表現でない．
  ///
  /// str が正規表現 pat にマッチするか調べる．
  ///
  /// パタンが正しくないときは -1 を返し，
  /// インタプリタにエラーメッセージをセットする．
  int
  regexp_match(const string& str,
	       const string& pat) const;

  /// @}
  //////////////////////////////////////////////////////////////////////

  
  /// @brief Tcl インタープリタを設定する．
  /// @param[in] interp Tcl インタープリタ
  void
  set_interp(Tcl_Interp* interp);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // インタープリタ
  Tcl_Interp* mInterp;

};

END_NAMESPACE_YM_TCLPP

#endif // YM_TCLPP_TCLBASE_H
