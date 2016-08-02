
/// @file libym_npn/tests/TvFuncConv.cc
/// @brief TvFuncConv の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TvFuncConv.cc 700 2007-05-31 00:41:30Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "TvFuncConv.h"


BEGIN_NAMESPACE_YM_NPN


// tvfunc_type を登録したかどうかを表すフラグ
bool TvFuncConv::mIsRegistered = false;


// TvFunc を TclObj に変換するための型定義
Tcl_ObjType TvFuncConv::mTvFuncType = {
  "tvfunc_type",
  TvFuncConv::free,
  TvFuncConv::dup,
  TvFuncConv::update,
  TvFuncConv::set_from_any
};

// Tcl_Obj と TvFunc* の変換用 Tcl_ObjType を作成して登録する．
inline
void
TvFuncConv::register_tvfunc_type()
{
  if ( mIsRegistered == false ) {
    // 未登録の場合のみ追加する．
    Tcl_RegisterObjType(&mTvFuncType);
    mIsRegistered = true;
  }
}

// free 関数
void
TvFuncConv::free(Tcl_Obj* obj)
{
  TvFunc* ptr = get_ptr(obj);
  delete ptr;
}

// duplicate 関数
void
TvFuncConv::dup(Tcl_Obj* src,
		Tcl_Obj* dup)
{
  // 本当に複製する．
  TvFunc* ptr = get_ptr(src);
  TvFunc* dup_ptr = new TvFunc(*ptr);
  set_ptr(dup, dup_ptr);
}

// update 関数
void
TvFuncConv::update(Tcl_Obj* obj)
{
  TvFunc* func_ptr = get_ptr(obj);
  size_t ni = func_ptr->ni();

  ostringstream buf;
  buf << ni << "|";
  func_ptr->dump(buf);
  string tmp = buf.str();

  Tcl_InvalidateStringRep(obj);
  obj->length = tmp.size();
  obj->bytes = Tcl_Alloc(obj->length + 1);
  strcpy(obj->bytes, tmp.c_str());
}

// 文字列表現から TvFunc を作って obj にセットする関数
int
TvFuncConv::set_from_any(Tcl_Interp* interp,
			 Tcl_Obj* obj)
{
  const char* str = Tcl_GetStringFromObj(obj, NULL);
  const char* bar = strchr(str, '|');

  size_t ni = 0;
  for (const char* p = str; p != bar; ++ p) {
    char c = *p;
    if ( c == ' ' || c == '\t' ) {
      continue;
    }
    if ( c >= '0' && c <= '9' ) {
      ni *= 10;
      ni += static_cast<size_t>(c) - static_cast<size_t>('0');
      continue;
    }
    if ( interp ) {
      TclObj emsg;
      emsg << str << " : Invalid form for TvFunc descriptor\n"
	   << "\tIt should be <#-of-inputs>|<bit pattern of length 2^ni>";
      Tcl_SetObjResult(interp, emsg.ptr());
    }
    return TCL_ERROR;
  }

  size_t ni_exp = 1UL << ni;
  vector<int> vec(ni_exp);
  const char* bp = bar + 1;
  for (char c = *bp; c != '\0'; ++ bp, c = *bp) {
    if ( c != ' ' && c != '\t' ) {
      break;
    }
  }
  for (size_t i = 0; i < ni_exp; ++ i) {
    char c = bp[i];
    if ( c == '0' ) {
      vec[i] = 0;
    }
    else if ( c == '1' ) {
      vec[i] = 1;
    }
    else {
      if ( interp ) {
	TclObj emsg;
	emsg << str << " : Invalid form for TvFunc descriptor\n"
	     << "\tIt should be <#-of-inputs(ni)>|<bit pattern of length 2^ni>";
	Tcl_SetObjResult(interp, emsg.ptr());
      }
      return TCL_ERROR;
    }
  }

  TvFunc* func_ptr = new TvFunc(ni, vec);
  
  if ( obj->typePtr != NULL && obj->typePtr->freeIntRepProc != NULL ) {
    // 以前の内部表現を解放する．
    obj->typePtr->freeIntRepProc(obj);
  }
  obj->typePtr = &mTvFuncType;
  set_ptr(obj, func_ptr);

  return TCL_OK;
}

// TvFunc を Tcl_Obj に変換する関数
TclObj
TvFuncConv::to_tclobj(const TvFunc& func)
{
  register_tvfunc_type();

  Tcl_Obj* result_obj = Tcl_NewObj();

  // TvFunc のポインターを設定
  result_obj->typePtr = &mTvFuncType;
  set_ptr(result_obj, new TvFunc(func));

  // 文字列表現はなし
  Tcl_InvalidateStringRep(result_obj);

  return TclObj(result_obj);
}

// TvFunc を Tcl_Obj に変換する関数
TclObj
TvFuncConv::to_tclobj(size_t ni,
		      const vector<int>& vec)
{
  register_tvfunc_type();

  Tcl_Obj* result_obj = Tcl_NewObj();

  // TvFunc のポインターを設定
  result_obj->typePtr = &mTvFuncType;
  set_ptr(result_obj, new TvFunc(ni, vec));

  // 文字列表現はなし
  Tcl_InvalidateStringRep(result_obj);

  return TclObj(result_obj);
}

// TclObj から TvFunc を取り出す関数
int
TvFuncConv::from_tclobj(const TclObj& obj,
			Tcl_Interp* interp,
			TvFunc& func)
{
  register_tvfunc_type();

  int code = obj.convert_to_type(&mTvFuncType, interp);
  if ( code != TCL_OK ) {
    return code;
  }

  const Tcl_Obj* obj_ptr = obj.ptr();
  func = *(get_ptr(obj_ptr));

  return TCL_OK;
}

END_NAMESPACE_YM_NPN
