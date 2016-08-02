#ifndef LIBYM_VERILOG_ELABORATOR_EI_EIPARAMETER_H
#define LIBYM_VERILOG_ELABORATOR_EI_EIPARAMETER_H

/// @file libym_verilog/elaborator/ei/EiParameter.h
/// @brief EiParameter のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiParameter.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ElbParameter.h"
#include "EiRange.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiParamHead.h EiParameter.h "EiParameter.h"
/// @brief parameter 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class EiParamHead :
  public ElbParamHead
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head パース木の宣言ヘッダ
  EiParamHead(const VlNamedObj* parent,
	      const PtDeclHead* pt_head);

  /// @brief デストラクタ
  virtual
  ~EiParamHead();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbParamHead の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;

  /// @brief このオブジェクトの属しているスコープを返す．
  const VlNamedObj*
  parent() const;

  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  virtual
  bool
  is_signed() const;

  /// @brief 範囲のMSBの取得
  /// @retval 範囲のMSB 範囲を持つとき
  /// @retval NULL 範囲を持たないとき
  virtual
  ElbExpr*
  left_range() const;

  /// @brief 範囲のLSBの取得
  /// @retval 範囲のLSB 範囲を持つとき
  /// @retval NULL 範囲を持たないとき
  virtual
  ElbExpr*
  right_range() const;

  /// @brief MSB の値を返す．
  virtual
  int
  left_range_const() const;

  /// @brief LSB の値を返す．
  virtual
  int
  right_range_const() const;

  /// @brief ビット幅を返す．
  virtual
  ymuint32
  bit_size() const;

  /// @brief LSB からのオフセット値の取得
  /// @param[in] index インデックス
  /// @retval index の LSB からのオフセット index が範囲内に入っている．
  /// @retval -1 index が範囲外
  virtual
  int
  bit_offset(int index) const;

  /// @breif 値の型を返す．
  /// @note 値を持たないオブジェクトの場合には kVpiValueNone を返す．
  virtual
  tVpiValueType
  value_type() const;

  /// @brief データ型の取得
  virtual
  tVpiVarType
  data_type() const;



private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のスコープ
  const VlNamedObj* mParent;

  // パース木の宣言ヘッダ
  const PtDeclHead* mPtHead;

};


//////////////////////////////////////////////////////////////////////
/// @clacc EiParamHeadV EiParameter.h "EiParameter.h"
/// @brief 範囲付きの EiParamHead
//////////////////////////////////////////////////////////////////////
class EiParamHeadV :
  public EiParamHead
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head パース木の宣言ヘッダ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] left_val 範囲の左側の値
  /// @param[in] right_val 範囲の右側の値
  EiParamHeadV(const VlNamedObj* parent,
		const PtDeclHead* pt_head,
		ElbExpr* left,
		ElbExpr* right,
		int left_val,
		int right_val);

  /// @brief デストラクタ
  virtual
  ~EiParamHeadV();


public:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲のMSBの取得
  /// @retval 範囲のMSB 範囲を持つとき
  /// @retval NULL 範囲を持たないとき
  virtual
  ElbExpr*
  left_range() const;

  /// @brief 範囲のLSBの取得
  /// @retval 範囲のLSB 範囲を持つとき
  /// @retval NULL 範囲を持たないとき
  virtual
  ElbExpr*
  right_range() const;

  /// @brief MSB の値を返す．
  virtual
  int
  left_range_const() const;

  /// @brief LSB の値を返す．
  virtual
  int
  right_range_const() const;

  /// @brief ビット幅を返す．
  ymuint32
  bit_size() const;

  /// @brief LSB からのオフセット値の取得
  /// @param[in] index インデックス
  /// @retval index の LSB からのオフセット index が範囲内に入っている．
  /// @retval -1 index が範囲外
  virtual
  int
  bit_offset(int index) const;

  /// @breif 値の型を返す．
  /// @note 値を持たないオブジェクトの場合には kVpiValueNone を返す．
  virtual
  tVpiValueType
  value_type() const;


protected:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 範囲
  EiRangeImpl mRange;

};


