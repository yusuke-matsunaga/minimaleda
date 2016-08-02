#ifndef YM_TCLPP_TCLOBJ_H
#define YM_TCLPP_TCLOBJ_H

/// @file ym_tclpp/TclObj.h
/// @brief TclObj のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TclObj.h 1417 2008-05-06 18:12:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_tclpp/tclpp_nsdef.h"


BEGIN_NAMESPACE_YM_TCLPP

//////////////////////////////////////////////////////////////////////
/// @class TclObj TclObj.h <ym_tclpp/TclObj.h>
/// @ingroup Tclpp
/// @brief Tcl_Obj への(スマート)ポインタ
///
/// @sa TclBase, TclObjVector, TclObjList
//////////////////////////////////////////////////////////////////////
class TclObj
{
  friend class TclBase;

public:

  //////////////////////////////////////////////////////////////////////
  /// @name コンストラクタ／デストラクタ／代入演算子
  /// /オブジェクトの生成
  /// @{

  /// @brief 内容を指定したコンストラクタ
  /// @param[in] obj Tcl_Obj の実体へのポインタ
  /// この時点で obj の参照回数は一つ増やされる．
  TclObj(Tcl_Obj* obj = NULL);
  
  /// @brief int 型の値をセットするコンストラクタ
  /// @param[in] value 値
  TclObj(int value);
  
  /// @brief unsigned int 型の値をセットするコンストラクタ
  /// @param[in] value 値
  TclObj(unsigned int value);
  
  /// @brief long 型の値をセットするコンストラクタ．
  /// @param[in] value 値
  TclObj(long value);
  
  /// @brief unsigned long 型の値をセットするコンストラクタ．
  /// @param[in] value 値
  TclObj(unsigned long value);
  
  /// @brief bool 型の値をセットするコンストラクタ．
  /// @param[in] value 値
  TclObj(bool value);
  
  /// @brief double 型の値をセットするコンストラクタ．
  /// @param[in] value 値
  TclObj(double value);
  
  /// @brief 文字列型の値をセットするコンストラクタ．
  /// @param[in] value 値
  TclObj(const string& value);
  
  /// @brief const char* の値をセットするコンストラクタ
  /// @param[in] value 値
  TclObj(const char* value);
  
  /// @brief リスト型の値をセットするコンストラクタ
  /// @param[in] objc 要素数
  /// @param[in] objv 要素のC型配列
  TclObj(size_t objc,
	 const TclObj objv[]);

  /// @brief リスト型の値をセットするコンストラクタ
  /// @param[in] objv 要素のベクタ
  TclObj(const TclObjVector& objv);
  
  /// @brief リスト型の値をセットするコンストラクタ
  /// @param[in] objv 要素のリスト
  TclObj(const TclObjList& objv);
  
  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  TclObj(const TclObj& src);
  
  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 自分自身
  const TclObj&
  operator=(const TclObj& src);
  
  /// @brief Tcl_Obj* からの代入演算子
  /// @param[in] obj コピー元のオブジェクト
  /// @return 自分自身
  const TclObj&
  operator=(Tcl_Obj* obj);
  
  /// @brief デストラクタ
  /// @note mPtr の参照回数は一つ減らされる．
  ~TclObj();
  
  /// @brief 新しいオブジェクト(型無し)を作りセットする．
  ///
  /// もしもすでにオブジェクトを持っていた場合には，
  /// そのオブジェクトをまっさらにする．
  void
  clear();
  
  /// @brief 現在のオブジェクトを捨ててポインタをNULLにする．
  ///
  /// といっても本当のTcl_Objの参照回数が1より大きければ
  /// 他のポインタが参照しているのでオブジェクトは破棄されない．
  void
  set_null();
  
  /// @brief 自分のオブジェクトの複製を作る．
  ///
  /// コピーコンストラクタの場合には同一のオブジェクトを指す．
  /// こちらは複製を作る点が異なるので注意
  TclObj
  duplicate() const;

  /// @}
  //////////////////////////////////////////////////////////////////////
  

  //////////////////////////////////////////////////////////////////////
  /// @name 状態を取得する関数
  /// @{

  /// @brief 妥当なポインタのチェック
  ///
  /// @return ポインタが NULL 以外の内容を持っているときに true を返す．
  bool
  is_validptr() const;

