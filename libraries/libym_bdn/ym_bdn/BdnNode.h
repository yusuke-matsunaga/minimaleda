#ifndef YM_BDN_BDNNODE_H
#define YM_BDN_BDNNODE_H

/// @file ym_bdn/BdnNode.h
/// @brief BdnNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

#include <ym_bdn/bdn_nsdef.h>
#include <ym_bdn/BdnNodeHandle.h>


BEGIN_NAMESPACE_YM_BDN

//////////////////////////////////////////////////////////////////////
/// @class BdnEdge BdnNode.h <ym_bdn/BdnNode.h>
/// @brief ノード間の枝を表すクラス
///
/// - 入力側のノード
/// - 出力側のノード
/// - 出力側のノードの何番目(0 or 1)の入力か
/// の情報を持つ．
/// @sa BdnNode BdNetwork
//////////////////////////////////////////////////////////////////////
class BdnEdge :
  public DlElem
{
  friend class BdnNode;
  friend class BdNetwork;

public:

  /// @brief 入力側のノードを得る．
  /// @retval 入力側のノード 通常の枝の場合
  /// @retval NULL 定数0に接続している枝の場合
  const BdnNode*
  from() const;

  /// @brief 出力側のノードを得る．
  /// @return 出力側のノードを返す．
  const BdnNode*
  to() const;

  /// @brief 入力側のノードを得る．
  /// @retval 入力側のノード 通常の枝の場合
  /// @retval NULL 定数0に接続している枝の場合
  BdnNode*
  from();

  /// @brief 出力側のノードを得る．
  /// @return 出力側のノードを返す．
  BdnNode*
  to();

  /// @brief 出力側のノードの何番目の入力かを示す．
  ymuint
  pos() const;

  /// @brief 出力ノードに接続している時 true を返す．
  bool
  is_poedge() const;


private:
  //////////////////////////////////////////////////////////////////////
  // プライベートメンバ関数
  // これらは friend class からしか呼ぶことができない．
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  BdnEdge();

  /// @brief デストラクタ
  ~BdnEdge();

  /// @brief from ノードをセットする．
  void
  set_from(BdnNode* from);

  /// @brief to ノードをセットする．
  void
  set_to(BdnNode* to,
	 ymuint pos);


private:
  //////////////////////////////////////////////////////////////////////
  // プライベートメンバ変数
  //////////////////////////////////////////////////////////////////////

  // 入力側のノード
  BdnNode* mFrom;

  // 出力側のノード
  BdnNode* mTo;

  // 種々のフラグ
  ymuint32 mFlags;


private:
  //////////////////////////////////////////////////////////////////////
  // 定数
  //////////////////////////////////////////////////////////////////////

  static
  const int kIposShift = 0;

  static
  const ymuint32 kIposMask = (1U << kIposShift);

};


