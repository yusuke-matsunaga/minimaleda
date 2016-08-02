
/// @file libym_cell/ci/restore.cc
/// @brief バイナリファイルを読み込む関数
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "CiLibrary.h"
#include "CiPin.h"


BEGIN_NAMESPACE_YM_CELL

BEGIN_NONAMESPACE

/// @brief 32ビットの一語を読み込む．
/// @param[in] s 入力元のストリーム
ymuint
read_word(istream& s)
{
  static char buf[4];
  s.read(buf, 4);
  ymuint ans =
    static_cast<ymuint>(buf[0]) |
    (static_cast<ymuint>(buf[1]) << 8) |
    (static_cast<ymuint>(buf[2]) << 16) |
    (static_cast<ymuint>(buf[3]) << 24);
  return ans;
}

/// @brief 文字列を読み込む．
/// @param[in] s 入力元のストリーム
string
read_str(istream& s)
{
  ymuint l = read_word(s);
  char* buf = new char[l + 1];
  s.read(buf, l);
  buf[l] = '\0';
  string ans(buf);
  delete [] buf;
  return ans;
}

/// @brief 倍精度浮動小数点数を読み込む．
/// @param[in] s 入力元のストリーム
double
read_real(istream& s)
{
  static char buf[sizeof(double)];
  s.read(buf, sizeof(double));
  return *(reinterpret_cast<double*>(buf));
}

END_NONAMESPACE

const CellLibrary*
restore_library(istream& s)
{
  string name = read_str(s);

  CiLibrary* library = new CiLibrary(name);

  ymuint nc = read_word(s);
  library->set_cell_num(nc);

  for (ymuint i = 0; i < nc; ++ i) {
    ShString name( read_str(s) );
    CellArea area( read_real(s) );
    ymuint np = read_word(s);
    ymuint nb = read_word(s);
    ymuint nc = read_word(s);
    CiCell* cell = library->new_cell(i, name, area, np, nb, nc);

    ymuint nt = read_word(s);
    vector<const CellTiming*> timing_list(nt);
    for (ymuint j = 0; j < nt; ++ j) {
      CellTime i_r( read_real(s) );
      CellTime i_f( read_real(s) );
      CellTime s_r( read_real(s) );
      CellTime s_f( read_real(s) );
      CellResistance r_r( read_real(s) );
      CellResistance f_r( read_real(s) );
      timing_list[j] = library->new_timing(j, kTimingCombinational,
					   i_r, i_f, s_r, s_f, r_r, f_r);
    }

    for (ymuint j = 0; j < np; ++ j) {
      ShString name( read_str(s) );
      ymuint d = read_word(s);
      switch ( d ) {
      case 1: // 入力
	{
	  CellCapacitance cap( read_real(s) );
	  CellCapacitance r_cap( read_real(s) );
	  CellCapacitance f_cap( read_real(s) );
	  library->new_cell_input(cell, j, name, cap, r_cap, f_cap);
	}
	break;

      case 2: // 出力
	{
	  CellCapacitance max_f( read_real(s) );
	  CellCapacitance min_f( read_real(s) );
	  CellCapacitance max_c( read_real(s) );
	  CellCapacitance min_c( read_real(s) );
	  CellTime max_t( read_real(s) );
	  CellTime min_t( read_real(s) );
	  CiOutputPin* pin = library->new_cell_output(cell, j, name,
						      max_f, min_f,
						      max_c, min_c,
						      max_t, min_t);
	  for ( ; ; ) {
	    ymuint unate = read_word(s);
	    if ( unate == 0 ) break;
	    if ( unate == 1 ) {
	      ymuint pin_id = read_word(s);
	      ymuint timing_id = read_word(s);
	      const CellTiming* timing = timing_list[timing_id];
	      library->set_opin_timing(pin, pin_id, kSensePosiUnate, timing);
	    }
	    else if ( unate == 2 ) {
	      ymuint pin_id = read_word(s);
	      ymuint timing_id = read_word(s);
	      const CellTiming* timing = timing_list[timing_id];
	      library->set_opin_timing(pin, pin_id, kSenseNegaUnate, timing);
	    }
	  }
	}
	break;

      case 3: // 入出力
	{
	  CellCapacitance cap( read_real(s) );
	  CellCapacitance r_cap( read_real(s) );
	  CellCapacitance f_cap( read_real(s) );
	  CellCapacitance max_f( read_real(s) );
	  CellCapacitance min_f( read_real(s) );
	  CellCapacitance max_c( read_real(s) );
	  CellCapacitance min_c( read_real(s) );
	  CellTime max_t( read_real(s) );
	  CellTime min_t( read_real(s) );
	  CiInoutPin* pin = library->new_cell_inout(cell, j, name,
						    cap, r_cap, f_cap,
						    max_f, min_f,
						    max_c, min_c,
						    max_t, min_t);
	  for ( ; ; ) {
	    ymuint unate = read_word(s);
	    if ( unate == 0 ) break;
	    if ( unate == 1 ) {
	      ymuint pin_id = read_word(s);
	      ymuint timing_id = read_word(s);
	      const CellTiming* timing = timing_list[timing_id];
	      library->set_opin_timing(pin, pin_id, kSensePosiUnate, timing);
	    }
	    else if ( unate == 2 ) {
	      ymuint pin_id = read_word(s);
	      ymuint timing_id = read_word(s);
	      const CellTiming* timing = timing_list[timing_id];
	      library->set_opin_timing(pin, pin_id, kSenseNegaUnate, timing);
	    }
	  }
	}
	break;
      }
    }
  }

  return library;
}

END_NAMESPACE_YM_CELL
