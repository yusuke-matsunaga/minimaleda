#ifndef LIBYM_VERILOG_PARSER_SCANNER_INPUTFILE_H
#define LIBYM_VERILOG_PARSER_SCANNER_INPUTFILE_H

/// @file libym_verilog/parser/scanner/InputFile.h
/// @brief InputFile のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: InputFile.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/verilog.h"

#include "ym_utils/File.h"
#include "ym_utils/FileRegion.h"
#include "ym_utils/FileDesc.h"
#include "ym_utils/StrBuff.h"

#include "RawLex.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class InputFile InputFile.h "InputFile.h"
/// @ingroup VlParser
/// @brief 入力ファイルを表すクラス
///
/// このクラスはファイルを一文字単位で読み出す．\n
/// ただし, 改行コードの処理系依存部分を吸収して常に '\\n' だけを
/// 返すようにしている．
/// @sa FileDesc RawLex
//////////////////////////////////////////////////////////////////////
class InputFile
{
  friend class InputMgr;
  
private:
  
  /// @brief コンストラクタ
  /// @param[in] lex 親の Lex
  /// @param[in] fd UNIX のファイル記述子
  /// @param[in] file_desc ファイル記述子
  InputFile(RawLex* lex,
	    int fd,
	    const FileDesc* file_desc);

  /// @brief デストラクタ
  ~InputFile();


public:

  /// @brief ファイルをクローズする．
  void
  close();

  /// @brief トークンの読み出しを行う．
  /// @param[out] buff 結果の文字列を格納するバッファ
  /// @param[out] token_loc トークンの位置情報
  int
  read_token(StrBuff& buff,
	     FileRegion& token_loc);

  /// @brief read_token() の下請け関数
  /// @param[out] buff 結果の文字列を格納するバッファ
  int
  _read_token(StrBuff& buff);
  
  /// @brief 2進数モードの読み込みを行う．
  /// @param[in] c 最初の文字
  /// @param[out] buff 結果を格納するバッファ
  /// @return トークンを返す．
  int
  read_bin_str(int c,
	       StrBuff& buff);
  
  /// @brief 8進数モードの読み込みを行う．
  /// @param[in] c 最初の文字
  /// @param[out] buff 結果を格納するバッファ
  /// @return トークンを返す．
  int
  read_oct_str(int c,
	       StrBuff& buff);
  
  /// @brief 10進数モードの読み込みを行う．
  /// @param[in] c 最初の文字
  /// @param[out] buff 結果を格納するバッファ
  /// @return トークンを返す．
  int
  read_dec_str(int c,
	       StrBuff& buff);
  
  /// @brief 16進数モードの読み込みを行う．
  /// @param[in] c 最初の文字
  /// @param[out] buff 結果を格納するバッファ
  /// @return トークンを返す．
  int
  read_hex_str(int c,
	       StrBuff& buff);

  /// @brief 識別子に用いられる文字([a-zA-Z0-9_$])が続く限り読みつづける．
  /// @param[out] buff 結果を格納する文字列バッファ
  void
  get_str(StrBuff& buff);
  
  /// @brief 二重引用符用の読み込み
  /// @param[out] buff 結果を格納する文字列バッファ
  /// @return トークン番号を返す．
  /// @note 可能性のあるトークンは
  ///  - STRING
  ///  - ERROR
  int
  read_dq_str(StrBuff& buff);
  
  /// @brief escaped identifier 用の読み込み
  /// @param[out] buff 結果を格納する文字列バッファ
  /// @return トークン番号を返す．
  /// @note 可能性のあるトークンは
  ///  - SPACE
  ///  - IDENTIFIER
  ///  - ERROR
  int
  read_esc_str(StrBuff& buff);
  
  /// @brief 数字を読み込む．
  /// @param[out] buff 結果を格納する文字列バッファ
  /// @return トークン番号を返す．
  /// @note 可能性のあるトークンは
  ///  - UNUM_INT
  ///  - RNUMBER
  ///  - ERROR
  int
  read_num(StrBuff& buff);
  
  /// @brief 空白文字を読み飛ばす
  void
  read_space();
  
  /// @brief '/' を読み込んだ後の処理
  /// @param[out] buff コメントを格納する文字列バッファ
  /// @return トークン番号を返す．
  /// @note 可能性のあるトークンは
  ///  - COMMENT1
  ///  - COMMENT2
  ///  - '/'
  ///  - ERROR
  int
  read_comment(StrBuff& buff);

  /// @brief 次の一文字を試し読みする．
  /// @return 読み込んだ文字を返す．
  int
  peek();

  /// @brief 直前の peek() を確定させる．
  void
  accept();

  /// @brief 直前の peek() を確定させる．
  /// @note こちらは改行文字でないことが明らかな時に用いるバージョン
  void
  _accept();

  /// @brief 次の一文字を読み出す．
  /// @return 読み込んだ文字を返す．
  int
  get();
  
  /// @brief ファイル記述子を返す．
  const FileDesc*
  file_desc() const;

  /// @brief ファイル名を返す．
  const char*
  filename() const;

  /// @brief 現在の行番号を返す．
  ymuint32
  cur_line() const;

  /// @brief 現在のカラム番号を返す．
  ymuint32
  cur_column() const;

  /// @brief 直前の行番号を返す．
  ymuint32
  last_line() const;

  /// @brief 直前のカラム番号を返す．
  ymuint32
  last_column() const;

  /// @brief 現在のファイル位置の取得
  /// @return 現在のファイル位置
  FileRegion
  cur_file_region() const;
  
  
private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 改行コードを読み込んだ時の処理
  void
  nl();
  
