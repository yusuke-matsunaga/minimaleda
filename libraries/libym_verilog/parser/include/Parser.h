#ifndef LIBYM_VERILOG_PARSER_PARSER_H
#define LIBYM_VERILOG_PARSER_PARSER_H

/// @file libym_verilog/parser/include/Parser.h
/// @brief Parser のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Parser.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/pt/PtP.h"
#include "ym_utils/File.h"
#include "ym_utils/MsgHandler.h"
#include "ym_utils/Alloc.h"
#include "PtiFwd.h"
#include "PtiFactory.h"
#include "PtiDecl.h"
#include "PtrList.h"


BEGIN_NAMESPACE_YM_VERILOG

class VlLineWatcher;
class PtMgr;
class PtiFactory;
class Lex;
class PuHierName;

union YYSTYPE;

//////////////////////////////////////////////////////////////////////
/// @class Parser Parser.h "Parser.h"
/// @ingroup Parser
/// @brief Verilog-HDL のパーサークラス
//////////////////////////////////////////////////////////////////////
class Parser
{
public:
  
  /// @brief コンストラクタ
  /// @param[in] msg_mgr メッセージマネージャ
  /// @param[in] ptmgr 読んだ結果のパース木を登録するマネージャ
  /// @param[in] alloc メモリアロケータ
  /// @param[in] ptifactory パース木の要素を生成するファクトリクラス
  Parser(MsgMgr& msg_mgr,
	 PtMgr& ptmgr,
	 AllocBase& alloc,
	 PtiFactory& ptifactory);
  
  /// @brief デストラクタ
  ~Parser();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイルを読み込む．
  /// @param[in] filename 読み込むファイル名
  /// @param[in] searchpath サーチパス
  /// @param[in] watcher_list 行番号ウオッチャーのリスト
  /// @retval true 正常に終了した．
  /// @retval false エラーが起こった．
  bool
  read_file(const string& filename,
	    const SearchPathList& searchpath,
	    const list<VlLineWatcher*>& watcher_list);

  
public:
  //////////////////////////////////////////////////////////////////////
  // UDP 関連の生成関数
  //////////////////////////////////////////////////////////////////////

  /// @brief Verilog1995 タイプのUDP を生成する．
  void
  new_Udp1995(const FileRegion& file_region,
	      const char* name,
	      const char* init_name,
	      const FileRegion& init_loc,
	      PtExpr* init_value,
	      PtrList<PtAttrInst>* ai_list);

  /// @brief Verilog2001 タイプのUDP を生成する．
  void
  new_Udp2001(const FileRegion& file_region,
	      const char* name,
	      const char* init_name,
	      const FileRegion& init_loc,
	      PtExpr* init_value,
	      PtrList<PtAttrInst>* ai_list);

  /// @brief combinational UDP 用のテーブルエントリの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] output_loc 出力記号の位置
  /// @param[in] output_symbol 出力記号
  void
  new_UdpEntry(const FileRegion& fr,
	       const FileRegion& output_loc,
	       tVpiUdpVal output_symbol);

  /// @brief sequential UDP 用のテーブルエントリの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] current_loc 現状態記号の位置
  /// @param[in] current_symbol 現状態記号
  /// @param[in] output_loc 出力記号の位置
  /// @param[in] output_symbol 出力記号
  void
  new_UdpEntry(const FileRegion& fr,
	       const FileRegion& current_loc,
	       tVpiUdpVal current_symbol,
	       const FileRegion& output_loc,
	       tVpiUdpVal output_symbol);
  
  /// @brief UDP のテーブルエントリの要素の値の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] symbol シンボル
  /// @return 生成された値
  void
  new_UdpValue(const FileRegion& fr,
	       tVpiUdpVal symbol);
  
  
private:

  /// @brief new_Udp の下請け関数
  void
  new_Udp(const FileRegion& file_region,
	  const char* udp_name,
	  const char* init_name,
	  const FileRegion& init_loc,
	  PtExpr* init_value,
	  PtrList<PtAttrInst>* ai_list,
	  bool is_seq,
	  const PtIOItem* out_item,
	  PtiPortArray port_array,
	  PtIOHeadArray iohead_array);

  
public:
  //////////////////////////////////////////////////////////////////////
  // モジュール関連の要素の生成関数
  //////////////////////////////////////////////////////////////////////

  /// @brief Verilog1995 タイプのモジュール(のテンプレート)を生成する．
  void
  new_Module1995(const FileRegion& file_region,
		 bool is_macro,
		 const char* name,
		 PtrList<PtAttrInst>* ai_list);

  /// @brief Verilog2001 タイプのモジュール(のテンプレート)を生成する．
  void
  new_Module2001(const FileRegion& file_region,
		 bool is_macro,
		 const char* name,
		 PtrList<PtAttrInst>* ai_list);

  
public:
  //////////////////////////////////////////////////////////////////////
  // ポート関連の要素の生成関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入出力宣言からポートを作る．
  PtiPortArray
  new_PortArray(PtIOHeadArray iohead_array);
  
  /// @brief 空のポートの生成
  void
  new_Port();

  /// @brief ポートの生成 (内側の式のみ指定するタイプ)
  /// @param[in] file_region ファイル位置
  void
  new_Port1(const FileRegion& file_region);

  /// @brief ポートの生成 (外側の名前のみ指定するタイプ)
  /// @param[in] file_region ファイル位置
  /// @param[in] name 外側の名前
  void
  new_Port2(const FileRegion& file_region,
	   const char* name);

  /// @brief ポートの生成 (外側の名前と内側の式を指定するタイプ)
  /// @param[in] file_region ファイル位置
  /// @param[in] name 外側の名前
  void
  new_Port3(const FileRegion& file_region,
	    const char* name);

  /// @brief ポート参照式の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name ポートに接続している内部の識別子名
  void
  new_PortRef(const FileRegion& fr,
	      const char* name);

  /// @brief ビット指定つきポート参照式の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name ポートに接続している内部の識別子名
  /// @param[in] index ビット指定用の式
  void
  new_PortRef(const FileRegion& fr,
	      const char* name,
	      PtExpr* index);

  /// @brief 範囲指定付きポート参照式の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name ポートに接続している内部の識別子名
  /// @param[in] range_mode 範囲指定のモード
  /// @param[in] left 範囲指定の左側の式
  /// @param[in] right 範囲指摘の右側の式
  void
  new_PortRef(const FileRegion& fr,
	      const char* name,
	      tVpiRangeMode range_mode,
	      PtExpr* left,
	      PtExpr* right);

  
public:
  //////////////////////////////////////////////////////////////////////
  // IO 宣言関係の要素の生成関数
  //////////////////////////////////////////////////////////////////////

  /// @brief IO 宣言のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type IO の種類
  /// @param[in] sign 符号付きのとき true となるフラグ
  void
  new_IOHead(const FileRegion& fr,
	     tPtIOType type,
	     bool sign,
	     PtrList<PtAttrInst>* ai_list);

  /// @brief IO 宣言のヘッダの生成 (reg 型)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type IO の種類
  /// @param[in] sign 符号付きのとき true となるフラグ
  void
  new_RegIOHead(const FileRegion& fr,
		tPtIOType type,
		bool sign,
		PtrList<PtAttrInst>* ai_list);

  /// @brief IO 宣言のヘッダの生成 (ネット型)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type IO の種類
  /// @param[in] net_type 補助的なネット型
  /// @param[in] sign 符号付きのとき true となるフラグ
  void
  new_NetIOHead(const FileRegion& fr,
		tPtIOType type,
		tVpiNetType net_type,
		bool sign,
		PtrList<PtAttrInst>* ai_list);

  /// @brief IO 宣言のヘッダの生成 (変数型)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type IO の種類
  /// @param[in] var_type 補助的な変数型
  void
  new_VarIOHead(const FileRegion& fr,
		tPtIOType type,
		tVpiVarType var_type,
		PtrList<PtAttrInst>* ai_list);

  /// @brief 範囲付きの IO 宣言のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type IO の種類
  /// @param[in] sign 符号付きのとき true となるフラグ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  void
  new_IOHead(const FileRegion& fr,
	     tPtIOType type,
	     bool sign,
	     PtExpr* left,
	     PtExpr* right,
	     PtrList<PtAttrInst>* ai_list);

  /// @brief 範囲付きの IO 宣言のヘッダの生成 (reg 型)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type IO の種類
  /// @param[in] sign 符号付きのとき true となるフラグ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  void
  new_RegIOHead(const FileRegion& fr,
		tPtIOType type,
		bool sign,
		PtExpr* left,
		PtExpr* right,
		PtrList<PtAttrInst>* ai_list);

  /// @brief 範囲付きの IO 宣言のヘッダの生成 (ネット型)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type IO の種類
  /// @param[in] net_type 補助的なネット型
  /// @param[in] sign 符号付きのとき true となるフラグ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  void
  new_NetIOHead(const FileRegion& fr,
		tPtIOType type,
		tVpiNetType net_type,
		bool sign,
		PtExpr* left,
		PtExpr* right,
		PtrList<PtAttrInst>* ai_list);

  /// @brief IO 宣言の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 要素名
  void
  new_IOItem(const FileRegion& fr,
	     const char* name);


  /// @brief 初期値付き IO 宣言の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 要素名
  /// @param[in] init_value 初期値を表す式
  void
  new_IOItem(const FileRegion& fr,
	     const char* name,
	     PtExpr* init_value);
  

public:
  //////////////////////////////////////////////////////////////////////
  // 宣言関連の要素の生成
  //////////////////////////////////////////////////////////////////////

