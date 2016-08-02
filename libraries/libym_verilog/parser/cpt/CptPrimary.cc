
/// @file libym_verilog/pt_compact/CptPrimary.cc
/// @brief CptPrimary の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CptPrimary.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "CptExpr.h"
#include "CptFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// PtPrimary のベース実装クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptPrimaryBase::CptPrimaryBase(const char* name) :
  mName(name)
{
}

// デストラクタ
CptPrimaryBase::~CptPrimaryBase()
{
}

// クラスの型を返す．
tPtExprType
CptPrimaryBase::type() const
{
  return kPtPrimaryExpr;
}

// 末尾の名前を取り出す．
const char*
CptPrimaryBase::name() const
{
  return mName;
}


//////////////////////////////////////////////////////////////////////
// PtPrimary の実装クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptPrimary::CptPrimary(const FileRegion& file_region,
		       const char* name) :
  CptPrimaryBase(name),
  mFileRegion(file_region)
{
}

// デストラクタ
CptPrimary::~CptPrimary()
{
}

// ファイル位置を返す．
FileRegion
CptPrimary::file_region() const
{
  return mFileRegion;
}

// index_list も range も持たないとき true を返す．
bool
CptPrimary::is_simple() const
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// インデックスつきの終端記号を表すクラス
//////////////////////////////////////////////////////////////////////
  
// コンストラクタ
CptPrimaryI::CptPrimaryI(const FileRegion& file_region,
			 const char* name,
			 PtExprArray index_array) :
  CptPrimaryBase(name),
  mFileRegion(file_region),
  mIndexArray(index_array)
{
}

// デストラクタ
CptPrimaryI::~CptPrimaryI()
{
}

// ファイル位置を返す．
FileRegion
CptPrimaryI::file_region() const
{
  return mFileRegion;
}

// @brief インデックスリストのサイズの取得
// @return インデックスリストのサイズ
ymuint32
CptPrimaryI::index_num() const
{
  return mIndexArray.size();
}
  
// @brief インデックスの取得
// @param[in] pos 位置番号 ( 0 <= pos < index_num() )
const PtExpr*
CptPrimaryI::index(ymuint32 pos) const
{
  return mIndexArray[pos];
}

// index_list も range も持たないとき true を返す．
bool
CptPrimaryI::is_simple() const
{
  return false;
}


//////////////////////////////////////////////////////////////////////
// 定数インデックスつきの終端記号を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptPrimaryCI::CptPrimaryCI(const FileRegion& file_region,
			   const char* name,
			   PtExprArray index_array) :
  CptPrimaryI(file_region, name, index_array)
{
}

// デストラクタ
CptPrimaryCI::~CptPrimaryCI()
{
}
  
// インデックスもしくは範囲が定数にならなければならないとき true を返す．
bool
CptPrimaryCI::is_const_index() const
{
  // この関数こそがこのクラスの存在意義
  return true;
}


//////////////////////////////////////////////////////////////////////
// 範囲指定つきの終端記号を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptPrimaryR::CptPrimaryR(const FileRegion& file_region,
			 const char* name,
			 tVpiRangeMode mode,
			 PtExpr* left,
			 PtExpr* right) :
  CptPrimaryBase(name),
  mFileRegion(file_region),
  mMode(mode),
  mLeftRange(left),
  mRightRange(right)
{
}

// デストラクタ
CptPrimaryR::~CptPrimaryR()
{
}

// ファイル位置を返す．
FileRegion
CptPrimaryR::file_region() const
{
  return mFileRegion;
}

// 範囲指定モードの取得
tVpiRangeMode
CptPrimaryR::range_mode() const
{
  return mMode;
}

// range の MSB を取出す．
const PtExpr*
CptPrimaryR::left_range() const
{
  return mLeftRange;
}

// range の LSB を取出す．
const PtExpr*
CptPrimaryR::right_range() const
{
  return mRightRange;
}

// index_list も range も持たないとき true を返す．
bool
CptPrimaryR::is_simple() const
{
  return false;
}


//////////////////////////////////////////////////////////////////////
// 定数範囲指定つきの終端記号を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptPrimaryCR::CptPrimaryCR(const FileRegion& file_region,
			   const char* name,
			   tVpiRangeMode mode,
			   PtExpr* left,
			   PtExpr* right) :
  CptPrimaryR(file_region, name, mode, left, right)
{
}

// デストラクタ
CptPrimaryCR::~CptPrimaryCR()
{
}
  
