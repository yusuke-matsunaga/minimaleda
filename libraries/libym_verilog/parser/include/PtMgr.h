#ifndef YM_VERILOG_PTMGR_H
#define YM_VERILOG_PTMGR_H

/// @file ym_verilog/PtMgr.h
/// @brief PtMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: PtMgr.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/pt/PtP.h"
#include "ym_utils/FileDescMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtMgr PtMgr.h <ym_verilog/PtMgr.h>
/// @ingroup PtMgr
/// @brief Verilog のパース木を管理するクラス
//////////////////////////////////////////////////////////////////////
class PtMgr
{
  friend class Parser;
  
public:
  
  /// @brief コンストラクタ
  PtMgr();
  
  /// @brief デストラクタ
  ~PtMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得する関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 今までに生成したインスタンスをすべて破壊する．
  void
  clear();

  /// @brief 登録されているモジュールのリストを返す．
  /// @return 登録されているモジュールのリスト
  const list<const PtModule*>&
  pt_module_list() const;

  /// @brief 登録されている UDP のリストを返す．
  /// @return 登録されている UDP のリスト
  const list<const PtUdp*>&
  pt_udp_list() const;
  
  /// @brief インスタンス記述で用いられている名前かどうか調べる．
  /// @param[in] name 調べる名前
  /// @return 用いられていたら true を返す．
  bool
  check_def_name(const char* name) const;


public:
  //////////////////////////////////////////////////////////////////////
  // データをセットする関数
  //////////////////////////////////////////////////////////////////////

  /// @brief UDP 定義を追加する．
  void
  reg_udp(const PtUdp* udp);

  /// @brief モジュール定義を追加する．
  void
  reg_module(const PtModule* module);

  /// @brief インスタンス定義名を追加する．
  void
  reg_defname(const char* name);

  
private:
  ////////////////////////////////////////////////////////////////////// 
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // ファイル記述子を管理するクラス
  FileDescMgr mFdMgr;

  // UDP 定義のリスト
  list<const PtUdp*> mUdpList;

  // モジュール定義のリスト
  list<const PtModule*> mModuleList;
  
  // インスタンス記述で用いられている名前
  // たぶんモジュール名か UDP名のはず
  hash_set<string> mDefNames;
  
};

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_PTMGR_H
