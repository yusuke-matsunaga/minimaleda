#ifndef YM_VERILOG_PT_PTSTMT_H
#define YM_VERILOG_PT_PTSTMT_H

/// @file ym_verilog/pt/PtStmt.h
/// @brief ステートメント関係の部品クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: PtStmt.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/pt/PtBase.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtStmt PtStmt.h <ym_verilog/pt/PtStmt.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief statement の共通の親クラス
//////////////////////////////////////////////////////////////////////
class PtStmt :
  public PtBase
{
public:

  /// @brief 仮想デストラクタ
  virtual
  ~PtStmt() { }


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型の取得
  /// @return 型
  virtual
  tPtStmtType
  type() const = 0;

  /// @brief ステートメントの種類を表す文字列の取得
  /// @return ステートメントの種類を表す文字列
  virtual
  const char*
  stmt_name() const = 0;

  /// @brief 階層ブランチの先頭の取得
  /// @return 階層ブランチの先頭
  /// @note kDisable/kEnable/kSysEnable で意味のある関数
  virtual
  PtNameBranchArray
  namebranch_array() const = 0;

  /// @brief 名前の取得
  /// @return 名前
  /// @note kDisable/kEnable/kSysEnable/kParBlock/kSeqBlock で意味のある関数
  virtual
  const char*
  name() const = 0;

  /// @brief 引数の数の取得
  /// @return 引数の数
  /// @note kEnable/kSysEnable で意味のある関数
  virtual
  ymuint32
  arg_num() const = 0;

  /// @brief 引数の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < arg_num() )
  /// @note kEnable/kSysEnable で意味のある関数
  virtual
  const PtExpr*
  arg(ymuint32 pos) const = 0;

  /// @brief コントロールの取得
  /// @return ディレイ/イベントコントロール
  /// @note kDc/kEc で意味のある関数
  virtual
  const PtControl*
  control() const = 0;

  /// @brief 本体のステートメントの取得
  /// @return 本体のステートメント
  /// @note kDc/kEc/kWait/kForever/kRepeat/kWhile/kFor/kIf で意味のある関数
  virtual
  const PtStmt*
  body() const = 0;

  /// @brief 式の取得
  /// @return 式
  /// @note kWait/kRepeat/kWhile/kFor/kIf/kCase/kCaseX/kCaseZ で意味のある関数
  virtual
  const PtExpr*
  expr() const = 0;

  /// @brief 左辺式の取得
  /// @return 左辺式
  /// @note kAssign/kForce/kPcAssign/kRelease/kDeassign で意味のある関数
  virtual
  const PtExpr*
  lhs() const = 0;

  /// @brief 右辺式の取得
  /// @return 右辺式
  /// @note kAssign/kForce/kPcAssign で意味のある関数
  virtual
  const PtExpr*
  rhs() const = 0;

  /// @brief イベントプライマリの取得
  /// @return イベントプライマリ
  /// @note kEvent で意味のある関数
  virtual
  const PtExpr*
  primary() const = 0;

  /// @brief 条件が成り立たなかったとき実行されるステートメントの取得
  /// @return 条件が成り立たなかったとき実行されるステートメント
  /// @note kIf で意味のある関数
  virtual
  const PtStmt*
  else_body() const = 0;

  /// @brief case item の要素数の取得
  /// @return case item の要素数
  /// @note kCase/kCaseX/kCaseZ で意味のある関数
  virtual
  ymuint32
  caseitem_num() const = 0;

  /// @brief case item の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < caseitem_size() )
  /// @note kCase/kCaseX/kCaseZ で意味のある関数
  virtual
  const PtCaseItem*
  caseitem(ymuint32 pos) const = 0;

  /// @brief 初期化代入文の取得
  /// @return 初期化代入文
  /// @note kFor で意味のある関数
  virtual
  const PtStmt*
  init_stmt() const = 0;

  /// @brief 繰り返し代入文の取得
  /// @return 繰り返し代入文
  /// @note kFor で意味のある関数
  virtual
  const PtStmt*
  next_stmt() const = 0;

  /// @brief 宣言ヘッダ配列の取得
  /// @note kNamedParBlock/kNamedSeqBlock で意味のある関数
  virtual
  PtDeclHeadArray
  declhead_array() const = 0;

  /// @brief 子供のステートメント配列の取得
  /// @note kParBlock/kSeqBlock で意味のある関数
  virtual
  PtStmtArray
  stmt_array() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class PtCaseItem PtStmt.h <ym_verilog/pt/PtStmt.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief caseitemを表すクラス
//////////////////////////////////////////////////////////////////////
class PtCaseItem :
  public PtBase
{
public:

  /// @brief 仮想デストラクタ
  virtual
  ~PtCaseItem() { }


public:
  //////////////////////////////////////////////////////////////////////
  // PtCaseItem の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ラベルの数の取得
  /// @retval ラベルの数 通常の case ラベルの場合
  /// @retval 0 default の場合
  virtual
  ymuint32
  label_num() const = 0;

  /// @brief ラベルの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < label_num() )
  virtual
  const PtExpr*
  label(ymuint32 pos) const = 0;

  /// @brief 本体のステートメントの取得
  /// @return 本体のステートメント
  virtual
  const PtStmt*
  body() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_PT_PTSTMT_H
