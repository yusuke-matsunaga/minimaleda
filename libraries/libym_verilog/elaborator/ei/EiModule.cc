
/// @file libym_verilog/elb_impl/EiModule.cc
/// @brief EiModule の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiModule.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiModule.h"
#include "EiPort.h"
#include "EiIODecl.h"
#include "EiExpr.h"

#include "ym_verilog/pt/PtModule.h"
#include "ym_verilog/pt/PtDecl.h"
#include "ym_verilog/pt/PtItem.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief module を生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_module モジュールテンプレート
// @param[in] pt_head パース木のヘッダ定義
// @param[in] pt_inst パース木のインスタンス定義
ElbModule*
EiFactory::new_Module(const VlNamedObj* parent,
		      const PtModule* pt_module,
		      const PtItem* pt_head,
		      const PtInst* pt_inst)
{
  void* p = mAlloc.get_memory(sizeof(EiModule2));
  EiModule2* module = new (p) EiModule2(parent,
					pt_module,
					pt_head,
					pt_inst);

  ymuint32 port_num = pt_module->port_num();
  void* q = mAlloc.get_memory(sizeof(EiPort) * port_num);
  EiPort* port_array = new (q) EiPort[port_num];
  
  ymuint32 io_num = pt_module->iodecl_num();
  void* r = mAlloc.get_memory(sizeof(EiIODecl) * io_num);
  EiIODecl* io_array = new (r) EiIODecl[io_num];
  
  module->init(port_array, io_array);

  return module;
}

// @brief module array を生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_module モジュールテンプレート
// @param[in] pt_head パース木のヘッダ定義
// @param[in] pt_inst パース木のインスタンス定義
// @param[in] left 範囲の MSB の式
// @param[in] right 範囲の LSB の式
// @param[in] left_val 範囲の MSB の値
// @param[in] right_val 範囲の LSB の値
ElbModuleArray*
EiFactory::new_ModuleArray(const VlNamedObj* parent,
			   const PtModule* pt_module,
			   const PtItem* pt_head,
			   const PtInst* pt_inst,
			   ElbExpr* left,
			   ElbExpr* right,
			   int left_val,
			   int right_val)
{
  EiRangeImpl range;
  range.set(left, right, left_val, right_val);
  
  ymuint32 n = range.size();
  void* q = mAlloc.get_memory(sizeof(EiModule1) * n);
  EiModule1* array = new (q) EiModule1[n];
  
  void* p = mAlloc.get_memory(sizeof(EiModuleArray));
  EiModuleArray* module_array = new (p) EiModuleArray(parent,
						      pt_module,
						      pt_head,
						      pt_inst,
						      range,
						      array);

  ymuint32 port_num = pt_module->port_num();
  ymuint32 io_num = pt_module->iodecl_num();
  
  for (ymuint32 i = 0; i < n; ++ i) {
    void* r = mAlloc.get_memory(sizeof(EiPort) * port_num);
    EiPort* port_array = new (r) EiPort[port_num];
    
    void* s = mAlloc.get_memory(sizeof(EiIODecl) * io_num);
    EiIODecl* io_array = new (s) EiIODecl[io_num];
    
    int index = module_array->mRange.index(i);
    array[i].init(port_array, io_array,
		  module_array, index);
  }
  
  return module_array;
}


//////////////////////////////////////////////////////////////////////
// クラス EiModuleHead
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] pt_module モジュールテンプレート
// @param[in] pt_head パース木のヘッダ定義
// @param[in] pt_inst インスタンス定義
EiModuleHead::EiModuleHead(const VlNamedObj* parent,
			   const PtModule* pt_module,
			   const PtItem* pt_head,
			   const PtInst* pt_inst) :
  mParent(parent),
  mPtModule(pt_module),
  mPtHead(pt_head),
  mPtInst(pt_inst)
{
}

// @brief デストラクタ
EiModuleHead::~EiModuleHead()
{
}

// @brief このオブジェクトの属しているスコープを返す．
const VlNamedObj*
EiModuleHead::parent() const
{
  return mParent;
}

// @brief ファイル位置の取得
FileRegion
EiModuleHead::file_region() const
{
  if ( mPtInst ) {
    return mPtInst->file_region();
  }
  else {
    return mPtModule->file_region();
  }
}
  
