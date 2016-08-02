#ifndef YM_BNET_BNETSOPDECOMP_H
#define YM_BNET_BNETSOPDECOMP_H

/// @file ym_bnet/BNetSopDecomp.h
/// @brief BNetSopDecomp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BNetSopDecomp.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_bnet/bnet_nsdef.h"
#include "ym_lexp/LogExpr.h"


BEGIN_NAMESPACE_YM_BNET

//////////////////////////////////////////////////////////////////////
/// @class BNetSopDecomp BNetSopDecomp.h <ym_bnet/BNetSopDecomp.h>
/// @ingroup BnetGroup
/// @brief BNetwork のノードのSOP分解を行うクラス
/// @sa BNetwork BNetManip
//////////////////////////////////////////////////////////////////////
class BNetSopDecomp
{
public:
  //////////////////////////////////////////////////////////////////////
  /// @name コンストラクタ/デストラクタ/初期化
  /// @{

  /// @brief コンストラクタ．
  BNetSopDecomp();
  
  /// @brief デストラクタ
  ~BNetSopDecomp();

  /// @}
  //////////////////////////////////////////////////////////////////////
  

public:
  //////////////////////////////////////////////////////////////////////
  /// @name ネットワークを分解する関数
  /// @{
  
  /// @brief ネットワーク全体を積和形論理式に分解する．
  /// @param[in] network 対象のネットワーク
  void
  operator()(BNetwork& network);
  
  /// @}
  

private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////
  
  // sop_decomp_node で用いられるサブルーティン
  // expr を根とする論理式を分解して root_node を根のノードと
  // する木を作る．
  // この関数を呼び出す時には根のタイプは二項演算子であることを
  // 保証すること．
  void
  sop_decomp_sub(BNode* orig_node,
		 const LogExpr& expr,
		 BNode* root_node);
  
  // tmp_fanins[b: b + ni - 1] を入力とする XOR をあらわす
  // SOP 2分木を network 上のノードとして作る．
  // root_node が NULL でない場合には root_node を根のノードとする．
  // root_node が NULL の場合には新しいノードを作る．
  // いずれの場合でも根のノードを返す．
  BNode*
  build_xorsop_tree(size_t b,
		    size_t ni,
		    const BNodeVector& tmp_fanins,
		    tPol pol,
		    BNode* root_node);
  
  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // ネットワークを変更するためのオブジェクト
  BNetManip* mManip;
  
};

END_NAMESPACE_YM_BNET

#endif // YM_BNET_BNETSOPDECOMP_H
