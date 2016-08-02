/// @file libym_aig/tests/optaig3.cc
/// @brief 3入力の最適AIGを求めるプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: optaig3.cc 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_npn/TvFunc.h"
#include "ym_npn/NpnMgr.h"
#include "ym_npn/NpnMap.h"
#include "AigTemplate.h"
#include "ym_sat/SatSolver.h"


#define VERIFY_FF2TABLE 0


BEGIN_NAMESPACE_YM

// 3入力のNPN同値類代表関数
ymuint32 npn3rep[] = {
  0x00U,
  0xCAU,
  0xE8U,
  0x28U,
  0x66U,
  0x68U,
  0x6AU,
  0x80U,
  0x81U,
  0x88U,
  0x96U,
  0x98U,
  0xA8U,
  0xAAU,
  0xFFU
};

// 4入力のNPN同値類代表関数
ymuint32 npn4rep[] = {
  0x0000U,
  0xC282U,
  0xE880U,
  0x62C8U,
  0xE881U,
  0xE882U,
  0xE883U,
  0xE886U,
  0x8EE0U,
  0x6A68U,
  0xE888U,
  0xE889U,
  0x6A6AU,
  0x56AAU,
  0xE88AU,
  0x8EE8U,
  0xC8A8U,
  0xE898U,
  0x2AA8U,
  0x782EU,
  0x4A98U,
  0x9818U,
  0xC2A4U,
  0x9696U,
  0xC2A8U,
  0xE8A8U,
  0xE41AU,
  0x9088U,
  0xE8AAU,
  0x4AA4U,
  0x4AA8U,
  0x2AC0U,
  0xAA64U,
  0x1888U,
  0xAA68U,
  0x96A8U,
  0x96AAU,
  0xBCA8U,
  0xE2ACU,
  0x6AA8U,
  0x6AA9U,
  0x6AAAU,
  0x0660U,
  0x6008U,
  0x4AC2U,
  0x6AACU,
  0xCA68U,
  0xB82CU,
  0x724EU,
  0x8180U,
  0x8181U,
  0x8000U,
  0x8001U,
  0x8002U,
  0x2968U,
  0x8AA8U,
  0xE2CAU,
  0x6AC2U,
  0xAA94U,
  0xEA68U,
  0x588AU,
  0x6AC8U,
  0xE8E8U,
  0x6ACAU,
  0x9868U,
  0xAAA8U,
  0xF08EU,
  0x96E8U,
  0x8AC0U,
  0xAAAAU,
  0x7886U,
  0x7888U,
  0x7889U,
  0x6C62U,
  0xCA98U,
  0x1EE8U,
  0x6C6AU,
  0x6AE8U,
  0x665AU,
  0xCAA4U,
  0xA4A8U,
  0x6968U,
  0xCAA8U,
  0xAAC0U,
  0x696AU,
  0xCAAAU,
  0xAAC1U,
  0xAAC2U,
  0x9886U,
  0xCAACU,
  0xC00AU,
  0x9888U,
  0x988AU,
  0x6666U,
  0xAAC8U,
  0x6668U,
  0xC4A2U,
  0x8960U,
  0x9898U,
  0x2828U,
  0xEAA8U,
  0xE006U,
  0x8660U,
  0xCAC2U,
  0xE008U,
  0xE009U,
  0xAC62U,
  0x8668U,
  0xCACAU,
  0xE4A8U,
  0xE4AAU,
  0x4828U,
  0xA968U,
  0xD88AU,
  0x6818U,
  0xEAC8U,
  0x43A8U,
  0x6088U,
  0x69A8U,
  0xCAE8U,
  0x4228U,
  0x6828U,
  0xC96AU,
  0x8998U,
  0x682AU,
  0xF888U,
  0xEC62U,
  0x8080U,
  0xC66AU,
  0x0880U,
  0xE968U,
  0x1AC8U,
  0xE668U,
  0xD2ACU,
  0xA088U,
  0xA9A8U,
  0x2880U,
  0xA824U,
  0x8228U,
  0x6CE2U,
  0xA828U,
  0x5AACU,
  0x2888U,
  0x6248U,
  0x624AU,
  0x69E8U,
  0x83C2U,
  0x6868U,
  0x4882U,
  0xA9C2U,
  0x1968U,
  0x66E8U,
  0xC828U,
  0xC82AU,
  0xA842U,
  0x17E8U,
  0x89E0U,
  0xE816U,
  0x8E70U,
  0xE818U,
  0xE819U,
  0x700EU,
  0x1668U,
  0x5ACAU,
  0x3C6AU,
  0x86E0U,
  0xC3A8U,
  0xE088U,
  0xE08AU,
  0x4282U,
  0x6880U,
  0x6881U,
  0xC228U,
  0xE826U,
  0x6882U,
  0xC229U,
  0xC22AU,
  0xE828U,
  0x9006U,
  0xE829U,
  0xE82AU,
  0x9008U,
  0x0EE0U,
  0x6888U,
  0x688AU,
  0xA248U,
  0x4EB8U,
  0x8880U,
  0xA6E8U,
  0x8888U,
  0x9AC8U,
  0x2EE8U,
  0xE84AU,
  0x68A8U,
  0x8280U,
  0xC6E2U,
  0xA880U,
  0xA881U,
  0x2BE8U,
  0xA882U,
  0xA888U,
  0xE246U,
  0x16A8U,
  0x4EE2U,
  0xE248U,
  0xE249U,
  0x3CA8U,
  0xE24AU,
  0x3CAAU,
  0x9C6AU,
  0xE868U,
  0xC882U,
  0x9968U,
  0x4A68U,
  0x6EE0U,
  0xC88AU,
  0x9666U,
  0xF00EU,
  0x9668U,
  0x9669U,
  0x966AU,
  0xA8A8U,
  0xFFFFU // end-marker
};

// 5入力のNPN同値類代表関数
ymuint32 npn5rep[] = {
#if 0
#include "npn5rep.h"
#else
  0xacb00e68U,
  0xFFFFFFFFU
#endif
};

hash_map<ymuint32, size_t> npn4map;

hash_map<ymuint32, size_t> npn5map;

// 2入力関数に対する最適AIGを作るための表
AigTemplate aig2table[16];

// 3入力関数に対する最適AIGを作るための表
AigTemplate aig3table[14];

// 4入力関数に対する最適AIGを作るための表
AigTemplate aig4table[222];

// 5入力関数に対する最適AIGを作るための表
AigTemplate aig5table[616126];


bool
optaig(size_t ni,
       ymuint32 pat,
       int limit,
       AigTemplate& templ);


// ni 入力関数のパタンを出力する．
void
dump_pat(ostream& s,
	 size_t ni,
	 ymuint32 pat)
{
  size_t np = 1U << ni;
  for (size_t i = 0; i < np; ++ i) {
    if ( pat & (1U << i) ) {
      s << "1";
    }
    else {
      s << "0";
    }
  }
}

bool
search_aigtable(size_t ni,
		ymuint32 pat,
		AigTemplate& templ)
{
  NpnMap cmap;
  ymuint32 pat0;
  {
    // pat に対応する真理値表を作る．
    // 結果は func に入る．
    size_t np = 1U << ni;
    vector<int> vals(np);
    for (size_t i = 0; i < np; ++ i) {
      if ( pat & (1U << i) ) {
	vals[i] = 1;
      }
      else {
	vals[i] = 0;
      }
    }
    TvFunc func(ni, vals);

    // その NPN 同値類を求める．
    // 結果は pat0 と cmap に入る．
    NpnMgr mgr;
    mgr.cannonical(func, cmap);
    TvFunc func0 = func.xform(cmap);
    pat0 = 0U;
    for (size_t i = 0; i < np; ++ i) {
      if ( func0.value(i) ) {
	pat0 |= (1U << i);
      }
    }
  }
  
  if ( ni == 3 ) {
    // pat0 のエントリを探す．
    size_t id0;
    for (id0 = 0; ; ++ id0) {
      if ( npn3rep[id0] == pat0 ) {
	break;
      }
      if ( npn3rep[id0] == 0xFFU ) {
	// npn3rep に pat0 のエントリが無かった．
	// テーブルのデータがミスっていることになる．
	assert_not_reached(__FILE__, __LINE__);
      }
    }
    AigTemplate& tmp0 = aig3table[id0];
    if ( tmp0.set() ) {
      // tmp0 を cmap にしたがって変換したものを tmepl に入れる．
      NpnMap rmap = inverse(cmap);
      templ.set_copy(tmp0, rmap);
      return true;
    }
    return false;
  }
  
  if ( ni == 4 ) {
    // pat0 に対応するエントリを探す．
    // いくつかの sanity check を行う．
    hash_map<ymuint32, size_t>::const_iterator p = npn4map.find(pat0);
    assert_cond(p != npn4map.end(), __FILE__, __LINE__);
    size_t id0 = p->second;
    assert_cond(npn4rep[id0] == pat0, __FILE__, __LINE__);
    
    AigTemplate& tmp0 = aig4table[id0];
    if ( tmp0.set() ) {
      // tmp0 を cmap にしたがって変換したものを tmepl に入れる．
      NpnMap rmap = inverse(cmap);
      templ.set_copy(tmp0, rmap);
      return true;
    }
    return false;
  }
  
  if ( ni == 5 ) {
    // pat0 に対応するエントリを探す．
    // いくつかの sanity check を行う．
    hash_map<ymuint32, size_t>::const_iterator p = npn5map.find(pat0);
    assert_cond(p != npn5map.end(), __FILE__, __LINE__);
    size_t id0 = p->second;
    assert_cond(npn5rep[id0] == pat0, __FILE__, __LINE__);
    
    AigTemplate& tmp0 = aig5table[id0];
    if ( tmp0.set() ) {
      // tmp0 を cmap にしたがって変換したものを tmepl に入れる．
      NpnMap rmap = inverse(cmap);
      templ.set_copy(tmp0, rmap);
      return true;
    }
    return false;
  }
  
  assert_not_reached(__FILE__, __LINE__);
  return false;
}

