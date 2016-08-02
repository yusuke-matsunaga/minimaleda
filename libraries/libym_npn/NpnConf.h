#ifndef LIBYM_NPN_NPNCONF_H
#define LIBYM_NPN_NPNCONF_H

/// @file libym_npn/NpnConf.h
/// @brief NpnConf のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: NpnConf.h 1508 2008-06-30 04:55:42Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#include <ym_npn/NpnMap.h>
#include "NpnRawSig.h"


BEGIN_NAMESPACE_YM_NPN

//////////////////////////////////////////////////////////////////////
/// @class NpnConf NpnConf.h "NpnConf.h"
/// @brief 1つのコンフィグレーションを表すクラス
/// 具体的には
///  - 出力の極性
///  - 入力の極性
///  - 入力の順列
/// を表す
/// ただし，上記の情報が未確定なものを含む可能性がある．
//////////////////////////////////////////////////////////////////////
class NpnConf
{
public:

  /// @brief 空のコンストラクタ
  /// @note 内容は不定
  NpnConf();

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  NpnConf(const NpnConf& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 自分自身への参照
  const NpnConf&
  operator=(const NpnConf& src);

  /// @brief 出力極性を固定するコピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  /// @param[in] pol 出力極性(-1/1)
  NpnConf(const NpnConf& src,
	  int pol);

  /// @brief グループ g 内の c というクラスを切り出す．
  /// @param[in] src コピー元のオブジェクト
  /// @param[in] g 切り出す対象のグループ番号
  /// @param[in] c 切り出す対象の等価入力クラス
  /// @note c のみを独立したグループとする．
  NpnConf(const NpnConf& src,
	  ymuint g,
	  ymuint c);

  /// @brief デストラクタ
  ~NpnConf();


public:

  /// @brief 完全な正規形になっているとき true を返す．
  /// @param[in] g0 調べ始める最初のグループ番号
  bool
  is_resolved(ymuint g0 = 0) const;

  /// @brief 出力極性が確定のときに true を返す．
  bool
  is_opol_fixed() const;

  /// @brief 出力極性の割り当ての取得
  /// @return 出力極性の割り当て状態
  ///  -   1 : 肯定
  ///  -  -1 : 否定
  int
  opol() const;

  /// @brief 入力極性の取得
  /// @param[in] pos 入力番号
  /// @return 指定された入力の極性
  ///  -  1 : 肯定
  ///  - -1 : 否定
  int
  ipol(ymuint pos) const;

  /// @brief 入力順序の取得
  /// @param[in] pos 入力位置
  /// @return pos 番めの入力番号
  ymuint
  iorder(ymuint pos) const;


public:

  /// @brief 等価入力クラス数の取得
  /// @return クラス数
  ymuint
  nc() const;

  /// @brief 等価入力クラスの取得
  /// @param[in] pos 入力クラス番号
  /// @return 指定された入力クラスの先頭番号＋極性
  pair<ymuint, int>
  input(ymuint pos) const;

  /// @brief 等価入力クラスの先頭番号の取得
  /// @param[in] pos 入力クラス番号
  /// @return 指定された入力クラスの先頭の入力番号インデックス
  ymuint
  ic_rep(ymuint pos) const;

  /// @brief 入力クラスの極性の割り当ての取得
  /// @param[in] pos 入力クラス番号
  /// @return pos に対応する入力の極性の割り当て状態
  ///  -  0 : 未定(肯定も否定もありうる)
  ///  -  1 : 肯定
  ///  -  2 : 否定
  int
  ic_pol(ymuint pos) const;

  /// @brief グループ数の取得
  /// @return グループ数
  ymuint
  ng() const;

  /// @brief グループの先頭の取得
  /// @param[in] pos グループ番号( < ng() )
  /// @return pos 番目のグループの先頭のクラス番号
  ymuint
  begin(ymuint pos) const;

  /// @brief グループの末尾の取得
  /// @param[in] pos グループ番号( < ng() )
  /// @return pos 番目のグループの末尾のクラス番号
  /// @note 末尾とは実際の最後の要素の次を指す．
  ymuint
  end(ymuint pos) const;

  /// @brief グループの要素数の取得
  /// @param[in] pos グループ番号( < ng() )
  /// @return pos 番めのグループの要素数
  ymuint
  gnum(ymuint pos) const;

  /// @brief 重み別 Walsh の 0次係数を返す．
  int
  walsh_w0(ymuint w) const;

  /// @brief Walsh の 2次係数を返す．
  int
  walsh_2(ymuint pos1,
	  ymuint pos2) const;

  /// @brief Walsh の 2次係数を返す (入力順序付き)．
  int
  walsh_2i(ymuint pos1,
	   ymuint pos2) const;

  /// @brief W2 を用いた大小比較関数
  bool
  w2gt(ymuint rep0,
       ymuint rep1,
       ymuint rep2) const;

  /// @brief W2 を用いた等価比較関数
  bool
  w2eq(ymuint rep0,
       ymuint rep1,
       ymuint rep2) const;

  /// @brief 内容を NpnMap にセットする．
  void
  set_map(NpnMap& map) const;


public:
  //////////////////////////////////////////////////////////////////////
  // NpnRawSig の関数に対するラッパ
  //////////////////////////////////////////////////////////////////////

  /// @brief 対象の関数を得る．
  const TvFunc&
  func() const;

  /// @brief 入力数を得る．
  ymuint
  ni() const;

  // 等価入力クラスの要素数を返す．
  ymuint
  ic_num(ymuint rep) const;

  /// @brief 独立(無関係)な入力クラスの先頭番号を返す．
  ymuint
  indep_rep() const;

  /// @brief 独立な入力クラスの要素数を返す．
  ymuint
  indep_num() const;

  // 等価入力クラスの pos の次の要素を返す．
  ymuint
  ic_link(ymuint pos) const;

  // 等価入力クラスの bisym マークを返す．
  bool
  bisym(ymuint rep) const;

  /// @brief rep1 が rep2 より大きければ true を返す．
  bool
  w1gt(ymuint rep1,
       ymuint rep2) const;

  /// @brief rep1 と rep2 が等しければ true を返す．
  bool
  w1eq(ymuint rep1,
       ymuint rep2) const;

  /// @brief Walsh 係数を出力する．
  void
  dump_walsh(ostream& s) const;


public:

  /// @brief シグネチャを設定する．
  void
  set_sig(const NpnRawSig* sig);

  /// @brief 出力極性の割り当ての設定
  /// @param[in] val 割り当て状態
  /// @sa opol
  void
  set_opol(int val);

  /// @brief 入力クラスの極性の割り当ての設定
  /// @param[in] pos 入力クラス番号
  /// @param[in] val 割り当て状態
  /// @sa ic_pol
  void
  set_ic_pol(ymuint pos,
	     int val);

  /// @brief 入力クラスを追加する．
  /// @param[in] pos 先頭の入力番号
  /// @param[in] pol 極性
  void
  add_ic_rep(ymuint pos,
	     int pol);

  /// @brief 入力グループの開始番号を追加する．
  /// @param[in] index 先頭の入力クラス番号
  void
  add_ig(ymuint index);

  /// @brief グループの細分化を行う．
  /// @param[in] g0 対象のグループ番号
  /// @param[in] gt 2つの入力クラスの大小比較関数オブジェクト
  /// @param[in] eq 2つの入力クラスの等価比較関数オブジェクト
  /// @return 増えたグループ数を返す．
  template <typename T1,
	    typename T2>
  ymuint
  refine(ymuint g0,
	 T1 gt,
	 T2 eq);


public:

  /// @brief 内容を出力する．
  void
  dump(ostream& s) const;


private:

  // コピーを行う．
  void
  copy(const NpnConf& src);

  // 入力順序を正しくする．
  void
  validate_iorder() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // オリジナルのシグネチャ
  const NpnRawSig* mSig;

  // 出力極性が確定していたら true
  bool mOpolFixed;

  // 出力極性
  //  1 : 肯定
  // -1 : 否定
  ymint32 mOpol;

  // 入力極性
  //  1: 肯定
  // -1: 否定
  ymint32 mIpols[kNpnMaxNi];

  // 入力の順序
  mutable
  ymuint32 mIorder[kNpnMaxNi];

  // mIorder が正しいとき true となるフラグ
  mutable
  bool mIorderValid;

  // クラス数
  ymuint32 mNc;

  // 入力クラスの順列
  // 各要素は下位2ビットで極性(0, 1, 2)，
  // 残りが入力配列へのインデックスを表す．
  ymint32 mIcList[kNpnMaxNi];

  // グループ数
  ymuint32 mNg;

  // グループの先頭のインデックス
  ymuint32 mIndex[kNpnMaxNi + 1];

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// コンストラクタ
inline
NpnConf::NpnConf() :
  mOpolFixed(false),
  mOpol(1),
  mIorderValid(false),
  mNc(0),
  mNg(0)
{
  mIndex[0] = 0;
  for (ymuint i = 0; i < kNpnMaxNi; ++ i) {
    mIpols[i] = 1;
  }
}

// デストラクタ
inline
NpnConf::~NpnConf()
{
}

// @brief 出力極性が確定のときに true を返す．
inline
bool
NpnConf::is_opol_fixed() const
{
  return mOpolFixed;
}

// 出力極性の割り当ての取得
inline
int
NpnConf::opol() const
{
  return mOpol;
}

// @brief 入力極性の取得
inline
int
NpnConf::ipol(ymuint pos) const
{
  return mIpols[pos];
}

// @brief 入力順序の取得
// @param[in] pos 入力位置
// @return pos 番めの入力番号
inline
ymuint
NpnConf::iorder(ymuint pos) const
{
  if ( !mIorderValid ) {
    validate_iorder();
  }
  return mIorder[pos];
}

// クラス数の取得
inline
ymuint
NpnConf::nc() const
{
  return mNc;
}

// @brief 入力クラスの取得
// @param[in] pos インデックス番号
// @param[out] pol 指定された入力クラスの極性
// @return 指定された入力クラスの入力番号インデックス
inline
pair<ymuint, int>
NpnConf::input(ymuint pos) const
{
  int v = mIcList[pos];
  ymuint index = static_cast<ymuint>(v >> 2);
  int pol = v & 3;
  return make_pair(index, pol);
}

// @brief 入力クラスの入力番号インデックスの取得
// @param[in] pos 入力クラス番号
// @return 指定された入力クラスの先頭の入力番号インデックス
inline
ymuint
NpnConf::ic_rep(ymuint pos) const
{
  int v = mIcList[pos];
  ymuint index = static_cast<ymuint>(v >> 2);
  return index;
}

// @brief 入力クラスの極性の割り当ての取得
// @param[in] pos 入力クラス番号
// @return pos に対応する入力の極性の割り当て状態
//  -  0 : 未定(肯定も否定もありうる)
//  -  1 : 肯定
//  -  2 : 否定
inline
int
NpnConf::ic_pol(ymuint pos) const
{
  int v = mIcList[pos];
  int pol = v & 3;
  return pol;
}

// グループ数の取得
inline
ymuint
NpnConf::ng() const
{
  return mNg;
}

// グループの先頭の取得
inline
ymuint
NpnConf::begin(ymuint pos) const
{
  return mIndex[pos];
}

// グループの末尾の取得
inline
ymuint
NpnConf::end(ymuint pos) const
{
  return mIndex[pos + 1];
}

// @brief グループの要素数の取得
// @param[in] pos グループ番号( < ng() )
// @return pos 番めのグループの要素数
inline
ymuint
NpnConf::gnum(ymuint pos) const
{
  return mIndex[pos + 1] - mIndex[pos];
}

// @brief 対象の関数を得る．
inline
const TvFunc&
NpnConf::func() const
{
  return mSig->func();
}

// @brief 入力数を得る．
inline
ymuint
NpnConf::ni() const
{
  return mSig->ni();
}

// 等価入力クラスの要素数を返す．
inline
ymuint
NpnConf::ic_num(ymuint rep) const
{
  return mSig->ic_num(rep);
}

// @brief 独立(無関係)な入力クラスの先頭番号を返す．
inline
ymuint
NpnConf::indep_rep() const
{
  return mSig->indep_rep();
}

// @brief 独立な入力クラスの要素数を返す．
inline
ymuint
NpnConf::indep_num() const
{
  return mSig->indep_num();
}

// 等価入力クラスの pos の次の要素を返す．
inline
ymuint
NpnConf::ic_link(ymuint pos) const
{
  return mSig->ic_link(pos);
}

// 等価入力クラスの bisym マークを返す．
inline
bool
NpnConf::bisym(ymuint rep) const
{
  return mSig->bisym(rep);
}

// @brief rep1 が rep2 より大きければ true を返す．
inline
bool
NpnConf::w1gt(ymuint rep1,
	      ymuint rep2) const
{
  return mSig->w1gt(rep1, rep2);
}

// @brief rep1 と rep2 が等しければ true を返す．
inline
bool
NpnConf::w1eq(ymuint rep1,
	      ymuint rep2) const
{
  return mSig->w1eq(rep1, rep2);
}

// @brief Walsh の 2次係数を返す．
inline
int
NpnConf::walsh_2(ymuint pos1,
		 ymuint pos2) const
{
  int w2 = mSig->walsh_2(pos1, pos2);
  return w2 * ipol(pos1) * ipol(pos2) * opol();
}

// @brief Walsh の 2次係数を返す (入力順序付き)．
inline
int
NpnConf::walsh_2i(ymuint pos1,
		  ymuint pos2) const
{
  if ( !mIorderValid ) {
    validate_iorder();
  }
  return walsh_2(mIorder[pos1], mIorder[pos2]);
}

// @brief W2 を用いた大小比較関数
inline
bool
NpnConf::w2gt(ymuint rep0,
	      ymuint rep1,
	      ymuint rep2) const
{
  int v1 = mSig->walsh_2(rep0, rep1) * ipol(rep0) * ipol(rep1) * opol();
  int v2 = mSig->walsh_2(rep0, rep2) * ipol(rep0) * ipol(rep2) * opol();
  return v1 >= v2;
}

// @brief W2 を用いた等価比較関数
inline
bool
NpnConf::w2eq(ymuint rep0,
	      ymuint rep1,
	      ymuint rep2) const
{
  int v1 = mSig->walsh_2(rep0, rep1) * ipol(rep1);
  int v2 = mSig->walsh_2(rep0, rep2) * ipol(rep2);
  return v1 == v2;
}

// @brief Walsh 係数を出力する．
inline
void
NpnConf::dump_walsh(ostream& s) const
{
  mSig->dump_walsh(s);
}

// @brief シグネチャを設定する．
inline
void
NpnConf::set_sig(const NpnRawSig* sig)
{
  mSig = sig;
}

// 出力極性の割り当ての設定
inline
void
NpnConf::set_opol(int val)
{
  mOpol = val;
  mOpolFixed = true;
}

// @brief 入力クラスを追加する．
// @param[in] pos 先頭の入力番号
inline
void
NpnConf::add_ic_rep(ymuint pos,
		    int pol)
{
  int v = (static_cast<int>(pos) << 2) | pol;
  mIcList[mNc] = v;
  ++ mNc;
}

// @brief 入力グループの開始番号を追加する．
// @param[in] index 先頭の入力クラス番号
inline
void
NpnConf::add_ig(ymuint index)
{
  mIndex[mNg] = index;
  ++ mNg;
  mIndex[mNg] = mNc;
}

// @brief グループの細分化を行う．
// @param[in] g0 対象のグループ番号
// @param[in] gt 2つの入力クラスの大小比較関数オブジェクト
// @param[in] eq 2つの入力クラスの等価比較関数オブジェクト
// @return 増えたグループ数を返す．
template <typename T1,
	  typename T2>
inline
ymuint
NpnConf::refine(ymuint g0,
		T1 gt,
		T2 eq)
{
  ymuint ng0 = mNg;
  ymuint s = begin(g0);
  ymuint e = end(g0);
  // gt 関数にしたがって整列させる．
  for (ymuint i = s + 1; i < e; ++ i) {
    int tmp1 = mIcList[i];
    ymuint pos1 = static_cast<ymuint>(tmp1 >> 2);
    ymuint j = i;
    for ( ; j > s; -- j) {
      int tmp2 = mIcList[j - 1];
      ymuint pos2 = static_cast<ymuint>(tmp2 >> 2);
      if ( gt(pos2, pos1) ) {
	break;
      }
      mIcList[j] = tmp2;
    }
    if ( j < i ) {
      mIcList[j] = tmp1;
      mIorderValid = false;
    }
  }
  // eq 関数にしたがってグループ化する．
  ymuint prev = ic_rep(s);
  for (ymuint i = s + 1; i < e; ++ i) {
    ymuint pos1 = ic_rep(i);
    if ( !eq(prev, pos1) ) {
      // 新しいグループを作る．
      for (ymuint g1 = mNg; g1 > g0; -- g1) {
	mIndex[g1 + 1] = mIndex[g1];
      }
      ++ mNg;
      ++ g0;
      mIndex[g0] = i;
      prev = pos1;
    }
  }
  return mNg - ng0;
}

END_NAMESPACE_YM_NPN

#endif // YM_NPN_NPNCONF
