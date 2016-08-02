
/// @file libym_verilog/pt_compact/CptGenerate.cc
/// @brief CptGenerate の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CptGenerate.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "CptGenerate.h"
#include "CptFactory.h"


BEGIN_NAMESPACE_YM_VERILOG


//////////////////////////////////////////////////////////////////////
// generate 文で生成される要素を持つ本体
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptGenBody::CptGenBody(PtDeclHeadArray declhead_array,
		       PtItemArray item_array) :
  mDeclHeadArray(declhead_array),
  mItemArray(item_array)
{
}

// デストラクタ
CptGenBody::~CptGenBody()
{
}


//////////////////////////////////////////////////////////////////////
// generate ブロックの基底クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptGenBase::CptGenBase(const FileRegion& file_region,
		       PtDeclHeadArray declhead_array,
		       PtItemArray item_array) :
  mFileRegion(file_region),
  mBody(declhead_array, item_array)
{
}

// デストラクタ
CptGenBase::~CptGenBase()
{
}

// ファイル位置を返す．
FileRegion
CptGenBase::file_region() const
{
  return mFileRegion;
}
  
// @brief 宣言ヘッダ配列の取得
PtDeclHeadArray
CptGenBase::declhead_array() const
{
  return mBody.declhead_array();
}
  
// @brief item 配列の取得
PtItemArray
CptGenBase::item_array() const
{
  return mBody.item_array();
}


//////////////////////////////////////////////////////////////////////
// generate ブロックを表すノード
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptGenerate::CptGenerate(const FileRegion& file_region,
			 PtDeclHeadArray declhead_array,
			 PtItemArray item_array) :
  CptGenBase(file_region, declhead_array, item_array)
{
}

// デストラクタ
CptGenerate::~CptGenerate()
{
}

// 型を返す．
tPtItemType
CptGenerate::type() const
{
  return kPtItem_Generate;
}


//////////////////////////////////////////////////////////////////////
// generate ブロックの中のブロック
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptGenBlock::CptGenBlock(const FileRegion& file_region,
			 PtDeclHeadArray declhead_array,
			 PtItemArray item_array) :
  CptGenBase(file_region, declhead_array, item_array)
{
}

// デストラクタ
CptGenBlock::~CptGenBlock()
{
}

// 型を返す．
tPtItemType
CptGenBlock::type() const
{
  return kPtItem_GenBlock;
}


//////////////////////////////////////////////////////////////////////
// 名前つきの GenBlock
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptGenBlockN::CptGenBlockN(const FileRegion& file_region,
			   const char* name,
			   PtDeclHeadArray declhead_array,
			   PtItemArray item_array) :
  CptGenBlock(file_region, declhead_array, item_array),
  mName(name)
{
}

// デストラクタ
CptGenBlockN::~CptGenBlockN()
{
}

// 名前を返す．
const char*
CptGenBlockN::name() const
{
  return mName;
}


//////////////////////////////////////////////////////////////////////
// gen_if 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptGenIf::CptGenIf(const FileRegion& file_region,
		   PtExpr* cond,
		   PtDeclHeadArray then_declhead_array,
		   PtItemArray then_item_array,
		   PtDeclHeadArray else_declhead_array,
		   PtItemArray else_item_array) :
  mFileRegion(file_region),
  mCond(cond),
  mThenBody(then_declhead_array, then_item_array),
  mElseBody(else_declhead_array, else_item_array)
{
}

// デストラクタ
CptGenIf::~CptGenIf()
{
}

// ファイル位置を返す．
FileRegion
CptGenIf::file_region() const
{
  return mFileRegion;
}

// 型を返す．
tPtItemType
CptGenIf::type() const
{
  return kPtItem_GenIf;
}

// 条件式を返す．
const PtExpr*
CptGenIf::expr() const
{
  return mCond;
}
  
// @brief 条件が成り立ったときに生成される宣言ヘッダ配列の取得
PtDeclHeadArray
CptGenIf::then_declhead_array() const
{
  return mThenBody.declhead_array();
}
  