// 分解などで2入力以下に落とせる関数の処理を行う．
bool
decomp3(ymuint32 pat,
	AigTemplate& templ)
{
  // 各変数に対するコファクターを計算する．
  // ちょっとトリッキー
  ymuint32 pat0[3];
  ymuint32 pat1[3];
  
  ymuint32 tmp;

  tmp = pat & 0x55U;        // 0x2x4x6x
  tmp |= (tmp >> 1);        // 0224466x
  tmp &= 0x33U;             // 02xx46xx
  tmp |= (tmp >> 2);        // 024646xx
  tmp &= 0x0FU;             // 0246xxxx
  pat0[0] = tmp;
  
  tmp = (pat >> 1) & 0x55U; // 1x3x5x7x
  tmp |= (tmp >> 1);        // 1335577x
  tmp &= 0x33U;             // 13xx57xx
  tmp |= (tmp >> 2);        // 135757xx
  tmp &= 0x0FU;             // 1357xxxx
  pat1[0] = tmp;

  tmp = pat & 0x33U;        // 01xx45xx
  tmp |= (tmp >> 2);        // 014545xx
  tmp &= 0x0FU;             // 0145xxxx
  pat0[1] = tmp;

  tmp = (pat >> 2) & 0x33U; // 23xx67xx
  tmp |= (tmp >> 2);        // 236767xx
  tmp &= 0x0FU;             // 2367xxxx
  pat1[1] = tmp;

  tmp = pat & 0x0FU;        // 0123xxxx
  pat0[2] = tmp;

  tmp = (pat >> 4) & 0x0FU; // 4567xxxx
  pat1[2] = tmp;
  
  if ( 0 ) {
    cout << "step1(";
    dump_pat(cout, 3, pat);
    cout << ")" << endl;
    cout << "  c0: ";
    dump_pat(cout, 2, pat0[0]);
    cout << ", ";
    dump_pat(cout, 2, pat1[0]);
    cout << endl;
    cout << "  c1: ";
    dump_pat(cout, 2, pat0[1]);
    cout << ", ";
    dump_pat(cout, 2, pat1[1]);
    cout << endl;
    cout << "  c2: ";
    dump_pat(cout, 2, pat0[2]);
    cout << ", ";
    dump_pat(cout, 2, pat1[2]);
    cout << endl;
  }
  
  for (size_t i = 0; i < 3; ++ i) {
    if ( pat0[i] == pat1[i] ) {
      // i 番めの変数と無関係
      hash_map<size_t, size_t> vmap;
      size_t k = 0;
      for (size_t j = 0; j < 3; ++ j) {
	if ( j != i ) {
	  vmap.insert(make_pair(k, j));
	  ++ k;
	}
      }
      templ.set_copy(aig2table[pat0[i]], vmap);
      return true;
    }
    
    if ( pat0[i] == 0x0U ) {
      if ( pat1[i] == 0xFU ) {
	// xi
	templ.set_literal(i, false);
	return true;
      }
      // xi & f_xi
      hash_map<size_t, size_t> vmap;
      size_t k = 0;
      for (size_t j = 0; j < 3; ++ j) {
	if ( j != i ) {
	  vmap.insert(make_pair(k, j));
	  ++ k;
	}
      }
      AigTemplate lit1;
      lit1.set_literal(i, false);
      AigTemplate expr1(aig2table[pat1[i]], vmap);
      templ.set_and(lit1, false, expr1, false);
      return true;
    }
    
    if ( pat0[i] == 0xFU ) {
      if ( pat1[i] == 0x0U ) {
	// xi'
	templ.set_literal(i, true);
	return true;
      }
      // xi' | f_xi
      hash_map<size_t, size_t> vmap;
      size_t k = 0;
      for (size_t j = 0; j < 3; ++ j) {
	if ( j != i ) {
	  vmap.insert(make_pair(k, j));
	  ++ k;
	}
      }
      AigTemplate lit1;
      lit1.set_literal(i, true);
      AigTemplate expr1(aig2table[pat1[i]], vmap);
      templ.set_or(lit1, false, expr1, false);
      return true;
    }
    
    if ( pat1[i] == 0x0U ) {
      // xi' & f_xi'
      hash_map<size_t, size_t> vmap;
      size_t k = 0;
      for (size_t j = 0; j < 3; ++ j) {
	if ( j != i ) {
	  vmap.insert(make_pair(k, j));
	  ++ k;
	}
      }
      AigTemplate lit1;
      lit1.set_literal(i, true);
      AigTemplate expr1(aig2table[pat0[i]], vmap);
      templ.set_and(lit1, false, expr1, false);
      return true;
    }
    
    if ( pat1[i] == 0xFU ) {
      // xi | f_xi'
      hash_map<size_t, size_t> vmap;
      size_t k = 0;
      for (size_t j = 0; j < 3; ++ j) {
	if ( j != i ) {
	  vmap.insert(make_pair(k, j));
	  ++ k;
	}
      }
      AigTemplate lit1;
      lit1.set_literal(i, false);
      AigTemplate expr1(aig2table[pat0[i]], vmap);
      templ.set_or(lit1, false, expr1, false);
      return true;
    }
    
    if ( pat0[i] == (pat1[i] ^ 0xFU) ) {
      // xi xor f_xi'
      hash_map<size_t, size_t> vmap;
      size_t k = 0;
      for (size_t j = 0; j < 3; ++ j) {
	if ( j != i ) {
	  vmap.insert(make_pair(k, j));
	  ++ k;
	}
      }
      AigTemplate lit1;
      lit1.set_literal(i, false);
      AigTemplate expr1(aig2table[pat0[i]], vmap);
      templ.set_xor(lit1, false, expr1, false);
      return true;
    }
  }

  return false;
}

