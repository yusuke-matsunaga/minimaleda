#ifndef LIBYM_VERILOG_ELABORATOR_EI_EIRANGE_H
#define LIBYM_VERILOG_ELABORATOR_EI_EIRANGE_H

/// @file libym_verilog/elaborator/ei/EiRange.h
/// @brief EiRange のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiRange.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ElbRange.h"
#include "ElbExpr.h"
#include "ym_verilog/pt/PtDecl.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiRange EiRange.h "EiRange.h"
/// @brief ElbRange の実装クラス
//////////////////////////////////////////////////////////////////////
class EiRange :
  public ElbRange
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  EiRange();

  /// @brief デストラクタ
  virtual
  ~EiRange();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;

  /// @brief ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;


public:
  //////////////////////////////////////////////////////////////////////
  // VlRange の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素数(ビット幅)を返す．
  virtual
  ymuint
  size() const;

  /// @brief MSB を返す．
  virtual
  VlExpr*
  left_range() const;

  /// @brief LSB を返す．
  virtual
  VlExpr*
  right_range() const;

  /// @brief MSB の値を返す．
  virtual
  int
  left_range_const() const;

  /// @brief LSB の値を返す．
  virtual
  int
  right_range_const() const;

  /// @brief 範囲のチェック
  /// @param[in] index インデックス
  /// @retval true index が範囲内に入っている．
  /// @retval false index が範囲外
  virtual
  bool
  is_in(int index) const;

  /// @brief LSB からのオフセット値の取得
  /// @param[in] index インデックス
  /// @retval index の LSB からのオフセット index が範囲内に入っている．
  /// @retval -1 index が範囲外
  virtual
  int
  offset(int index) const;

  /// @brief MSB からのオフセット値の取得
  /// @param[in] index インデックス
  /// @retval index の MSB からのオフセット index が範囲内に入っている．
  /// @retval -1 index が範囲外
  virtual
  int
  roffset(int index) const;

  /// @brief offset の逆関数
  /// @param[in] offset LSB からのオフセット値
  /// @return offset に対応したインデックスを返す．
  virtual
  int
  index(int offset) const;

  /// @brief roffset の逆関数
  /// @param[in] roffset MSB からのオフセット値
  /// @return roffset に対応したインデックスを返す．
  virtual
  int
  rindex(int roffset) const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbRange の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を設定する．
  /// @param[in] src 元となる情報
  virtual
  void
  set(const ElbRangeSrc& src);


public:
  //////////////////////////////////////////////////////////////////////
  // クラスメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素数(ビット幅)を返す．
  /// @param[in] left 範囲の MSB
  /// @param[in] right 範囲の LSB
  static
  ymuint
  calc_size(int left,
	    int right);

  /// @brief 範囲のチェック
  /// @param[in] left 範囲の MSB
  /// @param[in] right 範囲の LSB
  /// @param[in] index インデックス
  /// @retval true index が範囲内に入っている．
  /// @retval false index が範囲外
  static
  bool
  is_in(int left,
	int right,
	int index);

  /// @brief LSB からのオフセット値の取得
  /// @param[in] left 範囲の MSB
  /// @param[in] right 範囲の LSB
  /// @param[in] index インデックス
  /// @retval index の LSB からのオフセット index が範囲内に入っている．
  /// @retval -1 index が範囲外
  static
  int
  offset(int left,
	 int right,
	 int index);

  /// @brief MSB からのオフセット値の取得
  /// @param[in] left 範囲の MSB
  /// @param[in] right 範囲の LSB
  /// @param[in] index インデックス
  /// @retval index の MSB からのオフセット index が範囲内に入っている．
  /// @retval -1 index が範囲外
  static
  int
  roffset(int left,
	  int right,
	  int index);

  /// @brief offset の逆関数
  /// @param[in] left 範囲の MSB
  /// @param[in] right 範囲の LSB
  /// @param[in] offset LSB からのオフセット値
  /// @return offset に対応したインデックスを返す．
  static
  int
  index(int left,
	int right,
	int offset);

  /// @brief roffset の逆関数
  /// @param[in] left 範囲の MSB
  /// @param[in] right 範囲の LSB
  /// @param[in] roffset MSB からのオフセット値
  /// @return roffset に対応したインデックスを返す．
  static
  int
  rindex(int left,
	 int right,
	 int roffset);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パース木の範囲定義
  const PtRange* mPtRange;

  // 範囲の MSB
  ElbExpr* mLeftRange;

  // 範囲の LSB
  ElbExpr* mRightRange;

  // MSB の値
  int mLeftVal;

  // LSB の値
  int mRightVal;

};


//////////////////////////////////////////////////////////////////////
/// @class EiRangeImpl EiRange.h "EiRange.h"
/// @brief 範囲を表す部品クラス
/// @note ElbRange の派生クラスではない
//////////////////////////////////////////////////////////////////////
class EiRangeImpl
{
public:

  /// @brief コンストラクタ
  EiRangeImpl();

