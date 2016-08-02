
/// @file libym_verilog/pt_compact/CptMisc.cc
/// @brief CptMisc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CptMisc.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "CptMisc.h"
#include "CptFactory.h"

#include "ym_verilog/pt/PtExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// delay control/event control/repeat control を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptControl::CptControl()
{
}

// デストラクタ
CptControl::~CptControl()
{
}

// @brief  遅延式の取得
// @retval 遅延を表す式 delay control の場合
// @retval NULL 上記以外
// デフォルトでは NULL を返す．
const PtExpr*
CptControl::delay() const
{
  return NULL;
}

// @brief イベントリストのサイズの取得
// @retval イベントリストのサイズ event control/repeat control の場合
// @retval 0 上記以外
ymuint32
CptControl::event_num() const
{
  return 0;
}
  
// @brief イベントリストの要素の取得
// @param[in] pos 位置番号 ( 0 <= pos < event_num() )
// @note event control/repeat control の場合のみ意味を持つ
const PtExpr*
CptControl::event(ymuint32 pos) const
{
  return NULL;
}

// @brief 繰り返し数の取得
// @retval 繰り返し数を表す式 repeat control の場合
// @retval NULL 上記以外
// デフォルトでは NULL を返す．
const PtExpr*
CptControl::rep_expr() const
{
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// delay control を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptDelayControl::CptDelayControl(const FileRegion& file_region,
				 PtExpr* delay) :
  mTopLoc(file_region.start_loc()),
  mDelay(delay)
{
  assert_cond(delay, __FILE__, __LINE__);
}

// デストラクタ
CptDelayControl::~CptDelayControl()
{
}

// ファイル位置を返す．
FileRegion
CptDelayControl::file_region() const
{
  return FileRegion(mTopLoc, mDelay->file_region().end_loc());
}

// 型を返す．
tPtCtrlType
CptDelayControl::type() const
{
  return kPtDelayControl;
}

// 遅延式を返す．
const PtExpr*
CptDelayControl::delay() const
{
  return mDelay;
}


//////////////////////////////////////////////////////////////////////
// event を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptEventControl::CptEventControl(const FileRegion& file_region,
				 PtExprArray event_array) :
  mFileRegion(file_region),
  mEventArray(event_array)
{
}

// デストラクタ
CptEventControl::~CptEventControl()
{
}

// ファイル位置を返す．
FileRegion
CptEventControl::file_region() const
{
  return mFileRegion;
}

// 型を返す．
tPtCtrlType
CptEventControl::type() const
{
  return kPtEventControl;
}

// @brief イベントリストのサイズの取得
// @retval イベントリストのサイズ event control/repeat control の場合
// @retval 0 上記以外
ymuint32
CptEventControl::event_num() const
{
  return mEventArray.size();
}

// @brief イベントリストの要素の取得
// @param[in] pos 位置番号 ( 0 <= pos < event_num() )
// @note event control/repeat control の場合のみ意味を持つ
const PtExpr*
CptEventControl::event(ymuint32 pos) const
{
  return mEventArray[pos];
}


//////////////////////////////////////////////////////////////////////
// repeat 形式の event を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptRepeatControl::CptRepeatControl(const FileRegion& file_region,
				   PtExpr* expr,
				   PtExprArray event_array) :
  mFileRegion(file_region),
  mRepExpr(expr),
  mEventArray(event_array)
{
  assert_cond(expr, __FILE__, __LINE__);
}

// デストラクタ
CptRepeatControl::~CptRepeatControl()
{
}

// ファイル位置を返す．
FileRegion
CptRepeatControl::file_region() const
{
  return mFileRegion;
}

// 型を返す．
tPtCtrlType
CptRepeatControl::type() const
{
  return kPtRepeatControl;
}

// 繰り返し数を得る．
const PtExpr*
CptRepeatControl::rep_expr() const
{
  return mRepExpr;
}

// @brief イベントリストのサイズの取得
// @retval イベントリストのサイズ event control/repeat control の場合
// @retval 0 上記以外
ymuint32
CptRepeatControl::event_num() const
{
  return mEventArray.size();
}

