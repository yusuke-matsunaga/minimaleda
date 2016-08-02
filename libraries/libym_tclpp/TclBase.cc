
/// @file libym_tclpp/TclBase.cc
/// @brief TclBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TclBase.cc 1417 2008-05-06 18:12:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_tclpp/TclBase.h"
#include "ym_tclpp/TclObj.h"
#include "ym_tclpp/TclVarTrace.h"

#include "StrBuf.h"


BEGIN_NAMESPACE_YM_TCLPP

// コンストラクタ
// 最初は特定のインタープリタには結び付いていない．
TclBase::TclBase() :
  mInterp(NULL)
{
}

// デストラクタ
TclBase::~TclBase()
{
}
  
// インタープリタを得る．
// NULL の場合もあり得る．
Tcl_Interp*
TclBase::interp() const
{
  return mInterp;
}


//////////////////////////////////////////////////////////////////////
// スクリプトの評価関数
//////////////////////////////////////////////////////////////////////

// script を評価し，その完了コードを返す．
// 結果またはエラーメッセージをインタープリタにセットする．
int
TclBase::eval(const TclObj& script) const
{
  return Tcl_EvalObj(interp(), script.mPtr);
}
  
// グローバルレベルで script を評価し，その完了コードを返す．
// 結果またはエラーメッセージをインタープリタにセットする．
int
TclBase::global_eval(const TclObj& script) const
{
  return Tcl_GlobalEvalObj(interp(), script.mPtr);
}
  
// file という名のファイルを評価し，その完了コードを返す．
// 結果またはエラーメッセージをインタープリタにセットする．
int
TclBase::eval_file(const string& file) const
{
  StrBuf sptr(file);
  return Tcl_EvalFile(interp(), sptr);
}
  
// script を履歴リストにイベントとして登録し，flags が0ならば
// 評価する．flags が TCL_NO_EVALなら評価しない．完了コードを返す．
// 結果またはエラーメッセージをインタープリタにセットする．
int
TclBase::record_and_eval(const TclObj& script,
			 int flags) const
{
  return Tcl_RecordAndEvalObj(interp(), script.mPtr, flags);
}
  

//////////////////////////////////////////////////////////////////////
// コマンド操作関数
//////////////////////////////////////////////////////////////////////

// コマンドの登録に関しては，ClientData と Tcl_CmdProc,
// Tcl_CmdDeleteProc をひとまとめにしたクラス CmdObj
// を用いる．直接 Tcl_CreateCommand() を呼んでははいけない．

// コマンドを削除する．
int
TclBase::delete_command(const string& cmdName) const
{
  StrBuf sptr(cmdName);
  return Tcl_DeleteCommand(interp(), sptr);
}
  
// token を使ってコマンドを削除する．
int
TclBase::delete_command(Tcl_Command token) const
{
  return Tcl_DeleteCommandFromToken(interp(), token);
}
  
// コマンドに関する情報を得る．
// cmdName が登録されていれば info にその情報をセットし，0 を返す．
// 登録されていなければ 1 を返す．
// TCL_OK や TCL_ERROR ではないので注意．
int
TclBase::command_info(const string& cmd_name,
		      Tcl_CmdInfo* info) const
{
  StrBuf sptr(cmd_name);
  return Tcl_GetCommandInfo(interp(), sptr, info);
}
  
// コマンドに関する情報をセットする．
// cmdName が登録されていれば info の情報を cmdName の情報にコピーし，
// TCL_OK を返す．登録されていなければ TCL_ERROR を返す．
// この関数を使うことで，コマンド削除時に用いられる ClientData を
// CreateCommand() の引数の ClientData から変更することができる．
int
TclBase::set_command_info(const string& cmd_name,
			  Tcl_CmdInfo* info) const
{
  StrBuf sptr(cmd_name);
  return Tcl_SetCommandInfo(interp(), sptr, info);
}

// コマンド名を得る．
// token は CreateCommand() が返した値．
string
TclBase::command_name(Tcl_Command token) const
{
  CONST84 char* result = Tcl_GetCommandName(interp(), token);
  return string(result);
}
  

