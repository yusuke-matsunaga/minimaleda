/// @file libym_aig/tests/GpNode.h
/// @brief GpNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: GpNode.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"
#include "ym_utils/Alloc.h"


BEGIN_NAMESPACE_YM

class GpNode;

//////////////////////////////////////////////////////////////////////
/// @class GpHandle GpNode.h "GpNode.h"
/// @brief GpNode + 否定属性の情報を持つハンドルクラス
//////////////////////////////////////////////////////////////////////
class GpHandle
{
  friend class GpMgr;
  
public:

  /// @brief 空のコンストラクタ
  GpHandle();

  /// @brief 普通のコンストラクタ
  GpHandle(GpNode* node,
	   bool inv = false);

  /// @brief デストラクタ
  ~GpHandle();


public:
  //////////////////////////////////////////////////////////////////////
  // 内部の情報を取り出すメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードを取り出す．
  GpNode*
  node() const;

  /// @brief 否定属性を取り出す．
  bool
  inv() const;
  
  /// @brief 定数0を表しているとき true を返す．
  bool
  is_const0() const;

  /// @brief 定数1を表しているとき true を返す．
  bool
  is_const1() const;
  
  /// @brief 等価比較演算子
  bool
  operator==(const GpHandle right) const;
  

public:
  //////////////////////////////////////////////////////////////////////
  // 演算子
  //////////////////////////////////////////////////////////////////////

  /// @brief 極性を入れ替える．
  GpHandle
  operator~() const;
  

private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を直接指定したコンストラクタ
  GpHandle(ympuint data);

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノードのポインタ + 否定属性
  ympuint mNodePol;
  
};


//////////////////////////////////////////////////////////////////////
/// @class GpNode GpNode.h "GpNode.h"
/// @brief genpat で用いられるパタンを表すノード
//////////////////////////////////////////////////////////////////////
class GpNode
{
  friend class GpMgr;
private:

  /// @brief コンストラクタ
  /// @param[in] id ノード番号
  GpNode(ymuint id);

  /// @brief デストラクタ
  ~GpNode();

  
public:

  /// @brief ID 番号を返す．
  ymuint32
  id() const;

  /// @brief レベルを返す．
  ymuint32
  level() const;

  /// @brief サイズを返す．
  /// @note DAG の場合にはレベルより少ない数となる．
  ymuint32
  size() const;
  
  /// @brief このパタンが表している関数のパタンを返す．
  ymuint32
  pat() const;
  
  /// @brief 入力ノードの時 true を返す．
  bool
  is_input() const;

  /// @brief AND ノードの時 true を返す．
  bool
  is_and() const;

  /// @brief 入力番号を返す．
  ymuint32
  input_id() const;
  
  /// @brief ファンインのハンドルを返す．
  /// @param[in] pos ファンイン位置番号 (0 or 1)
  GpHandle
  fanin_handle(ymuint pos) const;
  
  /// @brief ファンインを返す．
  /// @param[in] pos ファンイン位置番号 (0 or 1)
  GpNode*
  fanin(ymuint pos) const;
  
  /// @brief ファンインの否定属性を返す．
  /// @param[in] pos ファンイン位置番号 (0 or 1)
  bool
  inv(ymuint pos) const;
  
  /// @brief ファンイン0のハンドルを返す．
  GpHandle
  fanin0_handle() const;
  
  /// @brief ファンイン0を返す．
  GpNode*
  fanin0() const;
  
  /// @brief ファンイン0の否定属性を返す．
  bool
  inv0() const;
  
  /// @brief ファンイン1のハンドルを返す．
  GpHandle
  fanin1_handle() const;
  
  /// @brief ファンイン1を返す．
  GpNode*
  fanin1() const;
  
  /// @brief ファンイン1の否定属性を返す．
  bool
  inv1() const;
  
  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  ymuint32 mId;
  
  // レベル
  ymuint32 mLevel;

  // ノード数
  ymuint32 mSize;
  
  // このパタンが表している関数のパタン
  ymuint32 mPat;

  // 入力番号 + α
  ymuint32 mInputId;

  // ファンイン
  GpHandle mFanin[2];
  
  // 構造ハッシュ用のリンク
  GpNode* mSlink;
  
};


//////////////////////////////////////////////////////////////////////
/// @class GpMgr GpNode.h "GpNode.h"
/// @brief GpNode を管理するためのクラス
//////////////////////////////////////////////////////////////////////
class GpMgr
{
public:

  /// @brief コンストラクタ
  GpMgr();

  /// @brief デストラクタ
  ~GpMgr();
  
  
public:
  
  /// @brief 定数0ノードを生成する．
  GpHandle
  make_const0();

  /// @brief 定数1ノードを生成する．
  GpHandle
  make_const1();
  
  /// @brief 入力ノードを生成する．
  /// @param[in] input_id 入力番号
  /// @note すでに同じ入力番号のノードが存在していたらそれを返す．
  GpHandle
  make_input(ymuint32 input_id,
	     ymuint32 fv);

