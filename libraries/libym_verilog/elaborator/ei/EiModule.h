#ifndef LIBYM_VERILOG_ELABORATOR_EI_EIMODULE_H
#define LIBYM_VERILOG_ELABORATOR_EI_EIMODULE_H

/// @file libym_verilog/elaborator/ei/EiModule.h
/// @brief EiModule のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiModule.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ElbModule.h"
#include "EiRange.h"
#include "ym_utils/StrBuff.h"


BEGIN_NAMESPACE_YM_VERILOG

class EiModuleArray;
class EiPort;
class EiIODecl;
class EiExpr;

//////////////////////////////////////////////////////////////////////
/// @calss EiModuleHead EiModule.h "EiModule.h"
/// @brief EiModuleArray と EiModule に共通なヘッダ部分
//////////////////////////////////////////////////////////////////////
class EiModuleHead
{
  friend class EiModuleArray;
  friend class EiModule2;

private:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_module モジュールテンプレート
  /// @param[in] pt_head パース木のヘッダ定義
  /// @param[in] pt_inst インスタンス定義
  EiModuleHead(const VlNamedObj* parent,
	       const PtModule* pt_module,
	       const PtItem* pt_head,
	       const PtInst* pt_inst);

  /// @brief デストラクタ
  ~EiModuleHead();


public:
  //////////////////////////////////////////////////////////////////////
  // EiModuleHead の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief このオブジェクトの属しているスコープを返す．
  const VlNamedObj*
  parent() const;

  /// @brief ファイル位置の取得
  FileRegion
  file_region() const;

  /// @brief インスタンス名を返す．
  const char*
  name() const;

  /// @brief definition location を返す．
  /// @return 定義側のファイル位置の情報を返す．
  FileRegion
  def_file_region() const;

  /// @brief definition name を返す．
  /// @return 定義名を返す．
  const char*
  def_name() const;

  /// @brief ポート数を返す．
  ymuint32
  port_num() const;

  /// @brief 入出力宣言数を返す．
  ymuint32
  io_num() const;

  /// @brief cell instance のチェック
  /// @return cell instance の場合に true を返す．
  bool
  is_cell_instance() const;

  /// @brief protect のチェック
  /// @return protect されていたら true を返す．
  bool
  is_protected() const;

  /// @brief top module の時 true を返す．
  bool
  is_top_module() const;

  /// @brief time unit を返す．
  /// @return 結果は 2 〜 -15 の整数
  /// @return もしくは未定義を表す -16
  int
  time_unit() const;

  /// @brief time precision を返す．
  /// @return 結果は 2 〜 -15 の整数
  /// @return もしくは未定義を表す -16
  int
  time_precision() const;

  /// @brief default net type を返す．
  tVpiNetType
  def_net_type() const;

  /// @brief unconnected drive を返す．
  tVpiUnconnDrive
  unconn_drive() const;

  /// @brief default delay mode を返す．
  tVpiDefDelayMode
  def_delay_mode() const;

  /// @brief default decay time を返す．
  int
  def_decay_time() const;

  /// @brief config 情報を返す．
  string
  config() const;

  /// @brief library 情報を返す．
  string
  library() const;

  /// @brief cell 情報を返す．
  string
  cell() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のスコープ
  const VlNamedObj* mParent;

  // パース木のモジュール定義
  const PtModule* mPtModule;

  // パース木のヘッダ定義
  const PtItem* mPtHead;

  // パース木のモジュールインスタンス定義
  const PtInst* mPtInst;

};


