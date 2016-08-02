#ifndef LIBYM_VERILOG_ELABORATOR_EI_EIPRIMITIVE_H
#define LIBYM_VERILOG_ELABORATOR_EI_EIPRIMITIVE_H

/// @file libym_verilog/elaborator/ei/EiPrimitive.h
/// @brief EiPrimitive のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiPrimitive.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.


#include "ElbPrimitive.h"
#include "EiRange.h"


BEGIN_NAMESPACE_YM_VERILOG

class EiPrimitive;
class EiPrimitive1;
class EiPrimArray;
class EiPrimTerm;

//////////////////////////////////////////////////////////////////////
/// @class EiPrimHead EiPrimitive.h "EiPrimitive.h"
/// @brief primitive/primitive array のヘッダ情報
//////////////////////////////////////////////////////////////////////
class EiPrimHead :
  public ElbPrimHead
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_header パース木の定義
  EiPrimHead(const VlNamedObj* parent,
	     const PtItem* pt_header);

  /// @brief デストラクタ
  virtual
  ~EiPrimHead();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbPrimHead の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief このオブジェクトの属しているスコープを返す．
  virtual
  const VlNamedObj*
  parent() const;

  /// @brief primitive type を返す．
  virtual
  tVpiPrimType
  prim_type() const;

  /// @brief プリミティブの定義名を返す．
  virtual
  const char*
  def_name() const;

  /// @brief UDP 定義を返す．
  /// @note このクラスでは NULL を返す．
  virtual
  const ElbUdpDefn*
  udp_defn() const;

  /// @brief 0 の強さを得る．
  virtual
  tVpiStrength
  drive0() const;

  /// @brief 1 の強さを得る．
  virtual
  tVpiStrength
  drive1() const;

  /// @brief 遅延式を得る．
  /// @note このクラスでは NULL を返す．
  virtual
  ElbDelay*
  delay() const;

  /// @brief 遅延式を設定する．
  /// @note このクラスではなにもしない．
  virtual
  void
  set_delay(ElbDelay* expr);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のスコープ
  const VlNamedObj* mParent;

  // パース木の定義
  const PtItem* mPtHead;

};


//////////////////////////////////////////////////////////////////////
/// @class EiPrimHeadD EiPrimitive.h "EiPrimitive.h"
/// @brief 遅延付きの EiPrimHead
//////////////////////////////////////////////////////////////////////
class EiPrimHeadD :
  public EiPrimHead
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_header パース木の定義
  EiPrimHeadD(const VlNamedObj* parent,
	      const PtItem* pt_header);

  /// @brief デストラクタ
  virtual
  ~EiPrimHeadD();


public:
  //////////////////////////////////////////////////////////////////////
  // EiPrimHead の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 遅延式を得る．
  virtual
  ElbDelay*
  delay() const;

  /// @brief 遅延式を設定する．
  virtual
  void
  set_delay(ElbDelay* expr);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 遅延値
  ElbDelay* mDelay;

};


//////////////////////////////////////////////////////////////////////
/// @class EiPrimHeadU EiPrimitive.h "EiPrimitive.h"
/// @brief UDP 型の EiPrimHead
//////////////////////////////////////////////////////////////////////
class EiPrimHeadU :
  public EiPrimHead
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_header パース木の定義
  /// @param[in] udp UDP 定義
  EiPrimHeadU(const VlNamedObj* parent,
	      const PtItem* pt_header,
	      const ElbUdpDefn* udp);

  /// @brief デストラクタ
  virtual
  ~EiPrimHeadU();


public:
  //////////////////////////////////////////////////////////////////////
  // EiPrimHead の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief primitive type を返す．
  virtual
  tVpiPrimType
  prim_type() const;

  /// @brief プリミティブの定義名を返す．
  virtual
  const char*
  def_name() const;

  /// @brief UDP 定義を返す．
  virtual
  const ElbUdpDefn*
  udp_defn() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // UDP 定義
  const ElbUdpDefn* mUdp;

};


//////////////////////////////////////////////////////////////////////
/// @class EiPrimHeadUD EiPrimitive.h "EiPrimitive.h"
/// @brief 遅延付きの EiPrimHeadU
//////////////////////////////////////////////////////////////////////
class EiPrimHeadUD :
  public EiPrimHeadU
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_header パース木の定義
  /// @param[in] udp UDP 定義
  EiPrimHeadUD(const VlNamedObj* parent,
	       const PtItem* pt_header,
	       const ElbUdpDefn* udp);

  /// @brief デストラクタ
  virtual
  ~EiPrimHeadUD();