// インデックスもしくは範囲が定数にならなければならないとき true を返す．
bool
CptPrimaryCR::is_const_index() const
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// インデックスと範囲指定つきの終端記号を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptPrimaryIR::CptPrimaryIR(const FileRegion& file_region,
			   const char* name,
			   PtExprArray index_array,
			   tVpiRangeMode mode,
			   PtExpr* left,
			   PtExpr* right) :
  CptPrimaryI(file_region, name, index_array),
  mMode(mode),
  mLeftRange(left),
  mRightRange(right)
{
}

// デストラクタ
CptPrimaryIR::~CptPrimaryIR()
{
}

// 範囲指定モード
tVpiRangeMode
CptPrimaryIR::range_mode() const
{
  return mMode;
}

// range の MSB を取出す．
const PtExpr*
CptPrimaryIR::left_range() const
{
  return mLeftRange;
}

// range の LSB を取出す．
const PtExpr*
CptPrimaryIR::right_range() const
{
  return mRightRange;
}


//////////////////////////////////////////////////////////////////////
// 階層名を持つ primary operator を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptPrimaryH::CptPrimaryH(const FileRegion& file_region,
			 PtNameBranchArray nb_array,
			 const char* tail_name) :
  CptPrimaryBase(tail_name),
  mFileRegion(file_region),
  mNbArray(nb_array)
{
}
  
// デストラクタ
CptPrimaryH::~CptPrimaryH()
{
}

// ファイル位置を返す．
FileRegion
CptPrimaryH::file_region() const
{
  return mFileRegion;
}
  
// @brief 階層ブランチの取得
PtNameBranchArray
CptPrimaryH::namebranch_array() const
{
  return mNbArray;
}


//////////////////////////////////////////////////////////////////////
// 階層名を持つインデックスつき primary を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptPrimaryHI::CptPrimaryHI(const FileRegion& file_region,
			   PtNameBranchArray nb_array,
			   const char* tail_name,
			   PtExprArray index_array) :
  CptPrimaryI(file_region, tail_name, index_array),
  mNbArray(nb_array)
{
}
  
// デストラクタ
CptPrimaryHI::~CptPrimaryHI()
{
}
  
// @brief 階層ブランチの取得
PtNameBranchArray
CptPrimaryHI::namebranch_array() const
{
  return mNbArray;
}


//////////////////////////////////////////////////////////////////////
// 階層名を持つ定数インデックスつき primary を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptPrimaryHCI::CptPrimaryHCI(const FileRegion& file_region,
			     PtNameBranchArray nb_array,
			     const char* tail_name,
			     PtExprArray index_array) :
  CptPrimaryHI(file_region, nb_array, tail_name, index_array)
{
}
  
// デストラクタ
CptPrimaryHCI::~CptPrimaryHCI()
{
}
  
// インデックスもしくは範囲が定数にならなければならないとき true を返す．
bool
CptPrimaryHCI::is_const_index() const
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// 階層名を持つ範囲指定つき primary を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptPrimaryHR::CptPrimaryHR(const FileRegion& file_region,
			   PtNameBranchArray nb_array,
			   const char* tail_name,
			   tVpiRangeMode mode,
			   PtExpr* left,
			   PtExpr* right) :
  CptPrimaryR(file_region, tail_name, mode, left, right),
  mNbArray(nb_array)
{
}
  
// デストラクタ
CptPrimaryHR::~CptPrimaryHR()
{
}
  
// @brief 階層ブランチの取得
PtNameBranchArray
CptPrimaryHR::namebranch_array() const
{
  return mNbArray;
}


//////////////////////////////////////////////////////////////////////
// 階層名を持つインデックスと範囲指定つき primary を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptPrimaryHIR::CptPrimaryHIR(const FileRegion& file_region,
			     PtNameBranchArray nb_array,
			     const char* tail_name,
			     PtExprArray index_array,
			     tVpiRangeMode mode,
			     PtExpr* left,
			     PtExpr* right) :
  CptPrimaryIR(file_region, tail_name, index_array, mode, left, right),
  mNbArray(nb_array)
{
}

// デストラクタ
CptPrimaryHIR::~CptPrimaryHIR()
{
}
  
// @brief 階層ブランチの取得
PtNameBranchArray
CptPrimaryHIR::namebranch_array() const
{
  return mNbArray;
}


//////////////////////////////////////////////////////////////////////
// primary 関係
//////////////////////////////////////////////////////////////////////

