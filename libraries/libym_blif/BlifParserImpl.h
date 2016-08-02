#ifndef LIBYM_BLIF_BLIFPARSERIMPL_H
#define LIBYM_BLIF_BLIFPARSERIMPL_H

/// @file libym_blif/BlifParserImpl.h
/// @brief BlifParserImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BlifParserImpl.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_blif/blif_nsdef.h"
#include "ym_utils/MsgHandler.h"
#include "BlifScanner.h"
#include "IdHash.h"


BEGIN_NAMESPACE_YM_BLIF

//////////////////////////////////////////////////////////////////////
/// @class BlifParserImpl BlifParserImpl.h "BlifParserImpl.h"
/// @brief BlifParser の実装クラス
//////////////////////////////////////////////////////////////////////
class BlifParserImpl
{
public:

  /// @brief コンストラクタ
  BlifParserImpl();
  
  /// @brief デストラクタ
  ~BlifParserImpl();


public:

  /// @brief 読み込みを行う．
  /// @param[in] filename ファイル名
  /// @retval true 読み込みが成功した．
  /// @retval false 読み込みが失敗した．
  bool
  read(const string& filename);

  /// @brief イベントハンドラの登録
  void
  add_handler(BlifHandler* handler);

  
public:
  
  /// @brief メッセージマネージャの取得
  MsgMgr&
  msg_mgr();

  /// @brief ID番号から文字列を得る．
  const char*
  id2str(ymuint32 id);

  /// @brief ID番号からそれに関連した位置情報を得る．
  const FileRegion&
  id2loc(ymuint32 id);

  /// @brief ID番号からそれに関連した位置情報を得る．
  const FileRegion&
  id2def_loc(ymuint32 id);
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 字句解析器
  BlifScanner mScanner;

  // イベントハンドラのリスト
  list<BlifHandler*> mHandlerList;
  
  // メッセージハンドラの管理者
  MsgMgr mMsgMgr;

  // 識別子のハッシュ表
  IdHash mIdHash;
  
  // 位置情報の配列
  vector<FileRegion> mLocArray;
  
  // IdCellの配列
  vector<IdCell*> mNameArray;

  // ID 番号の配列
  vector<ymuint32> mIdArray;

  // キューブ数
  ymuint32 mNc;
  
  // パタンのバッファ
  StrBuff mCoverPat;

  // 出力の極性
  char mOpat;
  
  // 位置情報バッファ
  FileRegion mLoc1;
  
  // 文字列バッファ1
  StrBuff mName1;

  // 文字列バッファ2
  StrBuff mName2;
  
};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////
  
// @brief メッセージマネージャの取得
inline
MsgMgr&
BlifParserImpl::msg_mgr()
{
  return mMsgMgr;
}

// @brief ID番号から文字列を得る．
inline
const char*
BlifParserImpl::id2str(ymuint32 id)
{
  return mIdHash.str(id);
}

// @brief ID番号からそれに関連した位置情報を得る．
inline
const FileRegion&
BlifParserImpl::id2loc(ymuint32 id)
{
  return mIdHash.loc(id);
}

// @brief ID番号からそれに関連した位置情報を得る．
inline
const FileRegion&
BlifParserImpl::id2def_loc(ymuint32 id)
{
  return mIdHash.def_loc(id);
}

END_NAMESPACE_YM_BLIF

#endif // LIBYM_BLIF_BLIFPARSERIMPL_H
