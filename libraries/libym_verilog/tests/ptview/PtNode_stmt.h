#ifndef PTNODE_STMT_H
#define PTNODE_STMT_H

/// @file libym_verilog/tests/ptview/PtNode_stmt.h
/// @brief PtNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: PtNode_stmt.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include "PtNode.h"
#include "ym_verilog/verilog.h"
#include "ym_verilog/pt/PtP.h"
#include "ym_verilog/pt/PtArray.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class StmtListNode PtNode_stmt.h
/// @brief ステートメントリストを表すノード
//////////////////////////////////////////////////////////////////////
class StmtListNode :
  public PtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] label ラベル
  /// @param[in] stmt_array ステートメントの配列
  StmtListNode(const QString& label,
	       const PtStmtArray& stmt_array);

  /// @brief デストラクタ
  virtual
  ~StmtListNode();


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
  
  // ステートメントの配列
  PtStmtArray mStmtArray;
  
};


//////////////////////////////////////////////////////////////////////
/// @class StmtNode PtNode_stmt.h
/// @brief ステートメントを表すノード
//////////////////////////////////////////////////////////////////////
class StmtNode :
  public PtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] label
  /// @param[in] stmt ステートの配列
  StmtNode(const QString& label,
	   const PtStmt* stmt);

  /// @brief デストラクタ
  virtual
  ~StmtNode();


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
  const PtStmt* mStmt;
  
};


//////////////////////////////////////////////////////////////////////
/// @Class ArgListNode PtNode_stmt.h
/// @brief 引数のリストを表すノード
//////////////////////////////////////////////////////////////////////
class ArgListNode :
  public PtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] stmt 親の enable 文
  ArgListNode(const PtStmt* stmt);

  /// @brief デストラクタ
  virtual
  ~ArgListNode();


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
  const PtStmt* mStmt;
  
};


//////////////////////////////////////////////////////////////////////
/// @class CaseItemListNode PtNode_stmt.h
/// @brief caseitem のリストを表すノード
//////////////////////////////////////////////////////////////////////
class CaseItemListNode :
  public PtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] stmt 親の case 文
  CaseItemListNode(const PtStmt* stmt);

  /// @brief デストラクタ
  virtual
  ~CaseItemListNode();


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
  const PtStmt* mStmt;
  
};


//////////////////////////////////////////////////////////////////////
/// @class CaseItemNode PtNode_stmt.h
/// @brief caseitem を表すノード
//////////////////////////////////////////////////////////////////////
class CaseItemNode :
  public PtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] caseitem caseitem 文
  CaseItemNode(const PtCaseItem* caseitem);

  /// @brief デストラクタ
  virtual
  ~CaseItemNode();


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
  
  // caseitem 文を表すパース木
  const PtCaseItem* mCaseItem;
  
};

END_NAMESPACE_YM_VERILOG

#endif // PTNODE_STMT_H
