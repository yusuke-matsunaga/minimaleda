#ifndef LIBYM_VERILOG_ELB_ELBPROCESS_H
#define LIBYM_VERILOG_ELB_ELBPROCESS_H

/// @file libym_verilog/elaborator/include/ElbProcess.h
/// @brief ElbProcess のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ElbProcess.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/vl/VlProcess.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbProcess ElbProcess.h <ym_verilog/vl/VlProcess.h>
/// @brief initial/always 文を表すクラス
/// IEEE Std 1364-2001 26.6.27 Process, block, statement, event statement
//////////////////////////////////////////////////////////////////////
class ElbProcess :
  public VlProcess
{
  friend class CellProcess;

protected:
  
  /// @brief コンストラクタ
  ElbProcess();
  
  /// @brief デストラクタ
  virtual
  ~ElbProcess();
  

public:
  //////////////////////////////////////////////////////////////////////
  // ElbProcess の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 本体のステートメントをセットする．
  /// @param[in] stmt 本体のステートメント
  virtual
  void
  set_stmt(ElbStmt* stmt) = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbProcess の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素を得る．
  const ElbProcess*
  next() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 次の要素を指すポインタ
  ElbProcess* mNext;
  
};

//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
ElbProcess::ElbProcess() :
  mNext(NULL)
{
}

// @brief デストラクタ
inline
ElbProcess::~ElbProcess()
{
}

// @brief 次の要素を得る．
inline
const ElbProcess*
ElbProcess::next() const
{
  return mNext;
}

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELB_ELBPROCESS_H
