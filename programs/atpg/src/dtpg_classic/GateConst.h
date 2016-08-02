#ifndef DTPG_CLASSIC_GATECONST_H
#define DTPG_CLASSIC_GATECONST_H

/// @file src/dtpg_classic/GateConst.h
/// @brief GateConst のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: GateConst.h 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "Gate.h"


BEGIN_NAMESPACE_YM_ATPG_DTPGCL

//////////////////////////////////////////////////////////////////////
/// @class GateConst GateConst.h "GateConst.h"
/// @brief 定数を表すクラス
//////////////////////////////////////////////////////////////////////
class GateConst :
  public Gate
{
public:

  /// @brief コンストラクタ
  /// @param[in] id ゲートID
  /// @param[in] node 元のノード
  GateConst(ymuint id,
	    const TgNode* node);

  /// デストラクタ
  virtual
  ~GateConst();


public:

  /// @brief 可観測性を計算する．
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
  fwd_imp0_f();
  
  virtual
  bool
  fwd_imp1_f();
  
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
/// @class GateConst0 GateConst.h "GateConst.h"
/// @brief 定数0を表すクラス
//////////////////////////////////////////////////////////////////////
class GateConst0 :
  public GateConst
{
public:

  /// @brief コンストラクタ
  /// @param[in] id ゲートID
  /// @param[in] node 元のノード
  GateConst0(ymuint id,
	     const TgNode* node);

  /// デストラクタ
  virtual
  ~GateConst0();


public:
  
  virtual
  void
  calc_C();
  
  virtual
  tTgGateType
  gate_type() const;

  virtual
  bool
  bwd_imp0_h();

  virtual
  bool
  bwd_imp1_h();
  
};


//////////////////////////////////////////////////////////////////////
/// @class GateConst1 GateConst.h "GateConst.h"
/// @brief 定数1を表すクラス
//////////////////////////////////////////////////////////////////////
class GateConst1 :
  public GateConst
{
public:

  /// @biref コンストラクタ
  /// @param[in] id ゲートID
  /// @param[in] node 元のノード
  GateConst1(ymuint id,
	     const TgNode* node);
  
  /// @brief デストラクタ
  virtual
  ~GateConst1();

  
public:
  
  virtual
  void
  calc_C();
  
  virtual
  tTgGateType
  gate_type() const;

  virtual
  bool
  bwd_imp0_h();

  virtual
  bool
  bwd_imp1_h();
  
};

END_NAMESPACE_YM_ATPG_DTPGCL

#endif
