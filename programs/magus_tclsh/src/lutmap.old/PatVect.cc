
/// @file magus/lutmap/PatVect.cc
/// @brief PatVect の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: PatVect.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2007 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include "magus_config.h"
#endif

#include "PatVect.h"


BEGIN_NAMESPACE_MAGUS_LUTMAP

//////////////////////////////////////////////////////////////////////
// クラス PatVect
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] size サイズ(単位はワード)
PatVect::PatVect(ymuint size) :
  mSize(size)
{
  mVect = new ymuint32[mSize];
}

// @brief コピーコンストラクタ
// @param[in] src ソース
PatVect::PatVect(const PatVect& src) :
  mSize(src.mSize)
{
  mVect = new ymuint32[mSize];
  copy(src);
}

// @brief 代入演算子
// @param[in] src ソース
const PatVect&
PatVect::operator=(const PatVect& src)
{
  if ( mSize != src.mSize ) {
    delete [] mVect;
    mSize = src.mSize;
    mVect = new ymuint32[mSize];
  }
  copy(src);
  return *this;
}

// デストラクタ
PatVect::~PatVect()
{
  delete [] mVect;
}
  
// コピーする．
void
PatVect::copy(const PatVect& src)
{
  assert_cond(mSize == src.mSize, __FILE__, __LINE__);
  for (ymuint i = 0; i < mSize; ++ i) {
    mVect[i] = src.mVect[i];
  }
}
  
// @brief サイズを変更する．
// @note 現在の内容は失われる．
void
PatVect::resize(ymuint size)
{
  if ( mSize != size ) {
    delete [] mVect;
    mSize = size;
    mVect = new ymuint32[mSize];
  }
}
  
// @brief 内容をクリアする(0に初期化する)．
void
PatVect::clear()
{
  for (ymuint i = 0; i < mSize; ++ i) {
    mVect[i] = 0U;
  }
}
  
// @brief 内容をセットする(1に初期化する)．
void
PatVect::set()
{
  for (ymuint i = 0; i < mSize; ++ i) {
    mVect[i] = ~0U;
  }
}

// @brief ランダムパタンを設定する．
// @param[in] randgen 乱数発生器
void
PatVect::set_random(RandGen& randgen)
{
  for (ymuint i = 0; i < mSize; ++ i) {
    mVect[i] = randgen.int32();
  }
}
    
// @brief 等価比較演算子
bool
PatVect::operator==(const PatVect& src) const
{
  assert_cond(mSize == src.mSize, __FILE__, __LINE__);
  for (ymuint i = 0; i < mSize; ++ i) {
    if ( mVect[i] != src.mVect[i] ) {
      return false;
    }
  }
  return true;
}

// @brief 否定を計算して自分自身に代入する．
const PatVect&
PatVect::negate()
{
  for (ymuint i = 0; i < mSize; ++ i) {
    mVect[i] = ~mVect[i];
  }
  return *this;
}

// @brief 論理積を計算して自分自身に代入する．
const PatVect&
PatVect::operator&=(const PatVect& src)
{
  assert_cond(mSize == src.mSize, __FILE__, __LINE__);
  for (ymuint i = 0; i < mSize; ++ i) {
    mVect[i] &= src.mVect[i];
  }
  return *this;
}

// @brief 論理和を計算して自分自身に代入する．
const PatVect&
PatVect::operator|=(const PatVect& src)
{
  assert_cond(mSize == src.mSize, __FILE__, __LINE__);
  for (ymuint i = 0; i < mSize; ++ i) {
    mVect[i] |= src.mVect[i];
  }
  return *this;
}

// @brief 排他的論理和を計算して自分自身に代入する．
const PatVect&
PatVect::operator^=(const PatVect& src)
{
  assert_cond(mSize == src.mSize, __FILE__, __LINE__);
  for (ymuint i = 0; i < mSize; ++ i) {
    mVect[i] ^= src.mVect[i];
  }
  return *this;
}

// @brief ドントケアを考慮して両立するとき true を返す．
// @param[in] opr1, opr2 2つのパタン
// @param[in] dc ドントケアを表すパタン
bool
eq_with_dc(const PatVect& opr1,
	   const PatVect& opr2,
	   const PatVect& dc)
{
  ymuint size = opr1.size();
  assert_cond(opr2.size() == size, __FILE__, __LINE__);
  assert_cond(dc.size() == size, __FILE__, __LINE__);

  for (ymuint i = 0; i < size; ++ i) {
    if ( (opr1.block(i) ^ opr2.block(i)) & ~dc.block(i) ) {
      return false;
    }
  }
  return true;
}

// @brief ドントケアを考慮して両立するとき true を返す．
// @param[in] opr1, opr2 2つのパタン
// @param[in] dc1, dc2 ドントケアを表すパタン
bool
eq_with_dc(const PatVect& opr1,
	   const PatVect& opr2,
	   const PatVect& dc1,
	   const PatVect& dc2)
{
  ymuint size = opr1.size();
  assert_cond(opr2.size() == size, __FILE__, __LINE__);
  assert_cond(dc1.size() == size, __FILE__, __LINE__);
  assert_cond(dc2.size() == size, __FILE__, __LINE__);

  for (ymuint i = 0; i < size; ++ i) {
    if ( (opr1.block(i) ^ opr2.block(i)) & ~dc1.block(i) & ~dc2.block(i) ) {
      return false;
    }
  }
  return true;
}

// @brief 論理式に応じた計算を行う．
// @param[in] expr 論理式
// @param[in] ipat 論理式の入力変数のパタン
// @param[out] opat 論理式の出力のパタン
void
calc_pat(const LogExpr& expr,
	 const vector<PatVect*>& ipat,
	 PatVect& opat)
{
  if ( expr.is_zero() ) {
    opat.resize(ipat[0]->size());
    opat.clear();
    return;
  }
  if ( expr.is_one() ) {
    opat.resize(ipat[0]->size());
    opat.set();
  }
  if ( expr.is_posiliteral() ) {
    tVarId id = expr.varid();
    opat = *ipat[id];
    return;
  }
  if ( expr.is_negaliteral() ) {
    tVarId id = expr.varid();
    opat = ~(*ipat[id]);
    return;
  }
  ymuint nc = expr.child_num();
  
  if ( expr.is_and() ) {
    PatVect tmp;
    calc_pat(expr.child(0), ipat, tmp);
    opat = tmp;
    for (ymuint i = 1; i < nc; ++ i) {
      calc_pat(expr.child(i), ipat, tmp);
      opat &= tmp;
    }
    return;
  }
  if ( expr.is_or() ) {
    PatVect tmp;
    calc_pat(expr.child(0), ipat, tmp);
    opat = tmp;
    for (ymuint i = 1; i < nc; ++ i) {
      calc_pat(expr.child(i), ipat, tmp);
      opat |= tmp;
    }
    return;
  }
  if ( expr.is_xor() ) {
    PatVect tmp;
    calc_pat(expr.child(0), ipat, tmp);
    opat = tmp;
    for (ymuint i = 1; i < nc; ++ i) {
      calc_pat(expr.child(i), ipat, tmp);
      opat ^= tmp;
    }
    return;
  }
  assert_not_reached(__FILE__, __LINE__);
}

END_NAMESPACE_MAGUS_LUTMAP
