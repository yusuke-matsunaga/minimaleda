#ifndef CALC_CVF_NODESET_H
#define CALC_CVF_NODESET_H

/// @file calc_cvf/NodeSet.h
/// @brief NodeSet のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: NodeSet.h 1920 2008-12-20 15:52:42Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.

#if HAVE_CONFIG_H
#include "seal_config.h"
#endif


#include "SimNode.h"


BEGIN_NAMESPACE_YM_SEAL_CVF

//////////////////////////////////////////////////////////////////////
/// @class NodeSet NodeSet.h "NodeSet.h"
/// @brief SimNode の集合を表すクラス
//////////////////////////////////////////////////////////////////////
class NodeSet
{
public:

  /// @brief コンストラクタ
  /// @param[in] node_num 全ノード数
  NodeSet(size_t node_num);

  /// @brief デストラクタ
  ~NodeSet();


public:
  
  /// @brief クリアする．
  void
  clear();

  /// @brief ノードを追加する．
  void
  add(SimNode* node);

  /// @brief pos 番めの要素を削除する．
  /// @note といってもこの位置に NULL が書き込まれるだけ．
  void
  del(size_t pos);

  /// @brief 要素数を返す．
  size_t
  num() const;

  /// @brief pos 番めの要素を返す．
  SimNode*
  node(size_t pos) const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノードの配列
  vector<SimNode*> mArray;

  // mArray に含まれているノードのマーク
  vector<bool> mMark;
  
};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] node_num 全ノード数
inline
NodeSet::NodeSet(size_t node_num) :
  mMark(node_num, false)
{
  mArray.reserve(node_num);
}

// @brief デストラクタ
inline
NodeSet::~NodeSet()
{
}
  
// @brief クリアする．
inline
void
NodeSet::clear()
{
  for (vector<SimNode*>::iterator p = mArray.begin();
       p != mArray.end(); ++ p) {
    SimNode* node = *p;
    if ( node ) {
      mMark[node->id()] = false;
    }
  }
  mArray.clear();
}

// @brief ノードを追加する．
inline
void
NodeSet::add(SimNode* node)
{
  if ( !mMark[node->id()] ) {
    mMark[node->id()] = true;
    mArray.push_back(node);
  }
}

// @brief pos 番めの要素を削除する．
// @note といってもこの位置に NULL が書き込まれるだけ．
inline
void
NodeSet::del(size_t pos)
{
  SimNode* node = mArray[pos];
  mMark[node->id()] = false;
  mArray[pos] = NULL;
}

// @brief 要素数を返す．
inline
size_t
NodeSet::num() const
{
  return mArray.size();
}

// @brief pos 番めの要素を返す．
inline
SimNode*
NodeSet::node(size_t pos) const
{
  return mArray[pos];
}

END_NAMESPACE_YM_SEAL_CVF

#endif // CALC_CVF_NODESET_H