  /// @brief パラメータ宣言のヘッダの生成 (型指定なし)
  /// @param[in] fr ファイル位置の情報
  void
  new_ParamH(const FileRegion& fr,
	     PtrList<PtAttrInst>* ai_list,
	     bool paramport = false);

  /// @brief 範囲指定型パラメータ宣言のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] sign 符号付きのとき true となるフラグ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  void
  new_ParamH(const FileRegion& fr,
	     bool sign,
	     PtExpr* left,
	     PtExpr* right,
	     PtrList<PtAttrInst>* ai_list,
	     bool paramport = false);

  /// @brief 組み込み型パラメータ宣言のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] var_type データ型
  void
  new_ParamH(const FileRegion& fr,
	     tVpiVarType var_type,
	     PtrList<PtAttrInst>* ai_list,
	     bool paramport = false);

  /// @brief local param 宣言のヘッダの生成 (型指定なし)
  /// @param[in] fr ファイル位置の情報
  void
  new_LocalParamH(const FileRegion& fr,
		  PtrList<PtAttrInst>* ai_list);
  
  /// @brief 範囲指定型 local param 宣言のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] sign 符号付きのとき true となるフラグ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  void
  new_LocalParamH(const FileRegion& fr,
		  bool sign,
		  PtExpr* left,
		  PtExpr* right,
		  PtrList<PtAttrInst>* ai_list);

  /// @brief 組み込み型パラメータ宣言のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] var_type データ型
  void
  new_LocalParamH(const FileRegion& fr,
		  tVpiVarType var_type,
		  PtrList<PtAttrInst>* ai_list);

  /// @brief specparam 宣言のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @return 生成された specparam
  void
  new_SpecParamH(const FileRegion& fr,
		 PtrList<PtAttrInst>* ai_list);

  /// @brief 範囲指定型 specparam 宣言のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  void
  new_SpecParamH(const FileRegion& fr,
		 PtExpr* left,
		 PtExpr* right,
		 PtrList<PtAttrInst>* ai_list);

  /// @brief イベント宣言のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @return 生成されたイベント
  void
  new_EventH(const FileRegion& fr,
	     PtrList<PtAttrInst>* ai_list);

  /// @brief genvar 宣言のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  void
  new_GenvarH(const FileRegion& fr,
	      PtrList<PtAttrInst>* ai_list);

  /// @brief 変数宣言のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] var_type データ型
  void
  new_VarH(const FileRegion& fr,
	   tVpiVarType var_type,
	   PtrList<PtAttrInst>* ai_list);

  /// @brief 1ビット型 reg 宣言のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] sign 符号付きの時 true となるフラグ
  void
  new_RegH(const FileRegion& fr,
	   bool sign,
	   PtrList<PtAttrInst>* ai_list);

  /// @brief 範囲指定型 reg 宣言のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] sign 符号付きの時 true となるフラグ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  void
  new_RegH(const FileRegion& fr,
	   bool sign,
	   PtExpr* left,
	   PtExpr* right,
	   PtrList<PtAttrInst>* ai_list);
  
  /// @brief 1ビット型 net 宣言のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type net の型
  /// @param[in] sign 符号の有無を表すフラグ
  void
  new_NetH(const FileRegion& fr,
	   tVpiNetType type,
	   bool sign,
	   PtrList<PtAttrInst>* ai_list);
  
  /// @brief 1ビット型 net 宣言のヘッダの生成 (strength あり)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type net の型
  /// @param[in] sign 符号の有無を表すフラグ
  /// @param[in] strength 信号強度
  void
  new_NetH(const FileRegion& fr,
	   tVpiNetType type,
	   bool sign,
	   PtStrength* strength,
	   PtrList<PtAttrInst>* ai_list);
  
  /// @brief 1ビット型 net 宣言のヘッダの生成 (遅延あり)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type net の型
  /// @param[in] sign 符号の有無を表すフラグ
  /// @param[in] delay 遅延
  void
  new_NetH(const FileRegion& fr,
	   tVpiNetType type,
	   bool sign,
	   PtDelay* delay,
	   PtrList<PtAttrInst>* ai_list);
  
  /// @brief 1ビット型 net 宣言のヘッダの生成 (strength, 遅延あり)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type net の型
  /// @param[in] sign 符号の有無を表すフラグ
  /// @param[in] strength 信号強度
  /// @param[in] delay 遅延
  void
  new_NetH(const FileRegion& fr,
	   tVpiNetType type,
	   bool sign,
	   PtStrength* strength,
	   PtDelay* delay,
	   PtrList<PtAttrInst>* ai_list);
  
  /// @brief 範囲指定型 net 宣言のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type net の型
  /// @param[in] vstype vector/scalar 指定
  /// @param[in] sign 符号の有無を表すフラグ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  void
  new_NetH(const FileRegion& fr,
	   tVpiNetType type,
	   tVpiVsType vstype,
	   bool sign,
	   PtExpr* left,
	   PtExpr* right,
	   PtrList<PtAttrInst>* ai_list);
  
  /// @brief 範囲指定型 net 宣言のヘッダの生成 (strengthあり)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type net の型
  /// @param[in] vstype vector/scalar 指定
  /// @param[in] sign 符号の有無を表すフラグ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] strength 信号強度
  void
  new_NetH(const FileRegion& fr,
	   tVpiNetType type,
	   tVpiVsType vstype,
	   bool sign,
	   PtExpr* left,
	   PtExpr* right,
	   PtStrength* strength,
	   PtrList<PtAttrInst>* ai_list);
  
  /// @brief 範囲指定型 net 宣言のヘッダの生成 (遅延あり)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type net の型
  /// @param[in] vstype vector/scalar 指定
  /// @param[in] sign 符号の有無を表すフラグ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] delay 遅延
  void
  new_NetH(const FileRegion& fr,
	   tVpiNetType type,
	   tVpiVsType vstype,
	   bool sign,
	   PtExpr* left,
	   PtExpr* right,
	   PtDelay* delay,
	   PtrList<PtAttrInst>* ai_list);
  
  /// @brief 範囲指定型 net 宣言のヘッダの生成 (strength, 遅延あり)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type net の型
  /// @param[in] vstype vector/scalar 指定
  /// @param[in] sign 符号の有無を表すフラグ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] strength 信号強度
  /// @param[in] delay 遅延
  void
  new_NetH(const FileRegion& fr,
	   tVpiNetType type,
	   tVpiVsType vstype,
	   bool sign,
	   PtExpr* left,
	   PtExpr* right,
	   PtStrength* strength,
	   PtDelay* delay,
	   PtrList<PtAttrInst>* ai_list);

  /// @brief 宣言要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 名前
  void
  new_DeclItem(const FileRegion& fr,
	       const char* name);

  /// @brief 初期値付き宣言要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] init_value 初期値を表す式
  void
  new_DeclItem(const FileRegion& fr,
	       const char* name,
	       PtExpr* init_value);

  /// @brief 配列型宣言要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] range_list 配列の各次元の範囲のリスト
  void
  new_DeclItem(const FileRegion& fr,
	       const char* name,
	       PtrList<PtRange>* range_list);

  /// @brief 範囲の生成
  /// @param[in] msb MSB を表す式
  /// @param[in] lsb LSB を表す式
  PtRange*
  new_Range(const FileRegion& fr,
	    PtExpr* msb,
	    PtExpr* lsb);
  

public:
  //////////////////////////////////////////////////////////////////////
  // item 関連の要素の生成
  //////////////////////////////////////////////////////////////////////

  /// @brief defparam 文のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  void
  new_DefParamH(const FileRegion& fr,
		PtrList<PtAttrInst>* ai_list);

  /// @brief defparam 文の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] value 値を表す式
  void
  new_DefParam(const FileRegion& fr,
	       const char* name,
	       PtExpr* value);

  /// @brief defparam 文の要素の生成 (階層つき識別子)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] hname 階層名
  /// @param[in] value 値を表す式
  void
  new_DefParam(const FileRegion& fr,
	       PuHierName* hname,
	       PtExpr* value);

  /// @brief continuous assign 文のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  void
  new_ContAssignH(const FileRegion& fr,
		  PtrList<PtAttrInst>* ai_list);

  /// @brief continuous assign 文のヘッダの生成 (strengthつき)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] strength 信号強度
  void
  new_ContAssignH(const FileRegion& fr,
		  PtStrength* strength,
		  PtrList<PtAttrInst>* ai_list);

  /// @brief continuous assign 文のヘッダの生成 (遅延付き)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] delay 遅延値
  void
  new_ContAssignH(const FileRegion& fr,
		  PtDelay* delay,
		  PtrList<PtAttrInst>* ai_list);

  /// @brief continuous assign 文のヘッダの生成 (strength, 遅延付き)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] strength 信号強度
  /// @param[in] delay 遅延値
  /// @param[in] elem_array 要素のリスト
  void
  new_ContAssignH(const FileRegion& fr,
		  PtStrength* strength,
		  PtDelay* delay,
		  PtrList<PtAttrInst>* ai_list);

  /// @brief continuous assign 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] lhs 左辺式
  /// @param[in] rhs 右辺式
  void
  new_ContAssign(const FileRegion& fr,
		 PtExpr* lhs,
		 PtExpr* rhs);

  /// @brief initial 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] body 本体のステートメント
  void
  new_Initial(const FileRegion& fr,
	      PtStmt* body,
	      PtrList<PtAttrInst>* ai_list);

  /// @brief always 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] body 本体のステートメント
  void
  new_Always(const FileRegion& fr,
	     PtStmt* body,
	     PtrList<PtAttrInst>* ai_list);

