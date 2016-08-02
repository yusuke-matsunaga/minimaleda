#ifndef LIBYM_VERILOG_PT_SIMPLE_SPTMODULE_H
#define LIBYM_VERILOG_PT_SIMPLE_SPTMODULE_H

/// @file libym_verilog/pt_simple/SptModule.h
/// @brief SptModule のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SptModule.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/FileRegion.h"

#include "ym_verilog/pt/PtModule.h"
#include "PtiDecl.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// module を表すノード
//////////////////////////////////////////////////////////////////////
class SptModule :
  public PtModule
{
  friend class SptFactory;

private:
  
  /// コンストラクタ
  SptModule(const FileRegion& file_region,
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
	    PtIOHeadArray iodecl_array,
	    PtDeclHeadArray paramdecl_array,
	    PtDeclHeadArray localparamdecl_array,
	    PtDeclHeadArray decl_array,
	    PtItemArray item_array);
  
  /// デストラクタ
  virtual
  ~SptModule();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtModule の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// ファイル位置の取得
  virtual
  FileRegion
  file_region() const;
  
  /// 名前の取得
  virtual
  const char*
  name() const;
  
  /// macromodule 情報の取得
  virtual
  bool
  is_macromodule() const;
  
  /// cell 情報の取得
  virtual
  bool
  is_cell() const;
    
  /// protect 情報の取得
  virtual
  bool
  is_protected() const;
  
  /// time unit の取得
  virtual
  int
  time_unit() const;
  
  /// time precision の取得
  virtual
  int
  time_precision() const;
  
  /// default net type の取得
  virtual
  tVpiNetType
  nettype() const;
  
  /// unconnected drive の取得
  virtual
  tVpiUnconnDrive
  unconn_drive() const;
  
  /// default delay mode の取得
  virtual
  tVpiDefDelayMode
  delay_mode() const;
  
  /// default decay time の取得
  virtual
  int
  decay_time() const;

  /// portfaults 情報の取得
  virtual
  bool
  portfaults() const;
  
  /// suppress_faults 情報の取得
  virtual
  bool
  suppress_faults() const;
  
  /// config 情報の取得
  virtual
  const string&
  config() const;
  
  /// library 情報の取得
  virtual
  const string&
  library() const;
  
  /// cell 情報の取得
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
  /// @param[in] pos 位置番号 ( 0 <= pos < port_num() )
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

  /// 関数名から関数の検索
  virtual
  const PtItem*
  find_function(const char* name) const;

  /// top_module フラグを下ろす
  virtual
  void
  clear_topmodule() const;
  
  /// top module のチェック
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
  mutable
  int mFlags;
    
  // decay time
  int mDefDecayTime;
  
  // config 情報
  string mConfig;
  
  // library 情報
  string mLibrary;
  
  // cell 情報
  string mCell;

  // パラメータポート宣言の配列
  PtDeclHeadArray mParamPortArray;

  // ポートの配列
  PtiPortArray mPortArray;

  // 入出力宣言の配列
  PtIOHeadArray mIOHeadArray;
  
  // 入出力宣言の要素数
  ymuint32 mIODeclNum;
  
  // parameter 宣言の配列
  PtDeclHeadArray mParamHeadArray;

  // localparam 宣言の配列
  PtDeclHeadArray mLocalparamHeadArray;
  
  // 宣言リスト
  PtDeclHeadArray mDeclHeadArray;
  
  // 要素のリスト
  PtItemArray mItemArray;

  // 関数定義の辞書
  hash_map<string, const PtItem*> mFuncDic;

};


//////////////////////////////////////////////////////////////////////
/// port を表すクラス
//////////////////////////////////////////////////////////////////////
class SptPort :
  public PtiPort
{
  friend class SptFactory;

private:
  
  /// コンストラクタ
  SptPort(const FileRegion& file_region,
	  PtiPortRefArray portref_array,
	  const char* ext_name);
  
  /// デストラクタ
  virtual
  ~SptPort();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtPort の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////
  
  /// ファイル位置の取得
  virtual
  FileRegion
  file_region() const;

  /// 外向の名前の取得
  virtual
  const char*
  ext_name() const;

  /// 内部のポート結線リストのサイズの取得
  virtual
  ymuint32
  portref_num() const;

  /// 内部のポート結線の取得
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

  // ファイル位置
  FileRegion mFileRegion;

  // 外部向きの名前
  const char* mExtName;
  
  // portref の配列
  PtiPortRefArray mPortRefArray;
  
};


//////////////////////////////////////////////////////////////////////
/// port reference を表すクラス
//////////////////////////////////////////////////////////////////////
class SptPortRef :
  public PtiPortRef
{
  friend class SptFactory;

private:
  
  /// コンストラクタ
  SptPortRef(const FileRegion& file_region,
	     const char* name,
	     PtExpr* index,
	     tVpiRangeMode mode,
	     PtExpr* left,
	     PtExpr* right);
  
  // デストラクタ
  virtual
  ~SptPortRef();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtPortRef の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////
  
  /// ファイル位置の取得
  virtual
  FileRegion
  file_region() const;
  
  /// 名前の取得
  virtual
  const char*
  name() const;
  
  /// インデックスの取得
  virtual
  const PtExpr*
  index() const;

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

  // ファイル位置
  FileRegion mFileRegion;

  // 名前
  const char* mName;

  // インデックス
  PtExpr* mIndex;

  // range のモード
  tVpiRangeMode mMode;

  // 範囲のMSB
  PtExpr* mLeftRange;

  // 範囲のLSB
  PtExpr* mRightRange;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_PT_SIMPLE_SPTMODULE_H
