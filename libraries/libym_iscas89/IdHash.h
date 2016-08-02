#ifndef LIBYM_ISCAS89_IDHASH_H
#define LIBYM_ISCAS89_IDHASH_H

/// @file libym_iscas89/IdHash.h
/// @brief IdHash のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: IdHash.h 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_iscas89/iscas89_nsdef.h"
#include "ym_utils/Alloc.h"
#include "ym_utils/FileRegion.h"


BEGIN_NAMESPACE_YM_ISCAS89

//////////////////////////////////////////////////////////////////////
/// @class IdCell IdHash.h "IdHash.h"
/// @brief 識別子に関する情報を表すデータ構造
//////////////////////////////////////////////////////////////////////
class IdCell
{
  friend class IdHash;

private:

  /// @brief コンストラクタ
  IdCell(ymuint32 id,
	 const char* str);

  /// @brief デストラクタ
  ~IdCell();


public:

  /// @brief ID番号を得る．
  ymuint32
  id() const;

  /// @brief 文字列を返す．
  const char*
  str() const;
  
  /// @brief 位置情報を返す．
  const FileRegion&
  loc() const;
  
  /// @brief 定義している位置情報を返す．
  const FileRegion&
  def_loc() const;
  
  /// @brief 定義されているか調べる．
  /// @retval true 定義済み
  /// @retval false 未定義
  bool
  is_defined() const;
  
  /// @brief 外部入力かどうか調べる．
  /// @return 外部入力なら true を返す．
  bool
  is_input() const;
  
  /// @brief 外部出力かどうか調べる．
  /// @return 外部出力なら true を返す．
  bool
  is_output() const;
  

public:

  /// @brief 位置情報を設定する．
  void
  set_loc(const FileRegion& loc);

  /// @brief 定義済みの印をつける．
  void
  set_defined();

  /// @brief 外部入力の印をつける．
  void
  set_input();

  /// @brief 外部出力の印をつける．
  void
  set_output();

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID 番号
  ymuint32 mId;
  
  // 位置情報
  FileRegion mLoc;
  
  // 位置情報その2
  FileRegion mLoc2;
  
  // いくつかのフラグ
  ymuint32 mFlags;

  // 次の要素を指すポインタ
  IdCell* mLink;

  // 文字列領域(ダミー)
  char mStr[1];
  

private:
  //////////////////////////////////////////////////////////////////////
  // 定数
  //////////////////////////////////////////////////////////////////////
  
  // defined マークのシフト量
  static
  const int kDefSft = 0;

  // input マークのシフト量
  static
  const int kInSft = 1;

  // output マークのシフト量
  static
  const int kOutSft = 2;

};


//////////////////////////////////////////////////////////////////////
/// @class IdHash IdHash.h "IdHash.h"
/// @brief 識別子を管理するハッシュ表
//////////////////////////////////////////////////////////////////////
class IdHash
{
public:

  /// @brief コンストラクタ
  IdHash();

  /// @brief デストラクタ
  ~IdHash();


public:

  /// @brief 内容をクリアする．
  void
  clear();

  /// @brief 識別子に対応するセルを探す．
  /// @param[in] str 文字列
  /// @param[in] create 存在しないときに新規生成するなら true
  /// @return 対応する IdCell を返す．
  IdCell*
  find(const char* str,
       bool create);
  
  /// @brief 登録されている要素数を返す．(= ID番号の最大値 + 1)
  size_t
  num() const;

  /// @brief ID 番号に対応する IdCell を得る．
  IdCell*
  cell(ymuint32 id) const;
	  
  /// @brief ID 番号から文字列を得る．
  const char*
  str(ymuint32 id) const;

  /// @brief ID番号から位置情報を得る．
  const FileRegion&
  loc(ymuint32 id) const;

  /// @brief ID番号からその識別子を定義している位置情報を得る．
  const FileRegion&
  def_loc(ymuint32 id) const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  // ハッシュ表を拡大する．
  void
  alloc_table(size_t new_size);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // IdCell用のアロケータ
  SimpleAlloc mAlloc;

  // テーブルサイズ
  size_t mTableSize;

  // ハッシュ表
  IdCell** mTable;

  // IDをキーにして IdCell を格納する配列
  vector<IdCell*> mCellArray;
  
  // ハッシュ表を拡張する目安
  size_t mNextLimit;
  
};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ID番号を得る．
inline
ymuint32
IdCell::id() const
{
  return mId;
}

// @brief 位置情報を返す．
inline
const FileRegion&
IdCell::loc() const
{
  return mLoc;
}

// @brief 定義している位置情報を返す．
inline
const FileRegion&
IdCell::def_loc() const
{
  return mLoc2;
}

// @brief 文字列を返す．
inline
const char*
IdCell::str() const
{
  return mStr;
}

// @brief 定義されているか調べる．
// @retval true 定義済み
// @retval false 未定義
inline
bool
IdCell::is_defined() const
{
  return static_cast<bool>((mFlags >> kDefSft) & 1U);
}

// @brief 外部入力かどうか調べる．
// @return 外部入力なら true を返す．
inline
bool
IdCell::is_input() const
{
  return static_cast<bool>((mFlags >> kInSft) & 1U);
}

// @brief 外部出力かどうか調べる．
// @return 外部出力なら true を返す．
inline
bool
IdCell::is_output() const
{
  return static_cast<bool>((mFlags >> kOutSft) & 1U);
}

// @brief 位置情報を設定する．
inline
void
IdCell::set_loc(const FileRegion& loc)
{
  mLoc = loc;
}

// @brief 定義済みの印をつける．
inline
void
IdCell::set_defined()
{
  mFlags |= (1U << kDefSft);
  mLoc2 = mLoc;
}

// @brief 外部入力の印をつける．
inline
void
IdCell::set_input()
{
  mFlags |= (1U << kInSft);
}

// @brief 外部出力の印をつける．
inline
void
IdCell::set_output()
{
  mFlags |= (1U << kOutSft);
}
  
// @brief 登録されている要素数を返す．(= ID番号の最大値 + 1)
inline
size_t
IdHash::num() const
{
  return mCellArray.size();
}

// @brief ID 番号に対応する IdCell を得る．
inline
IdCell*
IdHash::cell(ymuint32 id) const
{
  return mCellArray[id];
}
  
// @brief ID 番号から文字列を得る．
inline
const char*
IdHash::str(ymuint32 id) const
{
  return cell(id)->str();
}

// @brief ID番号から位置情報を得る．
inline
const FileRegion&
IdHash::loc(ymuint32 id) const
{
  return cell(id)->loc();
}

// @brief ID番号からその識別子を定義している位置情報を得る．
inline
const FileRegion&
IdHash::def_loc(ymuint32 id) const
{
  return cell(id)->def_loc();
}

END_NAMESPACE_YM_ISCAS89

#endif // LIBYM_ISCAS89_IDHASH_H
