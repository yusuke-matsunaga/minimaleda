#ifndef LIBYM_MISLIB_MISLIBPARSERIMPL_H
#define LIBYM_MISLIB_MISLIBPARSERIMPL_H

/// @file libym_mislib/MislibParserImpl.h
/// @brief MislibParserImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MislibParserImpl.h 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include <ym_mislib/mislib_nsdef.h>
#include <ym_utils/MsgHandler.h>
#include <ym_utils/FileRegion.h>
#include <ym_utils/Alloc.h>
#include <ym_utils/ShString.h>
#include <ym_lexp/LogExpr.h>

#include "MislibLex.h"


BEGIN_NAMESPACE_YM_MISLIB

class MislibPt;

//////////////////////////////////////////////////////////////////////
/// @class MislibParserImpl MislibParserImpl.h "MislibParserImpl.h"
/// @brief Mislib ファイルのパーサーの実装クラス
//////////////////////////////////////////////////////////////////////
class MislibParserImpl
{
public:

  /// @brief コンストラクタ
  MislibParserImpl();

  /// @brief デストラクタ
  ~MislibParserImpl();
  

public:
  
  /// @brief mislib ファイルを読み込む
  /// @param[in] filename ファイル名
  /// @retval true 読み込みが成功した．
  /// @retval false 読み込みが失敗した．
  bool
  read(const string& filename);

  /// @brief イベントハンドラの登録
  /// @param[in] handler 登録するハンドラ
  /// @note handler はこのインスタンスが破壊される時に同時に破壊される．
  void
  add_handler(MislibHandler* handler);
  
  /// @brief メッセージマネージャの取得
  MsgMgr&
  msg_mgr();

  
public:
  //////////////////////////////////////////////////////////////////////
  // mislib_grammer.yy で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief NOT ノードを生成する．
  MislibPt*
  new_not(const FileRegion& loc,
	  MislibPt* child1);
  
  /// @brief AND ノードを生成する．
  MislibPt*
  new_and(const FileRegion& loc,
	  MislibPt* child1,
	  MislibPt* child2);

  /// @brief OR ノードを生成する．
  MislibPt*
  new_or(const FileRegion& loc,
	 MislibPt* child1,
	 MislibPt* child2);

  /// @brief PIN ノードを生成する．
  MislibPt*
  new_pin(const FileRegion& loc,
	  MislibPt* name,
	  MislibPt* phase,
	  MislibPt* input_load,
	  MislibPt* max_load,
	  MislibPt* rise_block_delay,
	  MislibPt* rise_fanout_delay,
	  MislibPt* fall_block_delay,
	  MislibPt* fall_fanout_delay);

  /// @brief PIN リストノードを生成する．
  MislibPt*
  new_pinlist();

  /// @brief GATE ノードを生成する．(通常版)
  /// @note 下記のエラーチェックを行う．
  ///  - 重複したゲート名
  ///  - ipin_list 中の入力ピン名が重複している．
  ///  - ipin_list 中の入力ピン名と出力ピン名が重複している．
  ///  - ipin_list の要素数と expr の入力数が合わない．
  void
  new_gate1(const FileRegion& loc,
	    MislibPt* name,
	    MislibPt* area,
	    MislibPt* oname,
	    MislibPt* expr,
	    MislibPt* ipin_list);

  /// @brief GATE ノードを生成する．(ワイルドカードの入力ピン)
  /// @note 下記のエラーチェックを行う．
  ///  - 重複したゲート名
  void
  new_gate2(const FileRegion& loc,
	    MislibPt* name,
	    MislibPt* area,
	    MislibPt* oname,
	    MislibPt* expr,
	    MislibPt* ipin);

  /// @brief GATE ノードを生成する．(入力ピンなし)
  /// @note 下記のエラーチェックを行う．
  ///  - 重複したゲート名
  ///  - expr が定数式でない．
  void
  new_gate3(const FileRegion& loc,
	    MislibPt* name,
	    MislibPt* area,
	    MislibPt* oname,
	    MislibPt* expr);
  
  /// @brief 字句解析を行う．
  /// @param[out] lval トークンの値を格納する変数
  /// @param[out] lloc トークンの位置を格納する変数
  /// @return トークンの型を返す．
  int
  scan(MislibPt*& lval,
       FileRegion& lloc);
  
  /// @brief 今までに生成したすべてのオブジェクトを解放する．
  void
  clear();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部でのみ使われるメンバ関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 文字列ノードを生成する．
  MislibPt*
  new_str(const FileRegion& loc,
	  ShString str);
  
  /// @brief 数値ノードを生成する．
  MislibPt*
  new_num(const FileRegion& loc,
	  double num);

  /// @brief NONINV ノードを生成する．
  MislibPt*
  new_noninv(const FileRegion& loc);

  /// @brief INV ノードを生成する．
  MislibPt*
  new_inv(const FileRegion& loc);

  /// @brief UNKNOWN ノードを生成する．
  MislibPt*
  new_unknown(const FileRegion& loc);

  /// @brief 定数0ノードを生成する．
  MislibPt*
  new_const0(const FileRegion& loc);

  /// @brief 定数1ノードを生成する．
  MislibPt*
  new_const1(const FileRegion& loc);
  
  /// @brief ゲート名の登録
  /// @return 重複していたら true を返す．
  bool
  reg_gate_name(const ShString& name,
		const FileRegion& name_loc);
  
  /// @brief MislibPt を MislibPin に変換する．
  void
  pt_to_pin(MislibPt* pt_pin,
	    MislibPin& pin);
  
  /// @brief 論理式をチェックする．
  /// @param[in] pt 論理式の根のノード
  /// @param[in] inputs 入力ピンの配列
  /// @param[out] expr pt に対応する論理式
  /// @param[out] used 各入力ピンが使われていたら true を入れる．
  /// @return エラーがあったら true を返す．
  bool
  check_expr(MislibPt* pt,
	     const vector<MislibPin>& inputs,
	     LogExpr& expr,
	     vector<bool>& used);
  
  /// @brief MislibPt を論理式に変換する．
  /// @param[in] pt 論理式の根のノード
  /// @param[out] names 論理式中に現れた名前を入れる配列
  /// @return 論理式を返す．
  LogExpr
  make_expr(MislibPt* pt,
	    vector<MislibPt*>& names);
  
  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // MislibPt のメモリ確保用アロケータ
  SimpleAlloc mAlloc;

  // 字句解析器
  MislibLex mLex;
  
  // イベントハンドラのリスト
  list<MislibHandler*> mHandlerList;

  // メッセージハンドラの管理者
  MsgMgr mMsgMgr;
  
  // ゲート名をキーにしてその定義位置を保持するハッシュ表
  hash_map<ShString, FileRegion> mGateHash;
  
  // 読み込み時のエラーの有無を示すフラグ
  bool mError;
  
};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief メッセージマネージャの取得
inline
MsgMgr&
MislibParserImpl::msg_mgr()
{
  return mMsgMgr;
}

END_NAMESPACE_YM_MISLIB

#endif // LIBYM_MISLIB_MISLIBPARSERIMPL_H