  /// @brief task 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name task 名
  /// @param[in] automatic automatic task の時に true となるフラグ
  /// @param[in] stmt 本体のステートメント
  void
  new_Task(const FileRegion& fr,
	   const char* name,
	   bool automatic,
	   PtStmt* stmt,
	   PtrList<PtAttrInst>* ai_list);
  
  /// @brief 1ビット型 function 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name function 名
  /// @param[in] automatic automatic task の時に true となるフラグ
  /// @param[in] sign signed 属性がついていたら true となるフラグ
  /// @param[in] stmt 本体のステートメント
  void
  new_Function(const FileRegion& fr,
	       const char* name,
	       bool automatic,
	       bool sign,
	       PtStmt* stmt,
	       PtrList<PtAttrInst>* ai_list);
  
  /// @brief 範囲指定型 function 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name function 名
  /// @param[in] automatic automatic task の時に true となるフラグ
  /// @param[in] sign 符号つきの時に true となるフラグ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] stmt 本体のステートメント
  void
  new_SizedFunc(const FileRegion& fr,
		const char* name,
		bool automatic,
		bool sign,
		PtExpr* left,
		PtExpr* right,
		PtStmt* stmt,
		PtrList<PtAttrInst>* ai_list);
  
  /// @brief 組み込み型 function 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name function 名
  /// @param[in] automatic automatic task の時に true となるフラグ
  /// @param[in] sign signed 属性がついていたら true となるフラグ
  /// @param[in] func_type 関数の戻値の型
  /// @param[in] stmt 本体のステートメント
  void
  new_TypedFunc(const FileRegion& fr,
		const char* name,
		bool automatic,
		bool sign,
		tVpiVarType func_type,
		PtStmt* stmt,
		PtrList<PtAttrInst>* ai_list);
  
  /// @brief gate instance 文のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type primitive の型
  void
  new_GateH(const FileRegion& fr,
	    tVpiPrimType type,
	    PtrList<PtAttrInst>* ai_list);
  
  /// @brief gate instance 文のヘッダの生成 (strength付き)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type primitive の型
  /// @param[in] strength 信号強度
  void
  new_GateH(const FileRegion& fr,
	    tVpiPrimType type,
	    PtStrength* strength,
	    PtrList<PtAttrInst>* ai_list);
  
  /// @brief gate instance 文のヘッダの生成 (遅延付き)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type primitive の型
  /// @param[in] delay 遅延値
  void
  new_GateH(const FileRegion& fr,
	    tVpiPrimType type,
	    PtDelay* delay,
	    PtrList<PtAttrInst>* ai_list);
  
  /// @brief gate instance 文のヘッダの生成 (strength, 遅延付き)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type primitive の型
  /// @param[in] strength 信号強度
  /// @param[in] delay 遅延値
  void
  new_GateH(const FileRegion& fr,
	    tVpiPrimType type,
	    PtStrength* strength,
	    PtDelay* delay,
	    PtrList<PtAttrInst>* ai_list);

  /// @brief module instance/UDP instance 文のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] def_name 定義名
  void
  new_MuH(const FileRegion& fr,
	  const char* def_name,
	  PtrList<PtAttrInst>* ai_list);

  /// @brief module instance/UDP instance 文のヘッダの生成 (strength付き)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] def_name 定義名
  /// @param[in] strength 信号強度
  void
  new_MuH(const FileRegion& fr,
	  const char* def_name,
	  PtStrength* strength,
	  PtrList<PtAttrInst>* ai_list);

  /// @brief module instance/UDP instance 文のヘッダの生成 (遅延付き)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] def_name 定義名
  /// @param[in] delay 遅延値
  void
  new_MuH(const FileRegion& fr,
	  const char* def_name,
	  PtDelay* delay,
	  PtrList<PtAttrInst>* ai_list);

  /// @brief module instance/UDP instance 文のヘッダの生成 (strength, 遅延付き)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] def_name 定義名
  /// @param[in] strength 信号強度
  /// @param[in] delay 遅延値
  void
  new_MuH(const FileRegion& fr,
	  const char* def_name,
	  PtStrength* strength,
	  PtDelay* delay,
	  PtrList<PtAttrInst>* ai_list);

  /// @brief module instance/UDP instance 文のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] def_name 定義名
  /// @param[in] con_list ポート割り当てリスト
  void
  new_MuH(const FileRegion& fr,
	  const char* def_name,
	  PtrList<PtConnection>* con_list,
	  PtrList<PtAttrInst>* ai_list);

  /// @brief module instance/UDP/gate instance の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] con_list ポート割り当ての配列
  void
  new_Inst(const FileRegion& fr,
	   PtrList<PtConnection>* con_list);

  /// @brief module instance/UDP/gate instance の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] expr1 ポート割り当て
  void
  new_Inst(const FileRegion& fr,
	   PtExpr* expr1);

  /// @brief module instance/UDP/gate instance の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] expr1, expr2 ポート割り当て
  void
  new_Inst(const FileRegion& fr,
	   PtExpr* expr1,
	   PtExpr* expr2);

  /// @brief module instance/UDP/gate instance の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] expr1, expr2, expr3 ポート割り当て
  void
  new_Inst(const FileRegion& fr,
	   PtExpr* expr1,
	   PtExpr* expr2,
	   PtExpr* expr3);

  /// @brief module instance/UDP/gate instance の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] expr1, expr2, expr3, expr4 ポート割り当て
  void
  new_Inst(const FileRegion& fr,
	   PtExpr* expr1,
	   PtExpr* expr2,
	   PtExpr* expr3,
	   PtExpr* expr4);

  /// @brief 名前付き module instance/UDP/gate instance の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] con_list ポート割り当ての配列
  void
  new_InstN(const FileRegion& fr,
	    const char* name,
	    PtrList<PtConnection>* con_list);

  /// @brief 名前付き module instance/UDP/gate instance の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] expr1 ポート割り当て
  void
  new_InstN(const FileRegion& fr,
	    const char* name,
	    PtExpr* expr1);

  /// @brief 名前付き module instance/UDP/gate instance の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] expr1, expr2 ポート割り当て
  void
  new_InstN(const FileRegion& fr,
	    const char* name,
	    PtExpr* expr1,
	    PtExpr* expr2);

  /// @brief 名前付き module instance/UDP/gate instance の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] expr1, expr2, expr3 ポート割り当て
  void
  new_InstN(const FileRegion& fr,
	    const char* name,
	    PtExpr* expr1,
	    PtExpr* expr2,
	    PtExpr* expr3);

  /// @brief 名前付き module instance/UDP/gate instance の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] expr1, expr2, expr3, expr4 ポート割り当て
  void
  new_InstN(const FileRegion& fr,
	    const char* name,
	    PtExpr* expr1,
	    PtExpr* expr2,
	    PtExpr* expr3,
	    PtExpr* expr4);

  /// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] con_list ポート割り当ての配列
  void
  new_InstV(const FileRegion& fr,
	    const char* name,
	    PtExpr* left,
	    PtExpr* right,
	    PtrList<PtConnection>* con_list);

  /// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] expr1 ポート割り当て
  void
  new_InstV(const FileRegion& fr,
	    const char* name,
	    PtExpr* left,
	    PtExpr* right,
	    PtExpr* expr1);

  /// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] expr1, expr2 ポート割り当て
  void
  new_InstV(const FileRegion& fr,
	    const char* name,
	    PtExpr* left,
	    PtExpr* right,
	    PtExpr* expr1,
	    PtExpr* expr2);

  /// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] expr1, expr2, expr3 ポート割り当て
  void
  new_InstV(const FileRegion& fr,
	    const char* name,
	    PtExpr* left,
	    PtExpr* right,
	    PtExpr* expr1,
	    PtExpr* expr2,
	    PtExpr* expr3);

  /// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] expr1, expr2, expr3, expr4 ポート割り当て
  void
  new_InstV(const FileRegion& fr,
	    const char* name,
	    PtExpr* left,
	    PtExpr* right,
	    PtExpr* expr1,
	    PtExpr* expr2,
	    PtExpr* expr3,
	    PtExpr* expr4);

  /// @brief generate 文の生成
  /// @param[in] fr ファイル位置の情報
  void
  new_Generate(const FileRegion& fr,
	       PtrList<PtAttrInst>* ai_list);
  
  /// @brief generate block 文の生成
  /// @param[in] fr ファイル位置の情報
  void
  new_GenBlock(const FileRegion& fr);

  /// @brief 名前付き generate block 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 名前
  void
  new_GenBlock(const FileRegion& fr,
	       const char* name);

  /// @brief generate if 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] cond 条件を表す式
  void
  new_GenIf(const FileRegion& fr,
	    PtExpr* cond);

  /// @brief generate if 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] cond 条件を表す式
  void
  new_GenIfElse(const FileRegion& fr,
		PtExpr* cond);

  /// @brief generate case 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] expr 選択式
  /// @param[in] item_list generate case item のリスト
  void
  new_GenCase(const FileRegion& fr,
	      PtExpr* expr,
	      PtrList<PtGenCaseItem>* item_list);

  /// @brief generate case の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] label_list 比較式のリスト
  /// @return 生成された generate case item
  PtGenCaseItem*
  new_GenCaseItem(const FileRegion& fr,
		  PtrList<PtExpr>* label_list);

  /// @brief generate for 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] loop_var ループ変数
  /// @param[in] init_expr 初期化式
  /// @param[in] cond ループ条件式
  /// @param[in] inc_var 増加式の左辺の変数
  /// @param[in] inc_expr 増加式
  /// @param[in] block_name ブロック名
  void
  new_GenFor(const FileRegion& fr,
	     const char* loop_var,
	     PtExpr* init_expr,
	     PtExpr* cond,
	     const char* inc_var,
	     PtExpr* inc_expr,
	     const char* block_name);

