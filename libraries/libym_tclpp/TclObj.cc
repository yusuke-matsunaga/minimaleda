
/// @file libym_tclpp/TclObj.cc
/// @brief TclObj の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TclObj.cc 997 2007-09-07 09:58:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_tclpp/TclObj.h"

#include "StrBuf.h"


BEGIN_NAMESPACE_YM_TCLPP

// コンストラクタ
TclObj::TclObj(Tcl_Obj* obj) :
  mPtr(obj)
{
  inc();
}
  
// int 型の値をセットするコンストラクタ
TclObj::TclObj(int value)
{
  set_ptr(Tcl_NewIntObj(value));
}
  
// unsigned int 型の値をセットするコンストラクタ
TclObj::TclObj(unsigned int value)
{
  set_ptr(Tcl_NewIntObj(value));
}
  
// long 型の値をセットするコンストラクタ．
TclObj::TclObj(long value)
{
  set_ptr(Tcl_NewLongObj(value));
}

// unsigned long 型の値をセットするコンストラクタ．
TclObj::TclObj(unsigned long value)
{
  set_ptr(Tcl_NewLongObj(value));
}
  
// bool 型の値をセットするコンストラクタ．
TclObj::TclObj(bool value)
{
  set_ptr(Tcl_NewBooleanObj(static_cast<int>(value)));
}
  
// double 型の値をセットするコンストラクタ．
TclObj::TclObj(double value)
{
  set_ptr(Tcl_NewDoubleObj(value));
}
  
// 文字列型の値をセットするコンストラクタ．
TclObj::TclObj(const string& value)
{
  StrBuf sptr(value);
  set_ptr(Tcl_NewStringObj(sptr, -1));
}
  
// const char* の値をセットするコンストラクタ
TclObj::TclObj(const char* value)
{
  StrBuf sptr(value);
  set_ptr(Tcl_NewStringObj(sptr, -1));
}
  
// リスト型の値をセットするコンストラクタ
TclObj::TclObj(size_t objc,
	       const TclObj objv[])
{
  vector<Tcl_Obj*> tmp_array;
  set_array(objc, objv, tmp_array);
  set_ptr(Tcl_NewListObj(objc, &(tmp_array.front())));
}

TclObj::TclObj(const TclObjVector& objv)
{
  vector<Tcl_Obj*> tmp_array;
  set_array(objv, tmp_array);
  set_ptr(Tcl_NewListObj(tmp_array.size(), &(tmp_array.front())));
}

TclObj::TclObj(const TclObjList& objv)
{
  vector<Tcl_Obj*> tmp_array;
  set_array(objv, tmp_array);
  set_ptr(Tcl_NewListObj(tmp_array.size(), &(tmp_array.front())));
}
  
// コピーコンストラクタ
TclObj::TclObj(const TclObj& src) :
  mPtr(src.mPtr)
{
  inc();
}
  
// 代入演算子
const TclObj&
TclObj::operator=(const TclObj& src)
{
  if ( mPtr != src.mPtr ) {
    // 通常は this != &src で比べるが，スマートポインタの場合，
    // オブジェクトそのものよりも参照が一緒かどうかで比べれば良い．
    dec();
    mPtr = src.mPtr;
    inc();
  }
  return *this;
}

// Tcl_Obj* からの代入演算子
const TclObj&
TclObj::operator=(Tcl_Obj* obj)
{
  if ( mPtr != obj ) {
    dec();
    mPtr = obj;
    inc();
  }
  return *this;
}
  
// デストラクタ
// mPtr の参照回数は一つ減らされる．
TclObj::~TclObj()
{
  dec();
}
  
// 新しいオブジェクト(型無し)を作りセットする．
// もしもすでにオブジェクトを持っていた場合には，
// そのオブジェクトをまっさらにする．
void
TclObj::clear()
{
  if ( !mPtr ) {
    set_ptr(Tcl_NewObj());
  }
  else {
    // 今はこれくらいしか思い付かない．
    // 本当は tcl の方の仕様が悪い．
    set_string("");
  }
}

// 現在のオブジェクトを捨ててポインタをNULLにする．
// といっても本当のTcl_Objの参照回数が1より大きければ
// 他のポインタが参照しているのでオブジェクトは破棄されない．
void
TclObj::set_null()
{
  if ( mPtr ) {
    Tcl_DecrRefCount(mPtr);
    mPtr = NULL;
  }
}

// 自分のオブジェクトの複製を作る．
// コピーコンストラクタの場合には同一のオブジェクトを指す．
// こちらは複製を作る点が異なるので注意
TclObj
TclObj::duplicate() const
{
  Tcl_Obj* new_obj = Tcl_DuplicateObj(mPtr);
  return TclObj(new_obj);
}

