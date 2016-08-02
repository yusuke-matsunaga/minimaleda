
/// @file libym_npn/NpnRawSig.cc
/// @brief NpnRawSig の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: NpnRawSig.cc 1510 2008-06-30 05:11:10Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"
#include "ym_npn/TvFunc.h"

#include "NpnRawSig.h"
#include "NpnConf.h"


const int debug_normalize = 0x0001;
const int debug_none      = 0x0000;
const int debug_all       = 0xffff;

#ifdef YM_DEBUG
#define DEBUG_FLAG debug_all
#endif

#ifndef DEBUG_FLAG
#define DEBUG_FLAG debug_none
#endif

int debug = DEBUG_FLAG;


BEGIN_NAMESPACE_YM_NPN

//////////////////////////////////////////////////////////////////////
// NPN同値類を区別するためのシグネチャを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
NpnRawSig::NpnRawSig()
{
}

// デストラクタ
NpnRawSig::~NpnRawSig()
{
}

// @brief W0/W1 を用いて正規化する．
void
NpnRawSig::normalize(const TvFunc& func,
		     NpnConf& conf)
{
  mFunc = func;

  mNi = func.ni();

  // Walsh の0次と1次の係数を計算する．
  // 2次の係数はオンデマンドで計算する．
  mW0 = func.walsh_01(mW1);
  for (ymuint i = 0; i < mNi; ++ i) {
    for (ymuint j = 0; j < mNi; ++ j) {
      mW2flag[i * mNi + j] = 0;
    }
  }

  if ( debug & debug_normalize ) {
    cout << "Before normalize" << endl;
    cout << func << endl;
    dump_walsh(cout);
  }

  // mW0 が非負になるように出力極性の調整を行う．
  mOpol = 1;
  if ( walsh_0() < 0 ) {
    mOpol = -1;
    invert_all();
  }
  else if ( walsh_0() == 0 ) {
    mOpol = 0;
  }

  // Walsh の1次係数を計算し，極性の調整を行う．
  // 同時に等価入力クラスをつくる．
  mNc = 0;
  mIndepNum = 0;
  for (ymuint i = 0; i < mNi; ++ i) {
    mIcNum[i] = 2;
    mIcLink[i] = static_cast<ymuint>(-1);

    // i の1次係数を求める．
    int w1 = walsh_1(i);

    // w1 が非負になるように調整する．
    // w1 が 0 の時には実際のサポートかどうかも調べる．
    int ipol = 1;
    if ( w1 < 0 ) {
      w1 = -w1;
      ipol = -1;
      invert_input(i);
    }
    else if ( w1 == 0 ) {
      ipol = 0;
      bool stat = func.check_sup(i);
      if ( !stat ) {
	// この入力はサポートではなかった
	add_indep(i);
	// 意味はないけど極性は決まったものとみなす．
	mIpols[i] = 1;
	continue;
      }
    }
    mIpols[i] = ipol;

    // 等価な入力があるかどうか調べる．
    bool found = false;
    for (ymuint p = 0; p < mNc; ++ p) {
      ymuint pos1 = mIcRep[p];
      if ( w1 != walsh_1(pos1) ) {
	continue;
      }
      // 1次係数が等しい場合
      // 対称性のチェックを行う．
      tPol poldiff = (mIpols[pos1] * ipol == -1) ? kPolNega : kPolPosi;
      bool stat = func.check_sym(i, pos1, poldiff);
      if ( stat ) {
	// 対称だった
	found = true;
	if ( w1 == 0 && ic_num(pos1) == 1 ) {
	  // bi-symmetry かどうかチェックする
	  bool stat = func.check_sym(i, pos1, ~poldiff);
	  if ( stat ) {
	    set_bisym(pos1);
	  }
	}
	add_elem(pos1, i);
	break;
      }
      if ( w1 == 0 ) {
	// w1 == 0 の時には逆相での対称性もチェックする．
	// この場合，最初の要素の極性は常に kPolPosi のはず
	bool stat = func.check_sym(i, pos1, kPolNega);
	if ( stat ) {
	  // 逆相で対称だった．
	  found = true;
	  add_elem(pos1, i);
	  mIpols[i] = -1;
	  invert_input(i);
	  break;
	}
      }
    }
    if ( !found ) {
      // 対称な入力が見つからなかった時には新たな入力クラスをつくる．
      mIcRep[mNc] = i;
      ++ mNc;
    }
  }

  if ( mOpol == 0 ) {
    // もしも入力の極性がすべて決まっていれば
    // w2 の最大値と最小値の絶対値の大きい方の出力極性を選ぶ．
    // 等しいときには総和を正にする．
    int min = walsh_2(0, 0);
    int max = min;
    int sum = 0;
    bool valid = true;
    for (ymuint i = 0; i < mNi; ++ i) {
      if ( walsh_1(i) == 0 ) {
	valid = false;
	break;
      }
      for (ymuint j = 0; j < mNi; ++ j) {
	int w2 = walsh_2(i, j);
	if ( min > w2 ) {
	  min = w2;
	}
	if ( max < w2 ) {
	  max = w2;
	}
	sum += w2;
      }
    }
    if ( valid ) {
      if ( -min > max ) {
	mOpol = -1;
	// w1 を正にするには入力をすべて反転する必要がある．
	for (ymuint i = 0; i < mNi; ++ i) {
	  if ( walsh_1(i) != 0 ) {
	    mIpols[i] *= -1;
	  }
	}
	// と同時に w2 も反転させる．
	invert_all_w2();
      }
      else if ( -min < max ) {
	mOpol = 1;
      }
      else { // -min == max
	if ( sum < 0 ) {
	  mOpol = -1;
	  // w1 を正にするには入力をすべて反転する必要がある．
	  for (ymuint i = 0; i < mNi; ++ i) {
	    if ( walsh_1(i) != 0 ) {
	      mIpols[i] *= -1;
	    }
	  }
	  // と同時に w2 も反転させる．
	  invert_all_w2();
	}
	else if ( sum > 0 ) {
	  mOpol = 1;
	}
      }
    }
  }

  if ( debug & debug_normalize ) {
    cout << "After normalize" << endl;
    dump_walsh(cout);
    dump_pols(cout);
  }

  if ( mNc > 0 ) {
    for (ymuint i = 0; i < mNc; ++ i) {
      ymuint pos = mIcRep[i];
      int ipol1 = (walsh_1(pos) == 0) ? 0 : 1;
      conf.add_ic_rep(pos, ipol1);
    }
    // 最初はひとかたまりのグループにしておく．
    conf.add_ig(0);
  }
  if ( mOpol != 0 ) {
    conf.set_opol(1);
  }
  conf.set_sig(this);

}

