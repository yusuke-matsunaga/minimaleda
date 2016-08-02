
/// @file libym_verilog/pt_simple/SptExpr.cc
/// @brief SptExpr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SptExpr.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "SptExpr.h"
#include "SptFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// expression を表す基底クラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] file_region ファイル位置の情報
// @param[in] type 式の種類を表す型
SptExpr::SptExpr(const FileRegion& file_region,
		 tPtExprType type) :
  mFileRegion(file_region),
  mType(type)
{
}

// @brief デストラクタ
SptExpr::~SptExpr()
{
}
   
// @brief ファイル位置の取得
// @return ファイル位置
FileRegion
SptExpr::file_region() const
{
  return mFileRegion;
}

// @brief 式の型の取得
// @return 式の型
tPtExprType
SptExpr::type() const
{
  return mType;
}

// @brief 演算子の種類の取得
// @return 演算子の種類
// このクラスでは kVpiNullOp を返す．
tVpiOpType
SptExpr::opr_type() const
{
  return kVpiNullOp;
}

// @brief 階層ブランチの取得
// system function call の場合は常に NULL
// このクラスでは NULL を返す．
PtNameBranchArray
SptExpr::namebranch_array() const
{
  return PtNameBranchArray();
}

// @brief 末尾の名前の取得
// @return 末尾の名前
// このクラスでは NULL を返す．
const char*
SptExpr::name() const
{
  return NULL;
}
  
// @brief オペランドの数の取得
// @return 子供の数
ymuint32
SptExpr::operand_num() const
{
  return 0;
}

// @brief オペランドの取得
// @param[in] pos 取り出すオペランンドの位置(最初の位置は 0)
// @return pos 番目のオペランド
const PtExpr*
SptExpr::operand(ymuint32 pos) const
{
  return NULL;
}

// @brief 定数インデックスのチェック
// @retval true インデックスもしくは範囲が定数にならなければならないとき
// @retval false 上記以外
// このクラスでは false を返す．
bool
SptExpr::is_const_index() const
{
  return false;
}

// @brief インデックスリストのサイズの取得
// @return インデックスリストのサイズ
ymuint32
SptExpr::index_num() const
{
  return 0;
}

// @brief インデックスの取得
// @param[in] pos 位置番号 ( 0 <= pos < index_num() )
const PtExpr*
SptExpr::index(ymuint32 pos) const
{
  return NULL;
}

// @brief 範囲指定モードの取得
// @return 範囲指定モード
// このクラスでは kVpiNoRange を返す．
tVpiRangeMode
SptExpr::range_mode() const
{
  return kVpiNoRange;
}
  
// @brief 範囲の左側の式の取得
// @return 範囲の左側の式
// このクラスでは NULL を返す．
const PtExpr*
SptExpr::left_range() const
{
  return NULL;
}
  
// @brief 範囲の右側の式の取得
// @return 範囲の右側の式
// このクラスでは NULL を返す．
const PtExpr*
SptExpr::right_range() const
{
  return NULL;
}

// @brief 定数の種類の取得
// @return 定数の種類
// このクラスでは kVpiIntConst を返す．
tVpiConstType
SptExpr::const_type() const
{
  return kVpiIntConst; // ダミー
}

// @brief 整数型の定数のサイズの取得
// @return サイズ\n
// サイズ指定の無い場合と整数型の定数でない場合には 0 を返す．
// このクラスでは 0 を返す．
ymuint32
SptExpr::const_size() const
{
  return 0;
}

// @brief 整数型の値の取得
// @return 値
// このクラスでは 0 を返す．
ymuint32
SptExpr::const_uint() const
{
  return 0;
}

// @brief 整数型および文字列型の定数の文字列表現の取得
// @return 値の文字列表現\n
// 整数型のサイズと基数は含まない．
// このクラスでは NULL を返す．
const char*
SptExpr::const_str() const
{
  return NULL;
}

// @brief 実数型の値の取得
// @return 値
// このクラスでは 0.0 を返す．
double
SptExpr::const_real() const
{
  return 0.0;
}