  /// @brief specify block item の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] id specify block item の種類
  /// @param[in] terminal_list 端子のリスト
  void
  new_SpecItem(const FileRegion& fr,
	       tVpiSpecItemType id,
	       PtrList<PtExpr>* terminal_list);

  /// @brief path 仕様を生成する．
  /// @param[in] fr ファイル位置の情報
  /// @param[in] id spec path の種類
  /// @param[in] expr 条件式
  /// @param[in] path_decl パス記述
  void
  new_SpecPath(const FileRegion& fr,
	       tVpiSpecPathType id,
	       PtExpr* expr,
	       PtPathDecl* path_decl);

  /// @brief パス記述の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] edge
  /// @param[in] input_list
  /// @param[in] input_pol
  /// @param[in] op
  /// @param[in] output_list
  /// @param[in] output_pol
  /// @param[in] expr
  /// @param[in] path_delay
  PtPathDecl*
  new_PathDecl(const FileRegion& fr,
	       int edge,
	       PtrList<PtExpr>* input_list,
	       int input_pol,
	       int op,
	       PtrList<PtExpr>* output_list,
	       int output_pol,
	       PtExpr* expr,
	       PtPathDelay* path_delay);

  /// @brief パス記述の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] edge
  /// @param[in] input_list
  /// @param[in] input_pol
  /// @param[in] op
  /// @param[in] output
  /// @param[in] output_pol
  /// @param[in] expr
  /// @param[in] path_delay
  PtPathDecl*
  new_PathDecl(const FileRegion& fr,
	       int edge,
	       PtrList<PtExpr>* input_list,
	       int input_pol,
	       int op,
	       PtExpr* output,
	       int output_pol,
	       PtExpr* expr,
	       PtPathDelay* path_delay);
  
  /// @brief path delay value の生成 (値が1個)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] value 値
  /// @return 生成された path delay value
  PtPathDelay*
  new_PathDelay(const FileRegion& fr,
		PtExpr* value);

  /// @brief path delay value の生成 (値が2個)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] value1 値1
  /// @param[in] value2 値2
  /// @return 生成された path delay value
  PtPathDelay*
  new_PathDelay(const FileRegion& fr,
		PtExpr* value1,
		PtExpr* value2);

  /// @brief path delay value の生成 (値が3個)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] value1 値1
  /// @param[in] value2 値2
  /// @param[in] value3 値3
  /// @return 生成された path delay value
  PtPathDelay*
  new_PathDelay(const FileRegion& fr,
		PtExpr* value1,
		PtExpr* value2,
		PtExpr* value3);

  /// @brief path delay value の生成 (値が6個)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] value1 値1
  /// @param[in] value2 値2
  /// @param[in] value3 値3
  /// @param[in] value4 値4
  /// @param[in] value5 値5
  /// @param[in] value6 値6
  /// @return 生成された path delay value
  PtPathDelay*
  new_PathDelay(const FileRegion& fr,
		PtExpr* value1,
		PtExpr* value2,
		PtExpr* value3,
		PtExpr* value4,
		PtExpr* value5,
		PtExpr* value6);

  /// @brief path delay value の生成 (値が12個)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] value1 値1
  /// @param[in] value2 値2
  /// @param[in] value3 値3
  /// @param[in] value4 値4
  /// @param[in] value5 値5
  /// @param[in] value6 値6
  /// @param[in] value7 値7
  /// @param[in] value8 値8
  /// @param[in] value9 値9
  /// @param[in] value10 値10
  /// @param[in] value11 値11
  /// @param[in] value12 値12
  /// @return 生成された path delay value
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

  
public:
  //////////////////////////////////////////////////////////////////////
  // ステートメント関連の要素の生成
  //////////////////////////////////////////////////////////////////////

  /// @brief disable 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 停止するスコープ名
  /// @return 生成された disable 文
  PtStmt*
  new_Disable(const FileRegion& fr,
	      const char* name,
	      PtrList<PtAttrInst>* ai_list);
  
  /// @brief disable 文の生成 (階層付き識別子)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] hname 階層名
  /// @return 生成された disable 文
  PtStmt*
  new_Disable(const FileRegion& fr,
	      PuHierName* hname,
	      PtrList<PtAttrInst>* ai_list);

  /// @brief enable 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 起動するタスク名
  /// @param[in] arg_array 引数のリスト
  /// @return 生成された enable 文
  PtStmt*
  new_Enable(const FileRegion& fr,
	     const char* name,
	     PtrList<PtExpr>* arg_list,
	     PtrList<PtAttrInst>* ai_list);

  /// @brief enable 文の生成 (階層付き識別子)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] hname 階層名
  /// @param[in] name 末尾の名前
  /// @param[in] arg_array 引数のリスト
  /// @return 生成された enable 文
  PtStmt*
  new_Enable(const FileRegion& fr,
	     PuHierName* hname,
	     PtrList<PtExpr>* arg_list,
	     PtrList<PtAttrInst>* ai_list);

  /// @brief system task enable 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 起動するシステムタスク名
  /// @param[in] arg_array 引数のリスト
  PtStmt*
  new_SysEnable(const FileRegion& fr,
		const char* name,
		PtrList<PtExpr>* arg_list,
		PtrList<PtAttrInst>* ai_list);

  /// @brief delay control 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] delay delay control (delay control 文とは別物)
  /// @param[in] body 本体のステートメント
  /// @return 生成された delay control 文
  PtStmt*
  new_DcStmt(const FileRegion& fr,
	     PtControl* delay,
	     PtStmt* body,
	     PtrList<PtAttrInst>* ai_list);

  /// @brief event control 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] event event control (event control 文とは別物)
  /// @param[in] body 本体のステートメント
  /// @return 生成された event control 文
  PtStmt*
  new_EcStmt(const FileRegion& fr,
	     PtControl* event,
	     PtStmt* body,
	     PtrList<PtAttrInst>* ai_list);

  /// @brief wait 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] cond 条件式
  /// @param[in] body 本体のステートメント
  /// @return 生成された wait 文
  PtStmt*
  new_Wait(const FileRegion& fr,
	   PtExpr* cond,
	   PtStmt* body,
	   PtrList<PtAttrInst>* ai_list);

  /// @brief assign 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] lhs 左辺式
  /// @param[in] rhs 右辺式
  /// @return 生成された assign 文
  PtStmt*
  new_Assign(const FileRegion& fr,
	     PtExpr* lhs,
	     PtExpr* rhs,
	     PtrList<PtAttrInst>* ai_list);

  /// @brief control 付き assign 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] lhs 左辺式
  /// @param[in] rhs 右辺式
  /// @param[in] control 制御式
  /// @return 生成された assign 文
  PtStmt*
  new_Assign(const FileRegion& fr,
	     PtExpr* lhs,
	     PtExpr* rhs,
	     PtControl* control,
	     PtrList<PtAttrInst>* ai_list);

  /// @brief nonblocking assign 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] lhs 左辺式
  /// @param[in] rhs 右辺式
  /// @return 生成された nonblocking assign 文
  PtStmt*
  new_NbAssign(const FileRegion& fr,
	       PtExpr* lhs,
	       PtExpr* rhs,
	       PtrList<PtAttrInst>* ai_list);

  /// @brief control 付き nonblocking assign 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] lhs 左辺式
  /// @param[in] rhs 右辺式
  /// @param[in] control 制御式
  /// @return 生成された nonblocking assign 文
  PtStmt*
  new_NbAssign(const FileRegion& fr,
	       PtExpr* lhs,
	       PtExpr* rhs,
	       PtControl* control,
	       PtrList<PtAttrInst>* ai_list);

  /// @brief event 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] event イベント名
  /// @return 生成された event 文
  PtStmt*
  new_EventStmt(const FileRegion& fr,
		PtExpr* event,
		PtrList<PtAttrInst>* ai_list);

  /// @brief null 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @return 生成された null 文
  PtStmt*
  new_NullStmt(const FileRegion& fr,
	       PtrList<PtAttrInst>* ai_list);

  /// @brief if 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] expr 条件式
  /// @param[in] then_body 成り立ったときに実行されるステートメント
  /// @return 生成された if 文
  PtStmt*
  new_If(const FileRegion& fr,
	 PtExpr* expr,
	 PtStmt* then_body,
	 PtrList<PtAttrInst>* ai_list);

  /// @brief if 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] expr 条件式
  /// @param[in] then_body 成り立ったときに実行されるステートメント
  /// @param[in] else_body 成り立たなかったときに実行されるステートメント
  /// @return 生成された if 文
  PtStmt*
  new_If(const FileRegion& fr,
	 PtExpr* expr,
	 PtStmt* then_body,
	 PtStmt* else_body,
	 PtrList<PtAttrInst>* ai_list);

  /// @brief case 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] expr 条件式
  /// @param[in] caseitem_list case item のリスト
  /// @return 生成された case 文
  PtStmt*
  new_Case(const FileRegion& fr,
	   PtExpr* expr,
	   PtrList<PtCaseItem>* caseitem_list,
	   PtrList<PtAttrInst>* ai_list);

  /// @brief casex 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] expr 条件式
  /// @param[in] caseitem_list case item のリスト
  /// @return 生成された case 文
  PtStmt*
  new_CaseX(const FileRegion& fr,
	    PtExpr* expr,
	    PtrList<PtCaseItem>* caseitem_list,
	    PtrList<PtAttrInst>* ai_list);

  /// @brief casez 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] expr 条件式
  /// @param[in] caseitem_list case item のリスト
  /// @return 生成された case 文
  PtStmt*
  new_CaseZ(const FileRegion& fr,
	    PtExpr* expr,
	    PtrList<PtCaseItem>* caseitem_list,
	    PtrList<PtAttrInst>* ai_list);

