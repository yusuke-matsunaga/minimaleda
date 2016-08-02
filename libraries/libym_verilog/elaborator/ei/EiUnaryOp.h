#ifndef LIBYM_VERILOG_ELABORATOR_EI_EIUNARYOP_H
#define LIBYM_VERILOG_ELABORATOR_EI_EIUNARYOP_H

/// @file libym_verilog/elaborator/ei/EiUnaryOp.h
/// @brief EiExpr の単項演算子の派生クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiUnaryOp.h 2507 2009-10-17 16:24:02Z matsunaga $
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
//
// - 1個
//   - 返り値 scalar, オペランド scalar
//     - NotOp
//   - 返り値 scalar, オペランド bitvector
//     - UnaryAndOp, UnaryNandOp, UnaryOrOp, UnaryNorOp,
//       UnaryXorOp, UnaryXNorOp
//   - 返り値 オペランドと同一, オペランド bitvector
//     - BitNegOp
//   - 返り値 オペランドと同一, オペランド any
//     - PlusOp, MinusOp
//   - 返り値 none, オペランド none
//     - PosedgeOp, NegedgeOp
//////////////////////////////////////////////////////////////////////


#include "EiOperation.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiUnaryOp EiUnaryOp.h "EiUnaryOp.h"
/// @brief 単項演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class EiUnaryOp :
  public EiOperation
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] opr1 オペランド
  EiUnaryOp(const PtBase* pt_expr,
	    ElbExpr* opr1);

  /// @brief デストラクタ
  virtual
  ~EiUnaryOp();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
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
  // EiOperation の仮想関数
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

  /// @brief オペランドを返す．
  const ElbExpr*
  operand1() const { return mOpr1; }

  /// @brief オペランドを返す．
  ElbExpr*
  operand1() { return mOpr1; }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // オペランド
  ElbExpr* mOpr1;

};


//////////////////////////////////////////////////////////////////////
/// @class EiNotOp EiUnaryOp.h "EiUnaryOp.h"
/// @note 論理否定演算子
//////////////////////////////////////////////////////////////////////
class EiNotOp :
  public EiUnaryOp
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] opr1 オペランド
  EiNotOp(const PtBase* pt_expr,
	  ElbExpr* opr1);

  /// @brief デストラクタ
  virtual
  ~EiNotOp();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
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
  // ElbExpr の仮想関数
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

};


//////////////////////////////////////////////////////////////////////
/// @class EiBitNeg EiUnaryOp.h "EiUnaryOp.h"
/// @brief ビットワイズ反転演算子
//////////////////////////////////////////////////////////////////////
class EiBitNegOp :
  public EiUnaryOp
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] opr1 オペランド
  EiBitNegOp(const PtBase* pt_expr,
	     ElbExpr* opr1);

  /// @brief デストラクタ
  virtual
  ~EiBitNegOp();


public:
  //////////////////////////////////////////////////////////////////////
  // VlbExpr の仮想関数
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
/// @class EiReductionOp EiUnaryOp.h "EiUnaryOp.h"
/// @note リダクション演算子の基底クラス
//////////////////////////////////////////////////////////////////////
class EiReductionOp :
  public EiUnaryOp
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] opr1 オペランド
  EiReductionOp(const PtBase* pt_expr,
		ElbExpr* opr1);

  /// @brief デストラクタ
  virtual
  ~EiReductionOp();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式のタイプを返す．
  virtual
  tVpiValueType
  value_type() const;

  /// @brief スカラー値を返す．
  virtual
  tVpiScalarVal
  eval_scalar() const;

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

};


//////////////////////////////////////////////////////////////////////
/// @class EiUnaryAndOp EiUnaryOp.h "EiUnaryOp.h"
/// @brief リダクション AND 演算子
//////////////////////////////////////////////////////////////////////
class EiUnaryAndOp :
  public EiReductionOp
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] opr1 オペランド
  EiUnaryAndOp(const PtBase* pt_expr,
	       ElbExpr* opr1);

  /// @brief デストラクタ
  virtual
  ~EiUnaryAndOp();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 論理値を返す．
  virtual
  tVpiScalarVal
  eval_logic() const;


