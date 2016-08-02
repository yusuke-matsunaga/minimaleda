#ifndef LIBYM_VERILOG_PT_COMPACT_CPTMISC_H
#define LIBYM_VERILOG_PT_COMPACT_CPTMISC_H

/// @file libym_verilog/pt_compact/CptMisc.h
/// @brief CptMisc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CptMisc.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/pt/PtMisc.h"
#include "ym_verilog/pt/PtArray.h"
#include "ym_utils/FileRegion.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @brief delay control/event control/repeat control を表すベースクラス
//////////////////////////////////////////////////////////////////////
class CptControl :
  public PtControl
{
protected:
  
  /// @brief コンストラクタ
  CptControl();
  
  /// @brief デストラクタ
  virtual
  ~CptControl();


public:
  //////////////////////////////////////////////////////////////////////
  // PtControl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 遅延式の取得
  /// @retval 遅延を表す式 delay control の場合
  /// @retval NULL 上記以外
  /// @note デフォルトでは NULL を返す．
  virtual
  const PtExpr*
  delay() const;

  /// @brief イベントリストのサイズの取得
  /// @retval イベントリストのサイズ event control/repeat control の場合
  /// @retval 0 上記以外
  virtual
  ymuint32
  event_num() const;
  
  /// @brief イベントリストの要素の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < event_num() )
  /// @note event control/repeat control の場合のみ意味を持つ
  virtual
  const PtExpr*
  event(ymuint32 pos) const;

  /// @brief 繰り返し数の取得
  /// @retval 繰り返し数を表す式 repeat control の場合
  /// @retval NULL 上記以外
  /// @note デフォルトでは NULL を返す．
  virtual
  const PtExpr*
  rep_expr() const;

};


//////////////////////////////////////////////////////////////////////
/// @brief delay control を表すクラス
//////////////////////////////////////////////////////////////////////
class CptDelayControl :
  public CptControl
{
  friend class CptFactory;

private:
  
  /// @brief コンストラクタ
  CptDelayControl(const FileRegion& file_region,
		  PtExpr* value);
  
  /// @brief デストラクタ
  virtual
  ~CptDelayControl();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtControl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;

  /// @brief 型を返す．
  /// @note ここでは kPtDelayControl を返す．
  virtual
  tPtCtrlType
  type() const;
  
  /// @brief 遅延式を返す．
  virtual
  const PtExpr*
  delay() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の位置
  FileLoc mTopLoc;

  // 遅延を表す式
  PtExpr* mDelay;

};


