#ifndef LIBYM_VERILOG_ELABORATOR_DECLGEN_H
#define LIBYM_VERILOG_ELABORATOR_DECLGEN_H

/// @file libym_verilog/elaborator/DeclGen.h
/// @brief DeclGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DeclGen.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/verilog.h"
#include "ym_verilog/pt/PtP.h"
#include "ElbProxy.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class DeclGen DeclGen.h "DeclGen.h"
/// @brief ElbDecl を生成するクラス
//////////////////////////////////////////////////////////////////////
class DeclGen :
  public ElbProxy
{
public:

  /// @brief コンストラクタ
  /// @param[in] elab 生成器
  /// @param[in] elb_mgr Elbオブジェクトを管理するクラス
  /// @param[in] elb_factory Elbオブジェクトを生成するファクトリクラス
  DeclGen(Elaborator& elab,
	  ElbMgr& elb_mgr,
	  ElbFactory& elb_factory);

  /// @brief デストラクタ
  ~DeclGen();


public:
  //////////////////////////////////////////////////////////////////////
  // DeclGen の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief IO宣言要素を実体化する．
  /// @param[in] module 親のモジュール
  /// @param[in] taskfunc 親のタスク/関数
  /// @param[in] pt_head_array IO宣言ヘッダの配列
  /// @note module, taskfunc は1つのみが値を持つ．残りは NULL．
  void
  instantiate_iodecl(ElbModule* module,
		     ElbTaskFunc* taskfunc,
		     PtIOHeadArray pt_head_array);

  /// @brief 宣言要素のリストをインスタンス化する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head_array 宣言ヘッダの配列
  void
  instantiate_decl(const VlNamedObj* parent,
		   PtDeclHeadArray pt_head_array);

  /// @brief パラメータ用の instantiate 関数
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head_array 宣言ヘッダの配列
  /// @param[in] is_local local_param の時 true
  void
  instantiate_param(const VlNamedObj* parent,
		    PtDeclHeadArray pt_head_array,
		    bool is_local);
  

private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief net をインスタンス化する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head 宣言のヘッダ
  void
  instantiate_net_head(const VlNamedObj* parent,
		       const PtDeclHead* pt_head);
  
  /// @brief net の遅延値を生成する．
  /// @param[in] net_head ネットのヘッダ
  /// @param[in] pt_delay パース木の遅延式定義
  void
  link_net_delay(ElbDeclHead* net_head,
		 const PtDelay* pt_delay);

  /// @brief net の初期値を生成する．
  /// @param[in] net ネット
  /// @param[in] pt_item パース木のネット定義要素
  void
  link_net_assign(ElbDecl* net,
		  const PtDeclItem* pt_item);

  /// @brief reg をインスタンス化する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head 宣言のヘッダ
  void
  instantiate_reg_head(const VlNamedObj* parent,
		       const PtDeclHead* pt_head);

  /// @brief variable をインスタンス化する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head 宣言のヘッダ
  void
  instantiate_var_head(const VlNamedObj* parent,
		       const PtDeclHead* pt_head);

  /// @brief named_event をインスタンス化する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head 宣言のヘッダ
  void
  instantiate_event_head(const VlNamedObj* parent,
			 const PtDeclHead* pt_head);

  /// @brief genvar をインスタンス化する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head 宣言のヘッダ
  void
  instantiate_genvar_head(const VlNamedObj* parent,
			  const PtDeclHead* pt_head);

  /// @brief 配列の次元リストを生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_item 要素定義
  /// @param[in] range_src 範囲の情報を設定する配列
  bool
  instantiate_dimension_list(const VlNamedObj* parent,
			     const PtDeclItem* pt_item,
			     vector<ElbRangeSrc>& range_src);
  
};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELABORATOR_DECLGEN_H
