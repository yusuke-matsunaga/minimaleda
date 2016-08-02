#ifndef LOGBASE_NODECMD_H
#define LOGBASE_NODECMD_H

/// @file magus/logbase/NodeCmd.h
/// @brief NodeCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: NodeCmd.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

// ノードを扱うコマンドオブジェクト


#include "BNetCmd.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// 複数のノードに対して処理を行うコマンドの親クラス
//////////////////////////////////////////////////////////////////////
class NodeCmd :
  public BNetCmd
{
public:

  // パタンマッチのモード
  enum tMode { kNONE, kGLOB, kREGEXP };

  // 扱うノードのタイプを表す列挙型
  enum tNodeType { kPI, kPO, kFF, kINT, kFI, kFO, kALL };

  // ノードの処理順
  enum tNodeOrder { kNRM, kASC, kDSC };


protected:

  /// @brief コンストラクタ
  NodeCmd(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~NodeCmd();


protected:

  // 一応のコマンド処理関数
  // 要は引数のオプション解析をして set_nodes() を呼んで，
  // foreach_node() を呼ぶだけなのでオプション解析部だけを
  // 書き直せば簡単に上書きできる．
  int
  cmd_proc(TclObjVector& objv);

  // ノードのタイプと順序を指定するオプション引数をパーズする．
  // エラーが起きたら TCL_ERROR を返す．
  virtual
  int
  parse_args(TclObjVector& objv);

  // foreach_node() の直前に呼ばれる関数
  // デフォルトでは何もしない．
  virtual
  int
  before_node_proc();

  // foreach_node() の直後に呼ばれる関数
  // デフォルトでは何もしない．
  virtual
  int
  after_node_proc();

  // 引数の情報を基に処理すべきノードを配列にセットする．
  // 引数が何も与えられなかったときの振る舞いを変えたいので仮想関数と
  // している．
  // デフォルトの定義では空の引数ならノードをセットしない．
  // 普通は下の2つの set_nodes() を組み合わせれば良いはず．
  virtual
  int
  set_nodes(TclObjVector& objv);

  // 引数の名前をノード名だと思って配列に納める．
  // 存在しないノード名が与えられたときには TCL_ERROR を返す．
  int
  set_nodes_from_args(TclObjVector& objv);

  // ノードのタイプおよび処理順に従ってノードを配列に納める．
  // type が FI/FO の場合には基点となるノードを base に入れる．
  // 今は常に TCL_OK を返す．
  int
  set_all_nodes(TclObjVector& objv);

  // マッチングモードを返す．
  tMode
  match_mode() const;


private:

  // ノード名がパタンにマッチするか調べ，マッチした場合に配列に入れる．
  void
  add_node(BNode* node);

  // 外部入力ノードのうち，パタンにマッチするものを配列に入れる．
  void
  set_pi_node();

  // 外部出力ノードのうち，パタンにマッチするものを配列にいれる．
  void
  set_po_node();

  // ラッチノードのうち，パタンにマッチするものを配列にいれる．
  void
  set_latch_node();

  // ロジックノードのうち，パタンにマッチするものを配列にいれる．
  void
  set_logic_node();

  // ロジックノードのうち，パタンにマッチするものを配列にいれる．
  // こちらは入力からのトポロジカル順
  void
  set_logic_node_asc();

  // ロジックノードのうち，パタンにマッチするものを配列にいれる．
  // こちらは出力からのトポロジカル順
  void
  set_logic_node_dsc();

  // ノードのファンインを配列に入れる．
  void
  set_fanin(BNode* node);

  // ノードのファンアウトを配列に入れる．
  void
  set_fanout(BNode* node);

  // 配列に納められたノードに対して NodeProc() を呼び出す．
  // 実行結果を返す．
  // cond が与えられたときは実行時にノード毎に評価して
  // 真の時のみ NodeProc() を呼び出す．
  int
  foreach_node(const TclObj& cond = NULL);

  // 一つのノードに対する処理
  // 処理結果を返す．
  virtual
  int
  node_proc(BNode* node) = 0;

  // mNodeArray をクリアして，size ぶんの領域を確保する．
  void
  init_array(size_t size);


private:
  //////////////////////////////////////////////////////////////////////
  // メンバ変数
  //////////////////////////////////////////////////////////////////////

  // glob オプション解析用のオブジェクト
  TclPopt* mPoptGlob;

  // regexp オプション解析用のオブジェクト
  TclPopt* mPoptRegexp;

  // ao オプション解析用のオブジェクト
  TclPopt* mPoptAscOrder;

  // do オプション解析用のオブジェクト
  TclPopt* mPoptDscOrder;

  // cond オプション解析用のオブジェクト
  TclPoptObj* mPoptCond;

  // pi オプション解析用のオブジェクト
  TclPopt* mPoptPI;

  // po オプション解析用のオブジェクト
  TclPopt* mPoptPO;

  // logic オプション解析用のオブジェクト
  TclPopt* mPoptLogic;

  // ff オプション解析用のオブジェクト
  TclPopt* mPoptFF;

  // マッチングモード
  tMode mMatchMode;

  // パタンの配列
  vector<string> mPats;

  // 処理すべきノードのタイプ
  tNodeType mNodeType;

  // 処理すべきノードの順序
  tNodeOrder mNodeOrder;

  // 処理すべきノードの条件
  TclObj mNodeCond;

  // ファンイン・ファンアウト等の基点になるノード
  BNode* mBaseNode;

  // 処理すべきノードの配列
  vector<BNode*> mNodeArray;

};


//////////////////////////////////////////////////////////////////////
// 複数のノードに対して処理を行うコマンドの親クラス
// NodeCmd との違いは引数が省略されたときに
// すべてのノードを対象にするところ．
//////////////////////////////////////////////////////////////////////
class AllNodeCmd :
  public NodeCmd
{
protected:

  /// @brief コンストラクタ
  AllNodeCmd(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~AllNodeCmd();


private:

  // 引数の情報を基に処理すべきノードを配列にセットする．
  // 引数が何も与えられなかったときにはすべてのノードをセットする．
  virtual
  int
  set_nodes(TclObjVector& objv);

};

END_NAMESPACE_MAGUS

#endif
