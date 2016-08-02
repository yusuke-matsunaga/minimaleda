#ifndef LIBYM_AIG_TESTS_AIGTEMPLATE_H
#define LIBYM_AIG_TESTS_AIGTEMPLATE_H

/// @file libym_aig/tests/AigTemplate.h
/// @brief AigTemplate のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: AigTemplate.h 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_aig/AigHandle.h"
#include "ym_npn/NpnMap.h"


BEGIN_NAMESPACE_YM

class AigMgr;

//////////////////////////////////////////////////////////////////////
/// @brief AtNode の型
//////////////////////////////////////////////////////////////////////
enum tAtType {
  kAtConst0,  // 定数0
  kAtConst1,  // 定数1
  kAtLiteral, // 葉(入力番号＋極性)
  kAtAnd,     // AND(2つの入力＋極性)
  kAtOr,      // OR(2つの入力＋極性)
  kAtXor,     // XOR(2つの入力＋極性)
};


//////////////////////////////////////////////////////////////////////
/// @brief AigTemplate の構成要素
//////////////////////////////////////////////////////////////////////
struct AtNode
{
  // mNodes 中の位置
  size_t mPos;
  
  // 型
  tAtType mType;

  // リテラルの時の入力番号
  ymuint32 mId;

  // AND の時のファンインノード
  AtNode* mInputs[2];
  
  // 極性1
  bool mInv1;

  // 極性2
  bool mInv2;
  
  // eval 用の作業領域
  ymulong mVal;
  
};


//////////////////////////////////////////////////////////////////////
/// @brief AIG を構築するためのテンプレート
//////////////////////////////////////////////////////////////////////
class AigTemplate
{
public:
  
  // コンストラクタ
  AigTemplate();
  
  // コピーコンストラクタ
  AigTemplate(const AigTemplate& src);
  
  // コピーコンストラクタ
  AigTemplate(const AigTemplate& src,
	      const hash_map<size_t, size_t>& vmap);

  // 代入演算子
  const AigTemplate&
  operator=(const AigTemplate& src);
  
  // デストラクタ
  ~AigTemplate();
  

public:

  // AIG を作る．
  AigHandle
  make_aig(AigMgr& mgr,
	   const vector<AigHandle>& inputs);
  
  // 内容がセットされていたら true を返す．
  bool
  set() const;
  
  // コストを返す．
  int
  cost() const;
  
  // 値の評価を行う．
  ymulong
  eval(const vector<ymulong>& ivals) const;

  // 自分の複製を生成するコードを出力する．
  void
  dump_code(ostream& s,
	    const string& target) const;
  
  // 内容をダンプする．
  void
  dump(ostream& s) const;

  
public:
  
  // 内容を空にする．
  void
  clear();
  
  // 定数0にセットする．
  void
  set_const0();

  // 定数1にセットする．
  void
  set_const1();

  // リテラルにセットする．
  void
  set_literal(tVarId id,
	      bool inv);
  
  // and にセットする．
  void
  set_and(const AigTemplate& input0,
	  bool inv0,
	  const AigTemplate& input1,
	  bool inv1);

  // or にセットする．
  void
  set_or(const AigTemplate& input0,
	 bool inv0,
	 const AigTemplate& input1,
	 bool inv1);

  // xor にセットする．
  void
  set_xor(const AigTemplate& input0,
	  bool inv0,
	  const AigTemplate& input1,
	  bool inv1);

  // 変数の付け替えを行ってコピーする．
  void
  set_copy(const AigTemplate& src,
	   const hash_map<size_t, size_t>& vmap);

  // 変数の付け替えを行ってコピーする．
  void
  set_copy(const AigTemplate& src,
	   const NpnMap& vmap);
  
  // 双対形をセットする．
  void
  set_dual(const AigTemplate& src);
  
  // compose を行う．
  void
  set_compose(const AigTemplate& src1,
	      const vector<AigTemplate>& tmap);

  
private:
  
  // make_aig() の下請け関数
  AigHandle
  make_aig_sub(AtNode* node,
	       AigMgr& mgr,
	       const vector<AigHandle>& inputs);
  
  // src の複製を作る．
  AtNode*
  dup_node(const AigTemplate& src,
	   const hash_map<size_t, size_t>& vmap);
 
  // dup_node の下請け関数
  AtNode*
  dup_node_sub(AtNode* node,
	       const hash_map<size_t, size_t>& vmap);
 
  // set_copy の下請け関数
  AtNode*
  set_copy_sub(AtNode* node,
	       const NpnMap& vmap);
  
  // set_dual の下請け関数
  AtNode*
  set_dual_sub(AtNode* node);
  
  // set_compose の下請け関数
  AtNode*
  set_compose_sub(AtNode* node,
		  const vector<AtNode*>& nmap);
  
  // 定数ノードを作る．
  AtNode*
  get_cnode(tAtType type);
  
  // リテラルノードを作る．
  AtNode*
  get_lnode(ymuint32 id,
	    bool inv);

  // AND/OR/XORノードを作る．
  AtNode*
  get_anode(tAtType type,
	    AtNode* input0,
	    bool inv0,
	    AtNode* input1,
	    bool inv1);

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 根のノード
  AtNode* mRoot;

  // 全ノードの配列
  vector<AtNode*> mNodes;
  
};

END_NAMESPACE_YM

#endif // LIBYM_AIG_TESTS_AIGTEMPLATE_H
