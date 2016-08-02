#ifndef YM_CELL_UNIT_H
#define YM_CELL_UNIT_H

// Copyright (C) 2005 Yusuke Matsunaga
// All rights reserved.
//
// $Source: /export/home/project/synth/cvsrepos/ymtools/src/libym_cell/Unit.h,v $
// $Author: matsunaga $
// $Revision: 1.2 $
// $Date: 2006/01/01 17:56:25 $
//
/**
 * @file libym_cell/Unit.h
 * @author Yusuke Matsunaga
 * 単位を表すクラス
 */

#include <ymtools/cell/common.h>


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// 単位を表すクラス
//////////////////////////////////////////////////////////////////////
class Unit
{
public:
  
  /// コンストラクタ
  /**
     \param bias 小数点の位置．正なら左方向にずらす
     \param ustr 単位を表す文字列
  */
  Unit(int bias, const string& ustr);

  /// デストラクタ
  virtual ~Unit();


public:

  /// バイアスを返す．
  int bias() const;

  /// 単位文字列を返す．
  string ustr() const;

  /// 単位つきの数値の文字列表現をつくる．
  string value2str(int value) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ                                                     //
  //////////////////////////////////////////////////////////////////////

  // バイアス
  int mBias;

  // 単位文字列
  string mUstr;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// バイアスを返す．
inline
int
Unit::bias() const
{
  return mBias;
}

// 単位文字列を返す．
inline
string
Unit::ustr() const
{
  return mUstr;
}

END_NAMESPACE_YM_CELL

#endif // YM_CELL_UNIT_H
