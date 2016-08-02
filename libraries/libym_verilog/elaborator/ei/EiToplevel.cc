
/// @file libym_verilog/elaborator/EiToplevel.cc
/// @brief EiToplevel の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiToplevel.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiToplevel.h"

#include "ym_utils/FileRegion.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief トップレベルのスコープを生成する
// @param[in] alloc アロケータ
const VlNamedObj*
EiFactory::new_Toplevel()
{
  void* p = mAlloc.get_memory(sizeof(EiToplevel));
  EiToplevel* toplevel = new (p) EiToplevel();

  return toplevel;
}


//////////////////////////////////////////////////////////////////////
// elaboration 中の toplevel scope を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
EiToplevel::EiToplevel()
{
}

// デストラクタ
EiToplevel::~EiToplevel()
{
}

// @brief 型の取得
tVpiObjType
EiToplevel::type() const
{
  return kVpiScope;
}

// @brief ファイル位置の取得
FileRegion
EiToplevel::file_region() const
{
  return FileRegion();
}

// @brief このオブジェクトの属しているスコープを返す．
const VlNamedObj*
EiToplevel::parent() const
{
  return NULL;
}

/// @brief このオブジェクトの属している module を返す．
const VlModule*
EiToplevel::parent_module() const
{
  return NULL;
}
  
// @brief 名前の取得
const char*
EiToplevel::name() const
{
  return "";
}

END_NAMESPACE_YM_VERILOG
