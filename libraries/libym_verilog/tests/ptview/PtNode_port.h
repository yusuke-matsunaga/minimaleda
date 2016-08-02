#ifndef PTNODE_PORT_H
#define PTNODE_PORT_H

/// @file libym_verilog/tests/ptview/PtNode_port.h
/// @brief PtNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: PtNode_port.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include "PtNode.h"
#include "ym_verilog/verilog.h"
#include "ym_verilog/pt/PtP.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PortNode PtNodeImpl.h
/// @brief ポートを表すノード
//////////////////////////////////////////////////////////////////////
class PortNode :
  public PtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] port ポートを表すパース木
  PortNode(const PtPort* port);

  /// @brief デストラクタ
  virtual
  ~PortNode();


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
  const PtPort* mPort;
  
};


//////////////////////////////////////////////////////////////////////
/// @class PortRefNode PtNodeImpl.h
/// @brief ポート参照式を表すノード
//////////////////////////////////////////////////////////////////////
class PortRefNode :
  public PtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] portref ポート参照式を表すパース木
  PortRefNode(const PtPortRef* portref);

  /// @brief デストラクタ
  virtual
  ~PortRefNode();


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

  // 対応するポート参照式
  const PtPortRef* mPortRef;
  
};

END_NAMESPACE_YM_VERILOG

#endif // PTNODE_PORT_H
