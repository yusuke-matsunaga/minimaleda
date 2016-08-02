#ifndef MAGUS_LUTMAP_ILPGEN_H
#define MAGUS_LUTMAP_ILPGEN_H

/// @file magus/lutmap/ILPGen.cc
/// @brief ILP 問題を生成するクラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ILPGen.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2007 Yusuke Matsunaga
/// All rights reserved.

#include "Cut.h"
#include "EnumCutOp.h"


BEGIN_NAMESPACE_MAGUS_LUTMAP

//////////////////////////////////////////////////////////////////////
// ILPGen 用の cut
//////////////////////////////////////////////////////////////////////
class ILPGenCut
{
  friend class ILPGen;
private:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] root 根のノード
  /// @param[in] ni 入力数
  /// @param[in] inputs 入力のノードのベクタ
  ILPGenCut(ymuint id,
	    SbjNode* root,
	    ymuint ni,
	    SbjNode** inputs);

  // デストラクタ
  ~ILPGenCut();


public:

  // ID 番号を得る．
  ymuint
  id() const;

  /// @brief 根のノードを得る．
  SbjNode*
  root() const;
  
  /// @brief 入力のサイズを得る．
  ymuint
  ni() const;

  /// @brief pos 番目の入力を得る．
  SbjNode*
  input(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID 番号
  ymuint32 mId;
  
  // 根のノード
  SbjNode* mRoot;

  // 入力のノード配列
  vector<SbjNode*> mInputs;

};


//////////////////////////////////////////////////////////////////////
// ILP 問題を生成するクラス
//////////////////////////////////////////////////////////////////////
class ILPGen :
  public EnumCutOp
{
public:

  // コンストラクタ
  ILPGen();

  // デストラクタ
  virtual
  ~ILPGen();


public:

  // 現在の制約式を出力する．
  void
  dump(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // EnumCutOp の仮想関数
  //////////////////////////////////////////////////////////////////////

  // 最初に呼ばれる関数
  virtual
  void
  all_init(const SbjGraph& sbjgraph,
	   ymuint limit,
	   tCutMode mode);

  // eval_node() の先頭で呼ばれる関数
  virtual
  void
  node_init(SbjNode* node,
	    ymuint pos);

  // カットが一つ見つかったときに呼ばれる関数
  virtual
  void
  found(SbjNode* root);

  // カットが一つ見つかったときに呼ばれる関数
  virtual
  void
  found(SbjNode* root,
	ymuint ni,
	SbjNode* inputs[]);

  // eval_node() の最後に呼ばれる関数
  virtual
  void
  node_end(SbjNode* node,
	   ymuint pos,
	   ymuint ncuts);

  // 最後に呼ばれる関数
  virtual
  void
  all_end(const SbjGraph& sbjgraph,
	  ymuint limit,
	  tCutMode mode);


private:

  // カットを生成する．
  ILPGenCut*
  new_cut(SbjNode* root,
	  ymuint ni,
	  SbjNode* inputs[]);

  // 全てのカットを削除する．
  void
  delete_cuts();

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 次に生成されるカットの ID 番号
  ymuint32 mCutId;

  // すべてのカット
  // 各ノードごとにそのノードを根とするカットのリストを入れておく配列
  vector<list<ILPGenCut*> > mAllCut;

  list<ymuint32> mVars;
  
  list<string> mConstr;

};

END_NAMESPACE_MAGUS_LUTMAP

#endif // MAGUS_LUTMAP_LB_H
