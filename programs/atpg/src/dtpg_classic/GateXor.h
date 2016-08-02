#ifndef DTPG_CLASSIC_GATEXOR_H
#define DTPG_CLASSIC_GATEXOR_H

/// @file src/dtpg_classic/GateXor.h
/// @brief GateXor の派生クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: GateXor.h 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "Gate.h"


BEGIN_NAMESPACE_YM_ATPG_DTPGCL

//////////////////////////////////////////////////////////////////////
/// @class GateXor GateXor.h "GateXor.h"
/// @brief XOR ゲートを表すクラス
//////////////////////////////////////////////////////////////////////
class GateXor :
  public Gate
{
public:
  
  /// @brief コンストラクタ
  /// @param[in] id ゲートID
  /// @param[in] node 元のノード
  /// @param[in] ni ファンイン数
  GateXor(ymuint id,
	  const TgNode* node,
	  ymuint ni);
  
  /// @brief デストラクタ
  virtual
  ~GateXor();
  
  
public:
  
  virtual
  void
  calc_C();
  
  virtual
  ymuint
  calc_O(ymuint pos);
  
  virtual
  bool
  calc_obs(ymuint ipos) const;
  
  virtual
  tTgGateType
  gate_type() const;
  
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
  fwd_imp0_f();
  
  virtual
  bool
  fwd_imp1_f();

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
/// @class XGateNor GateXor.h "GateXor.h"
/// @brief XNOR ゲートを表すクラス
//////////////////////////////////////////////////////////////////////
class XGateNor :
  public GateXor
{
public:
  
  /// @brief コンストラクタ
  /// @param[in] id ゲートID
  /// @param[in] node 元のノード
  /// @param[in] ni ファンイン数
  XGateNor(ymuint id,
	   const TgNode* node,
	   ymuint ni);

  /// @brief デストラクタ
  virtual
  ~XGateNor();


public:
  
  virtual
  tTgGateType
  gate_type() const;
  
};

END_NAMESPACE_YM_ATPG_DTPGCL

#endif