//////////////////////////////////////////////////////////////////////
/// @brief event control を表すクラス
//////////////////////////////////////////////////////////////////////
class CptEventControl :
  public CptControl
{
  friend class CptFactory;

private:
  
  /// @brief コンストラクタ
  CptEventControl(const FileRegion& file_region,
		  PtExprArray event_array);
  
  /// @brief デストラクタ
  virtual
  ~CptEventControl();


public:
  //////////////////////////////////////////////////////////////////////
  // PtControl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;

  /// @brief 型を返す．
  /// @note ここでは kPtEventControl を返す．
  virtual
  tPtCtrlType
  type() const;

  /// @brief イベントリストのサイズの取得
  /// @retval イベントリストのサイズ event control/repeat control の場合
  /// @retval 0 上記以外
  virtual
  ymuint32
  event_num() const;
  
  /// @brief イベントリストの要素の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < event_num() )
  /// @note event control/repeat control の場合のみ意味を持つ
  virtual
  const PtExpr*
  event(ymuint32 pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // イベントの配列
  PtExprArray mEventArray;

};


//////////////////////////////////////////////////////////////////////
/// @brief repeat 形式の event を表すクラス
//////////////////////////////////////////////////////////////////////
class CptRepeatControl :
  public CptControl
{
  friend class CptFactory;

private:
  
  /// @brief コンストラクタ
  CptRepeatControl(const FileRegion& file_region,
		   PtExpr* expr,
		   PtExprArray event_array);
  
  /// @brief デストラクタ
  virtual
  ~CptRepeatControl();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtControl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;

  /// @brief 型を返す．
  /// @note ここでは kPtRepeatControl を返す．
  virtual
  tPtCtrlType
  type() const;

  /// @brief 繰り返し数を得る．
  virtual
  const PtExpr*
  rep_expr() const;

  /// @brief イベントリストのサイズの取得
  /// @retval イベントリストのサイズ event control/repeat control の場合
  /// @retval 0 上記以外
  virtual
  ymuint32
  event_num() const;
  
  /// @brief イベントリストの要素の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < event_num() )
  /// @note event control/repeat control の場合のみ意味を持つ
  virtual
  const PtExpr*
  event(ymuint32 pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 繰り返し数を表す式
  PtExpr* mRepExpr;

  // イベントのリスト
  PtExprArray mEventArray;

};


//////////////////////////////////////////////////////////////////////
/// @brief ordered_connection/named_connection を表すクラス
//////////////////////////////////////////////////////////////////////
class CptConnection :
  public PtConnection
{
protected:
  
  /// @brief コンストラクタ
  CptConnection(const FileRegion& file_region,
		PtExpr* expr);
  
  /// @brief デストラクタ
  virtual
  ~CptConnection();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtConnection に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を取出す．
  virtual
  FileRegion
  file_region() const;

  /// @brief 名前の取得
  /// @retval 名前 named connection の場合
  /// @retval "" ordered connection の場合
  /// @note デフォルトでは NULL を返す．
  virtual
  const char*
  name() const;

  /// @brief 式を取り出す．
  virtual
  const PtExpr*
  expr() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 接続を表す式
  PtExpr* mExpr;

};


//////////////////////////////////////////////////////////////////////
/// @brief orered_connection を表すクラス
//////////////////////////////////////////////////////////////////////
class CptOrderedCon :
  public CptConnection
{
  friend class CptFactory;

protected:
  
  /// @brief コンストラクタ
  CptOrderedCon(const FileRegion& file_region,
		PtExpr* expr);
  
  /// @brief デストラクタ
  virtual
  ~CptOrderedCon();
  
};


//////////////////////////////////////////////////////////////////////
/// @brief named_connection を表すクラス
//////////////////////////////////////////////////////////////////////
class CptNamedCon :
  public CptConnection
{
  friend class CptFactory;

protected:
  
  /// @brief コンストラクタ
  CptNamedCon(const FileRegion& file_region,
	      const char* name,
	      PtExpr* expr);
  
  /// @brief デストラクタ
  virtual
  ~CptNamedCon();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtConnection の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を取り出す．
  virtual
  const char*
  name() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パラメータ名/ポート名
  const char* mName;

};


//////////////////////////////////////////////////////////////////////
/// @brief strength を表すクラス
//////////////////////////////////////////////////////////////////////
class CptStrength :
  public PtStrength
{
  friend class CptFactory;
  
private:
  
  /// @brief drive strength を表すコンストラクタ
  CptStrength(const FileRegion& file_region,
	      tVpiStrength value1,
	      tVpiStrength value2);
  
  /// @brief charge strength を表すコンストラクタ
  CptStrength(const FileRegion& file_region,
	      tVpiStrength value1);
  
  /// @brief デストラクタ
  virtual
  ~CptStrength();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtStrength の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を取出す．
  virtual
  FileRegion
  file_region() const;

  /// @brief drive strength0 を返す．
  virtual
  tVpiStrength
  drive0() const;

  /// @brief drive strength1 を返す．
  virtual
  tVpiStrength
  drive1() const;

  /// @brief charge strength を返す．
  virtual
  tVpiStrength
  charge() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  tVpiStrength mValue1;
  tVpiStrength mValue2;
  tVpiStrength mValue3;
};


//////////////////////////////////////////////////////////////////////
/// @brief delay を表すクラス
//////////////////////////////////////////////////////////////////////
class CptDelay :
  public PtDelay
{
  friend class CptFactory;

private:

  /// @brief 一つの値をとるコンストラクタ
  CptDelay(const FileRegion& file_region,
	   PtExpr* value1);
  
  /// @brief 二つの値をとるコンストラクタ
  CptDelay(const FileRegion& file_region,
	   PtExpr* value1,
	   PtExpr* value2);
  
  /// @brief 三つの値をとるコンストラクタ
  CptDelay(const FileRegion& file_region,
	   PtExpr* value1,
	   PtExpr* value2,
	   PtExpr* value3);
  
  /// @brief デストラクタ
  virtual
  ~CptDelay();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDelay の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を取出す．
  virtual
  FileRegion
  file_region() const;

  /// @brief 値を取り出す．
  virtual
  const PtExpr*
  value(ymuint32 pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 値のリスト
  PtExpr* mValue[3];

};


//////////////////////////////////////////////////////////////////////
/// @brief 階層名を表すクラス
//////////////////////////////////////////////////////////////////////
class CptNameBranch :
  public PtNameBranch
{
public:

  /// @brief コンストラクタ
  CptNameBranch(const char* name);

  /// @brief デストラクタ
  virtual
  ~CptNameBranch();


public:
  //////////////////////////////////////////////////////////////////////
  // PtNameBranch の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を取り出す．
  virtual
  const char*
  name() const;

  /// @brief インデックスの有無のチェック
  /// @retval true インデックスを持っている時
  /// @retval false インデックスを持っていない時
  /// @note デフォルトで false を返す．
  virtual
  bool
  has_index() const;

  /// @brief インデックスの取得
  /// @return インデックスの値
  /// @note デフォルトで 0 を返す．
  virtual
  int
  index() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  const char* mName;
  
};


//////////////////////////////////////////////////////////////////////
/// @brief インデックスつきの階層名を表すクラス
//////////////////////////////////////////////////////////////////////
class CptNameBranchI :
  public CptNameBranch
{
public:

  /// @brief コンストラクタ
  CptNameBranchI(const char* name,
		 int index);

  /// @brief デストラクタ
  virtual
  ~CptNameBranchI();


public:
  //////////////////////////////////////////////////////////////////////
  // PtNameBranch の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief インデックスを持っている時 true を返す．
  virtual
  bool
  has_index() const;

  /// @brief インデックスを取り出す．
  virtual
  int
  index() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // インデックス
  int mIndex;

};


//////////////////////////////////////////////////////////////////////
/// @brief attribute_instance を表すクラス
//////////////////////////////////////////////////////////////////////
class CptAttrInst :
  public PtAttrInst
{
  friend class CptFactory;

private:
  
  /// @brief コンストラクタ
  CptAttrInst(PtAttrSpecArray as_array);
  
  /// @brief デストラクタ
  virtual
  ~CptAttrInst();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtAttrInst の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素数の取得
  /// @return 要素数
  virtual
  ymuint32
  attrspec_num() const;
    
  /// @brief 要素の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < attrspec_num() )
  virtual
  const PtAttrSpec*
  attrspec(ymuint32 pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // attr spec のリスト
  PtAttrSpecArray mAttrSpecArray;

};


//////////////////////////////////////////////////////////////////////
/// @brief attr_spec を表すクラス
//////////////////////////////////////////////////////////////////////
class CptAttrSpec :
  public PtAttrSpec
{
  friend class CptFactory;

private:

  /// @brief コンストラクタ
  CptAttrSpec(const FileRegion& file_region,
	      const char* name,
	      PtExpr* expr);
  
  /// @brief デストラクタ
  virtual
  ~CptAttrSpec();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtAttrSpec の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;

  /// @brief 名前を取り出す．
  virtual
  const char*
  name() const;
  
  /// @brief 式を取り出す．NULL の場合もある．
  virtual
  const PtExpr*
  expr() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 名前
  const char* mName;

  // 式
  PtExpr* mExpr;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_PT_COMPACT_CPTMISC_H
