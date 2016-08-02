#ifndef LIBYM_BNET_BNODEMGR_H
#define LIBYM_BNET_BNODEMGR_H

/// @file libym_bnet/BNodeMgr.h
/// @brief BNodeMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BNodeMgr.h 958 2007-08-28 05:38:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_bnet/BNetwork.h"
#include "ym_utils/Alloc.h"


BEGIN_NAMESPACE_YM_BNET

//////////////////////////////////////////////////////////////////////
/// @class BNodeMgr BNodeMgr.h "BNodeMgr.h"
/// @brief BNode を管理するクラス
//////////////////////////////////////////////////////////////////////
class BNodeMgr
{
public:

  /// @brief 唯一のオブジェクトを取り出す．
  static
  BNodeMgr&
  the_obj();


private:

  /// @brief コンストラクタ．
  BNodeMgr();
  
  /// @brief デストラクタ
  ~BNodeMgr();


public:
  
  /// @brief BNode を確保する．
  BNode*
  alloc_node();

  /// @brief BNode を削除する．
  void
  free_node(BNode* node);

  /// @brief BNode の名前用の文字列領域を確保する．
  const char*
  alloc_name(const char* src);

  /// @brief alloc_name() で確保した文字列領域を開放する．
  void
  free_name(const char* src);

  /// @brief BNodeEdge の配列を確保する．
  BNodeEdge*
  alloc_edgearray(size_t n);

  /// @brief BNodeEdge の配列を削除する．
  void
  free_edgearray(size_t n,
		 BNodeEdge* array);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // BNode 用の領域を確保するクラス
  UnitAlloc mNodeAlloc;

  // 文字列領域を確保するクラス
  FragAlloc mNameAlloc;
  
  // BNodeEdge の配列用の領域を確保するクラス
  FragAlloc mEdgeAlloc;

};

END_NAMESPACE_YM_BNET

#endif // LIBYM_BNET_BNODEMGR_H
