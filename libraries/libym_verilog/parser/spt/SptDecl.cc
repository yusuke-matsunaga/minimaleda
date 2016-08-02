
/// @file libym_verilog/pt_simple/SptDecl.cc
/// @brief SptDecl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SptDecl.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "SptDecl.h"
#include "SptFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// IO宣言のヘッダのベース実装クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
// @param type IO の種類
// @param aux_type 補助的な型
// @param sign 符号の有無を表すフラグ
// @param left 範囲の左側の式
// @param right 範囲の右側の式 
// net_type と var_type は両方同時に指定されることはない．
// 少なくとも一つは kVpiNone(net_type) か kVpiVarNone(var_type)
// になっていなければならない．
SptIOHead::SptIOHead(const FileRegion& file_region,
		     tPtIOType type,
		     tVpiAuxType aux_type,
		     tVpiNetType net_type,
		     tVpiVarType var_type,
		     bool sign,
		     PtExpr* left,
		     PtExpr* right) :
  mFileRegion(file_region),
  mType(type),
  mAuxType(aux_type),
  mNetType(net_type),
  mVarType(var_type),
  mSigned(sign),
  mLeftRange(left),
  mRightRange(right)
{
}

// デストラクタ
SptIOHead::~SptIOHead()
{
}

// ファイル位置の取得
// @return ファイル位置
FileRegion
SptIOHead::file_region() const
{
  return mFileRegion;
}

// 型の取得
// @return 型
tPtIOType
SptIOHead::type() const
{
  return mType;
}

// 補助的な型の取得
// @return 補助的な型
tVpiAuxType
SptIOHead::aux_type() const
{
  return mAuxType;
}

// 補助的なネット型の取得
// @return ネット型
tVpiNetType
SptIOHead::net_type() const
{
  return mNetType;
}

// 補助的な変数型の取得
// @return 変数型
tVpiVarType
SptIOHead::var_type() const
{
  return mVarType;
}

// 符号の取得
// @retval true 符号付き
// @retval false 符号なし
bool
SptIOHead::is_signed() const
{
  return mSigned;
}

// 範囲のMSBの取得
// @retval 範囲のMSB
// @retval NULL 範囲を持たないとき
const PtExpr*
SptIOHead::left_range() const
{
  return mLeftRange;
}

// 範囲のLSBの取得
// @retval 範囲のLSB
// @retval NULL 範囲を持たないとき
const PtExpr*
SptIOHead::right_range() const
{
  return mRightRange;
}

// @brief 要素数の取得
ymuint32
SptIOHead::item_num() const
{
  return mItemArray.size();
}

// @brief 要素の取得
// @param[in] pos 位置番号 ( 0 <= pos < item_num() )
const PtIOItem*
SptIOHead::item(ymuint32 pos) const
{
  return mItemArray[pos];
}

// 要素リストの設定
// @param elem_array 要素の配列
void
SptIOHead::set_elem(PtIOItemArray elem_array)
{
  mItemArray = elem_array;
}


//////////////////////////////////////////////////////////////////////
// IO宣言要素を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
// @param name 名前
// @param init_value 初期値
SptIOItem::SptIOItem(const FileRegion& file_region,
		     const char* name,
		     PtExpr* init_value) :
  mFileRegion(file_region),
  mName(name),
  mInitValue(init_value)
{
}
  
// デストラクタ
SptIOItem::~SptIOItem()
{
}

// ファイル位置の取得
// @return ファイル位置
FileRegion
SptIOItem::file_region() const
{
  return mFileRegion;
}

// 名前の取得
// @return 名前
const char*
SptIOItem::name() const
{
  return mName;
}

// 初期値の取得
// @retval 初期値
// @retval NULL 初期値を持たないとき
const PtExpr*
SptIOItem::init_value() const
{
  return mInitValue;
}


