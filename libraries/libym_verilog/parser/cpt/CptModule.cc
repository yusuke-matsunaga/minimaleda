
/// @file libym_verilog/pt_compact/CptModule.cc
/// @brief CptModule の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CptModule.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "CptModule.h"
#include "CptFactory.h"

#include "ym_verilog/pt/PtItem.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// module を表すノード
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptModule::CptModule(const FileRegion& file_region,
		     const char* name,
		     bool macro,
		     bool is_cell,
		     bool is_protected,
		     int time_unit,
		     int time_precision,
		     tVpiNetType net_type,
		     tVpiUnconnDrive unconn,
		     tVpiDefDelayMode delay,
		     int decay,
		     bool explicit_name,
		     bool portfaults,
		     bool suppress_faults,
		     const string& config,
		     const string& library,
		     const string& cell,
		     PtDeclHeadArray paramport_array,
		     PtiPortArray port_array,
		     PtIOHeadArray iohead_array,
		     PtDeclHeadArray paramhead_array,
		     PtDeclHeadArray lparamhead_array,
		     PtDeclHeadArray declhead_array,
		     PtItemArray item_array) :
  mFileRegion(file_region),
  mName(name),
  mDefDecayTime(decay),
  mConfig(config),
  mLibrary(library),
  mCell(cell),
  mParamPortArray(paramport_array),
  mPortArray(port_array),
  mIOHeadArray(iohead_array),
  mParamHeadArray(paramhead_array),
  mLparamHeadArray(lparamhead_array),
  mDeclHeadArray(declhead_array),
  mItemArray(item_array)
{
  mFlags =
    is_cell
    | (is_protected << 1)
    | ((time_precision + 16) << 2)
    | ((time_unit + 16) << 7)
    | (net_type << 12)
    | (unconn << 16)
    | (delay << 18)
    | (macro << 21)
    | (explicit_name << 22)
    | (1 << 23) // top_module
    | (portfaults << 25)
    | (suppress_faults << 26)
    ;
  
  ymuint32 n = 0;
  for (ymuint32 i = 0; i < mIOHeadArray.size(); ++ i) {
    n += mIOHeadArray[i]->item_num();
  }
  mIODeclNum = n;

  for (ymuint32 i = 0; i < mItemArray.size(); ++ i) {
    const PtItem* item = mItemArray[i];
    if ( item->type() == kPtItem_Func ) {
      mFuncDic.insert(make_pair(item->name(), item));
    }
  }
}

// デストラクタ
CptModule::~CptModule()
{
}

// ファイル位置を返す．
FileRegion
CptModule::file_region() const
{
  return mFileRegion;
}

// 名前を取り出す．
const char*
CptModule::name() const
{
  return mName;
}
  
// @brief パラメータポート宣言配列の取得
PtDeclHeadArray
CptModule::paramport_array() const
{
  return mParamPortArray;
}

// @brief ポート数の取得
// @return ポート数
ymuint32
CptModule::port_num() const
{
  return mPortArray.size();
}

// ポートを返す．
const PtPort*
CptModule::port(ymuint32 pos) const
{
  return mPortArray[pos];
}
  
// @brief 入出力宣言ヘッダ配列の取得
PtIOHeadArray
CptModule::iohead_array() const
{
  return mIOHeadArray;
}
  
// @brief 入出力宣言の要素数の取得
// @note 個々のヘッダが持つ要素数の総和を計算する．
ymuint32
CptModule::iodecl_num() const
{
  return mIODeclNum;
}

// @brief parameter 宣言ヘッダ配列の取得
PtDeclHeadArray
CptModule::paramhead_array() const
{
  return mParamHeadArray;
}
  
// @brief localparam 宣言ヘッダ配列の取得
PtDeclHeadArray
CptModule::localparamhead_array() const
{
  return mLparamHeadArray;
}
  
