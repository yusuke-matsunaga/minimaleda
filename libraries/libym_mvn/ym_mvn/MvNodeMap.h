#ifndef LIBYM_MVN_CONV_MVNODEMAP_H
#define LIBYM_MVN_CONV_MVNODEMAP_H

/// @file libym_mvn/conv/MvNodeMap.h
/// @brief MvNodeMap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/mvn_nsdef.h"
#include "ym_sbj/sbj_nsdef.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvNodeMap MvNodeMap.h "MvNodeMap.h"
/// @brief MvNode と SbjNode の対応を記録するクラス
//////////////////////////////////////////////////////////////////////
class MvNodeMap
{
public:

  /// @brief コンストラクタ
  /// @param[in] n MvNode の ID の最大値
  MvNodeMap(ymuint n);

  /// @brief デストラクタ
  ~MvNodeMap();


public:

  /// @brief 登録する．(1ビット版)
  /// @param[in] mvnode MvNode
  /// @param[in] sbjnode SbjNode
  /// @param[in] inv 反転属性
  void
  put(const MvNode* mvnode,
      SbjNode* sbjnode,
      bool inv);

  /// @brief 登録する．(ベクタ版)
  /// @param[in] mvnode MvNode
  /// @param[in] index ビット位置
  /// @param[in] sbjnode SbjNode
  /// @param[in] inv 反転属性
  void
  put(const MvNode* mvnode,
      ymuint index,
      SbjNode* sbjnode,
      bool inv);

  /// @brief 探す．(1ビット版)
  bool
  get(const MvNode* mvnode,
      SbjNode*& sbjnode,
      bool& inv) const;

  /// @brief 探す．(ベクタ版)
  bool
  get(const MvNode* mvnode,
      ymuint index,
      SbjNode*& sbjnode,
      bool& inv) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  vector<vector<ympuint> > mArray;

};


/// @relates MvNodeMap, MvMgr
/// @brief MvNodeMap の内容を出力する．
void
dump_mvnode_map(ostream& s,
		const MvMgr& mvmgr,
		const MvNodeMap& mvnode_map);

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_CONV_MVNODEMAP_H
