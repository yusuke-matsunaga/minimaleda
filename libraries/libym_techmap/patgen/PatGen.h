#ifndef LIBYM_TECHMAP_PATGEN_PATGEN_H
#define LIBYM_TECHMAP_PATGEN_PATGEN_H

/// @file libym_techmap/patgen/PatGen.h
/// @brief PatGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "patgen_nsdef.h"
#include "ym_lexp/lexp_nsdef.h"
#include "ym_utils/Alloc.h"


BEGIN_NAMESPACE_YM_TECHMAP_PATGEN

class PgNode;
class PgHandle;

//////////////////////////////////////////////////////////////////////
/// @class PatGen PatGen.h "PatGen.h"
/// @brief パタングラフを生成するクラス
/// @note 前処理で使われるのであまり効率を考えなく良い．
//////////////////////////////////////////////////////////////////////
class PatGen
{
public:

  /// @brief コンストラクタ
  PatGen();

  /// @brief デストラクタ
  ~PatGen();


public:
  //////////////////////////////////////////////////////////////////////
  // このクラスのメインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化する．
  void
  init();

  /// @brief 論理式から対応するパタングラフを生成する．
  /// @param[in] expr 元になる論理式
  /// @param[out] pat_list パタン番号のリスト
  /// @note expr は定数を含んではいけない．
  void
  operator()(const LogExpr& expr,
	     vector<PgHandle>& pat_list);

  /// @brief パタンを登録する．
  /// @param[in] root 根のハンドル
  /// @return パタン番号を返す．
  ymuint
  reg_pat(PgHandle root);

  /// @brief 使われていないパタンとノードを削除してID番号を詰める．
  /// @note 同時に入力ノードの入力番号とノード番号を一致させる．
  void
  sweep();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 全ノード数を返す．
  ymuint
  node_num() const;

  /// @brief ノードを返す．
  /// @param[in] pos ノード番号 ( 0 <= pos < node_num() )
  PgNode*
  node(ymuint pos) const;

  /// @brief パタン数を返す．
  ymuint
  pat_num() const;

  /// @brief パタンの根のハンドルを返す．
  /// @param[in] id パタン番号 ( 0 <= id < pat_num() )
  PgHandle
  pat_root(ymuint id) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief パタングラフを生成する再帰関数
  /// @param[in] expr 元になる論理式
  /// @param[out] pg_list パタングラフ番号のリスト
  void
  pg_sub(const LogExpr& expr,
	 vector<PgHandle>& pg_list);

  /// @brief 入力ノードを作る．
  /// @param[in] id 入力番号
  /// @note 既にあるときはそれを返す．
  PgNode*
  make_input(ymuint id);

  /// @brief テンプレートにしたがって2分木を作る．
  PgHandle
  make_bintree(const LogExpr& expr,
	       const vector<PgHandle>& input,
	       int pat[],
	       ymuint& pos);

  /// @brief 論理式の種類に応じてノードを作る．
  PgHandle
  make_node(const LogExpr& expr,
	    PgHandle l_handle,
	    PgHandle r_handle);

  /// @brief ノードを作る．
  PgNode*
  new_node();

  /// @brief ノードを削除する．
  void
  delete_node(PgNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // ハッシュ表用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ハッシュ表を確保する．
  void
  alloc_table(ymuint req_size);

  /// @brief PgNode のハッシュ関数
  static
  ymuint
  hash_func(ymuint type,
	    PgNode* l_node,
	    PgNode* r_node);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // PgNode のメモリ確保用アロケータ
  UnitAlloc mAlloc;

  // 入力ノードを収める配列
  vector<PgNode*> mInputList;

  // 全ノードを収める配列
  vector<PgNode*> mNodeList;

  // AND/XOR ノードのハッシュ表
  PgNode** mHashTable;

  // mHashTable のサイズ
  ymuint32 mHashSize;

  // ハッシュ表を拡大する目安
  ymuint32 mNextLimit;

  // パタンの根のハンドルのリスト
  vector<PgHandle> mPatList;

};


//////////////////////////////////////////////////////////////////////
// 関連する関数
//////////////////////////////////////////////////////////////////////

/// @relates PatGen
/// @brief グラフ構造全体の内容を表示する．
/// @param[in] s 出力先のストリーム
/// @param[in] pat_gen 対象のオブジェクト
void
pg_display(ostream& s,
	   const PatGen& pat_gen);

END_NAMESPACE_YM_TECHMAP_PATGEN

#endif // LIBYM_TECHMAP_PATGEN_PATGEN_H
