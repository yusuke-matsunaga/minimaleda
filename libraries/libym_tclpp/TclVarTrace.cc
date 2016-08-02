
/// @file libym_tclpp/TclVarTrace.cc
/// @brief TclVarTrace の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TclVarTrace.cc 700 2007-05-31 00:41:30Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_tclpp/TclVarTrace.h"

#include "StrBuf.h"


BEGIN_NAMESPACE_YM_TCLPP


// コンストラクタ
TclVarTrace::TclVarTrace()
{
}

// デストラクタ
TclVarTrace::~TclVarTrace()
{
  unbind();
}

// 変数にバインドする．
// 既にバインドしている場合には無視する．
// 終了コードを返す．
int
TclVarTrace::bind(Tcl_Interp* interp,
		  const string& name1,
		  int flags)
{
  int result = TCL_OK;
  if ( !is_bound() ) {
    mName1 = name1;
    mName2 = "";
    mFlags = flags;
    // もともと flags に TCL_TRACE_UNSETS が含まれていなくても
    // それを足しておく．
    flags |= TCL_TRACE_UNSETS;
    StrBuf sptr(mName1);
    result = Tcl_TraceVar(interp, sptr, flags,
			  vartrace_callback, (ClientData) this);
    if ( result == TCL_OK ) {
      set_interp(interp);
    }
  }
  return result;
}

// 変数にバインドする．
// 既にバインドしている場合には無視する．
// 終了コードを返す．
int
TclVarTrace::bind(Tcl_Interp* interp,
		  const string& name1,
		  const string& name2,
		  int flags)
{
  int result = TCL_OK;
  if ( !is_bound() ) {
    mName1 = name1;
    mName2 = name2;
    mFlags = flags;
    // もともと flags に TCL_TRACE_UNSETS が含まれていなくても
    // それを足しておく．
    flags |= TCL_TRACE_UNSETS;
    StrBuf sptr1(mName1);
    StrBuf sptr2(mName2);
    result = Tcl_TraceVar2(interp, sptr1, sptr2, flags,
			   vartrace_callback, (ClientData) this);
    if ( result == TCL_OK ) {
      set_interp(interp);
    }
  }
  return result;
}

// バインドを解く．
// 既にバインドがなければ何もしない．
// 常に成功する(はず)
void
TclVarTrace::unbind()
{
  // もともと mFlags に TCL_TRACE_UNSETS が含まれていなくても
  // それを足しておく．
  int flags = mFlags | TCL_TRACE_UNSETS;
  if ( is_bound() ) {
    if ( mName2 !=  "" ) {
      StrBuf sptr1(mName1);
      StrBuf sptr2(mName2);
      Tcl_UntraceVar2(interp(), sptr1, sptr2, flags,
		      vartrace_callback, (ClientData) this);
    }
    else {
      StrBuf sptr1(mName1);
      Tcl_UntraceVar(interp(), sptr1, flags,
		     vartrace_callback, (ClientData) this);
    }
    set_interp(NULL);
  }
}

// トレースコールバックの入口
char*
TclVarTrace::vartrace_callback(ClientData clientData,
			       Tcl_Interp* interp,
			       CONST84 char* name1,
			       CONST84 char* name2,
			       int flags)
{
  // オブジェクトを得る．
  TclVarTrace* trace_obj = static_cast<TclVarTrace*>( clientData );

  // 念のため trace_obj の持っているインタプリタと interp が一致
  // する事を確かめておく．
  assert_cond( interp == trace_obj->interp(), __FILE__, __LINE__);

  char* result = NULL;

  if ((flags & TCL_TRACE_UNSETS) == 0 ||
      (trace_obj->flags() & TCL_TRACE_UNSETS) == TCL_TRACE_UNSETS) {
    // 元の mFlags がどうであれ，TCL_TRACE_UNSETS を加えているので，
    // 本当のトレースコールバック関数を呼ぶべきかどうか判定する．
    result = trace_obj->vartrace_proc(name1, name2, flags);
  }

  if ( flags & TCL_TRACE_DESTROYED ) {
    // バインドを外す．
    trace_obj->set_interp(NULL);
  }

  return result;
}

END_NAMESPACE_YM_TCLPP
