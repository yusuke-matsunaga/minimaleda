#ifndef DTPG_CLASSIC_GATEBUF_H
#define DTPG_CLASSIC_GATEBUF_H

/// @file src/dtpg_classic/GateBuf.h
/// @brief GateBuf のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: GateBuf.h 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "Gate.h"


BEGIN_NAMESPACE_YM_ATPG_DTPGCL

//////////////////////////////////////////////////////////////////////
/// @class GateBuf GateBuf.h "GateBuf.h"
/// @brief BUF ゲートを表すクラス
//////////////////////////////////////////////////////////////////////
class GateBuf :
  public Gate
{
public:

  /// @brief コンストラクタ
  /// @param[in] id ゲートID
  /// @param[in] node 元のノード
  GateBuf(ymuint id,
	  const TgNode* node);
  
  /// @brief デストラクタ
  virtual
  ~GateBuf();


public:
  
  virtual
  void
  calc_C();
  
  virtual
  ymuint
  calc_O(ymuint pos);
  
  virtual
  tTgGateType
  gate_type() const;
  
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
/// @class GateNot GateBuf.h "GateBuf.h"
/// @brief NOT ゲートを表すクラス
//////////////////////////////////////////////////////////////////////
class GateNot :
  public GateBuf
{
public:
  
  /// @brief コンストラクタ
  /// @param[in] id ゲートID
  /// @param[in] node 元のノード
  GateNot(ymuint id,
	  const TgNode* node);
  
  /// @brief デストラクタ
  virtual
  ~GateNot();


public:
  
  virtual
  void
  calc_C();
  
  virtual
  ymuint
  calc_O(ymuint pos);
  
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

END_NAMESPACE_YM_ATPG_DTPGCL

#endif
