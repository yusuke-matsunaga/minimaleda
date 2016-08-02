#ifndef YM_UTILS_FILEDESCMGR_H
#define YM_UTILS_FILEDESCMGR_H

/// @file ym_utils/FileDescMgr.h
/// @brief FileDescMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: FileDescMgr.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"


BEGIN_NAMESPACE_YM

// 前方参照のためのクラス宣言
// 本物は <ym_utils/FileDesc.h> に定義してある．
class FileDesc;

// 本物は <ym_utils/FileLoc.h> に定義してある．
class FileLoc;

//////////////////////////////////////////////////////////////////////
/// @class FileDescMgr FileDescMgr.h <ym_utils/FileDescMgr.h>
/// @brief FileDesc を生成する管理クラス
//////////////////////////////////////////////////////////////////////
class FileDescMgr
{
public:

  /// @brief コンストラクタ
  FileDescMgr();

  /// @brief デストラクタ
  /// @note このクラスで生成した FileDesc はすべて削除される．
  ~FileDescMgr();


public:

  /// @brief 新しい FileDesc を生成する．
  /// @param[in] filename ファイル名
  /// @return 生成された FileDesc
  const FileDesc*
  new_file_desc(const char* filename);

  /// @brief 新しい FileDesc を生成する．
  /// @param[in] filename ファイル名
  /// @return 生成された FileDesc
  const FileDesc*
  new_file_desc(const string& filename);

  /// @brief 新しい FileDesc を生成する．
  /// @param[in] filename ファイル名
  /// @param[in] parent_file_loc インクルード元の親ファイルの情報
  /// @return 生成された FileDesc
  const FileDesc*
  new_file_desc(const char* filename,
		const FileLoc& parent_file_loc);

  /// @brief 新しい FileDesc を生成する．
  /// @param[in] filename ファイル名
  /// @param[in] parent_file_loc インクルード元の親ファイルの情報
  /// @return 生成された FileDesc
  const FileDesc*
  new_file_desc(const string& filename,
		const FileLoc& parent_file_loc);
  
  /// @brief 生成したすべての FileDesc を削除する．
  void
  clear();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // FileDesc の配列
  vector<FileDesc*> mFdArray;
  
};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 新しい FileDesc を生成する．
// @param[in] filename ファイル名
// @return 生成された FileDesc
inline
const FileDesc*
FileDescMgr::new_file_desc(const string& filename)
{
  return new_file_desc(filename.c_str());
}

// @brief 新しい FileDesc を生成する．
// @param[in] filename ファイル名
// @param[in] parent_file_loc インクルード元の親ファイルの情報
// @return 生成された FileDesc
inline
const FileDesc*
FileDescMgr::new_file_desc(const string& filename,
			   const FileLoc& parent_file_loc)
{
  return new_file_desc(filename.c_str(), parent_file_loc);
}

END_NAMESPACE_YM

#endif // YM_UTILS_FILEDESCMGR_H
