
/// @file libym_verilog/elb_impl/EiUdp.cc
/// @brief EiUdp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiUdp.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiUdp.h"
#include "ElbExpr.h"

#include "ym_verilog/pt/PtUdp.h"
#include "ym_verilog/pt/PtDecl.h"
#include "ym_verilog/pt/PtArray.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief UDP定義を生成する．
// @param[in] pt_udp パース木の UDP 定義
// @param[in] is_protected プロテクト属性
ElbUdpDefn*
EiFactory::new_UdpDefn(const PtUdp* pt_udp,
		       bool is_protected)
{
  ymuint32 port_num = pt_udp->port_num();
  void* q = mAlloc.get_memory(sizeof(EiUdpIO) * port_num);
  EiUdpIO* iodecl = new (q) EiUdpIO[port_num];

  ymuint32 table_size = pt_udp->table_array().size();
  void* r = mAlloc.get_memory(sizeof(EiTableEntry) * table_size);
  EiTableEntry* table = new (r) EiTableEntry[table_size];

  ymuint32 row_size = port_num;
  if ( pt_udp->prim_type() == kVpiSeqPrim ) {
    ++ row_size;
  }
  ymuint32 vsize = row_size * table_size;
  void* s = mAlloc.get_memory(sizeof(tVpiUdpVal) * vsize);
  tVpiUdpVal* val_array = new (s) tVpiUdpVal[vsize];

  void* p = mAlloc.get_memory(sizeof(EiUdpDefn));
  EiUdpDefn* udp = new (p) EiUdpDefn(pt_udp, is_protected,
				     port_num, iodecl,
				     table_size, table,
				     val_array);
  for (ymuint32 i = 0; i < port_num; ++ i) {
    iodecl[i].mUdp = udp;
  }
  for (ymuint32 i = 0; i < table_size; ++ i) {
    table[i].mUdp = udp;
    table[i].mValArray = val_array;
    val_array += row_size;
  }

  return udp;
}


//////////////////////////////////////////////////////////////////////
// クラス EiUdpDefn
//////////////////////////////////////////////////////////////////////

// @param[in] pt_udp パース木の UDP 定義
// @param[in] is_protected プロテクト属性
// @param[in] io_num ポート数
// @param[in] io_array IOの配列
// @param[in] table_num テーブルの行数
// @param[in] table テーブル
// @param[in] val_array テーブル中の値を納める配列
EiUdpDefn::EiUdpDefn(const PtUdp* pt_udp,
		     bool is_protected,
		     ymuint32 io_num,
		     EiUdpIO* io_array,
		     ymuint32 table_num,
		     EiTableEntry* table,
		     tVpiUdpVal* val_array) :
  mPtUdp(pt_udp),
  mPortNum(io_num),
  mProtected(is_protected),
  mIODeclList(io_array),
  mInitExpr(NULL),
  mInitVal(kVpiScalarX),
  mTableEntrySize(table_num),
  mTableEntryList(table),
  mValArray(val_array)
{
}

// @brief デストラクタ
EiUdpDefn::~EiUdpDefn()
{
}

// @brief 型の取得
tVpiObjType
EiUdpDefn::type() const
{
  return kVpiUdpDefn;
}

// @brief ファイル位置を返す．
FileRegion
EiUdpDefn::file_region() const
{
  return mPtUdp->file_region();
}

// @brief 定義された名前を返す．
const char*
EiUdpDefn::def_name() const
{
  return mPtUdp->name();
}

// @brief primitive type を返す．
tVpiPrimType
EiUdpDefn::prim_type() const
{
  return mPtUdp->prim_type();
}

// @brief ポート数を返す．
ymuint32
EiUdpDefn::port_num() const
{
  return mPortNum;
}

// @brief 入力の宣言要素を返す．
// @param[in] pos 入力番号
const VlIODecl*
EiUdpDefn::input(ymuint32 pos) const
{
  return &mIODeclList[pos];
}

// @brief 出力の宣言要素を返す．
const VlIODecl*
EiUdpDefn::output() const
{
  return &mIODeclList[mPortNum - 1];
}

// @brief protected かどうかを返す．
bool
EiUdpDefn::is_protected() const
{
  return mProtected;
}

// @brief 初期値の式を返す．
const VlExpr*
EiUdpDefn::init_expr() const
{
  return mInitExpr;
}

// @brief 初期値を返す．
// @return 0/1/X を返す．
tVpiScalarVal
EiUdpDefn::init_val() const
{
  return mInitVal;
}

// @brief table entry の行数を返す．
ymuint32
EiUdpDefn::table_size() const
{
  return mTableEntrySize;
}

// @brief table entry を返す．
// @param[in] pos 行番号
const VlTableEntry*
EiUdpDefn::table_entry(ymuint32 pos) const
{
  return &mTableEntryList[pos];
}

// @brief 入出力オブジェクトの内容を設定する．
// @param[in] pos ポート中の位置
// @param[in] file_region ソースファイル上の位置
// @param[in] name 名前
// @param[in] dir 向き
void
EiUdpDefn::set_io(ymuint32 pos,
		  const PtIOHead* pt_header,
		  const PtIOItem* pt_item)
{
  mIODeclList[pos].set(pt_header, pt_item);
}

// @brief 初期値を設定する．
// @param[in] init_expr 初期値を表す式
// @param[in] init_val 初期値
void
EiUdpDefn::set_initial(ElbExpr* init_expr,
		       tVpiScalarVal init_val)
{
  mInitExpr = init_expr;
  mInitVal = init_val;
}