// 分解などで3入力以下に落とせる関数の処理を行う．
bool
decomp4(ymuint32 pat,
	AigTemplate& templ)
{
  // 各変数に対するコファクターを計算する．
  // ちょっとトリッキー
  ymuint32 pat0[4];
  ymuint32 pat1[4];
  
  ymuint32 tmp;

  tmp = pat & 0x5555U;        // 0x2x4x6x8xAxCxEx
  tmp |= (tmp >> 1);          // 022446688AACCEEx
  tmp &= 0x3333U;             // 02xx46xx8AxxCExx
  tmp |= (tmp >> 2);          // 0246468A8ACECExx
  tmp &= 0x0F0FU;             // 0246xxxx8ACExxxx
  tmp |= (tmp >> 4);          // 02468ACE8ACExxxx
  tmp &= 0x00FFU;             // 02468ACExxxxxxxx
  pat0[0] = tmp;
  
  tmp = (pat >> 1) & 0x5555U; // 1x3x5x7x9xBxDxFx
  tmp |= (tmp >> 1);          // 133557799BBDDFFx
  tmp &= 0x3333U;             // 13xx57xx9BxxDFxx
  tmp |= (tmp >> 2);          // 1357579B9BDFDFxx
  tmp &= 0x0F0FU;             // 1357xxxx9BDFxxxx
  tmp |= (tmp >> 4);          // 13579BDF9BDFxxxx
  tmp &= 0x00FFU;             // 13579BDFxxxxxxxx
  pat1[0] = tmp;

  tmp = pat & 0x3333U;        // 01xx45xx89xxCDxx
  tmp |= (tmp >> 2);          // 0145458989CDCDxx
  tmp &= 0x0F0FU;             // 0145xxxx89CDxxxx
  tmp |= (tmp >> 4);          // 014589CD89CDxxxx
  tmp &= 0x00FFU;             // 014589CDxxxxxxxx
  pat0[1] = tmp;

  tmp = (pat >> 2) & 0x3333U; // 23xx67xxABxxEFxx
  tmp |= (tmp >> 2);          // 236767ABABEFEFxx
  tmp &= 0x0F0FU;             // 2367xxxxABEFxxxx
  tmp |= (tmp >> 4);          // 2367ABEFABEFxxxx
  tmp &= 0x00FFU;             // 2367ABEFxxxxxxxx
  pat1[1] = tmp;

  tmp = pat & 0x0F0FU;        // 0123xxxx89ABxxxx
  tmp |= (tmp >> 4);          // 012389AB89ABxxxx
  tmp &= 0x00FFU;             // 012389ABxxxxxxxx
  pat0[2] = tmp;

  tmp = (pat >> 4) & 0x0F0FU; // 4567xxxxCDEFxxxx
  tmp |= (tmp >> 4);          // 4567CDEFCDEFxxxx
  tmp &= 0x00FFU;             // 4567CDEFxxxxxxxx
  pat1[2] = tmp;
  
  pat0[3] = pat & 0x00FFU;
  pat1[3] = (pat >> 8) & 0x00FFU;
  
  for (size_t i = 0; i < 4; ++ i) {
    if ( pat0[i] == pat1[i] ) {
      // i 番めの変数と無関係
      hash_map<size_t, size_t> vmap;
      size_t k = 0;
      for (size_t j = 0; j < 4; ++ j) {
	if ( j != i ) {
	  vmap.insert(make_pair(k, j));
	  ++ k;
	}
      }
      AigTemplate tmp;
      bool stat = search_aigtable(3, pat0[i], tmp);
      assert_cond(stat, __FILE__, __LINE__);
      templ.set_copy(tmp, vmap);
      return true;
    }
    
    if ( pat0[i] == 0x00U ) {
      if ( pat1[i] == 0xFFU ) {
	// xi
	templ.set_literal(i, false);
	return true;
      }
      // xi & f_xi
      hash_map<size_t, size_t> vmap;
      size_t k = 0;
      for (size_t j = 0; j < 4; ++ j) {
	if ( j != i ) {
	  vmap.insert(make_pair(k, j));
	  ++ k;
	}
      }
      AigTemplate lit1;
      lit1.set_literal(i, false);
      AigTemplate tmp;
      bool stat = search_aigtable(3, pat1[i], tmp);
      assert_cond(stat, __FILE__, __LINE__);
      AigTemplate expr1(tmp, vmap);
      templ.set_and(lit1, false, expr1, false);
      return true;
    }
    
    if ( pat0[i] == 0xFFU ) {
      if ( pat1[i] == 0x00U ) {
	// xi'
	templ.set_literal(i, true);
	return true;
      }
      // xi' | f_xi
      hash_map<size_t, size_t> vmap;
      size_t k = 0;
      for (size_t j = 0; j < 4; ++ j) {
	if ( j != i ) {
	  vmap.insert(make_pair(k, j));
	  ++ k;
	}
      }
      AigTemplate lit1;
      lit1.set_literal(i, true);
      AigTemplate tmp;
      bool stat = search_aigtable(3, pat1[i], tmp);
      assert_cond(stat, __FILE__, __LINE__);
      AigTemplate expr1(tmp, vmap);
      templ.set_or(lit1, false, expr1, false);
      return true;
    }
    
    if ( pat1[i] == 0x00UL ) {
      // xi' & f_xi'
      hash_map<size_t, size_t> vmap;
      size_t k = 0;
      for (size_t j = 0; j < 4; ++ j) {
	if ( j != i ) {
	  vmap.insert(make_pair(k, j));
	  ++ k;
	}
      }
      AigTemplate lit1;
      lit1.set_literal(i, true);
      AigTemplate tmp;
      bool stat = search_aigtable(3, pat0[i], tmp);
      assert_cond(stat, __FILE__, __LINE__);
      AigTemplate expr1(tmp, vmap);
      templ.set_and(lit1, false, expr1, false);
      return true;
    }
    
    if ( pat1[i] == 0xFFUL ) {
      // xi | f_xi'
      hash_map<size_t, size_t> vmap;
      size_t k = 0;
      for (size_t j = 0; j < 4; ++ j) {
	if ( j != i ) {
	  vmap.insert(make_pair(k, j));
	  ++ k;
	}
      }
      AigTemplate lit1;
      lit1.set_literal(i, false);
      AigTemplate tmp;
      bool stat = search_aigtable(3, pat0[i], tmp);
      assert_cond(stat, __FILE__, __LINE__);
      AigTemplate expr1(tmp, vmap);
      templ.set_or(lit1, false, expr1, false);
      return true;
    }
    
    if ( pat0[i] == (pat1[i] ^ 0xFFU) ) {
      // xi xor f_xi'
      hash_map<size_t, size_t> vmap;
      size_t k = 0;
      for (size_t j = 0; j < 4; ++ j) {
	if ( j != i ) {
	  vmap.insert(make_pair(k, j));
	  ++ k;
	}
      }
      AigTemplate lit1;
      lit1.set_literal(i, false);
      AigTemplate tmp;
      bool stat = search_aigtable(3, pat0[i], tmp);
      assert_cond(stat, __FILE__, __LINE__);
      AigTemplate expr1(tmp, vmap);
      templ.set_xor(lit1, false, expr1, false);
      return true;
    }
  }

  int v[16];
  for (size_t i = 0; i < 16; ++ i) {
    v[i] = (pat >> i) & 1;
  }
  
  for (size_t x1 = 0; x1 < 3; ++ x1) {
    for (size_t x2 = x1 + 1; x2 < 4; ++ x2) {
      // (x1, x2) に対するコファクターを求める．
      // まず，残りの変数 x3, x4 を求める．
      size_t x3;
      for (x3 = 0; x3 < 4; ++ x3) {
	if ( x3 != x1 && x3 != x2 ) break;
      }
      assert_cond(x3 < 4, __FILE__, __LINE__);
      size_t x4;
      for (x4 = x3 + 1; x4 < 4; ++ x4) {
	if ( x4 != x1 && x4 != x2 ) break;
      }
      assert_cond(x4 < 4, __FILE__, __LINE__);

      size_t mask1 = 1U << x1;
      size_t mask2 = 1U << x2;
      size_t mask3 = 1U << x3;
      size_t mask4 = 1U << x4;
      size_t idx0 = 0U;
      size_t idx1 = mask3;
      size_t idx2 = mask4;
      size_t idx3 = mask3 + mask4;
      size_t base = 0;
      ymuint32 pat[4];
      pat[0] =
	(v[base + idx0] << 0) |
	(v[base + idx1] << 1) |
	(v[base + idx2] << 2) |
	(v[base + idx3] << 3);
      base = mask1;
      pat[1] =
	(v[base + idx0] << 0) |
	(v[base + idx1] << 1) |
	(v[base + idx2] << 2) |
	(v[base + idx3] << 3);
      base = mask2;
      pat[2] =
	(v[base + idx0] << 0) |
	(v[base + idx1] << 1) |
	(v[base + idx2] << 2) |
	(v[base + idx3] << 3);
      base = mask1 + mask2;
      pat[3] =
	(v[base + idx0] << 0) |
	(v[base + idx1] << 1) |
	(v[base + idx2] << 2) |
	(v[base + idx3] << 3);
      
      if ( 0 ) {
	cout << "x1 = " << x1
	     << ", x2 = " << x2
	     << ", x3 = " << x3
	     << ", x4 = " << x4
	     << endl;
	cout << "pat00 = ";
	dump_pat(cout, 2, pat[0]);
	cout << endl;
	cout << "pat01 = ";
	dump_pat(cout, 2, pat[1]);
	cout << endl;
	cout << "pat10 = ";
	dump_pat(cout, 2, pat[2]);
	cout << endl;
	cout << "pat11 = ";
	dump_pat(cout, 2, pat[3]);
	cout << endl;
      }

      // 4つのコファクター pat[0], pat[1], pat[2], pat[3] を分類する．
      // pat[0] と等しいグループを groupa とし，残りを gorupb, groupc
      // とする．4つとも別々の場合にはなにもしない．
      ymuint32 pata = pat[0];
      size_t groupa = 1U;
      ymuint32 patb = 0x0U;
      size_t groupb = 0U;
      size_t groupc = 0U;
      if ( pat[1] == pata ) {
	groupa |= 2U;
      }
      else {
	patb = pat[1];
	groupb |= 2U;
      }
      if ( pat[2] == pata ) {
	groupa |= 4U;
      }
      else if ( groupb == 0U || pat[2] == patb ) {
	patb = pat[2];
	groupb |= 4U;
      }
      else {
	groupc |= 4U;
      }
      if ( pat[3] == pata ) {
	groupa |= 8U;
      }
      else if ( groupb == 0U || pat[3] == patb ) {
	patb = pat[3];
	groupb |= 8U;
      }
      else {
	groupc |= 8U;
      }

      if ( groupc == 0U ) {
	assert_cond( groupa + groupb == 15U, __FILE__, __LINE__);
	// コファクターが2つのパタンからなる．
	switch ( groupa ) {
	case 1U:  // 00         : 10, 01, 11
	  {// t = x1 | x2 とおいて x3, x4, t の関数を作る．
	    AigTemplate lit1;
	    lit1.set_literal(x1, false);
	    AigTemplate lit2;
	    lit2.set_literal(x2, false);
	    AigTemplate t;
	    t.set_or(lit1, false, lit2, false);
	    ymuint32 pat1 = pata | (patb << 4);
	    AigTemplate tmp;
	    bool stat = search_aigtable(3, pat1, tmp);
	    assert_cond(stat, __FILE__, __LINE__);
	    AigTemplate lit3;
	    lit3.set_literal(x3, false);
	    AigTemplate lit4;
	    lit4.set_literal(x4, false);
	    vector<AigTemplate> tmap;
	    tmap.push_back(lit3);
	    tmap.push_back(lit4);
	    tmap.push_back(t);
	    templ.set_compose(tmp, tmap);
	  }
	  return true;
	  
	case 7U:  // 00, 10, 01 : 11
	  {// t = x1 & x2 とおいて x3, x4, t の関数を作る．
	    AigTemplate lit1;
	    lit1.set_literal(x1, false);
	    AigTemplate lit2;
	    lit2.set_literal(x2, false);
	    AigTemplate t;
	    t.set_and(lit1, false, lit2, false);
	    ymuint32 pat1 = pata | (patb << 4);
	    AigTemplate tmp;
	    bool stat = search_aigtable(3, pat1, tmp);
	    assert_cond(stat, __FILE__, __LINE__);
	    AigTemplate lit3;
	    lit3.set_literal(x3, false);
	    AigTemplate lit4;
	    lit4.set_literal(x4, false);
	    vector<AigTemplate> tmap;
	    tmap.push_back(lit3);
	    tmap.push_back(lit4);
	    tmap.push_back(t);
	    templ.set_compose(tmp, tmap);
	  }
	  return true;
	  
	case 11U: // 00, 10, 11 : 01
	  {// t = x1 & x2' とおいて x3, x4, t の関数を作る．
	    AigTemplate lit1;
	    lit1.set_literal(x1, false);
	    AigTemplate lit2;
	    lit2.set_literal(x2, true);
	    AigTemplate t;
	    t.set_and(lit1, false, lit2, false);
	    ymuint32 pat1 = pata | (patb << 4);
	    AigTemplate tmp;
	    bool stat = search_aigtable(3, pat1, tmp);
	    assert_cond(stat, __FILE__, __LINE__);
	    AigTemplate lit3;
	    lit3.set_literal(x3, false);
	    AigTemplate lit4;
	    lit4.set_literal(x4, false);
	    vector<AigTemplate> tmap;
	    tmap.push_back(lit3);
	    tmap.push_back(lit4);
	    tmap.push_back(t);
	    templ.set_compose(tmp, tmap);
	  }
	  return true;
	  
	case 13U: // 00, 01, 11 : 10
	  {// t = x1' & x2 とおいて x3, x4, t の関数を作る．
	    AigTemplate lit1;
	    lit1.set_literal(x1, true);
	    AigTemplate lit2;
	    lit2.set_literal(x2, false);
	    AigTemplate t;
	    t.set_and(lit1, false, lit2, false);
	    ymuint32 pat1 = pata | (patb << 4);
	    AigTemplate tmp;
	    bool stat = search_aigtable(3, pat1, tmp);
	    assert_cond(stat, __FILE__, __LINE__);
	    AigTemplate lit3;
	    lit3.set_literal(x3, false);
	    AigTemplate lit4;
	    lit4.set_literal(x4, false);
	    vector<AigTemplate> tmap;
	    tmap.push_back(lit3);
	    tmap.push_back(lit4);
	    tmap.push_back(t);
	    templ.set_compose(tmp, tmap);
	  }
	  return true;
	  
	case 9U:  // 00, 11     : 10, 01
	  {// t = x1 ^ x2 とおいて x3, x4, t の関数を作る．
	    AigTemplate lit1;
	    lit1.set_literal(x1, false);
	    AigTemplate lit2;
	    lit2.set_literal(x2, false);
	    AigTemplate t;
	    t.set_xor(lit1, false, lit2, false);
	    ymuint32 pat1 = pata | (patb << 4);
	    AigTemplate tmp;
	    bool stat = search_aigtable(3, pat1, tmp);
	    assert_cond(stat, __FILE__, __LINE__);
	    AigTemplate lit3;
	    lit3.set_literal(x3, false);
	    AigTemplate lit4;
	    lit4.set_literal(x4, false);
	    vector<AigTemplate> tmap;
	    tmap.push_back(lit3);
	    tmap.push_back(lit4);
	    tmap.push_back(t);
	    templ.set_compose(tmp, tmap);
	  }
	  return true;

	default:
	  // これはありえない
	  assert_not_reached(__FILE__, __LINE__);
	  break;
	}
      }
    }
  }
  return false;
}

