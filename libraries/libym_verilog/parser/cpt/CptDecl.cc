
/// @file libym_verilog/pt_compact/CptDecl.cc
/// @brief CptDecl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CptDecl.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "CptDecl.h"
#include "CptFactory.h"

#include "ym_verilog/pt/PtExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// 宣言要素のヘッダの基底クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
CptDeclHead::CptDeclHead(const FileRegion& file_region) :
  mFileRegion(file_region)
{
}

// デストラクタ
CptDeclHead::~CptDeclHead()
{
}

// ファイル位置の取得
FileRegion
CptDeclHead::file_region() const
{
  return mFileRegion;
}

// @brief 符号の取得
// @retval true 符号つき
// @retval false 符号なし
// @note このクラスでは false を返す．
bool
CptDeclHead::is_signed() const
{
  return false;
}
  
// @brief 範囲のMSBの取得
// @retval 範囲のMSB 範囲を持つとき
// @retval NULL 範囲を持たないとき
// @note このクラスでは NULL を返す．
const PtExpr*
CptDeclHead::left_range() const
{
  return NULL;
}

// @brief 範囲のLSBの取得
// @retval 範囲のLSB 範囲を持つとき
// @retval NULL 範囲を持たないとき
// @note このクラスでは NULL を返す．
const PtExpr*
CptDeclHead::right_range() const
{
  return NULL;
}

// @brief データ型の取得
// @retval データ型 kParam, kLocalParam, kVar の場合
// @retval kVpiVarNone 上記以外
// @note このクラスでは kVpiVarNone を返す．
tVpiVarType
CptDeclHead::data_type() const
{
  return kVpiVarNone;
}
  
// @brief net 型の取得
// @retval net 型 net 型の要素の場合
// @retval kVpiNone net 型の要素でない場合
// @note このクラスでは kVpiNone を返す．
tVpiNetType
CptDeclHead::net_type() const
{
  return kVpiNone;
}

// @brief vectored|scalared 属性の取得
// @retval kVpiVsNone vectored|scalared 指定なし
// @retval kVpiVectored vectored 指定あり
// @retval kVpiScalared scalared 指定あり
// @note このクラスでは kVpiVsNone を返す．
tVpiVsType
CptDeclHead::vs_type() const
{
  return kVpiVsNone;
}

// @brief strength の取得
// @retval strength
// @retval NULL strength の指定なし
// @note このクラスでは NULL を返す．
const PtStrength*
CptDeclHead::strength() const
{
  return NULL;
}

// @brief delay の取得
// @retval delay
// @retval NULL delay の指定なし
// @note このクラスでは NULL を返す．
const PtDelay*
CptDeclHead::delay() const
{
  return NULL;
}

// @brief 要素数の取得
// @return 要素数
ymuint32
CptDeclHead::item_num() const
{
  return mItemArray.size();
}

// @brief 要素の取得
// @param[in] pos 位置番号 ( 0 <= pos < item_num() )
const PtDeclItem*
CptDeclHead::item(ymuint32 pos) const
{
  return mItemArray[pos];
}

// @brief 要素リストの設定
// @param[in] elem_array 要素リスト(配列)
void
CptDeclHead::set_elem(PtDeclItemArray item_array)
{
  mItemArray = item_array;
}


//////////////////////////////////////////////////////////////////////
/// parameter 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
CptParamH::CptParamH(const FileRegion& file_region) :
  CptDeclHead(file_region)
{
}
  
// デストラクタ
CptParamH::~CptParamH()
{
}

// 宣言要素の型の取得
// @return 宣言要素の型
tPtDeclType
CptParamH::type() const
{
  return kPtDecl_Param;
}


//////////////////////////////////////////////////////////////////////
/// localparam 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
CptLocalParamH::CptLocalParamH(const FileRegion& file_region) :
  CptDeclHead(file_region)
{
}
  
// デストラクタ
CptLocalParamH::~CptLocalParamH()
{
}

// 宣言要素の型の取得
// @return 宣言要素の型
tPtDeclType
CptLocalParamH::type() const
{
  return kPtDecl_LocalParam;
}