// @brief 宣言ヘッダ配列の取得
PtDeclHeadArray
CptModule::declhead_array() const
{
  return mDeclHeadArray;
}
  
// @brief item 配列の取得
PtItemArray
CptModule::item_array() const
{
  return mItemArray;
}
  
// macromodule の時 true を返す．
bool
CptModule::is_macromodule() const
{
  return static_cast<bool>((mFlags >> 21) & 1);
}

// cell の時 true を返す．
// 具体的には `celldefine --- `endcelldefine の間にあるモジュールのこと
bool
CptModule::is_cell() const
{
  return static_cast<bool>(mFlags & 1);
}
    
// protect されていたら true を返す．
bool
CptModule::is_protected() const
{
  return static_cast<bool>((mFlags >> 1) & 1);
}
  
// time unit を返す．
// 結果は 2 〜 -15 の整数
// もしくは未定義を表す -16
int
CptModule::time_unit() const
{
  return ((mFlags >> 7) & 0x1f) - 16;
}

// time precision を返す．
// 結果は 2 〜 -15 の整数
// もしくは未定義を表す -16
int
CptModule::time_precision() const
{
  return ((mFlags >> 2) & 0x1f) - 16;
}

// default net type を返す．
tVpiNetType
CptModule::nettype() const
{
  return static_cast<tVpiNetType>((mFlags >> 12) & 0xf);
}
  
// unconnected drive を返す．
tVpiUnconnDrive
CptModule::unconn_drive() const
{
  return static_cast<tVpiUnconnDrive>((mFlags >> 16) & 0x3);
}
  
// default delay mode を返す．
tVpiDefDelayMode
CptModule::delay_mode() const
{
  return static_cast<tVpiDefDelayMode>((mFlags >> 18) & 0x7);
}

// default decay time を返す．
int
CptModule::decay_time() const
{
  return mDefDecayTime;
}

// 名無しのポートを持つことを記録する．
void
CptModule::set_named_port()
{
  mFlags |= (1 << 22);
}

// すべてのポートが外部名を持っているときに true を返す．
// { a, b } のような名無しポートがあると false となる．
// true の時しか名前による結合は行えない．
bool
CptModule::explicit_name() const
{
  return (mFlags >> 22) & 1;
}

// 親がいないときに true を返す．
bool
CptModule::is_topmodule() const
{
  return static_cast<bool>((mFlags >> 23) & 1);
}

// top_module フラグを下ろす．
void
CptModule::clear_topmodule() const
{
  mFlags &= ~(1 << 23);
}

// @brief in_use フラグの設定
void
CptModule::set_in_use() const
{
  mFlags |= (1 << 24);
}

// @brief in_use フラグの解除
void
CptModule::reset_in_use() const
{
  mFlags &= ~(1 << 24);
}

// @brief in_use フラグの取得
bool
CptModule::is_in_use() const
{
  return static_cast<bool>((mFlags >> 24) & 1);
}

// portfaults の状態を返す．
// true で enable_portfaults を表す．
bool
CptModule::portfaults() const
{
  return static_cast<bool>((mFlags >> 25) & 1);
}

// suppress_faults の状態を返す．
// true で suppress_faults が効いている．
bool
CptModule::suppress_faults() const
{
  return static_cast<bool>((mFlags >> 26) & 1);
} 
  
// config 情報を返す．
const string&
CptModule::config() const
{
  return mConfig;
}

// library 情報を返す．
const string&
CptModule::library() const
{
  return mLibrary;
}
  
// cell 情報を返す．
const string&
CptModule::cell() const
{
  return mCell;
}

// 関数名から関数を検索する．
// なければ NULL を返す．
const PtItem*
CptModule::find_function(const char* name) const
{
  hash_map<string, const PtItem*>::const_iterator p
    = mFuncDic.find(name);
  if ( p == mFuncDic.end() ) {
    return NULL;
  }
  return p->second;
}


