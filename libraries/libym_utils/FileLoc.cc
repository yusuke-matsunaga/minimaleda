
/// @file libym_utils/FileLoc.cc
/// @brief ファイル位置を表す構造体の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: FileLoc.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/FileLoc.h"
#include "ym_utils/FileRegion.h"
#include "ym_utils/FileDescMgr.h"
#include "ym_utils/FileDesc.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス FileLoc
//////////////////////////////////////////////////////////////////////

// @brief ファイル名の取得
// @return ファイル名を返す．
const char*
FileLoc::filename() const
{
  if ( is_valid() ) {
    return mFileDesc->name();
  }
  return "<null-file>";
}

// @brief インクルード元の親のファイル情報を返す．
// @return インクルード元の親のファイル位置情報を返す．
// @note インクルードされていないファイルの場合には
// NULL が返される．
const FileLoc*
FileLoc::parent_file_loc() const
{
  if ( is_valid() ) {
    return mFileDesc->parent_file_loc();
  }
  return NULL;
}

// @brief インクルード元の親のファイル情報をリストに積む．
// @note トップレベルのファイルが先頭になる．
void
FileLoc::parent_file_list(list<const FileLoc*> file_list) const
{
  file_list.clear();

  // 親の情報を stack に積んでいく．
  for (const FileLoc* p = mFileDesc->parent_file_loc();
       p; p = p->parent_file_loc()) {
    file_list.push_front(p);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス FileRegion
//////////////////////////////////////////////////////////////////////

BEGIN_NONAMESPACE

// 通常のデータの時 true を返す．
inline
bool
is_normal(ympuint data)
{
  if ( data & 1UL ) {
    return true;
  }
  else {
    return false;
  }
}

// 通常の末尾のデータを作成する．
inline
ympuint
end_data(ymuint end_line,
	 ymuint end_column)
{
  return (((end_line << 12) | (end_column & 0xfff)) << 1) | 1UL;
}

// 別ファイルの場合の末尾のデータを作成する．
inline
ympuint
end_data(const FileDesc* file_desc,
	 ymuint end_line,
	 ymuint end_column)
{
  FileLoc* fl = new FileLoc(file_desc, end_line, end_column);
  return reinterpret_cast<ympuint>(fl);
}

// 通常のデータのときに line を取り出す．
ymuint
extract_line(ympuint data)
{
  return data >> 13;
}

// 通常のデータのときに column を取り出す．
ymuint
extract_column(ympuint data)
{
  return (data >> 1) & 0xfff;
}

// 別ファイルの場合に FileLoc を取り出す．
FileLoc*
end_fl(ympuint data)
{
  return reinterpret_cast<FileLoc*>(data);
}

END_NONAMESPACE

// @brief 空のデフォルトコンストラクタ
// 結果は invalid な情報を持つオブジェクトとなる．
FileRegion::FileRegion() :
  mExtraData(1UL)
{
}

// @brief 内容を指定したコンストラクタ
// @param[in] file_desc ファイル記述子
// @param[in] start_line 先頭の行番号
// @param[in] start_column 先頭のコラム番号
// @param[in] end_line 末尾の行番号
// @param[in] end_column 末尾のコラム番号
FileRegion::FileRegion(const FileDesc* file_desc,
		       ymuint start_line,
		       ymuint start_column,
		       ymuint end_line,
		       ymuint end_column) :
  mStartLoc(file_desc, start_line, start_column),
  mExtraData(end_data(end_line, end_column))
{
}

// @brief 内容を指定したコンストラクタ
// @param[in] start_file_desc 先頭のファイル記述子
// @param[in] start_line 先頭の行番号
// @param[in] start_column 先頭のコラム番号
// @param[in] end_file_desc 末尾のファイル記述
// @param[in] end_line 末尾の行番号
// @param[in] end_column 末尾のコラム番号
FileRegion::FileRegion(const FileDesc* start_file_desc,
		       ymuint start_line,
		       ymuint start_column,
		       const FileDesc* end_file_desc,
		       ymuint end_line,
		       ymuint end_column) :
  mStartLoc(start_file_desc, start_line, start_column)
{
  if ( start_file_desc == end_file_desc ) {
    mExtraData = end_data(end_line, end_column);
  }
  else {
    mExtraData = end_data(end_file_desc, end_line, end_column);
  }
}

// @brief 内容を指定したコンストラクタ
// @param[in] file_loc ファイル上の位置
FileRegion::FileRegion(const FileLoc& file_loc) :
  mStartLoc(file_loc),
  mExtraData(end_data(file_loc.line(), file_loc.column()))
{
}

// @brief 内容を指定したコンストラクタ
// @param[in] start_file_loc 先頭のファイル上の位置
// @param[in] end_file_loc 末尾ののファイル上の位置
FileRegion::FileRegion(const FileLoc& start_file_loc,
		       const FileLoc& end_file_loc) :
  mStartLoc(start_file_loc)
{
  if ( start_file_loc.file_desc() == end_file_loc.file_desc() ) {
    mExtraData = end_data(end_file_loc.line(), end_file_loc.column());
  }
  else {
    mExtraData = end_data(end_file_loc.file_desc(),
			  end_file_loc.line(), end_file_loc.column());
  }
}

// @brief 内容を指定したコンストラクタ
// @param[in] start_file_region 先頭のファイル上の位置
// @param[in] end_file_region 末尾ののファイル上の位置
FileRegion::FileRegion(const FileRegion& start_file_region,
		       const FileRegion& end_file_region) :
  mStartLoc(start_file_region.start_loc())
{
  if ( start_file_region.file_desc() == end_file_region.file_desc() ) {
    mExtraData = end_data(end_file_region.end_line(),
			  end_file_region.end_column());
  }
  else {
    mExtraData = end_data(end_file_region.file_desc(),
			  end_file_region.end_line(),
			  end_file_region.end_column());
  }
}

// @brief デストラクタ
FileRegion::~FileRegion()
{
  if ( !is_normal(mExtraData) ) {
    FileLoc* fl = end_fl(mExtraData);
    delete fl;
  }
}
  
// @brief 末尾の行番号の取得
// @return 行番号
ymuint
FileRegion::end_line() const
{
  if ( is_normal(mExtraData) ) {
    return extract_line(mExtraData);
  }
  else {
    FileLoc* fl = end_fl(mExtraData);
    return fl->line();
  }
}

// @brief 末尾のコラム位置の取得
// @return コラム位置
ymuint
FileRegion::end_column() const
{
  if ( is_normal(mExtraData) ) {
    return extract_column(mExtraData);
  }
  else {
    FileLoc* fl = end_fl(mExtraData);
    return fl->column();
  }
}

// @brief 末尾のファイル位置の取得
// @return ファイル位置
FileLoc
FileRegion::end_loc() const
{
  if ( is_normal(mExtraData) ) {
    return FileLoc(file_desc(),
		   extract_line(mExtraData),
		   extract_column(mExtraData));
  }
  else {
    return *end_fl(mExtraData);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス FileDescMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
FileDescMgr::FileDescMgr()
{
}

// @brief デストラクタ
// @note このクラスで生成した FileDesc はすべて削除される．
FileDescMgr::~FileDescMgr()
{
  clear();
}

// @brief 新しい FileDesc を生成する．
// @param[in] filename ファイル名
// @return 生成された FileDesc
const FileDesc*
FileDescMgr::new_file_desc(const char* filename)
{
  FileDesc* fd = new FileDesc(filename, FileLoc());
  mFdArray.push_back(fd);
  return fd;
}

// @brief 新しい FileDesc を生成する．
// @param[in] filename ファイル名
// @param[in] parent_file_loc インクルード元の親ファイルの情報
// @return 生成された FileDesc
const FileDesc*
FileDescMgr::new_file_desc(const char* filename,
			   const FileLoc& parent_file_loc)
{
  FileDesc* fd = new FileDesc(filename, parent_file_loc);
  mFdArray.push_back(fd);
  return fd;
}

// @brief 生成したすべての FileDesc を削除する．
void
FileDescMgr::clear()
{
  for (size_t i = 0; i < mFdArray.size(); ++ i) {
    delete mFdArray[i];
  }
  mFdArray.clear();
}


//////////////////////////////////////////////////////////////////////
// クラス FileDesc
//////////////////////////////////////////////////////////////////////

// @brief ファイル名のみのコンストラクタ
// @param[in] name ファイル名
// インクルードされていないファイルを表す．
FileDesc::FileDesc(const char* name) :
  mName(name)
{
  // mParentFileLoc は空で初期化される．
}

// @brief インクルード元のファイル情報を含むコンストラクタ
// @param[in] name ファイル名
// @param[in] file_loc インクルード元のファイル位置
FileDesc::FileDesc(const char* name,
		   const FileLoc& file_loc) :
  mName(name),
  mParentFileLoc(file_loc)
{
}

// @brief デストラクタ
FileDesc::~FileDesc()
{
}


//////////////////////////////////////////////////////////////////////
// 出力ストリーム演算子
//////////////////////////////////////////////////////////////////////

// @relates FileLoc
// @brief FileLoc を表示するための関数
// @param[in] s 出力ストリーム
// @param[in] file_loc ファイル位置の情報
// @return s をそのまま返す
ostream&
operator<<(ostream& s,
	   const FileLoc& file_loc)
{
  if ( file_loc.is_valid() ) {
    s << file_loc.file_desc()
      << ": line " << file_loc.line()
      << ", column " << file_loc.column();
  }
  else {
    s << "invalid file_loc";
  }
  return s;
}

// @relates FileRegion
// @brief FileRegion を表示するための関数
// @param[in] s 出力ストリーム
// @param[in] file_region ファイル領域の情報
// @return s をそのまま返す
ostream&
operator<<(ostream& s,
	   const FileRegion& file_region)
{
  const FileLoc& first = file_region.start_loc();
  const FileLoc& last = file_region.end_loc();

  if ( first.is_valid() ) {
    // 意味のあるファイル記述子
    if ( first.file_desc() == last.file_desc() ) {
      // 同じファイル
      s << first.file_desc();
      if ( first.line() == last.line() ) {
	// 同じ行番号
	s << ": line " << first.line();
	if ( first.column() == last.column() ) {
	  // 同じコラム位置
	  s << ", column " << first.column();
	}
	else {
	  // 異なるコラム
	  s << ", column " << first.column()
	    << " - " << last.column();
	}
      }
      else {
	// 異なる行
	s << ": line " << first.line()
	  << ", column " << first.column()
	  << " - line " << last.line()
	  << ", column " << last.column();
      }
    }
    else {
      // 異なるファイル
      s << first.file_desc()
	<< ": line " << first.line()
	<< ", column " << first.column()
	<< " - ";
      s << last.file_desc()
	<< ": line " << last.line()
	<< ", column " << last.column();
    }
  }
  else {
    s << "---";
  }
  return s;
}

// @relates FileDesc
// @brief FileDesc の内容をストリームに出力する
// @param[in] s 出力ストリーム
// @param[in] fd ファイル情報を表すオブジェクト
// @return s を返す．
ostream&
operator<<(ostream& s,
	   const FileDesc* fd)
{
  list<const FileLoc*> file_list;
  for (const FileLoc* p = fd->parent_file_loc();
       p; p = p->parent_file_loc()) {
    file_list.push_front(p);
  }
  for (list<const FileLoc*>::const_iterator p = file_list.begin();
       p != file_list.end(); ++ p) {
    const FileLoc* tmp = *p;
    s << "In file included from " << tmp->filename()
      << ": line " << tmp->line() << ":" << endl;
  }
  s << fd->name();
  return s;
}

END_NAMESPACE_YM
