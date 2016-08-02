
/// @file libym_verilog/scanner/RawLex.cc
/// @brief RawLex の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: RawLex.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "RawLex.h"

#include "ym_verilog/VlLineWatcher.h"

#include "LexPlugin.h"
#include "LexState.h"
#include "LexCondState.h"
#include "LexCondPlugin.h"
#include "LexMacroPlugin.h"
#include "StdLexPlugin.h"

#include "InputMgr.h"
#include "RsrvWordDic.h"
#include "MacroSource.h"
#include "TokenInfo.h"
#include "LexPluginDict.h"

#include "print_token.h"
#include "parser.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// RawLex のパブリックなメンバ関数
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] msg_mgr メッセージマネージャ
// @param[in] fd_mgr ファイル記述子を管理するクラス
RawLex::RawLex(MsgMgr& msg_mgr,
	       FileDescMgr& fd_mgr) :
  mMsgMgr(msg_mgr),
  mInputMgr(new InputMgr(this, fd_mgr)),
  mDic(RsrvWordDic::the_dic()),
  mDebug(false)
{
  mCondState = new LexCondState(*this);
  new LpIfdef(*this, "ifdef", mCondState);
  new LpIfdef(*this, "ifndef", mCondState);
  new LpElse(*this, "else", mCondState);
  new LpElsif(*this, "elsif", mCondState);
  new LpEndif(*this, "endif", mCondState);

  new LpInclude(*this, "include", mInputMgr);
  new LpLine(*this, "line", mInputMgr);

  new LpDefine(*this, "define");
  new LpUndef(*this, "undef");

  new LpResetAll(*this, "resetall");

  clear();
}

// @brief デストラクタ
RawLex::~RawLex()
{
  clear();

  // LexPlugin は LexPluginDict のデストラクタで削除される．
  
  for (list<LexState*>::iterator p = mStates.begin();
       p != mStates.end(); ++ p) {
    delete *p;
  }
}

// @brief 初期状態に戻す．
// 読み込みの途中でこれを呼ぶと大変なことになる．
// 普通は正常に読み終わったあとか, エラーの後で呼ぶ．
void
RawLex::clear()
{
  mModuleMode = false;

  mMacroStack.clear();

  resetall(FileRegion());

  mInputMgr->clear();

  mContext = kNormal;
}

// @brief サーチパスリストを設定する．
// @param[in] searchpath セットするサーチパス
void
RawLex::set_searchpath(const SearchPathList& searchpath)
{
  mInputMgr->set_searchpath(searchpath);
}

// @brief ファイルをオープンする．
// @param[in] filename ファイル名
// @retval true オープンに成功した．
// @retval false ファイルが開けなかった
bool
RawLex::open_file(const string& filename)
{
  return mInputMgr->open_file(filename);
}


//////////////////////////////////////////////////////////////////////
// トークンの読み出し関係
//////////////////////////////////////////////////////////////////////