// @brief イベントリストの要素の取得
// @param[in] pos 位置番号 ( 0 <= pos < event_num() )
// @note event control/repeat control の場合のみ意味を持つ
const PtExpr*
CptRepeatControl::event(ymuint32 pos) const
{
  return mEventArray[pos];
}


//////////////////////////////////////////////////////////////////////
// ordered_connection/named_connection を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptConnection::CptConnection(const FileRegion& file_region,
			     PtExpr* expr) :
  mFileRegion(file_region),
  mExpr(expr)
{
}

// デストラクタ
CptConnection::~CptConnection()
{
}
  
// ファイル位置を取出す．
FileRegion
CptConnection::file_region() const
{
  return mFileRegion;
}

// @brief 名前の取得
// @retval 名前 named connection の場合
// @retval "" ordered connection の場合
// デフォルトでは NULL を返す．
const char*
CptConnection::name() const
{
  return NULL;
}

// 式を取出す
const PtExpr*
CptConnection::expr() const
{
  return mExpr;
}


//////////////////////////////////////////////////////////////////////
// ordered_connection を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptOrderedCon::CptOrderedCon(const FileRegion& file_region,
			     PtExpr* expr) :
  CptConnection(file_region, expr)
{
}

// デストラクタ
CptOrderedCon::~CptOrderedCon()
{
}


//////////////////////////////////////////////////////////////////////
// named_connection を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptNamedCon::CptNamedCon(const FileRegion& file_region,
			 const char* name,
			 PtExpr* expr) :
  CptConnection(file_region, expr),
  mName(name)
{
}

// デストラクタ
CptNamedCon::~CptNamedCon()
{
}

// 名前を取り出す．
const char*
CptNamedCon::name() const
{
  return mName;
}


//////////////////////////////////////////////////////////////////////
// strength を表すクラス
//////////////////////////////////////////////////////////////////////

// drive strength 用のコンストラクタ
CptStrength::CptStrength(const FileRegion& file_region,
			 tVpiStrength value1,
			 tVpiStrength value2) :
  mFileRegion(file_region)
{
  mValue1 = value1;
  mValue2 = value2;
  mValue3 = kVpiNoStrength;
}

// charge strength 用のコンストラクタ
CptStrength::CptStrength(const FileRegion& file_region,
			 tVpiStrength value1) :
  mFileRegion(file_region)
{
  // charge strength
  mValue1 = kVpiNoStrength;
  mValue2 = kVpiNoStrength;
  mValue3 = value1;
}

// デストラクタ
CptStrength::~CptStrength()
{
}

// ファイル位置を取出す．
FileRegion
CptStrength::file_region() const
{
  return mFileRegion;
}

// drive strength0 を返す．
tVpiStrength
CptStrength::drive0() const
{
  return mValue1;
}

// drive strength1 を返す．
tVpiStrength
CptStrength::drive1() const
{
  return mValue2;
}

// charge strength を返す．
tVpiStrength
CptStrength::charge() const
{
  return mValue3;
}


//////////////////////////////////////////////////////////////////////
// delay を表すクラス
//////////////////////////////////////////////////////////////////////

// 一つの値をとるコンストラクタ
CptDelay::CptDelay(const FileRegion& file_region,
		   PtExpr* value1) :
  mFileRegion(file_region)
{
  mValue[0] = value1;
  mValue[1] = NULL;
  mValue[2] = NULL;
}
  
// 二つの値をとるコンストラクタ
CptDelay::CptDelay(const FileRegion& file_region,
		   PtExpr* value1,
		   PtExpr* value2) :
  mFileRegion(file_region)
{
  mValue[0] = value1;
  mValue[1] = value2;
  mValue[2] = NULL;
}
  
// 三つの値をとるコンストラクタ
CptDelay::CptDelay(const FileRegion& file_region,
		   PtExpr* value1,
		   PtExpr* value2,
		   PtExpr* value3) :
  mFileRegion(file_region)
{
  mValue[0] = value1;
  mValue[1] = value2;
  mValue[2] = value3;
}

