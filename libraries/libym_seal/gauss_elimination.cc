
/// @file libym_seal/gauss_elimination.cc
/// @brief gauss_elimination の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: gauss_elimination.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "Matrix.h"
#include "SMatrix.h"
#include "ym_utils/StopWatch.h"


BEGIN_NAMESPACE_YM_SEAL

// ガウスの消去法を行って連立方程式の解を得る．
bool
gaussian_elimination(const Matrix& src_matrix,
		     vector<double>& solution)
{
  ymuint32 nv = src_matrix.row_size();
  
  if ( nv + 1 != src_matrix.col_size() ) {
    // 1列は右辺の定数だとして変数と数と方程式の数は等しくなければならない．
    cout << "nr + 1 != nc" << endl;
    return false;
  }

  Matrix works(src_matrix);
  vector<ymuint32> row_idx(nv);
  vector<double> max_elem(nv);
  // max_elem の計算
  for (ymuint i = 0; i < nv; ++ i) {
    row_idx[i] = i;
    double max = 0.0;
    for (ymuint j = 0; j < nv; ++ j) {
      double v = works.elem(i, j);
      v = fabs(v);
      if ( max < v ) {
	max = v;
      }
    }
    if ( max == 0.0 ) {
      // すべて 0 の行がある．
      cout << "all 0 row" << endl;
      return false;
    }
    max_elem[i] = max;
  }

  // 変数を一つずつ選んでゆく
  for (ymuint i = 0; i < nv; ++ i) {
    cout << i << " / " << nv << endl;
    // i 番めの係数が最大の行を選ぶ．
    double max = 0.0;
    ymuint max_j = 0;
    for (ymuint j = i; j < nv; ++ j) {
      ymuint r = row_idx[j];
      double v = fabs(works.elem(r, i));
      v /= max_elem[r];
      if ( max < v ) {
	max = v;
	max_j = j;
      }
    }
    if ( max == 0.0 ) {
      // 全ての行でこの変数が消えてしまった．
      cout << "all 0 col" << endl;
      return false;
    }

    // i 番めと max_j 番めの行を入れ替える．
    if ( i != max_j ) {
      ymuint32 tmp = row_idx[i];
      row_idx[i] = row_idx[max_j];
      row_idx[max_j] = tmp;
    }

    // i 番め以降の行からこの変数を消去する．
    // 具体的には i番目以降の行 (j行)から i行 に A_ji / A_ii をかけた
    // ものを引く．
    // ただし， A_ji = 0 の時はなにもしない．
    for (ymuint j = i + 1; j < nv; ++ j) {
      ymuint r_i = row_idx[i];
      ymuint r_j = row_idx[j];
      double d = works.elem(r_j, i) / works.elem(r_i, i);
      // max_elem の更新
      double max = 0.0;
      for (ymuint k = i; k < nv; ++ k) {
	works.elem(r_j, k) -= works.elem(r_i, k) * d;
	double v = fabs(works.elem(r_j, k));
	if ( max < v ) {
	  max = v;
	}
      }
      works.elem(r_j, nv) -= works.elem(r_i, nv) * d;
      max_elem[r_j] = max;
    }
  }

  // 最後のチェック
  if ( works.elem(row_idx[nv - 1], nv - 1) == 0.0 ) {
    cout << "work.elem(row_idx[nv - 1], nv - 1) == 0.0" << endl;
    return false;
  }
  
  // 後方代入
  solution.clear();
  solution.resize(nv);
  for (ymuint i = nv; i -- > 0; ) {
    ymuint r = row_idx[i];
    double v = works.elem(r, nv);
    for (ymuint j = i + 1; j < nv; ++ j) {
      v -= works.elem(r, j) * solution[j];
    }
    solution[i] = v / works.elem(r, i);
  }
  
#if 1
#if 0
  display(cout, src_matrix);
  cout << " --> works" << endl;
  display(cout, works);
#endif
  // 解の検証
  bool error = false;
  for (ymuint i = 0; i < nv; ++ i) {
    double v = 0.0;
    for (ymuint j = 0; j < nv; ++ j) {
      v += src_matrix.elem(i, j) * solution[j];
    }
    double c = src_matrix.elem(i, nv);
    double delta = v - src_matrix.elem(i, nv);
    if ( fabs(delta) >= 1e-10 ) {
      cout << "error at " << i << "th row" << endl
	   << " v = " << v << ", c = " << c << endl
	   << " delta = " << delta << endl;
      error = true;
    }
#if 0
    if ( !is_similar(v, src_matrix.elem(i, nv)) ) {
      cout << "error at " << i << "th row" << endl
	   << " v = " << v << ", c = " << src_matrix.elem(i, nv) << endl;
      double delta = v - src_matrix.elem(i, nv);
      cout << " delta = " << delta << endl;
      error = true;
    }
#endif
  }
  assert_cond(!error, __FILE__, __LINE__);
#endif
  
  return true;
}


