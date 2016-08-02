#ifndef LIBYM_VERILOG_PT_COMPACT_CPTDECL_H
#define LIBYM_VERILOG_PT_COMPACT_CPTDECL_H

/// @file libym_verilog/pt_compact/CptDecl.h
/// @brief CptDecl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CptDecl.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/pt/PtDecl.h"
#include "ym_verilog/pt/PtArray.h"

#include "ym_utils/FileRegion.h"

#include "PtiDecl.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// 宣言要素のヘッダの基底クラス
//////////////////////////////////////////////////////////////////////
class CptDeclHead :
  public PtiDeclHead
{
protected:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  CptDeclHead(const FileRegion& file_region);

  /// デストラクタ
  virtual
  ~CptDeclHead();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置の取得
  /// @return ファイル位置
  virtual
  FileRegion
  file_region() const;

  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  /// @note このクラスでは false を返す．
  virtual
  bool
  is_signed() const;
  
  /// @brief 範囲のMSBの取得
  /// @retval 範囲のMSB 範囲を持つとき
  /// @retval NULL 範囲を持たないとき
  /// @note このクラスでは NULL を返す．
  virtual
  const PtExpr*
  left_range() const;

  /// @brief 範囲のLSBの取得
  /// @retval 範囲のLSB 範囲を持つとき
  /// @retval NULL 範囲を持たないとき
  /// @note このクラスでは NULL を返す．
  virtual
  const PtExpr*
  right_range() const;

  /// @brief データ型の取得
  /// @retval データ型 kParam, kLocalParam, kVar の場合
  /// @retval kVpiVarNone 上記以外
  /// @note このクラスでは kVpiVarNone を返す．
  virtual
  tVpiVarType
  data_type() const;
  
  /// @brief net 型の取得
  /// @retval net 型 net 型の要素の場合
  /// @retval kVpiNone net 型の要素でない場合
  /// @note このクラスでは kVpiNone を返す．
  virtual
  tVpiNetType
  net_type() const;

  /// @brief vectored|scalared 属性の取得
  /// @retval kVpiVsNone vectored|scalared 指定なし
  /// @retval kVpiVectored vectored 指定あり
  /// @retval kVpiScalared scalared 指定あり
  /// @note このクラスでは kVpiVsNone を返す．
  virtual
  tVpiVsType
  vs_type() const;

  /// @brief strength の取得
  /// @retval strength
  /// @retval NULL strength の指定なし
  /// @note このクラスでは NULL を返す．
  virtual
  const PtStrength*
  strength() const;

  /// @brief delay の取得
  /// @retval delay
  /// @retval NULL delay の指定なし
  /// @note このクラスでは NULL を返す．
  virtual
  const PtDelay*
  delay() const;

  /// @brief 要素数の取得
  /// @return 要素数
  virtual
  ymuint32
  item_num() const;

  /// @brief 要素の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < item_num() )
  virtual
  const PtDeclItem*
  item(ymuint32 pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // PtiDeclHead の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素リストの設定
  /// @param[in] elem_array 要素リスト(配列)
  virtual
  void
  set_elem(PtDeclItemArray elem_array);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 要素の配列
  PtDeclItemArray mItemArray;

};


//////////////////////////////////////////////////////////////////////
/// parameter 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptParamH :
  public CptDeclHead
{
  friend class CptFactory;
  
protected:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  CptParamH(const FileRegion& file_region);
  
  /// @brief デストラクタ
  virtual
  ~CptParamH();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスを識別するための型を返す．
  /// @return 宣言要素の型
  virtual
  tPtDeclType
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// localparam 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptLocalParamH :
  public CptDeclHead
{
  friend class CptFactory;
  
protected:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  CptLocalParamH(const FileRegion& file_region);
  
  /// @brief デストラクタ
  virtual
  ~CptLocalParamH();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスを識別するための型を返す．
  /// @return 宣言要素の型
  virtual
  tPtDeclType
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// 符号なし範囲つき(ビットベクタタイプ)の parameter ヘッダのクラス
//////////////////////////////////////////////////////////////////////
class CptParamHV :
  public CptParamH
{
  friend class CptFactory;
  
protected:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  CptParamHV(const FileRegion& file_region,
	     PtExpr* left,
	     PtExpr* right);

  /// @brief デストラクタ
  virtual
  ~CptParamHV();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  virtual
  bool
  is_signed() const;

  /// @brief 範囲のMSBの取得
  /// @return 範囲のMSB
  virtual
  const PtExpr*
  left_range() const;

  /// @brief 範囲のLSBの取得
  /// @return 範囲のLSB
  virtual
  const PtExpr*
  right_range() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 範囲のMSB
  PtExpr* mLeftRange;

  // 範囲のLSB
  PtExpr* mRightRange;
  
};


//////////////////////////////////////////////////////////////////////
/// 符号付き範囲つき(ビットベクタタイプ)の parameter ヘッダのクラス
//////////////////////////////////////////////////////////////////////
class CptParamHSV :
  public CptParamHV
{
  friend class CptFactory;
  
protected:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  CptParamHSV(const FileRegion& file_region,
	      PtExpr* left,
	      PtExpr* right);

  /// @brief デストラクタ
  virtual
  ~CptParamHSV();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  virtual
  bool
  is_signed() const;
  
};


//////////////////////////////////////////////////////////////////////
/// 符号なし範囲つき(ビットベクタタイプ)の localparam ヘッダのクラス
//////////////////////////////////////////////////////////////////////
class CptLocalParamHV :
  public CptParamHV
{
  friend class CptFactory;
  
protected:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  CptLocalParamHV(const FileRegion& file_region,
		  PtExpr* left,
		  PtExpr* right);

  /// @brief デストラクタ
  virtual
  ~CptLocalParamHV();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスを識別するための型を返す．
  /// @return 宣言要素の型
  virtual
  tPtDeclType
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// 符号付き範囲つき(ビットベクタタイプ)の localparam ヘッダのクラス
//////////////////////////////////////////////////////////////////////
class CptLocalParamHSV :
  public CptParamHSV
{
  friend class CptFactory;
  
protected:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  CptLocalParamHSV(const FileRegion& file_region,
		   PtExpr* left,
		   PtExpr* right);

  /// @brief デストラクタ
  virtual
  ~CptLocalParamHSV();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスを識別するための型を返す．
  /// @return 宣言要素の型
  virtual
  tPtDeclType
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// 組み込み型 parameter ヘッダのクラス
//////////////////////////////////////////////////////////////////////
class CptParamHT :
  public CptDeclHead
{
  friend class CptFactory;
  
protected:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] var_type データ型
  CptParamHT(const FileRegion& file_region,
	     tVpiVarType var_type);

  /// @brief デストラクタ
  virtual
  ~CptParamHT();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスを識別するための型を返す．
  /// @return 宣言要素の型
  virtual
  tPtDeclType
  type() const;

  /// @brief データ型の取得
  /// @retval データ型 kParam, kLocalParam, kVar の場合
  /// @retval kVpiVarNone 上記以外
  virtual
  tVpiVarType
  data_type() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // データ型
  tVpiVarType mVarType;

};


//////////////////////////////////////////////////////////////////////
/// 組み込み型 localparam ヘッダのクラス
//////////////////////////////////////////////////////////////////////
class CptLocalParamHT :
  public CptParamHT
{
  friend class CptFactory;
  
protected:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] var_type データ型
  CptLocalParamHT(const FileRegion& file_region,
		  tVpiVarType var_type);

  /// @brief デストラクタ
  virtual
  ~CptLocalParamHT();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスを識別するための型を返す．
  /// @return 宣言要素の型
  virtual
  tPtDeclType
  type() const;
  
};


//////////////////////////////////////////////////////////////////////
/// reg 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptRegH :
  public CptDeclHead
{
  friend class CptFactory;
  
protected:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  CptRegH(const FileRegion& file_region);

  /// @brief デストラクタ
  virtual
  ~CptRegH();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスを識別するための型を返す．
  /// @return 宣言要素の型
  virtual
  tPtDeclType
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// 1ビット符号つき？？？ の reg 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptRegHS :
  public CptRegH
{
  friend class CptFactory;
  
protected:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  CptRegHS(const FileRegion& file_region);

  /// @brief デストラクタ
  virtual
  ~CptRegHS();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 符号の取得
  /// @return このクラスでは常に true を返す．
  virtual
  bool
  is_signed() const;

};


//////////////////////////////////////////////////////////////////////
/// ビットベクタ型の reg 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptRegHV :
  public CptRegH
{
  friend class CptFactory;
  
protected:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  CptRegHV(const FileRegion& file_region,
	   PtExpr* left,
	   PtExpr* right);

  /// @brief デストラクタ
  ~CptRegHV();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////
    
  /// @brief 符号の有無の取得
  /// @return このクラスでは常に false を返す．
  virtual
  bool
  is_signed() const;

  /// @brief 範囲のMSBの取得
  /// @return 範囲のMSB
  virtual
  const PtExpr*
  left_range() const;

  /// @brief 範囲のLSBの取得
  /// @return 範囲のLSB
  virtual
  const PtExpr*
  right_range() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 範囲のMSB
  PtExpr* mLeftRange;

  // 範囲のLSB
  PtExpr* mRightRange;
  
};


//////////////////////////////////////////////////////////////////////
/// 符号つきビットベクタ型の reg 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptRegHSV :
  public CptRegHV
{
  friend class CptFactory;
  
protected:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  CptRegHSV(const FileRegion& file_region,
	    PtExpr* left,
	    PtExpr* right);

  /// @brief デストラクタ
  ~CptRegHSV();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////
    
  /// @brief 符号の有無の取得
  /// @return このクラスでは常に true を返す．
  virtual
  bool
  is_signed() const;
  
};


//////////////////////////////////////////////////////////////////////
/// 変数宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptVarH :
  public CptDeclHead
{
  friend class CptFactory;
  
protected:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] var_type データ型
  CptVarH(const FileRegion& file_region,
	  tVpiVarType var_type);

  /// @brief デストラクタ
  virtual
  ~CptVarH();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスを識別するための型を返す．
  /// @return 宣言要素の型
  virtual
  tPtDeclType
  type() const;

  /// @brief データ型の取得
  /// @retval データ型 kParam, kLocalParam, kVar の場合
  /// @retval kVpiVarNone 上記以外
  virtual
  tVpiVarType
  data_type() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // データ型
  tVpiVarType
  mVarType;

};


//////////////////////////////////////////////////////////////////////
/// genvar宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptGenvarH :
  public CptDeclHead
{
  friend class CptFactory;
  
protected:
  
  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] elem_top 要素のリスト
  CptGenvarH(const FileRegion& file_region);
  
  /// @brief デストラクタ
  virtual
  ~CptGenvarH();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスを識別するための型を返す．
  /// @return 宣言要素の型
  virtual
  tPtDeclType
  type() const;
  
};


//////////////////////////////////////////////////////////////////////
/// net宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptNetH :
  public CptDeclHead
{
  friend class CptFactory;
  
protected:
  
  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] net_type net の型
  /// @param[in] sign 符号付きのとき true となるフラグ
  CptNetH(const FileRegion& file_region,
	  tVpiNetType net_type,
	  bool sign);

  /// @brief デストラクタ
  virtual
  ~CptNetH();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスを識別するための型を返す．
  /// @return 宣言要素の型
  virtual
  tPtDeclType
  type() const;

  /// @brief net type を返す．(vpiWire など)
  /// @return net 型
  virtual
  tVpiNetType
  net_type() const;
    
  /// @brief 符号の有無の取得
  virtual
  bool
  is_signed() const;


protected:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // net type + sign (+ vstype)
  ymuint32 mFlags;

};


//////////////////////////////////////////////////////////////////////
/// strength つきの net宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptNetHS :
  public CptNetH
{
  friend class CptFactory;
  
protected:
  
  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] net_type net の型
  /// @param[in] sign 符号付きのとき true となるフラグ
  /// @param[in] strength 信号強度
  CptNetHS(const FileRegion& file_region,
	   tVpiNetType net_type,
	   bool sign,
	   PtStrength* strength);

  /// @brief デストラクタ
  virtual
  ~CptNetHS();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief strength を返す．
  /// @return strength
  virtual
  const PtStrength*
  strength() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  /// strength
  PtStrength* mStrength;

};


