#ifndef LIBYM_VERILOG_PT_COMPACT_CPTMODULE_H
#define LIBYM_VERILOG_PT_COMPACT_CPTMODULE_H

/// @file libym_verilog/pt_compact/CptModule.h
/// @brief CptModule のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CptModule.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/pt/PtModule.h"
#include "ym_verilog/pt/PtArray.h"
#include "ym_verilog/pt/PtP.h"

#include "ym_utils/FileRegion.h"

#include "PtiDecl.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @brief module を表すノード
//////////////////////////////////////////////////////////////////////
class CptModule :
  public PtModule
{
  friend class CptFactory;

private:
  
  /// @brief コンストラクタ
  CptModule(const FileRegion& file_region,
	    const char* name,
	    bool macro,
	    bool is_cell,
	    bool is_protected,
	    int time_unit,
	    int time_precision,
	    tVpiNetType net_type,
	    tVpiUnconnDrive unconn,
	    tVpiDefDelayMode delay,
	    int decay,
	    bool explicit_name,
	    bool portfaults,
	    bool suppress_faults,
	    const string& config,
	    const string& library,
	    const string& cell,
	    PtDeclHeadArray paramport_array,
	    PtiPortArray port_array,
	    PtIOHeadArray iohead_array,
	    PtDeclHeadArray paramhead_array,
	    PtDeclHeadArray lparamhead_array,
	    PtDeclHeadArray declhead_array,
	    PtItemArray item_array);
  
  /// @brief デストラクタ
  virtual
  ~CptModule();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtModule の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置の取得
  virtual
  FileRegion
  file_region() const;
  
  /// @brief 名前の取得
  virtual
  const char*
  name() const;
  
  /// @brief macromodule 情報の取得
  virtual
  bool
  is_macromodule() const;
  
  /// @brief cell 情報の取得
  virtual
  bool
  is_cell() const;
    
  /// @brief protect 情報の取得
  virtual
  bool
  is_protected() const;
  
  /// @brief time unit の取得
  virtual
  int
  time_unit() const;
  
  /// @brief time precision の取得
  virtual
  int
  time_precision() const;
  
  /// @brief default net type の取得
  virtual
  tVpiNetType
  nettype() const;
  
  /// @brief unconnected drive の取得
  virtual
  tVpiUnconnDrive
  unconn_drive() const;
  
  /// @brief default delay mode の取得
  virtual
  tVpiDefDelayMode
  delay_mode() const;
  
  /// @brief default decay time の取得
  virtual
  int
  decay_time() const;

  /// @brief portfaults 情報の取得
  virtual
  bool
  portfaults() const;
  
  /// @brief suppress_faults 情報の取得
  virtual
  bool
  suppress_faults() const;
  
  /// @brief config 情報の取得
  virtual
  const string&
  config() const;
  
  /// @brief library 情報の取得
  virtual
  const string&
  library() const;
  
  /// @brief cell 情報の取得
  virtual
  const string&
  cell() const;
  
  /// @brief パラメータポート宣言配列の取得
  virtual
  PtDeclHeadArray
  paramport_array() const;

  /// @brief ポート数の取得
  /// @return ポート数
  virtual
  ymuint32
  port_num() const;
  
  /// @brief ポートの取得
  virtual
  const PtPort*
  port(ymuint32 pos) const;
  
  /// @brief 入出力宣言ヘッダ配列の取得
  virtual
  PtIOHeadArray
  iohead_array() const;
  
  /// @brief 入出力宣言の要素数の取得
  /// @note 個々のヘッダが持つ要素数の総和を計算する．
  virtual
  ymuint32
  iodecl_num() const;

  /// @brief parameter 宣言ヘッダ配列の取得
  virtual
  PtDeclHeadArray
  paramhead_array() const;
  
  /// @brief localparam 宣言ヘッダ配列の取得
  virtual
  PtDeclHeadArray
  localparamhead_array() const;
  
  /// @brief 宣言ヘッダ配列の取得
  virtual
  PtDeclHeadArray
  declhead_array() const;
  
  /// @brief item 配列の取得
  virtual
  PtItemArray
  item_array() const;

  /// @brief 関数名から関数の検索
  virtual
  const PtItem*
  find_function(const char* name) const;

  /// @brief top_module フラグを下ろす．
  virtual
  void
  clear_topmodule() const;
  
  /// @brief top module のチェック
  virtual
  bool
  is_topmodule() const;

  /// @brief in_use フラグの設定
  virtual
  void
  set_in_use() const;

  /// @brief in_use フラグの解除
  virtual
  void
  reset_in_use() const;

  /// @brief in_use フラグの取得
  virtual
  bool
  is_in_use() const;


private:

  // すべてのポートが名前を持っていることを記録する．
  void
  set_named_port();

  // すべてのポートが外部名を持っているときに true を返す．
  // { a, b } のような名無しポートがあると false となる．
  // true の時しか名前による結合は行えない．
  bool
  explicit_name() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 名前
  const char* mName;

  // 様々な情報をパックしたもの
  mutable int mFlags;
    
  // decay time
  int mDefDecayTime;
  
  // config 情報
  string mConfig;
  
  // library 情報
  string mLibrary;
  
  // cell 情報
  string mCell;

  // パラメータポート宣言のリスト
  PtDeclHeadArray mParamPortArray;
  
  // ポートの配列
  PtiPortArray mPortArray;

  // 入出力宣言リスト
  PtIOHeadArray mIOHeadArray;
  
  // 入出力宣言の要素数
  ymuint32 mIODeclNum;

  // parameter 宣言のリスト
  PtDeclHeadArray mParamHeadArray;

  // localparam 宣言のリスト
  PtDeclHeadArray mLparamHeadArray;
  
  // 宣言リスト
  PtDeclHeadArray mDeclHeadArray;
  
  // 要素のリスト
  PtItemArray mItemArray;

  // 関数定義の辞書
  hash_map<string, const PtItem*> mFuncDic;

};


