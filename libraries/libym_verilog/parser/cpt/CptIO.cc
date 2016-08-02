
/// @file libym_verilog/pt_compact/CptIO.cc
/// @brief CptIO の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CptIO.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "CptIO.h"
#include "CptFactory.h"

#include "ym_verilog/pt/PtExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// IO宣言のヘッダのベース実装クラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] file_region ファイル位置の情報
// @param[in] type IOの種類
// @param[in] aux_type 補助的な型
// @param[in] net_type 補助的なネット型
// @param[in] var_type 補助的な変数型
// @param[in] sign 符号つきの時 true にするフラグ
CptIOHBase::CptIOHBase(const FileRegion& file_region,
		       tPtIOType type,
		       tVpiAuxType aux_type,
		       tVpiNetType net_type,
		       tVpiVarType var_type,
		       bool sign) :
  mFileRegion(file_region)
{
  mAttr =
    static_cast<unsigned int>(sign) |
    (static_cast<unsigned int>(type) << 1) |
    (static_cast<unsigned int>(aux_type) << 8) |
    (static_cast<unsigned int>(net_type) << 16) |
    (static_cast<unsigned int>(var_type) << 24);
}

// @brief デストラクタ
CptIOHBase::~CptIOHBase()
{
}

// @brief ファイル位置の取得
FileRegion
CptIOHBase::file_region() const
{
  return mFileRegion;
}

// @brief 型の取得
tPtIOType
CptIOHBase::type() const
{
  return static_cast<tPtIOType>((mAttr & 0xff) >> 1);
}

// @brief 補助的な型の取得
tVpiAuxType
CptIOHBase::aux_type() const
{
  return static_cast<tVpiAuxType>((mAttr >> 8) & 0xff);
}

// @bief 補助的なネット型の取得
tVpiNetType
CptIOHBase::net_type() const
{
  return static_cast<tVpiNetType>((mAttr >> 16) & 0xff);
}

// @brief 補助的な変数型の取得
tVpiVarType
CptIOHBase::var_type() const
{
  return static_cast<tVpiVarType>((mAttr >> 24) & 0xff);
}

// @brief 符号の取得
// @retval true 符号付き
// @retval false 符号なし
bool
CptIOHBase::is_signed() const
{
  return static_cast<bool>(mAttr & 1);
}

// @brief 範囲のMSBの取得
// @retval 範囲のMSB
// @retval NULL 範囲を持たないとき
// @note このクラスでは NULL を返す．
const PtExpr*
CptIOHBase::left_range() const
{
  return NULL;
}

// @brief 範囲のLSBの取得
// @retval 範囲のLSB
// @retval NULL 範囲を持たないとき
// @note このクラスでは NULL を返す．
const PtExpr*
CptIOHBase::right_range() const
{
  return NULL;
}
  
// @brief 要素数の取得
ymuint32
CptIOHBase::item_num() const
{
  return mItemArray.size();
}

// @brief 要素の取得
// @param[in] pos 位置番号 ( 0 <= pos < item_num() )
const PtIOItem*
CptIOHBase::item(ymuint32 pos) const
{
  return mItemArray[pos];
}

// @brief 要素リストの設定
// @param[in] elem_array 要素リスト
void
CptIOHBase::set_elem(PtIOItemArray elem_array)
{
  mItemArray = elem_array;
}


//////////////////////////////////////////////////////////////////////
// 範囲を持たない IO宣言のヘッダ
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] file_region ファイル位置の情報
// @param[in] type IOの種類
// @param[in] aux_type 補助的な型
// @param[in] net_type 補助的なネット型
// @param[in] var_type 補助的な変数型
// @param[in] sign 符号の有無
CptIOH::CptIOH(const FileRegion& file_region,
	       tPtIOType type,
	       tVpiAuxType aux_type,
	       tVpiNetType net_type,
	       tVpiVarType var_type,
	       bool sign) :
  CptIOHBase(file_region,
	     type, aux_type,
	     net_type, var_type,
	     sign)
{
}

// @brief デストラクタ
CptIOH::~CptIOH()
{
}


//////////////////////////////////////////////////////////////////////
// ビットベクタ型のIO宣言のヘッダ
//////////////////////////////////////////////////////////////////////
  
