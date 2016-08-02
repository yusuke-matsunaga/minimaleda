#ifndef YM_UTILS_MSGHANDLER_H
#define YM_UTILS_MSGHANDLER_H

/// @file ym_utils/MsgHandler.h
/// @brief MsgHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MsgHandler.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"
#include "ym_utils/Binder.h"


BEGIN_NAMESPACE_YM

class FileRegion;
class MsgHandler;

//////////////////////////////////////////////////////////////////////
/// @brief メッセージの種類を表す列挙型
//////////////////////////////////////////////////////////////////////
enum tMsgType {
  /// @brief エラー
  /// @note 不正なデータなどの再現性のある致命的な不具合
  kMsgError   = 1,
  /// @brief 警告
  /// @note 軽微な不具合
  kMsgWarning = 2,
  /// @brief 失敗
  /// @note メモリ不足，ファイル読み書き失敗のような外的要因の不具合
  kMsgFailure = 3,
  /// @brief 情報
  /// @note 付加的な情報
  kMsgInfo    = 4,
  /// @brief デバッグ情報
  /// @note デバッグ用の付加的な情報
  kMsgDebug   = 5
};

/// @brief tMsgType のストリーム出力演算子
/// @param[in] s 出力先のストリーム
/// @param[in] type メッセージの種類
ostream&
operator<<(ostream& s,
	   tMsgType type);


//////////////////////////////////////////////////////////////////////
/// @class MsgMgr MsgHandler.h <ym_utils/MsgHandler.h>
/// @ingroup YmUtils
/// @brief メッセージを管理するクラス
//////////////////////////////////////////////////////////////////////
class MsgMgr
{
public:

  /// @brief コンストラクタ
  MsgMgr();

  /// @brief デストラクタ
  /// @note ここに登録してあるすべてのハンドラは削除される．
  ~MsgMgr();
  

public:
  //////////////////////////////////////////////////////////////////////
  // ハンドラの登録
  //////////////////////////////////////////////////////////////////////

  /// @brief ハンドラを登録する．
  /// @param[in] handler 登録するハンドラ
  void
  reg_handler(MsgHandler* handler);

  /// @brief ハンドラの登録を解除する．
  /// @param[in] handler 解除するハンドラ
  void
  unreg_handler(MsgHandler* handler);
  

public:
  //////////////////////////////////////////////////////////////////////
  // メッセージの出力
  //////////////////////////////////////////////////////////////////////

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

  
public:
  //////////////////////////////////////////////////////////////////////
  // 統計情報の管理
  //////////////////////////////////////////////////////////////////////

  /// @brief カウント値をクリアする．
  void
  clear_count();

  /// @brief 全メッセージ数を得る．
  ymuint32
  msg_num() const;

  /// @brief エラーメッセージ数を得る．
  ymuint32
  error_num() const;

  /// @brief 警告メッセージ数を得る．
  ymuint32
  warning_num() const;

  /// @brief 情報メッセージ数を得る．
  ymuint32
  info_num() const;

  /// @brief 失敗メッセージ数を得る．
  ymuint32
  fail_num() const;

  /// @brief デバッグメッセージ数を得る．
  ymuint32
  debug_num() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メッセージを各ハンドラに送るクラス
  T6BindMgr<const char*,
	    int,
	    const FileRegion&,
	    tMsgType,
	    const char*,
	    const char*> mMgr;
  
  // エラーメッセージ数
  ymuint32 mErrorNum;

  // 警告メッセージ数
  ymuint32 mWarningNum;

  // 情報メッセージ数
  ymuint32 mInfoNum;

  // 失敗メッセージ数
  ymuint32 mFailNum;

  // デバッグメッセージ数
  ymuint32 mDebugNum;
  
};


