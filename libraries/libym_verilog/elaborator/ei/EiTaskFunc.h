#ifndef LIBYM_VERILOG_ELABORATOR_EI_EITASKFUNC_H
#define LIBYM_VERILOG_ELABORATOR_EI_EITASKFUNC_H

/// @file libym_verilog/elaborator/ei/ElbTaskFuncImpl.h
/// @brief ElbTaskFuncImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiTaskFunc.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ElbTaskFunc.h"
#include "EiRange.h"


BEGIN_NAMESPACE_YM_VERILOG

class EiIODecl;

//////////////////////////////////////////////////////////////////////
/// @class EiTask EiTaskFunc.h "EiTaskFunc.h"
/// @brief ElbTaskFunc の実装クラス
//////////////////////////////////////////////////////////////////////
class EiTaskFunc :
  public ElbTaskFunc
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_item パース木の定義
  /// @param[in] io_num IOの数
  /// @param[in] io_array IO の配列
  EiTaskFunc(const VlNamedObj* parent,
	     const PtItem* pt_item,
	     ymuint32 io_num,
	     EiIODecl* io_array);

  /// @brief デストラクタ
  virtual
  ~EiTaskFunc();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

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
  // VlTaskFunc の仮想関数(タスク/関数共通)
  //////////////////////////////////////////////////////////////////////

  /// @brief automatic 宣言されていたら true を返す．
  virtual
  bool
  automatic() const;

  /// @brief 入出力数を得る．
  virtual
  ymuint32
  io_num() const;

  /// @brief 入出力の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < io_num() )
  virtual
  const VlIODecl*
  io(ymuint32 pos) const;

  /// @brief 本体のステートメントを得る．
  virtual
  const VlStmt*
  stmt() const;


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
	      ElbDecl* decl);

  /// @brief 本体のステートメントをセットする．
  virtual
  void
  set_stmt(ElbStmt* stmt);

  /// @brief 入出力を得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < io_num() )
  virtual
  ElbIODecl*
  _io(ymuint32 pos) const;

  /// @brief 本体の ElbStmt を得る．
  virtual
  ElbStmt*
  _stmt() const;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief PtItem を取り出す．
  const PtItem*
  pt_item() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のスコープ
  const VlNamedObj* mParent;

  // パース木のタスク/関数定義
  const PtItem* mPtItem;

  // 入出力数
  ymuint32 mIODeclNum;

  // 入出力のリスト
  EiIODecl* mIODeclList;

  // 本体のステートメント
  ElbStmt* mStmt;

};


