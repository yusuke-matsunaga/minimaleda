#ifndef YM_VERILOG_PT_PTUDP_H
#define YM_VERILOG_PT_PTUDP_H

/// @file ym_verilog/pt/PtUdp.h
/// @brief PtUdp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: PtUdp.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/pt/PtBase.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtUdp PtUdp.h <ym_verilog/pt/PtUdp.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief UDP を表すノード
//////////////////////////////////////////////////////////////////////
class PtUdp :
  public PtNamedBase
{
public:

  /// @brief 仮想デストラクタ
  virtual
  ~PtUdp() { }


public:
  //////////////////////////////////////////////////////////////////////
  // PtUdp の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief primitive type を返す．
  virtual
  tVpiPrimType
  prim_type() const = 0;

  /// @brief ポート数を取り出す．
  virtual
  ymuint32
  port_num() const = 0;

  /// @brief ポートを取り出す．
  /// @param[in] pos 位置番号 ( 0 <= pos < port_num() )
  virtual
  const PtPort*
  port(ymuint32 pos) const = 0;

  /// @brief 入出力宣言ヘッダ配列の取得
  virtual
  PtIOHeadArray
  iohead_array() const = 0;

  /// @brief 初期値を取出す．
  virtual
  const PtExpr*
  init_value() const = 0;

  /// @brief テーブルを取り出す．
  virtual
  PtUdpEntryArray
  table_array() const = 0;
  
};


//////////////////////////////////////////////////////////////////////
/// @class PtUdpEntry PtUdp.h <ym_verilog/pt/PtUdp.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief UDP の中身のテーブルを表すクラス
//////////////////////////////////////////////////////////////////////
class PtUdpEntry :
  public PtBase
{
public:

  /// @brief 仮想デストラクタ
  virtual
  ~PtUdpEntry() { }


public:
  //////////////////////////////////////////////////////////////////////
  // PtUdpEntry の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力値の配列を取り出す．
  virtual
  PtUdpValueArray
  input_array() const = 0;

  /// @brief 現状態の値を取り出す．
  virtual
  const PtUdpValue*
  current() const = 0;

  /// @brief 出力の値を取り出す．
  virtual
  const PtUdpValue*
  output() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class PtUdpValue PtUdp.h <ym_verilog/pt/PtUdp.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief UDP のテーブルの値を表すクラス
//////////////////////////////////////////////////////////////////////
class PtUdpValue :
  public PtBase
{
public:

  /// @brief 仮想デストラクタ
  virtual
  ~PtUdpValue() { }


public:
  //////////////////////////////////////////////////////////////////////
  // PtUdpValue の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief シンボルを取り出す．
  virtual
  tVpiUdpVal
  symbol() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_PT_PTUDP_H
