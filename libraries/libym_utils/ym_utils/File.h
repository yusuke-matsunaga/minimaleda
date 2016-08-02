#ifndef YM_UTILS_FILE_H
#define YM_UTILS_FILE_H

/// @file ym_utils/File.h
/// @brief ファイル操作関係のクラスのヘッダファイル
/// @author Yusuke Matsunaga
///
/// $Id: File.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class PathName File.h <ym_utils/File.h>
/// @ingroup YmUtils
/// @brief パス名を表すクラス
///
/// 直感的に言えばパス名を表す文字列のみをもっているクラスだが，
/// 相対パスやチルダ("~")展開のようなパス名固有の処理を内蔵したクラス
//////////////////////////////////////////////////////////////////////
class PathName
{
public:

  /// @brief パスの型
  enum tType {
    /// @brief 絶対パス
    kAbsolute,
    /// @brief ホームからの相対パス．最初の文字列はユーザ名
    kHome,
    /// @brief カレントディレクトリからの相対パス
    kRelative
  };

public:
  
  /// @brief 空のコンストラクタ
  /// @note 結果は相対パスのカレントディレクトリを表すオブジェクトとなる
  PathName();
  
  /// @brief 文字列からの変換コンストラクタ
  /// @param[in] path_str パスを表す文字列
  PathName(const string& path_str);
  
  /// @brief 文字列からの変換コンストラクタ
  /// @param[in] path_str パスを表す文字列
  PathName(const char* path_str);
  
  /// @brief デストラクタ
  ~PathName();


public:

  /// @brief パス名の妥当性チェック
  /// @return 意味のある値を持っているとき true を返す．
  /// @note ここでは形式のみチェックする．
  /// そのパスが存在するか等は関係ない．
  bool
  is_valid() const;
  
  /// @brief パスの型を返す．
  /// @return パスの型
  tType
  type() const;
  
  /// @brief パス名の文字列表現を返す．
  /// @return パス名の文字列表現
  string
  str() const;
  
  /// @brief パス名のヘッダを返す．
  /// @return パス名のヘッダ (最後の名前を含まないもの)
  /// @note 元々が階層を含まない場合には空になる．
  PathName
  head() const;
  
  /// @brief パス名のテイルを返す．
  /// @return パス名のテイル (ヘッダを含まないもの)
  /// @note パス名が '/' で終わっていたらテイルは空となる．
  string
  tail() const;
  
  /// @brief パス名の本体(tail から "."と拡張子を除いた物)を返す
  /// @return パス名の本体(tail から "."と拡張子を除いた物)を返す．
  string
  body() const;
  
  /// @brief パス名の拡張子を返す．
  /// @return パス名の拡張子
  /// @note "." を含まない場合には空になる．
  string
  ext() const;
  
  /// @brief パス名の展開
  /// @return 展開したパス
  /// @note パスタイプが kHome と kRelative の時にフルパス形式に展開する．
  /// 結果は必ず kAbsolute になる．
  PathName
  expand() const;
  
  /// @brief パスが存在しているか調べる．
  /// @param[out] sbp stat システムコールの結果を格納する構造体
  /// NULL ならどこにも結果を格納しない
  /// @return 存在していたら true を返す．
  bool
  stat(struct stat* sbp = NULL) const;
  
  /// @brief 末尾にパスをつなげる．
  /// @param[in] src 追加するパス
  /// @return 結合したパス
  /// @note
  /// - src のタイプが kRelative でないときは無視する．
  /// - src が空の場合も無視する．
  /// - 自分自身が空の場合には src を代入する．
  const PathName&
  operator+=(const PathName& src);
  

private:
  
  /// @brief 文字列のリストからの変換コンストラクタ
  /// @param[in] path_list パスリスト
  /// @param[in] type パスタイプ
  PathName(const list<string>& path_list,
	   tType type);

