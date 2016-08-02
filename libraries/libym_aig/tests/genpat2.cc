/// @file libym_aig/tests/genpat.cc
/// @brief N 入力関数を実現するパタンを生成するプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: genpat2.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"
#include "GpNode.h"
//#include <ym_npn/TvFunc.h>
//#include <ym_npn/NpnMgr.h>
//#include <ym_npn/NpnMap.h>


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
check_fv(GpNode* node,
	 ymuint32 fv)
{
  ymuint32 npat = node->pat();
  if ( npat & 1U ) {
    npat = ~npat;
  }
  if ( npat == fv ) {
    return true;
  }
  if ( node->is_input() ) {
    return false;
  }
  return check_fv(node->fanin0(), fv) || check_fv(node->fanin1(), fv);
}

bool
check_fv2_sub(GpNode* node1,
	      GpNode* node2,
	      ymuint32 fv,
	      ymuint32 mask)
{
  for (size_t pp = 0; pp < 4; ++ pp) {
    ymuint32 tmp1 = node1->pat();
    if ( pp & 1U ) {
      tmp1 = ~tmp1;
    }
    ymuint32 tmp2 = node2->pat();
    if ( pp & 2U ) {
      tmp2 = ~tmp2;
    }
    ymuint32 tmp = tmp1 & tmp2 & mask;
    if ( tmp & 1U ) {
      tmp = ~tmp & mask;
    }
    if ( tmp == fv ) {
#if 0
      cout << "check_fv2: " << hex << fv << dec << endl;
      dump_gp(node1);
      cout << endl;
      dump_gp(node2);
      cout << endl;
#endif
      return true;
    }
  }
  if ( node2->is_and() ) {
    if ( check_fv2_sub(node1, node2->fanin0(), fv, mask) ) {
      return true;
    }
    if ( check_fv2_sub(node1, node2->fanin1(), fv, mask) ) {
      return true;
    }
    return false;
  }
  return false;
}

bool
check_fv2(GpNode* node1,
	  GpNode* node2,
	  ymuint32 fv,
	  ymuint32 mask,
	  bool first)
{
  if ( !first ) {
    for (size_t pp = 0; pp < 4; ++ pp) {
      ymuint32 tmp1 = node1->pat();
      if ( pp & 1U ) {
	tmp1 = ~tmp1;
      }
      ymuint32 tmp2 = node2->pat();
      if ( pp & 2U ) {
	tmp2 = ~tmp2;
      }
      ymuint32 tmp = tmp1 & tmp2 & mask;
      if ( tmp & 1U ) {
	tmp = ~tmp & mask;
      }
      if ( tmp == fv ) {
#if 0
	cout << "check_fv2: " << hex << fv << dec << endl;
	dump_gp(node1);
	cout << endl;
	dump_gp(node2);
	cout << endl;
#endif
	return true;
      }
    }
  }
  
  if ( node1->is_and() ) {
    if ( check_fv2(node1->fanin0(), node2, fv, mask, false) ) {
      return true;
    }
    if ( check_fv2(node1->fanin1(), node2, fv, mask, false) ) {
      return true;
    }
  }
  if ( node2->is_and() ) {
    if ( check_fv2_sub(node1, node2->fanin0(), fv, mask) ) {
      return true;
    }
    if ( check_fv2_sub(node1, node2->fanin1(), fv, mask) ) {
      return true;
    }
  }
  return false;
}

// パタンを表すデータ構造
struct Pat
{
  Pat(ymuint32 f0,
      ymuint32 f1,
      ymuint32 pols) :
    mF0(f0),
    mF1(f1),
    mPols(pols)
  {
  }
  
  ymuint32 mF0;
  ymuint32 mF1;
  ymuint32 mPols;
};


// 一つの関数に対するパタンリスト
struct PatList
{
  // 最小サイズ
  ymuint32 mMinSize;

  // パタンリスト
  vector<Pat> mList;
  
  PatList() :
    mMinSize(0xffffffff)
  {
  }
  
  // 追加する．
  bool
  push_back(ymuint32 size,
	    ymuint32 f0,
	    ymuint32 f1,
	    ymuint32 pols)
  {
    if ( mMinSize > size ) {
      mMinSize = size;
      mList.clear();
    }
    if ( mMinSize == size ) {
      mList.push_back(Pat(f0, f1, pols));
      return true;
    }
    return false;
  }
  
};

// 非冗長なパタンを全列挙する．
void
genpat(ymuint ni)
{
  assert_cond(ni <= 5, __FILE__, __LINE__);
  
  cout << endl;
  cout << "genpat(" << ni << ")" << endl;
  
  // マスク
  ymuint np = 1U << ni;
  ymuint nf = 1U << np;
  ymuint32 mask = nf - 1U;

  // 関数ベクタをキーとしてパタンリストを保持する配列
  vector<PatList> func_table(nf);

  // 関数を保持する配列
  vector<ymuint32> func_list;
  
  // レベル0のパタンを作る．
  for (ymuint i = 0; i < ni; ++ i) {
    ymuint32 fv;
    switch ( i ) {
    case 0: fv = 0xaaaaaaaa; break;
    case 1: fv = 0xcccccccc; break;
    case 2: fv = 0xf0f0f0f0; break;
    case 3: fv = 0xff00ff00; break;
    case 4: fv = 0xffff0000; break;
    default: assert_not_reached(__FILE__, __LINE__);
    }
    fv &= mask;
    assert_cond((fv & 1U) == 0, __FILE__, __LINE__);
    func_table[fv].push_back(0, 0, 0, 0);
    func_list.push_back(fv);
  }
  
  for (ymuint rpos = 0; rpos < func_list.size(); ++ rpos) {
    ymuint32 func1 = func_list[rpos];
    for (ymuint i =  0; i < rpos; ++ i) {
      ymuint32 func2 = func_list[i];
      // func1 と func2 を子供とするパタンを生成する．
      // ただしファンインの極性の組み合わせが4通りある．
      
      // サイズを計算
      ymuint32 size1 = func_table[func1].mMinSize;
      ymuint32 size2 = func_table[func2].mMinSize;
      ymuint32 size = size1 + size2 + 1;
      for (ymint pp = 0; pp < 4; ++ pp) {
	ymuint32 fv1 = func1;
	bool inv1 = false;
	if ( pp & 1U ) {
	  fv1 = ~fv1;
	  inv1 = true;
	}
	ymuint32 fv2 = func2;
	bool inv2 = false;
	if ( pp & 2U ) {
	  fv2 = ~fv2;
	  inv2 = true;
	}
	ymuint32 fv = fv1 & fv2 & mask;
	// 出力極性の正規化を行う．
	ymuint32 fv0 = fv;
	bool inv = false;
	if ( fv & 1U ) {
	  fv0 = ~fv & mask;
	  inv = true;
	}
	if ( fv0 == 0U ) {
	  // 定数は無視
	  continue;
	}
	if ( func_table[fv0].push_back(size, fv1, fv2, pp) ) {
	  func_list.push_back(fv0);
	  //cout << hex << func1 << " x " << func2 << " = " << fv0 << dec << endl;
	}
      }
    }
  }
  
  ymuint total_cost = 0;
  for (ymuint i = 2; i < nf; i += 2) {
    if ( func_table[i].mList.empty() ) {
      cout << "error for function#" << i << endl;
      exit(1);
    }
    total_cost += func_table[i].mMinSize;
  }
  cout << "total cost = " << total_cost << endl;
}

END_NAMESPACE_YM

int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm;
  
  genpat(2);
  
  genpat(3);
  
  genpat(4);
  
  return 0;
}