// @brief インスタンス名を返す．
const char*
EiModuleHead::name() const
{
  if ( mPtInst ) {
    return mPtInst->name();
  }
  else {
    return mPtModule->name();
  }
}
  
// @brief definition location を返す．
// @return 定義側のファイル位置の情報を返す．
FileRegion
EiModuleHead::def_file_region() const
{
  return mPtModule->file_region();
}
  
// @brief definition name を返す．
// @return 定義名を返す．
const char*
EiModuleHead::def_name() const
{
  return mPtModule->name();
}

// @brief ポート数を返す．
ymuint32
EiModuleHead::port_num() const
{
  return mPtModule->port_num();
}
  
/// @brief 入出力宣言数を返す．
ymuint32
EiModuleHead::io_num() const
{
  return mPtModule->iodecl_num();
}
  
// @brief cell instance のチェック
// @return cell instance の場合に true を返す．
bool
EiModuleHead::is_cell_instance() const
{
  return mPtModule->is_cell();
}

// @brief protect のチェック
// @return protect されていたら true を返す．
bool
EiModuleHead::is_protected() const
{
  return mPtModule->is_protected();
}

// @brief top module の時 true を返す．
bool
EiModuleHead::is_top_module() const
{
  return mPtInst == NULL;
}
  
// @brief time unit を返す．
// @return 結果は 2 〜 -15 の整数
// @return もしくは未定義を表す -16
int
EiModuleHead::time_unit() const
{
  return mPtModule->time_unit();
}
  
// @brief time precision を返す．
// @return 結果は 2 〜 -15 の整数
// @return もしくは未定義を表す -16
int
EiModuleHead::time_precision() const
{
  return mPtModule->time_precision();
}
  
// @brief default net type を返す．
tVpiNetType
EiModuleHead::def_net_type() const
{
  return mPtModule->nettype();
}
  
// @brief unconnected drive を返す．
tVpiUnconnDrive
EiModuleHead::unconn_drive() const
{
  return mPtModule->unconn_drive();
}
  
// @brief default delay mode を返す．
tVpiDefDelayMode
EiModuleHead::def_delay_mode() const
{
  return mPtModule->delay_mode();
}
  
// @brief default decay time を返す．
int
EiModuleHead::def_decay_time() const
{
  return mPtModule->decay_time();
}
  
// @brief config 情報を返す．
string
EiModuleHead::config() const
{
  return mPtModule->config();
}
  
// @brief library 情報を返す．
string
EiModuleHead::library() const
{
  return mPtModule->library();
}
  
// @brief cell 情報を返す．
string
EiModuleHead::cell() const
{
  return mPtModule->cell();
}


//////////////////////////////////////////////////////////////////////
// クラス EiModule
//////////////////////////////////////////////////////////////////////
  
// @brief コンストラクタ
EiModule::EiModule() :
  mPortList(NULL),
  mIODeclList(NULL)
{
}

// @brief デストラクタ
EiModule::~EiModule()
{
}

// @brief ポート配列とIO配列を初期化する．
// @param[in] port_array ポート配列
// @param[in] io_array IO 配列
void
EiModule::init(EiPort* port_array,
	       EiIODecl* io_array)
{
  mPortList = port_array;
  mIODeclList = io_array;
}

// @brief 型の取得
tVpiObjType
EiModule::type() const
{
  return kVpiParamAssign;
}

// @brief ファイル位置の取得
FileRegion
EiModule::file_region() const
{
  return head().file_region();
}

// @brief このオブジェクトの属しているスコープを返す．
const VlNamedObj*
EiModule::parent() const
{
  return head().parent();
}
  
// @brief definition location を返す．
// @return 定義側のファイル位置の情報を返す．
FileRegion
EiModule::def_file_region() const
{
  return head().def_file_region();
}
  
// @brief definition name を返す．
// @return 定義名を返す．
const char*
EiModule::def_name() const
{
  return head().def_name();
}

// @brief cell instance のチェック
// @return cell instance の場合に true を返す．
bool
EiModule::is_cell_instance() const
{
  return head().is_cell_instance();
}
  
// @brief protect のチェック
// @return protect されていたら true を返す．
bool
EiModule::is_protected() const
{
  return head().is_protected();
}

// @brief top module の時 true を返す．
bool
EiModule::is_top_module() const
{
  return head().is_top_module();
}
  
