#ifndef YM_MINCOV_MINCOVSOLVER_H
#define YM_MINCOV_MINCOVSOLVER_H

/// @file ym_mincov/MincovSolver.h
/// @brief MincovSolver のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: MincovSolver.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mincov/mincov_nsdef.h"


BEGIN_NAMESPACE_YM_MINCOV

class MincovCost;

//////////////////////////////////////////////////////////////////////
/// @class MincovSolver MincovSolver.h <ym_mincov/MincovSolver.h>
/// @brief 最小被覆問題を解くクラス
//////////////////////////////////////////////////////////////////////
class MincovSolver
{
public:

  /// @brief コンストラクタ
  MincovSolver();
  
  /// @brief デストラクタ
  ~MincovSolver();


public:
  
  /// @brief 最小被覆問題を解く．
  /// @param[in] matrix 対象の行列
  /// @param[out] solution 選ばれた列集合
  /// @return 最良解
  MincovCost
  operator()(const MincovMatrix& matrix,
	     vector<ymuint32>& solution);

  
private:
  
  /// @brief 解を求める再帰関数
  /// @param[in] matrix 対象の行列
  /// @param[in] best_sofar 現時点の最良解
  /// @param[out] solution 選ばれた列集合
  /// @return 最良解
  MincovCost
  solve(MincovMatrix& matrix,
	const MincovCost& best_sofar,
	vector<ymuint32>& solution);

  /// @brief 下限を求める．
  /// @param[in] matrix 対象の行列
  /// @return 下限値
  MincovCost
  lower_bound(MincovMatrix& matrix);
  
  /// @brief 簡単化を行う．
  /// @param[in] matrix 対象の行列
  /// @param[out] selected_cols 簡単化中で選択された列の集合
  void
  reduce(MincovMatrix& matrix,
	 vector<ymuint32>& selected_cols);
  
  /// @brief 行支配を探し，行を削除する．
  /// @param[in] matrix 対象の行列
  /// @return 削除された行があったら true を返す．
  bool
  row_dominance(MincovMatrix& matrix);

  /// @brief 列支配を探し，列を削除する．
  /// @param[in] matrix 対象の行列
  /// @return 削除された列があったら true を返す．
  bool
  col_dominance(MincovMatrix& matrix);

  /// @brief 必須列を探し，列を選択する．
  /// @param[in] matrix 対象の行列
  /// @param[out] selected_cols 選択された列を追加する列集合
  /// @return 選択された列があったら true を返す．
  bool
  essential_col(MincovMatrix& matrix,
		vector<ymuint32>& selected_cols);

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

END_NAMESPACE_YM_MINCOV

#endif // YM_MINCOV_MINCOVMATRIX_H
