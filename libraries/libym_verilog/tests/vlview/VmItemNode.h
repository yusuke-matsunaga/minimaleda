#ifndef VMITEMNODE_H
#define VMITEMNODE_H

/// @file libym_verilog/tests/vlview/VmItemNode.h
/// @brief VmItemNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VmItemNode.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "VmNode.h"
#include "ym_verilog/verilog.h"
#include "ym_verilog/vl/VlFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VmParamAssignListNode VmItemNode.h
/// @brief パラメータ代入文のリストを表すノード
//////////////////////////////////////////////////////////////////////
class VmParamAssignListNode :
  public VmNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] pa_list パラメータ代入文のリスト
  VmParamAssignListNode(const vector<const VlParamAssign*>& pa_list);

  /// @brief デストラクタ
  virtual
  ~VmParamAssignListNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;
    
  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // パラメータ代入文の配列
  vector<const VlParamAssign*> mParamAssignArray;
  
};


//////////////////////////////////////////////////////////////////////
/// @class VmParamAssignNode VmItemNode.h
/// @brief パラメータ代入文を表すノード
//////////////////////////////////////////////////////////////////////
class VmParamAssignNode :
  public VmNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] paramassign パラメータ代入文
  VmParamAssignNode(const VlParamAssign* paramassign);

  /// @brief デストラクタ
  virtual
  ~VmParamAssignNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;
    
  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // パラメータ代入文
  const VlParamAssign* mParamAssign;
  
};


//////////////////////////////////////////////////////////////////////
/// @class VmDefParamListNode VmItemNode.h
/// @brief defparamのリストを表すノード
//////////////////////////////////////////////////////////////////////
class VmDefParamListNode :
  public VmNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] defparam_list defparamのリスト
  VmDefParamListNode(const vector<const VlDefParam*>& defparam_list);

  /// @brief デストラクタ
  virtual
  ~VmDefParamListNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;
    
  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // defparamの配列
  vector<const VlDefParam*> mDefParamArray;
  
};


//////////////////////////////////////////////////////////////////////
/// @class VmDefParamNode VmItemNode.h
/// @brief defparam文を表すノード
//////////////////////////////////////////////////////////////////////
class VmDefParamNode :
  public VmNode
{
public:
  
  /// @brief コンストラクタ
  /// @param[in] defparam defparam文
  VmDefParamNode(const VlDefParam* defparam);

  /// @brief デストラクタ
  virtual
  ~VmDefParamNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;
    
  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // defparam文
  const VlDefParam* mDefParam;

};


//////////////////////////////////////////////////////////////////////
/// @class VmScopeListNode VmItemNode.h
/// @brief スコープのリストを表すノード
//////////////////////////////////////////////////////////////////////
class VmScopeListNode :
  public VmNode1
{
public:

  /// @brief コンストラクタ
  /// @param[in] vl_mgr VlMgr
  /// @param[in] scope_list スコープのリスト
  VmScopeListNode(const VlMgr& vl_mgr,
		  const vector<const VlNamedObj*>& scope_list);

  /// @brief デストラクタ
  virtual
  ~VmScopeListNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;
    
  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // スコープの配列
  vector<const VlNamedObj*> mScopeArray;
  
};


//////////////////////////////////////////////////////////////////////
/// @class VmScopeNode VmItemNode.h
/// @brief スコープを表すノード
//////////////////////////////////////////////////////////////////////
class VmScopeNode :
  public VmNode1
{
public:
  
  /// @brief コンストラクタ
  /// @param[in] vl_mgr VlMgr
  /// @param[in] scope スコープ
  VmScopeNode(const VlMgr& vl_mgr,
	      const VlNamedObj* scope);

  /// @brief デストラクタ
  virtual
  ~VmScopeNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;
    
  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // スコープ
  const VlNamedObj* mScope;

};


//////////////////////////////////////////////////////////////////////
/// @class VmTaskListNode VmItemNode.h
/// @brief タスクのリストを表すノード
//////////////////////////////////////////////////////////////////////
class VmTaskListNode :
  public VmNode1
{
public:

  /// @brief コンストラクタ
  /// @param[in] vl_mgr VlMgr
  /// @param[in] task_list タスクのリスト
  VmTaskListNode(const VlMgr& vl_mgr,
		 const vector<const VlTaskFunc*>& task_list);

  /// @brief デストラクタ
  virtual
  ~VmTaskListNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;
    
  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // タスクの配列
  vector<const VlTaskFunc*> mTaskArray;
  
};


//////////////////////////////////////////////////////////////////////
/// @class VmTaskNode VmItemNode.h
/// @brief タスクを表すノード
//////////////////////////////////////////////////////////////////////
class VmTaskNode :
  public VmNode1
{
public:
  
  /// @brief コンストラクタ
  /// @param[in] vl_mgr VlMgr
  /// @param[in] task タスク
  VmTaskNode(const VlMgr& vl_mgr,
	     const VlTaskFunc* task);

  /// @brief デストラクタ
  virtual
  ~VmTaskNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;
    
  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // タスク
  const VlTaskFunc* mTask;

};


//////////////////////////////////////////////////////////////////////
/// @class VmFunctionListNode VmItemNode.h
/// @brief 関数のリストを表すノード
//////////////////////////////////////////////////////////////////////
class VmFunctionListNode :
  public VmNode1
{
public:

  /// @brief コンストラクタ
  /// @param[in] vl_mgr VlMgr
  /// @param[in] function_list 関数のリスト
  VmFunctionListNode(const VlMgr& vl_mgr,
		     const vector<const VlTaskFunc*>& function_list);

  /// @brief デストラクタ
  virtual
  ~VmFunctionListNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;
    
  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 関数の配列
  vector<const VlTaskFunc*> mFunctionArray;
  
};


