#ifndef YM_UTILS_SHSTRING_H
#define YM_UTILS_SHSTRING_H

/// @file ym_utils/ShString.h
/// @brief ShString のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ShString.h 1052 2007-10-24 11:08:51Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"


BEGIN_NAMESPACE_YM

class StrPool;

/// @relates ShString
/// @ingroup YmUtils
/// @brief 共有された文字列用の ID 番号の型
/// @note 実際には const char* をキャストしただけ
typedef const void* StrId;


//////////////////////////////////////////////////////////////////////
/// @class ShString ShString.h <ym_utils/ShString.h>
/// @ingroup ShStringGroup
/// @brief StrPool で共有された文字列へのオートポインタ
/// @sa StrPool
//////////////////////////////////////////////////////////////////////
class ShString
{
public:
  /// @brief 空のコンストラクタ
  /// @note NULL がセットされる．
  ShString();

  /// @brief id を指定したコンストラクタ
  /// @param[in] id 文字列ID
  ShString(StrId id);

  /// @brief C文字列を指定したコンストラクタ
  /// @param[in] str C文字列
  explicit
  ShString(const char* str);

  /// @brief string を指定したコンストラクタ
  /// @param[in] str 文字列 (string)
  explicit
  ShString(const string& str);
  
  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  ShString(const ShString& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 自分自身を返す．
  const ShString&
  operator=(const ShString& src);

  /// @brief C文字列からのからの代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 自分自身を返す．
  const ShString&
  operator=(const char* src);

  /// @brief string からのからの代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 自分自身を返す．
  const ShString&
  operator=(const string& src);
  
  /// @brief デストラクタ
  ~ShString();

  /// @brief const char* に変換する．
  operator const char*() const;

  /// @brief string に変換する．
  operator string() const;

  /// @brief id を返す．
  StrId
  id() const;

  /// @brief ハッシュ用のキーを返す．
  size_t
  hash() const;

  
private:

  /// @brief 共有文字列を作ってセットする．
  /// @param[in] str 入力の文字列
  void
  set(const char* str);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 文字列へのポインタ
  // ただし StrPool 上で共有化されている．
  const char* mPtr;

};

/// @name 共有文字列関係の関数
/// @{

/// @relates ShString
/// @ingroup YmUtils
/// @brief 等価比較演算子
/// @param[in] a, b 比較対象の文字列
/// @return a と b が同じ文字列の時 true を返す．
bool
operator==(const ShString& a,
	   const ShString& b);

/// @relates ShString
/// @ingroup YmUtils
/// @brief 非等価比較演算子
/// @param[in] a, b 比較対象の文字列
/// @return a と b が異なる文字列の時 true を返す．
bool
operator!=(const ShString& a,
	   const ShString& b);

/// @relates ShString
/// @ingroup YmUtils
/// @brief ShString 用のストリーム出力演算子
/// @param[in] s 出力ストリーム
/// @param[in] str 出力対象の文字列
/// @return s をそのまま返す．
ostream&
operator<<(ostream& s,
	   const ShString& str);

/// @}


//////////////////////////////////////////////////////////////////////
// ShString のインライン関数
//////////////////////////////////////////////////////////////////////

// 空のコンストラクタ
inline
ShString::ShString() :
  mPtr(NULL)
{
}

// id を指定したコンストラクタ
inline
ShString::ShString(StrId id) :
  mPtr(static_cast<const char*>(id))
{
}

// コピーコンストラクタ
inline
ShString::ShString(const ShString& src) :
  mPtr(src.mPtr)
{
}

// C文字列を指定したコンストラクタ
inline
ShString::ShString(const char* str)
{
  set(str);
}

// string を指定したコンストラクタ
inline
ShString::ShString(const string& str)
{
  set(str.c_str());
}

// 代入演算子
inline
const ShString&
ShString::operator=(const ShString& src)
{
  mPtr = src.mPtr;
  return *this;
}

// C文字列からの代入演算子
inline
const ShString&
ShString::operator=(const char* src)
{
  set(src);
  return *this;
}

// string からの代入演算子
inline
const ShString&
ShString::operator=(const string& src)
{
  set(src.c_str());
  return *this;
}

// デストラクタ
inline
ShString::~ShString()
{
}

// @brief const char* に変換する．
inline
ShString::operator const char*() const
{
  return mPtr;
}

// string に変換する．
inline
ShString::operator string() const
{
  if ( mPtr ) {
    return string(mPtr);
  }
  else {
    return string();
  }
}

// 共有文字列 ID の取得
inline
StrId
ShString::id() const
{
  return static_cast<StrId>(mPtr);
}

// @brief ハッシュ用のキーを返す．
inline
size_t
ShString::hash() const
{
  // 共有されているのでポインタ比較でOK
  return reinterpret_cast<size_t>(mPtr)/sizeof(void*);
}

// 等価比較演算子
inline
bool
operator==(const ShString& a,
	   const ShString& b)
{
  return a.id() == b.id();
}

// 非等価比較演算子
inline
bool
operator!=(const ShString& a,
	   const ShString& b)
{
  return !(a == b);
}

END_NAMESPACE_YM

BEGIN_NAMESPACE_HASH

// ShString をキーにしたハッシュ関数クラスの定義
template <>
struct hash<nsYm::ShString>
{
  size_t
  operator()(nsYm::ShString str) const
  {
    return str.hash();
  }
};

END_NAMESPACE_HASH

#endif // YM_UTILS_STRPOOL_H
