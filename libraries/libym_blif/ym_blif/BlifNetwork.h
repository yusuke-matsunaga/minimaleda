#ifndef YM_BLIF_BLIFNETWORK_H
#define YM_BLIF_BLIFNETWORK_H

/// @file ym_blif/BlifNetwork.h
/// @brief BlifNetwork のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BlifNetwork.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_blif/blif_nsdef.h"
#include "ym_utils/Alloc.h"


BEGIN_NAMESPACE_YM_BLIF

class BlifNetworkHandler;

//////////////////////////////////////////////////////////////////////
/// @class BlifNode BlifNetwork.h <ym_blif/BlifNetwork.h>
/// @ingroup BlifGroup
/// @brief blif ファイルの表すネットワークのノード
/// @sa BlifParser
//////////////////////////////////////////////////////////////////////
class BlifNode
{
  friend class BlifNetwork;
  friend class BlifNetworkHandler;
  
public:

  /// @brief ノードの種類
  enum tType {
    /// @brief 未定義
    kUndef,
    /// @brief 外部入力
    kInput,
    /// @brief 論理
    kLogic,
    /// @brief ラッチ
    kLatch
  };
  
  
private:

  /// @brief コンストラクタ
  /// @param[in] id ID 番号
  BlifNode(ymuint32 id);

  /// @brief デストラクタ
  ~BlifNode();


public:

  /// @brief ID を返す．
  ymuint32
  id() const;
  
  /// @brief 名前を返す．
  const char*
  name() const;
  
  /// @brief 型を返す．
  tType
  type() const;

  /// @brief ファンイン数を得る．
  ymuint32
  ni() const;
  
  /// @brief ファンインを求める．
  /// @param[in] pos 入力位置 ( 0 <= pos < ni() )
  const BlifNode*
  fanin(ymuint32 pos) const;
  
  /// @brief カバーのキューブ数を得る．
  ymuint32
  nc() const;

  /// @brief 入力キューブのパタンを得る．
  /// @param[in] c_pos キューブの位置 ( 0 <= c_pos < nc() )
  /// @param[in] i_pos 入力位置 ( 0 <= i_pos < ni() )
  char
  cube_pat(ymuint32 c_pos,
	   ymuint32 i_pos) const;

  /// @brief 出力キューブを表すパタンを得る．
  char
  opat() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  // ここのメモリ領域はすべて BlifNetwork::mAlloc が管理する．
  //////////////////////////////////////////////////////////////////////
  
  // ID 番号
  ymuint32 mId;
  
  // 名前
  const char* mName;
  
  // 型
  tType mType;

  // ファンイン数
  ymuint32 mNi;
  
  // ファンインの配列
  BlifNode** mFanins;
  
  // キューブ数
  ymuint32 mNc;
  
  // カバーを表す文字の配列
  // 中身は '0', '1', '-' のいづれか
  // サイズは ni() * nc()
  // end-of-string マーカはないので注意
  const char* mCover;
  
  // 出力キューブを表す文字　'0' or '1'
  char mOpat;
  
};


//////////////////////////////////////////////////////////////////////
/// @class BlifNetwork BlifNetwork.h <ym_blif/BlifNetwork.h>
/// @ingroup BlifGroup
/// @brief blif ファイルの表しているネットワーク
/// @sa BlifNetworkReader
//////////////////////////////////////////////////////////////////////
class BlifNetwork
{
  friend class BlifNetworkHandler;
  
public:

  /// @brief コンストラクタ
  BlifNetwork();
  
  /// @brief デストラクタ
  ~BlifNetwork();


public:

  /// @brief model 名を得る．
  string
  name() const;
  
  /// @brief ノードの ID 番号の最大値 + 1 を求める．
  ymuint32
  max_node_id() const;
  
  /// @brief ID 番号からノードを得る．
  /// @param[in] id ID 番号
  /// @note 使われていない ID の場合には NULL が返される．
  const BlifNode*
  node(ymuint32 id) const;
  
  /// @brief 外部入力数を得る．
  ymuint32
  npi() const;
  
  /// @brief 外部入力を得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < npi() )
  const BlifNode*
  pi(ymuint32 pos) const;

  /// @brief 外部出力数を得る．
  ymuint32
  npo() const;
  
  /// @brief 外部出力を得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < npo() )
  const BlifNode*
  po(ymuint32 pos) const;
  
  /// @brief ラッチ数を得る．
  ymuint32
  nff() const;

  /// @brief ラッチを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < nff() )
  const BlifNode*
  ff(ymuint32 pos) const;
  
  /// @brief 論理ノード数を得る．
  ymuint32
  nlogic() const;

  /// @brief 論理ノードを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < nlogic() )
  const BlifNode*
  logic(ymuint32 pos) const;
  
  /// @brief 内容を blif 形式で出力する．
  /// @param[in] s 出力先のストリーム
  void
  write_blif(ostream& s) const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // BlifNetworkReader のみが用いる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をクリアする．
  void
  clear();
  
  /// @brief ノードを取り出す．
  /// @param[in] id ID 番号
  /// @note なければ作成する．
  BlifNode*
  get_node(ymuint32 id);
  
  /// @brief ノード名を設定する．
  /// @param[in] node 対象のノード
  /// @param[in] name 名前
  void
  set_node_name(BlifNode* node,
		const char* name);
  
