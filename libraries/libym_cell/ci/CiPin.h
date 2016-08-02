#ifndef LIBYM_CELL_CI_CIPIN_H
#define LIBYM_CELL_CI_CIPIN_H

/// @file　libym_cell/ci/CiPin.h
/// @brief CiPin のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CiPin.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/CellPin.h"
#include "ym_lexp/LogExpr.h"
#include "ym_utils/ShString.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CellPinBase CiPin.h "ci/CiPin.h"
/// @brief ピンの基底クラス
//////////////////////////////////////////////////////////////////////
class CiPin :
  public CellPin
{
  friend class CiLibrary;

protected:

  /// @brief コンストラクタ
  /// @param[in] name ピン名
  CiPin(const ShString& name);

  /// @brief デストラクタ
  virtual
  ~CiPin();


public:
  //////////////////////////////////////////////////////////////////////
  // 共通属性
  //////////////////////////////////////////////////////////////////////

  /// @brief ピン番号を返す．
  virtual
  ymuint
  id() const;

  /// @brief ピン名を返す．
  virtual
  string
  name() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 入力ピンの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 負荷容量を返す．
  virtual
  CellCapacitance
  capacitance() const;

  /// @brief 立ち上がり時の負荷容量を返す．
  virtual
  CellCapacitance
  rise_capacitance() const;

  /// @brief 立ち下がり時の負荷容量を返す．
  virtual
  CellCapacitance
  fall_capacitance() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 出力ピンの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 論理式を持っているときに true を返す．
  virtual
  bool
  has_function() const;

  /// @brief 機能を表す論理式を返す．
  virtual
  LogExpr
  function() const;

  /// @brief 最大ファンアウト容量を返す．
  virtual
  CellCapacitance
  max_fanout() const;

  /// @brief 最小ファンアウト容量を返す．
  virtual
  CellCapacitance
  min_fanout() const;

  /// @brief 最大負荷容量を返す．
  virtual
  CellCapacitance
  max_capacitance() const;

  /// @brief 最小負荷容量を返す．
  virtual
  CellCapacitance
  min_capacitance() const;

  /// @brief 最大遷移時間を返す．
  virtual
  CellTime
  max_transition() const;

  /// @brief 最小遷移時間を返す．
  virtual
  CellTime
  min_transition() const;

  /// @brief タイミング情報の取得
  /// @param[in] ipos 開始ピン番号
  /// @param[in] timing_sense タイミング情報の摘要条件
  /// @return 条件に合致するタイミング情報を返す．
  /// @note なければ NULL を返す．
  virtual
  const CellTiming*
  timing(ymuint ipos,
	 tCellTimingSense sense) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 設定用の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 出力ピン(入出力ピン)の関数を設定する．
  /// @param[in] function 関数を表す論理式
  virtual
  void
  set_function(const LogExpr& function);

  /// @brief 出力ピン(入出力ピン)のタイミング情報を設定する．
  /// @param[in] pin_id 入力ピンのピン番号
  /// @param[in] sense タイミング情報の適用条件
  /// @param[in] timing 設定するタイミング情報
  virtual
  void
  set_timing(ymuint pin_id,
	     tCellTimingSense sense,
	     const CellTiming* timing);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ピン番号
  ymuint32 mId;

  // 名前
  ShString mName;

};


//////////////////////////////////////////////////////////////////////
/// @class CiInputPin CiPin.h "CiPin.h"
/// @brief セルの入力ピンを表すクラス
//////////////////////////////////////////////////////////////////////
class CiInputPin :
  public CiPin
{
  friend class CiLibrary;

private:

  /// @brief コンストラクタ
  /// @param[in] name ピン名
  /// @param[in] capacitance 負荷容量
  /// @param[in] rise_capacitance 立ち上がり時の負荷容量
  /// @param[in] fall_capacitance 立ち下がり時の負荷容量
  CiInputPin(const ShString& name,
	     CellCapacitance capacitance,
	     CellCapacitance rise_capacitance,
	     CellCapacitance fall_capacitance);

  /// @brief デストラクタ
  virtual
  ~CiInputPin();


public:
  //////////////////////////////////////////////////////////////////////
  // 共通属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 方向を返す．
  virtual
  tCellDirection
  direction() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 入力ピンの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 負荷容量を返す．
  virtual
  CellCapacitance
  capacitance() const;

  /// @brief 立ち上がり時の負荷容量を返す．
  virtual
  CellCapacitance
  rise_capacitance() const;

  /// @brief 立ち下がり時の負荷容量を返す．
  virtual
  CellCapacitance
  fall_capacitance() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 負荷
  CellCapacitance mCapacitance;

  // rise 負荷
  CellCapacitance mRiseCapacitance;

  // fall 負荷
  CellCapacitance mFallCapacitance;

};


