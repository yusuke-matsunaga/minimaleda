
/// @file libym_bnet/BNodeMgr.cc
/// @brief BNodeMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BNodeMgr.cc 1343 2008-03-25 17:15:35Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "BNodeMgr.h"


BEGIN_NAMESPACE_YM_BNET


// 唯一のオブジェクトを返す．
BNodeMgr&
BNodeMgr::the_obj()
{
  // 唯一のオブジェクト
  static BNodeMgr mTheObj;
  return mTheObj;
}

// コンストラクタ
BNodeMgr::BNodeMgr() :
  mNodeAlloc(sizeof(BNode), 1024),
  mNameAlloc(4096),
  mEdgeAlloc(4096)
{
}

// デストラクタ
BNodeMgr::~BNodeMgr()
{
}

// BNode を確保する．
BNode*
BNodeMgr::alloc_node()
{
  void* p = mNodeAlloc.get_memory(sizeof(BNode));
  return new (p) BNode;
}

// @brief BNode を削除する．
void
BNodeMgr::free_node(BNode* node)
{
  mNodeAlloc.put_memory(sizeof(BNode), node);
}

// @brief BNode の名前用の文字列領域を確保する．
const char*
BNodeMgr::alloc_name(const char* src)
{
  if ( src == NULL ) {
    static char dummy[1] = { '\0' };
    src = dummy;
  }
  size_t size = strlen(src) + 1;
  void* p = mNameAlloc.get_memory(size);
  char* ans = static_cast<char*>(p);
  char* dst = ans;
  while ( (*dst ++ = *src ++) ) ;
  return ans;
}

// @brief alloc_name() で確保した文字列領域を開放する．
void
BNodeMgr::free_name(const char* src)
{
  size_t size = strlen(src) + 1;
  mNameAlloc.put_memory(size, const_cast<char*>(src));
}

// @brief BNodeEdge の配列を確保する．
BNodeEdge*
BNodeMgr::alloc_edgearray(size_t n)
{
  void* p = mEdgeAlloc.get_memory(sizeof(BNodeEdge) * n);
  return new (p) BNodeEdge[n];
}

// @brief BNodeEdge の配列を削除する．
void
BNodeMgr::free_edgearray(size_t n,
			 BNodeEdge* array)
{
  mEdgeAlloc.put_memory(sizeof(BNodeEdge) * n, array);
}

END_NAMESPACE_YM_BNET