// @brief インデックスとして使える式のチェック
// @retval true 階層名の添字として使える式
// @retval false 使えない式
// このクラスでは false を返す．
bool
SptExpr::is_index_expr() const
{
  return false;
}
  
// @brief インデックスの値の取得
// @return 階層名の添字として使える式の時にその値を返す．
// このクラスでは const_uint() をキャストして返す．
int
SptExpr::index_value() const
{
  return static_cast<int>(const_uint());
}

// @brief simple primary のチェック
// @retval true index_list も range も持たないとき
// @retval false 上記以外
bool
SptExpr::is_simple() const
{
  if ( type() == kPtPrimaryExpr &&
       index_num() == 0 &&
       left_range() == NULL ) {
    return true;
  }
  return false;
}


//////////////////////////////////////////////////////////////////////
// 演算子のベース実装クラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] file_reigon ファイル位置の情報
// @param[in] ai_list 属性インスタンスのリスト
// @param[in] op_type 演算子の種類
// @param[in] opr_list　オペランドのリスト
SptOpr1::SptOpr1(const FileRegion& file_region,
		 tVpiOpType op_type,
		 PtExpr* opr1,
		 PtExpr* opr2,
		 PtExpr* opr3) :
  SptExpr(file_region, kPtOprExpr),
  mOprType(op_type)
{
  mExprList[0] = opr1;
  if ( (mExprList[1] = opr2) ) {
    if ( (mExprList[2] = opr3) ) {
      mSize = 3;
    }
    else {
      mSize = 2;
    }
  }
  else {
    mSize = 1;
  }
}

// @brief デストラクタ
SptOpr1::~SptOpr1()
{
}

// @brief 演算子の種類の取得
// @return 演算子の種類
tVpiOpType
SptOpr1::opr_type() const
{
  return mOprType;
}

// @brief インデックスとして使える式のチェック
// @retval true 階層名の添字として使える式
// @retval false 使えない式
bool
SptOpr1::is_index_expr() const
{
  // 算術演算なら本当はOKだけどめんどくさいので単項のマイナスのみOKとする．
  if ( mOprType == kVpiMinusOp || mOprType == kVpiNullOp ) {
    return operand(0)->is_index_expr();
  }
  else {
    return false;
  }
}
  
// @brief インデックスの値の取得
// @return 階層名の添字として使える式の時にその値を返す．
int
SptOpr1::index_value() const
{
  if ( mOprType == kVpiNullOp ) {
    return operand(0)->index_value();
  }
  if ( mOprType == kVpiMinusOp ) {
    return - operand(0)->index_value();
  }
  return 0;
}

// @brief オペランドの数の取得
// @return オペランドの数
ymuint32
SptOpr1::operand_num() const
{
  return mSize;
}
  
// @brief オペランドを取り出す
// @param[in] pos 取り出すオペランドの位置（最初の位置は 0）
// @return pos 番目のオペランド
const PtExpr*
SptOpr1::operand(ymuint32 pos) const
{
  return mExprList[pos];
}


//////////////////////////////////////////////////////////////////////
// 演算子のベース実装クラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] file_reigon ファイル位置の情報
// @param[in] op_type 演算子の種類
// @param[in] opr_array　オペランドのリスト
SptOpr2::SptOpr2(const FileRegion& file_region,
		 tVpiOpType op_type,
		 PtExprArray opr_array) :
  SptExpr(file_region, kPtOprExpr),
  mOprType(op_type),
  mExprArray(opr_array)
{
}

// @brief デストラクタ
SptOpr2::~SptOpr2()
{
}

// @brief 演算子の種類の取得
// @return 演算子の種類
tVpiOpType
SptOpr2::opr_type() const
{
  return mOprType;
}

// @brief インデックスとして使える式のチェック
// @retval true 階層名の添字として使える式
// @retval false 使えない式
bool
SptOpr2::is_index_expr() const
{
  return false;
}
  
// @brief インデックスの値の取得
// @return 階層名の添字として使える式の時にその値を返す．
int
SptOpr2::index_value() const
{
  return 0;
}

// @brief オペランドの数の取得
// @return オペランドの数
ymuint32
SptOpr2::operand_num() const
{
  return mExprArray.size();
}
  
