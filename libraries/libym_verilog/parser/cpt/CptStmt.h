#ifndef LIBYM_VERILOG_PT_COMPACT_CPTSTMT_H
#define LIBYM_VERILOG_PT_COMPACT_CPTSTMT_H

/// @file libym_verilog/pt_compact/CptStmt.h
/// @brief CptStmt のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CptStmt.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/pt/PtStmt.h"
#include "ym_verilog/pt/PtArray.h"

#include "ym_utils/FileRegion.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @brief statement の共通の親クラス
//////////////////////////////////////////////////////////////////////
class CptStmt :
  public PtStmt
{
public:

  /// @brief コンストラクタ
  CptStmt(const FileRegion& file_region);

  /// @brief デストラクタ
  virtual
  ~CptStmt();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置の取得
  /// @return ファイル位置
  virtual
  FileRegion
  file_region() const;

  /// @brief ステートメントの種類を表す文字列の取得
  /// @return ステートメントの種類を表す文字列
  virtual
  const char*
  stmt_name() const;
  
  /// @brief 階層ブランチの取得
  /// @note kDisable/kEnable/kSysEnable で意味のある関数
  /// @note このクラスでは NULL を返す．
  virtual
  PtNameBranchArray
  namebranch_array() const;

  /// @brief 名前の取得
  /// @return 名前
  /// @note kDisable/kEnable/kSysEnable/kParBlock/kSeqBlock で意味のある関数
  /// @note このクラスでは NULL を返す．
  virtual
  const char*
  name() const;

  /// @brief 引数の数の取得
  /// @return 引数の数
  /// @note kEnable/kSysEnable で意味のある関数
  virtual
  ymuint32
  arg_num() const;
  
  /// @brief 引数の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < arg_num() )
  /// @note kEnable/kSysEnable で意味のある関数
  virtual
  const PtExpr*
  arg(ymuint32 pos) const;

  /// @brief コントロールの取得
  /// @return ディレイ/イベントコントロール
  /// @note kDc/kEc で意味のある関数
  /// @note このクラスでは NULL を返す．
  virtual
  const PtControl*
  control() const;

  /// @brief 本体のステートメントの取得
  /// @return 本体のステートメント
  /// @note kDc/kEc/kWait/kForever/kRepeat/kWhile/kFor/kIf で意味のある関数
  /// @note このクラスでは NULL を返す．
  virtual
  const PtStmt*
  body() const;

  /// @brief 式の取得
  /// @return 式
  /// @note kWait/kRepeat/kWhile/kFor/kIf/kCase/kCaseX/kCaseZ で意味のある関数
  /// @note このクラスでは NULL を返す．
  virtual
  const PtExpr*
  expr() const;

  /// @brief 左辺式の取得
  /// @return 左辺式
  /// @note kAssign/kForce/kPcAssign/kRelease/kDeassign で意味のある関数
  /// @note このクラスでは NULL を返す．
  virtual
  const PtExpr*
  lhs() const;

  /// @brief 右辺式の取得
  /// @return 右辺式
  /// @note kAssign/kForce/kPcAssign で意味のある関数
  /// @note このクラスでは NULL を返す．
  virtual
  const PtExpr*
  rhs() const;

  /// @brief イベントプライマリの取得
  /// @return イベントプライマリ
  /// @note kEvent で意味のある関数
  /// @note このクラスでは NULL を返す．
  virtual
  const PtExpr*
  primary() const;

  /// @brief 条件が成り立たなかったとき実行されるステートメントの取得
  /// @return 条件が成り立たなかったとき実行されるステートメント
  /// @note kIf で意味のある関数
  /// @note このクラスでは NULL を返す．
  virtual
  const PtStmt*
  else_body() const;

  /// @brief case item の要素数の取得
  /// @return case item の要素数
  /// @note kCase/kCaseX/kCaseZ で意味のある関数
  /// @note このクラスでは 0 を返す．
  virtual
  ymuint32
  caseitem_num() const;

  /// @brief case item の取得
  /// @note kCase/kCaseX/kCaseZ で意味のある関数
  /// @note このクラスでは NULL を返す．
  virtual
  const PtCaseItem*
  caseitem(ymuint32 pos) const;

  /// @brief 初期化代入文の取得
  /// @return 初期化代入文
  /// @note kFor で意味のある関数
  /// @note このクラスでは NULL を返す．
  virtual
  const PtStmt*
  init_stmt() const;
  
  /// @brief 繰り返し代入文の取得
  /// @return 繰り返し代入文
  /// @note kFor で意味のある関数
  /// @note このクラスでは NULL を返す．
  virtual
  const PtStmt*
  next_stmt() const;
  
  /// @brief 宣言ヘッダ配列の取得
  virtual
  PtDeclHeadArray
  declhead_array() const;
  
  /// @brief 子供のステートメント配列の取得
  /// @note kParBlock/kSeqBlock で意味のある関数
  virtual
  PtStmtArray
  stmt_array() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

};


