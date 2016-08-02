#ifndef YM_GDS_GDSRECORD_H
#define YM_GDS_GDSRECORD_H

/// @file ym_gds/GdsRecord.h
/// @brief GdsRecord のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: GdsRecord.h 1343 2008-03-25 17:15:35Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#include <ym_gds/gds_nsdef.h>


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
/// @class GdsRecord GdsRecord.h "ym_gds/GdsRecord.h"
/// @brief GDS-II のひとつのレコードを表すクラス
//////////////////////////////////////////////////////////////////////
class GdsRecord
{
  friend class GdsRecMgr;
  friend class GdsScanner;
  
private:

  /// @brief コンストラクタ
  GdsRecord() { }

  /// @brief デストラクタ
  ~GdsRecord() { }


public:

  /// @brief 先頭のオフセットを取り出す．
  size_t
  offset() const;

  /// @brief サイズを取り出す．
  size_t
  size() const;

  /// @brief レコード型を取り出す．
  tGdsRtype
  rtype() const;

  /// @brief レコード型を表すトークンを返す．
  int
  rtype_token() const;

  /// @brief レコード型を表す文字列を返す．
  const char*
  rtype_string() const;

  /// @brief データ型を取り出す．
  tGdsDtype
  dtype() const;

  /// @brief データ型を表す文字列を返す．
  const char*
  dtype_string() const;

  /// @brief データサイズを取り出す．
  size_t
  dsize() const;

  /// @brief pos 番目のバイトデータを返す．
  tGdsByte
  conv_1byte(size_t pos) const;
  
  /// @brief pos 番目の 2バイトのデータを符号つき数(2の補数表現)に変換する．
  /// @note kGds2Int 用の変換関数
  int
  conv_2byte_int(size_t pos) const;

  /// @brief pos 番目の 4バイトのデータを符号つき数(2の補数表現)に変換する．
  /// @note kGds4Int 用の変換関数
  int
  conv_4byte_int(size_t pos) const;

  /// @brief pos 番目の 4バイトのデータを浮動小数点数に変換する．
  /// @note kGds4Real 用の変換関数
  double
  conv_4byte_real(size_t pos) const;

  /// @brief pos 番目の 8バイトのデータを浮動小数点数に変換する．
  /// @note kGds8Real 用の変換関数
  double
  conv_8byte_real(size_t pos) const;

  /// @brief データを文字列に変換する．
  /// @note 末尾に '\\0'(EndOfString) があるとは限らない．
  /// @note GdsString 用の変換関数
  string
  conv_string() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // このレコードの先頭のオフセット
  size_t mOffset;

  // このレコード全体のサイズ
  size_t mSize;

  // レコード型
  tGdsRtype mRtype;

  // データ型
  tGdsDtype mDtype;

  // データ本体を格納する配列(のダミー定義)
  tGdsByte mData[1];

};

END_NAMESPACE_YM_GDS

#endif
