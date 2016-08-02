/// @file libym_aig/tests/enum_pat3.cc
/// @brief 3入力関数のパタンを列挙するプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: enum_pat3.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_npn/TvFunc.h"
#include "ym_npn/NpnMgr.h"
#include "ym_npn/NpnMap.h"
#include "ym_aig/AigMgr.h"

#include "ym_sat/SatSolver.h"


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


class EnumPat
{
public:

  // コンストラクタ
  EnumPat();

  // デストラクタ
  ~EnumPat();


public:

  // pat を実現するパタンのリストを列挙する．
  AigHandle
  enum_pat2(ymuint32 pat,
	    const vector<ymuint32>& vmap);

  // pat を実現するパタンのリストを列挙する．
  // ただし，parent_list に含まれる関数を部分木に含んではいけない．
  bool
  enum_pat3(ymuint32 pat,
	    list<ymuint32>& parent_list,
	    list<AigHandle>& pat_list);

  // 分解などで2入力以下に落とせる関数の処理を行う．
  bool
  decomp3(ymuint32 pat,
	  list<AigHandle>& pat_list);

  // pat を2つの関数の AND で実現する．
  void
  try_anddecomp(ymuint32 pat,
		list<ymuint32>& parent_list,
		list<AigHandle>& pat_list);

  // pat を2つの関数の OR で実現する．
  void
  try_ordecomp(ymuint32 pat,
	       list<ymuint32>& parent_list,
	       list<AigHandle>& pat_list);

  // pat を2つの関数の OR で実現する．
  void
  try_xordecomp(ymuint32 pat,
		list<ymuint32>& parent_list,
		list<AigHandle>& pat_list);

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // AIG マネージャ
  AigMgr mAigMgr;

  // 入力のノード
  AigHandle mInput[3];
  
};


// ni 入力関数のパタンを出力する．
void
dump_pat(ostream& s,
	 ymuint32 ni,
	 ymuint32 pat)
{
  ymuint32 np = 1U << ni;
  for (ymuint32 i = 0; i < np; ++ i) {
    if ( pat & (1U << i) ) {
      s << "1";
    }
    else {
      s << "0";
    }
  }
}

#if 0
// pat を実現するパタンがあるか探す．
bool
search_aigtable(ymuint32 ni,
		ymuint32 pat,
		AigTemplate& templ)
{
  NpnMap cmap;
  ymuint32 pat0;
  {
    // pat に対応する真理値表を作る．
    // 結果は func に入る．
    ymuint32 np = 1U << ni;
    vector<int> vals(np);
    for (ymuint32 i = 0; i < np; ++ i) {
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
    for (ymuint32 i = 0; i < np; ++ i) {
      if ( func0.value(i) ) {
	pat0 |= (1U << i);
      }
    }
  }
  
  if ( ni == 3 ) {
    // pat0 のエントリを探す．
    ymuint32 id0;
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
      // tmp0 を cmap にしたがって変換したものを templ に入れる．
      NpnMap rmap = inverse(cmap);
      templ.set_copy(tmp0, rmap);
      return true;
    }
    return false;
  }
  
  assert_not_reached(__FILE__, __LINE__);
  return false;
}
#endif


// コンストラクタ
EnumPat::EnumPat()
{
  for (ymuint32 i = 0; i < 3; ++ i) {
    mInput[i] = mAigMgr.make_input();
  }
}

// デストラクタ
EnumPat::~EnumPat()
{
}

// pat を実現するパタンのリストを列挙する．
AigHandle
EnumPat::enum_pat2(ymuint32 pat,
		   const vector<ymuint32>& vmap)
{
  AigHandle input0 = mInput[vmap[0]];
  AigHandle input1 = mInput[vmap[1]];
  
  switch ( pat ) {
  case 0x0: return mAigMgr.make_zero();
  case 0x1: return mAigMgr.make_and(~input0, ~input1);
  case 0x2: return mAigMgr.make_and( input0, ~input1);
  case 0x3: return ~input1;
  case 0x4: return mAigMgr.make_and(~input0,  input1);
  case 0x5: return ~input0;
  case 0x6: return mAigMgr.make_xor( input0,  input1);
  case 0x7: return mAigMgr.make_or(~input0, ~input1);
  case 0x8: return mAigMgr.make_or( input0,  input1);
  case 0x9: return mAigMgr.make_xor( input0, ~input1);
  case 0xA: return input0;
  case 0xB: return mAigMgr.make_or(~input0,  input1);
  case 0xC: return input1;
  case 0xD: return mAigMgr.make_or( input0, ~input1);
  case 0xE: return mAigMgr.make_or( input0,  input1);
  case 0xF: return mAigMgr.make_one();
  default: break;
  }
  assert_not_reached(__FILE__, __LINE__);
  return input0;
}