// @brief オペランドの取得
// @param[in] pos 取り出すオペランンドの位置(最初の位置は 0)
// @return pos 番目のオペランド
const PtExpr*
SptOpr2::operand(ymuint32 pos) const
{
  return mExprArray[pos];
}


//////////////////////////////////////////////////////////////////////
// function call / system function call に共通の基底クラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] file_region ファイル位置の情報
// @param[in] ai_list 属性インスタンスのリスト
// @param[in] type 関数の型(kFuncCall or kSysFuncCall)
// @param[in] nblist 階層名のリスト
// @param[in] name 名前
// @param[in] arg_list 引数のリスト
SptFuncCall::SptFuncCall(const FileRegion& file_region,
			 tPtExprType type,
			 PtNameBranchArray nb_array,
			 const char* name,
			 PtExprArray arg_array) :
  SptExpr(file_region, type),
  mNbArray(nb_array),
  mName(name),
  mArgArray(arg_array)
{
}

// @brief デストラクタ
SptFuncCall::~SptFuncCall()
{
}

// @brief 階層ブランチの取得
PtNameBranchArray
SptFuncCall::namebranch_array() const
{
  return mNbArray;
}

// @brief 末尾の名前の取得
// @return 末尾の名前
const char*
SptFuncCall::name() const
{
  return mName;
}
  
// @brief 引数の数の取得
// @return 引数の数
ymuint32
SptFuncCall::operand_num() const
{
  return mArgArray.size();
}
  
// @brief オペランドの取得
// @param[in] pos 取り出すオペランンドの位置(最初の位置は 0)
// @return pos 番目のオペランド
const PtExpr*
SptFuncCall::operand(ymuint32 pos) const
{
  return mArgArray[pos];
}


//////////////////////////////////////////////////////////////////////
// PtPrimary の実装クラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] file_region ファイル位置の情報
// @param[in] nblist 階層名のリスト
// @param[in] tail_name 末尾のリスト
// @param[in] const_index インデックスが定数でなければいけないとき true となるフラグ
// @param[in] index_array インデックスのリスト
// @param[in] mode 範囲指定のモード
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
SptPrimary::SptPrimary(const FileRegion& file_region,
		       PtNameBranchArray nb_array,
		       const char* tail_name,
		       bool const_index,
		       PtExprArray index_array,
		       tVpiRangeMode mode,
		       PtExpr* left,
		       PtExpr* right) :
  SptExpr(file_region, kPtPrimaryExpr),
  mNbArray(nb_array),
  mName(tail_name),
  mConstIndex(const_index),
  mIndexArray(index_array),
  mMode(mode),
  mLeftRange(left),
  mRightRange(right)
{
}

// @brief デストラクタ
SptPrimary::~SptPrimary()
{
}

// @brief 階層ブランチの取得
PtNameBranchArray
SptPrimary::namebranch_array() const
{
  return mNbArray;
}

// @brief 末尾の名前の取得
// @return 末尾の名前
const char*
SptPrimary::name() const
{
  return mName;
}

// @brief 定数インデックスのチェック
// @retval true インデックスもしくは範囲が定数にならなければならないとき
// @retval false 上記以外
bool
SptPrimary::is_const_index() const
{
  return mConstIndex;
}

// @brief インデックスリストのサイズの取得
// @return インデックスリストのサイズ
ymuint32
SptPrimary::index_num() const
{
  return mIndexArray.size();
}
  
// @brief インデックスの取得
// @param[in] pos 位置番号 ( 0 <= pos < index_num() )
const PtExpr*
SptPrimary::index(ymuint32 pos) const
{
  return mIndexArray[pos];
}

// @brief 範囲指定モードの取得
// @return 範囲指定モード
tVpiRangeMode
SptPrimary::range_mode() const
{
  return mMode;
}
  
// @brief 範囲の左側の式の取得
// @return 範囲の左側の式
const PtExpr*
SptPrimary::left_range() const
{
  return mLeftRange;
}
  
// @brief 範囲の右側の式の取得
// @return 範囲の右側の式
const PtExpr*
SptPrimary::right_range() const
{
  return mRightRange;
}