// デストラクタ
CptDelay::~CptDelay()
{
}

// ファイル位置を取出す．
FileRegion
CptDelay::file_region() const
{
  return mFileRegion;
}

// 値を取り出す．
const PtExpr*
CptDelay::value(ymuint32 pos) const
{
  if ( pos < 3 ) {
    return mValue[pos];
  }
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// 階層名を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptNameBranch::CptNameBranch(const char* name) :
  mName(name)
{
}

// デストラクタ
CptNameBranch::~CptNameBranch()
{
}

// 名前を取り出す．
const char*
CptNameBranch::name() const
{
  return mName;
}

// @brief インデックスの有無のチェック
// @retval true インデックスを持っている時
// @retval false インデックスを持っていない時
// デフォルトで false を返す．
bool
CptNameBranch::has_index() const
{
  return false;
}

// インデックスの取得
// @return インデックスの値
// デフォルトで 0 を返す．
int
CptNameBranch::index() const
{
  return 0;
}


//////////////////////////////////////////////////////////////////////
// インデックスつきの階層名を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptNameBranchI::CptNameBranchI(const char* name,
			       int index) :
  CptNameBranch(name),
  mIndex(index)
{
}

// デストラクタ
CptNameBranchI::~CptNameBranchI()
{
}

// インデックスを持っている時 true を返す．
bool
CptNameBranchI::has_index() const
{
  return true;
}

// インデックスを取り出す．
int
CptNameBranchI::index() const
{
  return mIndex;
}


//////////////////////////////////////////////////////////////////////
// attribute_instance を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptAttrInst::CptAttrInst(PtAttrSpecArray as_array) :
  mAttrSpecArray(as_array)
{
}

// デストラクタ
CptAttrInst::~CptAttrInst()
{
}

// @brief 要素数の取得
// @return 要素数
ymuint32
CptAttrInst::attrspec_num() const
{
  return mAttrSpecArray.size();
}
    
// @brief 要素の取得
// @param[in] pos 位置番号 ( 0 <= pos < attrspec_num() )
const PtAttrSpec*
CptAttrInst::attrspec(ymuint32 pos) const
{
  return mAttrSpecArray[pos];
}


//////////////////////////////////////////////////////////////////////
// attr_spec を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptAttrSpec::CptAttrSpec(const FileRegion& file_region,
			 const char* name,
			 PtExpr* expr) :
  mFileRegion(file_region),
  mName(name),
  mExpr(expr)
{
}
  
// デストラクタ
CptAttrSpec::~CptAttrSpec()
{
}

// ファイル位置を返す．
FileRegion
CptAttrSpec::file_region() const
{
  return mFileRegion;
}

// 名前を取り出す．
const char*
CptAttrSpec::name() const
{
  return mName;
}
  
// 式を取り出す．NULL の場合もある．
const PtExpr*
CptAttrSpec::expr() const
{
  return mExpr;
}

  
//////////////////////////////////////////////////////////////////////
// その他
//////////////////////////////////////////////////////////////////////

// ディレイコントロールを生成する．
PtControl*
CptFactory::new_DelayControl(const FileRegion& file_region,
			     PtExpr* value)
{
  ++ mNumDelayControl;
  void* p = alloc().get_memory(sizeof(CptDelayControl));
  return new (p) CptDelayControl(file_region, value);
}

// イベントコントロールを生成する．
PtControl*
CptFactory::new_EventControl(const FileRegion& file_region,
			     PtExprArray event_array)
{
  ++ mNumEventControl;
  void* p = alloc().get_memory(sizeof(CptEventControl));
  return new (p) CptEventControl(file_region, event_array);
}

// リピートコントロールを生成する．
PtControl*
CptFactory::new_RepeatControl(const FileRegion& file_region,
			      PtExpr* expr,
			      PtExprArray event_array)
{
  ++ mNumRepeatControl;
  void* p = alloc().get_memory(sizeof(CptRepeatControl));
  return new (p) CptRepeatControl(file_region, expr, event_array);
}