//////////////////////////////////////////////////////////////////////
/// 符号なし範囲つき(ビットベクタタイプ)の parameter ヘッダのクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
// @param left 範囲の左側の式
// @param right 範囲の右側の式
CptParamHV::CptParamHV(const FileRegion& file_region,
		       PtExpr* left,
		       PtExpr* right) :
  CptParamH(file_region),
  mLeftRange(left),
  mRightRange(right)
{
}

// デストラクタ
CptParamHV::~CptParamHV()
{
}

// 符号の取得
// @retval true 符号つき
// @retval false 符号なし
bool
CptParamHV::is_signed() const
{
  // このクラスは常に false を返す．
  return false;
}

// 範囲のMSBの取得
// @return 範囲のMSB
const PtExpr*
CptParamHV::left_range() const
{
  return mLeftRange;
}

// 範囲のLSBの取得
// @return 範囲のLSB
const PtExpr*
CptParamHV::right_range() const
{
  return mRightRange;
}


//////////////////////////////////////////////////////////////////////
/// 符号付き範囲つき(ビットベクタタイプ)の parameter ヘッダのクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
// @param left 範囲の左側の式
// @param right 範囲の右側の式
CptParamHSV::CptParamHSV(const FileRegion& file_region,
			 PtExpr* left,
			 PtExpr* right) :
  CptParamHV(file_region, left, right)
{
}

// デストラクタ
CptParamHSV::~CptParamHSV()
{
}

// 符号の取得
// @retval true 符号つき
// @retval false 符号なし
bool
CptParamHSV::is_signed() const
{
  // このクラスは常に true を返す．
  return true;
}


//////////////////////////////////////////////////////////////////////
// 符号なし範囲つき(ビットベクタタイプ)の localparam ヘッダのクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
// @param left 範囲の左側の式
// @param right 範囲の右側の式
CptLocalParamHV::CptLocalParamHV(const FileRegion& file_region,
				 PtExpr* left,
				 PtExpr* right) :
  CptParamHV(file_region, left, right)
{
}

// デストラクタ
CptLocalParamHV::~CptLocalParamHV()
{
}

// 宣言要素の型の取得
tPtDeclType
CptLocalParamHV::type() const
{
  return kPtDecl_LocalParam;
}


//////////////////////////////////////////////////////////////////////
// 符号付き範囲つき(ビットベクタタイプ)の localparam ヘッダのクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
// @param left 範囲の左側の式
// @param right 範囲の右側の式
CptLocalParamHSV::CptLocalParamHSV(const FileRegion& file_region,
				   PtExpr* left,
				   PtExpr* right) :
  CptParamHSV(file_region, left, right)
{
}

// デストラクタ
CptLocalParamHSV::~CptLocalParamHSV()
{
}

// 宣言要素の型の取得
tPtDeclType
CptLocalParamHSV::type() const
{
  return kPtDecl_LocalParam;
}


//////////////////////////////////////////////////////////////////////
// 組み込み型の parameter ヘッダのクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
// @param var_type データ型
CptParamHT::CptParamHT(const FileRegion& file_region,
		       tVpiVarType var_type) :
  CptDeclHead(file_region),
  mVarType(var_type)
{
}

// デストラクタ
CptParamHT::~CptParamHT()
{
}

// 宣言要素の型の取得
tPtDeclType
CptParamHT::type() const
{
  return kPtDecl_Param;
}

// データ型の取得
// @retval データ型 kParam, kLocalParam, kVar の場合
// @retval kVpiVarNone 上記以外
tVpiVarType
CptParamHT::data_type() const
{
  return mVarType;
}


//////////////////////////////////////////////////////////////////////
// 組み込み型の localparam ヘッダのクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
// @param var_type データ型
CptLocalParamHT::CptLocalParamHT(const FileRegion& file_region,
				 tVpiVarType var_type) :
  CptParamHT(file_region, var_type)
{
}

// デストラクタ
CptLocalParamHT::~CptLocalParamHT()
{
}

// 宣言要素の型の取得
tPtDeclType
CptLocalParamHT::type() const
{
  return kPtDecl_LocalParam;
}


//////////////////////////////////////////////////////////////////////
// reg 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
    
// コンストラクタ
// @param file_region ファイル位置の情報
CptRegH::CptRegH(const FileRegion& file_region) :
  CptDeclHead(file_region)
{
}

// デストラクタ
CptRegH::~CptRegH()
{
}

