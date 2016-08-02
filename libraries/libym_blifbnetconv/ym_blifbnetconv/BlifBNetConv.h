#ifndef YM_BLIFBNETCONV_BLIFBNETCONV_H
#define YM_BLIFBNETCONV_BLIFBNETCONV_H

/// @file ym_blifbnetconv/BlifBNetConv.h
/// @brief BlifBNetConv のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BlifBNetConv.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_bnet/bnet_nsdef.h"
#include "ym_blif/blif_nsdef.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class BlifBNetConv BlifBNetConv.h <ym_bnet/BlifBNetConv.h>
/// @ingroup BnetGroup
/// @brief BlifNetwork を BNetwork に変換するクラス
/// @sa BNetwork nsYm::nsBlif::BlifNetwork
//////////////////////////////////////////////////////////////////////
class BlifBNetConv
{
public:

  /// @brief コンストラクタ
  BlifBNetConv();

  /// @brief デストラクタ
  ~BlifBNetConv();

  
public:
  
  /// @brief 変換する．
  /// @param[in] blif_network 変換元のネットワーク
  /// @param[in] network 変換先のネットワーク
  bool
  operator()(const BlifNetwork& blif_network,
	     BNetwork& network);

  
private:
  
  /// @brief blif_node に対応した BNode を生成する．
  BNode*
  make_node(const BlifNode* blif_node);
  
  
private:
  //////////////////////////////////////////////////////////////////////
  //　データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // BNetwork を操作するオブジェクト
  BNetManip* mManip;
  
  // BlifNode の ID をキーとして対応する BNode を収める配列
  vector<BNode*> mNodeMap;

};

END_NAMESPACE_YM

#endif // YM_BLIFBNETCONV_BLIFBNETCONV_H
