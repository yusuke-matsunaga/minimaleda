
/// @file libym_verilog/pt_compact/CptSpecItem.cc
/// @brief CptSpecItem の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CptSpecItem.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "CptSpecItem.h"
#include "CptFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// specify_block の item を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptSpecItem::CptSpecItem(const FileRegion& file_region,
			 tVpiSpecItemType id,
			 PtExprArray terminal_array) :
  mFileRegion(file_region),
  mId(id),
  mTerminalArray(terminal_array)
{
}

// デストラクタ
CptSpecItem::~CptSpecItem()
{
}

// ファイル位置を返す．
FileRegion
CptSpecItem::file_region() const
{
  return mFileRegion;
}

// 型を返す．
tPtItemType
CptSpecItem::type() const
{
  return kPtItem_SpecItem;
}

// トークン番号を返す．??? たぶん変更される．
tVpiSpecItemType
CptSpecItem::specitem_type() const
{
  return mId;
}

// ターミナルリストの要素数を返す．
ymuint32
CptSpecItem::size() const
{
  return mTerminalArray.size();
}
  
// @brief ターミナルの取得
// @param[in] pos 位置番号 ( 0 <= pos < size() )
const PtExpr*
CptSpecItem::terminal(ymuint32 pos) const
{
  return mTerminalArray[pos];
}


//////////////////////////////////////////////////////////////////////
// specify_block の path_declaration を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptSpecPath::CptSpecPath(const FileRegion& file_region,
			 tVpiSpecPathType id,
			 PtExpr* expr,
			 PtPathDecl* path_decl) :
  mFileRegion(file_region),
  mId(id),
  mExpr(expr),
  mPathDecl(path_decl)
{
}

// デストラクタ
CptSpecPath::~CptSpecPath()
{
}

// ファイル位置を返す．
FileRegion
CptSpecPath::file_region() const
{
  return mFileRegion;
}

// 型を返す．
tPtItemType
CptSpecPath::type() const
{
  return kPtItem_SpecPath;
}

// トークン番号を返す．??? たぶん変更される．
tVpiSpecPathType
CptSpecPath::specpath_type() const
{
  return mId;
}

// モジュールパスの式を返す．
const PtExpr*
CptSpecPath::expr() const
{
  return mExpr;
}

// パス記述を返す．
const PtPathDecl*
CptSpecPath::path_decl() const
{
  return mPathDecl;
}


//////////////////////////////////////////////////////////////////////
// path_decl を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptPathDecl::CptPathDecl(const FileRegion& file_region,
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
  mInputArray(input_array),
  mInputPol(input_pol),
  mOp(op),
  mOutputArray(output_array),
  mOutputPol(output_pol),
  mExpr(expr),
  mPathDelay(path_delay)
{
}

// デストラクタ
CptPathDecl::~CptPathDecl()
{
}

// ファイル位置を返す．
FileRegion
CptPathDecl::file_region() const
{
  return mFileRegion;
}

// edge_descriptor を取り出す．
// 0の場合もある．
int
CptPathDecl::edge() const
{
  return mEdge;
}

// @brief 入力リストの要素数の取得
// @return 入力リストの要素数
ymuint32
CptPathDecl::input_num() const
{
  return mInputArray.size();
}
  
// @brief 入力の取得
// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
const PtExpr*
CptPathDecl::input(ymuint32 pos) const
{
  return mInputArray[pos];
}

// 入力の極性を取り出す．
// 0の場合もありうる．
int
CptPathDecl::input_pol() const
{
  return mInputPol;
}

// パス記述子(?)を得る．vpiParallel か vpiFull
int
CptPathDecl::op() const
{
  return mOp;
}

// @brief 出力リストの要素数の取得
// @return 出力リストの要素数
ymuint32
CptPathDecl::output_num() const
{
  return mOutputArray.size();
}

// @brief 出力の取得
// @param[in] pos 位置番号 ( 0 <= pos < output_num() )
const PtExpr*
CptPathDecl::output(ymuint32 pos) const
{
  return mOutputArray[pos];
}

// 出力の極性を取り出す．
// 0の場合もありうる．
int
CptPathDecl::output_pol() const
{
  return mOutputPol;
}

// 式を取り出す．
// NULL の場合もありうる．
const PtExpr*
CptPathDecl::expr() const
{
  return mExpr;
}

// path_delay_value を取り出す．
const PtPathDelay*
CptPathDecl::path_delay() const
{
  return mPathDelay;
}