//////////////////////////////////////////////////////////////////////
/// delay つきの net宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptNetHD :
  public CptNetH
{
  friend class CptFactory;
  
protected:
  
  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] net_type net の型
  /// @param[in] sign 符号付きのとき true となるフラグ
  /// @param[in] delay 遅延
  CptNetHD(const FileRegion& file_region,
	   tVpiNetType net_type,
	   bool sign,
	   PtDelay* delay);

  /// @brief デストラクタ
  virtual
  ~CptNetHD();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief delay を返す．
  /// @return delay
  virtual
  const PtDelay*
  delay() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // delay
  PtDelay* mDelay;

};


//////////////////////////////////////////////////////////////////////
/// strength と delay つきの net宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptNetHSD :
  public CptNetH
{
  friend class CptFactory;
  
protected:
  
  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] net_type net の型
  /// @param[in] strength 信号強度
  /// @param[in] sign 符号付きのとき true となるフラグ
  /// @param[in] delay 遅延
  CptNetHSD(const FileRegion& file_region,
	    tVpiNetType net_type,
	    bool sign,
	    PtStrength* strength,
	    PtDelay* delay);

  /// @brief デストラクタ
  virtual
  ~CptNetHSD();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief strength を返す．
  /// @return strength
  virtual
  const PtStrength*
  strength() const;

  /// @brief delay を返す．
  /// @return delay
  virtual
  const PtDelay*
  delay() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // strength
  PtStrength* mStrength;

  // delay
  PtDelay* mDelay;

};