  /// @brief デストラクタ
  virtual
  ~EiRangeImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // 値をセットする関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を設定する．
  /// @param[in] left 範囲の MSB の式
  /// @param[in] right 範囲の LSB の式
  /// @param[in] left_val 範囲の MSB の値
  /// @param[in] right_val 範囲の LSB の値
  void
  set(ElbExpr* left,
      ElbExpr* right,
      int left_val,
      int right_val);


public:
  //////////////////////////////////////////////////////////////////////
  // EiRangeImpl の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素数(ビット幅)を返す．
  ymuint
  size() const;

  /// @brief MSB を返す．
  ElbExpr*
  left_range() const;

  /// @brief LSB を返す．
  ElbExpr*
  right_range() const;

  /// @brief MSB の値を返す．
  int
  left_range_const() const;

  /// @brief LSB の値を返す．
  int
  right_range_const() const;

  /// @brief 範囲のチェック
  /// @param[in] index インデックス
  /// @retval true index が範囲内に入っている．
  /// @retval false index が範囲外
  bool
  is_in(int index) const;

  /// @brief LSB からのオフセット値の取得
  /// @param[in] index インデックス
  /// @retval index の LSB からのオフセット index が範囲内に入っている．
  /// @retval -1 index が範囲外
  int
  offset(int index) const;

  /// @brief MSB からのオフセット値の取得
  /// @param[in] index インデックス
  /// @retval index の MSB からのオフセット index が範囲内に入っている．
  /// @retval -1 index が範囲外
  int
  roffset(int index) const;

  /// @brief offset の逆関数
  /// @param[in] offset LSB からのオフセット値
  /// @return offset に対応したインデックスを返す．
  int
  index(int offset) const;

  /// @brief roffset の逆関数
  /// @param[in] roffset MSB からのオフセット値
  /// @return roffset に対応したインデックスを返す．
  int
  rindex(int roffset) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 範囲の MSB
  ElbExpr* mLeftRange;

  // 範囲の LSB
  ElbExpr* mRightRange;

  // MSB の値
  int mLeftVal;

  // LSB の値
  int mRightVal;

};


//////////////////////////////////////////////////////////////////////
/// @class EiRangeArray EiRange.h "EiRange.h"
/// @brief EiRangeImpl の配列 + α
//////////////////////////////////////////////////////////////////////
class EiRangeArray
{
public:

  /// @brief コンストラクタ
  /// @brief dim_size 次元数
  EiRangeArray(ymuint dim_size,
	       EiRange* array);

  // デストラクタ
  ~EiRangeArray();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容にアクセスする関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 次元数を得る．
  ymuint
  size() const;

  /// @brief pos 番めの範囲を返す．
  /// @param[in] pos 位置番号
  EiRange*
  range(ymuint pos) const;

  /// @brief 要素数を計算する
  /// @return サイズを返す．
  ymuint
  elem_size() const;

  /// @brief アドレス(オフセット)からインデックスの配列を作る．
  /// @param[in] offset オフセット
  /// @param[out] index_array
  void
  index(ymuint offset,
	vector<int>& index_array) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 次元数
  ymuint32 mDimSize;

  // 範囲の配列
  EiRange* mArray;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief サイズを返す．
inline
ymuint
EiRange::calc_size(int left,
		   int right)
{
  int ans = 0;
  if ( left >= right ) {
    ans = left - right + 1;
  }
  else {
    ans = right - left + 1;
  }
  return ans;
}

// index が範囲内に入っていたら true を返す．
// 範囲外の時には false を返す．
inline
bool
EiRange::is_in(int left,
	       int right,
	       int index)
{
  if ( left >= right ) {
    return right <= index && index <= left;
  }
  else {
    return right >= index && index >= left;
  }
}

// index のLSBからのオフセットを返す．
// 範囲外の時は -1 を返す。
inline
int
EiRange::offset(int left,
		int right,
		int index)
{
  if ( left >= right ) {
    if ( right <= index && index <= left ) {
      return index - right;
    }
  }
  else {
    if ( right >= index && index >= left ) {
      return right - index;
    }
  }
  return -1;
}

// index のMSBからのオフセットを返す．
// 範囲外の時は -1 を返す。
inline
int
EiRange::roffset(int left,
		 int right,
		 int index)
{
  if ( left >= right ) {
    if ( right <= index && index <= left ) {
      return left - index;
    }
  }
  else {
    if ( right >= index && index >= left ) {
      return index - left;
    }
  }
  return -1;
}

// offset の逆関数
inline
int
EiRange::index(int left,
	       int right,
	       int offset)
{
  if ( left >= right ) {
    return offset + right;
  }
  else {
    return right - offset;
  }
}

// roffset の逆関数
inline
int
EiRange::rindex(int left,
		int right,
		int roffset)
{
  if ( left >= right ) {
    return left - roffset;
  }
  else {
    return roffset + left;
  }
}

// @brief 次元数を得る．
inline
ymuint
EiRangeArray::size() const
{
  return mDimSize;
}

// @brief pos 番めの範囲を返す．
// @param[in] pos 位置番号
inline
EiRange*
EiRangeArray::range(ymuint pos) const
{
  return &mArray[pos];
}


END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELABORATOR_EI_EIRANGE_H
