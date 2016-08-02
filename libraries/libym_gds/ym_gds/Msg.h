#ifndef YM_GDS_MSG_H
#define YM_GDS_MSG_H

/// @file ym_gds/Msg.h
/// @brief Msg のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Msg.h 1343 2008-03-25 17:15:35Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.

// メッセージを表すクラス

#include <ym_gds/gds_nsdef.h>


BEGIN_NAMESPACE_YM_GDS

class MsgHelper;

//////////////////////////////////////////////////////////////////////
// メッセージを表す基底クラス
//////////////////////////////////////////////////////////////////////
class Msg
{
  friend class MsgHelper;
  friend class MsgMgr;
public:
  // メッセージの種類
  enum tType {
    kError,
    kWarning,
    kInfo,
    kFail,
    kDebug
  };


private:
  
  // コンストラクタ
  Msg(const char* src_file,
      int src_line,
      size_t offset,
      tType type,
      const string& label,
      const string& body);

  // デストラクタ
  virtual
  ~Msg();


public:

  // メッセージの生成場所のファイル名を返す．
  const char*
  src_file() const;

  // メッセージの生成場所の行番号を返す．
  int
  src_line() const;

  // ファイル位置情報を返す．
  size_t
  offset() const;

  // メッセージの種類を返す．
  tType
  type() const;

  // ラベルを返す．
  const string&
  label() const;

  // 本文を返す．
  const string&
  body() const;


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
  tType mType;

  // ラベル
  string mLabel;

  // 本文
  string mBody;

};


//////////////////////////////////////////////////////////////////////
// メッセージ用のマスク値
//////////////////////////////////////////////////////////////////////
enum tMsgMask {
  kMsgMaskNone    = 0,
  kMsgMaskError   = 1,
  kMsgMaskWarning = 2,
  kMsgMaskInfo    = 4,
  kMsgMaskFail    = 8,
  kMsgMaskDebug   = 16,
  kMsgMaskAll     = kMsgMaskError | kMsgMaskWarning | kMsgMaskInfo |
                    kMsgMaskFail  | kMsgMaskDebug
};

// マスクに対する演算
inline
tMsgMask
operator~(tMsgMask a)
{
  size_t a_int = static_cast<size_t>(a);
  return static_cast<tMsgMask>(~a_int & kMsgMaskAll);
}

inline
tMsgMask
operator|(tMsgMask a,
	  tMsgMask b)
{
  // 実はただのキャスト
  size_t a_int = static_cast<size_t>(a);
  size_t b_int = static_cast<size_t>(b);
  return static_cast<tMsgMask>(a_int | b_int);
}

inline
tMsgMask
operator&(tMsgMask a,
	  tMsgMask b)
{
  // 実はただのキャスト
  size_t a_int = static_cast<size_t>(a);
  size_t b_int = static_cast<size_t>(b);
  return static_cast<tMsgMask>(a_int & b_int);
}

inline
tMsgMask
operator^(tMsgMask a,
	  tMsgMask b)
{
  // 実はただのキャスト
  size_t a_int = static_cast<size_t>(a);
  size_t b_int = static_cast<size_t>(b);
  return static_cast<tMsgMask>(a_int ^ b_int);
}


//////////////////////////////////////////////////////////////////////
// メッセージを扱うクラスの基底クラス
//////////////////////////////////////////////////////////////////////
class MsgHandler
{
 public:

  // コンストラクタ
  // マスクを設定する．
  explicit
  MsgHandler(size_t mask) : mMask(mask) { };

  // デストラクタ
  virtual
  ~MsgHandler() { };

  // メッセージを処理する仮想関数
  virtual
  void
  operator()(const Msg* msg) = 0;

  // マスクを得る．
  size_t
  mask() const { return mMask; }

  // マスクを設定する．
  void
  set_mask(size_t new_mask) { mMask = new_mask; }
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // マスク
  size_t mMask;
  
};


//////////////////////////////////////////////////////////////////////
// メッセージを管理するクラス
//////////////////////////////////////////////////////////////////////
class MsgMgr
{
public:

  // 唯一のインスタンスを得るための関数
  static MsgMgr& the_mgr();
  

private:

  // コンストラクタ
  MsgMgr();

  // デストラクタ
  ~MsgMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // メッセージ出力関係の非メンバ関数
  //////////////////////////////////////////////////////////////////////

  // エラーメッセージ用のヘッダ(ファイル情報)を出力する
  // メッセージはこの関数の返り値に '<<' 演算子で書き込む．
  friend
  ostream&
  error_header(const char* src_file,
	       int src_line,
	       const string& label,
	       size_t offset,
	       const string& body);
  
  // 警告メッセージ用のヘッダ(ファイル情報)を出力する
  // メッセージはこの関数の返り値に '<<' 演算子で書き込む．
  friend
  ostream&
  warning_header(const char* src_file,
		 int src_line,
		 const string& label,
		 size_t offset,
		 const string& body);
  