//////////////////////////////////////////////////////////////////////
// 結果にアクセスするための関数
//////////////////////////////////////////////////////////////////////

// 結果をセットする．
void
TclBase::set_result(const TclObj& obj) const
{
  Tcl_SetObjResult(interp(), obj.mPtr);
}

// 結果をクリアする．
void
TclBase::reset_result() const
{
  Tcl_ResetResult(interp());
}
  
// 結果を取り出す．
TclObj
TclBase::result() const
{
  Tcl_Obj* tmp = Tcl_GetObjResult(interp());
  return TclObj(tmp);
}
  

//////////////////////////////////////////////////////////////////////
// オブジェクトの型変換
//////////////////////////////////////////////////////////////////////

// オブジェクトを整数に変換し，ans に代入する．
// 正しい文字列なら TCL_OK を返し，エラーがあれば TCL_ERROR
// を返し，エラーメッセージをインタプリタにセットする．
int
TclBase::int_conv(const TclObj& obj,
		  ymint& ans) const
{
  return obj.get_int(ans, interp());
}
  
// オブジェクトを整数に変換し，ans に代入する．
// 正しい文字列なら TCL_OK を返し，エラーがあれば TCL_ERROR
// を返し，エラーメッセージをインタプリタにセットする．
int
TclBase::uint_conv(const TclObj& obj,
		   ymuint& ans) const
{
  return obj.get_uint(ans, interp());
}
  
// オブジェクトを long に変換し，ans に代入する．
// 正しい文字列なら TCL_OK を返し，エラーがあれば TCL_ERROR
// を返し，エラーメッセージをインタプリタにセットする．
int
TclBase::long_conv(const TclObj& obj,
		   ymlong& ans) const
{
  return obj.get_long(ans, interp());
}
  
// オブジェクトを unsigned long に変換し，ans に代入する．
// 正しい文字列なら TCL_OK を返し，エラーがあれば TCL_ERROR
// を返し，エラーメッセージをインタプリタにセットする．
int
TclBase::ulong_conv(const TclObj& obj,
		    ymulong& ans) const
{
  return obj.get_ulong(ans, interp());
}
  
// オブジェクトを double に変換し，ans に代入する．
// 正しい文字列なら TCL_OK を返し，エラーがあれば TCL_ERROR
// を返し，エラーメッセージをインタプリタにセットする．
int
TclBase::double_conv(const TclObj& obj,
		     double& ans) const
{
  return obj.get_double(ans, interp());
}
  
// オブジェクトをブール値に変換し，ans に代入する．
// 正しい文字列なら TCL_OK を返し，エラーがあれば TCL_ERROR
// を返し，エラーメッセージをインタプリタにセットする．
int
TclBase::bool_conv(const TclObj& obj,
		   bool& ans) const
{
  return obj.get_bool(ans, interp());
}
  
// オブジェクトをリストに変換し，objv に代入する．
// リストとして正しい文字列でなかった場合には TCL_ERROR を返す．
int
TclBase::list_conv(const TclObj& obj,
		   TclObjVector& objv) const
{
  return obj.list_elements(objv, interp());
}
int
TclBase::list_conv(const TclObj& obj,
		   TclObjList& objv) const
{
  return obj.list_elements(objv, interp());
}
  
// オブジェクトをリストに変換し，リストの要素数を length に入れる．
// リストとして正しい文字列でなかった場合には TCL_ERROR を返す．
int
TclBase::list_length(const TclObj& obj,
		     size_t& length) const
{
  return obj.list_length(length, interp());
}
  
// オブジェクトをリストに変換し，index で指された位置の要素を取り出す．
// リストとして正しい文字列でなかった場合には TCL_ERROR を返す．
int
TclBase::list_index(const TclObj& obj,
		    size_t index,
		    TclObj& elem) const
{
  return obj.list_index(index, elem, interp());
}