// 分解などで2入力以下に落とせる関数の処理を行う．
bool
EnumPat::decomp3(ymuint32 pat,
		 list<AigHandle>& pat_list)
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
  
  for (ymuint32 i = 0; i < 3; ++ i) {
    if ( pat0[i] == pat1[i] ) {
      // i 番めの変数と無関係
      vector<ymuint32> vmap;
      for (ymuint32 j = 0; j < 3; ++ j) {
	if ( j != i ) {
	  vmap.push_back(j);
	}
      }
      AigHandle pat = enum_pat2(pat0[i], vmap);
      pat_list.push_back(pat);
      return true;
    }
    
    if ( pat0[i] == 0x0U ) {
      if ( pat1[i] == 0xFU ) {
	// xi
	pat_list.push_back(mInput[i]);
	return true;
      }
      // xi & f_xi
      vector<ymuint32> vmap;
      for (ymuint32 j = 0; j < 3; ++ j) {
	if ( j != i ) {
	  vmap.push_back(j);
	}
      }
      AigHandle tmp = enum_pat2(pat1[i], vmap);
      AigHandle pat = mAigMgr.make_and(mInput[i], tmp);
      pat_list.push_back(pat);
      return true;
    }
    
    if ( pat0[i] == 0xFU ) {
      if ( pat1[i] == 0x0U ) {
	// xi'
	pat_list.push_back(~mInput[i]);
	return true;
      }
      // xi' | f_xi
      vector<ymuint32> vmap;
      for (ymuint32 j = 0; j < 3; ++ j) {
	if ( j != i ) {
	  vmap.push_back(j);
	}
      }
      AigHandle tmp = enum_pat2(pat1[i], vmap);
      AigHandle pat = mAigMgr.make_or(~mInput[i], tmp);
      pat_list.push_back(pat);
      return true;
    }
    
    if ( pat1[i] == 0x0U ) {
      // xi' & f_xi'
      vector<ymuint32> vmap;
      for (ymuint32 j = 0; j < 3; ++ j) {
	if ( j != i ) {
	  vmap.push_back(j);
	}
      }
      AigHandle tmp = enum_pat2(pat0[i], vmap);
      AigHandle pat = mAigMgr.make_and(~mInput[i], tmp);
      pat_list.push_back(pat);
      return true;
    }
    
    if ( pat1[i] == 0xFU ) {
      // xi | f_xi'
      vector<ymuint32> vmap;
      for (ymuint32 j = 0; j < 3; ++ j) {
	if ( j != i ) {
	  vmap.push_back(j);
	}
      }
      AigHandle tmp = enum_pat2(pat0[i], vmap);
      AigHandle pat = mAigMgr.make_or(mInput[i], tmp);
      pat_list.push_back(pat);
      return true;
    }
    
    if ( pat0[i] == (pat1[i] ^ 0xFU) ) {
      // xi xor f_xi'
      vector<ymuint32> vmap;
      for (ymuint32 j = 0; j < 3; ++ j) {
	if ( j != i ) {
	  vmap.push_back(j);
	}
      }
      AigHandle tmp = enum_pat2(pat0[i], vmap);
      AigHandle pat = mAigMgr.make_xor(mInput[0], tmp);
      pat_list.push_back(pat);
      return true;
    }
  }

  return false;
}

