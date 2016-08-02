#ifndef YM_GDS_GDSSCANNER_H
#define YM_GDS_GDSSCANNER_H

/// @file ym_gds/GdsScanner.h
/// @brief GDS-II ファイルの読み込みを行うクラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: GdsScanner.h 1343 2008-03-25 17:15:35Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#include <ym_gds/gds_nsdef.h>


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
/// @class GdsScanner GdsScanner.h "GdsScanner.h"
/// @brief GDS-II の読み込みを行うクラス
//////////////////////////////////////////////////////////////////////
class GdsScanner
{
public:

  /// @brief コンストラクタ
  /// @param[in] is 入力ストリーム
  /// @param[in] mgr GdsRecord 管理用のオブジェクト
  GdsScanner(std::istream& is,
	     GdsRecMgr& mgr);

  /// @brief デストラクタ
  ~GdsScanner();


public:

  /// @brief レコード一つ分の読み込みを行う．
  /// @retval record 読み込んだレコード
  /// @retval NULL エラーが起った場合や末尾に達した場合
  GdsRecord*
  read_rec();

  /// @brief 現在の位置を返す．
  size_t
  cur_pos() const;


private:

  // ストリームから1バイト読んで符号なし整数に変換する．
  ymuint
  read_1byte_uint();

  // ストリームから2バイト読んで符号なし整数に変換する．
  ymuint
  read_2byte_uint();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力ストリーム
  std::istream& mIs;

  // 入力ストリームから読み込んだバイト数
  size_t mPos;

  // GdsRecord の管理用オブジェクト
  GdsRecMgr& mMgr;
  
};

END_NAMESPACE_YM_GDS

#endif
