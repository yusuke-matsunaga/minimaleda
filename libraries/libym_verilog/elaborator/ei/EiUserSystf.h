#ifndef LIBYM_VERILOG_ELABORATOR_EI_EIUSERSYSTF_H
#define LIBYM_VERILOG_ELABORATOR_EI_EIUSERSYSTF_H

/// @file libym_verilog/elaborator/ei/EiUserSystf.h
/// @brief EiUserSystf のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiUserSystf.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ElbUserSystf.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiUserSystf EiUserSystf.h "EiUserSystf.h"
/// @brief システムタスク/関数の定義を表すクラス
/// IEEE Std 1364-2001 26.6.14 UDP
//////////////////////////////////////////////////////////////////////
class EiUserSystf :
  public ElbUserSystf
{
public:

  /// @brief コンストラクタ
  EiUserSystf();

  /// @brief デストラクタ
  virtual
  ~EiUserSystf();


public:
  //////////////////////////////////////////////////////////////////////
  // VlUserSystf に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  virtual
  string
  name() const;

  /// @brief system task の時 true を返す．
  virtual
  bool
  system_task() const;

  /// @brief system function の時 true を返す．
  virtual
  bool
  system_function() const;

  /// @brief system function の型を返す．
  virtual
  tVpiFuncType
  function_type() const;

  /// @brief compile 時のコールバック関数
  virtual
  ymuint32
  on_compile();

  /// @brief 実行時のコールバック関数
  virtual
  ymuint32
  on_call();

  /// @brief SizedFunc の場合にサイズを返す．
  virtual
  ymuint32
  size() const;

};


//////////////////////////////////////////////////////////////////////
/// @class EiUserSystf1 EiUserSystf.h "EiUserSystf.h"
/// @brief システムタスク/関数の定義を表すクラス
/// IEEE Std 1364-2001 26.6.14 UDP
//////////////////////////////////////////////////////////////////////
class EiUserSystf1 :
  public ElbUserSystf
{
public:

  /// @brief コンストラクタ
  /// @param[in]
  EiUserSystf1();

  /// @brief デストラクタ
  virtual
  ~EiUserSystf1();


public:
  //////////////////////////////////////////////////////////////////////
  // VlUserSystf に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  virtual
  string
  name() const;

  /// @brief system task の時 true を返す．
  virtual
  bool
  system_task() const;

  /// @brief system function の時 true を返す．
  virtual
  bool
  system_function() const;

  /// @brief system function の型を返す．
  virtual
  tVpiFuncType
  function_type() const;

  /// @brief compile 時のコールバック関数
  virtual
  ymuint32
  on_compile();

  /// @brief 実行時のコールバック関数
  virtual
  ymuint32
  on_call();

  /// @brief SizedFunc の場合にサイズを返す．
  virtual
  ymuint32
  size() const;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELABORATOR_EI_EIUSERSYSTF_H