  /// @brief AND ノードを生成する．
  /// @param[in] fanin0 ファンイン0
  /// @param[in] fanin1 ファンイン1
  /// @note もしも既に同じ構造のノードがあればそれを返す．
  /// @note 場合によってはファンインその物や定数ノードを返すこともある．
  GpHandle
  make_and(GpHandle fanin0,
	   GpHandle fanin1,
	   ymuint32 fv);
  

private:

  /// @brief ノードを生成する下請け関数
  GpNode*
  new_node();
  
  /// @brief node を根とするパタンのノード数を数える．
  ymuint32
  count_size(GpNode* node);
  
  /// @brief ハッシュ関数
  ymuint32
  hash_func(GpHandle fanin0,
	    GpHandle fanin1);
  
  /// @brief ハッシュテーブルを拡大する．
  void
  alloc_table(ymuint req_size);

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // メモリアロケータ
  SimpleAlloc mAlloc;
  
  // 全ノードのリスト
  vector<GpNode*> mNodeList;
  
  // 入力ノードのリスト
  vector<GpNode*> mInputList;

  // ANDノードのリスト
  vector<GpNode*> mAndList;

  // 構造ハッシュ
  GpNode** mHashTable;

  // mHashTable のサイズ
  ymuint32 mHashSize;

  // テーブルを拡大する目安
  ymuint32 mNextLimit;
  
};


//////////////////////////////////////////////////////////////////////
// GpHandle のインライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
inline
GpHandle::GpHandle() :
  mNodePol(0UL)
{
}

// @brief 普通のコンストラクタ
inline
GpHandle::GpHandle(GpNode* node,
		   bool inv) :
  mNodePol(reinterpret_cast<ympuint>(node) | static_cast<ympuint>(inv))
{
}

// @brief 内容を直接指定したコンストラクタ
inline
GpHandle::GpHandle(ympuint data) :
  mNodePol(data)
{
}

// @brief デストラクタ
inline
GpHandle::~GpHandle()
{
}

// @brief ノードを取り出す．
inline
GpNode*
GpHandle::node() const
{
  return reinterpret_cast<GpNode*>(mNodePol & ~1UL);
}

// @brief 否定属性を取り出す．
inline
bool
GpHandle::inv() const
{
  return static_cast<bool>(mNodePol & 1UL);
}
  
// @brief 定数0を表しているとき true を返す．
inline
bool
GpHandle::is_const0() const
{
  return node() == NULL && !inv();
}

// @brief 定数1を表しているとき true を返す．
inline
bool
GpHandle::is_const1() const
{
  return node() == NULL && inv();
}

// @brief 等価比較演算子
inline
bool
GpHandle::operator==(const GpHandle right) const
{
  return mNodePol == right.mNodePol;
}

// @brief 極性を入れ替える．
inline
GpHandle
GpHandle::operator~() const
{
  return GpHandle(mNodePol ^ 1UL);
}


//////////////////////////////////////////////////////////////////////
// GpNode のインライン関数の定義
//////////////////////////////////////////////////////////////////////

// ID 番号を返す．
inline
ymuint32
GpNode::id() const
{
  return mId;
}

// @brief レベルを返す．
inline
ymuint32
GpNode::level() const
{
  return mLevel;
}

// @brief サイズを返す．
inline
ymuint32
GpNode::size() const
{
  return mSize;
}
  
// このパタンが表している関数のパタンを返す．
inline
ymuint32
GpNode::pat() const
{
  return mPat;
}
  
// 入力ノードの時 true を返す．
inline
bool
GpNode::is_input() const
{
  return static_cast<bool>((mInputId & 3U) == 2U);
} 

// AND ノードの時 true を返す．
inline
bool
GpNode::is_and() const
{
  return static_cast<bool>((mInputId & 3U) == 3U);
}

// 入力番号を返す．
inline
ymuint32
GpNode::input_id() const
{
  return (mInputId >> 4);
}
  
// @brief ファンインのハンドルを返す．
// @param[in] pos ファンイン位置番号 (0 or 1)
inline
GpHandle
GpNode::fanin_handle(ymuint pos) const
{
  return mFanin[pos];
}
  
// ファンインを返す．
inline
GpNode*
GpNode::fanin(ymuint pos) const
{
  return fanin_handle(pos).node();
}
  
// ファンインの否定属性を返す．
inline
bool
GpNode::inv(ymuint pos) const
{
  return fanin_handle(pos).inv();
}

// @brief ファンイン0のハンドルを返す．
inline
GpHandle
GpNode::fanin0_handle() const
{
  return mFanin[0];
}

// ファンイン0を返す．
inline
GpNode*
GpNode::fanin0() const
{
  return fanin0_handle().node();
}

// ファンイン0の否定属性を返す．
inline
bool
GpNode::inv0() const
{
  return fanin0_handle().inv();
}

// @brief ファンイン0のハンドルを返す．
inline
GpHandle
GpNode::fanin1_handle() const
{
  return mFanin[1];
}

// ファンイン1を返す．
inline
GpNode*
GpNode::fanin1() const
{
  return fanin1_handle().node();
}

// ファンイン0の否定属性を返す．
inline
bool
GpNode::inv1() const
{
  return fanin1_handle().inv();
}

END_NAMESPACE_YM