//////////////////////////////////////////////////////////////////////
// 式の評価
//////////////////////////////////////////////////////////////////////

// オブジェクトを式として評価し，その値を long int として
// ans に代入し，TCL_OK を返す．評価中にエラーが発生した場合には
// TCL_ERROR を返し，エラーメッセージをインタプリタにセットする．
int
TclBase::expr_long(const TclObj& obj,
		   long& ans) const
{
  return Tcl_ExprLongObj(interp(), obj.mPtr, &ans);
}
  
// オブジェクトを式として評価し，その値を double として
// ans に代入し，TCL_OK を返す．
// 評価中にエラーが発生した場合には TCL_ERROR を返し，
// エラーメッセージをインタプリタにセットする．
int
TclBase::expr_double(const TclObj& obj,
		     double& ans) const
{
  return Tcl_ExprDoubleObj(interp(), obj.mPtr, &ans);
}
  
// オブジェクトを式として評価し，その値をブール値として ans に代入し，
// TCL_OK を返す．評価中にエラーが発生した場合には TCL_ERROR
// を返し，エラーメッセージをインタプリタにセットする．
int
TclBase::expr_bool(const TclObj& obj,
		   bool& ans) const
{
  int tmp;
  int result = Tcl_ExprBooleanObj(interp(), obj.mPtr, &tmp);
  if ( tmp == 0 ) {
    ans = false;
  }
  else {
    ans = true;
  }
  return result;
}
  
// オブジェクトを式として評価し，結果を格納する．
int
TclBase::expr(const TclObj& obj,
	      TclObj& ans) const
{
  Tcl_Obj* tmp;
  int result = Tcl_ExprObj(interp(), obj.mPtr, &tmp);
  if ( result == TCL_OK ) {
    ans = tmp;
  }
  return result;
}
  
// double を文字列に変換する．精度は Tcl 変数 tcl_precision で
// 指定する．
string
TclBase::print_double(double val) const
{
  char buf[TCL_DOUBLE_SPACE];
  Tcl_PrintDouble(interp(), val, buf);
  return string(buf);
}


//////////////////////////////////////////////////////////////////////
// エラー情報の設定
//////////////////////////////////////////////////////////////////////

// message をTcl変数 errorInfo に入っているスタックトレースに追加する．
void
TclBase::add_errorinfo(const string& message) const
{
  StrBuf sptr(message);
  Tcl_AddErrorInfo(interp(), sptr);
}
  
// obj を Tcl 変数 errorCode にセットする．
void
TclBase::set_errorcode(const TclObj& obj) const
{
  Tcl_SetObjErrorCode(interp(), obj.mPtr);
}
  
// バックグラウンドエラーの報告
void
TclBase::background_error() const
{
  Tcl_BackgroundError(interp());
}


//////////////////////////////////////////////////////////////////////
// Tcl変数の操作
//////////////////////////////////////////////////////////////////////

// name を変数名とする変数に値として
// value を代入する．
// 変数が存在しなければ新たに作られる．変数に新たに代入された
// 値を返す．エラーの場合には NULL を持つオブジェクトを返す．
// flags については Tcl の本を参照
TclObj
TclBase::set_var(const TclObj& name,
		 const TclObj& value,
		 int flags) const
{
  Tcl_Obj* result =
    Tcl_ObjSetVar2(interp(), name.mPtr, NULL, value.mPtr, flags);
  return TclObj(result);
}

// base(idx) を変数名とする変数に値として
// value を代入する．
// 変数が存在しなければ新たに作られる．変数に新たに代入された
// 値を返す．エラーの場合には NULL を持つオブジェクトを返す．
// flags については Tcl の本を参照
TclObj
TclBase::set_var(const TclObj& base,
		 const TclObj& idx,
		 const TclObj& value,
		 int flags) const
{
  Tcl_Obj* result =
    Tcl_ObjSetVar2(interp(), base.mPtr, idx.mPtr, value.mPtr, flags);
  return TclObj(result);
}
  