// 分解などで3入力以下に落とせる関数の処理を行う．
bool
ndecomp4(ymuint32 pat,
	 int limit,
	 AigTemplate& templ)
{
  for (size_t x1 = 0; x1 < 4; ++ x1) {
    for (size_t x2 = 0; x2 < 4; ++ x2) {
      if ( x1 == x2 ) continue;

      size_t x3;
      for (x3 = 0; x3 < 3; ++ x3) {
	if ( x3 != x1 && x3 != x2 ) break;
      }
      assert_cond(x3 < 3, __FILE__, __LINE__);
      size_t x4;
      for (x4 = x3 + 1; x4 < 4; ++ x4) {
	if ( x4 != x1 && x4 != x2 ) break;
      }
      assert_cond(x4 < 4, __FILE__, __LINE__);
      
      // x1: h のみに入力している変数
      // x2: h と g に入力している変数
      // x3, x4: g のみに入力している変数
      if ( 0 ) {
	cout << "x1 = " << x1
	     << ", x2 = " << x2
	     << ", x3 = " << x3
	     << ", x4 = " << x4 << endl;
      }
      
      SatSolver* solver = SatSolverFactory::gen_solver();
      // h と g の真理値を表すSAT変数を生成
      for (size_t i = 0; i < 16; ++ i) {
	tVarId id = solver->new_var();
	assert_cond(id == i, __FILE__, __LINE__);
      }

      // f の各々の真理値に対するSAT節を生成
      for (size_t p = 0; p < 16; ++ p) {
	size_t pos = 0UL;
	if ( p & 1UL ) {
	  // x1 が 1
	  pos |= (1UL << x1);
	}
	if ( p & 2UL ) {
	  // x2 が 1
	  pos |= (1UL << x2);
	}
	if ( p & 4UL ) {
	  // x3 が 1
	  pos |= (1UL << x3);
	}
	if ( p & 8UL ) {
	  // x4 が 1
	  pos |= (1UL << x4);
	}
	size_t gidx = (p >> 1) + 8;
	size_t hidx0 = (p & 0x3U);
	size_t hidx1 = hidx0 | 0x4U;
	if ( pat & (1UL << pos) ) {
	  // f(x1, x2, x3, x4) = 1
	  solver->add_clause(Literal(gidx, kPolNega), Literal(hidx1, kPolPosi));
	  solver->add_clause(Literal(gidx, kPolPosi), Literal(hidx0, kPolPosi));
	}
	else {
	  // f(x1, x2, x3, x4) = 0
	  solver->add_clause(Literal(gidx, kPolNega), Literal(hidx1, kPolNega));
	  solver->add_clause(Literal(gidx, kPolPosi), Literal(hidx0, kPolNega));
	}
      }

      vector<tBool3> model;
      tBool3 ans = solver->solve(model);
      delete solver;
      if ( ans == kB3True ) {
	ymuint32 pat_h = 0U;
	ymuint32 pat_g = 0U;
	for (size_t p = 0; p < 8; ++ p) {
	  if ( model[p] == kB3True ) {
	    pat_h |= (1U << p);
	  }
	  if ( model[p + 8] == kB3True ) {
	    pat_g |= (1U << p);
	  }
	}
	AigTemplate lit1;
	lit1.set_literal(x1, false);
	AigTemplate lit2;
	lit2.set_literal(x2, false);
	AigTemplate lit3;
	lit3.set_literal(x3, false);
	AigTemplate lit4;
	lit4.set_literal(x4, false);
	AigTemplate tmp1;
	bool stat1 = search_aigtable(3, pat_g, tmp1);
	assert_cond(stat1, __FILE__, __LINE__);
	vector<AigTemplate> tmap1;
	tmap1.push_back(lit2);
	tmap1.push_back(lit3);
	tmap1.push_back(lit4);
	AigTemplate g;
	g.set_compose(tmp1, tmap1);
	AigTemplate tmp2;
	bool stat2 = search_aigtable(3, pat_h, tmp2);
	assert_cond(stat2, __FILE__, __LINE__);
	vector<AigTemplate> tmap2;
	tmap2.push_back(lit1);
	tmap2.push_back(lit2);
	tmap2.push_back(g);
	AigTemplate tmp3;
	tmp3.set_compose(tmp2, tmap2);
	if ( tmp3.cost() <= limit ) {
	  templ.set_copy(tmp3, hash_map<size_t, size_t>());
	  return true;
	}
      }
    }
  }
  return false;
}