// int 型の値をセットする．
// もしもポインタがNULLなら新しいオブジェクトを生成する．
void
TclObj::set_int(int value)
{
  if ( !mPtr ) {
    set_ptr(Tcl_NewIntObj(value));
  }
  else if ( Tcl_IsShared(mPtr) ) {
    // 共有されているときは新しいオブジェクトを作る．
    Tcl_DecrRefCount(mPtr);
    set_ptr(Tcl_NewIntObj(value));
  }
  else {
    Tcl_SetIntObj(mPtr, value);
  }
}

// unsigned int 型の値をセットする．
// もしもポインタがNULLなら新しいオブジェクトを生成する．
void
TclObj::set_uint(unsigned int value)
{
  set_int(static_cast<int>(value));
}

// long 型の値をセットする．
// もしもポインタがNULLなら新しいオブジェクトを生成する．
void
TclObj::set_long(long value)
{
  if ( !mPtr ) {
    set_ptr(Tcl_NewLongObj(value));
  }
  else if ( Tcl_IsShared(mPtr) ) {
    Tcl_DecrRefCount(mPtr);
    set_ptr(Tcl_NewLongObj(value));
  }
  else {
    Tcl_SetLongObj(mPtr, value);
  }
}
  
// unsigned long 型の値をセットする．
// もしもポインタがNULLなら新しいオブジェクトを生成する．
void
TclObj::set_ulong(unsigned long value)
{
  set_long(static_cast<long>(value));
}
  
// bool 型の値をセットする．
// もしもポインタがNULLなら新しいオブジェクトを生成する．
void
TclObj::set_bool(bool value)
{
  if ( !mPtr ) {
    set_ptr(Tcl_NewBooleanObj(static_cast<int>(value)));
  }
  else if ( Tcl_IsShared(mPtr) ) {
    Tcl_DecrRefCount(mPtr);
    set_ptr(Tcl_NewBooleanObj(static_cast<int>(value)));
  }
  else {
    Tcl_SetBooleanObj(mPtr, static_cast<int>(value));
  }
}
  
// double 型の値をセットする．
// もしもポインタがNULLなら新しいオブジェクトを生成する．
void
TclObj::set_double(double value)
{
  if ( !mPtr ) {
    set_ptr(Tcl_NewDoubleObj(value));
  }
  else if ( Tcl_IsShared(mPtr) ) {
    Tcl_DecrRefCount(mPtr);
    set_ptr(Tcl_NewDoubleObj(value));
  }
  else {
    Tcl_SetDoubleObj(mPtr, value);
  }
}

// 文字列型の値をセットする．
// もしもポインタがNULLなら新しいオブジェクトを生成する．
void
TclObj::set_string(const string& value)
{
  StrBuf sptr(value);
  if ( !mPtr ) {
    set_ptr(Tcl_NewStringObj(sptr, -1));
  }
  else if ( Tcl_IsShared(mPtr) ) {
    Tcl_DecrRefCount(mPtr);
    set_ptr(Tcl_NewStringObj(sptr, -1));
  }
  else {
    Tcl_SetStringObj(mPtr, sptr, -1);
  }
}
void
TclObj::set_string(const char* value)
{
  StrBuf sptr(value);
  if ( !mPtr ) {
    set_ptr(Tcl_NewStringObj(sptr, -1));
  }
  else if ( Tcl_IsShared(mPtr) ) {
    Tcl_DecrRefCount(mPtr);
    set_ptr(Tcl_NewStringObj(sptr, -1));
  }
  else {
    Tcl_SetStringObj(mPtr, sptr, -1);
  }
}
  
// set_int() を利用した int からの代入演算子
const TclObj&
TclObj::operator=(int value)
{
  set_int(value);
  return *this;
}
  
// set_uint() を利用した unsigned int からの代入演算子
const TclObj&
TclObj::operator=(unsigned int value)
{
  set_uint(value);
  return *this;
}
  
// set_long() を利用した long からの代入演算子
const TclObj&
TclObj::operator=(long value)
{
  set_long(value);
  return *this;
}
  
// set_ulong() を利用した unsigned long からの代入演算子
const TclObj&
TclObj::operator=(unsigned long value)
{
  set_ulong(value);
  return *this;
}

// set_bool() を利用した long からの代入演算子
const TclObj&
TclObj::operator=(bool value)
{
  set_bool(value);
  return *this;
}
  
// set_double() を利用した long からの代入演算子
const TclObj&
TclObj::operator=(double value)
{
  set_double(value);
  return *this;
}
  
