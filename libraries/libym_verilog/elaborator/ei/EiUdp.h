#ifndef LIBYM_VERILOG_ELABORATOR_EI_EIUDP_H
#define LIBYM_VERILOG_ELABORATOR_EI_EIUDP_H

/// @file libym_verilog/elaborator/ei/ElbUdpImpl.h
/// @brief ElbUdpImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiUdp.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

// UDP定義を表すクラス
// IEEE Std 1364-2001 26.6.14 UDP


#include "ElbUdp.h"
#include "ElbIODecl.h"


BEGIN_NAMESPACE_YM_VERILOG

class EiUdpIO;
class EiTableEntry;
class EiExpr;

//////////////////////////////////////////////////////////////////////
/// @class EiUdpDefn ElbUdpImpl.h "ElbUdpImpl.h"
/// @brief ElbUdpDefn の実装クラス
//////////////////////////////////////////////////////////////////////
class EiUdpDefn :
  public ElbUdpDefn
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_udp パース木の UDP 定義
  /// @param[in] is_protected プロテクト属性
  /// @param[in] io_num ポート数
  /// @param[in] io_array IOの配列
  /// @param[in] table_num テーブルの行数
  /// @param[in] table テーブル
  /// @param[in] val_array テーブル中の値を納める配列
  EiUdpDefn(const PtUdp* pt_udp,
	    bool is_protected,
	    ymuint32 io_num,
	    EiUdpIO* io_array,
	    ymuint32 table_num,
	    EiTableEntry* table,
	    tVpiUdpVal* val_array);

  /// @brief デストラクタ
  virtual
  ~EiUdpDefn();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;

  /// @brief ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;


public:
  //////////////////////////////////////////////////////////////////////
  // VlUdpDefn の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 定義された名前を返す．
  virtual
  const char*
  def_name() const;

  /// @brief primitive type を返す．
  virtual
  tVpiPrimType
  prim_type() const;

  /// @brief ポート数を返す．
  virtual
  ymuint32
  port_num() const;

  /// @brief 入力の宣言要素を返す．
  /// @param[in] pos 入力番号 ( 0 <= pos < port_num() - 1 )
  virtual
  const VlIODecl*
  input(ymuint32 pos) const;

  /// @brief 出力の宣言要素を返す．
  virtual
  const VlIODecl*
  output() const;

  /// @brief protected かどうかを返す．
  virtual
  bool
  is_protected() const;

  /// @brief 初期値の式を返す．
  virtual
  const VlExpr*
  init_expr() const;

  /// @brief 初期値を返す．
  /// @return 0/1/X を返す．
  virtual
  tVpiScalarVal
  init_val() const;

  /// @brief table entry の行数を返す．
  virtual
  ymuint32
  table_size() const;

  /// @brief table entry を返す．
  /// @param[in] pos 行番号
  virtual
  const VlTableEntry*
  table_entry(ymuint32 pos) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入出力オブジェクトの内容を設定する．
  /// @param[in] pos ポート中の位置
  /// @param[in] file_region ソースファイル上の位置
  /// @param[in] name 名前
  /// @param[in] dir 向き
  void
  set_io(ymuint32 pos,
	 const PtIOHead* pt_header,
	 const PtIOItem* pt_item);

  /// @brief 初期値を設定する．
  /// @param[in] init_expr 初期値を表す式
  /// @param[in] init_val 初期値
  void
  set_initial(ElbExpr* init_expr,
	      tVpiScalarVal init_val);

  /// @brief table entry の内容を設定する．
  /// @param[in] pos 行番号
  /// @param[in] pt_udp_entry パース木の一行分の定義
  /// @param[in] vals シンボル値の配列
  void
  set_tableentry(ymuint32 pos,
		 const PtUdpEntry* pt_udp_entry,
		 const vector<tVpiUdpVal>& vals);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パース木の UDP 定義
  const PtUdp* mPtUdp;

  // ポート数
  ymuint32 mPortNum;

  // protect されているとき true にするフラグ
  bool mProtected;

  // IO decl のリスト(配列)
  EiUdpIO* mIODeclList;

  // 初期値の式
  ElbExpr* mInitExpr;

  // 初期値
  tVpiScalarVal mInitVal;

  // table entry のサイズ
  ymuint32 mTableEntrySize;

  // table entry のリスト
  EiTableEntry* mTableEntryList;

  // table entry の値の配列
  tVpiUdpVal* mValArray;

};


