#ifndef LIBYM_CELL_MISLIB_MISLIBPARSER_H
#define LIBYM_CELL_MISLIB_MISLIBPARSER_H

/// @file libym_cell/mislib/MislibParser.h
/// @brief MislibParser のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MislibParser.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/cell_nsdef.h"
#include "ym_utils/MsgHandler.h"
#include "ym_utils/FileRegion.h"
#include "ym_utils/Alloc.h"
#include "ym_utils/ShString.h"

#include "MislibPt.h"
#include "MislibLex.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class MislibParser MislibParser.h "MislibParser.h"
/// @brief Mislib ファイルのパーサーの実装クラス
//////////////////////////////////////////////////////////////////////
class MislibParser
{
public:

  /// @brief コンストラクタ
  MislibParser();

  /// @brief デストラクタ
  ~MislibParser();


public:

  /// @brief mislib ファイルを読み込んでライブラリを生成する．
  /// @param[in] filename ファイル名
  /// @return 生成したライブラリを返す．
  /// @note 読み込みが失敗したら NULL を返す．
  const CellLibrary*
  read(const string& filename);

  /// @brief メッセージマネージャの取得
  MsgMgr&
  msg_mgr();


public:
  //////////////////////////////////////////////////////////////////////
  // mislib_grammer.yy で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 今までに生成したすべてのオブジェクトを解放する．
  void
  clear();

  /// @brief リストノードを生成する．
  MislibPt*
  new_list();

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

  /// @brief GATE ノードを生成する．(通常版)
  void
  new_gate1(const FileRegion& loc,
	    MislibPt* name,
	    MislibPt* area,
	    MislibPt* oname,
	    MislibPt* expr,
	    MislibPt* ipin_list);

  /// @brief GATE ノードを生成する．(ワイルドカードの入力ピン)
  void
  new_gate2(const FileRegion& loc,
	    MislibPt* name,
	    MislibPt* area,
	    MislibPt* oname,
	    MislibPt* expr,
	    MislibPt* ipin);

  /// @brief GATE ノードを生成する．(入力ピンなし:定数ノード)
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

  /// @brief エラーメッセージを出力する．
  /// @note 副作用で mError が true にセットされる．
  void
  error(const FileRegion& loc,
	const char* msg);


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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // MislibPt のメモリ確保用アロケータ
  SimpleAlloc mAlloc;

  // 字句解析器
  MislibLex mLex;

  // メッセージハンドラの管理者
  MsgMgr mMsgMgr;

  // ゲートのリスト
  MislibPt* mGateList;

  // 読み込み時のエラーの有無を示すフラグ
  bool mError;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief メッセージマネージャの取得
inline
MsgMgr&
MislibParser::msg_mgr()
{
  return mMsgMgr;
}

END_NAMESPACE_YM_CELL

#endif // LIBYM_CELL_MISLIB_MISLIBPARSER_H