  /// @brief 共有オブジェクトのチェック
  /// @return 他のポインタからも指されていて共有されていたら true を返す．
  /// もちろん，mPtr が NULL なら false
  bool
  is_shared() const;

  /// @}
  //////////////////////////////////////////////////////////////////////

  
  //////////////////////////////////////////////////////////////////////
  /// @name int/long/bool/double/string の値をセットする関数
  /// @{

  /// @brief int 型の値をセットする．
  /// @param[in] value 値
  /// もしもポインタがNULLなら新しいオブジェクトを生成する．
  void
  set_int(int value);
  
  /// @brief unsigned int 型の値をセットする．
  /// @param[in] value 値
  /// もしもポインタがNULLなら新しいオブジェクトを生成する．
  void
  set_uint(unsigned int value);
  
  /// @brief long 型の値をセットする．
  /// @param[in] value 値
  /// もしもポインタがNULLなら新しいオブジェクトを生成する．
  void
  set_long(long value);
  
  /// @brief unsigned long 型の値をセットする．
  /// @param[in] value 値
  /// もしもポインタがNULLなら新しいオブジェクトを生成する．
  void
  set_ulong(unsigned long value);
  
  /// @brief bool 型の値をセットする．
  /// @param[in] value 値
  /// もしもポインタがNULLなら新しいオブジェクトを生成する．
  void
  set_bool(bool value);
  
  /// @brief double 型の値をセットする．
  /// @param[in] value 値
  /// もしもポインタがNULLなら新しいオブジェクトを生成する．
  void
  set_double(double value);
  
  /// @brief 文字列型の値をセットする．
  /// @param[in] value 値
  /// もしもポインタがNULLなら新しいオブジェクトを生成する．
  void
  set_string(const string& value);
  
  /// @brief 文字列型の値をセットする．
  /// @param[in] value 値
  /// もしもポインタがNULLなら新しいオブジェクトを生成する．
  void
  set_string(const char* value);

  /// @}
  //////////////////////////////////////////////////////////////////////
  
  
  //////////////////////////////////////////////////////////////////////
  /// @name 型変換をともなう代入演算子
  /// @{

  /// @brief int からの代入演算子
  /// @param[in] value 値
  /// @return 自分自身
  const TclObj&
  operator=(int value);
  
  /// @brief unsigned int からの代入演算子
  /// @param[in] value 値
  /// @return 自分自身
  const TclObj&
  operator=(unsigned int value);
  
  /// @brief long からの代入演算子
  /// @param[in] value 値
  /// @return 自分自身
  const TclObj&
  operator=(long value);
  
  /// @brief unsigned long からの代入演算子
  /// @param[in] value 値
  /// @return 自分自身
  const TclObj&
  operator=(unsigned long value);
  
  /// @brief long からの代入演算子
  /// @param[in] value 値
  /// @return 自分自身
  const TclObj&
  operator=(bool value);
  
  /// @brief double からの代入演算子
  /// @param[in] value 値
  /// @return 自分自身
  const TclObj&
  operator=(double value);
  
  /// @brief string からの代入演算子
  /// @param[in] value 値
  /// @return 自分自身
  const TclObj&
  operator=(const string& value);
  
  /// @brief const char* からの代入演算子
  /// @param[in] value 値
  /// @return 自分自身
  const TclObj&
  operator=(const char* value);

  /// @}
  //////////////////////////////////////////////////////////////////////
  
  
  //////////////////////////////////////////////////////////////////////
  /// @name int/long/bool/double への型変換
  /// @{

  /// @brief int 型への変換
  /// @param[in] value 変換した値を格納する変数
  /// @param[in] interp NULL でなければエラーメッセージ
  /// をセットするのに用いる．
  /// @retval TCL_OK 変換が成功した
  /// @retval TCL_ERROR エラーが起きた
  int
  get_int(int& value,
	  Tcl_Interp* interp = NULL) const;
  
  /// @brief unsigned int 型への変換
  /// @param[in] value 変換した値を格納する変数
  /// @param[in] interp NULL でなければエラーメッセージ
  /// をセットするのに用いる．
  /// @retval TCL_OK 変換が成功した
  /// @retval TCL_ERROR エラーが起きた
  int
  get_uint(unsigned int& value,
	   Tcl_Interp* interp = NULL) const;
  
