#ifndef LIBYM_VERILOG_PT_COMPACT_CPTFACTORY_H
#define LIBYM_VERILOG_PT_COMPACT_CPTFACTORY_H

/// @file libym_verilog/pt_compact/CptFactory.h
/// @brief CptFactory のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CptFactory.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "PtiFactory.h"
#include "ym_utils/Alloc.h"


BEGIN_NAMESPACE_YM_VERILOG

class CptDefStub;

//////////////////////////////////////////////////////////////////////
/// @class CptFactory CptFactory.h "CptFactory.h"
/// @brief 省メモリ版 Pt クラス(Cpt クラス)を生成するファクトリクラス
//////////////////////////////////////////////////////////////////////
class CptFactory :
  public PtiFactory
{
public:

  /// @brief コンストラクタ
  /// @param[in] alloc メモリ確保用のアロケータ
  CptFactory(AllocBase& alloc);

  /// @brief デストラクタ
  virtual
  ~CptFactory();


public:
  //////////////////////////////////////////////////////////////////////
  // UDP 関係
  //////////////////////////////////////////////////////////////////////

  /// @brief combinational UDP の生成
  virtual
  PtUdp*
  new_CmbUdp(const FileRegion& fr,
	     const char* name,
	     PtiPortArray port_array,
	     PtIOHeadArray io_array,
	     PtUdpEntryArray entry_array);

  /// @brief sequential UDP の生成
  virtual
  PtUdp*
  new_SeqUdp(const FileRegion& fr,
	     const char* name,
	     PtiPortArray port_array,
	     PtIOHeadArray io_array,
	     PtExpr* init_value,
	     PtUdpEntryArray entry_array);

  /// @brief combinational UDP 用のテーブルエントリの生成
  virtual
  PtUdpEntry*
  new_UdpEntry(const FileRegion& fr,
	       PtUdpValueArray input_array,
	       PtUdpValue* output);

  /// @brief sequential UDP 用のテーブルエントリの生成
  virtual
  PtUdpEntry*
  new_UdpEntry(const FileRegion& fr,
	       PtUdpValueArray input_array,
	       PtUdpValue* current,
	       PtUdpValue* output);

  /// @brief UDP のテーブルエントリの要素の値の生成
  virtual
  PtUdpValue*
  new_UdpValue(const FileRegion& fr,
	       tVpiUdpVal symbol);


  //////////////////////////////////////////////////////////////////////
  // モジュール関係
  //////////////////////////////////////////////////////////////////////

  /// @brief モジュールの生成
  virtual
  PtModule*
  new_Module(const FileRegion& fr,
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
	     PtDeclHeadArray localparamhead_array,
	     PtDeclHeadArray declhead_array,
	     PtItemArray item_array);


  //////////////////////////////////////////////////////////////////////
  // ポート関係
  //////////////////////////////////////////////////////////////////////

  /// @brief ポートの生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] ext_name ポート名 (空文字列の場合もある)
  /// @return 生成されたポート
  virtual
  PtiPort*
  new_Port(const FileRegion& file_region,
	   const char* ext_name = NULL);

  /// @brief ポートの生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] portref ポートに接続している式 (ポート参照式)
  /// @param[in] ext_name ポート名 (空文字列の場合もある)
  /// @return 生成されたポート
  virtual
  PtiPort*
  new_Port(const FileRegion& file_region,
	   PtiPortRef* portref,
	   const char* ext_name);

  /// @brief ポートの生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] portref_array ポートに接続している式 (ポート参照式) のリスト
  /// @param[in] ext_name ポート名 (空文字列の場合もある)
  /// @return 生成されたポート
  virtual
  PtiPort*
  new_Port(const FileRegion& file_region,
	   PtiPortRefArray portref_array,
	   const char* ext_name);

  /// @brief ポート参照式の生成
  virtual
  PtiPortRef*
  new_PortRef(const FileRegion& fr,
	      const char* name);

  /// @brief ビット指定つきポート参照式の生成
  virtual
  PtiPortRef*
  new_PortRef(const FileRegion& fr,
	      const char* name,
	      PtExpr* index);

  /// @brief 範囲指定付きポート参照式の生成
  virtual
  PtiPortRef*
  new_PortRef(const FileRegion& fr,
	      const char* name,
	      tVpiRangeMode mode,
	      PtExpr* left,
	      PtExpr* right);


  //////////////////////////////////////////////////////////////////////
  // IO 宣言関係
  //////////////////////////////////////////////////////////////////////

  /// @brief IO 宣言のヘッダの生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] type IO の種類
  /// @param[in] sign 符号付きのとき true となるフラグ
  /// @return 生成された IO宣言ヘッダ
  virtual
  PtiIOHead*
  new_IOHead(const FileRegion& fr,
	     tPtIOType type,
	     bool sign);

  /// @brief IO 宣言のヘッダの生成 (reg 型)
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] type IO の種類
  /// @param[in] sign 符号付きのとき true となるフラグ
  /// @return 生成された IO宣言ヘッダ
  virtual
  PtiIOHead*
  new_RegIOHead(const FileRegion& fr,
		tPtIOType type,
		bool sign);

  /// @brief IO 宣言のヘッダの生成 (ネット型)
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] type IO の種類
  /// @param[in] net_type 補助的なネット型
  /// @param[in] sign 符号付きのとき true となるフラグ
  /// @return 生成された IO宣言ヘッダ
  virtual
  PtiIOHead*
  new_NetIOHead(const FileRegion& fr,
		tPtIOType type,
		tVpiNetType net_type,
		bool sign);

  /// @brief IO 宣言のヘッダの生成 (変数型)
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] type IO の種類
  /// @param[in] var_type 補助的な変数型
  /// @return 生成された IO宣言ヘッダ
  virtual
  PtiIOHead*
  new_VarIOHead(const FileRegion& fr,
		tPtIOType type,
		tVpiVarType var_type);

  /// @brief 範囲付きの IO 宣言のヘッダの生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] type IO の種類
  /// @param[in] sign 符号付きのとき true となるフラグ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @return 生成された IO宣言ヘッダ
  virtual
  PtiIOHead*
  new_IOHead(const FileRegion& fr,
	     tPtIOType type,
	     bool sign,
	     PtExpr* left,
	     PtExpr* right);

  /// @brief 範囲付きの IO 宣言のヘッダの生成 (reg 型)
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] type IO の種類
  /// @param[in] sign 符号付きのとき true となるフラグ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @return 生成された IO宣言ヘッダ
  virtual
  PtiIOHead*
  new_RegIOHead(const FileRegion& fr,
		tPtIOType type,
		bool sign,
		PtExpr* left,
		PtExpr* right);

  /// @brief 範囲付きの IO 宣言のヘッダの生成 (ネット型)
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] type IO の種類
  /// @param[in] net_type 補助的なネット型
  /// @param[in] sign 符号付きのとき true となるフラグ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @return 生成された IO宣言ヘッダ
  virtual
  PtiIOHead*
  new_NetIOHead(const FileRegion& fr,
		tPtIOType type,
		tVpiNetType net_type,
		bool sign,
		PtExpr* left,
		PtExpr* right);

  /// @brief IO 宣言の要素の生成
  /// @param[in] file_region ファイル上の位置
  /// @param[in] name 名前
  virtual
  PtIOItem*
  new_IOItem(const FileRegion& file_region,
	     const char* name);

  /// @brief 初期値付き IO 宣言の要素の生成
  /// @param[in] file_region ファイル上の位置
  /// @param[in] name 名前
  /// @param[in] init_value 初期値
  virtual
  PtIOItem*
  new_IOItem(const FileRegion& file_region,
	     const char* name,
	     PtExpr* init_value);


  //////////////////////////////////////////////////////////////////////
  // その他の宣言関係
  //////////////////////////////////////////////////////////////////////

  /// @brief パラメータ宣言のヘッダの生成 (型指定なし)
  virtual
  PtiDeclHead*
  new_ParamH(const FileRegion& fr);

  /// @brief 範囲指定型パラメータ宣言のヘッダの生成
  virtual
  PtiDeclHead*
  new_ParamH(const FileRegion& fr,
	     bool sign,
	     PtExpr* left,
	     PtExpr* right);

  /// @brief 組み込み型パラメータ宣言のヘッダの生成
  virtual
  PtiDeclHead*
  new_ParamH(const FileRegion& fr,
	     tVpiVarType var_type);

  /// @brief local param 宣言のヘッダの生成 (型指定なし)
  virtual
  PtiDeclHead*
  new_LocalParamH(const FileRegion& fr);

  /// @brief 範囲指定型 local param 宣言のヘッダの生成
  virtual
  PtiDeclHead*
  new_LocalParamH(const FileRegion& fr,
		  bool sign,
		  PtExpr* left,
		  PtExpr* right);

  /// @brief 組み込み型 local param 宣言のヘッダの生成
  virtual
  PtiDeclHead*
  new_LocalParamH(const FileRegion& fr,
		  tVpiVarType var_type);

  /// @brief specparam 宣言のヘッダの生成
  virtual
  PtiDeclHead*
  new_SpecParamH(const FileRegion& fr);

  /// @brief 範囲指定型 specparam 宣言のヘッダの生成
  virtual
  PtiDeclHead*
  new_SpecParamH(const FileRegion& fr,
		 PtExpr* left,
		 PtExpr* right);

  /// @brief イベント宣言のヘッダの生成
  virtual
  PtiDeclHead*
  new_EventH(const FileRegion& fr);

  /// @brief genvar 宣言のヘッダの生成
  virtual
  PtiDeclHead*
  new_GenvarH(const FileRegion& fr);

  /// @brief 変数宣言のヘッダの生成
  virtual
  PtiDeclHead*
  new_VarH(const FileRegion& fr,
	   tVpiVarType var_type);

  /// @brief 1ビット型 reg 宣言のヘッダを生成の生成
  virtual
  PtiDeclHead*
  new_RegH(const FileRegion& fr,
	   bool sign);

  /// @brief ビットベクタ型 reg 宣言のヘッダの生成
  virtual
  PtiDeclHead*
  new_RegH(const FileRegion& fr,
	   bool sign,
	   PtExpr* left,
	   PtExpr* right);

  /// @brief 1ビット型 net 宣言のヘッダの生成
  virtual
  PtiDeclHead*
  new_NetH(const FileRegion& fr,
	   tVpiNetType type,
	   bool sign);

  /// @brief 1ビット型 net 宣言のヘッダの生成(strength あり)
  virtual
  PtiDeclHead*
  new_NetH(const FileRegion& fr,
	   tVpiNetType type,
	   bool sign,
	   PtStrength* strength);

  /// @brief 1ビット型 net 宣言のヘッダの生成 (遅延あり)
  virtual
  PtiDeclHead*
  new_NetH(const FileRegion& fr,
	   tVpiNetType type,
	   bool sign,
	   PtDelay* delay);

  /// @brief 1ビット型 net 宣言のヘッダの生成 (strength, 遅延あり)
  virtual
  PtiDeclHead*
  new_NetH(const FileRegion& fr,
	   tVpiNetType type,
	   bool sign,
	   PtStrength* strength,
	   PtDelay* delay);

  /// @brief 範囲指定型 net 宣言のヘッダの生成
  virtual
  PtiDeclHead*
  new_NetH(const FileRegion& fr,
	   tVpiNetType type,
	   tVpiVsType vstype,
	   bool sign,
	   PtExpr* left,
	   PtExpr* right);

  /// @brief 範囲指定型 net 宣言のヘッダの生成 (strengthあり)
  virtual
  PtiDeclHead*
  new_NetH(const FileRegion& fr,
	   tVpiNetType type,
	   tVpiVsType vstype,
	   bool sign,
	   PtExpr* left,
	   PtExpr* right,
	   PtStrength* strength);

  /// @brief 範囲指定型 net 宣言のヘッダの生成 (遅延あり)
  virtual
  PtiDeclHead*
  new_NetH(const FileRegion& fr,
	   tVpiNetType type,
	   tVpiVsType vstype,
	   bool sign,
	   PtExpr* left,
	   PtExpr* right,
	   PtDelay* delay);

  /// @brief 範囲指定型 net 宣言のヘッダの生成 (strength, 遅延あり)
  virtual
  PtiDeclHead*
  new_NetH(const FileRegion& fr,
	   tVpiNetType type,
	   tVpiVsType vstype,
	   bool sign,
	   PtExpr* left,
	   PtExpr* right,
	   PtStrength* strength,
	   PtDelay* delay);

  /// @brief 宣言要素の生成
  virtual
  PtDeclItem*
  new_DeclItem(const FileRegion& fr,
	       const char* name);

  /// @brief 初期値付き宣言要素の生成
  virtual
  PtDeclItem*
  new_DeclItem(const FileRegion& fr,
	       const char* name,
	       PtExpr* init_value);

  /// @brief 配列型宣言要素の生成
  virtual
  PtDeclItem*
  new_DeclItem(const FileRegion& fr,
	       const char* name,
	       PtRangeArray range_array);

  /// @brief 範囲の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] msb MSB を表す式
  /// @param[in] lsb LSB を表す式
  virtual
  PtRange*
  new_Range(const FileRegion& fr,
	    PtExpr* msb,
	    PtExpr* lsb);


  //////////////////////////////////////////////////////////////////////
  // item 関係
  //////////////////////////////////////////////////////////////////////

  /// @brief defparam 文のヘッダの生成
  virtual
  PtItem*
  new_DefParamH(const FileRegion& fr,
		PtDefParamArray elem_array);

  /// @brief defparam 文の要素の生成
  virtual
  PtDefParam*
  new_DefParam(const FileRegion& fr,
	       const char* name,
	       PtExpr* value);

  /// @brief defparam 文の要素の生成 (階層つき識別子)
  virtual
  PtDefParam*
  new_DefParam(const FileRegion& fr,
	       PtNameBranchArray nb_array,
	       const char* tail_name,
	       PtExpr* value);

  /// @brief continuous assign 文のヘッダの生成
  virtual
  PtItem*
  new_ContAssignH(const FileRegion& fr,
		  PtContAssignArray elem_array);

  /// @brief continuous assign 文のヘッダの生成 (strengthつき)
  virtual
  PtItem*
  new_ContAssignH(const FileRegion& fr,
		  PtStrength* strength,
		  PtContAssignArray elem_array);

  /// @brief continuous assign 文のヘッダの生成 (遅延付き)
  virtual
  PtItem*
  new_ContAssignH(const FileRegion& fr,
		  PtDelay* delay,
		  PtContAssignArray elem_array);

  /// @brief continuous assign 文のヘッダの生成 (strength, 遅延付き)
  virtual
  PtItem*
  new_ContAssignH(const FileRegion& fr,
		  PtStrength* strength,
		  PtDelay* delay,
		  PtContAssignArray elem_array);

  /// @brief continuous assign 文の生成
  virtual
  PtContAssign*
  new_ContAssign(const FileRegion& fr,
		 PtExpr* lhs,
		 PtExpr* rhs);

  /// @brief initial 文の生成
  virtual
  PtItem*
  new_Initial(const FileRegion& fr,
	      PtStmt* body);

  /// @brief always 文の生成
  virtual
  PtItem*
  new_Always(const FileRegion& fr,
	     PtStmt* body);

  /// @brief task 文の生成
  virtual
  PtItem*
  new_Task(const FileRegion& fr,
	   const char* name,
	   bool automatic,
	   PtIOHeadArray iohead_array,
	   PtDeclHeadArray paramhead_array,
	   PtDeclHeadArray lparamhead_array,
	   PtDeclHeadArray declhead_array,
	   PtStmt* stmt);

  /// @brief 1ビット型 function 文の生成
  virtual
  PtItem*
  new_Function(const FileRegion& fr,
	       const char* name,
	       bool automatic,
	       bool sign,
	       PtIOHeadArray iohead_array,
	       PtDeclHeadArray paraead_array,
	       PtDeclHeadArray lparamhead_array,
	       PtDeclHeadArray declhead_array,
	       PtStmt* stmt);

  /// @brief 範囲指定型 function 文の生成
  virtual
  PtItem*
  new_SizedFunc(const FileRegion& fr,
		const char* name,
		bool automatic,
		bool sign,
		PtExpr* left,
		PtExpr* right,
		PtIOHeadArray iohead_array,
		PtDeclHeadArray paraead_array,
		PtDeclHeadArray lparamhead_array,
		PtDeclHeadArray declhead_array,
		PtStmt* stmt);

  /// @brief 組み込み型 function 文の生成
  virtual
  PtItem*
  new_TypedFunc(const FileRegion& fr,
		const char* name,
		bool automatic,
		bool sign,
		tVpiVarType func_type,
		PtIOHeadArray iohead_array,
		PtDeclHeadArray paraead_array,
		PtDeclHeadArray lparamhead_array,
		PtDeclHeadArray declhead_array,
		PtStmt* stmt);

  /// @brief gate instance 文のヘッダの生成
  virtual
  PtItem*
  new_GateH(const FileRegion& fr,
	    tVpiPrimType type,
	    PtInstArray elem_array);

  /// @brief gate instance 文のヘッダの生成 (strength付き)
  virtual
  PtItem*
  new_GateH(const FileRegion& fr,
	    tVpiPrimType type,
	    PtStrength* strength,
	    PtInstArray elem_array);

  /// @brief gate instance 文のヘッダの生成 (遅延付き)
  virtual
  PtItem*
  new_GateH(const FileRegion& fr,
	    tVpiPrimType type,
	    PtDelay* delay,
	    PtInstArray elem_array);

  /// @brief gate instance 文のヘッダの生成 (strength, 遅延付き)
  virtual
  PtItem*
  new_GateH(const FileRegion& fr,
	    tVpiPrimType type,
	    PtStrength* strength,
	    PtDelay* delay,
	    PtInstArray elem_array);

  /// @brief module instance/UDP instance 文のヘッダの生成
  virtual
  PtItem*
  new_MuH(const FileRegion& fr,
	  const char* def_name,
	  PtInstArray elem_array);

  /// @brief module instance/UDP instance 文のヘッダの生成 (strength付き)
  virtual
  PtItem*
  new_MuH(const FileRegion& fr,
	  const char* def_name,
	  PtStrength* strength,
	  PtInstArray elem_array);

  /// @brief module instance/UDP instance 文のヘッダの生成 (遅延付き)
  virtual
  PtItem*
  new_MuH(const FileRegion& fr,
	  const char* def_name,
	  PtDelay* delay,
	  PtInstArray elem_array);

  /// @brief module instance/UDP instance 文のヘッダの生成 (strength, 遅延付き)
  virtual
  PtItem*
  new_MuH(const FileRegion& fr,
	  const char* def_name,
	  PtStrength* strength,
	  PtDelay* delay,
	  PtInstArray elem_array);

  /// @brief module instance/UDP instance 文のヘッダの生成
  virtual
  PtItem*
  new_MuH(const FileRegion& fr,
	  const char* def_name,
	  PtConnectionArray con_array,
	  PtInstArray elem_array);

  /// @brief module instance/UDP/gate instance の要素の生成
  virtual
  PtInst*
  new_Inst(const FileRegion& fr,
	   PtConnectionArray con_array);

  /// @brief 名前付き module instance/UDP/gate instance の要素の生成
  virtual
  PtInst*
  new_InstN(const FileRegion& fr,
	    const char* name,
	    PtConnectionArray con_array);

  /// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
  virtual
  PtInst*
  new_InstV(const FileRegion& fr,
	    const char* name,
	    PtExpr* left,
	    PtExpr* right,
	    PtConnectionArray con_array);

  /// @brief generate 文の生成
  virtual
  PtItem*
  new_Generate(const FileRegion& fr,
	       PtDeclHeadArray declhead_array,
	       PtItemArray item_array);

  /// @brief generate block 文の生成
  virtual
  PtItem*
  new_GenBlock(const FileRegion& fr,
	       PtDeclHeadArray declhead_array,
	       PtItemArray item_array);

  /// @brief 名前付き generate block 文の生成
  virtual
  PtItem*
  new_GenBlock(const FileRegion& fr,
	       const char* name,
	       PtDeclHeadArray declhead_array,
	       PtItemArray item_array);

  /// @brief generate if 文の生成
  virtual
  PtItem*
  new_GenIf(const FileRegion& fr,
	    PtExpr* cond,
	    PtDeclHeadArray then_declhead_array,
	    PtItemArray then_item_array,
	    PtDeclHeadArray else_declhead_array,
	    PtItemArray else_item_array);

  /// @brief generate case 文の生成
  virtual
  PtItem*
  new_GenCase(const FileRegion& fr,
	      PtExpr* expr,
	      PtGenCaseItemArray item_array);

  /// @brief generate case の要素の生成
  virtual
  PtGenCaseItem*
  new_GenCaseItem(const FileRegion& fr,
		  PtExprArray label_array,
		  PtDeclHeadArray declhead_array,
		  PtItemArray item_array);

  /// @brief generate for 文の生成
  virtual
  PtItem*
  new_GenFor(const FileRegion& fr,
	     const char* loop_var,
	     PtExpr* init_expr,
	     PtExpr* cond,
	     PtExpr* next_expr,
	     const char* block_name,
	     PtDeclHeadArray declhead_array,
	     PtItemArray item_array);

  /// @brief specify block item の生成
  virtual
  PtItem*
  new_SpecItem(const FileRegion& fr,
	       tVpiSpecItemType id,
	       PtExprArray terminal_top);

  /// @brief path 仕様を生成する．
  virtual
  PtItem*
  new_SpecPath(const FileRegion& fr,
	       tVpiSpecPathType id,
	       PtExpr* expr,
	       PtPathDecl* path_decl);

  /// @brief パス記述の生成
  virtual
  PtPathDecl*
  new_PathDecl(const FileRegion& fr,
	       int edge,
	       PtExprArray input_array,
	       int input_pol,
	       int op,
	       PtExprArray output_array,
	       int output_pol,
	       PtExpr* expr,
	       PtPathDelay* path_delay);

  /// @brief path delay value の生成 (値が1個)
  virtual
  PtPathDelay*
  new_PathDelay(const FileRegion& fr,
		PtExpr* value);

  /// @brief path delay value の生成 (値が2個)
  virtual
  PtPathDelay*
  new_PathDelay(const FileRegion& fr,
		PtExpr* value1,
		PtExpr* value2);

  /// @brief path delay value の生成 (値が3個)
  virtual
  PtPathDelay*
  new_PathDelay(const FileRegion& fr,
		PtExpr* value1,
		PtExpr* value2,
		PtExpr* value3);

  /// @brief path delay value の生成 (値が6個)
  virtual
  PtPathDelay*
  new_PathDelay(const FileRegion& fr,
		PtExpr* value1,
		PtExpr* value2,
		PtExpr* value3,
		PtExpr* value4,
		PtExpr* value5,
		PtExpr* value6);

  /// @brief path delay value の生成 (値が12個)
  virtual
  PtPathDelay*
  new_PathDelay(const FileRegion& fr,
		PtExpr* value1,
		PtExpr* value2,
		PtExpr* value3,
		PtExpr* value4,
		PtExpr* value5,
		PtExpr* value6,
		PtExpr* value7,
		PtExpr* value8,
		PtExpr* value9,
		PtExpr* value10,
		PtExpr* value11,
		PtExpr* value12);


  //////////////////////////////////////////////////////////////////////
  // statement 関係
  //////////////////////////////////////////////////////////////////////

  /// @brief disable 文の生成
  virtual
  PtStmt*
  new_Disable(const FileRegion& fr,
	      const char* name);

  /// @brief disable 文の生成 (階層付き識別子)
  virtual
  PtStmt*
  new_Disable(const FileRegion& fr,
	      PtNameBranchArray nb_array,
	      const char* tail_name);

  /// @brief enable 文の生成
  virtual
  PtStmt*
  new_Enable(const FileRegion& fr,
	     const char* name,
	     PtExprArray arg_array);

  /// @brief enable 文の生成 (階層付き識別子)
  virtual
  PtStmt*
  new_Enable(const FileRegion& fr,
	     PtNameBranchArray nb_array,
	     const char* tail_name,
	     PtExprArray arg_array);

  /// @brief system task enable 文の生成
  virtual
  PtStmt*
  new_SysEnable(const FileRegion& fr,
		const char* task_name,
		PtExprArray arg_array);

  /// @brief delay control 文の生成
  virtual
  PtStmt*
  new_DcStmt(const FileRegion& fr,
	     PtControl* delay,
	     PtStmt* body);

  /// @brief event control 文の生成
  virtual
  PtStmt*
  new_EcStmt(const FileRegion& fr,
	     PtControl* event,
	     PtStmt* body);

  /// @brief wait 文の生成
  virtual
  PtStmt*
  new_Wait(const FileRegion& fr,
	   PtExpr* cond,
	   PtStmt* body);

  /// @brief assign 文の生成
  virtual
  PtStmt*
  new_Assign(const FileRegion& fr,
	     PtExpr* lhs,
	     PtExpr* rhs);

  /// @brief control 付き assign 文の生成
  virtual
  PtStmt*
  new_Assign(const FileRegion& fr,
	     PtExpr* lhs,
	     PtExpr* rhs,
	     PtControl* control);

  /// @brief nonblocking assign 文の生成
  virtual
  PtStmt*
  new_NbAssign(const FileRegion& fr,
	       PtExpr* lhs,
	       PtExpr* rhs);

  /// @brief control 付き nonblocking assign 文の生成
  virtual
  PtStmt*
  new_NbAssign(const FileRegion& fr,
	       PtExpr* lhs,
	       PtExpr* rhs,
	       PtControl* control);

  /// @brief event 文の生成
  virtual
  PtStmt*
  new_EventStmt(const FileRegion& fr,
		PtExpr* event);

  /// @brief null 文の生成
  virtual
  PtStmt*
  new_NullStmt(const FileRegion& fr);

  /// @brief if 文の生成
  virtual
  PtStmt*
  new_If(const FileRegion& fr,
	 PtExpr* expr,
	 PtStmt* then_body);

  /// @brief if 文の生成
  virtual
  PtStmt*
  new_If(const FileRegion& fr,
	 PtExpr* expr,
	 PtStmt* then_body,
	 PtStmt* else_body);

  /// @brief case 文の生成
  virtual
  PtStmt*
  new_Case(const FileRegion& fr,
	   PtExpr* expr,
	   PtCaseItemArray caseitem_array);

  /// @brief casex 文の生成
  virtual
  PtStmt*
  new_CaseX(const FileRegion& fr,
	    PtExpr* expr,
	    PtCaseItemArray caseitem_array);

  /// @brief casez 文の生成
  virtual
  PtStmt*
  new_CaseZ(const FileRegion& fr,
	    PtExpr* expr,
	    PtCaseItemArray caseitem_array);

  /// @brief case item の生成
  virtual
  PtCaseItem*
  new_CaseItem(const FileRegion& fr,
	       PtExprArray label_array,
	       PtStmt* body);

  /// @brief forever 文の生成
  virtual
  PtStmt*
  new_Forever(const FileRegion& fr,
	      PtStmt* body);

  /// @brief repeat 文の生成
  virtual
  PtStmt*
  new_Repeat(const FileRegion& fr,
	     PtExpr* expr,
	     PtStmt* body);

  /// @brief while 文の生成
  virtual
  PtStmt*
  new_While(const FileRegion& fr,
	    PtExpr* cond,
	    PtStmt* body);

  /// @brief for 文の生成
  virtual
  PtStmt*
  new_For(const FileRegion& fr,
	  PtStmt* init,
	  PtExpr* cond,
	  PtStmt* next,
	  PtStmt* body);

  /// @brief procedural assign 文の生成
  virtual
  PtStmt*
  new_PcAssign(const FileRegion& fr,
	       PtExpr* lhs,
	       PtExpr* rhs);

  /// @brief deassign 文の生成
  virtual
  PtStmt*
  new_Deassign(const FileRegion& fr,
	       PtExpr* lhs);

  /// @brief force 文の生成
  virtual
  PtStmt*
  new_Force(const FileRegion& fr,
	    PtExpr* lhs,
	    PtExpr* rhs);

  /// @brief release 文の生成
  virtual
  PtStmt*
  new_Release(const FileRegion& fr,
	      PtExpr* lhs);

  /// @brief parallel block の生成
  virtual
  PtStmt*
  new_ParBlock(const FileRegion& fr,
	       PtStmtArray stmt_array);

  /// @brief 名前付き parallel block の生成
  virtual
  PtStmt*
  new_NamedParBlock(const FileRegion& fr,
		    const char* name,
		    PtDeclHeadArray declhead_array,
		    PtStmtArray stmt_array);

  /// @brief sequential block の生成
  virtual
  PtStmt*
  new_SeqBlock(const FileRegion& fr,
	       PtStmtArray stmt_array);

  /// @brief 名前付き sequential block の生成
  virtual
  PtStmt*
  new_NamedSeqBlock(const FileRegion& fr,
		    const char* name,
		    PtDeclHeadArray declhead_array,
		    PtStmtArray stmt_array);


  //////////////////////////////////////////////////////////////////////
  // expression 関係
  //////////////////////////////////////////////////////////////////////

  /// @brief 単項演算子の生成
  virtual
  PtExpr*
  new_Opr(const FileRegion& fr,
	  tVpiOpType type,
	  PtExpr* opr);

  /// @brief 二項演算子の生成
  virtual
  PtExpr*
  new_Opr(const FileRegion& fr,
	  tVpiOpType type,
	  PtExpr* opr1,
	  PtExpr* opr2);

  /// @brief 三項演算子の生成
  virtual
  PtExpr*
  new_Opr(const FileRegion& fr,
	  tVpiOpType type,
	  PtExpr* opr1,
	  PtExpr* opr2,
	  PtExpr* opr3);

  /// @brief concatination 演算子の生成
  virtual
  PtExpr*
  new_Concat(const FileRegion& fr,
	     PtExprArray expr_top);

  /// @brief multi-concatination 演算子の生成
  virtual
  PtExpr*
  new_MultiConcat(const FileRegion& fr,
		  PtExprArray expr_top);

  /// @brief min/typ/max delay 演算子の生成
  virtual
  PtExpr*
  new_MinTypMax(const FileRegion& fr,
		PtExpr* val0,
		PtExpr* val1,
		PtExpr* val2);

  /// @brief primary の生成
  virtual
  PtExpr*
  new_Primary(const FileRegion& fr,
	      const char* name);

  /// @brief インデックス付き primary の生成
  virtual
  PtExpr*
  new_Primary(const FileRegion& fr,
	      const char* name,
	      PtExprArray index_array);

  /// @brief 範囲指定付き primary の生成
  virtual
  PtExpr*
  new_Primary(const FileRegion& fr,
	      const char* name,
	      tVpiRangeMode mode,
	      PtExpr* left,
	      PtExpr* right);

  /// @brief インデックスと範囲指定付き primary の生成
  virtual
  PtExpr*
  new_Primary(const FileRegion& fr,
	      const char* name,
	      PtExprArray index_array,
	      tVpiRangeMode mode,
	      PtExpr* left,
	      PtExpr* right);

  /// @brief primary の生成 (階層付き)
  virtual
  PtExpr*
  new_Primary(const FileRegion& fr,
	      PtNameBranchArray nb_array,
	      const char* tail_name);

  /// @brief インデックス付き primary の生成 (階層付き)
  virtual
  PtExpr*
  new_Primary(const FileRegion& fr,
	      PtNameBranchArray nb_array,
	      const char* tail_name,
	      PtExprArray index_array);

  /// @brief 範囲指定付き primary の生成 (階層付き)
  virtual
  PtExpr*
  new_Primary(const FileRegion& fr,
	      PtNameBranchArray nb_array,
	      const char* tail_name,
	      tVpiRangeMode mode,
	      PtExpr* left,
	      PtExpr* right);

  /// @brief インデックスと範囲指定付き primary の生成 (階層付き)
  virtual
  PtExpr*
  new_Primary(const FileRegion& fr,
	      PtNameBranchArray nb_array,
	      const char* tail_name,
	      PtExprArray index_array,
	      tVpiRangeMode mode,
	      PtExpr* left,
	      PtExpr* right);

  /// @brief constant primary の生成
  virtual
  PtExpr*
  new_CPrimary(const FileRegion& fr,
	       const char* name,
	       PtExprArray index_array);

  /// @brief 範囲指定付き constant primary の生成
  virtual
  PtExpr*
  new_CPrimary(const FileRegion& fr,
	       const char* name,
	       tVpiRangeMode mode,
	       PtExpr* left,
	       PtExpr* right);

  /// @brief インデックス付き constant primary の生成 (階層付き)
  virtual
  PtExpr*
  new_CPrimary(const FileRegion& fr,
	       PtNameBranchArray nb_array,
	       const char* tail_name,
	       PtExprArray index_array);

  /// @brief function call の生成
  virtual
  PtExpr*
  new_FuncCall(const FileRegion& fr,
	       const char* name,
	       PtExprArray arg_array);

  /// @brief function call の生成 (階層付き)
  virtual
  PtExpr*
  new_FuncCall(const FileRegion& fr,
	       PtNameBranchArray nb_array,
	       const char* tail_name,
	       PtExprArray arg_array);

  /// @brief system function call の生成
  virtual
  PtExpr*
  new_SysFuncCall(const FileRegion& fr,
		  const char* name,
		  PtExprArray arg_array);

  /// @brief 整数型の定数の生成
  virtual
  PtExpr*
  new_IntConst(const FileRegion& fr,
	       ymuint32 value);

  /// @brief 整数型の定数の生成
  virtual
  PtExpr*
  new_IntConst(const FileRegion& fr,
	       const char* value);

  /// @brief 基底付き整数型の定数の生成
  virtual
  PtExpr*
  new_IntConst(const FileRegion& fr,
	       tVpiConstType const_type,
	       const char* value);

  /// @brief サイズと基底付き定数の生成
  virtual
  PtExpr*
  new_IntConst(const FileRegion& fr,
	       ymuint32 size,
	       tVpiConstType const_type,
	       const char* value);

  /// @brief 実数型の定数の生成
  virtual
  PtExpr*
  new_RealConst(const FileRegion& fr,
		double value);

  /// @brief 文字列型の定数の生成
  virtual
  PtExpr*
  new_StringConst(const FileRegion& fr,
		  const char* value);


  //////////////////////////////////////////////////////////////////////
  // その他
  //////////////////////////////////////////////////////////////////////

  /// @brief ディレイコントロールの生成
  virtual
  PtControl*
  new_DelayControl(const FileRegion& fr,
		   PtExpr* value);

  /// @brief イベントコントロールの生成
  virtual
  PtControl*
  new_EventControl(const FileRegion& fr,
		   PtExprArray event_array);

  /// @brief リピートコントロールの生成
  virtual
  PtControl*
  new_RepeatControl(const FileRegion& fr,
		    PtExpr* expr,
		    PtExprArray event_array);

  /// @brief 順序つき結合子の生成
  virtual
  PtConnection*
  new_OrderedCon(const FileRegion& fr,
		 PtExpr* expr);

  /// @brief 順序つき結合子の生成
  virtual
  PtConnection*
  new_OrderedCon(PtExpr* expr);

  /// @brief 名前付き結合子の生成
  virtual
  PtConnection*
  new_NamedCon(const FileRegion& fr,
	       const char* name,
	       PtExpr* expr);

  /// @brief drive strength の生成
  virtual
  PtStrength*
  new_Strength(const FileRegion& fr,
	       tVpiStrength value1,
	       tVpiStrength value2);

  /// @brief charge strength の生成
  virtual
  PtStrength*
  new_Strength(const FileRegion& fr,
	       tVpiStrength value);

  /// @brief 遅延値の生成 (1つの値)
  virtual
  PtDelay*
  new_Delay(const FileRegion& fr,
	    PtExpr* value1);

  /// @brief 遅延値の生成 (2つの値)
  virtual
  PtDelay*
  new_Delay(const FileRegion& fr,
	    PtExpr* value1,
	    PtExpr* value2);

  /// @brief 遅延値の生成 (3つの値)
  virtual
  PtDelay*
  new_Delay(const FileRegion& fr,
	    PtExpr* value1,
	    PtExpr* value2,
	    PtExpr* value3);

  /// @brief 階層名の生成
  virtual
  PtNameBranch*
  new_NameBranch(const char* name);

  /// @brief インデックス付きの階層名の生成
  virtual
  PtNameBranch*
  new_NameBranch(const char* name,
		 int index);


  //////////////////////////////////////////////////////////////////////
  // attribute instance 関係
  //////////////////////////////////////////////////////////////////////

  /// @brief attribute instance の生成
  virtual
  PtAttrInst*
  new_AttrInst(const FileRegion& fr,
	       PtAttrSpecArray as_array);

  /// @brief attribute spec の生成
  virtual
  PtAttrSpec*
  new_AttrSpec(const FileRegion& fr,
	       const char* name,
	       PtExpr* expr);


  //////////////////////////////////////////////////////////////////////
  // その他
  //////////////////////////////////////////////////////////////////////

  /// @brief 文字列領域の確保
  virtual
  const char*
  new_string(const char* str);