// pat を2つの関数の AND で実現する．
void
EnumPat::try_anddecomp(ymuint32 pat,
		       list<ymuint32>& parent_list,
		       list<AigHandle>& pat_list)
{
  ymuint32 nip = (1U << 3);
  
  // 0の位置を pos_array に入れる．
  // ただし最初の要素だけ pos0 に入れる．
  ymuint32 pos0 = 0;
  vector<ymuint32> pos_array;
  pos_array.reserve(nip);
  bool first = true;
  for (ymuint32 pos = 0; pos < nip; ++ pos) {
    if ( (pat & (1U << pos)) == 0U ) {
      if ( first ) {
	pos0 = pos;
	first = false;
      }
      else {
	pos_array.push_back(pos);
      }
    }
  }
  ymuint32 n = pos_array.size();
  assert_cond(n > 0, __FILE__, __LINE__);
  
  ymuint32 np = 1U << n;
  for (ymuint32 p = 0U; p < np; ++ p) {
    ymuint32 pat_a = (1U << pos0);
    for (ymuint32 i = 0; i < n; ++ i) {
      if ( p & (1U << i) ) {
	pat_a |= (1U << pos_array[i]);
      }
    }
    pat_a ^= 0xFF;
    
    // pat_a が parent_list に含まれていたらスキップする．
    bool found = false;
    for (list<ymuint32>::iterator p = parent_list.begin();
	 p != parent_list.end(); ++ p) {
      if ( pat_a == *p ) {
	found = true;
	break;
      }
    }
    if ( found ) continue;
    
    list<AigHandle> pat_a_list;
    bool stat1 = enum_pat3(pat_a, parent_list, pat_a_list);
    assert_cond(stat1, __FILE__, __LINE__);
    
    for (ymuint32 q = 1U; q < np; ++ q) {
      ymuint32 pat_b = 0U;
      for (ymuint32 i = 0; i < n; ++ i) {
	if ( q & (1U << i) ) {
	  pat_b |= (1U << pos_array[i]);
	}
      }
      pat_b ^= 0xFF;
      
      if ( (pat_a & pat_b) != pat ) continue;
      
      // pat_b が parent_list に含まれていたらスキップする．
      bool found = false;
      for (list<ymuint32>::iterator p = parent_list.begin();
	   p != parent_list.end(); ++ p) {
	if ( pat_b == *p ) {
	  found = true;
	  break;
	}
      }
      if ( found ) continue;
      
      list<AigHandle> pat_b_list;
      bool stat2 = enum_pat3(pat_b, parent_list, pat_b_list);
      assert_cond(stat2, __FILE__, __LINE__);
      
      for (list<AigHandle>::iterator p = pat_a_list.begin();
	   p != pat_a_list.end(); ++ p) {
	AigHandle handle1 = *p;
	for (list<AigHandle>::iterator q = pat_b_list.begin();
	     q != pat_b_list.end(); ++ q) {
	  AigHandle handle2 = *q;
	  AigHandle pat = mAigMgr.make_and(handle1, handle2);
	  pat_list.push_back(pat);
	}
      }
    }
  }
}

