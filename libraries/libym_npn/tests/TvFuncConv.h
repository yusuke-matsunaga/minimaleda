#ifndef LIBYM_NPN_TESTS_TVFUNCCONV_H
#define LIBYM_NPN_TESTS_TVFUNCCONV_H

/// @file libym_npn/tests/TvFuncConv.h
/// @brief TvFuncConv のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TvFuncConv.h 37 2006-07-17 07:04:25Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#include <ym_tclpp/TclObj.h>
#include <ym_npn/TvFunc.h>


BEGIN_NAMESPACE_YM_NPN


//////////////////////////////////////////////////////////////////////
/// @class TvFuncConv TvFuncConv.h "TvFuncConv.h"
/// @brief TvFunc と TclObj の間の変換を行うクラス
//////////////////////////////////////////////////////////////////////
class TvFuncConv
{
public:

  /// @brief TvFunc を Tcl_Obj に変換する関数
  /// @param[in] func 論理関数を表す TvFunc オブジェクト
  /// @preturn 変換された TclObj を返す．
  static
  TclObj to_tclobj(const TvFunc& func);

  /// @brief 論理関数を表すベクタを Tcl_Obj に変換する関数
  /// @param[in] ni 論理関数の入力数
  /// @param[in] vec 論理関数ベクタ
  /// @preturn 変換された TclObj を返す．
  /// @note vec の要素数は $2^{ni}$ でなければならない．
  static
  TclObj to_tclobj(size_t ni,
		   const vector<int>& vec);

  /// @brief TclObj から TvFunc を取り出す関数
  /// @param[in] obj TclObj
  /// @param[in] interp Tcl インタープリタ
  /// @param[out] func 変換された TvFunc
  static
  int from_tclobj(const TclObj& obj,
		  Tcl_Interp* interp,
		  TvFunc& func);


private:
  //////////////////////////////////////////////////////////////////////
  // Tcl_ObjType に設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief データを解放する．
  /// @param[in] obj 解放するデータを持つ Tcl_Obj
  static
  void free(Tcl_Obj* obj);

  /// @brief データの複製を作る．
  /// @param[in] src 複製元の Tcl_Obj
  /// @param[in] dst 複製先の Tcl_Obj
  static
  void dup(Tcl_Obj* src,
	   Tcl_Obj* dst);

  /// @brief データから文字列表現を作る．
  /// @param[in] obj 変換を行う Tcl_Obj
  static
  void update(Tcl_Obj* obj);

  /// @brief 文字列表現からデータを作る．
  /// @param[in] interp Tcl インタープリタ
  /// @param[in] obj 変換を行う Tcl_Obj
  static
  int set_from_any(Tcl_Interp* inpterp,
		   Tcl_Obj* obj);

private:

  // mTvFuncType を登録する．
  static
  void register_tvfunc_type();

  // Tcl_Obj から TvFunc へのポインタを取り出す関数
  static
  TvFunc* get_ptr(const Tcl_Obj* obj);

  // Tcl_Obj に TvFunc へのポインタをセットする関数
  static
  void set_ptr(Tcl_Obj* obj,
	       TvFunc* ptr);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // mTvFuncType を登録したかどうかを示すフラグ
  static
  bool mIsRegistered;

  // 型変換で用いられるテーブル
  static
  Tcl_ObjType mTvFuncType;
  
};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// Tcl_Obj から TvFunc へのポインタを取り出す関数
// やっていることはただのキャスト
inline
TvFunc*
TvFuncConv::get_ptr(const Tcl_Obj* obj)
{
  return static_cast<TvFunc*>(obj->internalRep.otherValuePtr);
}

// Tcl_Obj に TvFunc へのポインタをセットする関数
// やっていることはただのキャスト
inline
void
TvFuncConv::set_ptr(Tcl_Obj* obj,
		    TvFunc* ptr)
{
  obj->internalRep.otherValuePtr = static_cast<void*>(ptr);
}

END_NAMESPACE_YM_NPN

#endif // LIBYM_NPN_TESTS_TVFUNCCONV_H
