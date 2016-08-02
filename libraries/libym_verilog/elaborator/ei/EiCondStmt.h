#ifndef LIBYM_VERILOG_ELABORATOR_EI_EICONDSTMT_H
#define LIBYM_VERILOG_ELABORATOR_EI_EICONDSTMT_H

/// @file libym_verilog/elaborator/ei/EiCondStmt.h
/// @brief EiCondStmt のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiCondStmt.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

// ステートメントを表すクラス
// IEEE Std 1364-2001 26.6.32 While, repeat, wait
// IEEE Std 1364-2001 26.6.33 For
// IEEE Std 1364-2001 26.6.34 Forever
// IEEE Std 1364-2001 26.6.35 If, if-else
// IEEE Std 1364-2001 26.6.36 Case


#include "EiStmt.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiLoopStmt EiCondStmt.h "EiCondStmt.h"
/// @brief while 文/ repeat 文/ wait 文 の基底クラス
/// IEEE Std 1364-2001 26.6.32 While, repeat, wait
//////////////////////////////////////////////////////////////////////
class EiLoopStmt :
  public EiStmtBase
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] cond 条件式
  /// @param[in] stmt 本体のステートメント
  EiLoopStmt(const VlNamedObj* parent,
	     ElbProcess* process,
	     const PtStmt* pt_stmt,
	     ElbExpr* cond,
	     ElbStmt* stmt);

  /// @brief デストラクタ
  virtual
  ~EiLoopStmt();


public:
  //////////////////////////////////////////////////////////////////////
  // VlStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 条件式を返す．
  virtual
  const VlExpr*
  expr() const;

  /// @brief 本体のステートメントを返す．
  virtual
  const VlStmt*
  body_stmt() const;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスで用いられる関数
  //////////////////////////////////////////////////////////////////////

  ElbStmt*
  _body_stmt() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 条件式
  ElbExpr* mCondition;

  // 本体
  ElbStmt* mBodyStmt;

};


//////////////////////////////////////////////////////////////////////
/// @class EiWhileStmt EiCondStmt.h "EiCondStmt.h"
/// @brief while 文を表すクラス
/// IEEE Std 1364-2001 26.6.32 While, repeat, wait
//////////////////////////////////////////////////////////////////////
class EiWhileStmt :
  public EiLoopStmt
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] cond 条件式
  /// @param[in] stmt 本体のステートメント
  EiWhileStmt(const VlNamedObj* parent,
	      ElbProcess* process,
	      const PtStmt* pt_stmt,
	      ElbExpr* cond,
	      ElbStmt* stmt);

  /// @brief デストラクタ
  virtual
  ~EiWhileStmt();


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
  // ElbStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief function 中の実行を行う．
  virtual
  const VlNamedObj*
  func_exec(bool constant_function) const;

};


//////////////////////////////////////////////////////////////////////
/// @class EiRepeatStmt EiCondStmt.h "EiCondStmt.h"
/// @brief repeat 文を表すクラス
/// IEEE Std 1364-2001 26.6.32 While, repeat, wait
//////////////////////////////////////////////////////////////////////
class EiRepeatStmt :
  public EiLoopStmt
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] cond 条件式
  /// @param[in] stmt 本体のステートメント
  EiRepeatStmt(const VlNamedObj* parent,
	       ElbProcess* process,
	       const PtStmt* pt_stmt,
	       ElbExpr* cond,
	       ElbStmt* stmt);

  /// @brief デストラクタ
  virtual
  ~EiRepeatStmt();


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
  // ElbStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief function 中の実行を行う．
  virtual
  const VlNamedObj*
  func_exec(bool constant_function) const;

};


//////////////////////////////////////////////////////////////////////
/// @class EiWaitStmt EiCondStmt.h "EiCondStmt.h"
/// @brief wait 文を表すクラス
/// IEEE Std 1364-2001 26.6.32 While, repeat, wait
//////////////////////////////////////////////////////////////////////
class EiWaitStmt :
  public EiLoopStmt
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] cond 条件式
  /// @param[in] stmt 本体のステートメント
  EiWaitStmt(const VlNamedObj* parent,
	     ElbProcess* process,
	     const PtStmt* pt_stmt,
	     ElbExpr* cond,
	     ElbStmt* stmt);

  /// @brief デストラクタ
  virtual
  ~EiWaitStmt();


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
  // ElbStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief function 中の実行を行う．
  /// @note このクラスは function 中では使えない．
  virtual
  const VlNamedObj*
  func_exec(bool constant_function) const;

};


