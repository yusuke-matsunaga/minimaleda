#ifndef LIBYM_VERILOG_ELABORATOR_EI_EIBINARYOP_H
#define LIBYM_VERILOG_ELABORATOR_EI_EIBINARYOP_H

/// @file libym_verilog/elaborator/ei/EiBinaryOp.h
/// @brief EiExpr の2項演算子の派生クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiBinaryOp.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

//////////////////////////////////////////////////////////////////////
/// expression を表すクラス
/// IEEE Std 1364-2001 26.6.25 Simple expressions
/// IEEE Std 1364-2001 26.6.26 Expressions
///
/// operation の分類
/// オペランドの数
///
/// - 2個
///   - 返り値 scalar, オペランド any
///     - EqOp, NeqOp, CaseEqOp, CaseNeqOp, GtOp, GeOp, LtOp, LeOp
///   - 返り値 scalar, オペランド scalar
///     - LogAndOp, LogOrOp
///   - 返り値 オペランドと同一, オペランド bitvector
///     - BitAndOp, BitOrOp, BitXorOp, BitXNorOp
///   - 返り値 オペランドと同一, オペランド any
///     - AddOp, SubOp, MultOp, DivOp, ModOp
///     - PowerOp
///   - 返り値 第1オペランドと同一, オペランド any
///     - LShiftOp, RShiftOp, ArithLShiftOp, ArithRShiftOp
//////////////////////////////////////////////////////////////////////


#include "EiOperation.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiBinaryOp EiBinaryOp.h "EiBinaryOp.h"
/// @brief 二項演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class EiBinaryOp :
  public EiOperation
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] pt_obj パース木の定義要素
  /// @param[in] opr1 オペランド1
  /// @param[in] opr2 オペランド2
  EiBinaryOp(const PtBase* pt_obj,
	     ElbExpr* opr1,
	     ElbExpr* opr2);

  /// @brief デストラクタ
  virtual
  ~EiBinaryOp();


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

  /// @brief 第1オペランドを返す．
  const ElbExpr*
  operand1() const { return mOpr[0]; }

  /// @brief 第2オペランドを返す．
  const ElbExpr*
  operand2() const { return mOpr[1]; }

  /// @brief 第1オペランドを返す．
  ElbExpr*
  operand1() { return mOpr[0]; }

  /// @brief 第2オペランドを返す．
  ElbExpr*
  operand2() { return mOpr[1]; }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // オペランド
  ElbExpr* mOpr[2];

};


//////////////////////////////////////////////////////////////////////
/// @class EiCompareOp EiBinaryOp.h "EiBinaryOp.h"
/// @brief 比較演算子の基底クラス
//////////////////////////////////////////////////////////////////////
class EiCompareOp :
  public EiBinaryOp
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] pt_obj パース木の定義要素
  /// @param[in] opr1 オペランド1
  /// @param[in] opr2 オペランド2
  EiCompareOp(const PtBase* pt_obj,
	      ElbExpr* opr1,
	      ElbExpr* opr2);

  /// @brief デストラクタ
  virtual
  ~EiCompareOp();


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
  // ElbExpr の設定用の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要求される式の型を計算してセットする．
  /// @param[in] type 要求される式の型
  /// @note 必要であればオペランドに対して再帰的に処理を行なう．
  virtual
  void
  set_reqsize(tVpiValueType type);


protected:
  //////////////////////////////////////////////////////////////////////
  // EiCompareOp の派生クラスのみに公開する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief オペランドを評価するときの型を返す．
  tVpiValueType
  opr_type() const { return mOprType; }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // オペランドの型
  tVpiValueType mOprType;

};


