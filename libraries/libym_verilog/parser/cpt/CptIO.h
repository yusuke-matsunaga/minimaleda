#ifndef LIBYM_VERILOG_PT_COMPACT_CPTIO_H
#define LIBYM_VERILOG_PT_COMPACT_CPTIO_H

/// @file libym_verilog/pt_compact/CptIO.h
/// @brief CptIO のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CptIO.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/pt/PtDecl.h"
#include "ym_verilog/pt/PtArray.h"

#include "ym_utils/FileRegion.h"

#include "PtiDecl.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @brief IO宣言のヘッダのベース実装クラス
//////////////////////////////////////////////////////////////////////
class CptIOHBase :
  public PtiIOHead
{
protected:
  
  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] type IOの種類
  /// @param[in] aux_type 補助的な型
  /// @param[in] net_type 補助的なネット型
  /// @param[in] var_type 補助的な変数型
  /// @param[in] sign 符号つきの時 true にするフラグ
  CptIOHBase(const FileRegion& file_region,
	     tPtIOType type,
	     tVpiAuxType aux_type,
	     tVpiNetType net_type,
	     tVpiVarType var_type,
	     bool sign);

  /// @brief デストラクタ
  virtual
  ~CptIOHBase();


public:
  //////////////////////////////////////////////////////////////////////
  // PtIOH の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置の取得
  virtual
  FileRegion
  file_region() const;

  /// @brief 型の取得
  virtual
  tPtIOType
  type() const;

  /// @brief 補助的な型の取得
  virtual
  tVpiAuxType
  aux_type() const;

  /// @brief 補助的なネット型の取得
  virtual
  tVpiNetType
  net_type() const;

  /// @brief 補助的な変数型の取得
  virtual
  tVpiVarType
  var_type() const;

  /// @brief 符号の取得
  /// @retval true 符号付き
  /// @retval false 符号なし
  virtual
  bool
  is_signed() const;

  /// @brief 範囲のMSBの取得
  /// @retval 範囲のMSB
  /// @retval NULL 範囲を持たないとき
  /// @note このクラスでは NULL を返す．
  virtual
  const PtExpr*
  left_range() const;

  /// @brief 範囲のLSBの取得
  /// @retval 範囲のLSB
  /// @retval NULL 範囲を持たないとき
  /// @note このクラスでは NULL を返す．
  virtual
  const PtExpr*
  right_range() const;
  
  /// @brief 要素数の取得
  virtual
  ymuint32
  item_num() const;
  
  /// @brief 要素の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < item_num() )
  virtual
  const PtIOItem*
  item(ymuint32 pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // PtiIOHead の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素リストの設定
  /// @param[in] elem_array 要素リスト
  virtual
  void
  set_elem(PtIOItemArray elem_array);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // type と aux_type と符号を格納するメンバ
  ymuint32 mAttr;
  
  // 要素の配列
  PtIOItemArray mItemArray;
  
};


//////////////////////////////////////////////////////////////////////
/// @brief 範囲を持たない IO宣言のヘッダ
//////////////////////////////////////////////////////////////////////
class CptIOH :
  public CptIOHBase
{
  friend class CptFactory;

protected:
  
  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] type IOの種類
  /// @param[in] aux_type 補助的な型
  /// @param[in] net_type 補助的なネット型
  /// @param[in] var_type 補助的な変数型
  /// @param[in] sign 符号の有無
  CptIOH(const FileRegion& file_region,
	 tPtIOType type,
	 tVpiAuxType aux_type,
	 tVpiNetType net_type,
	 tVpiVarType var_type,
	 bool sign);

  /// @brief デストラクタ
  virtual
  ~CptIOH();

};


//////////////////////////////////////////////////////////////////////
/// @brief ビットベクタ型の IO宣言のヘッダ
//////////////////////////////////////////////////////////////////////
class CptIOHV :
  public CptIOHBase
{
  friend class CptFactory;

protected:
  
  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] type IOの種類
  /// @param[in] aux_type 補助的な型
  /// @param[in] net_type 補助的なネット型
  /// @param[in] sign 符号つきの時に true とするフラグ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  CptIOHV(const FileRegion& file_region,
	  tPtIOType type,
	  tVpiAuxType aux_type,
	  tVpiNetType net_type,
	  bool sign,
	  PtExpr* left,
	  PtExpr* right);

  /// @brief デストラクタ
  virtual
  ~CptIOHV();


public:
  //////////////////////////////////////////////////////////////////////
  // PtIOH の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲のMSBの取得
  virtual
  const PtExpr*
  left_range() const;

  /// @brief 範囲のLSBの取得
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
/// @brief IO宣言要素のベース実装クラス
//////////////////////////////////////////////////////////////////////
class CptIOItem :
  public PtIOItem
{
  friend class CptFactory;

protected:
  
  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] name 名前
  CptIOItem(const FileRegion& file_region,
	    const char* name);

  /// @brief デストラクタ
  virtual
  ~CptIOItem();


public:
  //////////////////////////////////////////////////////////////////////
  // PtIOItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置の取得
  virtual
  FileRegion
  file_region() const;

  /// @brief 名前の取得
  virtual
  const char*
  name() const;
  
  /// @brief 初期値の取得
  /// @retval 初期値
  /// @retval NULL 初期値を持たないとき
  /// @note このクラスでは NULL を返す．
  virtual
  const PtExpr*
  init_value() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // トークンの位置
  FileRegion mLoc;

  // 名前
  const char* mName;

};


//////////////////////////////////////////////////////////////////////
/// @brief 初期値をもった IO宣言要素の基底クラス
//////////////////////////////////////////////////////////////////////
class CptIOItemI :
  public CptIOItem
{
  friend class CptFactory;

protected:
  
  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] init_value 初期値
  CptIOItemI(const FileRegion& file_region,
	     const char* name,
	     PtExpr* init_value);

  /// @brief デストラクタ
  virtual
  ~CptIOItemI();

public:
  //////////////////////////////////////////////////////////////////////
  // PtIOItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置の取得
  virtual
  FileRegion
  file_region() const;
  
  /// @brief 初期値の取得
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


END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_PT_COMPACT_CPTIO_H