//////////////////////////////////////////////////////////////////////
// 宣言要素のヘッダの基底クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
// @param type 宣言要素の型
// @param sign 符号の有無を表すフラグ
// @param left 範囲の左側の式
// @param right 範囲の右側の式
// @param net_type net 型
// @param vs_type vectored/scalared 属性
// @param strength 信号強度
// @param delay 遅延
// net_type/vs_type/strength/delay が意味を持つのは type == kNet の
// 時のみ
SptDeclHead::SptDeclHead(const FileRegion& file_region,
		   tPtDeclType type,
		   bool sign,
		   PtExpr* left,
		   PtExpr* right,
		   tVpiVarType var_type,
		   tVpiNetType net_type,
		   tVpiVsType vs_type,
		   PtStrength* strength,
		   PtDelay* delay) :
  mFileRegion(file_region),
  mType(type),
  mSigned(sign),
  mLeftRange(left),
  mRightRange(right),
  mVarType(var_type),
  mNetType(net_type),
  mVsType(vs_type),
  mStrength(strength),
  mDelay(delay)
{
}

// デストラクタ
SptDeclHead::~SptDeclHead()
{
}

// ファイル位置の取得
// @return ファイル位置
FileRegion
SptDeclHead::file_region() const
{
  return mFileRegion;
}

// 宣言要素の型の取得
// @return 宣言要素の型
tPtDeclType
SptDeclHead::type() const
{
  return mType;
}

// 符号の取得
// @retval true 符号つき
// @retval false 符号なし
bool
SptDeclHead::is_signed() const
{
  return mSigned;
}
  
// 範囲のMSBの取得
// @retval 範囲のMSB
// @retval NULL 範囲を持たないとき
const PtExpr*
SptDeclHead::left_range() const
{
  return mLeftRange;
}

// 範囲のLSBの取得
// @retval 範囲のLSB
// @retval NULL 範囲を持たないとき
const PtExpr*
SptDeclHead::right_range() const
{
  return mRightRange;
}

// データ型の取得
// @retval データ型 kParam, kLocalParam, kVar の場合
// @retval kVpiVarNone 上記以外
tVpiVarType
SptDeclHead::data_type() const
{
  return mVarType;
}

// net 型の取得
// @retval net 型 net 型の要素の場合
// @retval kVpiNone net 型の要素でない場合
tVpiNetType
SptDeclHead::net_type() const
{
  return mNetType;
}

// vectored|scalared 属性の取得
// @retval kVpiVsNone vectored|scalared 指定なし
// @retval kVpiVectored vectored 指定あり
// @retval kVpiScalared scalared 指定あり
tVpiVsType
SptDeclHead::vs_type() const
{
  return mVsType;
}

// strength の取得
// @retval strength
// @retval NULL strength の指定なし
const PtStrength*
SptDeclHead::strength() const
{
  return mStrength;
}

// delay の取得
// @retval delay
// @retval NULL delay の指定なし
const PtDelay*
SptDeclHead::delay() const
{
  return mDelay;
}

// @brief 要素数の取得
ymuint32
SptDeclHead::item_num() const
{
  return mItemArray.size();
}

// @brief 要素の取得
// @param[in] pos 位置番号 ( 0 <= pos < item_num() )
const PtDeclItem*
SptDeclHead::item(ymuint32 pos) const
{
  return mItemArray[pos];
}

// 要素リストの設定
// @param elem_array 要素の配列
void
SptDeclHead::set_elem(PtDeclItemArray elem_array)
{
  mItemArray = elem_array;
}


//////////////////////////////////////////////////////////////////////
// 宣言要素のベース実装クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
// @param name 名前
// @param range_array 範囲の配列
// @param init_value 初期値
// range_array と init_value の両方が同時に使われることはない．
// どちらか一方は必ず NULL である．両方 NULL の場合もあり得る．
SptDeclItem::SptDeclItem(const FileRegion& file_region,
			 const char* name,
			 PtRangeArray range_array,
			 PtExpr* init_value) :
  mFileRegion(file_region),
  mName(name),
  mRangeArray(range_array),
  mInitValue(init_value)
{
}
  
// デストラクタ
SptDeclItem::~SptDeclItem()
{
}

// ファイル位置の取得
// @return ファイル位置
FileRegion
SptDeclItem::file_region() const
{
  return mFileRegion;
}

// 名前の取得
// @return 名前
const char*
SptDeclItem::name() const
{
  return mName;
}

// dimension list のサイズの取得
// @return dimension list のサイズ
ymuint32
SptDeclItem::dimension_list_size() const
{
  return mRangeArray.size();
}

// 範囲の取得
// @return 先頭の要素
const PtRange*
SptDeclItem::range(ymuint32 pos) const
{
  return mRangeArray[pos];
}

// 初期値の取得
// @retval 初期値
// @retval NULL 設定がない場合
const PtExpr*
SptDeclItem::init_value() const
{
  return mInitValue;
}


