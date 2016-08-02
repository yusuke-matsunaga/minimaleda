
/// @file libym_verilog/pt_compact/CptExpr.cc
/// @brief CptExpr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CptExpr.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "CptExpr.h"
#include "CptFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// expression を表す基底クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptExpr::CptExpr()
{
}

// デストラクタ
CptExpr::~CptExpr()
{
}

// @brief 演算子の種類の取得
// @return 演算子の種類
// このクラスでは kVpiNullOp を返す．
tVpiOpType
CptExpr::opr_type() const
{
  return kVpiNullOp;
}
  
// @brief 階層ブランチの取得
PtNameBranchArray
CptExpr::namebranch_array() const
{
  return PtNameBranchArray();
}

// @brief 末尾の名前の取得
// @return 末尾の名前
// このクラスでは NULL を返す．
const char*
CptExpr::name() const
{
  return NULL;
}

// @brief オペランドの数の取得
// @return 子供の数
ymuint32
CptExpr::operand_num() const
{
  return 0;
}
  
// @brief オペランドの取得
// @param[in] pos 取り出すオペランンドの位置(最初の位置は 0)
// @return pos 番目のオペランド
const PtExpr*
CptExpr::operand(ymuint32 pos) const
{
  return NULL;
}

// @brief 定数インデックスのチェック
// @retval true インデックスもしくは範囲が定数にならなければならないとき
// @retval false 上記以外
// このクラスでは false を返す．
bool
CptExpr::is_const_index() const
{
  return false;
}

// @brief インデックスリストのサイズの取得
// @return インデックスリストのサイズ
ymuint32
CptExpr::index_num() const
{
  return 0;
}
  
// @brief インデックスの取得
// @param[in] pos 位置番号 ( 0 <= pos < index_num() )
const PtExpr*
CptExpr::index(ymuint32 pos) const
{
  return NULL;
}

// @brief 範囲指定モードの取得
// @return 範囲指定モード
// このクラスでは kVpiNoRange を返す．
tVpiRangeMode
CptExpr::range_mode() const
{
  return kVpiNoRange;
}
  
// @brief 範囲の左側の式の取得
// @return 範囲の左側の式
// このクラスでは NULL を返す．
const PtExpr*
CptExpr::left_range() const
{
  return NULL;
}
  
// @brief 範囲の右側の式の取得
// @return 範囲の右側の式
// このクラスでは NULL を返す．
const PtExpr*
CptExpr::right_range() const
{
  return NULL;
}

// @brief 定数の種類の取得
// @return 定数の種類
// このクラスでは kVpiIntConst を返す．
tVpiConstType
CptExpr::const_type() const
{
  return kVpiIntConst; // ダミー
}

// @brief 整数型の定数のサイズの取得
// @return サイズ\n
// サイズ指定の無い場合と整数型の定数でない場合には 0 を返す．
// このクラスでは 0 を返す．
ymuint32
CptExpr::const_size() const
{
  return 0;
}

// @brief 整数型の値の取得
// @return 値
// このクラスでは 0 を返す．
ymuint32
CptExpr::const_uint() const
{
  return 0;
}

// @brief 整数型および文字列型の定数の文字列表現の取得
// @return 値の文字列表現\n
// 整数型のサイズと基数は含まない．
// このクラスでは NULL を返す．
const char*
CptExpr::const_str() const
{
  return NULL;
}

// @brief 実数型の値の取得
// @return 値
// このクラスでは 0.0 を返す．
double
CptExpr::const_real() const
{
  return 0.0;
}

// @brief インデックスとして使える式のチェック
// @retval true 階層名の添字として使える式
// @retval false 使えない式
// このクラスでは false を返す．
bool
CptExpr::is_index_expr() const
{
  return false;
}
  
// @brief インデックスの値の取得
// @return 階層名の添字として使える式の時にその値を返す．
// このクラスでは const_uint() をキャストして返す．
int
CptExpr::index_value() const
{
  return static_cast<int>(const_uint());
}

