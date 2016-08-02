#ifndef YM_ISCAS89_ISCAS89HANDLER_H
#define YM_ISCAS89_ISCAS89HANDLER_H

/// @file ym_iscas89/Iscas89Handler.h
/// @brief Iscas89Handler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Iscas89Handler.h 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_iscas89/iscas89_nsdef.h"
#include "ym_utils/FileRegion.h"
#include "ym_utils/MsgHandler.h"


BEGIN_NAMESPACE_YM

/// @brief ゲートの型
/// @ingroup Iscas89Group
enum tIscas89GateType {
  kGt89BUFF,
  kGt89NOT,
  kGt89AND,
  kGt89NAND,
  kGt89OR,
  kGt89NOR,
  kGt89XOR,
  kGt89XNOR,
  kGt89DFF
};

END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_ISCAS89

//////////////////////////////////////////////////////////////////////
/// @class Iscas89Handler Iscas89Handler.h <ym_iscas89/Iscas89Handler.h>
/// @ingroup Iscas89Group
/// @brief iscas89 パーサーのイベントハンドラの基底クラス
/// @sa Iscas89Parser
//////////////////////////////////////////////////////////////////////
class Iscas89Handler
{
  friend class Iscas89ParserImpl;

protected:

  /// @brief コンストラクタ
  Iscas89Handler();
  
  /// @brief デストラクタ
  virtual
  ~Iscas89Handler();


public:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスは必要に応じて下記の仮想関数を上書きすること
  // デフォルトの実装はなにもしない．
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  init();

  /// @brief INPUT 文を読み込む．
  /// @param[in] loc ファイル位置
  /// @param[in] name_id 入力ピン名の ID 番号
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  read_input(const FileRegion& loc,
	     ymuint32 name_id);

  /// @brief OUTPUT 文を読み込む．
  /// @param[in] loc ファイル位置
  /// @param[in] name_id 出力ピン名の ID 番号
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  read_output(const FileRegion& loc,
	      ymuint32 name_id);

  /// @brief ゲート文を読み込む．
  /// @param[in] loc ファイル位置
  /// @param[in] type ゲートの型
  /// @param[in] oname_id 出力名の ID 番号
  /// @param[in] iname_list 入力名のリスト
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  read_gate(const FileRegion& loc,
	    tIscas89GateType type,
	    ymuint32 oname_id,
	    const vector<ymuint32>& iname_list);
  
  /// @brief 通常終了時の処理
  virtual
  void
  normal_exit();

  /// @brief エラー終了時の処理
  virtual
  void
  error_exit();


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスが利用することのできる関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief メッセージマネージャの取得
  MsgMgr&
  msg_mgr();
  
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

  // パーサー本体
  Iscas89ParserImpl* mParser;

};

END_NAMESPACE_YM_ISCAS89

#endif // YM_ISCAS89_ISCAS89HANDLER_H
