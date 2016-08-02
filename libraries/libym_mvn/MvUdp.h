#ifndef LIBYM_MVN_MVUDP_H
#define LIBYM_MVN_MVUDP_H

/// @file libym_mvn/MvUdp.h
/// @brief MvCombUdp, MvSeqUdp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/MvNode.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvCombUdp MvUdp.h "MvUdp.h"
/// @brief 組み合わせ回路の UDP を表すクラス
//////////////////////////////////////////////////////////////////////
class MvCombUdp :
  public MvNode
{
  friend class MvMgr;
public:

  /// @brief ノードの種類を得る．
  virtual
  tType
  type() const;


private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] ni 入力数
  MvCombUdp(MvModule* module,
	    ymuint ni);

  /// @brief デストラクタ
  ~MvCombUdp();

};


//////////////////////////////////////////////////////////////////////
/// @class MvSeqUdp MvUdp.h "MvUdp.h"
/// @brief 組み合わせ回路の UDP を表すクラス
//////////////////////////////////////////////////////////////////////
class MvSeqUdp :
  public MvNode
{
  friend class MvMgr;
public:

  /// @brief ノードの種類を得る．
  virtual
  tType
  type() const;


private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] ni 入力数
  MvSeqUdp(MvModule* module,
	   ymuint ni);

  /// @brief デストラクタ
  ~MvSeqUdp();

};

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_MVUDP_H