// name を変数名とする変数の値を返す．
// エラーの場合には NULL を持つオブジェクトを返す．
// flags については Tcl の本を参照
TclObj
TclBase::var(const TclObj& name,
	     int flags) const
{
  Tcl_Obj* result = Tcl_ObjGetVar2(interp(), name.mPtr, NULL, flags);
  return TclObj(result);
}
  
// base(idx) を変数名とする変数の値を返す．
// エラーの場合には NULL を持つオブジェクト返す．
// flags については Tcl の本を参照
TclObj
TclBase::var(const TclObj& base,
	     const TclObj& idx,
	     int flags) const
{
  Tcl_Obj* result = Tcl_ObjGetVar2(interp(), base.mPtr, idx.mPtr, flags);
  return TclObj(result);
}
  
// name を変数名とする変数を削除し，TCL_OKを返す．
// 変数が存在しない場合には TCL_ERROR を返す．
// flags については Tcl の本を参照
int
TclBase::unset_var(const string& name,
		   int flags) const
{
  StrBuf sptr(name.c_str());
  int result = Tcl_UnsetVar(interp(), sptr, flags);
  return result;
}
  
// base(idx) を変数名とする変数を削除し，TCL_OKを返す．
// 変数が存在しない場合には TCL_ERROR を返す．
// flags については Tcl の本を参照
int
TclBase::unset_var(const string& base,
		   const string& idx,
		   int flags) const
{
  StrBuf sptr1(base);
  StrBuf sptr2(idx);
  int result = Tcl_UnsetVar2(interp(), sptr1, sptr2, flags);
  return result;
}

// Tcl変数 name とそのアドレスが addr であるC変数をリンクさせる．
// type に関しては Tcl の本を参照
int
TclBase::link_var(const string& name,
		  char* addr,
		  int type) const
{
  StrBuf sptr(name);
  int result = Tcl_LinkVar(interp(), sptr, addr, type);
  return result;
}
  
// Tcl変数 name に対するリンクを削除する．
void
TclBase::unlink_var(const string& name) const
{
  StrBuf sptr(name);
  Tcl_UnlinkVar(interp(), sptr);
}


//////////////////////////////////////////////////////////////////////
// Tcl変数の値の取得と型変換を組み合わせた関数
//////////////////////////////////////////////////////////////////////

// name という変数の値を整数に変換し，ans に代入する．
// 正しい文字列なら TCL_OK を返し，エラーがあれば TCL_ERROR
// を返し，エラーメッセージをインタプリタにセットする．
int
TclBase::int_from_var(const TclObj& name,
		      int flags,
		      int& ans) const
{
  TclObj obj = var(name, flags);
  if ( !obj.is_validptr() ) {
    return TCL_ERROR;
  }
  return obj.get_int(ans, interp());
}
  
// base(idx) という変数の値を整数に変換し，ans に代入する．
// 正しい文字列なら TCL_OK を返し，エラーがあれば TCL_ERROR
// を返し，エラーメッセージをインタプリタにセットする．
int
TclBase::int_from_var(const TclObj& base,
		      const TclObj& idx,
		      int flags,
		      int& ans) const
{
  TclObj obj = var(base, idx, flags);
  if ( !obj.is_validptr() ) {
    return TCL_ERROR;
  }
  return obj.get_int(ans, interp());
}
  
// name という変数の値を符号なし整数に変換し，ans に代入する．
// 正しい文字列なら TCL_OK を返し，エラーがあれば TCL_ERROR
// を返し，エラーメッセージをインタプリタにセットする．
int
TclBase::uint_from_var(const TclObj& name,
		       int flags,
		       unsigned int& ans) const
{
  TclObj obj = var(name, flags);
  if ( !obj.is_validptr() ) {
    return TCL_ERROR;
  }
  return obj.get_uint(ans, interp());
}