//////////////////////////////////////////////////////////////////////
// 定数
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] file_region ファイル位置の情報
// @param[in] const_type 定数の型
// @param[in] size ビットサイズ
// @param[in] base 基数
// @param[in] svalue 値の文字列表現
// @param[in] rvalue 実数型の時の値
SptConstant::SptConstant(const FileRegion& file_region,
			 tVpiConstType const_type,
			 ymuint32 size,
			 ymuint32 uvalue,
			 const char* svalue,
			 double rvalue) :
  SptExpr(file_region, kPtConstExpr),
  mConstType(const_type),
  mSize(size),
  mUintValue(uvalue),
  mStrValue(svalue),
  mRealValue(rvalue)
{
}

// @brief デストラクタ
SptConstant::~SptConstant()
{
}

// @brief 整数型の定数のサイズの取得
// @return サイズ
ymuint32
SptConstant::const_size() const
{
  return mSize;
}

// @brief 整数型の値の取得
// @return 整数型の値
ymuint32
SptConstant::const_uint() const
{
  return mUintValue;
}

// @brief 整数型および文字列型の定数の文字列表現の取得
// @return 文字列型の値
const char*
SptConstant::const_str() const
{
  return mStrValue;
}

// @brief 実数型の値の取得
// @return 実数値
double
SptConstant::const_real() const
{
  return mRealValue;
}

// @brief インデックスとして使える式のチェック
// @retval true インデックスとして使える場合
// @retval false 使えない場合
bool
SptConstant::is_index_expr() const
{
  // ただの整数の場合のみが使える．
  return const_type() == kVpiIntConst && const_str() == NULL;
}

// @brief 定数の種類の取得
// @return 定数の種類
tVpiConstType
SptConstant::const_type() const
{
  return mConstType;
}

  
//////////////////////////////////////////////////////////////////////
// expression 関係
//////////////////////////////////////////////////////////////////////

// @brief 単項演算子の生成
// @param[in] file_region ファイル位置の情報
// @param[in] type 演算の種類
// @param[in] opr オペランド
// @return 生成された演算子
PtExpr*
SptFactory::new_Opr(const FileRegion& file_region,
		    tVpiOpType type,
		    PtExpr* opr)
{
  void* p = alloc().get_memory(sizeof(SptOpr1));
  return new (p) SptOpr1(file_region, type,
			 opr, NULL, NULL);
}

// @brief 二項演算子の生成
// @param[in] file_region ファイル位置の情報
// @param[in] type 演算の種類
// @param[in] opr1 オペランド1
// @param[in] opr2 オペランド2
// @return 生成された演算子
PtExpr*
SptFactory::new_Opr(const FileRegion& file_region,
		    tVpiOpType type,
		    PtExpr* opr1,
		    PtExpr* opr2)
{
  void* p = alloc().get_memory(sizeof(SptOpr1));
  return new (p) SptOpr1(file_region, type,
			 opr1, opr2, NULL);
}

// @brief 三項演算子の生成
// @param[in] file_region ファイル位置の情報
// @param[in] type 演算の種類
// @param[in] opr1 オペランド1
// @param[in] opr2 オペランド2
// @param[in] opr3 オペランド3
// @return 生成された演算子
PtExpr*
SptFactory::new_Opr(const FileRegion& file_region,
		    tVpiOpType type,
		    PtExpr* opr1,
		    PtExpr* opr2,
		    PtExpr* opr3)
{
  void* p = alloc().get_memory(sizeof(SptOpr1));
  return new (p) SptOpr1(file_region, type,
			 opr1, opr2, opr3);
}

// @brief concatination 演算子の生成
// @param[in] file_region ファイル位置の情報
// @param[in] expr_list オペランドのリスト
// @return 生成された concatination 演算子
PtExpr*
SptFactory::new_Concat(const FileRegion& file_region,
		       PtExprArray expr_array)
{
  void* p = alloc().get_memory(sizeof(SptOpr2));
  return new (p) SptOpr2(file_region, kVpiConcatOp, expr_array);
}