//////////////////////////////////////////////////////////////////////
/// @brief disable 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptDisable :
  public CptStmt
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptDisable(const FileRegion& file_region,
	     const char* name);

  /// @brief デストラクタ
  virtual
  ~CptDisable();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  /// @note このクラスは kPtDisableStmt を返す．
  virtual
  tPtStmtType
  type() const;


public:
  //////////////////////////////////////////////////////////////////////
  // PtDisable の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 末尾の名前を返す．
  virtual
  const char*
  name() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 末尾の名前
  const char* mName;

};


//////////////////////////////////////////////////////////////////////
/// @brief 階層つき名前を持つ disable 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptDisableH :
  public CptDisable
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptDisableH(const FileRegion& file_region,
	      PtNameBranchArray nb_array,
	      const char* tail_name);

  /// @brief デストラクタ
  virtual
  ~CptDisableH();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の仮想関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 階層ブランチの取得
  virtual
  PtNameBranchArray
  namebranch_array() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 階層ブランチのリスト
  PtNameBranchArray mNbArray;

};


//////////////////////////////////////////////////////////////////////
/// @brief task enable 文 / system task enable 文に共通な基底クラス
//////////////////////////////////////////////////////////////////////
class CptEnableBase :
  public CptStmt
{
protected:

  /// @brief コンストラクタ
  CptEnableBase(const FileRegion& file_region,
		const char* name,
		PtExprArray arg_array);
  
  /// @brief デストラクタ
  virtual
  ~CptEnableBase();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 末尾の名前を返す．
  virtual
  const char*
  name() const;

  /// @brief 引数の数の取得
  /// @return 引数の数
  /// @note kEnable/kSysEnable で意味のある関数
  virtual
  ymuint32
  arg_num() const;
  
  /// @brief 引数の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < arg_num() )
  /// @note kEnable/kSysEnable で意味のある関数
  virtual
  const PtExpr*
  arg(ymuint32 pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 末尾の名前
  const char* mName;

  // 引数の配列
  PtExprArray mArgArray;

};


//////////////////////////////////////////////////////////////////////
/// @brief 階層なしの名前を持った enable 文
//////////////////////////////////////////////////////////////////////
class CptEnable :
  public CptEnableBase
{
  friend class CptFactory;

protected:
  
  /// @brief コンストラクタ
  CptEnable(const FileRegion& file_region,
	    const char* name,
	    PtExprArray arg_array);
  
  /// @brief デストラクタ
  virtual
  ~CptEnable();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  /// @note このクラスは kPtEnableStmt を返す．
  virtual
  tPtStmtType
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @brief 階層つきの名前をもった enable 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptEnableH :
  public CptEnable
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptEnableH(const FileRegion& file_region,
	     PtNameBranchArray nb_array,
	     const char* tail_name,
	     PtExprArray arg_array);
  
  /// @brief デストラクタ
  virtual
  ~CptEnableH();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の仮想関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 階層ブランチの取得
  virtual
  PtNameBranchArray
  namebranch_array() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 階層ブランチのリスト
  PtNameBranchArray mNbArray;

};


//////////////////////////////////////////////////////////////////////
/// @brief system task enable 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptSysEnable :
  public CptEnableBase
{
  friend class CptFactory;

private:
  
  /// @brief コンストラクタ
  CptSysEnable(const FileRegion& file_region,
	       const char* task_name,
	       PtExprArray arg_array);
  
  /// @brief デストラクタ
  virtual
  ~CptSysEnable();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  /// @note このクラスは kPtSysEnableStmt を返す．
  virtual
  tPtStmtType
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @brief CtrlStmt の基底クラス
//////////////////////////////////////////////////////////////////////
class CptCtrlStmt :
  public CptStmt
{
protected:
  
  /// @brief コンストラクタ
  CptCtrlStmt(const FileRegion& file_region,
	      PtControl* delay,
	      PtStmt* body);
  
  /// @brief デストラクタ
  virtual
  ~CptCtrlStmt();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief コントロールを返す．
  virtual
  const PtControl*
  control() const;

  /// @brief 本体を取り出す．
  virtual
  const PtStmt*
  body() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // コントロール
  PtControl* mDelay;

  // 本体
  PtStmt* mBody;

};


//////////////////////////////////////////////////////////////////////
/// @brief delay control statement
//////////////////////////////////////////////////////////////////////
class CptDcStmt :
  public CptCtrlStmt
{
  friend class CptFactory;
  
private:
  
  /// @brief コンストラクタ
  CptDcStmt(const FileRegion& file_region,
	    PtControl* delay,
	    PtStmt* body);
  
  /// @brief デストラクタ
  virtual
  ~CptDcStmt();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  /// @note このクラスは kPtDcStmt を返す．
  virtual
  tPtStmtType
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @brief event control statement
//////////////////////////////////////////////////////////////////////
class CptEcStmt :
  public CptCtrlStmt
{
  friend class CptFactory;

private:
  
  /// @brief コンストラクタ
  CptEcStmt(const FileRegion& file_region,
	    PtControl* event,
	    PtStmt* body);
  
  /// @brief デストラクタ
  virtual
  ~CptEcStmt();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  /// @note このクラスは kPtEcStmt を返す．
  virtual
  tPtStmtType
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @brief wait 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptWait :
  public CptStmt
{
  friend class CptFactory;

private:
  
  /// @brief コンストラクタ
  CptWait(const FileRegion& file_region,
	  PtExpr* cond,
	  PtStmt* body);
  
  /// @brief デストラクタ
  virtual
  ~CptWait();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  /// @note このクラスは kPtWaitStmt を返す．
  virtual
  tPtStmtType
  type() const;

  /// @brief 条件を返す．
  virtual
  const PtExpr*
  cond() const;

  /// @brief 実行すべき本体を返す．
  virtual
  const PtStmt*
  body() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 条件
  PtExpr* mCond;

  // 実行すべき本体
  PtStmt* mBody;

};


//////////////////////////////////////////////////////////////////////
/// @brief 代入文系の基底クラス
//////////////////////////////////////////////////////////////////////
class CptAssignBase :
  public CptStmt
{
protected:

  /// @brief コンストラクタ
  CptAssignBase(const FileRegion& file_region,
		PtExpr* lhs);
  
  /// @brief デストラクタ
  virtual
  ~CptAssignBase();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief blocking assignment の時 true を返す．
  /// @note デフォルトでは true を返す．
  virtual
  bool
  is_blocking() const;

  /// @brief lhs を得る．
  virtual
  const PtExpr*
  lhs() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 左辺
  PtExpr* mLhs;

};


//////////////////////////////////////////////////////////////////////
/// @brief 代入文
//////////////////////////////////////////////////////////////////////
class CptAssign :
  public CptAssignBase
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptAssign(const FileRegion& file_region,
	    PtExpr* lhs,
	    PtExpr* rhs);
  
  /// @brief デストラクタ
  virtual
  ~CptAssign();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  /// @note このクラスは kPtAssignStmt を返す．
  virtual
  tPtStmtType
  type() const;

  /// @brief rhs を得る．
  virtual
  const PtExpr*
  rhs() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 右辺
  PtExpr* mRhs;

};


//////////////////////////////////////////////////////////////////////
/// @brief コントロールつきの代入文
//////////////////////////////////////////////////////////////////////
class CptAssignC :
  public CptAssign
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptAssignC(const FileRegion& file_region,
	     PtExpr* lhs,
	     PtExpr* rhs,
	     PtControl* control);
  
  /// @brief デストラクタ
  virtual
  ~CptAssignC();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief コントロールを返す．
  virtual
  const PtControl*
  control() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // コントロール
  PtControl* mControl;

};


//////////////////////////////////////////////////////////////////////
/// @brief ノンブロッキング代入文
//////////////////////////////////////////////////////////////////////
class CptNbAssign :
  public CptAssign
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptNbAssign(const FileRegion& file_region,
	      PtExpr* lhs,
	      PtExpr* rhs);
  
  /// @brief デストラクタ
  virtual
  ~CptNbAssign();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief blocking assignment の時 true を返す．
  /// @note ここでは false を返す．
  virtual
  bool
  is_blocking() const;

};


//////////////////////////////////////////////////////////////////////
/// @brief コントロールつきノンブロッキング代入文
//////////////////////////////////////////////////////////////////////
class CptNbAssignC :
  public CptAssignC
{
  friend class CptFactory;

private:

  /// @brief コンストラクタ
  CptNbAssignC(const FileRegion& file_region,
	       PtExpr* lhs,
	       PtExpr* rhs,
	       PtControl* control);
  
  /// @brief デストラクタ
  virtual
  ~CptNbAssignC();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief blocking assignment の時 true を返す．
  /// @note ここでは false を返す．
  virtual
  bool
  is_blocking() const;

};


//////////////////////////////////////////////////////////////////////
/// @brief procedural continuous assigment 文
//////////////////////////////////////////////////////////////////////
class CptPcAssign :
  public CptAssignBase
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptPcAssign(const FileRegion& file_region,
	      PtExpr* lhs,
	      PtExpr* rhs);
  
  /// @brief デストラクタ
  virtual
  ~CptPcAssign();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  /// @note このクラスは kPtPcaAssignStmt を返す．
  virtual
  tPtStmtType
  type() const;

  /// @brief 右辺式を返す．
  virtual
  const PtExpr*
  rhs() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 右辺
  PtExpr* mRhs;

};