//////////////////////////////////////////////////////////////////////
/// @brief port を表すクラス
//////////////////////////////////////////////////////////////////////
class CptPort :
  public PtiPort
{
  friend class CptFactory;

protected:
  
  /// @brief コンストラクタ
  CptPort(const FileRegion& file_region,
	  const char* ext_name);
  
  /// @brief デストラクタ
  virtual
  ~CptPort();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtPort の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief ファイル位置の取得
  /// @return ファイル位置
  virtual
  FileRegion
  file_region() const;

  /// @brief 外向の名前の取得
  /// @return 外向の名前(本当のポート名)
  /// @return 無い場合は NULL を返す
  virtual
  const char*
  ext_name() const;

  /// @brief 内部のポート結線リストのサイズの取得
  /// @return 内部のポート結線リストのサイズ
  /// @note このクラスでは 0 を返す．
  virtual
  ymuint32
  portref_num() const;

  /// @brief 内部のポート結線の取得
  /// @return 先頭の内部のポート結線
  /// @note このクラスでは NULL を返す．
  virtual
  const PtPortRef*
  portref(ymuint32 pos) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief portref を得る．
  /// @note このクラスでは NULL を返す．
  virtual
  PtiPortRef*
  _portref(ymuint32 pos);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 外部向きの名前
  const char* mExtName;

};


//////////////////////////////////////////////////////////////////////
/// @brief port を表すクラス (portref リスト付き)
//////////////////////////////////////////////////////////////////////
class CptPort1 :
  public CptPort
{
  friend class CptFactory;

private:
  
  /// @brief コンストラクタ
  CptPort1(const FileRegion& file_region,
	   PtiPortRef* portref,
	   const char* ext_name);
  
  /// @brief デストラクタ
  virtual
  ~CptPort1();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtPort の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内部のポート結線リストのサイズの取得
  /// @return 内部のポート結線リストのサイズ
  virtual
  ymuint32
  portref_num() const;

  /// @brief 内部のポート結線の取得
  /// @return 先頭の内部のポート結線
  virtual
  const PtPortRef*
  portref(ymuint32 pos) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief portref を得る．
  virtual
  PtiPortRef*
  _portref(ymuint32 pos);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 内部向きの接続を表す式
  PtiPortRef* mPortRef;

};


