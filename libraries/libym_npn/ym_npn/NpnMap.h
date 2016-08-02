#ifndef YM_NPN_NPNMAP_H
#define YM_NPN_NPNMAP_H

/// @file ym_npn/NpnMap.h
/// @brief NpnMap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: NpnMap.h 1594 2008-07-18 10:26:12Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_npn/npn_nsdef.h"
#include "ym_lexp/Literal.h"


BEGIN_NAMESPACE_YM_NPN

//////////////////////////////////////////////////////////////////////
// 型と定数の定義
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
/// @ingroup NpnGroup
/// @brief 極性を含んだマッピング情報
//////////////////////////////////////////////////////////////////////
typedef ymuint32 tNpnImap;

//////////////////////////////////////////////////////////////////////
/// @ingroup NpnGroup
/// @brief 不正な tNpnImap の状態を表すエラーコード
//////////////////////////////////////////////////////////////////////
const tNpnImap kImapBad = 0xffffffff;


//////////////////////////////////////////////////////////////////////
/// @class NpnMap NpnMap.h <ym_npn/NpnMap.h>
/// @ingroup NpnGroup
/// @brief NPN変換の情報を入れるクラス
///
/// @sa tVarId, tPol, tNpnImap
//////////////////////////////////////////////////////////////////////
class NpnMap
{
public:

  /// @brief 空のコンストラクタ．
  /// @note 内容は不定
  NpnMap();

  /// @brief 入力数(と出力極性)を指定したコンストラクタ
  /// @param[in] ni 入力数
  /// @param[in] pol 出力極性
  /// @note 各入力の変換内容は kImapBad になっている．
  explicit
  NpnMap(ymuint ni,
	 tPol pol = kPolPosi);

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  NpnMap(const NpnMap& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 自分自身への定数参照を返す．
  const NpnMap&
  operator=(const NpnMap& src);

  /// @brief デストラクタ
  ~NpnMap();

  /// @brief 内容をクリアする．
  /// @note 入力の変換内容は kImapBad になる．
  /// 出力の極性は kPolPosi
  void
  clear();

  /// @brief 入力数を再設定する．
  /// @param[in] ni 入力数
  /// @note 以前の内容はクリアされる．
  void
  resize(ymuint ni);

  /// @brief 恒等変換を表すように設定する．
  /// @param[in] ni 入力数
  void
  set_identity(ymuint ni);

  /// @brief 入力の変換内容の設定
  /// @param[in] pos 入力番号
  /// @param[in] dst_pos 変換先の入力番号
  /// @param[in] pol 極性
  void
  set(tVarId pos,
      tVarId dst_pos,
      tPol pol);

  /// @brief 入力の変換内容の設定
  /// @param[in] pos 入力番号
  /// @param[in] imap 変換情報(変換先の入力番号と極性)
  /// @sa tNpnImap
  void
  set(tVarId pos,
      tNpnImap imap);

  /// @brief 出力極性を設定する．
  /// @param[in] pol 出力極性
  void
  set_opol(tPol pol);

  /// @brief 入力数を得る．
  /// @return 入力数
  ymuint
  ni() const;

  /// @brief 入力の変換情報の取得
  /// @param[in] pos 入力番号
  /// @return pos 番目の入力の変換情報
  /// @note pos に対応するマッピング情報がないときには
  /// kImapBad を返す．
  /// @sa tNpnImap
  tNpnImap
  imap(tVarId pos) const;

  /// @brief 出力極性を返す．
  /// @return 出力極性
  tPol
  opol() const;

  /// @brief 内容が等しいか調べる．
  /// @param[in] src 比較対象のマップ
  /// @return 自分自身と src が等しいときに true を返す．
  bool
  operator==(const NpnMap& src) const;

  /// @brief 逆写像を求める．
  /// @param[in] src 入力となるマップ
  /// @return src の逆写像
  /// @note 1対1写像でなければ答えは不定．
  friend
  NpnMap
  inverse(const NpnMap& src);

  /// @brief 合成を求める．
  /// @param[in] src1,src2 入力となるマップ
  /// @return src1 と src2 を合成したもの
  /// @note src1の値域とsrc2の定義域は一致していな
  /// ければならない．そうでなければ答えは不定．
  friend
  NpnMap
  operator*(const NpnMap& src1,
	    const NpnMap& src2);

  /// @brief 内容を表示する(主にデバッグ用)．
  /// @param[in] s 出力ストリーム
  /// @param[in] map 出力対象のマップ
  /// @note 改行はしない．
  friend
  ostream&
  operator<<(ostream& s,
	     const NpnMap& map);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力数 + 出力の極性
  ymuint32 mNiPol;

  // 入力のマッピング情報
  tNpnImap mImap[kNpnMaxNi];

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// 入力数を得る．
inline
ymuint
NpnMap::ni() const
{
  return mNiPol >> 1;
}

// posに対応するマッピング情報を得る．
inline
tNpnImap
NpnMap::imap(tVarId pos) const
{
  if ( pos < ni() ) {
    return mImap[pos];
  }
  return kImapBad;
}

// 出力極性を返す．
inline
tPol
NpnMap::opol() const
{
  return (mNiPol & 1) ? kPolNega : kPolPosi;
}


//////////////////////////////////////////////////////////////////////
// tMapImap 関係のコンビニエンス関数
//////////////////////////////////////////////////////////////////////

/// @ingroup NpnGroup
/// @brief マップされた入力番号を取り出す．
/// @param[in] x パックされた入力マッピング情報
/// @return 入力のマッピング先の番号
///
/// @sa tNpnImap, tVarId
inline
tVarId
npnimap_pos(tNpnImap x)
{
  return x >> 1;
}

/// @ingroup NpnGroup
/// @brief 入力極性を取り出す．
/// @param[in] x パックされた入力マッピング情報
/// @return 入力の極性
///
/// @sa tNpnImap, tPol
inline
tPol
npnimap_pol(tNpnImap x)
{
  return static_cast<tPol>(x & 1UL);
}

/// @ingroup NpnGroup
/// @brief 入力番号と極性をパックして tNpnImap を作る．
/// @param[in] pos 入力のマッピング先の番号
/// @param[in] pol 入力の極性
/// @return パックされた入力マッピング情報
/// @sa tNpnImap, tVarId, tPol
inline
tNpnImap
npnimap_pack(tVarId pos,
	     tPol pol)
{
  return tNpnImap((pos << 1) | ymuint32(pol));
}

END_NAMESPACE_YM_NPN

BEGIN_NAMESPACE_YM

using nsNpn::tNpnImap;
using nsNpn::npnimap_pos;
using nsNpn::npnimap_pol;
using nsNpn::npnimap_pack;

END_NAMESPACE_YM

#endif // YM_NPN_NPNMAP_H