// @brief simple primary のチェック
// @retval true index_list も range も持たないとき
// @retval false 上記以外
// @note このクラスでは false を返す．
bool
CptExpr::is_simple() const
{
  return false;
}


//////////////////////////////////////////////////////////////////////
// 演算子のベース実装クラス
//////////////////////////////////////////////////////////////////////
  
// コンストラクタ
CptOpr::CptOpr(tVpiOpType op_type) :
  mOpType(op_type)
{
}

// デストラクタ
CptOpr::~CptOpr()
{
}

// クラスの型を返す．
tPtExprType
CptOpr::type() const
{
  return kPtOprExpr;
}

// 演算子のトークン番号を得る．
tVpiOpType
CptOpr::opr_type() const
{
  return mOpType;
}


//////////////////////////////////////////////////////////////////////
// 単項演算子を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptOpr1::CptOpr1(const FileRegion& file_region,
		 tVpiOpType op_type,
		 PtExpr* opr) :
  CptOpr(op_type),
  mFileRegion(file_region),
  mOpr(opr)
{
  assert_cond(opr, __FILE__, __LINE__);
}

// デストラクタ
CptOpr1::~CptOpr1()
{
}

// ファイル位置を返す．
FileRegion
CptOpr1::file_region() const
{
  return mFileRegion;
}

// 階層名の添字として使える式の時に true を返す．
bool
CptOpr1::is_index_expr() const
{
  // 算術演算はOKだけどめんどくさいので単項のマイナスのみOKとする．
  if ( opr_type() == 0 || opr_type() == vpiMinusOp ) {
    return operand(0)->is_index_expr();
  }
  else {
    return false;
  }
}
  
// 階層名の添字として使える式の時にその値を返す．
int
CptOpr1::index_value() const
{
  if ( opr_type() == 0 ) {
    return operand(0)->index_value();
  }
  if ( opr_type() == vpiMinusOp ) {
    return - operand(0)->index_value();
  }
  return 0;
}
  
// @brief オペランドの数の取得
// @return 子供の数
ymuint32
CptOpr1::operand_num() const
{
  return 1;
}

// @brief オペランドの取得
// @param[in] pos 取り出すオペランンドの位置(最初の位置は 0)
// @return pos 番目のオペランド
const PtExpr*
CptOpr1::operand(ymuint32 pos) const
{
  if ( pos == 0 ) {
    return mOpr;
  }
  else {
    return NULL;
  }
}


//////////////////////////////////////////////////////////////////////
// 二項演算子を表すクラス
//////////////////////////////////////////////////////////////////////
  
// コンストラクタ
CptOpr2::CptOpr2(tVpiOpType op_type,
		 PtExpr* opr1,
		 PtExpr* opr2) :
  CptOpr(op_type)
{
  mOpr[0] = opr1;
  mOpr[1] = opr2;
  assert_cond(opr1, __FILE__, __LINE__);
  assert_cond(opr2, __FILE__, __LINE__);
}

// デストラクタ
CptOpr2::~CptOpr2()
{
}

// ファイル位置を返す．
FileRegion
CptOpr2::file_region() const
{
  return FileRegion(mOpr[0]->file_region(), mOpr[1]->file_region());
}

// @brief オペランドの数の取得
// @return 子供の数
ymuint32
CptOpr2::operand_num() const
{
  return 2;
}

// @brief オペランドの取得
// @param[in] pos 取り出すオペランンドの位置(最初の位置は 0)
// @return pos 番目のオペランド
const PtExpr*
CptOpr2::operand(ymuint32 pos) const
{
  if ( pos < 2 ) {
    return mOpr[pos];
  }
  else {
    return NULL;
  }
}


//////////////////////////////////////////////////////////////////////
// 三項演算子を表すクラス
//////////////////////////////////////////////////////////////////////
  