// 分解などで4入力以下に落とせる関数の処理を行う．
bool
decomp5(ymuint32 pat,
	AigTemplate& templ)
{
  // 各変数に対するコファクターを計算する．
  // ちょっとトリッキー
  ymuint32 pat0[5];
  ymuint32 pat1[5];
  
  ymuint32 tmp;

  tmp = pat & 0x55555555U;        // 0x2x4x6x8xAxCxEx0x2x4x6x8xAxCxEx
  tmp |= (tmp >> 1);              // 022446688AACCEE0022446688AACCEEx
  tmp &= 0x33333333U;             // 02xx46xx8AxxCExx02xx46xx8AxxCExx
  tmp |= (tmp >> 2);              // 0246468A8ACECE020246468A8ACECExx
  tmp &= 0x0F0F0F0FU;             // 0246xxxx8ACExxxx0246xxxx8ACExxxx
  tmp |= (tmp >> 4);              // 02468ACE8ACE024602468ACE8ACExxxx
  tmp &= 0x00FF00FFU;             // 02468ACExxxxxxxx02468ACExxxxxxxx
  tmp |= (tmp >> 8);              // 02468ACE02468ACE02468ACExxxxxxxx
  tmp &= 0x0000FFFFU;             // 02468ACE02468ACExxxxxxxxxxxxxxxx
  pat0[0] = tmp;
  
  tmp = (pat >> 1) & 0x55555555U; // 1x3x5x7x9xBxDxFx1x3x5x7x9xBxDxFx
  tmp |= (tmp >> 1);              // 133557799BBDDFF1133557799BBDDFFx
  tmp &= 0x33333333U;             // 13xx57xx9BxxDFxx13xx57xx9BxxDFxx
  tmp |= (tmp >> 2);              // 1357579B9BDFDF131357579B9BDFDFxx
  tmp &= 0x0F0F0F0FU;             // 1357xxxx9BDFxxxx1357xxxx9BDFxxxx
  tmp |= (tmp >> 4);              // 13579BDF9BDF135713579BDF9BDFxxxx
  tmp &= 0x00FF00FFU;             // 13579BDFxxxxxxxx13579BDFxxxxxxxx
  tmp |= (tmp >> 8);              // 13579BDF13579BDF13579BDFxxxxxxxx
  tmp &= 0x0000FFFFU;             // 13579BDF13579BDFxxxxxxxxxxxxxxxx
  pat1[0] = tmp;

  tmp = pat & 0x33333333U;        // 01xx45xx89xxCDxx01xx45xx89xxCDxx
  tmp |= (tmp >> 2);              // 0145458989CDCD010145458989CDCDxx
  tmp &= 0x0F0F0F0FU;             // 0145xxxx89CDxxxx0145xxxx89CDxxxx
  tmp |= (tmp >> 4);              // 014589CD89CD0145014589CD89CDxxxx
  tmp &= 0x00FF00FFU;             // 014589CDxxxxxxxx014589CDxxxxxxxx
  tmp |= (tmp >> 8);              // 014589CD014589CD014589CDxxxxxxxx
  tmp &= 0x0000FFFFU;             // 014589CD014589CDxxxxxxxxxxxxxxxx
  pat0[1] = tmp;

  tmp = (pat >> 2) & 0x33333333U; // 23xx67xxABxxEFxx23xx67xxABxxEFxx
  tmp |= (tmp >> 2);              // 236767ABABEFEF23236767ABABEFEFxx
  tmp &= 0x0F0F0F0FU;             // 2367xxxxABEFxxxx2367xxxxABEFxxxx
  tmp |= (tmp >> 4);              // 2367ABEFABEF23672367ABEFABEFxxxx
  tmp &= 0x00FF00FFU;             // 2367ABEFxxxxxxxx2367ABEFxxxxxxxx
  tmp |= (tmp >> 8);              // 2367ABEF2367ABEF2367ABEFxxxxxxxx
  tmp &= 0x0000FFFFU;             // 2367ABEF2367ABEFxxxxxxxxxxxxxxxx
  pat1[1] = tmp;

  tmp = pat & 0x0F0F0F0FU;        // 0123xxxx89ABxxxx0123xxxx89ABxxxx
  tmp |= (tmp >> 4);              // 012389AB89AB0123012389AB89ABxxxx
  tmp &= 0x00FF00FFU;             // 012389ABxxxxxxxx012389ABxxxxxxxx
  tmp |= (tmp >> 8);              // 012389AB012389AB012389ABxxxxxxxx
  tmp &= 0x0000FFFFU;             // 012389AB012389ABxxxxxxxxxxxxxxxx
  pat0[2] = tmp;

  tmp = (pat >> 4) & 0x0F0F0F0FU; // 4567xxxxCDEFxxxx4567xxxxCDEFxxxx
  tmp |= (tmp >> 4);              // 4567CDEFCDEF45674567CDEFCDEFxxxx
  tmp &= 0x00FF00FFU;             // 4567CDEFxxxxxxxx4567CDEFxxxxxxxx
  tmp |= (tmp >> 8);              // 4567CDEF4567CDEF4567CDEFxxxxxxxx
  tmp &= 0x0000FFFFU;             // 4567CDEF4567CDEFxxxxxxxxxxxxxxxx
  pat1[2] = tmp;

  tmp = pat & 0x00FF00FFU;        // 01234567xxxxxxxx01234567xxxxxxxx
  tmp |= (tmp >> 8);              // 012345670123456701234567xxxxxxxx
  tmp &= 0x0000FFFFU;             // 0123456701234567xxxxxxxxxxxxxxxx
  pat0[3] = tmp;

  tmp = (pat >> 8) & 0x00FF00FFU; // 89ABCDEFxxxxxxxx89ABCDEFxxxxxxxx
  tmp |= (tmp >> 8);              // 89ABCDEF89ABCDEF89ABCDEFxxxxxxxx
  tmp &= 0x0000FFFFU;             // 89ABCDEF89ABCDEFxxxxxxxxxxxxxxxx
  pat1[3] = tmp;
  
  pat0[4] = pat & 0x0000FFFFU;
  pat1[4] = (pat >> 16) & 0x0000FFFFU;
  
  for (size_t i = 0; i < 5; ++ i) {
    if ( 0 ) {
      cout << "x1 = " << i << endl;
      cout << "pat0 = ";
      dump_pat(cout, 4, pat0[i]);
      cout << endl;
      cout << "pat1 = ";
      dump_pat(cout, 4, pat1[i]);
      cout << endl;
    }
    
    if ( pat0[i] == pat1[i] ) {
      // i 番めの変数と無関係
      hash_map<size_t, size_t> vmap;
      size_t k = 0;
      for (size_t j = 0; j < 5; ++ j) {
	if ( j != i ) {
	  vmap.insert(make_pair(k, j));
	  ++ k;
	}
      }
      AigTemplate tmp;
      bool stat = search_aigtable(4, pat0[i], tmp);
      assert_cond(stat, __FILE__, __LINE__);
      templ.set_copy(tmp, vmap);
      return true;
    }
    
    if ( pat0[i] == 0x0000U ) {
      if ( pat1[i] == 0xFFFFU ) {
	// xi
	templ.set_literal(i, false);
	return true;
      }
      // xi & f_xi
      hash_map<size_t, size_t> vmap;
      size_t k = 0;
      for (size_t j = 0; j < 5; ++ j) {
	if ( j != i ) {
	  vmap.insert(make_pair(k, j));
	  ++ k;
	}
      }
      AigTemplate lit1;
      lit1.set_literal(i, false);
      AigTemplate tmp;
      bool stat = search_aigtable(4, pat1[i], tmp);
      assert_cond(stat, __FILE__, __LINE__);
      AigTemplate expr1(tmp, vmap);
      templ.set_and(lit1, false, expr1, false);
      return true;
    }
    
    if ( pat0[i] == 0xFFFFU ) {
      if ( pat1[i] == 0x0000U ) {
	// xi'
	templ.set_literal(i, true);
	return true;
      }
      // xi' | f_xi
      hash_map<size_t, size_t> vmap;
      size_t k = 0;
      for (size_t j = 0; j < 5; ++ j) {
	if ( j != i ) {
	  vmap.insert(make_pair(k, j));
	  ++ k;
	}
      }
      AigTemplate lit1;
      lit1.set_literal(i, true);
      AigTemplate tmp;
      bool stat = search_aigtable(4, pat1[i], tmp);
      assert_cond(stat, __FILE__, __LINE__);
      AigTemplate expr1(tmp, vmap);
      templ.set_or(lit1, false, expr1, false);
      return true;
    }
    
    if ( pat1[i] == 0x0000UL ) {
      // xi' & f_xi'
      hash_map<size_t, size_t> vmap;
      size_t k = 0;
      for (size_t j = 0; j < 5; ++ j) {
	if ( j != i ) {
	  vmap.insert(make_pair(k, j));
	  ++ k;
	}
      }
      AigTemplate lit1;
      lit1.set_literal(i, true);
      AigTemplate tmp;
      bool stat = search_aigtable(4, pat0[i], tmp);
      assert_cond(stat, __FILE__, __LINE__);
      AigTemplate expr1(tmp, vmap);
      templ.set_and(lit1, false, expr1, false);
      return true;
    }
    
    if ( pat1[i] == 0xFFFFUL ) {
      // xi | f_xi'
      hash_map<size_t, size_t> vmap;
      size_t k = 0;
      for (size_t j = 0; j < 5; ++ j) {
	if ( j != i ) {
	  vmap.insert(make_pair(k, j));
	  ++ k;
	}
      }
      AigTemplate lit1;
      lit1.set_literal(i, false);
      AigTemplate tmp;
      bool stat = search_aigtable(4, pat0[i], tmp);
      assert_cond(stat, __FILE__, __LINE__);
      AigTemplate expr1(tmp, vmap);
      templ.set_or(lit1, false, expr1, false);
      return true;
    }
    
    if ( pat0[i] == (pat1[i] ^ 0xFFFFU) ) {
      // xi xor f_xi'
      hash_map<size_t, size_t> vmap;
      size_t k = 0;
      for (size_t j = 0; j < 5; ++ j) {
	if ( j != i ) {
	  vmap.insert(make_pair(k, j));
	  ++ k;
	}
      }
      AigTemplate lit1;
      lit1.set_literal(i, false);
      AigTemplate tmp;
      bool stat = search_aigtable(4, pat0[i], tmp);
      assert_cond(stat, __FILE__, __LINE__);
      AigTemplate expr1(tmp, vmap);
      templ.set_xor(lit1, false, expr1, false);
      return true;
    }
  }

  int v[32];
  for (size_t i = 0; i < 32; ++ i) {
    v[i] = (pat >> i) & 1;
  }
  
  // 2変数のコファクター
  for (size_t x1 = 0; x1 < 4; ++ x1) {
    for (size_t x2 = x1 + 1; x2 < 5; ++ x2) {
      // (x1, x2) に対するコファクターを求める．
      size_t x3;
      for (x3 = 0; x3 < 5; ++ x3) {
	if ( x3 != x1 && x3 != x2 ) break;
      }
      assert_cond(x3 < 5, __FILE__, __LINE__);
      size_t x4;
      for (x4 = x3 + 1; x4 < 5; ++ x4) {
	if ( x4 != x1 && x4 != x2 ) break;
      }
      assert_cond(x4 < 5, __FILE__, __LINE__);
      size_t x5;
      for (x5 = x4 + 1; x5 < 5; ++ x5) {
	if ( x5 != x1 && x5 != x2 ) break;
      }
      assert_cond(x5 < 5, __FILE__, __LINE__);

      size_t mask1 = 1U << x1;
      size_t mask2 = 1U << x2;
      size_t mask3 = 1U << x3;
      size_t mask4 = 1U << x4;
      size_t mask5 = 1U << x5;
      size_t idx0 = 0U;
      size_t idx1 = mask3;
      size_t idx2 = mask4;
      size_t idx3 = mask3 + mask4;
      size_t idx4 = mask5;
      size_t idx5 = mask3 + mask5;
      size_t idx6 = mask4 + mask5;
      size_t idx7 = mask3 + mask4 + mask5;
      size_t base = 0;
      ymuint32 pat[4];
      pat[0] =
	(v[base + idx0] << 0) |
	(v[base + idx1] << 1) |
	(v[base + idx2] << 2) |
	(v[base + idx3] << 3) |
	(v[base + idx4] << 4) |
	(v[base + idx5] << 5) |
	(v[base + idx6] << 6) |
	(v[base + idx7] << 7);
      base = mask1;
      pat[1] =
	(v[base + idx0] << 0) |
	(v[base + idx1] << 1) |
	(v[base + idx2] << 2) |
	(v[base + idx3] << 3) |
	(v[base + idx4] << 4) |
	(v[base + idx5] << 5) |
	(v[base + idx6] << 6) |
	(v[base + idx7] << 7);
      base = mask2;
      pat[2] =
	(v[base + idx0] << 0) |
	(v[base + idx1] << 1) |
	(v[base + idx2] << 2) |
	(v[base + idx3] << 3) |
	(v[base + idx4] << 4) |
	(v[base + idx5] << 5) |
	(v[base + idx6] << 6) |
	(v[base + idx7] << 7);
      base = mask1 + mask2;
      pat[3] =
	(v[base + idx0] << 0) |
	(v[base + idx1] << 1) |
	(v[base + idx2] << 2) |
	(v[base + idx3] << 3) |
	(v[base + idx4] << 4) |
	(v[base + idx5] << 5) |
	(v[base + idx6] << 6) |
	(v[base + idx7] << 7);

      if ( 0 ) {
	cout << "x1 = " << x1
	     << ", x2 = " << x2
	     << ", x3 = " << x3
	     << ", x4 = " << x4
	     << ", x5 = " << x5
	     << endl;
	cout << "pat00 = ";
	dump_pat(cout, 3, pat[0]);
	cout << endl;
	cout << "pat01 = ";
	dump_pat(cout, 3, pat[1]);
	cout << endl;
	cout << "pat10 = ";
	dump_pat(cout, 3, pat[2]);
	cout << endl;
	cout << "pat11 = ";
	dump_pat(cout, 3, pat[3]);
	cout << endl;
      }
      
      ymuint32 pata = pat[0];
      size_t groupa = 1U;
      ymuint32 patb = 0x0U;
      size_t groupb = 0U;
      size_t groupc = 0U;
      if ( pat[1] == pata ) {
	groupa |= 2U;
      }
      else {
	patb = pat[1];
	groupb |= 2U;
      }
      if ( pat[2] == pata ) {
	groupa |= 4U;
      }
      else if ( groupb == 0U || pat[2] == patb ) {
	patb = pat[2];
	groupb |= 4U;
      }
      else {
	groupc |= 4U;
      }
      if ( pat[3] == pata ) {
	groupa |= 8U;
      }
      else if ( groupb == 0U || pat[3] == patb ) {
	patb = pat[3];
	groupb |= 8U;
      }
      else {
	groupc |= 8U;
      }

      if ( groupc == 0U ) {
	assert_cond( groupa + groupb == 15U, __FILE__, __LINE__);
	// コファクターが2つのパタンからなる．
	switch ( groupa ) {
	case 1U:  // 00         : 10, 01, 11
	  {// t = x1 | x2 とおいて x3, x4, x5, t の関数を作る．
	    AigTemplate lit1;
	    lit1.set_literal(x1, false);
	    AigTemplate lit2;
	    lit2.set_literal(x2, false);
	    AigTemplate t;
	    t.set_or(lit1, false, lit2, false);
	    ymuint32 pat1 = pata | (patb << 8);
	    AigTemplate tmp;
	    bool stat = search_aigtable(4, pat1, tmp);
	    assert_cond(stat, __FILE__, __LINE__);
	    AigTemplate lit3;
	    lit3.set_literal(x3, false);
	    AigTemplate lit4;
	    lit4.set_literal(x4, false);
	    AigTemplate lit5;
	    lit5.set_literal(x5, false);
	    vector<AigTemplate> tmap;
	    tmap.push_back(lit3);
	    tmap.push_back(lit4);
	    tmap.push_back(lit5);
	    tmap.push_back(t);
	    templ.set_compose(tmp, tmap);
	  }
	  return true;
	  
	case 7U:  // 00, 10, 01 : 11
	  {// t = x1 & x2 とおいて x3, x4, x5, t の関数を作る．
	    AigTemplate lit1;
	    lit1.set_literal(x1, false);
	    AigTemplate lit2;
	    lit2.set_literal(x2, false);
	    AigTemplate t;
	    t.set_and(lit1, false, lit2, false);
	    ymuint32 pat1 = pata | (patb << 8);
	    AigTemplate tmp;
	    bool stat = search_aigtable(4, pat1, tmp);
	    assert_cond(stat, __FILE__, __LINE__);
	    AigTemplate lit3;
	    lit3.set_literal(x3, false);
	    AigTemplate lit4;
	    lit4.set_literal(x4, false);
	    AigTemplate lit5;
	    lit5.set_literal(x5, false);
	    vector<AigTemplate> tmap;
	    tmap.push_back(lit3);
	    tmap.push_back(lit4);
	    tmap.push_back(lit5);
	    tmap.push_back(t);
	    templ.set_compose(tmp, tmap);
	  }
	  return true;
	  
	case 11U: // 00, 10, 11 : 01
	  {// t = x1 & x2' とおいて x3, x4, x5, t の関数を作る．
	    AigTemplate lit1;
	    lit1.set_literal(x1, false);
	    AigTemplate lit2;
	    lit2.set_literal(x2, true);
	    AigTemplate t;
	    t.set_and(lit1, false, lit2, false);
	    ymuint32 pat1 = pata | (patb << 8);
	    AigTemplate tmp;
	    bool stat = search_aigtable(4, pat1, tmp);
	    assert_cond(stat, __FILE__, __LINE__);
	    AigTemplate lit3;
	    lit3.set_literal(x3, false);
	    AigTemplate lit4;
	    lit4.set_literal(x4, false);
	    AigTemplate lit5;
	    lit5.set_literal(x5, false);
	    vector<AigTemplate> tmap;
	    tmap.push_back(lit3);
	    tmap.push_back(lit4);
	    tmap.push_back(lit5);
	    tmap.push_back(t);
	    templ.set_compose(tmp, tmap);
	  }
	  return true;
	  
	case 13U: // 00, 01, 11 : 10
	  {// t = x1' & x2 とおいて x3, x4, x5, t の関数を作る．
	    AigTemplate lit1;
	    lit1.set_literal(x1, true);
	    AigTemplate lit2;
	    lit2.set_literal(x2, false);
	    AigTemplate t;
	    t.set_and(lit1, false, lit2, false);
	    ymuint32 pat1 = pata | (patb << 8);
	    AigTemplate tmp;
	    bool stat = search_aigtable(4, pat1, tmp);
	    assert_cond(stat, __FILE__, __LINE__);
	    AigTemplate lit3;
	    lit3.set_literal(x3, false);
	    AigTemplate lit4;
	    lit4.set_literal(x4, false);
	    AigTemplate lit5;
	    lit5.set_literal(x5, false);
	    vector<AigTemplate> tmap;
	    tmap.push_back(lit3);
	    tmap.push_back(lit4);
	    tmap.push_back(lit5);
	    tmap.push_back(t);
	    templ.set_compose(tmp, tmap);
	  }
	  return true;
	  
	case 9U:  // 00, 11     : 10, 01
	  {// t = x1 ^ x2 とおいて x3, x4, x5, t の関数を作る．
	    AigTemplate lit1;
	    lit1.set_literal(x1, false);
	    AigTemplate lit2;
	    lit2.set_literal(x2, false);
	    AigTemplate t;
	    t.set_xor(lit1, false, lit2, false);
	    ymuint32 pat1 = pata | (patb << 8);
	    AigTemplate tmp;
	    bool stat = search_aigtable(4, pat1, tmp);
	    assert_cond(stat, __FILE__, __LINE__);
	    AigTemplate lit3;
	    lit3.set_literal(x3, false);
	    AigTemplate lit4;
	    lit4.set_literal(x4, false);
	    AigTemplate lit5;
	    lit5.set_literal(x5, false);
	    vector<AigTemplate> tmap;
	    tmap.push_back(lit3);
	    tmap.push_back(lit4);
	    tmap.push_back(lit5);
	    tmap.push_back(t);
	    templ.set_compose(tmp, tmap);
	  }
	  return true;

	default:
	  // これはありえない
	  assert_not_reached(__FILE__, __LINE__);
	  break;
	}
      }
    }
  }
  
  // 3変数のコファクター
  for (size_t x1 = 0; x1 < 3; ++ x1) {
    for (size_t x2 = x1 + 1; x2 < 4; ++ x2) {
      for (size_t x3 = x2 + 1; x3 < 5; ++ x3) {
	// (x1, x2, x3) に対するコファクターを求める．
	size_t x4;
	for (x4 = 0; x4 < 5; ++ x4) {
	  if ( x4 != x1 && x4 != x2 && x4 != x3 ) break;
	}
	assert_cond(x4 < 5, __FILE__, __LINE__);
	size_t x5;
	for (x5 = x4 + 1; x5 < 5; ++ x5) {
	  if ( x5 != x1 && x5 != x2 && x5 != x3 ) break;
	}
	assert_cond(x5 < 5, __FILE__, __LINE__);

	size_t mask1 = 1U << x1;
	size_t mask2 = 1U << x2;
	size_t mask3 = 1U << x3;
	size_t mask4 = 1U << x4;
	size_t mask5 = 1U << x5;
	size_t idx0 = 0U;
	size_t idx1 = mask4;
	size_t idx2 = mask5;
	size_t idx3 = mask4 + mask5;
	size_t base = 0;
	ymuint32 pat[8];
	pat[0] =
	  (v[base + idx0] << 0) |
	  (v[base + idx1] << 1) |
	  (v[base + idx2] << 2) |
	  (v[base + idx3] << 3);
	base = mask1;
	pat[1] =
	  (v[base + idx0] << 0) |
	  (v[base + idx1] << 1) |
	  (v[base + idx2] << 2) |
	  (v[base + idx3] << 3);
	base = mask2;
	pat[2] =
	  (v[base + idx0] << 0) |
	  (v[base + idx1] << 1) |
	  (v[base + idx2] << 2) |
	  (v[base + idx3] << 3);
	base = mask1 + mask2;
	pat[3] =
	  (v[base + idx0] << 0) |
	  (v[base + idx1] << 1) |
	  (v[base + idx2] << 2) |
	  (v[base + idx3] << 3);
	base = mask3;
	pat[4] =
	  (v[base + idx0] << 0) |
	  (v[base + idx1] << 1) |
	  (v[base + idx2] << 2) |
	  (v[base + idx3] << 3);
	base = mask1 + mask3;
	pat[5] =
	  (v[base + idx0] << 0) |
	  (v[base + idx1] << 1) |
	  (v[base + idx2] << 2) |
	  (v[base + idx3] << 3);
	base = mask2 + mask3;
	pat[6] =
	  (v[base + idx0] << 0) |
	  (v[base + idx1] << 1) |
	  (v[base + idx2] << 2) |
	  (v[base + idx3] << 3);
	base = mask1 + mask2 + mask3;
	pat[7] =
	  (v[base + idx0] << 0) |
	  (v[base + idx1] << 1) |
	  (v[base + idx2] << 2) |
	  (v[base + idx3] << 3);
	
	if ( 0 ) {
	  cout << "x1 = " << x1
	       << ", x2 = " << x2
	       << ", x3 = " << x3
	       << ", x4 = " << x4
	       << ", x5 = " << x5
	       << endl;
	  cout << "pat000 = ";
	  dump_pat(cout, 2, pat[0]);
	  cout << endl;
	  cout << "pat001 = ";
	  dump_pat(cout, 2, pat[1]);
	  cout << endl;
	  cout << "pat010 = ";
	  dump_pat(cout, 2, pat[2]);
	  cout << endl;
	  cout << "pat011 = ";
	  dump_pat(cout, 2, pat[3]);
	  cout << endl;
	  cout << "pat100 = ";
	  dump_pat(cout, 2, pat[4]);
	  cout << endl;
	  cout << "pat101 = ";
	  dump_pat(cout, 2, pat[5]);
	  cout << endl;
	  cout << "pat110 = ";
	  dump_pat(cout, 2, pat[6]);
	  cout << endl;
	  cout << "pat111 = ";
	  dump_pat(cout, 2, pat[7]);
	  cout << endl;
	}
 
      
	ymuint32 pata = pat[0];
	size_t groupa = 1U;
	ymuint32 patb = 0x0U;
	size_t groupb = 0U;
	size_t groupc = 0U;
	for (size_t i = 1; i < 8; ++ i) {
	  if ( pat[i] == pata ) {
	    groupa |= (1U << i);
	  }
	  else if ( groupb == 0U || pat[i] == patb ) {
	    patb = pat[i];
	    groupb |= (1U << i);
	  }
	  else {
	    groupc |= (1U << i);
	  }
	}
	if ( groupc == 0U ) {
	  assert_cond( groupa + groupb == 255U, __FILE__, __LINE__);
	  // コファクターのパタンが2種類からなる．

	  AigTemplate tmp;
	  bool stat1 = search_aigtable(3, groupb, tmp);
	  assert_cond(stat1, __FILE__, __LINE__);
	  vector<AigTemplate> tmap1;
	  AigTemplate lit1;
	  lit1.set_literal(x1, false);
	  tmap1.push_back(lit1);
	  AigTemplate lit2;
	  lit2.set_literal(x2, false);
	  tmap1.push_back(lit2);
	  AigTemplate lit3;
	  lit3.set_literal(x3, false);
	  tmap1.push_back(lit3);
	  AigTemplate g;
	  g.set_compose(tmp, tmap1);

	  ymuint32 pat1 = pata | (patb << 4);
	  bool stat2 = search_aigtable(3, pat1, tmp);
	  assert_cond(stat2, __FILE__, __LINE__);
	  vector<AigTemplate> tmap2;
	  AigTemplate lit4;
	  lit4.set_literal(x4, false);
	  tmap2.push_back(lit4);
	  AigTemplate lit5;
	  lit5.set_literal(x5, false);
	  tmap2.push_back(lit5);
	  tmap2.push_back(g);
	  templ.set_compose(tmp, tmap2);
	  return true;
	}
      }
    }
  }
  return false;
}

