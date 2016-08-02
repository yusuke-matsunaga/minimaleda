#ifndef LIBYM_VERILOG_PT_SIMPLE_SPTEXPR_H
#define LIBYM_VERILOG_PT_SIMPLE_SPTEXPR_H

/// @file libym_verilog/pt_simple/SptExpr.h
/// @brief SptExpr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SptExpr.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/FileRegion.h"

#include "ym_verilog/pt/PtExpr.h"
#include "ym_verilog/pt/PtArray.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// expression を表す基底クラス
//////////////////////////////////////////////////////////////////////
class SptExpr :
  public PtExpr
{
  friend class SptFactory;

protected:

  /// コンストラクタ
  SptExpr(const FileRegion& file_region,
	  tPtExprType type);

  /// デストラクタ
  virtual
  ~SptExpr();


public:
  //////////////////////////////////////////////////////////////////////
  // PtExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// ファイル位置の取得
  virtual
  FileRegion
  file_region() const;

  /// 式の型の取得
  virtual
  tPtExprType
  type() const;

  /// @brief 演算子の種類の取得
  /// @return 演算子の種類
  /// @note このクラスでは kVpiNullOp を返す．
  virtual
  tVpiOpType
  opr_type() const;

  /// @brief 階層ブランチの取得
  /// system function call の場合は常に NULL
  /// @note このクラスでは NULL を返す．
  virtual
  PtNameBranchArray
  namebranch_array() const;

  /// @brief 末尾の名前の取得
  /// @return 末尾の名前
  /// @note このクラスでは NULL を返す．
  virtual
  const char*
  name() const;
  
  /// @brief オペランドの数の取得
  /// @return 子供の数
  virtual
  ymuint32
  operand_num() const;
  
  /// @brief オペランドの取得
  /// @param[in] pos 取り出すオペランンドの位置(最初の位置は 0)
  /// @return pos 番目のオペランド
  virtual
  const PtExpr*
  operand(ymuint32 pos) const;

  /// @brief 定数インデックスのチェック
  /// @retval true インデックスもしくは範囲が定数にならなければならないとき
  /// @retval false 上記以外
  /// @note このクラスでは false を返す．
  virtual
  bool
  is_const_index() const;

  /// @brief インデックスリストのサイズの取得
  /// @return インデックスリストのサイズ
  virtual
  ymuint32
  index_num() const;
  
  /// @brief インデックスの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < index_num() )
  virtual
  const PtExpr*
  index(ymuint32 pos) const;

  /// @brief 範囲指定モードの取得
  /// @return 範囲指定モード
  /// @note このクラスでは kVpiNoRange を返す．
  virtual
  tVpiRangeMode
  range_mode() const;
  
  /// @brief 範囲の左側の式の取得
  /// @return 範囲の左側の式
  /// @note このクラスでは NULL を返す．
  virtual
  const PtExpr*
  left_range() const;
  
  /// @brief 範囲の右側の式の取得
  /// @return 範囲の右側の式
  /// @note このクラスでは NULL を返す．
  virtual
  const PtExpr*
  right_range() const;

  /// @brief 定数の種類の取得
  /// @return 定数の種類
  /// @note このクラスでは kVpiIntConst を返す．
  virtual
  tVpiConstType
  const_type() const;

  /// @brief 整数型の定数のサイズの取得
  /// @return サイズ\n
  /// サイズ指定の無い場合と整数型の定数でない場合には 0 を返す．
  /// @note このクラスでは 0 を返す．
  virtual
  ymuint32
  const_size() const;

  /// @brief 整数型の値の取得
  /// @return 値
  /// @note このクラスでは 0 を返す．
  virtual
  ymuint32
  const_uint() const;

  /// @brief 整数型および文字列型の定数の文字列表現の取得
  /// @return 値の文字列表現\n
  /// 整数型時のサイズと基数は含まない．
  /// @note このクラスでは NULL を返す．
  virtual
  const char*
  const_str() const;

  /// @brief 実数型の値の取得
  /// @return 値
  /// @note このクラスでは 0.0 を返す．
  virtual
  double
  const_real() const;

  /// @brief インデックスとして使える式のチェック
  /// @retval true 階層名の添字として使える式
  /// @retval false 使えない式
  /// @note このクラスでは false を返す．
  virtual
  bool
  is_index_expr() const;
  
  /// @brief インデックスの値の取得
  /// @return 階層名の添字として使える式の時にその値を返す．
  /// @note このクラスでは const_uint() をキャストして返す．
  virtual
  int
  index_value() const;

  /// @brief simple primary のチェック
  /// @retval true index_list も range も持たないとき
  /// @retval false 上記以外
  /// @note このクラスでは false を返す．
  virtual
  bool
  is_simple() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 式の種類
  tPtExprType mType;

};


