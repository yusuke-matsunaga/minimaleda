
/// @file libym_seal/lu_decomp.cc
/// @brief LU 分解を行って連立方程式を求解するアルゴリズムの実装
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: gauss_elimination.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "Matrix.h"
#include "ym_utils/StopWatch.h"


BEGIN_NAMESPACE_YM_SEAL

// LU分解を行って連立方程式の解を得る．
bool
lu_decomp(const Matrix& src_matrix,
	  vector<double>& solution)
{
  ymuint32 nv = src_matrix.row_size();
  
  if ( nv + 1 != src_matrix.col_size() ) {
    // 1列は右辺の定数だとして変数と数と方程式の数は等しくなければならない．
    cout << "nr + 1 != nc" << endl;
    return false;
  }

#if 0
  Matrix works(src_matrix);
  vector<ymuint32> row_idx(nv);
#endif
  vector<double> max_elem(nv);
  // max_elem の計算
  for (ymuint i = 0; i < nv; ++ i) {
#if 0
    row_idx[i] = i;
#endif
    double max = 0.0;
    for (ymuint j = 0; j < nv; ++ j) {
      double v = src_matrix.elem(i, j);
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
#if 0
  // 行の正規化
  for (ymuint i = 0; i < nv; ++ i) {
    for (ymuint j = 0; j <= nv; ++ j) {
      works.elem(i, j) /= max_elem[i];
    }
  }
#endif

  // LU 分解を行う．
  double* LU = new double[nv * nv];
  for (ymuint i = 0; i < nv; ++ i) {
    for (ymuint j = 0; j < nv; ++ j) {
      LU[i * nv + j] = src_matrix.elem(i, j) / max_elem[i];
    }
  }
  for (ymuint c = 0; c < nv; ++ c) {
    cout << c << " / " << nv << endl;
    // 係数が最大となる行を選ぶ
#if 0
    ymuint max_r = 0;
    {
      double max_v = 0.0;
      for (ymuint i = c; i < nv; ++ i) {
	ymuint r = row_idx[i];
	double v = works.elem(r, c);
	for (ymuint k = 0; k < c; ++ k) {
	  ymuint r1 = row_idx[k];
	  v -= L.elem(r, r1) * U.elem(r1, c);
	}
	v = fabs(v);
	if ( max_v < v ) {
	  max_v = v;
	  max_r = r;
	}
      }
    }
    // max_r を c 番目の行にする．
    for (ymuint j = c + 1; j < nv; ++ j) {
      if ( row_idx[j] == max_r ) {
	ymuint tmp = row_idx[c];
	row_idx[c] = max_r;
	row_idx[j] = tmp;
	break;
      }
    }
#endif

    for (ymuint i = 0; i <= c; ++ i) {
#if 0
      ymuint r1 = row_idx[i];
#else
      ymuint r1 = i;
#endif
      ymuint base = r1 * nv;
      double v = LU[base + c];
      for (ymuint k = 0; k < c; ++ k) {
#if 0
	ymuint r2 = row_idx[k];
#else
	ymuint r2 = k;
#endif
	v -= LU[base + r2] * LU[r2 * nv + c];
      }
      LU[base + c] = v;
    }
    for (ymuint i = c + 1; i < nv; ++ i) {
#if 0
      ymuint r1 = row_idx[i];
#else
      ymuint r1 = i;
#endif
      ymuint base = r1 * nv;
      double v = LU[base + c];
      for (ymuint k = 0; k < i; ++ k) {
#if 0
	ymuint r2 = row_idx[k];
#else
	ymuint r2 = k;
#endif
	v -= LU[base + r2] * LU[r2 * nv + c];
      }
      v /= LU[base + r1];
      LU[base + c] = v;
    }
  }

  // Ly = z より y を求める．
  vector<double> y(nv);
  for (ymuint i = 0; i < nv; ++ i) {
#if 0
    ymuint r = row_idx[i];
#else
    ymuint r = i;
#endif
    double v = src_matrix.elem(r, nv) / max_elem[r];
    for (ymuint k = 0; k < i; ++ k) {
#if 0
      ymuint s = row_idx[k];
#else
      ymuint s = k;
#endif
      v -= LU[r * nv + s] * y[s];
    }
    y[r] = v;
  }

  // Ux = y より x を求める．
  solution.clear();
  solution.resize(nv);
  for (ymuint i = 0; i < nv; ++ i) {
#if 0
    ymuint r = row_idx[nv - i - 1];
#else
    ymuint r = nv - i - 1;
#endif
    double v = y[r];
    for (ymuint k = 0; k < i; ++ k) {
#if 0
      ymuint s = row_idx[nv - k - 1];
#else
      ymuint s = nv - k - 1;
#endif
      v -= LU[r * nv + s] * solution[s];
    }
    v /= LU[r * nv + r];
    solution[r] = v;
  }
  delete [] LU;
    
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

END_NAMESPACE_YM_SEAL