bool
try_ordecomp(size_t ni,
	     ymuint32 pat,
	     int limit,
	     AigTemplate& templ)
{
  if ( limit <= 0 ) {
    return false;
  }
  
  size_t nip = (1U << ni);
  
  // 1の位置を pos_array に入れる．
  // ただし最初の要素だけ pos0 に入れる．
  size_t pos0 = 0;
  vector<size_t> pos_array;
  bool first = true;
  for (size_t pos = 0; pos < nip; ++ pos) {
    if ( pat & (1U << pos) ) {
      if ( first ) {
	pos0 = pos;
	first = false;
      }
      else {
	pos_array.push_back(pos);
      }
    }
  }
  size_t n = pos_array.size();
  assert_cond(n > 0, __FILE__, __LINE__);
  
  size_t np = 1U << n;
  for (size_t p = 0U; p < np; ++ p) {
    ymuint32 pat_a = (1U << pos0);
    for (size_t i = 0; i < n; ++ i) {
      if ( p & (1U << i) ) {
	pat_a |= (1U << pos_array[i]);
      }
    }
    if ( pat_a == pat ) continue;
    AigTemplate tmp1;
    bool stat1 = search_aigtable(ni, pat_a, tmp1);
    if ( !stat1 || tmp1.cost() >= limit - 1) continue;
    
    for (size_t q = 1U; q < np; ++ q) {
      ymuint32 pat_b = 0U;
      for (size_t i = 0; i < n; ++ i) {
	if ( q & (1U << i) ) {
	  pat_b |= (1U << pos_array[i]);
	}
      }
      if ( (pat_a | pat_b) != pat ) continue;
      AigTemplate tmp2;
      bool stat2 = search_aigtable(ni, pat_b, tmp2);
      if ( !stat2 || tmp2.cost() >= limit - 1) continue;
      
      AigTemplate tmp;
      tmp.set_or(tmp1, false, tmp2, false);
      if ( tmp.cost() <= limit ) {
	templ = tmp;
	return true;
      }
    }
  }
  
  return false;
}

