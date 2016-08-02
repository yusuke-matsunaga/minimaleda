#ifndef VMSTMTNODE_H
#define VMSTMTNODE_H

/// @file libym_verilog/tests/vlview/VmStmtNode.h
/// @brief VmStmtNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VmStmtNode.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "VmNode.h"
#include "ym_verilog/verilog.h"
#include "ym_verilog/vl/VlFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VmProcessListNode VmStmtNode.h
/// @brief プロセスのリストを表すノード
//////////////////////////////////////////////////////////////////////
class VmProcessListNode :
  public VmNode1
{
public:

  /// @brief コンストラクタ
  /// @param[in] vl_mgr VlMgr
  /// @param[in] process_array プロセスの配列
  VmProcessListNode(const VlMgr& vl_mgr,
		    const vector<const VlProcess*>& process_array);

  /// @brief デストラクタ
  virtual
  ~VmProcessListNode();


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

  // プロセスの配列
  vector<const VlProcess*> mProcessArray;
  
};


//////////////////////////////////////////////////////////////////////
/// @class VmProcessNode VmStmtNode.h
/// @brief プロセスを表すノード
//////////////////////////////////////////////////////////////////////
class VmProcessNode :
  public VmNode1
{
public:

  /// @brief コンストラクタ
  /// @param[in] vl_mgr VlMgr
  /// @param[in] process プロセス
  VmProcessNode(const VlMgr& vl_mgr,
		const VlProcess* process);

  /// @brief デストラクタ
  virtual
  ~VmProcessNode();


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

  // プロセス
  const VlProcess* mProcess;

};


//////////////////////////////////////////////////////////////////////
/// @class VmStmtListNode VmStmtNode.h
/// @brief ステートメントリストを表すノード
//////////////////////////////////////////////////////////////////////
class VmStmtListNode :
  public VmNode1
{
public:

  /// @brief コンストラクタ
  /// @param[in] vl_mgr VlMgr
  /// @param[in] label ラベル
  /// @param[in] stmt 親のステートメント
  VmStmtListNode(const VlMgr& vl_mgr,
		 const QString& label,
		 const VlStmt* stmt);

  /// @brief デストラクタ
  virtual
  ~VmStmtListNode();


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
  
  // ラベル
  QString mLabel;
  
  // 親のステートメント
  const VlStmt* mStmt;
  
};


//////////////////////////////////////////////////////////////////////
/// @class VmStmtNode VmStmtNode.h
/// @brief ステートメントを表すノード
//////////////////////////////////////////////////////////////////////
class VmStmtNode :
  public VmNode1
{
public:

  /// @brief コンストラクタ
  /// @param[in] vl_mgr VlMgr
  /// @param[in] label ラベル
  /// @param[in] stmt ステートメント
  VmStmtNode(const VlMgr& vl_mgr,
	     const QString& label,
	     const VlStmt* stmt);

  /// @brief デストラクタ
  virtual
  ~VmStmtNode();


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
  
  // ラベル
  QString mLabel;
  
  // ステートメント
  const VlStmt* mStmt;
  
};


//////////////////////////////////////////////////////////////////////
/// @Class VmArgListNode VmStmtNode.h
/// @brief 引数のリストを表すノード
//////////////////////////////////////////////////////////////////////
class VmArgListNode :
  public VmNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] stmt 親の enable 文
  VmArgListNode(const VlStmt* stmt);

  /// @brief デストラクタ
  virtual
  ~VmArgListNode();


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
  
  // 親の enable 文
  const VlStmt* mStmt;
  
};


//////////////////////////////////////////////////////////////////////
/// @class VmCaseItemListNode VmStmtNode.h
/// @brief caseitem のリストを表すノード
//////////////////////////////////////////////////////////////////////
class VmCaseItemListNode :
  public VmNode1
{
public:

  /// @brief コンストラクタ
  /// @param[in] vl_mgr VlMgr
  /// @param[in] stmt 親の case 文
  VmCaseItemListNode(const VlMgr& vl_mgr,
		     const VlStmt* stmt);

  /// @brief デストラクタ
  virtual
  ~VmCaseItemListNode();


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
  
  // 親の case 文
  const VlStmt* mStmt;
  
};


//////////////////////////////////////////////////////////////////////
/// @class VmCaseItemNode VmStmtNode.h
/// @brief caseitem を表すノード
//////////////////////////////////////////////////////////////////////
class VmCaseItemNode :
  public VmNode1
{
public:

  /// @brief コンストラクタ
  /// @param[in] vl_mgr VlMgr
  /// @param[in] caseitem caseitem 文
  VmCaseItemNode(const VlMgr& vl_mgr,
		 const VlCaseItem* caseitem);

  /// @brief デストラクタ
  virtual
  ~VmCaseItemNode();


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
  
  // caseitem 文を表すオブジェクト
  const VlCaseItem* mCaseItem;
  
};


//////////////////////////////////////////////////////////////////////
/// @Class VmControlNode VmStmtNode.h
/// @brief control を表すノード
//////////////////////////////////////////////////////////////////////
class VmControlNode :
  public VmNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] control コントロールを表すオブジェクト
  VmControlNode(const VlControl* control);

  /// @brief デストラクタ
  virtual
  ~VmControlNode();


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
  
  // コントロール
  const VlControl* mControl;
  
};

END_NAMESPACE_YM_VERILOG

#endif // VMSTMTNODE_H
