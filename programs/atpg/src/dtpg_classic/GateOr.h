#ifndef DTPG_CLASSIC_GATETOR_H
#define DTPG_CLASSIC_GATETOR_H

/// @file src/dtpg_classic/GateOr.h
/// @brief GateOr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: GateOr.h 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "Gate.h"


BEGIN_NAMESPACE_YM_ATPG_DTPGCL

//////////////////////////////////////////////////////////////////////
/// @class GateOr GateOr.h "GateOr.h"
/// @brief OR ゲートを表すクラス
//////////////////////////////////////////////////////////////////////
class GateOr :
  public Gate
{
public:
  
  /// @brief コンストラクタ
  /// @param[in] id ゲートID
  /// @param[in] node 元のノード
  /// @param[in] ni ファンイン数
  GateOr(ymuint id,
	 const TgNode* node,
	 ymuint ni);
  
  /// @brief デストラクタ
  virtual
  ~GateOr();


public:
  
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
  
  /// @brief 正常値/故障値の後方含意を行う．
  bool
  check_last_h();
  
  /// @brief 正常値の後方含意を行う．
  bool
  check_last_g();
  
  /// @brief 故障値の後方含意を行う．
  bool
  check_last_f();
    
  virtual
  void
  calc_C();
  
  virtual
  ymuint
  calc_O(ymuint pos);

  virtual
  Val3
  c_val() const;

  virtual
  Val3
  o_val() const;
  
  
protected:
  
  void
  calc_C_sub(ymuint& sum_0,
	     ymuint& min_1);
  
};


//////////////////////////////////////////////////////////////////////
/// @class GateNor GateOr.h "GateOr.h"
/// @brief NOR ゲートを表すクラス
//////////////////////////////////////////////////////////////////////
class GateNor :
  public GateOr
{
public:
  
  /// @brief コンストラクタ
  /// @param[in] id ゲートID
  /// @param[in] node 元のノード
  /// @param[in] ni ファンイン数
  GateNor(ymuint id,
	  const TgNode* node,
	  ymuint ni);
  
  /// @brief デストラクタ
  virtual
  ~GateNor();


public:
  
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
    
  virtual
  void
  calc_C();
  
  virtual
  ymuint
  calc_O(ymuint pos);

  virtual
  Val3
  o_val() const;
  
};

END_NAMESPACE_YM_ATPG_DTPGCL

#endif
