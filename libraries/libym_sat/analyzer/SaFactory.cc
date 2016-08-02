
/// @file libym_sat/analyzer/SaFactory.cc
/// @brief SaFactory の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SaFactory.cc 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "SaUIP1.h"
#include "SaSimple.h"


BEGIN_NAMESPACE_YM_SAT

// @brief SatAnalyzerの派生クラスを生成する．
// @param[in] option どのクラスを生成するかを決めるオプション文字列
SatAnalyzer*
SaFactory::gen_analyzer(const string& option)
{
  if ( option == "uip1" ) {
    return new SaUIP1;
  }
  if ( option == "simple" ) {
    return new SaSimple;
  }
  // default fall-back
  return new SaUIP1;
}

END_NAMESPACE_YM_SAT