//////////////////////////////////////////////////////////////////////
/// ビットベクタ型の net宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptNetHV :
  public CptNetH
{
  friend class CptFactory;
  
protected:
  
  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] net_type net の型
  /// @param[in] vstype vectored/scalared 属性を表す値
  /// @param[in] sign 符号の有無を表すフラグ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  CptNetHV(const FileRegion& file_region,
	   tVpiNetType net_type,
	   tVpiVsType vstype,
	   bool sign,
	   PtExpr* left,
	   PtExpr* right);

  /// @brief デストラクタ
  virtual
  ~CptNetHV();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief vectored/scalared 属性を返す．
  /// @retval kVpiVsNone vectored|scalared 指定なし
  /// @retval kVpiVectored vectored 指定あり
  /// @retval kVpiScalared scalared 指定あり
  virtual
  tVpiVsType
  vs_type() const;

  /// @brief 範囲のMSBを取り出す．
  /// @return 範囲のMSB
  virtual
  const PtExpr*
  left_range() const;

  /// @brief 範囲のLSBを取り出す．
  /// @return 範囲のLSB
  virtual
  const PtExpr*
  right_range() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 範囲のMSB
  PtExpr* mLeftRange;

  // 範囲のLSB
  PtExpr* mRightRange;

};


//////////////////////////////////////////////////////////////////////
/// strength つきの PtNetHV
//////////////////////////////////////////////////////////////////////
class CptNetHVS :
  public CptNetHV
{
  friend class CptFactory;
  
protected:
  
  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] net_type net の型
  /// @param[in] vstype vectored/scalared 属性を表す値
  /// @param[in] sign 符号の有無を表すフラグ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] strength 信号強度
  CptNetHVS(const FileRegion& file_region,
	    tVpiNetType net_type,
	    tVpiVsType vstype,
	    bool sign,
	    PtExpr* left,
	    PtExpr* right,
	    PtStrength* strength);

  /// @brief デストラクタ
  virtual
  ~CptNetHVS();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief strength を返す．
  /// @return strength
  virtual
  const PtStrength*
  strength() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // strength
  PtStrength* mStrength;

};


