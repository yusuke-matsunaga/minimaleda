#ifndef LIBYM_TECHMAP_PATMGR_H
#define LIBYM_TECHMAP_PATMGR_H

/// @file libym_techmap/PatMgr.h
/// @brief PatMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_techmap/techmap_nsdef.h"
#include "ym_cell/cell_nsdef.h"


BEGIN_NAMESPACE_YM_TECHMAP

class FuncGroup;
class RepFunc;
class PatGraph;

//////////////////////////////////////////////////////////////////////
/// @class PatMgr PatMgr.h "PatMgr.h"
/// @brief パタングラフを管理するクラス
///
/// このクラスが持っている情報は
/// - パタングラフ
/// - 各々のパタングラフに対応する論理関数
/// - その論理関数とNPN同値な関数の集合
/// で，最後の関数がセルの論理関数と対応づけられる．
///
/// 情報の設定は専用形式のバイナリファイルを読み込むことでのみ行える．
/// バイナリファイルの生成は patgen/PatGen, pg_dump を参照のこと．
//////////////////////////////////////////////////////////////////////
class PatMgr
{
public:

  /// @brief ノードの種類
  enum tType {
    kInput = 0,
    kAnd   = 2,
    kXor   = 3
  };

public:

  /// @brief コンストラクタ
  PatMgr();

  /// @brief デストラクタ
  ~PatMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief データを読み込んでセットする．
  /// @param[in] s 入力元のストリーム
  /// @retval true 読み込みが成功した．
  /// @retval false 読み込みが失敗した．
  bool
  load(istream& s);


public:
  //////////////////////////////////////////////////////////////////////
  // 論理関数グループに関する情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief セルライブラリを返す．
  const CellLibrary&
  library() const;

  /// @brief このセルライブラリに含まれるセルの最大の入力数を得る．
  ymuint
  max_input() const;

  /// @brief 論理関数の個数を返す．
  ymuint
  func_num() const;

  /// @brief 関数グループを返す．
  /// @param[in] id 関数番号　( 0 <= id < func_num() )
  const FuncGroup&
  func_group(ymuint id) const;

  /// @brief 定数0の関数グループを返す．
  const FuncGroup&
  const0_func() const;

  /// @brief 定数1の関数グループを返す．
  const FuncGroup&
  const1_func() const;

  /// @brief バッファセルの関数グループを返す．
  const FuncGroup&
  buf_func() const;

  /// @brief インバータセルの関数グループを返す．
  const FuncGroup&
  inv_func() const;

  /// @brief 代表関数の個数を返す．
  ymuint
  rep_num() const;

  /// @brief 代表関数を返す．
  /// @param[in] id 代表関数番号
  const RepFunc&
  rep(ymuint id) const;


public:
  //////////////////////////////////////////////////////////////////////
  // パタングラフ関係の情報取得用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 総ノード数を返す．
  ymuint
  node_num() const;

  /// @brief ノードの種類を返す．
  /// @param[in] id ノード番号 ( 0 <= id < node_num() )
  tType
  node_type(ymuint id) const;

  /// @brief ノードが入力ノードの時に入力番号を返す．
  /// @param[in] id ノード番号 ( 0 <= id < node_num() )
  /// @note 入力ノードでない場合の返り値は不定
  ymuint
  input_id(ymuint id) const;

  /// @brief 入力のノード番号を返す．
  /// @param[in] input_id 入力番号 ( 0 <= input_id < input_num() )
  /// @return input_id の入力に対応するノードのノード番号
  ymuint
  input_node(ymuint input_id) const;

  /// @brief 総枝数を返す．
  ymuint
  edge_num() const;

  /// @brief 枝のファンイン元のノード番号を返す．
  /// @param[in] id 枝番号 ( 0 <= id < node_num() * 2 )
  ymuint
  edge_from(ymuint id) const;

  /// @brief 枝のファンアウト先のノード番号を返す．
  /// @param[in] id 枝番号 ( 0 <= id < node_num() * 2 )
  ymuint
  edge_to(ymint id) const;

  /// @brief 枝のファンアウト先の入力位置( 0 or 1 ) を返す．
  /// @param[in] id 枝番号 ( 0 <= id < node_num() * 2 )
  ymuint
  edge_pos(ymuint id) const;

  /// @brief 枝の反転属性を返す．
  /// @param[in] id 枝番号 ( 0 <= id < node_num() * 2 )
  bool
  edge_inv(ymuint id) const;

  /// @brief 総パタン数を返す．
  ymuint
  pat_num() const;