//////////////////////////////////////////////////////////////////////
// port を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptPort::CptPort(const FileRegion& file_region,
		 const char* ext_name) :
  mFileRegion(file_region),
  mExtName(ext_name)
{
}

// デストラクタ
CptPort::~CptPort()
{
}

// ファイル位置を返す．
FileRegion
CptPort::file_region() const
{
  return mFileRegion;
}
  
// 外向の名前(本当のポート名)を取出す
// 無い場合は NULL を返す
const char*
CptPort::ext_name() const
{
  return mExtName;
}

// 内部のポート結線リストのサイズを取り出す．
ymuint32
CptPort::portref_num() const
{
  return 0;
}

// 内部のポート結線の先頭を取り出す．
const PtPortRef*
CptPort::portref(ymuint32 pos) const
{
  return NULL;
}

// @brief portref を得る．
PtiPortRef*
CptPort::_portref(ymuint32 pos)
{
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// port を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptPort1::CptPort1(const FileRegion& file_region,
		   PtiPortRef* portref,
		   const char* ext_name) :
  CptPort(file_region, ext_name),
  mPortRef(portref)
{
}

// デストラクタ
CptPort1::~CptPort1()
{
}

// 内部のポート結線リストのサイズを取り出す．
ymuint32
CptPort1::portref_num() const
{
  return 1;
}

// 内部のポート結線の先頭を取り出す．
const PtPortRef*
CptPort1::portref(ymuint32 /* pos */) const
{
  return mPortRef;
}

// @brief portref を得る．
PtiPortRef*
CptPort1::_portref(ymuint32 /* pos */)
{
  return mPortRef;
}


//////////////////////////////////////////////////////////////////////
// port を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptPort2::CptPort2(const FileRegion& file_region,
		   PtiPortRefArray portref_array,
		   const char* ext_name) :
  CptPort(file_region, ext_name),
  mPortRefArray(portref_array)
{
}

// デストラクタ
CptPort2::~CptPort2()
{
}

// 内部のポート結線リストのサイズを取り出す．
ymuint32
CptPort2::portref_num() const
{
  return mPortRefArray.size();
}

// 内部のポート結線を取り出す．
const PtPortRef*
CptPort2::portref(ymuint32 pos) const
{
  return mPortRefArray[pos];
}

// @brief portref を得る．
PtiPortRef*
CptPort2::_portref(ymuint32 pos)
{
  return mPortRefArray[pos];
}


//////////////////////////////////////////////////////////////////////
// port reference のベース実装クラス
//////////////////////////////////////////////////////////////////////
  
// コンストラクタ
CptPortRef::CptPortRef(const FileRegion& file_region,
		       const char* name) :
  mFileRegion(file_region),
  mName(name)
{
}
  
// デストラクタ
CptPortRef::~CptPortRef()
{
}

// ファイル位置を返す．
FileRegion
CptPortRef::file_region() const
{
  return mFileRegion;
}
  
// 名前を取り出す．
const char*
CptPortRef::name() const
{
  return mName;
}
  
// @brief インデックスの取得
// @return インデックスを表す式
// このクラスでは NULL を返す．
const PtExpr*
CptPortRef::index() const
{
  return NULL;
}

// @brief 範囲指定モードの取得
// @retval kVpiNoRange 範囲指定なし
// @retval kVpiConstRange [ a : b ] のタイプ
// @retval kVpiPlusRange  [ a :+ b ] のタイプ
// @retval kVpiMinusRange [ a :- b ] のタイプ
// このクラスでは kVpiNoRange を返す．
tVpiRangeMode
CptPortRef::range_mode() const
{
  return kVpiNoRange;
}
  
// @brief 範囲の左側の式の取得
// @return 範囲の左側の式
// このクラスでは NULL を返す．
const PtExpr*
CptPortRef::left_range() const
{
  return NULL;
}