//////////////////////////////////////////////////////////////////////
/// @class EiEqOp EiBinaryOp.h "EiBinaryOp.h"
/// @brief 等価比較演算子
//////////////////////////////////////////////////////////////////////
class EiEqOp :
  public EiCompareOp
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_obj パース木の定義要素
  /// @param[in] opr1 オペランド1
  /// @param[in] opr2 オペランド2
  EiEqOp(const PtBase* pt_obj,
	 ElbExpr* opr1,
	 ElbExpr* opr2);

  /// @brief デストラクタ
  virtual
  ~EiEqOp();


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
/// @class EiNeqOp EiBinaryOp.h "EiBinaryOp.h"
/// @brief 非等価比較演算子
//////////////////////////////////////////////////////////////////////
class EiNeqOp :
  public EiCompareOp
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_obj パース木の定義要素
  /// @param[in] opr1 オペランド1
  /// @param[in] opr2 オペランド2
  EiNeqOp(const PtBase* pt_obj,
	  ElbExpr* opr1,
	  ElbExpr* opr2);

  /// @brief デストラクタ
  virtual
  ~EiNeqOp();


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
/// @class EiCaseEqOp EiBinaryOp.h "EiBinaryOp.h"
/// @brief case 型等価比較演算子
//////////////////////////////////////////////////////////////////////
class EiCaseEqOp :
  public EiCompareOp
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_obj パース木の定義要素
  /// @param[in] opr1 オペランド1
  /// @param[in] opr2 オペランド2
  EiCaseEqOp(const PtBase* pt_obj,
	     ElbExpr* opr1,
	     ElbExpr* opr2);

  /// @brief デストラクタ
  virtual
  ~EiCaseEqOp();


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
/// @class EiCaseNeqOp EiBinaryOp.h "EiBinaryOp.h"
/// @brief case 型非等価比較演算子
//////////////////////////////////////////////////////////////////////
class EiCaseNeqOp :
  public EiCompareOp
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_obj パース木の定義要素
  /// @param[in] opr1 オペランド1
  /// @param[in] opr2 オペランド2
  EiCaseNeqOp(const PtBase* pt_obj,
	      ElbExpr* opr1,
	      ElbExpr* opr2);

  /// @brief デストラクタ
  virtual
  ~EiCaseNeqOp();


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
/// @class EiGtOp EiBinaryOp.h "EiBinaryOp.h"
/// @brief Greater Than 比較演算子
//////////////////////////////////////////////////////////////////////
class EiGtOp :
  public EiCompareOp
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_obj パース木の定義要素
  /// @param[in] opr1 オペランド1
  /// @param[in] opr2 オペランド2
  EiGtOp(const PtBase* pt_obj,
	 ElbExpr* opr1,
	 ElbExpr* opr2);

  /// @brief デストラクタ
  virtual
  ~EiGtOp();


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
/// @class EiGeOp EiBinaryOp.h "EiBinaryOp.h"
/// @brief Greater than or Equal 比較演算子
//////////////////////////////////////////////////////////////////////
class EiGeOp :
  public EiCompareOp
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_obj パース木の定義要素
  /// @param[in] opr1 オペランド1
  /// @param[in] opr2 オペランド2
  EiGeOp(const PtBase* pt_obj,
	 ElbExpr* opr1,
	 ElbExpr* opr2);

  /// @brief デストラクタ
  virtual
  ~EiGeOp();


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
/// @class EiLtOp EiBinaryOp.h "EiBinaryOp.h"
/// @brief Less Than 比較演算子
//////////////////////////////////////////////////////////////////////
class EiLtOp :
  public EiCompareOp
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_obj パース木の定義要素
  /// @param[in] opr1 オペランド1
  /// @param[in] opr2 オペランド2
  EiLtOp(const PtBase* pt_obj,
	 ElbExpr* opr1,
	 ElbExpr* opr2);

  /// @brief デストラクタ
  virtual
  ~EiLtOp();


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
/// @class EiLeOp EiBinaryOp.h "EiBinaryOp.h"
/// @brief Less than or Equal 比較演算子
//////////////////////////////////////////////////////////////////////
class EiLeOp :
  public EiCompareOp
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_obj パース木の定義要素
  /// @param[in] opr1 オペランド1
  /// @param[in] opr2 オペランド2
  EiLeOp(const PtBase* pt_obj,
	 ElbExpr* opr1,
	 ElbExpr* opr2);

  /// @brief デストラクタ
  virtual
  ~EiLeOp();


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
/// @class EiBinaryLogOp EiBinaryOp.h "EiBinaryOp.h"
/// @brief 2項論理演算子の基底クラス
//////////////////////////////////////////////////////////////////////
class EiBinaryLogOp :
  public EiBinaryOp
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] pt_obj パース木の定義要素
  /// @param[in] opr1 オペランド1
  /// @param[in] opr2 オペランド2
  EiBinaryLogOp(const PtBase* pt_obj,
		ElbExpr* opr1,
		ElbExpr* opr2);

  /// @brief デストラクタ
  virtual
  ~EiBinaryLogOp();


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
/// @class EiLogAndOp EiBinaryOp.h "EiBinaryOp.h"
/// @brief 論理 AND 演算子
//////////////////////////////////////////////////////////////////////
class EiLogAndOp :
  public EiBinaryLogOp
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_obj パース木の定義要素
  /// @param[in] opr1 オペランド1
  /// @param[in] opr2 オペランド2
  EiLogAndOp(const PtBase* pt_obj,
	     ElbExpr* opr1,
	     ElbExpr* opr2);

  /// @brief デストラクタ
  virtual
  ~EiLogAndOp();


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
/// @class EiLogOrOp EiBinaryOp.h "EiBinaryOp.h"
/// @brief 論理 OR 演算子
//////////////////////////////////////////////////////////////////////
class EiLogOrOp :
  public EiBinaryLogOp
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_obj パース木の定義要素
  /// @param[in] opr1 オペランド1
  /// @param[in] opr2 オペランド2
  EiLogOrOp(const PtBase* pt_obj,
	    ElbExpr* opr1,
	    ElbExpr* opr2);

  /// @brief デストラクタ
  virtual
  ~EiLogOrOp();


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
/// @class EiBinaryBitOp EiBinaryOp.h "EiBinaryOp.h"
/// @brief 2項論理演算子の基底クラス
//////////////////////////////////////////////////////////////////////
class EiBinaryBitOp :
  public EiBinaryOp
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] pt_obj パース木の定義要素
  /// @param[in] opr1 オペランド1
  /// @param[in] opr2 オペランド2
  EiBinaryBitOp(const PtBase* pt_obj,
		ElbExpr* opr1,
		ElbExpr* opr2);

  /// @brief デストラクタ
  virtual
  ~EiBinaryBitOp();


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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 式の型
  tVpiValueType mType;

};