// 宣言要素の型の取得
tPtDeclType
CptRegH::type() const
{
  return kPtDecl_Reg;
}


//////////////////////////////////////////////////////////////////////
/// 1ビット符号つき？？？ の reg 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptRegHS::CptRegHS(const FileRegion& file_region) :
  CptRegH(file_region)
{
}

// デストラクタ
CptRegHS::~CptRegHS()
{
}

// 符号の取得
// このクラスでは true を返す．
bool
CptRegHS::is_signed() const
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// ビットベクタ型の reg 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
    
// コンストラクタ
// @param file_region ファイル位置の情報
// @param left 範囲の左側の式
// @param right 範囲の右側の式
CptRegHV::CptRegHV(const FileRegion& file_region,
		   PtExpr* left,
		   PtExpr* right) :
  CptRegH(file_region),
  mLeftRange(left),
  mRightRange(right)
{
}

// デストラクタ
CptRegHV::~CptRegHV()
{
}

// 符号の取得
bool
CptRegHV::is_signed() const
{
  return false;
}

// 範囲のMSBの取得
// @return 範囲のMSB
const PtExpr*
CptRegHV::left_range() const
{
  return mLeftRange;
}

// 範囲のLSBの取得
// @return 範囲のLSB
const PtExpr*
CptRegHV::right_range() const
{
  return mRightRange;
}


//////////////////////////////////////////////////////////////////////
// 符号つきビットベクタ型の reg 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
    
// コンストラクタ
// @param file_region ファイル位置の情報
// @param left 範囲の左側の式
// @param right 範囲の右側の式
CptRegHSV::CptRegHSV(const FileRegion& file_region,
		     PtExpr* left,
		     PtExpr* right) :
  CptRegHV(file_region, left, right)
{
}

// デストラクタ
CptRegHSV::~CptRegHSV()
{
}

// 符号の取得
bool
CptRegHSV::is_signed() const
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// 変数宣言 ヘッダのクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
// @param var_type データ型
CptVarH::CptVarH(const FileRegion& file_region,
		 tVpiVarType var_type) :
  CptDeclHead(file_region),
  mVarType(var_type)
{
}

// デストラクタ
CptVarH::~CptVarH()
{
}

// 宣言要素の型の取得
tPtDeclType
CptVarH::type() const
{
  return kPtDecl_Var;
}

// データ型の取得
// @retval データ型 kParam, kLocalParam, kVar の場合
// @retval kVpiVarNone 上記以外
tVpiVarType
CptVarH::data_type() const
{
  return mVarType;
}


//////////////////////////////////////////////////////////////////////
// genvar宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
CptGenvarH::CptGenvarH(const FileRegion& file_region) :
  CptDeclHead(file_region)
{
}
  
// デストラクタ
CptGenvarH::~CptGenvarH()
{
}

// 宣言要素の型の取得
tPtDeclType
CptGenvarH::type() const
{
  return kPtDecl_Genvar;
}


//////////////////////////////////////////////////////////////////////
// net宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
// @param net_type net の型
CptNetH::CptNetH(const FileRegion& file_region,
		 tVpiNetType net_type,
		 bool sign) :
  CptDeclHead(file_region)
{
  mFlags = (static_cast<ymuint32>(net_type) << 1) | static_cast<ymuint32>(sign);
}

// デストラクタ
CptNetH::~CptNetH()
{
}

// 宣言要素の型の取得
// @return 宣言要素の型
tPtDeclType
CptNetH::type() const
{
  return kPtDecl_Net;
}

// net 型の取得
tVpiNetType
CptNetH::net_type() const
{
  return static_cast<tVpiNetType>((mFlags >> 1) & 15);
}
    
// 符号の有無の取得
bool
CptNetH::is_signed() const
{
  return static_cast<bool>(mFlags & 1);
}


//////////////////////////////////////////////////////////////////////
// strength つきの net宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
  
// コンストラクタ
// @param file_region ファイル位置の情報
// @param net_type net の型
// @param strength 信号強度
CptNetHS::CptNetHS(const FileRegion& file_region,
		   tVpiNetType net_type,
		   bool sign,
		   PtStrength* strength) :
  CptNetH(file_region, net_type, sign),
  mStrength(strength)
{
}

// デストラクタ
CptNetHS::~CptNetHS()
{
}

