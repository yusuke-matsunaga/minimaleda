#ifndef LIBYM_AIG_AIGMGRIMPL_H
#define LIBYM_AIG_AIGMGRIMPL_H

/// @file libym_aig/AigMgrImpl.h
/// @brief AigMgrImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: AigMgrImpl.h 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_aig/AigMgr.h"
#include "ym_aig/AigNode.h"
#include "ym_utils/Alloc.h"
#include "ym_utils/ItvlMgr.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
/// @class AigMgrImpl AigMgrImpl.h "AigMgrImpl.h"
/// @brief AigMgr の実装クラス
//////////////////////////////////////////////////////////////////////
class AigMgrImpl
{
public:
  
  /// @brief コンストラクタ
  AigMgrImpl();
  
  /// @brief デストラクタ
  virtual
  ~AigMgrImpl();
  

public:
  //////////////////////////////////////////////////////////////////////
  // AigMgr の機能を実現しているメンバ関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 入力ノード数を得る．
  size_t
  input_num() const;

  /// @brief 入力ノードを取り出す．
  /// @param[in] pos 入力番号 ( 0 <= pos < input_num() )
  AigNode*
  input_node(size_t pos) const;

  /// @brief ノード数を得る．
  size_t
  node_num() const;

  /// @brief ノードを取り出す．
  /// @param[in] pos ノード番号 ( 0 <= pos < node_num() )
  /// @note ANDノードの他に入力ノードも含まれる．
  AigNode*
  node(size_t pos) const;
  
  /// @brief 定数0関数をつくる．
  AigHandle
  make_zero();

  /// @brief 定数1関数をつくる．
  AigHandle
  make_one();

  /// @brief 外部入力を作る．
  AigHandle
  make_input();
  
  /// @brief 2つのノードの AND を取る．
  /// @param[in] edge1, edge2 入力の AIG ハンドル
  AigHandle
  make_and(AigHandle edge1,
	   AigHandle edge2);
  
  
private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるメンバ関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief ノードを作る．
  /// @param[in] input 外部入力ノードの時に true にするフラグ
  AigNode*
  new_node(bool input);
  
  /// @brief ハッシュ表を確保する．
  void
  alloc_table(ymuint req_size);

  /// @brief AigNode のハッシュ関数
  static
  ymuint
  hash_func(AigHandle handle1,
	    AigHandle handle2);

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノードを確保するためのアロケータ
  SimpleAlloc mAlloc;

  // ID 番号をキーにしたノードの配列
  // 全てのノードのリスト(mInputNodes + mAndNodes)
  vector<AigNode*> mAllNodes;
  
  // 入力ノードの配列
  vector<AigNode*> mInputNodes;
  
  // AND ノードのハッシュ表
  AigNode** mHashTable;

  // mHashTable のサイズ
  ymuint32 mHashSize;
  
  // ハッシュ表を拡大する目安
  ymuint32 mNextLimit;
  
};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////
  
// @brief 入力ノード数を得る．
inline
size_t
AigMgrImpl::input_num() const
{
  return mInputNodes.size();
}

// @brief 入力ノードを取り出す．
// @param[in] pos 入力番号 ( 0 <= pos < input_num() )
inline
AigNode*
AigMgrImpl::input_node(size_t pos) const
{
  return mInputNodes[pos];
}

// @brief ノード数を得る．
inline
size_t
AigMgrImpl::node_num() const
{
  return mAllNodes.size();
}

// @brief ノードを取り出す．
// @param[in] pos ノード番号 ( 0 <= pos < node_num() )
// @note ANDノードの他に入力ノードも含まれる．
inline
AigNode*
AigMgrImpl::node(size_t pos) const
{
  return mAllNodes[pos];
}

// @brief 定数0関数をつくる．
inline
AigHandle
AigMgrImpl::make_zero()
{
  return AigHandle(NULL, false);
}

// @brief 定数1関数をつくる．
inline
AigHandle
AigMgrImpl::make_one()
{
  return AigHandle(NULL, true);
}

// AigNode のハッシュ関数
inline
ymuint
AigMgrImpl::hash_func(AigHandle handle1,
		      AigHandle handle2)
{
  return handle1.mPackedData + handle2.mPackedData;
}

END_NAMESPACE_YM_AIG

#endif // LIBYM_AIG_AIGMGRIMPL_H
