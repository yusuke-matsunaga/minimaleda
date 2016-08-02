#ifndef YM_SAT_DIMACSPARSER_H
#define YM_SAT_DIMACSPARSER_H

/// @file ym_sat/DimacsParser.h
/// @brief DimacsParser のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DimacsParser.h 1343 2008-03-25 17:15:35Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_sat/sat_nsdef.h"


BEGIN_NAMESPACE_YM_SAT

// クラスの前方宣言
class DimacsParser;


//////////////////////////////////////////////////////////////////////
/// @class DimacsHandler DimacsParser.h <ym_sat/DimacsParser.h>
/// @ingroup SatGroup
/// @brief DIMACS パーサーのイベントハンドラの基底クラス
/// @sa DimacsParser
//////////////////////////////////////////////////////////////////////
class DimacsHandler
{
  friend class DimacsParser;
protected:

  /// @brief コンストラクタ
  DimacsHandler();

  /// @brief デストラクタ
  virtual
  ~DimacsHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // 公開メソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief パーサーを得る．
  DimacsParser*
  parser() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスは必要に応じて下記の仮想関数を上書きすること
  // デフォルトのハンドラはなにもしない．
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  init();
  
  /// @brief p 行の読込み
  /// @param[in] lineno 行番号
  /// @param[in] nv 変数の数
  /// @param[in] nc 節の数
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  read_p(int lineno,
	 size_t nv,
	 size_t nc);
  
  /// @brief clause 行の読込み
  /// @param[in] lineno 行番号
  /// @param[in] lits リテラルの配列．最後の0は含まない
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  read_clause(int lineno,
	      const vector<int>& lits);
  
  /// @brief 終了処理
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  end();

  /// @brief エラー終了時の処理
  virtual
  void
  error_exit();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のパーサー
  DimacsParser* mParser;

};


//////////////////////////////////////////////////////////////////////
/// @class DimacsMsgHandler DimacsParser.h <ym_sat/DimacsParser.h>
/// @ingroup SatGroup
/// @brief DIMACS形式のファイルを読み込むパーサーのメッセージハンドラ
/// @sa DimacsParser
//////////////////////////////////////////////////////////////////////
class DimacsMsgHandler
{
  friend class DimacsParser;
public:

  /// @brief コンストラクタ
  DimacsMsgHandler();

  /// @brief デストラクタ
  virtual
  ~DimacsMsgHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // 公開メソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief パーサーを得る．
  DimacsParser*
  parser() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスは下記の仮想関数を上書きすること
  //////////////////////////////////////////////////////////////////////

  /// @brief メッセージが登録されるたびに呼ばれる仮想関数
  /// @param[in] src_file この関数を読んでいるソースファイル名
  /// @param[in] src_line この関数を読んでいるソースの行番号
  /// @param[in] lineno ファイル中の行番号
  /// @param[in] label メッセージラベル
  /// @param[in] body メッセージ本文
  virtual
  void
  operator()(const char* src_file,
	     int src_line,
	     int lineno,
	     const string& label,
	     const string& body) = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のパーサー
  DimacsParser* mParser;

};


//////////////////////////////////////////////////////////////////////
/// @class DimacsParser DimacsParser.h <ym_sat/DimacsParser.h>
/// @ingroup SatGroup
/// @brief DIMACS形式の CNFファイルを読み込むパーサークラス
/// @sa DimacsHandler DimacsMsgHandler
//////////////////////////////////////////////////////////////////////
class DimacsParser
{
public:

  /// @brief コンストラクタ
  DimacsParser();
  
  /// @brief デストラクタ
  ~DimacsParser();
  
  /// @brief 読み込みを行う．
  /// @param[in] in 入力元のストリーム
  /// @retval true 読み込みが成功した．
  /// @retval false 読み込みが失敗した．
  bool
  read(istream& in);

  /// @brief イベントハンドラの登録
  void
  add_handler(DimacsHandler* handler);

  /// @brief メッセージハンドラの登録
  void
  add_msg_handler(DimacsMsgHandler* handler);


private:

  /// @brief トークンの型定義
  enum tToken {
    kC,
    kP,
    kNUM,
    kZERO,
    kNL,
    kEOF,
    kERR
  };


private:

  // @brief トークンの読込み
  tToken
  read_token(istream& in);

  // 一文字読み出す．
  int
  read_char(istream& in);

  /// @brief メッセージの追加
  /// @param[in] src_file この関数を読んでいるソースファイル名
  /// @param[in] src_line この関数を読んでいるソースの行番号
  /// @param[in] loc ファイル位置
  /// @param[in] label メッセージラベル
  /// @param[in] body メッセージ本文
  void
  add_msg(const char* src_file,
	  int src_line,
	  int lineno,
	  const string& label,
	  const string& body);
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // イベントハンドラのリスト
  list<DimacsHandler*> mHandlerList;

  // メッセージハンドラのリスト
  list<DimacsMsgHandler*> mMsgHandlerList;

  // 現在処理中の行番号
  int mLineNo;

  // read_token で読んだトークンが kNUM の場合の値
  int mCurVal;

  // \r を読み込んだ直後に true となるフラグ
  bool mCR;

  // \n を読んでしまったときに true となるフラグ
  bool mNL;
  
};

END_NAMESPACE_YM_SAT

BEGIN_NAMESPACE_YM

using nsSat::DimacsHandler;
using nsSat::DimacsMsgHandler;
using nsSat::DimacsParser;

END_NAMESPACE_YM

#endif // YM_SAT_DIMACSPARSER_H
