
/// @file libym_verilog/pt_compact/CptStmt.cc
/// @brief CptStmt の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CptStmt.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "CptStmt.h"
#include "CptFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// statement の共通の親クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptStmt::CptStmt(const FileRegion& file_region) :
  mFileRegion(file_region)
{
}

// デストラクタ
CptStmt::~CptStmt()
{
}

// ファイル位置の取得
FileRegion
CptStmt::file_region() const
{
  return mFileRegion;
}

// @brief ステートメントの種類を表す文字列の取得
// @return ステートメントの種類を表す文字列
const char*
CptStmt::stmt_name() const
{
  switch ( type() ) {
  case kPtDisableStmt:   return "disable statment";
  case kPtEnableStmt:    return "task enable statement";
  case kPtSysEnableStmt: return "system task enable statement";
  case kPtDcStmt:        return "delay control statement";
  case kPtEcStmt:        return "event control statement";
  case kPtAssignStmt:    return "assignment";
  case kPtNbAssignStmt:  return "nonblocking assignment";
  case kPtEventStmt:     return "event statement";
  case kPtNullStmt:      return "null statement";
  case kPtIfStmt:        return "if statement";
  case kPtCaseStmt:      return "case statement";
  case kPtCaseXStmt:     return "casex statement";
  case kPtCaseZStmt:     return "casez statement";
  case kPtWaitStmt:      return "wait statement";
  case kPtForeverStmt:   return "forever statement";
  case kPtRepeatStmt:    return "repeat statement";
  case kPtWhileStmt:     return "while statement";
  case kPtForStmt:       return "for-loop statement";
  case kPtPcAssignStmt:  return "procedural continuous assignment";
  case kPtDeassignStmt:  return "deassign statement";
  case kPtForceStmt:     return "force statement";
  case kPtReleaseStmt:   return "release statement";
  case kPtNamedParBlockStmt:
  case kPtParBlockStmt:  return "parallel block";
  case kPtNamedSeqBlockStmt:
  case kPtSeqBlockStmt:  return "sequential block";
  }
  return "";
}
  
// @brief 階層ブランチの取得
// @note kDisable/kEnable/kSysEnable で意味のある関数
// @note このクラスでは NULL を返す．
PtNameBranchArray
CptStmt::namebranch_array() const
{
  return PtNameBranchArray();
}

// @brief 名前の取得
// @return 名前
// kDisable/kEnable/kSysEnable/kParBlock/kSeqBlock で意味のある関数/
// このクラスでは NULL を返す．
const char*
CptStmt::name() const
{
  return NULL;
}

// @brief 引数の数の取得
// @return 引数の数
// @note kEnable/kSysEnable で意味のある関数
ymuint32
CptStmt::arg_num() const
{
  return 0;
}

// @brief 引数の取得
// @param[in] pos 位置番号 ( 0 <= pos < arg_num() )
// @note kEnable/kSysEnable で意味のある関数
const PtExpr*
CptStmt::arg(ymuint32 pos) const
{
  return NULL;
}

// @brief コントロールの取得
// @return ディレイ/イベントコントロール
// kDc/kEc で意味のある関数
// このクラスでは NULL を返す．
const PtControl*
CptStmt::control() const
{
  return NULL;
}

// @brief 本体のステートメントの取得
// @return 本体のステートメント
// kDc/kEc/kWait/kForever/kRepeat/kWhile/kFor/kIf で意味のある関数
// このクラスでは NULL を返す．
const PtStmt*
CptStmt::body() const
{
  return NULL;
}

// @brief 式の取得
// @return 式
// kWait/kRepeat/kWhile/kFor/kIf/kCase/kCaseX/kCaseZ で意味のある関数
// このクラスでは NULL を返す．
const PtExpr*
CptStmt::expr() const
{
  return NULL;
}

// @brief 左辺式の取得
// @return 左辺式
// kAssign/kForce/kPcAssign/kRelease/kDeassign で意味のある関数
// このクラスでは NULL を返す．
const PtExpr*
CptStmt::lhs() const
{
  return NULL;
}

// @brief 右辺式の取得
// @return 右辺式
// kAssign/kForce/kPcAssign で意味のある関数
// このクラスでは NULL を返す．
const PtExpr*
CptStmt::rhs() const
{
  return NULL;
}

// @brief イベントプライマリの取得
// @return イベントプライマリ
// kEvent で意味のある関数
// このクラスでは NULL を返す．
const PtExpr*
CptStmt::primary() const
{
  return NULL;
}

// @brief 条件が成り立たなかったとき実行されるステートメントの取得
// @return 条件が成り立たなかったとき実行されるステートメント
// kIf で意味のある関数
// このクラスでは NULL を返す．
const PtStmt*
CptStmt::else_body() const
{
  return NULL;
}

// @brief case item の要素数の取得
// @return case item の要素数
// kCase/kCaseX/kCaseZ で意味のある関数
// このクラスでは 0 を返す．
ymuint32
CptStmt::caseitem_num() const
{
  return 0;
}

