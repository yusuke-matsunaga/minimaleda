#ifndef LIBYM_VERILOG_ELB_ELBTASKFUNC_H
#define LIBYM_VERILOG_ELB_ELBTASKFUNC_H

/// @file libym_verilog/elaborator/include/ElbTaskFunc.h
/// @brief ElbTaskFunc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ElbTaskFunc.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/vl/VlTaskFunc.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbTaskFunc ElbTaskFunc.h "ElbTaskFunc.h"
/// @brief elaboration 中の task/function を表すクラス
/// IEEE Std 1364-2001 26.6.18 Task, function declaration
//////////////////////////////////////////////////////////////////////
class ElbTaskFunc :
  public VlTaskFunc
{
  friend class CellTask;
  friend class CellFunction;

protected:

  /// @brief コンストラクタ
  ElbTaskFunc();

  /// @brief デストラクタ
  virtual
  ~ElbTaskFunc();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbTaskFunc の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入出力の初期設定を行う．
  /// @param[in] pos 位置番号
  /// @param[in] pos 位置番号
  /// @param[in] head ヘッダ
  /// @param[in] pt_item パース木のIO宣言要素
  /// @param[in] decl 対応する宣言要素
  virtual
  void
  init_iodecl(ymuint32 pos,
  	      ElbIOHead* head,
	      const PtIOItem* pt_item,
	      ElbDecl* decl) = 0;

  /// @brief 本体のステートメントをセットする．
  virtual
  void
  set_stmt(ElbStmt* stmt) = 0;

  /// @brief 入出力を得る．
  /// @param[in] pos 位置番号 (0 <= pos < io_num())
  virtual
  ElbIODecl*
  _io(ymuint32 pos) const = 0;

  /// @brief 本体の ElbStmt を得る．
  virtual
  ElbStmt*
  _stmt() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbTaskFunc の仮想関数
  // ただし Function の時しか意味を持たない．
  //////////////////////////////////////////////////////////////////////


  /// @brief 出力変数をセットする．
  /// @param[in] ovar 出力変数
  virtual
  void
  set_ovar(ElbDecl* ovar) = 0;

  /// @brief constant function の時に true を返す．
  virtual
  bool
  is_constant_function() const = 0;

  /// @brief スカラー値を返す．
  /// @param[in] arg_list 引数のリスト
  /// @note constant function の場合のみ意味を持つ．
  virtual
  tVpiScalarVal
  eval_scalar(const vector<ElbExpr*>& arg_list) const = 0;

  /// @brief 論理値を返す．
  /// @param[in] arg_list 引数のリスト
  /// @note constant function の場合のみ意味を持つ．
  virtual
  tVpiScalarVal
  eval_logic(const vector<ElbExpr*>& arg_list) const = 0;

  /// @brief real 型の値を返す．
  /// @param[in] arg_list 引数のリスト
  /// @note constant function の場合のみ意味を持つ．
  virtual
  double
  eval_real(const vector<ElbExpr*>& arg_list) const = 0;

  /// @brief bitvector 型の値を返す．
  /// @param[in] arg_list 引数のリスト
  /// @note constant function の場合のみ意味を持つ．
  virtual
  void
  eval_bitvector(const vector<ElbExpr*>& arg_list,
		 BitVector& bitvector,
		 tVpiValueType req_type = kVpiValueNone) const = 0;

  /// @brief 範囲のMSBの取得
  /// @retval 範囲のMSB 範囲を持つとき
  /// @retval NULL 範囲を持たないとき
  virtual
  ElbExpr*
  _left_range() const = 0;

  /// @brief 範囲のLSBの取得
  /// @retval 範囲のLSB 範囲を持つとき
  /// @retval NULL 範囲を持たないとき
  virtual
  ElbExpr*
  _right_range() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbTaskFunc の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素を返す．
  const ElbTaskFunc*
  next() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 次の要素を指すポインタ
  ElbTaskFunc* mNext;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
ElbTaskFunc::ElbTaskFunc() :
  mNext(NULL)
{
}

// @brief デストラクタ
inline
ElbTaskFunc::~ElbTaskFunc()
{
}

// @brief 次の要素を返す．
inline
const ElbTaskFunc*
ElbTaskFunc::next() const
{
  return mNext;
}

END_NAMESPACE_YM_VERILOG

#endif // LiBYM_VERILOG_ELB_ELBTASKFUNC_H