//////////////////////////////////////////////////////////////////////
/// @brief port を表すクラス (portref リスト付き)
//////////////////////////////////////////////////////////////////////
class CptPort2 :
  public CptPort
{
  friend class CptFactory;

private:
  
  /// @brief コンストラクタ
  CptPort2(const FileRegion& file_region,
	   PtiPortRefArray portref_array,
	   const char* ext_name);
  
  /// @brief デストラクタ
  virtual
  ~CptPort2();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtPort の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内部のポート結線リストのサイズの取得
  /// @return 内部のポート結線リストのサイズ
  virtual
  ymuint32
  portref_num() const;

  /// @brief 内部のポート結線の取得
  /// @return 先頭の内部のポート結線
  virtual
  const PtPortRef*
  portref(ymuint32 pos) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief portref を得る．
  virtual
  PtiPortRef*
  _portref(ymuint32 pos);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // ポート参照式の配列
  PtiPortRefArray mPortRefArray;

};


//////////////////////////////////////////////////////////////////////
/// port reference のベース実装クラス
//////////////////////////////////////////////////////////////////////
class CptPortRef :
  public PtiPortRef
{
  friend class CptFactory;

protected:
  
  /// コンストラクタ
  CptPortRef(const FileRegion& file_region,
	     const char* name);
  
  /// デストラクタ
  virtual
  ~CptPortRef();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtPortRef の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////
  
  /// ファイル位置の取得
  virtual
  FileRegion
  file_region() const;

  /// @brief 名前の取得
  /// @return 名前
  virtual
  const char*
  name() const;
  
  /// @brief インデックスの取得
  /// @return インデックスを表す式
  /// @note このクラスでは NULL を返す．
  virtual
  const PtExpr*
  index() const;

  /// @brief 範囲指定モードの取得
  /// @retval kVpiNoRange 範囲指定なし
  /// @retval kVpiConstRange [ a : b ] のタイプ
  /// @retval kVpiPlusRange  [ a :+ b ] のタイプ
  /// @retval kVpiMinusRange [ a :- b ] のタイプ
  /// @note このクラスでは kVpiNoRange を返す．
  virtual
  tVpiRangeMode
  range_mode() const;
  
  /// @brief 範囲の左側の式の取得
  /// @return 範囲の左側の式
  /// @note このクラスでは NULL を返す．
  virtual
  const PtExpr*
  left_range() const;
  
  /// @brief 範囲の右側の式の取得
  /// @return 範囲の右側の式
  /// @note このクラスでは NULL を返す．
  virtual
  const PtExpr*
  right_range() const;
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 名前
  const char* mName;

};


//////////////////////////////////////////////////////////////////////
/// インデックスつきの port reference を表すクラス
//////////////////////////////////////////////////////////////////////
class CptPortRefI :
  public CptPortRef
{
public:
  
  /// コンストラクタ
  CptPortRefI(const FileRegion& file_region,
	      const char* name,
	      PtExpr* index);
  
  /// デストラクタ
  virtual
  ~CptPortRefI();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtPortRef の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////
  
  /// インデックスの取得
  virtual
  const PtExpr*
  index() const;
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // インデックス
  PtExpr* mIndex;

};


//////////////////////////////////////////////////////////////////////
/// 範囲指定つきの port reference を表すクラス
//////////////////////////////////////////////////////////////////////
class CptPortRefR :
  public CptPortRef
{
public:
  
  /// コンストラクタ
  CptPortRefR(const FileRegion& file_region,
	      const char* name,
	      tVpiRangeMode mode,
	      PtExpr* left,
	      PtExpr* right);
  
  /// デストラクタ
  virtual
  ~CptPortRefR();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtPortRef の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// 範囲指定モードの取得
  virtual
  tVpiRangeMode
  range_mode() const;
  
  /// 範囲の左側の式の取得
  virtual
  const PtExpr*
  left_range() const;
  
  /// 範囲の右側の式の取得
  virtual
  const PtExpr*
  right_range() const;
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 範囲指定モード
  tVpiRangeMode mMode;

  // 範囲のMSB
  PtExpr* mLeftRange;

  // 範囲のLSB
  PtExpr* mRightRange;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_PT_COMPACT_CPTMODULE_H
