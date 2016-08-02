#ifndef YM_MVN_MVNPIN_H
#define YM_MVN_MVNPIN_H

/// @file ym_mvn/MvnPin.h
/// @brief MvnInputPin/MvnOutputPin のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/mvn_nsdef.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvInputPin MvPin.h "ym_mvn/MvPin.h"
/// @brief ノードの入力ピンを表すクラス
//////////////////////////////////////////////////////////////////////
class MvInputPin :
  public DlElem
{
  friend class MvMgr;
  friend class MvNode;

private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  MvInputPin();

  /// @brief デストラクタ
  ~MvInputPin();


public:

  /// @breif 属しているノードを得る．
  MvNode*
  node() const;

  /// @brief ビット幅を得る．
  ymuint
  bit_width() const;

  /// @brief ノードの何番目のピンかを返す．
  /// @note 入力ピンと出力ピンは区別される．
  ymuint
  pos() const;

  /// @brief 接続している出力ピンを得る．
  MvOutputPin*
  src_pin() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のノード
  MvNode* mNode;

  // ビット幅
  ymuint32 mBitWidth;

  // 位置
  ymuint32 mPos;

  // 接続している出力ピン
  MvOutputPin* mSrcPin;

};


//////////////////////////////////////////////////////////////////////
/// @class MvOutputPin MvPin.h "ym_mvn/MvPin.h"
/// @brief ノードの出力ピンを表すクラス
//////////////////////////////////////////////////////////////////////
class MvOutputPin
{
  friend class MvMgr;
  friend class MvNode;

private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  MvOutputPin();

  /// @brief デストラクタ
  ~MvOutputPin();


public:

  /// @breif 属しているノードを得る．
  MvNode*
  node() const;

  /// @brief ビット幅を得る．
  ymuint
  bit_width() const;

  /// @brief ノードの何番目のピンかを返す．
  /// @note 入力ピンと出力ピンは区別される．
  ymuint
  pos() const;

  /// @brief 接続している入力ピンのリストを得る．
  const MvInputPinList&
  dst_pin_list() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のノード
  MvNode* mNode;

  // ビット幅
  ymuint32 mBitWidth;

  // 位置
  ymuint32 mPos;

  // 接続している入力ピンのリスト
  MvInputPinList mDstPinList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @breif 属しているノードを得る．
inline
MvNode*
MvInputPin::node() const
{
  return mNode;
}

// @brief ビット幅を得る．
inline
ymuint
MvInputPin::bit_width() const
{
  return mBitWidth;
}

// @brief ノードの何番目のピンかを返す．
// @note 入力ピンと出力ピンは区別される．
inline
ymuint
MvInputPin::pos() const
{
  return mPos;
}

// @brief 接続している出力ピンを得る．
inline
MvOutputPin*
MvInputPin::src_pin() const
{
  return mSrcPin;
}

// @breif 属しているノードを得る．
inline
MvNode*
MvOutputPin::node() const
{
  return mNode;
}

// @brief ビット幅を得る．
inline
ymuint
MvOutputPin::bit_width() const
{
  return mBitWidth;
}

// @brief ノードの何番目のピンかを返す．
// @note 入力ピンと出力ピンは区別される．
inline
ymuint
MvOutputPin::pos() const
{
  return mPos;
}

// @brief 接続している入力ピンのリストを得る．
inline
const MvInputPinList&
MvOutputPin::dst_pin_list() const
{
  return mDstPinList;
}

END_NAMESPACE_YM_MVN

#endif // YM_MVN_MVNPIN_H
