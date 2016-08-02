#ifndef LIBYM_VERILOG_ELB_ELBUDP_H
#define LIBYM_VERILOG_ELB_ELBUDP_H

/// @file libym_verilog/elaborator/include/ElbUdp.h
/// @brief ElbUdp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ElbUdp.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/vl/VlUdp.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbUdpDefn ElbUdp.h <ym_verilog/vl/VlUdp.h>
/// @brief UDP定義を表すクラス
/// IEEE Std 1364-2001 26.6.14 UDP
//////////////////////////////////////////////////////////////////////
class ElbUdpDefn :
  public VlUdpDefn
{
public:
  
  /// @brief コンストラクタ
  ElbUdpDefn() { }
  
  /// @brief デストラクタ
  virtual
  ~ElbUdpDefn() { }


public:
  //////////////////////////////////////////////////////////////////////
  // ElbUdpDefn の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入出力オブジェクトの内容を設定する．
  /// @param[in] pos ポート中の位置
  /// @param[in] file_region ソースファイル上の位置
  /// @param[in] name 名前
  /// @param[in] dir 向き
  virtual
  void
  set_io(ymuint32 pos,
	 const PtIOHead* pt_header,
	 const PtIOItem* pt_item) = 0;

  /// @brief 初期値を設定する．
  /// @param[in] init_expr 初期値を表す式
  /// @param[in] init_val 初期値
  virtual
  void
  set_initial(ElbExpr* init_expr,
	      tVpiScalarVal init_val) = 0;

  /// @brief table entry の内容を設定する．
  /// @param[in] pos 行番号
  /// @param[in] pt_udp_entry パース木の一行分の定義
  /// @param[in] vals シンボル値の配列
  virtual
  void
  set_tableentry(ymuint32 pos,
		 const PtUdpEntry* pt_udp_entry,
		 const vector<tVpiUdpVal>& vals) = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class ElbTableEntry ElbUdp.h <ym_verilog/vl/VlUdp.h>
/// @brief UDP の table entry を表すクラス
//////////////////////////////////////////////////////////////////////
class ElbTableEntry :
  public VlTableEntry
{
public:
  
  /// @brief コンストラクタ
  ElbTableEntry() { }
  
  /// @brief デストラクタ
  ~ElbTableEntry() { }
  

public:
  //////////////////////////////////////////////////////////////////////
  // ElbTableEntry の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 設定する．
  virtual
  void
  set(const PtUdpEntry* pt_entry,
      const vector<tVpiUdpVal>& vals) = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELB_ELBUDP_H
