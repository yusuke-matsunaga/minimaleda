#ifndef LIBYM_VERILOG_ELB_VLDUMPERIMPL_H
#define LIBYM_VERILOG_ELB_VLDUMPERIMPL_H

/// @file libym_verilog/elb/VlDumperImpl.h
/// @brief VlDumperImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlDumperImpl.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include <ym_verilog/verilog.h>
#include <ym_verilog/vl/VlFwd.h>
#include <ym_utils/FileRegion.h>


BEGIN_NAMESPACE_YM_VERILOG

class VlMgr;

//////////////////////////////////////////////////////////////////////
/// @class VlDumperImpl VlDumperImpl.h "VlDumperImpl.h"
/// @ingroup Vlgroup
/// @brief VlDumper の実装クラス
//////////////////////////////////////////////////////////////////////
class VlDumperImpl
{
public:

  /// @brief コンストラクタ
  /// @param[in] s 出力ストリーム
  VlDumperImpl(ostream& s);
  
  /// @brief デストラクタ
  ~VlDumperImpl();
  

public:
  //////////////////////////////////////////////////////////////////////
  // 内容を出力する関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief VlMgr の持っている内容を出力する
  /// @param[in] vlmgr VlMgr
  void
  put(const VlMgr& vlmgr);
  

public:
  //////////////////////////////////////////////////////////////////////
  // オプションの設定/取得用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置情報を出力するモードにする
  void
  enable_file_loc_mode();

  /// @brief ファイル位置情報の出力を抑制するモードにする．
  void
  disable_file_loc_mode();

  /// @brief ファイル位置情報を出力するモードのとき true を返す．
  bool
  file_loc_mode() const;

  /// @brief NULL ポインタの出力を抑止する．
  void
  enable_nullptr_suppress_mode();

  /// @brief NULL ポインタの出力を行う．
  void
  disable_nullptr_suppress_mode();

  /// @brief NULL ポインタの出力抑止モードの状態を得る．
  bool
  nullptr_suppress_mode() const;

  /// @brief bit 展開した内容を出力する．
  void
  enable_bitexpand_mode();

  /// @brief bit 展開しない．
  void
  disable_bitexpand_mode();

  /// @brief bit 展開モードの時 true を返す．
  bool
  bitexpand_mode() const;
  

public:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief module の内容を出力する関数
  /// @param[in] label ラベル
  /// @param[in] mgr VlMgr
  /// @param[in] module オブジェクト
  void
  put_module(const char* label,
	     const VlMgr& mgr,
	     const VlModule* module);

  /// @brief module array の内容を出力する関数
  /// @param[in] mgr VlMgr
  /// @param[in] modulearray オブジェクト
  void
  put_modulearray(const char* label,
		  const VlMgr& mgr,
		  const VlModuleArray* modulearray);
  
  /// @brief module array のリストの内容を出力する関数
  void
  put_modulearray_list(const char* label,
		       const VlMgr& mgr,
		       const vector<const VlModuleArray*>& module_array_list);
  
  /// @brief scope の内容を出力する関数
  /// @param[in] mgr VlMgr
  void
  put_scope(const char* label,
	    const VlMgr& mgr,
	    const VlNamedObj* scope);

  void
  put_scope_sub(const VlMgr& mgr,
		const VlNamedObj* scope);
  
  /// @brief io decl の内容を出力する関数
  /// @param[in] mgr VlMgr
  void
  put_iodecl(const char* label,
	     const VlMgr& mgr,
	     const VlIODecl* iodecl);

  /// @brief port の内容を出力する関数
  /// @param[in] mgr VlMgr
  void
  put_port(const char* label,
	   const VlMgr& mgr,
	   const VlPort* port);

  /// @brief 宣言要素の内容を出力する関数
  /// @param[in] mgr VlMgr
  void
  put_decl(const char* label,
	   const VlMgr& mgr,
	   const VlDecl* decl);
  
  /// @brief 宣言要素のリストの内容を出力する関数
  void
  put_decl_list(const char* label,
		const VlMgr& mgr,
		const vector<const VlDecl*>& decl_list);
  
  /// @brief def param のリストの内容を出力する関数
  void
  put_defparam_list(const char* label,
		    const VlMgr& mgr,
		    const vector<const VlDefParam*>& defparam_list);
  