// @brief case item の取得
// kCase/kCaseX/kCaseZ で意味のある関数
// このクラスでは NULL を返す．
const PtCaseItem*
CptStmt::caseitem(ymuint32 /* pos */) const
{
  return NULL;
}

// @brief 初期化代入文の取得
// @return 初期化代入文
// kFor で意味のある関数
// このクラスでは NULL を返す．
const PtStmt*
CptStmt::init_stmt() const
{
  return NULL;
}
  
// @brief 繰り返し代入文の取得
// @return 繰り返し代入文
// kFor で意味のある関数
// このクラスでは NULL を返す．
const PtStmt*
CptStmt::next_stmt() const
{
  return NULL;
}
  
// @brief 宣言ヘッダ配列の取得
PtDeclHeadArray
CptStmt::declhead_array() const
{
  return PtDeclHeadArray();
}
  
// @brief 子供のステートメント配列の取得
// @note kParBlock/kSeqBlock で意味のある関数
PtStmtArray
CptStmt::stmt_array() const
{
  return PtStmtArray();
}


//////////////////////////////////////////////////////////////////////
// disable 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptDisable::CptDisable(const FileRegion& file_region,
		       const char* name) :
  CptStmt(file_region),
  mName(name)
{
}

// デストラクタ
CptDisable::~CptDisable()
{
}

// クラスの型を返す仮想関数
tPtStmtType
CptDisable::type() const
{
  return kPtDisableStmt;
}

// 末尾の名前を返す．
const char*
CptDisable::name() const
{
  return mName;
}


//////////////////////////////////////////////////////////////////////
// 階層つき名前を持つ disable 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptDisableH::CptDisableH(const FileRegion& file_region,
			 PtNameBranchArray nb_array,
			 const char* tail_name) :
  CptDisable(file_region, tail_name),
  mNbArray(nb_array)
{
}

// デストラクタ
CptDisableH::~CptDisableH()
{
}
  
// @brief 階層ブランチの取得
// @note kDisable/kEnable/kSysEnable で意味のある関数
// @note このクラスでは NULL を返す．
PtNameBranchArray
CptDisableH::namebranch_array() const
{
  return mNbArray;
}


//////////////////////////////////////////////////////////////////////
// task enable 文 / system task enable 文に共通な基底クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptEnableBase::CptEnableBase(const FileRegion& file_region,
			     const char* name,
			     PtExprArray arg_array) :
  CptStmt(file_region),
  mName(name),
  mArgArray(arg_array)
{
}

// デストラクタ
CptEnableBase::~CptEnableBase()
{
}

// 末尾の名前を返す．
const char*
CptEnableBase::name() const
{
  return mName;
}

// @brief 引数の数の取得
// @return 引数の数
// @note kEnable/kSysEnable で意味のある関数
ymuint32
CptEnableBase::arg_num() const
{
  return mArgArray.size();
}
  
// @brief 引数の取得
// @param[in] pos 位置番号 ( 0 <= pos < arg_num() )
// @note kEnable/kSysEnable で意味のある関数
const PtExpr*
CptEnableBase::arg(ymuint32 pos) const
{
  return mArgArray[pos];
}


//////////////////////////////////////////////////////////////////////
// 階層なしの名前を持つ task enable 文を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptEnable::CptEnable(const FileRegion& file_region,
		     const char* name,
		     PtExprArray arg_array) :
  CptEnableBase(file_region, name, arg_array)
{
}

// デストラクタ
CptEnable::~CptEnable()
{
}

// クラスの型を返す仮想関数
tPtStmtType
CptEnable::type() const
{
  return kPtEnableStmt;
}


//////////////////////////////////////////////////////////////////////
// 階層つき名前を持つ enable 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptEnableH::CptEnableH(const FileRegion& file_region,
		       PtNameBranchArray nb_array,
		       const char* tail_name,
		       PtExprArray arg_array) :
  CptEnable(file_region, tail_name, arg_array),
  mNbArray(nb_array)
{
}

// デストラクタ
CptEnableH::~CptEnableH()
{
}
  
// @brief 階層ブランチの取得
PtNameBranchArray
CptEnableH::namebranch_array() const
{
  return mNbArray;
}


//////////////////////////////////////////////////////////////////////
// system task enable 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptSysEnable::CptSysEnable(const FileRegion& file_region,
			   const char* task_name,
			   PtExprArray arg_array) :
  CptEnableBase(file_region, task_name, arg_array)
{
}

// デストラクタ
CptSysEnable::~CptSysEnable()
{
}

// クラスの型を返す仮想関数
tPtStmtType
CptSysEnable::type() const
{
  return kPtSysEnableStmt;
}


