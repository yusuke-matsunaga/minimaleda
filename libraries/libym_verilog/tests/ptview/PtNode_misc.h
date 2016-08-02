#ifndef PTNODE_MISC_H
#define PTNODE_MISC_H

/// @file libym_verilog/tests/vlview/PtNode_misc.h
/// @brief PtNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: PtNode_misc.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include "PtNode.h"
#include <ym_verilog/verilog.h>
#include <ym_verilog/pt/PtP.h>
#include <ym_verilog/pt/PtArray.h>


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ControlNode PtNode_misc.h
/// @brief Control を表すノード
//////////////////////////////////////////////////////////////////////
class ControlNode :
  public PtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] control コントロール
  ControlNode(const PtControl* control);

  /// @brief デストラクタ
  ~ControlNode();


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

  // コントロールを表すパース木
  const PtControl* mControl;
  
};


//////////////////////////////////////////////////////////////////////
/// @class ConnectionListNode PtNode_misc.h
/// @brief Connection のリストを表すノード
//////////////////////////////////////////////////////////////////////
class ConnectionListNode :
  public PtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] con_array 接続のリスト
  ConnectionListNode(const PtConnectionArray& con_array);

  /// @brief デストラクタ
  ~ConnectionListNode();


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

  // 接続のリスト
  PtConnectionArray mConArray;
  
};


//////////////////////////////////////////////////////////////////////
/// @class ConnectionNode PtNode_misc.h
/// @brief Connection を表すノード
//////////////////////////////////////////////////////////////////////
class ConnectionNode :
  public PtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] con 接続
  ConnectionNode(const PtConnection* con);

  /// @brief デストラクタ
  ~ConnectionNode();


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

  // 接続を表すパース木
  const PtConnection* mCon;
  
};


//////////////////////////////////////////////////////////////////////
/// @class StrengthNode PtNode_misc.h
/// @brief strength 情報を表すノード
//////////////////////////////////////////////////////////////////////
class StrengthNode :
  public PtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] strength strength 情報
  StrengthNode(const PtStrength* strength);

  /// @brief デストラクタ
  ~StrengthNode();


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

  // strength 情報
  const PtStrength* mStrength;
  
};


//////////////////////////////////////////////////////////////////////
/// @class DelayNode PtNode_misc.h
/// @brief 遅延情報を表すノード
//////////////////////////////////////////////////////////////////////
class DelayNode :
  public PtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] delay 遅延情報
  DelayNode(const PtDelay* delay);

  /// @brief デストラクタ
  ~DelayNode();


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

  // 遅延情報
  const PtDelay* mDelay;
  
};


//////////////////////////////////////////////////////////////////////
/// @class NameBranchListNode PtNode_misc.h
/// @brief NameBranch のリストを表すノード
//////////////////////////////////////////////////////////////////////
class NameBranchListNode :
  public PtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] name_branch_list 階層名のリスト
  NameBranchListNode(const PtNameBranchArray& name_branch_list);

  /// @brief デストラクタ
  ~NameBranchListNode();


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

  // 階層名の配列
  PtNameBranchArray mNameBranchArray;
  
};


//////////////////////////////////////////////////////////////////////
/// @class NameBranchNode PtNode_misc.h
/// @brief NameBranch を表すノード
//////////////////////////////////////////////////////////////////////
class NameBranchNode :
  public PtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] name_branch 1階層分のデータ
  NameBranchNode(const PtNameBranch* name_branch);

  /// @brief デストラクタ
  ~NameBranchNode();


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

  // 階層名のデータ
  const PtNameBranch* mNameBranch;
  
};


//////////////////////////////////////////////////////////////////////
/// @class StrNode PtNode_misc.h
/// @brief 文字列を表すノード
//////////////////////////////////////////////////////////////////////
class StrNode :
  public PtScalarNode
{
  
public:
  
  /// @brief コンストラクタ
  /// @param[in] label ラベル
  /// @param[in] str 文字列
  StrNode(const QString& label,
	  const QString& str);

  /// @brief デストラクタ
  virtual
  ~StrNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // ラベル
  QString mLabel;
  
  // 文字列
  QString mStr;
  
};


//////////////////////////////////////////////////////////////////////
/// @class AuxTypeNode PtNode_misc.h
/// @brief 補助的なデータ型を表すノード
//////////////////////////////////////////////////////////////////////
class AuxTypeNode :
  public PtScalarNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] aux_type 補助的なデータ型
  /// @param[in] net_type ネットの型
  /// @param[in] var_type 変数の型
  AuxTypeNode(tVpiAuxType aux_type,
	      tVpiNetType net_type,
	      tVpiVarType var_type);

  /// @brief デストラクタ
  virtual
  ~AuxTypeNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 補助的なデータ型
  tVpiAuxType mAuxType;

  // ネットの型
  tVpiNetType mNetType;

  // 変数の型
  tVpiVarType mVarType;
  
};


//////////////////////////////////////////////////////////////////////
/// @class NetTypeNode PtNode_misc.h
/// @brief ネットの型を表すノード
//////////////////////////////////////////////////////////////////////
class NetTypeNode :
  public PtScalarNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] label ラベル
  /// @param[in] net_type ネットの型
  NetTypeNode(const QString& label,
	      tVpiNetType net_type);

  /// @brief デストラクタ
  virtual
  ~NetTypeNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // ラベル
  QString mLabel;
  
  // ネットの型
  tVpiNetType mNetType;
  
};