//////////////////////////////////////////////////////////////////////
/// @class EiModule EiModule.h "EiModule.h"
/// @brief ElbModule の実装クラスの基底クラス
//////////////////////////////////////////////////////////////////////
class EiModule :
  public ElbModule
{
protected:

  /// @brief コンストラクタ
  EiModule();

  /// @brief デストラクタ
  virtual
  ~EiModule();

  /// @brief ポート配列とIO配列を初期化する．
  /// @param[in] port_array ポート配列
  /// @param[in] io_array IO 配列
  void
  init(EiPort* port_array,
       EiIODecl* io_array);


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;

  /// @brief ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;


public:
  //////////////////////////////////////////////////////////////////////
  // VlNamedObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief このオブジェクトの属しているスコープを返す．
  virtual
  const VlNamedObj*
  parent() const;


public:
  //////////////////////////////////////////////////////////////////////
  // VlModule の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief definition location を返す．
  /// @return 定義側のファイル位置の情報を返す．
  virtual
  FileRegion
  def_file_region() const;

  /// @brief definition name を返す．
  /// @return 定義名を返す．
  virtual
  const char*
  def_name() const;

  /// @brief cell instance のチェック
  /// @return cell instance の場合に true を返す．
  virtual
  bool
  is_cell_instance() const;

  /// @brief protect のチェック
  /// @return protect されていたら true を返す．
  virtual
  bool
  is_protected() const;

  /// @brief top module の時 true を返す．
  virtual
  bool
  is_top_module() const;

  /// @brief time unit を返す．
  /// @return 結果は 2 〜 -15 の整数
  /// @return もしくは未定義を表す -16
  virtual
  int
  time_unit() const;

  /// @brief time precision を返す．
  /// @return 結果は 2 〜 -15 の整数
  /// @return もしくは未定義を表す -16
  virtual
  int
  time_precision() const;

  /// @brief default net type を返す．
  virtual
  tVpiNetType
  def_net_type() const;

  /// @brief unconnected drive を返す．
  virtual
  tVpiUnconnDrive
  unconn_drive() const;

  /// @brief default delay mode を返す．
  virtual
  tVpiDefDelayMode
  def_delay_mode() const;

  /// @brief default decay time を返す．
  virtual
  int
  def_decay_time() const;

  /// @brief config 情報を返す．
  virtual
  string
  config() const;

  /// @brief library 情報を返す．
  virtual
  string
  library() const;

  /// @brief cell 情報を返す．
  virtual
  string
  cell() const;

  /// @brief ポート数を返す．
  virtual
  ymuint32
  port_num() const;

  /// @brief ポートの取得
  /// @param[in] pos 取得するポートの位置 (0 <= pos < port_num())
  /// @return pos 番目のポートを返す．
  virtual
  const VlPort*
  port(ymuint32 pos) const;

  /// @brief 入出力数を得る．
  virtual
  ymuint32
  io_num() const;

  /// @brief 入出力の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < io_num() )
  virtual
  const VlIODecl*
  io(ymuint32 pos) const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbModule の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入出力の初期設定を行う．
  /// @param[in] pos 位置番号
  /// @param[in] head ヘッダ
  /// @param[in] pt_item パース木のIO宣言要素
  /// @param[in] decl 対応する宣言要素
  /// @param[in] 符号付き属性の補正値
  virtual
  void
  init_iodecl(ymuint32 pos,
	      ElbIOHead* head,
	      const PtIOItem* pt_item,
	      ElbDecl* decl);

  /// @brief ポートの初期設定を行う．
  /// @param[in] index ポート番号
  /// @param[in] pt_port パース木のポート定義
  /// @param[in] low_conn 下位の接続
  /// @param[in] dir 向き
  virtual
  void
  init_port(ymuint32 index,
	    const PtPort* pt_port,
	    ElbExpr* low_conn,
	    tVpiDirection dir);

  /// @brief ポートの high_conn を接続する．
  /// @param[in] index ポート番号
  /// @param[in] high_conn 上位の接続の式
  /// @param[in] conn_by_name 名前による割り当て時に true とするフラグ
  virtual
  void
  set_port_high_conn(ymuint32 index,
		     ElbExpr* high_conn,
		     bool conn_by_name);


private:
  //////////////////////////////////////////////////////////////////////
  // EiModule の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ヘッダ情報を返す．
  virtual
  const EiModuleHead&
  head() const = 0;

  /// @brief ヘッダ情報を返す．
  virtual
  EiModuleHead&
  head() = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ポートの配列
  EiPort* mPortList;

  // 入出力の配列
  EiIODecl* mIODeclList;

};


//////////////////////////////////////////////////////////////////////
/// @class EiModule1 EiModule.h "EiModule.h"
/// @brief 配列要素の EiModule
//////////////////////////////////////////////////////////////////////
class EiModule1 :
  public EiModule
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  EiModule1();

  /// @brief デストラクタ
  virtual
  ~EiModule1();


private:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期設定を行う．
  /// @param[in] port_array ポート配列
  /// @param[in] io_array IO 配列
  /// @param[in] module_array 親の配列
  /// @param[in] index 配列中のインデックス
  void
  init(EiPort* port_array,
       EiIODecl* io_array,
       EiModuleArray* module_array,
       int index);


public:
  //////////////////////////////////////////////////////////////////////
  // VlNamedObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  virtual
  const char*
  name() const;


