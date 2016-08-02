
/// @file libym_utils/NameMgr.cc
/// @brief NameMgr の実装ファイル
/// @author Yusuke Matsunaga
///
/// $Id: NameMgr.cc 958 2007-08-28 05:38:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/NameMgr.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス NameMgr
//////////////////////////////////////////////////////////////////////

// コンストラクタ
NameMgr::NameMgr(const char* prefix,
		 const char* suffix) :
  mPrefix(prefix),
  mSuffix(suffix)
{
}

// デストラクタ
NameMgr::~NameMgr()
{
}

// @brief 接頭語と接尾語を変更する．
// @param[in] prefix 接頭語
// @param[in] suffix 接尾語
// @note 既に登録されている名前はクリアされる．
void
NameMgr::change(const char* prefix,
		const char* suffix)
{
  clear();
  mPrefix = prefix;
  mSuffix = suffix;
}

// 登録している名前を全てクリアする
void
NameMgr::clear()
{
  mInterval.clear();
}

// 次に使用可能な名前を接頭語も含めて返す．
// 使用可能な名前がない場合(!!!)，アボートする．
const StrBuff&
NameMgr::new_name(bool add_name)
{
  int d = mInterval.avail_num();
  assert_cond(d >= 0, __FILE__, __LINE__);
  mTmpString.reserve(mPrefix.size() + mSuffix.size() + 10);
  mTmpString.clear();
  mTmpString.put_str(mPrefix);
  mTmpString.put_digit(d);
  mTmpString.put_str(mSuffix);
  mLastNum = d;

  if ( add_name ) {
    mInterval.erase(d);
  }

  return mTmpString;
}

// 名前を登録する．
// name が <prefix>ddd<suffix> の形でない場合にはなにもしない．
void
NameMgr::add(const char* name)
{
  int d = -1;
  if ( strcmp(name, mTmpString.c_str()) == 0 ) {
    // name が直前の new_name() で作られたものなら数字はすぐにわかる．
    d = mLastNum;
  }
  else {
    // 数字を抜き出す．
    d = str_to_num(name);
  }
  
  if ( d != -1 ) {
    // 区間から d を削除
    mInterval.erase(d);
  }
}

// 名前を削除する(使用可能にする)．
// name が <prefix>ddd<suffix> の形でない場合にはなにもしない．
void
NameMgr::erase(const char* name)
{
  // 数字を抜き出す．
  int d = str_to_num(name);

  if ( d != -1 ) {
    // 区間に d を追加
    mInterval.add(d);
  }
}

// name が <prefix>ddd<suffix> の形の場合に
// ddd の部分を数値に直したものを返す．
// そうでなければ -1 を返す．
int
NameMgr::str_to_num(const char* name) const
{
  size_t plen = mPrefix.size();
  size_t slen = mSuffix.size();
  size_t nlen = strlen(name);

  // 接頭語と接尾語を足した長さのほうが長ければ数値があるわけない．
  if ( plen + slen >= nlen ) {
    return -1;
  }
  
  // prefix を比較
  for (size_t i = 0; i < plen; ++ i) {
    if ( mPrefix[i] != name[i] ) {
      return -1;
    }
  }
  // suffix を比較
  for (size_t i = 0; i < slen; ++ i ) {
    if ( mSuffix[i] != name[nlen - slen + i] ) {
      return -1;
    }
  }
  
  // 数字を抜き出す．
  int d = 0;
  for (size_t i = plen; i < nlen - slen; ++ i) {
    char c = name[i];
    if ( !isdigit(c) ) {
      // 数字以外の文字があった．
      return -1;
    }
    d = (d * 10) + (c - '0');
  }
  
  // それを返す．
  return d;
}

// 内容を表示する．
void
NameMgr::dump(ostream& s) const
{
  s << "<<<<NameMgr>>>>" << endl
    << "Prefix: '" << mPrefix << "'" << endl
    << "Suffix: '" << mSuffix << "'" << endl;
  mInterval.dump(s);
}

END_NAMESPACE_YM
