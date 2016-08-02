#ifndef YM_VERILOG_VPI_VPIDUMPER_H
#define YM_VERILOG_VPI_VPIDUMPER_H

/// @file ym_verilog/vpi/VpiDumper.h
/// @brief VpiDumper のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VpiDumper.h 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#include <ym_verilog/verilog.h>
#include <ym_utils/FileRegion.h>


BEGIN_NAMESPACE_YM_VERILOG

class VpiMgr;
class VpiHeader;
class VpiHandle;


//////////////////////////////////////////////////////////////////////
/// @class VpiDumper VpiDumper.h <ym_verilog/vpi/VpiDumper.h>
/// @ingroup VlElb
/// @brief VpiObj の出力用クラス
//////////////////////////////////////////////////////////////////////
class VpiDumper
{
  friend class VpiHeader;
public:

  /// @brief コンストラクタ
  /// @param[in] s 出力ストリーム
  VpiDumper(ostream& s);


public:

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

  /// @brief VpiMgr の持っている内容を出力する
  /// @param[in] vpimgr VpiMgr
  void
  put(const VpiMgr& vpimgr);

  /// @brief VpiHandle を出力する．
  /// @param[in] label ラベル
  /// @param[in] handle オブジェクトハンドル
  void
  put(const char* label,
      const VpiHandle& handle);

  /// @brief ハンドルの prop という文字属性を出力する．
  /// @param[in] label ラベル
  /// @param[in] handle オブジェクトハンドル
  /// @param[in] prop 属性
  void
  put_str(const char* label,
	  const VpiHandle& handle,
	  int prop);
  
  /// @brief module の内容を出力する関数
  /// @param[in] label ラベル
  /// @param[in] handle オブジェクトハンドル
  void
  put_module(const char* label,
	     const VpiHandle& handle);

  /// @brief module array の内容を出力する関数
  void
  put_module_array(const char* label,
		   const VpiHandle& handle);

  /// @brief scope の内容を出力する関数
  void
  put_scope(const char* label,
	    const VpiHandle& handle);

  /// @brief primitive array の内容を出力する関数
  void
  put_prim_array(const char* label,
		 const VpiHandle& handle);

  /// @brief named begin の内容を出力する関数
  void
  put_named_begin(const char* label,
		  const VpiHandle& handle);

  /// @brief named fork の内容を出力する関数
  void
  put_named_fork(const char* label,
		 const VpiHandle& handle);

  /// @brief io decl の内容を出力する関数
  void
  put_io_decl(const char* label,
	      const VpiHandle& handle);

  /// @brief port の内容を出力する関数
  void
  put_port(const char* label,
	   const VpiHandle& handle);

  /// @brief port bit の内容を出力する関数
  void
  put_port_bit(const char* label,
	       const VpiHandle& handle);

  /// @brief net の内容を出力する関数
  void
  put_net(const char* label,
	  const VpiHandle& handle);
  
  /// @brief net bit の内容を出力する関数
  void
  put_net_bit(const char* label,
	      const VpiHandle& handle);

  /// @brief net array の内容を出力する関数
  void
  put_net_array(const char* label,
		const VpiHandle& handle);

  /// @brief reg の内容を出力する関数
  void
  put_reg(const char* label,
	  const VpiHandle& handle);

  /// @brief reg bit の内容を出力する関数
  void
  put_reg_bit(const char* label,
	      const VpiHandle& handle);

  /// @brief reg array の内容を出力する関数
  void
  put_reg_array(const char* label,
		const VpiHandle& handle);

  /// @brief variables の内容を出力する関数
  void
  put_variables(const char* label,
		const VpiHandle& handle);

  /// @brief var select の内容を出力する関数
  void
  put_var_select(const char* label,
		 const VpiHandle& handle);

  /// @brief range の内容を出力する関数
  void
  put_range(const char* label,
	    const VpiHandle& handle);

  /// @brief named event の内容を出力する関数
  void
  put_named_event(const char* label,
		  const VpiHandle& handle);

  /// @brief named  event array の内容を出力する関数
  void
  put_named_event_array(const char* label,
			const VpiHandle& handle);

  /// @brief parameter の内容を出力する関数
  void
  put_parameter(const char* label,
		const VpiHandle& handle);

  /// @brief spec param の内容を出力する関数
  void
  put_spec_param(const char* label,
		 const VpiHandle& handle);

  /// @brief def param の内容を出力する関数
  void
  put_def_param(const char* label,
		const VpiHandle& handle);

  /// @brief param assign の内容を出力する関数
  void
  put_param_assign(const char* label,
		   const VpiHandle& handle);

  /// @brief primitive の内容を出力する関数
  void
  put_primitive(const char* label,
		const VpiHandle& handle);

  /// @brief prim term の内容を出力する関数
  void
  put_prim_term(const char* label,
		const VpiHandle& handle);

  /// @brief udp defn の内容を出力する関数
  void
  put_udp_defn(const char* label,
	       const VpiHandle& handle);

  /// @brief task の内容を出力する関数
  void
  put_task(const char* label,
	   const VpiHandle& handle);

