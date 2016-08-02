
/// @file libym_mincov/MincovSolver.cc
/// @brief MincovSolver の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: MincovSolver.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mincov/MincovSolver.h"
#include "ym_mincov/MincovCost.h"
#include "ym_mincov/MincovMatrix.h"
#include "MaxClique.h"
#include "ym_utils/MFSet.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
// クラス MincovSolver
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MincovSolver::MincovSolver()
{
}

// @brief デストラクタ
MincovSolver::~MincovSolver()
{
}
  
// @brief 最小被覆問題を解く．
// @param[in] matrix 対象の行列
// @param[out] solution 選ばれた列集合
// @return 最良解
MincovCost
MincovSolver::operator()(const MincovMatrix& matrix,
			 vector<ymuint32>& solution)
{
  MincovMatrix work(matrix);
  MincovCost dummy_best = MincovCost::zero();
  return solve(work, dummy_best, solution);
}
  
// @brief 解を求める再帰関数
// @param[in] matrix 対象の行列
// @param[in] best_sofar 現時点の最良解
// @param[out] solution 選ばれた列集合
// @return 最良解
MincovCost
MincovSolver::solve(MincovMatrix& matrix,
		    const MincovCost& best_sofar,
		    vector<ymuint32>& solution)
{
  MincovCost best(best_sofar);
  
  vector<ymuint32> tmp_solution;
  
  reduce(matrix, tmp_solution);

  MincovCost lb = lower_bound(matrix);
  if ( lb >= best ) {
    return lb;
  }

  // 次の分岐のための列をとってくる．

  // その列を選択したときの最良解を求める．
  vector<ymuint32> tmp_solution1;
  MincovCost v1 = solve(matrix, best, tmp_solution1);
  if ( best > v1 ) {
    best = v1;
    solution = tmp_solution;
    solution.insert(solution.end(), tmp_solution1.begin(), tmp_solution1.end());
    // + 選んだ列
  }

  // その列を選択しなかったときの最良解を求める．
  tmp_solution1.clear();
  MincovCost v2 = solve(matrix, best, tmp_solution1);
  if ( best > v2 ) {
    best = v2;
    solution = tmp_solution;
    solution.insert(solution.end(), tmp_solution1.begin(), tmp_solution1.end());
  }
  
  return best;
}

// @brief 下限を求める．
// @param[in] matrix 対象の行列
// @return 下限値
MincovCost
MincovSolver::lower_bound(MincovMatrix& matrix)
{
  // MIS を用いた下限

  // まず，列を共有する行のグループを求める．
  ymuint32 rs = matrix.row_size();
  MFSet rset(rs);
  for (const MincovColHead* col = matrix.col_front();
       !matrix.is_col_end(col); col = col->next()) {
    const MincovCell* cell = col->front();
    ymuint32 rpos0 = cell->row_pos();
    for (cell = cell->col_next();
	 !col->is_end(cell); cell = cell->col_next()) {
      ymuint32 rpos = cell->row_pos();
      rset.merge(rpos0, rpos);
    }
  }

  vector<ymuint32> row_list;
  row_list.reserve(rs);
  for (const MincovRowHead* row = matrix.row_front();
       !matrix.is_row_end(row); row = row->next()) {
    row_list.push_back(row->pos());
  }
  ymuint32 nr = row_list.size();
  MaxClique mc(nr);
  for (ymuint i = 0; i < nr - 1; ++ i) {
    ymuint32 rpos1 = row_list[i];
    for (ymuint j = i + 1; j < nr; ++ j) {
      ymuint32 rpos2 = row_list[j];
      if ( rset.find(rpos1) != rset.find(rpos2) ) {
	mc.connect(i, j);
	mc.connect(j, i);
      }
    }
    const MincovRowHead* row = matrix.row(rpos1);
    const MincovCost* min_cost = NULL;
    for (const MincovCell* cell = row->front();
	 !row->is_end(cell); cell = cell->row_next()) {
      ymuint32 cpos = cell->col_pos();
      if ( min_cost == NULL || *min_cost > matrix.col_cost(cpos) ) {
	min_cost = &matrix.col_cost(cpos);
      }
    }
    assert_cond(min_cost, __FILE__, __LINE__);
    mc.set_cost(i, min_cost);
  }
  vector<ymuint32> mis;
  MincovCost cost1 = mc.solve(mis);
  return cost1;
}