//////////////////////////////////////////////////////////////////////
// @class SptRange SptDecl.h "SptDecl.h"
// @brief 範囲を表すクラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SptRange::SptRange(const FileRegion& file_region,
		   PtExpr* msb,
		   PtExpr* lsb) :
  mFileRegion(file_region),
  mMsb(msb),
  mLsb(lsb)
{
}

// @brief デストラクタ
SptRange::~SptRange()
{
}

// ファイル位置の取得
FileRegion
SptRange::file_region() const
{
  return mFileRegion;
}

// @brief 範囲の MSB を取り出す．
const PtExpr*
SptRange::left() const
{
  return mMsb;
}

// @brief 範囲の LSB を取り出す．
const PtExpr*
SptRange::right() const
{
  return mLsb;
}

  
//////////////////////////////////////////////////////////////////////
// IO 宣言関係のオブジェクトを生成する関数
//////////////////////////////////////////////////////////////////////

// @brief IO 宣言のヘッダの生成
// @param[in] file_region ファイル位置の情報
// @param[in] type IO の種類
// @param[in] sign 符号付きのとき true となるフラグ
// @return 生成された IO宣言ヘッダ
PtiIOHead*
SptFactory::new_IOHead(const FileRegion& file_region,
		       tPtIOType type,
		       bool sign)
{
  void* p = alloc().get_memory(sizeof(SptIOHead));
  return new (p) SptIOHead(file_region,
			   type, kVpiAuxNone,
			   kVpiNone, kVpiVarNone,
			   sign, NULL, NULL);
}

// @brief IO 宣言のヘッダの生成 (reg 型)
// @param[in] file_region ファイル位置の情報
// @param[in] type IO の種類
// @param[in] sign 符号付きのとき true となるフラグ
// @return 生成された IO宣言ヘッダ
PtiIOHead*
SptFactory::new_RegIOHead(const FileRegion& file_region,
			  tPtIOType type,
			  bool sign)
{
  void* p = alloc().get_memory(sizeof(SptIOHead));
  return new (p) SptIOHead(file_region,
			   type, kVpiAuxReg,
			   kVpiNone, kVpiVarNone,
			   sign, NULL, NULL);
}

// @brief IO 宣言のヘッダの生成 (ネット型)
// @param[in] file_region ファイル位置の情報
// @param[in] type IO の種類
// @param[in] net_type 補助的なネット型
// @param[in] sign 符号付きのとき true となるフラグ
// @return 生成された IO宣言ヘッダ
PtiIOHead*
SptFactory::new_NetIOHead(const FileRegion& file_region,
			  tPtIOType type,
			  tVpiNetType net_type,
			  bool sign)
{
  void* p = alloc().get_memory(sizeof(SptIOHead));
  return new (p) SptIOHead(file_region,
			   type, kVpiAuxNet,
			   net_type, kVpiVarNone,
			   sign, NULL, NULL);
}

// @brief IO 宣言のヘッダの生成 (変数型)
// @param[in] file_region ファイル位置の情報
// @param[in] type IO の種類
// @param[in] var_type 補助的な変数型
// @return 生成された IO宣言ヘッダ
PtiIOHead*
SptFactory::new_VarIOHead(const FileRegion& file_region,
			  tPtIOType type,
			  tVpiVarType var_type)
{
  void* p = alloc().get_memory(sizeof(SptIOHead));
  return new (p) SptIOHead(file_region,
			   type, kVpiAuxVar,
			   kVpiNone, var_type,
			   false, NULL, NULL);
}