public:
  //////////////////////////////////////////////////////////////////////
  // EiPrimHead の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 遅延式を得る．
  virtual
  ElbDelay*
  delay() const;

  /// @brief 遅延式を設定する．
  void
  set_delay(ElbDelay* expr);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 遅延値
  ElbDelay* mDelay;

};


//////////////////////////////////////////////////////////////////////
/// @class EiPrimitive EiPrimitive.h "EiPrimitive.h"
/// @brief ElbPrimitive の実装クラス
//////////////////////////////////////////////////////////////////////
class EiPrimitive :
  public ElbPrimitive
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  EiPrimitive();

  /// @brief デストラクタ
  virtual
  ~EiPrimitive();


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
  // VlNamedObj の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief このオブジェクトの属しているスコープを返す．
  virtual
  const VlNamedObj*
  parent() const;


public:
  //////////////////////////////////////////////////////////////////////
  // VlPrimitive の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief primitive type を返す．
  virtual
  tVpiPrimType
  prim_type() const;

  /// @brief プリミティブの定義名を返す．
  virtual
  const char*
  def_name() const;

  /// @brief UPD 定義を返す．
  virtual
  const VlUdpDefn*
  udp_defn() const;

  /// @brief 0 の強さを得る．
  virtual
  tVpiStrength
  drive0() const;

  /// @brief 1 の強さを得る．
  virtual
  tVpiStrength
  drive1() const;

  /// @brief 遅延式を得る．
  virtual
  const VlDelay*
  delay() const;

  /// @brief ポート数を得る．
  virtual
  ymuint32
  port_num() const;

  /// @brief ポート端子を得る．
  /// @param[in] pos 位置番号 (0 <= pos < port_num())
  virtual
  const VlPrimTerm*
  prim_term(ymuint32 pos) const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbPrimitive の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 接続する．
  /// @param[in] pos ポート番号 (0 から始まる)
  /// @param[in] expr 接続する式
  void
  connect(ymuint32 pos,
	  ElbExpr* expr);


protected:
  //////////////////////////////////////////////////////////////////////
  // 初期化用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ポート配列を初期化する．
  /// @param[in] term_array 端子の配列
  void
  init_port(EiPrimTerm* term_array);


private:
  //////////////////////////////////////////////////////////////////////
  // EiPrimitive の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ヘッダを得る．
  virtual
  ElbPrimHead*
  head() const = 0;

  /// @brief パース木のインスタンス定義を得る．
  virtual
  const PtInst*
  pt_inst() const = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ポート数
  ymuint32 mPortNum;

  // ポートの配列
  EiPrimTerm* mPortArray;

};


//////////////////////////////////////////////////////////////////////
/// @class EiPrimitive1 EiPrimitive.h "EiPrimitive.h"
/// @brief 配列要素の EiPrimitive
//////////////////////////////////////////////////////////////////////
class EiPrimitive1 :
  public EiPrimitive
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  EiPrimitive1();

  /// @brief デストラクタ
  virtual
  ~EiPrimitive1();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期設定を行う．
  /// @param[in] prim_array 親の配列
  /// @param[in] index インデックス番号
  /// @param[in] term_array 端子の配列
  void
  init(EiPrimArray* prim_array,
       int index,
       EiPrimTerm* term_array);


public:
  //////////////////////////////////////////////////////////////////////
  // VlNamedObj の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  virtual
  const char*
  name() const;


private:
  //////////////////////////////////////////////////////////////////////
  // EiPrimitive の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ヘッダを得る．
  virtual
  ElbPrimHead*
  head() const;

  /// @brief パース木のインスタンス定義を得る．
  virtual
  const PtInst*
  pt_inst() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親の配列
  EiPrimArray* mPrimArray;

  // インデックス番号
  int mIndex;

  // 名前
  const char* mName;

};


//////////////////////////////////////////////////////////////////////
/// @class EiPrimitive2 EiPrimitive.h "EiPrimitive.h"
/// @brief 単一要素の EiPrimitive
//////////////////////////////////////////////////////////////////////
class EiPrimitive2 :
  public EiPrimitive
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] head ヘッダ
  /// @param[in] pt_inst インスタンス定義
  /// @param[in] term_array 端子の配列
  EiPrimitive2(ElbPrimHead* head,
	       const PtInst* pt_inst,
	       EiPrimTerm* term_array);

  /// @brief デストラクタ
  virtual
  ~EiPrimitive2();


public:
  //////////////////////////////////////////////////////////////////////
  // VlNamedObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  virtual
  const char*
  name() const;


