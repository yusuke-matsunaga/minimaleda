#ifndef YM_NPN_NPNMGR_H
#define YM_NPN_NPNMGR_H

/// @file ym_npn/NpnMgr.h
/// @brief NpnMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: NpnMgr.h 1508 2008-06-30 04:55:42Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_npn/NpnMap.h"


BEGIN_NAMESPACE_YM_NPN

class NpnConf;

//////////////////////////////////////////////////////////////////////
/// @class NpnMgr NpnMgr.h <ym_npn/NpnMgr.h>
/// @brief NPN同値類の正規形を求めるためのクラス
//////////////////////////////////////////////////////////////////////
class NpnMgr
{
public:

  /// @brief コンストラクタ
  NpnMgr();

  /// @brief デストラクタ
  ~NpnMgr();


public:

  /// @brief func の正規化を行う．
  /// @param[in] func 対象の論理関数
  /// @param[out] cmap 正規化するための変換マップ
  /// @note func.xform(cmap) の結果が正規化された関数となる．
  void
  cannonical(const TvFunc& func,
	     NpnMap& cmap);

  /// @brief 直前の cannonical の呼び出しにおける NpnMap の全候補を返す．
  /// @param[out] map_list 変換マップを格納するリスト
  void
  all_map(list<NpnMap>& map_list) const;

  /// @brief w2max_recur の起動回数を返す．
  /// 直前の cannonical の呼び出しにおける
  /// w2max_recur の起動回数を返す．
  ymulong
  w2max_count() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief シグネチャが最大になるように極性と順序を決める．
  /// @param[out] conf
  /// @param[in] g0 調べ始める最初のグループ番号
  void
  w2max_recur(NpnConf& conf,
	      ymuint g0);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // w2max_recur で用いる現在の最適解のリスト
  list<NpnConf> mMaxList;

  // 1回の cannonical あたりの w2max_recur の起動回数
  ymulong mW2max_count;

};

END_NAMESPACE_YM_NPN

#endif // YM_NPN_NPNMGR_H