//////////////////////////////////////////////////////////////////////
// CtrlStmt の基底クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptCtrlStmt::CptCtrlStmt(const FileRegion& file_region,
			 PtControl* delay,
			 PtStmt* body) :
  CptStmt(file_region),
  mDelay(delay),
  mBody(body)
{
  assert_cond(delay, __FILE__, __LINE__);
  assert_cond(body, __FILE__, __LINE__);
}

// デストラクタ
CptCtrlStmt::~CptCtrlStmt()
{
}

// コントロールを返す．
const PtControl*
CptCtrlStmt::control() const
{
  return mDelay;
}

// 本体を取り出す．
const PtStmt*
CptCtrlStmt::body() const
{
  return mBody;
}


//////////////////////////////////////////////////////////////////////
// delay control statement 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptDcStmt::CptDcStmt(const FileRegion& file_region,
		     PtControl* delay,
		     PtStmt* body) :
  CptCtrlStmt(file_region, delay, body)
{
}

// デストラクタ
CptDcStmt::~CptDcStmt()
{
}

// クラスの型を返す仮想関数
tPtStmtType
CptDcStmt::type() const
{
  return kPtDcStmt;
}


//////////////////////////////////////////////////////////////////////
// event control statement 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptEcStmt::CptEcStmt(const FileRegion& file_region,
		     PtControl* event,
		     PtStmt* body) :
  CptCtrlStmt(file_region, event, body)
{
}

// デストラクタ
CptEcStmt::~CptEcStmt()
{
}

// クラスの型を返す仮想関数
tPtStmtType
CptEcStmt::type() const
{
  return kPtEcStmt;
}


//////////////////////////////////////////////////////////////////////
// wait 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptWait::CptWait(const FileRegion& file_region,
		 PtExpr* cond,
		 PtStmt* body) :
  CptStmt(file_region),
  mCond(cond),
  mBody(body)
{
  assert_cond(cond, __FILE__, __LINE__);
  assert_cond(body, __FILE__, __LINE__);
}

// デストラクタ
CptWait::~CptWait()
{
}

// クラスの型を返す仮想関数
tPtStmtType
CptWait::type() const
{
  return kPtWaitStmt;
}

// 条件を返す．
const PtExpr*
CptWait::cond() const
{
  return mCond;
}

// 実行すべき本体を返す．
const PtStmt*
CptWait::body() const
{
  return mBody;
}


//////////////////////////////////////////////////////////////////////
// 代入文系の基底クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptAssignBase::CptAssignBase(const FileRegion& file_region,
			     PtExpr* lhs) :
  CptStmt(file_region),
  mLhs(lhs)
{
  assert_cond(lhs, __FILE__, __LINE__);
}

// デストラクタ
CptAssignBase::~CptAssignBase()
{
}

// blocking assignment の時 true を返す．
bool
CptAssignBase::is_blocking() const
{
  return true;
}

// lhs を得る．
const PtExpr*
CptAssignBase::lhs() const
{
  return mLhs;
}


//////////////////////////////////////////////////////////////////////
// 代入文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptAssign::CptAssign(const FileRegion& file_region,
		     PtExpr* lhs,
		     PtExpr* rhs) :
  CptAssignBase(file_region, lhs),
  mRhs(rhs)
{
  assert_cond(rhs, __FILE__, __LINE__);
}

// デストラクタ
CptAssign::~CptAssign()
{
}

// クラスの型を返す仮想関数
tPtStmtType
CptAssign::type() const
{
  return kPtAssignStmt;
}

// rhs を得る．
const PtExpr*
CptAssign::rhs() const
{
  return mRhs;
}


//////////////////////////////////////////////////////////////////////
// コントロールつき代入文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptAssignC::CptAssignC(const FileRegion& file_region,
		       PtExpr* lhs,
		       PtExpr* rhs,
		       PtControl* control) :
  CptAssign(file_region, lhs, rhs),
  mControl(control)
{
  assert_cond(control, __FILE__, __LINE__);
}

// デストラクタ
CptAssignC::~CptAssignC()
{
}

// コントロールを返す．
const PtControl*
CptAssignC::control() const
{
  return mControl;
}


//////////////////////////////////////////////////////////////////////
// ノンブロッキング代入文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptNbAssign::CptNbAssign(const FileRegion& file_region,
			 PtExpr* lhs,
			 PtExpr* rhs) :
  CptAssign(file_region, lhs, rhs)
{
}

// デストラクタ
CptNbAssign::~CptNbAssign()
{
}

// blocking assignment の時 true を返す．
bool
CptNbAssign::is_blocking() const
{
  return false;
}


//////////////////////////////////////////////////////////////////////
// コントロールつきノンブロッキング代入文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptNbAssignC::CptNbAssignC(const FileRegion& file_region,
			   PtExpr* lhs,
			   PtExpr* rhs,
			   PtControl* control) :
  CptAssignC(file_region, lhs, rhs, control)
{
}

// デストラクタ
CptNbAssignC::~CptNbAssignC()
{
}

