#ifndef YM_VERILOG_VPI_VPICONTASSIGN_H
#define YM_VERILOG_VPI_VPICONTASSIGN_H

/// @file ym_verilog/vpi/VpiContAssign.h
/// @brief 継続的代入文を表すクラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VpiContAssign.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.

// IEEE Std 1364-2001 26.6.24 Continuous assignment


#include <ym_verilog/vpi/VpiBase.h>
#include <ym_verilog/vpi/VpiIterator.h>


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VpiCaCommon VpiContAssign.h <ym_verilog/vpi/VpiContAssign.h>
/// @brief ContAssign/ContAssignBit に共通な基底クラス
/// IEEE Std 1364-2001 26.6.24 Continuous assignment
//////////////////////////////////////////////////////////////////////
class VpiCaCommon :
  public VpiBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // VpiCaCommon に固有の仮想関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief ビット幅を返す．
  virtual
  int
  size() const = 0;
  
  /// @brief 0の強さを返す．
  virtual
  tVpiStrength
  strength0() const = 0;
  
  /// @brief 1の強さを返す．
  virtual
  tVpiStrength
  strength1() const = 0;
  
  /// @brief ネット宣言中の assignment の時に true を返す．
  virtual
  bool
  has_net_decl_assign() const = 0;

  /// @brief 左辺を返す．
  virtual
  VpiExpr*
  lhs() const = 0;
  
  /// @brief 右辺を返す．
  virtual
  VpiExpr*
  rhs() const = 0;
  
  /// @brief 遅延を表す式を返す．
  virtual
  VpiExpr*
  delay() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class VpiContAssign VpiContAssign.h <ym_verilog/vpi/VpiContAssign.h>
/// @brief continuous assign のベクタを表すクラス
/// IEEE Std 1364-2001 26.6.24 Continuous assignment
//////////////////////////////////////////////////////////////////////
class VpiContAssign :
  public VpiCaCommon
{
public:
  //////////////////////////////////////////////////////////////////////
  // VpiContAssign に固有の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のモジュールを取出す
  virtual
  VpiModule*
  module() const = 0;

  /// @brief 親のスコープを取出す
  virtual
  VpiScope*
  scope() const = 0;

  /// ビット要素の取得
  virtual
  VpiContAssignBit*
  elem(ymuint32 offset) const = 0;
  
  /// @brief 個々のビットのリストの反復子を返す．
  virtual
  VpiContAssignBitIteratorImpl*
  bit_iterator_obj() = 0;

  /// @brief 個々のビットのリストの反復子を返す．
  virtual
  VpiContAssignBitIterator
  bit_iterator() = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class VpiContAssignBit VpiContAssign.h <ym_verilog/vpi/VpiContAssign.h>
/// @brief continuous assign のビットを表すクラス
/// IEEE Std 1364-2001 26.6.24 Continuous assignment
//////////////////////////////////////////////////////////////////////
class VpiContAssignBit :
  public VpiCaCommon
{
public:
  //////////////////////////////////////////////////////////////////////
  // VpiContAssignBit に固有の仮想関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief ビット位置を返す．
  virtual
  int
  offset() const = 0;
  
  /// @brief 親のベクタを返す．
  virtual
  VpiContAssign*
  parent() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_VPI_VPICONTASSIGN_H
