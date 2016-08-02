#ifndef LIBYM_VERILOG_ELABORATOR_EI_EIVARPARTSELECT_H
#define LIBYM_VERILOG_ELABORATOR_EI_EIVARPARTSELECT_H

/// @file libym_verilog/elaborator/ei/EiVarPartSelect.h
/// @brief EiVarPartSelect のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiPartSelect.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "EiPrimary.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiVarPartSelect EiVarPartSelect.h "EiVarPartSelect.h"
/// @brief 可変パート選択付きのプライマリ式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiVarPartSelect :
  public EiExprBase1
{
protected:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] base 範囲のベースアドレスを表す式
  /// @param[in] range 範囲を表す式
  /// @param[in] range_val 範囲の値
  EiVarPartSelect(const PtBase* pt_expr,
		  ElbExpr* base,
		  ElbExpr* range,
		  int range_val);

  /// @brief デストラクタ
  virtual
  ~EiVarPartSelect();


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

  /// @brief 範囲の LSB の式を返す．
  /// @note 通常の範囲選択の時，意味を持つ．
  /// @note それ以外では NULL を返す．
  virtual
  const VlExpr*
  right_range() const;

  /// @brief 範囲の LSB の値を返す．
  /// @note 式に対する範囲選択の時，意味を持つ．
  /// @note それ以外では 0 を返す．
  virtual
  int
  right_range_val() const;

  /// @brief 範囲のビット幅を表す式を返す．
  /// @note 可変範囲選択の時，意味を持つ．
  /// @note それ以外では NULL を返す．
  virtual
  const VlExpr*
  range_width() const;

  /// @brief 範囲のビット幅を返す．
  /// @note 可変範囲選択の時，意味を持つ．
  /// @note それ以外では 0 を返す．
  virtual
  int
  range_width_val() const;

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

  // 範囲のベースの式
  ElbExpr* mBase;

  // 範囲
  ElbExpr* mRange;

  // 範囲の値
  int mRangeVal;

};


//////////////////////////////////////////////////////////////////////
/// @class EiDeclVarPartSelect EiVarPartSelect.h "EiVarPartSelect.h"
/// @brief ElbDecl を対象にしたパート選択子
//////////////////////////////////////////////////////////////////////
class EiDeclVarPartSelect :
  public EiVarPartSelect
{
protected:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] obj 本体のオブジェクト
  /// @param[in] base 範囲のベースアドレスを表す式
  /// @param[in] range 範囲を表す式
  /// @param[in] range_val 範囲の値
  EiDeclVarPartSelect(const PtBase* pt_expr,
		      ElbDecl* obj,
		      ElbExpr* base,
		      ElbExpr* range,
		      int range_val);

  /// @brief デストラクタ
  virtual
  ~EiDeclVarPartSelect();


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
/// @class EiDeclPlusPartSelect EiVarPartSelect.h "EiVarPartSelect.h"
/// @brief 可変パート選択付きのプライマリ式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiDeclPlusPartSelect :
  public EiDeclVarPartSelect
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] obj 本体のオブジェクト
  /// @param[in] base 範囲のベースアドレスを表す式
  /// @param[in] range 範囲を表す式
  /// @param[in] range_val 範囲の値
  EiDeclPlusPartSelect(const PtBase* pt_expr,
		       ElbDecl* obj,
		       ElbExpr* base,
		       ElbExpr* range,
		       int range_val);

  /// @brief デストラクタ
  virtual
  ~EiDeclPlusPartSelect();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲指定のモードを返す．
  virtual
  tVpiRangeMode
  range_mode() const;

  /// @brief 範囲の MSB の値を返す．
  /// @note 式に対する範囲選択の時，意味を持つ．
  /// @note それ以外では 0 を返す．
  virtual
  int
  left_range_val() const;

};


//////////////////////////////////////////////////////////////////////
/// @class EiDeclMinusPartSelect EiVarPartSelect.h "EiVarPartSelect.h"
/// @brief 可変パート選択付きのプライマリ式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiDeclMinusPartSelect :
  public EiDeclVarPartSelect
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] obj 本体のオブジェクト
  /// @param[in] base 範囲のベースアドレスを表す式
  /// @param[in] range 範囲を表す式
  /// @param[in] range_val 範囲の値
  EiDeclMinusPartSelect(const PtBase* pt_expr,
			ElbDecl* obj,
			ElbExpr* base,
			ElbExpr* range,
			int range_val);

  /// @brief デストラクタ
  virtual
  ~EiDeclMinusPartSelect();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲指定のモードを返す．
  virtual
  tVpiRangeMode
  range_mode() const;

  /// @brief 範囲の MSB の値を返す．
  /// @note 式に対する範囲選択の時，意味を持つ．
  /// @note それ以外では 0 を返す．
  virtual
  int
  left_range_val() const;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELABORATOR_EI_EIVARPARTSELECT_H
