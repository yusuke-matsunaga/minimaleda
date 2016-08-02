#ifndef YM_BNET_BNETDECOMP_H
#define YM_BNET_BNETDECOMP_H

/// @file ym_bnet/BNetDecomp.h
/// @brief BNetDecomp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BNetDecomp.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_bnet/bnet_nsdef.h"
#include "ym_lexp/LogExpr.h"
#include "ym_utils/RandGen.h"


BEGIN_NAMESPACE_YM_BNET

//////////////////////////////////////////////////////////////////////
/// @class BNetDecomp BNetDecomp.h <ym_bnet/BNetDecomp.h>
/// @ingroup BnetGroup
/// @brief BNetwork のノードの分解を行うクラス
/// @sa BNetwork BNetManip
//////////////////////////////////////////////////////////////////////
class BNetDecomp
{
public:
  //////////////////////////////////////////////////////////////////////
  /// @name コンストラクタ/デストラクタ/初期化
  /// @{

  /// @brief コンストラクタ．
  BNetDecomp();
  
  /// @brief デストラクタ
  virtual
  ~BNetDecomp();

  /// @}
  //////////////////////////////////////////////////////////////////////
  

public:
  //////////////////////////////////////////////////////////////////////
  /// @name ネットワークを分解する関数
  /// @{
  
  /// @brief 単純分解を行う．(バランス型: type1)
  /// @param[in] network 操作対象のネットワーク
  /// @param[in] max_fanin ファンインの最大数(0, 1で制限なし)
  /// @param[in] no_xor true の時は XOR/XNOR もAND/OR に分解する．
  /// @note 単純分解とは(AND/OR/XOR)のみからなるノードの木に分解すること
  /// @note ただし，入力に否定属性がつく場合がある．
  /// @note 副作用として sweep() が行われる．
  void
  operator()(BNetwork& network,
	     ymuint max_fanin,
	     bool no_xor = false);
  
  /// @brief 単純分解を行う．(ランダム型: type2)
  /// @param[in] network 操作対象のネットワーク
  /// @param[in] max_fanin ファンインの最大数(0, 1で制限なし)
  /// @param[in] randgen 分解で用いる乱数発生器
  /// @param[in] no_xor true の時は XOR/XNOR もAND/OR に分解する．
  /// @note 単純分解とは(AND/OR/XOR)のみからなるノードの木に分解すること
  /// @note ただし，入力に否定属性がつく場合がある．
  /// @note 副作用として sweep() が行われる．
  void
  operator()(BNetwork& network,
	     ymuint max_fanin,
	     RandGen& randgen,
	     bool no_xor = false);
  
  /// @}
  

private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////
  
  // decomp_type2_node で用いられるサブルーティン
  // expr を根とする論理式を分解して root_node を根のノード
  // とする木を作る．
  // この関数を呼び出す時には根のタイプは二項演算子であるこ
  // とを保証すること．
  void
  decomp_type1_sub(BNode* orig_node,
		   ymuint max_fanin,
		   const LogExpr& expr,
		   BNode* root_node,
		   bool no_xor);
  
  // decomp_type2_node で用いられるサブルーティン
  // expr を根とする論理式を分解して root_node を根のノード
  // とする木を作る．
  // この関数を呼び出す時には根のタイプは二項演算子であるこ
  // とを保証すること．
  void
  decomp_type2_sub(BNode* orig_node,
		   ymuint max_fanin,
		   const LogExpr& expr,
		   BNode* root_node,
		   bool no_xor);
  
  // tmp_fanins[b: b+ni-1] を入力とする
  // max_fanin 入力ノードの木を network 上のノードとして作る．
  // タイプは type_node の根と同タイプとする．
  // root_node が NULL でない場合には root_node を根のノードとする．
  // root_node が NULL の場合には新しいノードを作る．
  // いずれの場合でも根のノードを返す．
  BNode*
  build_tree(ymuint b,
	     ymuint ni,
	     const vector<pair<BNode*, tPol> >& tmp_fanins,
	     ymuint max_fanin,
	     const LogExpr& type_node,
	     BNode* root_node,
	     bool no_xor);
  
  // depth_map に基づいて node の深さを計算する．
  // node の直接のファンインのノードの深さはすでに計算されているものとする．
  int
  calc_depth(BNode* node);

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // ネットワークを変更するためのオブジェクト
  BNetManip* mManip;
  
  // type1 で用いる深さ保持用の連想配列
  hash_map<ymuint32, int> mDepthMap;

  // type2 で用いる乱数発生器
  RandGen* mRandGen;
  
};

END_NAMESPACE_YM_BNET

#endif // YM_BNET_BNETDECOMP_H
