#ifndef LIBYM_AIG_LRMGR_H
#define LIBYM_AIG_LRMGR_H

/// @file libym_aig/LrMgr.h
/// @brief LrMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: LrMgr.h 1594 2008-07-18 10:26:12Z matsunaga $
///
/// Copyright (C) 2005-2007 Yusuke Matsunaga
/// All rights reserved.


#include "AigTemplate.h"


BEGIN_NAMESPACE_YM_AIG

class AigMgr;

//////////////////////////////////////////////////////////////////////
// local rewrite を行うクラス
//////////////////////////////////////////////////////////////////////
class LrMgr
{
public:

  /// @brief コンストラクタ
  LrMgr();

  /// @brief デストラクタ
  ~LrMgr();
  

public:

  /// @brief local rewrite を行う．
  /// @param[in] aig_mgr 対象のネットワーク
  void
  local_rewrite(AigMgr& aig_mgr);
  
  /// @brief ログレベルを設定する．
  void
  set_loglevel(int level);
  
  /// @brief ログ出力用ストリームを設定する．
  void
  set_logstream(ostream* out);
  
  
private:
  
  // 与えられた関数に対する AigTemplate を求める．
  bool
  find_aig(size_t ni,
	   ymuint32 pat,
	   AigTemplate& templ);

  /// @brief 置き換えテーブルを初期化する．
  void
  init_table();

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 2入力関数用のテンプレート
  // 全ての関数に対して用意する．
  AigTemplate aig2table[16];

  // 3入力関数用のテンプレート
  // 全ての関数に対して用意する．
  AigTemplate aig3table[256];

  // 4入力関数用のテンプレート
  // NPN同値類の代表関数に対してのみ用意する．
  AigTemplate aig4table[222];

  // NPN同値類の代表関数のパタンから aig4table の番号を得るためのハッシュ表
  hash_map<ymuint32, size_t> npn4map;
  
  // ログレベル
  int mLogLevel;

  // ログ出力用のストリーム
  ostream* mLogStream;
  
};

END_NAMESPACE_YM_AIG

#endif // LIBYM_AIG_LRMGR_H
