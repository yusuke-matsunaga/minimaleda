#ifndef YM_VERILOG_PT_PTARRAY_H
#define YM_VERILOG_PT_PTARRAY_H

/// @file ym_verilog/pt/PtArray.h
/// @brief PtArray のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: PtArray.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/pt/PtP.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtArray PtArray.h <ym_verilog/pt/PtArray.h>
/// @brief ポインタ配列のテンプレートクラス
//////////////////////////////////////////////////////////////////////
template <typename T>
class PtArray
{
public:

  /// @brief 空のコンストラクタ
  PtArray();

  /// @brief コンストラクタ
  /// @param[in] num 要素数
  /// @param[in] array 配列本体
  PtArray(ymuint32 num,
	  T** array);

  /// @brief デストラクタ
  /// @note このクラスではメモリの開放は行わない．
  ~PtArray();


public:

  /// @brief 要素数の取得
  /// @return 要素数
  ymuint32
  size() const;

  /// @brief 要素の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < size() )
  T*
  operator[](ymuint32 pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素数
  ymuint32 mNum;

  // ポインタ配列本体
  T** mArray;

};


//////////////////////////////////////////////////////////////////////
// PtArray のインライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
template <typename T>
inline
PtArray<T>::PtArray() :
  mNum(0),
  mArray(NULL)
{
}

// @brief コンストラクタ
// @param[in] num 要素数
// @param[in] array 配列本体
template <typename T>
inline
PtArray<T>::PtArray(ymuint32 num,
		    T** array) :
  mNum(num),
  mArray(array)
{
}

// @brief デストラクタ
template <typename T>
inline
PtArray<T>::~PtArray()
{
  // mArray は他で確保されていると仮定する．
}

// @brief 要素数の取得
// @return 要素数
template <typename T>
inline
ymuint32
PtArray<T>::size() const
{
  return mNum;
}

// @brief 要素の取得
// @param[in] pos 位置番号 ( 0 <= pos < size() )
template <typename T>
inline
T*
PtArray<T>::operator[](ymuint32 pos) const
{
  return mArray[pos];
}

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_PT_PTARRAY_H