  /// @brief param assign のリストの内容を出力する関数
  void
  put_paramassign_list(const char* label,
		       const VlMgr& mgr,
		       const vector<const VlParamAssign*>& paramassign_list);
  
  /// @brief primitive array のリストの内容を出力する関数
  void
  put_primarray_list(const char* label,
		     const VlMgr& mgr,
		     vector<const VlPrimArray*>& primarray_list);

  /// @brief primitive の内容を出力する関数
  void
  put_primitive(const char* label,
		const VlMgr& mgr,
		const VlPrimitive* primitive);

  /// @brief primitive のリスト内容を出力する関数
  void
  put_primitive_list(const char* label,
		     const VlMgr& mgr,
		     const vector<const VlPrimitive*>& primitive_list);

  /// @brief prim term の内容を出力する関数
  void
  put_primterm(const char* label,
	       const VlMgr& mgr,
	       const VlPrimTerm* primterm);

  /// @brief udp defn の内容を出力する関数
  void
  put_udp_defn(const char* label,
	       const VlMgr& mgr,
	       const VlUdpDefn* udp);

  /// @brief task の内容を出力する関数
  void
  put_task(const char* label,
	   const VlMgr& mgr,
	   const VlTaskFunc* task);

  /// @brief function の内容を出力する関数
  void
  put_function(const char* label,
	       const VlMgr& mgr,
	       const VlTaskFunc* taskfunc);

#if 0
  /// @brief frame の内容を出力する関数
  void
  put_frame(const char* label,
	    const VlHandle& handle);
#endif
  
  /// @brief continuous assignment の内容を出力する関数
  void
  put_contassign(const char* label,
		 const VlMgr& mgr,
		 const VlContAssign* contassign);
  
  /// @brief continuous assignment のリストの内容を出力する関数
  void
  put_contassign_list(const char* label,
		      const VlMgr& mgr,
		      const vector<const VlContAssign*>& ca_list);

  /// @brief initial/always の内容を出力する関数
  void
  put_process(const char* label,
	      const VlMgr& mgr,
	      const VlProcess* process);

  /// @brief initial/always のリストの内容を出力する関数
  void
  put_process_list(const char* label,
		   const VlMgr& mgr,
		   const vector<const VlProcess*>& process_list);

  /// @brief statement の内容を出力する関数
  void
  put_stmt(const char* label,
	   const VlMgr& mgr,
	   const VlStmt* stmt);

  /// @brief statement のリストの内容を出力する関数
  void
  put_stmt_list(const char* label,
		const VlMgr& mgr,
		const vector<const VlStmt*>& stmt_list);
  
  /// @brief delay/event/repeat control の内容を出力する関数
  void
  put_control(const char* label,
	      const VlMgr& mgr,
	      const VlControl* obj);
  
  /// @brief ブロック内の stmt リストを出力する関数
  void
  put_child_stmt_list(const char* label,
		      const VlMgr& mgr,
		      const VlStmt* stmt);
  
  /// @brief 引数のリストを出力する関数
  void
  put_argument_list(const char* label,
		    const VlMgr& mgr,
		    const VlStmt* stmt);
  
  /// @brief 式の内容を出力する関数
  void
  put_expr(const char* label,
	   const VlMgr& mgr,
	   const VlExpr* expr);

  /// @brief 遅延値を出力する関数
  void
  put_delay(const char* label,
	    const VlMgr& mgr,
	    const VlDelay* delay);
  
  /// @brief range の内容を出力する関数
  void
  put_range(const char* label,
	    const VlMgr& mgr,
	    const VlRange* obj);
  

  //////////////////////////////////////////////////////////////////////
  // その他の型
  //////////////////////////////////////////////////////////////////////

  /// @brief FileLoc の情報を出力する．
  /// @param[in] label ラベル
  /// @param[in] file_loc ファイルの位置情報
  void
  put(const char* label,
      const FileLoc& file_loc);

  /// @brief FileRegion の情報をに出力する．
  /// @param[in] label ラベル
  /// @param[in] file_region ファイル領域の情報
  void
  put(const char* label,
      const FileRegion& file_region);