// strength の取得
const PtStrength*
CptNetHS::strength() const
{
  return mStrength;
}


//////////////////////////////////////////////////////////////////////
// delay つきの net宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
  
// コンストラクタ
// @param file_region ファイル位置の情報
// @param net_type net の型
// @param delay 遅延
CptNetHD::CptNetHD(const FileRegion& file_region,
		   tVpiNetType net_type,
		   bool sign,
		   PtDelay* delay) :
  CptNetH(file_region, net_type, sign),
  mDelay(delay)
{
}

// デストラクタ
CptNetHD::~CptNetHD()
{
}

// delay の取得
const PtDelay*
CptNetHD::delay() const
{
  return mDelay;
}


//////////////////////////////////////////////////////////////////////
// strength と delay つきの net宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
  
// コンストラクタ
// @param file_region ファイル位置の情報
// @param net_type net の型
// @param strength 信号強度
// @param delay 遅延
CptNetHSD::CptNetHSD(const FileRegion& file_region,
		     tVpiNetType net_type,
		     bool sign,
		     PtStrength* strength,
		     PtDelay* delay) :
  CptNetH(file_region, net_type, sign),
  mStrength(strength),
  mDelay(delay)
{
}

// デストラクタ
CptNetHSD::~CptNetHSD()
{
}

// strength の取得
const PtStrength*
CptNetHSD::strength() const
{
  return mStrength;
}

// delay の取得
const PtDelay*
CptNetHSD::delay() const
{
  return mDelay;
}


//////////////////////////////////////////////////////////////////////
// ビットベクタ型の net宣言のヘッダを表すクラス 
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
// @param net_type net の型
// @param vstype vectored/scalared 属性を表す値
// @param sign 符号の有無を表すフラグ
// @param left 範囲の左側の式
// @param right 範囲の右側の式
CptNetHV::CptNetHV(const FileRegion& file_region,
		   tVpiNetType net_type,
		   tVpiVsType vstype,
		   bool sign,
		   PtExpr* left,
		   PtExpr* right) :
  CptNetH(file_region, net_type, sign),
  mLeftRange(left),
  mRightRange(right)
{
  mFlags |= (static_cast<ymuint32>(vstype) << 5);
}

// デストラクタ
CptNetHV::~CptNetHV()
{
}

// vectored|scalared 属性の取得
// @retval kVpiVsNone vectored|scalared 指定なし
// @retval kVpiVectored vectored 指定あり
// @retval kVpiScalared scalared 指定あり
tVpiVsType
CptNetHV::vs_type() const
{
  return static_cast<tVpiVsType>(mFlags >> 5);
}

// 範囲のMSBの取得
const PtExpr*
CptNetHV::left_range() const
{
  return mLeftRange;
}

// 範囲のLSBの取得
const PtExpr*
CptNetHV::right_range() const
{
  return mRightRange;
}


//////////////////////////////////////////////////////////////////////
// strength つきの CptNetHV
//////////////////////////////////////////////////////////////////////
  
// コンストラクタ
// @param file_region ファイル位置の情報
// @param net_type net の型
// @param vstype vectored/scalared 属性を表す値
// @param sign 符号の有無を表すフラグ
// @param left 範囲の左側の式
// @param right 範囲の右側の式
// @param strength 信号強度
CptNetHVS::CptNetHVS(const FileRegion& file_region,
		     tVpiNetType net_type,
		     tVpiVsType vstype,
		     bool sign,
		     PtExpr* left,
		     PtExpr* right,
		     PtStrength* strength) :
  CptNetHV(file_region, net_type,
	   vstype, sign, left, right),
  mStrength(strength)
{
}

// デストラクタ
CptNetHVS::~CptNetHVS()
{
}

// strength の取得
const PtStrength*
CptNetHVS::strength() const
{
  return mStrength;
}


//////////////////////////////////////////////////////////////////////
// delay つきの CptNetHV
//////////////////////////////////////////////////////////////////////
  
