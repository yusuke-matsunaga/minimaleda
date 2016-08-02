#ifndef LIBYM_ISCAS89_ISCAS89PARSERIMPL_H
#define LIBYM_ISCAS89_ISCAS89PARSERIMPL_H

/// @file libym_iscas89/Iscas89ParserImpl.h
/// @brief Iscas89ParserImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Iscas89ParserImpl.h 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_iscas89/iscas89_nsdef.h"
#include "ym_iscas89/Iscas89Handler.h"
#include "ym_utils/MsgHandler.h"
#include "Iscas89Scanner.h"

#include "ym_utils/StrBuff.h"
#include "IdHash.h"


BEGIN_NAMESPACE_YM_ISCAS89

//////////////////////////////////////////////////////////////////////
/// @class Iscas89ParserImpl Iscas89ParserImpl.h "Iscas89ParserImpl.h"
/// @brief Iscas89Parser の実装クラス
//////////////////////////////////////////////////////////////////////
class Iscas89ParserImpl
{
public:

  /// @brief コンストラクタ
  Iscas89ParserImpl();
  
  /// @brief デストラクタ
  ~Iscas89ParserImpl();


public:

  /// @brief 読み込みを行う．
  /// @param[in] filename ファイル名
  /// @retval true 読み込みが成功した．
  /// @retval false 読み込みが失敗した．
  bool
  read(const string& filename);

  /// @brief イベントハンドラの登録
  void
  add_handler(Iscas89Handler* handler);


public:
  
  /// @brief メッセージマネージャを取り出す．
  MsgMgr&
  msg_mgr();
  
  /// @brief yylex() 用の処理を行う．
  /// @param[out] lval トークンの値を格納する変数
  /// @param[out] lloc トークンの位置を格納する変数
  /// @return トークンの型を返す．
  int
  scan(ymuint32& lval,
       FileRegion& lloc);
  
  /// @brief INPUT 文を読み込む．
  /// @param[in] loc ファイル位置
  /// @param[in] name_id 入力ピン名の ID 番号
  /// @return エラーが起きたら false を返す．
  bool
  read_input(const FileRegion& loc,
	     ymuint32 name_id);

  /// @brief OUTPUT 文を読み込む．
  /// @param[in] loc ファイル位置
  /// @param[in] name_id 出力ピン名の ID 番号
  /// @return エラーが起きたら false を返す．
  bool
  read_output(const FileRegion& loc,
	      ymuint32 name_id);

  /// @brief ゲート文を読み込む．
  /// @param[in] loc ファイル位置
  /// @param[in] oname_id 出力名の ID 番号
  /// @param[in] type ゲートタイプ
  /// @return エラーが起きたら false を返す．
  /// @note 入力名のリストは push_str() で積まれている．
  bool
  read_gate(const FileRegion& loc,
	    ymuint32 oname_id,
	    tIscas89GateType type);
  
  /// @brief 名前をリストに追加する．
  /// @param[in] str_id 文字列の ID 番号
  void
  push_str(ymuint32 str_id);
  
  /// @brief 文字列用の領域を確保する．
  /// @param[in] src_str ソース文字列
  /// @param[in] loc 文字列の位置情報
  /// @return 文字列の ID 番号
  ymuint32
  reg_str(const StrBuff& src_str,
	  const FileRegion& loc);
  
  /// @brief ID 番号から IdCell を得る．
  IdCell*
  id2cell(ymuint32 id) const;
  
  /// @brief ID 番号から文字列を得る．
  const char*
  id2str(ymuint32 id) const;
  
  /// @brief ID 番号から位置情報を得る．
  FileRegion
  id2loc(ymuint32 id) const;
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 字句解析器
  Iscas89Scanner mScanner;

  // イベントハンドラのリスト
  list<Iscas89Handler*> mHandlerList;
  
  // メッセージハンドラの管理者
  MsgMgr mMsgMgr;
  
  // 文字列のハッシュ
  IdHash mIdHash;
  
  // 位置情報バッファ
  FileRegion mLoc1;

  // 入力IDのリスト
  vector<ymuint32> mInputList;
  
};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////
  
// @brief メッセージマネージャを取り出す．
inline
MsgMgr&
Iscas89ParserImpl::msg_mgr()
{
  return mMsgMgr;
}
  
// @brief ID 番号から IdCell を得る．
inline
IdCell*
Iscas89ParserImpl::id2cell(ymuint32 id) const
{
  return mIdHash.cell(id);
}
  
// @brief ID 番号から文字列を得る．
inline
const char*
Iscas89ParserImpl::id2str(ymuint32 id) const
{
  return mIdHash.str(id);
}
  
// @brief ID 番号から位置情報を得る．
inline
FileRegion
Iscas89ParserImpl::id2loc(ymuint32 id) const
{
  return mIdHash.loc(id);
}

END_NAMESPACE_YM_ISCAS89

#endif // LIBYM_ISCAS89_ISCAS89PARSERIMPL_H