  /// @brief long 型への変換
  /// @param[in] value 変換した値を格納する変数
  /// @param[in] interp NULL でなければエラーメッセージ
  /// をセットするのに用いる．
  /// @retval TCL_OK 変換が成功した
  /// @retval TCL_ERROR エラーが起きた
  int
  get_long(long& value,
	   Tcl_Interp* interp = NULL) const;
  
  /// @brief unsigned long 型への変換
  /// @param[in] value 変換した値を格納する変数
  /// @param[in] interp NULL でなければエラーメッセージ
  /// をセットするのに用いる．
  /// @retval TCL_OK 変換が成功した
  /// @retval TCL_ERROR エラーが起きた
  int
  get_ulong(unsigned long& value,
	    Tcl_Interp* interp = NULL) const;
  
  /// @brief bool 型への変換
  /// @param[in] value 変換した値を格納する変数
  /// @param[in] interp NULL でなければエラーメッセージ
  /// をセットするのに用いる．
  /// @retval TCL_OK 変換が成功した
  /// @retval TCL_ERROR エラーが起きた
  int
  get_bool(bool& value,
	   Tcl_Interp* interp = NULL) const;
  
  /// @brief double 型への変換
  /// @param[in] value 変換した値を格納する変数
  /// @param[in] interp NULL でなければエラーメッセージ
  /// をセットするのに用いる．
  /// @retval TCL_OK 変換が成功した
  /// @retval TCL_ERROR エラーが起きた
  int
  get_double(double& value,
	     Tcl_Interp* interp = NULL) const;

  /// @}
  //////////////////////////////////////////////////////////////////////
  
  
  //////////////////////////////////////////////////////////////////////
  /// @name 文字列操作関数
  /// @{

  /// @brief 文字列型への変換
  /// @param[out] lengthPtr NULL でなければ文字列の長さを格納する．
  /// @return 変換された文字列を返す．
  /// @note 他の型と異なりエラーとはならない．
  string
  get_string(int* lengthPtr = NULL) const;
  
  /// @brief string へのキャスト演算子
  /// @return 変換された文字列を返す．
  /// @note get_string() の別名
  operator
  string() const;
  
  /// @brief 文字列の追加
  /// @param[in] value 追加する文字列
  void
  append(const string& value);
  
  /// @brief << 演算子
  /// @param[in] value 追加する文字列 
  /// @note append() の別名
  TclObj&
  operator<<(const string& value);
  
  /// @brief 文字列領域の大きさをセットする．
  /// @warning 結構乱暴な関数なので使い方に気をつけること．
  /// @param[in] newLength 設定するサイズ
  void
  set_length(size_t newLength);
  
  /// @brief 複数のオブジェクトの連結
  ///
  /// 複数のオブジェクトをつなげたオブジェクトを作りセットする．
  /// @param[in] objc 要素数
  /// @param[in] objv 要素のC型配列
  void
  concat(size_t objc,
	 const TclObj objv[]);
  
  /// @brief 複数のオブジェクトの連結
  ///
  /// 複数のオブジェクトをつなげたオブジェクトを作りセットする．
  /// @param objv 要素のベクタ
  void
  concat(const TclObjVector& objv);
  
  /// @brief 複数のオブジェクトの連結
  ///
  /// 複数のオブジェクトをつなげたオブジェクトを作りセットする．
  /// @param objv 要素のリスト
  void
  concat(const TclObjList& objv);

  /// @}
  //////////////////////////////////////////////////////////////////////
  

  //////////////////////////////////////////////////////////////////////
  /// @name リスト操作関数
  /// @{

  /// @brief リストオブジェクトにリストを追加する．
  /// @param[in] elemList 追加するリストオブジェクト
  /// @param[in] interp NULL でなければエラーメッセージ
  /// をセットするのに用いる．
  /// @retval TCL_OK 変換が成功した
  /// @retval TCL_ERROR エラーが起きた
  int
  append_list(const TclObj& elemList,
	      Tcl_Interp* interp = NULL);
  
