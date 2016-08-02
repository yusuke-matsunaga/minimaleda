#ifndef LIBYM_MVN_MVCONSTBITSELECT_H
#define LIBYM_MVN_MVCONSTBITSELECT_H

/// @file libym_mvn/MvConstBitSelect.h
/// @brief MvConstBitSelect のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvUnaryOp.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvConstBitSelect MvConstBitSelect.h "MvConstBitSelect.h"
/// @brief 定数ビット指定を表すノード
//////////////////////////////////////////////////////////////////////
class MvConstBitSelect :
  public MvUnaryOp
{
  friend class MvMgr;
public:

  /// @brief ノードの種類を得る．
  virtual
  tType
  type() const;

  /// @brief ビット位置を得る．
  /// @note type() が kConstBitSelect の時のみ意味を持つ．
  /// @note デフォルトの実装では 0 を返す．
  virtual
  ymuint
  bitpos() const;


public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] bitpos ビット位置
  /// @param[in] bit_width 入力のビット幅
  MvConstBitSelect(MvModule* module,
		   ymuint bitpos,
		   ymuint bit_width);

  /// @brief デストラクタ
  ~MvConstBitSelect();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ビット位置
  ymuint32 mBitPos;
  
};

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_MVCONSTBITSELECT_H
