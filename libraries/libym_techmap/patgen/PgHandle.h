#ifndef LIBYM_TECHMAP_PATGEN_PGHANDLE_H
#define LIBYM_TECHMAP_PATGEN_PGHANDLE_H

/// @file libym_techmap/patgen/PgHandle.h
/// @brief PgHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "patgen_nsdef.h"


BEGIN_NAMESPACE_YM_TECHMAP_PATGEN

class PgNode;

//////////////////////////////////////////////////////////////////////
/// @class PgHandle PgHandle.h "PgHandle.h"
/// @brief PgNode と極性を表すクラス
//////////////////////////////////////////////////////////////////////
class PgHandle
{
public:

  /// @brief コンストラクタ
  /// @param[in] node ノード
  /// @param[in] inv 反転属性
  explicit
  PgHandle(PgNode* node = NULL,
	   bool inv = false);

  /// @brief デストラクタ
  ~PgHandle();


public:

  /// @brief ノードを取り出す．
  PgNode*
  node() const;

  /// @brief 反転属性を取り出す．
  bool
  inv() const;

  /// @brief 値を設定する．
  /// @param[in] node ノード
  /// @param[in] inv 反転属性
  void
  set(PgNode* node,
      bool inv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ポインタと反転属性を合わせたもの
  ympuint mData;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] node ノード
// @param[in] inv 反転属性
inline
PgHandle::PgHandle(PgNode* node,
		   bool inv)
{
  set(node, inv);
}

// @brief デストラクタ
inline
PgHandle::~PgHandle()
{
}

// @brief ノードを取り出す．
inline
PgNode*
PgHandle::node() const
{
  return reinterpret_cast<PgNode*>(mData & ~1UL);
}

// @brief 反転属性を取り出す．
inline
bool
PgHandle::inv() const
{
  return static_cast<bool>(mData & 1UL);
}

// @brief 値を設定する．
// @param[in] node ノード
// @param[in] inv 反転属性
inline
void
PgHandle::set(PgNode* node,
	      bool inv)
{
  // bool に対する & 1UL は不必要だが念のため．
  mData = reinterpret_cast<ympuint>(node) | (static_cast<ympuint>(inv) & 1UL);
}

END_NAMESPACE_YM_TECHMAP_PATGEN

#endif // LIBYM_TECHMAP_PATGEN_PGHANDLE_H