//////////////////////////////////////////////////////////////////////
/// @class BdnNode BdnNode.h <ym_bdn/BdnNode.h>
/// @brief ノードを表すクラス
///
/// ノードの種類は
/// - 外部入力ノード
/// - 外部出力ノード
/// - ラッチノード
/// - 論理ノード
/// の 4種類がある．
///
/// 全てのノードはID番号をもつ．
/// 外部入力ノードと外部出力ノードはそれぞれ外部入力番号，外部出力番号を持つ．
/// ラッチノードは1つのファンインとリセット値(0/1/2)を持つ．
/// 論理ノードは2つのファンインと機能コードを持つ．
/// 外部出力以外のノードは複数のファンアウトを持つ．
///
/// @sa BdnEdge BdNetwork
//////////////////////////////////////////////////////////////////////
class BdnNode :
  public DlElem
{
  friend class BdNetwork;

public:
  /// @brief ノードの型
  enum tType {
    /// @brief 外部入力ノード
    kINPUT  = 0,
    /// @brief 外部出力ノード
    kOUTPUT = 1,
    /// @brief ラッチノード
    kLATCH  = 2,
    /// @brief 論理ノード
    kLOGIC  = 3
  };


private:

  /// @brief コンストラクタ
  BdnNode();

  /// @brief デストラクタ
  ~BdnNode();


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 構造に関する情報を取り出す関数
  /// @{

  /// @brief ID 番号の取得
  /// @return ID 番号を返す．
  /// @note ID 番号はノードの生成時に BdNetwork により自動的に割り振られる．
  /// @sa BdNetwork
  ymuint
  id() const;

  /// @brief ID を表す文字列の取得
  /// @note デバッグ時にしか意味を持たない
  string
  id_str() const;

  /// @brief タイプを得る．
  tType
  type() const;

  /// @brief 入力ノードの時に true を返す．
  bool
  is_input() const;

  /// @brief 出力ノードの時に true を返す．
  bool
  is_output() const;

  /// @brief ラッチノードの時に true を返す．
  bool
  is_latch() const;

  /// @brief 論理ノードの時に true を返す．
  bool
  is_logic() const;

  /// @brief 外部入力番号を返す．
  /// @note 外部入力ノードでない場合の値は不定
  ymuint
  input_id() const;

  /// @brief 外部出力番号を返す．
  /// @note 外部出力ノードでない場合の値は不定
  ymuint
  output_id() const;

  /// @brief 出力ノードの極性を返す．
  /// @return 反転していたら true を返す．
  /// @note 出力ノードおよびラッチノードの場合のみ意味を持つ．
  bool
  output_inv() const;

  /// @brief リセット値を得る．
  /// @retval 0 0
  /// @retval 1 1
  /// @retval 2 不定
  /// @note ラッチノードの場合のみ意味を持つ．
  int
  reset_val() const;

  /// @brief 機能コードを得る．
  /// @note 論理ノードの場合のみ意味を持つ．
  /// @note 機能コードは2入力の真理値表(4bit)
  ymuint32
  fcode() const;

  /// @brief ファンインのノードを得る．
  /// @param[in] pos 入力番号(0 or 1)
  /// @return pos 番めのファンインのノード
  /// @note 該当するファンインがなければ NULL を返す．
  const BdnNode*
  fanin(ymuint pos) const;

  /// @brief ファンインのノードを得る．
  /// @param[in] pos 入力番号(0 or 1)
  /// @return pos 番めのファンインのノード
  /// @note 該当するファンインがなければ NULL を返す．
  BdnNode*
  fanin(ymuint pos);

  /// @brief ファンイン0のノードを得る．
  /// @return 0番めのファンインのノード
  const BdnNode*
  fanin0() const;

  /// @brief ファンイン0のノードを得る．
  /// @return 0番めのファンインのノード
  BdnNode*
  fanin0();

  /// @brief ファンイン1のノードを得る．
  /// @return 1番めのファンインのノード
  /// @note 該当するファンインがなければ NULL を返す．
  const BdnNode*
  fanin1() const;

  /// @brief ファンイン1のノードを得る．
  /// @return 1番めのファンインのノード
  /// @note 該当するファンインがなければ NULL を返す．
  BdnNode*
  fanin1();

  /// @brief ファンアウト数を得る．
  ymuint
  fanout_num() const;

  /// @brief ファンアウトリストを得る．
  const BdnFanoutList&
  fanout_list() const;

  /// @brief 出力ノードにファンアウトしているとき true を返す．
  bool
  pomark() const;

  /// @brief レベルを得る．
  ymuint
  level() const;

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // プライベートメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief タイプを入力に設定する．
  /// @param[in] id 入力番号
  void
  set_input(ymuint id);

  /// @brief タイプを出力に設定する．
  /// @param[in] id 出力番号
  void
  set_output(ymuint id);

  /// @brief タイプをラッチに設定する．
  /// @param[in] reset_val リセット値
  void
  set_latch(int reset_val);

  /// @brief 出力もしくはラッチの入力の極性を設定する．
  /// @param[in] inv 極性
  void
  set_inv(bool inv);

  /// @brief タイプを論理に設定する．
  void
  set_logic(ymuint fcode);

  /// @brief ファンアウトに出力が含まれているか調べ pomark をセットする．
  void
  scan_po();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID 番号
  ymuint32 mId;

  // タイプ(3bit) + POマーク(1bit) + 機能コード(4bit)
  //                               + 出力極性(1bit) + 出力番号
  //                               +                + 入力番号
  //                               + 出力極性(1bit) + リセット値(2bit)
  ymuint32 mFlags;

  // ファンインの枝(そのもの)の配列
  BdnEdge mFanins[2];

  // ファンアウトの枝のリスト
  BdnFanoutList mFanoutList;

  // ハッシュ用のリンク
  BdnNode* mLink;

  // レベル
  ymuint32 mLevel;


private:
  //////////////////////////////////////////////////////////////////////
  // 定数の定義
  //////////////////////////////////////////////////////////////////////

  // mFlags の演算で用いる定数
  static
  const int kTypeShift = 0;
  static
  const int kPoShift = 3;
  static
  const int kFcodeShift = 4;
  static
  const int kInvShift = 4;
  static
  const int kIdShift = 5;
  static
  const int kRstShift = 5;

  static
  const ymuint32 kPoMask = 1U << kPoShift;
  static
  const ymuint32 kInvMask = 1U << kInvShift;
  static
  const ymuint32 kRstMask = 3U << kRstShift;

};


