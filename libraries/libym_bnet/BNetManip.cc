
/// @file libym_bnet/BNetManip.cc
/// @brief BNetManip の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BNetManip.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_bnet/BNetManip.h"
#include "BNodeMgr.h"


BEGIN_NAMESPACE_YM_BNET

// @brief コンストラクタ．
// @param[in] network 操作対象のネットワーク
BNetManip::BNetManip(BNetwork* network) :
  mNetwork(network)
{
  mCurTmpMark = 1;
}

// @brief デストラクタ
BNetManip::~BNetManip()
{
}

// ノードの消去(eliminate)を行う．
void
BNetManip::eliminate_node(BNode* node)
{
  // もしもこの node が trivial な true buffer ならばファンアウト先が
  // 外部出力でも消去できる．
  if ( node->is_buffer() ) {
    BNode* fi_node = node->fanin(0);
    replace_node(node, fi_node);
    return;
  }

  // ファンアウトリストの複製を作る．
  BNodeEdgeList fanouts(node->fanouts());

  size_t old_ni = node->ni();
  const LogExpr& orig_f = node->func();
  for (BNodeEdgeList::const_iterator it = fanouts.begin();
       it != fanouts.end(); it ++) {
    BNodeEdge* edge = *it;
    BNode* fo_node = edge->to();
    if ( fo_node->is_logic() ) {
      // 中間ノードの場合

      // node の論理式を併合した新しい論理式を作る．
      size_t oni = fo_node->ni();
      VarVarMap vvmap;
      for (size_t i = 0; i < old_ni; i ++) {
	vvmap[i] = i + oni;
      }
      LogExpr tmp = orig_f.remap_var(vvmap);
      LogExpr new_expr = fo_node->func().compose(edge->pos(), tmp);

      // 新しいファンインの配列を作る．
      size_t new_ni = oni + old_ni;
      BNodeVector fanins(new_ni);
      for (size_t i = 0; i < oni; i ++) {
	// この中には node も含まれることになるが，
	// 論理式中にはこの node に対応したリテラルは現れない．
	fanins[i] = fo_node->fanin(i);
      }
      for (size_t i = 0; i < old_ni; i ++) {
	fanins[i + oni] = node->fanin(i);
      }

      // このファンインと論理式を用いて fo_node を変更する．
      change_logic(fo_node, new_expr, fanins);
    }
  }
}

// 外部入力節点を追加する．
// 通常は新たに生成された節点を返すが，名前が重複していた場合には
// 節点を作らずに NULL を返す(エラーとなる)．
BNode*
BNetManip::new_input(const char* name)
{
  return mNetwork->new_node(BNode::kPI, name);
}

// name という名前を持つ外部出力節点を作り登録する．
// そのときに作られた外部出力節点を返す．
// 初期状態では入力数0になっている．
// その後，ChgOutput() を呼ぶことで適切に入力がセットされる．
// 同一の名前を持つ節点が既に存在すればエラーとなる(NULLを返す)．
BNode*
BNetManip::new_output(const char* name)
{
  return mNetwork->new_node(BNode::kPO, name);
}

// 中間節点を追加する．
// そのときに作られた中間出力節点を返す．
// 入力数無しの定数0節点の状態で初期化される．
// 名前が重複していたらエラーとなり，NULLを返す．
BNode*
BNetManip::new_logic(const char* name)
{
  return mNetwork->new_node(BNode::kLO, name);
}

// latch 節点を追加する．
// そのときに作られた中間節点を返す．
// ファンインが未接続で，初期値0の状態でえ初期化される．
// 名前が重複していたらエラーとなり，NULLを返す．
BNode*
BNetManip::new_latch(const char* name)
{
  return mNetwork->new_node(BNode::kFF, name);
}

// nodeで示される節点を削除する．
// エラーが起きたら false を返す．
// エラー条件に関してはBNetwork.hを参照のこと
bool
BNetManip::delete_node(BNode* node)
{
  return mNetwork->delete_node(node);
}

// nodeで示される節点の名前を name に変更する．
// エラーが起きたら false を返す．
bool
BNetManip::rename_node(BNode* node,
		       const char* name)
{
  return mNetwork->rename_node(node, name);
}

