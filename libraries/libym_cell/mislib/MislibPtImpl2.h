#ifndef LIBYM_CELL_MISLIB_MISLIBPTIMPL2_H
#define LIBYM_CELL_MISLIB_MISLIBPTIMPL2_H

/// @file libym_cell/mislib/MislibPtImpl2.h
/// @brief MislibPt の派生クラスのヘッダファイル(その2)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MislibPtImpl2.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MislibPt.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class MislibPtNot MislibPtImpl2.h "MislibPtImpl2.h"
/// @brief NOT論理式を表すクラス
//////////////////////////////////////////////////////////////////////
class MislibPtNot :
  public MislibPt
{
  friend class MislibParser;
private:

  /// @brief コンストラクタ
  /// @param[in] loc 位置情報
  /// @param[in] child1 1番目の子供
  MislibPtNot(const FileRegion& loc,
	      MislibPt* child1);

  /// @brief デストラクタ
  virtual
  ~MislibPtNot();


public:

  /// @brief 種類を取り出す．
  virtual
  tType
  type() const;

  /// @brief 論理式を表す型のときに true を返す．
  virtual
  bool
  is_expr() const;

  /// @brief 1番目の子供を取り出す．
  virtual
  MislibPt*
  child1() const;

  /// @brief 内容を出力する．
  /// デバッグ用
  virtual
  void
  dump(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 子供
  MislibPt* mChild1;

};


//////////////////////////////////////////////////////////////////////
/// @class MislibPtAnd MislibPtImpl2.h "MislibPtImpl2.h"
/// @brief AND論理式を表すクラス
//////////////////////////////////////////////////////////////////////
class MislibPtAnd :
  public MislibPt
{
  friend class MislibParser;
private:

  /// @brief コンストラクタ
  /// @param[in] loc 位置情報
  /// @param[in] child1 1番目の子供
  /// @param[in] child2 2番目の子供
  MislibPtAnd(const FileRegion& loc,
	      MislibPt* child1,
	      MislibPt* child2);

  /// @brief デストラクタ
  virtual
  ~MislibPtAnd();


public:

  /// @brief 種類を取り出す．
  virtual
  tType
  type() const;

  /// @brief 論理式を表す型のときに true を返す．
  virtual
  bool
  is_expr() const;

  /// @brief 1番目の子供を取り出す．
  virtual
  MislibPt*
  child1() const;

  /// @brief 2番目の子供を取り出す．
  virtual
  MislibPt*
  child2() const;

  /// @brief 内容を出力する．
  /// デバッグ用
  virtual
  void
  dump(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 1番目の子供
  MislibPt* mChild1;

  // 2番目の子供
  MislibPt* mChild2;

};


//////////////////////////////////////////////////////////////////////
/// @class MislibPtOr MislibPtImpl2.h "MislibPtImpl2.h"
/// @brief OR論理式を表すクラス
//////////////////////////////////////////////////////////////////////
class MislibPtOr :
  public MislibPt
{
  friend class MislibParser;
private:

  /// @brief コンストラクタ
  /// @param[in] loc 位置情報
  /// @param[in] child1 1番目の子供
  /// @param[in] child2 2番目の子供
  MislibPtOr(const FileRegion& loc,
	     MislibPt* child1,
	     MislibPt* child2);

  /// @brief デストラクタ
  virtual
  ~MislibPtOr();


public:

  /// @brief 種類を取り出す．
  virtual
  tType
  type() const;

  /// @brief 論理式を表す型のときに true を返す．
  virtual
  bool
  is_expr() const;

  /// @brief 1番目の子供を取り出す．
  virtual
  MislibPt*
  child1() const;

  /// @brief 2番目の子供を取り出す．
  virtual
  MislibPt*
  child2() const;

  /// @brief 内容を出力する．
  /// デバッグ用
  virtual
  void
  dump(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 1番目の子供
  MislibPt* mChild1;

  // 2番目の子供
  MislibPt* mChild2;

};


//////////////////////////////////////////////////////////////////////
/// @class MislibPtPin MislibPtImpl2.h "MislibPtImpl2.h"
/// @brief ピンを表すクラス
//////////////////////////////////////////////////////////////////////
class MislibPtPin :
  public MislibPt
{
  friend class MislibParser;
  friend class MislibPtPinList;
private:

  /// @brief コンストラクタ
  /// @param[in] loc 位置情報
  /// @param[in] name ピン名
  /// @param[in] phase 極性
  /// @param[in] input_load 入力負荷
  /// @param[in] max_load 最大駆動負荷
  /// @param[in] rise_block_delay 立ち上がり固定遅延
  /// @param[in] rise_fanout_delay 立ち上がり負荷依存遅延
  /// @param[in] fall_block_delay 立ち下がり固定遅延
  /// @param[in] fall_fanout_delay 立ち下がり負荷依存遅延
  MislibPtPin(const FileRegion& loc,
	      MislibPt* name,
	      MislibPt* phase,
	      MislibPt* input_load,
	      MislibPt* max_load,
	      MislibPt* rise_block_delay,
	      MislibPt* rise_fanout_delay,
	      MislibPt* fall_block_delay,
	      MislibPt* fall_fanout_delay);

  /// @brief デストラクタ
  virtual
  ~MislibPtPin();


public:

  /// @brief 種類を取り出す．
  virtual
  tType
  type() const;

  /// @brief ピン名を表すオブジェクトを取り出す．
  virtual
  MislibPt*
  name() const;

  /// @brief 極性情報を表すオブジェクトを取り出す．
  virtual
  MislibPt*
  phase() const;

  /// @brief 入力負荷を表すオブジェクトを取り出す．
  virtual
  MislibPt*
  input_load() const;

  /// @brief 最大駆動負荷を表すオブジェクトを取り出す．
  virtual
  MislibPt*
  max_load() const;

  /// @brief 立ち上がり固定遅延を表すオブジェクトを取り出す．
  virtual
  MislibPt*
  rise_block_delay() const;

  /// @brief 立ち上がり負荷依存遅延を表すオブジェクトを取り出す．
  virtual
  MislibPt*
  rise_fanout_delay() const;

  /// @brief 立ち下がり固定遅延を表すオブジェクトを取り出す．
  virtual
  MislibPt*
  fall_block_delay() const;

  /// @brief 立ち下がり負荷依存遅延を表すオブジェクトを取り出す．
  virtual
  MislibPt*
  fall_fanout_delay() const;

  /// @brief 次のピンを設定する．
  virtual
  void
  set_next(MislibPt* pin);

  /// @brief 次のピンを取り出す．
  virtual
  MislibPt*
  next() const;

  /// @brief 内容を出力する．
  /// デバッグ用
  virtual
  void
  dump(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力ピン名
  MislibPt* mName;

  // 極性情報
  MislibPt* mPhase;

  // 入力負荷
  MislibPt* mInputLoad;

  // 最大駆動負荷
  MislibPt* mMaxLoad;

  // 立ち上がり固有遅延
  MislibPt* mRiseBlockDelay;

  // 立ち上がり負荷依存遅延係数
  MislibPt* mRiseFanoutDelay;

  // 立ち下がり固有遅延
  MislibPt* mFallBlockDelay;

  // 立ち下がり負荷依存遅延係数
  MislibPt* mFallFanoutDelay;

  // 次のピン
  MislibPt* mNext;

};


//////////////////////////////////////////////////////////////////////
/// @class MislibPtGate MislibPtImpl2.h "MislibPtImpl2.h"
/// @brief ゲート(セル)を表すクラス
//////////////////////////////////////////////////////////////////////
class MislibPtGate :
  public MislibPt
{
  friend class MislibParser;
private:

  /// @brief コンストラクタ
  /// @param[in] loc 位置情報
  /// @param[in] name 名前を表すパース木
  /// @param[in] area 面積を表すパース木
  /// @param[in] opin_name 出力ピン名を表すパース木
  /// @param[in] opin_expr 出力の論理式を表すパース木
  /// @param[in] ipin_list 入力ピンのリストを表すパース木
  MislibPtGate(const FileRegion& loc,
	       MislibPt* name,
	       MislibPt* area,
	       MislibPt* opin_name,
	       MislibPt* opin_expr,
	       MislibPt* ipin_list);

  /// @brief デストラクタ
  virtual
  ~MislibPtGate();


public:

  /// @brief 種類を取り出す．
  virtual
  tType
  type() const;

  /// @brief ピン名/ゲート名を表すオブジェクトを取り出す．
  virtual
  MislibPt*
  name() const;

  /// @brief 面積を表すオブジェクトを返す．
  virtual
  MislibPt*
  area() const;

  /// @brief 出力ピン名を表すオブジェクトを返す．
  virtual
  MislibPt*
  opin_name() const;

  /// @brief 出力の論理式を表すオブジェクトを返す．
  virtual
  MislibPt*
  opin_expr() const;

  /// @brief 入力ピンのリストを表すオブジェクトを返す．
  virtual
  MislibPt*
  ipin_list() const;

  /// @brief 次の要素を設定する．
  virtual
  void
  set_next(MislibPt* pin);

  /// @brief 次の要素を取り出す．
  virtual
  MislibPt*
  next() const;

  /// @brief 内容を出力する．
  /// デバッグ用
  virtual
  void
  dump(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  MislibPt* mName;

  // 面積
  MislibPt* mArea;

  // 出力ピン名
  MislibPt* mOpinName;

  // 出力の論理式
  MislibPt* mOpinExpr;

  // 入力ピンのリスト
  MislibPt* mIpinList;

  // 次の要素
  MislibPt* mNext;

};

END_NAMESPACE_YM_CELL

#endif // LIBYM_CELL_MISLIB_MISLIBPTIMPL2_H