//////////////////////////////////////////////////////////////////////
// inline 関数の定義
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// クラス BdnEdge
//////////////////////////////////////////////////////////////////////

// コンストラクタ
inline
BdnEdge::BdnEdge() :
  mFrom(NULL),
  mTo(NULL),
  mFlags(0U)
{
}

// デストラクタ
inline
BdnEdge::~BdnEdge()
{
}

// 入力側のノードを得る．
inline
const BdnNode*
BdnEdge::from() const
{
  return mFrom;
}

// 出力側のノードを得る．
inline
const BdnNode*
BdnEdge::to() const
{
  return mTo;
}

// 入力側のノードを得る．
inline
BdnNode*
BdnEdge::from()
{
  return mFrom;
}

// 出力側のノードを得る．
inline
BdnNode*
BdnEdge::to()
{
  return mTo;
}

// 出力側のノードの何番目の入力かを示す．
inline
ymuint
BdnEdge::pos() const
{
  return (mFlags >> kIposShift) & 1U;
}

// @brief from ノードをセットする．
inline
void
BdnEdge::set_from(BdnNode* from)
{
  mFrom = from;
}

// @brief to ノードをセットする．
inline
void
BdnEdge::set_to(BdnNode* to,
		ymuint pos)
{
  mTo = to;
  if ( pos ) {
    mFlags |= kIposMask;
  }
  else {
    mFlags &= ~kIposMask;
  }
}

// @brief 出力ノードに接続している時 true を返す．
inline
bool
BdnEdge::is_poedge() const
{
  return to()->is_output();
}


//////////////////////////////////////////////////////////////////////
// クラス BdnNode
//////////////////////////////////////////////////////////////////////

// ID 番号を得る．
inline
ymuint
BdnNode::id() const
{
  return mId;
}

// タイプを得る．
inline
BdnNode::tType
BdnNode::type() const
{
  return static_cast<tType>(mFlags & 7U);
}

// 入力ノードの時に true を返す．
inline
bool
BdnNode::is_input() const
{
  return type() == kINPUT;
}

// 出力ノードの時に true を返す．
inline
bool
BdnNode::is_output() const
{
  return type() == kOUTPUT;
}

// ラッチノードの時に true を返す．
inline
bool
BdnNode::is_latch() const
{
  return type() == kLATCH;
}

// 論理ノードの時に true を返す．
inline
bool
BdnNode::is_logic() const
{
  return type() == kLOGIC;
}