//////////////////////////////////////////////////////////////////////
/// @class EiTask EiTaskFunc.h "EiTaskFunc.h"
/// @brief タスクを表す EiTaskFunc の実装クラス
//////////////////////////////////////////////////////////////////////
class EiTask :
  public EiTaskFunc
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_item パース木の定義
  /// @param[in] io_num IOの数
  /// @param[in] io_array IO の配列
  EiTask(const VlNamedObj* parent,
	 const PtItem* pt_item,
	 ymuint32 io_num,
	 EiIODecl* io_array);

  /// @brief デストラクタ
  virtual
  ~EiTask();


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
  // VlTaskFunc の仮想関数(関数の時のみ意味を持つもの)
  //////////////////////////////////////////////////////////////////////

  /// @brief function type を返す．
  virtual
  tVpiFuncType
  func_type() const;

  /// @brief 出力のビット幅を返す．
  virtual
  ymuint32
  bit_size() const;

  /// @brief 符号付きの時 true を返す．
  virtual
  bool
  is_signed() const;

  /// @brief 範囲のMSBを返す．
  virtual
  const VlExpr*
  left_range() const;

  /// @brief 範囲のLSBを返す．
  virtual
  const VlExpr*
  right_range() const;

  /// @brief 範囲の MSB の値を返す．
  /// @retval 範囲のMSBの値 範囲指定を持つとき
  /// @retval -1 範囲指定を持たないとき
  /// @note このクラスでは -1 を返す．
  virtual
  int
  left_range_const() const;

  /// @brief 範囲の LSB の値を返す．
  /// @retval 範囲のLSBの値 範囲指定を持つとき
  /// @retval -1 範囲指定を持たないとき
  /// @note このクラスでは -1 を返す．
  virtual
  int
  right_range_const() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbTaskFunc の仮想関数
  // ただし Function の時しか意味を持たない．
  //////////////////////////////////////////////////////////////////////

  /// @brief 出力変数をセットする．
  /// @param[in] ovar 出力変数
  virtual
  void
  set_ovar(ElbDecl* ovar);

  /// @brief constant function の時に true を返す．
  virtual
  bool
  is_constant_function() const;

  /// @brief スカラー値を返す．
  /// @param[in] arg_list 引数のリスト
  /// @note constant function の場合のみ意味を持つ．
  virtual
  tVpiScalarVal
  eval_scalar(const vector<ElbExpr*>& arg_list) const;

  /// @brief 論理値を返す．
  /// @param[in] arg_list 引数のリスト
  /// @note constant function の場合のみ意味を持つ．
  virtual
  tVpiScalarVal
  eval_logic(const vector<ElbExpr*>& arg_list) const;

  /// @brief real 型の値を返す．
  /// @param[in] arg_list 引数のリスト
  /// @note constant function の場合のみ意味を持つ．
  virtual
  double
  eval_real(const vector<ElbExpr*>& arg_list) const;

  /// @brief bitvector 型の値を返す．
  /// @param[in] arg_list 引数のリスト
  /// @note constant function の場合のみ意味を持つ．
  virtual
  void
  eval_bitvector(const vector<ElbExpr*>& arg_list,
		 BitVector& bitvector,
		 tVpiValueType req_type = kVpiValueNone) const;

  /// @brief 範囲のMSBの取得
  /// @retval 範囲のMSB 範囲を持つとき
  /// @retval NULL 範囲を持たないとき
  /// @note このクラスでは NULL を返す．
  virtual
  ElbExpr*
  _left_range() const;

  /// @brief 範囲のLSBの取得
  /// @retval 範囲のLSB 範囲を持つとき
  /// @retval NULL 範囲を持たないとき
  /// @note このクラスでは NULL を返す．
  virtual
  ElbExpr*
  _right_range() const;

};


//////////////////////////////////////////////////////////////////////
/// @class EiFunction EiTaskFunc.h "EiTaskFunc.h"
/// @brief 関数を表す EiTaskFunc の実装クラス
//////////////////////////////////////////////////////////////////////
class EiFunction :
  public EiTaskFunc
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_item パース木の定義
  /// @param[in] io_num IOの数
  /// @param[in] io_array IO の配列
  EiFunction(const VlNamedObj* parent,
	     const PtItem* pt_item,
	     ymuint32 io_num,
	     EiIODecl* io_array);

  /// @brief デストラクタ
  virtual
  ~EiFunction();


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
  // VlTaskFunc の仮想関数(関数の時のみ意味を持つもの)
  //////////////////////////////////////////////////////////////////////

  /// @brief function type を返す．
  virtual
  tVpiFuncType
  func_type() const;

  /// @brief 出力のビット幅を返す．
  virtual
  ymuint32
  bit_size() const;

  /// @brief 符号付きの時 true を返す．
  virtual
  bool
  is_signed() const;

  /// @brief 範囲のMSBを返す．
  virtual
  const VlExpr*
  left_range() const;

  /// @brief 範囲のLSBを返す．
  virtual
  const VlExpr*
  right_range() const;

  /// @brief 範囲の MSB の値を返す．
  /// @retval 範囲のMSBの値 範囲指定を持つとき
  /// @retval -1 範囲指定を持たないとき
  /// @note このクラスでは -1 を返す．
  virtual
  int
  left_range_const() const;

  /// @brief 範囲の LSB の値を返す．
  /// @retval 範囲のLSBの値 範囲指定を持つとき
  /// @retval -1 範囲指定を持たないとき
  /// @note このクラスでは -1 を返す．
  virtual
  int
  right_range_const() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbTaskFunc の仮想関数
  // ただし Function の時しか意味を持たない．
  //////////////////////////////////////////////////////////////////////

  /// @brief 出力変数をセットする．
  /// @param[in] ovar 出力変数
  virtual
  void
  set_ovar(ElbDecl* ovar);

  /// @brief constant function の時に true を返す．
  virtual
  bool
  is_constant_function() const;

  /// @brief スカラー値を返す．
  /// @param[in] arg_list 引数のリスト
  /// @note constant function の場合のみ意味を持つ．
  virtual
  tVpiScalarVal
  eval_scalar(const vector<ElbExpr*>& arg_list) const;

  /// @brief 論理値を返す．
  /// @param[in] arg_list 引数のリスト
  /// @note constant function の場合のみ意味を持つ．
  virtual
  tVpiScalarVal
  eval_logic(const vector<ElbExpr*>& arg_list) const;

  /// @brief real 型の値を返す．
  /// @param[in] arg_list 引数のリスト
  /// @note constant function の場合のみ意味を持つ．
  virtual
  double
  eval_real(const vector<ElbExpr*>& arg_list) const;

  /// @brief bitvector 型の値を返す．
  /// @param[in] arg_list 引数のリスト
  /// @note constant function の場合のみ意味を持つ．
  virtual
  void
  eval_bitvector(const vector<ElbExpr*>& arg_list,
		 BitVector& bitvector,
		 tVpiValueType req_type = kVpiValueNone) const;

  /// @brief 範囲のMSBの取得
  /// @retval 範囲のMSB 範囲を持つとき
  /// @retval NULL 範囲を持たないとき
  /// @note このクラスでは NULL を返す．
  virtual
  ElbExpr*
  _left_range() const;

  /// @brief 範囲のLSBの取得
  /// @retval 範囲のLSB 範囲を持つとき
  /// @retval NULL 範囲を持たないとき
  /// @note このクラスでは NULL を返す．
  virtual
  ElbExpr*
  _right_range() const;


