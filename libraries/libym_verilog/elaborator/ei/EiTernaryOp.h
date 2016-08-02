#ifndef LIBYM_VERILOG_ELABORATOR_EI_EITERNARYOP_H
#define LIBYM_VERILOG_ELABORATOR_EI_EITERNARYOP_H

/// @file libym_verilog/elaborator/ei/EiTernaryOp.h
/// @brief EiExpr の3項演算子の派生クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiTernaryOp.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

//////////////////////////////////////////////////////////////////////
// expression を表すクラス
// IEEE Std 1364-2001 26.6.25 Simple expressions
// IEEE Std 1364-2001 26.6.26 Expressions
//
// operation の分類
// オペランドの数
// - 3個
//   - 返り値 第2オペランド or 第3オペランド, オペランド any
//     - ConditionOp
//   - 返り値 いずれかのオペランド, オペランド any
//     - MinTypMaxOp
//////////////////////////////////////////////////////////////////////


#include "EiOperation.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbTerinaryOp EiTernaryOp.h "EiTernaryOp.h"
/// @brief 三項演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class EiTernaryOp :
  public EiOperation
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] pt_obj パース木の定義要素
  /// @param[in] opr1 オペランド1
  /// @param[in] opr2 オペランド2
  /// @param[in] opr3 オペランド3
  EiTernaryOp(const PtBase* pt_obj,
	      ElbExpr* opr1,
	      ElbExpr* opr2,
	      ElbExpr* opr3);

  /// @brief デストラクタ
  virtual
  ~EiTernaryOp();


public:
  //////////////////////////////////////////////////////////////////////
  // VlbExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 定数の時 true を返す．
  /// @note オペランドが定数ならこの式も定数となる．
  virtual
  bool
  is_const() const;

  /// @brief オペランド数を返す．
  virtual
  ymuint32
  operand_num() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief オペランドを返す．
  /// @param[in] pos 位置番号
  virtual
  ElbExpr*
  _operand(ymuint32 pos) const;


protected:
  //////////////////////////////////////////////////////////////////////
  // 派生クラス用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 第1オペランドを返す．
  const ElbExpr*
  operand1() const { return mOpr[0]; }

  /// @brief 第2オペランドを返す．
  const ElbExpr*
  operand2() const { return mOpr[1]; }

  /// @brief 第3オペランドを返す．
  const ElbExpr*
  operand3() const { return mOpr[2]; }

  /// @brief 第1オペランドを返す．
  ElbExpr*
  operand1() { return mOpr[0]; }

  /// @brief 第2オペランドを返す．
  ElbExpr*
  operand2() { return mOpr[1]; }

  /// @brief 第3オペランドを返す．
  ElbExpr*
  operand3() { return mOpr[2]; }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // オペランド
  ElbExpr* mOpr[3];

};


//////////////////////////////////////////////////////////////////////
/// @class EiConditionOp EiBinaryOp.h "EiBinaryOp.h"
/// @brief 条件演算子
//////////////////////////////////////////////////////////////////////
class EiConditionOp :
  public EiTernaryOp
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_obj パース木の定義要素
  /// @param[in] opr1 オペランド1
  /// @param[in] opr2 オペランド2
  /// @param[in] opr3 オペランド3
  EiConditionOp(const PtBase* pt_obj,
		ElbExpr* opr1,
		ElbExpr* opr2,
		ElbExpr* opr3);

  /// @brief デストラクタ
  virtual
  ~EiConditionOp();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式のタイプを返す．
  virtual
  tVpiValueType
  value_type() const;

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
  // ElbExpr の設定用の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要求される式の型を計算してセットする．
  /// @param[in] type 要求される式の型
  /// @note 必要であればオペランドに対して再帰的に処理を行なう．
  virtual
  void
  set_reqsize(tVpiValueType type);


public:
  //////////////////////////////////////////////////////////////////////
  // EiOperation の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子のタイプを返す．
  virtual
  tVpiOpType
  op_type() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 式の型
  tVpiValueType mType;

};


//////////////////////////////////////////////////////////////////////
/// @class EiMinTypMaxOp EiBinaryOp.h "EiBinaryOp.h"
/// @brief 条件演算子
//////////////////////////////////////////////////////////////////////
class EiMinTypMaxOp :
  public EiTernaryOp
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_obj パース木の定義要素
  /// @param[in] opr1 オペランド1
  /// @param[in] opr2 オペランド2
  /// @param[in] opr3 オペランド3
  EiMinTypMaxOp(const PtBase* pt_obj,
		ElbExpr* opr1,
		ElbExpr* opr2,
		ElbExpr* opr3);

  /// @brief デストラクタ
  virtual
  ~EiMinTypMaxOp();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式のタイプを返す．
  virtual
  tVpiValueType
  value_type() const;

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
  // ElbExpr の設定用の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要求される式の型を計算してセットする．
  /// @param[in] type 要求される式の型
  /// @note 必要であればオペランドに対して再帰的に処理を行なう．
  virtual
  void
  set_reqsize(tVpiValueType type);


public:
  //////////////////////////////////////////////////////////////////////
  // EiOperation の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子のタイプを返す．
  virtual
  tVpiOpType
  op_type() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 式の型
  tVpiValueType mType;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELABORATOR_EI_EITERNARYOP_H
