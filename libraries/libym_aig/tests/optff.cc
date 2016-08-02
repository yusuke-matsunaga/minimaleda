
/// @file libym_aig/tests/optff.cc
/// @brief 最適ファクタードフォームを求めるプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: optff.cc 1594 2008-07-18 10:26:12Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_lexp/LogExpr.h"
#include "ym_npn/TvFunc.h"
#include "ym_npn/NpnMgr.h"
#include "ym_npn/NpnMap.h"


BEGIN_NAMESPACE_YM

// 関数ベクタとそれに対応するファクタードフォームを記録するためのクラス
struct FuncRec
{
  // 関数の真理値表ベクタ
  ymuint32 mFuncVect;

  // ファクタードフォーム
  LogExpr mExpr;

};

// 3入力関数に対する最適ファクタードフォームの表
hash_map<ymuint32, LogExpr> optff3_table;

// optff3_table を作る．
void
init_table()
{
  LogExpr a = LogExpr::make_posiliteral(0);
  LogExpr b = LogExpr::make_posiliteral(1);
  LogExpr c = LogExpr::make_posiliteral(2);

  // 定数0
  optff3_table.insert(make_pair(0UL, LogExpr::make_zero()));

  // a
  optff3_table.insert(make_pair(0xAAUL, a));

  // ab
  optff3_table.insert(make_pair(0x88UL, a & b));

  // ab' + a'b
  optff3_table.insert(make_pair(0x66UL, (a & ~b) | (~a & b)));

  // abc
  optff3_table.insert(make_pair(0x80UL, a & b & c));

  // a(b + c)
  optff3_table.insert(make_pair(0xA8UL, a & (b | c)));

  // ac' + bc
  optff3_table.insert(make_pair(0xCAUL, (a & ~c) | (b & c)));

  // ab + (a + b)c
  optff3_table.insert(make_pair(0xE8UL, (a & b) | ((a | b) & c)));

  // a(bc' + b'c)
  optff3_table.insert(make_pair(0x28UL, a & ((b & ~c) | (~b & c))));

  // ab + a'b'c
  optff3_table.insert(make_pair(0x98UL, (a & b) | (~a & ~b & c)));

  // a'b'c' + abc
  optff3_table.insert(make_pair(0x81UL, (~a & ~b & ~c) | (a & b & c)));

  // ac' + (ab' + a'b)c
  optff3_table.insert(make_pair(0x6AUL, (a & ~c) | (((a & ~b) | (~a & b)) & c)));

  // abc' + (ab' + a'b)c
  optff3_table.insert(make_pair(0x68UL, (a & b & ~c) | (((a & ~b) | (~a & b)) & c)));

  // (ab' + a'b)c' + (a'b' + ab)c
  optff3_table.insert(make_pair(0x96UL, (((a & ~b) | (~a & b)) & ~c) | (((~a & ~b) | (a & b)) & c)));

}

// 3入力関数に対する最適ファクタードフォームを求める．
LogExpr
optff3(const TvFunc& func)
{
  assert_cond(func.ni() == 3, __FILE__, __LINE__);

  NpnMgr mgr;
  NpnMap cmap;
  mgr.cannonical(func, cmap);

  TvFunc cfunc = func.xform(cmap);
  ymuint32 pat = 0UL;
  for (size_t pos = 0; pos < 8; ++ pos) {
    if ( cfunc.value(pos) ) {
      pat |= (1UL << pos);
    }
  }
  hash_map<ymuint32, LogExpr>::iterator p = optff3_table.find(pat);
  if ( p == optff3_table.end() ) {
    cout << "Assertion Error" << endl
	 << " could not find entry for ";
    for (size_t pos = 0; pos < 8; ++ pos) {
      cout << cfunc.value(pos);
    }
    cout << endl;
    cout << "pat = " << hex << pat << dec << endl;
    exit(1);
  }

  NpnMap rmap = inverse(cmap);
  VarLogExprMap lmap;
  for (size_t i = 0; i < 3; ++ i) {
    tNpnImap imap = rmap.imap(i);
    LogExpr tmp = LogExpr::make_literal(npnimap_pos(imap), npnimap_pol(imap));
    lmap.insert(make_pair(i, tmp));
  }
  LogExpr ans = p->second.compose(lmap);
  if ( rmap.opol() == kPolNega ) {
    ans = ~ans;
  }

  // 検証
  {
    ymuint32 pat = 0UL;
    for (size_t pos = 0; pos < 8; ++ pos) {
      if ( func.value(pos) ) {
	pat |= (1UL << pos);
      }
    }
    vector<ymulong> vals(3);
    for (size_t i = 0; i < 3; ++ i) {
      ymulong pat = 0UL;
      for (size_t pos = 0; pos < 8; ++ pos) {
	if ( pos & (1UL << i) ) {
	  pat |= (1UL << pos);
	}
      }
      vals[i] = pat;
    }
    ymulong pat2 = ans.eval(vals, 0xFFUL);
    if ( pat != pat2 ) {
      cout << "error" << endl
	   << "pat = ";
      for (size_t pos = 0; pos < 8; ++ pos) {
	if ( pat & (1UL << pos) ) {
	  cout << "1";
	}
	else {
	  cout << "0";
	}
      }
      cout << endl;
      cout << "pat2 = ";
      for (size_t pos = 0; pos < 8; ++ pos) {
	if ( pat2 & (1UL << pos) ) {
	  cout << "1";
	}
	else {
	  cout << "0";
	}
      }
      cout << endl;
    }
  }
  return ans;
}

void
test0()
{
  init_table();
  
  vector<int> values(8);
  for (ymulong v = 0; v < 256; ++ v) {
    for (size_t p = 0; p < 8; ++ p) {
      values[p] = (v & (1UL << p)) ? 1 : 0;
    }
    TvFunc f(3, values);
    LogExpr expr = optff3(f);
    cout << "  ff3table[" << setw(3) << v << "] = " << expr << ";" << endl;
  }
}

END_NAMESPACE_YM


int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm;

  test0();
  
  return 0;
}