// @brief 条件が成り立ったときに生成される item 配列の取得
PtItemArray
CptGenIf::then_item_array() const
{
  return mThenBody.item_array();
}
  
// @brief 条件が成り立たなかったときに生成される宣言ヘッダ配列の取得
PtDeclHeadArray
CptGenIf::else_declhead_array() const
{
  return mElseBody.declhead_array();
}
  
// @brief 条件が成り立たなかったときに生成される item 配列の取得
PtItemArray
CptGenIf::else_item_array() const
{
  return mElseBody.item_array();
}


//////////////////////////////////////////////////////////////////////
// caseitem を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptGenCaseItem::CptGenCaseItem(const FileRegion& file_region,
			       PtExprArray label_array,
			       PtDeclHeadArray declhead_array,
			       PtItemArray item_array) :
  mFileRegion(file_region),
  mLabelArray(label_array),
  mBody(declhead_array, item_array)
{
}

// デストラクタ
CptGenCaseItem::~CptGenCaseItem()
{
}

// ファイル位置を返す．
FileRegion
CptGenCaseItem::file_region() const
{
  return mFileRegion;
}

// @brief ラベルの数の取得
// @return ラベル数\n
// 0 の時は default の意味
ymuint32
CptGenCaseItem::label_num() const
{
  return mLabelArray.size();
}

// @brief ラベルの取得
// @param[in] pos 位置番号 ( 0 <= pos < label_num() )
const PtExpr*
CptGenCaseItem::label(ymuint32 pos) const
{
  return mLabelArray[pos];
}
  
// @brief 宣言ヘッダ配列の取得
PtDeclHeadArray
CptGenCaseItem::declhead_array() const
{
  return mBody.declhead_array();
}
  
// @brief item 配列の取得
PtItemArray
CptGenCaseItem::item_array() const
{
  return mBody.item_array();
}


//////////////////////////////////////////////////////////////////////
// case 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptGenCase::CptGenCase(const FileRegion& file_region,
		       PtExpr* expr,
		       PtGenCaseItemArray item_array) :
  mFileRegion(file_region),
  mExpr(expr),
  mCaseItemArray(item_array)
{
}

// デストラクタ
CptGenCase::~CptGenCase()
{
}

// ファイル位置を返す．
FileRegion
CptGenCase::file_region() const
{
  return mFileRegion;
}

// 型を返す．
tPtItemType
CptGenCase::type() const
{
  return kPtItem_GenCase;
}

// 比較式を返す．
const PtExpr*
CptGenCase::expr() const
{
  return mExpr;
}

// case item の要素数を返す．
ymuint32
CptGenCase::size() const
{
  return mCaseItemArray.size();
}

// case item を返す．
const PtGenCaseItem*
CptGenCase::caseitem(ymuint32 pos) const
{
  return mCaseItemArray[pos];
}


//////////////////////////////////////////////////////////////////////
// gen_for 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptGenFor::CptGenFor(const FileRegion& file_region,
		     const char* loop_var,
		     PtExpr* init_expr,
		     PtExpr* cond,
		     PtExpr* next_expr,
		     const char* block_name,
		     PtDeclHeadArray declhead_array,
		     PtItemArray item_array) :
  mFileRegion(file_region),
  mName(block_name),
  mLoopVar(loop_var),
  mInitExpr(init_expr),
  mCond(cond),
  mNextExpr(next_expr),
  mBody(declhead_array, item_array)
{
}

// デストラクタ
CptGenFor::~CptGenFor()
{
}

// ファイル位置を返す．
FileRegion
CptGenFor::file_region() const
{
  return mFileRegion;
}

// 型を返す．
tPtItemType
CptGenFor::type() const
{
  return kPtItem_GenFor;
}

// 名前を返す．
const char*
CptGenFor::name() const
{
  return mName;
}
  
// @brief 宣言ヘッダ配列の取得
PtDeclHeadArray
CptGenFor::declhead_array() const
{
  return mBody.declhead_array();
}
  
