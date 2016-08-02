
/// @file libym_verilog/pt_compact/CptUdp.cc
/// @brief CptUdp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CptUdp.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "CptUdp.h"
#include "CptFactory.h"

#include "PtiDecl.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// UDP を表すノード
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptUdp::CptUdp(const FileRegion& file_region,
	       const char* name,
	       PtiPortArray port_array,
	       PtIOHeadArray iohead_array,
	       bool is_seq,
	       PtExpr* init_value,
	       PtUdpEntryArray entry_array) :
  mFileRegion(file_region),
  mName(name),
  mPortArray(port_array),
  mIOHeadArray(iohead_array),
  mSeq(is_seq),
  mInitValue(init_value),
  mTableArray(entry_array)
{
}

// デストラクタ
CptUdp::~CptUdp()
{
}

// ファイル位置を返す．
FileRegion
CptUdp::file_region() const
{
  return mFileRegion;
}

// primitivie type を返す．
tVpiPrimType
CptUdp::prim_type() const
{
  if ( mSeq ) {
    return kVpiSeqPrim;
  }
  else {
    return kVpiCombPrim;
  }
}

// 名前を返す．
const char*
CptUdp::name() const
{
  return mName;
}

// ポート数を取り出す．
ymuint32
CptUdp::port_num() const
{
  return mPortArray.size();
}
  
// 先頭のポートを取り出す．
const PtPort*
CptUdp::port(ymuint32 pos) const
{
  return mPortArray[pos];
}
  
// @brief 入出力宣言ヘッダ配列の取得
PtIOHeadArray
CptUdp::iohead_array() const
{
  return mIOHeadArray;
}

// 初期値を取出す．
const PtExpr*
CptUdp::init_value() const
{
  return mInitValue;
}

// @brief テーブルを取り出す．
PtUdpEntryArray
CptUdp::table_array() const
{
  return mTableArray;
}


//////////////////////////////////////////////////////////////////////
// UDP のテーブルの中身を表すクラス(組合せ回路用)
//////////////////////////////////////////////////////////////////////
  
// コンストラクタ
CptUdpEntry::CptUdpEntry(const FileRegion& file_region,
			 PtUdpValueArray input_array,
			 PtUdpValue* output) :
  mFileRegion(file_region),
  mInputArray(input_array),
  mOutput(output)
{
}

// デストラクタ
CptUdpEntry::~CptUdpEntry()
{
}

// ファイル位置を返す．
FileRegion
CptUdpEntry::file_region() const
{
  return mFileRegion;
}
  
// @brief 入力値の配列を取り出す．
PtUdpValueArray
CptUdpEntry::input_array() const
{
  return mInputArray;
}

// @brief 現状態の値を取り出す．
// このクラスでは NULL を返す．
const PtUdpValue*
CptUdpEntry::current() const
{
  return NULL;
}

// 出力の値を取り出す．
PtUdpValue*
CptUdpEntry::output() const
{
  return mOutput;
}


//////////////////////////////////////////////////////////////////////
// UDP のテーブルの中身を表すクラス(順序回路用)
//////////////////////////////////////////////////////////////////////
  
// コンストラクタ
CptUdpEntryS::CptUdpEntryS(const FileRegion& file_region,
			   PtUdpValueArray input_array,
			   PtUdpValue* current,
			   PtUdpValue* output) :
  CptUdpEntry(file_region, input_array, output),
  mCurrent(current)
{
}

// デストラクタ
CptUdpEntryS::~CptUdpEntryS()
{
}

// 現状態の値を取り出す．
PtUdpValue*
CptUdpEntryS::current() const
{
  return mCurrent;
}


//////////////////////////////////////////////////////////////////////
// UDP のテーブルの値を表すクラス
//////////////////////////////////////////////////////////////////////
  
// コンストラクタ
CptUdpValue::CptUdpValue(const FileRegion& file_region,
			 tVpiUdpVal symbol) :
  mFileRegion(file_region),
  mSymbol(symbol)
{
}

// デストラクタ
CptUdpValue::~CptUdpValue()
{
}

// ファイル位置を返す．
FileRegion
CptUdpValue::file_region() const
{
  return mFileRegion;
}

// シンボルを取り出す．
tVpiUdpVal
CptUdpValue::symbol() const
{
  return mSymbol;
}


//////////////////////////////////////////////////////////////////////
// UDP 関係
//////////////////////////////////////////////////////////////////////

// combinational UDP の生成
// @param file_region ファイル位置情報
// @param name UDP名
// @param port_list ポート名のリスト
// @param iolist IOリスト
// @param entry_top テーブルエントリのリスト
// @return 生成された UDP
PtUdp*
CptFactory::new_CmbUdp(const FileRegion& file_region,
		       const char* name,
		       PtiPortArray port_array,
		       PtIOHeadArray iohead_array,
		       PtUdpEntryArray entry_array)
{
  ++ mNumUdp;
  void* p = alloc().get_memory(sizeof(CptUdp));
  return new (p) CptUdp(file_region,
			name,
			port_array,
			iohead_array,
			false, NULL,
			entry_array);
}

// sequential UDP の生成
// @param file_region ファイル位置の情報
// @param ai_top 属性インスタンスのリスト
// @param name UDP名
// @param port_top ポート名のリスト
// @param iolist IOリスト
// @param init_value 初期値を表す式
// @param entry_top テーブルエントリのリスト
// @return 生成された UDP
PtUdp*
CptFactory::new_SeqUdp(const FileRegion& file_region,
		       const char* name,
		       PtiPortArray port_array,
		       PtIOHeadArray iohead_array,
		       PtExpr* init_value,
		       PtUdpEntryArray entry_array)
{
  ++ mNumUdp;
  void* p = alloc().get_memory(sizeof(CptUdp));
  return new (p) CptUdp(file_region,
			name,
			port_array,
			iohead_array,
			true,
			init_value,
			entry_array);
}

// combinational UDP 用のテーブルエントリの生成
// @param file_region ファイル位置の情報
// @param input_list 入力値のリスト
// @param output 出力値のリスト
// @return 生成されたテーブルエントリ
PtUdpEntry*
CptFactory::new_UdpEntry(const FileRegion& file_region,
			 PtUdpValueArray input_array,
			 PtUdpValue* output)
{
  ++ mNumUdpEntry;
  void* p = alloc().get_memory(sizeof(CptUdpEntry));
  return new (p) CptUdpEntry(file_region,
			     input_array,
			     output);
}

// sequential UDP 用のテーブルエントリの生成
// @param file_region ファイル位置の情報
// @param input_list 入力値のリスト
// @param current 現状態値
// @param output 出力値のリスト
// @return 生成されたテーブルエントリ
PtUdpEntry*
CptFactory::new_UdpEntry(const FileRegion& file_region,
			 PtUdpValueArray input_array,
			 PtUdpValue* current,
			 PtUdpValue* output)
{
  ++ mNumUdpEntryS;
  void* p = alloc().get_memory(sizeof(CptUdpEntryS));
  return new (p) CptUdpEntryS(file_region,
			      input_array,
			      current,
			      output);
}

// UDP のテーブルエントリの要素の値の生成 (1つの値)
// @param file_region ファイル位置の情報
// @param symbol シンボル
// @return 生成された値
PtUdpValue*
CptFactory::new_UdpValue(const FileRegion& file_region,
			 tVpiUdpVal symbol)
{
  ++ mNumUdpValue;
  void* p = alloc().get_memory(sizeof(CptUdpValue));
  return new (p) CptUdpValue(file_region, symbol);
}

END_NAMESPACE_YM_VERILOG