//////////////////////////////////////////////////////////////////////
/// UDP 用 IO decl の基底クラス
//////////////////////////////////////////////////////////////////////
class EiUdpIO :
  public VlIODecl
{
  friend class EiFactory;
  friend class EiUdpDefn;

private:

  /// @brief コンストラクタ
  EiUdpIO();

  /// @brief デストラクタ
  virtual
  ~EiUdpIO();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;

  /// @brief ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;


public:
  //////////////////////////////////////////////////////////////////////
  // VlIODecl に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  virtual
  const char*
  name() const;

  /// @brief 方向を返す．
  virtual
  tVpiDirection
  direction() const;

  /// @brief 符号の属性の取得
  /// @return 符号付きのとき true を返す．
  /// @note このクラスは false を返す．
  virtual
  bool
  is_signed() const;

  /// @brief 範囲の MSB の取得
  /// @return このクラスは NULL を返す．
  virtual
  const VlExpr*
  left_range() const;

  /// @brief 範囲の LSB の取得
  /// @return このクラスは NULL を返す．
  virtual
  const VlExpr*
  right_range() const;

  /// @brief MSB の値を返す．
  /// @retval 範囲のMSBの値 範囲指定を持つとき
  /// @retval -1 範囲指定を持たないとき
  virtual
  int
  left_range_const() const;

  /// @brief LSB の値を返す．
  /// @retval 範囲のLSBの値 範囲指定を持つとき
  /// @retval -1 範囲指定を持たないとき
  virtual
  int
  right_range_const() const;

  /// @brief サイズを返す．
  /// このクラスは 1 を返す．
  virtual
  ymuint32
  bit_size() const;

  /// @brief 対応する宣言要素を返す．
  /// @note このクラスでは NULL を返す．
  virtual
  const VlDecl*
  decl() const;

  /// @brief 親のモジュールの取得
  /// @return このクラスは NULL を返す．
  virtual
  const VlModule*
  module() const;

  /// @brief 親のUDP定義を返す．
  virtual
  const VlUdpDefn*
  udp_defn() const;

  /// @brief 親のタスクの取得
  /// @return このクラスは NULL を返す．
  virtual
  const VlTaskFunc*
  task() const;

  /// @brief 親の関数の取得
  /// @return このクラスは NULL を返す．
  virtual
  const VlTaskFunc*
  function() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数 (EiUdpDefn が使う)
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を設定する．
  /// @param[in] pt_header パース木のIO宣言ヘッダ
  /// @param[in] pt_item パース木のIO宣言定義
  void
  set(const PtIOHead* pt_header,
      const PtIOItem* pt_item);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親の UDP
  ElbUdpDefn* mUdp;

  // パース木のIO宣言ヘッダ
  const PtIOHead* mPtHeader;

  // パース木のIO宣言定義
  const PtIOItem* mPtItem;

};


//////////////////////////////////////////////////////////////////////
/// @class EiTableEntry ElbUdpImpl.h "ElbUdpImpl.h"
/// @brief UDP の table entry を表すクラス
//////////////////////////////////////////////////////////////////////
class EiTableEntry :
  public ElbTableEntry
{
  friend class EiFactory;
  friend class EiUdpDefn;

private:

  /// @brief コンストラクタ
  EiTableEntry();

  /// @brief デストラクタ
  virtual
  ~EiTableEntry();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;

  /// @brief ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;


public:
  //////////////////////////////////////////////////////////////////////
  // VlTableEntry の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 一行の要素数を返す．
  virtual
  ymuint32
  size() const;

  /// @brief pos 番目の位置の値を返す．
  virtual
  tVpiUdpVal
  val(ymuint32 pos) const;

  /// @brief 一行文の内容を表す文字列をつくる．
  virtual
  string
  str() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbTableEntry の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 設定する．
  void
  set(const PtUdpEntry* pt_entry,
      const vector<tVpiUdpVal>& vals);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親の UDP
  ElbUdpDefn* mUdp;

  // パース木の定義
  const PtUdpEntry* mPtUdpEntry;

  // 値の配列 (実体は EiUdpDefn が持っている)
  tVpiUdpVal* mValArray;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELABORATOR_EI_EIUDP_H
