#ifndef LIBYM_VERILOG_ELABORATOR_ITEMGEN_H
#define LIBYM_VERILOG_ELABORATOR_ITEMGEN_H

/// @file libym_verilog/elaborator/ItemGen.h
/// @brief ItemGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ItemGen.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/verilog.h"
#include "ym_verilog/pt/PtP.h"
#include "ElbProxy.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ItemGen ItemGen.h "ItemGen.h"
/// @brief 構成要素を生成するクラス
//////////////////////////////////////////////////////////////////////
class ItemGen :
  public ElbProxy
{
public:

  /// @brief コンストラクタ
  /// @param[in] elab 生成器
  /// @param[in] elb_mgr Elbオブジェクトを管理するクラス
  /// @param[in] elb_factory Elbオブジェクトを生成するファクトリクラス
  ItemGen(Elaborator& elab,
	  ElbMgr& elb_mgr,
	  ElbFactory& elb_factory);

  /// @brief デストラクタ
  ~ItemGen();


public:
  //////////////////////////////////////////////////////////////////////
  // ItemGen の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief スコープに関係する要素を実体化する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_item_array 要素定義の配列
  void
  phase1_item(const VlNamedObj* parent,
	      PtItemArray pt_item_array);

  /// @brief defparam 文によるパラメータ割り当てを行う．
  /// @return 該当するパラメータが見つかったら true を返す．
  bool
  defparam_override(const VlModule* module,
		    const PtItem* pt_head,
		    const PtDefParam* pt_defparam,
		    const VlNamedObj* ulimit);

  /// @brief constant function の生成を行う．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_function 関数定義
  ElbTaskFunc*
  instantiate_constant_function(const VlNamedObj* parent,
				const PtItem* pt_function);
  
  
private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief continous assignment に関連した式の名前解決を行う．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head ヘッダ
  void
  instantiate_cont_assign(const VlNamedObj* parent,
			  const PtItem* pt_head);
  
  /// @brief process 文の生成を行う．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_item パース木の定義
  void
  instantiate_process(const VlNamedObj* parent,
		      const PtItem* pt_item);
  
  /// @brief task/function の生成を行う．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_tf タスク/関数定義
  void
  phase1_tf(const VlNamedObj* parent,
	      const PtItem* pt_tf);

  /// @param[in] task/function 内の宣言要素の生成を行う．
  /// @param[in] taskfunc タスク/関数本体
  /// @param[in] pt_item パース木の関数定義
  void
  phase2_tf(ElbTaskFunc* taskfunc,
	    const PtItem* pt_item);

  /// @param[in] task/function 内のステートメントの生成を行う．
  /// @param[in] taskfunc タスク/関数本体
  /// @param[in] pt_item パース木の関数定義
  void
  phase3_tf(ElbTaskFunc* task,
	    const PtItem* pt_item);

  /// @brief module/Udp instance の生成を行う
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head ヘッダ
  void
  phase1_muheader(const VlNamedObj* parent,
		  const PtItem* pt_head);

  /// @brief module array のインスタンス化を行う．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_module モジュール定義
  /// @param[in] pt_head ヘッダ
  /// @param[in] pt_inst インスタンス定義
  void
  phase1_module_array(const VlNamedObj* parent,
		      const PtModule* pt_module,
		      const PtItem* pt_head,
		      const PtInst* pt_inst);

  /// @brief module array instance の入出力端子の接続を行う．
  /// @param[in] module_array モジュール配列
  /// @param[in] pt_module モジュール定義
  /// @param[in] pt_inst インスタンス定義
  void
  link_module_array(ElbModuleArray* module_array,
		    const PtModule* pt_module,
		    const PtInst* pt_inst);

  /// @brief module instance の入出力端子の接続を行う．
  /// @param[in] module モジュール
  /// @param[in] pt_module モジュール定義
  /// @param[in] pt_inst インスタンス定義
  void
  link_module(ElbModule* module,
	      const PtModule* pt_module,
	      const PtInst* pt_inst);

  /// @brief gate instance の生成を行う
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head ヘッダ
  void
  instantiate_gateheader(const VlNamedObj* parent,
			 const PtItem* pt_head);

  /// @brief UDP instance の生成を行う
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head ヘッダ
  /// @param[in] udpdefn UDP
  void
  instantiate_udpheader(const VlNamedObj* parent,
			const PtItem* pt_head,
			const ElbUdpDefn* udpdefn);

  /// @brief gate delay の生成を行う
  /// @param[in] prim_head ゲートプリミティブのヘッダ
  /// @param[in] pt_delay パース木の遅延式
  void
  link_gate_delay(ElbPrimHead* prim_head,
		  const PtDelay* pt_delay);

  /// @brief UDP instance に付随する遅延式の生成を行う．
  /// @param[in] parent 親のスコープ
  /// @param[in] prim_head ヘッダ
  /// @param[in] pt_head パース木のインスタンス定義ヘッダ
  void
  link_udp_delay(ElbPrimHead* prim_head,
		 const PtItem* pt_head);

  /// @brief gate array instance で使われている式の名前解決を行う．
  /// @param[in] prim_array プリミティブ配列
  /// @param[in] pt_inst インスタンス定義
  void
  link_prim_array(ElbPrimArray* prim_array,
		  const PtInst* pt_inst);

  /// @brief gate instance で使われている式の名前解決を行う．
  /// @param[in] primitive プリミティブ配列
  /// @param[in] pt_inst インスタンス定義
  void
  link_primitive(ElbPrimitive* primitive,
		 const PtInst* pt_inst);

  /// @brief generate block を実際にインスタンス化を行う．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_genblock generate block 定義
  void
  phase1_generate(const VlNamedObj* parent,
		  const PtItem* pt_genblock);

  /// @brief PtGenBlock に対応するインスタンスの生成を行う
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_genblock generate block 定義
  void
  phase1_genblock(const VlNamedObj* parent,
		  const PtItem* pt_genblock);

  /// @brief generate if に対応するインスタンスの生成を行う
  /// @param[in] parent 親のスコープ
  /// @parma[in] pt_genif generate if 定義
  void
  phase1_genif(const VlNamedObj* parent,
	       const PtItem* pt_genif);

  /// @brief generate case に対応するインスタンスの生成を行う
  /// @param[in] parent 親のスコープ
  /// @parma[in] pt_gencase generate case 定義
  void
  phase1_gencase(const VlNamedObj* parent,
		 const PtItem* pt_gencase);

  /// @brief generate for に対応するインスタンスの生成を行う
  /// @param[in] parent 親のスコープ
  /// @parma[in] pt_genfor generate for 定義
  void
  phase1_genfor(const VlNamedObj* parent,
		const PtItem* pt_genfor);
  
  /// @brief generate block の要素でスコープに関連するものの生成を行う．
  /// @note と同時の残りの処理をキューに積む．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_decl_array パース木の宣言の配列
  /// @param[in] pt_item_array パース木の要素の配列
  void
  phase1_genitem(const VlNamedObj* parent,
		 PtDeclHeadArray pt_decl_array,
		 PtItemArray pt_item_array);
  
};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELABORATOR_ITEMGEN_H
