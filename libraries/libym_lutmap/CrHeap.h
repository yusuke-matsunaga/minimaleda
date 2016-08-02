#ifndef LIBYM_LUTMAP_CRHEAP_H
#define LIBYM_LUTMAP_CRHEAP_H

/// @file libym_lutmap/CrHeap.h
/// @brief CrHeap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CrHeap.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_lutmap/lutmap_nsdef.h"


BEGIN_NAMESPACE_YM_LUTMAP

class CrNode;

//////////////////////////////////////////////////////////////////////
/// @class CrHeap CrHeap.h "CrHeap.h"
/// @brief CrNode 用のヒープ木
//////////////////////////////////////////////////////////////////////
class CrHeap
{
public:

  /// @brief コンストラクタ
  CrHeap();

  /// @brief デストラクタ
  ~CrHeap();


public:

  /// @brief 初期化する．
  /// @param[in] max_n 最大要素数
  void
  init(ymuint max_n);
  
  /// @brief ヒープに追加する．
  void
  put(CrNode* n);

  /// @brief ヒープの先頭要素を抜き出す．
  CrNode*
  get();

  /// @brief ヒープから削除する．
  void
  remove(CrNode* n);

  // 値の更新に伴ってヒープの再構築を行う．
  void
  update(CrNode* n,
	 ymuint gain);


private:
  
  // 引数の位置にある要素を適当な位置まで沈めてゆく
  void
  move_down(ymuint pos);

  // 引数の位置にある要素を適当な位置まで上げてゆく
  void
  move_up(ymuint pos);

  // pos の位置に node を置く
  void
  set(ymuint pos,
      CrNode* node);

  // pos の位置の要素を返す．
  CrNode*
  get(ymuint pos);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ゲインを指標としたヒープ木
  vector<CrNode*> mHeap;

  // ヒープ木の要素数
  ymuint32 mNum;

};

END_NAMESPACE_YM_LUTMAP

#endif // LIBYM_LUTMAP_CRHEAP_H