// base(idx) という変数の値を符号なし整数に変換し，ans に代入する．
// 正しい文字列なら TCL_OK を返し，エラーがあれば TCL_ERROR
// を返し，エラーメッセージをインタプリタにセットする．
int
TclBase::uint_from_var(const TclObj& base,
		       const TclObj& idx,
		       int flags,
		       unsigned int& ans) const
{
  TclObj obj = var(base, idx, flags);
  if ( !obj.is_validptr() ) {
    return TCL_ERROR;
  }
  return obj.get_uint(ans, interp());
}
  
// name という変数の値を long整数に変換し，ans に代入する．
// 正しい文字列なら TCL_OK を返し，エラーがあれば TCL_ERROR
// を返し，エラーメッセージをインタプリタにセットする．
int
TclBase::long_from_var(const TclObj& name,
		       int flags,
		       long& ans) const
{
  TclObj obj = var(name, flags);
  if ( !obj.is_validptr() ) {
    return TCL_ERROR;
  }
  return obj.get_long(ans, interp());
}
  
// base(idx) という変数の値を long整数に変換し，ans に代入する．
// 正しい文字列なら TCL_OK を返し，エラーがあれば TCL_ERROR
// を返し，エラーメッセージをインタプリタにセットする．
int
TclBase::long_from_var(const TclObj& base,
		       const TclObj& idx,
		       int flags,
		       long& ans) const
{
  TclObj obj = var(base, idx, flags);
  if ( !obj.is_validptr() ) {
    return TCL_ERROR;
  }
  return obj.get_long(ans, interp());
}
  
// name という変数の値を 符号なしlong整数に変換し，ans に代入する．
// 正しい文字列なら TCL_OK を返し，エラーがあれば TCL_ERROR
// を返し，エラーメッセージをインタプリタにセットする．
int
TclBase::ulong_from_var(const TclObj& name,
			int flags,
			unsigned long& ans) const
{
  TclObj obj = var(name, flags);
  if ( !obj.is_validptr() ) {
    return TCL_ERROR;
  }
  return obj.get_ulong(ans, interp());
}
  
// base(idx) という変数の値を 符号なしlong整数に変換し，ans に代入する．
// 正しい文字列なら TCL_OK を返し，エラーがあれば TCL_ERROR
// を返し，エラーメッセージをインタプリタにセットする．
int
TclBase::ulong_from_var(const TclObj& base,
			const TclObj& idx,
			int flags,
			unsigned long& ans) const
{
  TclObj obj = var(base, idx, flags);
  if ( !obj.is_validptr() ) {
    return TCL_ERROR;
  }
  return obj.get_ulong(ans, interp());
}
  
// name という変数の値を double に変換し，ans に代入する．
// 正しい文字列なら TCL_OK を返し，エラーがあれば TCL_ERROR
// を返し，エラーメッセージをインタプリタにセットする．
int
TclBase::double_from_var(const TclObj& name,
			 int flags,
			 double& ans) const
{
  TclObj obj = var(name, flags);
  if ( !obj.is_validptr() ) {
    return TCL_ERROR;
  }
  return obj.get_double(ans, interp());
}

// base(idx) という変数の値を double に変換し，ans に代入する．
// 正しい文字列なら TCL_OK を返し，エラーがあれば TCL_ERROR
// を返し，エラーメッセージをインタプリタにセットする．
int
TclBase::double_from_var(const TclObj& base,
			 const TclObj& idx,
			 int flags,
			 double& ans) const
{
  TclObj obj = var(base, idx, flags);
  if ( !obj.is_validptr() ) {
    return TCL_ERROR;
  }
  return obj.get_double(ans, interp());
}
  
// name という変数の値をブール値に変換し，ans に代入する．
// 正しい文字列なら TCL_OK を返し，エラーがあれば TCL_ERROR
// を返し，エラーメッセージをインタプリタにセットする．
int
TclBase::bool_from_var(const TclObj& name,
		       int flags,
		       bool& ans) const
{
  TclObj obj = var(name, flags);
  if ( !obj.is_validptr() ) {
    return TCL_ERROR;
  }
  return obj.get_bool(ans, interp());
}
  