// コンストラクタ
CptOpr3::CptOpr3(tVpiOpType op_type,
		 PtExpr* opr1,
		 PtExpr* opr2,
		 PtExpr* opr3) :
  CptOpr(op_type)
{
  mOpr[0] = opr1;
  mOpr[1] = opr2;
  mOpr[2] = opr3;
  assert_cond(opr1, __FILE__, __LINE__);
  assert_cond(opr2, __FILE__, __LINE__);
  assert_cond(opr3, __FILE__, __LINE__);
}

// デストラクタ
CptOpr3::~CptOpr3()
{
}

// ファイル位置を返す．
FileRegion
CptOpr3::file_region() const
{
  return FileRegion(mOpr[0]->file_region(), mOpr[2]->file_region());
}

// @brief オペランドの数の取得
// @return 子供の数
ymuint32
CptOpr3::operand_num() const
{
  return 3;
}

// @brief オペランドの取得
// @param[in] pos 取り出すオペランンドの位置(最初の位置は 0)
// @return pos 番目のオペランド
const PtExpr*
CptOpr3::operand(ymuint32 pos) const
{
  if ( pos < 3 ) {
    return mOpr[pos];
  }
  else {
    return NULL;
  }
}


//////////////////////////////////////////////////////////////////////
// concatenation を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptConcat::CptConcat(const FileRegion& file_region,
		     PtExprArray expr_array) :
  mFileRegion(file_region),
  mExprArray(expr_array)
{
}

// デストラクタ
CptConcat::~CptConcat()
{
}

// ファイル位置を返す．
FileRegion
CptConcat::file_region() const
{
  return mFileRegion;
}

// クラスの型を返す．
tPtExprType
CptConcat::type() const
{
  return kPtOprExpr;
}

///演算子の種類の取得
tVpiOpType
CptConcat::opr_type() const
{
  return kVpiConcatOp;
}
  
// @brief オペランドの数の取得
// @return 子供の数
ymuint32
CptConcat::operand_num() const
{
  return mExprArray.size();
}

// @brief オペランドの取得
// @param[in] pos 取り出すオペランンドの位置(最初の位置は 0)
// @return pos 番目のオペランド
const PtExpr*
CptConcat::operand(ymuint32 pos) const
{
  return mExprArray[pos];
}


//////////////////////////////////////////////////////////////////////
// multiple concatenation を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptMultiConcat::CptMultiConcat(const FileRegion& file_region,
			       PtExprArray expr_array) :
  CptConcat(file_region, expr_array)
{
}

// デストラクタ
CptMultiConcat::~CptMultiConcat()
{
}

// 演算子の種類の取得
tVpiOpType
CptMultiConcat::opr_type() const
{
  return kVpiMultiConcatOp;
}


//////////////////////////////////////////////////////////////////////
// min/typ/max delay のベース実装クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptMinTypMax::CptMinTypMax(PtExpr* val0,
			   PtExpr* val1,
			   PtExpr* val2)
{
  mValue[0] = val0;
  mValue[1] = val1;
  mValue[2] = val2;
  assert_cond(val0, __FILE__, __LINE__);
  assert_cond(val1, __FILE__, __LINE__);
  assert_cond(val2, __FILE__, __LINE__);
}

// デストラクタ
CptMinTypMax::~CptMinTypMax()
{
}

// ファイル位置を返す．
FileRegion
CptMinTypMax::file_region() const
{
  return FileRegion(mValue[0]->file_region(), mValue[2]->file_region());
}

// クラスの型を返す．
tPtExprType
CptMinTypMax::type() const
{
  return kPtOprExpr;
}

// 演算子の種類の取得
tVpiOpType
CptMinTypMax::opr_type() const
{
  return kVpiMinTypMaxOp;
}

// 子供の数の取得
ymuint32
CptMinTypMax::operand_num() const
{
  return 3;
}
  
// 値(式)を取出す．
// idx = 0 : Min
//     = 1 : Typ
//     = 2 : Max
const PtExpr*
CptMinTypMax::operand(ymuint32 idx) const
{
  if ( idx < 3 ) {
    return mValue[idx];
  }
  else {
    return NULL;
  }
}


