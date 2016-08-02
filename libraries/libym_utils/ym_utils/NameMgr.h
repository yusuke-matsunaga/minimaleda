#ifndef YM_UTILS_NAMEMGR_H
#define YM_UTILS_NAMEMGR_H

/// @file ym_utils/NameMgr.h
/// @brief NameMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: NameMgr.h 958 2007-08-28 05:38:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"
#include "ym_utils/ItvlMgr.h"
#include "ym_utils/StrBuff.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class NameMgr NameMgr.h <ym_utils/NameMgr.h>
/// @ingroup YmUtils
/// @brief \<prefix\>ddd\<suffix\> という形の名前で使用可能なものを
/// 表すためのクラス
//////////////////////////////////////////////////////////////////////
class NameMgr
{
public:

  /// @brief コンストラクタ
  /// @param[in] prefix 接頭語
  /// @param[in] suffix 接尾語
  NameMgr(const char* prefix,
	  const char* suffix);
  
  /// @brief デストラクタ
  ~NameMgr();

  /// @brief 接頭語と接尾語を変更する．
  /// @param[in] prefix 接頭語
  /// @param[in] suffix 接尾語
  /// @note 既に登録されている名前はクリアされる．
  void
  change(const char* prefix,
	 const char* suffix);

  /// @brief 登録している名前を全てクリアする．
  void
  clear();
  
  /// @brief 接頭語を返す．
  string
  prefix() const;
  
  /// @brief 接尾語を返す．
  string
  suffix() const;
  
  /// @brief 次に使用可能な名前を接頭語，接尾語を連結して返す．
  /// @param[in] add_name true の時，名前の登録も行う．
  const StrBuff&
  new_name(bool add_name);

  /// @brief 名前を登録する．
  /// @param[in] name 登録する名前
  /// @note 名前が \<prefix\>ddd\<suffix\> の形でない場合には何もしない．
  void
  add(const char* name);

  /// @brief 名前を削除する(使用可能にする)．
  /// @param[in] name 削除する名前
  /// @note 名前が \<prefix\>ddd\<suffix\> の形でない場合には何もしない．
  void
  erase(const char* name);
  
  /// @brief 内容を表示する．
  /// @param[in] s 出力先のストリーム
  /// @note 主にデバッグ用
  void
  dump(ostream& s) const;
  

private:

  // name が <prefix>ddd<suffix> の形の時に
  // ddd を数値に直したものを返す．
  // 形にあっていない場合には -1 を返す．
  int
  str_to_num(const char* name) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 接頭語
  StrBuff mPrefix;

  // 接尾語
  StrBuff mSuffix;

  // 使用可能な数字を表す区間(interval)リスト
  ItvlMgr mInterval;

  // new_name() で用いる作業領域
  mutable
  StrBuff mTmpString;

  // 最後の new_name() で得られた番号
  mutable
  int mLastNum;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////
  
// 接頭語を返す．
inline
string
NameMgr::prefix() const
{
  return mPrefix;
}

// 接尾語を返す．
inline
string
NameMgr::suffix() const
{
  return mSuffix;
}

END_NAMESPACE_YM

#endif // YM_UTILS_NAMEMGR_H