  /// @brief direction を出力する関数
  /// @param[in] label ラベル
  /// @param[in] direction 向き
  void
  put(const char* label,
      tVpiDirection direction);

  /// @brief const type を出力する関数
  /// @param[in] label ラベル
  /// @param[in] const_type 定数の型
  void
  put(const char* label,
      tVpiConstType const_type);

  /// @brief prim type を出力する関数
  /// @param[in] label ラベル
  /// @param[in] prim_type プリミティブの型
  void
  put(const char* label,
      tVpiPrimType prim_type);

  /// @brief func type の出力
  /// @param[in] label ラベル
  /// @param[in] func_type
  void
  put(const char* label,
      tVpiFuncType func_type);

  /// @brief op type の出力
  /// @param[in] label ラベル
  /// @param[in] op_type 演算子の種類
  void
  put(const char* label,
      tVpiOpType op_type);

  /// @brief strength を出力する関数
  /// @param[in] label ラベル
  /// @param[in] str 信号強度
  /// @param[in] put_on_null true の時, 空でも出力する．
  void
  put(const char* label,
      tVpiStrength str,
      bool put_on_null = true);

  /// @brief unconnected drive の出力
  /// @param[in] label ラベル
  /// @param[in] ud unconnected drive
  void
  put(const char* label,
      tVpiUnconnDrive ud);

  /// @brief default delay mode の出力
  /// @param[in] label ラベル
  /// @param[in] delay_mode default delay mode
  void
  put(const char* label,
      tVpiDefDelayMode delay_mode);

  /// @brief ネット型の出力
  /// @param[in] label ラベル
  /// @param[in] net_type ネット型
  void
  put(const char* label,
      tVpiNetType net_type);

  /// @brief ブール値の出力
  /// @param[in] label ラベル
  /// @param[in] b ブール値
  void
  put(const char* label,
      bool b);

  /// @brief 整数値の出力
  /// @param[in] label ラベル
  /// @param[in] d 整数値
  void
  put(const char* label,
      int d);

  /// @brief 整数値の出力
  /// @param[in] label ラベル
  /// @param[in] d 整数値
  void
  put(const char* label,
      ymuint32 d);

  /// @brief 文字列の出力
  /// @param[in] label ラベル
  /// @param[in] str 文字列
  void
  put(const char* label,
      const string& str);

  /// @brief 文字列の出力
  /// @param[in] label ラベル
  /// @param[in] str 文字列
  void
  put(const char* label,
      const char* str);

  /// @brief 開始マーカを出力する．
  /// @param[in] label ラベル
  /// @param[in] type 属性
  /// @param[in] need_cr マーカ出力直後に改行するときに true とするフラグ
  void
  begin_marker(const char* label,
	       const string& type,
	       bool need_cr = true);

  /// @brief 終了マーカを出力する．
  /// @param[in] label ラベル
  void
  end_marker(const char* label);

  // 改行が必要なら改行する．
  void
  cr(bool force = false);

  /// @brief file_loc の親のファイル情報を出力する．
  /// @param[in] file_loc ファイル位置の情報
  void
  put_parent_file(const FileLoc& file_loc);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力ストリーム
  ostream& mStream;

  // マーカ出力後に改行していないことを示すフラグ
  list<bool> mDoCR;
  
  // インデント量
  int mIndent;

  // ファイル位置情報の出力フラグ
  bool mFileLocMode;

  // NULL ポインタ出力抑止フラグ
  bool mNullptrSuppressMode;

  // bit 展開フラグ
  bool mBitExpandMode;

};


//////////////////////////////////////////////////////////////////////
/// マーカを出力するクラス
//////////////////////////////////////////////////////////////////////
class VlDumpHeader
{
public:

  /// @brief コンストラクタ
  /// @param[in] dumper 出力先
  /// @param[in] label ラベル
  /// @param[in] type 属性
  /// @param[in] need_cr true の時マーカ出力直後に改行する
  VlDumpHeader(VlDumperImpl* dumper,
	       const char* label,
	       const char* type,
	       bool need_cr = true);

  /// @brief デストラクタ
  ~VlDumpHeader();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力先のストリーム
  VlDumperImpl* mDumper;

  // マーカー名
  const char* mName;

};


END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELB_VLDUMPERIMPL_H