// old_node を new_node に置き換える．
// エラーが起きたら false を返す．
bool
BNetManip::replace_node(BNode* old_node,
			BNode* new_node)
{
  if ( !old_node ) {
    // old_node が NULL ! こんなのは論外
    return false;
  }

  if ( !new_node ) {
    // new_node が NULL ! こんなのは論外
    return false;
  }

  if ( old_node->parent() != mNetwork ) {
    // old_node はこのネットワークに属している節点ではない 
    BNET_ERROR("node is not a member of the network.");
    return false;
  }

  if ( new_node->parent() != mNetwork ) {
    // new_node はこのネットワークに属している節点ではない 
    BNET_ERROR("node is not a member of the network.");
    return false;
  }

  if ( new_node->is_output() ) {
    // new_node が外部出力ならそのファンインを用いる．
    new_node = new_node->fanin(0);
  }

  // 実際の置き換えを行う．

  // reference ではなくコピーを作る．
  BNodeEdgeList folist(old_node->fanouts());
  for (BNodeEdgeList::const_iterator p = folist.begin();
       p != folist.end(); ++p) {
    BNodeEdge* edge = *p;
    BNode* fo_node = edge->to();

    bool found = false;
    size_t ni = fo_node->ni();
    for (size_t i = 0; i < ni; ++ i) {
      if ( fo_node->fanin(i) == new_node ) {
	found = true;
	break;
      }
    }
    if ( found ) {
      // fo_node のファンインに new_node がすでに含まれていた場合
      vector<BNode*> fanins(ni);
      for (size_t i = 0; i < ni; ++ i) {
	fanins[i] = fo_node->fanin(i);
      }
      // ダブっているのを承知で new_node を加える．
      fanins[edge->pos()] = new_node;
      // すべてはこの change_logic() がうまく処理してくれる．
      // ちなにグローバルな構造は変わらないので TFO チェックは不要
      change_logic(fo_node, fo_node->func(), fanins, false);
    }
    else {
      // fo_node のファンインに new_node が含まれていなければ話は簡単
      // ファンイン情報を変える．
      mNetwork->change_edge_fanin(edge, old_node, new_node);
    }
  }

  // ここで mFanouts は空になっているはず．
  assert_cond( old_node->fanouts().size() == 0, __FILE__, __LINE__);

  return true;
}

// ノードの内容を変更するメンバ関数
// 外部出力節点 onode の入力を node に変更する．
// エラーが起きたら false を返す．
bool
BNetManip::change_output(BNode* onode,
			 BNode* node)
{
  if ( !onode ) {
    // onode が NULL ! こんなのは論外
    return false;
  }

  if ( !node ) {
    // node が NULL ! こんなのも論外
    return false;
  }

  if ( onode->ni() == 1 && onode->fanin(0) == node ) {
    // 変わらないので何もしない．
    return true;
  }

  if ( onode->parent() != mNetwork ) {
    // このネットワークに属している節点ではない．
    BNET_ERROR("onode is not a member of the network.");
    return false;
  }

  if ( !onode->is_output() ) {
    // onode は外部出力節点ではない．
    BNET_ERROR("onode is not an output.");
    return false;
  }

  if ( node->parent() != mNetwork ) {
    // このネットワークに属している節点ではない．
    BNET_ERROR("node is not a member of the network.");
    return false;
  }

  if ( node->is_output() ) {
    // nodeも外部出力の場合にはnodeのファンインをnodeだと思う．
    node = node->fanin(0);
  }

  // 内容を設定する．
  mNetwork->set_node_fanins(onode, vector<BNode*>(1, node));

  // 正常終了
  return true;
}