// @brief 範囲の右側の式の取得
// @return 範囲の右側の式
// このクラスでは NULL を返す．
const PtExpr*
CptPortRef::right_range() const
{
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// インデックスつきの port reference を表すクラス
//////////////////////////////////////////////////////////////////////
  
// コンストラクタ
CptPortRefI::CptPortRefI(const FileRegion& file_region,
			 const char* name,
			 PtExpr* index) :
  CptPortRef(file_region, name),
  mIndex(index)
{
}
  
// デストラクタ
CptPortRefI::~CptPortRefI()
{
}
  
// インデックスを返す．
const PtExpr*
CptPortRefI::index() const
{
  return mIndex;
}


//////////////////////////////////////////////////////////////////////
// 範囲指定つきの port reference を表すクラス
//////////////////////////////////////////////////////////////////////
  
// コンストラクタ
CptPortRefR::CptPortRefR(const FileRegion& file_region,
			 const char* name,
			 tVpiRangeMode mode,
			 PtExpr* left,
			 PtExpr* right) :
  CptPortRef(file_region, name),
  mMode(mode),
  mLeftRange(left),
  mRightRange(right)
{
}
  
// デストラクタ
CptPortRefR::~CptPortRefR()
{
}

// range のモードを取り出す．
tVpiRangeMode
CptPortRefR::range_mode() const
{
  return mMode;
}   
  
// range の MSB を取り出す．
const PtExpr*
CptPortRefR::left_range() const
{
  return mLeftRange;
}
  
// range の LSB を取り出す．
const PtExpr*
CptPortRefR::right_range() const
{
  return mRightRange;
}

  
//////////////////////////////////////////////////////////////////////
// モジュール関係
//////////////////////////////////////////////////////////////////////

// モジュールの生成
// @param file_region ファイル位置の情報
// @param name モジュール名
// @param macro macromodule の時 true となるフラグ
// @param is_cell `celldefine - `endcelldefine で囲まれていたときに true となるフラグ
// @param is_protected 保護されているときに true となるフラグ．
// verilog の構文にこんな情報はない．
// @param time_unit 時刻の単位を表す数値．
// @param time_precision 時刻の精度を表す数値
// 有効な値は 2 - -15 で 2 の時に 100秒を表す．
// 以下，1減るごとに10分の1になる．-16 で未定義を表す．
// @param net_type 未定義の識別子から暗黙のネットを生成するときのネット型．
// @param unconn unconnected_drive の値．
// @param delay delay_mode の値．
// @param decay default_decay_time の値．
// 意味のある値は正の整数もしくは無限大をあらわす -1
// @param explicit_name ポートがすべて名前付きのときに true となるフラグ
// @param portfaults Verifault 用．
// @param suppress_faults Verifault 用
// @param config config 情報
// @param library library 情報
// @param cell cell 情報
// @param param_port_array パラメータポートのリスト
// @param port_array ポートのリスト
// @param iohead_array 入出力のリスト
// @param paramhead_array parameter のリスト
// @param lparamhead_array localparameter のリスト
// @param declhead_array 宣言のリスト
// @param item_array 要素のリスト
// @return 生成されたモジュール
// paramport_array の内容と paramhead_array の内容は重複しない．
PtModule*
CptFactory::new_Module(const FileRegion& file_region,
		       const char* name,
		       bool macro,
		       bool is_cell,
		       bool is_protected,
		       int time_unit,
		       int time_precision,
		       tVpiNetType net_type,
		       tVpiUnconnDrive unconn,
		       tVpiDefDelayMode delay,
		       int decay,
		       bool explicit_name,
		       bool portfaults,
		       bool suppress_faults,
		       const string& config,
		       const string& library,
		       const string& cell,
		       PtDeclHeadArray paramport_array,
		       PtiPortArray port_array,
		       PtIOHeadArray iohead_array,
		       PtDeclHeadArray paramhead_array,
		       PtDeclHeadArray lparamhead_array,
		       PtDeclHeadArray declhead_array,
		       PtItemArray item_array)
{
  ++ mNumModule;
  void* p = alloc().get_memory(sizeof(CptModule));
  return new (p) CptModule(file_region, name,
			   macro, is_cell, is_protected,
			   time_unit, time_precision,
			   net_type, unconn,
			   delay, decay,
			   explicit_name,
			   portfaults, suppress_faults,
			   config, library, cell,
			   paramport_array,
			   port_array,
			   iohead_array,
			   paramhead_array,
			   lparamhead_array,
			   declhead_array,
			   item_array);
}

  
//////////////////////////////////////////////////////////////////////
// ポート関係
//////////////////////////////////////////////////////////////////////

// @brief ポートの生成
// @param[in] file_region ファイル位置の情報
// @param[in] ext_name ポート名 (空文字列の場合もある)
// @return 生成されたポート
PtiPort*
CptFactory::new_Port(const FileRegion& file_region,
		     const char* ext_name)
{
  ++ mNumPort;
  void* p = alloc().get_memory(sizeof(CptPort));
  return new (p) CptPort(file_region, ext_name);
}

// ポートの生成
// @param file_region ファイル位置の情報
// @param portref ポートに接続している式 (ポート参照式)
// @param ext_name ポート名 (空文字列の場合もある)
// @return 生成されたポート
PtiPort*
CptFactory::new_Port(const FileRegion& file_region,
		     PtiPortRef* portref,
		     const char* ext_name)
{
  ++ mNumPort;
  void* p = alloc().get_memory(sizeof(CptPort1));
  return new (p) CptPort1(file_region, portref, ext_name);
}

// ポートの生成
// @param file_region ファイル位置の情報
// @param portref_list ポートに接続している式 (ポート参照式) のリスト
// @param ext_name ポート名 (空文字列の場合もある)
// @return 生成されたポート
PtiPort*
CptFactory::new_Port(const FileRegion& file_region,
		     PtiPortRefArray portref_array,
		     const char* ext_name)
{
  ++ mNumPort;
  void* p = alloc().get_memory(sizeof(CptPort2));
  return new (p) CptPort2(file_region, portref_array, ext_name);
}

// ポート参照式の生成
// @param file_region ファイル位置の情報
// @param name ポートに接続している内部の識別子名
// @return 生成されたポート参照式
PtiPortRef*
CptFactory::new_PortRef(const FileRegion& file_region,
			const char* name)
{
  ++ mNumPortRef;
  void* p = alloc().get_memory(sizeof(CptPortRef));
  return new (p) CptPortRef(file_region, name);
}

// ビット指定つきポート参照式の生成
// @param file_region ファイル位置の情報
// @param name ポートに接続している内部の識別子名
// @param index ビット指定用の式
// @return 生成されたポート参照式
PtiPortRef*
CptFactory::new_PortRef(const FileRegion& file_region,
			const char* name,
			PtExpr* index)
{
  ++ mNumPortRefI;
  void* p = alloc().get_memory(sizeof(CptPortRefI));
  return new (p) CptPortRefI(file_region, name, index);
}

// 範囲指定付きポート参照式の生成
// @param file_region ファイル位置の情報
// @param name ポートに接続している内部の識別子名
// @param range_mode 範囲指定のモード
// @param left 範囲指定の左側の式
// @param right 範囲指摘の右側の式
// @return 生成されたポート参照式
PtiPortRef*
CptFactory::new_PortRef(const FileRegion& file_region,
			const char* name,
			tVpiRangeMode range_mode,
			PtExpr* left,
			PtExpr* right)
{
  ++ mNumPortRefR;
  void* p = alloc().get_memory(sizeof(CptPortRefR));
  return new (p) CptPortRefR(file_region, name, range_mode, left, right);
}

END_NAMESPACE_YM_VERILOG
