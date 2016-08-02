#ifndef LIBYM_VERILOG_ELABORATOR_EI_EIBLOCKSTMT_H
#define LIBYM_VERILOG_ELABORATOR_EI_EIBLOCKSTMT_H

/// @file libym_verilog/elaborator/ei/EiBlockStmtStmt.h
/// @brief EiBlockStmtStmt のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiBlockStmt.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

// ステートメントを表すクラス
// IEEE Std 1364-2001 26.6.27 Process, block, statement, event statement


#include "EiStmt.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiBlockStmt EiBlockStmtStmt.h "EiBlockStmtStmt.h"
/// @brief ブロックを表すクラス
/// IEEE Std 1364-2001 26.6.27 Process, block, statement, event statement
//////////////////////////////////////////////////////////////////////
class EiBlockStmt :
  public EiStmtBase
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] stmt_num ステートメントのリストの要素数
  /// @param[in] array ステートメントのリスト用配列
  EiBlockStmt(const VlNamedObj* parent,
	      ElbProcess* process,
	      const PtStmt* pt_stmt,
	      ymuint32 stmt_num,
	      ElbStmt** array);

  /// @brief デストラクタ
  virtual
  ~EiBlockStmt();


public:
  //////////////////////////////////////////////////////////////////////
  // VlStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 子供のステートメントのリストの要素数を返す．
  virtual
  ymuint32
  child_stmt_num() const;


public:
  //////////////////////////////////////////////////////////////////////
  // EiStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 子供のステートメントを返す．
  /// @param[in] pos 位置番号
  virtual
  ElbStmt*
  _child_stmt(ymuint32 pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ステートメント数
  ymuint32 mStmtNum;

  // ステートメントのリスト
  ElbStmt** mStmtList;

};


//////////////////////////////////////////////////////////////////////
/// @class EiBegin EiBlockStmtStmt.h "EiBlockStmtStmt.h"
/// @brief sequential blockstatement を表すクラス
/// IEEE Std 1364-2001 26.6.27 Process, block, statement, event statement
//////////////////////////////////////////////////////////////////////
class EiBegin :
  public EiBlockStmt
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] stmt_num ステートメントのリストの要素数
  /// @param[in] array ステートメントのリスト用配列
  EiBegin(const VlNamedObj* parent,
	  ElbProcess* process,
	  const PtStmt* pt_stmt,
	  ymuint32 stmt_num,
	  ElbStmt** array);

  /// @brief デストラクタ
  virtual
  ~EiBegin();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;


public:
  //////////////////////////////////////////////////////////////////////
  // VlStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief function 中の実行を行う．
  virtual
  const VlNamedObj*
  func_exec(bool constant_function) const;

};


//////////////////////////////////////////////////////////////////////
/// @class EiFork EiBlockStmtStmt.h "EiBlockStmtStmt.h"
/// @brief parallel blockstatement を表すクラス
/// IEEE Std 1364-2001 26.6.27 Process, block, statement, event statement
//////////////////////////////////////////////////////////////////////
class EiFork :
  public EiBlockStmt
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] stmt_num ステートメントのリストの要素数
  /// @param[in] array ステートメントのリスト用配列
  EiFork(const VlNamedObj* parent,
	 ElbProcess* process,
	 const PtStmt* pt_stmt,
	 ymuint32 stmt_num,
	 ElbStmt** array);

  /// @brief デストラクタ
  virtual
  ~EiFork();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;


public:
  //////////////////////////////////////////////////////////////////////
  // VlStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief function 中の実行を行う．
  /// @note このクラスは function 中では使えないのでエラーとなる．
  virtual
  const VlNamedObj*
  func_exec(bool constant_function) const;

};


//////////////////////////////////////////////////////////////////////
/// @class EiNamedBlockStmt EiBlockStmtStmt.h "EiBlockStmtStmt.h"
/// @brief 名前つきのブロックを表すクラス
/// IEEE Std 1364-2001 26.6.27 Process, block, statement, event statement
//////////////////////////////////////////////////////////////////////
class EiNamedBlockStmt :
  public ElbStmt
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] block 自分自身に対応するスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] stmt_num ステートメントのリストの要素数
  /// @param[in] array ステートメントのリスト用配列
  EiNamedBlockStmt(const VlNamedObj* block,
		   ElbProcess* process,
		   ymuint32 stmt_num,
		   ElbStmt** array);

  /// @brief デストラクタ
  virtual
  ~EiNamedBlockStmt();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;


public:
  //////////////////////////////////////////////////////////////////////
  // VlStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のスコープを返す．
  virtual
  const VlNamedObj*
  parent() const;

  /// @brief 対応するスコープを返す．
  virtual
  const VlNamedObj*
  scope() const;

  /// @brief 子供ののステートメントのリストの要素数を返す．
  ymuint32
  child_stmt_num() const;


public:
  //////////////////////////////////////////////////////////////////////
  // EiStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 子供のステートメントを返す．
  /// @param[in] pos 位置番号
  ElbStmt*
  _child_stmt(ymuint32 pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 自身に対応するスコープ
  const VlNamedObj* mBlockScope;

  // 親のプロセス
  ElbProcess* mProcess;

  // ステートメント数
  ymuint32 mStmtNum;

  // ステートメントのリスト
  ElbStmt** mStmtList;

};


//////////////////////////////////////////////////////////////////////
/// @class EiNamedBegin EiBlockStmtStmt.h "EiBlockStmtStmt.h"
/// @brief 名前付き sequential blockstatement を表すクラス
/// IEEE Std 1364-2001 26.6.27 Process, block, statement, event statement
//////////////////////////////////////////////////////////////////////
class EiNamedBegin :
  public EiNamedBlockStmt
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] block 自分自身に対応するスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] stmt_num ステートメントのリストの要素数
  /// @param[in] array ステートメントのリスト用配列
  EiNamedBegin(const VlNamedObj* block,
	       ElbProcess* process,
	       ymuint32 stmt_num,
	       ElbStmt** array);

  /// @brief デストラクタ
  virtual
  ~EiNamedBegin();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;


public:
  //////////////////////////////////////////////////////////////////////
  // VlStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief function 中の実行を行う．
  virtual
  const VlNamedObj*
  func_exec(bool constant_function) const;

};


//////////////////////////////////////////////////////////////////////
/// @class EiNamedFork ElbStmt.h "ElbStmt.h"
/// @brief 名前付き parallel blockstatement を表すクラス
/// IEEE Std 1364-2001 26.6.27 Process, block, statement, event statement
//////////////////////////////////////////////////////////////////////
class EiNamedFork :
  public EiNamedBlockStmt
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] block 自分自身に対応するスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] stmt_num ステートメントのリストの要素数
  /// @param[in] array ステートメントのリスト用配列
  EiNamedFork(const VlNamedObj* block,
	      ElbProcess* process,
	      ymuint32 stmt_num,
	      ElbStmt** array);

  /// @brief デストラクタ
  virtual
  ~EiNamedFork();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;


public:
  //////////////////////////////////////////////////////////////////////
  // VlStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief function 中の実行を行う．
  /// @note このクラスは function 中では使えない．
  virtual
  const VlNamedObj*
  func_exec(bool constant_function) const;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELABORATOR_EI_EIBLOCKSTMT_H