public:
  //////////////////////////////////////////////////////////////////////
  // VlModule の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 配列要素の時 true を返す．
  virtual
  bool
  is_array() const;

  /// @brief インデックスの値を返す．
  /// @note 配列要素の時のみ意味を持つ．
  virtual
  int
  index() const;

  /// @brief 親の配列を返す．
  /// @note 配列要素の時のみ意味を持つ．
  virtual
  const VlModuleArray*
  module_array() const;


private:
  //////////////////////////////////////////////////////////////////////
  // EiModule の派生クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ヘッダ情報を返す．
  virtual
  const EiModuleHead&
  head() const;

  /// @brief ヘッダ情報を返す．
  virtual
  EiModuleHead&
  head();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親の配列
  EiModuleArray* mModuleArray;

  // インデックス
  int mIndex;

  // 名前
  StrBuff mName;

};


//////////////////////////////////////////////////////////////////////
/// @class EiModule2 EiModule.h "EiModule.h"
/// @brief 単独要素の EiModule
//////////////////////////////////////////////////////////////////////
class EiModule2 :
  public EiModule
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_module モジュールテンプレート
  /// @param[in] pt_head パース木のヘッダ定義
  /// @param[in] pt_inst パース木のインスタンス定義
  EiModule2(const VlNamedObj* parent,
	    const PtModule* pt_module,
	    const PtItem* pt_head,
	    const PtInst* pt_inst);

  /// @brief デストラクタ
  virtual
  ~EiModule2();


public:
  //////////////////////////////////////////////////////////////////////
  // VlNamedObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  virtual
  const char*
  name() const;


public:
  //////////////////////////////////////////////////////////////////////
  // VlModule の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 配列要素の時 true を返す．
  virtual
  bool
  is_array() const;

  /// @brief インデックスの値を返す．
  /// @note 配列要素の時のみ意味を持つ．
  virtual
  int
  index() const;

  /// @brief 親の配列を返す．
  /// @note 配列要素の時のみ意味を持つ．
  virtual
  const VlModuleArray*
  module_array() const;


private:
  //////////////////////////////////////////////////////////////////////
  // EiModule の派生クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ヘッダ情報を返す．
  virtual
  const EiModuleHead&
  head() const;

  /// @brief ヘッダ情報を返す．
  virtual
  EiModuleHead&
  head();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ヘッダ情報
  EiModuleHead mHead;

};


//////////////////////////////////////////////////////////////////////
/// @class EiModuleArray EiModule.h "EiModule.h"
/// @brief ElbModuleArray の実装クラス
//////////////////////////////////////////////////////////////////////
class EiModuleArray :
  public ElbModuleArray
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_module モジュールテンプレート
  /// @param[in] pt_head パース木のヘッダ定義
  /// @param[in] pt_inst パース木のインスタンス定義
  /// @param[in] range 範囲
  /// @param[in] array モジュール配列
  EiModuleArray(const VlNamedObj* parent,
		const PtModule* pt_module,
		const PtItem* pt_head,
		const PtInst* pt_inst,
		const EiRangeImpl& range,
		EiModule1* array);

  /// @brief デストラクタ
  virtual
  ~EiModuleArray();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;

  /// @brief ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;


public:
  //////////////////////////////////////////////////////////////////////
  // VlNamedObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief このオブジェクトの属しているスコープを返す．
  virtual
  const VlNamedObj*
  parent() const;

  /// @brief 名前の取得
  virtual
  const char*
  name() const;


public:
  //////////////////////////////////////////////////////////////////////
  // VlModuleArray の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲の MSB を返す．
  virtual
  const VlExpr*
  left_range() const;

  /// @brief 範囲の LSB を返す．
  virtual
  const VlExpr*
  right_range() const;

  /// @brief 要素数を返す．
  virtual
  ymuint32
  elem_num() const;

  /// @brief 要素を返す．
  /// @param[in] offset 位置番号 (0 <= offset < elem_num())
  virtual
  const VlModule*
  elem_by_offset(ymuint32 offset) const;

  /// @brief 要素を返す．
  /// @param[in] index インデックス
  virtual
  const VlModule*
  elem_by_index(int index) const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbModuleArray の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素を返す．
  ElbModule*
  _module(ymuint32 offset);


public:
  //////////////////////////////////////////////////////////////////////
  // EiModuleArray の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ヘッダを返す．
  const EiModuleHead&
  head() const;

  /// @brief ヘッダを返す．
  EiModuleHead&
  head();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ヘッダ
  EiModuleHead mHead;

  // 範囲
  EiRangeImpl mRange;

  // 要素の配列
  EiModule1* mArray;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELABORATOR_EI_EIMODULE_H
