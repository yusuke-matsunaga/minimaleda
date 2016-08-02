
/// @file libym_verilog/common/udpval.cc
/// @brief tVpiUdpVal を扱う関数の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: udpval.cc 700 2007-05-31 00:41:30Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/verilog.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief UDP テーブルのシンボルを表す文字列を得る．
// @param[in] val UDP テーブルのシンボル値
// @return val を表す文字列
string
symbol2string(tVpiUdpVal val)
{
  switch ( val ) {
  case kVpiUdpVal0:  return "0";
  case kVpiUdpVal1:  return "1";
  case kVpiUdpValX:  return "x";
  case kVpiUdpValB:  return "b";
  case kVpiUdpValQ:  return "?";

  case kVpiUdpVal00: return "00";
  case kVpiUdpVal01: return "01";
  case kVpiUdpVal0X: return "0x";
  case kVpiUdpVal0B: return "0b";
  case kVpiUdpVal0Q: return "0?";

  case kVpiUdpVal10: return "10";
  case kVpiUdpVal11: return "11";
  case kVpiUdpVal1X: return "1x";
  case kVpiUdpVal1B: return "1b";
  case kVpiUdpVal1Q: return "1?";

  case kVpiUdpValX0: return "x0";
  case kVpiUdpValX1: return "x1";
  case kVpiUdpValXX: return "xx";
  case kVpiUdpValXB: return "xb";
  case kVpiUdpValXQ: return "x?";

  case kVpiUdpValB0: return "b0";
  case kVpiUdpValB1: return "b1";
  case kVpiUdpValBX: return "bx";
  case kVpiUdpValBB: return "bb";
  case kVpiUdpValBQ: return "b?";

  case kVpiUdpValQ0: return "?0";
  case kVpiUdpValQ1: return "?1";
  case kVpiUdpValQX: return "?x";
  case kVpiUdpValQB: return "?b";
  case kVpiUdpValQQ: return "??";

  case kVpiUdpValP:  return "p";
  case kVpiUdpValN:  return "n";

  case kVpiUdpValNC: return "-";

  default:
    ;
  }
  if ( val < 8 ) {
    // 予想外の1値
    return "@";
  }
  // 予想外の2値
  return "@@";
}

BEGIN_NONAMESPACE

inline
int
concat(char c1,
       char c2)
{
  return (static_cast<int>(c1) << 8) + static_cast<int>(c2);
}

END_NONAMESPACE

// @brief UDP テーブルのシンボルを 2バイトの文字にする．
// @param[in] val UDP テーブルのシンボル値
// @return val を表す2バイトの文字
int
symbol2dbyte(tVpiUdpVal val)
{
  switch ( val ) {
  case kVpiUdpVal0:  return concat('0', 0);
  case kVpiUdpVal1:  return concat('1', 0);
  case kVpiUdpValX:  return concat('x', 0);
  case kVpiUdpValB:  return concat('b', 0);
  case kVpiUdpValQ:  return concat('?', 0);

  case kVpiUdpVal00: return concat('0', '0');
  case kVpiUdpVal01: return concat('0', '1');
  case kVpiUdpVal0X: return concat('0', 'x');
  case kVpiUdpVal0B: return concat('0', 'b');
  case kVpiUdpVal0Q: return concat('0', '?');

  case kVpiUdpVal10: return concat('1', '0');
  case kVpiUdpVal11: return concat('1', '1');
  case kVpiUdpVal1X: return concat('1', 'x');
  case kVpiUdpVal1B: return concat('1', 'b');
  case kVpiUdpVal1Q: return concat('1', '?');

  case kVpiUdpValX0: return concat('x', '0');
  case kVpiUdpValX1: return concat('x', '1');
  case kVpiUdpValXX: return concat('x', 'x');
  case kVpiUdpValXB: return concat('x', 'b');
  case kVpiUdpValXQ: return concat('x', '?');

  case kVpiUdpValB0: return concat('b', '0');
  case kVpiUdpValB1: return concat('b', '1');
  case kVpiUdpValBX: return concat('b', 'x');
  case kVpiUdpValBB: return concat('b', 'b');
  case kVpiUdpValBQ: return concat('b', '?');

  case kVpiUdpValQ0: return concat('?', '0');
  case kVpiUdpValQ1: return concat('?', '1');
  case kVpiUdpValQX: return concat('?', 'x');
  case kVpiUdpValQB: return concat('?', 'b');
  case kVpiUdpValQQ: return concat('?', '?');

  case kVpiUdpValP:  return concat('p', 0);
  case kVpiUdpValN:  return concat('n', 0);

  case kVpiUdpValNC: return concat('-', 0);

  default:
    ;
  }
  if ( val < 8 ) {
    // 予想外の1値
    return concat('@', 0);
  }
  // 予想外の2値
  return concat('@', '@');
}

// @brief シンボルのマージ
// @param symbol1 シンボル1
// @param symbol2 シンボル2
// @return symbol1 と symbol2 をあわせたシンボルを返す．
// @note symbol1, symbol2 ともにレベルシンボルでなければならない．
tVpiUdpVal
merge_udp_value(tVpiUdpVal symbol1,
		tVpiUdpVal symbol2)
{
  assert_cond(symbol1 > 0 && symbol1 < 8, __FILE__, __LINE__);
  assert_cond(symbol2 > 0 && symbol2 < 8, __FILE__, __LINE__);
  int new_val = 0;
  if ( symbol1 & kVpiUdpVal0 ) {
    if ( symbol2 & kVpiUdpVal0 ) {
      new_val |= kVpiUdpVal00;
    }
    if ( symbol2 & kVpiUdpVal1 ) {
      new_val |= kVpiUdpVal01;
    }
    if ( symbol2 & kVpiUdpValX ) {
      new_val |= kVpiUdpVal0X;
    }
  }
  if ( symbol1 & kVpiUdpVal1 ) {
    if ( symbol2 & kVpiUdpVal0 ) {
      new_val |= kVpiUdpVal10;
    }
    if ( symbol2 & kVpiUdpVal1 ) {
      new_val |= kVpiUdpVal11;
    }
    if ( symbol2 & kVpiUdpValX ) {
      new_val |= kVpiUdpVal1X;
    }
  }
  if ( symbol1 & kVpiUdpValX ) {
    if ( symbol2 & kVpiUdpVal0 ) {
      new_val |= kVpiUdpValX0;
    }
    if ( symbol2 & kVpiUdpVal1 ) {
      new_val |= kVpiUdpValX1;
    }
    if ( symbol2 & kVpiUdpValX ) {
      new_val |= kVpiUdpValXX;
    }
  }
  assert_cond(new_val > 0, __FILE__, __LINE__);
  return static_cast<tVpiUdpVal>(new_val);
}


END_NAMESPACE_YM_VERILOG