// @brief time unit を返す．
// @return 結果は 2 〜 -15 の整数
// @return もしくは未定義を表す -16
int
EiModule::time_unit() const
{
  return head().time_unit();
}
  
// @brief time precision を返す．
// @return 結果は 2 〜 -15 の整数
// @return もしくは未定義を表す -16
int
EiModule::time_precision() const
{
  return head().time_precision();
}
  
// @brief default net type を返す．
tVpiNetType
EiModule::def_net_type() const
{
  return head().def_net_type();
}
  
// @brief unconnected drive を返す．
tVpiUnconnDrive
EiModule::unconn_drive() const
{
  return head().unconn_drive();
}

// @brief default delay mode を返す．
tVpiDefDelayMode
EiModule::def_delay_mode() const
{
  return head().def_delay_mode();
}
  
// @brief default decay time を返す．
int
EiModule::def_decay_time() const
{
  return head().def_decay_time();
}
  
// @brief config 情報を返す．
string
EiModule::config() const
{
  return head().config();
}
  
// @brief library 情報を返す．
string
EiModule::library() const
{
  return head().library();
}
  
// @brief cell 情報を返す．
string
EiModule::cell() const
{
  return head().cell();
}

// @brief ポート数を返す．
ymuint32
EiModule::port_num() const
{
  return head().port_num();
}

// @brief ポートの取得
// @param[in] pos 取得するポートの位置 (0 <= pos < port_num())
// @return pos 番目のポートを返す．
const VlPort*
EiModule::port(ymuint32 pos) const
{
  return &mPortList[pos];
}
  
// @brief 入出力数を得る．
ymuint32
EiModule::io_num() const
{
  return head().io_num();
}

// @brief 入出力の取得
// @param[in] pos 位置番号 ( 0 <= pos < io_num() )
const VlIODecl*
EiModule::io(ymuint32 pos) const
{
  return &mIODeclList[pos];
}
  
// @brief 入出力の初期設定を行う．
// @param[in] pos 位置番号
// @param[in] head ヘッダ
// @param[in] pt_item パース木のIO宣言要素
// @param[in] decl 対応する宣言要素
void
EiModule::init_iodecl(ymuint32 pos,
		      ElbIOHead* head,
		      const PtIOItem* pt_item,
		      ElbDecl* decl)
{
  mIODeclList[pos].init(head, pt_item, decl);
}
  
// @brief ポートの初期設定を行う．
// @param[in] index ポート番号
// @param[in] pt_port パース木のポート定義
// @param[in] low_conn 下位の接続
// @param[in] dir 向き
void
EiModule::init_port(ymuint32 index,
		    const PtPort* pt_port,
		    ElbExpr* low_conn,
		    tVpiDirection dir)
{
  mPortList[index].init(this, pt_port, index, low_conn, dir);
}

// @brief ポートの high_conn を接続する．
// @param[in] index ポート番号
// @param[in] high_conn 上位の接続の式
// @param[in] conn_by_name 名前による割り当て時に true とするフラグ
void
EiModule::set_port_high_conn(ymuint32 index,
			     ElbExpr* high_conn,
			     bool conn_by_name)
{
  mPortList[index].set_high_conn(high_conn, conn_by_name);
}


//////////////////////////////////////////////////////////////////////
// クラス EiModule1
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiModule1::EiModule1()
{
}

// @brief デストラクタ
EiModule1::~EiModule1()
{
}

// @brief 初期設定を行う．
// @param[in] port_array ポート配列
// @param[in] io_num IO 数
// @param[in] io_array IO 配列
// @param[in] module_array 親の配列
// @param[in] index 配列中のインデックス
void
EiModule1::init(EiPort* port_array,
		EiIODecl* io_array,
		EiModuleArray* module_array,
		int index)
{
  EiModule::init(port_array, io_array);
  
  mModuleArray = module_array;
  mIndex = index;
  
  // 名前の生成
  ostringstream buf;
  buf << module_array->name() << "[" << index << "]";
  mName = buf.str().c_str();
}
  
// @brief 名前の取得
const char*
EiModule1::name() const
{
  return mName.c_str();
}
  
// @brief 配列要素の時 true を返す．
bool
EiModule1::is_array() const
{
  return true;
}

// @brief インデックスの値を返す．
// @note 配列要素の時のみ意味を持つ．
int
EiModule1::index() const
{
  return mIndex;
}

