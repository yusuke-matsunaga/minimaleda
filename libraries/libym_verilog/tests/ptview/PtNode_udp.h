#ifndef PTNODE_UDP_H
#define PTNODE_UDP_H

/// @file libym_verilog/tests/ptview/PtNode_udp.h
/// @brief PtNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: PtNode_udp.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include "PtNode.h"
#include "ym_verilog/verilog.h"
#include "ym_verilog/pt/PtP.h"
#include "ym_verilog/pt/PtArray.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class UdpNode PtNode_udp.h
/// @brief UDP を表すノード
//////////////////////////////////////////////////////////////////////
class UdpNode :
  public PtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] udp UDP を表すパース木
  UdpNode(const PtUdp* udp);

  /// @brief デストラクタ
  virtual
  ~UdpNode();


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
  const PtUdp* mUdp;
  
};


//////////////////////////////////////////////////////////////////////
/// @class UdpPortListNode PtNode_udp.h
/// @brief UDP のポートリストを表すノード
//////////////////////////////////////////////////////////////////////
class UdpPortListNode :
  public PtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] udp UDP を表すパース木
  UdpPortListNode(const PtUdp* udp);

  /// @brief デストラクタ
  virtual
  ~UdpPortListNode();


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
  const PtUdp* mUdp;
  
};


//////////////////////////////////////////////////////////////////////
/// @class UdpEntryListNode PtNode_udp.h
/// @brief UDP のテーブルリストを表すノード
//////////////////////////////////////////////////////////////////////
class UdpEntryListNode :
  public PtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] table_array テーブルの配列
  UdpEntryListNode(const PtUdpEntryArray& table_array);

  /// @brief デストラクタ
  virtual
  ~UdpEntryListNode();


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

  // 対応するテーブルの配列
  PtUdpEntryArray mTableArray;
  
};


//////////////////////////////////////////////////////////////////////
/// @class UdpEntryNode PtNode_udp.h
/// @brief UDP のテーブルを表すノード
//////////////////////////////////////////////////////////////////////
class UdpEntryNode :
  public PtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] table テーブルの1行分
  UdpEntryNode(const PtUdpEntry* table);

  /// @brief デストラクタ
  virtual
  ~UdpEntryNode();


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
  const PtUdpEntry* mTable;
  
};


//////////////////////////////////////////////////////////////////////
/// @class UdpValNode PtNode_udp.h
/// @brief UDP のテーブルの値を表すノード
//////////////////////////////////////////////////////////////////////
class UdpValNode :
  public PtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] label ラベル
  /// @param[in] val 値
  UdpValNode(const QString& label,
	     const PtUdpValue* val);

  /// @brief デストラクタ
  virtual
  ~UdpValNode();


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
  
  // ラベル
  QString mLabel;
  
  // 値
  const PtUdpValue* mVal;
  
};

END_NAMESPACE_YM_VERILOG

#endif // PTNODE_UDP_H