public:
  //////////////////////////////////////////////////////////////////////
  // EiOperation の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子のタイプを返す．
  virtual
  tVpiOpType
  op_type() const;

};


//////////////////////////////////////////////////////////////////////
/// @class EiUnaryNandOp EiUnaryOp.h "EiUnaryOp.h"
/// @brief リダクション NAND 演算子
//////////////////////////////////////////////////////////////////////
class EiUnaryNandOp :
  public EiReductionOp
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] opr1 オペランド
  EiUnaryNandOp(const PtBase* pt_expr,
		ElbExpr* opr1);

  /// @brief デストラクタ
  virtual
  ~EiUnaryNandOp();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 論理値を返す．
  virtual
  tVpiScalarVal
  eval_logic() const;


public:
  //////////////////////////////////////////////////////////////////////
  // EiOperation の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子のタイプを返す．
  virtual
  tVpiOpType
  op_type() const;

};


//////////////////////////////////////////////////////////////////////
/// @class EiUnaryOrOp EiUnaryOp.h "EiUnaryOp.h"
/// @brief リダクション OR 演算子
//////////////////////////////////////////////////////////////////////
class EiUnaryOrOp :
  public EiReductionOp
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] opr1 オペランド
  EiUnaryOrOp(const PtBase* pt_expr,
		ElbExpr* opr1);

  /// @brief デストラクタ
  virtual
  ~EiUnaryOrOp();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 論理値を返す．
  virtual
  tVpiScalarVal
  eval_logic() const;


public:
  //////////////////////////////////////////////////////////////////////
  // EiOperation の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子のタイプを返す．
  virtual
  tVpiOpType
  op_type() const;

};


//////////////////////////////////////////////////////////////////////
/// @class EiUnaryNorOp EiUnaryOp.h "EiUnaryOp.h"
/// @brief リダクション NOR 演算子
//////////////////////////////////////////////////////////////////////
class EiUnaryNorOp :
  public EiReductionOp
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] opr1 オペランド
  EiUnaryNorOp(const PtBase* pt_expr,
	       ElbExpr* opr1);

  /// @brief デストラクタ
  virtual
  ~EiUnaryNorOp();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 論理値を返す．
  virtual
  tVpiScalarVal
  eval_logic() const;


public:
  //////////////////////////////////////////////////////////////////////
  // EiOperation の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子のタイプを返す．
  virtual
  tVpiOpType
  op_type() const;

};


//////////////////////////////////////////////////////////////////////
/// @class EiUnaryXorOp EiUnaryOp.h "EiUnaryOp.h"
/// @brief リダクション XOR 演算子
//////////////////////////////////////////////////////////////////////
class EiUnaryXorOp :
  public EiReductionOp
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] opr1 オペランド
  EiUnaryXorOp(const PtBase* pt_expr,
	       ElbExpr* opr1);

  /// @brief デストラクタ
  virtual
  ~EiUnaryXorOp();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 論理値を返す．
  virtual
  tVpiScalarVal
  eval_logic() const;


public:
  //////////////////////////////////////////////////////////////////////
  // EiOperation の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子のタイプを返す．
  virtual
  tVpiOpType
  op_type() const;

};


//////////////////////////////////////////////////////////////////////
/// @class EiUnaryXnorOp EiUnaryOp.h "EiUnaryOp.h"
/// @brief リダクション XNOR 演算子
//////////////////////////////////////////////////////////////////////
class EiUnaryXnorOp :
  public EiReductionOp
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] opr1 オペランド
  EiUnaryXnorOp(const PtBase* pt_expr,
		ElbExpr* opr1);

  /// @brief デストラクタ
  virtual
  ~EiUnaryXnorOp();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 論理値を返す．
  virtual
  tVpiScalarVal
  eval_logic() const;


public:
  //////////////////////////////////////////////////////////////////////
  // EiOperation の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子のタイプを返す．
  virtual
  tVpiOpType
  op_type() const;

};