// @brief item 配列の取得
PtItemArray
CptGenFor::item_array() const
{
  return mBody.item_array();
}

// 繰り返し制御用の変数名を返す．
const char*
CptGenFor::loop_var() const
{
  return mLoopVar;
}

// 初期化文の右辺を返す．
const PtExpr*
CptGenFor::init_expr() const
{
  return mInitExpr;
}

// 繰り返し条件を返す．
const PtExpr*
CptGenFor::expr() const
{
  return mCond;
}

// 増加文の右辺を返す．
const PtExpr*
CptGenFor::next_expr() const
{
  return mNextExpr;
}


//////////////////////////////////////////////////////////////////////
// generate 文関係
//////////////////////////////////////////////////////////////////////

// generate 文を生成する．
PtItem*
CptFactory::new_Generate(const FileRegion& file_region,
			 PtDeclHeadArray declhead_array,
			 PtItemArray item_array)
{
  ++ mNumGenerate;
  void* p = alloc().get_memory(sizeof(CptGenerate));
  return new (p) CptGenerate(file_region, declhead_array, item_array);
}

// generate block 文を生成する．
PtItem*
CptFactory::new_GenBlock(const FileRegion& file_region,
			 PtDeclHeadArray declhead_array,
			 PtItemArray item_array)
{
  ++ mNumGenBlock;
  void* p = alloc().get_memory(sizeof(CptGenBlock));
  return new (p) CptGenBlock(file_region, declhead_array, item_array);
}

// generate block 文を生成する．
PtItem*
CptFactory::new_GenBlock(const FileRegion& file_region,
			 const char* name,
			 PtDeclHeadArray declhead_array,
			 PtItemArray item_array)
{
  ++ mNumGenBlockN;
  void* p = alloc().get_memory(sizeof(CptGenBlockN));
  return new (p) CptGenBlockN(file_region, name, declhead_array, item_array);
}

// generate if 文を生成する．
PtItem*
CptFactory::new_GenIf(const FileRegion& file_region,
		      PtExpr* cond,
		      PtDeclHeadArray then_declhead_array,
		      PtItemArray then_item_array,
		      PtDeclHeadArray else_declhead_array,
		      PtItemArray else_item_array)
{
  ++ mNumGenIf;
  void* p = alloc().get_memory(sizeof(CptGenIf));
  return new (p) CptGenIf(file_region, cond,
			  then_declhead_array, then_item_array,
			  else_declhead_array, else_item_array);
}

// generate case 文を生成する．
PtItem*
CptFactory::new_GenCase(const FileRegion& file_region,
			PtExpr* expr,
			PtGenCaseItemArray item_array)
{
  ++ mNumGenCase;
  void* p = alloc().get_memory(sizeof(CptGenCase));
  return new (p) CptGenCase(file_region, expr, item_array);
}

// generate case の要素を生成する．
PtGenCaseItem*
CptFactory::new_GenCaseItem(const FileRegion& file_region,
			    PtExprArray label_array,
			    PtDeclHeadArray declhead_array,
			    PtItemArray item_array)
{
  ++ mNumGenCaseItem;
  void* p = alloc().get_memory(sizeof(CptGenCaseItem));
  return new (p) CptGenCaseItem(file_region, label_array,
				declhead_array, item_array);
}

// generate for 文を生成する．
PtItem*
CptFactory::new_GenFor(const FileRegion& file_region,
		       const char* loop_var,
		       PtExpr* init_expr,
		       PtExpr* cond,
		       PtExpr* next_expr,
		       const char* block_name,
		       PtDeclHeadArray declhead_array,
		       PtItemArray item_array)
{
  ++ mNumGenFor;
  void* p = alloc().get_memory(sizeof(CptGenFor));
  return new (p) CptGenFor(file_region, loop_var, init_expr, cond,
			   next_expr, block_name,
			   declhead_array, item_array);
}

END_NAMESPACE_YM_VERILOG