// blocking assignment の時 true を返す．
bool
CptNbAssignC::is_blocking() const
{
  return false;
}


//////////////////////////////////////////////////////////////////////
// procedural continuous assignment 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptPcAssign::CptPcAssign(const FileRegion& file_region,
			 PtExpr* lhs,
			 PtExpr* rhs) :
  CptAssignBase(file_region, lhs),
  mRhs(rhs)
{
  assert_cond(rhs, __FILE__, __LINE__);
}

// デストラクタ
CptPcAssign::~CptPcAssign()
{
}

// クラスの型を返す仮想関数
tPtStmtType
CptPcAssign::type() const
{
  return kPtPcAssignStmt;
}

// 右辺式を返す．
const PtExpr*
CptPcAssign::rhs() const
{
  return mRhs;
}


//////////////////////////////////////////////////////////////////////
// deassign 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptDeassign::CptDeassign(const FileRegion& file_region,
			 PtExpr* lhs) :
  CptAssignBase(file_region, lhs)
{
}

// デストラクタ
CptDeassign::~CptDeassign()
{
}

// クラスの型を返す仮想関数
tPtStmtType
CptDeassign::type() const
{
  return kPtDeassignStmt;
}


//////////////////////////////////////////////////////////////////////
// force 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptForce::CptForce(const FileRegion& file_region,
		   PtExpr* lhs,
		   PtExpr* rhs) :
  CptPcAssign(file_region, lhs, rhs)
{
}

// デストラクタ
CptForce::~CptForce()
{
}

// クラスの型を返す仮想関数
tPtStmtType
CptForce::type() const
{
  return kPtForceStmt;
}


//////////////////////////////////////////////////////////////////////
// release 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptRelease::CptRelease(const FileRegion& file_region,
		       PtExpr* lhs) :
  CptDeassign(file_region, lhs)
{
}

// デストラクタ
CptRelease::~CptRelease()
{
}

// クラスの型を返す仮想関数
tPtStmtType
CptRelease::type() const
{
  return kPtReleaseStmt;
}


//////////////////////////////////////////////////////////////////////
// event statement を表すノード
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptEventStmt::CptEventStmt(const FileRegion& file_region,
			   PtExpr* event) :
  CptStmt(file_region),
  mPrimary(event)
{
  assert_cond(event, __FILE__, __LINE__);
}

// デストラクタ
CptEventStmt::~CptEventStmt()
{
}

// クラスの型を返す仮想関数
tPtStmtType
CptEventStmt::type() const
{
  return kPtEventStmt;
}

// イベントプライマリを返す．
const PtExpr*
CptEventStmt::primary() const
{
  return mPrimary;
}


//////////////////////////////////////////////////////////////////////
// null statement 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptNullStmt::CptNullStmt(const FileRegion& file_region) :
  CptStmt(file_region)
{
}

// デストラクタ
CptNullStmt::~CptNullStmt()
{
}

// クラスの型を返す仮想関数
tPtStmtType
CptNullStmt::type() const
{
  return kPtNullStmt;
}


//////////////////////////////////////////////////////////////////////
// if 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptIf::CptIf(const FileRegion& file_region,
	     PtExpr* expr,
	     PtStmt* then_body) :
  CptStmt(file_region),
  mCond(expr),
  mThen(then_body)
{
  assert_cond(expr, __FILE__, __LINE__);
}

// デストラクタ
CptIf::~CptIf()
{
}

// クラスの型を返す仮想関数
tPtStmtType
CptIf::type() const
{
  return kPtIfStmt;
}

// 条件式を返す．
const PtExpr*
CptIf::expr() const
{
  return mCond;
}

// 成り立ったとき実行されるステートメント
const PtStmt*
CptIf::body() const
{
  return mThen;
}


//////////////////////////////////////////////////////////////////////
// else 節付きの if 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptIfElse::CptIfElse(const FileRegion& file_region,
		     PtExpr* expr,
		     PtStmt* then_body,
		     PtStmt* else_body) :
  CptIf(file_region, expr, then_body),
  mElse(else_body)
{
  assert_cond(expr, __FILE__, __LINE__);
}

// デストラクタ
CptIfElse::~CptIfElse()
{
}

// 成り立たなかったとき実行されるステートメント
const PtStmt*
CptIfElse::else_body() const
{
  return mElse;
}


//////////////////////////////////////////////////////////////////////
// case 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptCase::CptCase(const FileRegion& file_region,
		 PtExpr* expr,
		 PtCaseItemArray caseitem_array) :
  CptStmt(file_region),
  mExpr(expr),
  mCaseItemArray(caseitem_array)
{
  assert_cond(expr, __FILE__, __LINE__);
}

// デストラクタ
CptCase::~CptCase()
{
}

// クラスの型を返す仮想関数
tPtStmtType
CptCase::type() const
{
  return kPtCaseStmt;
}

// 比較される式を返す．
const PtExpr*
CptCase::expr() const
{
  return mExpr;
}

