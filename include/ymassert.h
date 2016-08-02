#ifndef YMASSERT_H
#define YMASSERT_H

/// @file ymassert.h
/// @brief assertion 関係のクラス
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: ymassert.h 693 2007-05-28 09:13:57Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.

#include <ymnsdef.h>

BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @brief assertion チェック用の変数(定数)
//////////////////////////////////////////////////////////////////////
#ifdef NDEBUG
const bool ymtools_check = false;
#else
// NDEBUG に関わらずチェックを行いたくなければ
// この値を false にすればよい．
const bool ymtools_check = true;
#endif


//////////////////////////////////////////////////////////////////////
/// @class AssertError ymtools.h <ymtools.h>
/// @brief assert 違反用の例外の基底クラス
//////////////////////////////////////////////////////////////////////
class AssertError :
  public exception
{
public:
  
  /// @brief コンストラクタ
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  AssertError(const char* file,
	      int line) :
    mFileName(file),
    mLineNumber(line)
  {
  }


public:

  /// @brief assert 違反の発生したソースファイル名
  const char* mFileName;

  /// @brief assert 違反の発生したソースファイルの行番号
  int mLineNumber;

};


//////////////////////////////////////////////////////////////////////
/// @relates AssertError
/// @brief AssertError を ostream に書き出す
/// @param[in] s 出力ストリーム
/// @param[in] obj エラーオブジェクト
/// @return s をそのまま返す
//////////////////////////////////////////////////////////////////////
inline
ostream&
operator<<(ostream& s,
	   const AssertError& obj)
{
  s << "assertion failed at file: " << obj.mFileName
    << ", line: " << obj.mLineNumber;
  return s;
}


//////////////////////////////////////////////////////////////////////
/// @class AssertNotReached ymtools.h <ymtools/ymtools.h>
/// @brief 到達してはいけない部分に達したときの例外
//////////////////////////////////////////////////////////////////////
class AssertNotReached :
  public AssertError
{
public:
  /// @brief コンストラクタ
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @note 通常 file, line には __FILE__, __LINE__ マクロを用いる．
  AssertNotReached(const char* file,
		   int line) :
    AssertError(file, line) { }
};


//////////////////////////////////////////////////////////////////////
/// @relates AssertNotReached
/// @brief 通常は到達してはいけない部分に達したときに例外を投げる関数．
/// @param[in] file 例外の発生したソースファイル名
/// @param[in] line 例外の発生したソースファイルの行番号
/// @note 通常 file, line には __FILE__, __LINE__ マクロを用いる．
//////////////////////////////////////////////////////////////////////
inline
void
assert_not_reached(const char* file,
		   int line)
{
  if ( ymtools_check ) throw AssertNotReached(file, line);
}


//////////////////////////////////////////////////////////////////////
/// @brief assert 違反で任意の例外クラスを投げるテンプレート関数
/// @param[in] assertion 例外発生条件
/// @param[in] except エラーオブジェクト
//////////////////////////////////////////////////////////////////////
template<typename A,
	 typename E>
void
assert_cond(A assertion,
	    E except)
{
  if ( ymtools_check && !assertion ) throw except;
}


//////////////////////////////////////////////////////////////////////
/// @relates AssertError
/// @brief assert 違反で AssertError 例外を投げるテンプレート関数
/// @param[in] assertion 例外発生条件
/// @param[in] file ファイル名
/// @param[in] line 行番号
/// @note 通常 file, line には __FILE__, __LINE__ マクロを用いる．
//////////////////////////////////////////////////////////////////////
template<typename A>
void
assert_cond(A assertion,
	    const char* file,
	    int line)
{
  if ( ymtools_check && !assertion ) throw AssertError(file, line);
}

END_NAMESPACE_YM

#endif // YMASSERT_H
