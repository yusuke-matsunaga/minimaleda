#ifndef LIBYM_VERILOG_ELABORATOR_EI_EIFUNCCALL_H
#define LIBYM_VERILOG_ELABORATOR_EI_EIFUNCCALL_H

/// @file libym_verilog/elaborator/ei/EiFuncCall.h
/// @brief EiFuncCall のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiFuncCall.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "EiExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiFcBase EiFuncCall.h "EiFuncCall.h"
/// @brief function call/system-function call に共通な基底クラス
//////////////////////////////////////////////////////////////////////
class EiFcBase :
  public EiExprBase1
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] pt_obj パース木の定義要素
  /// @param[in] arg_size 引数の数
  /// @param[in] arg_list 引数のリスト
  EiFcBase(const PtBase* pt_obj,
	   ymuint32 arg_size,
	   ElbExpr** arg_list);

  /// @brief デストラクタ
  virtual
  ~EiFcBase();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 引数の数を返す．
  virtual
  ymuint32
  argument_num() const;

  /// @brief 引数の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < argument_num() )
  virtual
  ElbExpr*
  argument(ymuint32 pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 引数の数
  ymuint32 mArgNum;

  // 引数の配列
  ElbExpr** mArgList;

};


//////////////////////////////////////////////////////////////////////
/// @class EiFuncCall EiFuncCall.h "EiFuncCall.h"
/// @brief 関数呼び出しを表すクラス
//////////////////////////////////////////////////////////////////////
class EiFuncCall :
  public EiFcBase
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_obj パース木の定義要素
  /// @param[in] func 関数
  /// @param[in] arg_size 引数の数
  /// @param[in] arg_list 引数のリスト
  EiFuncCall(const PtBase* pt_obj,
	     const ElbTaskFunc* func,
	     ymuint32 arg_size,
	     ElbExpr** arg_list);

  /// @brief デストラクタ
  virtual
  ~EiFuncCall();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式のタイプを返す．
  virtual
  tVpiValueType
  value_type() const;

  /// @brief 定数の時 true を返す．
  virtual
  bool
  is_const() const;

  /// @brief 関数呼び出しの時に true を返す．
  virtual
  bool
  is_funccall() const;

  /// @brief 対象の関数を返す．
  /// @note kVpiFuncCall の時，意味を持つ．
  virtual
  const VlTaskFunc*
  function() const;

  /// @brief スカラー値を返す．
  virtual
  tVpiScalarVal
  eval_scalar() const;

  /// @brief 論理値を返す．
  virtual
  tVpiScalarVal
  eval_logic() const;

  /// @brief real 型の値を返す．
  virtual
  double
  eval_real() const;

  /// @brief bitvector 型の値を返す．
  virtual
  void
  eval_bitvector(BitVector& bitvector,
		 tVpiValueType req_type = kVpiValueNone) const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要求される式の型を計算してセットする．
  /// @param[in] type 要求される式の型
  /// @note 必要であればオペランドに対して再帰的に処理を行なう．
  virtual
  void
  set_reqsize(tVpiValueType type);

  /// @brief decompile() の実装関数
  /// @param[in] pprim 親の演算子の優先順位
  virtual
  string
  decompile_impl(int ppri) const;


private:
  //////////////////////////////////////////////////////////////////////
  // eval_XXXX の下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 関数の値を評価する．
  ElbExpr*
  evaluate() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 関数
  const ElbTaskFunc* mFunc;

};


//////////////////////////////////////////////////////////////////////
/// @class EiSysFuncCall EiFuncCall.h "EiFuncCall.h"
/// @brief システム関数呼び出しを表すクラス
//////////////////////////////////////////////////////////////////////
class EiSysFuncCall :
  public EiFcBase
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_obj パース木の定義要素
  /// @param[in] user_systf システム関数
  /// @param[in] arg_size 引数の数
  /// @param[in] arg_list 引数のリスト
  EiSysFuncCall(const PtBase* pt_obj,
		const ElbUserSystf* user_systf,
		ymuint32 arg_size,
		ElbExpr** arg_list);

  /// @brief デストラクタ
  virtual
  ~EiSysFuncCall();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式のタイプを返す．
  virtual
  tVpiValueType
  value_type() const;

  /// @brief 定数の時 true を返す．
  /// @note このクラスは false を返す．
  virtual
  bool
  is_const() const;

  /// @brief システム関数よびあどい時に true を返す．
  virtual
  bool
  is_sysfunccall() const;

  /// @brief 対象のシステム関数を返す．
  /// @note kVpiSysFuncCall の時，意味を持つ．
  virtual
  const VlUserSystf*
  user_systf() const;

  /// @brief スカラー値を返す．
  virtual
  tVpiScalarVal
  eval_scalar() const;

  /// @brief 論理値を返す．
  virtual
  tVpiScalarVal
  eval_logic() const;

  /// @brief real 型の値を返す．
  virtual
  double
  eval_real() const;

  /// @brief bitvector 型の値を返す．
  virtual
  void
  eval_bitvector(BitVector& bitvector,
		 tVpiValueType req_type = kVpiValueNone) const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要求される式の型を計算してセットする．
  /// @param[in] type 要求される式の型
  /// @note 必要であればオペランドに対して再帰的に処理を行なう．
  virtual
  void
  set_reqsize(tVpiValueType type);

  /// @brief decompile() の実装関数
  /// @param[in] pprim 親の演算子の優先順位
  virtual
  string
  decompile_impl(int ppri) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // システム関数
  const ElbUserSystf* mUserSystf;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELABORATOR_EI_EIFUNCCALL_H