// @brief multi-concatination 演算子の生成
// @param[in] file_region ファイル位置の情報
// @param[in] expr_list 結合するオペランドのリスト
// @return 生成された multi-concatination 演算子
PtExpr*
SptFactory::new_MultiConcat(const FileRegion& file_region,
			    PtExprArray expr_array)
{
  void* p = alloc().get_memory(sizeof(SptOpr2));
  return new (p) SptOpr2(file_region, kVpiMultiConcatOp, expr_array);
}

// @brief min/typ/max delay 演算子の生成
// @param[in] file_region ファイル位置の情報
// @param[in] val0 minimum 値
// @param[in] val1 typical 値
// @param[in] val2 maximum 値
// @return 生成された min/typ/max 演算子
PtExpr*
SptFactory::new_MinTypMax(const FileRegion& file_region,
			  PtExpr* val0,
			  PtExpr* val1,
			  PtExpr* val2)
{
  void* p = alloc().get_memory(sizeof(SptOpr1));
  return new (p) SptOpr1(file_region, kVpiMinTypMaxOp,
			 val0, val1, val2);
}

// @brief primary の生成
// @param[in] file_region ファイル位置の情報
// @param[in] name 識別子名
// @return 生成された primary
PtExpr*
SptFactory::new_Primary(const FileRegion& file_region,
			const char* name)
{
  void* p = alloc().get_memory(sizeof(SptPrimary));
  return new (p) SptPrimary(file_region,
			    PtNameBranchArray(),
			    name,
			    false);
}

// @brief インデックス付き primary の生成
// @param[in] file_region ファイル位置の情報
// @param[in] name 識別子名
// @param[in] index_array インデックスのリスト
// @return 生成された primary
PtExpr*
SptFactory::new_Primary(const FileRegion& file_region,
			const char* name,
			PtExprArray index_array)
{
  void* p = alloc().get_memory(sizeof(SptPrimary));
  return new (p) SptPrimary(file_region,
			    PtNameBranchArray(),
			    name,
			    false,
			    index_array);
}

// @brief 範囲指定付き primary の生成
// @param[in] file_region ファイル位置の情報
// @param[in] name 識別子名
// @param[in] mode 範囲指定のモード
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @return 生成された primary
PtExpr*
SptFactory::new_Primary(const FileRegion& file_region,
			const char* name,
			tVpiRangeMode mode,
			PtExpr* left,
			PtExpr* right)
{
  void* p = alloc().get_memory(sizeof(SptPrimary));
  return new (p) SptPrimary(file_region,
			    PtNameBranchArray(),
			    name,
			    false,
			    PtExprArray(),
			    mode, left, right);
}

// @brief インデックスと範囲指定付き primary の生成
// @param[in] file_region ファイル位置の情報
// @param[in] name 識別子名
// @param[in] index_array インデックスのリスト
// @param[in] mode 範囲指定のモード
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @return 生成された primary
PtExpr*
SptFactory::new_Primary(const FileRegion& file_region,
			const char* name,
			PtExprArray index_array,
			tVpiRangeMode mode,
			PtExpr* left,
			PtExpr* right)
{
  void* p = alloc().get_memory(sizeof(SptPrimary));
  return new (p) SptPrimary(file_region,
			    PtNameBranchArray(),
			    name,
			    false,
			    index_array,
			    mode, left, right);
}

// @brief primary の生成 (階層付き)
// @param[in] file_region ファイル位置の情報
// @param[in] nb_array 識別子の階層部分のリスト
// @param[in] tail_name 識別子の末尾
// @return 生成された primary
PtExpr*
SptFactory::new_Primary(const FileRegion& file_region,
			PtNameBranchArray nb_array,
			const char* tail_name)
{
  void* p = alloc().get_memory(sizeof(SptPrimary));
  return new (p) SptPrimary(file_region,
			    nb_array,
			    tail_name,
			    false);
}

// @brief インデックス付き primary の生成 (階層付き)
// @param[in] file_region ファイル位置の情報
// @param[in] nb_array 識別子の階層部分のリスト
// @param[in] tail_name 識別子の末尾
// @param[in] index_array インデックスのリスト
// @return 生成された primary
PtExpr*
SptFactory::new_Primary(const FileRegion& file_region,
			PtNameBranchArray nb_array,
			const char* tail_name,
			PtExprArray index_array)
{
  void* p = alloc().get_memory(sizeof(SptPrimary));
  return new (p) SptPrimary(file_region,
			    nb_array,
			    tail_name,
			    false,
			    index_array);
}

