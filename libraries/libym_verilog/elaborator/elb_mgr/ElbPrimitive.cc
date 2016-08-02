
/// @file libym_verilog/elb/ElbPrimitive.cc
/// @brief ElbPrimitive の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ElbPrimitive.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ElbPrimitive.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief 入出力ポート数を計算する．
// @param[in] type プリミティブの型
// @param[in] port_size 全ポート数
// @param[out] output_num 出力ポート数
// @param[out] inout_num 入出力ポート数
// @param[out] input_num 入力ポート数
// @retval 1 port_size が多すぎる．
// @retval 0 port_size が範囲内に収まっている．
// @retval -1 port_size が少なすぎる．
int
ElbPrimitive::get_port_size(tVpiPrimType type,
			    ymuint32 port_size,
			    ymuint32& output_num,
			    ymuint32& inout_num,
			    ymuint32& input_num)
{
  ymuint32 min_size = 0;
  ymuint32 max_size = port_size;
  
  switch ( type ) {
  case kVpiAndPrim:
  case kVpiNandPrim:
  case kVpiNorPrim:
  case kVpiOrPrim:
  case kVpiXorPrim:
  case kVpiXnorPrim:
    min_size = 3;
    output_num = 1;
    inout_num = 0;
    input_num = port_size - 1;
    break;

  case kVpiBufPrim:
  case kVpiNotPrim:
    min_size = 2;
    output_num = port_size - 1;
    inout_num = 0;
    input_num = 1;
    break;

  case kVpiBufif0Prim:
  case kVpiBufif1Prim:
  case kVpiNotif0Prim:
  case kVpiNotif1Prim:
    min_size = 3;
    max_size = 3;
    output_num = 1;
    inout_num = 0;
    input_num = 2;
    break;

  case kVpiNmosPrim:
  case kVpiPmosPrim:
  case kVpiRnmosPrim:
  case kVpiRpmosPrim:
    min_size = 3;
    max_size = 3;
    output_num = 1;
    inout_num = 0;
    input_num = 2;
    break;

  case kVpiCmosPrim:
  case kVpiRcmosPrim:
    min_size = 4;
    max_size = 4;
    output_num = 1;
    inout_num = 0;
    input_num = 3;
    break;
    
  case kVpiTranPrim:
  case kVpiRtranPrim:
    min_size = 2;
    max_size = 2;
    output_num = 0;
    inout_num = 2;
    input_num = 0;
    break;

  case kVpiRtranif0Prim:
  case kVpiRtranif1Prim:
  case kVpiTranif0Prim:
  case kVpiTranif1Prim:
    min_size = 3;
    max_size = 3;
    output_num = 0;
    inout_num = 2;
    input_num = 1;
    break;

  case kVpiPullupPrim:
  case kVpiPulldownPrim:
    min_size = 1;
    max_size = 1;
    output_num = 1;
    inout_num = 0;
    input_num = 0;
    break;

  default:
    // UDP
    min_size = 1;
    output_num = 1;
    inout_num = 0;
    input_num = port_size - 1;
    break;
  }
  if ( port_size < min_size ) {
    return -1;
  }
  if ( port_size > max_size ) {
    return 1;
  }
  return 0;
}

END_NAMESPACE_YM_VERILOG