// @brief 親の配列を返す．
// @note 配列要素の時のみ意味を持つ．
const VlModuleArray*
EiModule1::module_array() const
{
  return mModuleArray;
}

// @brief ヘッダ情報を返す．
const EiModuleHead&
EiModule1::head() const
{
  return mModuleArray->head();
}

// @brief ヘッダ情報を返す．
EiModuleHead&
EiModule1::head()
{
  return mModuleArray->head();
}

  
//////////////////////////////////////////////////////////////////////
// クラス EiModule2
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] pt_module モジュールテンプレート
// @param[in] pt_head パース木のヘッダ定義
// @param[in] pt_inst インスタンス定義
EiModule2::EiModule2(const VlNamedObj* parent,
		     const PtModule* pt_module,
		     const PtItem* pt_head,
		     const PtInst* pt_inst) :
  mHead(parent, pt_module, pt_head, pt_inst)
{
}
  
// デストラクタ
EiModule2::~EiModule2()
{
}

// @brief 名前の取得
const char*
EiModule2::name() const
{
  return mHead.name();
}

// @brief 配列要素の時 true を返す．
bool
EiModule2::is_array() const
{
  return false;
}

// @brief インデックスの値を返す．
// @note 配列要素の時のみ意味を持つ．
int
EiModule2::index() const
{
  return 0;
}

// @brief 親の配列を返す．
// @note 配列要素の時のみ意味を持つ．
const VlModuleArray*
EiModule2::module_array() const
{
  return NULL;
}

// @brief ヘッダ情報を返す．
const EiModuleHead&
EiModule2::head() const
{
  return mHead;
}

// @brief ヘッダ情報を返す．
EiModuleHead&
EiModule2::head()
{
  return mHead;
}


//////////////////////////////////////////////////////////////////////
/// クラス EiModuleArray
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] pt_module モジュールテンプレート
// @param[in] pt_head パース木の定義
// @param[in] pt_inst パース木のインスタンス定義
// @param[in] left 範囲の MSB
// @param[in] right 範囲の LSB
EiModuleArray::EiModuleArray(const VlNamedObj* parent,
			     const PtModule* pt_module,
			     const PtItem* pt_head,
			     const PtInst* pt_inst,
			     const EiRangeImpl& range,
			     EiModule1* array) :
  mHead(parent, pt_module, pt_head, pt_inst),
  mRange(range),
  mArray(array)
{
}

// @brief デストラクタ
EiModuleArray::~EiModuleArray()
{
}

// @brief 型の取得
tVpiObjType
EiModuleArray::type() const
{
  return kVpiModuleArray;
}

// @brief ファイル位置の取得
FileRegion
EiModuleArray::file_region() const
{
  return mHead.file_region();
}

// @brief このオブジェクトの属しているスコープを返す．
const VlNamedObj*
EiModuleArray::parent() const
{
  return mHead.parent();
}

// @brief 名前の取得
const char*
EiModuleArray::name() const
{
  return mHead.name();
}

// @brief 範囲の MSB を返す．
const VlExpr*
EiModuleArray::left_range() const
{
  return mRange.left_range();
}
  
// @brief 範囲の LSB を返す．
const VlExpr*
EiModuleArray::right_range() const
{
  return mRange.right_range();
}
  
// @brief 要素数を返す．
ymuint32
EiModuleArray::elem_num() const
{
  return mRange.size();
}

// @brief 要素を返す．
// @param[in] offset 位置番号 (0 <= offset < elem_num())
const VlModule*
EiModuleArray::elem_by_offset(ymuint32 offset) const
{
  return &mArray[offset];
}

// @brief 要素を返す．
// @param[in] index インデックス
const VlModule*
EiModuleArray::elem_by_index(int index) const
{
  ymuint32 offset = mRange.offset(index);
  return &mArray[offset];
}

// @brief ヘッダ情報を返す．
const EiModuleHead&
EiModuleArray::head() const
{
  return mHead;
}

// @brief ヘッダ情報を返す．
EiModuleHead&
EiModuleArray::head()
{
  return mHead;
}
  
// @brief 要素を返す．
ElbModule*
EiModuleArray::_module(ymuint32 offset)
{
  return &mArray[offset];
}

END_NAMESPACE_YM_VERILOG