//////////////////////////////////////////////////////////////////////
/// @class EiParameter EiParameter.h "EiParameter.h"
/// @brief ElbParameter の実装クラス
//////////////////////////////////////////////////////////////////////
class EiParameter :
  public ElbParameter
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] head ヘッダ
  /// @param[in] pt_item パース木の宣言要素
  EiParameter(ElbParamHead* head,
	      const PtNamedBase* pt_item);

  /// @brief デストラクタ
  virtual
  ~EiParameter();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;

  /// @brief ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;


public:
  //////////////////////////////////////////////////////////////////////
  // VlNamedObj の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief このオブジェクトの属しているスコープを返す．
  virtual
  const VlNamedObj*
  parent() const;

  /// @brief 名前の取得
  virtual
  const char*
  name() const;


public:
  //////////////////////////////////////////////////////////////////////
  // VlDecl の関数
  //////////////////////////////////////////////////////////////////////

  /// @breif 値の型を返す．
  /// @note 値を持たないオブジェクトの場合には kVpiValueNone を返す．
  virtual
  tVpiValueType
  value_type() const;

  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  virtual
  bool
  is_signed() const;

  /// @brief MSB の値を返す．
  virtual
  int
  left_range_const() const;

  /// @brief LSB の値を返す．
  virtual
  int
  right_range_const() const;

  /// @brief ビット幅を返す．
  virtual
  ymuint32
  bit_size() const;

  /// @brief オフセット値の取得
  /// @param[in] index インデックス
  /// @retval index のオフセット index が範囲内に入っている．
  /// @retval -1 index が範囲外
  virtual
  int
  bit_offset(int index) const;

  /// @brief データ型の取得
  /// @retval データ型 kParam, kLocalParam, kVar の場合
  /// @retval kVpiVarNone 上記以外
  virtual
  tVpiVarType
  data_type() const;

  /// @brief 初期値の取得
  /// @retval 初期値
  /// @retval NULL 設定がない場合
  virtual
  const VlExpr*
  init_value() const;

  /// @brief localparam のときに true 返す．
  /// @note このクラスでは false を返す．
  virtual
  bool
  is_local_param() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbDeclBase の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲のMSBの取得
  /// @retval 範囲のMSB 範囲を持つとき
  /// @retval NULL 範囲を持たないとき
  virtual
  ElbExpr*
  _left_range() const;

  /// @brief 範囲のLSBの取得
  /// @retval 範囲のLSB 範囲を持つとき
  /// @retval NULL 範囲を持たないとき
  virtual
  ElbExpr*
  _right_range() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief スカラー値を返す．
  virtual
  tVpiScalarVal
  get_scalar() const;

  /// @brief 論理値を返す．
  virtual
  tVpiScalarVal
  get_logic() const;

  /// @brief real 型の値を返す．
  virtual
  double
  get_real() const;

  /// @brief bitvector 型の値を返す．
  virtual
  void
  get_bitvector(BitVector& bitvector,
		tVpiValueType req_type = kVpiValueNone) const;

  /// @brief ビット選択値を返す．
  /// @param[in] index ビット位置
  virtual
  tVpiScalarVal
  get_bitselect(int index) const;

  /// @brief 範囲選択値を返す．
  /// @param[in] left 範囲の MSB
  /// @param[in] right 範囲の LSB
  /// @param[out] val 値
  virtual
  void
  get_partselect(int left,
		 int right,
		 BitVector& val) const;

  /// @brief 値の設定
  /// @param[in] expr 値を表す式
  virtual
  void
  set_expr(ElbExpr* expr);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ヘッダ
  ElbParamHead* mHead;

  // パース木の宣言要素
  const PtNamedBase* mPtItem;

  // 値
  ElbExpr* mExpr;

};


//////////////////////////////////////////////////////////////////////
/// @class EiLocalParam EiParameter.h "EiParameter.h"
/// @brief ElbParameter の実装クラス (localparam版)
//////////////////////////////////////////////////////////////////////
class EiLocalParam :
  public EiParameter
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] head ヘッダ
  /// @param[in] pt_item パース木の宣言要素
  EiLocalParam(ElbParamHead* head,
	       const PtNamedBase* pt_item);

  /// @brief デストラクタ
  virtual
  ~EiLocalParam();


public:
  //////////////////////////////////////////////////////////////////////
  // VlDecl の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief localparam のときに true 返す．
  /// @note このクラスでは true を返す．
  virtual
  bool
  is_local_param() const;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELABORATOR_EI_EIPARAMETER_H
