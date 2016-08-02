#ifndef LIBYM_VERILOG_ELABORATOR_EI_EIGENVAR_H
#define LIBYM_VERILOG_ELABORATOR_EI_EIGENVAR_H

/// @file libym_verilog/elaborator/ei/EiGenvar.h
/// @brief EiGenvar のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiGenvar.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ElbGenvar.h"
#include <ym_verilog/pt/PtP.h>


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiGenvar EiGenvar.h "EiGenvar.h"
/// @brief genvar を表すクラス
//////////////////////////////////////////////////////////////////////
class EiGenvar :
  public ElbGenvar
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_item パース木の定義要素
  /// @param[in] val 初期値
  EiGenvar(const VlNamedObj* parent,
	   const PtDeclItem* pt_item,
	   int val);

  /// @brief デストラクタ
  virtual
  ~EiGenvar();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;

  /// @brief ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;


public:
  //////////////////////////////////////////////////////////////////////
  // VlNamedObj の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief このオブジェクトの属しているスコープを返す．
  virtual
  const VlNamedObj*
  parent() const;

  /// @brief 名前の取得
  virtual
  const char*
  name() const;


public:
  //////////////////////////////////////////////////////////////////////
  // EiGenvar に固有の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 使用中の時 true を返す．
  bool
  is_inuse() const;

  /// @brief 使用中にする．
  void
  set_inuse();

  /// @brief 使用を終える．
  void
  reset_inuse();

  /// @brief 現在の値を返す．
  int
  value() const;

  /// @brief 値を設定する．
  void
  set_value(int value);

  /// @brief 元となったパース木の定義要素を返す．
  const PtDeclItem*
  pt_item() const;

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のスコープ
  const VlNamedObj* mParent;

  // パース木の定義要素
  const PtDeclItem* mPtItem;

  // 使用中の時に true にする flag
  bool mInUse;

  // 値
  int mValue;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELABORATOR_EI_EIGENVAR_H
