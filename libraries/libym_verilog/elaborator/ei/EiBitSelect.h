#ifndef LIBYM_VERILOG_ELABORATOR_EI_EIBITSELECT_H
#define LIBYM_VERILOG_ELABORATOR_EI_EIBITSELECT_H

/// @file libym_verilog/elaborator/ei/EiBitSelect.h
/// @brief EiBitSelect のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiBitSelect.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "EiPrimary.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiBitSelect EiBitSelect.h "EiBitSelect.h"
/// @brief ビット選択付きのプライマリ式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiBitSelect :
  public EiExprBase1
{
protected:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] bit_index ビット選択式
  EiBitSelect(const PtBase* pt_expr,
	      ElbExpr* bit_index);

  /// @brief デストラクタ
  virtual
  ~EiBitSelect();


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

  /// @brief ビット指定の時に true を返す．
  virtual
  bool
  is_bitselect() const;

  /// @brief 固定選択子の時 true を返す．
  /// @note ビット選択，部分選択の時，意味を持つ．
  virtual
  bool
  is_constant_select() const;

  /// @brief インデックス式を返す．
  virtual
  const VlExpr*
  index() const;

  /// @brief インデックス値を返す．
  virtual
  int
  index_val() const;

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

  // インデックス
  ElbExpr* mIndex;

};


//////////////////////////////////////////////////////////////////////
/// @class EiDeclBitSelect EiBitSelect.h "EiBitSelect.h"
/// @brief ビット選択付きのプライマリ式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiDeclBitSelect :
  public EiBitSelect
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] obj 本体のオブジェクト
  /// @param[in] bit_index ビット選択式
  EiDeclBitSelect(const PtBase* pt_expr,
		  ElbDecl* obj,
		  ElbExpr* bit_index);

  /// @brief デストラクタ
  virtual
  ~EiDeclBitSelect();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
  virtual
  const VlDecl*
  decl_obj() const;

  /// @brief スカラー値を返す．
  virtual
  tVpiScalarVal
  eval_scalar() const;


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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象の宣言要素
  ElbDecl* mObj;

};


//////////////////////////////////////////////////////////////////////
/// @class EiExprBitSelect EiBitSelect.h "EiBitSelect.h"
/// @brief ビット選択付きの式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiExprBitSelect :
  public EiExprBase1
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] expr 本体の式
  /// @param[in] bit_index ビット位置
  EiExprBitSelect(const PtBase* pt_expr,
		  ElbExpr* expr,
		  int bit_index);

  /// @brief デストラクタ
  virtual
  ~EiExprBitSelect();


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

  /// @brief ビット指定の時に true を返す．
  virtual
  bool
  is_bitselect() const;

  /// @brief 親の式を返す．
  /// @note 正確には式に対するビット選択/部分選択の時のみ意味を持つ．
  virtual
  const VlExpr*
  parent_expr() const;

  /// @brief インデックス値を返す．
  /// @note 式に対するビット選択の時，意味を持つ．
  virtual
  int
  index_val() const;

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
  int mIndex;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELABORATOR_EI_EIBITSELECT_H