//////////////////////////////////////////////////////////////////////
// function call / system function call に共通の基底クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptFuncCallBase::CptFuncCallBase(const FileRegion& file_region,
				 const char* name,
				 PtExprArray arg_array) :
  mFileRegion(file_region),
  mName(name),
  mArgArray(arg_array)
{
}

// デストラクタ
CptFuncCallBase::~CptFuncCallBase()
{
}

// ファイル位置を返す．
FileRegion
CptFuncCallBase::file_region() const
{
  return mFileRegion;
}

// 末尾の名前を返す．
const char*
CptFuncCallBase::name() const
{
  return mName;
}

// @brief オペランドの数の取得
// @return 子供の数
ymuint32
CptFuncCallBase::operand_num() const
{
  return mArgArray.size();
}

// @brief オペランドの取得
// @param[in] pos 取り出すオペランンドの位置(最初の位置は 0)
// @return pos 番目のオペランド
const PtExpr*
CptFuncCallBase::operand(ymuint32 pos) const
{
  return mArgArray[pos];
}


//////////////////////////////////////////////////////////////////////
// 階層なし名前を持つ function callを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptFuncCall::CptFuncCall(const FileRegion& file_region,
			 const char* name,
			 PtExprArray arg_array) :
  CptFuncCallBase(file_region, name, arg_array)
{
}

// デストラクタ
CptFuncCall::~CptFuncCall()
{
}

// クラスの型を返す．
tPtExprType
CptFuncCall::type() const
{
  return kPtFuncCallExpr;
}


//////////////////////////////////////////////////////////////////////
// 階層つき名前を持つ function call を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptFuncCallH::CptFuncCallH(const FileRegion& file_region,
			   PtNameBranchArray nb_array,
			   const char* tail_name,
			   PtExprArray arg_array) :
  CptFuncCall(file_region, tail_name, arg_array),
  mNbArray(nb_array)
{
}

// デストラクタ
CptFuncCallH::~CptFuncCallH()
{
}
  
// @brief 階層ブランチの取得
PtNameBranchArray
CptFuncCallH::namebranch_array() const
{
  return mNbArray;
}


//////////////////////////////////////////////////////////////////////
// system function callを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptSysFuncCall::CptSysFuncCall(const FileRegion& file_region,
			       const char* name,
			       PtExprArray arg_array) :
  CptFuncCallBase(file_region, name, arg_array)
{
}

// デストラクタ
CptSysFuncCall::~CptSysFuncCall()
{
}

// クラスの型を返す．
tPtExprType
CptSysFuncCall::type() const
{
  return kPtSysFuncCallExpr;
}


//////////////////////////////////////////////////////////////////////
// PtConstant のベース実装クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptConstant::CptConstant(const FileRegion& file_region) :
  mFileRegion(file_region)
{
}

// デストラクタ
CptConstant::~CptConstant()
{
}

// ファイル位置を返す．
FileRegion
CptConstant::file_region() const
{
  return mFileRegion;
}

// クラスの型を返す．
tPtExprType
CptConstant::type() const
{
  return kPtConstExpr;
}


//////////////////////////////////////////////////////////////////////
// 整数型の定数(サイズ/基数の指定なし)
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptIntConstant1::CptIntConstant1(const FileRegion& file_region,
				 ymuint32 value) :
  CptConstant(file_region),
  mValue(value)
{
}

// デストラクタ
CptIntConstant1::~CptIntConstant1()
{
}

// 定数の種類を表す型(vpiIntConst, vpiBinaryConst など) を返す．
tVpiConstType
CptIntConstant1::const_type() const
{
  return kVpiIntConst;
}

// 階層名の添字として使える式の時に true を返す．
bool
CptIntConstant1::is_index_expr() const
{
  return true;
}

// 整数型の値の取得
ymuint32
CptIntConstant1::const_uint() const
{
  return mValue;
}


//////////////////////////////////////////////////////////////////////
// 整数型の定数(基数のみ指定あり)
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptIntConstant2::CptIntConstant2(const FileRegion& file_region,
				 tVpiConstType const_type,
				 const char* value) :
  CptConstant(file_region),
  mConstType(const_type),
  mValue(value)
{
}

