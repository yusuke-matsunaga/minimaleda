#ifndef YM_VERILOG_PT_PTDECL_H
#define YM_VERILOG_PT_PTDECL_H

/// @file ym_verilog/pt/PtDecl.h
/// @brief 宣言要素関係の部品クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: PtDecl.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/pt/PtBase.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtIOHead PtDecl.h <ym_verilog/pt/PtDecl.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief IO宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class PtIOHead :
  public PtBase
{
public:

  /// @brief デストラクタ
  virtual
  ~PtIOHead() { }


public:
  //////////////////////////////////////////////////////////////////////
  // PtIOH の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  /// @return 型
  virtual
  tPtIOType
  type() const = 0;

  /// @brief 補助的な型の取得
  /// @return 補助的な型
  virtual
  tVpiAuxType
  aux_type() const = 0;

  /// @brief 補助的なネット型の取得
  /// @return ネット型
  virtual
  tVpiNetType
  net_type() const = 0;

  /// @brief 補助的な変数型の取得
  /// @return 変数型
  virtual
  tVpiVarType
  var_type() const = 0;

  /// @brief 符号の取得
  /// @retval true 符号付き
  /// @retval false 符号なし
  virtual
  bool
  is_signed() const = 0;

  /// @brief 範囲のMSBの取得
  /// @retval 範囲のMSB
  /// @retval NULL 範囲を持たないとき
  virtual
  const PtExpr*
  left_range() const = 0;

  /// @brief 範囲のLSBの取得
  /// @retval 範囲のLSB
  /// @retval NULL 範囲を持たないとき
  virtual
  const PtExpr*
  right_range() const = 0;
  
  /// @brief 要素数の取得
  virtual
  ymuint32
  item_num() const = 0;
  
  /// @brief 要素の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < item_num() )
  virtual
  const PtIOItem*
  item(ymuint32 pos) const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class PtIOItem PtDecl.h <ym_verilog/pt/PtDecl.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief IO宣言要素の基底クラス
//////////////////////////////////////////////////////////////////////
class PtIOItem :
  public PtNamedBase
{
public:

  /// @brief デストラクタ
  virtual
  ~PtIOItem() { }


public:
  //////////////////////////////////////////////////////////////////////
  // PtIOItem の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 初期値の取得
  /// @retval 初期値
  /// @retval NULL 初期値を持たないとき
  virtual
  const PtExpr*
  init_value() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class PtDeclHead PtDecl.h <ym_verilog/pt/PtDecl.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief 宣言要素のヘッダの基底クラス
//////////////////////////////////////////////////////////////////////
class PtDeclHead :
  public PtBase
{
public:

  /// @brief デストラクタ
  virtual
  ~PtDeclHead() { }


public:
  //////////////////////////////////////////////////////////////////////
  // PtDeclH の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 宣言要素の型の取得
  /// @return 宣言要素の型
  virtual
  tPtDeclType
  type() const = 0;

  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  virtual
  bool
  is_signed() const = 0;

  /// @brief 範囲のMSBの取得
  /// @retval 範囲のMSB 範囲を持つとき
  /// @retval NULL 範囲を持たないとき
  virtual
  const PtExpr*
  left_range() const = 0;

  /// @brief 範囲のLSBの取得
  /// @retval 範囲のLSB 範囲を持つとき
  /// @retval NULL 範囲を持たないとき
  virtual
  const PtExpr*
  right_range() const = 0;

  /// @brief データ型の取得
  /// @retval データ型 kParam, kLocalParam, kVar の場合
  /// @retval kVpiVarNone 上記以外
  virtual
  tVpiVarType
  data_type() const = 0;

  /// @brief net 型の取得
  /// @retval net 型 net 型の要素の場合
  /// @retval kVpiNone net 型の要素でない場合
  virtual
  tVpiNetType
  net_type() const = 0;

  /// @brief vectored|scalared 属性の取得
  /// @retval kVpiVsNone vectored|scalared 指定なし
  /// @retval kVpiVectored vectored 指定あり
  /// @retval kVpiScalared scalared 指定あり
  virtual
  tVpiVsType
  vs_type() const = 0;

  /// @brief strength の取得
  /// @retval strength
  /// @retval NULL strength の指定なし
  virtual
  const PtStrength*
  strength() const = 0;

  /// @brief delay の取得
  /// @retval delay
  /// @retval NULL delay の指定なし
  virtual
  const PtDelay*
  delay() const = 0;

  /// @brief 要素数の取得
  /// @return 要素数
  virtual
  ymuint32
  item_num() const = 0;

  /// @brief 要素の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < item_num() )
  virtual
  const PtDeclItem*
  item(ymuint32 pos) const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class PtDeclItem PtDecl.h <ym_verilog/pt/PtDecl.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief 宣言要素のクラス
//////////////////////////////////////////////////////////////////////
class PtDeclItem :
  public PtNamedBase
{
public:

  /// @brief デストラクタ
  virtual
  ~PtDeclItem() { }


public:
  //////////////////////////////////////////////////////////////////////
  // PtDeclItem の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief dimension list のサイズの取得
  /// @return dimension list のサイズ
  virtual
  ymuint32
  dimension_list_size() const = 0;

  /// @brief 範囲の取得
  virtual
  const PtRange*
  range(ymuint32 pos) const = 0;

  /// @brief 初期値の取得
  /// @retval 初期値
  /// @retval NULL 設定がない場合
  virtual
  const PtExpr*
  init_value() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class PtRange PtDecl.h <ym_verilog/pt/PtDecl.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief 範囲を表すクラス
//////////////////////////////////////////////////////////////////////
class PtRange :
  public PtBase
{
public:

  /// @brief デストラクタ
  virtual
  ~PtRange() { }


public:
  //////////////////////////////////////////////////////////////////////
  // PtRange の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲の MSB を取り出す．
  virtual
  const PtExpr*
  left() const = 0;

  /// @brief 範囲の LSB を取り出す．
  virtual
  const PtExpr*
  right() const = 0;
  
};

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_PT_PTDECL_H