// コンストラクタ
// @param file_region ファイル位置の情報
// @param net_type net の型
// @param vstype vectored/scalared 属性を表す値
// @param sign 符号の有無を表すフラグ
// @param left 範囲の左側の式
// @param right 範囲の右側の式
// @param delay 遅延
CptNetHVD::CptNetHVD(const FileRegion& file_region,
		     tVpiNetType net_type,
		     tVpiVsType vstype,
		     bool sign,
		     PtExpr* left,
		     PtExpr* right,
		     PtDelay* delay) :
  CptNetHV(file_region, net_type,
	   vstype, sign, left, right),
  mDelay(delay)
{
}

// デストラクタ
CptNetHVD::~CptNetHVD()
{
}

// delay の取得
const PtDelay*
CptNetHVD::delay() const
{
  return mDelay;
}


//////////////////////////////////////////////////////////////////////
// strength と delay つきの CptNetHV
//////////////////////////////////////////////////////////////////////
  
// コンストラクタ
// @param file_region ファイル位置の情報
// @param net_type net の型
// @param vstype vectored/scalared 属性を表す値
// @param sign 符号の有無を表すフラグ
// @param left 範囲の左側の式
// @param right 範囲の右側の式
// @param strength 信号強度
// @param delay 遅延
CptNetHVSD::CptNetHVSD(const FileRegion& file_region,
		       tVpiNetType net_type,
		       tVpiVsType vstype,
		       bool sign,
		       PtExpr* left,
		       PtExpr* right,
		       PtStrength* strength,
		       PtDelay* delay) :
  CptNetHV(file_region, net_type,
	   vstype, sign, left, right),
  mStrength(strength),
  mDelay(delay)
{
}

// デストラクタ
CptNetHVSD::~CptNetHVSD()
{
}

// strength の取得
const PtStrength*
CptNetHVSD::strength() const
{
  return mStrength;
}

// delay の取得
const PtDelay*
CptNetHVSD::delay() const
{
  return mDelay;
}


//////////////////////////////////////////////////////////////////////
// event 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
CptEventH::CptEventH(const FileRegion& file_region) :
  CptDeclHead(file_region)
{
}
  
// デストラクタ
CptEventH::~CptEventH()
{
}

// 宣言要素の型の取得
tPtDeclType
CptEventH::type() const
{
  return kPtDecl_Event;
}


//////////////////////////////////////////////////////////////////////
// specparam 宣言のヘッダのベース実装クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
CptSpecParamH::CptSpecParamH(const FileRegion& file_region) :
  CptDeclHead(file_region)
{
}
  
// デストラクタ
CptSpecParamH::~CptSpecParamH()
{
}

// 宣言要素の型の取得
tPtDeclType
CptSpecParamH::type() const
{
  return kPtDecl_SpecParam;
}


//////////////////////////////////////////////////////////////////////
// ビットベクタ型の specparam
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
// @param left 範囲の左側の式
// @param right 範囲の右側の式
CptSpecParamHV::CptSpecParamHV(const FileRegion& file_region,
			       PtExpr* left,
			       PtExpr* right) :
  CptSpecParamH(file_region),
  mLeftRange(left),
  mRightRange(right)
{
}

// デストラクタ
CptSpecParamHV::~CptSpecParamHV()
{
}

// 範囲のMSBの取得
const PtExpr*
CptSpecParamHV::left_range() const
{
  return mLeftRange;
}

// 範囲のLSBの取得
const PtExpr*
CptSpecParamHV::right_range() const
{
  return mRightRange;
}


//////////////////////////////////////////////////////////////////////
// 宣言要素のベース実装クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param name 名前
CptDeclItemBase::CptDeclItemBase(const char* name) :
  mName(name)
{
}
  
// デストラクタ
CptDeclItemBase::~CptDeclItemBase()
{
}

// 名前の取得
const char*
CptDeclItemBase::name() const
{
  return mName;
}

// dimension list のサイズの取得
// @return ここでは常に 0 を返す．
ymuint32
CptDeclItemBase::dimension_list_size() const
{
  return 0;
}

// 範囲の取得
const PtRange*
CptDeclItemBase::range(ymuint32 pos) const
{
  return NULL;
}