// @brief すべてのシグネチャを反転させる．
void
NpnRawSig::invert_all()
{
  mW0 *= -1;
  for (ymuint i = 0; i < mNi; ++ i) {
    mW1[i] *= -1;
  }
  invert_all_w2();
}

// @brief 一つの入力に関するシグネチャを反転する．
void
NpnRawSig::invert_input(ymuint pos)
{
  mW1[pos] *= -1;
  for (ymuint i = 0; i < mNi; ++ i) {
    invert_w2(pos, i);
  }
}

// @brief 2次の係数のみを反転する．
void
NpnRawSig::invert_all_w2()
{
  for (ymuint i = 0; i < mNi; ++ i) {
    for (ymuint j = i + 1; j < mNi; ++ j) {
      invert_w2(i, j);
    }
  }
}

// @brief w2 の要素を反転する．
void
NpnRawSig::invert_w2(ymuint pos1,
		     ymuint pos2)
{
  if ( pos2 > pos1 ) {
    int tmp = pos1;
    pos1 = pos2;
    pos2 = tmp;
  }
  ymuint base = pos1 * ni() + pos2;
  if ( mW2flag[base] & 1 ) {
    mW2[base] *= -1;
  }
  else {
    mW2flag[base] ^= 2;
  }
}

// @brief 重み別 Walsh の 0次係数を得る．
int
NpnRawSig::walsh_w0(ymuint w,
		    tPol opol,
		    tPol ipol[]) const
{
  if ( mOpol == -1 ) {
    opol = ~opol;
  }
  ymuint32 ibits = 0UL;
  for (ymuint i = 0; i < ni(); ++ i) {
    tPol ip = ipol[i];
    if ( mIpols[i] == -1 ) {
      ip = ~ip;
    }
    if ( ip == kPolNega ) {
      ibits |= (1UL << i);
    }
  }
  return mFunc.walsh_w0(w, opol, ibits);
}

// @brief Walsh 係数を出力する．
void
NpnRawSig::dump_walsh(ostream& s) const
{
  s << "W0: " << walsh_0() << endl
    << "w1:";
  for (ymuint i = 0; i < ni(); ++ i) {
    s << " " << walsh_1(i);
  }
  s << endl;
  s << "W2:" << endl;
  for (ymuint i = 0; i < ni(); ++ i) {
    s << "   ";
    for (ymuint j = 0; j < ni(); ++ j) {
      s << " " << setw(4) << walsh_2(i, j);
    }
    s << endl;
  }
  s << endl;
}

// @brief 極性情報を出力する．
void
NpnRawSig::dump_pols(ostream& s) const
{
  s << "opol: ";
  if ( opol() == -1 ) {
    s << "N";
  }
  else if ( opol() == 1 ) {
    s << "P";
  }
  else {
    s << "-";
  }
  s << endl
    << "ipol:";
  for (ymuint i = 0; i < ni(); ++ i) {
    s << " ";
    if ( ipol(i) == -1 ) {
      s << "N";
    }
    else if ( ipol(i) == 1 ) {
      s << "P";
    }
    else {
      s << "-";
    }
  }
  s << endl;
}

END_NAMESPACE_YM_NPN