//////////////////////////////////////////////////////////////////////
/// @brief deassigment 文
//////////////////////////////////////////////////////////////////////
class CptDeassign :
  public CptAssignBase
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptDeassign(const FileRegion& file_region,
	      PtExpr* lhs);
  
  /// @brief デストラクタ
  virtual
  ~CptDeassign();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  /// @note このクラスは kPtDeassignStmt を返す．
  virtual
  tPtStmtType
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @brief force 文
//////////////////////////////////////////////////////////////////////
class CptForce :
  public CptPcAssign
{
  friend class CptFactory;
  
private:

  /// @brief コンストラクタ
  CptForce(const FileRegion& file_region,
	   PtExpr* lhs,
	   PtExpr* rhs);
  
  /// @brief デストラクタ
  virtual
  ~CptForce();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  /// @note このクラスは kPtForceStmt を返す．
  virtual
  tPtStmtType
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @brief release 文
//////////////////////////////////////////////////////////////////////
class CptRelease :
  public CptDeassign
{
  friend class CptFactory;

private:

  /// @brief コンストラクタ
  CptRelease(const FileRegion& file_region,
	     PtExpr* lhs);
  
  /// @brief デストラクタ
  virtual
  ~CptRelease();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  /// @note このクラスは kPtReleaseStmt を返す．
  virtual
  tPtStmtType
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @brief event statement を表すノード
//////////////////////////////////////////////////////////////////////
class CptEventStmt :
  public CptStmt
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptEventStmt(const FileRegion& file_region,
	       PtExpr* event);

  /// @brief デストラクタ
  virtual
  ~CptEventStmt();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  /// @note このクラスは kPtEventStmt を返す．
  virtual
  tPtStmtType
  type() const;

  /// @brief イベントプライマリを返す．
  virtual
  const PtExpr*
  primary() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // イベント名を表すプライマリ
  PtExpr* mPrimary;

};


//////////////////////////////////////////////////////////////////////
/// @brief null statement を表すノード
//////////////////////////////////////////////////////////////////////
class CptNullStmt :
  public CptStmt
{
  friend class CptFactory;

private:
  
  /// @brief コンストラクタ
  CptNullStmt(const FileRegion& file_region);
  
  /// @brief デストラクタ
  virtual
  ~CptNullStmt();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  /// @note このクラスは kPtNullStmt を返す．
  virtual
  tPtStmtType
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @brief if 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptIf :
  public CptStmt
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptIf(const FileRegion& file_region,
	PtExpr* expr,
	PtStmt* then_body);
  
  /// @brief デストラクタ
  virtual
  ~CptIf();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  /// @note このクラスは kPtIfStmt を返す．
  virtual
  tPtStmtType
  type() const;

  /// @brief 条件式を返す．
  virtual
  const PtExpr*
  expr() const;

  /// @brief 成り立ったとき実行されるステートメント
  virtual
  const PtStmt*
  body() const;
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 条件式
  PtExpr* mCond;

  // 成り立ったとき実行されるステートメント
  PtStmt* mThen;

};


//////////////////////////////////////////////////////////////////////
/// @brief else 節付きの if 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptIfElse :
  public CptIf
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptIfElse(const FileRegion& file_region,
	    PtExpr* expr,
	    PtStmt* then_body,
	    PtStmt* else_body);
  
  /// @brief デストラクタ
  virtual
  ~CptIfElse();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 成り立たなかったとき実行されるステートメント
  virtual
  const PtStmt*
  else_body() const;
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 成り立たなかったとき実行されるステートメント
  PtStmt* mElse;

};