  /// @brief case item の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] label_list ラベルのリスト
  /// @param[in] body 本体のステートメント
  /// @return 生成された case item
  PtCaseItem*
  new_CaseItem(const FileRegion& fr,
	       PtrList<PtExpr>* label_list,
	       PtStmt* body);

  /// @brief forever 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] body 本体のステートメント
  /// @return 生成された forever 文
  PtStmt*
  new_Forever(const FileRegion& fr,
	      PtStmt* body,
	      PtrList<PtAttrInst>* ai_list);

  /// @brief repeat 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] expr 繰り返し数を表す式
  /// @param[in] body 本体のステートメント
  /// @return 生成された repeat 文
  PtStmt*
  new_Repeat(const FileRegion& fr,
	     PtExpr* expr,
	     PtStmt* body,
	     PtrList<PtAttrInst>* ai_list);

  /// @brief while 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] cond 繰り返し条件式
  /// @param[in] body 本体のステートメント
  /// @return 生成された while 文
  PtStmt*
  new_While(const FileRegion& fr,
	    PtExpr* cond,
	    PtStmt* body,
	    PtrList<PtAttrInst>* ai_list);

  /// @brief for 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] init 初期化文
  /// @param[in] cond 繰り返し条件式
  /// @param[in] next 増加文
  /// @param[in] body 本体のステートメント
  /// @return 生成された for 文
  PtStmt*
  new_For(const FileRegion& fr,
	  PtStmt* init,
	  PtExpr* cond,
	  PtStmt* next,
	  PtStmt* body,
	  PtrList<PtAttrInst>* ai_list);

  /// @brief procedural assign 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] lhs 左辺式
  /// @param[in] rhs 右辺式
  /// @return 生成された procedural assign 文
  PtStmt*
  new_PcAssign(const FileRegion& fr,
	       PtExpr* lhs,
	       PtExpr* rhs,
	       PtrList<PtAttrInst>* ai_list);
  
  /// @brief deassign 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] lhs 左辺式
  /// @return 生成された deassign 文
  PtStmt*
  new_Deassign(const FileRegion& fr,
	       PtExpr* lhs,
	       PtrList<PtAttrInst>* ai_list);

  /// @brief force 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] lhs 左辺式
  /// @param[in] rhs 右辺式
  /// @return 生成された force 文
  PtStmt*
  new_Force(const FileRegion& fr,
	    PtExpr* lhs,
	    PtExpr* rhs,
	    PtrList<PtAttrInst>* ai_list);

  /// @brief release 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] lhs 左辺式
  /// @return 生成された release 文
  PtStmt*
  new_Release(const FileRegion& fr,
	      PtExpr* lhs,
	      PtrList<PtAttrInst>* ai_list);

  /// @brief parallel block の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] stmt_list 本体のステートメントのリスト
  /// @return 生成された parallel block
  PtStmt*
  new_ParBlock(const FileRegion& fr,
	       PtrList<PtStmt>* stmt_list,
	       PtrList<PtAttrInst>* ai_list);

  /// @brief 名前付き parallel block の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name ブロック名
  /// @param[in] stmt_list 本体のステートメントのリスト
  /// @return 生成された parallel block
  PtStmt*
  new_NamedParBlock(const FileRegion& fr,
		    const char* name,
		    PtrList<PtStmt>* stmt_list,
		    PtrList<PtAttrInst>* ai_list);

  /// @brief sequential block の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] stmt_list 本体のステートメントのリスト
  /// @return 生成された sequential block
  PtStmt*
  new_SeqBlock(const FileRegion& fr,
	       PtrList<PtStmt>* stmt_list,
	       PtrList<PtAttrInst>* ai_list);

  /// @brief 名前付き sequential block の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name ブロック名
  /// @param[in] stmt_list 本体のステートメントのリスト
  /// @return 生成された sequential block
  PtStmt*
  new_NamedSeqBlock(const FileRegion& fr,
		    const char* name,
		    PtrList<PtStmt>* stmt_list,
		    PtrList<PtAttrInst>* ai_list);
  

public:
  //////////////////////////////////////////////////////////////////////
  // 式の生成
  //////////////////////////////////////////////////////////////////////

  /// @brief 単項演算子の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type 演算の種類
  /// @param[in] opr オペランド
  /// @return 生成された演算子
  PtExpr*
  new_Opr(const FileRegion& fr,
	  tVpiOpType type,
	  PtExpr* opr,
	  PtrList<PtAttrInst>* ai_list);

  /// @brief 二項演算子の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type 演算の種類
  /// @param[in] opr1 オペランド1
  /// @param[in] opr2 オペランド2
  /// @return 生成された演算子
  PtExpr*
  new_Opr(const FileRegion& fr,
	  tVpiOpType type,
	  PtExpr* opr1,
	  PtExpr* opr2,
	  PtrList<PtAttrInst>* ai_list);

  /// @brief 三項演算子の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type 演算の種類
  /// @param[in] opr1 オペランド1
  /// @param[in] opr2 オペランド2
  /// @param[in] opr3 オペランド3
  /// @return 生成された演算子
  PtExpr*
  new_Opr(const FileRegion& fr,
	  tVpiOpType type,
	  PtExpr* opr1,
	  PtExpr* opr2,
	  PtExpr* opr3,
	  PtrList<PtAttrInst>* ai_list);

  /// @brief concatination 演算子の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] expr_list オペランドのリスト
  /// @return 生成された concatination 演算子
  PtExpr*
  new_Concat(const FileRegion& fr,
	     PtrList<PtExpr>* expr_list);

  /// @brief multi-concatination 演算子の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] rep 繰り返し数
  /// @param[in] expr_list 結合するオペランドのリスト
  /// @return 生成された multi-concatination 演算子
  PtExpr*
  new_MultiConcat(const FileRegion& fr,
		  PtExpr* rep,
		  PtrList<PtExpr>* expr_list);
  
  /// @brief min/typ/max delay 演算子の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] val0 minimum 値
  /// @param[in] val1 typical 値
  /// @param[in] val2 maximum 値
  /// @return 生成された min/typ/max 演算子
  PtExpr*
  new_MinTypMax(const FileRegion& fr,
		PtExpr* val0,
		PtExpr* val1,
		PtExpr* val2);

  /// @brief primary の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 識別子名
  /// @return 生成された primary
  PtExpr*
  new_Primary(const FileRegion& fr,
	      const char* name);

  /// @brief インデックス付き primary の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 識別子名
  /// @param[in] index_list インデックスのリスト
  /// @return 生成された primary
  PtExpr*
  new_Primary(const FileRegion& fr,
	      const char* name,
	      PtrList<PtExpr>* index_array);

  /// @brief 範囲指定付き primary の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 識別子名
  /// @param[in] mode 範囲指定のモード
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @return 生成された primary
  PtExpr*
  new_Primary(const FileRegion& fr,
	      const char* name,
	      tVpiRangeMode mode,
	      PtExpr* left,
	      PtExpr* right);

  /// @brief インデックスと範囲指定付き primary の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 識別子名
  /// @param[in] index_list インデックスのリスト
  /// @param[in] mode 範囲指定のモード
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @return 生成された primary
  PtExpr*
  new_Primary(const FileRegion& fr,
	      const char* name,
	      PtrList<PtExpr>* index_list,
	      tVpiRangeMode mode,
	      PtExpr* left,
	      PtExpr* right);

  /// @brief primary の生成 (階層付き)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] hname 階層名
  /// @return 生成された primary
  PtExpr*
  new_Primary(const FileRegion& fr,
	      PuHierName* hname);

  /// @brief インデックス付き primary の生成 (階層付き)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] hname 階層名
  /// @param[in] index_list インデックスのリスト
  /// @return 生成された primary
  PtExpr*
  new_Primary(const FileRegion& fr,
	      PuHierName* hname,
	      PtrList<PtExpr>* index_list);

  /// @brief 範囲指定付き primary の生成 (階層付き)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] hname 階層名
  /// @param[in] mode 範囲指定のモード
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @return 生成された primary
  PtExpr*
  new_Primary(const FileRegion& fr,
	      PuHierName* hname,
	      tVpiRangeMode mode,
	      PtExpr* left,
	      PtExpr* right);

  /// @brief インデックスと範囲指定付き primary の生成 (階層付き)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] hname 階層名
  /// @param[in] index_list インデックスのリスト
  /// @param[in] mode 範囲指定のモード
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @return 生成された primary
  PtExpr*
  new_Primary(const FileRegion& fr,
	      PuHierName* hname,
	      PtrList<PtExpr>* index_list,
	      tVpiRangeMode mode,
	      PtExpr* left,
	      PtExpr* right);
  
  /// @brief constant primary の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 識別子名
  /// @param[in] index インデックス
  /// @return 生成された const primary
  PtExpr*
  new_CPrimary(const FileRegion& fr,
	       const char* name,
	       PtExpr* index);
  
  /// @brief constant primary の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 識別子名
  /// @param[in] index_list インデックスのリスト
  /// @return 生成された const primary
  PtExpr*
  new_CPrimary(const FileRegion& fr,
	       const char* name,
	       PtrList<PtExpr>* index_list);

  /// @brief 範囲指定付き constant primary の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 識別子名
  /// @param[in] mode 範囲指定のモード
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @return 生成された constant primary
  PtExpr*
  new_CPrimary(const FileRegion& fr,
	       const char* name,
	       tVpiRangeMode mode,
	       PtExpr* left,
	       PtExpr* right);
  