//////////////////////////////////////////////////////////////////////
/// @class VmFunctionNode VmItemNode.h
/// @brief 関数を表すノード
//////////////////////////////////////////////////////////////////////
class VmFunctionNode :
  public VmNode1
{
public:
  
  /// @brief コンストラクタ
  /// @param[in] vl_mgr VlMgr
  /// @param[in] function 関数
  VmFunctionNode(const VlMgr& vl_mgr,
		 const VlTaskFunc* function);

  /// @brief デストラクタ
  virtual
  ~VmFunctionNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;
    
  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 関数
  const VlTaskFunc* mFunction;

};


//////////////////////////////////////////////////////////////////////
/// @class VmPrimArrayListNode VmItemNode.h
/// @brief プリミティブ配列のリストを表すノード
//////////////////////////////////////////////////////////////////////
class VmPrimArrayListNode :
  public VmNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] primarray_list プリミティブ配列のリスト
  VmPrimArrayListNode(const vector<const VlPrimArray*>& primarray_list);

  /// @brief デストラクタ
  virtual
  ~VmPrimArrayListNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;
    
  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // プリミティブ配列の配列
  vector<const VlPrimArray*> mPrimArrayArray;
  
};


//////////////////////////////////////////////////////////////////////
/// @class VmPrimArrayNode VmItemNode.h
/// @brief プリミティブ配列を表すノード
//////////////////////////////////////////////////////////////////////
class VmPrimArrayNode :
  public VmNode
{
public:
  
  /// @brief コンストラクタ
  /// @param[in] primarray プリミティブ配列
  VmPrimArrayNode(const VlPrimArray* primarray);

  /// @brief デストラクタ
  virtual
  ~VmPrimArrayNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;
    
  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // プリミティブ配列
  const VlPrimArray* mPrimArray;
};


//////////////////////////////////////////////////////////////////////
/// @class VmPrimitiveListNode VmItemNode.h
/// @brief プリミティブのリストを表すノード
//////////////////////////////////////////////////////////////////////
class VmPrimitiveListNode :
  public VmNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] prim_list プリミティブのリスト
  VmPrimitiveListNode(const vector<const VlPrimitive*>& prim_list);

  /// @brief デストラクタ
  virtual
  ~VmPrimitiveListNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;
    
  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // プリミティブの配列
  vector<const VlPrimitive*> mPrimArray;
  
};


//////////////////////////////////////////////////////////////////////
/// @class VmPrimitiveNode VmItemNode.h
/// @brief プリミティブを表すノード
//////////////////////////////////////////////////////////////////////
class VmPrimitiveNode :
  public VmNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] primitive プリミティブ
  VmPrimitiveNode(const VlPrimitive* primitive);

  /// @brief デストラクタ
  virtual
  ~VmPrimitiveNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;
    
  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // プリミティブ
  const VlPrimitive* mPrimitive;
  
};


//////////////////////////////////////////////////////////////////////
/// @class VmPrimTermListNode VItemNode.h
/// @brief PrimTerm のリストを表すノード
//////////////////////////////////////////////////////////////////////
class VmPrimTermListNode :
  public VmNode
{
public:
  
  /// @brief コンストラクタ
  /// @param[in] primitive 親のプリミティブ
  VmPrimTermListNode(const VlPrimitive* primitive);

  /// @brief デストラクタ
  virtual
  ~VmPrimTermListNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;
    
  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 親のプリミティブ
  const VlPrimitive* mPrimitive;
  
};


//////////////////////////////////////////////////////////////////////
/// @class VmPrimTermNode VmItermNode
/// @brief PrimTerm を表すノード
//////////////////////////////////////////////////////////////////////
class VmPrimTermNode :
  public VmNode
{
public:
  
  /// @brief コンストラクタ
  /// @param[in] prim_term プリミティブの端子
  VmPrimTermNode(const VlPrimTerm* prim_term);

  /// @brief デストラクタ
  virtual
  ~VmPrimTermNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;
    
  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // プリミティブの端子
  const VlPrimTerm* mPrimTerm;

};


//////////////////////////////////////////////////////////////////////
/// @class VmContAssignListNode VmItemNode.h
/// @brief continuous assign 文のリストを表すノード
//////////////////////////////////////////////////////////////////////
class VmContAssignListNode :
  public VmNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] ca_list 継続的代入文のリスト
  VmContAssignListNode(const vector<const VlContAssign*>& ca_list);

  /// @brief デストラクタ
  virtual
  ~VmContAssignListNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;
    
  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // continuous assign 文の配列
  vector<const VlContAssign*> mContAssignArray;
  
};


//////////////////////////////////////////////////////////////////////
/// @class VmContAssignNode VmItemNode.h
/// @brief continuous assign 文を表すノード
//////////////////////////////////////////////////////////////////////
class VmContAssignNode :
  public VmNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] cont_assign 継続的代入文
  VmContAssignNode(const VlContAssign* cont_assign);

  /// @brief デストラクタ
  virtual
  ~VmContAssignNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;
    
  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // continuous assign 文
  const VlContAssign* mContAssign;
  
};

END_NAMESPACE_YM_VERILOG

#endif // VMITEMNODE_H
