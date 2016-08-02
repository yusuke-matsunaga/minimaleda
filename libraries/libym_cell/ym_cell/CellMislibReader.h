#ifndef YM_CELL_CELLMISLIBREADER_H
#define YM_CELL_CELLMISLIBREADER_H

/// @file ym_cell/CellMislibReader.h
/// @brief CellMislibReader のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CellMislibReader.h 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/cell_type.h"
#include "ym_utils/MsgHandler.h"


BEGIN_NAMESPACE_YM_CELL

class MislibParser;

//////////////////////////////////////////////////////////////////////
/// @class CellMislibReader CellMislibReader.h <ym_cell/CellMislibReader.h>
/// @brief mislib 形式のファイルを読み込んで CellLibrary に設定するクラス
/// @sa CellLibrary Cell
//////////////////////////////////////////////////////////////////////
class CellMislibReader
{
public:

  /// @brief コンストラクタ
  CellMislibReader();

  /// @brief デストラクタ
  ~CellMislibReader();


public:

  /// @brief mislib 形式のファイルを読み込んでライブラリを生成する．
  /// @param[in] filename ファイル名
  /// @return 読み込んで作成したセルライブラリを返す．
  /// @note エラーが起きたら NULL を返す．
  const CellLibrary*
  read(const string& filename);

  /// @brief メッセージハンドラを付加する．
  void
  add_msg_handler(MsgHandler* msg_handler);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // mislib のパーサー
  MislibParser* mParser;

};

END_NAMESPACE_YM_CELL

#endif // YM_CELL_CELLMISLIBREADER_H