  // 情報メッセージ用のヘッダ(ファイル情報)を出力する
  // メッセージはこの関数の返り値に '<<' 演算子で書き込む．
  friend
  ostream&
  info_header(const char* src_file,
	      int src_line,
	      const string& label,
	      size_t offset,
	      const string& body);
  
  // 失敗メッセージ用のヘッダ(ファイル情報)を出力する
  // メッセージはこの関数の返り値に '<<' 演算子で書き込む．
  friend
  ostream&
  fail_header(const char* src_file,
	      int src_line,
	      const string& label,
	      size_t offset,
	      const string& body);
  
  // デバッグメッセージ用のヘッダを出力する
  // メッセージはこの関数の返り値に '<<' 演算子で書き込む．
  friend
  ostream&
  debug_header(const char* src_file,
	       int src_line,
	       const string& label,
	       size_t offset,
	       const string& body);

  // 上記の XXX_header 関数で書き込まれたメッセージを出力する．
  friend
  void
  msg_end();
  
  
  //////////////////////////////////////////////////////////////////////
  // メンバ関数
  //////////////////////////////////////////////////////////////////////

  // 内容をクリアする．
  void
  clear();

  // 全メッセージ数を得る．
  size_t
  msg_num() const;

  // エラーメッセージ数を得る．
  size_t
  error_num() const;

  // 警告メッセージ数を得る．
  size_t
  warning_num() const;

  // 情報メッセージ数を得る．
  size_t
  info_num() const;

  // 失敗メッセージ数を得る．
  size_t
  fail_num() const;

  // デバッグメッセージ数を得る．
  size_t
  debug_num() const;

  // メッセージハンドラを登録する．
  void
  reg_handler(MsgHandler* handler);


private:

  // メッセージ用のストリームを返す．
  ostream&
  msg_header(const char* src_file,
	     int src_line,
	     Msg::tType type,
	     const string& label,
	     size_t offset,
	     const string& body);

  // msg_end() 中で呼ばれる関数
  void
  put_msg();
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 全メッセージ数
  size_t mAllMsgNum;

  // エラーメッセージ数
  size_t mErrorMsgNum;

  // 警告メッセージ数
  size_t mWarningMsgNum;

  // 情報メッセージ数
  size_t mInfoMsgNum;

  // 失敗メッセージ数
  size_t mFailMsgNum;

  // デバッグメッセージ数
  size_t mDebugMsgNum;

  // メッセージ出力用のヘルパーオブジェクト
  MsgHelper* mMsgHelper;

  // メッセージハンドラのリスト
  list<MsgHandler*> mHandlerList;
  

private:
  //////////////////////////////////////////////////////////////////////
  // 静的データメンバ
  //////////////////////////////////////////////////////////////////////

  // 唯一のオブジェクト
  static MsgMgr* mTheMgr;

};


//////////////////////////////////////////////////////////////////////
// テスト用のメッセージハンドラ
//////////////////////////////////////////////////////////////////////
class TestMsgHandler :
  public MsgHandler
{
 public:
  
  // コンストラクタ
  explicit
  TestMsgHandler(size_t mask = kMsgMaskAll);

  // デストラクタ
  virtual
  ~TestMsgHandler();

  // メッセージを処理する．
  virtual
  void
  operator()(const Msg* msg);
  
};


// エラーメッセージ用のヘッダ(ファイル情報)を出力する
// メッセージはこの関数の返り値に '<<' 演算子で書き込む．
ostream&
error_header(const char* src_file,
	     int src_line,
	     const string& label,
	     size_t offset,
	     const string& body = string());

// 警告メッセージ用のヘッダ(ファイル情報)を出力する
// メッセージはこの関数の返り値に '<<' 演算子で書き込む．
ostream&
warning_header(const char* src_file,
	       int src_line,
	       const string& label,
	       size_t offset,
	       const string& body = string());
  
// 情報メッセージ用のヘッダ(ファイル情報)を出力する
// メッセージはこの関数の返り値に '<<' 演算子で書き込む．
ostream&
info_header(const char* src_file,
	    int src_line,
	    const string& label,
	    size_t offset,
	    const string& body = string());
  
// 失敗メッセージ用のヘッダ(ファイル情報)を出力する
// メッセージはこの関数の返り値に '<<' 演算子で書き込む．
ostream&
fail_header(const char* src_file,
	    int src_line,
	    const string& label,
	    size_t offset,
	    const string& body = string());
  
// デバッグメッセージ用のヘッダを出力する
// メッセージはこの関数の返り値に '<<' 演算子で書き込む．
ostream&
debug_header(const char* src_file,
	     int src_line,
	     const string& label,
	     size_t offset,
	     const string& body = string());

// 上記の XXX_header 関数で書き込まれたメッセージを出力する．
void
msg_end();

END_NAMESPACE_YM_GDS

#endif // YM_GDS_MSG_H