// @brief 簡単化を行う．
// @param[in] matrix 対象の行列
// @param[out] selected_cols 簡単化中で選択された列の集合
void
MincovSolver::reduce(MincovMatrix& matrix,
		     vector<ymuint32>& selected_cols)
{
  for ( ; ; ) {
    bool change = false;

    // 行支配を探し，行の削除を行う．
    if ( row_dominance(matrix) ) {
      change = true;
    }

    // 列支配を探し，列の削除を行う．
    if ( col_dominance(matrix) ) {
      change = true;
    }

    // 必須列を探し，列の選択を行う．
    if ( essential_col(matrix, selected_cols) ) {
      change = true;
    }

    if ( !change ) {
      break;
    }
  }
}


BEGIN_NONAMESPACE

struct RowLt
{
  RowLt()
  {
  }

  ~RowLt()
  {
  }

  bool
  operator()(const MincovRowHead* a,
	     const MincovRowHead* b)
  {
    return a->num() <= b->num();
  }
};

END_NONAMESPACE


// @brief 行支配を探し，行を削除する．
// @param[in] matrix 対象の行列
// @return 削除された行があったら true を返す．
bool
MincovSolver::row_dominance(MincovMatrix& matrix)
{
  bool change = false;
  
  // 削除された行番号に印をつけるための配列
  vector<bool> del_mark(matrix.row_size(), false);

  // 残っている行のリストを作る．
  vector<const MincovRowHead*> row_list;
  row_list.reserve(matrix.row_size());
  for (const MincovRowHead* row = matrix.row_front();
       !matrix.is_row_end(row); row = row->next()) {
    row_list.push_back(row);
  }
  // 要素数の少ない順にソートする．
  sort(row_list.begin(), row_list.end(), RowLt());

  for (vector<const MincovRowHead*>::iterator p = row_list.begin();
       p != row_list.end(); ++ p) {
    const MincovRowHead* row = *p;
    if ( del_mark[row->pos()] ) continue;

    // row の行に要素を持つ列で要素数が最小のものを求める．
    ymuint32 min_num = matrix.row_size() + 1;
    const MincovColHead* min_col = NULL;
    for (const MincovCell* cell = row->front();
	 !row->is_end(cell); cell = cell->row_next()) {
      ymuint32 col_pos = cell->col_pos();
      const MincovColHead* col = matrix.col(col_pos);
      ymuint32 col_num = col->num();
      if ( min_num > col_num ) {
	min_num = col_num;
	min_col = col;
      }
    }

    // min_col の列に要素を持つ行を tmp_rows に入れる．
    vector<ymuint32> tmp_rows;
    tmp_rows.reserve(min_num);
    for (const MincovCell* cell = min_col->front();
	 !min_col->is_end(cell); cell = cell->col_next()) {
      ymuint32 row_pos = cell->row_pos();
      if ( matrix.row(row_pos)->num() > row->num() ) {
	// ただし row よりも要素数の多いもののみを対象にする．
	tmp_rows.push_back(row_pos);
      }
    }

    // min_col 以外の列に含まれない行を tmp_rows から落とす．
    for (const MincovCell* cell = row->front();
	 !row->is_end(cell); cell = cell->row_next()) {
      ymuint32 col_pos = cell->col_pos();
      const MincovColHead* col = matrix.col(col_pos);
      if ( col == min_col ) continue;
      ymuint rpos = 0;
      ymuint wpos = 0;
      ymuint32 row1 = tmp_rows[rpos];
      const MincovCell* cell = col->front();
      ymuint32 row2 = cell->row_pos();
      while ( rpos < tmp_rows.size() && !col->is_end(cell) ) {
	if ( row1 == row2 ) {
	  if ( wpos != rpos ) {
	    tmp_rows[wpos] = row1;
	  }
	  ++ wpos;
	  ++ rpos;
	  row1 = tmp_rows[rpos];
	}
	else if ( row1 < row2 ) {
	  // row1 を削除
	  ++ rpos;
	}
	else {
	  cell = cell->col_next();
	}
      }
      if ( wpos < tmp_rows.size() ) {
	tmp_rows.erase(tmp_rows.begin() + wpos, tmp_rows.end());
      }
    }
    
    // tmp_rows に残った行は row_pos に支配されている．
    for (vector<ymuint32>::iterator p = tmp_rows.begin();
	 p != tmp_rows.end(); ++ p) {
      ymuint row_pos = *p;
      matrix.delete_row(row_pos);
      del_mark[row_pos] = true;
      change = true;
    }
  }

  return true;
}


