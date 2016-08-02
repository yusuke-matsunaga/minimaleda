#ifndef YM_VERILOG_PT_PTMISC_H
#define YM_VERILOG_PT_PTMISC_H

/// @file ym_verilog/pt/PtMisc.h
/// @brief その他のの部品クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: PtMisc.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/pt/PtBase.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtControl PtMisc.h <ym_verilog/pt/PtMisc.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief delay control/event control/repeat control を表すクラス
//////////////////////////////////////////////////////////////////////
class PtControl :
  public PtBase
{
public:

  /// @brief 仮想デストラクタ
  virtual
  ~PtControl() { }


public:
  //////////////////////////////////////////////////////////////////////
  // PtControl の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  /// @return 型
  virtual
  tPtCtrlType
  type() const = 0;

  /// @brief 遅延式の取得
  /// @retval 遅延を表す式 delay control の場合
  /// @retval NULL 上記以外
  virtual
  const PtExpr*
  delay() const = 0;

  /// @brief イベントリストのサイズの取得
  /// @retval イベントリストのサイズ event control/repeat control の場合
  /// @retval 0 上記以外
  virtual
  ymuint32
  event_num() const = 0;

  /// @brief イベントリストの要素の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < event_num() )
  /// @note event control/repeat control の場合のみ意味を持つ
  virtual
  const PtExpr*
  event(ymuint32 pos) const = 0;

  /// @brief 繰り返し数の取得
  /// @retval 繰り返し数を表す式 repeat control の場合
  /// @retval NULL 上記以外
  virtual
  const PtExpr*
  rep_expr() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class PtConnection PtMisc.h <ym_verilog/pt/PtMisc.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief orered_connection/named_connection を表すクラス
//////////////////////////////////////////////////////////////////////
class PtConnection :
  public PtBase
{
public:

  /// @brief 仮想デストラクタ
  virtual
  ~PtConnection() { }


public:
  //////////////////////////////////////////////////////////////////////
  // PtConnection の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  /// @retval 名前 named connection の場合
  /// @retval "" ordered connection の場合
  virtual
  const char*
  name() const = 0;

  /// @brief 式の取得
  /// @return 式
  virtual
  const PtExpr*
  expr() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class PtStrength PtMisc.h <ym_verilog/pt/PtMisc.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief strength を表すクラス
//////////////////////////////////////////////////////////////////////
class PtStrength :
  public PtBase
{
public:

  /// @brief 仮想デストラクタ
  virtual
  ~PtStrength() { }


public:
  //////////////////////////////////////////////////////////////////////
  // PtStrength の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief drive strength0 の取得
  /// @return 0 の強度
  virtual
  tVpiStrength
  drive0() const = 0;

  /// @brief drive strength1 の取得
  /// @return 1 の強度
  virtual
  tVpiStrength
  drive1() const = 0;

  /// @brief charge strength の取得
  /// @return 電荷の強度
  virtual
  tVpiStrength
  charge() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class PtDelay PtMisc.h <ym_verilog/pt/PtMisc.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief delay を表すクラス
//////////////////////////////////////////////////////////////////////
class PtDelay :
  public PtBase
{
public:

  /// @brief 仮想デストラクタ
  virtual
  ~PtDelay() { }


public:
  //////////////////////////////////////////////////////////////////////
  // PtDelay の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値の取得
  /// @param[in] pos 取得する遅延値の位置(0 〜 2)
  /// @return pos 番目の遅延を表す式\n
  /// 該当する要素がなければ NULL を返す．
  virtual
  const PtExpr*
  value(ymuint32 pos) const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class PtNameBranch PtMisc.h <ym_verilog/pt/PtMisc.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief 階層名を表すクラス
//////////////////////////////////////////////////////////////////////
class PtNameBranch
{
public:

  /// @brief 仮想デストラクタ
  virtual
  ~PtNameBranch() { }


public:

  /// @brief 名前の取得
  /// @return 名前
  virtual
  const char*
  name() const = 0;

  /// @brief インデックスの有無のチェック
  /// @retval true インデックスを持っている時
  /// @retval false インデックスを持っていない時
  /// @note デフォルトで false を返す．
  virtual
  bool
  has_index() const = 0;

  /// @brief インデックスの取得
  /// @return インデックスの値
  /// @note デフォルトで 0 を返す．
  virtual
  int
  index() const = 0;

};

/// @relates PtNameBranch
/// @brief 階層名を作り出す関数
/// @param[in] nb_array 階層ブランチリスト
/// @param[in] name 末尾の名前
/// @return 階層名を展開したものを返す．
string
expand_full_name(const PtNameBranchArray& nb_array,
		 const char* name);


//////////////////////////////////////////////////////////////////////
/// @class PtAttrInst PtMisc.h <ym_verilog/pt/PtMisc.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief attribute_instance を表すクラス
//////////////////////////////////////////////////////////////////////
class PtAttrInst
{
public:

  /// @brief 仮想デストラクタ
  virtual
  ~PtAttrInst() { }


public:
  //////////////////////////////////////////////////////////////////////
  // PtAttrInst の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素数の取得
  /// @return 要素数
  virtual
  ymuint32
  attrspec_num() const = 0;

  /// @brief 要素の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < attrspec_num() )
  virtual
  const PtAttrSpec*
  attrspec(ymuint32 pos) const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class PtAttrSpec PtMisc.h <ym_verilog/pt/PtMisc.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief attr_spec を表すクラス
//////////////////////////////////////////////////////////////////////
class PtAttrSpec
{
public:

  /// @brief 仮想デストラクタ
  virtual
  ~PtAttrSpec() { }


public:
  //////////////////////////////////////////////////////////////////////
  // PtAttrSpec の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置の取得
  /// @return ファイル位置
  virtual
  FileRegion
  file_region() const = 0;

  /// @brief 名前の取得
  /// @return 名前
  virtual
  const char*
  name() const = 0;

  /// @brief 式の取得
  /// @return 式
  virtual
  const PtExpr*
  expr() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_PT_PTMISC_H
