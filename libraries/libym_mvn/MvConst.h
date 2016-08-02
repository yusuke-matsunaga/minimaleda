#ifndef LIBYM_MVN_MVCONST_H
#define LIBYM_MVN_MVCONST_H

/// @file libym_mvn/MvConst.h
/// @brief MvConst のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/MvNode.h"
#include "ym_mvn/MvPin.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvConst MvConst.h "MvConst.h"
/// @brief 入力ノードを表すクラス
//////////////////////////////////////////////////////////////////////
class MvConst :
  public MvNode
{
  friend class MvMgr;

public:

  /// @brief ノードの種類を得る．
  virtual
  tType
  type() const;

  /// @brief 定数値を得る．
  /// @param[out] val 値を格納するベクタ
  /// @note type() が kConst の時のみ意味を持つ．
  virtual
  void
  const_value(vector<ymuint32>& val) const;


private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] bit_width ビット幅
  /// @param[in] val 値
  MvConst(MvModule* module,
	  ymuint bit_width,
	  const vector<ymuint32>& val);

  /// @brief デストラクタ
  ~MvConst();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 値
  vector<ymuint32> mVal;

};

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_MVCONST_H
