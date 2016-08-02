#ifndef LIBYM_VERILOG_PT_COMPACT_CPTGENERATE_H
#define LIBYM_VERILOG_PT_COMPACT_CPTGENERATE_H

/// @file libym_verilog/pt_compact/CptGenerate.h
/// @brief CptGenerate のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CptGenerate.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "CptItem.h"



BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @brief generate 文で生成される要素を持つ本体
//////////////////////////////////////////////////////////////////////
class CptGenBody
{
public:
  
  /// @brief コンストラクタ
  CptGenBody(PtDeclHeadArray declhead_array,
	     PtItemArray item_array);
  
  /// @brief デストラクタ
  ~CptGenBody();


public:
  
  /// @brief 宣言ヘッダ配列の取得
  PtDeclHeadArray
  declhead_array() const { return mDeclHeadArray; }
  
  /// @brief item 配列の取得
  PtItemArray
  item_array() const { return mItemArray; }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 宣言の配列
  PtDeclHeadArray mDeclHeadArray;
  
  // 要素の配列
  PtItemArray mItemArray;

};


//////////////////////////////////////////////////////////////////////
/// @brief generate ブロックの基底クラス
//////////////////////////////////////////////////////////////////////
class CptGenBase :
  public CptItem
{
  friend class CptFactory;

protected:
  
  /// @brief コンストラクタ
  CptGenBase(const FileRegion& file_region,
	     PtDeclHeadArray declhead_array,
	     PtItemArray item_array);
  
  /// @brief デストラクタ
  virtual
  ~CptGenBase();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;
  
  /// @brief 宣言ヘッダ配列の取得
  virtual
  PtDeclHeadArray
  declhead_array() const;
  
  /// @brief item 配列の取得
  virtual
  PtItemArray
  item_array() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 本体
  CptGenBody mBody;

};


