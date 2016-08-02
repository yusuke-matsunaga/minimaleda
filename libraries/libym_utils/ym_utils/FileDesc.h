#ifndef YM_UTILS_FILEDESC_H
#define YM_UTILS_FILEDESC_H

/// @file ym_utils/FileDesc.h
/// @brief FileDesc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: FileDesc.h 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/FileLoc.h"
#include "ym_utils/StrBuff.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class FileDesc FileDesc.h <ym_utils/FileDesc.h>
/// @ingroup YmUtils
/// @brief ファイルの情報を表すための構造体
/// @note 通常はファイル名だけを持つが，インクルードされたファイルの
/// 場合にはインクルードしている親ファイルのインクルード位置の情報を持つ
/// @sa FileDescMgr FileLoc FileRegion
//////////////////////////////////////////////////////////////////////
class FileDesc
{
  friend class FileDescMgr;
private:
  
  /// @brief ファイル名のみのコンストラクタ
  /// @param[in] name ファイル名
  /// @note インクルードされていないファイルを表す．
  FileDesc(const char* name);
  
  /// @brief インクルード元のファイル情報を含むコンストラクタ
  /// @param[in] name ファイル名
  /// @param[in] file_loc インクルード元のファイル位置
  FileDesc(const char* name,
	   const FileLoc& file_loc);

  /// @brief デストラクタ
  ~FileDesc();


public:

  /// @brief ファイル名の取得
  /// @return ファイル名
  const char*
  name() const;

  /// @brief インクルード元の親のファイル情報を返す．
  /// @return インクルード元の親のファイル位置情報を返す．
  /// @note インクルードされていないファイルの場合には
  /// NULL が返される．
  const FileLoc*
  parent_file_loc() const;
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル名
  StrBuff mName;

  // このファイルをインクルードしているファイル番号と行番号
  // この mFileDesc が NULL ならインクルードされていない．
  FileLoc mParentFileLoc;

};


//////////////////////////////////////////////////////////////////////
/// @name FileDesc の出力関数
/// @{

/// @relates FileDesc
/// @brief FileDesc の内容をストリームに出力する
/// @param[in] s 出力ストリーム
/// @param[in] fd ファイル情報を表すオブジェクト
/// @return s を返す．
ostream&
operator<<(ostream& s,
	   const FileDesc* fd);

/// @}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ファイル名の取得
// @return ファイル名
inline
const char*
FileDesc::name() const
{
  return mName.c_str();
}

// @brief インクルード元の親のファイル情報を返す．
// @return インクルード元の親のファイル位置情報を返す．
// インクルードされていないファイルの場合には is_valid() == false
// であるような FileLoc が返される．
inline
const FileLoc*
FileDesc::parent_file_loc() const
{
  if ( mParentFileLoc.is_valid() ) {
    return &mParentFileLoc;
  }
  return NULL;
}

END_NAMESPACE_YM

#endif // YM_UTILS_FILEDESC_H