// @brief 外部入力番号を返す．
// @note 外部入力ノードでない場合の値は不定
inline
ymuint
BdnNode::input_id() const
{
  return (mFlags >> kIdShift);
}

// @brief 外部出力番号を返す．
// @note 外部出力ノードでない場合の値は不定
inline
ymuint
BdnNode::output_id() const
{
  return (mFlags >> kIdShift);
}

// @brief 出力ノードの極性を得る．
inline
bool
BdnNode::output_inv() const
{
  return static_cast<bool>((mFlags >> kInvShift) & 1U);
}

// @brief リセット値を得る．
// @retval 0 0
// @retval 1 1
// @retval 2 不定
// @note ラッチノードの場合のみ意味を持つ．
inline
int
BdnNode::reset_val() const
{
  return static_cast<int>((mFlags >> kRstShift) & 3U);
}

// @brief 機能コードを得る．
inline
ymuint
BdnNode::fcode() const
{
  return (mFlags >> kFcodeShift) & 0xf;
}

// @brief ファンインのノードを得る．
inline
const BdnNode*
BdnNode::fanin(ymuint pos) const
{
  return mFanins[pos & 1U].from();
}

// @brief ファンインのノードを得る．
inline
BdnNode*
BdnNode::fanin(ymuint pos)
{
  return mFanins[pos & 1U].from();
}

// @brief ファンイン0のノードを得る．
// @return 0番めのファンインのノード
inline
const BdnNode*
BdnNode::fanin0() const
{
  return mFanins[0].from();
}

// @brief ファンイン0のノードを得る．
// @return 0番めのファンインのノード
inline
BdnNode*
BdnNode::fanin0()
{
  return mFanins[0].from();
}

// @brief ファンイン1のノードを得る．
// @return 1番めのファンインのノード
// @note 該当するファンインがなければ NULL を返す．
inline
const BdnNode*
BdnNode::fanin1() const
{
  return mFanins[1].from();
}

// @brief ファンイン1のノードを得る．
// @return 1番めのファンインのノード
// @note 該当するファンインがなければ NULL を返す．
inline
BdnNode*
BdnNode::fanin1()
{
  return mFanins[1].from();
}

// ファンアウトリストを得る．
inline
const BdnFanoutList&
BdnNode::fanout_list() const
{
  return mFanoutList;
}

// ファンアウト数を得る．
inline
ymuint
BdnNode::fanout_num() const
{
  return mFanoutList.size();
}

// @brief 出力ノードにファンアウトしているとき true を返す．
inline
bool
BdnNode::pomark() const
{
  return static_cast<bool>((mFlags >> kPoShift) & 1U);
}

// @brief レベルを得る．
inline
ymuint
BdnNode::level() const
{
  return mLevel;
}

// タイプを入力に設定する．
inline
void
BdnNode::set_input(ymuint id)
{
  mFlags = static_cast<ymuint32>(kINPUT) | (id << kIdShift);
}

// @brief タイプを出力に設定する．
// @param[in] id 出力番号
inline
void
BdnNode::set_output(ymuint id)
{
  mFlags = static_cast<ymuint32>(kOUTPUT) | (id << kIdShift);
}

// @brief タイプをラッチに設定する．
// @param[in] reset_val リセット値
inline
void
BdnNode::set_latch(int reset_val)
{
  mFlags = static_cast<ymuint32>(kLATCH) | ((reset_val & 3) << kRstShift);
}

// @brief 出力の極性を設定する．
// @param[in] inv 極性
inline
void
BdnNode::set_inv(bool inv)
{
  if ( inv ) {
    mFlags |= kInvMask;
  }
  else {
    mFlags &= ~kInvMask;
  }
}

// タイプを論理に設定する．
inline
void
BdnNode::set_logic(ymuint fcode)
{
  mFlags = static_cast<ymuint32>(kLOGIC) | (fcode << kFcodeShift);
}

END_NAMESPACE_YM_BDN

#endif // YM_BDN_BDNNODE_H
