
/// @file libym_verilog/pt_simple/SptGenerate.cc
/// @brief SptGenerate の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SptGenerate.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "SptItem.h"


BEGIN_NAMESPACE_YM_VERILOG


//////////////////////////////////////////////////////////////////////
// generate 文で生成される要素を持つ本体
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SptGenBody::SptGenBody(PtDeclHeadArray declhead_array,
		       PtItemArray item_array) :
  mDeclArray(declhead_array),
  mItemArray(item_array)
{
}

// デストラクタ
SptGenBody::~SptGenBody()
{
}


//////////////////////////////////////////////////////////////////////
// generate ブロックの基底クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SptGenerate::SptGenerate(const FileRegion& file_region,
			 tPtItemType type,
			 const char* name,
			 PtDeclHeadArray declhead_array,
			 PtItemArray item_array) :
  SptItem(file_region, type),
  mName(name),
  mBody(declhead_array, item_array)
{
}

// デストラクタ
SptGenerate::~SptGenerate()
{
}

// 名前を返す．
const char*
SptGenerate::name() const
{
  return mName;
}
  
// @brief 宣言のリストの取得
PtDeclHeadArray
SptGenerate::declhead_array() const
{
  return mBody.declhead_array();
}

// @brief item リストの取得
PtItemArray
SptGenerate::item_array() const
{
  return mBody.item_array();
}


//////////////////////////////////////////////////////////////////////
// gen_if 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SptGenIf::SptGenIf(const FileRegion& file_region,
		   PtExpr* cond,
		   PtDeclHeadArray then_declhead_array,
		   PtItemArray then_item_array,
		   PtDeclHeadArray else_declhead_array,
		   PtItemArray else_item_array) :
  SptItem(file_region, kPtItem_GenIf),
  mCond(cond),
  mThenBody(then_declhead_array, then_item_array),
  mElseBody(else_declhead_array, else_item_array)
{
}

// デストラクタ
SptGenIf::~SptGenIf()
{
}

// 条件式を返す．
const PtExpr*
SptGenIf::expr() const
{
  return mCond;
}
  
// @brief 条件が成り立ったときに生成される宣言ヘッダ配列の取得
PtDeclHeadArray
SptGenIf::then_declhead_array() const
{
  return mThenBody.declhead_array();
}
  
// @brief 条件が成り立ったときに生成される item 配列の取得
PtItemArray
SptGenIf::then_item_array() const
{
  return mThenBody.item_array();
}
  
// @brief 条件が成り立たなかったときに生成される宣言ヘッダ配列の取得
PtDeclHeadArray
SptGenIf::else_declhead_array() const
{
  return mElseBody.declhead_array();
}
  
// @brief 条件が成り立たなかったときに生成される item 配列の取得
PtItemArray
SptGenIf::else_item_array() const
{
  return mElseBody.item_array();
}


//////////////////////////////////////////////////////////////////////
// caseitem を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SptGenCaseItem::SptGenCaseItem(const FileRegion& file_region,
			       PtExprArray label_array,
			       PtDeclHeadArray declhead_array,
			       PtItemArray item_array) :
  mFileRegion(file_region),
  mLabelArray(label_array),
  mBody(declhead_array, item_array)
{
}

// デストラクタ
SptGenCaseItem::~SptGenCaseItem()
{
}

// ファイル位置を返す．
FileRegion
SptGenCaseItem::file_region() const
{
  return mFileRegion;
}

// @brief ラベルの数の取得
// @return ラベル数\n
// 0 の時は default の意味
ymuint32
SptGenCaseItem::label_num() const
{
  return mLabelArray.size();
}

// @brief ラベルの取得
// @param[in] pos 位置番号 ( 0 <= pos < label_num() )
const PtExpr*
SptGenCaseItem::label(ymuint32 pos) const
{
  return mLabelArray[pos];
}
  
// @brief 宣言のリストの取得
PtDeclHeadArray
SptGenCaseItem::declhead_array() const
{
  return mBody.declhead_array();
}

// @brief item リストの取得
PtItemArray
SptGenCaseItem::item_array() const
{
  return mBody.item_array();
}


//////////////////////////////////////////////////////////////////////
// case 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SptGenCase::SptGenCase(const FileRegion& file_region,
		       PtExpr* expr,
		       PtGenCaseItemArray item_array) :
  SptItem(file_region, kPtItem_GenCase),
  mExpr(expr),
  mCaseItemArray(item_array)
{
}

// デストラクタ
SptGenCase::~SptGenCase()
{
}

// 比較式を返す．
const PtExpr*
SptGenCase::expr() const
{
  return mExpr;
}

// case item の要素数を返す．
ymuint32
SptGenCase::size() const
{
  return mCaseItemArray.size();
}

// case item を返す．
const PtGenCaseItem*
SptGenCase::caseitem(ymuint32 pos) const
{
  return mCaseItemArray[pos];
}


//////////////////////////////////////////////////////////////////////
// gen_for 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SptGenFor::SptGenFor(const FileRegion& file_region,
		     const char* loop_var,
		     PtExpr* init_expr,
		     PtExpr* cond,
		     PtExpr* next_expr,
		     const char* block_name,
		     PtDeclHeadArray declhead_array,
		     PtItemArray item_array) :
  SptItem(file_region, kPtItem_GenFor),
  mName(block_name),
  mLoopVar(loop_var),
  mInitExpr(init_expr),
  mCond(cond),
  mNextExpr(next_expr),
  mBody(declhead_array, item_array)
{
}

// デストラクタ
SptGenFor::~SptGenFor()
{
}

// 名前を返す．
const char*
SptGenFor::name() const
{
  return mName;
}
  
// @brief 宣言のリストの取得
PtDeclHeadArray
SptGenFor::declhead_array() const
{
  return mBody.declhead_array();
}

// @brief item リストの取得
PtItemArray
SptGenFor::item_array() const
{
  return mBody.item_array();
}

// 繰り返し制御用の変数名を返す．
const char*
SptGenFor::loop_var() const
{
  return mLoopVar;
}

// 初期化文の右辺を返す．
const PtExpr*
SptGenFor::init_expr() const
{
  return mInitExpr;
}

// 繰り返し条件を返す．
const PtExpr*
SptGenFor::expr() const
{
  return mCond;
}

// 増加文の右辺を返す．
const PtExpr*
SptGenFor::next_expr() const
{
  return mNextExpr;
}

END_NAMESPACE_YM_VERILOG