// 順序つき結合子を生成する．
PtConnection*
CptFactory::new_OrderedCon(const FileRegion& file_region,
			   PtExpr* expr)
{
  ++ mNumAiOrderedCon;
  void* p = alloc().get_memory(sizeof(CptOrderedCon));
  return new (p) CptOrderedCon(file_region, expr);
}

// 順序つき結合子を生成する．
PtConnection*
CptFactory::new_OrderedCon(PtExpr* expr)
{
  ++ mNumOrderedCon;
  FileRegion file_region;
  if ( expr ) {
    file_region = expr->file_region();
  }
  void* p = alloc().get_memory(sizeof(CptOrderedCon));
  return new (p) CptOrderedCon(file_region, expr);
}

// 名前つき結合子を生成する．
PtConnection*
CptFactory::new_NamedCon(const FileRegion& file_region,
			 const char* name,
			 PtExpr* expr)
{
  ++ mNumNamedCon;
  void* p = alloc().get_memory(sizeof(CptNamedCon));
  return new (p) CptNamedCon(file_region, name, expr);
}

// strength を生成する．
PtStrength*
CptFactory::new_Strength(const FileRegion& file_region,
			 tVpiStrength value1,
			 tVpiStrength value2)
{
  ++ mNumStrength;
  void* p = alloc().get_memory(sizeof(CptStrength));
  return new (p) CptStrength(file_region, value1, value2);
}

// strength を生成する．
PtStrength*
CptFactory::new_Strength(const FileRegion& file_region,
			 tVpiStrength value1)
{
  ++ mNumStrength;
  void* p = alloc().get_memory(sizeof(CptStrength));
  return new (p) CptStrength(file_region, value1);
}

// delay 値を生成する．
PtDelay*
CptFactory::new_Delay(const FileRegion& file_region,
		      PtExpr* value1)
{
  ++ mNumDelay;
  void* p = alloc().get_memory(sizeof(CptDelay));
  return new (p) CptDelay(file_region, value1);
}

// delay 値を生成する．
PtDelay*
CptFactory::new_Delay(const FileRegion& file_region,
		      PtExpr* value1,
		      PtExpr* value2)
{
  ++ mNumDelay;
  void* p = alloc().get_memory(sizeof(CptDelay));
  return new (p) CptDelay(file_region, value1, value2);
}

// delay 値を生成する．
PtDelay*
CptFactory::new_Delay(const FileRegion& file_region,
		      PtExpr* value1,
		      PtExpr* value2,
		      PtExpr* value3)
{
  ++ mNumDelay;
  void* p = alloc().get_memory(sizeof(CptDelay));
  return new (p) CptDelay(file_region, value1, value2, value3);
}

// 階層名を生成する．
PtNameBranch*
CptFactory::new_NameBranch(const char* name)
{
  ++ mNumNameBranch;
  void* p = alloc().get_memory(sizeof(CptNameBranch));
  return new (p) CptNameBranch(name);
}

// 階層名を生成する．
PtNameBranch*
CptFactory::new_NameBranch(const char* name,
			   int index)
{
  ++ mNumNameBranchI;
  void* p = alloc().get_memory(sizeof(CptNameBranchI));
  return new (p) CptNameBranchI(name, index);
}

  
//////////////////////////////////////////////////////////////////////
// attribute instance 関係
//////////////////////////////////////////////////////////////////////

// attribute instance を生成する．
PtAttrInst*
CptFactory::new_AttrInst(const FileRegion& file_region,
			 PtAttrSpecArray as_array)
{
  // file_region は不要
  ++ mNumAttrInst;
  void* p = alloc().get_memory(sizeof(CptAttrInst));
  return new (p) CptAttrInst(as_array);
}

// attribute spec を生成する．
PtAttrSpec*
CptFactory::new_AttrSpec(const FileRegion& file_region,
			 const char* name,
			 PtExpr* expr)
{
  ++ mNumAttrSpec;
  void* p = alloc().get_memory(sizeof(CptAttrSpec));
  return new (p) CptAttrSpec(file_region, name, expr);
}

END_NAMESPACE_YM_VERILOG