  /// @brief インデックス付き constant primary の生成 (階層付き)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] hname 階層名
  /// @param[in] index_list インデックスのリスト
  /// @return 生成された constant primary
  PtExpr*
  new_CPrimary(const FileRegion& fr,
	       PuHierName* hname,
	       PtrList<PtExpr>* index_list);
  
  /// @brief function call の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 関数名
  /// @param[in] arg_list 引数のリスト
  /// @return 生成された function call
  PtExpr*
  new_FuncCall(const FileRegion& fr,
	       const char* name,
	       PtrList<PtExpr>* arg_list,
	       PtrList<PtAttrInst>* ai_list);

  /// @brief function call の生成 (階層付き)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] hname 階層名
  /// @param[in] arg_list 引数のリスト
  /// @return 生成された function call
  PtExpr*
  new_FuncCall(const FileRegion& fr,
	       PuHierName* hname,
	       PtrList<PtExpr>* arg_list,
	       PtrList<PtAttrInst>* ai_list);

  /// @brief system function call の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 関数名
  /// @param[in] arg_array 引数のリスト
  /// @return 生成された function call
  PtExpr*
  new_SysFuncCall(const FileRegion& fr,
		  const char* name,
		  PtrList<PtExpr>* arg_list);

  /// @brief 整数型の定数の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] value 定数を表す整数値
  /// @return 生成された定数
  PtExpr*
  new_IntConst(const FileRegion& fr,
	       ymuint32 value);

  /// @brief 整数型の定数の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] value 定数の文字列表現
  /// @return 生成された定数
  PtExpr*
  new_IntConst(const FileRegion& fr,
	       const char* value);

  /// @brief 基底付き整数型の定数の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] const_type 定数の種類
  /// @param[in] value 定数の文字列表現
  /// @return 生成された定数
  PtExpr*
  new_IntConst(const FileRegion& fr,
	       tVpiConstType const_type,
	       const char* value);

  /// @brief サイズと基底付き定数の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] size サイズ
  /// @param[in] const_type 定数の種類
  /// @param[in] value 定数の文字列表現
  /// @return 生成された定数
  PtExpr*
  new_IntConst(const FileRegion& fr,
	       ymuint32 size,
	       tVpiConstType const_type,
	       const char* value);

  /// @brief 実数型の定数の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] value 定数の文字列表現
  /// @return 生成された定数
  PtExpr*
  new_RealConst(const FileRegion& fr,
		double value);

  /// @brief 文字列型の定数の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] value 文字列
  /// @return 生成された定数
  PtExpr*
  new_StringConst(const FileRegion& fr,
		  const char* value);


public:
  //////////////////////////////////////////////////////////////////////
  // その他
  //////////////////////////////////////////////////////////////////////

  /// @brief ディレイコントロールの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] value 遅延を表す式
  /// @return 生成されたディレイコントロール
  PtControl*
  new_DelayControl(const FileRegion& fr,
		   PtExpr* value);

  /// @brief イベントコントロールの生成 (any-event)
  /// @param[in] fr ファイル位置の情報
  /// @return 生成されたイベントコントロール
  PtControl*
  new_EventControl(const FileRegion& fr);

  /// @brief イベントコントロールの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] event_name イベントを表す名前
  /// @return 生成されたイベントコントロール
  PtControl*
  new_EventControl(const FileRegion& fr,
		   const char* event_name,
		   const FileRegion& name_loc);

  /// @brief イベントコントロールの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] event_name イベントを表す階層名
  /// @return 生成されたイベントコントロール
  PtControl*
  new_EventControl(const FileRegion& fr,
		   PuHierName* event_name,
		   const FileRegion& name_loc);

  /// @brief イベントコントロールの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] event_list イベントのリスト
  /// @return 生成されたイベントコントロール
  PtControl*
  new_EventControl(const FileRegion& fr,
		   PtrList<PtExpr>* event_array);

  /// @brief リピートコントロールの生成 (any-event)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] rep 繰り返し数を表す式
  /// @return 生成されたリピートコントロール
  PtControl*
  new_RepeatControl(const FileRegion& fr,
		    PtExpr* rep);

  /// @brief リピートコントロールの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] rep 繰り返し数を表す式
  /// @param[in] event 繰り返しの単位となるイベント
  /// @return 生成されたリピートコントロール
  PtControl*
  new_RepeatControl(const FileRegion& fr,
		    PtExpr* rep,
		    const char* event_name,
		    const FileRegion& name_loc);

  /// @brief リピートコントロールの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] rep 繰り返し数を表す式
  /// @param[in] event 繰り返しの単位となるイベント
  /// @return 生成されたリピートコントロール
  PtControl*
  new_RepeatControl(const FileRegion& fr,
		    PtExpr* rep,
		    PuHierName* event_name,
		    const FileRegion& name_loc);

  /// @brief リピートコントロールの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] rep 繰り返し数を表す式
  /// @param[in] event_list 繰り返しの単位となるイベントのリスト
  /// @return 生成されたリピートコントロール
  PtControl*
  new_RepeatControl(const FileRegion& fr,
		    PtExpr* rep,
		    PtrList<PtExpr>* event_list);

  /// @brief 順序つき結合子の生成
  /// @param[in] expr 結合する式
  /// @return 生成された結合子
  PtConnection*
  new_OrderedCon(PtExpr* expr);

  /// @brief 順序つき結合子の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] expr 結合する式
  /// @return 生成された結合子
  PtConnection*
  new_OrderedCon(const FileRegion& fr,
		 PtExpr* expr,
		 PtrList<PtAttrInst>* ai_list);
  
  /// @brief 名前付き結合子の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] expr 結合する式
  /// @return 生成された結合子
  PtConnection*
  new_NamedCon(const FileRegion& fr,
	       const char* name,
	       PtExpr* expr = NULL,
	       PtrList<PtAttrInst>* ai_list = NULL);

  /// @brief strength の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] value0 '0' の強度
  /// @param[in] value1 '1' の強度
  /// @return 生成された strength
  PtStrength*
  new_Strength(const FileRegion& fr,
	       tVpiStrength value0,
	       tVpiStrength value1);

  /// @brief charge strength の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] value 強度
  /// @return 生成された strength
  PtStrength*
  new_Strength(const FileRegion& fr,
	       tVpiStrength value);

  /// @brief 遅延値の生成 (1つの値)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] value1 値1
  /// @return 生成された遅延値
  PtDelay*
  new_Delay(const FileRegion& fr,
	    PtExpr* value1);

  /// @brief 遅延値の生成 (2つの値)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] value1 値1
  /// @param[in] value2 値2
  /// @return 生成された遅延値
  PtDelay*
  new_Delay(const FileRegion& fr,
	    PtExpr* value1,
	    PtExpr* value2);

  /// @brief 遅延値の生成 (3つの値)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] value1 値1
  /// @param[in] value2 値2
  /// @param[in] value3 値3
  /// @return 生成された遅延値
  PtDelay*
  new_Delay(const FileRegion& fr,
	    PtExpr* value1,
	    PtExpr* value2,
	    PtExpr* value3);

  /// @brief 階層名の生成
  /// @param[in] head_name 階層の上位部分
  /// @param[in] name 階層の最下位部分
  PuHierName*
  new_HierName(const char* head_name,
	       const char* name);

  /// @brief 階層名の生成
  /// @param[in] head_name 階層の上位部分
  /// @param[in] index インデックス
  /// @param[in] name 階層の最下位部分
  PuHierName*
  new_HierName(const char* head_name,
	       int index,
	       const char* name);
  
  /// @brief 階層名の追加
  /// @aram[in] hname 階層名の上位部分
  /// @param[in] name 追加する名前
  void
  add_HierName(PuHierName* hname,
		 const char* name);
  
  /// @brief 階層名の追加
  /// @aram[in] hname 階層名の上位部分
  /// @param[in] index インデックス
  /// @param[in] name 追加する名前
  void
  add_HierName(PuHierName* hname,
	       int index,
	       const char* name);
  
  /// @brief 階層名の取得
  /// @param[in] hname 階層名
  /// @param[out] 階層の上位部分の配列
  /// @return 階層の下位名
  /// @note この関数のなかで hname は削除される．
  const char*
  extract_HierName(PuHierName* hname,
		   PtNameBranchArray& nb_array);

  
  //////////////////////////////////////////////////////////////////////
  // attribute instance 関係
  //////////////////////////////////////////////////////////////////////

  /// @brief attribute instance の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] as_array attribute spec のリスト
  /// @return 生成された attribute instance
  PtAttrInst*
  new_AttrInst(const FileRegion& fr,
	       PtrList<PtAttrSpec>* as_list);

  /// @brief attribute spec の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] expr 値
  /// @return 生成された attribute spec
  PtAttrSpec*
  new_AttrSpec(const FileRegion& fr,
	       const char* name,
	       PtExpr* expr);
  

public:
  //////////////////////////////////////////////////////////////////////
  // Ver2.0 リスト関係
  //////////////////////////////////////////////////////////////////////

  /// @brief モジュール定義の開始
  /// - port list の初期化
  /// - paramport list の初期化
  /// - iohead list の初期化
  /// - paramhead list の初期化
  /// - localparamhead list の初期化
  /// - declhead list の初期化
  /// - item list の初期化
  /// を行う．
  void
  init_module();

  /// @brief モジュール定義の終了
  void
  end_module();
  
  /// @brief UDP定義の開始
  /// - port list の初期化
  /// - iohead list の初期化
  /// - declhead list の初期化
  /// - UDP entry list の初期化
  /// を行う．
  void
  init_udp();

  /// @brief UDP 定義の終了
  void
  end_udp();
  
  /// @brief task/function 定義の開始
  /// - iohead list の初期化
  /// - paramhead list の初期化
  /// - localparamhead list の初期化
  /// - declhead list の初期化
  /// を行う．
  void
  init_tf();