  /// @brief パタンを返す．
  /// @param[in] id パタン番号 ( 0 <= id < pat_num() )
  const PatGraph&
  pat(ymuint id) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を初期化する．
  /// @note 以前確保されたメモリは開放される．
  void
  init();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // セルライブラリ
  const CellLibrary* mLibrary;

  // 関数の数
  ymuint32 mFuncNum;

  // 関数グループの配列
  // サイズは mFuncNum
  FuncGroup* mFuncArray;

  // 代表関数の数
  ymuint32 mRepNum;

  // 代表関数の配列
  // サイズは mRepNum
  RepFunc* mRepArray;

  // ノード数
  ymuint32 mNodeNum;

  // ノードの種類+入力番号を納めた配列
  // サイズは mNodeNum
  ymuint32* mNodeTypeArray;

  // ファンインのノード番号＋反転属性を納めた配列
  // サイズは mNodeNum * 2
  ymuint32* mEdgeArray;

  // パタン数
  ymuint32 mPatNum;

  // パタンの配列
  // サイズは mPatNum
  PatGraph* mPatArray;

};


/// @relates PatMgr
/// @brief PatMgr の内容を出力する．
/// @param[in] s 出力先のストリーム
/// @param[in] patgraph パタングラフ
void
dump(ostream& s,
     const PatMgr& patgraph);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @bireif 論理関数の個数を返す．
inline
ymuint
PatMgr::func_num() const
{
  return mFuncNum;
}

// @brief 定数0の関数グループを返す．
inline
const FuncGroup&
PatMgr::const0_func() const
{
  // 決め打ち
  return func_group(0);
}

// @brief 定数1の関数グループを返す．
inline
const FuncGroup&
PatMgr::const1_func() const
{
  // 決め打ち
  return func_group(1);
}

// @brief バッファセルの関数グループを返す．
inline
const FuncGroup&
PatMgr::buf_func() const
{
  // 決め打ち
  return func_group(2);
}

// @brief インバータセルの関数グループを返す．
inline
const FuncGroup&
PatMgr::inv_func() const
{
  // 決め打ち
  return func_group(3);
}

// @brief 代表関数の個数を返す．
inline
ymuint
PatMgr::rep_num() const
{
  return mRepNum;
}

// @brief ノード数を返す．
inline
ymuint
PatMgr::node_num() const
{
  return mNodeNum;
}

// @brief ノードの種類を返す．
// @param[in] id ノード番号 ( 0 <= id < node_num() )
inline
PatMgr::tType
PatMgr::node_type(ymuint id) const
{
  return static_cast<tType>(mNodeTypeArray[id] & 3U);
}

// @brief ノードが入力ノードの時に入力番号を返す．
// @param[in] id ノード番号 ( 0 <= id < node_num() )
// @note 入力ノードでない場合の返り値は不定
inline
ymuint
PatMgr::input_id(ymuint id) const
{
  return (mNodeTypeArray[id] >> 2);
}

// @brief 入力のノード番号を返す．
// @param[in] input_id 入力番号 ( 0 <= input_id < input_num() )
// @return input_id の入力に対応するノードのノード番号
inline
ymuint
PatMgr::input_node(ymuint input_id) const
{
  return input_id;
}

// @brief 総枝数を返す．
inline
ymuint
PatMgr::edge_num() const
{
  return node_num() * 2;
}

// @brief 枝のファンイン元のノード番号を返す．
// @param[in] id 枝番号 ( 0 <= id < node_num() * 2 )
inline
ymuint
PatMgr::edge_from(ymuint id) const
{
  return (mEdgeArray[id] >> 1);
}

// @brief 枝のファンアウト先のノード番号を返す．
// @param[in] id 枝番号 ( 0 <= id < node_num() * 2 )
inline
ymuint
PatMgr::edge_to(ymint id) const
{
  return (id / 2);
}

// @brief 枝のファンアウト先の入力位置( 0 or 1 ) を返す．
// @param[in] id 枝番号 ( 0 <= id < node_num() * 2 )
inline
ymuint
PatMgr::edge_pos(ymuint id) const
{
  return (id & 1U);
}

// @brief 枝の反転属性を返す．
// @param[in] id 枝番号 ( 0 <= id < node_num() * 2 )
inline
bool
PatMgr::edge_inv(ymuint id) const
{
  return static_cast<bool>(mEdgeArray[id] & 1U);
}

// @brief 総パタン数を返す．
inline
ymuint
PatMgr::pat_num() const
{
  return mPatNum;
}

END_NAMESPACE_YM_TECHMAP

#endif // LIBYM_TECHMAP_PATMGR_H