//////////////////////////////////////////////////////////////////////
/// @class EiBitAndOp EiBinaryOp.h "EiBinaryOp.h"
/// @brief ビットワイズ AND 演算子
//////////////////////////////////////////////////////////////////////
class EiBitAndOp :
  public EiBinaryBitOp
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_obj パース木の定義要素
  /// @param[in] opr1 オペランド1
  /// @param[in] opr2 オペランド2
  EiBitAndOp(const PtBase* pt_obj,
	     ElbExpr* opr1,
	     ElbExpr* opr2);

  /// @brief デストラクタ
  virtual
  ~EiBitAndOp();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

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
/// @class EiBitOrOp EiBinaryOp.h "EiBinaryOp.h"
/// @brief ビットワイズ OR 演算子
//////////////////////////////////////////////////////////////////////
class EiBitOrOp :
  public EiBinaryBitOp
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_obj パース木の定義要素
  /// @param[in] opr1 オペランド1
  /// @param[in] opr2 オペランド2
  EiBitOrOp(const PtBase* pt_obj,
	    ElbExpr* opr1,
	    ElbExpr* opr2);

  /// @brief デストラクタ
  virtual
  ~EiBitOrOp();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

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
/// @class EiBitXorOp EiBinaryOp.h "EiBinaryOp.h"
/// @brief ビットワイズ XOR 演算子
//////////////////////////////////////////////////////////////////////
class EiBitXorOp :
  public EiBinaryBitOp
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_obj パース木の定義要素
  /// @param[in] opr1 オペランド1
  /// @param[in] opr2 オペランド2
  EiBitXorOp(const PtBase* pt_obj,
	     ElbExpr* opr1,
	     ElbExpr* opr2);

  /// @brief デストラクタ
  virtual
  ~EiBitXorOp();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

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
/// @class EiBitXnorOp EiBinaryOp.h "EiBinaryOp.h"
/// @brief ビットワイズ XNOR 演算子
//////////////////////////////////////////////////////////////////////
class EiBitXnorOp :
  public EiBinaryBitOp
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_obj パース木の定義要素
  /// @param[in] opr1 オペランド1
  /// @param[in] opr2 オペランド2
  EiBitXnorOp(const PtBase* pt_obj,
	      ElbExpr* opr1,
	      ElbExpr* opr2);

  /// @brief デストラクタ
  virtual
  ~EiBitXnorOp();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

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
/// @class EiBinaryArithOp EiBinaryOp.h "EiBinaryOp.h"
/// @brief 2項算術演算子の基底クラス
//////////////////////////////////////////////////////////////////////
class EiBinaryArithOp :
  public EiBinaryOp
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] pt_obj パース木の定義要素
  /// @param[in] opr1 オペランド1
  /// @param[in] opr2 オペランド2
  EiBinaryArithOp(const PtBase* pt_obj,
		  ElbExpr* opr1,
		  ElbExpr* opr2);

  /// @brief デストラクタ
  virtual
  ~EiBinaryArithOp();


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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 式の型
  tVpiValueType mType;

};