  /// @brief task/function 定義の終了
  void
  end_tf();
  
  /// @brief generate block の開始
  void
  init_generate();

  /// @brief generate block の終了
  void
  end_generate();
  
  /// @brief generate-if の開始
  void
  init_genif();
  
  /// @brief generate-if の else 節の開始
  void
  init_else();

  /// @brief generate-if の終了
  void
  end_genif();
  
  /// @brief block-statment の開始
  void
  init_block();

  /// @brief block-statement の終了
  void
  end_block();
  
  /// @brief ポートリストにポートを追加する．
  void
  add_port(PtiPort* port);

  /// @brief ポートリストを配列に変換する．
  PtiPortArray
  get_port_array();
  
  /// @brief ポート参照リストを初期化する．
  void
  init_portref_list();

  /// @brief ポート参照リストに要素を追加する．
  void
  add_portref(PtiPortRef* portref);

  /// @brief ポート参照リストを配列に変換する
  PtiPortRefArray
  get_portref_array();

  /// @brief parameter port 宣言ヘッダを追加する．
  void
  add_paramport_head(PtiDeclHead* head);

  /// @brief parameter port 宣言の終わり
  void
  end_paramport();
  
  /// @brief parameter port リストを配列に変換する．
  PtDeclHeadArray
  get_paramport_array();
  
  /// @brief IO宣言リストにIO宣言ヘッダを追加する．
  void
  add_io_head(PtiIOHead* head);

  /// @brief IO宣言リストにIO宣言要素を追加する．
  void
  add_io_item(PtIOItem* item);
  
  /// @brief IO宣言の終わり
  void
  end_io();
  
  /// @brief IO宣言リストを配列に変換する．
  PtIOHeadArray
  get_io_array();
  
  /// @brief parameter 宣言ヘッダを追加する．
  void
  add_param_head(PtiDeclHead* head);
  
  /// @param parameter 宣言の終わり
  void
  end_param();
  
  /// @brief parameter リストを配列に変換する．
  PtDeclHeadArray
  get_param_array();
  
  /// @brief localparam 宣言ヘッダを追加する．
  void
  add_localparam_head(PtiDeclHead* head);
  
  /// @param localparam 宣言の終わり
  void
  end_localparam();
  
  /// @brief localparam リストを配列に変換する．
  PtDeclHeadArray
  get_localparam_array();

  /// @brief 宣言リストに宣言ヘッダを追加する．
  void
  add_decl_head(PtiDeclHead* head);

  /// @brief 宣言リストに宣言要素を追加する．
  void
  add_decl_item(PtDeclItem* item);
  
  /// @param 宣言の終わり
  void
  end_decl();
  
  /// @brief 宣言リストを配列に変換する．
  PtDeclHeadArray
  get_decl_array();

  /// @brief generate-if 文の then 節の宣言リストを配列に変換する．
  PtDeclHeadArray
  get_then_decl_array();

  /// @brief generate-if 文の else 節の宣言リストを配列に変換する．
  PtDeclHeadArray
  get_else_decl_array();
  
  /// @brief item リストに要素を追加する．
  void
  add_item(PtItem* item);

  /// @brief item リストを配列に変換する．
  PtItemArray
  get_item_array();

  /// @brief generate-if 文の then 節の item リストを配列に変換する．
  PtItemArray
  get_then_item_array();

  /// @brief generate-if 文の else 節の item リストを配列に変換する．
  PtItemArray
  get_else_item_array();

  /// @brief UdpEntry を追加する．
  void
  add_udp_entry(PtUdpEntry* entry);

  /// @brief UdpEntry のリストを配列に変換する．
  PtUdpEntryArray
  get_udp_entry_array();
  
  /// @brief UdpValue のリストを初期化する．
  void
  init_udp_value_list();

  /// @brief UdpValue を追加する．
  void
  add_udp_value(PtUdpValue* value);

  /// @brief UdpValue のリストを配列に変換する．
  PtUdpValueArray
  get_udp_value_array();
  
  /// @brief defparam リストを初期化する．
  void
  init_defparam();

  /// @brief defparam リストに要素を追加する．
  void
  add_defparam(PtDefParam* defparam);

  /// @brief defparam リストを配列に変換する．
  PtDefParamArray
  get_defparam_array();
  
  /// @brief contassign リストを初期化する．
  void
  init_contassign();

  /// @brief contassign リストに要素を追加する．
  void
  add_contassign(PtContAssign* contassign);

  /// @brief contassign リストを配列に変換する．
  PtContAssignArray
  get_contassign_array();
  
  /// @brief instance リストを初期化する．
  void
  init_inst();

  /// @brief instance リストに要素を追加する．
  void
  add_inst(PtInst* inst);

  /// @brief instance リストを配列に変換する．
  PtInstArray
  get_inst_array();
  
  
public:
  //////////////////////////////////////////////////////////////////////
  // リスト関係
  //////////////////////////////////////////////////////////////////////

  template <typename T>
  PtrList<T, T>*
  new_list();
  
  template <typename T>
  PtArray<T>
  to_array(PtrList<T, T>* list);
  
  /// @brief 式のリストから配列を生成する．(multi_concat用)
  /// @param[in] pre_expr list の前に挿入する式
  /// @note 結果として list は削除される．
  PtExprArray
  ExprArray(PtExpr* pre_expr,
	    PtrList<PtExpr>* list);
  

public:
  
  /// @brief 関数内で使えるステートメントかどうかのチェック
  bool
  check_function_statement(const PtStmt* stmt);

  /// @brief default ラベルが2つ以上含まれていないかどうかのチェック
  bool
  check_default_label(const PtrList<PtCaseItem>* ci_list);
  
  /// @brief 使用されているモジュール名を登録する．
  /// @param[in] name 登録する名前
  void
  reg_defname(const char* name);
  
  /// @brief attribute instance を登録する．
  void
  reg_attrinst(PtBase* ptobj,
	       PtrList<PtAttrInst>* attr_list);
  
  
public:
  //////////////////////////////////////////////////////////////////////
  // メッセージ出力用の便利関数
  //////////////////////////////////////////////////////////////////////

  /// @brief メッセージを出力する．
  /// @param[in] src_file この関数を読んでいるソースファイル名
  /// @param[in] src_line この関数を読んでいるソースの行番号
  /// @param[in] file_loc ファイル位置
  /// @param[in] type メッセージの種類
  /// @param[in] label メッセージラベル
  /// @param[in] body メッセージ本文
  void
  put_msg(const char* src_file,
	  int src_line,
	  const FileRegion& file_loc,
	  tMsgType type,
	  const char* label,
	  const char* msg);
  
  /// @brief メッセージを出力する．
  /// @param[in] src_file この関数を読んでいるソースファイル名
  /// @param[in] src_line この関数を読んでいるソースの行番号
  /// @param[in] file_loc ファイル位置
  /// @param[in] type メッセージの種類
  /// @param[in] label メッセージラベル
  /// @param[in] body メッセージ本文
  void
  put_msg(const char* src_file,
	  int src_line,
	  const FileRegion& file_loc,
	  tMsgType type,
	  const char* label,
	  const string& msg);
  

public:

  /// @brief lex オブジェクトの取得
  /// @return lex オブジェクトを返す．
  Lex&
  lex();
  
  /// @brief yylex とのインターフェイス
  /// @param[out] lvalp 値を格納する変数
  /// @param[out] llocp 位置情報を格納する変数
  /// @return 読み込んだトークンの id を返す．
  int
  yylex(YYSTYPE& lval,
	FileRegion& lloc);
  

private:
  
  typedef PtrList<PtiDeclHead, PtDeclHead> PtDeclHeadList;
  typedef PtrList<PtItem, PtItem> PtItemList;
  
  /// @brief 宣言要素リストを最後の宣言ヘッダにセットする．
  void
  flush_declitem_list(PtDeclHeadList& head_list);
  
  /// @brief 新たな declhead リストを生成する．
  PtDeclHeadList*
  new_declhead_list();

  /// @brief declhead リストを削除する．
  void
  delete_declhead_list(PtDeclHeadList* list);

  /// @brief 新たな item リストを生成する．
  PtItemList*
  new_item_list();

  /// @brief item リストを削除する．
  void
  delete_item_list(PtItemList* list);

  /// @brief 現在の iohead リストをスタックに積む．
  void
  push_iohead_list();

  /// @brief スタックの末尾を iohead リストに戻す．
  void
  pop_iohead_list();
  
  /// @brief 現在の paramhead リストをスタックに積む．
  void
  push_paramhead_list();

  // @brief スタックの末尾を paramhead リストに戻す．
  void
  pop_paramhead_list();
  
  /// @brief 現在の declhead リストをスタックに積む．
  void
  push_declhead_list();
  
  /// @brief スタックの末尾を declhead リストに戻す．
  void
  pop_declhead_list();

  /// @brief 現在の item リストをスタックに積む．
  void
  push_item_list();

  /// @brief スタックの末尾を item リストに戻す．
  void
  pop_item_list();

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // メッセージマネージャ
  MsgMgr& mMsgMgr;
  
  // パース木を保持するクラス
  PtMgr& mPtMgr;

  // 本体のメモリアロケータ(配列確保用)
  AllocBase& mAlloc;
  
  // パース木の要素の生成を行うクラス
  PtiFactory& mFactory;

  // 字句解析を行うオブジェクト
  Lex* mLex;

  // PuList<> のメモリ確保用オブジェクト
  FragAlloc mTmpAlloc;
  
  // PtrList 用のアロケータ
  UnitAlloc mCellAlloc;
  

public:
  //////////////////////////////////////////////////////////////////////
  // 静的に存在するリスト
  //////////////////////////////////////////////////////////////////////