// set_string() を利用した const hf_CString& からの代入演算子
const TclObj&
TclObj::operator=(const string& value)
{
  set_string(value);
  return *this;
}
  
// const char* からの代入演算子
const TclObj&
TclObj::operator=(const char* str)
{
  set_string(str);
  return *this;
}
  
// 文字列を追加する．
void
TclObj::append(const string& value)
{
  instanciate();
  if ( value.c_str() ) {
    StrBuf p(value);
    Tcl_AppendToObj(mPtr, p, -1);
  }
  else {
    Tcl_AppendToObj(mPtr, "<null>", -1);
  }
}
  
// 文字列領域の大きさをセットする．
// 結構乱暴な関数なので使い方に気をつけること．
void
TclObj::set_length(size_t newLength)
{
  instanciate();
  Tcl_SetObjLength(mPtr, newLength);
}

// 複数のオブジェクトをつなげたオブジェクトを作りセットする．
void
TclObj::concat(size_t objc,
	       const TclObj objv[])
{
  vector<Tcl_Obj*> tmp_array;
  set_array(objc, objv, tmp_array);
  dec();
  set_ptr(Tcl_ConcatObj(objc, &(tmp_array.front())));
}

void
TclObj::concat(const TclObjVector& objv)
{
  vector<Tcl_Obj*> tmp_array;
  set_array(objv, tmp_array);
  dec();
  set_ptr(Tcl_ConcatObj(tmp_array.size(), &(tmp_array.front())));
}

void
TclObj::concat(const TclObjList& objv)
{
  vector<Tcl_Obj*> tmp_array;
  set_array(objv, tmp_array);
  dec();
  set_ptr(Tcl_ConcatObj(tmp_array.size(), &(tmp_array.front())));
}

// リストオブジェクトにリストを追加する．
// もしも interp が NULL でなければエラーメッセージをセットするのに
// 用いる．
int
TclObj::append_list(const TclObj& elemList,
		    Tcl_Interp* interp)
{
  instanciate();
  int code = Tcl_ListObjAppendList(interp, mPtr, elemList.mPtr);
  return code;
}
  
// リストオブジェクトに一つの要素を追加する．
// もしも interp が NULL でなければエラーメッセージをセットするのに
// 用いる．
int
TclObj::append_element(const TclObj& obj,
		       Tcl_Interp* interp)
{
  instanciate();
  int code = Tcl_ListObjAppendElement(interp, mPtr, obj.mPtr);
  return code;
}

// 複数のオブジェクトを要素とするリストオブジェクトを作りセットする．
// もしもポインタがNULLなら新しいオブジェクトを生成する．
void
TclObj::set_list(size_t objc,
		 const TclObj objv[])
{
  vector<Tcl_Obj*> tmp_array;
  set_array(objc, objv, tmp_array);
  set_list_sub(tmp_array);
}

void
TclObj::set_list(const TclObjVector& objv)
{
  vector<Tcl_Obj*> tmp_array;
  set_array(objv, tmp_array);
  set_list_sub(tmp_array);
}

void
TclObj::set_list(const TclObjList& objv)
{
  vector<Tcl_Obj*> tmp_array;
  set_array(objv, tmp_array);
  set_list_sub(tmp_array);
}

// リストオブジェクトの要素を objv に格納する．上の set_list() の逆関数
// もしも interp が NULL でなければエラーメッセージをセットするのに
// 用いる．
int
TclObj::list_elements(TclObjVector& objv,
		      Tcl_Interp* interp) const
{
  Tcl_Obj** tmp_array;
  int objc;
  int code = Tcl_ListObjGetElements(interp, mPtr, &objc, &tmp_array);
  if ( code == TCL_OK ) {
    objv.erase(objv.begin(), objv.end());
    objv.reserve(objc);
    for (int i = 0; i < objc; i ++) {
      // ここで自動的に参照回数がインクリメントされる．
      objv.push_back(tmp_array[i]);
    }
  }
  return code;
}
    
int
TclObj::list_elements(TclObjList& objv,
		      Tcl_Interp* interp) const
{
  Tcl_Obj** tmp_array;
  int objc;
  int code = Tcl_ListObjGetElements(interp, mPtr, &objc, &tmp_array);
  if ( code == TCL_OK ) {
    objv.erase(objv.begin(), objv.end());
    for (int i = 0; i < objc; i ++) {
      // ここで自動的に参照回数がインクリメントされる．
      objv.push_back(tmp_array[i]);
    }
  }
  return code;
}
  
