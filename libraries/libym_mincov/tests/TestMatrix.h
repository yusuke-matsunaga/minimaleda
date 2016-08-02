#ifndef TESTMATRIX_H
#define TESTMATRIX_H

/// @file libym_mincov/tests/TestMatrix.h
/// @brief TestMatrix のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TestMatrix.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mincov/MincovMatrix.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class TestMatrix TestMatrix.h "TestMatrix.h"
/// @brief テストの行列
//////////////////////////////////////////////////////////////////////
class TestMatrix
{
public:
  
  /// @brief コンストラクタ
  /// @param[in] row_size 行数
  /// @param[in] col_size 列数
  /// @param[in] cost_size コストの次元
  TestMatrix(ymuint32 row_size,
	     ymuint32 col_size,
	     ymuint32 cost_size);
  
  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元の行列
  TestMatrix(const TestMatrix& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元の行列
  const TestMatrix&
  operator=(const TestMatrix& src);
  
  /// @brief デストラクタ
  ~TestMatrix();


public:

  /// @brief 行数を得る．
  ymuint32
  row_size() const;

  /// @brief 列数を得る．
  ymuint32
  col_size() const;
  
  /// @brief コストの次元を返す．
  ymuint32
  cost_size() const;

  /// @brief 要素を得る．
  /// @param[in] row_pos 行番号 ( 0 <= row_pos < row_size() )
  /// @param[in] col_pos 列番号 ( 0 <= col_pos < col_size() )
  int
  elem(ymuint32 row_pos,
       ymuint32 col_pos) const;

  /// @brief 内容を出力する．
  void
  display(ostream& s) const;

  
public:
  
  /// @brief 内容をクリアする．
  /// @note 行/列のサイズは不変
  void
  clear();
  
  /// @brief サイズを変更する．
  /// @param[in] row_size 行数
  /// @param[in] col_size 列数
  /// @param[in] cost_size コストの次元
  /// @note 内容はクリアされる．
  void
  resize(ymuint32 row_size,
	 ymuint32 col_size,
	 ymuint32 cost_size);
  
  /// @brief 要素を追加する．
  /// @param[in] row_pos 追加する要素の行番号
  /// @param[in] col_pos 追加する要素の列番号
  void
  set(ymuint32 row_pos,
      ymuint32 col_pos);

  /// @brief コストを設定する．
  /// @param[in] col_pos 列番号
  /// @param[in] val_pos 値の位置番号
  /// @param[in] value 設定する値
  void
  set_cost(ymuint32 col_pos,
	   ymuint32 val_pos,
	   double value);
  
  /// @brief 列を選択し，被覆される行を削除する．
  /// @param[in] col_pos 選択した列
  void
  select_col(ymuint32 col_pos);
  
  /// @brief 行を削除する．
  void
  delete_row(ymuint32 row_pos);
  
  /// @brief 列を削除する．
  void
  delete_col(ymuint32 col_pos);
  
  /// @brief 削除スタックにマーカーを書き込む．
  void
  backup();

  /// @brief 直前のマーカーまで処理を戻す．
  void
  restore();
  

private:
  
  /// @brief 一貫性のチェックを行う．
  bool
  sanity_check();
  
  /// @brief 指定された行の非ゼロ要素の列番号を得る．
  /// @param[in] row_pos 行番号 ( 0 <= row_pos < row_size() )
  /// @param[out] cols 列番号を納めるベクタ
  void
  row_vector(ymuint32 row_pos,
	     vector<ymuint32>& cols) const;

  /// @brief 指定された列の非ゼロ要素の行番号を得る．
  /// @param[in] col_pos 列番号 ( 0 <= col_pos < col_size() )
  /// @param[out] rows 行番号を納めるベクタ
  void
  col_vector(ymuint32 col_pos,
	     vector<ymuint32>& rows) const;

  /// @brief 要素に値をセットする．
  /// @param[in] row_pos 行番号 ( 0 <= row_pos < row_size() )
  /// @param[in] col_pos 列番号 ( 0 <= col_pos < col_size() )
  void
  set_elem(ymuint32 row_pos,
	   ymuint32 col_pos,
	   int val);
  
  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 行数
  ymuint32 mRowSize;

  // 列数
  ymuint32 mColSize;
  
  // コストの次元
  ymuint32 mCostSize;
  
  // 要素の配列 (サイズは mRowSize * mColSize)
  vector<int> mBody;

  // backup/restore 用のスタック
  vector<TestMatrix*> mStack;
  
  // 本当の行列
  MincovMatrix mMatrix;
  
};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 行数を得る．
inline
ymuint32
TestMatrix::row_size() const
{
  return mRowSize;
}

// @brief 列数を得る．
inline
ymuint32
TestMatrix::col_size() const
{
  return mColSize;
}
  
// @brief コストの次元を返す．
inline
ymuint32
TestMatrix::cost_size() const
{
  return mCostSize;
}

// @brief 要素を得る．
// @param[in] row_pos 行番号 ( 0 <= row_pos < row_size() )
// @param[in] col_pos 列番号 ( 0 <= col_pos < col_size() )
inline
int
TestMatrix::elem(ymuint32 row_pos,
		 ymuint32 col_pos) const
{
  return mBody[row_pos * mColSize + col_pos];
}

// @brief 要素に値をセットする．
// @param[in] row_pos 行番号 ( 0 <= row_pos < row_size() )
// @param[in] col_pos 列番号 ( 0 <= col_pos < col_size() )
inline
void
TestMatrix::set_elem(ymuint32 row_pos,
		     ymuint32 col_pos,
		     int val)
{
  mBody[row_pos * mColSize + col_pos] = val;
}

END_NAMESPACE_YM_MINCOV

#endif // TESTMATRIX_H
