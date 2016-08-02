#ifndef YM_MVN_MVPORT_H
#define YM_MVN_MVPORT_H

/// @file ym_mvn/MvPort.h
/// @brief MvPort のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/mvn_nsdef.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvPortRef MvPort.h "ym_mvn/MvPort.h"
/// @brief ポートが参照している実体を表すクラス
///
/// 具体的には以下の3通りのタイプがある．
/// - ノードのみ
/// - ノード＋ビット指定
/// - ノード＋範囲指定
/// 実装としてはメンバ mMsb, mLsb の下位1ビットを使って，
/// - ノードのみ -> どちらも 0
/// - ノード＋ビット指定 -> mMsb のみ 1 mLsb は 0
/// - ノード＋範囲指定   -> mLsb のみ 1 mMsb は 0
/// で区別する．
//////////////////////////////////////////////////////////////////////
class MvPortRef
{
  friend class MvPort;
  friend class MvMgr;
  
public:

  /// @brief ノードを返す．
  /// @note ノードのタイプは kInput か kOutput
  MvNode*
  node() const;

  /// @brief 単純な形式の場合 true を返す．
  bool
  is_simple() const;
  
  /// @brief ビット指定ありの場合 true を返す．
  bool
  has_bitselect() const;
  
  /// @brief 範囲指定ありの場合 true を返す．
  /// @note ビット指定は含まない．
  bool
  has_partselect() const;

  /// @brief この実体のビット幅を返す．
  ymuint
  bit_width() const;

  /// @brief ビット指定位置を返す．
  /// @note has_bitselect() == true の時のみ意味を持つ．
  ymuint
  bitpos() const;
  
  /// @brief 範囲指定の MSB を返す．
  /// @note has_partselect() == true の時のみ意味を持つ．
  ymuint
  msb() const;

  /// @brief 範囲指定の LSB を返す．
  /// @note has_partselect() == true の時のみ意味を持つ．
  ymuint
  lsb() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  MvPortRef();

  /// @brief デストラクタ
  ~MvPortRef();


private:
  //////////////////////////////////////////////////////////////////////
  // MvMgr が用いる設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 通常タイプに設定する．
  /// @param[in] node ノード
  void
  set(MvNode* ndoe);

  /// @brief ビット指定タイプに設定する．
  /// @param[in] node ノード
  /// @param[in] bitpos ビット位置
  void
  set(MvNode* ndoe,
      ymuint bitpos);

  /// @brief 範囲指定タイプに設定する．
  /// @param[in] node ノード
  /// @param[in] msb 範囲指定の MSB
  /// @param[in] lsb 範囲指定の LSB
  void
  set(MvNode* ndoe,
      ymuint msb,
      ymuint lsb);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード
  MvNode* mNode;

  // 範囲指定の MSB
  // ただし下位1ビットは範囲指定/ビット指定ありのフラグ
  ymuint32 mMsb;

  // 範囲指定の LSB
  // ただし下位1ビットは範囲指定ありのフラグ
  ymuint32 mLsb;

};


//////////////////////////////////////////////////////////////////////
/// @class MvPort MvPort.h "ym_mvn/MvPort.h"
/// @brief MvModule のポートを表すクラス
///
/// 単純には1つのポートは1つの入力ノードもしくは出力ノードに対応するが，
/// 場合によっては対応するノードがない場合，複数ある場合，ノードの一部の
/// ビット線のみに対応する場合などがある．
/// そのために MvPortRef のベクタを用いる．
//////////////////////////////////////////////////////////////////////
class MvPort
{
  friend class MvMgr;
  
public:

  /// @brief 名前を得る．
  /// @note 空(NULL)の場合もある．
  const char*
  name() const;

  /// @brief ビット幅を得る．
  ymuint
  bit_width() const;

  /// @brief port_ref 数を得る．
  ymuint
  port_ref_num() const;

  /// @brief port_ref を得る．
  /// @param[in] pos 位置 ( 0 <= pos < port_ref_num() )
  const MvPortRef*
  port_ref(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] name 名前
  /// @param[in] np portref 数
  MvPort(const char* name,
	 ymuint np);

  /// @brief デストラクタ
  ~MvPort();

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  const char* mName;

  // portref の数
  ymuint32 mPortRefNum;

  // portref の配列
  MvPortRef* mPortRefArray;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ノードを返す．
// @note ノードのタイプは kInput か kOutput
inline
MvNode*
MvPortRef::node() const
{
  return mNode;
}

// @brief 単純な形式の場合 true を返す．
inline
bool
MvPortRef::is_simple() const
{
  return mMsb == 0U && mLsb == 0U;
}

// @brief ビット指定ありの場合 true を返す．
inline
bool
MvPortRef::has_bitselect() const
{
  return static_cast<bool>(mMsb & 1U);
}

// @brief 範囲指定ありの場合 true を返す．
// @note ビット指定は含まない．
inline
bool
MvPortRef::has_partselect() const
{
  return static_cast<bool>(mLsb & 1U);
} 

// @brief ビット指定位置を返す．
// @note has_bitselect() == true の時のみ意味を持つ．
inline
ymuint
MvPortRef::bitpos() const
{
  // 実は msb() のエイリアス
  return msb();
}

// @brief 範囲指定の MSB を返す．
// @note has_partselect() == true の時のみ意味を持つ．
inline
ymuint
MvPortRef::msb() const
{
  return mMsb >> 1;
}

// @brief 範囲指定の LSB を返す．
// @note has_partselect() == true の時のみ意味を持つ．
inline
ymuint
MvPortRef::lsb() const
{
  return mLsb >> 1;
}

// @brief コンストラクタ
inline
MvPortRef::MvPortRef()
{
}

// @brief デストラクタ
inline
MvPortRef::~MvPortRef()
{
}

// @brief 通常タイプに設定する．
// @param[in] node ノード
inline
void
MvPortRef::set(MvNode* node)
{
  mNode = node;
  mMsb = 0U;
  mLsb = 0U;
}

// @brief ビット指定タイプに設定する．
// @param[in] node ノード
// @param[in] bitpos ビット位置
inline
void
MvPortRef::set(MvNode* node,
	       ymuint bitpos)
{
  mNode = node;
  mMsb = (bitpos << 1) | 1U;
  mLsb = 0U;
}

// @brief 範囲指定タイプに設定する．
// @param[in] node ノード
// @param[in] msb 範囲指定の MSB
// @param[in] lsb 範囲指定の LSB
inline
void
MvPortRef::set(MvNode* node,
	       ymuint msb,
	       ymuint lsb)
{
  mNode = node;
  mMsb = (msb << 1);
  mLsb = (lsb << 1) | 1U;
}

// @brief 名前を得る．
// @note 空(NULL)の場合もある．
inline
const char*
MvPort::name() const
{
  return mName;
}

// @brief port_ref 数を得る．
inline
ymuint
MvPort::port_ref_num() const
{
  return mPortRefNum;
}

// @brief port_ref を得る．
// @param[in] pos 位置 ( 0 <= pos < port_ref_num() )
inline
const MvPortRef*
MvPort::port_ref(ymuint pos) const
{
  return &mPortRefArray[pos];
}

END_NAMESPACE_YM_MVN

#endif // YM_MVN_MVPORT_H
