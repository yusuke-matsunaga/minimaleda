
/// @file libym_verilog/pt_simple/SptMisc.cc
/// @brief その他の部品クラスの実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SptMisc.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "SptMisc.h"
#include "SptFactory.h"

#include "ym_verilog/pt/PtExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// delay control/event control/repeat control を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
// @param type クラスの型
// @param expr 式
//  - type == kDelayControl の場合は遅延式
//  - type == kRepeatControl の場合は繰り返し数を表す式
// @param event_array イベントリスト
SptControl::SptControl(const FileRegion& file_region,
		       tPtCtrlType type,
		       PtExpr* expr,
		       PtExprArray event_array) :
  mFileRegion(file_region),
  mType(type),
  mExpr(expr),
  mEventArray(event_array)
{
}

// デストラクタ
SptControl::~SptControl()
{
}

// ファイル位置の取得
// @return ファイル位置
FileRegion
SptControl::file_region() const
{
  return mFileRegion;
}

// 型の取得
// @return 型
tPtCtrlType
SptControl::type() const
{
  return mType;
}

// 遅延式の取得
// @retval 遅延を表す式 delay control の場合
// @retval NULL 上記以外
const PtExpr*
SptControl::delay() const
{
  if ( type() == kPtDelayControl ) {
    return mExpr;
  }
  else {
    return NULL;
  }
}

// イベントリストのサイズの取得
// @retval イベントリストのサイズ event control/repeat control の場合
// @retval NULL 上記以外
ymuint32
SptControl::event_num() const
{
  return mEventArray.size();
}
  
// @brief イベントリストの要素の取得
// @param[in] pos 位置番号 ( 0 <= pos < event_num() )
// @note event control/repeat control の場合のみ意味を持つ
const PtExpr*
SptControl::event(ymuint32 pos) const
{
  return mEventArray[pos];
}

// 繰り返し数の取得
// @retval 繰り返し数を表す式 repeat control の場合
// @retval NULL 上記以外
const PtExpr*
SptControl::rep_expr() const
{
  if ( type() == kPtRepeatControl ) {
    return mExpr;
  }
  else {
    return NULL;
  }
}


//////////////////////////////////////////////////////////////////////
// ordered_connection/named_connection を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
// @param ai_list 属性インスタンスのリスト
// @param name 名前 (空の場合もある)
// @param expr 式
SptConnection::SptConnection(const FileRegion& file_region,
			     PtExpr* expr,
			     const char* name) :
  mFileRegion(file_region),
  mName(name),
  mExpr(expr)
{
}

// デストラクタ
SptConnection::~SptConnection()
{
}
  
// ファイル位置の取得
// @return ファイル位置
FileRegion
SptConnection::file_region() const
{
  return mFileRegion;
}

// 名前の取得
// @retval 名前 named connection の場合
// @retval "" ordered connection の場合
const char*
SptConnection::name() const
{
  return mName;
}

// 式の取得
// @return 式
const PtExpr*
SptConnection::expr() const
{
  return mExpr;
}


//////////////////////////////////////////////////////////////////////
// strength を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ (0/1の強度用)
// @param file_region ファイル位置の情報
// @param drive0 0 の信号強度
// @param drive1 1 の信号強度
SptStrength::SptStrength(const FileRegion& file_region,
			 tVpiStrength drive0,
			 tVpiStrength drive1) :
  mFileRegion(file_region)
{
  mDrive0 = drive0;
  mDrive1 = drive1;
  mCharge = kVpiNoStrength;
}

// コンストラクタ (電荷の強度用)
// @param file_region ファイル位置の情報
// @param charge 電荷の信号強度
SptStrength::SptStrength(const FileRegion& file_region,
			 tVpiStrength charge) :
  mFileRegion(file_region)
{
  mDrive0 = kVpiNoStrength;
  mDrive1 = kVpiNoStrength;
  mCharge = charge;
}