bool
try_xordecomp(size_t ni,
	      ymuint32 pat,
	      int limit,
	      AigTemplate& templ)
{
  if ( limit <= 0 ) {
    return false;
  }
  
  size_t nip = (1U << ni);
  
  // 1の位置を pos_array に入れる．
  // ただし最初の要素だけ pos0 に入れる．
  size_t pos0 = 0;
  vector<size_t> pos_array;
  bool first = true;
  for (size_t pos = 0; pos < nip; ++ pos) {
    if ( pat & (1U << pos) ) {
      if ( first ) {
	pos0 = pos;
	first = false;
      }
      else {
	pos_array.push_back(pos);
      }
    }
  }
  size_t n = pos_array.size();
  assert_cond(n > 0, __FILE__, __LINE__);
  
  for (size_t p = 0U; p < nip; ++ p) {
    ymuint32 pat_a = (1U << pos0) | p;
    if ( pat_a == pat ) continue;
    AigTemplate tmp1;
    bool stat1 = search_aigtable(ni, pat_a, tmp1);
    if ( !stat1 || tmp1.cost() >= limit - 1 ) continue;
    
    ymuint32 pat_b = pat ^ pat_a;
    AigTemplate tmp2;
    bool stat2 = search_aigtable(ni, pat_b, tmp2);
    if ( !stat2 || tmp2.cost() >= limit - 1 ) continue;
      
    AigTemplate tmp;
    tmp.set_xor(tmp1, false, tmp2, false);
    if ( tmp.cost() <= limit ) {
      templ = tmp;
      return true;
    }
  }
  
  return false;
}

bool
optaig(size_t ni,
       ymuint32 pat,
       int limit,
       AigTemplate& templ)
{
  // OR 分解を試す．
  bool stat1 = try_ordecomp(ni, pat, limit, templ);
  if ( stat1 ) {
    return true;
  }
  
  // XOR 分解を試す．
  bool stat2 = try_xordecomp(ni, pat, limit, templ);
  if ( stat2 ) {
    return true;
  }
  
  // AND 分解を試す．
  ymuint32 pat_n = pat ^ ((1U << (1U << ni)) - 1);
  AigTemplate tmp1;
  bool stat3 = try_ordecomp(ni, pat_n, limit, tmp1);
  if ( stat3 ) {
    templ.set_dual(tmp1);
    return true;
  }
  
  return false;
}