  /// @brief function の内容を出力する関数
  void
  put_function(const char* label,
	       const VpiHandle& handle);

  /// @brief task call の内容を出力する関数
  void
  put_task_call(const char* label,
		const VpiHandle& handle);

  /// @brief func call の内容を出力する関数
  void
  put_func_call(const char* label,
		const VpiHandle& handle);

  /// @brief system task call の内容を出力する関数
  void
  put_sys_task_call(const char* label,
		    const VpiHandle& handle);

  /// @brief system func call の内容を出力する関数
  void
  put_sys_func_call(const char* label,
		    const VpiHandle& handle);

  /// @brief frame の内容を出力する関数
  void
  put_frame(const char* label,
	    const VpiHandle& handle);

  /// @brief continuous assignment の内容を出力する関数
  void
  put_cont_assign(const char* label,
		  const VpiHandle& handle);

  /// @brief continuous assignment bit の内容を出力する関数
  void
  put_cont_assign_bit(const char* label,
		      const VpiHandle& handle);

  /// @brief initial/always の内容を出力する関数
  void
  put_process(const char* label,
	      const VpiHandle& handle);

  /// @brief begin の内容を出力する関数
  void
  put_begin(const char* label,
	    const VpiHandle& handle);

  /// @brief fork の内容を出力する関数
  void
  put_fork(const char* label,
	   const VpiHandle& handle);

  /// @brief null stmt の内容を出力する関数
  void
  put_null_stmt(const char* label,
		const VpiHandle& handle);

  /// @brief event stmt の内容を出力する関数
  void
  put_event_stmt(const char* label,
		 const VpiHandle& handle);

  /// @brief assignment の内容を出力する関数
  void
  put_assignment(const char* label,
		 const VpiHandle& handle);

  /// @brief delay control の内容を出力する関数
  void
  put_delay_control(const char* label,
		    const VpiHandle& handle);

  /// @brief event control の内容を出力する関数
  void
  put_event_control(const char* label,
		    const VpiHandle& handle);

  /// @brief repeat control の内容を出力する関数
  void
  put_repeat_control(const char* label,
		     const VpiHandle& handle);

  /// @brief delay control statement の内容を出力する関数
  void
  put_delay_ctrl_stmt(const char* label,
		      const VpiHandle& handle);

  /// @brief event control statement の内容を出力する関数
  void
  put_event_ctrl_stmt(const char* label,
		      const VpiHandle& handle);

  /// @brief while の内容を出力する関数
  void
  put_while_stmt(const char* label,
		 const VpiHandle& handle);

  /// @brief repeat の内容を出力する関数
  void
  put_repeat_stmt(const char* label,
		  const VpiHandle& handle);

  /// @brief wait の内容を出力する関数
  void
  put_wait_stmt(const char* label,
		const VpiHandle& handle);

  /// @brief for の内容を出力する関数
  void
  put_for_stmt(const char* label,
	       const VpiHandle& handle);

  /// @brief forever の内容を出力する関数
  void
  put_forever_stmt(const char* label,
		   const VpiHandle& handle);

  /// @brief if の内容を出力する関数
  void
  put_if_stmt(const char* label,
	      const VpiHandle& handle);

  /// @brief if else の内容を出力する関数
  void
  put_if_else_stmt(const char* label,
		   const VpiHandle& handle);

  /// @brief case の内容を出力する関数
  void
  put_case_stmt(const char* label,
		const VpiHandle& handle);

  /// @brief case item の内容を出力する関数
  void
  put_case_item(const char* label,
		const VpiHandle& handle);

  /// @brief force の内容を出力する関数
  void
  put_force_stmt(const char* label,
		 const VpiHandle& handle);

  /// @brief assign stmt の内容を出力する関数
  void
  put_assign_stmt(const char* label,
		  const VpiHandle& handle);

  /// @brief deassign の内容を出力する関数
  void
  put_deassign_stmt(const char* label,
		    const VpiHandle& handle);

  /// @brief release の内容を出力する関数
  void
  put_release_stmt(const char* label,
		   const VpiHandle& handle);

  /// @brief disable の内容を出力する関数
  void
  put_disable_stmt(const char* label,
		   const VpiHandle& handle);

  /// @brief bit select の内容を出力する関数
  void
  put_bit_select(const char* label,
		 const VpiHandle& handle);

  /// @brief part select の内容を出力する関数
  void
  put_part_select(const char* label,
		  const VpiHandle& handle);

  /// @brief operation の内容を出力する関数
  void
  put_operation(const char* label,
		const VpiHandle& handle);

  /// @brief constant の内容を出力する関数
  void
  put_constant(const char* label,
	       const VpiHandle& handle);

  /// @brief iterator の内容を出力する関数
  void
  put_iterator(const char* label,
	       const VpiHandle& handle);
  

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

  /// @brief 文字列の出力
  /// @param[in] label ラベル
  /// @param[in] str 文字列
  void
  put(const char* label,
      const string& str);

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


END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_VPI_VPIDUMPER_H