// デストラクタ
SptStrength::~SptStrength()
{
}

// ファイル位置の取得
// @return ファイル位置
FileRegion
SptStrength::file_region() const
{
  return mFileRegion;
}

// drive strength0 の取得
// @return 0 の強度
tVpiStrength
SptStrength::drive0() const
{
  return mDrive0;
}

// drive strength1 の取得
// @return 1 の強度
tVpiStrength
SptStrength::drive1() const
{
  return mDrive1;
}

// charge strength の取得
// @return 電荷の強度
tVpiStrength
SptStrength::charge() const
{
  return mCharge;
}


//////////////////////////////////////////////////////////////////////
// delay を表すクラス
//////////////////////////////////////////////////////////////////////

// 一つの値をとるコンストラクタ
// @param file_region ファイル位置の情報
// @param value1 遅延値
SptDelay::SptDelay(const FileRegion& file_region,
		   PtExpr* value1) :
  mFileRegion(file_region)
{
  mValue[0] = value1;
  mValue[1] = NULL;
  mValue[2] = NULL;
}

// 二つの値をとるコンストラクタ
// @param file_region ファイル位置の情報
// @param value1 遅延値1
// @param value2 遅延値2
SptDelay::SptDelay(const FileRegion& file_region,
		   PtExpr* value1,
		   PtExpr* value2) :
  mFileRegion(file_region)
{
  mValue[0] = value1;
  mValue[1] = value2;
  mValue[2] = NULL;
}
  
// 三つの値をとるコンストラクタ
// @param file_region ファイル位置の情報
// @param value1 遅延値1
// @param value2 遅延値2
// @param value3 遅延値3
SptDelay::SptDelay(const FileRegion& file_region,
		   PtExpr* value1,
		   PtExpr* value2,
		   PtExpr* value3) :
  mFileRegion(file_region)
{
  mValue[0] = value1;
  mValue[1] = value2;
  mValue[2] = value3;
}

// ファイル位置の取得
// @return ファイル位置
SptDelay::~SptDelay()
{
}

// ファイル位置の取得
// @return ファイル位置
FileRegion
SptDelay::file_region() const
{
  return mFileRegion;
}

