#ifndef YM_VERILOG_VPI_VPIUDP_H
#define YM_VERILOG_VPI_VPIUDP_H

/// @file ym_verilog/vpi/VpiUdp.h
/// @brief VpiUdp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VpiUdp.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.


#include <ym_utils/ShString.h>
#include <ym_verilog/vpi/VpiBase.h>
#include <ym_verilog/vpi/VpiIterator.h>


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VpiUdpDefn VpiUdp.h <ym_verilog/vpi/VpiUdp.h>
/// @brief UDP定義を表すクラス
/// IEEE Std 1364-2001 26.6.14 UDP
//////////////////////////////////////////////////////////////////////
class VpiUdpDefn :
  public VpiBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // VpiUdpDefn に固有の仮想関数
  // 設定用
  //////////////////////////////////////////////////////////////////////

  /// @brief 入出力オブジェクトの内容を設定する．
  /// @param[in] pos ポート中の位置
  /// @param[in] file_region ソースファイル上の位置
  /// @param[in] name 名前
  /// @param[in] dir 向き
  virtual
  void
  set_io(ymuint32 pos,
	 const FileRegion& file_region,
	 const ShString& name,
	 tVpiDirection dir) = 0;

  /// @brief 初期化文を設定する．
  /// @param[in] initial 初期化文
  virtual
  void
  set_initial(VpiUdpInitial* initial) = 0;

  /// @brief table entry の内容を設定する．
  /// @param[in] pos 行番号
  /// @param[in] file_region ソースファイル上の位置
  /// @param[in] vals シンボル値の配列
  virtual
  void
  set_tableentry(ymuint32 pos,
		 const FileRegion& file_region,
		 const vector<tVpiUdpVal>& vals) = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // VpiUdpDefn に固有の仮想関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 定義された名前を返す．
  virtual
  string
  def_name() const = 0;
  
  /// @brief primitive type を返す．
  virtual
  tVpiPrimType
  prim_type() const = 0;
  
  /// @brief primitive type を表す文字列返す．
  virtual
  string
  s_prim_type() const = 0;

  /// @brief 入力数を返す．
  virtual
  int
  size() const = 0;
  
  /// @brief protected かどうかを返す．
  virtual
  bool
  is_protected() const = 0;

  /// @brief 初期値設定文を返す．
  virtual
  VpiUdpInitial*
  initial() const = 0;
  
  /// @brief IO decl のリストの反復子を返す．
  virtual
  VpiIODeclIteratorImpl*
  io_decl_iterator_obj() = 0;
  
  /// @brief table entry のリストの反復子を返す．
  virtual
  VpiTableEntryIteratorImpl*
  table_entry_iterator_obj() = 0;
  
  /// @brief IO decl のリストの反復子を返す．
  virtual
  VpiIODeclIterator
  io_decl_iterator() = 0;
  
  /// @brief table entry のリストの反復子を返す．
  virtual
  VpiTableEntryIterator
  table_entry_iterator() = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class VpiUdpInitial VpiUdp.h "<ym_verilog/vpi/VpiUdp.h>
/// @brief UDP 用 intial の基底クラス
/// @note 本当の initial 文だと使い勝手が悪いので専用のクラスを作る．
/// IEEE Std 1364-2001 26.6.14 UDP
/// IEEE Std 1364-2001 26.6.27 Process, block, statement, event statement
//////////////////////////////////////////////////////////////////////
class VpiUdpInitial :
  public VpiBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // VpiUdpInitial に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 親の UDP を返す．
  virtual
  VpiUdpDefn*
  udp_defn() const = 0;

  /// @brief 初期値の取得
  /// @return 0/1/X を返す．
  virtual
  tVpiScalarVal
  init_val() const = 0;

  /// @brief ステートメントを返す．
  /// @note 実際には VpiUdpAssign を返す．
  virtual
  VpiUdpAssign*
  stmt() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class VpiUdpAssign VpiUdp.h <ym_verilog/vpi/VpiUdp.h>
/// @brief UDP 用の代入文クラス
//////////////////////////////////////////////////////////////////////
class VpiUdpAssign :
  public VpiBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // VpiUdpAssign に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 親の UDP を返す．
  virtual
  VpiUdpDefn*
  udp() const = 0;

  /// @brief 親の process を返す．
  /// @note 実際には VpiUdpInitial を返す．
  virtual
  VpiUdpInitial*
  process() const = 0;
  
  /// @brief 右辺を返す．
  virtual
  VpiExpr*
  rhs() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class VpiTableEntry VpiUdp.h <ym_verilog/vpi/VpiUdp.h>
/// @brief UDP の table entry を表すクラス
//////////////////////////////////////////////////////////////////////
class VpiTableEntry :
  public VpiBase
{
public:
  /// @brief 親クラスのエイリアス
  typedef VpiBase Parent;


public:
  //////////////////////////////////////////////////////////////////////
  // VpiTableEntry に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief pos 番目の位置の値を返す．
  /// @note VPI 関数とは無関係だけどあると便利な関数
  virtual
  tVpiUdpVal
  val(ymuint32 pos) const = 0;
  
  /// @brief 一行の要素数を返す．
  virtual
  int
  size() const = 0;

  /// @brief 親の VpiUdpDefn へのポインタを返す．
  virtual
  VpiUdpDefn*
  udp_defn() const = 0;

  /// @brief 一行文の内容を表す文字列をつくる．
  virtual
  string
  str() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_VPI_VPIUDP_H