//////////////////////////////////////////////////////////////////////
/// @brief generate ブロックを表すノード
//////////////////////////////////////////////////////////////////////
class CptGenerate :
  public CptGenBase
{
  friend class CptFactory;

protected:
  
  /// @brief コンストラクタ
  CptGenerate(const FileRegion& file_region,
	      PtDeclHeadArray declhead_array,
	      PtItemArray item_array);
  
  /// @brief デストラクタ
  virtual
  ~CptGenerate();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  /// @note ここでは kPtItem_Generate を返す．
  virtual
  tPtItemType
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @brief generate ブロックの中のブロック
//////////////////////////////////////////////////////////////////////
class CptGenBlock :
  public CptGenBase
{
  friend class CptFactory;

protected:
  
  /// @brief コンストラクタ
  CptGenBlock(const FileRegion& file_region,
	      PtDeclHeadArray declhead_array,
	      PtItemArray item_array);
  
  /// @brief デストラクタ
  virtual
  ~CptGenBlock();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  /// @note ここでは kPtItem_GenBlock を返す．
  virtual
  tPtItemType
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @brief 名前つきの GenBlock
//////////////////////////////////////////////////////////////////////
class CptGenBlockN :
  public CptGenBlock
{
  friend class CptFactory;

protected:
  
  /// @brief コンストラクタ
  CptGenBlockN(const FileRegion& file_region,
	       const char* name,
	       PtDeclHeadArray declhead_array,
	       PtItemArray item_array);
  
  /// @brief デストラクタ
  virtual
  ~CptGenBlockN();


public:
  //////////////////////////////////////////////////////////////////////
  // PtGenerate の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  virtual
  const char*
  name() const;
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  const char* mName;

};


//////////////////////////////////////////////////////////////////////
/// @brief gen_if 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptGenIf :
  public CptItem
{
  friend class CptFactory;

protected:
  
  /// @brief コンストラクタ
  CptGenIf(const FileRegion& file_region,
	   PtExpr* cond,
	   PtDeclHeadArray then_declhead_array,
	   PtItemArray then_item_array,
	   PtDeclHeadArray else_declhead_array,
	   PtItemArray else_item_array);
  
  /// @brief デストラクタ
  virtual
  ~CptGenIf();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;

  /// @brief 型を返す．
  /// @note ここでは kPtItem_GenIf を返す．
  virtual
  tPtItemType
  type() const;

  /// @brief 条件式を返す．
  virtual
  const PtExpr*
  expr() const;
  
  /// @brief 条件が成り立ったときに生成される宣言ヘッダ配列の取得
  virtual
  PtDeclHeadArray
  then_declhead_array() const;
  
  /// @brief 条件が成り立ったときに生成される item 配列の取得
  virtual
  PtItemArray
  then_item_array() const;
  
  /// @brief 条件が成り立たなかったときに生成される宣言ヘッダ配列の取得
  virtual
  PtDeclHeadArray
  else_declhead_array() const;
  
  /// @brief 条件が成り立たなかったときに生成される item 配列の取得
  virtual
  PtItemArray
  else_item_array() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 条件式
  PtExpr* mCond;

  // 成り立ったとき生成される本体
  CptGenBody mThenBody;

  // 成り立たなかったとき生成される本体
  CptGenBody mElseBody;

};


//////////////////////////////////////////////////////////////////////
/// @brief gen_case 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptGenCase :
  public CptItem
{
  friend class CptFactory;

protected:
  
  /// @brief コンストラクタ
  CptGenCase(const FileRegion& file_region,
	     PtExpr* expr,
	     PtGenCaseItemArray item_array);
  
  /// @brief デストラクタ
  virtual
  ~CptGenCase();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;

  /// @brief 型を返す．
  /// @note ここでは kPtItem_GenCase を返す．
  virtual
  tPtItemType
  type() const;


public:
  //////////////////////////////////////////////////////////////////////
  // PtGenCase の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 比較式を返す．
  virtual
  const PtExpr*
  expr() const;

  /// @brief case item の要素数を返す．
  virtual
  ymuint32
  size() const;

  /// @brief case item を返す．
  virtual
  const PtGenCaseItem*
  caseitem(ymuint32 pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 比較式
  PtExpr* mExpr;
  
  // case item の配列
  PtGenCaseItemArray mCaseItemArray;

};


//////////////////////////////////////////////////////////////////////
/// @brief gencaseitemを表すクラス
//////////////////////////////////////////////////////////////////////
class CptGenCaseItem :
  public PtGenCaseItem
{
  friend class CptFactory;

protected:
  
  /// @brief コンストラクタ
  CptGenCaseItem(const FileRegion& file_region,
		 PtExprArray label_array,
		 PtDeclHeadArray declhead_array,
		 PtItemArray item_array);
  
  /// @brief デストラクタ
  virtual
  ~CptGenCaseItem();


public:
  //////////////////////////////////////////////////////////////////////
  // PtGenCaseItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;

  /// @brief ラベルの数の取得
  /// @return ラベル数\n
  /// 0 の時は default の意味
  virtual
  ymuint32
  label_num() const;

  /// @brief ラベルの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < label_num() )
  virtual
  const PtExpr*
  label(ymuint32 pos) const;
  
  /// @brief 宣言ヘッダ配列の取得
  virtual
  PtDeclHeadArray
  declhead_array() const;
  
  /// @brief item 配列の取得
  virtual
  PtItemArray
  item_array() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // ラベルの配列
  PtExprArray mLabelArray;

  // 生成される本体
  CptGenBody mBody;
  
};


//////////////////////////////////////////////////////////////////////
/// @brief gen_for 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptGenFor :
  public CptItem
{
  friend class CptFactory;

protected:
  
  /// @brief コンストラクタ
  CptGenFor(const FileRegion& file_region,
	    const char* loop_var,
	    PtExpr* init_expr,
	    PtExpr* cond,
	    PtExpr* next_expr,
	    const char* block_name,
	    PtDeclHeadArray declhead_array,
	    PtItemArray item_array);
  
  /// @brief デストラクタ
  virtual
  ~CptGenFor();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;

  /// @brief 型を返す．
  /// @note ここでは kPtItem_GenFor を返す．
  virtual
  tPtItemType
  type() const;

  /// @brief 名前を返す．
  virtual
  const char*
  name() const;
  
  /// @brief 宣言ヘッダ配列の取得
  virtual
  PtDeclHeadArray
  declhead_array() const;
  
  /// @brief item 配列の取得
  virtual
  PtItemArray
  item_array() const;

  /// @brief 繰り返し制御用の変数名を返す．
  virtual
  const char*
  loop_var() const;

  /// @brief 初期化文の右辺を返す．
  virtual
  const PtExpr*
  init_expr() const;

  /// @brief 繰り返し条件を返す．
  virtual
  const PtExpr*
  expr() const;

  /// @brief 増加文の右辺を返す．
  virtual
  const PtExpr*
  next_expr() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 名前
  const char* mName;

  // 繰り返し制御用の変数名
  const char* mLoopVar;

  // 初期化文の右辺
  PtExpr* mInitExpr;

  // 繰り返し条件
  PtExpr* mCond;

  // 増加文の右辺
  PtExpr* mNextExpr;

  // 生成される本体
  CptGenBody mBody;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_PT_COMPACT_CPTITEM_H