//////////////////////////////////////////////////////////////////////
/// delay つきの PtNetHV
//////////////////////////////////////////////////////////////////////
class CptNetHVD :
  public CptNetHV
{
  friend class CptFactory;
  
protected:
  
  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] net_type net の型
  /// @param[in] vstype vectored/scalared 属性を表す値
  /// @param[in] sign 符号の有無を表すフラグ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] delay 遅延
  CptNetHVD(const FileRegion& file_region,
	    tVpiNetType net_type,
	    tVpiVsType vstype,
	    bool sign,
	    PtExpr* left,
	    PtExpr* right,
	    PtDelay* delay);

  /// @brief デストラクタ
  virtual
  ~CptNetHVD();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief delay を返す．
  /// @return delay
  virtual
  const PtDelay*
  delay() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // delay
  PtDelay* mDelay;

};


//////////////////////////////////////////////////////////////////////
/// strength と delay つきの PtNetHV
//////////////////////////////////////////////////////////////////////
class CptNetHVSD :
  public CptNetHV
{
  friend class CptFactory;
  
protected:
  
  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] net_type net の型
  /// @param[in] vstype vectored/scalared 属性を表す値
  /// @param[in] sign 符号の有無を表すフラグ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] strength 信号強度
  /// @param[in] delay 遅延
  CptNetHVSD(const FileRegion& file_region,
	     tVpiNetType net_type,
	     tVpiVsType vstype,
	     bool sign,
	     PtExpr* left,
	     PtExpr* right,
	     PtStrength* strength,
	     PtDelay* delay);

  /// @brief デストラクタ
  virtual
  ~CptNetHVSD();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief strength を返す．
  /// @return strength
  virtual
  const PtStrength*
  strength() const;

  /// @brief delay を返す．
  /// @return delay
  virtual
  const PtDelay*
  delay() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // strength
  PtStrength* mStrength;

  // delay
  PtDelay* mDelay;

};


