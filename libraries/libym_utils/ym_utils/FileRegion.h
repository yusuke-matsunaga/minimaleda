#ifndef YM_UTILS_FILEREGION_H
#define YM_UTILS_FILEREGION_H

/// @file ym_utils/FileRegion.h
/// @brief FileRegion のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: FileRegion.h 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"
#include "ym_utils/FileLoc.h"


BEGIN_NAMESPACE_YM

// 前方参照のためのクラス宣言
// 本物は <ym_utils/FileDesc.h> に定義してある．
class FileDesc;

//////////////////////////////////////////////////////////////////////
/// @class FileRegion FileLoc.h <ymutils/FileLoc.h>
/// @ingroup YmUtils
/// @brief ファイル上の領域を表すクラス
/// @sa FileLoc
//////////////////////////////////////////////////////////////////////
class FileRegion
{
public:
  
  /// @brief 空のデフォルトコンストラクタ
  /// @note 結果は invalid な情報を持つオブジェクトとなる．
  FileRegion();

  /// @brief 内容を指定したコンストラクタ
  /// @param[in] file_desc ファイル記述子
  /// @param[in] start_line 先頭の行番号
  /// @param[in] start_column 先頭のコラム番号
  /// @param[in] end_line 末尾の行番号
  /// @param[in] end_column 末尾のコラム番号
  FileRegion(const FileDesc* file_desc,
	     ymuint start_line,
	     ymuint start_column,
	     ymuint end_line,
	     ymuint end_column);

  /// @brief 内容を指定したコンストラクタ
  /// @param[in] start_file_desc 先頭のファイル記述子
  /// @param[in] start_line 先頭の行番号
  /// @param[in] start_column 先頭のコラム番号
  /// @param[in] end_file_desc 末尾のファイル記述
  /// @param[in] end_line 末尾の行番号
  /// @param[in] end_column 末尾のコラム番号
  FileRegion(const FileDesc* start_file_desc,
	     ymuint start_line,
	     ymuint start_column,
	     const FileDesc* end_file_desc,
	     ymuint end_line,
	     ymuint end_column);

  /// @brief 内容を指定したコンストラクタ
  /// @param[in] file_loc ファイル上の位置
  FileRegion(const FileLoc& file_loc);

  /// @brief 内容を指定したコンストラクタ
  /// @param[in] start_file_loc 先頭のファイル上の位置
  /// @param[in] end_file_loc 末尾ののファイル上の位置
  FileRegion(const FileLoc& start_file_loc,
	     const FileLoc& end_file_loc);

  /// @brief 内容を指定したコンストラクタ
  /// @param[in] start_file_region 先頭のファイル上の位置
  /// @param[in] end_file_region 末尾ののファイル上の位置
  FileRegion(const FileRegion& start_file_region,
	     const FileRegion& end_file_region);
  
  /// @brief デストラクタ
  ~FileRegion();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取り出す関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief データの妥当性のチェック
  /// @return 意味のある値を持っているとき true を返す．
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
  
  /// @brief 先頭の行番号の取得
  /// @return 行番号
  ymuint
  start_line() const;

  /// @brief 先頭のコラム位置の取得
  /// @return コラム位置
  ymuint
  start_column() const;
  
  /// @brief 末尾の行番号の取得
  /// @return 行番号
  ymuint
  end_line() const;

  /// @brief 末尾のコラム位置の取得
  /// @return コラム位置
  ymuint
  end_column() const;

  /// @brief 先頭のファイル位置の取得
  /// @return ファイル位置
  FileLoc
  start_loc() const;

  /// @brief 末尾のファイル位置の取得
  /// @return ファイル位置
  FileLoc
  end_loc() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 先頭のファイル位置
  FileLoc mStartLoc;
  
  // 末尾の行番号とコラム位置 or 補助的な情報
  ympuint mExtraData;
  
};


//////////////////////////////////////////////////////////////////////
/// @name FileRegion の出力関数
/// @{

/// @relates FileRegion
/// @brief FileRegion を表示するための関数
/// @param[in] s 出力ストリーム
/// @param[in] file_region ファイル領域の情報
/// @return s をそのまま返す
ostream&
operator<<(ostream& s,
	   const FileRegion& file_region);

/// @}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief データの妥当性のチェック
// @return 意味のある値を持っているとき true を返す．
inline
bool
FileRegion::is_valid() const
{
  return mStartLoc.is_valid();
}
  
// @brief ファイル記述子の取得
// @return ファイル記述子を返す．
inline
const FileDesc*
FileRegion::file_desc() const
{
  return start_loc().file_desc();
}
  
// @brief ファイル名の取得
// @return ファイル名を返す．
inline
const char*
FileRegion::filename() const
{
  return start_loc().filename();
}

// @brief インクルード元の親のファイル情報を返す．
// @return インクルード元の親のファイル位置情報を返す．
// @note インクルードされていないファイルの場合には
// NULL が返される．
inline
const FileLoc*
FileRegion::parent_file_loc() const
{
  return start_loc().parent_file_loc();
}

// @brief インクルード元の親のファイル情報をリストに積む．
// @note トップレベルのファイルが先頭になる．
inline
void
FileRegion::parent_file_list(list<const FileLoc*> file_list) const
{
  start_loc().parent_file_list(file_list);
}

// @brief 先頭の行番号の取得
// @return 行番号
inline
ymuint
FileRegion::start_line() const
{
  return start_loc().line();
}

// @brief 先頭のコラム位置の取得
// @return コラム位置
inline
ymuint
FileRegion::start_column() const
{
  return start_loc().column();
}

// @brief 先頭のファイル位置の取得
// @return ファイル位置
inline
FileLoc
FileRegion::start_loc() const
{
  return mStartLoc;
}

END_NAMESPACE_YM

#endif // YM_UTILS_FILEREGION_H
