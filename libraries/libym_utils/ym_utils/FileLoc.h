#ifndef YM_UTILS_FILELOC_H
#define YM_UTILS_FILELOC_H

/// @file ym_utils/FileLoc.h
/// @brief FileLocのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: FileLoc.h 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"


BEGIN_NAMESPACE_YM

// 前方参照のためのクラス宣言
// 本物は <ym_tuils/FileDesc.h> に定義してある．
class FileDesc;

//////////////////////////////////////////////////////////////////////
/// @class FileLoc FileLoc.h <ym_utils/FileLoc.h>
/// @ingroup YmUtils
/// @brief ファイル位置を表すクラス
///
/// 基本的には
/// - ファイル名
/// - 行番号
/// - コラム位置
/// の情報を持つ
/// 実際にはファイルがインクルードされているときに親のファイルの情報
/// も持つのでファイル名の代わりに FileDesc へのポインタを持つ．
/// @sa FileDesc FileRegion
//////////////////////////////////////////////////////////////////////
class FileLoc
{
public:

  /// @brief 空のコンストラクタ
  /// @note 無効なデータを持つ
  FileLoc();
  
  /// @brief 内容を指定するコンストラクタ
  /// @param[in] file_desc ファイル記述子
  /// @param[in] line 行番号
  /// @param[in] column コラム番号
  FileLoc(const FileDesc* file_desc,
	  ymuint line,
	  ymuint column);
  
  /// @brief デストラクタ
  ~FileLoc();

  
public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取り出す関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief データの妥当性のチェック
  /// @retval true 意味のある値を持っている時
  /// @retval false 無効なデータの時
  bool
  is_valid() const;
  
  /// @brief ファイル記述子の取得
  /// @return ファイル記述子を返す．
  const FileDesc*
  file_desc() const;
  
  /// @brief ファイル名の取得
  /// @return ファイル名を返す．
  const char*
  filename() const;

  /// @brief インクルード元の親のファイル情報を返す．
  /// @return インクルード元の親のファイル位置情報を返す．
  /// @note インクルードされていないファイルの場合には
  /// NULL が返される．
  const FileLoc*
  parent_file_loc() const;

  /// @brief インクルード元の親のファイル情報をリストに積む．
  /// @note トップレベルのファイルが先頭になる．
  void
  parent_file_list(list<const FileLoc*> file_list) const;
  
  /// @brief 行番号の取得
  /// @return 行番号
  ymuint
  line() const;

  /// @brief コラム位置の取得
  /// @return コラム位置
  ymuint
  column() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル記述子
  const FileDesc* mFileDesc;

  // 行番号とコラム位置
  // 上位 20 ビットが行番号
  // 下位 12 ビットがコラム位置
  ymuint32 mLineColumn;

};


//////////////////////////////////////////////////////////////////////
/// @name FileLoc の出力関数
/// @{

/// @relates FileLoc
/// @brief FileLoc の内容をストリームに出力する
/// @param[in] s 出力ストリーム
/// @param[in] file_loc ファイル位置を表すオブジェクト
/// @return s を返す．
ostream&
operator<<(ostream& s,
	   const FileLoc& file_loc);

/// @}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// 空のコンストラクタ
// 無効なデータを持つ
inline
FileLoc::FileLoc() :
  mFileDesc(NULL),
  mLineColumn(0)
{
}

// @brief 内容を指定するコンストラクタ
// @param[in] file_desc ファイル記述子
// @param[in] line 行番号
// @param[in] column コラム番号
inline
FileLoc::FileLoc(const FileDesc* file_desc,
		 ymuint line,
		 ymuint column) :
  mFileDesc(file_desc),
  mLineColumn((line << 12) | (column & 0xfff))
{
}

// @brief デストラクタ
inline
FileLoc::~FileLoc()
{
}

// @brief データの妥当性のチェック
// @retval true 意味のある値を持っている時
// @retval false 無効なデータの時
inline
bool
FileLoc::is_valid() const
{
  return mFileDesc != NULL;
}

// @brief ファイル記述子の取得
// @return ファイル記述子を返す．
inline
const FileDesc*
FileLoc::file_desc() const
{
  return mFileDesc;
} 

// @brief 行番号の取得
// @return 行番号
inline
ymuint
FileLoc::line() const
{
  return mLineColumn >> 12;
}

// @brief コラム位置の取得
// @return コラム位置
inline
ymuint
FileLoc::column() const
{
  return mLineColumn & 0xfff;
}

END_NAMESPACE_YM

#endif // YM_UTILS_FILELOC_H