//////////////////////////////////////////////////////////////////////
/// @class EiAddOp EiBinaryOp.h "EiBinaryOp.h"
/// @brief 加算演算子
//////////////////////////////////////////////////////////////////////
class EiAddOp :
  public EiBinaryArithOp
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_obj パース木の定義要素
  /// @param[in] opr1 オペランド1
  /// @param[in] opr2 オペランド2
  EiAddOp(const PtBase* pt_obj,
	  ElbExpr* opr1,
	  ElbExpr* opr2);

  /// @brief デストラクタ
  virtual
  ~EiAddOp();


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
/// @class EiSubOp EiBinaryOp.h "EiBinaryOp.h"
/// @brief 減算演算子
//////////////////////////////////////////////////////////////////////
class EiSubOp :
  public EiBinaryArithOp
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_obj パース木の定義要素
  /// @param[in] opr1 オペランド1
  /// @param[in] opr2 オペランド2
  EiSubOp(const PtBase* pt_obj,
	  ElbExpr* opr1,
	  ElbExpr* opr2);

  /// @brief デストラクタ
  virtual
  ~EiSubOp();


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
/// @class EiMultOp EiBinaryOp.h "EiBinaryOp.h"
/// @brief 乗算演算子
//////////////////////////////////////////////////////////////////////
class EiMultOp :
  public EiBinaryArithOp
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_obj パース木の定義要素
  /// @param[in] opr1 オペランド1
  /// @param[in] opr2 オペランド2
  EiMultOp(const PtBase* pt_obj,
	   ElbExpr* opr1,
	   ElbExpr* opr2);

  /// @brief デストラクタ
  virtual
  ~EiMultOp();


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
/// @class EiDivOp EiBinaryOp.h "EiBinaryOp.h"
/// @brief 除算演算子
//////////////////////////////////////////////////////////////////////
class EiDivOp :
  public EiBinaryArithOp
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_obj パース木の定義要素
  /// @param[in] opr1 オペランド1
  /// @param[in] opr2 オペランド2
  EiDivOp(const PtBase* pt_obj,
	  ElbExpr* opr1,
	  ElbExpr* opr2);

  /// @brief デストラクタ
  virtual
  ~EiDivOp();


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
/// @class EiModOp EiBinaryOp.h "EiBinaryOp.h"
/// @brief 剰余算演算子
//////////////////////////////////////////////////////////////////////
class EiModOp :
  public EiBinaryArithOp
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_obj パース木の定義要素
  /// @param[in] opr1 オペランド1
  /// @param[in] opr2 オペランド2
  EiModOp(const PtBase* pt_obj,
	  ElbExpr* opr1,
	  ElbExpr* opr2);

  /// @brief デストラクタ
  virtual
  ~EiModOp();


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
/// @class EiPowerOp EiBinaryOp.h "EiBinaryOp.h"
/// @brief べき乗算演算子
//////////////////////////////////////////////////////////////////////
class EiPowerOp :
  public EiBinaryOp
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_obj パース木の定義要素
  /// @param[in] opr1 オペランド1
  /// @param[in] opr2 オペランド2
  EiPowerOp(const PtBase* pt_obj,
	    ElbExpr* opr1,
	    ElbExpr* opr2);

  /// @brief デストラクタ
  virtual
  ~EiPowerOp();


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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 式の型
  tVpiValueType mType;

};


