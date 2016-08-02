#ifndef LIBYM_VERILOG_PT_COMPACT_CPTSPECITEM_H
#define LIBYM_VERILOG_PT_COMPACT_CPTSPECITEM_H

/// @file libym_verilog/pt_compact/CptSpecItem.h
/// @brief CptSpecItem のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CptSpecItem.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "CptItem.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @brief specify_block item を表すクラス
//////////////////////////////////////////////////////////////////////
class CptSpecItem :
  public CptItem
{
  friend class CptFactory;

protected:
  
  /// @brief コンストラクタ
  CptSpecItem(const FileRegion& file_region,
	      tVpiSpecItemType id,
	      PtExprArray terminal_array);
  
  /// @brief デストラクタ
  virtual
  ~CptSpecItem();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;

  /// @brief 型を返す．
  /// @note ここでは kPtItem_SpecItem を返す．
  virtual
  tPtItemType
  type() const;

  /// @brief specify block item の種類を返す．
  virtual
  tVpiSpecItemType
  specitem_type() const;

  /// @brief ターミナルリストの要素数を返す．
  virtual
  ymuint32
  size() const;
    
  /// @brief ターミナルの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < size() )
  virtual
  const PtExpr*
  terminal(ymuint32 pos) const;
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // トークン番号
  tVpiSpecItemType mId;

  // ターミナルの配列
  PtExprArray mTerminalArray;

};


//////////////////////////////////////////////////////////////////////
/// @brief path 仕様 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptSpecPath :
  public CptItem
{
  friend class CptFactory;

protected:
  
  /// @brief コンストラクタ
  CptSpecPath(const FileRegion& file_region,
	      tVpiSpecPathType id,
	      PtExpr* expr,
	      PtPathDecl* path_decl);
  
  /// @brief デストラクタ
  virtual
  ~CptSpecPath();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;

  /// @brief 型を返す．
  /// @note ここでは kPtItem_SpecPath を返す．
  virtual
  tPtItemType
  type() const;

  /// @brief specify block path の種類を返す．
  virtual
  tVpiSpecPathType
  specpath_type() const;

  /// @brief モジュールパスの式を返す．
  virtual
  const PtExpr*
  expr() const;

  /// @brief パス記述を返す．
  virtual
  const PtPathDecl*
  path_decl() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // トークン番号
  tVpiSpecPathType mId;

  // モジュールパスの式
  PtExpr* mExpr;

  // パス記述
  PtPathDecl* mPathDecl;

};


//////////////////////////////////////////////////////////////////////
/// @brief path_delay_declaration を表すクラス
//////////////////////////////////////////////////////////////////////
class CptPathDecl :
  public PtPathDecl
{
  friend class CptFactory;

protected:
  
  /// @brief コンストラクタ
  CptPathDecl(const FileRegion& file_region,
	      int edge,
	      PtExprArray input_array,
	      int input_pol,
	      int op,
	      PtExprArray output_array,
	      int output_pol,
	      PtExpr* expr,
	      PtPathDelay* path_delay);
  
  /// @brief デストラクタ
  virtual
  ~CptPathDecl();


public:
  //////////////////////////////////////////////////////////////////////
  // PtPathDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;

  /// @brief edge_descriptor を取り出す．
  /// @note 0の場合もある．
  virtual
  int
  edge() const;

  /// @brief 入力リストの要素数の取得
  /// @return 入力リストの要素数
  virtual
  ymuint32
  input_num() const;
  
  /// @brief 入力の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
  virtual
  const PtExpr*
  input(ymuint32 pos) const;
  
  /// @brief 入力の極性を取り出す．
  /// @note 0の場合もありうる．
  virtual
  int
  input_pol() const;
  
  /// @brief パス記述子(?)を得る．vpiParallel か vpiFull
  virtual
  int
  op() const;

  /// @brief 出力リストの要素数の取得
  /// @return 出力リストの要素数
  virtual
  ymuint32
  output_num() const;

  /// @brief 出力の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < output_num() )
  virtual
  const PtExpr*
  output(ymuint32 pos) const;
  
  /// @brief 出力の極性を取り出す．
  /// @note 0の場合もありうる．
  virtual
  int
  output_pol() const;
  
  /// @brief 式を取り出す．
  /// @note NULL の場合もありうる．
  virtual
  const PtExpr*
  expr() const;
  
  /// @brief path_delay_value を取り出す．
  virtual
  const PtPathDelay*
  path_delay() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  int mEdge;
  PtExprArray mInputArray;
  int mInputPol;
  int mOp;
  PtExprArray mOutputArray;
  int mOutputPol;
  PtExpr* mExpr;
  PtPathDelay* mPathDelay;

};


//////////////////////////////////////////////////////////////////////
/// @brief path_delay_value を表すクラス
//////////////////////////////////////////////////////////////////////
class CptPathDelay :
  public PtPathDelay
{
  friend class CptFactory;

protected:
  
  /// @brief コンストラクタ
  CptPathDelay(const FileRegion& file_region,
	       PtExpr* value1);

  /// @brief コンストラクタ
  CptPathDelay(const FileRegion& file_region,
	       PtExpr* value1,
	       PtExpr* value2);

  /// @brief コンストラクタ
  CptPathDelay(const FileRegion& file_region,
	       PtExpr* value1,
	       PtExpr* value2,
	       PtExpr* value3);

  /// @brief コンストラクタ
  CptPathDelay(const FileRegion& file_region,
	       PtExpr* value1,
	       PtExpr* value2,
	       PtExpr* value3,
	       PtExpr* value4,
	       PtExpr* value5,
	       PtExpr* value6);

  /// @brief コンストラクタ
  CptPathDelay(const FileRegion& file_region,
	       PtExpr* value1,
	       PtExpr* value2,
	       PtExpr* value3,
	       PtExpr* value4,
	       PtExpr* value5,
	       PtExpr* value6,
	       PtExpr* value7,
	       PtExpr* value8,
	       PtExpr* value9,
	       PtExpr* value10,
	       PtExpr* value11,
	       PtExpr* value12);
  
  /// @brief デストラクタ
  virtual
  ~CptPathDelay();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtPathDelay の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;

  /// @brief 値を取り出す．
  /// @note 0の場合もある．
  virtual
  const PtExpr*
  value(ymuint32 pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // ディレイ値
  PtExpr* mValues[12];

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_PT_COMPACT_CPTSPECITEM_H
