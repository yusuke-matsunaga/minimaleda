#ifndef LIBYM_BDD_BMC_BMCNODE_H
#define LIBYM_BDD_BMC_BMCNODE_H

/// @file libym_bdd/bmc/BmcNode.h
/// @brief BmcNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BmcNode.h 693 2007-05-28 09:13:57Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#include <ym_bdd/Bdd.h>


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
// 変数の情報を格納しておくクラス
//////////////////////////////////////////////////////////////////////
class BmcVar
{
  friend class BddMgrClassic;
public:

  // 変数番号を得る．
  tVarId
  varid() const
  {
    return mId;
  }
  
  // レベルを得る．
  tLevel
  level() const
  {
    return tLevel(mId);
  }

private:

  // コンストラクタ
  BmcVar(tVarId id) :
    mId(id),
    mMark(0),
    mLink(NULL)
  {
  }
  
  // デストラクタ
  ~BmcVar()
  {
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数番号
  // レベルも同一
  tVarId mId;
  
  // 作業用のマーク
  int mMark;

  // compose用にBDDの枝を入れておくメンバ
  tBddEdge mCompEdge;

  // 変数リスト中の次の要素を指すポインタ
  BmcVar* mNext;

  // ハッシュ表中の次の要素を指すポインタ
  BmcVar* mLink;

};


//////////////////////////////////////////////////////////////////////
// BDDのノードを表す構造体
//////////////////////////////////////////////////////////////////////
class BmcNode
{
  friend class BddMgrClassic;

public:
  //////////////////////////////////////////////////////////////////////
  // 定数の定義
  // mRefMarkにパックされる情報
  // 最上位ビットから: positive-mark
  //                   negative-mark
  //                   リンクオーバーフロービット
  //                   残りビットは全てリンク数を表す．
  //////////////////////////////////////////////////////////////////////
  static
  const int kPBit = sizeof(ymuint32) * 8 - 1;
  static
  const int kNBit = sizeof(ymuint32) * 8 - 2;
  static
  const int kLMaxBit = sizeof(ymuint32) * 8 - 3;
  static
  const ymuint32 kPMask = 1UL << kPBit;
  static
  const ymuint32 kNMask = 1UL << kNBit;
  static
  const ymuint32 kLMax = 1UL << kLMaxBit;
  static
  const ymuint32 kLMax_1 = kLMax - 1;
  static
  const ymuint32 kLMask = kLMax | kLMax_1;
  
public:

  // 中間ノードの場合に0枝/1枝を得る．
  tBddEdge
  edge0() const;

  tBddEdge
  edge1() const;

  tBddEdge
  edge0(tPol p) const;

  tBddEdge
  edge1(tPol p) const;
  
  // 変数を得る．
  BmcVar*
  var() const;

  // 変数インデックス値を得る
  tVarId
  varid() const;
  
  // レベルを得る．
  tLevel
  level() const;

  // p-mark が付いていたらtrueを返す
  bool
  pmark() const;
  
  // n-mark が付いていたらtrueを返す
  bool
  nmark() const;
  
  // p-mark を付ける
  // val が非0でセット,0でリセット
  void
  pmark(int val);
  
  // n-mark を付ける
  // val が非0でセット,0でリセット
  void
  nmark(int val);
  
  // 両方のマークを消す
  void
  rst_mark();
  
  // リンク数を得る．
  size_t
  refcount() const;
  
  // 参照されていない時にtrueを返す
  bool
  noref() const;
  

private:

  // リンク数が最大値を越えている時に0，それ以外には1を返す．
  int
  linkdelta() const;
  
  // リンク数を増やす(オーバーフロー時は何もしない)
  size_t
  linkinc();
  
  // リンク数を減らす(オーバーフロー時は何もしない)
  size_t
  linkdec();
  
  // コンストラクタ
  // BddMgrClassic以外はこのオブジェクトを生成できない．
  BmcNode();
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 0枝
  tBddEdge mEdge0;

  // 1枝
  tBddEdge mEdge1;

  // 変数へのポインタ
  BmcVar* mVar;

  // 参照回数＋α(上の定数を参照)
  ymuint32 mRefMark;

  // 節点テーブル用のリンクポインタ
  BmcNode* mLink;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// 中間ノードの場合に左右の子供への枝を得る．
inline
tBddEdge
BmcNode::edge0() const
{
  return mEdge0;
}
inline
tBddEdge
BmcNode::edge1() const
{
  return mEdge1;
}
inline
tBddEdge
BmcNode::edge0(tPol p) const
{
  return addpol(mEdge0, p);
}
inline
tBddEdge
BmcNode::edge1(tPol p) const
{
  return addpol(mEdge1, p);
}

// 変数を得る．
inline
BmcVar*
BmcNode::var() const
{
  return mVar;
}

// 変数インデックス値を得る
inline
tVarId
BmcNode::varid() const
{
  return mVar->varid();
}

// レベルを得る
inline
tLevel
BmcNode::level() const
{
  return mVar->level();
}

// p-mark が付いていたらtrueを返す
inline
bool
BmcNode::pmark() const
{
  return ((mRefMark >> kPBit) & 1UL);
}

// n-mark が付いていたらtrueを返す
inline
bool
BmcNode::nmark() const
{
  return ((mRefMark >> kNBit) & 1UL);
}

// p-mark を付ける
inline
void
BmcNode::pmark(int val)
{
  if ( val ) {
    mRefMark |= kPMask;
  }
  else {
    mRefMark &= ~kPMask;
  }
}

// n-mark を付ける
inline
void
BmcNode::nmark(int val)
{
  if ( val ) {
    mRefMark |= kNMask;
  }
  else {
    mRefMark &= ~kNMask;
  }
}

// 両方のマークを消す
inline
void
BmcNode::rst_mark()
{
  mRefMark &= ~(kPMask | kNMask);
}

// リンク数を得る．
inline
size_t
BmcNode::refcount() const
{
  return static_cast<size_t>(mRefMark & kLMask);
}

// 参照されていない時にtrueを返す
inline
bool
BmcNode::noref() const
{
  return (mRefMark & kLMax_1) == 0UL;
}

// リンク数が最大値を越えている時に0，それ以外には1を返す．
inline
int
BmcNode::linkdelta() const
{
  return static_cast<int>(((~mRefMark) >> kLMaxBit) & 1);
}

// リンク数を増やす(オーバーフロー時は何もしない)
inline
size_t
BmcNode::linkinc()
{
  int d = linkdelta();
  return (mRefMark += d) & kLMask;
}

// リンク数を減らす(オーバーフロー時は何もしない)
inline
size_t
BmcNode::linkdec()
{
  int d = linkdelta();
  return (mRefMark -= d) & kLMask;
}

// コンストラクタ
// BddMgrClassic以外はこのオブジェクトを生成できない．
inline
BmcNode::BmcNode()
{
}

END_NAMESPACE_YM_BDD

BEGIN_NAMESPACE_HASH
// BmcNode へのポインタをキーにしたハッシュ関数クラスの定義
template <>
struct hash<nsYm::nsBdd::BmcNode*>
{
  size_t
  operator()(nsYm::nsBdd::BmcNode* node) const
  {
    return reinterpret_cast<size_t>(node)/sizeof(void*);
  }
};
END_NAMESPACE_HASH

#endif // LIBYM_BDD_BMC_BMCNODE_H
