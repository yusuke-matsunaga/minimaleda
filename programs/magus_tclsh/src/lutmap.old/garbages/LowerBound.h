#ifndef MAGUS_LUTMAP_LOWERBOUND_H
#define MAGUS_LUTMAP_LOWERBOUND_H

/// @file magus/lutmap/LowerBound.cc
/// @brief LowerBound のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: LowerBound.h 1077 2007-12-03 10:18:47Z matsunaga $
///
/// Copyright (C) 2005-2007 Yusuke Matsunaga
/// All rights reserved.

#include "lutmap.h"


BEGIN_NAMESPACE_MAGUS_LUTMAP

class SbjGraph;
class CutHolder;

//////////////////////////////////////////////////////////////////////
/// @class LowerBound LowerBound.h "LowerBound.h"
/// @brief DAG covering の下限を計算するための基底クラス
//////////////////////////////////////////////////////////////////////
class LowerBound
{
public:

  /// @brief デストラクタ
  virtual
  ~LowerBound() { }

  
public:

  /// @brief 下限を計算する．
  virtual
  int
  calc(const SbjGraph& sbjgraph,
       const CutHolder& cut_holder) = 0;
  
};


//////////////////////////////////////////////////////////////////////
/// @class LBFactory LowerBound.h "LowerBound.h"
/// @brief LowerBound (の継承クラス)を生成するファクトリクラス
//////////////////////////////////////////////////////////////////////
class LBFactory
{
public:

  /// @brief コンストラクタ
  LBFactory();
  
  /// @brief デストラクタ
  ~LBFactory();


public:

  /// @brief LowerBound を生成する．
  LowerBound*
  operator()(const string& args) const;

};

END_NAMESPACE_MAGUS_LUTMAP

#endif // MAGUS_LUTMAP_LOWERBOUND_H
