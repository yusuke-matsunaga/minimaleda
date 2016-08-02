#ifndef LIBYM_VERILOG_SCANNER_RAWLEX_H
#define LIBYM_VERILOG_SCANNER_RAWLEX_H

/// @file libym_verilog/scanner/RawLex.h
/// @brief RawLex のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: RawLex.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

#include "ym_verilog/verilog.h"

#include "ym_utils/StrBuff.h"
#include "ym_utils/File.h"
#include "ym_utils/FileDescMgr.h"
#include "ym_utils/FileRegion.h"
#include "ym_utils/Binder.h"
#include "ym_utils/MsgHandler.h"

#include "LexPluginDict.h"


BEGIN_NAMESPACE_YM_VERILOG

class VlLineWatcher;
class LexPlugin;
class LexState;
class LexCondState;
class LexPluginDict;
class RsrvWordDic;
class InputMgr;
class MacroSource;
class TokenInfo;
class TokenList;

//////////////////////////////////////////////////////////////////////
/// @class RawLex RawLex.h "RawLex.h"
/// @ingroup VlParser
/// @brief Verilog 用の低レベルな字句解析ルーティン
///
/// 基本的なトークンの切り出しはここで行うが，compiler-directive
/// の処理は Lex で行う．
/// @sa InputFile Lex VlLineWatcher
//////////////////////////////////////////////////////////////////////
class RawLex
{
  friend class LexPlugin;

public:
  
  /// @brief 解析用のコンテキスト
  enum tContext {
    /// @brief 通常
    kNormal,
    /// @brief UDP 定義のテーブル記述内
    kUdp,
    /// @brief 2進定数
    kBin,
    /// @brief 8進定数
    kOct,
    /// @brief 10進定数
    kDec,
    /// @brief 16進定数
    kHex
  };
  
  
public:
  
  /// @brief コンストラクタ
  /// @param[in] msg_mgr メッセージマネージャ
  /// @param[in] fd_mgr ファイル記述子を管理するクラス
  RawLex(MsgMgr& msg_mgr,
	 FileDescMgr& fd_mgr);

  /// @brief デストラクタ
  ~RawLex();


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
  
  /// @brief ファイルをオープンする．
  /// @param[in] filename ファイル名
  /// @retval true オープンに成功した．
  /// @retval false ファイルが開けなかった
  bool
  open_file(const string& filename);

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name トークン読み出しおよび関連した情報の取得
  /// @{

  /// @brief トークンの読み出し
  /// @return 読み込んだトークンの id を返す．
  int
  get_token();
  
  /// @brief 生のトークンの読み出し
  /// @return 読み出されたトークンの id を返す．
  int
  get_raw_token();

  /// @brief 最後に読んだトークンの位置を返す．
  /// @return 最後に読んだトークンのファイル上の位置
  FileRegion
  cur_token_loc() const;

  /// @brief 最後に読んだ文字列を返す．
  /// @return 最後に読んだ文字列
  const char*
  cur_string() const;
  
  /// @brief 最後に読んだ文字列を unsigned integer に変換する．
  /// @return 最後に読んだ文字列を unsigned integer に変換したもの
  ymuint32
  cur_uint() const;

  /// @brief 最後に読んだ文字列を real number に変換する．
  /// @return 最後に読んだ文字列を real number に変換したもの
  double
  cur_rnumber() const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 行番号ウォッチャーの設定
  /// @{

  /// @brief 行番号ウオッチャーを設定する．
  /// @param[in] watcher 登録対象のウオッチャー
  /// @note watcher がすでに登録されていればなにもしない．
  void
  reg_watcher(VlLineWatcher* watcher);

  /// @brief 行番号ウオッチャーの登録を解除する．
  /// @param[in] watcher 登録削除対象のウオッチャー
  /// @note watcher が登録されていなければなにもしない．
  void
  unreg_watcher(VlLineWatcher* watcher);

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 機能拡張用のインターフェイス
  /// @{

  /// @brief プラグインの追加
  /// @param[in] name プラグインを起動するコンパイラディレクティブ名
  /// @param[in] plugin 登録するプラグイン
  void
  add_plugin(const char* name,
	     LexPlugin* plugin);

  /// @brief プラグインの削除
  /// @param[in] name 削除するプラグイン名
  /// @retval true 削除が成功した．
  /// @retval false name という名のプラグインが存在しなかった．
  bool
  erase_plugin(const char* name);