// デストラクタ
CptIntConstant2::~CptIntConstant2()
{
}

// 定数の種類を表す型(vpiIntConst, vpiBinaryConst など) を返す．
tVpiConstType
CptIntConstant2::const_type() const
{
  return mConstType;
}

// 文字列型の値の取得
const char*
CptIntConstant2::const_str() const
{
  return mValue;
}


//////////////////////////////////////////////////////////////////////
// 整数型の定数(サイズと基数の指定あり)
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptIntConstant3::CptIntConstant3(const FileRegion& file_region,
				 ymuint32 size,
				 tVpiConstType const_type,
				 const char* value) :
  CptConstant(file_region),
  mConstType(const_type),
  mSize(size),
  mValue(value)
{
}

// デストラクタ
CptIntConstant3::~CptIntConstant3()
{
}

// 定数の種類を表す型(vpiIntConst, vpiBinaryConst など) を返す．
tVpiConstType
CptIntConstant3::const_type() const
{
  return mConstType;
}

// 整数型の定数のサイズの取得
ymuint32
CptIntConstant3::const_size() const
{
  return mSize;
}

// 文字列型の値の取得
const char*
CptIntConstant3::const_str() const
{
  return mValue;
}


//////////////////////////////////////////////////////////////////////
// 実数型の定数(中身は PtConstant)
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptRealConstant::CptRealConstant(const FileRegion& file_region,
				 double value) :
  CptConstant(file_region),
  mValue(value)
{
}

// デストラクタ
CptRealConstant::~CptRealConstant()
{
}

// 定数の種類を表す型(vpiRealConst) を返す．
tVpiConstType
CptRealConstant::const_type() const
{
  return kVpiRealConst;
}

// 実数型の値の取得
double
CptRealConstant::const_real() const
{
  return mValue;
}


//////////////////////////////////////////////////////////////////////
// 文字列型の定数(中身は PtConstant)
//////////////////////////////////////////////////////////////////////

// 値を表す文字列を引数にとるコンストラクタ
CptStringConstant::CptStringConstant(const FileRegion& file_region,
				     const char* value) :
  CptConstant(file_region),
  mValue(value)
{
}

// デストラクタ
CptStringConstant::~CptStringConstant()
{
}

// 定数の種類を表す型(vpiStringConst) を返す．
tVpiConstType
CptStringConstant::const_type() const
{
  return kVpiStringConst;
}

// 値を表示用の文字列の形で得る．
const char*
CptStringConstant::const_str() const
{
  return mValue;
}

  
//////////////////////////////////////////////////////////////////////
// expression 関係
//////////////////////////////////////////////////////////////////////

// 演算子を生成する．
PtExpr*
CptFactory::new_Opr(const FileRegion& file_region,
		    tVpiOpType type,
		    PtExpr* opr)
{
  ++ mNumOpr1;
  void* p = alloc().get_memory(sizeof(CptOpr1));
  return new (p) CptOpr1(file_region, type, opr);
}

PtExpr*
CptFactory::new_Opr(const FileRegion& file_region,
		    tVpiOpType type,
		    PtExpr* opr1,
		    PtExpr* opr2)
{
  // 実は file_region は不要
  ++ mNumOpr2;
  void* p = alloc().get_memory(sizeof(CptOpr2));
  return new (p) CptOpr2(type, opr1, opr2);
}

PtExpr*
CptFactory::new_Opr(const FileRegion& file_region,
		    tVpiOpType type,
		    PtExpr* opr1,
		    PtExpr* opr2,
		    PtExpr* opr3)
{
  // 実は file_region は不要
  ++ mNumOpr3;
  void* p = alloc().get_memory(sizeof(CptOpr3));
  return new (p) CptOpr3(type, opr1, opr2, opr3);
}

// concatination を生成する．
PtExpr*
CptFactory::new_Concat(const FileRegion& file_region,
		       PtExprArray expr_array)
{
  ++ mNumConcat;
  void* p = alloc().get_memory(sizeof(CptConcat));
  return new (p) CptConcat(file_region, expr_array);
}