  /// @brief リストオブジェクトに一つの要素を追加する．
  /// @param[in] obj 追加するオブジェクト
  /// @param[in] interp NULL でなければエラーメッセージ
  /// をセットするのに用いる．
  /// @retval TCL_OK 変換が成功した
  /// @retval TCL_ERROR エラーが起きた
  int
  append_element(const TclObj& obj,
		 Tcl_Interp* interp = NULL);

  /// @brief リストオブジェクトの生成
  ///
  /// 複数のオブジェクトを要素とするリストオブジェクトを作りセットする．
  /// @param[in] objc 要素数
  /// @param[in] objv 要素のC型配列
  void
  set_list(size_t objc,
	   const TclObj objv[]);
  
  /// @brief リストオブジェクトの生成
  ///
  /// 複数のオブジェクトを要素とするリストオブジェクトを作りセットする．
  /// @param[in] objv 要素のベクタ
  void
  set_list(const TclObjVector& objv);
  
  /// @brief リストオブジェクトの生成
  ///
  /// 複数のオブジェクトを要素とするリストオブジェクトを作りセットする．
  /// @param[in] objv 要素のリスト
  /// 複数のオブジェクトを要素とするリストオブジェクトを作りセットする．
  void
  set_list(const TclObjList& objv);
  
  /// @brief リストオブジェクトの要素の取得
  ///
  /// リストオブジェクトの要素を objv に格納する．
  /// 上の set_list() の逆関数
  /// @param[out] objv 取り出された要素を格納するベクタ
  /// @param[in] interp NULL でなければエラーメッセージ
  /// をセットするのに用いる．
  /// @retval TCL_OK 変換が成功した
  /// @retval TCL_ERROR エラーが起きた (たぶんリスト型ではなかった)
  int
  list_elements(TclObjVector& objv,
		Tcl_Interp* interp = NULL) const;
  
  /// @brief リストオブジェクトの要素の取得
  ///
  /// リストオブジェクトの要素を objv に格納する．
  /// 上の SetList() の逆関数
  /// @param[out] objv 取り出された要素を格納するリスト
  /// @param[in] interp NULL でなければエラーメッセージ
  /// をセットするのに用いる．
  /// @retval TCL_OK 変換が成功した
  /// @retval TCL_ERROR エラーが起きた (たぶんリスト型ではなかった)
  int
  list_elements(TclObjList& objv,
		Tcl_Interp* interp = NULL) const;

  /// @brief リストオブジェクトの要素数の取得
  /// @param[out] length 要素数を格納する変数
  /// @param[in] interp NULL でなければエラーメッセージ
  /// をセットするのに用いる．
  /// @retval TCL_OK 変換が成功した
  /// @retval TCL_ERROR エラーが起きた (たぶんリスト型ではなかった)
  int
  list_length(size_t& length,
	      Tcl_Interp* interp = NULL) const;

  /// @brief リストオブジェクトの要素の取得
  /// @param[in] index 取り出す要素の位置 (最初の位置は 0)
  /// @param[out] obj index で指された位置の要素を格納する変数
  /// @param[in] interp NULL でなければエラーメッセージ
  /// をセットするのに用いる．
  /// @retval TCL_OK 変換が成功した
  /// @retval TCL_ERROR エラーが起きた
  /// もしも範囲外の場合には NULL ポインタがセットされる．
  int
  list_index(size_t index,
	     TclObj& obj,
	     Tcl_Interp* interp = NULL) const;

  /// @brief リストの部分置換
  /// @param[in] first 置き換え対象の最初の位置
  /// @param[in] count 置き換え対象の要素数
  /// @param[in] objc 置き換える新しいリストの要素数
  /// @param[in] objv 置き換える新しいリストを表すC型配列
  /// @param[in] interp NULL でなければエラーメッセージ
  /// をセットするのに用いる．
  /// @retval TCL_OK 変換が成功した
  /// @retval TCL_ERROR エラーが起きた
  int
  replace_list(size_t first,
	       size_t count,
	       size_t objc,
	       const TclObj objv[],
	       Tcl_Interp* interp = NULL);
  
