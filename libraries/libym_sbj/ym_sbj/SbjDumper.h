#ifndef YM_SBJ_SBJDUMPER_H
#define YM_SBJ_SBJDUMPER_H

/// @file ym_sbj/SbjDumper.h
/// @brief SbjDumper のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_sbj/sbj_nsdef.h"


BEGIN_NAMESPACE_YM_SBJ

//////////////////////////////////////////////////////////////////////
/// @class SbjDumper SbjDumper.h "SbjDumper.h"
/// @brief SbjGraph の内容を出力するためのクラス
/// @note 実はクラスにする必要はない．
//////////////////////////////////////////////////////////////////////
class SbjDumper
{
public:

  /// @brief コンストラクタ
  SbjDumper();

  /// @brief デストラクタ
  ~SbjDumper();


public:
  //////////////////////////////////////////////////////////////////////
  // 出力関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 独自形式で出力する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] sbjgraph 対象のサブジェクトグラフ
  void
  dump(ostream& s,
       const SbjGraph& sbjgraph);

  /// @brief blif 形式で出力する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] sbjgraph 対象のサブジェクトグラフ
  void
  dump_blif(ostream&,
	    const SbjGraph& sbjgraph);

  /// @brief Verilog-HDL 形式で出力する関数
  /// @param[in] s 出力先のストリーム
  /// @param[in] sbjgraph 対象のネットワーク
  void
  dump_verilog(ostream& s,
	       const SbjGraph& sbjgraph);

};

END_NAMESPACE_YM_SBJ

#endif // YM_SBJ_SBJDUMPER_H