// ガウスの消去法を行って連立方程式の解を得る．
bool
gaussian_elimination(const SMatrix& src_matrix,
		     vector<double>& solution)
{
  StopWatch watch;
  ymuint32 nv = src_matrix.size();

  watch.start();
  cout << "gaussian_elimination(" << nv << ")" << endl;
  SMatrix works(nv);
  vector<ymuint32> row_idx(nv);
  vector<double> max_elem(nv);
  vector<bool> fixed(nv, false);
  for (ymuint i = 0; i < nv; ++ i) {
    for (SmCell* cell = src_matrix.row_top(i);
	 cell != src_matrix.row_end(i); cell = cell->right()) {
      ymuint j = cell->col_pos();
      double v = cell->value();
      works.set_value(i, j, v);
    }
    works.set_const(i, src_matrix.const_elem(i));
    row_idx[i] = i;
  }

  // max_elem の計算
  for (ymuint r = 0; r < nv; ++ r) {
    if ( works.row_top(r) == works.row_end(r) ) {
      // すべて 0 の行がある．
      cout << "all 0 row" << endl;
      return false;
    }
    double max = 0.0;
    for (SmCell* cell = works.row_top(r);
	 cell != works.row_end(r); cell = cell->right()) {
      double v = fabs(cell->value());
      if ( max < v ) {
	max = v;
      }
    }
    max_elem[r] = max;
  }
  
  // 変数を一つずつ選んでゆく
  vector<pair<ymuint, SmCell*> > cur_rows;
  cur_rows.reserve(nv);
  for (ymuint c = 0; c < nv; ++ c) {
#if 1
    cout << c << " / " << nv;
    cout.flush();
#endif
    
    // c 番めの係数が最大の行を選ぶ．
    double max = 0.0;
    ymuint max_r = 0;
    SmCell* max_cell = NULL;
    cur_rows.clear();
    for (ymuint r = 0; r < nv; ++ r) {
      if ( fixed[r] ) continue;
      SmCell* cell = works.find_elem(r, c);
      if ( cell ) {
	cur_rows.push_back(make_pair(r, cell));
	double v = fabs(cell->value());
	v /= max_elem[r];
	if ( max < v ) {
	  max = v;
	  max_r = r;
	  max_cell = cell;
	}
      }
    }

    if ( max == 0.0 ) {
      // 全ての行でこの変数が消えてしまった．
      cout << "all 0 col" << endl;
      return false;
    }

#if 1
    cout << " : --> " << max_r
	 << " / " << cur_rows.size();
    cout.flush();
#endif

    // c 番めの行として max_r 行を選ぶ．
    row_idx[c] = max_r;
    fixed[max_r] = true;

    // 残りの行からこの変数を消去する．
    // 具体的には i番目以降の行 (j行)から i行 に A_ji / A_ii をかけた
    // ものを引く．
    // ただし， A_ji = 0 の時はなにもしない．
    double v0 = max_cell->value();
    for (vector<pair<ymuint, SmCell*> >::iterator p = cur_rows.begin();
	 p != cur_rows.end(); ++ p) {
      ymuint r = p->first;
      SmCell* cell = p->second;
      if ( r != max_r ) {
	double d = cell->value() / v0;
	double v = works.pivot(max_r, r, d);
	max_elem[r] = v;
      }
    }
#if 1
    cout << " end (# of cells(K): "
	 << works.cell_num() / 1024
	 << ", used mem: "
	 << works.used_mem() / (1024 * 1024)
	 << "M, allocated: "
	 << works.allocated_mem() / (1024 * 1024)
	 << "M)" << endl;
#endif
  }

#if 0
  // 最後のチェック
  if ( work2.elem(row_idx[nr2 - 1], nr2 - 1) == 0.0 ) {
    cout << "work.elem(row_idx[nr2 - 1], nr2 - 1) == 0.0" << endl;
    return false;
  }
#endif
  
  // 後方代入
  solution.clear();
  solution.resize(nv);
  for (ymuint i = nv; i -- > 0; ) {
    ymuint r = row_idx[i];
    ymuint c = i;
    double v = works.const_elem(r);
    double k = 0;
    for (SmCell* cell = works.row_top(r);
	 cell != works.row_end(r); cell = cell->right()) {
      if ( cell->col_pos() == c ) {
	k = cell->value();
      }
      else {
	v -= cell->value() * solution[cell->col_pos()];
      }
    }
    solution[c] = v / k;
  }
  
#if 1
  // 解の検証
  bool error = false;
  for (ymuint i = 0; i < nv; ++ i) {
    double v = 0.0;
    for (SmCell* cell = src_matrix.row_top(i);
	 cell != src_matrix.row_end(i); cell = cell->right()) {
      v += cell->value() * solution[cell->col_pos()];
    }
    double c = src_matrix.const_elem(i);
    double delta = v - c;
    if ( fabs(delta) >= 1e-15 ) {
      cout << "error at " << i << "th row" << endl
	   << " v = " << v << ", c = " << c << endl
	   << " delta = " << delta << endl;
      error = true;
    }
#if 0
    if ( !is_similar(v, c) ) {
      cout << "error at " << i << "th row" << endl
	   << " v = " << v << ", c = " << c << endl
	   << " delta = " << delta << endl;
      error = true;
    }
#endif
  }
  assert_cond(!error, __FILE__, __LINE__);
#endif

  watch.stop();
  USTime time = watch.time();
  cout << "end" << "  " << time << endl;
  
  return true;
}

END_NAMESPACE_YM_SEAL