//////////////////////////////////////////////////////////////////////
/// @class EiShiftOp EiBinaryOp.h "EiBinaryOp.h"
/// @brief シフト演算子の基底クラス
//////////////////////////////////////////////////////////////////////
class EiShiftOp :
  public EiBinaryOp
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] pt_obj パース木の定義要素
  /// @param[in] opr1 オペランド1
  /// @param[in] opr2 オペランド2
  EiShiftOp(const PtBase* pt_obj,
	    ElbExpr* opr1,
	    ElbExpr* opr2);

  /// @brief デストラクタ
  virtual
  ~EiShiftOp();


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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 式の型
  tVpiValueType mType;

};


//////////////////////////////////////////////////////////////////////
/// @class EiLShiftOp EiBinaryOp.h "EiBinaryOp.h"
/// @brief 左シフト演算子
//////////////////////////////////////////////////////////////////////
class EiLShiftOp :
  public EiShiftOp
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_obj パース木の定義要素
  /// @param[in] opr1 オペランド1
  /// @param[in] opr2 オペランド2
  EiLShiftOp(const PtBase* pt_obj,
	     ElbExpr* opr1,
	     ElbExpr* opr2);

  /// @brief デストラクタ
  virtual
  ~EiLShiftOp();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

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
/// @class EiRShiftOp EiBinaryOp.h "EiBinaryOp.h"
/// @brief 右シフト演算子
//////////////////////////////////////////////////////////////////////
class EiRShiftOp :
  public EiShiftOp
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_obj パース木の定義要素
  /// @param[in] opr1 オペランド1
  /// @param[in] opr2 オペランド2
  EiRShiftOp(const PtBase* pt_obj,
	     ElbExpr* opr1,
	     ElbExpr* opr2);

  /// @brief デストラクタ
  virtual
  ~EiRShiftOp();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

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
/// @class EiArithLShiftOp EiBinaryOp.h "EiBinaryOp.h"
/// @brief 算術左シフト演算子
//////////////////////////////////////////////////////////////////////
class EiArithLShiftOp :
  public EiShiftOp
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_obj パース木の定義要素
  /// @param[in] opr1 オペランド1
  /// @param[in] opr2 オペランド2
  EiArithLShiftOp(const PtBase* pt_obj,
		  ElbExpr* opr1,
		  ElbExpr* opr2);

  /// @brief デストラクタ
  virtual
  ~EiArithLShiftOp();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

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
/// @class EiArithRShiftOp EiBinaryOp.h "EiBinaryOp.h"
/// @brief 算術左シフト演算子
//////////////////////////////////////////////////////////////////////
class EiArithRShiftOp :
  public EiShiftOp
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_obj パース木の定義要素
  /// @param[in] opr1 オペランド1
  /// @param[in] opr2 オペランド2
  EiArithRShiftOp(const PtBase* pt_obj,
		  ElbExpr* opr1,
		  ElbExpr* opr2);

  /// @brief デストラクタ
  virtual
  ~EiArithRShiftOp();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

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

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELABORATOR_EI_EIBINARYOP_H
