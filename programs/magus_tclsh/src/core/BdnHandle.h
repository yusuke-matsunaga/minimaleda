#ifndef SRC_LOGBASE_BDNHANDLE_H
#define SRC_LOGBASE_BDNHANDLE_H

/// @file src/logbase/BdnHandle.h
/// @brief BdnHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "NetHandle.h"
#include "ym_bdn/BdNetwork.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class BdnHandle BdnHandle.h "BdnHandle.h"
/// @brief Bdnwork を持つネットハンドル
//////////////////////////////////////////////////////////////////////
class BdnHandle :
  public NetHandle
{
public:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  BdnHandle(const string& name);

  /// @brief デストラクタ
  virtual
  ~BdnHandle();


public:

  /// @brief 型を返す．
  virtual
  tType
  type() const;

  /// @brief BdNetwork を得る．
  virtual
  const BdNetwork*
  bdn() const;

  /// @brief BdNetwork を得る．
  virtual
  BdNetwork*
  _bdn();

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
  BdNetwork mNetwork;

};

END_NAMESPACE_MAGUS

#endif // SRC_LOGBASE_BDNHANDLE_H