// index で指された位置の要素を取り出す．
// もしも範囲外の場合には NULL ポインタがセットされる．
// もしも interp が NULL でなければエラーメッセージをセットするのに
// 用いる．
int
TclObj::list_index(size_t index,
		   TclObj& obj,
		   Tcl_Interp* interp) const
{
  Tcl_Obj* tmp;
  int code = Tcl_ListObjIndex(interp, mPtr, index, &tmp);
  if ( code == TCL_OK ) {
    obj = tmp;
  }
  return code;
}

// first の位置から count 個の要素を削除し，その位置に代りに
// objc 個の objv[] の要素を挿入する．
int
TclObj::replace_list(size_t first,
		     size_t count,
		     size_t objc,
		     const TclObj objv[],
		     Tcl_Interp* interp)
{
  vector<Tcl_Obj*> tmp_array;
  set_array(objc, objv, tmp_array);
  int code = replace_list_sub(first, count, tmp_array, interp);
  return code;
}

// first の位置から count 個の要素を削除し，その位置に代りに
// objv の要素を挿入する．
int
TclObj::replace_list(size_t first,
		     size_t count,
		     const TclObjVector& objv,
		     Tcl_Interp* interp)
{
  vector<Tcl_Obj*> tmp_array;
  set_array(objv, tmp_array);
  int code = replace_list_sub(first, count, tmp_array, interp);
  return code;
}

int
TclObj::replace_list(size_t first,
		     size_t count,
		     const TclObjList& objv,
		     Tcl_Interp* interp)
{
  vector<Tcl_Obj*> tmp_array;
  set_array(objv, tmp_array);
  int code = replace_list_sub(first, count, tmp_array, interp);
  return code;
}

// type で示された型に変換する．
// typePtr が type になっていれば何もしない．
// エラーの場合にはインタプリタにメッセージをセットする．
// internalRep を更新するが意味的には内容を変えないので const とする．
int
TclObj::convert_to_type(Tcl_ObjType* type,
			Tcl_Interp* interp) const
{
  if ( mPtr->typePtr != type ) {
    int code = Tcl_ConvertToType(interp, mPtr, type);
    return code;
  }
  return TCL_OK;
}
  
// mPtr に対して修正を行なう時の前処理
// NULL ポインタなら新規に生成し，
// 共有されていたら複製を作る．
void
TclObj::instanciate()
{
  if ( !mPtr ) {
    set_ptr(Tcl_NewObj());
  }
  else if ( Tcl_IsShared(mPtr) ) {
    Tcl_Obj* dup_obj = Tcl_DuplicateObj(mPtr);
    Tcl_DecrRefCount(mPtr);
    set_ptr(dup_obj);
  }
}

// Tcl のリスト処理関数は Tcl_Obj** を引数としてとるが，この形の配列
// をそのまま外に見せると参照回数ポインタのメインテナンスが難しいので
// TclObj [] もしくは ObjVector, ObjList を使うような
// インターフェイスにしている．
// そのために以下のようなちょっとめんどくさい変換が必要になる．
    
// const TclObj objv[] から vector<Tcl_Obj*> をつくる共通関数
void
TclObj::set_array(size_t objc,
		  const TclObj objv[],
		  vector<Tcl_Obj*>& array)
{
  array.erase(array.begin(), array.end());
  array.reserve(objc);
  for (size_t i = 0; i < objc; i ++) {
    array.push_back(objv[i].mPtr);
  }
}
    
// TclObjVector から vector<Tcl_Obj*> をつくる共通関数
void
TclObj::set_array(const TclObjVector& src,
		  vector<Tcl_Obj*>& array)
{
  array.erase(array.begin(), array.end());
  array.reserve(src.size());
  for (TclObjVector::const_iterator vit = src.begin();
       vit != src.end(); ++ vit) {
    const TclObj& obj = *vit;
    array.push_back(obj.mPtr);
  }
}
    
// TclObjList から vector<Tcl_Obj*> をつくる共通関数
void
TclObj::set_array(const TclObjList& src,
		  vector<Tcl_Obj*>& array)
{
  array.erase(array.begin(), array.end());
  array.reserve(src.size());
  for (TclObjList::const_iterator lit = src.begin();
       lit != src.end(); ++ lit) {
    const TclObj& obj = *lit;
    array.push_back(obj.mPtr);
  }
}
  
