#ifndef LIBYM_BNETBLIFREADER_BNETBLIFHANDLER_H
#define LIBYM_BNETBLIFREADER_BNETBLIFHANDLER_H

/// @file libym_bnetblifreader/BNetBlifHandler.h
/// @brief BNetBlifHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BNetBlifReader.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_bnet/bnet_nsdef.h"
#include "ym_blif/BlifHandler.h"


BEGIN_NAMESPACE_YM_BNET

//////////////////////////////////////////////////////////////////////
/// @class BNetBlifHandler read_blif.cc
/// @brief BNetwork 用の BlifHandler
//////////////////////////////////////////////////////////////////////
class BNetBlifHandler :
  public BlifHandler
{
public:

  /// @brief コンストラクタ
  BNetBlifHandler();

  /// @brief デストラクタ
  virtual
  ~BNetBlifHandler();


public:

  /// @brief 読み込む対象のネットワークを設定する．
  void
  set_network(BNetwork* network);

  /// @brief 初期化
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  init();
  
  /// @brief .model 文の処理
  /// @param[in] loc1 .model の位置情報
  /// @param[in] loc2 文字列の位置情報
  /// @param[in] name 文字列
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  model(const FileRegion& loc1,
	const FileRegion& loc2,
	const char* name);

  /// @brief .inputs 文中の文字列の処理
  /// @param[in] name_id 文字列のID番号
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  inputs_elem(ymuint32 name_id);

  /// @brief .outputs 文中の文字列の処理
  /// @param[in] name_id 文字列のID番号
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  outputs_elem(ymuint32 name_id);

  /// @brief .names 文の処理
  /// @param[in] name_id_array 各識別子のID番号の配列
  /// @param[in] nc キューブ数
  /// @param[in] cover_pat 入力カバーを表す文字列
  /// @param[in] opat 出力の極性
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  /// @note cover_pat は ni 個ごとに1行のパタンを表す．
  /// 各要素のとりうる値は '0', '1', '-' を表す．
  /// @note opat は '0' か '1' のどちらか
  virtual
  bool
  names(const vector<ymuint32>& name_id_array,
	ymuint32 nc,
	const char* cover_pat,
	char opat);

  /// @brief .gate 文の開始
  /// @param[in] loc1 .gate の位置情報
  /// @param[in] loc2 セル名の位置情報
  /// @param[in] name セル名
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  gate_begin(const FileRegion& loc1,
	     const FileRegion& loc2,
	     const char* name);

  /// @brief .gate 文中のピン割り当ての処理
  /// @param[in] loc1 ピン名の位置情報
  /// @param[in] f_name ピン名
  /// @param[in] loc2 ノード名の位置情報
  /// @param[in] a_name ノード名
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  gate_assign(const FileRegion& loc1,
	      const char* f_name,
	      const FileRegion& loc2,
	      const char* a_name);

  /// @brief .gate 文の終了
  virtual
  bool
  gate_end();

  /// @brief .latch 文の処理
  /// @param[in] name1_id 最初の識別子のID番号
  /// @param[in] name2_id 次の識別子のID番号
  /// @param[in] loc4 リセット値の位置情報
  /// @param[in] rval リセット時の値('0'/'1') 未定義なら ' '
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  latch(ymuint32 name1_id,
	ymuint32 name2_id,
	const FileRegion& loc4,
	char rval);

  /// @brief .end 文の処理
  /// @param[in] loc 位置情報
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  end(const FileRegion& loc);

  /// @brief 通常終了時の処理
  virtual
  void
  normal_exit();

  /// @brief エラー終了時の処理
  virtual
  void
  error_exit();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードの取得 or 生成
  /// @param[in] name_id 名前ID
  BNode*
  get_node(ymuint32 name_id);

  /// @brief mNodeArray の拡大
  void
  resize(ymuint32 id);

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象の BNetwork
  BNetwork* mNetwork;

  // ネットワークを操作するクラス
  BNetManip* mManip;

  // ID番号をキーにした BNode を納めた配列
  vector<BNode*> mNodeArray;
  
};

END_NAMESPACE_YM_BNET

#endif // LIBYM_BNETBLIFREADER_BNETBLIFHANDLER_H
