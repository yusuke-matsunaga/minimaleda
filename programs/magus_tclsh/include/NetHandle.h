#ifndef MAGUS_NETHANDLE_H
#define MAGUS_NETHANDLE_H

/// @file include/NetHandle.h
/// @brief NetHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: NetHandle.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

#include "magus_nsdef.h"
#include "ym_bnet/bnet_nsdef.h"
#include "ym_bdn/bdn_nsdef.h"
#include "ym_mvn/mvn_nsdef.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class NetHandle NetHandle.h "NetHandle.h"
/// @brief ネットワークを保持するハンドル
//////////////////////////////////////////////////////////////////////
class NetHandle
{
  friend class MagMgr;

public:
  //////////////////////////////////////////////////////////////////////
  // 型の定義
  //////////////////////////////////////////////////////////////////////

  /// @brief NetHandle の表す型
  enum tType {
    /// @brief BNetwork
    kMagBNet,
    /// @brief BdNetwork
    kMagBdn,
    /// @brief MvNetwork
    kMagMvn
  };


protected:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  // MagMgr のみが生成/破壊を行う．
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] name 名前
  NetHandle(const string& name);

  /// @brief デストラクタ
  virtual
  ~NetHandle();


public:

  /// @brief ネットワーク名を得る．
  const string&
  name() const;

  /// @brief 型を返す．
  virtual
  tType
  type() const = 0;

  /// @brief BNetwork を得る．
  virtual
  const BNetwork*
  bnetwork() const;

  /// @brief BNetwork を得る．
  virtual
  BNetwork*
  _bnetwork();

  /// @brief BdNetwork を得る．
  virtual
  const BdNetwork*
  bdn() const;

  /// @brief BdNetwork を得る．
  virtual
  BdNetwork*
  _bdn();

  /// @brief MvNetwork を得る．
  virtual
  const MvMgr*
  mvn() const;

  /// @brief MvNetwork を得る．
  virtual
  MvMgr*
  _mvn();

  /// @brief ネットワークをクリアする．
  virtual
  void
  clear() = 0;

  /// @brief ネットワークをコピーする．
  /// @param[in] src コピー元のネットハンドル
  /// @param[in] allow_conv true ならタイプが異なる時に変換する．
  /// @return コピーが成功したら true を返す．
  virtual
  bool
  copy(const NetHandle* src,
       bool allow_conv = false) = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ネットワーク名
  string mName;

  // カレントネットワークスタックに積まれた回数
  int mCount;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ネットワーク名を得る．
inline
const string&
NetHandle::name() const
{
  return mName;
}

END_NAMESPACE_MAGUS

#endif // MAGUS_NETHANDLE_H