//////////////////////////////////////////////////////////////////////
/// オペランド数が固定の演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class SptOpr1 :
  public SptExpr
{
  friend class SptFactory;

private:
  
  /// コンストラクタ
  SptOpr1(const FileRegion& file_region,
	  tVpiOpType op_type,
	  PtExpr* opr1,
	  PtExpr* opr2 = NULL,
	  PtExpr* opr3 = NULL);

  /// デストラクタ
  virtual
  ~SptOpr1();


public:
  //////////////////////////////////////////////////////////////////////
  // PtExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// インデックスとして使える式のチェック
  virtual
  bool
  is_index_expr() const;
  
  /// インデックスの値の取得
  virtual
  int
  index_value() const;

  /// 演算子の種類の取得
  virtual
  tVpiOpType opr_type() const;
  
  /// @brief オペランドの数の取得
  /// @return 子供の数
  virtual
  ymuint32
  operand_num() const;
  
  /// pos 番目のオペランドを取り出す．
  virtual
  const PtExpr*
  operand(ymuint32 pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 演算子の種類
  tVpiOpType mOprType;

  // オペランド数
  ymuint32 mSize;
  
  // オペランドの配列
  PtExpr* mExprList[3];

};


//////////////////////////////////////////////////////////////////////
/// オペランド数が可変の演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class SptOpr2 :
  public SptExpr
{
  friend class SptFactory;

private:
  
  /// コンストラクタ
  SptOpr2(const FileRegion& file_region,
	  tVpiOpType op_type,
	  PtExprArray opr_array);

  /// デストラクタ
  virtual
  ~SptOpr2();


public:
  //////////////////////////////////////////////////////////////////////
  // PtExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// インデックスとして使える式のチェック
  virtual
  bool
  is_index_expr() const;
  
  /// インデックスの値の取得
  virtual
  int
  index_value() const;

  /// 演算子の種類の取得
  virtual
  tVpiOpType opr_type() const;
  
  /// @brief オペランドの数の取得
  /// @return 子供の数
  virtual
  ymuint32
  operand_num() const;
  
  /// pos 番目のオペランドを取り出す．
  virtual
  const PtExpr*
  operand(ymuint32 pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 演算子の種類
  tVpiOpType mOprType;

  // オペランドのリスト
  PtExprArray mExprArray;

};


//////////////////////////////////////////////////////////////////////
// function call / system function call に共通の基底クラス
//////////////////////////////////////////////////////////////////////
class SptFuncCall :
  public SptExpr
{
  friend class SptFactory;

protected:
  
  /// コンストラクタ
  SptFuncCall(const FileRegion& file_region,
	      tPtExprType type,
	      PtNameBranchArray nb_array,
	      const char* name,
	      PtExprArray arg_array);
  
  /// デストラクタ
  ~SptFuncCall();


public:
  //////////////////////////////////////////////////////////////////////
  // PtExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// 階層ブランチを返す．
  virtual
  PtNameBranchArray
  namebranch_array() const;

  /// 末尾の名前を返す．
  virtual
  const char*
  name() const;
  
  /// @brief オペランドの数の取得
  /// @return 子供の数
  ymuint32
  operand_num() const;
  
  /// @brief オペランドの取得
  /// @param[in] pos 取り出すオペランンドの位置(最初の位置は 0)
  /// @return pos 番目のオペランド
  const PtExpr*
  operand(ymuint32 pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 階層ブランチの配列
  PtNameBranchArray mNbArray;
  
  // 末尾の名前
  const char* mName;

  // 引数リスト
  PtExprArray mArgArray;

};


//////////////////////////////////////////////////////////////////////
// PtPrimary のベース実装クラス
//////////////////////////////////////////////////////////////////////
class SptPrimary :
  public SptExpr
{
  friend class SptFactory;

private:

  /// コンストラクタ
  SptPrimary(const FileRegion& file_region,
	     PtNameBranchArray nb_array,
	     const char* tail_name,
	     bool const_index,
	     PtExprArray index_array = PtExprArray(),
	     tVpiRangeMode mode = kVpiNoRange,
	     PtExpr* left = NULL,
	     PtExpr* right = NULL);

  /// デストラクタ
  virtual
  ~SptPrimary();


public:
  //////////////////////////////////////////////////////////////////////
  // PtExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// 階層ブランチを取り出す．
  virtual
  PtNameBranchArray
  namebranch_array() const;

  /// 末尾の名前を取り出す．
  virtual
  const char*
  name() const;

  /// インデックスもしくは範囲が定数にならなければならないとき true を返す．
  virtual
  bool
  is_const_index() const;

  /// @brief インデックスリストのサイズの取得
  /// @return インデックスリストのサイズ
  virtual
  ymuint32
  index_num() const;
  
  /// @brief インデックスの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < index_num() )
  virtual
  const PtExpr*
  index(ymuint32 pos) const;

  /// 範囲指定モードの取得
  virtual
  tVpiRangeMode
  range_mode() const;
  
  /// range の MSB を取出す．
  virtual
  const PtExpr*
  left_range() const;
  
  /// range の LSB を取出す．
  virtual
  const PtExpr*
  right_range() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 階層ブランチのリスト
  PtNameBranchArray mNbArray;

  // 末尾の名前
  const char* mName;

  // インデックス/範囲が定数の時 true
  bool mConstIndex;

  // インデックスの配列
  PtExprArray mIndexArray;

  // 範囲のモード
  tVpiRangeMode mMode;

  // 範囲のMSB
  PtExpr* mLeftRange;

  // 範囲のLSB
  PtExpr* mRightRange;

};


//////////////////////////////////////////////////////////////////////
// 定数
//////////////////////////////////////////////////////////////////////
class SptConstant :
  public SptExpr
{
  friend class SptFactory;

private:

  /// コンストラクタ
  SptConstant(const FileRegion& file_region,
	      tVpiConstType const_type,
	      ymuint32 size,
	      ymuint32 uvalue,
	      const char* svalue,
	      double rvalue);

  /// デストラクタ
  virtual
  ~SptConstant();


public:
  //////////////////////////////////////////////////////////////////////
  // PtExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// 階層名の添字として使える式の時に true を返す．
  virtual
  bool
  is_index_expr() const;

  /// 定数の種類を表す型を返す．
  virtual
  tVpiConstType
  const_type() const;

  // 整数型の定数のサイズの取得
  virtual
  ymuint32
  const_size() const;

  // 整数型の値の取得
  virtual
  ymuint32
  const_uint() const;

  // 文字列型の値の取得
  virtual
  const char*
  const_str() const;

  // 実数型の値の取得
  virtual
  double
  const_real() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 定数の型
  tVpiConstType mConstType;

  // サイズ
  // 0 の時は指定なし
  ymuint32 mSize;

  // 整数の値
  ymuint32 mUintValue;

  // 文字列の値
  const char* mStrValue;

  // 実数の値
  double mRealValue;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_PT_SIMPLE_SPTEXPR_H