// primary を生成する．
PtExpr*
CptFactory::new_Primary(const FileRegion& file_region,
			const char* name)
{
  ++ mNumPrimary;
  void* p = alloc().get_memory(sizeof(CptPrimary));
  return new (p) CptPrimary(file_region, name);
}

// primary を生成する．
PtExpr*
CptFactory::new_Primary(const FileRegion& file_region,
			const char* name,
			PtExprArray index_array)
{
  ++ mNumPrimaryI;
  void* p = alloc().get_memory(sizeof(CptPrimaryI));
  return new (p) CptPrimaryI(file_region, name, index_array);
}

// primary を生成する．
PtExpr*
CptFactory::new_Primary(const FileRegion& file_region,
			const char* name,
			tVpiRangeMode mode,
			PtExpr* left,
			PtExpr* right)
{
  ++ mNumPrimaryR;
  void* p = alloc().get_memory(sizeof(CptPrimaryR));
  return new (p) CptPrimaryR(file_region, name, mode, left, right);
}

// primary を生成する．
PtExpr*
CptFactory::new_Primary(const FileRegion& file_region,
			const char* name,
			PtExprArray index_array,
			tVpiRangeMode mode,
			PtExpr* left,
			PtExpr* right)
{
  ++ mNumPrimaryIR;
  void* p = alloc().get_memory(sizeof(CptPrimaryIR));
  return new (p) CptPrimaryIR(file_region, name, index_array, mode, left, right);
}

// primary を生成する．
PtExpr*
CptFactory::new_Primary(const FileRegion& file_region,
			PtNameBranchArray nb_array,
			const char* tail_name)
{
  ++ mNumPrimaryH;
  void* p = alloc().get_memory(sizeof(CptPrimaryH));
  return new (p) CptPrimaryH(file_region, nb_array, tail_name);
}

// primary を生成する．
PtExpr*
CptFactory::new_Primary(const FileRegion& file_region,
			PtNameBranchArray nb_array,
			const char* tail_name,
			PtExprArray index_array)
{
  ++ mNumPrimaryHI;
  void* p = alloc().get_memory(sizeof(CptPrimaryHI));
  return new (p) CptPrimaryHI(file_region, nb_array, tail_name, index_array);
}

// primary を生成する．
PtExpr*
CptFactory::new_Primary(const FileRegion& file_region,
			PtNameBranchArray nb_array,
			const char* tail_name,
			tVpiRangeMode mode,
			PtExpr* left,
			PtExpr* right)
{
  ++ mNumPrimaryHR;
  void* p = alloc().get_memory(sizeof(CptPrimaryHR));
  return new (p) CptPrimaryHR(file_region, nb_array, tail_name, mode, left, right);
}

// primary を生成する．
PtExpr*
CptFactory::new_Primary(const FileRegion& file_region,
			PtNameBranchArray nb_array,
			const char* tail_name,
			PtExprArray index_array,
			tVpiRangeMode mode,
			PtExpr* left,
			PtExpr* right)
{
  ++ mNumPrimaryHIR;
  void* p = alloc().get_memory(sizeof(CptPrimaryHIR));
  return new (p) CptPrimaryHIR(file_region, nb_array, tail_name,
			       index_array, mode, left, right);
}

// primary を生成する．
PtExpr*
CptFactory::new_CPrimary(const FileRegion& file_region,
			 const char* name,
			 PtExprArray index_array)
{
  ++ mNumPrimaryCI;
  void* p = alloc().get_memory(sizeof(CptPrimaryCI));
  return new (p) CptPrimaryCI(file_region, name, index_array);
}

// primary を生成する．
PtExpr*
CptFactory::new_CPrimary(const FileRegion& file_region,
			 const char* name,
			 tVpiRangeMode mode,
			 PtExpr* left,
			 PtExpr* right)
{
  ++ mNumPrimaryCR;
  void* p = alloc().get_memory(sizeof(CptPrimaryCR));
  return new (p) CptPrimaryCR(file_region, name, mode, left, right);
}

// primary を生成する．
PtExpr*
CptFactory::new_CPrimary(const FileRegion& file_region,
			 PtNameBranchArray nb_array,
			 const char* tail_name,
			 PtExprArray index_array)
{
  ++ mNumPrimaryHCI;
  void* p = alloc().get_memory(sizeof(CptPrimaryHCI));
  return new (p) CptPrimaryHCI(file_region, nb_array, tail_name, index_array);
}

END_NAMESPACE_YM_VERILOG
