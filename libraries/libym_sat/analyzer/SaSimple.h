#ifndef LIBYM_SAT_SASIMPLE_H
#define LIBYM_SAT_SASIMPLE_H

/// @file libym_sat/SaSimple.h
/// @brief SaSimple のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SaSimple.h 1293 2008-02-21 02:25:52Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "SaBase.h"


BEGIN_NAMESPACE_YM_SAT

//////////////////////////////////////////////////////////////////////
/// @class SaSimple SaSimple.h "SaSimple.h"
/// @brief 単純な解析器
//////////////////////////////////////////////////////////////////////
class SaSimple :
  public SaBase
{
public:

  /// @brief コンストラクタ
  SaSimple();
  
  /// @brief デストラクタ
  virtual
  ~SaSimple();


public:
  
  /// @brief 解析を行う．
  /// @param[in] creason 矛盾を起こした原因
  /// @param[out] learnt 学習された節を表すリテラルのベクタ
  /// @return バックトラックレベル
  virtual
  int
  analyze(SatReason creason,
	  vector<Literal>& learnt);


protected:

  void
  capture(SatReason creason,
	  vector<Literal>& learnt);
  
};

END_NAMESPACE_YM_SAT

#endif // LIBYM_SAT_SASIMPLE_H