// case item の要素数を返す．
ymuint32
CptCase::caseitem_num() const
{
  return mCaseItemArray.size();
}

// case item を返す．
const PtCaseItem*
CptCase::caseitem(ymuint32 pos) const
{
  return mCaseItemArray[pos];
}


//////////////////////////////////////////////////////////////////////
// casex 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptCaseX::CptCaseX(const FileRegion& file_region,
		   PtExpr* expr,
		   PtCaseItemArray caseitem_array) :
  CptCase(file_region, expr, caseitem_array)
{
}

// デストラクタ
CptCaseX::~CptCaseX()
{
}

// クラスの型を返す仮想関数
tPtStmtType
CptCaseX::type() const
{
  return kPtCaseXStmt;
}


//////////////////////////////////////////////////////////////////////
// casez 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptCaseZ::CptCaseZ(const FileRegion& file_region,
		   PtExpr* expr,
		   PtCaseItemArray caseitem_array) :
  CptCase(file_region, expr, caseitem_array)
{
}

// デストラクタ
CptCaseZ::~CptCaseZ()
{
}

// クラスの型を返す仮想関数
tPtStmtType
CptCaseZ::type() const
{
  return kPtCaseZStmt;
}


//////////////////////////////////////////////////////////////////////
// caseitem を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptCaseItem::CptCaseItem(const FileRegion& file_region,
			 PtExprArray label_array,
			 PtStmt* body) :
  mFileRegion(file_region),
  mLabelArray(label_array),
  mBody(body)
{
  assert_cond(body, __FILE__, __LINE__);
}

// デストラクタ
CptCaseItem::~CptCaseItem()
{
}

// ファイル位置を返す．
FileRegion
CptCaseItem::file_region() const
{
  return mFileRegion;
}

// ラベルの数を得る．
// 0 の時は '*' の意味
ymuint32
CptCaseItem::label_num() const
{
  return mLabelArray.size();
}
  
// @brief ラベルの取得
// @param[in] pos 位置番号 ( 0 <= pos < label_num() )
const PtExpr*
CptCaseItem::label(ymuint32 pos) const
{
  return mLabelArray[pos];
}

// 本体のステートメント得る．
PtStmt*
CptCaseItem::body() const
{
  return mBody;
}


//////////////////////////////////////////////////////////////////////
// LoopStmt のベースクラス
//////////////////////////////////////////////////////////////////////
  
// コンストラクタ
CptLoopStmt::CptLoopStmt(const FileRegion& file_region,
			 PtStmt* body) :
  CptStmt(file_region),
  mBody(body)
{
  assert_cond(body, __FILE__, __LINE__);
}
  
// デストラクタ
CptLoopStmt::~CptLoopStmt()
{
}

// 実行すべき本体を返す．
const PtStmt*
CptLoopStmt::body() const
{
  return mBody;
}


//////////////////////////////////////////////////////////////////////
// forever 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptForever::CptForever(const FileRegion& file_region,
		       PtStmt* body) :
  CptLoopStmt(file_region, body)
{
}

// デストラクタ
CptForever::~CptForever()
{
}

// クラスの型を返す仮想関数
tPtStmtType
CptForever::type() const
{
  return kPtForeverStmt;
}


//////////////////////////////////////////////////////////////////////
// repeat 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptRepeat::CptRepeat(const FileRegion& file_region,
		     PtExpr* expr,
		     PtStmt* body) :
  CptLoopStmt(file_region, body),
  mExpr(expr)
{
  assert_cond(expr, __FILE__, __LINE__);
}

// デストラクタ
CptRepeat::~CptRepeat()
{
}

// クラスの型を返す仮想関数
tPtStmtType
CptRepeat::type() const
{
  return kPtRepeatStmt;
}

// 繰り返し式を返す．
const PtExpr*
CptRepeat::expr() const
{
  return mExpr;
}


//////////////////////////////////////////////////////////////////////
// while 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptWhile::CptWhile(const FileRegion& file_region,
		   PtExpr* cond,
		   PtStmt* body) :
  CptRepeat(file_region, cond, body)
{
}

// デストラクタ
CptWhile::~CptWhile()
{
}

// クラスの型を返す仮想関数
tPtStmtType
CptWhile::type() const
{
  return kPtWhileStmt;
}


//////////////////////////////////////////////////////////////////////
// for 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptFor::CptFor(const FileRegion& file_region,
	       PtStmt* init,
	       PtExpr* cond,
	       PtStmt* next,
	       PtStmt* body) :
  CptRepeat(file_region, cond, body),
  mInit(init),
  mNext(next)
{
  assert_cond(init, __FILE__, __LINE__);
  assert_cond(next, __FILE__, __LINE__);
}

// デストラクタ
CptFor::~CptFor()
{
}

// クラスの型を返す仮想関数
tPtStmtType
CptFor::type() const
{
  return kPtForStmt;
}

