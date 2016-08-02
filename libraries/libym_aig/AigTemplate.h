#ifndef LIBYM_AIG_AIGTEMPLATE_H
#define LIBYM_AIG_AIGTEMPLATE_H

/// @file libym_aig/AigTemplate.h
/// @brief AigTemplate のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: AigTemplate.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_aig/aig_nsdef.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
/// @brief AIG を構築するためのテンプレート
/// ノード番号について
/// - 0 は定数0
/// - 1 〜 leaf_num() は入力ノード
/// - leaf_num() + 1 〜 leaf_num() + and_num() は AND ノード
//////////////////////////////////////////////////////////////////////
class AigTemplate
{
public:
  
  /// @brief コンストラクタ
  AigTemplate();
  
  /// @brief デストラクタ
  ~AigTemplate();
  

public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取り出す関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 入力数を返す．
  ymuint
  leaf_num() const;

  /// @brief AND ノード数を返す．
  ymuint
  and_num() const;

  /// @brief AND ノードのファンイン0を返す．
  /// @param[in] pos ノード番号 ( leaf_num() < pos <= leaf_num() + and_num() )
  /// @return ノード番号 x 2 + 極性を返す．
  /// @note AND ノード以外の場合には 0 を返す．
  ymuint
  fanin0(ymuint pos) const;

  /// @brief AND ノードのファンイン0のノード番号を返す．
  /// @param[in] pos ノード番号 ( leaf_num() < pos <= leaf_num() + and_num() )
  /// @note AND ノード以外の場合には 0 を返す．
  ymuint
  fanin0_node(ymuint pos) const;

  /// @brief AND ノードのファンイン0の極性を返す．
  /// @param[in] pos ノード番号 ( leaf_num() < pos <= leaf_num() + and_num() )
  /// @note AND ノード以外の場合には 0 を返す．
  ymuint
  fanin0_pol(ymuint pos) const;

  /// @brief AND ノードのファンイン1を返す．
  /// @param[in] pos ノード番号 ( leaf_num() < pos <= leaf_num() + and_num() )
  /// @return ノード番号 x 2 + 極性を返す．
  /// @note AND ノード以外の場合には 0 を返す．
  ymuint
  fanin1(ymuint pos) const;

  /// @brief AND ノードのファンイン1のノード番号を返す．
  /// @param[in] pos ノード番号 ( leaf_num() < pos <= leaf_num() + and_num() )
  /// @note AND ノード以外の場合には 0 を返す．
  ymuint
  fanin1_node(ymuint pos) const;

  /// @brief AND ノードのファンイン1の極性を返す．
  /// @param[in] pos ノード番号 ( leaf_num() < pos <= leaf_num() + and_num() )
  /// @note AND ノード以外の場合には 0 を返す．
  ymuint
  fanin1_pol(ymuint pos) const;
  
  /// @brief 出力の極性を返す．
  /// @retval 0 肯定
  /// @retval 1 否定
  ymuint
  opol() const;
  

public:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 初期化する．
  /// @param[in] leaf_num 入力数
  /// @param[in] and_num AND ノード数
  void
  init(ymuint leaf_num,
       ymuint and_num);
  
  /// @brief AND ノードの内容をセットする．
  /// @param[in] pos ノード番号
  /// @param[in] fanin0 ファンイン0の情報
  /// @param[in] fanin1 ファンイン1の情報
  void
  set_and(ymuint pos,
	  ymuint fanin0,
	  ymuint fanin1);
  
  /// @brief 出力の極性をセットする．
  void
  set_opol(ymuint pol);

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力ノード数 + 出力の極性
  ymuint8 mLeafNum;

  // AND ノード数
  ymuint8 mAndNum;

  // AND ノードの情報を収めた配列
  // サイズは mAndNum * 2
  ymuint8* mNodeArray;
  
};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 入力数を返す．
inline
ymuint
AigTemplate::leaf_num() const
{
  return mLeafNum >> 1;
}

// @brief AND ノード数を返す．
inline
ymuint
AigTemplate::and_num() const
{
  return mAndNum;
}

// @brief AND ノードのファンイン0を返す．
// @param[in] pos ノード番号 ( leaf_num() < pos <= leaf_num() + and_num() )
// @return ノード番号 x 2 + 極性を返す．
// @note AND ノード以外の場合には 0 を返す．
inline
ymuint
AigTemplate::fanin0(ymuint pos) const
{
  if ( pos <= leaf_num() || pos > leaf_num() + and_num() ) {
    return 0;
  }
  else {
    return mNodeArray[pos * 2];
  }
}

// @brief AND ノードのファンイン0のノード番号を返す．
// @param[in] pos ノード番号 ( leaf_num() < pos <= leaf_num() + and_num() )
// @note AND ノード以外の場合には 0 を返す．
inline
ymuint
AigTemplate::fanin0_node(ymuint pos) const
{
  return (fanin0(pos) >> 1);
}

// @brief AND ノードのファンイン0の極性を返す．
// @param[in] pos ノード番号 ( leaf_num() < pos <= leaf_num() + and_num() )
// @note AND ノード以外の場合には 0 を返す．
inline
ymuint
AigTemplate::fanin0_pol(ymuint pos) const
{
  return (fanin0(pos) & 1U);
}

// @brief AND ノードのファンイン1を返す．
// @param[in] pos ノード番号 ( leaf_num() < pos <= leaf_num() + and_num() )
// @return ノード番号 x 2 + 極性を返す．
// @note AND ノード以外の場合には 0 を返す．
inline
ymuint
AigTemplate::fanin1(ymuint pos) const
{
  if ( pos <= leaf_num() || pos > leaf_num() + and_num() ) {
    return 0;
  }
  else {
    return mNodeArray[pos * 2 + 1];
  }
}

// @brief AND ノードのファンイン1のノード番号を返す．
// @param[in] pos ノード番号 ( leaf_num() < pos <= leaf_num() + and_num() )
// @note AND ノード以外の場合には 0 を返す．
inline
ymuint
AigTemplate::fanin1_node(ymuint pos) const
{
  return (fanin1(pos) >> 1);
}

// @brief AND ノードのファンイン1の極性を返す．
// @param[in] pos ノード番号 ( leaf_num() < pos <= leaf_num() + and_num() )
// @note AND ノード以外の場合には 0 を返す．
inline
ymuint
AigTemplate::fanin1_pol(ymuint pos) const
{
  return (fanin1(pos) & 1U);
}
  
// @brief 出力の極性を返す．
// @retval 0 肯定
// @retval 1 否定
inline
ymuint
AigTemplate::opol() const
{
  return (mLeafNum & 1U);
}

END_NAMESPACE_YM_AIG

#endif // LIBYM_AIG_AIGTEMPLATE_H