// @brief コンストラクタ
// @param[in] file_region ファイル位置の情報
// @param[in] type IOの種類
// @param[in] aux_type 補助的な型
// @param[in] net_type 補助的なネット型
// @param[in] sign 符号つきの時に true とするフラグ
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
CptIOHV::CptIOHV(const FileRegion& file_region,
		 tPtIOType type,
		 tVpiAuxType aux_type,
		 tVpiNetType net_type,
		 bool sign,
		 PtExpr* left,
		 PtExpr* right) :
  CptIOHBase(file_region,
	     type, aux_type,
	     net_type, kVpiVarNone,
	     sign),
  mLeftRange(left),
  mRightRange(right)
{
}

// @brief デストラクタ
CptIOHV::~CptIOHV()
{
}

// @brief 範囲のMSBの取得
const PtExpr*
CptIOHV::left_range() const
{
  return mLeftRange;
}

// @brief 範囲のLSBの取得
const PtExpr*
CptIOHV::right_range() const
{
  return mRightRange;
}


//////////////////////////////////////////////////////////////////////
// IO宣言要素を表すクラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] file_region ファイル位置の情報
// @param[in] name 名前
CptIOItem::CptIOItem(const FileRegion& file_region,
		     const char* name) :
  mLoc(file_region),
  mName(name)
{
}
  
// @brief デストラクタ
CptIOItem::~CptIOItem()
{
}

// @brief ファイル位置の取得
FileRegion
CptIOItem::file_region() const
{
  return mLoc;
}

// @brief 名前の取得
const char*
CptIOItem::name() const
{
  return mName;
}

