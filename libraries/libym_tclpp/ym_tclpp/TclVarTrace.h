#ifndef YM_TCLPP_TCLVARTRACE_H
#define YM_TCLPP_TCLVARTRACE_H

/// @file ym_tclpp/TclVarTrace.h
/// @brief TclVarTrace のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TclVarTrace.h 1343 2008-03-25 17:15:35Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_tclpp/tclpp_nsdef.h"
#include "ym_tclpp/TclBase.h"


BEGIN_NAMESPACE_YM_TCLPP

//////////////////////////////////////////////////////////////////////
/// @class TclVarTrace TclVarTrace.h <ym_tclpp/TclVarTrace.h>
/// @ingroup Tclpp
/// @brief Tcl変数のトレースを実行するためのハンドラオブジェクト
//////////////////////////////////////////////////////////////////////
class TclVarTrace :
  public TclBase
{
  friend class TclBase;
public:

  /// @brief コンストラクタ
  /// @note この時点ではまだ特定のインタープリタには結び付いていない．
  TclVarTrace();
  
  /// @brief デストラクタ
  /// @note まだ変数にバインドしていたらトレースを削除する．
  virtual
  ~TclVarTrace();
  
  /// @brief 名前1を得る．
  string
  name1() const;
  
  /// @brief 名前2を得る．
  string
  name2() const;
  
  /// @brief フラグを得る．
  int
  flags() const;
  
  /// @brief 変数にバインドしている時に true を返す．
  bool
  is_bound() const;
  
  /// @brief 変数にバインドする．
  /// @param[in] interp Tclインタープリタ
  /// @param[in] name1 変数名
  /// @param[in] flags Tcl 変数アクセスに用いるフラグ
  /// @retval TCL_OK バインドが成功した．
  /// @retval TCL_ERROR バインドに失敗した．
  /// @note 既にバインドしている場合にはエラーとなる．
  virtual
  int
  bind(Tcl_Interp* interp,
       const string& name1,
       int flags);

  /// @brief 変数にバインドする．
  /// @param[in] interp Tclインタープリタ
  /// @param[in] name1 変数名
  /// @param[in] name2 インデックス
  /// @param[in] flags Tcl 変数アクセスに用いるフラグ
  /// @retval TCL_OK バインドが成功した．
  /// @retval TCL_ERROR バインドに失敗した．
  /// @note 既にバインドしている場合にはエラーとなる．
  virtual
  int
  bind(Tcl_Interp* interp,
       const string& name1,
       const string& name2,
       int flags);
  
  /// @brief バインドを解く．
  /// @note 既にバインドがなければ何もしない．
  /// @note この関数呼び出しは常に成功する(はず)
  virtual
  void
  unbind();

  /// @brief 変数が参照された時のトレースコールバック
  /// @param[in] name1 変数名
  /// @param[in] name2 インデックス(または NULL)
  /// @param[in] flags トレースが設定されたときのflag
  /// @return 通常は NULL を返す．エラーが起きたときは
  /// エラーメッセージを返す．
  /// @note これは純粋仮想関数なので多重定義しなければならない．
  /// 呼び出された時のname1,name2,flagsの内容を引数にとる．
  /// 配列全体に対してトレースを設定した場合などは設定時の name1,name2
  /// とこの関数が呼ばれるときの name1,name2 が異なっていることがある
  /// ので，mName1 == name1, mName2 == name2 と仮定してはいけない．
  /// @sa bind(Tcl_Interp* interp, const string& name1, int flags)
  /// @sa bind(Tcl_Interp* interp, const string& name1, const string& name2, int flags)
  virtual
  char*
  vartrace_proc(CONST84 char* name1,
		CONST84 char* name2,
		int flags) = 0;


private:

  // トレースコールバックの入口
  static
  char*
  vartrace_callback(ClientData clientData,
		    Tcl_Interp* interp,
		    CONST84 char* name1,
		    CONST84 char* name2,
		    int flags);


private:
  //////////////////////////////////////////////////////////////////////
  // メンバ変数
  //////////////////////////////////////////////////////////////////////

  // 変数名1
  string mName1;

  // 変数名2
  string mName2;

  // フラグ
  int mFlags;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// 名前1を得る．
inline
string
TclVarTrace::name1() const
{
  return mName1;
}

// 名前2を得る．
inline
string
TclVarTrace::name2() const
{
  return mName2;
}

// フラグを得る．
inline
int
TclVarTrace::flags() const
{
  return mFlags;
}

// 変数にバインドしている時に true を返す．
inline
bool
TclVarTrace::is_bound() const
{
  return interp() != NULL;
}

END_NAMESPACE_YM_TCLPP

#endif // YM_TCLPP_TCLVARTRACE_H