public:
  //////////////////////////////////////////////////////////////////////
  // おもにデバッグ用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 確保したオブジェクトに関する情報を出力する．
  virtual
  void
  dump_profile(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 確保したオブジェクトの数
  int mNumDefStub;
  int mNumUdp;
  int mNumUdpEntry;
  int mNumUdpEntryS;
  int mNumUdpValue;
  int mNumModule;
  int mNumPort;
  int mNumPortRef;
  int mNumPortRefI;
  int mNumPortRefR;
  int mNumIOH;
  int mNumIOHV;
  int mNumIOItem;
  int mNumIOItemI;
  int mNumParamH;
  int mNumParamHV;
  int mNumParamHT;
  int mNumLocalParamH;
  int mNumLocalParamHV;
  int mNumLocalParamHT;
  int mNumSpecParamH;
  int mNumSpecParamHV;
  int mNumEventH;
  int mNumGenvarH;
  int mNumVarH;
  int mNumRegH;
  int mNumRegHV;
  int mNumNetH;
  int mNumNetHS;
  int mNumNetHD;
  int mNumNetHSD;
  int mNumNetHV;
  int mNumNetHVS;
  int mNumNetHVD;
  int mNumNetHVSD;
  int mNumDeclItem;
  int mNumDeclItemI;
  int mNumDeclItemR;
  int mNumDefParamH;
  int mNumDefParam;
  int mNumContAssignH;
  int mNumContAssignHS;
  int mNumContAssignHD;
  int mNumContAssignHSD;
  int mNumContAssign;
  int mNumInitial;
  int mNumAlways;
  int mNumTask;
  int mNumFunction;
  int mNumSizedFunc;
  int mNumTypedFunc;
  int mNumGateH;
  int mNumGateHS;
  int mNumGateHD;
  int mNumGateHSD;
  int mNumMuH;
  int mNumMuHS;
  int mNumMuHD;
  int mNumMuHSD;
  int mNumMuHP;
  int mNumInst;
  int mNumInstN;
  int mNumInstR;
  int mNumGenerate;
  int mNumGenBlock;
  int mNumGenBlockN;
  int mNumGenIf;
  int mNumGenCase;
  int mNumGenCaseItem;
  int mNumGenFor;
  int mNumSpecItem;
  int mNumSpecPath;
  int mNumPathDecl;
  int mNumPathDelay;
  int mNumDisable;
  int mNumDisableH;
  int mNumEnable;
  int mNumEnableH;
  int mNumSysEnable;
  int mNumDcStmt;
  int mNumEcStmt;
  int mNumWait;
  int mNumAssign;
  int mNumAssignC;
  int mNumNbAssign;
  int mNumNbAssignC;
  int mNumEventStmt;
  int mNumNullStmt;
  int mNumIf;
  int mNumCase;
  int mNumCaseX;
  int mNumCaseZ;
  int mNumCaseItem;
  int mNumForever;
  int mNumRepeat;
  int mNumWhile;
  int mNumFor;
  int mNumPcAssign;
  int mNumDeassign;
  int mNumForce;
  int mNumRelease;
  int mNumParBlock;
  int mNumParBlockN;
  int mNumSeqBlock;
  int mNumSeqBlockN;
  int mNumOpr1;
  int mNumOpr2;
  int mNumOpr3;
  int mNumConcat;
  int mNumMultiConcat;
  int mNumMinTypMax3;
  int mNumPrimary;
  int mNumPrimaryI;
  int mNumPrimaryR;
  int mNumPrimaryIR;
  int mNumPrimaryH;
  int mNumPrimaryHI;
  int mNumPrimaryHR;
  int mNumPrimaryHIR;
  int mNumPrimaryCI;
  int mNumPrimaryCR;
  int mNumPrimaryHCI;
  int mNumFuncCall;
  int mNumFuncCallH;
  int mNumSysFuncCall;
  int mNumIntConstant1;
  int mNumIntConstant2;
  int mNumIntConstant3;
  int mNumRealConstant;
  int mNumStringConstant;
  int mNumDelayControl;
  int mNumEventControl;
  int mNumRepeatControl;
  int mNumOrderedCon;
  int mNumAiOrderedCon;
  int mNumNamedCon;
  int mNumAiNamedCon;
  int mNumStrength;
  int mNumDelay;
  int mNumNameBranch;
  int mNumNameBranchI;
  int mNumAttrInst;
  int mNumAttrSpec;
};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_PT_COMPACT_CPTFACTORY_H
