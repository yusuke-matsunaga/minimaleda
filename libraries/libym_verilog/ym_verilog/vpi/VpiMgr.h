#ifndef YM_VERILOG_VPI_VPIMGR_H
#define YM_VERILOG_VPI_VPIMGR_H

/// @file ym_verilog/vpi/VpiMgr.h
/// @brief VpiMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VpiMgr.h 1343 2008-03-25 17:15:35Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#include <ym_verilog/verilog.h>
#include <ym_utils/ShString.h>


BEGIN_NAMESPACE_YM_VERILOG

class VpiUdpDefn;
class VpiModule;
class VpiUserSystf;

//////////////////////////////////////////////////////////////////////
/// @class VpiMgr VpiMgr.h <ym_verilog/vpi/VpiMgr.h>
/// @brief Verilog-HDL のシミュレーションモデルを生成するクラス
//////////////////////////////////////////////////////////////////////
class VpiMgr
{
public:

  /// @brief デストラクタ
  virtual
  ~VpiMgr() { }


public:

  /// @brief クリアする．
  virtual
  void
  clear() = 0;

  /// @brief トップモジュールを返す．
  /// @return トップモジュールが1つしかなければそれを返す．
  /// @return 複数あったら NULL を返す．
  virtual
  VpiModule*
  top_module() const = 0;

  /// @brief トップモジュールのリストを返す．
  virtual
  const list<VpiModule*>&
  top_module_list() const = 0;

  /// @brief UDP定義のリストを返す．
  virtual
  const list<VpiUdpDefn*>&
  udp_list() const = 0;

  /// @brief 名前からトップモジュールを取出す．
  /// @param[in] name 名前
  /// @return name という名前のトップもジュールを返す．
  /// @return なければ NULL を返す．
  virtual
  VpiModule*
  top_module(const ShString& name) const = 0;
  
  /// @brief 名前から UDP 定義を取出す．
  /// @param[in] name 名前
  /// @return name という名の UDP を返す．
  /// @return なければ NULL を返す．
  virtual
  VpiUdpDefn*
  find_udp(const ShString& name) const = 0;

  /// @brief 名前から UserSystf を取出す．
  /// @param[in] name 名前
  /// @return name という名のユーザー定義関数を返す．
  /// @return なければ NULL を返す．
  virtual
  VpiUserSystf*
  find_user_systf(const ShString& name) const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // 要素を登録する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief UDP を登録する．
  /// @param[in] name 名前
  /// @param[in] udp 登録する UDP
  virtual
  void
  reg_udp(const ShString& name,
	  VpiUdpDefn* udp) = 0;

  /// @brief トップモジュールを登録する．
  /// @param[in] name 名前
  /// @param[in] module 登録するトップモジュール
  virtual
  void
  reg_topmodule(const ShString& name,
		VpiModule* module) = 0;


};

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_VPI_VPIMGR_H