  /// @brief 拡張子の直前のドットの位置を返す．
  /// @param[in] path 対象のパス文字列
  /// @return 拡張子の直前のドットの位置
  /// ただし，"." や ".." の場合や先頭がドットで始まっていて，
  /// それが唯一のドットの場合には拡張子はなしと見なす．
  static
  string::size_type
  dot_pos(const string& path);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // タイプ
  tType mType;

  // パス名を表す本体
  list<string> mPathList;

};

/// @name PathName に関連したファイル名操作関数
/// @{

/// @relates PathName
/// @ingroup YmUtils
/// @brief 2つのパス名を連結する．
/// @param[in] opr1, opr2 パス名
/// @return 連結したパス名
PathName
operator+(const PathName& opr1,
	  const PathName& opr2);

/// @relates PathName
/// @ingroup YmUtils
/// @brief カレントディレクトリを表す絶対パス名を返す．
/// @return カレントディレクトリを表す絶対パス名を返す．
PathName
cur_work_dir();

/// @relates PathName
/// @ingroup YmUtils
/// @brief ユーザのホームディレクトリの取得
/// @param[in] login ユーザーアカウント名
/// @return login のホームディレクトリ
PathName
user_home(const string& login);

/// @}


//////////////////////////////////////////////////////////////////////
/// @class SearchPathList File.h <ym_utils/File.h>
/// @ingroup YmUtils
/// @brief サーチパス(のリスト)を表すクラス
/// @sa PathName
//////////////////////////////////////////////////////////////////////
class SearchPathList
{
public:

  /// @brief コンストラクタ
  /// @note 空のリストを作る．
  SearchPathList();

  /// @brief 文字列からの変換コンストラクタ
  /// @param[in] str サーチパスを表す文字列
  /// @note サーチパス文字列は ':' で区切られた文字列で '.'
  /// はカレントディレクトリを表す．
  /// また，末尾が '/' で終わっている場合にはそのサブディレクトリ
  /// も探索する．
  SearchPathList(const string& str);
  
  /// @brief コピーコンストラクタ
  /// @param[in] src 代入元のオブジェクト
  SearchPathList(const SearchPathList& src);

  /// @brief 代入演算子
  /// @param[in] src 代入元のオブジェクト
  const SearchPathList&
  operator=(const SearchPathList& src);
  
  /// @brief デストラクタ
  ~SearchPathList();
  

public:
  
  /// @brief 以前の内容をクリアして文字列をセットする．
  /// @param[in] str セットするサーチパスを表す文字列
  /// @note サーチパス文字列は ':' で区切られた文字列で '.'
  /// はカレントディレクトリを表す．
  /// また，末尾が '/' で終わっている場合にはそのサブディレクトリ
  /// も探索する．
  void
  set(const string& str);

  /// @brief サーチパスの先頭に path を追加する．
  /// @param[in] path 追加するパス
  /// @note path は ':' を含んでいても良い
  void
  add_top(const string& path);

  /// @brief サーチパスの末尾に path を追加する．
  /// @param[in] path 追加するパス
  /// @note path は ':' を含んでいても良い
  void
  add_end(const string& path);

  /// @brief サーチパスを考慮して filename を探す
  /// @param[in] filename ファイル名
  /// @retval 最初に見つかったファイルの full-path
  /// @retval 空のパス名 見つからない場合
  /// @note サーチパスが空ならカレントディレクトリで filename を探す．
  PathName
  search(const PathName& filename) const;

  /// @brief 現在のサーチパスを取り出す．
  /// @param[in] separator 区切り文字
  /// @return サーチパスを文字列に変換したもの
  string
  to_string(const string& separator = ":") const;


private:

  // 文字列を PathName のリストに変換する
  static
  void
  to_list(const string& str,
	  list<PathName>& pathname_list);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実際のサーチパスのリスト
  list<PathName> mList;

};

END_NAMESPACE_YM

#endif // YM_UTILS_FILE_H