// 初期値の取得
// @return ここでは常に NULL を返す．
const PtExpr*
CptDeclItemBase::init_value() const
{
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// 宣言要素のベース実装クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
// @param name 名前
CptDeclItem::CptDeclItem(const FileRegion& file_region,
			 const char* name) :
  CptDeclItemBase(name),
  mLoc(file_region)
{
}

// デストラクタ
CptDeclItem::~CptDeclItem()
{
}

// ファイル位置の取得
FileRegion
CptDeclItem::file_region() const
{
  return mLoc;
}


//////////////////////////////////////////////////////////////////////
// 範囲をもった宣言要素のクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
// @param name 名前
// @param range_array 範囲のリスト
CptDeclItemR::CptDeclItemR(const FileRegion& file_region,
			   const char* name,
			   PtRangeArray range_array) :
  CptDeclItemBase(name),
  mFileRegion(file_region),
  mRangeArray(range_array)
{
}

// デストラクタ
CptDeclItemR::~CptDeclItemR()
{
}

// ファイル位置の取得
FileRegion
CptDeclItemR::file_region() const
{
  return mFileRegion;
}

// dimension list のサイズの取得
ymuint32
CptDeclItemR::dimension_list_size() const
{
  return mRangeArray.size();
}

// 範囲の取得
const PtRange*
CptDeclItemR::range(ymuint32 pos) const
{
  return mRangeArray[pos];
}


//////////////////////////////////////////////////////////////////////
// 初期値をもった宣言要素のクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
// @param name 名前
// @param init_value 初期値
CptDeclItemI::CptDeclItemI(const FileRegion& file_region,
			 const char* name,
			 PtExpr* init_value) :
  CptDeclItem(file_region, name),
  mInitValue(init_value)
{
  assert_cond(init_value, __FILE__, __LINE__);
}

// デストラクタ
CptDeclItemI::~CptDeclItemI()
{
}

// ファイル位置の取得
FileRegion
CptDeclItemI::file_region() const
{
  return FileRegion(CptDeclItem::file_region(), mInitValue->file_region());
}

// 初期値の取得
// @retval 初期値
// @retval NULL 設定がない場合
const PtExpr*
CptDeclItemI::init_value() const
{
  return mInitValue;
}


//////////////////////////////////////////////////////////////////////
// @class CptRange CptDecl.h "CptDecl.h"
// @brief 範囲を表すクラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CptRange::CptRange(const FileRegion& fr,
		   PtExpr* msb,
		   PtExpr* lsb) :
  mFileRegion(fr),
  mMsb(msb),
  mLsb(lsb)
{
}

// @brief デストラクタ
CptRange::~CptRange()
{
}

// ファイル位置の取得
FileRegion
CptRange::file_region() const
{
  return mFileRegion;
}

// @brief 範囲の MSB を取り出す．
const PtExpr*
CptRange::left() const
{
  return mMsb;
}

// @brief 範囲の LSB を取り出す．
const PtExpr*
CptRange::right() const
{
  return mLsb;
}

  
//////////////////////////////////////////////////////////////////////
// その他の宣言関係
//////////////////////////////////////////////////////////////////////

// パラメータ宣言のヘッダの生成 (型指定なし)
PtiDeclHead*
CptFactory::new_ParamH(const FileRegion& file_region)
{
  ++ mNumParamH;
  void* p = alloc().get_memory(sizeof(CptParamH));
  return new (p) CptParamH(file_region);
}

// 範囲指定型パラメータ宣言のヘッダの生成
PtiDeclHead*
CptFactory::new_ParamH(const FileRegion& file_region,
		       bool sign,
		       PtExpr* left,
		       PtExpr* right)
{
  ++ mNumParamHV;
  if ( sign ) {
    void* p = alloc().get_memory(sizeof(CptParamHSV));
    return new (p) CptParamHSV(file_region, left, right);
  }
  else {
    void* p = alloc().get_memory(sizeof(CptParamHV));
    return new (p) CptParamHV(file_region, left, right);
  }
}

// 組み込み型パラメータ宣言のヘッダの生成
PtiDeclHead*
CptFactory::new_ParamH(const FileRegion& file_region,
		       tVpiVarType var_type)
{
  ++ mNumParamHT;
  void* p = alloc().get_memory(sizeof(CptParamHT));
  return new (p) CptParamHT(file_region, var_type);
}

// local param 宣言のヘッダの生成 (型指定なし)
PtiDeclHead*
CptFactory::new_LocalParamH(const FileRegion& file_region)
{
  ++ mNumLocalParamH;
  void* p = alloc().get_memory(sizeof(CptLocalParamH));
  return new (p) CptLocalParamH(file_region);
}

