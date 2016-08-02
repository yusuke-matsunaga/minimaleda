
/// @file libym_iscas89/Iscas89ParserImpl.cc
/// @brief BlibParser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Iscas89ParserImpl.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "Iscas89ParserImpl.h"
#include "ym_iscas89/Iscas89Handler.h"
#include "ym_utils/FileDescMgr.h"


BEGIN_NAMESPACE_YM_ISCAS89

// yacc/bison が生成するヘッダファイル
#include "iscas89_grammer.h"

//////////////////////////////////////////////////////////////////////
// Iscas89ParserImpl
//////////////////////////////////////////////////////////////////////

// コンストラクタ
Iscas89ParserImpl::Iscas89ParserImpl()
{
}

// デストラクタ
Iscas89ParserImpl::~Iscas89ParserImpl()
{
  for (list<Iscas89Handler*>::iterator p = mHandlerList.begin();
       p != mHandlerList.end(); ++ p) {
    Iscas89Handler* handler = *p;
    delete handler;
  }
}

// 読み込みを行なう．
bool
Iscas89ParserImpl::read(const string& filename)
{
  int yyparse(Iscas89ParserImpl&);
  
  // ファイルをオープンする．
  ifstream input_stream;
  input_stream.open(filename.c_str());
  if ( !input_stream ) {
    // エラー
    ostringstream buf;
    buf << filename << " : No such file.";
    mMsgMgr.put_msg(__FILE__, __LINE__, FileRegion(),
		    kMsgFailure, "BLIF_PARSER", buf.str());
    return false;
  }
  
  // 初期化する．
  FileDescMgr fdmgr;
  const FileDesc* file_desc = fdmgr.new_file_desc(filename);
  mScanner.init(input_stream, file_desc);

  for (list<Iscas89Handler*>::iterator p = mHandlerList.begin();
       p != mHandlerList.end(); ++ p) {
    Iscas89Handler* handler = *p;
    if ( !handler->init() ) {
      return false;
    }
  }

  int status = yyparse(*this);

  if ( status == 0 ) {
    // 成功
    for (list<Iscas89Handler*>::iterator p = mHandlerList.begin();
	 p != mHandlerList.end(); ++ p) {
      Iscas89Handler* handler = *p;
      handler->normal_exit();
    }
    mIdHash.clear();
    return true;
  }
  else {
    // 失敗
    for (list<Iscas89Handler*>::iterator p = mHandlerList.begin();
	 p != mHandlerList.end(); ++ p) {
      Iscas89Handler* handler = *p;
      handler->error_exit();
    }
    mIdHash.clear();
    return false;
  }
}

// @brief イベントハンドラの登録
void
Iscas89ParserImpl::add_handler(Iscas89Handler* handler)
{
  mHandlerList.push_back(handler);
  handler->mParser = this;
}
  
// @brief yylex() 用の処理を行う．
// @param[out] lval トークンの値を格納する変数
// @param[out] lloc トークンの位置を格納する変数
// @return トークンの型を返す．
int
Iscas89ParserImpl::scan(ymuint32& lval,
			FileRegion& lloc)
{
  int id = mScanner.get_token();
  lloc = mScanner.cur_loc();
  if ( id == NAME ) {
    lval = reg_str(mScanner.cur_string(), lloc);
  }
  return id;
}

// @brief INPUT 文を読み込む．
// @param[in] loc ファイル位置
// @param[in] name_id 入力ピン名の ID 番号
// @return エラーが起きたら false を返す．
bool
Iscas89ParserImpl::read_input(const FileRegion& loc,
			      ymuint32 name_id)
{
  IdCell* cell = id2cell(name_id);
  if ( cell->is_defined() ) {
    ostringstream buf;
    buf << cell->str() << ": Defined more than once. Previous definition is "
	<< cell->def_loc();
    mMsgMgr.put_msg(__FILE__, __LINE__, cell->loc(),
		    kMsgError,
		    "ER_MLTDEF01",
		    buf.str());
    return false;
  }
  cell->set_defined();
  cell->set_input();
  for (list<Iscas89Handler*>::iterator p = mHandlerList.begin();
       p != mHandlerList.end(); ++ p) {
    Iscas89Handler* handler = *p;
    if ( !handler->read_input(loc, name_id) ) {
      return false;
    }
  }
  return true;
}

// @brief OUTPUT 文を読み込む．
// @param[in] loc ファイル位置
// @param[in] name_id 出力ピン名の ID 番号
// @return エラーが起きたら false を返す．
bool
Iscas89ParserImpl::read_output(const FileRegion& loc,
			       ymuint32 name_id)
{
  IdCell* cell = id2cell(name_id);
  if ( cell->is_input() ) {
    ostringstream buf;
    buf << cell->str() << ": Defined as both input and output. "
	<< "Previous definition is "
	<< cell->def_loc();
    mMsgMgr.put_msg(__FILE__, __LINE__, cell->loc(),
		    kMsgWarning,
		    "WR_MLTDEF02",
		    buf.str());
  }
  cell->set_output();
  for (list<Iscas89Handler*>::iterator p = mHandlerList.begin();
       p != mHandlerList.end(); ++ p) {
    Iscas89Handler* handler = *p;
    if ( !handler->read_output(loc, name_id) ) {
      return false;
    }
  }
  return true;
}

// @brief ゲート文を読み込む．
// @param[in] loc ファイル位置
// @param[in] oname_id 出力名の ID 番号
// @param[in] type ゲートタイプ
// @return エラーが起きたら false を返す．
// @note 入力名のリストは push_str() で積まれている．
bool
Iscas89ParserImpl::read_gate(const FileRegion& loc,
			     ymuint32 oname_id,
			     tIscas89GateType type)
{
  IdCell* cell = id2cell(oname_id);
  if ( cell->is_defined() ) {
    // 二重定義
    ostringstream buf;
    buf << cell->str() << ": Defined more than once. "
	<< "Previsous Definition is " << cell->def_loc();
    mMsgMgr.put_msg(__FILE__, __LINE__, cell->loc(),
		    kMsgError,
		    "ER_MLTDEF01",
		    buf.str());
    mInputList.clear();
    return false;
  }
  cell->set_defined();
  bool stat = true;
  for (list<Iscas89Handler*>::iterator p = mHandlerList.begin();
       p != mHandlerList.end(); ++ p) {
    Iscas89Handler* handler = *p;
    if ( !handler->read_gate(loc, type, oname_id, mInputList) ) {
      stat = false;
      break;
    }
  }
  mInputList.clear();
  return stat;
}

// @brief 名前をリストに追加する．
// @param[in] str_id 文字列の ID 番号
void
Iscas89ParserImpl::push_str(ymuint32 str_id)
{
  mInputList.push_back(str_id);
}

// @brief 文字列用の領域を確保する．
// @param[in] src_str ソース文字列
// @param[in] loc 文字列の位置情報
// @return 文字列の ID 番号
ymuint32
Iscas89ParserImpl::reg_str(const StrBuff& src_str,
			   const FileRegion& loc)
{
  IdCell* cell = mIdHash.find(src_str.c_str(), true);
  cell->set_loc(loc);
  return cell->id();
}

END_NAMESPACE_YM_ISCAS89
