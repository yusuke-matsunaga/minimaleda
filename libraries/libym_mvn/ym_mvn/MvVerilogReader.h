#ifndef YM_MVN_MVVERILOGREADER_H
#define YM_MVN_MVVERILOGREADER_H

/// @file ym_mvn/MvVerilogReader.h
/// @brief MvVerilogReader のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/mvn_nsdef.h"
#include "ym_verilog/verilog.h"
#include "ym_verilog/vl/VlFwd.h"
#include "ym_utils/File.h"
#include "ym_utils/MsgHandler.h"


BEGIN_NAMESPACE_YM_MVN_VERILOG

class ReaderImpl;

END_NAMESPACE_YM_MVN_VERILOG


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvVerilogReader MvVerilogReader.h "ym_mvn/MvVerilogReader.h"
/// @brief 合成可能な Verilog 記述を読み込んで MVN に設定するクラス
//////////////////////////////////////////////////////////////////////
class MvVerilogReader
{
public:

  /// @brief コンストラクタ
  MvVerilogReader();

  /// @brief デストラクタ
  ~MvVerilogReader();


public:

  /// @brief 内部のデータをクリアする．
  void
  clear();

  /// @brief verilog 形式のファイルを読み込む．
  /// @param[in] filename ファイル名
  /// @param[in] searchpath サーチパス
  /// @param[in] watcher_list 行番号ウォッチャーのリスト
  /// @retval true 正常に読み込めた．
  /// @retval false 読込中にエラーが起こった．
  bool
  read(const string& filename,
       const SearchPathList& searchpath = SearchPathList(),
       const list<VlLineWatcher*> watcher_list = list<VlLineWatcher*>());

  /// @brief 今まで読み込んだ情報からネットワークを生成する．
  /// @param[in] mgr ネットワーク生成用のマネージャ
  /// @param[out] node_map MvNode のID番号をキーにして対応する宣言要素を保持する配列
  /// @retval true 正常に処理を行った．
  /// @retval false 生成中にエラーが起こった．
  bool
  gen_network(MvMgr& mgr,
	      vector<pair<const VlDecl*, ymuint> >& node_map);

  /// @brief メッセージハンドラを付加する．
  /// @param[in] msg_handler 登録するハンドラ
  /// @note このハンドラはこのオブジェクトの破壊に伴って破壊される．
  void
  add_msg_handler(MsgHandler* msg_handler);

  /// @brief フリップフロップのセル名，ピン名を設定する．
  /// @param[in] cell_name セル名
  /// @param[in] data_pin_name データ入力ピン名
  /// @param[in] clock_pin_name クロック入力ピン名
  /// @param[in] q_pin_name ノーマル出力ピン名
  /// @param[in] qn_pin_name 反転出力ピン名
  /// @param[in] set_pin_name セットピン名
  /// @param[in] reset_pin_name リセットピン名
  /// @note 存在しない場合には空文字列を渡す．
  void
  set_ffname(const string& cell_name,
	     const string& data_pin_name,
	     const string& clock_pin_name,
	     const string& q_pin_name,
	     const string& qn_pin_name,
	     const string& set_pin_name,
	     const string& reset_pin_name);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実際の読み込みを行うオブジェクト
  nsVerilog::ReaderImpl* mImpl;

};

END_NAMESPACE_YM_MVN

#endif // YM_MVN_MVVERILOGREADER_H