//////////////////////////////////////////////////////////////////////
/// @class MsgHandler MsgHandler.h <ym_utils/MsgHandler.h>
/// @ingroup YmUtils
/// @brief メッセージハンドラを表す基底クラス
//////////////////////////////////////////////////////////////////////
class MsgHandler :
  public T6Binder<const char*,
		  int,
		  const FileRegion&,
		  tMsgType,
		  const char*,
		  const char*>
{
public:

  /// @brief kMsgError 用のビットマスク
  static
  const ymuint32 kMaskError = (1U << static_cast<ymuint32>(kMsgError));

  /// @brief kMsgWarning 用のビットマスク
  static
  const ymuint32 kMaskWarning = (1U << static_cast<ymuint32>(kMsgWarning));

  /// @brief kMsgInfo 用のビットマスク
  static
  const ymuint32 kMaskInfo = (1U << static_cast<ymuint32>(kMsgInfo));

  /// @brief kMsgFailure 用のビットマスク
  static
  const ymuint32 kMaskFailure = (1U << static_cast<ymuint32>(kMsgFailure));

  /// @brief kMsgDebug 用のビットマスク
  static
  const ymuint32 kMaskDebug = (1U << static_cast<ymuint32>(kMsgDebug));

  /// @brief 全てのを含むビットマスク
  static
  const ymuint32 kMaskAll
  = kMaskError | kMaskWarning | kMaskInfo | kMaskFailure | kMaskDebug;

  /// @brief 全てを含まないビットマスク
  static
  const ymuint32 kMaskNone = 0U;

  
public:

  /// @brief コンストラクタ
  /// @param[in] mask メッセージマスク
  MsgHandler(ymuint32 mask = kMaskAll);

  /// @brief デストラクタ
  virtual
  ~MsgHandler();
  

public:
  //////////////////////////////////////////////////////////////////////
  // メッセージマスクの管理
  //////////////////////////////////////////////////////////////////////
  
  /// @brief メッセージマスクの設定
  void
  set_mask(ymuint32 mask);

  /// @brief メッセージマスクの取得
  ymuint32
  mask() const;

  /// @brief マスクの付加
  /// @param[in] type 付加するメッセージタイプ
  /// @note type はビットマスクではない．
  void
  add_mask(tMsgType type);

  /// @brief マスクの削除
  /// @param[in] type 削除するメッセージタイプ
  /// @note type はビットマスクではない．
  void
  delete_mask(tMsgType type);

  
public:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief メッセージが登録されるたびに呼ばれる仮想関数
  /// @param[in] src_file この関数を読んでいるソースファイル名
  /// @param[in] src_line この関数を読んでいるソースの行番号
  /// @param[in] file_loc ファイル位置
  /// @param[in] type メッセージの種類
  /// @param[in] label メッセージラベル
  /// @param[in] body メッセージ本文
  virtual
  void
  put_msg(const char* src_file,
	  int src_line,
	  const FileRegion& loc,
	  tMsgType type,
	  const char* label,
	  const char* body) = 0;
  

private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるメンバ関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief メッセージが登録されるたびに呼ばれる仮想関数
  /// @param[in] src_file この関数を読んでいるソースファイル名
  /// @param[in] src_line この関数を読んでいるソースの行番号
  /// @param[in] file_loc ファイル位置
  /// @param[in] type メッセージの種類
  /// @param[in] label メッセージラベル
  /// @param[in] body メッセージ本文
  virtual
  void
  event_proc(const char* src_file,
	     int src_line,
	     const FileRegion& loc,
	     tMsgType type,
	     const char* label,
	     const char* body);
  
  /// @brief メッセータイプからビットマスクを得る．
  static
  ymuint32
  conv2bitmask(tMsgType type);

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メッセージマスク
  ymuint32 mMask;
  
};


//////////////////////////////////////////////////////////////////////
/// @class StreamMsgHandler MsgHandler.h <ym_utils/MsgHandler.h>
/// @ingroup YmUtils
/// @brief ストリーム出力する MsgHandler の実装
//////////////////////////////////////////////////////////////////////
class StreamMsgHandler :
  public MsgHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] stream_ptr ストリームへのポインタ
  StreamMsgHandler(ostream* stream_ptr);

  /// @brief デストラクタ
  virtual
  ~StreamMsgHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // MsgHandler (T6Binder) の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief メッセージが登録されるたびに呼ばれる仮想関数
  /// @param[in] src_file この関数を読んでいるソースファイル名
  /// @param[in] src_line この関数を読んでいるソースの行番号
  /// @param[in] file_loc ファイル位置
  /// @param[in] type メッセージの種類
  /// @param[in] label メッセージラベル
  /// @param[in] body メッセージ本文
  virtual
  void
  put_msg(const char* src_file,
	  int src_line,
	  const FileRegion& loc,
	  tMsgType type,
	  const char* label,
	  const char* body);
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力先のストリーム
  ostream* mStreamPtr;

};


//////////////////////////////////////////////////////////////////////
//　インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief メッセータイプからビットマスクを得る．
inline
ymuint32
MsgHandler::conv2bitmask(tMsgType type)
{
  return 1U << static_cast<ymuint32>(type);
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
MsgMgr::put_msg(const char* src_file,
		int src_line,
		const FileRegion& file_loc,
		tMsgType type,
		const char* label,
		const string& msg)
{
  put_msg(src_file, src_line, file_loc, type, label, msg.c_str());
}

END_NAMESPACE_YM

#endif // YM_UTILS_MSGHANDLER_H
