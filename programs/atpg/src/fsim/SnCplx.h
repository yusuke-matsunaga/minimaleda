#ifndef FSIM_SNCPLX_H
#define FSIM_SNCPLX_H

/// @file fsim/SnCplx.h
/// @brief SnCplx のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: SnCplx.h 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "SnGate.h"


BEGIN_NAMESPACE_YM_ATPG_FSIM

//////////////////////////////////////////////////////////////////////
/// @class SnCplx SimNode.h
/// @brief 上記以外の複雑な論理式を持つノード
//////////////////////////////////////////////////////////////////////
class SnCplx :
  public SnGate
{
public:

  /// @brief コンストラクタ
  SnCplx(ymuint32 id,
	 const LogExpr& lexp,
	 const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnCplx();


public:

  /// @brief 正常値の計算を行う．(2値版)
  virtual
  PackedVal
  _calc_gval2();

  /// @brief 故障値の計算を行う．(2値版)
  virtual
  PackedVal
  _calc_fval2();
  
  /// @brief ゲートの入力から出力までの可観測性を計算する．(2値版)
  virtual
  PackedVal
  calc_gobs2(ymuint ipos);
  
  /// @brief 内容をダンプする．
  virtual
  void
  dump(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 論理式
  LogExpr mExpr;
  
};

END_NAMESPACE_YM_ATPG_FSIM

#endif // FSIM_SNCPLX_H
