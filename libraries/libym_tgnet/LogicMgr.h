#ifndef LIBYM_TGNET_LOGICMGR_H
#define LIBYM_TGNET_LOGICMGR_H

/// @file libym_tgnet/LogicMgr.h
/// @brief LogicMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: LogicMgr.h 1920 2008-12-20 15:52:42Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_tgnet/tgnet_nsdef.h"
#include "ym_tgnet/TgGateTemplate.h"
#include "ym_lexp/LogExpr.h"


BEGIN_NAMESPACE_YM_TGNET

//////////////////////////////////////////////////////////////////////
/// @class LogicMgr LogicMgr.h "LogicMgr.h"
/// @brief logic ノードのタイプ番号を管理するクラス
//////////////////////////////////////////////////////////////////////
class LogicMgr
{
public:

  /// @brief コンストラクタ
  LogicMgr();

  /// @brief デストラクタ
  ~LogicMgr();


public:

  /// @brief 初期化する．
  void
  clear();

  /// @brief 新しい論理式を登録する．
  /// @param[in] lexp 論理式
  /// @return ゲートの型を返す．
  TgGateTemplate
  reg_logic(const LogExpr& lexp);

  /// @brief 登録されている論理式の数を返す．
  size_t
  num() const;

  /// @brief 論理式を取り出す．
  /// @param[in] gt_id ゲートテンプレート
  LogExpr
  get(TgGateTemplate gt_id) const;


public:

  /// @brief デバッグ用の関数
  /// @param[in] s 出力先のストリーム
  void
  dump(ostream& s) const;

  
private:

  class Cell;
  
  // cell と現在の真理値ベクタが等価かどうか調べる．
  bool
  equiv(Cell* cell,
	const vector<ymulong>& tv_array);

  // ハッシュ表を拡大して再ハッシュする．
  void
  expand();
  
  // ハッシュ表用の領域を確保する．
  void
  alloc_table(size_t req_size);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いるデータ構造
  //////////////////////////////////////////////////////////////////////

  // 一つの要素を表すセル
  struct Cell
  {
    // ID番号
    TgGateTemplate mId;

    // 論理式
    LogExpr mLexp;

    // 真理値表
    vector<ymulong> mTvArray;

    // 次のセルを指すリンク
    Cell* mLink;
  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ハッシュ表を拡大するときに使われる定数
  static
  const double kHashCapacity = 1.8;

  // 一般の論理関数のオフセット値
  static
  const ymuint32 kBase = static_cast<ymuint32>(kTgUsrDef);
  
  // Cell の配列
  vector<Cell*> mCellArray;
  
  // ハッシュ表のサイズ
  size_t mHashSize;

  // ハッシュ表
  Cell** mHashTable;

  // ハッシュ表を拡大する目安
  size_t mNextLimit;
  
};

END_NAMESPACE_YM_TGNET

#endif // LIBYM_TGNET_LOGICMGR_H
