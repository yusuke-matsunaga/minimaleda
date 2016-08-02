#ifndef FSIMX_SNCPLX_H
#define FSIMX_SNCPLX_H

/// @file fsim/SnCplx.h
/// @brief SnCplx のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: SnCplx.h 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "SnGate.h"


BEGIN_NAMESPACE_YM_ATPG_FSIMX

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

  /// @brief 正常値の計算を行う．(3値版)
  virtual
  PackedVal3
  _calc_gval3();

  /// @brief 故障値の計算を行う．(3値版)
  virtual
  PackedVal3
  _calc_fval3();

  /// @brief ゲートの入力から出力までの可観測性を計算する．(3値版)
  virtual
  PackedVal
  calc_gobs3(ymuint ipos);
  
  /// @brief 内容をダンプする．
  virtual
  void
  dump(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 論理式
  LogExpr mExpr[2];
  
};

END_NAMESPACE_YM_ATPG_FSIMX

#endif // FSIMX_SNCPLX_H
