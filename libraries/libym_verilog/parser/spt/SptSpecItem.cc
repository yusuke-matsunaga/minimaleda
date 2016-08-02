
/// @file libym_verilog/pt_simple/SptSpecItem.cc
/// @brief SptSpecItem の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SptSpecItem.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "SptItem.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// specify_block の item を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SptSpecItem::SptSpecItem(const FileRegion& file_region,
			 tVpiSpecItemType id,
			 PtExprArray terminal_array) :
  SptItem(file_region, kPtItem_SpecItem),
  mId(id),
  mTerminals(terminal_array)
{
}

// デストラクタ
SptSpecItem::~SptSpecItem()
{
}

// specify block item の種類の取得
tVpiSpecItemType
SptSpecItem::specitem_type() const
{
  return mId;
}

// ターミナルリストの要素数を返す．
ymuint32
SptSpecItem::size() const
{
  return mTerminals.size();
}
    
// @brief ターミナルの取得
// @param[in] pos 位置番号 ( 0 <= pos < size() )
const PtExpr*
SptSpecItem::terminal(ymuint32 pos) const
{
  return mTerminals[pos];
}


//////////////////////////////////////////////////////////////////////
// specify_block の path_declaration を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SptSpecPath::SptSpecPath(const FileRegion& file_region,
			 tVpiSpecPathType id,
			 PtExpr* expr,
			 PtPathDecl* path_decl) :
  SptItem(file_region, kPtItem_SpecPath),
  mId(id),
  mExpr(expr),
  mPathDecl(path_decl)
{
}

// デストラクタ
SptSpecPath::~SptSpecPath()
{
}

// specify block path の種類の取得
tVpiSpecPathType
SptSpecPath::specpath_type() const
{
  return mId;
}

// モジュールパスの式を返す．
const PtExpr*
SptSpecPath::expr() const
{
  return mExpr;
}

// パス記述を返す．
const PtPathDecl*
SptSpecPath::path_decl() const
{
  return mPathDecl;
}


//////////////////////////////////////////////////////////////////////
// path_decl を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SptPathDecl::SptPathDecl(const FileRegion& file_region,
			 int edge,
			 PtExprArray input_array,
			 int input_pol,
			 int op,
			 PtExprArray output_array,
			 int output_pol,
			 PtExpr* expr,
			 PtPathDelay* path_delay) :
  mFileRegion(file_region),
  mEdge(edge),
  mInputs(input_array),
  mInputPol(input_pol),
  mOp(op),
  mOutputs(output_array),
  mOutputPol(output_pol),
  mExpr(expr),
  mPathDelay(path_delay)
{
}

// デストラクタ
SptPathDecl::~SptPathDecl()
{
}

// ファイル位置を返す．
FileRegion
SptPathDecl::file_region() const
{
  return mFileRegion;
}

// edge_descriptor を取り出す．
// 0の場合もある．
int
SptPathDecl::edge() const
{
  return mEdge;
}

// @brief 入力リストの要素数の取得
// @return 入力リストの要素数
ymuint32
SptPathDecl::input_num() const
{
  return mInputs.size();
}

// @brief 入力の取得
// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
const PtExpr*
SptPathDecl::input(ymuint32 pos) const
{
  return mInputs[pos];
}

// 入力の極性を取り出す．
// 0の場合もありうる．
int
SptPathDecl::input_pol() const
{
  return mInputPol;
}

// パス記述子(?)を得る．vpiParallel か vpiFull
int
SptPathDecl::op() const
{
  return mOp;
}

// @brief 出力リストの要素数の取得
// @return 出力リストの要素数
ymuint32
SptPathDecl::output_num() const
{
  return mOutputs.size();
}

// @brief 出力の取得
// @param[in] pos 位置番号 ( 0 <= pos < output_num() )
const PtExpr*
SptPathDecl::output(ymuint32 pos) const
{
  return mOutputs[pos];
}

// 出力の極性を取り出す．
// 0の場合もありうる．
int
SptPathDecl::output_pol() const
{
  return mOutputPol;
}

// 式を取り出す．
// NULL の場合もありうる．
const PtExpr*
SptPathDecl::expr() const
{
  return mExpr;
}

// path_delay_value を取り出す．
const PtPathDelay*
SptPathDecl::path_delay() const
{
  return mPathDelay;
}


//////////////////////////////////////////////////////////////////////
// path_delay_value を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SptPathDelay::SptPathDelay(const FileRegion& file_region,
			   PtExpr* value) :
  mFileRegion(file_region)
{
  mValues[0] = value;
  for (ymuint32 i = 1; i < 12; ++ i) {
    mValues[i] = NULL;
  }
}
SptPathDelay::SptPathDelay(const FileRegion& file_region,
			   PtExpr* value1,
			   PtExpr* value2) :
  mFileRegion(file_region)
{
  mValues[0] = value1;
  mValues[1] = value2;
  for (ymuint32 i = 2; i < 12; ++ i) {
    mValues[i] = NULL;
  }
}
SptPathDelay::SptPathDelay(const FileRegion& file_region,
			   PtExpr* value1,
			   PtExpr* value2,
			   PtExpr* value3) :
  mFileRegion(file_region)
{
  mValues[0] = value1;
  mValues[1] = value2;
  mValues[2] = value3;
  for (ymuint32 i = 3; i < 12; ++ i) {
    mValues[i] = NULL;
  }
}
SptPathDelay::SptPathDelay(const FileRegion& file_region,
			   PtExpr* value1,
			   PtExpr* value2,
			   PtExpr* value3,
			   PtExpr* value4,
			   PtExpr* value5,
			   PtExpr* value6) :
  mFileRegion(file_region)
{
  mValues[0] = value1;
  mValues[1] = value2;
  mValues[2] = value3;
  mValues[3] = value4;
  mValues[4] = value5;
  mValues[5] = value6;
  for (ymuint32 i = 6; i < 12; ++ i) {
    mValues[i] = NULL;
  }
}
SptPathDelay::SptPathDelay(const FileRegion& file_region,
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
			   PtExpr* value12) :
  mFileRegion(file_region)
{
  mValues[0] = value1;
  mValues[1] = value2;
  mValues[2] = value3;
  mValues[3] = value4;
  mValues[4] = value5;
  mValues[5] = value6;
  mValues[6] = value7;
  mValues[7] = value8;
  mValues[8] = value9;
  mValues[9] = value10;
  mValues[10] = value11;
  mValues[11] = value12;
}

// デストラクタ
SptPathDelay::~SptPathDelay()
{
}

// ファイル位置を返す．
FileRegion
SptPathDelay::file_region() const
{
  return mFileRegion;
}

// 値を取り出す．
// 0の場合もある．
const PtExpr*
SptPathDelay::value(ymuint32 pos) const
{
  return mValues[pos];
}

END_NAMESPACE_YM_VERILOG
