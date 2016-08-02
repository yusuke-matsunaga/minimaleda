
/// @file libym_gds/Msg.cc
/// @brief メッセージ出力用のクラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Msg.cc 700 2007-05-31 00:41:30Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_gds/Msg.h"


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
// メッセージ生成用のヘルパークラス
//////////////////////////////////////////////////////////////////////
class MsgHelper :
  public ostringstream
{
public:

  // コンストラクタ
  MsgHelper();
  
  // デストラクタ
  virtual
  ~MsgHelper();

  // 内容を設定する．
  void set(const char* src_file,
	   int src_line,
	   size_t offset,
	   Msg::tType type,
	   const string& label,
	   const string& body);

  // Msg を生成する．
  Msg* new_msg() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メッセージの生成場所のファイル名
  const char* mSrcFile;

  // メッセージの生成場所の行番号
  int mSrcLine;

  // ファイル位置情報
  size_t mOffset;
  
  // メッセージの種類
  Msg::tType mType;

  // ラベル
  string mLabel;

  // 本文
  string mBody;

};

// コンストラクタ
MsgHelper::MsgHelper()
{
}
  
// デストラクタ
MsgHelper::~MsgHelper()
{
}

// 内容を設定する．
void
MsgHelper::set(const char* src_file,
	       int src_line,
	       size_t offset,
	       Msg::tType type,
	       const string& label,
	       const string& body)
{
  mSrcFile = src_file;
  mSrcLine = src_line;
  mOffset = offset;
  mType = type;
  mLabel = label;
  mBody = body;
  str("");
}

// Msg を生成する．
Msg*
MsgHelper::new_msg() const
{
  return new Msg(mSrcFile, mSrcLine, mOffset, mType, mLabel, mBody + str());
}


//////////////////////////////////////////////////////////////////////
// メッセージを表す基底クラス
//////////////////////////////////////////////////////////////////////
  
// コンストラクタ
Msg::Msg(const char* src_file,
	 int src_line,
	 size_t offset,
	 tType type,
	 const string& label,
	 const string& body) :
  mSrcFile(src_file),
  mSrcLine(src_line),
  mOffset(offset),
  mType(type),
  mLabel(label),
  mBody(body)
{
}

// デストラクタ
Msg::~Msg()
{
}

// メッセージの生成場所のファイル名を返す．
const char*
Msg::src_file() const
{
  return mSrcFile;
}

// メッセージの生成場所の行番号を返す．
int
Msg::src_line() const
{
  return mSrcLine;
}

// ファイル位置情報を返す．
size_t
Msg::offset() const
{
  return mOffset;
}

// メッセージの種類を返す．
Msg::tType
Msg::type() const
{
  return mType;
}

// ラベルを返す．
const string&
Msg::label() const
{
  return mLabel;
}

// 本文を返す．
const string&
Msg::body() const
{
  return mBody;
}


//////////////////////////////////////////////////////////////////////
// メッセージを管理するクラス
//////////////////////////////////////////////////////////////////////

// 唯一のインスタンス
MsgMgr* MsgMgr::mTheMgr = 0;

// 唯一のインスタンスを得るための関数
MsgMgr&
MsgMgr::the_mgr()
{
  if ( !mTheMgr ) {
    mTheMgr = new MsgMgr();
  }
  return *mTheMgr;
}

// コンストラクタ
MsgMgr::MsgMgr()
{
  mMsgHelper = new MsgHelper;
  clear();
}

// デストラクタ
MsgMgr::~MsgMgr()
{
  delete mMsgHelper;
}

// エラーメッセージを出力する．
ostream&
error_header(const char* src_file,
	     int src_line,
	     const string& label,
	     size_t offset,
	     const string& body)
{
  return MsgMgr::the_mgr().msg_header(src_file, src_line,
				      Msg::kError, label, offset, body);
}

// 警告メッセージを出力する．
ostream&
warning_header(const char* src_file,
	       int src_line,
	       const string& label,
	       size_t offset,
	       const string& body)
{
  return MsgMgr::the_mgr().msg_header(src_file, src_line,
				      Msg::kWarning, label, offset, body);
}

// 情報メッセージを出力する．
ostream&
info_header(const char* src_file,
	    int src_line,
	    const string& label,
	    size_t offset,
	    const string& body)
{
  return MsgMgr::the_mgr().msg_header(src_file, src_line,
				      Msg::kInfo, label, offset, body);
}

// 失敗メッセージを出力する．
ostream&
fail_header(const char* src_file,
	    int src_line,
	    const string& label,
	    size_t offset,
	    const string& body)
{
  return MsgMgr::the_mgr().msg_header(src_file, src_line,
				      Msg::kFail, label, offset, body);
}
  
