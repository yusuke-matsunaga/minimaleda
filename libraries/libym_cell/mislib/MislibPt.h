#ifndef LIBYM_CELL_MISLIB_MISLIBPT_H
#define LIBYM_CELL_MISLIB_MISLIBPT_H

/// @file libym_cell/mislib/MislibPt.h
/// @brief MislibPt のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MislibPt.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/cell_nsdef.h"
#include "ym_utils/ShString.h"
#include "ym_utils/FileRegion.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class MislibPt MislibPt.h "MislibPt.h"
/// @brief トークンを表す基底クラス
//////////////////////////////////////////////////////////////////////
class MislibPt
{
  friend class MislibParser;

public:

  /// @brief ノードの種類
  enum tType {
    /// @brief 文字列
    kStr,
    /// @brief 数値
    kNum,
    /// @brief NONINV 極性
    kNoninv,
    /// @brief INV 極性
    kInv,
    /// @brief UNKOWN 極性
    kUnknown,
    /// @brief 定数0
    kConst0,
    /// @brief 定数1
    kConst1,
    /// @brief リスト
    kList,
    /// @brief NOT論理式
    kNot,
    /// @brief AND論理式
    kAnd,
    /// @brief OR論理式
    kOr,
    /// @brief 入力ピン
    kPin,
    /// @brief ゲート
    kGate
  };


protected:

  /// @brief コンストラクタ
  /// @param[in] loc 位置情報
  MislibPt(const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~MislibPt();


public:
  //////////////////////////////////////////////////////////////////////
  // 共通な関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 位置情報を取り出す．
  const FileRegion&
  loc() const;

  /// @brief 種類を取り出す．
  virtual
  tType
  type() const = 0;

  /// @brief 論理式を表す型のときに true を返す．
  /// @note 文字列や定数0と定数1も論理式とみなす．
  /// @note デフォルトでは false を返す．
  virtual
  bool
  is_expr() const;

  /// @brief 内容を出力する．
  /// デバッグ用
  virtual
  void
  dump(ostream& s) const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // 文字列型のときに意味のある関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 文字列を取り出す
  /// @note デフォルトでは空文字列を返す．
  virtual
  ShString
  str() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 数値型のときに意味のある関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 数値を取り出す
  /// @note デフォルトでは 0.0 を返す．
  virtual
  double
  num() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 論理式型のときに意味のある関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 1番目の子供を取り出す．
  /// @note デフォルトでは NULL を返す．
  virtual
  MislibPt*
  child1() const;

  /// @brief 2番目の子供を取り出す．
  /// @note デフォルトでは NULL を返す．
  virtual
  MislibPt*
  child2() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ピン型のときに意味のある関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ピン名/ゲート名を表すオブジェクトを取り出す．
  /// @note デフォルトでは NULL を返す．
  virtual
  MislibPt*
  name() const;

  /// @brief 極性情報を表すオブジェクトを取り出す．
  /// @note デフォルトでは NULL を返す．
  virtual
  MislibPt*
  phase() const;

  /// @brief 入力負荷を表すオブジェクトを取り出す．
  /// @note デフォルトでは NULL を返す．
  virtual
  MislibPt*
  input_load() const;

  /// @brief 最大駆動負荷を表すオブジェクトを取り出す．
  /// @note デフォルトでは NULL を返す．
  virtual
  MislibPt*
  max_load() const;

  /// @brief 立ち上がり固定遅延を表すオブジェクトを取り出す．
  /// @note デフォルトでは NULL を返す．
  virtual
  MislibPt*
  rise_block_delay() const;

  /// @brief 立ち上がり負荷依存遅延を表すオブジェクトを取り出す．
  /// @note デフォルトでは NULL を返す．
  virtual
  MislibPt*
  rise_fanout_delay() const;

  /// @brief 立ち下がり固定遅延を表すオブジェクトを取り出す．
  /// @note デフォルトでは NULL を返す．
  virtual
  MislibPt*
  fall_block_delay() const;

  /// @brief 立ち下がり負荷依存遅延を表すオブジェクトを取り出す．
  /// @note デフォルトでは NULL を返す．
  virtual
  MislibPt*
  fall_fanout_delay() const;

  /// @brief 次の要素を設定する．
  /// @note デフォルトでは何もしない．
  virtual
  void
  set_next(MislibPt* pin);

  /// @brief 次の要素を取り出す．
  /// @note デフォルトでは NULL を返す．
  virtual
  MislibPt*
  next() const;


public:
  //////////////////////////////////////////////////////////////////////
  // リスト型のときに意味のある関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 末尾に要素を追加する．
  /// @note デフォルトでは何もしない．
  virtual
  void
  push_back(MislibPt* pin);

  /// @brief 先頭の要素を取り出す．
  /// @note デフォルトでは NULL を返す．
  virtual
  MislibPt*
  top() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ゲート型のときに意味のある関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 面積を表すオブジェクトを返す．
  /// @note デフォルトでは NULL を返す．
  virtual
  MislibPt*
  area() const;

  /// @brief 出力ピン名を表すオブジェクトを返す．
  /// @note デフォルトでは NULL を返す．
  virtual
  MislibPt*
  opin_name() const;

  /// @brief 出力の論理式を表すオブジェクトを返す．
  /// @note デフォルトでは NULL を返す．
  virtual
  MislibPt*
  opin_expr() const;

  /// @brief 入力ピンのリストを表すオブジェクトを返す．
  /// @note デフォルトでは NULL を返す．
  virtual
  MislibPt*
  ipin_list() const;


protected:
  //////////////////////////////////////////////////////////////////////
  // 派生クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 位置を出力する．
  void
  dump_loc(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 位置情報
  FileRegion mLoc;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 位置情報を取り出す．
inline
const FileRegion&
MislibPt::loc() const
{
  return mLoc;
}

END_NAMESPACE_YM_CELL

#endif // LIBYM_CELL_MISLIB_MISLIBPT_H