// @brief 一語を読み込む
// @return 読み込んだトークンの id を返す．
int
RawLex::get_token()
{
  // 本当の処理は get_raw_token() で行っている．
  // ここでの主要な処理は compiler directive の処理
  for ( ; ; ) {
    int id = get_raw_token();
    if ( mCondState->cond() ) {
      switch ( id ) {
      case EOF:
	if ( mDebug ) {
	  mMsgMgr.put_msg(__FILE__, __LINE__,
			  cur_token_loc(),
			  kMsgDebug,
			  "LEX",
			  "Reached to the EOF.");
	}
	return EOF;

      case SPACE:
      case NL:
      case COMMENT1:
      case COMMENT2:
	continue;
      
      case CD_SYMBOL:
	{
	  // 先頭の '`' をスキップする．
	  const char* macroname = cur_string() + 1;
	  LexPlugin* plugin = mPluginDict.find_plugin(macroname);
	  if ( plugin == NULL ) {
	    ostringstream buf;
	    buf << "macro `" << macroname << " is not defined.";
	    mMsgMgr.put_msg(__FILE__, __LINE__,
			    cur_token_loc(),
			    kMsgError,
			    "LEX",
			    buf.str());
	    goto error;
	  }
	  if ( !plugin->parse() ) {
	    goto error;
	  }
	}
	continue;

      case MODULE:
      case MACROMODULE:
	// module 内に入ったことを記録しておく
	mModuleMode = true;
	break;

      case ENDMODULE:
	// module 定義から抜けたことを記録しておく．
	mModuleMode = false;
	break;

      case UNUM_INT:
	{
	  // 小さい整数か大きい整数か判断する．
	  const char* tmp = cur_string();
	  ymuint32 end = strlen(tmp);
	  const ymuint32 sft = sizeof(ymuint32) * 8 - 4;
	  mCurUint = 0;
	  bool overflow = false;
	  for (ymuint32 pos = 0; pos < end; ++ pos) {
	    char c = tmp[pos];
	    ymuint32 v = c - '0';
	    ymuint32 u = mCurUint >> sft;
	    ymuint32 l = mCurUint - (u << sft);
	    ymuint32 u10 = u * 10;
	    ymuint32 l10 = l * 10;
	    mCurUint = v + l10 + ((u10 % 16) << sft);
	    if ( u10 / 16 > 0 ) {
	      // 桁あふれ
	      overflow = true;
	      break;
	    }
	  }
	  if ( overflow ) {
	    // 大きい整数型
	    id = UNUM_BIG;
	  }
	  // そうでなければ mCurUint に正しい値が入っている．
	}
	break;

      default:
	break;
      }
      
      if ( mDebug ) {
	ostringstream buf;
	print_token(buf, id, cur_string());
	mMsgMgr.put_msg(__FILE__, __LINE__,
			cur_token_loc(),
			kMsgDebug,
			"LEX",
			buf.str());
      }
      return id;
    }
    else {
      // 条件が成り立っていない場合は `ifdef 関係のディレクティブのみ
      // パーズすればよい
      switch ( id ) {
      case EOF:
	mMsgMgr.put_msg(__FILE__, __LINE__,
			cur_token_loc(),
			kMsgError,
			"LEX",
			"Unexpected EOF.");
	return ERROR;
	
      case CD_SYMBOL:
	{
	  // 先頭の '`' をスキップする．
	  const char* macroname = cur_string() + 1;
	  LexPlugin* plugin = mPluginDict.find_plugin(macroname);
	  if ( plugin && plugin->is_cond_plugin() ) {
	    if ( !plugin->parse() ) {
	      goto error;
	    }
	  }
	}
	continue;
	
      default:
	continue;
      }
    }
  }

 error:
  if ( mDebug ) {
    mMsgMgr.put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    kMsgDebug,
		    "LEX",
		    "Error exit.");
  }
  return ERROR;
}

// @brief トークンの読み出し
// @return 読み出されたトークン
int
RawLex::get_raw_token()
{
  // マクロ置き換え中の場合にはマクロソースから読み込みを行う．
  while ( !mMacroStack.empty() ) {
    MacroSource& macro = mMacroStack.back();
    const TokenInfo* token = macro.get();
    if ( token ) {
      mCurPos = token->loc();
      mCurString = token->str();
      if ( mDebug ) {
	ostringstream buf;
	buf << "get_raw_token(from macro) ==> ";
	print_token(buf, token->id(), mCurString);
	mMsgMgr.put_msg(__FILE__, __LINE__,
			cur_token_loc(),
			kMsgDebug,
			"LEX",
			buf.str());
      }
      return token->id();
    }
    mMacroStack.pop_back();
  }

  // 通常の読み込み
 LOOP:
  InputFile* input_file = mInputMgr->cur_file();
  int id = input_file->read_token(mStringBuff, mCurPos);
  mCurString = mStringBuff.c_str();
  
  switch ( id ) {
  case IDENTIFIER:
    // buff が予約語かどうか判定する．
    id = mDic.token(mCurString);
    
    // UDP のテーブル定義の中はルールが変わるのでモードを用意しておく
    if ( id == TABLE ) {
      mContext = kUdp;
    }
    else if ( id == ENDTABLE ) {
      mContext = kNormal;
    }
    break;

  case BASE_B:
    mContext = kBin;
    break;

  case BASE_O:
    mContext = kOct;
    break;

  case BASE_D:
    mContext = kDec;
    break;

  case BASE_H:
    mContext = kHex;
    break;

  case UNUMBER:
  case ERROR:
    mContext = kNormal;
    break;
    
  case EOF:
    if ( mInputMgr->wrap_up() ) {
      goto LOOP;
    }
    break;
    
  default:
    break;
  }
  
  if ( mDebug ) {
    ostringstream buf;
    buf << "get_raw_token(from "
	<< input_file->filename() << ") ==> ";
    print_token(buf, id, mCurString);
    mMsgMgr.put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    kMsgDebug,
		    "LEX",
		    buf.str());
  }

  return id;
}