//////////////////////////////////////////////////////////////////////
/// @brief caseitemを表すクラス
//////////////////////////////////////////////////////////////////////
class CptCaseItem :
  public PtCaseItem
{
  friend class CptFactory;

private:

  /// @brief コンストラクタ
  CptCaseItem(const FileRegion& file_region,
	      PtExprArray label_array,
	      PtStmt* body);
  
  /// @brief デストラクタ
  ~CptCaseItem();


public:
  //////////////////////////////////////////////////////////////////////
  // PtCaseItem の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;

  /// @brief ラベルの数の取得
  /// @retval ラベルの数 通常の case ラベルの場合
  /// @retval 0 default の場合
  virtual
  ymuint32
  label_num() const;
  
  /// @brief ラベルの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < label_num() )
  virtual
  const PtExpr*
  label(ymuint32 pos) const;

  /// @brief 本体のステートメント得る．
  virtual
  PtStmt*
  body() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // ラベルの配列
  PtExprArray mLabelArray;
  
  // ラベルが一致したときに実行されるステートメント
  PtStmt* mBody;

};


//////////////////////////////////////////////////////////////////////
/// @brief case 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptCase :
  public CptStmt
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptCase(const FileRegion& file_region,
	  PtExpr* expr,
	  PtCaseItemArray caseitem_array);
  
  /// @brief デストラクタ
  virtual
  ~CptCase();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  /// @note このクラスは kPtCaseStmt を返す．
  virtual
  tPtStmtType
  type() const;

  /// @brief 比較される式を返す．
  virtual
  const PtExpr*
  expr() const;

  /// @brief case item の要素数を返す．
  virtual
  ymuint32
  caseitem_num() const;

  /// @brief case item を返す．
  virtual
  const PtCaseItem*
  caseitem(ymuint32 pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 比較される式
  PtExpr* mExpr;
  
  // case item の配列
  PtCaseItemArray mCaseItemArray;

};