// @brief 初期値の取得
// @retval 初期値
// @retval NULL 初期値を持たないとき
// @note このクラスでは NULL を返す．
const PtExpr*
CptIOItem::init_value() const
{
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// 初期値をもった IO宣言要素の基底クラス
//////////////////////////////////////////////////////////////////////
  
// @brief コンストラクタ
// @param[in] file_region ファイル位置の情報
// @param[in] name 名前
// @param[in] init_value 初期値
CptIOItemI::CptIOItemI(const FileRegion& file_region,
		       const char* name,
		       PtExpr* init_value) :
  CptIOItem(file_region, name),
  mInitValue(init_value)
{
  assert_cond(init_value, __FILE__, __LINE__);
}
  
// @brief デストラクタ
CptIOItemI::~CptIOItemI()
{
}

// @brief ファイル位置の取得
FileRegion
CptIOItemI::file_region() const
{
  return FileRegion(CptIOItem::file_region(), mInitValue->file_region());
}
  
// @brief 初期値の取得
const PtExpr*
CptIOItemI::init_value() const
{
  return mInitValue;
}


//////////////////////////////////////////////////////////////////////
// IO 宣言関係
//////////////////////////////////////////////////////////////////////

// @brief IO 宣言のヘッダの生成
// @param[in] file_region ファイル位置の情報
// @param[in] type IO の種類
// @param[in] sign 符号付きのとき true となるフラグ
// @return 生成された IO宣言ヘッダ
PtiIOHead*
CptFactory::new_IOHead(const FileRegion& file_region,
		       tPtIOType type,
		       bool sign)
{
  ++ mNumIOH;
  void* p = alloc().get_memory(sizeof(CptIOH));
  return new (p) CptIOH(file_region,
			type, kVpiAuxNone,
			kVpiNone, kVpiVarNone, sign);
}

// @brief IO 宣言のヘッダの生成 (reg 型)
// @param[in] file_region ファイル位置の情報
// @param[in] type IO の種類
// @param[in] sign 符号付きのとき true となるフラグ
// @return 生成された IO宣言ヘッダ
PtiIOHead*
CptFactory::new_RegIOHead(const FileRegion& file_region,
			  tPtIOType type,
			  bool sign)
{
  ++ mNumIOH;
  void* p = alloc().get_memory(sizeof(CptIOH));
  return new (p) CptIOH(file_region,
			type, kVpiAuxReg,
			kVpiNone, kVpiVarNone, sign);
}

// @brief IO 宣言のヘッダの生成 (ネット型)
// @param[in] file_region ファイル位置の情報
// @param[in] type IO の種類
// @param[in] net_type 補助的なネット型
// @param[in] sign 符号付きのとき true となるフラグ
// @return 生成された IO宣言ヘッダ
PtiIOHead*
CptFactory::new_NetIOHead(const FileRegion& file_region,
			  tPtIOType type,
			  tVpiNetType net_type,
			  bool sign)
{
  ++ mNumIOH;
  void* p = alloc().get_memory(sizeof(CptIOH));
  return new (p) CptIOH(file_region,
			type, kVpiAuxNet,
			net_type, kVpiVarNone, sign);
}

// @brief IO 宣言のヘッダの生成 (変数型)
// @param[in] file_region ファイル位置の情報
// @param[in] type IO の種類
// @param[in] var_type 補助的な変数型
// @return 生成された IO宣言ヘッダ
PtiIOHead*
CptFactory::new_VarIOHead(const FileRegion& file_region,
			  tPtIOType type,
			  tVpiVarType var_type)
{
  ++ mNumIOH;
  void* p = alloc().get_memory(sizeof(CptIOH));
  return new (p) CptIOH(file_region,
			type, kVpiAuxVar,
			kVpiNone, var_type, false);
}

// @brief 範囲付きの IO 宣言のヘッダの生成
// @param[in] file_region ファイル位置の情報
// @param[in] type IO の種類
// @param[in] sign 符号付きのとき true となるフラグ
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @return 生成された IO宣言ヘッダ
PtiIOHead*
CptFactory::new_IOHead(const FileRegion& file_region,
		       tPtIOType type,
		       bool sign,
		       PtExpr* left,
		       PtExpr* right)
{
  ++ mNumIOHV;
  void* p = alloc().get_memory(sizeof(CptIOHV));
  return new (p) CptIOHV(file_region,
			 type, kVpiAuxNone, kVpiNone,
			 sign, left, right);
}

// @brief 範囲付きの IO 宣言のヘッダの生成 (reg 型)
// @param[in] file_region ファイル位置の情報
// @param[in] type IO の種類
// @param[in] sign 符号付きのとき true となるフラグ
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @return 生成された IO宣言ヘッダ
PtiIOHead*
CptFactory::new_RegIOHead(const FileRegion& file_region,
			  tPtIOType type,
			  bool sign,
			  PtExpr* left,
			  PtExpr* right)
{
  ++ mNumIOHV;
  void* p = alloc().get_memory(sizeof(CptIOHV));
  return new (p) CptIOHV(file_region,
			 type, kVpiAuxReg, kVpiNone,
			 sign, left, right);
}

// @brief 範囲付きの IO 宣言のヘッダの生成 (ネット型)
// @param[in] file_region ファイル位置の情報
// @param[in] type IO の種類
// @param[in] net_type 補助的なネット型
// @param[in] sign 符号付きのとき true となるフラグ
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @return 生成された IO宣言ヘッダ
PtiIOHead*
CptFactory::new_NetIOHead(const FileRegion& file_region,
			  tPtIOType type,
			  tVpiNetType net_type,
			  bool sign,
			  PtExpr* left,
			  PtExpr* right)
{
  ++ mNumIOHV;
  void* p = alloc().get_memory(sizeof(CptIOHV));
  return new (p) CptIOHV(file_region,
			 type, kVpiAuxNet, net_type,
			 sign, left, right);
}

// @brief IO 宣言の要素を生成する．
// @param[in] file_region ファイル上の位置
// @param[in] name 名前
PtIOItem*
CptFactory::new_IOItem(const FileRegion& file_region,
		       const char* name)
{
  ++ mNumIOItem;
  void* p = alloc().get_memory(sizeof(CptIOItem));
  return new (p) CptIOItem(file_region, name);
}

// @brief 初期値付き IO 宣言の要素の生成
// @param[in] file_region ファイル上の位置
// @param[in] name 名前
// @param[in] init_value 初期値
PtIOItem*
CptFactory::new_IOItem(const FileRegion& file_region,
		       const char* name,
		       PtExpr* init_value)
{
  ++ mNumIOItemI;
  void* p = alloc().get_memory(sizeof(CptIOItemI));
  return new (p) CptIOItemI(file_region, name, init_value);
}

END_NAMESPACE_YM_VERILOG