private:

  /// @brief 関数の値の評価を行う．
  /// @param[in] arg_list 引数のリスト
  /// @note constant function の場合のみ意味を持つ．
  void
  evaluate(const vector<ElbExpr*>& arg_list) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力変数
  ElbDecl* mOvar;

};


//////////////////////////////////////////////////////////////////////
/// @class EiFunctionV EiaskFunc.h "EiTaskFunc.h"
/// @brief ベクタ型の関数を表すクラス
//////////////////////////////////////////////////////////////////////
class EiFunctionV :
  public EiFunction
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_item パース木の定義
  /// @param[in] io_num IOの数
  /// @param[in] io_array IO の配列
  /// @param[in] left 範囲の MSB の式
  /// @param[in] right 範囲の LSB の式
  /// @param[in] left_val 範囲の MSB の値
  /// @param[in] right_val 範囲の LSB の値
  EiFunctionV(const VlNamedObj* parent,
	      const PtItem* pt_item,
	      ymuint32 io_num,
	      EiIODecl* io_array,
	      ElbExpr* left,
	      ElbExpr* right,
	      int left_val,
	      int right_val);

  /// @brief デストラクタ
  virtual
  ~EiFunctionV();


public:
  //////////////////////////////////////////////////////////////////////
  // VlFunction の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲の MSB の値を返す．
  /// @retval 範囲のMSBの値 範囲指定を持つとき
  /// @retval -1 範囲指定を持たないとき
  virtual
  int
  left_range_const() const;

  /// @brief 範囲の LSB の値を返す．
  /// @retval 範囲のLSBの値 範囲指定を持つとき
  /// @retval -1 範囲指定を持たないとき
  virtual
  int
  right_range_const() const;

  /// @brief 出力のビット幅を返す．
  virtual
  ymuint32
  bit_size() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbFunction の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲のMSBを返す．
  virtual
  ElbExpr*
  _left_range() const;

  /// @brief 範囲のLSBを返す．
  virtual
  ElbExpr*
  _right_range() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 範囲
  EiRangeImpl mRange;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief PtItem を取り出す．
inline
const PtItem*
EiTaskFunc::pt_item() const
{
  return mPtItem;
}

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELABORATOR_EI_EITASKFUNC_H