// 範囲指定型 local param 宣言のヘッダの生成
PtiDeclHead*
CptFactory::new_LocalParamH(const FileRegion& file_region,
			    bool sign,
			    PtExpr* left,
			    PtExpr* right)
{
  ++ mNumLocalParamHV;
  if ( sign ) {
    void* p = alloc().get_memory(sizeof(CptLocalParamHSV));
    return new (p) CptLocalParamHSV(file_region, left, right);
  }
  else {
    void* p = alloc().get_memory(sizeof(CptLocalParamHV));
    return new (p) CptLocalParamHV(file_region, left, right);
  }
}

// 組み込み型 local param 宣言のヘッダの生成
PtiDeclHead*
CptFactory::new_LocalParamH(const FileRegion& file_region,
			    tVpiVarType var_type)
{
  ++ mNumLocalParamHT;
  void* p = alloc().get_memory(sizeof(CptLocalParamHT));
  return new (p) CptLocalParamHT(file_region, var_type);
}

// specparam 宣言のヘッダを生成する．
PtiDeclHead*
CptFactory::new_SpecParamH(const FileRegion& file_region)
{
  ++ mNumSpecParamH;
  void* p = alloc().get_memory(sizeof(CptSpecParamH));
  return new (p) CptSpecParamH(file_region);
}

// specparam 宣言のヘッダを生成する．
PtiDeclHead*
CptFactory::new_SpecParamH(const FileRegion& file_region,
			   PtExpr* left,
			   PtExpr* right)
{
  ++ mNumSpecParamHV;
  void* p = alloc().get_memory(sizeof(CptSpecParamHV));
  return new (p) CptSpecParamHV(file_region, left, right);
}

// event 宣言のヘッダを生成する．
PtiDeclHead*
CptFactory::new_EventH(const FileRegion& file_region)
{
  ++ mNumEventH;
  void* p = alloc().get_memory(sizeof(CptEventH));
  return new (p) CptEventH(file_region);
}

// genvar 宣言のヘッダを生成する．
PtiDeclHead*
CptFactory::new_GenvarH(const FileRegion& file_region)
{
  ++ mNumGenvarH;
  void* p = alloc().get_memory(sizeof(CptGenvarH));
  return new (p) CptGenvarH(file_region);
}

// 変数宣言のヘッダを生成する．
PtiDeclHead*
CptFactory::new_VarH(const FileRegion& file_region,
		     tVpiVarType var_type)
{
  ++ mNumVarH;
  void* p = alloc().get_memory(sizeof(CptVarH));
  return new (p) CptVarH(file_region, var_type);
}

// reg 宣言のヘッダを生成する．
PtiDeclHead*
CptFactory::new_RegH(const FileRegion& file_region,
		     bool sign)
{
  ++ mNumRegH;
  if ( sign ) {
    void* p = alloc().get_memory(sizeof(CptRegHS));
    return new (p) CptRegHS(file_region);
  }
  else {
    void* p = alloc().get_memory(sizeof(CptRegH));
    return new (p) CptRegH(file_region);
  }
}

// reg 宣言のヘッダを生成する．
PtiDeclHead*
CptFactory::new_RegH(const FileRegion& file_region,
		     bool sign,
		     PtExpr* left,
		     PtExpr* right)
{
  ++ mNumRegHV;
  if ( sign ) {
    void* p = alloc().get_memory(sizeof(CptRegHSV));
    return new (p) CptRegHSV(file_region, left, right);
  }
  else {
    void* p = alloc().get_memory(sizeof(CptRegHV));
    return new (p) CptRegHV(file_region, left, right);
  }
}
  
// net 宣言のヘッダを生成する．
PtiDeclHead*
CptFactory::new_NetH(const FileRegion& file_region,
		     tVpiNetType type,
		     bool sign)
{
  ++ mNumNetH;
  void* p = alloc().get_memory(sizeof(CptNetH));
  return new (p) CptNetH(file_region,
			 type, sign);
}
  
// net 宣言のヘッダを生成する．
PtiDeclHead*
CptFactory::new_NetH(const FileRegion& file_region,
		     tVpiNetType type,
		     bool sign,
		     PtStrength* strength)
{
  ++ mNumNetHS;
  return new CptNetHS(file_region,
		      type, sign,
		      strength);
}
  