// 既存の中間節点を変更する．
// エラーが起きたら false を返す．
bool
BNetManip::change_logic(BNode* node,
			const LogExpr& expr,
			const BNodeVector& fanins,
			bool tfo_check)
{
  if ( !node ) {
    // node が NULL ! こんなのは論外
    return false;
  }

  if ( node->parent() != mNetwork ) {
    // このネットワークに属している節点ではない．
    BNET_ERROR("node is not a member of the network.");
    return false;
  }

  if( !node->is_logic() ) {
    // 中間節点ではない．
    BNET_ERROR("node is not a logic node.");
    return false;
  }
  
  // Phase-1: 重複したファンインを一つにまとめる処理を行う．
  size_t orig_ni = fanins.size();
  size_t new_ni = 0;
  LogExpr new_expr = expr;

  // 新しいファンインを mNodes に入れる．
  // もとのファンイン番号をキーにして新しいファンイン番号を
  // mTmpPos に入れる．
  // ただし，変化がない場合と重複していた場合には -1 を入れる．
  mTmpNodes.clear();
  mTmpNodes.reserve(orig_ni);
  mTmpPos.resize(orig_ni);

  // ファンインの重複チェック
  for (size_t i = 0; i < orig_ni; i ++) {
    BNode* node = fanins[i];
    size_t j = 0;
    for ( ; j < mTmpNodes.size(); ++ j) {
      if ( mTmpNodes[j] == node ) {
	break;
      }
    }
    if ( j < mTmpNodes.size() ) {
      mTmpPos[i] = j;
    }
    else {
      if ( i != mTmpNodes.size() ) {
	mTmpPos[i] = mTmpNodes.size();
      }
      else {
	mTmpPos[i] = -1;
      }
      mTmpNodes.push_back(node);
    }
  }
  new_ni = mTmpNodes.size();
  if ( new_ni < orig_ni ) {
    // 式のリテラルの置き換えを行う．
    mTmpMap.clear();
    for (size_t i = 0; i < orig_ni; i ++) {
      int pos = mTmpPos[i];
      if ( pos >= 0 ) {
	mTmpMap.insert(make_pair(i, pos));
      }
    }
    // リテラルを付け替えたファクタードフォームを作る．
    new_expr = new_expr.remap_var(mTmpMap);
  }
  
  // Phase-2: new_expr に現れないリテラルに対応したファンインは削除する．
  bool redundant = false;
  for (size_t i = 0; i < new_ni; i ++) {
    size_t lit = new_expr.litnum(i);
    if ( lit == 0 ) {
      redundant = true;
      break;
    }
  }
  if ( redundant ) {
    mTmpMap.clear();
    size_t wpos = 0;
    for (size_t i = 0; i < new_ni; i ++) {
      size_t lit = new_expr.litnum(i);
      if ( lit > 0 ) {
	if ( wpos < i ) {
	  BNode* node = mTmpNodes[i];
	  mTmpNodes[wpos] = node;
	  mTmpMap.insert(make_pair(i, wpos));
	}
	++ wpos;
      }
    }
    mTmpNodes.erase(mTmpNodes.begin() + wpos, mTmpNodes.end());
    new_ni = mTmpNodes.size();
    new_expr = new_expr.remap_var(mTmpMap);
  }

  if ( tfo_check ) {
    // node の TFO に印を付ける．
    if ( mTmpMark.size() < mNetwork->max_node_id() ) {
      mTmpMark.resize(mNetwork->max_node_id(), 0);
    }
    mark_TFO(node);
  }
    
  bool ans = true;
  for (size_t i = 0; i < new_ni; i ++) {
    BNode* inode = mTmpNodes[i];
    if ( !inode ) {
      // inode が NULL だとエラーになる．
      BNET_ERROR("inode is NULL");
      ans = false;
      break;
    }
    if ( inode->parent() != mNetwork ) {
      // inode が他のネットワークに属しているのでエラー．
      BNET_ERROR("inode belongs to another network");
      ans = false;
      break;
    }
    if ( tfo_check && get_TFO(inode) ) {
      // inode が node の TFO だったらエラー
      BNET_ERROR("inode is a TFO of node");
      ans = false;
      break;
    }
  }

  if ( tfo_check ) {
    clear_TFO(node);
  }
  
  if ( !ans ) {
    return false;
  }
  
  // 内容をセットしなおす．
  mNetwork->set_node_func(node, new_expr);
  mNetwork->set_node_fanins(node, mTmpNodes);

  // 正常終了
  return true;
}

// TFO にマークをつける．
void
BNetManip::mark_TFO(BNode* node)
{
  for (BNodeFoList::const_iterator p = node->fanouts_begin();
       p != node->fanouts_end(); ++p) {
    BNodeEdge* edge = *p;
    BNode* onode = edge->to();
    if ( onode->is_logic() && mTmpMark[onode->id()] != mCurTmpMark ) {
      mTmpMark[onode->id()] = mCurTmpMark;
      mark_TFO(onode);
    }
  }
}

