#ifndef LIBYM_MVN_MVCONSTPARTSELECT_H
#define LIBYM_MVN_MVCONSTPARTSELECT_H

/// @file libym_mvn/MvConstPartSelect.h
/// @brief MvConstPartSelect のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvUnaryOp.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvConstPartSelect MvConstPartSelect.h "MvConstPartSelect.h"
/// @brief 定数ビット指定を表すノード
//////////////////////////////////////////////////////////////////////
class MvConstPartSelect :
  public MvUnaryOp
{
  friend class MvMgr;
public:

  /// @brief ノードの種類を得る．
  virtual
  tType
  type() const;

  /// @brief 範囲指定の MSB を得る．
  /// @note type() が kConstPartSelect の時のみ意味を持つ．
  /// @note デフォルトの実装では 0 を返す．
  virtual
  ymuint
  msb() const;

  /// @brief 範囲指定の LSB を得る．
  /// @note type() が kConstPartSelect の時のみ意味を持つ．
  /// @note デフォルトの実装では 0 を返す．
  virtual
  ymuint
  lsb() const;


public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] msb 範囲指定の MSB
  /// @param[in] lsb 範囲指定の LSB
  /// @param[in] bit_width 入力のビット幅
  MvConstPartSelect(MvModule* module,
		    ymuint msb,
		    ymuint lsb,
		    ymuint bit_width);

  /// @brief デストラクタ
  ~MvConstPartSelect();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 範囲指定の MSB
  ymuint32 mMsb;

  // 範囲指定の LSB
  ymuint32 mLsb;
  
};

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_MVCONSTPARTSELECT_H