  /// @brief プラグインを登録できるか調べる．
  /// @param[in] name プラグイン名
  /// @return true 登録可能
  /// @return false 同名の定義済みプラグインがあり，登録不可能
  bool
  check_pluginname(const char* name);

  /// @brief 内部状態の追加
  /// @param[in] state 登録する内部状態
  void
  add_state(LexState* state);

  /// @}
  //////////////////////////////////////////////////////////////////////


public:  
  //////////////////////////////////////////////////////////////////////
  /// @name デバッグモードの制御
  /// @{

  /// @brief デバッグモードのセット
  /// @param[in] flag true ならすべてのトークンの情報を標準エラーに出力する
  void
  set_debug(bool flag = true);

  /// @brief デバッグモードの取得
  bool
  debug();

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name その他
  /// @{
  
  /// @brief メッセージマネージャの取得
  MsgMgr&
  msg_mgr();
  
  /// @brief `resetall の処理
  void
  resetall(const FileRegion& file_region);

  /// @brief マクロ定義の検査
  /// @param[in] name 名前
  /// @return name という名のマクロが定義されていたら true を返す．
  bool
  is_macro_defined(const char* name) const;

  /// @brief マクロの多重展開のチェック
  /// @param[in] name 名前
  /// @return マクロ展開が自己ループしていたら true を返す．
  bool
  check_macro(const char* name) const;
  
  /// @brief マクロの追加
  void
  push_macro(const char* name,
	     const TokenInfo* top,
	     TokenList* param_array);

  /// @brief `ifdef/`ifndef 文の現在の条件の取得
  /// @retval true 条件が成り立っている
  /// @retval false 条件が成り立っていない
  bool
  cond() const;
  
  /// @brief condition の書き換え
  /// @param[in] flag 設定する値
  void
  set_cond(bool flag);
  
  /// @brief condition の反転
  void
  flip_cond();
  
  /// @brief 改行文字を読み込んだときに呼ばれる関数
  void
  check_line();
  
  /// @brief コンテキストを返す．
  tContext
  context();
  
  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // メッセージマネージャ
  MsgMgr& mMsgMgr;
  
  // 入力ファイルを管理するオブジェクト
  InputMgr* mInputMgr;
  
  // 予約語辞書の参照
  const RsrvWordDic& mDic;

  // 条件コンパイル用の状態
  LexCondState* mCondState;

  // 現在のトークンの位置
  FileRegion mCurPos;
  
  // 現在のトークンの文字列
  const char* mCurString;

  // 現在のトークンが整数型のとき対応する整数値
  ymuint32 mCurUint;

  // module 定義中のとき true となるフラグ
  bool mModuleMode;

  // マクロのスタック
  vector<MacroSource> mMacroStack;
  
  // 行番号ウオッチャーを管理するクラス
  T1BindMgr<int> mWatcherMgr;

  // プラグイン用の辞書
  LexPluginDict mPluginDict;

  // 内部状態用のリスト
  list<LexState*> mStates;
  
  // コンテキスト
  tContext mContext;
  
  // read_token の結果の文字列を格納するバッファ
  StrBuff mStringBuff;
  
  // デバッグ時に true にするフラグ
  bool mDebug;
  
};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 最後に読んだトークンの位置を返す．
// @return 最後に読んだトークンのファイル上の位置
inline
FileRegion
RawLex::cur_token_loc() const
{
  return mCurPos;
}

// @brief 最後に読んだ文字列を返す．
// @return 最後に読んだ文字列
inline
const char*
RawLex::cur_string() const
{
  return mCurString;
}

// @brief 最後に読んだ文字列を unsigned integer に変換する．
// @return 最後に読んだ文字列を unsigned integer に変換したもの
inline
ymuint32
RawLex::cur_uint() const
{
  return mCurUint;
}

// @brief 最後に読んだ文字列を real number に変換する．
// @return 最後に読んだ文字列を real number に変換したもの
inline
double
RawLex::cur_rnumber() const
{
  return strtod(mCurString, static_cast<char**>(NULL));
}
  
// @brief メッセージマネージャの取得
inline
MsgMgr&
RawLex::msg_mgr()
{
  return mMsgMgr;
}
  
// @brief コンテキストを返す．
inline
RawLex::tContext
RawLex::context()
{
  return mContext;
}

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_SCANNER_RAWLEX_H
