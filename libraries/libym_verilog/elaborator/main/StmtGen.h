#ifndef LIBYM_VERILOG_ELABORATOR_STMTGEN_H
#define LIBYM_VERILOG_ELABORATOR_STMTGEN_H

/// @file libym_verilog/elaborator/StmtGen.h
/// @brief StmtGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: StmtGen.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/verilog.h"
#include "ym_verilog/pt/PtP.h"
#include "ElbProxy.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class StmtGen StmtGen.h "StmtGen.h"
/// @brief ElbStmt を生成するクラス
//////////////////////////////////////////////////////////////////////
class StmtGen :
  public ElbProxy
{
public:

  /// @brief コンストラクタ
  /// @param[in] elab 生成器
  /// @param[in] elb_mgr Elbオブジェクトを管理するクラス
  /// @param[in] elb_factory Elbオブジェクトを生成するファクトリクラス
  StmtGen(Elaborator& elab,
	  ElbMgr& elb_mgr,
	  ElbFactory& elb_factory);

  /// @brief デストラクタ
  ~StmtGen();


public:
  //////////////////////////////////////////////////////////////////////
  // StmtGen の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief スコープに関係するステートメントの実体化を行う．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_stmt 対象のステートメント
  /// @param[in] cf constant function 中のステートメントの時 true
  void
  phase1_stmt(const VlNamedObj* parent,
	      const PtStmt* pt_stmt,
	      bool cf = false);

  /// @brief ステートメントの実体化を行う．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス
  /// @param[in] env 生成時の環境
  /// @param[in] pt_stmt 対象のステートメント
  ElbStmt*
  instantiate_stmt(const VlNamedObj* parent,
		   ElbProcess* process,
		   const ElbEnv& env,
		   const PtStmt* pt_stmt);
  

private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief disable statement の実体化を行う．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] pt_stmt 対象のステートメント
  ElbStmt*
  instantiate_disable(const VlNamedObj* parent,
		      ElbProcess* process,
		      const PtStmt* pt_stmt);

  /// @brief enable の実体化を行う．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] env 生成時の環境
  /// @param[in] pt_stmt 対象のステートメント
  ElbStmt*
  instantiate_enable(const VlNamedObj* parent,
		     ElbProcess* process,
		     const ElbEnv& env,
		     const PtStmt* pt_stmt);

  /// @brief system enable 文の実体化を行う．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] env 生成時の環境
  /// @param[in] pt_stmt 対象のステートメント
  ElbStmt*
  instantiate_sysenable(const VlNamedObj* parent,
			ElbProcess* process,
			const ElbEnv& env,
			const PtStmt* pt_stmt);

  /// @brief delay / event control statement の実体化を行う．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] env 生成時の環境
  /// @param[in] pt_stmt 対象のステートメント
  ElbStmt*
  instantiate_ctrlstmt(const VlNamedObj* parent,
		       ElbProcess* process,
		       const ElbEnv& env,
		       const PtStmt* pt_stmt);

  /// 代入文のインスタンス化を行う．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] env 生成時の環境
  /// @param[in] pt_stmt 対象のステートメント
  /// @param[in] block ブロッキング代入の時 true
  ElbStmt*
  instantiate_assign(const VlNamedObj* parent,
		     ElbProcess* process,
		     const ElbEnv& env,
		     const PtStmt* pt_stmt,
		     bool block);

  /// @brief コントロールを生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] env 生成時の環境
  /// @param[in] pt_control パース木のコントロール定義
  ElbControl*
  instantiate_control(const VlNamedObj* parent,
		      const ElbEnv& env,
		      const PtControl* pt_control);

  /// @brief event statement の実体化を行う．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] pt_stmt 対象のステートメント
  ElbStmt*
  instantiate_eventstmt(const VlNamedObj* parent,
			ElbProcess* process,
			const PtStmt* pt_stmt);

  /// @brief null statement の実体化を行う．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] pt_stmt 対象のステートメント
  ElbStmt*
  instantiate_nullstmt(const VlNamedObj* parent,
		       ElbProcess* process,
		       const PtStmt* pt_stmt);

  /// @brief if 文の生成を行う．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] env 生成時の環境
  /// @param[in] pt_stmt 対象のステートメント
  ElbStmt*
  instantiate_if(const VlNamedObj* parent,
		 ElbProcess* process,
		 const ElbEnv& env,
		 const PtStmt* pt_stmt);

  /// @brief case 文の実体化を行う．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] env 生成時の環境
  /// @param[in] pt_stmt 対象のステートメント
  ElbStmt*
  instantiate_case(const VlNamedObj* parent,
		   ElbProcess* process,
		   const ElbEnv& env,
		   const PtStmt* pt_stmt);

  /// @brief wait 文の実体化を行う．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] env 生成時の環境
  /// @param[in] pt_stmt 対象のステートメント
  ElbStmt*
  instantiate_wait(const VlNamedObj* parent,
		   ElbProcess* process,
		   const ElbEnv& env,
		   const PtStmt* pt_stmt);

  /// @brief forever 文のインスタンス化を行う．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] env 生成時の環境
  /// @param[in] pt_stmt 対象のステートメント
  ElbStmt*
  instantiate_forever(const VlNamedObj* parent,
		      ElbProcess* process,
		      const ElbEnv& env,
		      const PtStmt* pt_stmt);

  /// @brief repeat 文のインスタンス化を行う．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] env 生成時の環境
  /// @param[in] pt_stmt 対象のステートメント
  ElbStmt*
  instantiate_repeat(const VlNamedObj* parent,
		     ElbProcess* process,
		     const ElbEnv& env,
		     const PtStmt* pt_stmt);

  /// @brief while 文のインスタンス化を行う．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] env 生成時の環境
  /// @param[in] pt_stmt 対象のステートメント
  ElbStmt*
  instantiate_while(const VlNamedObj* parent,
		    ElbProcess* process,
		    const ElbEnv& env,
		    const PtStmt* pt_stmt);

  /// @brief for 文のインスタンス化を行う．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] env 生成時の環境
  /// @param[in] pt_stmt 対象のステートメント
  ElbStmt*
  instantiate_for(const VlNamedObj* parent,
		  ElbProcess* process,
		  const ElbEnv& env,
		  const PtStmt* pt_stmt);

  /// @brief procedural continuous assign 文のインスタンス化を行う．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] env 生成時の環境
  /// @param[in] pt_stmt 対象のステートメント
  ElbStmt*
  instantiate_pca(const VlNamedObj* parent,
		  ElbProcess* process,
		  const ElbEnv& env,
		  const PtStmt* pt_stmt);

  /// @brief deassign 文のインスタンス化を行う．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] env 生成時の環境
  /// @param[in] pt_stmt 対象のステートメント
  ElbStmt*
  instantiate_deassign(const VlNamedObj* parent,
		       ElbProcess* process,
		       const ElbEnv& env,
		       const PtStmt* pt_stmt);

  /// @brief force 文のインスタンス化を行う．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] env 生成時の環境
  /// @param[in] pt_stmt 対象のステートメント
  ElbStmt*
  instantiate_force(const VlNamedObj* parent,
		    ElbProcess* process,
		    const ElbEnv& env,
		    const PtStmt* pt_stmt);

  /// @brief release 文のインスタンス化を行う．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] env 生成時の環境
  /// @param[in] pt_stmt 対象のステートメント
  ElbStmt*
  instantiate_release(const VlNamedObj* parent,
		      ElbProcess* process,
		      const ElbEnv& env,
		      const PtStmt* pt_stmt);

  /// @brief parallel block のインスタンス化を行う．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] env 生成時の環境
  /// @param[in] pt_stmt 対象のステートメント
  ElbStmt*
  instantiate_parblock(const VlNamedObj* parent,
		       ElbProcess* process,
		       const ElbEnv& env,
		       const PtStmt* pt_stmt);

  /// @brief sequential block のインスタンス化を行う．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] env 生成時の環境
  /// @param[in] pt_stmt 対象のステートメント
  ElbStmt*
  instantiate_seqblock(const VlNamedObj* parent,
		       ElbProcess* process,
		       const ElbEnv& env,
		       const PtStmt* pt_stmt);
  
  /// @brief ブロックスコープ内の宣言要素の生成を行う．
  void
  phase2_namedblock(const VlNamedObj* parent,
		    PtDeclHeadArray pt_head_array);

  /// @brief 名前つき parallel block のインスタンス化を行う．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] env 生成時の環境
  /// @param[in] pt_stmt 対象のステートメント
  ElbStmt*
  instantiate_namedparblock(const VlNamedObj* parent,
			    ElbProcess* process,
			    const ElbEnv& env,
			    const PtStmt* pt_stmt);

  /// @brief 名前つき sequential block のインスタンス化を行う．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] env 生成時の環境
  /// @param[in] pt_stmt 対象のステートメント
  ElbStmt*
  instantiate_namedseqblock(const VlNamedObj* parent,
			    ElbProcess* process,
			    const ElbEnv& env,
			    const PtStmt* pt_stmt);
  
};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELABORATOR_STMTGEN_H