//////////////////////////////////////////////////////////////////////
/// @brief casex 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptCaseX :
  public CptCase
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptCaseX(const FileRegion& file_region,
	   PtExpr* expr,
	   PtCaseItemArray caseitem_array);
  
  /// @brief デストラクタ
  virtual
  ~CptCaseX();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  /// @note ここでは kPtCaseXStmt を返す．
  virtual
  tPtStmtType
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @brief casez 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptCaseZ :
  public CptCase
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptCaseZ(const FileRegion& file_region,
	   PtExpr* expr,
	   PtCaseItemArray caseitem_array);
  
  /// @brief デストラクタ
  virtual
  ~CptCaseZ();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  /// @note ここでは kPtCaseZStmt を返す．
  virtual
  tPtStmtType
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @brief LoopStmt のベースクラス
//////////////////////////////////////////////////////////////////////
class CptLoopStmt :
  public CptStmt
{
protected:

  /// @brief コンストラクタ
  CptLoopStmt(const FileRegion& file_region,
	      PtStmt* body);
  
  /// @brief デストラクタ
  virtual
  ~CptLoopStmt();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 実行すべき本体を返す．
  virtual
  const PtStmt*
  body() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実行すべき本体
  PtStmt* mBody;

};


//////////////////////////////////////////////////////////////////////
/// @brief forever 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptForever :
  public CptLoopStmt
{
  friend class CptFactory;

private:

  /// @brief コンストラクタ
  CptForever(const FileRegion& file_region,
	     PtStmt* body);
  
  /// @brief デストラクタ
  virtual
  ~CptForever();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  /// @note このクラスは kPtForeverStmt を返す．
  virtual
  tPtStmtType
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @brief repeat 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptRepeat :
  public CptLoopStmt
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptRepeat(const FileRegion& file_region,
	    PtExpr* expr,
	    PtStmt* body);
  
  /// @brief デストラクタ
  virtual
  ~CptRepeat();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  /// @note このクラスは kPtRepeatStmt を返す．
  virtual
  tPtStmtType
  type() const;

  /// @brief 繰り返し式を返す．
  virtual
  const PtExpr*
  expr() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 繰り返し式
  PtExpr* mExpr;

};