  // ポートリスト
  PtrList<PtiPort> mPortList;
  
  // ポート参照リスト
  PtrList<PtiPortRef> mPortRefList;
  
  // parameter port 宣言ヘッダリスト
  PtDeclHeadList mParamPortHeadList;
  
  // モジュール用 IO宣言ヘッダリスト
  PtrList<PtiIOHead, PtIOHead> mModuleIOHeadList;
  
  // task/function 用 IO宣言ヘッダリスト
  PtrList<PtiIOHead, PtIOHead> mTfIOHeadList;
  
  // モジュール用の parameter 宣言ヘッダリスト
  PtDeclHeadList mModuleParamHeadList;

  // task/function 用の parameter 宣言ヘッダリスト
  PtDeclHeadList mTfParamHeadList;
  
  // モジュール用の localparam 宣言ヘッダリスト
  PtDeclHeadList mModuleLparamHeadList;

  // task/function 用の localparam 宣言ヘッダリスト
  PtDeclHeadList mTfLparamHeadList;
  
  // モジュール用の宣言ヘッダリスト
  PtDeclHeadList mModuleDeclHeadList;

  // task/function 用の宣言ヘッダリスト
  PtDeclHeadList mTfDeclHeadList;
  
  // モジュール用の item リスト
  PtItemList mModuleItemList;
  
  // IO宣言要素リスト
  PtrList<PtIOItem> mIOItemList;

  // 宣言要素リスト
  PtrList<PtDeclItem> mDeclItemList;
  
  // UDP エントリのリスト
  PtrList<PtUdpEntry> mUdpEntryList;

  // UDP のテーブルの値のリスト
  PtrList<PtUdpValue> mUdpValueList;
  
  // defparam 要素のリスト
  PtrList<PtDefParam> mDefParamList;
  
  // contassign リスト
  PtrList<PtContAssign> mContAssignList;
  
  // instance リスト
  PtrList<PtInst> mInstList;
  
  
public:
  //////////////////////////////////////////////////////////////////////
  // 現在，登録対象となっているリストを指すポインタ
  //////////////////////////////////////////////////////////////////////

  // 現在の IO宣言ヘッダリスト
  // 実際には mModuleIOHeadList か mTfIOHeadList を指す．
  PtrList<PtiIOHead, PtIOHead>* mCurIOHeadList;

  // 現在の parameter 宣言ヘッダリスト
  PtDeclHeadList* mCurParamHeadList;

  // 現在の localparam 宣言ヘッダリスト
  PtDeclHeadList* mCurLparamHeadList;

  // 現在の宣言ヘッダリスト
  PtDeclHeadList* mCurDeclHeadList;

  // 現在の item リスト
  PtItemList* mCurItemList;
  
  // 現在の then 宣言ヘッダリスト
  PtrList<PtiDeclHead, PtDeclHead>* mCurThenDeclHeadList;

  // 現在の then item リスト
  PtItemList* mCurThenItemList;
  
  // 現在の else 宣言ヘッダリスト
  PtrList<PtiDeclHead, PtDeclHead>* mCurElseDeclHeadList;

  // 現在の else item リスト
  PtItemList* mCurElseItemList;

  
public:
  //////////////////////////////////////////////////////////////////////
  // mCurXXXList のスタック
  //////////////////////////////////////////////////////////////////////

  // IO 宣言ヘッダリストのスタック
  vector<PtrList<PtiIOHead, PtIOHead>*> mIOHeadListStack;
  
  // parameter 宣言ヘッダリストのスタック
  vector<PtDeclHeadList*> mParamHeadListStack;
  
  // localparam 宣言ヘッダリストのスタック
  vector<PtDeclHeadList*> mLparamHeadListStack;
  
  // 宣言ヘッダリストのスタック
  vector<PtDeclHeadList*> mDeclHeadListStack;
  
  // item リストのスタック
  vector<PtItemList*> mItemListStack;
  
  // then 宣言ヘッダリストのスタック
  vector<PtDeclHeadList*> mThenDeclHeadListStack;
  
  // then item リストのスタック
  vector<PtItemList*> mThenItemListStack;
  
  // else 宣言ヘッダリストのスタック
  vector<PtDeclHeadList*> mElseDeclHeadListStack;
  
  // else item リストのスタック
  vector<PtItemList*> mElseItemListStack;
  
};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief lex オブジェクトの取得
// @return lex オブジェクトを返す．
inline
Lex&
Parser::lex()
{
  return *mLex;
}

// @brief メッセージを出力する．
// @param[in] src_file この関数を読んでいるソースファイル名
// @param[in] src_line この関数を読んでいるソースの行番号
// @param[in] file_loc ファイル位置
// @param[in] type メッセージの種類
// @param[in] label メッセージラベル
// @param[in] body メッセージ本文
inline
void
Parser::put_msg(const char* src_file,
		int src_line,
		const FileRegion& file_loc,
		tMsgType type,
		const char* label,
		const char* msg)
{
  mMsgMgr.put_msg(src_file, src_line, file_loc, type, label, msg);
}
  
// @brief メッセージを出力する．
// @param[in] src_file この関数を読んでいるソースファイル名
// @param[in] src_line この関数を読んでいるソースの行番号
// @param[in] file_loc ファイル位置
// @param[in] type メッセージの種類
// @param[in] label メッセージラベル
// @param[in] body メッセージ本文
inline
void
Parser::put_msg(const char* src_file,
		int src_line,
		const FileRegion& file_loc,
		tMsgType type,
		const char* label,
		const string& msg)
{
  mMsgMgr.put_msg(src_file, src_line, file_loc, type, label, msg);
}
  
template <typename T>
inline
PtrList<T, T>*
Parser::new_list()
{
  void* p = mTmpAlloc.get_memory(sizeof(PtrList<T, T>));
  return new (p) PtrList<T, T>(mCellAlloc);
}

template <typename T>
inline
PtArray<T>
Parser::to_array(PtrList<T, T>* list)
{
  if ( list ) {
    PtArray<T> array = list->to_array(mAlloc);

    list->~PtrList<T, T>();
    mTmpAlloc.put_memory(sizeof(PtrList<T, T>), list);

    return array;
  }
  else {
    return PtArray<T>();
  }
}

// @brief parameter port リストを配列に変換する．
inline
PtDeclHeadArray
Parser::get_paramport_array()
{
  return mParamPortHeadList.to_array(mAlloc);
}

// @brief IO宣言リストを配列に変換する．
inline
PtIOHeadArray
Parser::get_io_array()
{
  return mCurIOHeadList->to_array(mAlloc);
}

// @brief parameter リストを配列に変換する．
inline
PtDeclHeadArray
Parser::get_param_array()
{
  return mCurParamHeadList->to_array(mAlloc);
}

// @brief localparam リストを配列に変換する．
inline
PtDeclHeadArray
Parser::get_localparam_array()
{
  return mCurLparamHeadList->to_array(mAlloc);
}

// @brief 宣言リストを配列に変換する．
inline
PtDeclHeadArray
Parser::get_decl_array()
{
  return mCurDeclHeadList->to_array(mAlloc);
}

// @brief item リストを配列に変換する．
inline
PtItemArray
Parser::get_item_array()
{
  return mCurItemList->to_array(mAlloc);
}
  
// @brief 新たな declhead リストを生成する．
inline
Parser::PtDeclHeadList*
Parser::new_declhead_list()
{
  return new PtDeclHeadList(mCellAlloc);
}

// @brief declhead リストを削除する．
inline
void
Parser::delete_declhead_list(PtDeclHeadList* list)
{
  delete list;
}

// @brief 新たな item リストを生成する．
inline
Parser::PtItemList*
Parser::new_item_list()
{
  return new PtItemList(mCellAlloc);
}

// @brief item リストを削除する．
inline
void
Parser::delete_item_list(PtItemList* list)
{
  delete list;
}

// @brief 現在の iohead リストをスタックに積む．
inline
void
Parser::push_iohead_list()
{
  mIOHeadListStack.push_back(mCurIOHeadList);
}

// @brief スタックの末尾を iohead リストに戻す．
inline
void
Parser::pop_iohead_list()
{
  mCurIOHeadList = mIOHeadListStack.back();
  mIOHeadListStack.pop_back();
}

// @brief 現在の paramhead リストをスタックに積む．
inline
void
Parser::push_paramhead_list()
{
  mParamHeadListStack.push_back(mCurParamHeadList);
  mLparamHeadListStack.push_back(mCurLparamHeadList);
}

// @brief スタックの末尾を paramhead リストに戻す．
inline
void
Parser::pop_paramhead_list()
{
  mCurParamHeadList = mParamHeadListStack.back();
  mParamHeadListStack.pop_back();

  mCurLparamHeadList = mLparamHeadListStack.back();
  mLparamHeadListStack.pop_back();
}

// @brief 現在の declhead リストをスタックに積む．
inline
void
Parser::push_declhead_list()
{
  mDeclHeadListStack.push_back(mCurDeclHeadList);
}

// @brief スタックの末尾を declhead リストに戻す．
inline
void
Parser::pop_declhead_list()
{
  mCurDeclHeadList = mDeclHeadListStack.back();
  mDeclHeadListStack.pop_back();
}

// @brief 現在の item リストをスタックに積む．
inline
void
Parser::push_item_list()
{
  mItemListStack.push_back(mCurItemList);
}

// @brief スタックの末尾を item リストに戻す．
inline
void
Parser::pop_item_list()
{
  mCurItemList = mItemListStack.back();
  mItemListStack.pop_back();
}

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_PARSER_PARSER_H
