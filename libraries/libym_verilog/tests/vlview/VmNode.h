#ifndef VMNODE_H
#define VMNODE_H

/// @file libym_verilog/tests/vlview/VmNode.h
/// @brief VmNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VmNode.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include <QAbstractItemModel>
#include "ym_verilog/verilog.h"
#include "ym_utils/FileRegion.h"
#include "ym_verilog/vl/VlFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VmNode VmNode.h
/// @brief verilog の構造を表示するためのモデル
//////////////////////////////////////////////////////////////////////
class VmNode
{
  friend class VerilogModel;
  
public:
  
  /// @brief コンストラクタ
  VmNode();

  /// @brief デストラクタ
  virtual
  ~VmNode();
  
  
public:

  /// @brief 親のインデックスを返す．
  QModelIndex
  parent_index() const;
  
  /// @brief 子供の数を返す．
  int
  child_num() const;

  /// @brief 子供を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < row_num() )
  VmNode*
  child(int pos) const;

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const = 0;
  
  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const = 0;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスが用いる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief IODeclListNode を追加する．
  /// @param[in] iodecl_list IO宣言のリスト
  void
  add_iolist(const vector<const VlIODecl*>& iodecl_list) const;
  
  /// @brief ExprListNode を追加する．
  /// @param[in] label ラベル
  /// @param[in] expr_list 式の配列
  void
  add_exprlist(const QString& label,
	       const vector<const VlExpr*>& expr_list) const;
  
  /// @brief ExprNode を追加する．
  /// @param[in] label ラベル
  /// @param[in] expr 式
  void
  add_expr(const QString& label,
	   const VlExpr* expr) const;

  /// @brief strength 型のノードを追加する．
  /// @param[in] label ラベル
  /// @param[in] value 値
  void
  add_strength(const QString& label,
	       tVpiStrength value) const;

  /// @brief 遅延値型のノードを追加する．
  /// @param[in] value 値
  void
  add_delay(const VlDelay* value) const;

  /// @brief dir 型のノードを追加する．
  /// @param[in] value 値
  void
  add_dir(tVpiDirection dir) const;
  
  /// @brief 文字列型のノードを追加する．
  /// @param[in] label ラベル
  /// @param[in] value 値
  void
  add_str(const QString& label,
	  const QString& value) const;

  /// @brief 文字列型のノードを追加する．
  /// @param[in] label ラベル
  /// @param[in] value 値
  void
  add_str(const QString& label,
	  const string& value) const;

  /// @brief 文字列型のノードを追加する．
  /// @param[in] label ラベル
  /// @param[in] value 値
  void
  add_str(const QString& label,
	  const char* value) const;

  /// @brief ブール型のノードを追加する．
  /// @param[in] label ラベル
  /// @param[in] value 値
  void
  add_bool(const QString& label,
	    bool value) const;

  /// @brief 整数型のノードを追加する．
  /// @param[in] label ラベル
  /// @param[in] value 値
  void
  add_int(const QString& label,
	    int value) const;

  /// @brief 整数型のノードを追加する．
  /// @param[in] label ラベル
  /// @param[in] value 値
  void
  add_int(const QString& label,
	  ymuint value) const;
  
  /// @brief 子供を追加する．
  void
  add_child(VmNode* node) const;
    

private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const = 0;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 親のノード
  QModelIndex mParentIndex;
  
  // 子供の配列を作っているかを示すフラグ
  mutable
  bool mExpanded;
  
  // 子供の配列
  mutable
  std::vector<VmNode*> mChildren;
  
};


//////////////////////////////////////////////////////////////////////
/// @class VmNode1 VmNode.h
/// @brief VlMgr を持った VmNode の継承クラス
//////////////////////////////////////////////////////////////////////
class VmNode1 :
  public VmNode
{
public:
  
  /// @brief コンストラクタ
  /// @param[in] mgr VlMgr
  VmNode1(const VlMgr& mgr);

  /// @brief デストラクタ
  virtual
  ~VmNode1();


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスが用いる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief VlMgr を取り出す．
  const VlMgr&
  vl_mgr() const;
  
  /// @brief UdpListNode を追加する．
  void
  add_udplist() const;

  /// @brief ModuleListNode を追加する．
  void
  add_modulelist() const;

  /// @brief スコープに属する要素を追加する．
  /// @param[in] scope スコープ
  void
  add_scope_item(const VlNamedObj* scope) const;

  /// @brief DeclListNode を追加する．
  /// @param[in] label ラベル
  /// @param[in] decl_list 宣言のリスト
  void
  add_decllist(const QString& label,
	       const vector<const VlDecl*>& decl_list) const;

  /// @brief StmtNodeを追加する．
  /// @param[in] label ラベル
  /// @param[in] stmt ステートメント
  void
  add_stmt(const QString& label,
	   const VlStmt* stmt) const;

  /// @brief ParamAssignListNode を追加する．
  /// @param[in] pa_list parameter代入文のリスト
  void
  add_paramassignlist(const vector<const VlParamAssign*>& pa_list) const;

  /// @brief DefParamListNode を追加する．
  /// @param[in] defparam_list defparam 文のリスト
  void
  add_defparamlist(const vector<const VlDefParam*>& defparam_list) const;

  /// @brief ScopeListNode を追加する．
  /// @param[in] scope_list 内部スコープのリスト
  void
  add_scopelist(const vector<const VlNamedObj*>& scope_list) const;

  /// @brief TaskListNode を追加する．
  /// @param[in] task_list タスクのリスト
  void
  add_tasklist(const vector<const VlTaskFunc*>& task_list) const;
  
  /// @brief FuncListNode を追加する．
  /// @param[in] function_list 関数のリスト
  void
  add_functionlist(const vector<const VlTaskFunc*>& function_list) const;

  /// @brief ContAssignListNode を追加する．
  /// @param[in] ca_list 継続的代入文のリスト
  void
  add_contassignlist(const vector<const VlContAssign*>& ca_list) const;

  /// @brief ModuleArrayListNode を追加する．
  /// @param[in] ma_list モジュール配列のリスト
  void
  add_modulearraylist(const vector<const VlModuleArray*>& ma_list) const;

  /// @brief ModuleInstListNode を追加する．
  /// @param[in] module_list モジュールのリスト
  void
  add_moduleinstlist(const vector<const VlModule*>& module_list) const;

  /// @brief PrimitiveListNode を追加する．
  /// @param[in] primitive_list プリミティブのリスト
  void
  add_primitivelist(const vector<const VlPrimitive*>& primitive_list) const;

  /// @brief PrimArrayListNode を追加する．
  /// @param[in] primarray_list プリミティブ配列のリスト
  void
  add_primarraylist(const vector<const VlPrimArray*>& primarray_list) const;
  
  /// @brief ProcessListNode を追加する．
  /// @param[in] process_list プロセスのリスト
  void
  add_processlist(const vector<const VlProcess*>& process_list) const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // VlMgr
  const VlMgr& mMgr;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 親のインデックスを返す．
inline
QModelIndex
VmNode::parent_index() const
{
  return mParentIndex;
}
  
// @brief 子供の数を返す．
inline
int
VmNode::child_num() const
{
  if ( !mExpanded ) {
    expand();
    mExpanded = true;
  }
  return mChildren.size();
}

// @brief 子供を返す．
// @param[in] pos 位置番号 ( 0 <= pos < row_num() )
inline
VmNode*
VmNode::child(int pos) const
{
  if ( !mExpanded ) {
    expand();
    mExpanded = true;
  }
  return mChildren[pos];
}

END_NAMESPACE_YM_VERILOG

#endif // VMNODE_H