// 初期化代入式を取出す
const PtStmt*
CptFor::init_stmt() const
{
  return mInit;
}

// 繰り返し代入式を取出す
const PtStmt*
CptFor::next_stmt() const
{
  return mNext;
}


//////////////////////////////////////////////////////////////////////
// StmtBlock の基底クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptStmtBlock::CptStmtBlock(const FileRegion& file_region,
			   PtStmtArray stmt_array) :
  CptStmt(file_region),
  mStmtArray(stmt_array)
{
}

// デストラクタ
CptStmtBlock::~CptStmtBlock()
{
}
  
// @brief 子供のステートメント配列の取得
PtStmtArray
CptStmtBlock::stmt_array() const
{
  return mStmtArray;
}


//////////////////////////////////////////////////////////////////////
// 名前と宣言要素を持つ StmtBlock
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptStmtBlockN::CptStmtBlockN(const FileRegion& file_region,
			     const char* name,
			     PtDeclHeadArray declhead_array,
			     PtStmtArray stmt_array) :
  CptStmtBlock(file_region, stmt_array),
  mName(name),
  mDeclHeadArray(declhead_array)
{
}

// デストラクタ
CptStmtBlockN::~CptStmtBlockN()
{
}

// 名前を取り出す．
const char*
CptStmtBlockN::name() const
{
  return mName;
}
  
// @brief 宣言ヘッダ配列の取得
PtDeclHeadArray
CptStmtBlockN::declhead_array() const
{
  return mDeclHeadArray;
}


//////////////////////////////////////////////////////////////////////
// parallel block を表すノード
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptParBlock::CptParBlock(const FileRegion& file_region,
			 PtStmtArray stmt_array) :
  CptStmtBlock(file_region, stmt_array)
{
}

// デストラクタ
CptParBlock::~CptParBlock()
{
}

// クラスの型を返す仮想関数
tPtStmtType
CptParBlock::type() const
{
  return kPtParBlockStmt;
}


//////////////////////////////////////////////////////////////////////
// 名前と宣言要素を持つ parallel block を表すノード
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptParBlockN::CptParBlockN(const FileRegion& file_region,
			   const char* name,
			   PtDeclHeadArray declhead_array,
			   PtStmtArray stmt_array) :
  CptStmtBlockN(file_region, name, declhead_array, stmt_array)
{
}

// デストラクタ
CptParBlockN::~CptParBlockN()
{
}

// クラスの型を返す仮想関数
tPtStmtType
CptParBlockN::type() const
{
  return kPtNamedParBlockStmt;
}


//////////////////////////////////////////////////////////////////////
// sequential block を表すノード
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptSeqBlock::CptSeqBlock(const FileRegion& file_region,
			 PtStmtArray stmt_array) :
  CptStmtBlock(file_region, stmt_array)
{
}

// デストラクタ
CptSeqBlock::~CptSeqBlock()
{
}

// クラスの型を返す仮想関数
tPtStmtType
CptSeqBlock::type() const
{
  return kPtSeqBlockStmt;
}


//////////////////////////////////////////////////////////////////////
// 名前と宣言要素を持つ sequential block を表すノード
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptSeqBlockN::CptSeqBlockN(const FileRegion& file_region,
			   const char* name,
			   PtDeclHeadArray declhead_array,
			   PtStmtArray stmt_array) :
  CptStmtBlockN(file_region, name, declhead_array, stmt_array)
{
}

// デストラクタ
CptSeqBlockN::~CptSeqBlockN()
{
}

// クラスの型を返す仮想関数
tPtStmtType
CptSeqBlockN::type() const
{
  return kPtNamedSeqBlockStmt;
}

  
//////////////////////////////////////////////////////////////////////
// statement 関係
//////////////////////////////////////////////////////////////////////

// disable 文を生成する．
PtStmt*
CptFactory::new_Disable(const FileRegion& file_region,
			const char* name)
{
  ++ mNumDisable;
  void* p = alloc().get_memory(sizeof(CptDisable));
  return new (p) CptDisable(file_region, name);
}

PtStmt*
CptFactory::new_Disable(const FileRegion& file_region,
			PtNameBranchArray nb_array,
			const char* tail_name)
{
  ++ mNumDisableH;
  void* p = alloc().get_memory(sizeof(CptDisableH));
  return new (p) CptDisableH(file_region, nb_array, tail_name);
}

// enable 文を生成する．
PtStmt*
CptFactory::new_Enable(const FileRegion& file_region,
		       const char* name,
		       PtExprArray arg_array)
{
  ++ mNumEnable;
  void* p = alloc().get_memory(sizeof(CptEnable));
  return new (p) CptEnable(file_region, name, arg_array);
}

PtStmt*
CptFactory::new_Enable(const FileRegion& file_region,
		       PtNameBranchArray nb_array,
		       const char* tail_name,
		       PtExprArray arg_array)
{
  ++ mNumEnableH;
  void* p = alloc().get_memory(sizeof(CptEnableH));
  return new (p) CptEnableH(file_region, nb_array, tail_name, arg_array);
}

