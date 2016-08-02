#ifndef LIBYM_VERILOG_ELABORATOR_MODULEGEN_H
#define LIBYM_VERILOG_ELABORATOR_MODULEGEN_H

/// @file libym_verilog/elaborator/ModuleGen.h
/// @brief ModuleGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ModuleGen.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/verilog.h"
#include "ym_verilog/pt/PtP.h"
#include "ElbProxy.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ModuleGen ModuleGen.h "ModuleGen.h"
/// @brief ElbModule を生成するクラス
//////////////////////////////////////////////////////////////////////
class ModuleGen :
  public ElbProxy
{
public:

  /// @brief コンストラクタ
  /// @param[in] elab 生成器
  /// @param[in] elb_mgr Elbオブジェクトを管理するクラス
  /// @param[in] elb_factory Elbオブジェクトを生成するファクトリクラス
  ModuleGen(Elaborator& elab,
	    ElbMgr& elb_mgr,
	    ElbFactory& elb_factory);

  /// @brief デストラクタ
  ~ModuleGen();


public:
  //////////////////////////////////////////////////////////////////////
  // ModuleGen の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief top module のインスタンス化を行う．
  /// @param[in] toplevel トップレベルのスコープ
  /// @param[in] pt_module モジュール定義
  void
  phase1_topmodule(const VlNamedObj* toplevel,
		   const PtModule* pt_module);

  /// @brief module の中身のうちスコープに関係するインスタンス化を行う．
  /// @param[in] modle モジュール
  /// @param[in] pt_module モジュール定義
  /// @param[in] param_con パラメータ割り当ての情報
  void
  phase1_module_item(ElbModule* module,
		     const PtModule* pt_module,
		     const ElbParamCon* param_con);


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief phase2 で処理する内容をキューに積む．
  /// @param[in] modle モジュール
  /// @param[in] pt_module モジュール定義
  void
  add_phase2stub(ElbModule* module,
		 const PtModule* pt_module);

  /// @brief module の中身のインスタンス化を行う．
  /// @param[in] modle モジュール
  /// @param[in] pt_module モジュール定義
  void
  phase2_module_item(ElbModule* module,
		     const PtModule* pt_module);

  /// @brief port の生成を行う．
  /// @param[in] module 親のモジュール
  /// @param[in] pt_module モジュール定義
  void
  instantiate_port(ElbModule* module,
		   const PtModule* pt_module);

  /// @brief PtPortRef から expression を生成する．
  /// @param[in] module 親のモジュール
  /// @param[in] pt_portref パース木の portref 定義
  ElbExpr*
  instantiate_portref(ElbModule* module,
		      const PtPortRef* pt_portref);

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief phase2 で処理する内容をキューに積む．
// @param[in] modle モジュール
// @param[in] pt_module モジュール定義
inline
void
ModuleGen::add_phase2stub(ElbModule* module,
			  const PtModule* pt_module)
{
  ElbStub* stub = make_stub(this, &ModuleGen::phase2_module_item,
			    module, pt_module);
  ElbProxy::add_phase2stub(stub);
}

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELABORATOR_MODULEGEN_H