//////////////////////////////////////////////////////////////////////
/// event 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptEventH :
  public CptDeclHead
{
  friend class CptFactory;
  
protected:
  
  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  CptEventH(const FileRegion& file_region);
  
  /// @brief デストラクタ
  virtual
  ~CptEventH();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスを識別するための型を返す．
  /// @return 宣言要素の型
  virtual
  tPtDeclType
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// specparam 宣言のヘッダのベース実装クラス
//////////////////////////////////////////////////////////////////////
class CptSpecParamH :
  public CptDeclHead
{
  friend class CptFactory;
  
protected:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  CptSpecParamH(const FileRegion& file_region);
  
  /// @brief デストラクタ
  virtual
  ~CptSpecParamH();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスを識別するための型を返す．
  /// @return 宣言要素の型
  virtual
  tPtDeclType
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// ビットベクタ型の specparam
//////////////////////////////////////////////////////////////////////
class CptSpecParamHV :
  public CptSpecParamH
{
  friend class CptFactory;
  
protected:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  CptSpecParamHV(const FileRegion& file_region,
		 PtExpr* left,
		 PtExpr* right);

  /// @brief デストラクタ
  virtual
  ~CptSpecParamHV();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲の MSB を返す．
  /// @return 範囲のMSB
  virtual
  const PtExpr*
  left_range() const;

  /// @brief 範囲の LSB を返す．
  /// @return 範囲のLSB
  virtual
  const PtExpr*
  right_range() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 範囲のMSB
  PtExpr* mLeftRange;

  // 範囲のLSB
  PtExpr* mRightRange;

};


//////////////////////////////////////////////////////////////////////
/// 宣言要素のベースクラス
//////////////////////////////////////////////////////////////////////
class CptDeclItemBase :
  public PtDeclItem
{
  friend class CptFactory;
  
protected:
  
  /// @brief コンストラクタ
  /// @param[in] name 名前
  CptDeclItemBase(const char* name);
  
  /// @brief デストラクタ
  virtual
  ~CptDeclItemBase();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtDeclItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を取り出す．
  /// @return 名前
  virtual
  const char*
  name() const;

  /// @brief dimension list のサイズを取り出す．
  /// @return ここでは常に 0 を返す．
  virtual
  ymuint32
  dimension_list_size() const;

  /// 範囲の取得
  /// @note ここでは常に NULL を返す．
  virtual
  const PtRange*
  range(ymuint32 pos) const;
  
  /// @brief 初期値を取り出す．
  /// @return ここでは常に NULL を返す．
  virtual
  const PtExpr*
  init_value() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  const char* mName;

};


//////////////////////////////////////////////////////////////////////
/// 宣言要素のクラス
//////////////////////////////////////////////////////////////////////
class CptDeclItem :
  public CptDeclItemBase
{
  friend class CptFactory;
  
protected:
  
  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] name 名前
  CptDeclItem(const FileRegion& file_region,
	      const char* name);
  
  /// @brief デストラクタ
  virtual
  ~CptDeclItem();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtDeclItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  /// @return ファイル位置
  virtual
  FileRegion
  file_region() const;
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // トークンの位置
  FileRegion mLoc;

};


//////////////////////////////////////////////////////////////////////
/// 範囲を持った宣言要素のクラス
//////////////////////////////////////////////////////////////////////
class CptDeclItemR :
  public CptDeclItemBase
{
  friend class CptFactory;
  
protected:
  
  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] range_array 範囲のリスト
  CptDeclItemR(const FileRegion& file_region,
	       const char* name,
	       PtRangeArray range_array);
  
  /// @brief デストラクタ
  virtual
  ~CptDeclItemR();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtDeclItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  /// @return ファイル位置
  virtual
  FileRegion
  file_region() const;

  /// @brief dimension list のサイズを取り出す．
  /// @return dimension list のサイズ
  virtual
  ymuint32
  dimension_list_size() const;

  /// 範囲の取得
  virtual
  const PtRange*
  range(ymuint32 pos) const;
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 範囲の配列
  PtRangeArray mRangeArray;

};


