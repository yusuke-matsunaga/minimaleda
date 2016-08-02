#ifndef DTPG_CLASSIC_GATESA_H
#define DTPG_CLASSIC_GATESA_H

/// @file src/dtpg_classic/GateSa.h
/// @brief GateSa のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: GateSa.h 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "Gate.h"


BEGIN_NAMESPACE_YM_ATPG_DTPGCL

//////////////////////////////////////////////////////////////////////
/// @class GateSa GateSa.h "GateSa.h"
/// @brief 縮退故障を表すクラス
//////////////////////////////////////////////////////////////////////
class GateSa :
  public Gate
{
public:

  /// @brief コンストラクタ
  GateSa();

  /// @brief デストラクタ
  virtual
  ~GateSa();


public:
  
  virtual
  bool
  is_f_site() const;
  
  virtual
  void
  calc_C();
  
  virtual
  ymuint
  calc_O(ymuint pos);

  /// @brief 現在の値割り当てでの故障伝搬性を計算する．
  virtual
  bool
  calc_obs(ymuint ipos) const;
  
  virtual
  bool
  fwd_imp0_h();
  
  virtual
  bool
  fwd_imp1_h();
  
  virtual
  bool
  fwd_imp0_g();
  
  virtual
  bool
  fwd_imp1_g();

  virtual
  bool
  bwd_imp0_h();

  virtual
  bool
  bwd_imp1_h();
  
  virtual
  bool
  bwd_imp0_g();
  
  virtual
  bool
  bwd_imp1_g();
  
  virtual
  bool
  bwd_imp0_f();
  
  virtual
  bool
  bwd_imp1_f();

};


//////////////////////////////////////////////////////////////////////
/// @class GateSa0 GateSa.h "GateSa.h"
/// @brief 1縮退故障を表すクラス
//////////////////////////////////////////////////////////////////////
class GateSa0 :
  public GateSa
{
public:
  
  virtual
  const char*
  name() const;
  
  virtual
  tTgGateType
  gate_type() const;
  
  /// @brief 名前とゲート型を表す文字列を返す．
  virtual
  string
  description() const;
  
  virtual
  bool
  fwd_imp0_f();
  
  virtual
  bool
  fwd_imp1_f();

};


//////////////////////////////////////////////////////////////////////
/// @class GateSa0 GateSa.h "GateSa.h"
/// @brief 1縮退故障を表すクラス
//////////////////////////////////////////////////////////////////////
class GateSa1 :
  public GateSa
{
public:

  virtual
  const char*
  name() const;
  
  virtual
  tTgGateType
  gate_type() const;
  
  /// @brief 名前とゲート型を表す文字列を返す．
  virtual
  string
  description() const;
  
  virtual
  bool
  fwd_imp0_f();
  
  virtual
  bool
  fwd_imp1_f();
  
};

END_NAMESPACE_YM_ATPG_DTPGCL

#endif