//////////////////////////////////////////////////////////////////////
/// @class EiForStmt EiCondStmt.h "EiCondStmt.h"
/// @brief for statement を表すクラス
/// IEEE Std 1364-2001 26.6.33 For
//////////////////////////////////////////////////////////////////////
class EiForStmt :
  public EiLoopStmt
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] cond 条件式
  /// @param[in] init_stmt 初期化式
  /// @param[in] inc_stmt 増加式
  /// @param[in] stmt 本体のステートメント
  EiForStmt(const VlNamedObj* parent,
	    ElbProcess* process,
	    const PtStmt* pt_stmt,
	    ElbExpr* cond,
	    ElbStmt* init_stmt,
	    ElbStmt* inc_stmt,
	    ElbStmt* stmt);

  /// @brief デストラクタ
  virtual
  ~EiForStmt();


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

  /// 初期化文を返す．
  virtual
  const VlStmt*
  init_stmt() const;

  /// 繰り返し文を返す．
  virtual
  const VlStmt*
  inc_stmt() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief function 中の実行を行う．
  virtual
  const VlNamedObj*
  func_exec(bool constant_function) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 初期化文
  ElbStmt* mInitStmt;

  // 増加文
  ElbStmt* mIncStmt;

};


//////////////////////////////////////////////////////////////////////
/// @class EiForeverStmt EiCondStmt.h "EiCondStmt.h"
/// @brief forever statement を表すクラス
/// IEEE Std 1364-2001 26.6.34 Forever
//////////////////////////////////////////////////////////////////////
class EiForeverStmt :
  public EiStmtBase
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] stmt 本体のステートメント
  EiForeverStmt(const VlNamedObj* parent,
		ElbProcess* process,
		const PtStmt* pt_stmt,
		ElbStmt* stmt);

  /// @brief デストラクタ
  virtual
  ~EiForeverStmt();


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
  // VlStmt に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 本体のステートメントを返す．
  virtual
  const VlStmt*
  body_stmt() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief function 中の実行を行う．
  virtual
  const VlNamedObj*
  func_exec(bool constant_function) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 本体
  ElbStmt* mBodyStmt;

};


//////////////////////////////////////////////////////////////////////
/// @class EiIfStmt EiCondStmt.h "EiCondStmt.h"
/// @brief if statement を表すクラス
/// IEEE Std 1364-2001 26.6.35 If, if-else
//////////////////////////////////////////////////////////////////////
class EiIfStmt :
  public EiStmtBase
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] cond 条件式
  /// @param[in] stmt 本体のステートメント
  EiIfStmt(const VlNamedObj* parent,
	   ElbProcess* process,
	   const PtStmt* pt_stmt,
	   ElbExpr* cond,
	   ElbStmt* stmt);

  /// @brief デストラクタ
  virtual
  ~EiIfStmt();


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
  // VlStmt に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 条件式を返す．
  virtual
  const VlExpr*
  expr() const;

  /// @brief 本体のステートメントを返す．
  virtual
  const VlStmt*
  body_stmt() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief function 中の実行を行う．
  virtual
  const VlNamedObj*
  func_exec(bool constant_function) const;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスで用いられる関数
  //////////////////////////////////////////////////////////////////////

  ElbStmt*
  _body_stmt() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 条件式
  ElbExpr* mCondition;

  // 本体
  ElbStmt* mBodyStmt;

};


