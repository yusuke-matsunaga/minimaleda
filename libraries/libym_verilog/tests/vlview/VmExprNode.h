#ifndef VMEXPRNODE_H
#define VMEXPRNODE_H

/// @file libym_verilog/tests/vlview/VmExprNode.h
/// @brief VlPtNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VmExprNode.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "VmNode.h"
#include "ym_verilog/verilog.h"
#include "ym_verilog/vl/VlFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VmExprListNode VmExprNode.h
/// @brief 式のリストを表すノード
//////////////////////////////////////////////////////////////////////
class VmExprListNode :
  public VmNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] label ラベル
  /// @param[in] expr_array 式の配列
  VmExprListNode(const QString& label,
		 const vector<const VlExpr*>& expr_array);

  /// @brief デストラクタ
  virtual
  ~VmExprListNode();


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
  
  // 式の配列
  vector<const VlExpr*> mExprArray;
  
};


//////////////////////////////////////////////////////////////////////
/// @class VmExprNode VmExprNode.h
/// @brief 式を表すノード
//////////////////////////////////////////////////////////////////////
class VmExprNode :
  public VmNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] label ラベル
  /// @param[in] expr 式
  VmExprNode(const QString& label,
	     const VlExpr* expr);

  /// @brief デストラクタ
  virtual
  ~VmExprNode();


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

  // 式
  const VlExpr* mExpr;
  
};

END_NAMESPACE_YM_VERILOG

#endif // VLPTNODE_EXPR_H