void
verify(size_t ni,
       ymuint32 pat,
       const AigTemplate& templ)
{
  vector<ymulong> ivals(ni);
  size_t nip = 1UL << ni;
  for (size_t i = 0; i < ni; ++ i) {
    ymulong ipat = 0UL;
    for (size_t p = 0; p < nip; ++ p) {
      if ( p & (1UL << i) ) {
	ipat |= (1UL << p);
      }
    }
    ivals[i] = ipat;
  }
  ymulong tpat = templ.eval(ivals) & ((1UL << nip) - 1);
  if ( pat != tpat ) {
    cout << "Error" << endl;
    cout << "pat  = ";
    if ( ni == 3 ) {
      dump_pat(cout, 3, pat);
    }
    else if ( ni == 4 ) {
      dump_pat(cout, 4, pat);
    }
    else if ( ni == 5 ) {
      dump_pat(cout, 5, pat);
    }
    else {
      cout << hex << pat << dec;
    }
    cout << endl;
    cout << hex << pat << dec << endl;
    cout << "tpat = ";
    if ( ni == 3 ) {
      dump_pat(cout, 3, tpat);
    }
    else if ( ni == 4 ) {
      dump_pat(cout, 4, tpat);
    }
    else if ( ni == 5 ) {
      dump_pat(cout, 5, tpat);
    }
    else {
      cout << hex << tpat << dec;
    }
    cout << endl;
    templ.dump(cout);
  }
}

// aig2table を作る．
void
init_table2()
{
  AigTemplate a;
  a.set_literal(0, false);

  AigTemplate b;
  b.set_literal(1, false);
  
  // 0: 0000 : 定数0
  aig2table[0].set_const0();

  // 10: 0101 : a
  aig2table[10].set_literal(0, false);

  // 5: 1010 : a'
  aig2table[5].set_literal(0, true);

  // 12: 0011 : b
  aig2table[12].set_literal(1, false);

  // 3: 1100 : b'
  aig2table[3].set_literal(1, true);

  // 1: 1000 : a'b'
  aig2table[1].set_and(a, true, b, true);

  // 2: 0100 : ab'
  aig2table[2].set_and(a, false, b, true);

  // 4: 0010 : a'b
  aig2table[4].set_and(a, true, b, false);

  // 6: 0110 : ab' + a'b
  aig2table[6].set_xor(a, false, b, false);

  // 7: 1110 : a' + b'
  aig2table[7].set_or(a, true, b, true);

  // 8: 0001 : ab
  aig2table[8].set_and(a, false, b, false);

  // 9: 1001 : a'b' + ab
  aig2table[9].set_xor(a, false, b, true);

  // 11: 1101 : a + b'
  aig2table[11].set_or(a, false, b, true);

  // 13: 1011 : a' + b
  aig2table[13].set_or(a, true, b, false);

  // 14: 0111 : a + b
  aig2table[14].set_or(a, false, b, false);

  // 15: 1111 : 定数1
  aig2table[15].set_const1();
  
  for (size_t id = 0; id < 16; ++ id) {
    ostringstream buf;
    buf << "aig2table[" << id << "]";
    aig2table[id].dump_code(cout, buf.str());
  }
}

// aig3table を作る．
void
init_table3()
{
  size_t n_count = 0;
  for (size_t id = 0; ; ++ id) {
    ymuint32 pat = npn3rep[id];
    if ( pat == 0xFFU ) {
      break;
    }
    bool stat = decomp3(pat, aig3table[id]);
    if ( stat ) {
      // 見つかった．
#if 0
      cout << "#" << n_count << endl;
      cout << "pat  = " << hex << pat << dec << endl;
      aig3table[id].dump(cout);
      cout << endl;
#endif
      verify(3, pat, aig3table[id]);
      ++ n_count;
      continue;
    }
  }
  
  for (int limit = 3; limit <= 11; ++ limit) {
    for (size_t id = 0; ; ++ id) {
      ymuint32 pat = npn3rep[id];
      if ( pat == 0xFFU ) {
	break;
      }
      if ( aig3table[id].set() ) continue;

      bool stat = optaig(3, pat, limit, aig3table[id]);
      if ( stat ) {
	// 見つかった．
#if 0
	cout << "#" << n_count << endl;
	cout << "pat  = " << hex << pat << dec << endl;
	aig3table[id].dump(cout);
	cout << endl;
#endif
	verify(3, pat, aig3table[id]);
	++ n_count;
	continue;
      }
    }
  }

  for (ymuint32 pat = 0U; pat < 256U; ++ pat) {
    AigTemplate templ;
    bool stat = search_aigtable(3, pat, templ);
    assert_cond(stat, __FILE__, __LINE__);
    ostringstream buf;
    buf << "aig3table[" << pat << "]";
    templ.dump_code(cout, buf.str());
  }
}

// aig4table を作る．
void
init_table4()
{
  size_t n_count = 0;
  size_t c_total = 0;
  for (size_t id = 0; ; ++ id) {
    ymuint32 pat = npn4rep[id];
    if ( pat == 0xFFFFU ) {
      break;
    }
    npn4map.insert(make_pair(pat, id));
    bool stat = decomp4(pat, aig4table[id]);
    if ( stat ) {
      // 見つかった．
#if 0
      cout << "#" << n_count << endl;
      cout << "pat  = ";
      dump_pat(cout, 4, pat);
      cout << endl;
      aig4table[id].dump(cout);
      cout << endl;
#endif
      verify(4, pat, aig4table[id]);
      ++ n_count;
      c_total += aig4table[id].cost();
      continue;
    }
  }
  
  for (int limit = 4; limit <= 31; ++ limit) {
    for (size_t id = 0; ; ++ id) {
      ymuint32 pat = npn4rep[id];
      if ( pat == 0xFFFFU ) {
	break;
      }
      if ( aig4table[id].set() ) continue;

      bool stat = optaig(4, pat, limit, aig4table[id]);
      if ( stat ) {
	// 見つかった．
#if 0
	cout << "#" << n_count << endl;
	cout << "pat  = ";
	dump_pat(cout, 4, pat);
	cout << endl;
	aig4table[id].dump(cout);
	cout << endl;
#endif
	verify(4, pat, aig4table[id]);
	++ n_count;
	c_total += aig4table[id].cost();
	continue;
      }

      stat = ndecomp4(pat, limit, aig4table[id]);
      if ( stat ) {
	// 見つかった．
#if 0
	cout << "#" << n_count << endl;
	cout << "pat  = ";
	dump_pat(cout, 4, pat);
	cout << endl;
	aig4table[id].dump(cout);
	cout << endl;
#endif
	verify(4, pat, aig4table[id]);
	++ n_count;
	c_total += aig4table[id].cost();
	continue;
      }
    }
  }
  for (size_t id = 0; ; ++ id) {
    ymuint32 pat = npn4rep[id];
    if ( pat == 0xFFFFU ) {
      break;
    }
    ostringstream buf;
    buf << "aig4table[" << id << "]";
    aig4table[id].dump_code(cout, buf.str());
  }
  for (size_t id = 0; ; ++ id) {
    ymuint32 pat = npn4rep[id];
    if ( pat == 0xFFFFU ) {
      break;
    }
    cout << "  npn4map.insert(make_pair(0x" << hex << pat << dec << "U, "
	 << id << "));" << endl;
  }
}

// aig5table を作る．
void
init_table5()
{
  size_t n_count = 0;
  size_t c_total = 0;
  for (size_t id = 0; ; ++ id) {
    ymuint32 pat = npn5rep[id];
    npn5map.insert(make_pair(pat, id));
    if ( pat == 0xFFFFFFFFU ) {
      break;
    }
    bool stat = decomp5(pat, aig5table[id]);
    if ( !stat ) {
      // 見つからなかった．
      continue;
    }
#if 1
    cout << "#" << n_count << endl;
    cout << "pat  = ";
    dump_pat(cout, 5, pat);
    cout << endl;
    aig5table[id].dump(cout);
    cout << endl;
#endif
    verify(5, pat, aig5table[id]);
    ++ n_count;
    c_total += aig5table[id].cost();
  }

#if 0
  for (int limit = 5; limit <= 80; ++ limit) {
    for (size_t id = 0; ; ++ id) {
      ymuint32 pat = npn5rep[id];
      if ( pat == 0xFFFFU ) {
	break;
      }
      if ( aig5table[id].set() ) continue;

      bool stat = optaig(5, pat, limit, aig5table[id]);
      if ( !stat ) {
	// 見つからなかった．
	continue;
      }
#if 1
      cout << "#" << n_count << endl;
      cout << "pat  = ";
      dump_pat(cout, 5, pat);
      cout << endl;
      aig5table[id].dump(cout);
      cout << endl;
#endif
      verify(5, pat, aig5table[id]);
      ++ n_count;
      c_total += aig5table[id].cost();
    }
  }
#endif
  cout << "Total cost = " << c_total << endl;
}

void
test3()
{
  for (ymuint32 pat = 0U; pat < 256; ++ pat) {
    AigTemplate templ;
    bool stat = search_aigtable(3, pat, templ);
    assert_cond( stat, __FILE__, __LINE__);
    cout << "pat  = ";
    dump_pat(cout, 3, pat);
    cout << endl;
    templ.dump(cout);
    cout << endl;
  }
}

END_NAMESPACE_YM


int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm;

  init_table2();

  init_table3();
  
  init_table4();
  
  //init_table5();
  
  //test3();
  
  return 0;
}