// デバッグメッセージ用のヘッダを出力する
ostream&
debug_header(const char* src_file,
	     int src_line,
	     const string& label,
	     size_t offset,
	     const string& body)
{
  return MsgMgr::the_mgr().msg_header(src_file, src_line,
				      Msg::kDebug, label, offset, body);
}

// メッセージを出力する．
void
msg_end()
{
  MsgMgr::the_mgr().put_msg();
}

// 内容をクリアする．
void
MsgMgr::clear()
{
  mAllMsgNum = 0;
  mErrorMsgNum = 0;
  mWarningMsgNum = 0;
  mInfoMsgNum = 0;
  mFailMsgNum = 0;
  mDebugMsgNum = 0;
}

// 全メッセージ数を得る．
size_t
MsgMgr::msg_num() const
{
  return mAllMsgNum;
}

// エラーメッセージ数を得る．
size_t
MsgMgr::error_num() const
{
  return mErrorMsgNum;
}

// 警告メッセージ数を得る．
size_t
MsgMgr::warning_num() const
{
  return mWarningMsgNum;
}

// 情報メッセージ数を得る．
size_t
MsgMgr::info_num() const
{
  return mInfoMsgNum;
}

// 失敗メッセージ数を得る．
size_t
MsgMgr::fail_num() const
{
  return mFailMsgNum;
}

// デバッグメッセージ数を得る．
size_t
MsgMgr::debug_num() const
{
  return mDebugMsgNum;
}

// メッセージハンドラを登録する．
void
MsgMgr::reg_handler(MsgHandler* handler)
{
  mHandlerList.push_back(handler);
}

// メッセージ用のストリームを返す．
ostream&
MsgMgr::msg_header(const char* src_file,
		   int src_line,
		   Msg::tType type,
		   const string& label,
		   size_t offset,
		   const string& body)
{
  mMsgHelper->set(src_file, src_line, offset, type, label, body);
  return *mMsgHelper;
}

// msg_end() 中で呼ばれる関数
void
MsgMgr::put_msg()
{
  Msg* msg = mMsgHelper->new_msg();

  ++ mAllMsgNum;
  size_t bit = 0;
  switch ( msg->type() ) {
  case Msg::kError:   bit = kMsgMaskError;   ++ mErrorMsgNum; break;
  case Msg::kWarning: bit = kMsgMaskWarning; ++ mWarningMsgNum; break;
  case Msg::kInfo:    bit = kMsgMaskInfo;    ++ mInfoMsgNum; break;
  case Msg::kFail:    bit = kMsgMaskFail;    ++ mFailMsgNum; break;
  case Msg::kDebug:   bit = kMsgMaskDebug;   ++ mDebugMsgNum; break;
  }
  for (list<MsgHandler*>::const_iterator p = mHandlerList.begin();
       p != mHandlerList.end(); ++ p) {
    MsgHandler* mh = *p;
    if ( mh->mask() & bit ) {
      (*mh)(msg);
    }
  }
}


//////////////////////////////////////////////////////////////////////
// テスト用のメッセージハンドラ
//////////////////////////////////////////////////////////////////////

// コンストラクタ
TestMsgHandler::TestMsgHandler(size_t mask) :
  MsgHandler(mask)
{
}

// デストラクタ
TestMsgHandler::~TestMsgHandler()
{
}

// メッセージを処理する．
void
TestMsgHandler::operator()(const Msg* msg)
{
  cerr << "[";
  switch ( msg->type() ) {
  case Msg::kError:   cerr << "ERROR-"; break;
  case Msg::kWarning: cerr << "WARN-"; break;
  case Msg::kInfo:    cerr << "INFO-"; break;
  case Msg::kFail:    cerr << "FAIL-"; break;
  case Msg::kDebug:   cerr << "DEBUG-"; break;
  }
  cerr << msg->label() << "]: ";

  switch ( msg->type() ) {
  case Msg::kError:
  case Msg::kWarning:
    cerr << "(At " << msg->src_file() << ": line " << msg->src_line()
	 << ")" << endl
	 << hex << msg->offset() << dec << ": " << msg->body() << endl;
    break;
  case Msg::kInfo:
    cerr << hex << msg->offset() << dec << ": " << msg->body() << endl;
    break;
  case Msg::kFail:
    cerr << "(At " << msg->src_file() << ": line " << msg->src_line()
	 << ")" << endl
	 << msg->body() << endl;
    break;
  case Msg::kDebug:
    cerr << "(At " << msg->src_file() << ": line " << msg->src_line()
	 << ")" << endl
	 << hex << msg->offset() << dec << ": " << msg->body() << endl;
    break;
  }
}

END_NAMESPACE_YM_GDS