// 値の取得
// @param pos 取得する遅延値の位置(0 〜 3)
// @return pos 番目の遅延を表す式
// 該当する要素がなければ NULL を返す．
const PtExpr*
SptDelay::value(ymuint32 pos) const
{
  if ( pos < 3 ) {
    return mValue[pos];
  }
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// 階層名を表すクラス
//////////////////////////////////////////////////////////////////////

// 名前のみのコンストラクタ
// @param name 名前
SptNameBranch::SptNameBranch(const char* name) :
  mName(name),
  mIndex(0)
{
}

// 名前とインデックスのコンストラクタ
// @param name 名前
// @param index インデックス
SptNameBranch::SptNameBranch(const char* name,
			     int index) :
  mName(name)
{
  mIndex = (static_cast<unsigned int>(index) << 1) | 1;
}

// デストラクタ
SptNameBranch::~SptNameBranch()
{
}

// 名前の取得
// @return 名前
const char*
SptNameBranch::name() const
{
  return mName;
}

// インデックスの有無のチェック
// @retval true インデックスを持っている時
// @retval false インデックスを持っていない時
bool
SptNameBranch::has_index() const
{
  return mIndex != 0;
}

// インデックスの取得
// @return インデックスの値
int
SptNameBranch::index() const
{
  return static_cast<int>(mIndex) >> 1;
}


//////////////////////////////////////////////////////////////////////
// attribute_instance を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param as_array attr_spec のリスト
SptAttrInst::SptAttrInst(PtAttrSpecArray as_array) :
  mAttrSpecArray(as_array)
{
}

// デストラクタ
SptAttrInst::~SptAttrInst()
{
}

// 要素数の取得
// @return 要素数
ymuint32
SptAttrInst::attrspec_num() const
{
  return mAttrSpecArray.size();
}
    
// @brief 要素の取得
// @param[in] pos 位置番号 ( 0 <= pos < attrspec_num() )
const PtAttrSpec*
SptAttrInst::attrspec(ymuint32 pos) const
{
  return mAttrSpecArray[pos];
}


//////////////////////////////////////////////////////////////////////
// attr_spec を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
// @param name 名前
// @param expr 式(NULLの場合もある)
SptAttrSpec::SptAttrSpec(const FileRegion& file_region,
			 const char* name,
			 PtExpr* expr) :
  mFileRegion(file_region),
  mName(name),
  mExpr(expr)
{
}

// デストラクタ
SptAttrSpec::~SptAttrSpec()
{
}

// ファイル位置の取得
// @return ファイル位置
FileRegion
SptAttrSpec::file_region() const
{
  return mFileRegion;
}

// 名前の取得
// @return 名前
const char*
SptAttrSpec::name() const
{
  return mName;
}
  
// 式の取得
// @return 式
// NULL の場合もある．
const PtExpr*
SptAttrSpec::expr() const
{
  return mExpr;
}

  
//////////////////////////////////////////////////////////////////////
// その他
//////////////////////////////////////////////////////////////////////

// @brief ディレイコントロールの生成
// @param[in] file_region ファイル位置の情報
// @param[in] value 遅延を表す式
// @return 生成されたディレイコントロール
PtControl*
SptFactory::new_DelayControl(const FileRegion& file_region,
			     PtExpr* value)
{
  void* p = alloc().get_memory(sizeof(SptControl));
  return new (p) SptControl(file_region, kPtDelayControl,
			    value, PtExprArray());
}

// @brief イベントコントロールの生成
// @param[in] file_region ファイル位置の情報
// @param[in] event_array イベントのリスト
// @return 生成されたイベントコントロール
PtControl*
SptFactory::new_EventControl(const FileRegion& file_region,
			     PtExprArray event_array)
{
  void* p = alloc().get_memory(sizeof(SptControl));
  return new (p) SptControl(file_region, kPtEventControl,
			    NULL, event_array);
}

// @brief リピートコントロールの生成
// @param[in] file_region ファイル位置の情報
// @param[in] expr 繰り返し数を表す式
// @param[in] event_array 繰り返しの単位となるイベントのリスト
// @return 生成されたリピートコントロール
PtControl*
SptFactory::new_RepeatControl(const FileRegion& file_region,
			      PtExpr* expr,
			      PtExprArray event_array)
{
  void* p = alloc().get_memory(sizeof(SptControl));
  return new (p) SptControl(file_region, kPtRepeatControl,
			    expr, event_array);
}

// @brief 順序つき結合子の生成
// @param[in] file_region ファイル位置の情報
// @param[in] expr 結合する式
// @return 生成された結合子
PtConnection*
SptFactory::new_OrderedCon(const FileRegion& file_region,
			   PtExpr* expr)
{
  void* p = alloc().get_memory(sizeof(SptConnection));
  return new (p) SptConnection(file_region, expr, NULL);
}

// @brief 順序つき結合子の生成
// @param[in] expr 結合する式
// @return 生成された結合子
PtConnection*
SptFactory::new_OrderedCon(PtExpr* expr)
{
  FileRegion file_region;
  if ( expr ) {
    file_region = expr->file_region();
  }
  void* p = alloc().get_memory(sizeof(SptConnection));
  return new (p) SptConnection(file_region, expr);
}

// @brief 名前付き結合子の生成
// @param[in] file_region ファイル位置の情報
// @param[in] name 名前
// @param[in] expr 結合する式
// @return 生成された結合子
PtConnection*
SptFactory::new_NamedCon(const FileRegion& file_region,
			 const char* name,
			 PtExpr* expr)
{
  void* p = alloc().get_memory(sizeof(SptConnection));
  return new (p) SptConnection(file_region, expr, name);
}

// @brief strength の生成
// @param[in] file_region ファイル位置の情報
// @param[in] value0 '0' の強度
// @param[in] value1 '1' の強度
// @return 生成された strength
PtStrength*
SptFactory::new_Strength(const FileRegion& file_region,
			 tVpiStrength value0,
			 tVpiStrength value1)
{
  void* p = alloc().get_memory(sizeof(SptStrength));
  return new (p) SptStrength(file_region, value0, value1);
}

// @brief charge strength の生成
// @param[in] file_region ファイル位置の情報
// @param[in] value 強度
// @return 生成された strength
PtStrength*
SptFactory::new_Strength(const FileRegion& file_region,
			 tVpiStrength value)
{
  void* p = alloc().get_memory(sizeof(SptStrength));
  return new (p) SptStrength(file_region, value);
}

// @brief 遅延値の生成 (1つの値)
// @param[in] file_region ファイル位置の情報
// @param[in] value1 値1
// @return 生成された遅延値
PtDelay*
SptFactory::new_Delay(const FileRegion& file_region,
		      PtExpr* value1)
{
  void* p = alloc().get_memory(sizeof(SptDelay));
  return new (p) SptDelay(file_region, value1);
}

// @brief 遅延値の生成 (2つの値)
// @param[in] file_region ファイル位置の情報
// @param[in] value1 値1
// @param[in] value2 値2
// @return 生成された遅延値
PtDelay*
SptFactory::new_Delay(const FileRegion& file_region,
		      PtExpr* value1,
		      PtExpr* value2)
{
  void* p = alloc().get_memory(sizeof(SptDelay));
  return new (p) SptDelay(file_region, value1, value2);
}

// @brief 遅延値の生成 (3つの値)
// @param[in] file_region ファイル位置の情報
// @param[in] value1 値1
// @param[in] value2 値2
// @param[in] value3 値3
// @return 生成された遅延値
PtDelay*
SptFactory::new_Delay(const FileRegion& file_region,
		      PtExpr* value1,
		      PtExpr* value2,
		      PtExpr* value3)
{
  void* p = alloc().get_memory(sizeof(SptDelay));
  return new (p) SptDelay(file_region, value1, value2, value3);
}

// @brief 階層名の生成
// @param[in] name 名前
// @return 生成された階層名
PtNameBranch*
SptFactory::new_NameBranch(const char* name)
{
  void* p = alloc().get_memory(sizeof(SptNameBranch));
  return new (p) SptNameBranch(name);
}

// @brief インデックス付きの階層名の生成
// @param[in] name 名前
// @param[in] index インデックス
// @return 生成された階層名
PtNameBranch*
SptFactory::new_NameBranch(const char* name,
			   int index)
{
  void* p = alloc().get_memory(sizeof(SptNameBranch));
  return new (p) SptNameBranch(name, index);
}

  
//////////////////////////////////////////////////////////////////////
// attribute instance 関係
//////////////////////////////////////////////////////////////////////

// @brief attribute instance の生成
// @param[in] file_region ファイル位置の情報
// @param[in] as_array attribute spec のリスト
// @return 生成された attribute instance
PtAttrInst*
SptFactory::new_AttrInst(const FileRegion& file_region,
			 PtAttrSpecArray as_array)
{
  void* p = alloc().get_memory(sizeof(SptAttrInst));
  // file_region は不要
  return new (p) SptAttrInst(as_array);
}

// @brief attribute spec の生成
// @param[in] file_region ファイル位置の情報
// @param[in] name 名前
// @param[in] expr 値
// @return 生成された attribute spec
PtAttrSpec*
SptFactory::new_AttrSpec(const FileRegion& file_region,
			 const char* name,
			 PtExpr* expr)
{
  void* p = alloc().get_memory(sizeof(SptAttrSpec));
  return new (p) SptAttrSpec(file_region, name, expr);
}

END_NAMESPACE_YM_VERILOG
