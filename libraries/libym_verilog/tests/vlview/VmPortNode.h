#ifndef VMPORTNODE_H
#define VMPORTNODE_H

/// @file libym_verilog/tests/vlview/VmPortNode.h
/// @brief VmPortNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VmPortNode.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include "VmNode.h"
#include "ym_verilog/verilog.h"
#include "ym_verilog/vl/VlFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VmPortNode VmPortNode.h
/// @brief ポートを表すノード
//////////////////////////////////////////////////////////////////////
class VmPortNode :
  public VmNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] port ポートを表すオブジェクト
  VmPortNode(const VlPort* port);

  /// @brief デストラクタ
  virtual
  ~VmPortNode();


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

  // 対応するポート
  const VlPort* mPort;
  
};

END_NAMESPACE_YM_VERILOG

#endif // VMPORTNODE_H