BEGIN_NONAMESPACE

struct ColLt
{
  ColLt()
  {
  }

  ~ColLt()
  {
  }

  bool
  operator()(const MincovColHead* a,
	     const MincovColHead* b)
  {
    return a->num() <= b->num();
  }
};

END_NONAMESPACE


// @brief 列支配を探し，列を削除する．
// @param[in] matrix 対象の行列
// @return 削除された列があったら true を返す．
bool
MincovSolver::col_dominance(MincovMatrix& matrix)
{
  bool change = false;
  
  // 残っている列のリストを作る．
  vector<const MincovColHead*> col_list;
  col_list.reserve(matrix.col_size());
  for (const MincovColHead* col = matrix.col_front();
       !matrix.is_col_end(col); col = col->next()) {
    col_list.push_back(col);
  }
  // 要素数の少ない順にソートする．
  sort(col_list.begin(), col_list.end(), ColLt());

  for (vector<const MincovColHead*>::iterator p = col_list.begin();
       p != col_list.end(); ++ p) {
    const MincovColHead* col = *p;

    // col の列に要素を持つ行で要素数が最小のものを求める．
    ymuint32 min_num = matrix.col_size() + 1;
    const MincovRowHead* min_row = NULL;
    for (const MincovCell* cell = col->front();
	 !col->is_end(cell); cell = cell->col_next()) {
      ymuint32 row_pos = cell->row_pos();
      const MincovRowHead* row = matrix.row(row_pos);
      ymuint32 row_num = row->num();
      if ( min_num > row_num ) {
	min_num = row_num;
	min_row = row;
      }
    }

    // min_row の行に要素を持つ列を対象にして支配関係のチェックを行う．
    for (const MincovCell* cell = min_row->front();
	 !min_row->is_end(cell); cell = cell->row_next()) {
      const MincovColHead* col2 = matrix.col(cell->col_pos());
      if ( col2->num() <= col->num() ) {
	// ただし col よりも要素数の多くない列は調べる必要はない．
	continue;
      }
      const MincovCell* cell1 = col->front();
      ymuint32 pos1 = cell1->row_pos();
      const MincovCell* cell2 = col2->front();
      ymuint32 pos2 = cell2->row_pos();
      bool found = false;
      for ( ; ; ) {
	if ( pos1 < pos2 ) {
	  // col に含まれていて col2 に含まれない行があるので
	  // col2 は col を支配しない．
	  break;
	}
	if ( pos1 == pos2 ) {
	  cell1 = cell1->col_next();
	  if ( col->is_end(cell1) ) {
	    found = true;
	    break;
	  }
	}
	cell2 = cell2->col_next();
	if ( col2->is_end(cell2) ) {
	  break;
	}
      }
      if ( found &&
	   matrix.col_cost(col2->pos()) <= matrix.col_cost(col->pos()) ) {
	// col2 は col を支配している．
	matrix.delete_col(col->pos());
	change = true;
	break;
      }
    }
  }
  
  return change;
}

// @brief 必須列を探し，列を選択する．
// @param[in] matrix 対象の行列
// @param[out] selected_cols 選択された列を追加する列集合
// @return 選択された列があったら true を返す．
bool
MincovSolver::essential_col(MincovMatrix& matrix,
			    vector<ymuint32>& selected_cols)
{
  vector<ymuint32> tmp_list;
  tmp_list.reserve(matrix.col_size());
  for (const MincovRowHead* row = matrix.row_front();
       !matrix.is_row_end(row); row = row->next()) {
    if ( row->num() == 1 ) {
      const MincovCell* cell = row->front();
      ymuint32 col_pos = cell->col_pos();
      selected_cols.push_back(col_pos);
      tmp_list.push_back(col_pos);
    }
  }
  for (vector<ymuint32>::iterator p = tmp_list.begin();
       p != tmp_list.end(); ++ p) {
    ymuint32 col_pos = *p;
    matrix.select_col(col_pos);
  }
  return !tmp_list.empty();
}

END_NAMESPACE_YM_MINCOV
