#ifndef LIBYM_VERILOG_ELB_ELBMODULE_H
#define LIBYM_VERILOG_ELB_ELBMODULE_H

/// @file libym_verilog/elaborator/include/ElbModule.h
/// @brief ElbModule のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ElbModule.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/vl/VlModule.h"
#include "ym_verilog/pt/PtP.h"
#include "ElbFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbModuleArray ElbModule.h "ElbModule.h"
/// @brief elaboration 中の module 配列を表すクラス
//////////////////////////////////////////////////////////////////////
class ElbModuleArray :
  public VlModuleArray
{
  friend class CellModuleArray;
  
protected:
  
  /// @brief コンストラクタ
  ElbModuleArray();
  
  /// @brief デストラクタ
  virtual
  ~ElbModuleArray();


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数 (ElbMgr が使う)
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 要素を返す．
  virtual
  ElbModule*
  _module(ymuint32 offset) = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbModuleArray の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素を返す．
  const ElbModuleArray*
  next() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 次の要素を指すポインタ
  ElbModuleArray* mNext;
  
};


//////////////////////////////////////////////////////////////////////
/// @class ElbModule ElbModule.h "ElbModule.h"
/// @brief elaboration 中の module を表すクラス
//////////////////////////////////////////////////////////////////////
class ElbModule :
  public VlModule
{
  friend class CellModule;
  
protected:
  
  /// @brief コンストラクタ
  ElbModule();
  
  /// @brief デストラクタ
  virtual
  ~ElbModule();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbModule の仮想関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 入出力の初期設定を行う．
  /// @param[in] pos 位置番号
  /// @param[in] head ヘッダ
  /// @param[in] pt_item パース木のIO宣言要素
  /// @param[in] decl 対応する宣言要素
  virtual
  void
  init_iodecl(ymuint32 pos,
	      ElbIOHead* head,
	      const PtIOItem* pt_item,
	      ElbDecl* decl) = 0;
  
  /// @brief ポートの初期設定を行う．
  /// @param[in] index ポート番号
  /// @param[in] pt_port パース木のポート定義
  /// @param[in] low_conn 下位の接続
  /// @param[in] dir 向き
  virtual
  void
  init_port(ymuint32 index,
	    const PtPort* pt_port,
	    ElbExpr* low_conn,
	    tVpiDirection dir) = 0;

  /// @brief ポートの high_conn を接続する．
  /// @param[in] index ポート番号
  /// @param[in] high_conn 上位の接続の式
  /// @param[in] conn_by_name 名前による割り当て時に true とするフラグ
  virtual
  void
  set_port_high_conn(ymuint32 index,
		     ElbExpr* high_conn,
		     bool conn_by_name) = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbModule の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素を得る．
  const ElbModule*
  next() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 次の要素を指すポインタ
  ElbModule* mNext;
  
};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
ElbModuleArray::ElbModuleArray() :
  mNext(NULL)
{
}

// @brief デストラクタ
inline
ElbModuleArray::~ElbModuleArray()
{
}

// @brief 次の要素を返す．
inline
const ElbModuleArray*
ElbModuleArray::next() const
{
  return mNext;
}

// @brief コンストラクタ
inline
ElbModule::ElbModule() :
  mNext(NULL)
{
}

// @brief デストラクタ
inline
ElbModule::~ElbModule()
{
}

// 次の要素を得る．
inline
const ElbModule*
ElbModule::next() const
{
  return mNext;
}

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELB_ELBMODULE_H
