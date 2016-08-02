#ifndef SEALCMD_H
#define SEALCMD_H

/// @file include/SealCmd.h
/// @brief SealCmd のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SealCmd.h 1920 2008-12-20 15:52:42Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.

// Magus のコマンドオブジェクトの基底クラス

#include "seal_nsdef.h"
#include <ym_tclpp/TclCmd.h>
#include <ym_tgnet/tgnet_nsdef.h>


BEGIN_NAMESPACE_YM_SEAL

class SealMgr;

//////////////////////////////////////////////////////////////////////
// SEAL のコマンドオブジェクトの基底クラス
// おもな仕事はエラーが起きた時にメッセージをセット
// するような関数の殻をかぶせること．
// あとは変換などの便利関数を入れてある．
//////////////////////////////////////////////////////////////////////
class SealCmd :
  public TclCmd
{
public:

  /// @brief コンストラクタ
  SealCmd(SealMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~SealCmd();


protected:

  /// @brief ネットワークをセットした後に呼ぶ関数
  void
  after_set_network();
  
  /// @brief SealMgr を返す．
  SealMgr&
  mgr();
  
protected:
  
  /// @brief TgNetwork を取り出す．
  TgNetwork&
  _network();

  
private:
  //////////////////////////////////////////////////////////////////////
  // メンバ変数
  //////////////////////////////////////////////////////////////////////

  // 共通のデータを管理するオブジェクト
  SealMgr* mMgr;

};
  

//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief SealMgr を返す．
inline
SealMgr&
SealCmd::mgr()
{
  return *mMgr;
}

END_NAMESPACE_YM_SEAL

#endif // SEALCMD_H