// TFOの印を消す．
void
BNetManip::clear_TFO(BNode* node)
{
  // 実際には消さない．
  // ただ mCurTmpMark を一つ進める．
  ++ mCurTmpMark;
  if ( mCurTmpMark == 0 ) {
    // これが一回りして 0 になったときだけ全体をクリアする．
    for (vector<int>::iterator p = mTmpMark.begin();
	 p != mTmpMark.end(); ++ p) {
      *p = 0;
    }
    mCurTmpMark = 1;
  }
}

// TFO マークを得る．
bool
BNetManip::get_TFO(BNode* node)
{
  // 今のマークと同じ値を持っているときにマークされたとみなす．
  return mTmpMark[node->id()] == mCurTmpMark;
}

// @brief 中間ノードの変更
// @param[in] node 対象のノード
// @param[in] expr 新しい論理式
// @retval true 変更が成功した．
// @retval false 変更が失敗した．
// @note ファンインは現在のものをそのまま用いる．
// @note 失敗するのは以下の場合
//  - 論理式中に現われる変数の数とファンイン数が異なる場合
bool
BNetManip::change_logic(BNode* node,
			const LogExpr& expr)
{
  size_t ni = node->ni();
  vector<BNode*> fanins(ni);
  for (size_t i = 0; i < ni; ++ i) {
    fanins[i] = node->fanin(i);
  }
  // ファンインは変わらないので TFO チェックは不要
  return change_logic(node, expr, fanins, false);
}

// @brief latch ノードの変更
// @param[in] node 対象のノード
// @param[in] fanin ファンインのノード
// @param[in] reset_value 初期値
// @retval true 変更が成功した．
// @retval false 変更が失敗した．
// @note 失敗するのは以下の場合
//  - fanin がこのネットワークに属していない場合
bool
BNetManip::change_latch(BNode* node,
			BNode* fanin,
			int reset_value)
{
  if ( !node ) {
    // node が NULL ! こんなのは論外
    BNET_ERROR("node is NULL");
    return false;
  }

  if ( !fanin ) {
    // fanin が NULL !
    BNET_ERROR("fanin is NULL");
    return false;
  }

  if ( node->parent() != mNetwork ) {
    // このネットワークに属している節点ではない．
    BNET_ERROR("node is not a member of the network.");
    return false;
  }
  
  if ( fanin->parent() != mNetwork ) {
    // このネットワークに属している節点ではない．
    BNET_ERROR("fanin is not a member of the network.");
    return false;
  }

  if( !node->is_latch() ) {
    // latch 節点ではない
    if ( node->is_logic() ) {
      // logic 節点ならタイプを変える．
      mNetwork->set_node_type(node, BNode::kFF);
    }
    else {
      BNET_ERROR("node is not a latch node.");
      return false;
    }
  }

  // 内容をセットしなおす．
  mNetwork->set_node_fanins(node, vector<BNode*>(1, fanin));
  mNetwork->set_node_reset_value(node, reset_value);
  
  // 正常終了
  return true;
}

// node を定数ノードに変更する．
// cval が 0 の時 0 それ以外のときは 1 とみなす．
bool
BNetManip::change_to_const(BNode* node,
			   int cval)
{
  // 定数式を表すファクタードフォームを作る．
  LogExpr expr = (cval == 0) ? LogExpr::make_zero() : LogExpr::make_one();

  // ノードの内容を設定する．
  bool stat = change_logic(node, expr, vector<BNode*>(0));

  return stat;
}

// node を，src_node を入力としたバッファノードに変える．
bool
BNetManip::change_to_buffer(BNode* node,
			    BNode* src_node)
{
  // BUF を表すファクタードフォームを作る．
  LogExpr expr = LogExpr::make_posiliteral(0);

  // ノードの内容を設定する．
  bool stat = change_logic(node, expr, vector<BNode*>(1, src_node));

  return stat;
}

// node を，src_node を入力としたインバータノードに変える．
bool
BNetManip::change_to_inverter(BNode* node,
			      BNode* src_node)
{
  // NOT を表すファクタードフォームを作る．
  LogExpr expr = LogExpr::make_negaliteral(0);

  // ノードの内容を設定する．
  bool stat = change_logic(node, expr, vector<BNode*>(1, src_node));

  return stat;
}

