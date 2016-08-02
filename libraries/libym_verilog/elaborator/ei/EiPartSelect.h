#ifndef LIBYM_VERILOG_ELABORATOR_EI_EIPARTSELECT_H
#define LIBYM_VERILOG_ELABORATOR_EI_EIPARTSELECT_H

/// @file libym_verilog/elaborator/ei/EiPartSelect.h
/// @brief EiPartSelect のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiPartSelect.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "EiPrimary.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiPartSelectPart EiPartSelect.h "EiPartSelect.h"
/// @brief パート選択付きのプライマリ式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiPartSelect :
  public EiExprBase1
{
protected:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] index1, index2 パート選択式
  /// @param[in] index1_val, index2_val パート選択式の値
  EiPartSelect(const PtBase* pt_expr,
	       ElbExpr* index1,
	       ElbExpr* index2,
	       int index1_val,
	       int index2_val);

  /// @brief デストラクタ
  virtual
  ~EiPartSelect();


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
  /// @note 参照している要素の型によって決まる．
  virtual
  bool
  is_const() const;

  /// @brief 固定選択子の時 true を返す．
  /// @note ビット選択，部分選択の時，意味を持つ．
  virtual
  bool
  is_constant_select() const;

  /// @brief 範囲指定の時に true を返す．
  virtual
  bool
  is_partselect() const;

  /// @brief 範囲指定のモードを返す．
  virtual
  tVpiRangeMode
  range_mode() const;

  /// @brief 範囲の MSB の式を返す．
  /// @note 通常の範囲選択の時，意味を持つ．
  virtual
  const VlExpr*
  left_range() const;

  /// @brief 範囲の LSB の式を返す．
  /// @note 通常の範囲選択の時，意味を持つ．
  virtual
  const VlExpr*
  right_range() const;

  /// @brief 範囲の MSB の値を返す．
  /// @note 式に対する範囲選択の時，意味を持つ．
  virtual
  int
  left_range_val() const;

  /// @brief 範囲の LSB の値を返す．
  /// @note 式に対する範囲選択の時，意味を持つ．
  virtual
  int
  right_range_val() const;

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

  /// @brief decompile() の実装関数
  /// @param[in] pprim 親の演算子の優先順位
  virtual
  string
  decompile_impl(int ppri) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 範囲選択式
  ElbExpr* mIndex1;

  ElbExpr* mIndex2;

  // 範囲選択式の値
  int mIndex1Val;

  int mIndex2Val;

};


//////////////////////////////////////////////////////////////////////
/// @class EiDeclPartSelectPart EiPartSelect.h "EiPartSelect.h"
/// @brief パート選択付きのプライマリ式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiDeclPartSelect :
  public EiPartSelect
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] obj 本体のオブジェクト
  /// @param[in] index1, index2 パート選択式
  /// @param[in] index1_val, index2_val パート選択式の値
  EiDeclPartSelect(const PtBase* pt_expr,
		   ElbDecl* obj,
		   ElbExpr* index1,
		   ElbExpr* index2,
		   int index1_val,
		   int index2_val);

  /// @brief デストラクタ
  virtual
  ~EiDeclPartSelect();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
  virtual
  const VlDecl*
  decl_obj() const;

  /// @brief bitvector 型の値を返す．
  virtual
  void
  eval_bitvector(BitVector& bitvector,
		 tVpiValueType req_type = kVpiValueNone) const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief スカラー値を書き込む．
  /// @param[in] v 書き込む値
  /// @note 左辺式の時のみ意味を持つ．
  virtual
  void
  set_scalar(tVpiScalarVal v);

  /// @brief ビットベクタを書き込む．
  /// @param[in] v 書き込む値
  /// @note 左辺式の時のみ意味を持つ．
  virtual
  void
  set_bitvector(const BitVector& v);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象の宣言要素
  ElbDecl* mObj;

};


//////////////////////////////////////////////////////////////////////
/// @class EiExprPartSelectPart EiPartSelect.h "EiPartSelect.h"
/// @brief パート選択付きの式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiExprPartSelect :
  public EiExprBase1
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] expr 本体の式
  /// @param[in] index1, inde2 パート選択式
  EiExprPartSelect(const PtBase* pt_expr,
		   ElbExpr* obj,
		   int index1,
		   int index2);

  /// @brief デストラクタ
  virtual
  ~EiExprPartSelect();


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
  /// @note 参照している要素の型によって決まる．
  virtual
  bool
  is_const() const;

  /// @brief 固定選択子の時 true を返す．
  /// @note ビット選択，部分選択の時，意味を持つ．
  virtual
  bool
  is_constant_select() const;

  /// @brief 範囲指定の時に true を返す．
  virtual
  bool
  is_partselect() const;

  /// @brief 範囲指定のモードを返す．
  virtual
  tVpiRangeMode
  range_mode() const;

  /// @brief 親の式を返す．
  /// @note 式に対するビット選択/範囲選択の時，意味を持つ．
  virtual
  const VlExpr*
  parent_expr() const;

  /// @brief 範囲の MSB の値を返す．
  /// @note 式に対する範囲選択の時，意味を持つ．
  virtual
  int
  left_range_val() const;

  /// @brief 範囲の LSB の値を返す．
  /// @note 式に対する範囲選択の時，意味を持つ．
  virtual
  int
  right_range_val() const;

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

  // 対象の式
  ElbExpr* mExpr;

  // インデックス
  int mIndex1;

  int mIndex2;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELABORATOR_EI_EIPARTSELECT_H