// system task enable 文を生成する．
PtStmt*
CptFactory::new_SysEnable(const FileRegion& file_region,
			  const char* task_name,
			  PtExprArray arg_array)
{
  ++ mNumSysEnable;
  void* p = alloc().get_memory(sizeof(CptSysEnable));
  return new (p) CptSysEnable(file_region, task_name, arg_array);
}

// delay control 文を生成する．
PtStmt*
CptFactory::new_DcStmt(const FileRegion& file_region,
		       PtControl* delay,
		       PtStmt* body)
{
  ++ mNumDcStmt;
  void* p = alloc().get_memory(sizeof(CptDcStmt));
  return new (p) CptDcStmt(file_region, delay, body);
}

// event control 文を生成する．
PtStmt*
CptFactory::new_EcStmt(const FileRegion& file_region,
		       PtControl* event,
		       PtStmt* body)
{
  ++ mNumEcStmt;
  void* p = alloc().get_memory(sizeof(CptEcStmt));
  return new (p) CptEcStmt(file_region, event, body);
}

// wait 文を生成する．
PtStmt*
CptFactory::new_Wait(const FileRegion& file_region,
		     PtExpr* cond,
		     PtStmt* body)
{
  ++ mNumWait;
  void* p = alloc().get_memory(sizeof(CptWait));
  return new (p) CptWait(file_region, cond, body);
}

// assign 文を生成する．
PtStmt*
CptFactory::new_Assign(const FileRegion& file_region,
		       PtExpr* lhs,
		       PtExpr* rhs)
{
  ++ mNumAssign;
  void* p = alloc().get_memory(sizeof(CptAssign));
  return new (p) CptAssign(file_region, lhs, rhs);
}

// assign 文を生成する．
PtStmt*
CptFactory::new_Assign(const FileRegion& file_region,
		       PtExpr* lhs,
		       PtExpr* rhs,
		       PtControl* control)
{
  ++ mNumAssignC;
  void* p = alloc().get_memory(sizeof(CptAssignC));
  return new (p) CptAssignC(file_region, lhs, rhs, control);
}

// nonblocking assign 文を生成する．
PtStmt*
CptFactory::new_NbAssign(const FileRegion& file_region,
			 PtExpr* lhs,
			 PtExpr* rhs)
{
  ++ mNumNbAssign;
  void* p = alloc().get_memory(sizeof(CptNbAssign));
  return new (p) CptNbAssign(file_region, lhs, rhs);
}

// nonblocking assign 文を生成する．
PtStmt*
CptFactory::new_NbAssign(const FileRegion& file_region,
			 PtExpr* lhs,
			 PtExpr* rhs,
			 PtControl* control)
{
  ++ mNumNbAssignC;
  void* p = alloc().get_memory(sizeof(CptNbAssignC));
  return new (p) CptNbAssignC(file_region, lhs, rhs, control);
}

// event 文を生成する．
PtStmt*
CptFactory::new_EventStmt(const FileRegion& file_region,
			  PtExpr* event)
{
  ++ mNumEventStmt;
  void* p = alloc().get_memory(sizeof(CptEventStmt));
  return new (p) CptEventStmt(file_region, event);
}

// null 文を生成する．
PtStmt*
CptFactory::new_NullStmt(const FileRegion& file_region)
{
  ++ mNumNullStmt;
  void* p = alloc().get_memory(sizeof(CptNullStmt));
  return new (p) CptNullStmt(file_region);
}

// if 文を生成する．
PtStmt*
CptFactory::new_If(const FileRegion& file_region,
		   PtExpr* expr,
		   PtStmt* then_body)
{
  ++ mNumIf;
  void* p = alloc().get_memory(sizeof(CptIf));
  return new (p) CptIf(file_region, expr, then_body);
}

// if 文を生成する．
PtStmt*
CptFactory::new_If(const FileRegion& file_region,
		   PtExpr* expr,
		   PtStmt* then_body,
		   PtStmt* else_body)
{
  ++ mNumIf;
  void* p = alloc().get_memory(sizeof(CptIfElse));
  return new (p) CptIfElse(file_region, expr, then_body, else_body);
}

// case 文を生成する．
PtStmt*
CptFactory::new_Case(const FileRegion& file_region,
		     PtExpr* expr,
		     PtCaseItemArray caseitem_array)
{
  ++ mNumCase;
  void* p = alloc().get_memory(sizeof(CptCase));
  return new (p) CptCase(file_region, expr, caseitem_array);
}

// casex 文を生成する．
PtStmt*
CptFactory::new_CaseX(const FileRegion& file_region,
		      PtExpr* expr,
		      PtCaseItemArray caseitem_array)
{
  ++ mNumCaseX;
  void* p = alloc().get_memory(sizeof(CptCaseX));
  return new (p) CptCaseX(file_region, expr, caseitem_array);
}

