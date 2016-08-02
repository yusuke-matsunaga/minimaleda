
/// @file libym_utils/File.cc
/// @brief ファイル操作関係のクラスの実装ファイル
/// @author Yusuke Matsunaga
///
/// $Id: File.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/File.h"


/// @todo pwd.h や sys/param.h がないときの対処
#if HAVE_PWD_H
#  include <pwd.h>
#endif
#if HAVE_SYS_PARAM_H
#  include <sys/param.h>
#endif


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// パス名を表すクラス
//////////////////////////////////////////////////////////////////////

// 空のコンストラクタ
PathName::PathName() :
  mType(kRelative)
{
}

// 文字列からの変換コンストラクタ
PathName::PathName(const string& path_str)
{
  mType = kRelative; // デフォルト

  if ( path_str.size() > 0 ) {
    string::size_type pos = 0;
    if ( path_str[0] == '/' ) {
      mType = kAbsolute;
      pos = 1;
    }
    else if ( path_str[0] == '~' ) {
      mType = kHome;
      pos = 1;
    }
    else {
      mType = kRelative;
    }
    for ( ; ; ) {
      string::size_type pos2 = path_str.find('/', pos);
      if ( pos2 != string::npos ) {
	mPathList.push_back(path_str.substr(pos, pos2 - pos));
	pos = pos2 + 1;
      }
      else {
	mPathList.push_back(path_str.substr(pos));
	break;
      }
    }
  }
}

// 文字列からの変換コンストラクタ
PathName::PathName(const char* path_str)
{
  mType = kRelative; // デフォルト

  if ( path_str && strlen(path_str) > 0 ) {
    size_t pos = 0;
    if ( path_str[0] == '/' ) {
      mType = kAbsolute;
      pos = 1;
    }
    else if ( path_str[0] == '~' ) {
      mType = kHome;
      pos = 1;
    }
    else {
      mType = kRelative;
    }
    for ( ; ; ) {
      char c;
      size_t pos2 = pos;
      string buf;
      for ( ; (c = path_str[pos2]); ++ pos2) {
	if ( c == '/' ) {
	  break;
	}
	buf += c;
      }
      mPathList.push_back(buf);
      if ( c == '\0' ) {
	break;
      }
      pos = pos2 + 1;
    }
  }
}

// 文字列のリストからの変換コンストラクタ
PathName::PathName(const list<string>& path_list,
		   tType type) :
  mType(type),
  mPathList(path_list)
{
}

// デストラクタ
PathName::~PathName()
{
}

// パス名の妥当性チェック
bool
PathName::is_valid() const
{
  return !mPathList.empty();
}

// パスの型を返す．
PathName::tType
PathName::type() const
{
  return mType;
}

// パス名の文字列表現を返す．
string
PathName::str() const
{
  string ans;
  switch ( type() ) {
  case kAbsolute:
    ans += '/';
    break;
  case kHome:
    ans += '~';
    break;
  case kRelative:
    break;
  }
  const char* sep = "";
  for (list<string>::const_iterator p = mPathList.begin();
       p != mPathList.end(); ++ p) {
    ans += sep;
    ans += *p;
    sep = "/";
  }
  return ans;
}

// パス名のヘッダ(最後の名前を含まないもの)を返す．
PathName
PathName::head() const
{
  list<string> new_list(mPathList);
  if ( !new_list.empty() ) {
    new_list.pop_back();
  }
  return PathName(new_list, mType);
}

// パス名のテイル(ヘッダを含まないもの)を返す．
string
PathName::tail() const
{
  if ( mPathList.empty() ) {
    return string();
  }
  else {
    return mPathList.back();
  }
}

// 拡張子の直前のドットの位置を返す．
string::size_type
PathName::dot_pos(const string& path)
{
  if ( path == ".." ) {
    return string::npos;
  }
  string::size_type p = path.rfind('.');
  if ( p == 0 ) {
    // path == "." の場合もここではじかれる．
    return string::npos;
  }
  return p;
}

// パス名の本体(テイルから "." と拡張子を除いた物)を返す．
string
PathName::body() const
{
  string tmp = tail();
  string::size_type p = dot_pos(tmp);
  return tmp.substr(0, p);
}

// パス名の拡張子を返す．
string
PathName::ext() const
{
  string tmp = tail();
  string::size_type p = dot_pos(tmp);
  if ( p == string::npos ) {
    return string();
  }
  else {
    return tmp.substr(p + 1, string::npos);
  }
}

// パスタイプが kHome と kRelative の時にフルパス形式に展開する．
PathName
PathName::expand() const
{
  switch ( type() ) {
  case kAbsolute:
    return *this;

  case kHome:
    {
      list<string> new_list(mPathList);
      new_list.pop_front();
      return user_home(mPathList.front()) + PathName(new_list, kRelative);
    }

  case kRelative:
    return cur_work_dir() + (*this);
  }
  // ダミー
  return PathName();
}