//////////////////////////////////////////////////////////////////////
/// @brief while 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptWhile :
  public CptRepeat // ちょっと汚い継承
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptWhile(const FileRegion& file_region,
	   PtExpr* cond,
	   PtStmt* body);
  
  /// @brief デストラクタ
  virtual
  ~CptWhile();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  /// @note このクラスは kPtWhileStmt を返す．
  virtual
  tPtStmtType
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @brief for 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptFor :
  public CptRepeat
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptFor(const FileRegion& file_region,
	 PtStmt* init,
	 PtExpr* cond,
	 PtStmt* next,
	 PtStmt* body);
  
  /// @brief デストラクタ
  virtual
  ~CptFor();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  /// @note このクラスは kPtForStmt を返す．
  virtual
  tPtStmtType
  type() const;

  /// @brief 初期化代入式を取出す
  virtual
  const PtStmt*
  init_stmt() const;
  
  /// @brief 繰り返し代入式を取出す
  virtual
  const PtStmt*
  next_stmt() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 初期化文
  PtStmt* mInit;

  // 増加処理文
  PtStmt* mNext;
  
};


//////////////////////////////////////////////////////////////////////
/// @brief StmtBlock の基底クラス
//////////////////////////////////////////////////////////////////////
class CptStmtBlock :
  public CptStmt
{
protected:

  /// @brief コンストラクタ
  CptStmtBlock(const FileRegion& file_region,
	       PtStmtArray stmt_array);
  
  /// @brief デストラクタ
  virtual
  ~CptStmtBlock();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtStmtBlock の仮想関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 子供のステートメント配列の取得
  /// @note kParBlock/kSeqBlock で意味のある関数
  virtual
  PtStmtArray
  stmt_array() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // ステートメントの配列
  PtStmtArray mStmtArray;

};


//////////////////////////////////////////////////////////////////////
/// @brief 名前と宣言要素を持つ StmtBlock
//////////////////////////////////////////////////////////////////////
class CptStmtBlockN :
  public CptStmtBlock
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptStmtBlockN(const FileRegion& file_region,
		const char* name,
		PtDeclHeadArray declhead_array,
		PtStmtArray stmt_array);
  
  /// @brief デストラクタ
  virtual
  ~CptStmtBlockN();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を取り出す．
  virtual
  const char*
  name() const;
  
  /// @brief 宣言ヘッダ配列の取得
  virtual
  PtDeclHeadArray
  declhead_array() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  const char* mName;

  // 宣言の配列
  PtDeclHeadArray mDeclHeadArray;

};


//////////////////////////////////////////////////////////////////////
/// @brief parallel block を表すノード
//////////////////////////////////////////////////////////////////////
class CptParBlock :
  public CptStmtBlock
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptParBlock(const FileRegion& file_region,
	      PtStmtArray stmt_array);
  
  /// @brief デストラクタ
  virtual
  ~CptParBlock();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  /// @note このクラスは kPtParBlockStmt を返す．
  virtual
  tPtStmtType
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @brief 名前と宣言要素を持つ parallel block を表すノード
//////////////////////////////////////////////////////////////////////
class CptParBlockN :
  public CptStmtBlockN
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptParBlockN(const FileRegion& file_region,
	       const char* name,
	       PtDeclHeadArray declhead_array,
	       PtStmtArray stmt_array);
  
  /// @brief デストラクタ
  virtual
  ~CptParBlockN();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  /// @note このクラスは kPtNamedParBlockStmt を返す．
  virtual
  tPtStmtType
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @brief sequential block を表すノード
//////////////////////////////////////////////////////////////////////
class CptSeqBlock :
  public CptStmtBlock
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptSeqBlock(const FileRegion& file_region,
	      PtStmtArray stmt_array);
  
  /// @brief デストラクタ
  virtual
  ~CptSeqBlock();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  /// @note このクラスは kPtSeqBlockStmt を返す．
  virtual
  tPtStmtType
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @brief 名前と宣言要素を持つ sequential block を表すノード
//////////////////////////////////////////////////////////////////////
class CptSeqBlockN :
  public CptStmtBlockN
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptSeqBlockN(const FileRegion& file_region,
	       const char* name,
	       PtDeclHeadArray declhead_array,
	       PtStmtArray stmt_array);
  
  /// @brief デストラクタ
  virtual
  ~CptSeqBlockN();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  /// @note このクラスは kPtNamedSeqBlockStmt を返す．
  virtual
  tPtStmtType
  type() const;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_PT_COMPACT_CPTSTMT_H
