#ifndef DTPG_CLASSIC_GATEINPUT_H
#define DTPG_CLASSIC_GATEINPUT_H

/// @file src/dtpg_classic/GateInput.h
/// @brief GateInput の派生クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: GateInput.h 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "Gate.h"


BEGIN_NAMESPACE_YM_ATPG_DTPGCL

//////////////////////////////////////////////////////////////////////
/// @class GateInput GateInput.h "GateInput.h"
/// @brief 外部入力を表すクラス
//////////////////////////////////////////////////////////////////////
class GateInput :
  public Gate
{
public:
  
  /// @brief コンストラクタ
  /// @param[in] id ゲートID
  /// @param[in] node 元のノード
  /// @param[in] input_id 入力番号
  GateInput(ymuint id,
	    const TgNode* node,
	    ymuint input_id);

  /// @brief デストラクタ
  virtual
  ~GateInput();


public:
  
  virtual
  ymuint
  input_id() const;
  
  virtual
  void
  calc_C();

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

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 入力番号
  ymuint32 mInputId;
  
};

END_NAMESPACE_YM_ATPG_DTPGCL

#endif