//////////////////////////////////////////////////////////////////////
/// @class EiUnaryArithOp EiUnaryOp.h "EiUnaryOp.h"
//////////////////////////////////////////////////////////////////////
class EiUnaryArithOp :
  public EiUnaryOp
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] opr1 オペランド
  EiUnaryArithOp(const PtBase* pt_expr,
		 ElbExpr* opr1);

  /// @brief デストラクタ
  virtual
  ~EiUnaryArithOp();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式のタイプを返す．
  virtual
  tVpiValueType
  value_type() const;


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

};


//////////////////////////////////////////////////////////////////////
/// @class EiPlusOp EiUnaryOp.h "EiUnaryOp.h"
/// @brief 単項プラス演算子
//////////////////////////////////////////////////////////////////////
class EiPlusOp :
  public EiUnaryArithOp
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] opr1 オペランド
  EiPlusOp(const PtBase* pt_expr,
	   ElbExpr* opr1);

  /// @brief デストラクタ
  virtual
  ~EiPlusOp();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief int 型の値を返す．
  /// @param[out] val 結果を格納する変数
  /// @return 整数値に変換できたら true を返す．
  virtual
  bool
  eval_int(int& val) const;

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
  // EiOperation の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子のタイプを返す．
  virtual
  tVpiOpType
  op_type() const;

};


//////////////////////////////////////////////////////////////////////
/// @class EiMinusOp EiUnaryOp.h "EiUnaryOp.h"
/// @brief 単項マイナス演算子
//////////////////////////////////////////////////////////////////////
class EiMinusOp :
  public EiUnaryArithOp
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] opr1 オペランド
  EiMinusOp(const PtBase* pt_expr,
	    ElbExpr* opr1);

  /// @brief デストラクタ
  virtual
  ~EiMinusOp();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief int 型の値を返す．
  /// @param[out] val 結果を格納する変数
  /// @return 整数値に変換できたら true を返す．
  virtual
  bool
  eval_int(int& val) const;

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
  // EiOperation の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子のタイプを返す．
  virtual
  tVpiOpType
  op_type() const;

};


//////////////////////////////////////////////////////////////////////
/// @class EiEventEdgeOp EiUnaryOp.h "EiUnaryOp.h"
/// @brief posdedge / negaedge 演算子の基底クラス
//////////////////////////////////////////////////////////////////////
class EiEventEdgeOp :
  public EiUnaryOp
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] opr1 オペランド
  EiEventEdgeOp(const PtBase* pt_expr,
		ElbExpr* opr1);

  /// @brief デストラクタ
  virtual
  ~EiEventEdgeOp();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
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

};


//////////////////////////////////////////////////////////////////////
/// @class EiPosedgeOp EiUnaryOp.h "EiUnaryOp.h"
/// @brief posedge 演算子
//////////////////////////////////////////////////////////////////////
class EiPosedgeOp :
  public EiEventEdgeOp
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] opr1 オペランド
  EiPosedgeOp(const PtBase* pt_expr,
	      ElbExpr* opr1);

  /// @brief デストラクタ
  virtual
  ~EiPosedgeOp();


public:
  //////////////////////////////////////////////////////////////////////
  // EiOperation の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子のタイプを返す．
  virtual
  tVpiOpType
  op_type() const;

};


//////////////////////////////////////////////////////////////////////
/// @class EiNegedgeOp EiUnaryOp.h "EiUnaryOp.h"
/// @brief negedge 演算子
//////////////////////////////////////////////////////////////////////
class EiNegedgeOp :
  public EiEventEdgeOp
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] opr1 オペランド
  EiNegedgeOp(const PtBase* pt_expr,
	      ElbExpr* opr1);

  /// @brief デストラクタ
  virtual
  ~EiNegedgeOp();


public:
  //////////////////////////////////////////////////////////////////////
  // EiOperation の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子のタイプを返す．
  virtual
  tVpiOpType
  op_type() const;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELABORATOR_EI_EIUNARYOP_H