// net 宣言のヘッダを生成する．
PtiDeclHead*
CptFactory::new_NetH(const FileRegion& file_region,
		     tVpiNetType type,
		     bool sign,
		     PtDelay* delay)
{
  ++ mNumNetHD;
  void* p = alloc().get_memory(sizeof(CptNetHD));
  return new (p) CptNetHD(file_region,
			  type, sign,
			  delay);
}
  
// net 宣言のヘッダを生成する．
PtiDeclHead*
CptFactory::new_NetH(const FileRegion& file_region,
		     tVpiNetType type,
		     bool sign,
		     PtStrength* strength,
		     PtDelay* delay)
{
  ++mNumNetHSD;
  void* p = alloc().get_memory(sizeof(CptNetHSD));
  return new (p) CptNetHSD(file_region,
			   type, sign,
			   strength, delay);
}
  
// net 宣言のヘッダを生成する．
PtiDeclHead*
CptFactory::new_NetH(const FileRegion& file_region,
		     tVpiNetType type,
		     tVpiVsType vstype,
		     bool sign,
		     PtExpr* left,
		     PtExpr* right)
{
  ++ mNumNetHV;
  void* p = alloc().get_memory(sizeof(CptNetHV));
  return new (p) CptNetHV(file_region,
			  type, vstype, sign, left, right);
}
  
// net 宣言のヘッダを生成する．
PtiDeclHead*
CptFactory::new_NetH(const FileRegion& file_region,
		     tVpiNetType type,
		     tVpiVsType vstype,
		     bool sign,
		     PtExpr* left,
		     PtExpr* right,
		     PtStrength* strength)
{
  ++ mNumNetHVS;
  void* p = alloc().get_memory(sizeof(CptNetHVS));
  return new (p) CptNetHVS(file_region,
			   type, vstype, sign, left, right,
			   strength);
}
  
// net 宣言のヘッダを生成する．
PtiDeclHead*
CptFactory::new_NetH(const FileRegion& file_region,
		     tVpiNetType type,
		     tVpiVsType vstype,
		     bool sign,
		     PtExpr* left,
		     PtExpr* right,
		     PtDelay* delay)
{
  ++ mNumNetHVD;
  void* p = alloc().get_memory(sizeof(CptNetHVD));
  return new (p) CptNetHVD(file_region,
			   type, vstype, sign, left, right,
			   delay);
}
  
// net 宣言のヘッダを生成する．
PtiDeclHead*
CptFactory::new_NetH(const FileRegion& file_region,
		     tVpiNetType type,
		     tVpiVsType vstype,
		     bool sign,
		     PtExpr* left,
		     PtExpr* right,
		     PtStrength* strength,
		     PtDelay* delay)
{
  ++ mNumNetHVSD;
  void* p = alloc().get_memory(sizeof(CptNetHVSD));
  return new (p) CptNetHVSD(file_region,
			    type, vstype, sign, left, right,
			    strength, delay);
}

// 宣言要素を生成する．
PtDeclItem*
CptFactory::new_DeclItem(const FileRegion& file_region,
			 const char* name)
{
  ++ mNumDeclItem;
  void* p = alloc().get_memory(sizeof(CptDeclItem));
  return new (p) CptDeclItem(file_region, name);
}

PtDeclItem*
CptFactory::new_DeclItem(const FileRegion& file_region,
			 const char* name,
			 PtExpr* init_value)
{
  ++ mNumDeclItemI;
  void* p = alloc().get_memory(sizeof(CptDeclItemI));
  return new (p) CptDeclItemI(file_region, name, init_value);
}

PtDeclItem*
CptFactory::new_DeclItem(const FileRegion& file_region,
			 const char* name,
			 PtRangeArray range_array)
{
  ++ mNumDeclItemR;
  void* p = alloc().get_memory(sizeof(CptDeclItemR));
  return new (p) CptDeclItemR(file_region, name, range_array);
}

PtRange*
CptFactory::new_Range(const FileRegion& fr,
		      PtExpr* msb,
		      PtExpr* lsb)
{
  void* p = alloc().get_memory(sizeof(CptRange));
  return new (p) CptRange(fr, msb, lsb);
}

END_NAMESPACE_YM_VERILOG