//////////////////////////////////////////////////////////////////////
/// @class CiOutputPin CiPin.h "CiPin.h"
/// @brief セルの出力ピンを表すクラス
//////////////////////////////////////////////////////////////////////
class CiOutputPin :
  public CiPin
{
  friend class CiLibrary;

protected:

  /// @brief コンストラクタ
  /// @param[in] name ピン名
  /// @param[in] max_fanout 最大ファンアウト容量
  /// @param[in] min_fanout 最小ファンアウト容量
  /// @param[in] max_capacitance 最大負荷容量
  /// @param[in] min_capacitance 最小負荷容量
  /// @param[in] max_transition 最大遷移時間
  /// @param[in] min_transition 最小遷移時間
  CiOutputPin(const ShString& name,
	      CellCapacitance max_fanout,
	      CellCapacitance min_fanout,
	      CellCapacitance max_capacitance,
	      CellCapacitance min_capacitance,
	      CellTime max_transition,
	      CellTime min_transition);

  /// @brief デストラクタ
  virtual
  ~CiOutputPin();


public:
  //////////////////////////////////////////////////////////////////////
  // 共通属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 方向を返す．
  virtual
  tCellDirection
  direction() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 出力ピンの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 論理式を持っているときに true を返す．
  virtual
  bool
  has_function() const;

  /// @brief 機能を表す論理式を返す．
  virtual
  LogExpr
  function() const;

  /// @brief 最大ファンアウト容量を返す．
  virtual
  CellCapacitance
  max_fanout() const;

  /// @brief 最小ファンアウト容量を返す．
  virtual
  CellCapacitance
  min_fanout() const;

  /// @brief 最大負荷容量を返す．
  virtual
  CellCapacitance
  max_capacitance() const;

  /// @brief 最小負荷容量を返す．
  virtual
  CellCapacitance
  min_capacitance() const;

  /// @brief 最大遷移時間を返す．
  virtual
  CellTime
  max_transition() const;

  /// @brief 最小遷移時間を返す．
  virtual
  CellTime
  min_transition() const;

  /// @brief タイミング情報の取得
  /// @param[in] ipos 開始ピン番号
  /// @param[in] timing_sense タイミング情報の摘要条件
  /// @return 条件に合致するタイミング情報を返す．
  /// @note なければ NULL を返す．
  virtual
  const CellTiming*
  timing(ymuint ipos,
	 tCellTimingSense sense) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 設定用の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 出力ピン(入出力ピン)の関数を設定する．
  /// @param[in] function 関数を表す論理式
  virtual
  void
  set_function(const LogExpr& function);

  /// @brief 出力ピン(入出力ピン)のタイミング情報を設定する．
  /// @param[in] pin_id 入力ピンのピン番号
  /// @param[in] sense タイミング情報の適用条件
  /// @param[in] timing 設定するタイミング情報
  virtual
  void
  set_timing(ymuint pin_id,
	     tCellTimingSense sense,
	     const CellTiming* timing);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 論理式を持っているかどうかを表すフラグ
  ymuint32 mHasFunction;

  // 出力の論理式
  LogExpr mFunction;

  // ファンアウトの負荷
  CellCapacitance mFanoutLoad;

  // 最大ファンアウト容量
  CellCapacitance mMaxFanout;

  // 最小ファンアウト容量
  CellCapacitance mMinFanout;

  // 最大負荷容量
  CellCapacitance mMaxCapacitance;

  // 最小負荷容量
  CellCapacitance mMinCapacitance;

  // 最大遷移時間
  CellTime mMaxTransition;

  // 最小遷移時間
  CellTime mMinTransition;

  // タイミング情報を格納する配列
  // サイズは総ピン数 x 2
  const CellTiming** mTimingArray;

};


//////////////////////////////////////////////////////////////////////
/// @class CiInoutPin CiPin.h "CiPin.h"
/// @brief セルの入出力ピンを表すクラス
/// @note 多重継承はオーバーヘッドがかかるので愚直な実装を用いている．
//////////////////////////////////////////////////////////////////////
class CiInoutPin :
  public CiOutputPin
{
  friend class CiLibrary;

private:

  /// @brief コンストラクタ
  /// @param[in] name ピン名
  /// @param[in] capacitance 負荷容量
  /// @param[in] rise_capacitance 立ち上がり時の負荷容量
  /// @param[in] fall_capacitance 立ち下がり時の負荷容量
  /// @param[in] max_fanout 最大ファンアウト容量
  /// @param[in] min_fanout 最小ファンアウト容量
  /// @param[in] max_capacitance 最大負荷容量
  /// @param[in] min_capacitance 最小負荷容量
  /// @param[in] max_transition 最大遷移時間
  /// @param[in] min_transition 最小遷移時間
  CiInoutPin(const ShString& name,
	     CellCapacitance capacitance,
	     CellCapacitance rise_capacitance,
	     CellCapacitance fall_capacitance,
	     CellCapacitance max_fanout,
	     CellCapacitance min_fanout,
	     CellCapacitance max_capacitance,
	     CellCapacitance min_capacitance,
	     CellTime max_transition,
	     CellTime min_transition);

  /// @brief デストラクタ
  virtual
  ~CiInoutPin();


public:
  //////////////////////////////////////////////////////////////////////
  // 共通属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 方向を返す．
  virtual
  tCellDirection
  direction() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 入力ピンの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 負荷容量を返す．
  virtual
  CellCapacitance
  capacitance() const;

  /// @brief 立ち上がり時の負荷容量を返す．
  virtual
  CellCapacitance
  rise_capacitance() const;

  /// @brief 立ち下がり時の負荷容量を返す．
  virtual
  CellCapacitance
  fall_capacitance() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 負荷
  CellCapacitance mCapacitance;

  // rise 負荷
  CellCapacitance mRiseCapacitance;

  // fall 負荷
  CellCapacitance mFallCapacitance;

};

END_NAMESPACE_YM_CELL

#endif // LIBYM_CELL_CI_CIPIN_H