// パスが存在しているか調べる．
bool
PathName::stat(struct stat* sbp) const
{
  struct stat dummy;
  if ( !sbp ) {
    sbp = &dummy;
  }
  return ::stat(str().c_str(), sbp) == 0;
}

// 末尾にパスをつなげる．
const PathName&
PathName::operator+=(const PathName& src)
{
  if ( is_valid() ) {
    if ( src.is_valid() && src.type() == kRelative ) {
      if ( mPathList.back() == string() ) {
	// 最後が空('/'で終わっている)の場合にはその空要素を取り除く
	mPathList.pop_back();
      }
      mPathList.insert(mPathList.end(),
		       src.mPathList.begin(), src.mPathList.end());
    }
  }
  else {
    // this が空なので src をコピーする．
    mType = src.mType;
    mPathList = src.mPathList;
  }
  return *this;
}

// 2つのパス名を連結する．
PathName
operator+(const PathName& opr1,
	  const PathName& opr2)
{
  // ほとんど C++ の定石のコード
  return PathName(opr1) += opr2;
}


//////////////////////////////////////////////////////////////////////
// サーチパス(のリスト)を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SearchPathList::SearchPathList()
{
}

// 文字列からの変換コンストラクタ
SearchPathList::SearchPathList(const string& str)
{
  to_list(str, mList);
}
  
// @brief コピーコンストラクタ
// @param[in] src 代入元のオブジェクト
SearchPathList::SearchPathList(const SearchPathList& src) :
  mList(src.mList)
{
}

// @brief 代入演算子
// @param[in] src 代入元のオブジェクト
const SearchPathList&
SearchPathList::operator=(const SearchPathList& src)
{
  // 基本的にデフォルトの実装で大丈夫なはず．
  mList = src.mList;

  return *this;
}

// デストラクタ
SearchPathList::~SearchPathList()
{
}

// 以前の内容をクリアして文字列をセットする．
void
SearchPathList::set(const string& str)
{
  mList.clear();
  to_list(str, mList);
}

// サーチパスの先頭に path を追加する．
void
SearchPathList::add_top(const string& path)
{
  list<PathName> tmp_list;
  to_list(path, tmp_list);
  mList.splice(mList.begin(), tmp_list);
}

// サーチパスの末尾に path を追加する．
void
SearchPathList::add_end(const string& path)
{
  to_list(path, mList);
}

// サーチパスを考慮して filename を探す
PathName
SearchPathList::search(const PathName& filename) const
{
  PathName tmp;
  
  switch ( filename.type() ) {
  case PathName::kAbsolute:
  case PathName::kHome:
    // 絶対パスならそれを試すだけ
    // ホームディレクトリからの相対パスの場合も唯一のパスを試す．
    tmp = filename.expand();
    if ( tmp.stat() ) {
      return tmp;
    }
    break;
  
  case PathName::kRelative:
    if ( mList.empty() ) {
      // サーチパスが空の場合はカレントディレクトリからの相対パス
      // とみなす．
      tmp = filename.expand();
      if ( tmp.stat() ) {
	return tmp;
      }
    }
    else {
      // 相対パスの場合, 頭に search_path をつけて探す．
      for (list<PathName>::const_iterator p = mList.begin();
	   p != mList.end(); ++ p) {
	const PathName& path = *p;
	PathName tmp = (path + filename).expand();
	if ( tmp.stat() ) {
	  return tmp;
	}
      }
    }
    break;
  }

  return PathName();
}

// 現在のサーチパスを取り出す．
string
SearchPathList::to_string(const string& separator) const
{
  string ans;
  string sep = "";
  for (list<PathName>::const_iterator p = mList.begin();
       p != mList.end(); ++ p) {
    ans += sep;
    ans += p->str();
    sep = separator;
  }
  return ans;
}

// 文字列を PathName のリストに変換する
void
SearchPathList::to_list(const string& str,
			list<PathName>& pathname_list)
{
  string::size_type pos1 = 0;
  string::size_type pos2;
  for ( ; (pos2 = str.find(':', pos1)) != string::npos; pos1 = pos2 + 1) {
    pathname_list.push_back(PathName(str.substr(pos1, pos2 - 1)));
  }
  pathname_list.push_back(PathName(str.substr(pos1, string::npos)));
}

    
// カレントディレクトリの取得
PathName
cur_work_dir()
{
  char buff[MAXPATHLEN + 1];
  char* tmp = getcwd(buff, sizeof(buff));
  if ( tmp == NULL ) {
    // なんか知らないけどエラー
    return PathName();
  }

  return PathName(buff);
}

// ユーザのホームディレクトリの取得
PathName
user_home(const string& login)
{
  string hdir;
  if ( login == string() ) {
    char* henv = getenv("HOME");
    if ( henv ) {
      hdir = henv;
    }
  }
  else {
    struct passwd* ppw = getpwnam(login.c_str());
    if ( ppw ) {
      hdir = ppw->pw_dir;
    }
    endpwent();
  }
  return PathName(hdir);
}

END_NAMESPACE_YM
