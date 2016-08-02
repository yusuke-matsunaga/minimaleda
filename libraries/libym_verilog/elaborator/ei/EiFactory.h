#ifndef LIBYM_VERILOG_ELABORATOR_EI_EIFACTORY_H
#define LIBYM_VERILOG_ELABORATOR_EI_EIFACTORY_H

/// @file libym_verilog/elaborator/ei/EiFactory.h
/// @brief EiFactory のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiFactory.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ElbFactory.h"
#include "ym_utils/Alloc.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// EiObj の派生クラスを生成するファクトリクラス
//////////////////////////////////////////////////////////////////////
class EiFactory :
  public ElbFactory
{
public:

  /// @brief コンストラクタ
  /// @param[in] alloc メモリ確保用のアロケータ
  EiFactory(AllocBase& alloc);

  /// @brief デストラクタ
  virtual
  ~EiFactory();


public:
  //////////////////////////////////////////////////////////////////////
  // 管理用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内訳を表示する．
  virtual
  void
  dump_prof(ostream& s);


public:
  //////////////////////////////////////////////////////////////////////
  // インスタンス生成関係のメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief トップレベルのスコープを生成する
  /// @param[in] alloc アロケータ
  virtual
  const VlNamedObj*
  new_Toplevel();

  /// @brief generate block を生成する．
  /// @param[in] parent 親のスコープ環境
  /// @param[in] pt_item 対応するパース木の要素
  virtual
  ElbScope*
  new_GenBlock(const VlNamedObj* parent,
	       const PtItem* pt_item);

  /// @brief GfBlock 検索用の親の名前付きオブジェクトを作る．
  virtual
  ElbGfRoot*
  new_GfRoot(const VlNamedObj* parent,
	     const PtItem* pt_item);

  /// @brief generate for block を生成する．
  /// @param[in] parent 親のスコープ環境
  /// @param[in] pt_item 対応するパース木の要素
  /// @param[in] int gvi 対応する genvar の値
  virtual
  ElbScope*
  new_GfBlock(const VlNamedObj* parent,
	      const PtItem* pt_item,
	      int gvi);

  /// @brief UDP定義を生成する．
  /// @param[in] pt_udp パース木の UDP 定義
  /// @param[in] is_protected プロテクト属性
  virtual
  ElbUdpDefn*
  new_UdpDefn(const PtUdp* pt_udp,
	      bool is_protected);

  /// @brief module を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_module モジュールテンプレート
  /// @param[in] pt_head パース木のヘッダ定義
  /// @param[in] pt_inst パース木のインスタンス定義
  virtual
  ElbModule*
  new_Module(const VlNamedObj* parent,
	     const PtModule* pt_module,
	     const PtItem* pt_head,
	     const PtInst* pt_inst);

  /// @brief module array を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_module モジュールテンプレート
  /// @param[in] pt_head パース木のヘッダ定義
  /// @param[in] pt_inst パース木のインスタンス定義
  /// @param[in] left 範囲の MSB の式
  /// @param[in] right 範囲の LSB の式
  /// @param[in] left_val 範囲の MSB の値
  /// @param[in] right_val 範囲の LSB の値
  virtual
  ElbModuleArray*
  new_ModuleArray(const VlNamedObj* parent,
		  const PtModule* pt_module,
		  const PtItem* pt_head,
		  const PtInst* pt_inst,
		  ElbExpr* left,
		  ElbExpr* right,
		  int left_val,
		  int right_val);

  /// @brief module IO ヘッダを生成する．
  /// @param[in] module 親のモジュール
  /// @param[in] pt_header パース木のIO宣言ヘッダ
  virtual
  ElbIOHead*
  new_ModIOHead(ElbModule* module,
		const PtIOHead* pt_header);

  /// @brief タスク用の IO ヘッダを生成する．
  /// @param[in] task 親のタスク
  /// @param[in] pt_header パース木のIO宣言ヘッダ
  virtual
  ElbIOHead*
  new_TaskIOHead(ElbTaskFunc* task,
		 const PtIOHead* pt_header);

  /// @brief 関数用の IO ヘッダを生成する．
  /// @param[in] func 親の関数
  /// @param[in] pt_header パース木のIO宣言ヘッダ
  virtual
  ElbIOHead*
  new_FunctionIOHead(ElbTaskFunc* func,
		     const PtIOHead* pt_header);

  /// @brief 宣言要素のヘッダを生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head パース木の宣言ヘッダ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] left_val 範囲の MSB の値
  /// @param[in] right_val 範囲の LSB の値
  /// @param[in] has_delay 遅延値を持つとき true
  virtual
  ElbDeclHead*
  new_DeclHead(const VlNamedObj* parent,
	       const PtDeclHead* pt_head,
	       ElbExpr* left,
	       ElbExpr* right,
	       int left_val,
	       int right_val,
	       bool has_delay = false);

  /// @brief 宣言要素のヘッダを生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head パース木の宣言ヘッダ
  /// @param[in] has_delay 遅延値を持つとき true
  virtual
  ElbDeclHead*
  new_DeclHead(const VlNamedObj* parent,
	       const PtDeclHead* pt_head,
	       bool has_delay = false);

  /// @brief 宣言要素のヘッダを生成する．(IODecl 中の宣言用)
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head パース木のIO宣言ヘッダ
  /// @param[in] aux_type 補助的なデータ型
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] left_val 範囲の MSB の値
  /// @param[in] right_val 範囲の LSB の値
  /// @note 範囲なしの時には left と right に NULL を入れる．
  virtual
  ElbDeclHead*
  new_DeclHead(const VlNamedObj* parent,
	       const PtIOHead* pt_head,
	       tVpiAuxType aux_type,
	       ElbExpr* left,
	       ElbExpr* right,
	       int left_val,
	       int right_val);

  /// @brief 宣言要素のヘッダを生成する．(IODecl 中の宣言用)
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head パース木のIO宣言ヘッダ
  /// @param[in] aux_type 補助的なデータ型
  virtual
  ElbDeclHead*
  new_DeclHead(const VlNamedObj* parent,
	       const PtIOHead* pt_head,
	       tVpiAuxType aux_type);

  /// @brief 宣言要素のヘッダを生成する．(function の暗黙宣言用)
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_item パース木の関数定義
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] left_val 範囲の MSB の値
  /// @param[in] right_val 範囲の LSB の値
  virtual
  ElbDeclHead*
  new_DeclHead(const VlNamedObj* parent,
	       const PtItem* pt_item,
	       ElbExpr* left,
	       ElbExpr* right,
	       int left_val,
	       int right_val);

  /// @brief 宣言要素のヘッダを生成する．(function の暗黙宣言用)
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_item パース木の関数定義
  virtual
  ElbDeclHead*
  new_DeclHead(const VlNamedObj* parent,
	       const PtItem* pt_item);

  /// @brief 宣言要素を生成する．
  /// @param[in] head ヘッダ
  /// @param[in] pt_item パース木の宣言要素
  /// @param[in] init 初期値
  /// @param[in] is_local localparam の時 true
  virtual
  ElbDecl*
  new_Decl(ElbDeclHead* head,
	   const PtNamedBase* pt_item,
	   ElbExpr* init = NULL);

  /// @brief 暗黙のネットを生成する．
  /// @param[in] pt_expr パース木のプライマリ式
  virtual
  ElbDecl*
  new_ImpNet(const VlNamedObj* parent,
	     const PtExpr* pt_expr,
	     tVpiNetType net_type);

  /// @brief 範囲の配列を生成する．
  /// @param[in] dim_size 要素数
  virtual
  ElbRange*
  new_RangeArray(ymuint dim_size);

  /// @brief 宣言要素の配列を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] head ヘッダ
  /// @param[in] pt_item パース木の宣言要素
  /// @param[in] range_src 範囲の配列
  virtual
  ElbDeclArray*
  new_DeclArray(ElbDeclHead* head,
		const PtNamedBase* pt_item,
		const vector<ElbRangeSrc>& range_src);

  /// @brief 宣言要素を生成する(配列の要素)．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] parent_array 親の配列
  /// @param[in] index_list インデックスのリスト
  virtual
  ElbDecl*
  new_DeclElem(const PtBase* pt_expr,
	       ElbDeclArray* parent_array,
	       const vector<ElbExpr*>& index_list);

  /// @brief parameter 宣言のヘッダを生成する(範囲指定なし)．
  /// @param[in] parent 親のスコープ
  virtual
  ElbParamHead*
  new_ParamHead(const VlNamedObj* parent,
		const PtDeclHead* pt_head);

  /// @brief parameter 宣言のヘッダを生成する(範囲指定あり)．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head パース木の宣言ヘッダ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] left_val 範囲の MSB の値
  /// @param[in] right_val 範囲の LSB の値
  virtual
  ElbParamHead*
  new_ParamHead(const VlNamedObj* parent,
		const PtDeclHead* pt_head,
		ElbExpr* left,
		ElbExpr* right,
		int left_val,
		int right_val);

  /// @brief parameter 宣言を生成する．
  /// @param[in] head ヘッダ
  /// @param[in] pt_item パース木の宣言要素
  /// @param[in] init 初期値
  /// @param[in] is_local localparam の時 true
  virtual
  ElbDecl*
  new_Parameter(ElbParamHead* head,
		const PtNamedBase* pt_item,
		bool is_local = false);

  /// @brief genvar を生成する．
  /// @param[in] parent 親のスコープ環境
  /// @param[in] pt_item 対応するパース木の要素
  /// @param[in] val 初期値
  virtual
  ElbGenvar*
  new_Genvar(const VlNamedObj* parent,
	     const PtDeclItem* pt_item,
	     int val);

  /// @brief continuous assignment のヘッダを生成する．
  /// @param[in] module 親のモジュール
  /// @param[in] pt_head パース木のヘッダ定義
  /// @param[in] delay 遅延値
  /// @note 遅延を持たないときは NULL を与える．
  virtual
  ElbCaHead*
  new_CaHead(const VlModule* module,
	     const PtItem* pt_head,
	     ElbDelay* delay = NULL);

  /// @brief continuous assignment を生成する．
  /// @param[in] head ヘッダ
  /// @param[in] pt_obj 対応するパース木中の定義要素
  /// @param[in] lhs 左辺式
  /// @param[in] rhs 右辺式
  virtual
  ElbContAssign*
  new_ContAssign(ElbCaHead* head,
		 const PtBase* pt_obj,
		 ElbExpr* lhs,
		 ElbExpr* rhs);

  /// @brief net 宣言中の continuous assignment を生成する．
  /// @param[in] module 親のモジュール
  /// @param[in] pt_obj パース木の定義要素
  /// @param[in] lhs 左辺式
  /// @param[in] rhs 右辺式
  virtual
  ElbContAssign*
  new_ContAssign(const VlModule* module,
		 const PtBase* pt_obj,
		 ElbExpr* lhs,
		 ElbExpr* rhs);

  /// @brief パラメータ割り当て文を生成する．
  /// @param[in] module 親のモジュール
  /// @param[in] param 対象の parameter
  /// @param[in] rhs 割り当て式の右辺
  /// @param[in] named_con 名前による割り当ての時 true
  virtual
  ElbParamAssign*
  new_ParamAssign(const VlModule* module,
		  const PtBase* pt_obj,
		  ElbDecl* param,
		  ElbExpr* rhs,
		  bool named_con);

  /// @brief defparam 文を生成する．
  /// @param[in] module 親のモジュール
  /// @param[in] pt_header パース木の defparam ヘッダ
  /// @param[in] pt_item パース木の defparam 定義
  /// @param[in] param 対象の parameter
  /// @param[in] rhs 割り当て式の右辺
  virtual
  ElbDefParam*
  new_DefParam(const VlModule* module,
	       const PtItem* pt_header,
	       const PtDefParam* pt_defparam,
	       ElbDecl* param,
	       ElbExpr* rhs);

  /// @brief ゲートプリミティブのヘッダを生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_header パース木の定義
  /// @param[in] has_delay 遅延値を持つとき true
  virtual
  ElbPrimHead*
  new_PrimHead(const VlNamedObj* parent,
	       const PtItem* pt_header,
	       bool has_delay);

  /// @brief UDPプリミティブのヘッダを生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_header パース木の定義
  /// @param[in] udp UDP 定義
  /// @param[in] has_delay 遅延値を持つとき true
  virtual
  ElbPrimHead*
  new_UdpHead(const VlNamedObj* parent,
	      const PtItem* pt_header,
	      const ElbUdpDefn* udp,
	      bool has_delay);

  /// @brief プリミティブインスタンスを生成する．
  /// @param[in] head ヘッダ
  /// @param[in] pt_inst インスタンス定義
  virtual
  ElbPrimitive*
  new_Primitive(ElbPrimHead* head,
		const PtInst* pt_inst);

  /// @brief プリミティブ配列インスタンスを生成する．
  /// @param[in] head ヘッダ
  /// @param[in] pt_inst インスタンス定義
  /// @param[in] left 範囲の MSB の式
  /// @param[in] right 範囲の LSB の式
  /// @param[in] left_val 範囲の MSB の値
  /// @param[in] right_val 範囲の LSB の値
  virtual
  ElbPrimArray*
  new_PrimitiveArray(ElbPrimHead* head,
		     const PtInst* pt_inst,
		     ElbExpr* left,
		     ElbExpr* right,
		     int left_val,
		     int right_val);

  /// @brief function を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_item パース木の定義
  /// @param[in] left 範囲の MSB の式
  /// @param[in] right 範囲の LSB の式
  /// @param[in] left_val 範囲の MSB の値
  /// @param[in] right_val 範囲の LSB の値
  virtual
  ElbTaskFunc*
  new_Function(const VlNamedObj* parent,
	       const PtItem* pt_item,
	       ElbExpr* left,
	       ElbExpr* right,
	       int left_val,
	       int right_val);

  /// @brief function を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_item パース木の定義
  virtual
  ElbTaskFunc*
  new_Function(const VlNamedObj* parent,
	       const PtItem* pt_item);

  /// @brief task を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_item パース木の定義
  virtual
  ElbTaskFunc*
  new_Task(const VlNamedObj* parent,
	   const PtItem* pt_item);

  /// @brief プロセス文を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_item パース木の要素定義
  virtual
  ElbProcess*
  new_Process(const VlNamedObj* parent,
	      const PtItem* pt_item);

  /// @brief ステートメントの配列を生成する．
  /// @param[in] stmt_num 要素数
  virtual
  ElbStmt**
  new_StmtList(ymuint stmt_num);

  /// @brief 代入文を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] lhs 左辺の式
  /// @param[in] rhs 右辺の式
  /// @param[in] block ブロッキング代入の時 true
  /// @param[in] control コントロール
  virtual
  ElbStmt*
  new_Assignment(const VlNamedObj* parent,
		 ElbProcess* process,
		 const PtStmt* pt_stmt,
		 ElbExpr* lhs,
		 ElbExpr* rhs,
		 bool block,
		 ElbControl* control = NULL);

  /// @brief assign ステートメントを生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] lhs 左辺の式
  /// @param[in] rhs 右辺の式
  virtual
  ElbStmt*
  new_AssignStmt(const VlNamedObj* parent,
		 ElbProcess* process,
		 const PtStmt* pt_stmt,
		 ElbExpr* lhs,
		 ElbExpr* rhs);

  /// @brief deassign ステートメントを生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] lhs 左辺の式
  virtual
  ElbStmt*
  new_DeassignStmt(const VlNamedObj* parent,
		   ElbProcess* process,
		   const PtStmt* pt_stmt,
		   ElbExpr* lhs);

  /// @brief force ステートメントを生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] lhs 左辺の式
  /// @param[in] rhs 右辺の式
  virtual
  ElbStmt*
  new_ForceStmt(const VlNamedObj* parent,
		ElbProcess* process,
		const PtStmt* pt_stmt,
		ElbExpr* lhs,
		ElbExpr* rhs);

  /// @brief release ステートメントを生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] lhs 左辺の式
  virtual
  ElbStmt*
  new_ReleaseStmt(const VlNamedObj* parent,
		  ElbProcess* process,
		  const PtStmt* pt_stmt,
		  ElbExpr* lhs);

  /// @brief begin ブロックを生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] stmt_list 子のステートメントリスト
  virtual
  ElbStmt*
  new_Begin(const VlNamedObj* parent,
	    ElbProcess* process,
	    const PtStmt* pt_stmt,
	    ElbStmt** stmt_list);

  /// @brief fork ブロックを生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] stmt_list 子のステートメントリスト
  virtual
  ElbStmt*
  new_Fork(const VlNamedObj* parent,
	   ElbProcess* process,
	   const PtStmt* pt_stmt,
	   ElbStmt** stmt_list);

  /// @breif statement block を生成する．
  /// @param[in] parent 親のスコープ環境
  /// @param[in] pt_stmt 対応するパース木の要素
  virtual
  ElbScope*
  new_StmtScope(const VlNamedObj* parent,
		const PtStmt* pt_stmt);

  /// @brief 名前付き begin ブロックを生成する．
  /// @param[in] block 自分自身に対応するスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] stmt_list 子のステートメントリスト
  virtual
  ElbStmt*
  new_NamedBegin(const VlNamedObj* block,
		 ElbProcess* process,
		 const PtStmt* pt_stmt,
		 ElbStmt** stmt_list);


  /// @brief 名前付き fork ブロックを生成する．
  /// @param[in] block 自分自身に対応するスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] stmt_list 子のステートメントリスト
  virtual
  ElbStmt*
  new_NamedFork(const VlNamedObj* block,
		ElbProcess* process,
		const PtStmt* pt_stmt,
		ElbStmt** stmt_list);

  /// @brief while 文を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] cond 条件式
  /// @param[in] stmt 本体のステートメント
  virtual
  ElbStmt*
  new_WhileStmt(const VlNamedObj* parent,
		ElbProcess* process,
		const PtStmt* pt_stmt,
		ElbExpr* cond,
		ElbStmt* stmt);

  /// @brief repeat 文を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] cond 条件式
  /// @param[in] stmt 本体のステートメント
  virtual
  ElbStmt*
  new_RepeatStmt(const VlNamedObj* parent,
		ElbProcess* process,
		const PtStmt* pt_stmt,
		ElbExpr* cond,
		ElbStmt* stmt);

  /// @brief wait 文を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] cond 条件式
  /// @param[in] stmt 本体のステートメント
  virtual
  ElbStmt*
  new_WaitStmt(const VlNamedObj* parent,
	       ElbProcess* process,
	       const PtStmt* pt_stmt,
	       ElbExpr* cond,
	       ElbStmt* stmt);

  /// @brief for 文を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] cond 条件式
  /// @param[in] init_stmt 初期化式
  /// @param[in] inc_stmt 増加式
  /// @param[in] stmt 本体のステートメント
  virtual
  ElbStmt*
  new_ForStmt(const VlNamedObj* parent,
	      ElbProcess* process,
	      const PtStmt* pt_stmt,
	      ElbExpr* cond,
	      ElbStmt* init_stmt,
	      ElbStmt* inc_stmt,
	      ElbStmt* stmt);

  /// @brief forever 文を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] stmt 本体のステートメント
  virtual
  ElbStmt*
  new_ForeverStmt(const VlNamedObj* parent,
		  ElbProcess* process,
		  const PtStmt* pt_stmt,
		  ElbStmt* stmt);

  /// @brief if 文を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] cond 条件式
  /// @param[in] stmt 本体のステートメント
  /// @param[in] else_stmt else節のステートメント
  /// @note else_stmt は NULL もありうる．
  virtual
  ElbStmt*
  new_IfStmt(const VlNamedObj* parent,
	     ElbProcess* process,
	     const PtStmt* pt_stmt,
	     ElbExpr* cond,
	     ElbStmt* stmt,
	     ElbStmt* else_stmt);

  /// @brief case 文を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] expr 条件式
  virtual
  ElbStmt*
  new_CaseStmt(const VlNamedObj* parent,
	       ElbProcess* process,
	       const PtStmt* pt_stmt,
	       ElbExpr* expr);

  /// @brief イベント文を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] named_event 対象のイベント
  virtual
  ElbStmt*
  new_EventStmt(const VlNamedObj* parent,
		ElbProcess* process,
		const PtStmt* pt_stmt,
		ElbDecl* named_event);

  /// @brief NULL ステートメントを生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] pt_stmt パース木のステートメント定義
  virtual
  ElbStmt*
  new_NullStmt(const VlNamedObj* parent,
	       ElbProcess* process,
	       const PtStmt* pt_stmt);

  /// @brief タスクコール文を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] task 対象のタスク
  /// @param[in] arg_array 引数の配列
  virtual
  ElbStmt*
  new_TaskCall(const VlNamedObj* parent,
	       ElbProcess* process,
	       const PtStmt* pt_stmt,
	       ElbTaskFunc* task,
	       ElbExpr** arg_array);

  /// @brief システムタスクコール文を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] user_systf システムタスク
  /// @param[in] arg_array 引数の配列
  virtual
  ElbStmt*
  new_SysTaskCall(const VlNamedObj* parent,
		  ElbProcess* process,
		  const PtStmt* pt_stmt,
		  const ElbUserSystf* user_systf,
		  ElbExpr** arg_array);

  /// @brief disable 文を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] target 対象のスコープ
  virtual
  ElbStmt*
  new_DisableStmt(const VlNamedObj* parent,
		  ElbProcess* process,
		  const PtStmt* pt_stmt,
		  const VlNamedObj* target);

  /// @brief コントロール文を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] control コントロール
  /// @param[in] stmt 本体のステートメント
  virtual
  ElbStmt*
  new_CtrlStmt(const VlNamedObj* parent,
	       ElbProcess* process,
	       const PtStmt* pt_stmt,
	       ElbControl* control,
	       ElbStmt* stmt);

  /// @brief 遅延コントロールを生成する．
  /// @param[in] pt_control パース木の定義要素
  /// @param[in] delay 遅延式
  virtual
  ElbControl*
  new_DelayControl(const PtControl* pt_control,
		   ElbExpr* delay);

  /// @brief イベントコントロールを生成する．
  /// @param[in] pt_control パース木の定義要素
  /// @param[in] event_num イベントリストの要素数
  /// @param[in] event_list イベントリストを表す配列
  virtual
  ElbControl*
  new_EventControl(const PtControl* pt_control,
		   ymuint event_num,
		   ElbExpr** event_list);

  /// @brief リピートコントロールを生成する．
  /// @param[in] pt_control パース木の定義要素
  /// @param[in] rep 繰り返し式
  /// @param[in] event_num イベントリストの要素数
  /// @param[in] event_list イベントリストを表す配列
  virtual
  ElbControl*
  new_RepeatControl(const PtControl* pt_control,
		    ElbExpr* rep,
		    ymuint event_num,
		    ElbExpr** event_list);

  /// @brief 式のポインタ配列を生成する．
  /// @param[in] elem_num 要素数
  virtual
  ElbExpr**
  new_ExprList(ymuint elem_num);

  /// @brief 単項演算子を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] op_type 演算子のタイプ
  /// @param[in] opr1 オペランド
  virtual
  ElbExpr*
  new_UnaryOp(const PtBase* pt_expr,
	      tVpiOpType op_type,
	      ElbExpr* opr1);

  /// @brief 2項演算子を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] op_type 演算子のタイプ
  /// @param[in] opr1 オペランド
  /// @param[in] opr2 オペランド
  virtual
  ElbExpr*
  new_BinaryOp(const PtBase* pt_expr,
	       tVpiOpType op_type,
	       ElbExpr* opr1,
	       ElbExpr* opr2);

  /// @brief 3項演算子を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] op_type 演算子のタイプ
  /// @param[in] opr1 オペランド
  /// @param[in] opr2 オペランド
  /// @param[in] opr3 オペランド
  virtual
  ElbExpr*
  new_TernaryOp(const PtBase* pt_expr,
		tVpiOpType op_type,
		ElbExpr* opr1,
		ElbExpr* opr2,
		ElbExpr* opr3);

  /// @brief 連結演算子を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] opr_size オペランドの要素数
  /// @param[in] opr_list オペランドのリスト
  virtual
  ElbExpr*
  new_ConcatOp(const PtBase* pt_expr,
	       ymuint opr_size,
	       ElbExpr** opr_list);

  /// @brief 反復連結演算子を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] rep_expr 繰り返し数を表す式
  /// @param[in] rep_num 繰り返し数
  /// @param[in] opr_size オペランドの要素数
  /// @param[in] opr_list オペランドのリスト
  virtual
  ElbExpr*
  new_MultiConcatOp(const PtBase* pt_expr,
		    ElbExpr* rep_expr,
		    int rep_num,
		    ymuint opr_size,
		    ElbExpr** opr_list);

  /// @brief プライマリ式を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] obj 本体のオブジェクト
  /// @param[in] index_list インデックスのリスト
  virtual
  ElbExpr*
  new_Primary(const PtBase* pt_expr,
	      ElbDecl* obj);

  /// @brief ビット選択式を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] obj 本体のオブジェクト
  /// @param[in] bit_index ビット選択式
  virtual
  ElbExpr*
  new_BitSelect(const PtBase* pt_expr,
		ElbDecl* obj,
		ElbExpr* bit_index);

  /// @brief ビット選択式を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] expr 本体の式
  /// @param[in] bit_index ビット選択式
  virtual
  ElbExpr*
  new_BitSelect(const PtBase* pt_expr,
		ElbExpr* expr,
		int bit_index);

  /// @brief 部分選択式を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] obj 本体のオブジェクト
  /// @param[in] index1, index2 パート選択式
  /// @param[in] index1_val, index2_val パート選択式の値
  virtual
  ElbExpr*
  new_PartSelect(const PtBase* pt_expr,
		 ElbDecl* obj,
		 ElbExpr* index1,
		 ElbExpr* index2,
		 int index1_val,
		 int index2_val);

  /// @brief 部分選択式を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] expr 本体の式
  /// @param[in] index1, inde2 パート選択式
  virtual
  ElbExpr*
  new_PartSelect(const PtBase* pt_expr,
		 ElbExpr* expr,
		 int index1,
		 int index2);

  /// @brief 可変部分選択式を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] obj 本体のオブジェクト
  /// @param[in] base 範囲のベースアドレスを表す式
  /// @param[in] range 範囲を表す式
  /// @param[in] range_val 範囲の値
  virtual
  ElbExpr*
  new_PlusPartSelect(const PtBase* pt_expr,
		     ElbDecl* obj,
		     ElbExpr* base,
		     ElbExpr* range,
		     int range_val);

  /// @brief 可変部分選択式を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] obj 本体のオブジェクト
  /// @param[in] base 範囲のベースアドレスを表す式
  /// @param[in] range 範囲を表す式
  /// @param[in] range_val 範囲の値
  virtual
  ElbExpr*
  new_MinusPartSelect(const PtBase* pt_expr,
		      ElbDecl* obj,
		      ElbExpr* base,
		      ElbExpr* range,
		      int range_val);

  /// @brief 定数式を生成する．
  /// @param[in] pt_expr パース木の定義要素
  virtual
  ElbExpr*
  new_Constant(const PtExpr* pt_expr);

  /// @brief genvar 起因の定数式を生成する．
  /// @param[in] pt_primary パース木の定義要素
  /// @param[in] val 値
  virtual
  ElbExpr*
  new_GenvarConstant(const PtExpr* pt_primary,
		     int val);

  /// @brief 関数呼び出し式を生成する．
  /// @param[in] pt_obj パース木の定義要素
  /// @param[in] func 関数
  /// @param[in] arg_size 引数の数
  /// @param[in] arg_list 引数のリスト
  virtual
  ElbExpr*
  new_FuncCall(const PtBase* pt_obj,
	       const ElbTaskFunc* func,
	       ymuint arg_size,
	       ElbExpr** arg_list);

  /// @brief システム関数呼び出し式を生成する．
  /// @param[in] pt_obj パース木の定義要素
  /// @param[in] user_systf システム関数
  /// @param[in] arg_size 引数の数
  /// @param[in] arg_list 引数のリスト
  virtual
  ElbExpr*
  new_SysFuncCall(const PtBase* pt_obj,
		  const ElbUserSystf* user_systf,
		  ymuint arg_size,
		  ElbExpr** arg_list);

  /// @brief システム関数/システムタスクの引数を生成する．
  /// @param[in] pt_expr パース木中で参照している要素
  /// @param[in] arg 引数本体
  virtual
  ElbExpr*
  new_ArgHandle(const PtBase* pt_expr,
		const VlNamedObj* arg);

  /// @brief システム関数/システムタスクの引数を生成する．
  /// @param[in] pt_expr パース木中で参照している要素
  /// @param[in] arg 引数本体
  virtual
  ElbExpr*
  new_ArgHandle(const PtBase* pt_expr,
		ElbPrimitive* arg);

  /// @brief 遅延値を生成する．
  /// @param[in] pt_obj パース木の定義要素
  /// @param[in] elem_num 要素数
  /// @param[in] expr_list 式の配列
  virtual
  ElbDelay*
  new_Delay(const PtBase* pt_obj,
	    ymuint elem_num,
	    ElbExpr** expr_list);

  /// @brief attribute instance のリストを生成する．
  /// @param[in] n 要素数
  virtual
  ElbAttrList*
  new_AttrList(ymuint n);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリアロケータ
  AllocBase& mAlloc;

  ymuint32 mUdpDefnNum;
  ymuint32 mModuleArrayNum;
  ymuint32 mModule1Num;
  ymuint32 mModule2Num;
  ymuint32 mPortNum;
  ymuint32 mIODeclMRNum;
  ymuint32 mIODeclMNum;
  ymuint32 mParameterNNum;
  ymuint32 mParameterRNum;
  ymuint32 mParameterTNum;
  ymuint32 mSpecParamNum;
  ymuint32 mSpecParamRNum;
  ymuint32 mNetArrayNum;
  ymuint32 mNetNum;
  ymuint32 mNetInfoNum;
  ymuint32 mNetInfoRNum;
  ymuint32 mRegArrayNum;
  ymuint32 mRegNum;
  ymuint32 mIntArrayNum;
  ymuint32 mRealArrayNum;
  ymuint32 mTimeArrayNum;
  ymuint32 mIntNum;
  ymuint32 mTimeNum;
  ymuint32 mRealNum;
  ymuint32 mNeArrayNum;
  ymuint32 mNamedEventNum;
  ymuint32 mContAssignNum;
  ymuint32 mDefParamNum;
  ymuint32 mParamAssignNum;
  ymuint32 mPrimArrayNum;
  ymuint32 mPrimitiveNum;
  ymuint32 mTaskNum;
  ymuint32 mFunctionNum;
  ymuint32 mFunctionRNum;
  ymuint32 mFunctionTNum;
  ymuint32 mIODeclTFRNum;
  ymuint32 mIODeclTFNum;
  ymuint32 mInitialNum;
  ymuint32 mAlwaysNum;
  ymuint32 mBeginNum;
  ymuint32 mForkNum;
  ymuint32 mNamedBeginNum;
  ymuint32 mNamedForkNum;
  ymuint32 mIfStmtNum;
  ymuint32 mIfElseStmtNum;
  ymuint32 mWhileStmtNum;
  ymuint32 mRepeatStmtNum;
  ymuint32 mWaitStmtNum;
  ymuint32 mCaseStmtNum;
  ymuint32 mCaseItemNum;
  ymuint32 mForStmtNum;
  ymuint32 mDelayCtrlStmtNum;
  ymuint32 mEventCtrlStmtNum;
  ymuint32 mEventStmtNum;
  ymuint32 mAssignmentNum;
  ymuint32 mAssignStmtNum;
  ymuint32 mDeassignStmtNum;
  ymuint32 mDisableStmtNum;
  ymuint32 mTaskCallNum;
  ymuint32 mSysTaskCallNum;
  ymuint32 mForeverStmtNum;
  ymuint32 mForceStmtNum;
  ymuint32 mReleaseStmtNum;
  ymuint32 mNullStmtNum;
  ymuint32 mPrimaryNum;
  ymuint32 mBitSelectNum;
  ymuint32 mGenericBitSelectNum;
  ymuint32 mConstPartSelectNum;
  ymuint32 mPlusPartSelectNum;
  ymuint32 mMinusPartSelectNum;
  ymuint32 mNotOpNum;
  ymuint32 mBitNegOpNum;
  ymuint32 mUnaryAndOpNum;
  ymuint32 mUnaryNandOpNum;
  ymuint32 mUnaryOrOpNum;
  ymuint32 mUnaryNorOpNum;
  ymuint32 mUnaryXorOpNum;
  ymuint32 mUnaryXNorOpNum;
  ymuint32 mPlusOpNum;
  ymuint32 mMinusOpNum;
  ymuint32 mPosedgeOpNum;
  ymuint32 mNegedgeOpNum;
  ymuint32 mEqOpNum;
  ymuint32 mNeqOpNum;
  ymuint32 mCaseEqOpNum;
  ymuint32 mCaseNeqOpNum;
  ymuint32 mGtOpNum;
  ymuint32 mGeOpNum;
  ymuint32 mLtOpNum;
  ymuint32 mLeOpNum;
  ymuint32 mLogAndOpNum;
  ymuint32 mLogOrOpNum;
  ymuint32 mBitAndOpNum;
  ymuint32 mBitOrOpNum;
  ymuint32 mBitXorOpNum;
  ymuint32 mBitXNorOpNum;
  ymuint32 mAddOpNum;
  ymuint32 mSubOpNum;
  ymuint32 mMultOpNum;
  ymuint32 mDivOpNum;
  ymuint32 mModOpNum;
  ymuint32 mPowerOpNum;
  ymuint32 mLShiftOpNum;
  ymuint32 mRShiftOpNum;
  ymuint32 mArithLShiftOpNum;
  ymuint32 mArithRShiftOpNum;
  ymuint32 mConditionOpNum;
  ymuint32 mMinTypMaxOpNum;
  ymuint32 mConcatOpNum;
  ymuint32 mMultiConcatOpNum;
  ymuint32 mListOpNum;
  ymuint32 mEventOrOpNum;
  ymuint32 mFuncCallNum;
  ymuint32 mSysFuncCallNum;
  ymuint32 mIntConstNum;
  ymuint32 mBitVectorConstNum;
  ymuint32 mRealConstNum;
  ymuint32 mStringConstNum;
  ymuint32 mGenvarConstantNum;
  ymuint32 mRangeNum;
  ymuint32 mDelayControlNum;
  ymuint32 mEventControlNum;
  ymuint32 mRepeatControlNum;
  ymuint32 mAttributeNum;
  ymuint32 mStmtBlockScopeNum;
  ymuint32 mGenBlockNum;
  ymuint32 mGenForBlockNum;
  ymuint32 mModuleInfoNum;
};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELABORATOR_EI_EIFACTORY_H
