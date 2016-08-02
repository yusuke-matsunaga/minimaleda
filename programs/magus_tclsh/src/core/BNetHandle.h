#ifndef SRC_LOGBASE_BNETHANDLE_H
#define SRC_LOGBASE_BNETHANDLE_H

/// @file src/logbase/BNetHandle.h
/// @brief BNetHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "NetHandle.h"
#include "ym_bnet/BNetwork.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class BNetHandle BNetHandle.h "BNetHandle.h"
/// @brief BNetwork を持つネットハンドル
//////////////////////////////////////////////////////////////////////
class BNetHandle :
  public NetHandle
{
public:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  BNetHandle(const string& name);

  /// @brief デストラクタ
  virtual
  ~BNetHandle();


public:

  /// @brief 型を返す．
  virtual
  tType
  type() const;

  /// @brief BNetwork を得る．
  virtual
  const BNetwork*
  bnetwork() const;

  /// @brief BNetwork を得る．
  virtual
  BNetwork*
  _bnetwork();

  /// @brief ネットワークをクリアする．
  virtual
  void
  clear();

  /// @brief ネットワークをコピーする．
  /// @param[in] src コピー元のネットハンドル
  /// @param[in] allow_conv true ならタイプが異なる時に変換する．
  /// @return コピーが成功したら true を返す．
  virtual
  bool
  copy(const NetHandle* src,
       bool allow_conv = false);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ネットワークの実体
  BNetwork mNetwork;

};

END_NAMESPACE_MAGUS

#endif // SRC_LOGBASE_BNETHANDLE_H
