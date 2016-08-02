#ifndef YM_CEC_CEC_NSDEF
#define YM_CEC_CEC_NSDEF

/// @file magus/equiv/equiv_nsdef.h
/// @brief equiv2 の名前空間の定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: equiv_nsdef.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"
#include "ym_bnet/BNetwork.h"
#include "ym_sat/Bool3.h"


// 名前空間の定義
#define BEGIN_NAMESPACE_YM_CEC \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsCec)

#define END_NAMESPACE_YM_CEC \
END_NAMESPACE(nsCec) \
END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_CEC
  
/// @brief 組み合わせ回路の等価検証を行う関数 
void
check_ceq(const BNetwork& src_network1,
	  const BNetwork& src_network2,
	  const vector<pair<ymuint32, ymuint32> >& iassoc,
	  const vector<pair<ymuint32, ymuint32> >& oassoc,
	  ymint log_level,
	  ostream* log_out,
	  const string& sat_type,
	  const string& sat_option,
	  ostream* sat_out,
	  ymuint sigsize,
	  vector<Bool3>& stats);

END_NAMESPACE_YM_CEC

BEGIN_NAMESPACE_YM

using nsCec::check_ceq;

END_NAMESPACE_YM

#endif // YM_CEC_CEC_NSDEF
