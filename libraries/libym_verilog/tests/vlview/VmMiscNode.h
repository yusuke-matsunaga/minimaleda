#ifndef VMMISCNODE_H
#define VMMISCNODE_H

/// @file libym_verilog/tests/vlview/VmMiscNode.h
/// @brief VmMiscNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VmMiscNode.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "VmNode.h"
#include "ym_verilog/verilog.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VmDelayNode VmMiscNode.h
/// @brief 遅延情報を表すノード
//////////////////////////////////////////////////////////////////////
class VmDelayNode :
  public VmNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] delay 遅延情報
  VmDelayNode(const VlDelay* delay);

  /// @brief デストラクタ
  ~VmDelayNode();


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
  const VlDelay* mDelay;
  
};


//////////////////////////////////////////////////////////////////////
/// @class VmScalarNode VmMiscNode.h
/// @brief スカラー値を表すノード
//////////////////////////////////////////////////////////////////////
class VmScalarNode :
  public VmNode
{
public:

  /// @brief コンストラクタ
  VmScalarNode();

  /// @brief デストラクタ
  virtual
  ~VmScalarNode();


public:
    
  /// @brief 対象のファイル上での位置を返す．
  /// @note このクラスでは空の FileRegion を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  /// @note このクラスではなにもしない．
  virtual
  void
  expand() const;
  
};


//////////////////////////////////////////////////////////////////////
/// @class VmStrNode VmMiscNode.h
/// @brief 文字列を表すノード
//////////////////////////////////////////////////////////////////////
class VmStrNode :
  public VmScalarNode
{
  
public:
  
  /// @brief コンストラクタ
  /// @param[in] label ラベル
  /// @param[in] str 文字列
  VmStrNode(const QString& label,
	    const QString& str);

  /// @brief デストラクタ
  virtual
  ~VmStrNode();


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
/// @class VmAuxTypeNode VmMiscNode.h
/// @brief 補助的なデータ型を表すノード
//////////////////////////////////////////////////////////////////////
class VmAuxTypeNode :
  public VmScalarNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] aux_type 補助的なデータ型
  /// @param[in] net_type ネットの型
  /// @param[in] var_type 変数の型
  VmAuxTypeNode(tVpiAuxType aux_type,
		tVpiNetType net_type,
		tVpiVarType var_type);

  /// @brief デストラクタ
  virtual
  ~VmAuxTypeNode();


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
/// @class VmNetTypeNode VmMiscNode.h
/// @brief ネットの型を表すノード
//////////////////////////////////////////////////////////////////////
class VmNetTypeNode :
  public VmScalarNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] label ラベル
  /// @param[in] net_type ネットの型
  VmNetTypeNode(const QString& label,
		tVpiNetType net_type);
  
  /// @brief デストラクタ
  virtual
  ~VmNetTypeNode();


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
/// @class VmVarTypeNode VmMiscNode.h
/// @brief 変数の型を表すノード
//////////////////////////////////////////////////////////////////////
class VmVarTypeNode :
  public VmScalarNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] var_type 変数の型
  VmVarTypeNode(tVpiVarType var_type);

  /// @brief デストラクタ
  virtual
  ~VmVarTypeNode();


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
/// @class VmDirNode VmMiscNode.h
/// @brief 入出力の方向を表すノード
//////////////////////////////////////////////////////////////////////
class VmDirNode :
  public VmScalarNode
{
  
public:
  
  /// @brief コンストラクタ
  /// @param[in] dir 入出力の方向
  VmDirNode(tVpiDirection dir);

  /// @brief デストラクタ
  virtual
  ~VmDirNode();


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
/// @class VmUdNode VmMiscNode.h
/// @brief unconnected drive の型を表すノード
//////////////////////////////////////////////////////////////////////
class VmUdNode :
  public VmScalarNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] ud unconnected drive の型
  VmUdNode(tVpiUnconnDrive ud);

  /// @brief デストラクタ
  virtual
  ~VmUdNode();


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
/// @class VmDelayModeNode VmMiscNode.h
/// @brief delay mode の型を表すノード
//////////////////////////////////////////////////////////////////////
class VmDelayModeNode :
  public VmScalarNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] delay_mode delay mode の型
  VmDelayModeNode(tVpiDefDelayMode delay_mode);

  /// @brief デストラクタ
  virtual
  ~VmDelayModeNode();


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
/// @class VmPrimTypeNode VmMiscNode.h
/// @brief Primitive の型を表すノード
//////////////////////////////////////////////////////////////////////
class VmPrimTypeNode :
  public VmScalarNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] prim_type primitive の型
  VmPrimTypeNode(tVpiPrimType prim_type);

  /// @brief デストラクタ
  virtual
  ~VmPrimTypeNode();


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
/// @class VmRangeModeNode VmMiscNode.h
/// @brief 範囲指定の型を表すノード
//////////////////////////////////////////////////////////////////////
class VmRangeModeNode :
  public VmScalarNode
{
  
public:
  
  /// @brief コンストラクタ
  /// @param[in] range_mode 範囲指定の型
  VmRangeModeNode(tVpiRangeMode range_mode);

  /// @brief デストラクタ
  virtual
  ~VmRangeModeNode();


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
/// @class VmStrengthValNode VmMiscNode.h
/// @brief strength 値を表すノード
//////////////////////////////////////////////////////////////////////
class VmStrengthValNode :
  public VmScalarNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] label ラベル
  /// @param[in] strength strength
  VmStrengthValNode(const QString& label,
		    tVpiStrength strength);

  /// @brief デストラクタ
  ~VmStrengthValNode();


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
/// @class VmVsNode VmMiscNode.h
/// @brief vectored/scalared 情報を表すノード
//////////////////////////////////////////////////////////////////////
class VmVsNode :
  public VmScalarNode
{
  
public:
  
  /// @brief コンストラクタ
  /// @param[in] vs vectored/scalared 情報
  VmVsNode(tVpiVsType vs);

  /// @brief デストラクタ
  virtual
  ~VmVsNode();


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
/// @class VmBoolNode VmMiscNode.h
/// @brief 真理値を表すノード
//////////////////////////////////////////////////////////////////////
class VmBoolNode :
  public VmScalarNode
{
  
public:
  
  /// @brief コンストラクタ
  /// @param[in] label ラベル
  /// @param[in] val 真理値
  VmBoolNode(const QString& label,
	     bool val);

  /// @brief デストラクタ
  virtual
  ~VmBoolNode();


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


//////////////////////////////////////////////////////////////////////
/// @class VmIntNode VmMiscNode.h
/// @brief 整数値を表すノード
//////////////////////////////////////////////////////////////////////
class VmIntNode :
  public VmScalarNode
{
  
public:
  
  /// @brief コンストラクタ
  /// @param[in] label ラベル
  /// @param[in] val 値
  VmIntNode(const QString& label,
	    int val);

  /// @brief デストラクタ
  virtual
  ~VmIntNode();


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
  
  // 値
  int mVal;
  
};

END_NAMESPACE_YM_VERILOG

#endif // VMMISCNODE_H