// リストを作るための共通関数
void
TclObj::set_list_sub(vector<Tcl_Obj*>& array)
{
  size_t objc = array.size();
  Tcl_Obj** begin = &(array.front());
  if ( !mPtr ) {
    set_ptr(Tcl_NewListObj(objc, begin));
  }
  else if ( Tcl_IsShared(mPtr) ) {
    Tcl_DecrRefCount(mPtr);
    set_ptr(Tcl_NewListObj(objc, begin));
  }
  else {
    Tcl_SetListObj(mPtr, objc, begin);
  }
}

// リストの部分を置き換えるための共通関数
int
TclObj::replace_list_sub(size_t first,
			 size_t count,
			 vector<Tcl_Obj*>& array,
			 Tcl_Interp* interp)
{
  instanciate();
  size_t objc = array.size();
  Tcl_Obj** begin = &(array.front());
  int code = Tcl_ListObjReplace(interp, mPtr, first, count, objc, begin);
  return code;
}

// ポインタが NULL 以外の内容を持っているときに true を返す．
bool
TclObj::is_validptr() const
{
  if ( mPtr ) {
    return true;
  }
  else {
    return false;
  }
}
  
// 他のポインタからも指されていて共有されていたら true を返す．
// もちろん，mPtr が NULL なら false
bool
TclObj::is_shared() const
{
  if ( mPtr && Tcl_IsShared(mPtr) ) {
    return true;
  }
  else {
    return false;
  }
}

// int 型の値を得る．
// もしも interp が NULL でなければエラーメッセージをセットするのに
// 用いる．
int
TclObj::get_int(int& value,
		Tcl_Interp* interp) const
{
  return Tcl_GetIntFromObj(interp, mPtr, &value);
}
  
// unsigned int 型の値を得る．
// もしも interp が NULL でなければエラーメッセージをセットするのに
// 用いる．
int
TclObj::get_uint(unsigned int& value,
		 Tcl_Interp* interp) const
{
  return Tcl_GetIntFromObj(interp, mPtr, &((int&)value));
}
  
// long 型の値を得る．
// もしも interp が NULL でなければエラーメッセージをセットするのに
// 用いる．
int
TclObj::get_long(long& value,
		 Tcl_Interp* interp) const
{
  return Tcl_GetLongFromObj(interp, mPtr, &value);
}
  
// unsigned long 型の値を得る．
// もしも interp が NULL でなければエラーメッセージをセットするのに
// 用いる．
int
TclObj::get_ulong(unsigned long& value,
		  Tcl_Interp* interp) const
{
  return Tcl_GetLongFromObj(interp, mPtr, &((long&)value));
}
  
// bool 型の値を得る．
// もしも interp が NULL でなければエラーメッセージをセットするのに
// 用いる．
int
TclObj::get_bool(bool& value,
		 Tcl_Interp* interp) const
{
  int tmp;
  int code = Tcl_GetBooleanFromObj(interp, mPtr, &tmp);
  if (tmp == 0) {
    value = false;
  }
  else {
    value = true;
  }
  return code;
}

// double 型の値を得る．
// もしも interp が NULL でなければエラーメッセージをセットするのに
// 用いる．
int
TclObj::get_double(double& value,
		   Tcl_Interp* interp) const
{
  return Tcl_GetDoubleFromObj(interp, mPtr, &value);
}

string
TclObj::get_string(int* lengthPtr) const
{
  return Tcl_GetStringFromObj(mPtr, lengthPtr);
}
  
// string へのキャスト演算子
// get_string() の別名
TclObj::operator string() const
{
  return get_string();
}
  
// << 演算子
// append() の別名
TclObj&
TclObj::operator<<(const string& value)
{
  append(value);
  return *this;
}
  
// リストの要素数を得る．
// もしも interp が NULL でなければエラーメッセージをセットするのに
// 用いる．
int
TclObj::list_length(size_t& length,
		    Tcl_Interp* interp) const
{
  return Tcl_ListObjLength(interp, mPtr, &((int&)length));
}

// mPtr の参照回数を一つ増やす．
void
TclObj::inc()
{
  if ( mPtr ) {
    Tcl_IncrRefCount(mPtr);
  }
}

// mPtr の参照回数を一つ減らす．
void
TclObj::dec()
{
  if ( mPtr ) {
    Tcl_DecrRefCount(mPtr);
  }
}
  
// 文字列表現を無効化(invalidate)する．
void
TclObj::invalidate_stringrep()
{
  Tcl_InvalidateStringRep(mPtr);
}

// mPtr に新しいポインタをセットする．
// 参照回数が増やされる．
// mPtr は NULL でないことが分かっているときだけこの
// 関数を使うこと．
void
TclObj::set_ptr(Tcl_Obj* obj)
{
  Tcl_IncrRefCount(mPtr = obj);
}

END_NAMESPACE_YM_TCLPP