// @brief 範囲指定付き primary の生成 (階層付き)
// @param[in] file_region ファイル位置の情報
// @param[in] nb_array 識別子の階層部分のリスト
// @param[in] tail_name 識別子の末尾
// @param[in] mode 範囲指定のモード
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @return 生成された primary
PtExpr*
SptFactory::new_Primary(const FileRegion& file_region,
			PtNameBranchArray nb_array,
			const char* tail_name,
			tVpiRangeMode mode,
			PtExpr* left,
			PtExpr* right)
{
  void* p = alloc().get_memory(sizeof(SptPrimary));
  return new (p) SptPrimary(file_region,
			    nb_array,
			    tail_name,
			    false,
			    PtExprArray(),
			    mode, left, right);
}

// @brief インデックスと範囲指定付き primary の生成 (階層付き)
// @param[in] file_region ファイル位置の情報
// @param[in] nb_array 識別子の階層部分のリスト
// @param[in] tail_name 識別子の末尾
// @param[in] index_array インデックスのリスト
// @param[in] mode 範囲指定のモード
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @return 生成された primary
PtExpr*
SptFactory::new_Primary(const FileRegion& file_region,
			PtNameBranchArray nb_array,
			const char* tail_name,
			PtExprArray index_array,
			tVpiRangeMode mode,
			PtExpr* left,
			PtExpr* right)
{
  void* p = alloc().get_memory(sizeof(SptPrimary));
  return new (p) SptPrimary(file_region,
			    nb_array,
			    tail_name,
			    false,
			    index_array,
			    mode, left, right);
}

// @brief constant primary の生成
// @param[in] file_region ファイル位置の情報
// @param[in] name 識別子名
// @param[in] index_array インデックスのリスト
// @return 生成された const primary
PtExpr*
SptFactory::new_CPrimary(const FileRegion& file_region,
			 const char* name,
			 PtExprArray index_array)
{
  void* p = alloc().get_memory(sizeof(SptPrimary));
  return new (p) SptPrimary(file_region,
			    PtNameBranchArray(),
			    name,
			    true,
			    index_array);
}

// @brief 範囲指定付き constant primary の生成
// @param[in] file_region ファイル位置の情報
// @param[in] name 識別子名
// @param[in] mode 範囲指定のモード
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @return 生成された constant primary
PtExpr*
SptFactory::new_CPrimary(const FileRegion& file_region,
			 const char* name,
			 tVpiRangeMode mode,
			 PtExpr* left,
			 PtExpr* right)
{
  void* p = alloc().get_memory(sizeof(SptPrimary));
  return new (p) SptPrimary(file_region,
			    PtNameBranchArray(),
			    name,
			    true,
			    PtExprArray(),
			    mode, left, right);
}

// @brief インデックス付き constant primary の生成 (階層付き)
// @param[in] file_region ファイル位置の情報
// @param[in] nb_array 識別子の階層部分のリスト
// @param[in] tail_name 識別子の末尾
// @param[in] index_array インデックスのリスト
// @return 生成された constant primary
PtExpr*
SptFactory::new_CPrimary(const FileRegion& file_region,
			 PtNameBranchArray nb_array,
			 const char* tail_name,
			 PtExprArray index_array)
{
  void* p = alloc().get_memory(sizeof(SptPrimary));
  return new (p) SptPrimary(file_region,
			    nb_array,
			    tail_name,
			    true,
			    index_array);
}

// @brief function call の生成
// @param[in] file_region ファイル位置の情報
// @param[in] ai_top 属性インスタンスのリスト
// @param[in] name 関数名
// @param[in] arg_array 引数のリスト
// @return 生成された function call
PtExpr*
SptFactory::new_FuncCall(const FileRegion& file_region,
			 const char* name,
			 PtExprArray arg_array)
{
  void* p = alloc().get_memory(sizeof(SptFuncCall));
  return new (p) SptFuncCall(file_region, kPtFuncCallExpr,
			     PtNameBranchArray(),
			     name, arg_array);
}

