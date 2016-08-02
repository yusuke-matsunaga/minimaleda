#ifndef LIBYM_TGNET_TGISCAS89HANDLER_H
#define LIBYM_TGNET_TGISCAS89HANDLER_H

/// @file libym_tgnet/TgIscas89Handler.h
/// @brief TgIscas89Handler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TgIscas89Handler.h 2043 2009-02-21 07:08:48Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_tgnet/tgnet_nsdef.h"
#include "ym_iscas89/Iscas89Handler.h"


BEGIN_NAMESPACE_YM_TGNET

//////////////////////////////////////////////////////////////////////
/// @class TgIscas89Handler TgIscas89Handler.h
/// @brief TgNetwork 用の Iscas89Handler
//////////////////////////////////////////////////////////////////////
class TgIscas89Handler :
  public Iscas89Handler
{
public:

  /// @brief コンストラクタ
  TgIscas89Handler();

  /// @brief デストラクタ
  virtual
  ~TgIscas89Handler();


public:

  /// @brief ネットワークをセットする．
  void
  set_network(TgNetwork& network);

  /// @brief 初期化
  virtual
  bool
  init();

  /// @brief INPUT 文を読み込む．
  /// @param[in] loc ファイル位置
  /// @param[in] name_id 入力ピン名のID番号
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  read_input(const FileRegion& loc,
	     ymuint32 name_id);

  /// @brief OUTPUT 文を読み込む．
  /// @param[in] loc ファイル位置
  /// @param[in] name_id 出力ピン名のID番号
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  read_output(const FileRegion& loc,
	      ymuint32 name_id);

  /// @brief ゲート文を読み込む．
  /// @param[in] loc ファイル位置
  /// @param[in] type ゲートの型
  /// @param[in] oname_id 出力名のID番号
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


private:

  // 可能なら接続する．
  // できれなれば mConList に積む
  void
  connect(TgNode* from,
	  TgNode* to,
	  size_t pos);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  struct Connection
  {
    TgNode* mFrom;
    TgNode* mTo;
    size_t mPos;

    Connection(TgNode* from,
	       TgNode* to,
	       size_t pos) :
      mFrom(from),
      mTo(to),
      mPos(pos) { }
  };

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象の TgNetwork
  TgNetwork* mNetwork;

  // 後で接続する要素のリスト
  vector<Connection> mConList;
  
};

END_NAMESPACE_YM_TGNET

#endif // LIBYM_TGNET_TGISCAS89HANDLER_H