// @brief 範囲付きの IO 宣言のヘッダの生成
// @param[in] file_region ファイル位置の情報
// @param[in] type IO の種類
// @param[in] sign 符号付きのとき true となるフラグ
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @return 生成された IO宣言ヘッダ
PtiIOHead*
SptFactory::new_IOHead(const FileRegion& file_region,
		       tPtIOType type,
		       bool sign,
		       PtExpr* left,
		       PtExpr* right)
{
  void* p = alloc().get_memory(sizeof(SptIOHead));
  return new (p) SptIOHead(file_region,
			   type, kVpiAuxNone,
			   kVpiNone, kVpiVarNone,
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
SptFactory::new_RegIOHead(const FileRegion& file_region,
			  tPtIOType type,
			  bool sign,
			  PtExpr* left,
			  PtExpr* right)
{
  void* p = alloc().get_memory(sizeof(SptIOHead));
  return new (p) SptIOHead(file_region,
			   type, kVpiAuxReg,
			   kVpiNone, kVpiVarNone,
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
SptFactory::new_NetIOHead(const FileRegion& file_region,
			  tPtIOType type,
			  tVpiNetType net_type,
			  bool sign,
			  PtExpr* left,
			  PtExpr* right)
{
  void* p = alloc().get_memory(sizeof(SptIOHead));
  return new (p) SptIOHead(file_region,
			   type, kVpiAuxNet,
			   net_type, kVpiVarNone,
			   sign, left, right);
}

// @brief IO 宣言の要素の生成
// @param[in] file_region ファイル位置の情報
// @param[in] name 要素名
// @return 生成された要素
PtIOItem*
SptFactory::new_IOItem(const FileRegion& file_region,
		       const char* name)
{
  void* p = alloc().get_memory(sizeof(SptIOItem));
  return new (p) SptIOItem(file_region, name, NULL);
}

// @brief 初期値付き IO 宣言の要素の生成
// @param[in] file_region ファイル位置の情報
// @param[in] name 要素名
// @param[in] init_value 初期値を表す式
// @return 生成された要素
PtIOItem*
SptFactory::new_IOItem(const FileRegion& file_region,
		       const char* name,
		       PtExpr* init_value)
{
  void* p = alloc().get_memory(sizeof(SptIOItem));
  return new (p) SptIOItem(file_region, name, init_value);
}

  
//////////////////////////////////////////////////////////////////////
// その他の宣言関係
//////////////////////////////////////////////////////////////////////

// @brief パラメータ宣言のヘッダの生成 (型指定なし)
// @param[in] file_region ファイル位置の情報
// @return 生成されたパラメータ
PtiDeclHead*
SptFactory::new_ParamH(const FileRegion& file_region)
{
  void* p = alloc().get_memory(sizeof(SptDeclHead));
  return new (p) SptDeclHead(file_region,
			     kPtDecl_Param,
			     false, NULL, NULL,
			     kVpiVarNone, kVpiNone, kVpiVsNone,
			     NULL, NULL);
}

// @brief 範囲指定型パラメータ宣言のヘッダの生成
// @param[in] file_region ファイル位置の情報
// @param[in] sign 符号付きのとき true となるフラグ
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @return 生成されたパラメータ
PtiDeclHead*
SptFactory::new_ParamH(const FileRegion& file_region,
		       bool sign,
		       PtExpr* left,
		       PtExpr* right)
{
  void* p = alloc().get_memory(sizeof(SptDeclHead));
  return new (p) SptDeclHead(file_region,
			     kPtDecl_Param,
			     sign, left, right,
			     kVpiVarNone, kVpiNone, kVpiVsNone,
			     NULL, NULL);
}

// @brief 組み込み型パラメータ宣言のヘッダの生成
// @param[in] file_region ファイル位置の情報
// @param[in] var_type データ型
// @return 生成されたパラメータ
PtiDeclHead*
SptFactory::new_ParamH(const FileRegion& file_region,
		       tVpiVarType var_type)
{
  void* p = alloc().get_memory(sizeof(SptDeclHead));
  return new (p) SptDeclHead(file_region,
			     kPtDecl_Param,
			     false, NULL, NULL,
			     var_type, kVpiNone, kVpiVsNone,
			     NULL, NULL);
}

// @brief local param 宣言のヘッダの生成 (型指定なし)
// @param[in] file_region ファイル位置の情報
// @return 生成された localparam
PtiDeclHead*
SptFactory::new_LocalParamH(const FileRegion& file_region)
{
  void* p = alloc().get_memory(sizeof(SptDeclHead));
  return new (p) SptDeclHead(file_region,
			     kPtDecl_LocalParam,
			     false, NULL, NULL,
			     kVpiVarNone, kVpiNone, kVpiVsNone,
			     NULL, NULL);
}

// @brief 範囲指定型 local param 宣言のヘッダの生成
// @param[in] file_region ファイル位置の情報
// @param[in] sign 符号付きのとき true となるフラグ
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @return 生成された localparam
PtiDeclHead*
SptFactory::new_LocalParamH(const FileRegion& file_region,
			    bool sign,
			    PtExpr* left,
			    PtExpr* right)
{
  void* p = alloc().get_memory(sizeof(SptDeclHead));
  return new (p) SptDeclHead(file_region,
			     kPtDecl_LocalParam,
			     sign, left, right,
			     kVpiVarNone, kVpiNone, kVpiVsNone,
			     NULL, NULL);
}

// @brief 組み込み型 local param 宣言のヘッダの生成
// @param[in] file_region ファイル位置の情報
// @param[in] var_type データ型
// @return 生成された localparam
PtiDeclHead*
SptFactory::new_LocalParamH(const FileRegion& file_region,
			    tVpiVarType var_type)
{
  void* p = alloc().get_memory(sizeof(SptDeclHead));
  return new (p) SptDeclHead(file_region,
			     kPtDecl_LocalParam,
			     false, NULL, NULL,
			     var_type, kVpiNone, kVpiVsNone,
			     NULL, NULL);
}

// @brief specparam 宣言のヘッダの生成
// @param[in] file_region ファイル位置の情報
// @return 生成された specparam
PtiDeclHead*
SptFactory::new_SpecParamH(const FileRegion& file_region)
{
  void* p = alloc().get_memory(sizeof(SptDeclHead));
  return new (p) SptDeclHead(file_region,
			     kPtDecl_SpecParam,
			     false, NULL, NULL,
			     kVpiVarNone, kVpiNone, kVpiVsNone,
			     NULL, NULL);
}

// @brief 範囲指定型 specparam 宣言のヘッダの生成
// @param[in] file_region ファイル位置の情報
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @return 生成された specparam
PtiDeclHead*
SptFactory::new_SpecParamH(const FileRegion& file_region,
			   PtExpr* left,
			   PtExpr* right)
{
  void* p = alloc().get_memory(sizeof(SptDeclHead));
  return new (p) SptDeclHead(file_region,
			     kPtDecl_SpecParam,
			     false, left, right,
			     kVpiVarNone, kVpiNone, kVpiVsNone,
			     NULL, NULL);
}

// @brief イベント宣言のヘッダの生成
// @param[in] file_region ファイル位置の情報
// @return 生成されたイベント
PtiDeclHead*
SptFactory::new_EventH(const FileRegion& file_region)
{
  void* p = alloc().get_memory(sizeof(SptDeclHead));
  return new (p) SptDeclHead(file_region,
			     kPtDecl_Event,
			     false, NULL, NULL,
			     kVpiVarNone, kVpiNone, kVpiVsNone,
			     NULL, NULL);
}

// @brief genvar 宣言のヘッダの生成
// @param[in] file_region ファイル位置の情報
// @return 生成された genvar
PtiDeclHead*
SptFactory::new_GenvarH(const FileRegion& file_region)
{
  void* p = alloc().get_memory(sizeof(SptDeclHead));
  return new (p) SptDeclHead(file_region,
			     kPtDecl_Genvar,
			     false, NULL, NULL,
			     kVpiVarNone, kVpiNone, kVpiVsNone,
			     NULL, NULL);
}

// @brief 変数宣言のヘッダの生成
// @param[in] file_region ファイル位置の情報
// @param[in] var_type データ型
// @return 生成された変数
PtiDeclHead*
SptFactory::new_VarH(const FileRegion& file_region,
		     tVpiVarType var_type)
{
  void* p = alloc().get_memory(sizeof(SptDeclHead));
  return new (p) SptDeclHead(file_region,
			     kPtDecl_Var,
			     false, NULL, NULL,
			     var_type, kVpiNone, kVpiVsNone,
			     NULL, NULL);
}

// @brief 1ビット型 reg 宣言のヘッダの生成
// @param[in] file_region ファイル位置の情報
// @param[in] sign 符号の有無を表すフラグ
// @return 生成された reg
PtiDeclHead*
SptFactory::new_RegH(const FileRegion& file_region,
		     bool sign)
{
  void* p = alloc().get_memory(sizeof(SptDeclHead));
  return new (p) SptDeclHead(file_region,
			     kPtDecl_Reg,
			     sign, NULL, NULL,
			     kVpiVarNone, kVpiNone, kVpiVsNone,
			     NULL, NULL);
}

// @brief 範囲指定型 reg 宣言のヘッダの生成
// @param[in] file_region ファイル位置の情報
// @param[in] sign 符号付きの時 true となるフラグ
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @return 生成された reg
PtiDeclHead*
SptFactory::new_RegH(const FileRegion& file_region,
		     bool sign,
		     PtExpr* left,
		     PtExpr* right)
{
  void* p = alloc().get_memory(sizeof(SptDeclHead));
  return new (p) SptDeclHead(file_region,
			     kPtDecl_Reg,
			     sign, left, right,
			     kVpiVarNone, kVpiNone, kVpiVsNone,
			     NULL, NULL);
}
  
// @brief 1ビット型 net 宣言のヘッダの生成
// @param[in] file_region ファイル位置の情報
// @param[in] type net の型
// @param[in] sign 符号の有無を表すフラグ
// @return 生成されたネット
PtiDeclHead*
SptFactory::new_NetH(const FileRegion& file_region,
		     tVpiNetType type,
		     bool sign)
{
  void* p = alloc().get_memory(sizeof(SptDeclHead));
  return new (p) SptDeclHead(file_region,
			     kPtDecl_Net,
			     sign, NULL, NULL,
			     kVpiVarNone, type, kVpiVsNone,
			     NULL, NULL);
}
  
// @brief 1ビット型 net 宣言のヘッダの生成 (strength あり)
// @param[in] file_region ファイル位置の情報
// @param[in] type net の型
// @param[in] sign 符号の有無を表すフラグ
// @param[in] strength 信号強度
// @return 生成されたネット
PtiDeclHead*
SptFactory::new_NetH(const FileRegion& file_region,
		     tVpiNetType type,
		     bool sign,
		     PtStrength* strength)
{
  void* p = alloc().get_memory(sizeof(SptDeclHead));
  return new (p) SptDeclHead(file_region,
			     kPtDecl_Net,
			     sign, NULL, NULL,
			     kVpiVarNone, type, kVpiVsNone,
			     strength, NULL);
}
  
// @brief 1ビット型 net 宣言のヘッダの生成 (遅延あり)
// @param[in] file_region ファイル位置の情報
// @param[in] type net の型
// @param[in] sign 符号の有無を表すフラグ
// @param[in] delay 遅延
// @return 生成されたネット
PtiDeclHead*
SptFactory::new_NetH(const FileRegion& file_region,
		     tVpiNetType type,
		     bool sign,
		     PtDelay* delay)
{
  void* p = alloc().get_memory(sizeof(SptDeclHead));
  return new (p) SptDeclHead(file_region,
			     kPtDecl_Net,
			     sign, NULL, NULL,
			     kVpiVarNone, type, kVpiVsNone,
			     NULL, delay);
}
  
// @brief 1ビット型 net 宣言のヘッダの生成 (strength, 遅延あり)
// @param[in] file_region ファイル位置の情報
// @param[in] type net の型
// @param[in] sign 符号の有無を表すフラグ
// @param[in] strength 信号強度
// @param[in] delay 遅延
// @return 生成されたネット
PtiDeclHead*
SptFactory::new_NetH(const FileRegion& file_region,
		     tVpiNetType type,
		     bool sign,
		     PtStrength* strength,
		     PtDelay* delay)
{
  void* p = alloc().get_memory(sizeof(SptDeclHead));
  return new (p) SptDeclHead(file_region,
			     kPtDecl_Net,
			     sign, NULL, NULL,
			     kVpiVarNone, type, kVpiVsNone,
			     strength, delay);
}

// @brief 範囲指定型 net 宣言のヘッダの生成
// @param[in] file_region ファイル位置の情報
/// @param[in] type net の型
// @param[in] sign 符号の有無を表すフラグ
// @param[in] vstype vector/scalar 指定
// @param[in] sign 符号の有無を表すフラグ
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @return 生成されたネット
PtiDeclHead*
SptFactory::new_NetH(const FileRegion& file_region,
		     tVpiNetType type,
		     tVpiVsType vstype,
		     bool sign,
		     PtExpr* left,
		     PtExpr* right)
{
  void* p = alloc().get_memory(sizeof(SptDeclHead));
  return new (p) SptDeclHead(file_region,
			     kPtDecl_Net,
			     sign, left, right,
			     kVpiVarNone, type, vstype,
			     NULL, NULL);
}
  
// @brief 範囲指定型 net 宣言のヘッダの生成 (strengthあり)
// @param[in] file_region ファイル位置の情報
// @param[in] type net の型
// @param[in] vstype vector/scalar 指定
// @param[in] sign 符号の有無を表すフラグ
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @param[in] strength 信号強度
// @return 生成されたネット
PtiDeclHead*
SptFactory::new_NetH(const FileRegion& file_region,
		     tVpiNetType type,
		     tVpiVsType vstype,
		     bool sign,
		     PtExpr* left,
		     PtExpr* right,
		     PtStrength* strength)
{
  void* p = alloc().get_memory(sizeof(SptDeclHead));
  return new (p) SptDeclHead(file_region,
			     kPtDecl_Net,
			     sign, left, right,
			     kVpiVarNone, type, vstype,
			     strength, NULL);
}

// @brief 範囲指定型 net 宣言のヘッダの生成 (遅延あり)
// @param[in] file_region ファイル位置の情報
// @param[in] type net の型
// @param[in] vstype vector/scalar 指定
// @param[in] sign 符号の有無を表すフラグ
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @param[in] delay 遅延
// @return 生成されたネット
PtiDeclHead*
SptFactory::new_NetH(const FileRegion& file_region,
		     tVpiNetType type,
		     tVpiVsType vstype,
		     bool sign,
		     PtExpr* left,
		     PtExpr* right,
		     PtDelay* delay)
{
  void* p = alloc().get_memory(sizeof(SptDeclHead));
  return new (p) SptDeclHead(file_region,
			     kPtDecl_Net,
			     sign, left, right,
			     kVpiVarNone, type, vstype,
			     NULL, delay);
}

// @brief 範囲指定型 net 宣言のヘッダの生成 (strength, 遅延あり)
// @param[in] file_region ファイル位置の情報
// @param[in] type net の型
// @param[in] vstype vector/scalar 指定
// @param[in] sign 符号の有無を表すフラグ
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @param[in] strength 信号強度
// @param[in] delay 遅延
// @return 生成されたネット
PtiDeclHead*
SptFactory::new_NetH(const FileRegion& file_region,
		     tVpiNetType type,
		     tVpiVsType vstype,
		     bool sign,
		     PtExpr* left,
		     PtExpr* right,
		     PtStrength* strength,
		     PtDelay* delay)
{
  void* p = alloc().get_memory(sizeof(SptDeclHead));
  return new (p) SptDeclHead(file_region,
			     kPtDecl_Net,
			     sign, left, right,
			     kVpiVarNone, type, vstype,
			     strength, delay);
}

// @brief 宣言要素の生成
// @param[in] file_region ファイル位置の情報
// @param[in] name 名前
// @return 生成された要素
PtDeclItem*
SptFactory::new_DeclItem(const FileRegion& file_region,
			 const char* name)
{
  void* p = alloc().get_memory(sizeof(SptDeclItem));
  return new (p) SptDeclItem(file_region, name, PtRangeArray(), NULL);
}

// @brief 初期値付き宣言要素の生成
// @param[in] file_region ファイル位置の情報
// @param[in] name 名前
// @param[in] init_value 初期値を表す式
// @return 生成された要素
PtDeclItem*
SptFactory::new_DeclItem(const FileRegion& file_region,
			 const char* name,
			 PtExpr* init_value)
{
  void* p = alloc().get_memory(sizeof(SptDeclItem));
  return new (p) SptDeclItem(file_region, name, PtRangeArray(), init_value);
}

// @brief 配列型宣言要素の生成
// @param[in] file_region ファイル位置の情報
// @param[in] name 名前
// @param[in] range_array 配列の各次元の範囲のリスト
// @return 生成された要素
PtDeclItem*
SptFactory::new_DeclItem(const FileRegion& file_region,
			 const char* name,
			 PtRangeArray range_array)
{
  void* p = alloc().get_memory(sizeof(SptDeclItem));
  return new (p) SptDeclItem(file_region, name, range_array, NULL);
}

// @brief 範囲の生成
// @param[in] file_region ファイル位置の情報
// @param[in] msb MSB を表す式
// @param[in] lsb LSB を表す式
PtRange*
SptFactory::new_Range(const FileRegion& file_region,
		      PtExpr* msb,
		      PtExpr* lsb)
{
  void* p = alloc().get_memory(sizeof(SptRange));
  return new (p) SptRange(file_region, msb, lsb);
}

END_NAMESPACE_YM_VERILOG