// node を fanins を入力とした AND ノードに変える．
bool
BNetManip::change_to_and(BNode* node,
			 const BNodeVector& fanins)
{
  // AND を表すファクタードフォームを作る．
  size_t n = fanins.size();
  LogExprVector leaves(n);
  for (size_t i = 0; i < n; i ++) {
    leaves[i] = LogExpr::make_posiliteral(i);
  }
  LogExpr expr = LogExpr::make_and(leaves);

  // ノードの内容を設定する．
  bool stat = change_logic(node, expr, fanins);

  return stat;
}

// node を fanins を入力とした NAND ノードに変える．
bool
BNetManip::change_to_nand(BNode* node,
			  const BNodeVector& fanins)
{
  // NAND を表すファクタードフォームを作る．
  size_t n = fanins.size();
  LogExprVector leaves(n);
  for (size_t i = 0; i < n; i ++) {
    leaves[i] = LogExpr::make_posiliteral(i);
  }
  LogExpr expr = ~LogExpr::make_and(leaves);

  // ノードの内容を設定する．
  bool stat = change_logic(node, expr, fanins);

  return stat;
}

// node を fanins を入力とした OR ノードに変える．
bool
BNetManip::change_to_or(BNode* node,
			const BNodeVector& fanins)
{
  // OR を表すファクタードフォームを作る．
  size_t n = fanins.size();
  LogExprVector leaves(n);
  for (size_t i = 0; i < n; i ++) {
    leaves[i] = LogExpr::make_posiliteral(i);
  }
  LogExpr expr = LogExpr::make_or(leaves);

  // ノードの内容を設定する．
  bool stat = change_logic(node, expr, fanins);

  return stat;
}

// node を fanins を入力とした NOR ノードに変える．
bool
BNetManip::change_to_nor(BNode* node,
			 const BNodeVector& fanins)
{
  // OR を表すファクタードフォームを作る．
  size_t n = fanins.size();
  LogExprVector leaves(n);
  for (size_t i = 0; i < n; i ++) {
    leaves[i] = LogExpr::make_posiliteral(i);
  }
  LogExpr expr = ~LogExpr::make_or(leaves);

  // ノードの内容を設定する．
  bool stat = change_logic(node, expr, fanins);

  return stat;
}

// node を fanins を入力とした XOR ノードに変える．
bool
BNetManip::change_to_xor(BNode* node,
			 const BNodeVector& fanins)
{
  // XOR を表すファクタードフォームを作る．
  size_t n = fanins.size();
  LogExprVector leaves(n);
  for (size_t i = 0; i < n; i ++) {
    leaves[i] = LogExpr::make_posiliteral(i);
  }
  LogExpr expr = LogExpr::make_xor(leaves);

  // ノードの内容を設定する．
  bool stat = change_logic(node, expr, fanins);

  return stat;
}

// node を fanins を入力とした XNOR ノードに変える．
bool
BNetManip::change_to_xnor(BNode* node,
			  const BNodeVector& fanins)
{
  // XOR を表すファクタードフォームを作る．
  size_t n = fanins.size();
  LogExprVector leaves(n);
  for (size_t i = 0; i < n; i ++) {
    leaves[i] = LogExpr::make_posiliteral(i);
  }
  LogExpr expr = ~LogExpr::make_xor(leaves);

  // ノードの内容を設定する．
  bool stat = change_logic(node, expr, fanins);

  return stat;
}

// 定数ノードを生成する．
BNode*
BNetManip::make_const(int cval,
		      const char* dst_name)
{
  // 新しいノードを作る．
  BNode* node = new_logic(dst_name);
  if ( !node ) {
    // なんらかのエラーが起きた．
    return NULL;
  }

  // ノードの内容を設定する．
  bool stat = change_to_const(node, cval);
  if ( !stat ) {
    // なんらかのエラーが起きたのでノードを削除する．
    delete_node(node);
    return NULL;
  }

  // できたノードを返す．
  return node;
}

// src_node を入力とした，dst_name という名前のバッファノードを生成する．
BNode*
BNetManip::make_buffer(BNode* src_node,
		       const char* dst_name)
{
  // 新しいノードを作る．
  BNode* node = new_logic(dst_name);
  if ( !node ) {
    // なんらかのエラーが起きた．
    return NULL;
  }

  // ノードの内容を設定する．
  bool stat = change_to_buffer(node, src_node);
  if ( !stat ) {
    // なんらかのエラーが起きたのでノードを削除する．
    delete_node(node);
    return NULL;
  }

  // できたノードを返す．
  return node;
}