//////////////////////////////////////////////////////////////////////
/// 初期値をもった宣言要素のクラス
//////////////////////////////////////////////////////////////////////
class CptDeclItemI :
  public CptDeclItem
{
  friend class CptFactory;

protected:
  
  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] init_value 初期値
  CptDeclItemI(const FileRegion& file_region,
	       const char* name,
	       PtExpr* init_value);
  
  /// @brief デストラクタ
  virtual
  ~CptDeclItemI();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDeclItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  /// @return ファイル位置
  virtual
  FileRegion
  file_region() const;
  
  /// @brief 初期値を取り出す．
  /// @retval 初期値
  /// @retval NULL 設定がない場合
  virtual
  const PtExpr*
  init_value() const;
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 初期値
  PtExpr* mInitValue;

};


//////////////////////////////////////////////////////////////////////
/// @class CptRange CptDecl.h "CptDecl.h"
/// @brief 範囲を表すクラス
//////////////////////////////////////////////////////////////////////
class CptRange :
  public PtRange
{
public:

  /// @brief コンストラクタ
  CptRange(const FileRegion& fr,
	   PtExpr* msb,
	   PtExpr* lsb);

  /// @brief デストラクタ
  virtual
  ~CptRange();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtRange の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// ファイル位置の取得
  virtual
  FileRegion
  file_region() const;

  /// @brief 範囲の MSB を取り出す．
  virtual
  const PtExpr*
  left() const;

  /// @brief 範囲の LSB を取り出す．
  virtual
  const PtExpr*
  right() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // MSB
  PtExpr* mMsb;

  // LSB
  PtExpr* mLsb;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_PT_COMPACT_CPTDECL_H
