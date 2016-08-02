#ifndef LIBYM_VERILOG_SCANNER_INPUTMGR_H
#define LIBYM_VERILOG_SCANNER_INPUTMGR_H

/// @file libym_verilog/scanner/InputMgr.h
/// @brief InputMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: InputMgr.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/verilog.h"

#include "ym_utils/File.h"
#include "ym_utils/FileRegion.h"
#include "ym_utils/FileDescMgr.h"
#include "ym_utils/MsgHandler.h"
#include "ym_utils/StrBuff.h"

#include "InputFile.h"


BEGIN_NAMESPACE_YM_VERILOG

class RawLex;

//////////////////////////////////////////////////////////////////////
/// @class InputMgr InputMgr.h "InputMgr.h"
/// @ingroup VlParser
/// @brief 入力ファイルを管理するクラス
/// @sa InputFile FileDesc
//////////////////////////////////////////////////////////////////////
class InputMgr
{
public:
  
  /// @brief コンストラクタ
  /// @param[in] lex 親の Lex
  /// @param[in] fd_mgr ファイル記述子を管理するクラス
  InputMgr(RawLex* lex,
	   FileDescMgr& fd_mgr);
  
  /// @brief デストラクタ
  ~InputMgr();


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 初期化およびファイル関係
  /// @{

  /// @brief 初期状態に戻す．
  /// @note 読み込みの途中でこれを呼ぶと大変なことになる．
  /// 普通は正常に読み終わったあとか, エラーの後で呼ぶ．
  void
  clear();

  /// @brief サーチパスリストを設定する．
  /// @param[in] searchpath セットするサーチパス
  void
  set_searchpath(const SearchPathList& searchpath = SearchPathList());

  /// @brief 設定されているサーチパスを考慮して filename を探す．
  /// @param[in] filename ファイル名
  /// @return 実際の絶対パス
  /// @note PathName::is_valid() で結果がわかる．
  PathName
  search_file(const string& filename);

  /// @brief ファイルをオープンする．
  /// @param[in] filename ファイル名
  /// @param[in] parent_file インクルード元のファイル情報
  /// @retval true オープンに成功した．
  /// @retval false ファイルが開けなかった
  bool
  open_file(const string& filename,
	    const FileLoc& parent_file = FileLoc());

  /// @brief ファイルのオープン済チェック
  /// @param[in] filename チェックするファイル名
  /// @retval true name という名のファイルがオープンされている．
  /// @retval false name というなのファイルはオープンされていない．
  bool
  check_file(const char* filename) const;

  /// @brief 現在のファイル位置を強制的に書き換える．
  /// @param[in] filename 新しいファイル名
  /// @param[in] line     新しい行番号
  /// @param[in] level
  ///           - 0 インクルード関係のレベル変化無し
  ///           - 1 新しいファイルはインクルードされたファイル
  ///           - 2 新しいファイルはインクルードもとのファイル
  void
  set_file_loc(const char* filename,
	       ymuint32 line,
	       ymuint32 level);
  
  /// @brief 現在のファイルを返す．
  InputFile*
  cur_file();
  
  /// @brief 現在の InputFile が EOF を返したときの処理
  /// @return 処理を続けられる時 true を返す．
  bool
  wrap_up();
  
  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 新しい FileDesc を作る
  /// @param[in] filename ファイル名
  /// @return 生成された FileDesc
  const FileDesc*
  new_file_desc(const char* filename);

  /// @brief 新しい FileDesc を作る
  /// @param[in] filename ファイル名
  /// @param[in] parent_file_loc インクルード元の親ファイルの情報
  /// @return 生成された FileDesc
  const FileDesc*
  new_file_desc(const char* filename,
		const FileLoc& parent_file_loc);
  
  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 親の Lex
  RawLex* mLex;
  
  // ファイル記述子を管理するクラス
  FileDescMgr& mFdMgr;
  
  // サーチパス
  SearchPathList mSearchPathList;

  // 現在のファイル情報
  InputFile* mCurFile;
  
  // ファイル情報のスタック
  vector<InputFile*> mFileStack;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////
  
// @brief 現在のファイルを返す．
inline
InputFile*
InputMgr::cur_file()
{
  return mCurFile;
}

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_SCANNER_INPUTMGR_H
