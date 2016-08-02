#ifndef YM_VERILOG_VL_VLDUMPER_H
#define YM_VERILOG_VL_VLDUMPER_H

/// @file ym_verilog/vl/VlDumper.h
/// @brief VlDumper のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlDumper.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#include <ym_verilog/verilog.h>


BEGIN_NAMESPACE_YM_VERILOG

class VlMgr;
class VlDumperImpl;

//////////////////////////////////////////////////////////////////////
/// @class VlDumper VlDumper.h <ym_verilog/vl/VlDumper.h>
/// @ingroup Vlgroup
/// @brief VlObj の出力用クラス
//////////////////////////////////////////////////////////////////////
class VlDumper
{
public:

  /// @brief コンストラクタ
  /// @param[in] s 出力ストリーム
  VlDumper(ostream& s);
  
  /// @brief デストラクタ
  ~VlDumper();
  

public:
  //////////////////////////////////////////////////////////////////////
  // 内容を出力する関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief VlMgr の持っている内容を出力する
  /// @param[in] vlmgr VlMgr
  void
  operator()(const VlMgr& vlmgr);
  

public:
  //////////////////////////////////////////////////////////////////////
  // オプションの設定/取得用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置情報を出力するモードにする
  void
  enable_file_loc_mode();

  /// @brief ファイル位置情報の出力を抑制するモードにする．
  void
  disable_file_loc_mode();

  /// @brief ファイル位置情報を出力するモードのとき true を返す．
  bool
  file_loc_mode() const;

  /// @brief NULL ポインタの出力を抑止する．
  void
  enable_nullptr_suppress_mode();

  /// @brief NULL ポインタの出力を行う．
  void
  disable_nullptr_suppress_mode();

  /// @brief NULL ポインタの出力抑止モードの状態を得る．
  bool
  nullptr_suppress_mode() const;

  /// @brief bit 展開した内容を出力する．
  void
  enable_bitexpand_mode();

  /// @brief bit 展開しない．
  void
  disable_bitexpand_mode();

  /// @brief bit 展開モードの時 true を返す．
  bool
  bitexpand_mode() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 実際の実装クラス
  VlDumperImpl* mImpl;

};


END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_VL_VLDUMPER_H
