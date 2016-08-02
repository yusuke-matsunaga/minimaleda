#ifndef MAGUS_MAGBNETCMD_H
#define MAGUS_MAGBNETCMD_H

/// @file BNetCmd.h
/// @brief MagCmd のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MagCmd.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MagCmd.h"
#include "ym_bnet/bnet_nsdef.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class BNetCmd BNetCmd.h "BNetCmd.h"
/// @ingroup MagusGroup
/// @brief BNetwork を操作対象とする Magus のコマンドオブジェクトの基底クラス
//////////////////////////////////////////////////////////////////////
class BNetCmd :
  public MagCmd
{
public:

  /// @brief コンストラクタ
  BNetCmd(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~BNetCmd();


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief カレントネットワークの取得
  /// @note カレントネットワークが BNetwork でない場合には NULL を返す．
  BNetwork*
  cur_network() const;


protected:
  //////////////////////////////////////////////////////////////////////
  // TclCmd の仮想関数のオーバーロード
  //////////////////////////////////////////////////////////////////////

  /// @brief コマンド行の引数をパーズする関数．
  /// @param[in] objv 引数の配列
  /// objv で与えられるコマンド行引数のうちで，
  /// -network <network名> | -nwk <network名>
  /// の形のオプションがあればカレントネットワークのスタックに積む．
  /// その場合には mNetworkSpecified のフラグに true がセットされる．
  /// objv からはこれらのオプション文字列が削除される．
  /// エラーが起きた時にはメッセージをインタプリタにセットして TCL_ERROR
  /// を返す．
  virtual
  int
  before_cmd_proc(TclObjVector& objv);

  /// @brief コマンド処理関数の後で実行される関数
  /// before_cmd_proc() で退避されたカレントネットワーク，
  /// カレントライブラリを元に戻す．
  virtual
  void
  after_cmd_proc();


private:
  //////////////////////////////////////////////////////////////////////
  // メンバ変数
  //////////////////////////////////////////////////////////////////////

  // network オプションを解析するオブジェクト
  TclPoptObj* mPoptNtwk;

  // new_network オプションを解析するオブジェクト
  TclPoptObj* mPoptNewNtwk;

  // コマンドラインでネットワークの指定が行われたかを示すフラグ
  bool mNetworkSpecified;

};

END_NAMESPACE_MAGUS

#endif // MAGUS_MAGBNETCMD_H
