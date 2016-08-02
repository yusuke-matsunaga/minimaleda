#ifndef YM_UTILS_MFSET_H
#define YM_UTILS_MFSET_H

/// @file ym_utils/MFSet.h
/// @brief MFSet のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MFSet.h 958 2007-08-28 05:38:02Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"


BEGIN_NAMESPACE_YM

// クラスの前方参照定義
// ユーザは知る必要はない．
class MFSetCell;


//////////////////////////////////////////////////////////////////////
/// @class MFSet MFSet.h <ym_utils/MFSet.h>
/// @ingroup YmUtils
/// @brief Merge/Find set を実装したクラス
///
/// データ構造とアルゴリズムの教科書でおなじみの Merge/Find-set
/// お互いに素な集合のマージと検索のみを行なう抽象データ型
//////////////////////////////////////////////////////////////////////
class MFSet
{
public:
  typedef unsigned int id_type;

public:

  /// @brief コンストラクタ
  /// @param[in] n 確保したい要素の数．
  MFSet(size_t n);
  
  /// @brief デストラクタ
  ~MFSet();
  
  /// @brief 代表元の検索 (Find)
  /// @param[in] x 要素番号
  /// @retval 要素 x の属する集合の代表元
  /// @retval 0 要素 x が存在していない場合
  id_type
  find(id_type x);

  /// @brief 2つの集合の併合 (Merge)
  /// @param[in] x, y 代表元
  /// @retval 新たな代表元を返す．
  /// @retval 0 x か y が存在していなかった
  /// @note 2つの代表元 x, y の表す集合を併合する．
  /// @note 実は x, y が代表元でない場合，
  /// 内部で find(x), find(y)を呼ぶので処理は行えるが，
  /// 代表元が分かっている場合にはそれを使ったほうが処理は速い．
  id_type
  merge(id_type x,
	id_type y);
  

private:
  
  /// @brief 番号 x の要素セルを取ってくる．
  /// そのような要素がない場合にはNULLを返す．
  MFSetCell*
  get(id_type x);
  

private:

  // 配列の要素数
  size_t mNum;

  // 要素の配列
  MFSetCell* mCellArray;

};

END_NAMESPACE_YM

#endif // YM_UTILS_MFSET_H
