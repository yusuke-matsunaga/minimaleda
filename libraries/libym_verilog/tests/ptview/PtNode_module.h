#ifndef PTNODE_MODULE_H
#define PTNODE_MODULE_H

/// @file libym_verilog/tests/ptview/PtNode_module.h
/// @brief PtNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: PtNode_module.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include "PtNode.h"
#include "ym_verilog/verilog.h"
#include "ym_verilog/pt/PtP.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ModuleNode PtNodeImpl.h
/// @brief Module を表すノード
//////////////////////////////////////////////////////////////////////
class ModuleNode :
  public PtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] module Module を表すパース木
  ModuleNode(const PtModule* module);

  /// @brief デストラクタ
  virtual
  ~ModuleNode();


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

  // 対応する Module
  const PtModule* mModule;
  
};


//////////////////////////////////////////////////////////////////////
/// @class ModulePortListNode PtNodeImpl.h
/// @brief Module Port のリストを表すノード
//////////////////////////////////////////////////////////////////////
class ModulePortListNode :
  public PtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] module Module を表すパース木
  ModulePortListNode(const PtModule* module);

  /// @brief デストラクタ
  virtual
  ~ModulePortListNode();


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

  // 対応する Module
  const PtModule* mModule;
  
};

END_NAMESPACE_YM_VERILOG

#endif // PTNODE_MODLE_H