// casez 文を生成する．
PtStmt*
CptFactory::new_CaseZ(const FileRegion& file_region,
		      PtExpr* expr,
		      PtCaseItemArray caseitem_array)
{
  ++ mNumCaseZ;
  void* p = alloc().get_memory(sizeof(CptCaseZ));
  return new (p) CptCaseZ(file_region, expr, caseitem_array);
}

// case item を生成する．
PtCaseItem*
CptFactory::new_CaseItem(const FileRegion& file_region,
			 PtExprArray label_array,
			 PtStmt* body)
{
  ++ mNumCaseItem;
  void* p = alloc().get_memory(sizeof(CptCaseItem));
  return new (p) CptCaseItem(file_region, label_array, body);
}

// forever 文を生成する．
PtStmt*
CptFactory::new_Forever(const FileRegion& file_region,
			PtStmt* body)
{
  ++ mNumForever;
  void* p = alloc().get_memory(sizeof(CptForever));
  return new (p) CptForever(file_region, body);
}

// repeat 文を生成する．
PtStmt*
CptFactory::new_Repeat(const FileRegion& file_region,
		       PtExpr* expr,
		       PtStmt* body)
{
  ++ mNumRepeat;
  void* p = alloc().get_memory(sizeof(CptRepeat));
  return new (p) CptRepeat(file_region, expr, body);
}

// while 文を生成する．
PtStmt*
CptFactory::new_While(const FileRegion& file_region,
		      PtExpr* cond,
		      PtStmt* body)
{
  ++ mNumWhile;
  void* p = alloc().get_memory(sizeof(CptWhile));
  return new (p) CptWhile(file_region, cond, body);
}

// for 文を生成する．
PtStmt*
CptFactory::new_For(const FileRegion& file_region,
		    PtStmt* init,
		    PtExpr* cond,
		    PtStmt* next,
		    PtStmt* body)
{
  ++ mNumFor;
  void* p = alloc().get_memory(sizeof(CptFor));
  return new (p) CptFor(file_region, init, cond, next, body);
}

// procedural continuous assignment 文を生成する．
PtStmt*
CptFactory::new_PcAssign(const FileRegion& file_region,
			 PtExpr* lhs,
			 PtExpr* rhs)
{
  ++ mNumPcAssign;
  void* p = alloc().get_memory(sizeof(CptPcAssign));
  return new (p) CptPcAssign(file_region, lhs, rhs);
}

// deassign 文を生成する．
PtStmt*
CptFactory::new_Deassign(const FileRegion& file_region,
			 PtExpr* lhs)
{
  ++ mNumDeassign;
  void* p = alloc().get_memory(sizeof(CptDeassign));
  return new (p) CptDeassign(file_region, lhs);
}

// force 文を生成する．
PtStmt*
CptFactory::new_Force(const FileRegion& file_region,
		      PtExpr* lhs,
		      PtExpr* rhs)
{
  ++ mNumForce;
  void* p = alloc().get_memory(sizeof(CptForce));
  return new (p) CptForce(file_region, lhs, rhs);
}

// release 文を生成する．
PtStmt*
CptFactory::new_Release(const FileRegion& file_region,
			PtExpr* lhs)
{
  ++ mNumRelease;
  void* p = alloc().get_memory(sizeof(CptRelease));
  return new (p) CptRelease(file_region, lhs);
}

// par block を生成する．
PtStmt*
CptFactory::new_ParBlock(const FileRegion& file_region,
			 PtStmtArray stmt_array)
{
  ++ mNumParBlock;
  void* p = alloc().get_memory(sizeof(CptParBlock));
  return new (p) CptParBlock(file_region, stmt_array);
}

// par block を生成する．
PtStmt*
CptFactory::new_NamedParBlock(const FileRegion& file_region,
			      const char* name,
			      PtDeclHeadArray declhead_array,
			      PtStmtArray stmt_array)
{
  ++ mNumParBlockN;
  void* p = alloc().get_memory(sizeof(CptParBlockN));
  return new (p) CptParBlockN(file_region, name, declhead_array, stmt_array);
}

// seq block を生成する．
PtStmt*
CptFactory::new_SeqBlock(const FileRegion& file_region,
			 PtStmtArray stmt_array)
{
  ++ mNumSeqBlock;
  void* p = alloc().get_memory(sizeof(CptSeqBlock));
  return new (p) CptSeqBlock(file_region, stmt_array);
}

// seq block を生成する．
PtStmt*
CptFactory::new_NamedSeqBlock(const FileRegion& file_region,
			      const char* name,
			      PtDeclHeadArray declhead_array,
			      PtStmtArray stmt_array)
{
  ++ mNumSeqBlockN;
  void* p = alloc().get_memory(sizeof(CptSeqBlockN));
  return new (p) CptSeqBlockN(file_region, name, declhead_array, stmt_array);
}

END_NAMESPACE_YM_VERILOG