private:
  //////////////////////////////////////////////////////////////////////
  // EiPrimitive の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ヘッダを得る．
  virtual
  ElbPrimHead*
  head() const;

  /// @brief パース木のインスタンス定義を得る．
  virtual
  const PtInst*
  pt_inst() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ヘッダ
  ElbPrimHead* mHead;

  // パース木のインスタンス定義
  const PtInst* mPtInst;

};


//////////////////////////////////////////////////////////////////////
/// @class EiPrimArray EiPrimitive.h "EiPrimitive.h"
/// @brief ElbPrimArray の実装クラス
//////////////////////////////////////////////////////////////////////
class EiPrimArray :
  public ElbPrimArray
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] head ヘッダ
  /// @param[in] pt_inst インスタンス定義
  /// @param[in] range 範囲
  /// @param[in] elem_array 要素の配列
  /// @param[in] term_array 端子の配列
  EiPrimArray(ElbPrimHead* head,
	      const PtInst* pt_inst,
	      const EiRangeImpl& range,
	      EiPrimitive1* elem_array,
	      EiPrimTerm* term_array);

  /// @brief デストラクタ
  virtual
  ~EiPrimArray();


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
  // VlNamedObj の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief このオブジェクトの属しているスコープを返す．
  virtual
  const VlNamedObj*
  parent() const;

  /// @brief 名前の取得
  virtual
  const char*
  name() const;


public:
  //////////////////////////////////////////////////////////////////////
  // VlPrimArray の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief primitive type を返す．
  virtual
  tVpiPrimType
  prim_type() const;

  /// @brief プリミティブの定義名を返す．
  virtual
  const char*
  def_name() const;

  /// @brief UDP 定義を返す．
  virtual
  const VlUdpDefn*
  udp_defn() const;

  /// @brief 0 の強さを得る．
  virtual
  tVpiStrength
  drive0() const;

  /// @brief 1 の強さを得る．
  virtual
  tVpiStrength
  drive1() const;

  /// @brief 遅延式を得る．
  virtual
  const VlDelay*
  delay() const;

  /// @brief 範囲の MSB を返す．
  virtual
  const VlExpr*
  left_range() const;

  /// @brief 範囲の LSB を返す．
  virtual
  const VlExpr*
  right_range() const;

  /// @brief 要素数を返す．
  virtual
  ymuint32
  elem_num() const;

  /// @brief 要素のプリミティブを返す．
  /// @param[in] offset 位置番号 ( 0 <= offset < elem_num() )
  virtual
  const VlPrimitive*
  elem_by_offset(ymuint32 offset) const;

  /// @brief 要素を返す．
  /// @param[in] index インデックス
  virtual
  const VlPrimitive*
  elem_by_index(int index) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素のプリミティブを取り出す．
  ElbPrimitive*
  _primitive_by_offset(ymuint32 offset) const;

  /// @brief 要素のプリミティブを取り出す．
  ElbPrimitive*
  _primitive_by_index(int index) const;

  /// @brief ヘッダを得る．
  ElbPrimHead*
  head() const;

  /// @brief パース木のインスタンス定義を得る．
  const PtInst*
  pt_inst() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ヘッダ
  ElbPrimHead* mHead;

  // パース木のインスタンス定義
  const PtInst* mPtInst;

  // 範囲
  EiRangeImpl mRange;

  // 要素の配列
  EiPrimitive1* mArray;

};


//////////////////////////////////////////////////////////////////////
/// @class EiPrimTerm EiPrimitive.h "EiPrimitive.h"
/// @brief ElbPrimTerm の実装クラス
//////////////////////////////////////////////////////////////////////
class EiPrimTerm :
  public ElbPrimTerm
{
  friend class EiFactory;
  friend class EiPrimitive;

private:

  /// @brief コンストラクタ
  EiPrimTerm();

  /// @brief デストラクタ
  virtual
  ~EiPrimTerm();


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
  // VlPrimTerm の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のプリミティブを返す．
  virtual
  const VlPrimitive*
  primitive() const;

  /// @brief 入出力の種類を返す．
  virtual
  tVpiDirection
  direction() const;

  /// @brief 端子番号を返す．
  virtual
  ymuint32
  term_index() const;

  /// @brief 接続しているネットを表す式を返す．
  virtual
  const VlExpr*
  expr() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbPrimTerm の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を設定する．
  void
  set(ElbPrimitive* primitive,
      ymuint32 index,
      tVpiDirection dir);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のプリミティブ
  ElbPrimitive* mPrimitive;

  // インデックス + 方向
  ymuint32 mIndexDir;

  // 接続している式
  ElbExpr* mExpr;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELABORATOR_EI_EIPRIMITIVE_H