// base(idx) という変数の値をブール値に変換し，ans に代入する．
// 正しい文字列なら TCL_OK を返し，エラーがあれば TCL_ERROR
// を返し，エラーメッセージをインタプリタにセットする．
int
TclBase::bool_from_var(const TclObj& base,
		       const TclObj& idx,
		       int flags,
		       bool& ans) const
{
  TclObj obj = var(base, idx, flags);
  if ( !obj.is_validptr() ) {
    return TCL_ERROR;
  }
  return obj.get_bool(ans, interp());
}
  
// name という変数の値をリストに変換し，objv に代入する．
// リストとして正しい文字列でなかった場合には TCL_ERROR を返す．
int
TclBase::list_from_var(const TclObj& name,
		       int flags,
		       TclObjVector& objv) const
{
  TclObj obj = var(name, flags);
  if ( !obj.is_validptr() ) {
    return TCL_ERROR;
  }
  return obj.list_elements(objv, interp());
}
int
TclBase::list_from_var(const TclObj& name,
		       int flags,
		       TclObjList& objv) const
{
  TclObj obj = var(name, flags);
  if ( !obj.is_validptr() ) {
    return TCL_ERROR;
  }
  return obj.list_elements(objv, interp());
}
  
// base(idx) という変数の値をリストに変換し，objv に代入する．
// リストとして正しい文字列でなかった場合には TCL_ERROR を返す．
int
TclBase::list_from_var(const TclObj& base,
		       const TclObj& idx,
		       int flags,
		       TclObjVector& objv) const
{
  TclObj obj = var(base, idx, flags);
  if ( !obj.is_validptr() ) {
    return TCL_ERROR;
  }
  return obj.list_elements(objv, interp());
}
int
TclBase::list_from_var(const TclObj& base,
		       const TclObj& idx,
		       int flags,
		       TclObjList& objv) const
{
  TclObj obj = var(base, idx, flags);
  if ( !obj.is_validptr() ) {
    return TCL_ERROR;
  }
  return obj.list_elements(objv, interp());
}


//////////////////////////////////////////////////////////////////////
// 変数トレースの生成／破壊に関しては Tcl の生のAPIを使わずに
// TclVarTrace を用いる．
//////////////////////////////////////////////////////////////////////

// prev_obj が NULL ならば変数 name に設定してあるトレースのうち，
// flags が一致する(ただし，flags には TCL_GLOBAL_ONLY だけが
// 使われる)最初のトレースの TclVarTrace を返す．NULL でなければ，
// flags が一致するトレースのうち，TclVarTrace が prev_obj
// であるものから探索を開始し，次に一致したトレースの TclVarTrace
// を返す．もし一致するトレースがそれ以上ない場合には NULL を返す．
TclVarTrace*
TclBase::vartrace_info(const string& name,
		       int flags,
		       TclVarTrace* prev_obj) const
{
  StrBuf sptr(name);
  ClientData result = Tcl_VarTraceInfo(interp(), sptr, flags,
				       TclVarTrace::vartrace_callback,
				       (ClientData) prev_obj);
  return (TclVarTrace*) result;
}
  
// prev_obj が NULL ならば変数 base(idx) に設定してあるトレースの
// うち，flags が一致する(ただし，flags には TCL_GLOBAL_ONLY だ
// けが使われる)最初のトレースの TclVarTrace を返す．NULL でなければ，
// flags が一致するトレースのうち，TclVarTrace が prev_obj
// であるものから探索を開始し，次に一致したトレースの TclVarTrace
// を返す．もし一致するトレースがそれ以上ない場合には NULL を返す．
TclVarTrace*
TclBase::vartrace_info(const string& base,
		       const string& idx,
		       int flags,
		       TclVarTrace* prev_obj) const
{
  StrBuf sptr1(base);
  StrBuf sptr2(idx);
  ClientData result = Tcl_VarTraceInfo2(interp(), sptr1, sptr2, flags,
					TclVarTrace::vartrace_callback,
					(ClientData) prev_obj);
  return (TclVarTrace*) result;
}