//////////////////////////////////////////////////////////////////////
// 行番号ウォッチャー関係
//////////////////////////////////////////////////////////////////////

// @brief 行番号ウオッチャーを設定する．
// @param[in] watcher 登録対象のウオッチャー
// watcher がすでに登録されていればなにもしない．
void
RawLex::reg_watcher(VlLineWatcher* watcher)
{
  mWatcherMgr.reg_binder(watcher);
}

// @brief 行番号ウオッチャーの登録を解除する．
// @param[in] watcher 登録削除対象のウオッチャー
// watcher が登録されていなければなにもしない．
void
RawLex::unreg_watcher(VlLineWatcher* watcher)
{
  mWatcherMgr.unreg_binder(watcher);
}


//////////////////////////////////////////////////////////////////////
// プラグインの設定
//////////////////////////////////////////////////////////////////////

// @brief プラグインの追加
// @param[in] name プラグインを起動するコンパイラディレクティブ名
// @param[in] plugin 登録するプラグイン
void
RawLex::add_plugin(const char* name,
		   LexPlugin* plugin)
{
  mPluginDict.reg_plugin(plugin);
}

// @brief プラグインの削除
// @param[in] name 削除するプラグイン名
// @retval true 削除が成功した．
// @retval false name という名のプラグインが存在しなかった．
bool
RawLex::erase_plugin(const char* name)
{
  return mPluginDict.unreg_plugin(name);
}

// @brief プラグインを登録できるか調べる．
// @param[in] name プラグイン名
// @return true 登録可能
// @return false 同名の定義済みプラグインがあり，登録不可能
bool
RawLex::check_pluginname(const char* name)
{
  return mPluginDict.check_name(name);
}

// @brief 内部状態の追加
// @param[in] state 登録する内部状態
void
RawLex::add_state(LexState* state)
{
  mStates.push_back(state);
}


//////////////////////////////////////////////////////////////////////
// デバッグモードの制御
//////////////////////////////////////////////////////////////////////

// @brief デバッグモードのセット
// @param[in] flag true ならすべてのトークンの情報を標準エラーに出力する
void
RawLex::set_debug(bool flag)
{
  mDebug = flag;
}

// @brief デバッグモードの取得
bool
RawLex::debug()
{
  return mDebug;
}


//////////////////////////////////////////////////////////////////////
// 内部で用いられる関数
//////////////////////////////////////////////////////////////////////

// @brief `resetall の処理
void
RawLex::resetall(const FileRegion& file_region)
{
  for (list<LexState*>::iterator p = mStates.begin();
       p != mStates.end(); ++ p) {
    (*p)->resetall(file_region);
  }
} 

// @brief マクロ定義の検査
// @param[in] name 名前
// @return name という名のマクロが定義されていたら true を返す．
bool
RawLex::is_macro_defined(const char* name) const
{
  LexPlugin* plugin = mPluginDict.find_plugin(name);
  if ( plugin == NULL ) {
    return false;
  }
  return plugin->is_macro();
}

// @brief マクロの多重展開のチェック
// @param[in] name 名前
// @return マクロ展開が自己ループしていたら true を返す．
bool
RawLex::check_macro(const char* name) const
{
  ymuint32 n = mMacroStack.size();
  for (ymuint32 i = 0; i < n; ++ i) {
    if ( mMacroStack[i].check_name(name) ) {
      return true;
    }
  }
  return false;
}

// @brief マクロの追加
void
RawLex::push_macro(const char* name,
		   const TokenInfo* top,
		   TokenList* param_array)
{
  mMacroStack.push_back(MacroSource());
  MacroSource& macro = mMacroStack.back();
  macro.set(name, top, param_array);
}

// @brief 改行文字を読み込んだときに呼ばれる関数
void
RawLex::check_line()
{
  mWatcherMgr.prop_event(mInputMgr->cur_file()->cur_line());
}

END_NAMESPACE_YM_VERILOG