// multiple concatenation を生成する．
PtExpr*
CptFactory::new_MultiConcat(const FileRegion& file_region,
			    PtExprArray expr_array)
{
  ++ mNumMultiConcat;
  void* p = alloc().get_memory(sizeof(CptMultiConcat));
  return new (p) CptMultiConcat(file_region, expr_array);
}

// multiple concatenation を生成する．
PtExpr*
CptFactory::new_MinTypMax(const FileRegion& file_region,
			  PtExpr* val0,
			  PtExpr* val1,
			  PtExpr* val2)
{
  // 実は file_region は不要
  ++ mNumMinTypMax3;
  void* p = alloc().get_memory(sizeof(CptMinTypMax));
  return new (p) CptMinTypMax(val0, val1, val2);
}

// function call を生成する．
PtExpr*
CptFactory::new_FuncCall(const FileRegion& file_region,
			 const char* name,
			 PtExprArray arg_array)
{
  ++ mNumFuncCall;
  void* p = alloc().get_memory(sizeof(CptFuncCall));
  return new (p) CptFuncCall(file_region, name, arg_array);
}

// function call を生成する．
PtExpr*
CptFactory::new_FuncCall(const FileRegion& file_region,
			 PtNameBranchArray nb_array,
			 const char* tail_name,
			 PtExprArray arg_array)
{
  ++ mNumFuncCallH;
  void* p = alloc().get_memory(sizeof(CptFuncCallH));
  return new (p) CptFuncCallH(file_region, nb_array, tail_name, arg_array);
}

// system function call を生成する．
PtExpr*
CptFactory::new_SysFuncCall(const FileRegion& file_region,
			    const char* name,
			    PtExprArray arg_array)
{
  ++ mNumSysFuncCall;
  void* p = alloc().get_memory(sizeof(CptSysFuncCall));
  return new (p) CptSysFuncCall(file_region, name, arg_array);
}

// 定数を生成する．
PtExpr*
CptFactory::new_IntConst(const FileRegion& file_region,
			 ymuint32 value)
{
  ++ mNumIntConstant1;
  void* p = alloc().get_memory(sizeof(CptIntConstant1));
  return new (p) CptIntConstant1(file_region, value);
}

// 定数を生成する．
PtExpr*
CptFactory::new_IntConst(const FileRegion& file_region,
			 const char* value)
{
  ++ mNumIntConstant2;
  void* p = alloc().get_memory(sizeof(CptIntConstant2));
  return new (p) CptIntConstant2(file_region, kVpiIntConst, value);
}

// 定数を生成する．
PtExpr*
CptFactory::new_IntConst(const FileRegion& file_region,
			 tVpiConstType const_type,
			 const char* value)
{
  ++ mNumIntConstant2;
  void* p = alloc().get_memory(sizeof(CptIntConstant2));
  return new (p) CptIntConstant2(file_region, const_type, value);
}

// 定数を生成する．
PtExpr*
CptFactory::new_IntConst(const FileRegion& file_region,
			 ymuint32 size,
			 tVpiConstType const_type,
			 const char* value)
{
  ++ mNumIntConstant3;
  void* p = alloc().get_memory(sizeof(CptIntConstant3));
  return new (p) CptIntConstant3(file_region, size, const_type, value);
}

// 定数を生成する．
PtExpr*
CptFactory::new_RealConst(const FileRegion& file_region,
			  double value)
{
  ++ mNumRealConstant;
  void* p = alloc().get_memory(sizeof(CptRealConstant));
  return new (p) CptRealConstant(file_region, value);
}

// 定数を生成する．
PtExpr*
CptFactory::new_StringConst(const FileRegion& file_region,
			    const char* value)
{
  ++ mNumStringConstant;
  void* p = alloc().get_memory(sizeof(CptStringConstant));
  return new (p) CptStringConstant(file_region, value);
}

END_NAMESPACE_YM_VERILOG
