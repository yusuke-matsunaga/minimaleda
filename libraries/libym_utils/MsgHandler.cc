
/// @file libym_utils/MsgHandler.cc
/// @brief MsgHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MsgHandler.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/MsgHandler.h"
#include "ym_utils/FileRegion.h"


BEGIN_NAMESPACE_YM

// @brief tMsgType のストリーム出力演算子
// @param[in] s 出力先のストリーム
// @param[in] type メッセージの種類
ostream&
operator<<(ostream& s,
	   tMsgType type)
{
  switch ( type ) {
  case kMsgError:   s << "(ERROR  )"; break;
  case kMsgWarning: s << "(WARNING)"; break;
  case kMsgInfo:    s << "(INFO   )"; break;
  case kMsgFailure: s << "(FAILURE)"; break;
  case kMsgDebug:   s << "(DEBUG  )"; break;
  }
  return s;
}


//////////////////////////////////////////////////////////////////////
// MsgMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MsgMgr::MsgMgr()
{
  clear_count();
}

// @brief デストラクタ
// @note ここに登録してあるすべてのハンドラは削除される．
MsgMgr::~MsgMgr()
{
}

// @brief ハンドラを登録する．
// @param[in] handler 登録するハンドラ
void
MsgMgr::reg_handler(MsgHandler* handler)
{
  mMgr.reg_binder(handler);
}

// @brief ハンドラの登録を解除する．
// @param[in] handler 解除するハンドラ
void
MsgMgr::unreg_handler(MsgHandler* handler)
{
  mMgr.unreg_binder(handler);
}
  
// @brief メッセージを出力する．
// @param[in] src_file この関数を読んでいるソースファイル名
// @param[in] src_line この関数を読んでいるソースの行番号
// @param[in] file_loc ファイル位置
// @param[in] type メッセージの種類
// @param[in] label メッセージラベル
// @param[in] body メッセージ本文
void
MsgMgr::put_msg(const char* src_file,
		int src_line,
		const FileRegion& file_loc,
		tMsgType type,
		const char* label,
		const char* msg)
{
  switch ( type ) {
  case kMsgError:    ++ mErrorNum; break;
  case kMsgWarning:  ++ mWarningNum; break;
  case kMsgInfo:     ++ mInfoNum; break;
  case kMsgFailure:  ++ mFailNum; break;
  case kMsgDebug:    ++ mDebugNum; break;
  default: assert_not_reached(__FILE__, __LINE__);
  }
  
  mMgr.prop_event(src_file, src_line, file_loc, type, label, msg);
}
  
// @brief カウント値をクリアする．
void
MsgMgr::clear_count()
{
  mErrorNum = 0;
  mWarningNum = 0;
  mInfoNum = 0;
  mFailNum = 0;
  mDebugNum = 0;
}

// @brief 全メッセージ数を得る．
ymuint32
MsgMgr::msg_num() const
{
  return mErrorNum + mWarningNum + mInfoNum + mFailNum + mDebugNum;
}

// @brief エラーメッセージ数を得る．
ymuint32
MsgMgr::error_num() const
{
  return mErrorNum;
}

// @brief 警告メッセージ数を得る．
ymuint32
MsgMgr::warning_num() const
{
  return mWarningNum;
}

// @brief 情報メッセージ数を得る．
ymuint32
MsgMgr::info_num() const
{
  return mInfoNum;
}

// @brief 失敗メッセージ数を得る．
ymuint32
MsgMgr::fail_num() const
{
  return mFailNum;
}

// @brief デバッグメッセージ数を得る．
ymuint32
MsgMgr::debug_num() const
{
  return mDebugNum;
}


//////////////////////////////////////////////////////////////////////
// クラス MsgHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MsgHandler::MsgHandler(ymuint32 mask) :
  mMask(mask)
{
}

// @brief デストラクタ
MsgHandler::~MsgHandler()
{
}
    
// @brief メッセージマスクの設定
void
MsgHandler::set_mask(ymuint32 mask)
{
  mMask = mask;
}

// @brief メッセージマスクの取得
ymuint32
MsgHandler::mask() const
{
  return mMask;
}

// @brief マスクの付加
// @param[in] type 付加するメッセージタイプ
// @note type はビットマスクではない．
void
MsgHandler::add_mask(tMsgType type)
{
  mMask |= conv2bitmask(type);
}

// @brief マスクの削除
// @param[in] type 削除するメッセージタイプ
// @note type はビットマスクではない．
void
MsgHandler::delete_mask(tMsgType type)
{
  mMask &= ~(conv2bitmask(type));
}

// @brief メッセージが登録されるたびに呼ばれる仮想関数
// @param[in] src_file この関数を読んでいるソースファイル名
// @param[in] src_line この関数を読んでいるソースの行番号
// @param[in] file_loc ファイル位置
// @param[in] type メッセージの種類
// @param[in] label メッセージラベル
// @param[in] body メッセージ本文
void
MsgHandler::event_proc(const char* src_file,
		       int src_line,
		       const FileRegion& loc,
		       tMsgType type,
		       const char* label,
		       const char* body)
{
  ymuint32 bit = conv2bitmask(type);
  if ( mMask & bit ) {
    put_msg(src_file, src_line, loc, type, label, body);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス StreamMsgHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
StreamMsgHandler::StreamMsgHandler(ostream* stream_ptr) :
  mStreamPtr(stream_ptr)
{
}

// @brief デストラクタ
StreamMsgHandler::~StreamMsgHandler()
{
}

// @brief メッセージが登録されるたびに呼ばれる仮想関数
// @param[in] src_file この関数を読んでいるソースファイル名
// @param[in] src_line この関数を読んでいるソースの行番号
// @param[in] loc ファイル位置
// @param[in] type メッセージの種類
// @param[in] label メッセージラベル
// @param[in] body メッセージ本文
void
StreamMsgHandler::put_msg(const char* src_file,
			  int src_line,
			  const FileRegion& loc,
			  tMsgType type,
			  const char* label,
			  const char* body)
{
  (*mStreamPtr) << loc << type << " [" << label << "]: " << body << endl;
}

END_NAMESPACE_YM