  /// @brief バッファに充填する．
  /// @brief 読み込まれた文字数を返す．
  int
  fill_buff();
  
  /// @brief メッセージを出力する．
  /// @param[in] src_file この関数を読んでいるソースファイル名
  /// @param[in] src_line この関数を読んでいるソースの行番号
  /// @param[in] file_loc ファイル位置
  /// @param[in] type メッセージの種類
  /// @param[in] label メッセージラベル
  /// @param[in] body メッセージ本文
  void
  put_msg(const char* src_file,
	  int src_line,
	  const FileRegion& file_loc,
	  tMsgType type,
	  const char* label,
	  const char* msg);
  
  /// @brief メッセージを出力する．
  /// @param[in] src_file この関数を読んでいるソースファイル名
  /// @param[in] src_line この関数を読んでいるソースの行番号
  /// @param[in] file_loc ファイル位置
  /// @param[in] type メッセージの種類
  /// @param[in] label メッセージラベル
  /// @param[in] body メッセージ本文
  void
  put_msg(const char* src_file,
	  int src_line,
	  const FileRegion& file_loc,
	  tMsgType type,
	  const char* label,
	  const string& msg);
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 親の Lex
  RawLex* mLex;
  
  // UNIX のファイル記述子
  int mFd;
  
  // ファイル記述子
  const FileDesc* mFileDesc;

  // バッファ中の読み出し位置
  ymuint32 mReadPos;

  // バッファの末尾
  ymuint32 mEndPos;

  // ファイルバッファ
  char mBuff[4096];
  
  // 現在の行番号
  ymuint32 mCurLine;

  // 現在のコラム番号
  ymuint32 mCurColumn;
  
  // 直前の行番号
  ymuint32 mLastLine;

  // 直前のコラム番号
  ymuint32 mLastColumn;
  
  // 最後の文字が '\n' の時 true となるフラグ
  bool mNL;

};

//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief トークンの読み出しを行う．
// @param[out] buff 結果の文字列を格納するバッファ
// @param[out] token_loc トークンの位置情報
inline
int
InputFile::read_token(StrBuff& buff,
		      FileRegion& token_loc)
{
  ymuint32 first_line = cur_line();
  ymuint32 first_column = cur_column();

  int id = _read_token(buff);
  
  token_loc = FileRegion(file_desc(),
			 first_line, first_column,
			 last_line(),last_column());

  return id;
}
  
// @brief ファイル記述子を返す．
inline
const FileDesc*
InputFile::file_desc() const
{
  return mFileDesc;
}

// @brief ファイル名を返す．
inline
const char*
InputFile::filename() const
{
  return mFileDesc->name();
}

// @brief 次の一文字を読み出す．
// @return 読み込んだ文字を返す．
inline
int
InputFile::get()
{
  int c = peek();
  accept();
  return c;
}

// @brief 現在の行番号を返す．
inline
ymuint32
InputFile::cur_line() const
{
  return mCurLine;
}

// @brief 現在のカラム番号を返す．
inline
ymuint32
InputFile::cur_column() const
{
  return mCurColumn;
}

// @brief 直前の行番号を返す．
inline
ymuint32
InputFile::last_line() const
{
  return mLastLine;
}

// @brief 直前のカラム番号を返す．
inline
ymuint32
InputFile::last_column() const
{
  return mLastColumn;
}

// @brief 現在のファイル位置の取得
// @return 現在のファイル位置
inline
FileRegion
InputFile::cur_file_region() const
{
  FileLoc tmp(file_desc(), cur_line(), cur_column());
  return FileRegion(tmp, tmp);
}

// @brief 直前の peek() を確定させる．
inline
void
InputFile::_accept()
{
  mLastLine = mCurLine;
  mLastColumn = mCurColumn;
  ++ mCurColumn;
  ++ mReadPos;
}
  
// @brief 改行コードを読み込んだ時の処理
inline
void
InputFile::nl()
{
  mNL = true;
  ++ mCurLine;
  mCurColumn = 1;
  mLex->check_line();
}

// @brief バッファに充填する．
inline
int
InputFile::fill_buff()
{
  mReadPos = 0;
  if ( mFd >= 0 ) {
    ssize_t n = read(mFd, mBuff, 4096);
    if ( n < 0 ) {
      abort();
    }
    mEndPos = n;
  }
  else {
    mEndPos = 0;
  }
  return mEndPos;
}
  
// @brief メッセージを出力する．
// @param[in] src_file この関数を読んでいるソースファイル名
// @param[in] src_line この関数を読んでいるソースの行番号
// @param[in] file_loc ファイル位置
// @param[in] type メッセージの種類
// @param[in] label メッセージラベル
// @param[in] body メッセージ本文
inline
void
InputFile::put_msg(const char* src_file,
		   int src_line,
		   const FileRegion& file_loc,
		   tMsgType type,
		   const char* label,
		   const char* msg)
{
  mLex->msg_mgr().put_msg(src_file, src_line,
			  file_loc, type,
			  label, msg);
}
  
// @brief メッセージを出力する．
// @param[in] src_file この関数を読んでいるソースファイル名
// @param[in] src_line この関数を読んでいるソースの行番号
// @param[in] file_loc ファイル位置
// @param[in] type メッセージの種類
// @param[in] label メッセージラベル
// @param[in] body メッセージ本文
inline
void
InputFile::put_msg(const char* src_file,
		   int src_line,
		   const FileRegion& file_loc,
		   tMsgType type,
		   const char* label,
		   const string& msg)
{
  mLex->msg_mgr().put_msg(src_file, src_line,
			  file_loc, type,
			  label, msg);
}

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_PARSER_SCANNER_INPUTFILE_H