//////////////////////////////////////////////////////////////////////
// path_delay_value を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptPathDelay::CptPathDelay(const FileRegion& file_region,
			   PtExpr* value) :
  mFileRegion(file_region)
{
  mValues[0] = value;
  for (ymuint32 i = 1; i < 12; ++ i) {
    mValues[i] = NULL;
  }
}
CptPathDelay::CptPathDelay(const FileRegion& file_region,
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
CptPathDelay::CptPathDelay(const FileRegion& file_region,
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
CptPathDelay::CptPathDelay(const FileRegion& file_region,
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
CptPathDelay::CptPathDelay(const FileRegion& file_region,
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
CptPathDelay::~CptPathDelay()
{
}

// ファイル位置を返す．
FileRegion
CptPathDelay::file_region() const
{
  return mFileRegion;
}

// 値を取り出す．
// 0の場合もある．
const PtExpr*
CptPathDelay::value(ymuint32 pos) const
{
  return mValues[pos];
}


//////////////////////////////////////////////////////////////////////
// spec item 関係
//////////////////////////////////////////////////////////////////////

// specify block item を生成する．
PtItem*
CptFactory::new_SpecItem(const FileRegion& file_region,
			 tVpiSpecItemType id,
			 PtExprArray terminal_array)
{
  ++ mNumSpecItem;
  void* p = alloc().get_memory(sizeof(CptSpecItem));
  return new (p) CptSpecItem(file_region, id, terminal_array);
}

// path 仕様を生成する．
PtItem*
CptFactory::new_SpecPath(const FileRegion& file_region,
			 tVpiSpecPathType id,
			 PtExpr* expr,
			 PtPathDecl* path_decl)
{
  ++ mNumSpecPath;
  void* p = alloc().get_memory(sizeof(CptSpecPath));
  return new (p) CptSpecPath(file_region, id, expr, path_decl);
}

// path 記述を生成する．
PtPathDecl*
CptFactory::new_PathDecl(const FileRegion& file_region,
			 int edge,
			 PtExprArray input_array,
			 int input_pol,
			 int op,
			 PtExprArray output_array,
			 int output_pol,
			 PtExpr* expr,
			 PtPathDelay* path_delay)
{
  ++ mNumPathDecl;
  void* p = alloc().get_memory(sizeof(CptPathDecl));
  return new (p) CptPathDecl(file_region, edge, input_array, input_pol,
			     op, output_array, output_pol,
			     expr, path_delay);
}

// path delay value を生成する．
PtPathDelay*
CptFactory::new_PathDelay(const FileRegion& file_region,
			  PtExpr* value)
{
  ++ mNumPathDelay;
  void* p = alloc().get_memory(sizeof(CptPathDelay));
  return new (p) CptPathDelay(file_region, value);
}

// path delay value を生成する．
PtPathDelay*
CptFactory::new_PathDelay(const FileRegion& file_region,
			  PtExpr* value1,
			  PtExpr* value2)
{
  ++ mNumPathDelay;
  void* p = alloc().get_memory(sizeof(CptPathDelay));
  return new (p) CptPathDelay(file_region, value1, value2);
}

// path delay value を生成する．
PtPathDelay*
CptFactory::new_PathDelay(const FileRegion& file_region,
			  PtExpr* value1,
			  PtExpr* value2,
			  PtExpr* value3)
{
  ++ mNumPathDelay;
  void* p = alloc().get_memory(sizeof(CptPathDelay));
  return new (p) CptPathDelay(file_region, value1, value2, value3);
}

// path delay value を生成する．
PtPathDelay*
CptFactory::new_PathDelay(const FileRegion& file_region,
			  PtExpr* value1,
			  PtExpr* value2,
			  PtExpr* value3,
			  PtExpr* value4,
			  PtExpr* value5,
			  PtExpr* value6)
{
  ++ mNumPathDelay;
  void* p = alloc().get_memory(sizeof(CptPathDelay));
  return new (p) CptPathDelay(file_region, value1, value2, value3,
			      value4, value5, value6);
}

// path delay value を生成する．
PtPathDelay*
CptFactory::new_PathDelay(const FileRegion& file_region,
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
			  PtExpr* value12)
{
  ++ mNumPathDelay;
  void* p = alloc().get_memory(sizeof(CptPathDelay));
  return new (p) CptPathDelay(file_region, value1, value2, value3,
			      value4, value5, value6,
			      value7, value8, value9,
			      value10, value11, value12);
}

END_NAMESPACE_YM_VERILOG