  /// @brief ノードを外部入力に設定する．
  /// @note 既に他の型に設定されていたら false を返す．
  bool
  set_input_type(BlifNode* node);

  /// @brief ノードを論理ノードに設定する．
  /// @param[in] ni ファンイン数
  /// @param[in] nc キューブ数
  /// @param[in] cover_pat 全キューブのパタンをつなげたもの
  /// @param[in] opat 出力のパタン
  /// @note 既に他の型に設定されていたら false を返す．
  bool
  set_logic_type(BlifNode* node,
		 ymuint32 ni,
		 ymuint32 nc,
		 const char* cover_pat,
		 char opat);

  /// @brief ノードをラッチノードに設定する．
  /// @param[in] rval リセット値 ( '0', '1', ' ' のいづれか )
  /// @note 既に他の型に設定されていたら false を返す．
  bool
  set_latch_type(BlifNode* node,
		 char rval);
  
  /// @brief BlifNode のファンイン領域を確保する．
  /// @param[in] node 対象のノード
  /// @param[in] ni ファンイン数
  void
  alloc_fanin(BlifNode* node,
	      ymuint32 ni);
  
  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリアロケータ
  SimpleAlloc mAlloc;
  
  // 名前 (.model)
  string mName;
  
  // ID をキーにしてノードを収めた配列
  // ID は穴が空いているかもしれない．
  vector<BlifNode*> mNodeArray;
  
  // 外部入力の配列
  vector<BlifNode*> mPIArray;

  // 外部出力の配列
  vector<BlifNode*> mPOArray;

  // ラッチの配列
  vector<BlifNode*> mFFArray;

  // 論理ノードの配列
  vector<BlifNode*> mLogicArray;
  
};


//////////////////////////////////////////////////////////////////////
// BlifNode のインライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ID を返す．
inline
ymuint32
BlifNode::id() const
{
  return mId;
}

// @brief 名前を返す．
inline
const char*
BlifNode::name() const
{
  return mName;
}
  
// @brief 型を返す．
inline
BlifNode::tType
BlifNode::type() const
{
  return mType;
}

// @brief ファンイン数を得る．
inline
ymuint32
BlifNode::ni() const
{
  return mNi;
}
  
// @brief ファンインを求める．
// @param[in] pos 入力位置 ( 0 <= pos < ni() )
inline
const BlifNode*
BlifNode::fanin(ymuint32 pos) const
{
  return mFanins[pos];
}
  
// @brief カバーのキューブ数を得る．
inline
ymuint32
BlifNode::nc() const
{
  return mNc;
}

// @brief 入力キューブのパタンを得る．
// @param[in] c_pos キューブの位置 ( 0 <= c_pos < nc() )
// @param[in] i_pos 入力位置 ( 0 <= i_pos < ni() )
inline
char
BlifNode::cube_pat(ymuint32 c_pos,
		   ymuint32 i_pos) const
{
  return mCover[c_pos * ni() + i_pos];
}

// @brief 出力キューブを表すパタンを得る．
inline
char
BlifNode::opat() const
{
  return mOpat;
}


//////////////////////////////////////////////////////////////////////
//  BlifNetwork のインライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief model 名を得る．
inline
string
BlifNetwork::name() const
{
  return mName;
}
  
// @brief ノードの ID 番号の最大値 + 1 を求める．
inline
ymuint32
BlifNetwork::max_node_id() const
{
  return mNodeArray.size();
}
  
// @brief ID 番号からノードを得る．
// @param[in] id ID 番号
// @note 使われていない ID の場合には NULL が返される．
inline
const BlifNode*
BlifNetwork::node(ymuint32 id) const
{
  return mNodeArray[id];
}

// @brief 外部入力数を得る．
inline
ymuint32
BlifNetwork::npi() const
{
  return mPIArray.size();
}
  
// @brief 外部入力を得る．
// @param[in] pos 位置番号 ( 0 <= pos < npi() )
inline
const BlifNode*
BlifNetwork::pi(ymuint32 pos) const
{
  return mPIArray[pos];
}

// @brief 外部出力数を得る．
inline
ymuint32
BlifNetwork::npo() const
{
  return mPOArray.size();
}
  
// @brief 外部出力を得る．
// @param[in] pos 位置番号 ( 0 <= pos < npo() )
inline
const BlifNode*
BlifNetwork::po(ymuint32 pos) const
{
  return mPOArray[pos];
}
  
// @brief ラッチ数を得る．
inline
ymuint32
BlifNetwork::nff() const
{
  return mFFArray.size();
}

// @brief ラッチを得る．
// @param[in] pos 位置番号 ( 0 <= pos < nff() )
inline
const BlifNode*
BlifNetwork::ff(ymuint32 pos) const
{
  return mFFArray[pos];
}
  
// @brief 論理ノード数を得る．
inline
ymuint32
BlifNetwork::nlogic() const
{
  return mLogicArray.size();
}

// @brief 論理ノードを得る．
// @param[in] pos 位置番号 ( 0 <= pos < nlogic() )
// @note 論理ノードはトポロジカル順に整列している．
inline
const BlifNode*
BlifNetwork::logic(ymuint32 pos) const
{
  return mLogicArray[pos];
}

END_NAMESPACE_YM_BLIF

#endif // YM_BLIF_BLIFNETWORK_H
