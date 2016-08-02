#ifndef YM_VERILOG_PT_POPORT_H
#define YM_VERILOG_PT_POPORT_H

/// @file ym_verilog/pt/PtPort.h
/// @brief PtPort のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: PtPort.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/pt/PtBase.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtPort PtPort.h <ym_verilog/pt/PtPort.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief port を表すクラス
//////////////////////////////////////////////////////////////////////
class PtPort :
  public PtBase
{
public:

  /// @brief 仮想デストラクタ
  virtual
  ~PtPort() { }


public:
  //////////////////////////////////////////////////////////////////////
  // PtPort の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 外向の名前の取得
  /// @return 外向の名前(本当のポート名)\n
  /// 無い場合は NULL を返す
  virtual
  const char*
  ext_name() const = 0;

  /// @brief 内部のポート結線リストのサイズの取得
  virtual
  ymuint32
  portref_num() const = 0;

  /// @brief 内部のポート結線リストの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < portref_num() )
  virtual
  const PtPortRef*
  portref(ymuint32 pos) const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class PtPortRef PtPort.h <ym_verilog/pt/PtPort.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief port reference を表すクラス
//////////////////////////////////////////////////////////////////////
class PtPortRef :
  public PtNamedBase
{
public:

  /// @brief 仮想デストラクタ
  virtual
  ~PtPortRef() { }


public:
  //////////////////////////////////////////////////////////////////////
  // PtPortRef の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief インデックスの取得
  /// @return インデックスを表す式
  virtual
  const PtExpr*
  index() const = 0;

  /// @brief 範囲指定モードの取得
  /// @retval kVpiNoRange 範囲指定なし
  /// @retval kVpiConstRange [ a : b ] のタイプ
  /// @retval kVpiPlusRange  [ a :+ b ] のタイプ
  /// @retval kVpiMinusRange [ a :- b ] のタイプ
  virtual
  tVpiRangeMode
  range_mode() const = 0;

  /// @brief 範囲の左側の式の取得
  /// @return 範囲の左側の式
  virtual
  const PtExpr*
  left_range() const = 0;

  /// @brief 範囲の右側の式の取得
  /// @return 範囲の右側の式
  virtual
  const PtExpr*
  right_range() const = 0;

  /// @brief 方向を得る．
  virtual
  tVpiDirection
  dir() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_PT_POPORT_H