// pat を2つの関数の OR で実現する．
void
EnumPat::try_ordecomp(ymuint32 pat,
		      list<ymuint32>& parent_list,
		      list<AigHandle>& pat_list)
{
  ymuint32 nip = (1U << 3);
  
  // 1の位置を pos_array に入れる．
  // ただし最初の要素だけ pos0 に入れる．
  ymuint32 pos0 = 0;
  vector<ymuint32> pos_array;
  pos_array.reserve(nip);
  bool first = true;
  for (ymuint32 pos = 0; pos < nip; ++ pos) {
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
  ymuint32 n = pos_array.size();
  assert_cond(n > 0, __FILE__, __LINE__);
  
  ymuint32 np = 1U << n;
  for (ymuint32 p = 0U; p < np; ++ p) {
    ymuint32 pat_a = (1U << pos0);
    for (ymuint32 i = 0; i < n; ++ i) {
      if ( p & (1U << i) ) {
	pat_a |= (1U << pos_array[i]);
      }
    }
    // pat_a が parent_list に含まれていたらスキップする．
    bool found = false;
    for (list<ymuint32>::iterator p = parent_list.begin();
	 p != parent_list.end(); ++ p) {
      if ( pat_a == *p ) {
	found = true;
	break;
      }
    }
    if ( found ) continue;
    
    list<AigHandle> pat_a_list;
    bool stat1 = enum_pat3(pat_a, parent_list, pat_a_list);
    assert_cond(stat1, __FILE__, __LINE__);
    
    for (ymuint32 q = 1U; q < np; ++ q) {
      ymuint32 pat_b = 0U;
      for (ymuint32 i = 0; i < n; ++ i) {
	if ( q & (1U << i) ) {
	  pat_b |= (1U << pos_array[i]);
	}
      }
      if ( (pat_a | pat_b) != pat ) continue;
      
      // pat_b が parent_list に含まれていたらスキップする．
      bool found = false;
      for (list<ymuint32>::iterator p = parent_list.begin();
	   p != parent_list.end(); ++ p) {
	if ( pat_b == *p ) {
	  found = true;
	  break;
	}
      }
      if ( found ) continue;
      
      list<AigHandle> pat_b_list;
      bool stat2 = enum_pat3(pat_b, parent_list, pat_b_list);
      assert_cond(stat2, __FILE__, __LINE__);
      
      for (list<AigHandle>::iterator p = pat_a_list.begin();
	   p != pat_a_list.end(); ++ p) {
	AigHandle handle1 = *p;
	for (list<AigHandle>::iterator q = pat_b_list.begin();
	     q != pat_b_list.end(); ++ q) {
	  AigHandle handle2 = *q;
	  AigHandle pat = mAigMgr.make_or(handle1, handle2);
	  pat_list.push_back(pat);
	}
      }
    }
  }
}

void
EnumPat::try_xordecomp(ymuint32 pat,
		       list<ymuint32>& parent_list,
		       list<AigHandle>& pat_list)
{
  ymuint32 nip = (1U << 3);
  
  // 1の位置を pos_array に入れる．
  // ただし最初の要素だけ pos0 に入れる．
  ymuint32 pos0 = 0;
  vector<ymuint32> pos_array;
  bool first = true;
  for (ymuint32 pos = 0; pos < nip; ++ pos) {
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
  ymuint32 n = pos_array.size();
  assert_cond(n > 0, __FILE__, __LINE__);
  
  for (ymuint32 p = 0U; p < nip; ++ p) {
    ymuint32 pat_a = (1U << pos0) | p;
    
    // pat_a が parent_list に含まれていたらスキップする．
    bool found = false;
    for (list<ymuint32>::iterator p = parent_list.begin();
	 p != parent_list.end(); ++ p) {
      if ( pat_a == *p ) {
	found = true;
	break;
      }
    }
    if ( found ) continue;
    
    list<AigHandle> pat_a_list;
    bool stat1 = enum_pat3(pat_a, parent_list, pat_a_list);
    assert_cond(stat1, __FILE__, __LINE__);
    
    ymuint32 pat_b = pat ^ pat_a;
      
    // pat_b が parent_list に含まれていたらスキップする．
    found = false;
    for (list<ymuint32>::iterator p = parent_list.begin();
	 p != parent_list.end(); ++ p) {
      if ( pat_b == *p ) {
	found = true;
	break;
      }
    }
    if ( found ) continue;
      
    list<AigHandle> pat_b_list;
    bool stat2 = enum_pat3(pat_b, parent_list, pat_b_list);
    assert_cond(stat2, __FILE__, __LINE__);
      
    for (list<AigHandle>::iterator p = pat_a_list.begin();
	 p != pat_a_list.end(); ++ p) {
      AigHandle handle1 = *p;
      for (list<AigHandle>::iterator q = pat_b_list.begin();
	   q != pat_b_list.end(); ++ q) {
	AigHandle handle2 = *q;
	AigHandle pat = mAigMgr.make_or(handle1, handle2);
	pat_list.push_back(pat);
      }
    }
  }
}

bool
EnumPat::enum_pat3(ymuint32 pat,
		   list<ymuint32>& parent_list,
		   list<AigHandle>& pat_list)
{
  bool stat = decomp3(pat, pat_list);
  if ( stat ) {
    // 見つかった．
    //verify(3, pat, aig3table[id]);
    return true;
  }
  
  parent_list.push_back(pat);
  
  // AND 分解を試す．
  try_anddecomp(pat, parent_list, pat_list);
  
  // OR 分解を試す．
  try_ordecomp(pat, parent_list, pat_list);
  
  // XOR 分解を試す．
  try_xordecomp(pat, parent_list, pat_list);
  
  parent_list.pop_back();
  
  return !pat_list.empty();
}

#if 0
void
verify(ymuint32 ni,
       ymuint32 pat,
       const AigTemplate& templ)
{
  vector<ymulong> ivals(ni);
  ymuint32 nip = 1UL << ni;
  for (ymuint32 i = 0; i < ni; ++ i) {
    ymulong ipat = 0UL;
    for (ymuint32 p = 0; p < nip; ++ p) {
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
#endif

// aig3table を作る．
void
init_table3()
{
  EnumPat enum_pat;
  
  ymuint32 n_count = 0;
  for (ymuint32 id = 0; ; ++ id) {
    ymuint32 pat = npn3rep[id];
    if ( pat == 0xFFU ) {
      break;
    }
    list<ymuint32> parent_list;
    list<AigHandle> pat_list;
    bool stat = enum_pat.enum_pat3(pat, parent_list, pat_list);
    if ( stat ) {
      // 見つかった．
#if 0
      cout << "#" << n_count << endl;
      cout << "pat  = " << hex << pat << dec << endl;
      aig3table[id].dump(cout);
      cout << endl;
#endif
      //verify(3, pat, aig3table[id]);
      ++ n_count;
      continue;
    }
  }
  
#if 0
  for (ymuint32 pat = 0U; pat < 256U; ++ pat) {
    AigTemplate templ;
    bool stat = search_aigtable(3, pat, templ);
    assert_cond(stat, __FILE__, __LINE__);
    ostringstream buf;
    buf << "aig3table[" << pat << "]";
    templ.dump_code(cout, buf.str());
  }
#endif
}

END_NAMESPACE_YM


int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm;

  init_table3();
  
  return 0;
}
