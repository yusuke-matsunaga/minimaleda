#ifndef LIBYM_VERILOG_ELABORATOR_EI_EISTMT_H
#define LIBYM_VERILOG_ELABORATOR_EI_EISTMT_H

/// @file libym_verilog/elaborator/ei/EiStmt.h
/// @brief EiStmt のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiStmt.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

// ステートメントを表すクラス
// IEEE Std 1364-2001 26.6.28 Assignment
// IEEE Std 1364-2001 26.6.29 Delay control
// IEEE Std 1364-2001 26.6.30 Event control
// IEEE Std 1364-2001 26.6.37 Assign statement, deassign, force, release


#include "ElbStmt.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiStmtBase EiStmt.h "EiStmt.h"
/// @brief ElbStmt の実装クラス
//////////////////////////////////////////////////////////////////////
class EiStmtBase :
  public ElbStmt
{
protected:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] pt_stmt パース木のステートメント定義
  EiStmtBase(const VlNamedObj* parent,
	     ElbProcess* process,
	     const PtStmt* pt_stmt);

  /// デストラクタ
  virtual
  ~EiStmtBase();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のスコープを返す．
  virtual
  const VlNamedObj*
  parent() const;

  /// @brief 親の process を返す．
  virtual
  ElbProcess*
  process() const;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから参照可能な関数
  //////////////////////////////////////////////////////////////////////

  /// @brief パース木の定義要素を得る．
  const PtStmt*
  pt_stmt() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のスコープ
  const VlNamedObj* mParent;

  // 親のプロセス
  ElbProcess* mProcess;

  // パース木のステートメント定義
  const PtStmt* mPtStmt;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief パース木の定義要素を得る．
inline
const PtStmt*
EiStmtBase::pt_stmt() const
{
  return mPtStmt;
}

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELABORATOR_EI_EISTMT_H