//////////////////////////////////////////////////////////////////////
// 以下は便利なユーティリティ関数
//////////////////////////////////////////////////////////////////////

// ファイル名の'~'展開を行ない，結果を subst_name に入れる．
// エラーが起きた場合にはインタープリタにエラーメッセージを
// 入れて false を返す．
bool
TclBase::tilde_subst(const string& name,
		     string& subst_name) const
{
  StrBuf sptr(name);
  Tcl_DString dstr;
  char* result = Tcl_TildeSubst(interp(), sptr, &dstr);
  if ( !result ) {
    // エラーが起こった．
    // Ousterhout の本の例題ではここで Tcl_DStringFree() を呼んでいないが
    // いいんだろうか？
    return false;
  }
  
  // string に内容をコピーする．
  subst_name = result;
  
  // これで心おきなく buffer を解放できる．
  Tcl_DStringFree(&dstr);
  
  // 正常終了
  return true;
}

// errno に基づいてエラーコードをTcl変数 errorCode に入れ，
// エラーメッセージ文字列を返す．
// この関数ではエラーメッセージはインタープリタの結果にはセットされない．
string
TclBase::posix_error() const
{
  CONST84 char* ans = Tcl_PosixError(interp());
  return string(ans);
}

// ファイル名の '~' 置換を行なって入力用ファイルを開く
// 成功した場合には ifs に開いたファイルをセットする．
// 失敗した場合にはメッセージをインタプリタにセット
// して false を返す．
bool
TclBase::open_ifile(ifstream& ifs,
		    const string& name) const
{
  // ファイル名の '~' 置換を行なう．
  string ex_name;
  bool stat = tilde_subst(name, ex_name);
  if ( !stat ) {
    // ファイル名文字列の中に誤り
    return false;
  }
  
  ifs.open(ex_name.c_str());
  if ( !ifs.is_open() ) {
    // 開けなかった．
    TclObj errmsg;
    errmsg << ex_name << ": " << posix_error();
    set_result(errmsg);
    return false;
  }
  
  return true;
}
  
// ファイル名の '~' 置換を行なって出力用ファイルを開く
// 成功した場合には ofs に開いたファイルをセットする．
// 失敗した場合にはメッセージをインタプリタにセット
// して false を返す．
bool
TclBase::open_ofile(ofstream& ofs,
		    const string& name) const
{
  // ファイル名の '~' 置換を行なう．
  string ex_name;
  bool stat = tilde_subst(name, ex_name);
  if ( !stat ) {
    // ファイル名文字列の中に誤り
    return false;
  }
  
  ofs.open(ex_name.c_str());
  if ( !ofs.is_open() ) {
    // 開けなかった．
    TclObj errmsg;
    errmsg << ex_name << ": " << posix_error();
    set_result(errmsg);
    return false;
  }
  
  return true;
}
  
// Tcl コマンド string match と同様の処理を行なう．
// いわゆる glob 形式のマッチング
// 実はインタプリタを用いないのでただの関数にできるのだが，
// 名前空間の問題もあるのでメンバ関数にしている．
// イヤなら Tcl の素の関数を呼べば良い．
// マッチした場合には 1 を，マッチしなかった場合には 0 を返す．
int
TclBase::string_match(const string& str,
		      const string& pat) const
{
  StrBuf sptr1(str);
  StrBuf sptr2(pat);
  return Tcl_StringMatch(sptr1, sptr2);
}
  
// string が正規表現 pat にマッチするか調べる．
// マッチした場合には 1 を，マッチしなかった場合には 0 を返す．
// パタンが正しくないときは -1 を返し，インタプリタにエラーメッセージ
// をセットする．
int
TclBase::regexp_match(const string& str,
		      const string& pat) const
{
  StrBuf sptr1(str);
  StrBuf sptr2(pat);
  return Tcl_RegExpMatch(interp(), sptr1, sptr2);
}

// インタープリタを設定する．
void
TclBase::set_interp(Tcl_Interp* interp)
{
  mInterp = interp;
}

END_NAMESPACE_YM_TCLPP