// @brief function call の生成 (階層付き)
// @param[in] file_region ファイル位置の情報
// @param[in] nb_array 関数名の階層部分のリスト
// @param[in] tail_name 関数名の末尾
// @param[in] arg_array 引数のリスト
// @return 生成された function call
PtExpr*
SptFactory::new_FuncCall(const FileRegion& file_region,
			 PtNameBranchArray nb_array,
			 const char* tail_name,
			 PtExprArray arg_array)
{
  void* p = alloc().get_memory(sizeof(SptFuncCall));
  return new (p) SptFuncCall(file_region, kPtFuncCallExpr,
			     nb_array, tail_name, arg_array);
}

// @brief system function call の生成
// @param[in] file_region ファイル位置の情報
// @param[in] name 関数名
// @param[in] arg_array 引数のリスト
// @return 生成された function call
PtExpr*
SptFactory::new_SysFuncCall(const FileRegion& file_region,
			    const char* name,
			    PtExprArray arg_array)
{
  void* p = alloc().get_memory(sizeof(SptFuncCall));
  return new (p) SptFuncCall(file_region, kPtSysFuncCallExpr,
			     PtNameBranchArray(),
			     name, arg_array);
}

// @brief 整数型の定数の生成
// @param[in] file_region ファイル位置の情報
// @param[in] value 定数の文字列表現
// @return 生成された定数
PtExpr*
SptFactory::new_IntConst(const FileRegion& file_region,
			 ymuint32 value)
{
  void* p = alloc().get_memory(sizeof(SptConstant));
  return new (p) SptConstant(file_region, kVpiIntConst,
			     0, value, NULL, 0.0);
}

// @brief 整数型の定数の生成
// @param[in] file_region ファイル位置の情報
// @param[in] value 定数の文字列表現
// @return 生成された定数
PtExpr*
SptFactory::new_IntConst(const FileRegion& file_region,
			 const char* value)
{
  void* p = alloc().get_memory(sizeof(SptConstant));
  return new (p) SptConstant(file_region, kVpiIntConst,
			     0, 0, value, 0.0);
}

// @brief 基底付き整数型の定数の生成
// @param[in] file_region ファイル位置の情報
// @param[in] const_type 定数の種類
// @param[in] value 定数の文字列表現
// @return 生成された定数
PtExpr*
SptFactory::new_IntConst(const FileRegion& file_region,
			 tVpiConstType const_type,
			 const char* value)
{
  void* p = alloc().get_memory(sizeof(SptConstant));
  return new (p) SptConstant(file_region, const_type,
			     0, 0, value, 0.0);
}

// @brief サイズと基底付き定数の生成
// @param[in] file_region ファイル位置の情報
// @param[in] size サイズ
// @param[in] const_type 定数の種類
// @param[in] value 定数の文字列表現
// @return 生成された定数
PtExpr*
SptFactory::new_IntConst(const FileRegion& file_region,
			 ymuint32 size,
			 tVpiConstType const_type,
			 const char* value)
{
  void* p = alloc().get_memory(sizeof(SptConstant));
  return new (p) SptConstant(file_region, const_type,
			     size, 0, value, 0.0);
}

// @brief 実数型の定数の生成
// @param[in] file_region ファイル位置の情報
// @param[in] value 定数の文字列表現
// @return 生成された定数
PtExpr*
SptFactory::new_RealConst(const FileRegion& file_region,
			  double value)
{
  void* p = alloc().get_memory(sizeof(SptConstant));
  return new (p) SptConstant(file_region, kVpiRealConst,
			     0, 0, NULL, value);
}

// @brief 文字列型の定数の生成
// @param[in] file_region ファイル位置の情報
// @param[in] value 文字列
// @return 生成された定数
PtExpr*
SptFactory::new_StringConst(const FileRegion& file_region,
			    const char* value)
{
  void* p = alloc().get_memory(sizeof(SptConstant));
  return new (p) SptConstant(file_region, kVpiStringConst,
			     0, 0, value, 0.0);
}

END_NAMESPACE_YM_VERILOG
