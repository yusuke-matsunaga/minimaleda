#ifndef VMUDPNODE_H
#define VMUDPNODE_H

/// @file libym_verilog/tests/vlview/VmUdpNode.h
/// @brief VmUdpNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlPtNode_udp.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "VmNode.h"
#include "ym_verilog/verilog.h"
#include "ym_verilog/vl/VlFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VmUdpListNode VmRootNode.h
/// @brief UDP のリストを表すノード
//////////////////////////////////////////////////////////////////////
class VmUdpListNode :
  public VmNode1
{
public:

  /// @brief コンストラクタ
  /// @param[in] vl_mgr VlMgr
  VmUdpListNode(const VlMgr& vl_mgr);

  /// @brief デストラクタ
  virtual
  ~VmUdpListNode();


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
  
};


//////////////////////////////////////////////////////////////////////
/// @class VmUdpNode VmUdpNode.h
/// @brief UDP を表すノード
//////////////////////////////////////////////////////////////////////
class VmUdpNode :
  public VmNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] udp UDP を表すオブジェクト
  VmUdpNode(const VlUdpDefn* udp);

  /// @brief デストラクタ
  virtual
  ~VmUdpNode();


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
  
  // 対応する UDP
  const VlUdpDefn* mUdp;
  
};


//////////////////////////////////////////////////////////////////////
/// @class VmUdpEntryListNode VmUdpNode.h
/// @brief UDP のテーブルリストを表すノード
//////////////////////////////////////////////////////////////////////
class VmUdpEntryListNode :
  public VmNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] udp UDP を表すオブジェクト
  VmUdpEntryListNode(const VlUdpDefn* udp);

  /// @brief デストラクタ
  virtual
  ~VmUdpEntryListNode();


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
  
  // 親のUDP
  const VlUdpDefn* mUdp;
  
};


//////////////////////////////////////////////////////////////////////
/// @class VmUdpEntryNode VmUdpNode.h
/// @brief UDP のテーブルを表すノード
//////////////////////////////////////////////////////////////////////
class VmUdpEntryNode :
  public VmNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] table テーブルの1行分
  VmUdpEntryNode(const VlTableEntry* table);

  /// @brief デストラクタ
  virtual
  ~VmUdpEntryNode();


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
  
  // 対応するテーブル
  const VlTableEntry* mTable;
  
};

END_NAMESPACE_YM_VERILOG

#endif // VMUDPNODE_H