// @brief table entry の内容を設定する．
// @param[in] pos 行番号
// @param[in] pt_udp_entry パース木の一行分の定義
// @param[in] vals シンボル値の配列
void
EiUdpDefn::set_tableentry(ymuint32 pos,
			  const PtUdpEntry* pt_udp_entry,
			  const vector<tVpiUdpVal>& vals)
{
  mTableEntryList[pos].set(pt_udp_entry, vals);
}


//////////////////////////////////////////////////////////////////////
/// クラス EiUdpIO
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiUdpIO::EiUdpIO()
{
}

// @brief デストラクタ
EiUdpIO::~EiUdpIO()
{
}

// @brief 型の取得
tVpiObjType
EiUdpIO::type() const
{
  return kVpiIODecl;
}

// ファイル位置を返す．
FileRegion
EiUdpIO::file_region() const
{
  return mPtItem->file_region();
}

// @brief 名前を返す．
const char*
EiUdpIO::name() const
{
  return mPtItem->name();
}

// @brief 方向を返す．
tVpiDirection
EiUdpIO::direction() const
{
  switch ( mPtHeader->type() ) {
  case kPtIO_Input:  return kVpiInput;
  case kPtIO_Output: return kVpiOutput;
  default: assert_not_reached(__FILE__, __LINE__);
  }
  // ダミー
  return kVpiNoDirection;
}

// @brief 符号の属性の取得
// @return 符号付きのとき true を返す．
// @note このクラスは false を返す．
bool
EiUdpIO::is_signed() const
{
  return false;
}

// @brief 範囲の MSB の取得
// @return このクラスは NULL を返す．
const VlExpr*
EiUdpIO::left_range() const
{
  return NULL;
}

// @brief 範囲の LSB の取得
// @return このクラスは NULL を返す．
const VlExpr*
EiUdpIO::right_range() const
{
  return NULL;
}

// @brief MSB の値を返す．
// @retval 範囲のMSBの値 範囲指定を持つとき
// @retval -1 範囲指定を持たないとき
int
EiUdpIO::left_range_const() const
{
  return -1;
}

// @brief LSB の値を返す．
// @retval 範囲のLSBの値 範囲指定を持つとき
// @retval -1 範囲指定を持たないとき
int
EiUdpIO::right_range_const() const
{
  return -1;
}

// @brief サイズを返す．
// このクラスは 1 を返す．
ymuint32
EiUdpIO::bit_size() const
{
  return 1;
}

// @brief 対応する宣言要素を返す．
// @note このクラスでは NULL を返す．
const VlDecl*
EiUdpIO::decl() const
{
  return NULL;
}

// @brief 親のモジュールの取得
// @return このクラスは NULL を返す．
const VlModule*
EiUdpIO::module() const
{
  return NULL;
}

// @brief 親のUDP定義を返す．
const VlUdpDefn*
EiUdpIO::udp_defn() const
{
  return mUdp;
}

// @brief 親のタスク/の取得
// @return このクラスは NULL を返す．
const VlTaskFunc*
EiUdpIO::task() const
{
  return NULL;
}

// @brief 親の関数の取得
// @return このクラスは NULL を返す．
const VlTaskFunc*
EiUdpIO::function() const
{
  return NULL;
}

// @brief 内容を設定する．
// @param[in] pt_header パース木のIO宣言ヘッダ
// @param[in] pt_item パース木のIO宣言定義
void
EiUdpIO::set(const PtIOHead* pt_header,
	     const PtIOItem* pt_item)
{
  mPtHeader = pt_header;
  mPtItem = pt_item;
}


//////////////////////////////////////////////////////////////////////
/// クラス EiTableEntry
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiTableEntry::EiTableEntry()
{
}

// @brief デストラクタ
EiTableEntry::~EiTableEntry()
{
}

// @brief 型の取得
tVpiObjType
EiTableEntry::type() const
{
  return kVpiTableEntry;
}

// @brief ファイル位置を返す．
FileRegion
EiTableEntry::file_region() const
{
  return mPtUdpEntry->file_region();
}

// @brief 一行の要素数を返す．
ymuint32
EiTableEntry::size() const
{
  ymuint32 row_size = mUdp->port_num();
  if ( mUdp->prim_type() == kVpiSeqPrim ) {
    ++ row_size;
  }
  return row_size;
}

// @brief pos 番目の位置の値を返す．
tVpiUdpVal
EiTableEntry::val(ymuint32 pos) const
{
  return mValArray[pos];
}

// @brief 一行文の内容を表す文字列をつくる．
string
EiTableEntry::str() const
{
  ymuint32 n = size();
  ymuint32 in = n - 1; // 出力変数の分を減らす
  if ( mUdp->prim_type() == vpiSeqPrim ) {
    -- in; // さらに状態変数の分を減らす．
  }
  ymuint32 in1 = in - 1;
  ymuint32 n1 = n - 1;
  string s;
  for (ymuint32 pos = 0; pos < n; ++ pos) {
    s += symbol2string(val(pos));
    if ( pos < in1 ) {
      s += " ";
    }
    else if ( pos < n1 ) {
      s += " : ";
    }
  }
  return s;
}

// @brief 設定する．
void
EiTableEntry::set(const PtUdpEntry* pt_entry,
		  const vector<tVpiUdpVal>& vals)
{
  mPtUdpEntry = pt_entry;
  ymuint32 n = size();
  for (ymuint32 i = 0; i < n; ++ i) {
    mValArray[i] = vals[i];
  }
}

END_NAMESPACE_YM_VERILOG
