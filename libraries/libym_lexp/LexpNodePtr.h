#ifndef LIBYM_LEXP_LEXPNODEPTR_H
#define LIBYM_LEXP_LEXPNODEPTR_H

/// @file libym_lexp/LexpNodePtr.h
/// @brief LexpNodePtr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: LexpNodePtr.h 958 2007-08-28 05:38:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_lexp/LogExpr.h"


BEGIN_NAMESPACE_YM_LEXP

//////////////////////////////////////////////////////////////////////
/// @class LexpNodePtr LexpNode.h "LexpNode.h*
/// @brief LexpNode 用のスマートポインタ
//////////////////////////////////////////////////////////////////////
class LexpNodePtr
{
  friend class LexpNodePtrList;

public:

  /// @brief コンストラクタ
  /// @param[in] ptr Cポインタ
  LexpNodePtr(const LexpNode* ptr = NULL);

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  LexpNodePtr(const LexpNodePtr& src);

  /// @brief 代入演算子
  /// @param[in] src 代入元のオブジェクト
  const LexpNodePtr&
  operator=(const LexpNodePtr& src);

  /// @brief デストラクタ
  ~LexpNodePtr();

  
public:

  /// @brief 中身を取り出す演算子
  operator const LexpNode*() const;

  /// @brief 中身を取り出す演算子
  const LexpNode*
  operator->() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実際のポインタ
  const LexpNode* mPtr;
  
};

typedef vector<LexpNodePtr> LexpNodeList;
typedef vector<LexpNodePtr>::iterator LexpNode_itr;
typedef vector<LexpNodePtr>::const_iterator LexpNode_citr;
typedef hash_map<tVarId, LexpNodePtr> VarLexpNodeMap;



//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] ptr Cポインタ
inline
LexpNodePtr::LexpNodePtr(const LexpNode* ptr) :
  mPtr(ptr)
{
  if ( mPtr ) {
    mPtr->inc_ref();
  }
}

// @brief コピーコンストラクタ
// @param[in] src コピー元のオブジェクト
inline
LexpNodePtr::LexpNodePtr(const LexpNodePtr& src) :
  mPtr(src.mPtr)
{
  if ( mPtr ) {
    mPtr->inc_ref();
  }
}

// @brief 代入演算子
// @param[in] src 代入元のオブジェクト
inline
const LexpNodePtr&
LexpNodePtr::operator=(const LexpNodePtr& src)
{
  if ( src.mPtr ) {
    src.mPtr->inc_ref();
  }
  if ( mPtr ) {
    mPtr->dec_ref();
  }
  mPtr = src.mPtr;
  return *this;
}

// @brief デストラクタ
inline
LexpNodePtr::~LexpNodePtr()
{
  if ( mPtr ) {
    mPtr->dec_ref();
  }
}

// @brief 中身を取り出す演算子
inline
LexpNodePtr::operator const LexpNode*() const
{
  return mPtr;
}

// @brief 中身を取り出す演算子
inline
const LexpNode*
LexpNodePtr::operator->() const
{
  return mPtr;
}

END_NAMESPACE_YM_LEXP

#endif // YM_LEXP_LOGEXPR_H