//////////////////////////////////////////////////////////////////////
/// @class VarTypeNode PtNode_misc.h
/// @brief 変数の型を表すノード
//////////////////////////////////////////////////////////////////////
class VarTypeNode :
  public PtScalarNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] var_type 変数の型
  VarTypeNode(tVpiVarType var_type);

  /// @brief デストラクタ
  virtual
  ~VarTypeNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数の型
  tVpiVarType mVarType;
  
};


//////////////////////////////////////////////////////////////////////
/// @class DirNode PtNode_misc.h
/// @brief 入出力の方向を表すノード
//////////////////////////////////////////////////////////////////////
class DirNode :
  public PtScalarNode
{
  
public:
  
  /// @brief コンストラクタ
  /// @param[in] dir 入出力の方向
  DirNode(tVpiDirection dir);

  /// @brief デストラクタ
  virtual
  ~DirNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入出力の方向
  tVpiDirection mDir;
  
};


//////////////////////////////////////////////////////////////////////
/// @class UdNode PtNode_misc.h
/// @brief unconnected drive の型を表すノード
//////////////////////////////////////////////////////////////////////
class UdNode :
  public PtScalarNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] ud unconnected drive の型
  UdNode(tVpiUnconnDrive ud);

  /// @brief デストラクタ
  virtual
  ~UdNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // unconnected drive の型
  tVpiUnconnDrive mUd;
  
};


//////////////////////////////////////////////////////////////////////
/// @class DelayModeNode PtNode_misc.h
/// @brief delay mode の型を表すノード
//////////////////////////////////////////////////////////////////////
class DelayModeNode :
  public PtScalarNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] delay_mode delay mode の型
  DelayModeNode(tVpiDefDelayMode delay_mode);

  /// @brief デストラクタ
  virtual
  ~DelayModeNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // delay mode の型
  tVpiDefDelayMode mDelayMode;
  
};


//////////////////////////////////////////////////////////////////////
/// @class PrimTypeNode PtNode_misc.h
/// @brief Primitive の型を表すノード
//////////////////////////////////////////////////////////////////////
class PrimTypeNode :
  public PtScalarNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] prim_type primitive の型
  PrimTypeNode(tVpiPrimType prim_type);

  /// @brief デストラクタ
  virtual
  ~PrimTypeNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // primitive の型
  tVpiPrimType mPrimType;
  
};


//////////////////////////////////////////////////////////////////////
/// @class OpTypeNode PtNode_misc.h
/// @brief 演算子の型を表すノード
//////////////////////////////////////////////////////////////////////
class OpTypeNode :
  public PtScalarNode
{
  
public:
  
  /// @brief コンストラクタ
  /// @param[in] op_type 演算子の型
  OpTypeNode(tVpiOpType op_type);

  /// @brief デストラクタ
  virtual
  ~OpTypeNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 演算子の型
  tVpiOpType mOpType;
  
};


//////////////////////////////////////////////////////////////////////
/// @class ConstTypeNode PtNode_misc.h
/// @brief 定数の型を表すノード
//////////////////////////////////////////////////////////////////////
class ConstTypeNode :
  public PtScalarNode
{
  
public:
  
  /// @brief コンストラクタ
  /// @param[in] const_type 定数の型
  ConstTypeNode(tVpiConstType const_type);

  /// @brief デストラクタ
  virtual
  ~ConstTypeNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 定数の型
  tVpiConstType mConstType;
  
};


//////////////////////////////////////////////////////////////////////
/// @class RangeModeNode PtNode_misc.h
/// @brief 範囲指定の型を表すノード
//////////////////////////////////////////////////////////////////////
class RangeModeNode :
  public PtScalarNode
{
  
public:
  
  /// @brief コンストラクタ
  /// @param[in] range_mode 範囲指定の型
  RangeModeNode(tVpiRangeMode range_mode);

  /// @brief デストラクタ
  virtual
  ~RangeModeNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 範囲指定の型
  tVpiRangeMode mRangeMode;
  
};


//////////////////////////////////////////////////////////////////////
/// @class StrengthValNode PtNode_misc.h
/// @brief strength 値を表すノード
//////////////////////////////////////////////////////////////////////
class StrengthValNode :
  public PtScalarNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] label ラベル
  /// @param[in] strength strength
  StrengthValNode(const QString& label,
		  tVpiStrength strength);

  /// @brief デストラクタ
  ~StrengthValNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // ラベル
  QString mLabel;
  
  // strength 情報
  tVpiStrength mStrength;
  
};


//////////////////////////////////////////////////////////////////////
/// @class VsNode PtNode_misc.h
/// @brief vectored/scalared 情報を表すノード
//////////////////////////////////////////////////////////////////////
class VsNode :
  public PtScalarNode
{
  
public:
  
  /// @brief コンストラクタ
  /// @param[in] vs vectored/scalared 情報
  VsNode(tVpiVsType vs);

  /// @brief デストラクタ
  virtual
  ~VsNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // vectored/scalared 情報
  tVpiVsType mVs;
  
};


//////////////////////////////////////////////////////////////////////
/// @class BoolNode PtNode_misc.h
/// @brief 真理値を表すノード
//////////////////////////////////////////////////////////////////////
class BoolNode :
  public PtScalarNode
{
  
public:
  
  /// @brief コンストラクタ
  /// @param[in] label ラベル
  /// @param[in] val 真理値
  BoolNode(const QString& label,
	   bool val);

  /// @brief デストラクタ
  virtual
  ~BoolNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // ラベル
  QString mLabel;
  
  // 真理値
  bool mVal;
  
};

END_NAMESPACE_YM_VERILOG

#endif // PTNODE_MISC_H