//////////////////////////////////////////////////////////////////////
/// @class EiIfElseStmt EiCondStmt.h "EiCondStmt.h"
/// @brief if else statement を表すクラス
/// IEEE Std 1364-2001 26.6.35 If, if-else
//////////////////////////////////////////////////////////////////////
class EiIfElseStmt :
  public EiIfStmt
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] cond 条件式
  /// @param[in] stmt 本体のステートメント
  /// @param[in] else_stmt else節のステートメント
  EiIfElseStmt(const VlNamedObj* parent,
	       ElbProcess* process,
	       const PtStmt* pt_stmt,
	       ElbExpr* cond,
	       ElbStmt* stmt,
	       ElbStmt* else_stmt);

  /// @brief デストラクタ
  virtual
  ~EiIfElseStmt();


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
  // VlStmt に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief else ステートメントを返す．
  virtual
  const VlStmt*
  else_stmt() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief function 中の実行を行う．
  virtual
  const VlNamedObj*
  func_exec(bool constant_function) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // else ステートメント
  ElbStmt* mElseStmt;

};


//////////////////////////////////////////////////////////////////////
/// @class EiCaseItem EiCondStmt.h "EiCondStmt.h"
/// @brief case item を表すクラス
/// IEEE Std 1364-2001 26.6.36 Case
//////////////////////////////////////////////////////////////////////
class EiCaseItem :
  public ElbCaseItem
{
  friend class EiFactory;
  friend class EiCaseStmt;

private:

  /// @brief コンストラクタ
  EiCaseItem();

  /// @brief デストラクタ
  virtual
  ~EiCaseItem();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;

  /// @brief ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;


public:
  //////////////////////////////////////////////////////////////////////
  // VlCaseItem に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 条件式のリストの要素数を返す．
  virtual
  ymuint32
  expr_num() const;

  /// @brief 条件式を返す．
  /// @param[in] pos 位置番号
  virtual
  const VlExpr*
  expr(ymuint32 pos) const;

  /// @brief 本体のステートメントを返す．
  virtual
  const VlStmt*
  body_stmt() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パース木の caseitem 定義
  const PtCaseItem* mPtCaseItem;

  // ラベルのリストの要素数
  ymuint32 mExprNum;

  // ラベルのリスト
  ElbExpr** mExprList;

  // 本文
  ElbStmt* mBodyStmt;

};


//////////////////////////////////////////////////////////////////////
/// @class EiCaseStmt EiCondStmt.h "EiCondStmt.h"
/// @brief case statement を表すクラス
/// IEEE Std 1364-2001 26.6.36 Case
//////////////////////////////////////////////////////////////////////
class EiCaseStmt :
  public EiStmtBase
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] expr 条件式
  /// @param[in] caseitem_num caseitem の要素数
  /// @param[in] caseitem_array caseitem の配列
  EiCaseStmt(const VlNamedObj* parent,
	     ElbProcess* process,
	     const PtStmt* pt_stmt,
	     ElbExpr* expr,
	     ymuint32 caseitem_num,
	     EiCaseItem* caseitem_array);

  /// @brief デストラクタ
  virtual
  ~EiCaseStmt();


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

  /// @brief case type を返す．
  virtual
  tVpiCaseType
  case_type() const;

  /// @brief 条件式を返す．
  virtual
  const VlExpr*
  expr() const;

  /// @brief case item のリストの要素数を返す．
  virtual
  ymuint32
  case_item_num() const;

  /// @brief case item を返す．
  /// @param[in] pos 位置番号
  virtual
  const VlCaseItem*
  case_item(ymuint32 pos) const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief caseitem を設定する．
  /// @param[in] pos 位置番号
  /// @param[in] pt_caseitem パース木の caseitem 定義
  /// @param[in] expr_array 条件式を納める配列
  /// @param[in] stmt 本体のステートメント
  virtual
  void
  set_caseitem(ymuint32 pos,
	       const PtCaseItem* pt_caseitem,
	       ElbExpr** expr_array,
	       ElbStmt* stmt);

  /// @brief function 中の実行を行う．
  virtual
  const VlNamedObj*
  func_exec(bool constant_function) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 条件式
  ElbExpr* mCondition;

  // case item の要素数
  ymuint32 mCaseItemNum;

  // case item のリスト
  EiCaseItem* mCaseItemList;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELABORATOR_EI_EICONDSTMT_H