// src_node を入力とした，dst_name という名前のインバータノードを生成する．
BNode*
BNetManip::make_inverter(BNode* src_node,
			 const char* dst_name)
{
  // 新しいノードを作る．
  BNode* node = new_logic(dst_name);
  if ( !node ) {
    // なんらかのエラーが起きた．
    return NULL;
  }

  // ノードの内容を設定する．
  bool stat = change_to_inverter(node, src_node);
  if ( !stat ) {
    // なんらかのエラーが起きたのでノードを削除する．
    delete_node(node);
    return NULL;
  }

  // できたノードを返す．
  return node;
}

// fanins を入力とした，dst_name という名前の AND ノードを生成する．
BNode*
BNetManip::make_and(const BNodeVector& fanins,
		    const char* dst_name)
{
  // 新しいノードを作る．
  BNode* node = new_logic(dst_name);
  if ( !node ) {
    // なんらかのエラーが起きた．
    return NULL;
  }

  // ノードの内容を設定する．
  bool stat = change_to_and(node, fanins);
  if ( !stat ) {
    // なんらかのエラーが起きたのでノードを削除する．
    delete_node(node);
    return NULL;
  }

  // できたノードを返す．
  return node;
}

// fanins を入力とした，dst_name という名前の NAND ノードを生成する．
BNode*
BNetManip::make_nand(const BNodeVector& fanins,
		     const char* dst_name)
{
  // 新しいノードを作る．
  BNode* node = new_logic(dst_name);
  if ( !node ) {
    // なんらかのエラーが起きた．
    return NULL;
  }

  // ノードの内容を設定する．
  bool stat = change_to_nand(node, fanins);
  if ( !stat ) {
    // なんらかのエラーが起きたのでノードを削除する．
    delete_node(node);
    return NULL;
  }

  // できたノードを返す．
  return node;
}

// fanins を入力とした，dst_name という名前の OR ノードを生成する．
BNode*
BNetManip::make_or(const BNodeVector& fanins,
		   const char* dst_name)
{
  // 新しいノードを作る．
  BNode* node = new_logic(dst_name);
  if ( !node ) {
    // なんらかのエラーが起きた．
    return NULL;
  }

  // ノードの内容を設定する．
  bool stat = change_to_or(node, fanins);
  if ( !stat ) {
    // なんらかのエラーが起きたのでノードを削除する．
    delete_node(node);
    return NULL;
  }

  // できたノードを返す．
  return node;
}

// fanins を入力とした，dst_name という名前の NOR ノードを生成する．
BNode*
BNetManip::make_nor(const BNodeVector& fanins,
		    const char* dst_name)
{
  // 新しいノードを作る．
  BNode* node = new_logic(dst_name);
  if ( !node ) {
    // なんらかのエラーが起きた．
    return NULL;
  }

  // ノードの内容を設定する．
  bool stat = change_to_nor(node, fanins);
  if ( !stat ) {
    // なんらかのエラーが起きたのでノードを削除する．
    delete_node(node);
    return NULL;
  }

  // できたノードを返す．
  return node;
}

// fanins を入力とした，dst_name という名前の XOR ノードを生成する．
BNode*
BNetManip::make_xor(const BNodeVector& fanins,
		    const char* dst_name)
{
  // 新しいノードを作る．
  BNode* node = new_logic(dst_name);
  if ( !node ) {
    // なんらかのエラーが起きた．
    return NULL;
  }

  // ノードの内容を設定する．
  bool stat = change_to_xor(node, fanins);
  if ( !stat ) {
    // なんらかのエラーが起きたのでノードを削除する．
    delete_node(node);
    return NULL;
  }

  // できたノードを返す．
  return node;
}

// fanins を入力とした，dst_name という名前の XNOR ノードを生成する．
BNode*
BNetManip::make_xnor(const BNodeVector& fanins,
		     const char* dst_name)
{
  // 新しいノードを作る．
  BNode* node = new_logic(dst_name);
  if ( !node ) {
    // なんらかのエラーが起きた．
    return NULL;
  }

  // ノードの内容を設定する．
  bool stat = change_to_xnor(node, fanins);
  if ( !stat ) {
    // なんらかのエラーが起きたのでノードを削除する．
    delete_node(node);
    return NULL;
  }

  // できたノードを返す．
  return node;
}

END_NAMESPACE_YM_BNET