  /// @brief リストの部分置換
  /// @param[in] first 置き換え対象の最初の位置
  /// @param[in] count 置き換え対象の要素数
  /// @param[in] objv 置き換える新しいリストを表すベクタ
  /// @param[in] interp NULL でなければエラーメッセージ
  /// をセットするのに用いる．
  /// @retval TCL_OK 変換が成功した
  /// @retval TCL_ERROR エラーが起きた
  int
  replace_list(size_t first,
	       size_t count,
	       const TclObjVector& objv,
	       Tcl_Interp* interp = NULL);
  
  /// @brief リストの部分置換
  /// @param[in] first 置き換え対象の最初の位置
  /// @param[in] count 置き換え対象の要素数
  /// @param[in] objv 置き換える新しいリストを表すリスト
  /// @param[in] interp NULL でなければエラーメッセージ
  /// をセットするのに用いる．
  /// @retval TCL_OK 変換が成功した
  /// @retval TCL_ERROR エラーが起きた
  int
  replace_list(size_t first,
	       size_t count,
	       const TclObjList& objv,
	       Tcl_Interp* interp = NULL);

  /// @}
  //////////////////////////////////////////////////////////////////////

  
  //////////////////////////////////////////////////////////////////////
  /// @name 拡張型に対するアクセス
  /// @{

  /// @brief type で示された型に変換する．
  /// @param[in] type 変換先の型
  /// @param[in] interp NULL でなければエラーメッセージ
  /// をセットするのに用いる．
  /// @retval TCL_OK 変換が成功した
  /// @retval TCL_ERROR エラーが起きた
  /// 内部の型がすでに type になっていれば何もしない．
  /// 場合によっては内部表現を更新するが意味的には内容を変えないので
  /// const としている
  int
  convert_to_type(Tcl_ObjType* type,
		  Tcl_Interp* interp = NULL) const;
  
  /// @brief ポインタの中身を取り出す．
  /// @warning 通常は高位レベルメンバ関数を呼ぶようにして生の
  /// ポインタは極力使わないようにするのが賢明
  const Tcl_Obj*
  ptr() const { return mPtr; }

  /// @brief ポインタの中身を取り出す．
  /// @warning 通常は高位レベルメンバ関数を呼ぶようにして生の
  /// ポインタは極力使わないようにするのが賢明
  Tcl_Obj*
  ptr() { return mPtr; }

  /// @}
  //////////////////////////////////////////////////////////////////////
  

protected:

  /// @brief mPtr の参照回数を一つ増やす．
  void
  inc();
  
  /// @brief mPtr の参照回数を一つ減らす．
  void
  dec();
  
  /// @brief 文字列表現を無効化(invalidate)する．
  void
  invalidate_stringrep();
  
  /// @brief mPtr に新しいポインタをセットする．
  /// @param[in] obj セットする新しいオブジェクトへのポインタ
  /// obj の参照回数が増やされる．
  /// @warning obj が NULL でないことが分かっているときだけ
  /// この関数を使うこと．
  void
  set_ptr(Tcl_Obj* obj);
  
  /// @brief mPtr に対して修正を行なう時の前処理
  ///
  /// mPtr が NULL ポインタなら新規にオブジェクトを生成し，
  /// 共有されていたら複製を作る．
  void
  instanciate();
  

private:

  // const TclObj objv[] から vector<Tcl_Obj*> をつくる共通関数
  void
  set_array(size_t objc,
	    const TclObj objv[],
	    vector<Tcl_Obj*>& array);
  
  // TclObjVector から vector<Tcl_Obj*> をつくる共通関数
  void
  set_array(const TclObjVector& src,
	    vector<Tcl_Obj*>& array);
  
  // TclObjList から vector<Tcl_Obj*> をつくる共通関数
  void
  set_array(const TclObjList& src,
	    vector<Tcl_Obj*>& array);
  
  // リストを作るための共通関数
  void
  set_list_sub(vector<Tcl_Obj*>& array);
  
  // リストの部分を置き換えるための共通関数
  int
  replace_list_sub(size_t first,
		   size_t count,
		   vector<Tcl_Obj*>& array,
		   Tcl_Interp* interp);
  
private:
  //////////////////////////////////////////////////////////////////////
  // メンバ変数
  //////////////////////////////////////////////////////////////////////

  /// 本当の Tcl_Obj へのポインタ
  Tcl_Obj* mPtr;

};

END_NAMESPACE_YM_TCLPP

#endif // YM_TCLPP_TCLOBJ_H
