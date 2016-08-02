#ifndef LIBYM_VERILOG_ELB_ELBIODECL_H
#define LIBYM_VERILOG_ELB_ELBIODECL_H

/// @file libym_verilog/elaborator/include/ElbIODecl.h
/// @brief ElbIODecl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ElbIODecl.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/vl/VlIODecl.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbIOHead  ElbIODecl.h "ElbIODecl.h"
/// @brief IO のヘッダの基底クラス
//////////////////////////////////////////////////////////////////////
class ElbIOHead
{
protected:
  
  /// @brief デストラクタ
  virtual
  ~ElbIOHead() { }


public:
  //////////////////////////////////////////////////////////////////////
  // ElbIOHead の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 方向を返す．
  virtual
  tVpiDirection
  direction() const = 0;

  /// @brief 親のモジュールの取得
  virtual
  ElbModule*
  module() const = 0;

  /// @brief 親のタスクの取得
  virtual
  ElbTaskFunc*
  task() const = 0;

  /// @brief 親の関数の取得
  virtual
  ElbTaskFunc*
  function() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class ElbIODecl ElbIODecl.h <ym_verilog/vl/VlIODecl.h>
/// @brief 入出力宣言要素を表すクラス
//////////////////////////////////////////////////////////////////////
class ElbIODecl :
  public VlIODecl
{
protected:

  /// @brief デストラクタ
  virtual
  ~ElbIODecl() { }


public:
  //////////////////////////////////////////////////////////////////////
  // ElbIODecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 対応する ElbDecl を返す．
  virtual
  ElbDecl*
  _decl() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELB_ELBIODECL_H
