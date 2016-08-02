#ifndef LIBYM_VERILOG_PT_COMPACT_CPTITEM_H
#define LIBYM_VERILOG_PT_COMPACT_CPTITEM_H

/// @file libym_verilog/pt_compact/CptItem.h
/// @brief CptItem のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CptItem.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/pt/PtItem.h"
#include "ym_verilog/pt/PtArray.h"

#include "ym_utils/FileRegion.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// item の基底クラス
//////////////////////////////////////////////////////////////////////
class CptItem :
  public PtItem
{
protected:
  
  /// @brief コンストラクタ
  CptItem();

  /// @brief デストラクタ
  virtual
  ~CptItem();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief プリミティブタイプの取得
  /// @return プリミティブタイプ
  /// @note このクラスでは kVpiAndPrim を返す．
  virtual
  tVpiPrimType
  prim_type() const;

  /// @brief strength の取得
  /// @return 信号強度
  /// @note このクラスでは NULL を返す．
  virtual
  const PtStrength*
  strength() const;

  /// @brief delay の取得
  /// @return 遅延
  /// @note このクラスでは NULL を返す．
  virtual
  const PtDelay*
  delay() const;
  
  /// @brief パラメータ割り当てリストの取得
  virtual
  PtConnectionArray
  paramassign_array() const;

  /// @brief 要素数の取得
  /// @return 要素数
  /// @note このクラスでは 0 を返す．
  virtual
  ymuint32
  size() const;

  /// @brief defparam 要素の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < size() )
  virtual
  const PtDefParam*
  defparam(ymuint32 pos) const;

  /// @brief continuous assign 要素の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < size() )
  virtual
  const PtContAssign*
  contassign(ymuint32 pos) const;

  /// @brief module/UDP/gate instance 要素の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < size() )
  virtual
  const PtInst*
  inst(ymuint32 pos) const;

  /// @brief automatic 情報の取得
  /// @retval true automatic 宣言された task/function
  /// @retval false 上記以外
  virtual
  bool
  automatic() const;

  /// @brief 名前の取得
  /// @return 名前
  /// @note このクラスでは NULL を返す．
  virtual
  const char*
  name() const;
  
  /// @brief IO宣言の要素数の取得
  virtual
  ymuint32
  ioitem_num() const;
  
  /// @brief IO宣言リストの配列の取得
  virtual
  PtIOHeadArray
  iohead_array() const;

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

  /// @brief 本体のステートメントの取得
  /// @return 本体のステートメント
  /// @note このクラスでは NULL を返す．
  virtual
  const PtStmt*
  body() const;

  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  /// @note このクラスでは false を返す．
  virtual
  bool
  is_signed() const;

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

  /// @brief 戻値のデータ型の取得
  /// @return 戻値のデータ型
  /// @note このクラスでは kVpiVarNone を返す．
  virtual
  tVpiVarType
  data_type() const;

  /// @brief constant function の展開中の印をつける．
  /// @note このクラスではなにもしない．
  virtual
  void
  set_in_use() const;

  /// @brief constant function の展開中の印を消す．
  /// @note このクラスではなにもしない．
  virtual
  void
  clear_in_use() const;

  /// @brief 使用中(constant function として展開中)のチェック
  /// @return 使用中の時 true を返す．
  /// @note このクラスでは false を返す．
  virtual
  bool
  is_in_use() const;

  /// @brief specify block item の種類の取得
  /// @return specify block item の種類
  /// @note このクラスでは kVpiPulsestyleOnEvent を返す．
  virtual
  tVpiSpecItemType
  specitem_type() const;

  /// @brief specify block path の種類の取得
  /// @return specify block path の種類
  /// @note このクラスでは kVpiSpecPathNull を返す．
  virtual
  tVpiSpecPathType
  specpath_type() const;
  
  /// @brief ターミナルの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < size() )
  virtual
  const PtExpr*
  terminal(ymuint32 pos) const;

  /// @brief パス記述の取得
  /// @return パス記述
  /// @note このクラスでは NULL を返す．
  virtual
  const PtPathDecl*
  path_decl() const;
  
  /// @brief 条件式の取得
  /// @return 条件式
  /// @note このクラスでは NULL を返す．
  virtual
  const PtExpr*
  expr() const;
  
  /// @brief 条件が成り立ったときに生成される宣言ヘッダ配列の取得
  virtual
  PtDeclHeadArray
  then_declhead_array() const;
  
  /// @brief 条件が成り立ったときに生成される item 配列の取得
  virtual
  PtItemArray
  then_item_array() const;
  
  /// @brief 条件が成り立たなかったときに生成される宣言ヘッダ配列の取得
  virtual
  PtDeclHeadArray
  else_declhead_array() const;
  
  /// @brief 条件が成り立たなかったときに生成される item 配列の取得
  virtual
  PtItemArray
  else_item_array() const;

  /// @brief case item の取得
  /// @note このクラスでは NULL を返す．
  virtual
  const PtGenCaseItem*
  caseitem(ymuint32 pos) const;

  /// @brief 繰り返し制御用の変数名の取得
  /// @return 繰り返し制御用の変数名
  /// @note このクラスでは NULL を返す．
  virtual
  const char*
  loop_var() const;

  /// @brief 初期化文の右辺の取得
  /// @return 初期化文の右辺
  /// @note このクラスでは NULL を返す．
  virtual
  const PtExpr*
  init_expr() const;

  /// @brief 増加文の右辺の取得
  /// @return 増加文の右辺
  /// @note このクラスでは NULL を返す．
  virtual
  const PtExpr*
  next_expr() const;

};


//////////////////////////////////////////////////////////////////////
/// @brief defparam 文のヘッダ
//////////////////////////////////////////////////////////////////////
class CptDefParamH :
  public CptItem
{
  friend class CptFactory;

protected:
  
  /// @brief コンストラクタ
  CptDefParamH(const FileRegion& file_region,
	       PtDefParamArray dp_array);
  
  /// @brief デストラクタ
  virtual
  ~CptDefParamH();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置の取得
  virtual
  FileRegion
  file_region() const;

  /// @brief 型を返す．
  /// @note ここでは kPtItem_DefParam を返す．
  virtual
  tPtItemType
  type() const;

  /// @brief 要素数を返す．
  virtual
  ymuint32
  size() const;

  /// @brief defparam 要素の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < size() )
  virtual
  const PtDefParam*
  defparam(ymuint32 pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;
  
  // 要素の配列
  PtDefParamArray mArray;

};


//////////////////////////////////////////////////////////////////////
/// @brief defparam 文を表すクラス
//////////////////////////////////////////////////////////////////////
class CptDefParam :
  public PtDefParam
{
  friend class CptFactory;

protected:
  
  /// @brief コンストラクタ
  /// @note value は常に定数式
  CptDefParam(const FileRegion& file_region,
	      PtNameBranchArray nb_array,
	      const char* tail_name,
	      PtExpr* value);
  
  /// @brief デストラクタ
  virtual
  ~CptDefParam();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtDefParam の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置の取得
  /// @return ファイル位置
  virtual
  FileRegion
  file_region() const;
  
  /// @brief 階層ブランチの取得
  PtNameBranchArray
  namebranch_array() const;

  /// @brief 末尾の名前の取得
  /// @return 末尾の名前
  virtual
  const char*
  name() const;

  /// @brief 値の取得
  /// @return 値を表す式
  virtual
  const PtExpr*
  expr() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の位置
  FileLoc mTopLoc;

  // 階層ブランチのリスト
  PtNameBranchArray mNbArray;

  // 末尾の名前
  const char* mName;

  // 値
  PtExpr* mExpr;
  
};


//////////////////////////////////////////////////////////////////////
/// @brief continuous assign 文のヘッダ
//////////////////////////////////////////////////////////////////////
class CptContAssignH :
  public CptItem
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptContAssignH(const FileRegion& file_region,
		 PtContAssignArray ca_array);
  
  /// @brief デストラクタ
  virtual
  ~CptContAssignH();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;

  /// @brief 型を返す．
  /// @note ここでは kPtItem_ContAssign を返す．
  virtual
  tPtItemType
  type() const;

  /// @brief 要素数を得る．
  virtual
  ymuint32
  size() const;

  /// @brief continuous assign 要素の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < size() )
  virtual
  const PtContAssign*
  contassign(ymuint32 pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 要素の配列
  PtContAssignArray mArray;

};


//////////////////////////////////////////////////////////////////////
/// @brief strength つきの continuous assign 文のヘッダ
//////////////////////////////////////////////////////////////////////
class CptContAssignHS :
  public CptContAssignH
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptContAssignHS(const FileRegion& file_region,
		  PtStrength* str,
		  PtContAssignArray ca_array);
  
  /// @brief デストラクタ
  virtual
  ~CptContAssignHS();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtContAssignH の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief strength を返す．
  virtual
  const PtStrength*
  strength() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // strength
  PtStrength* mStrength;

};


//////////////////////////////////////////////////////////////////////
/// @brief delay つきの continuous assign 文のヘッダ
//////////////////////////////////////////////////////////////////////
class CptContAssignHD :
  public CptContAssignH
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptContAssignHD(const FileRegion& file_region,
		  PtDelay* delay,
		  PtContAssignArray ca_array);
  
  /// @brief デストラクタ
  virtual
  ~CptContAssignHD();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtContAssignH の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief delay を返す．
  virtual
  const PtDelay*
  delay() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // delay
  PtDelay* mDelay;

};


//////////////////////////////////////////////////////////////////////
/// @brief strength と delay つきの continuous assign 文のヘッダ
//////////////////////////////////////////////////////////////////////
class CptContAssignHSD :
  public CptContAssignH
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptContAssignHSD(const FileRegion& file_region,
		   PtStrength* str,
		   PtDelay* delay,
		   PtContAssignArray ca_array);
  
  /// @brief デストラクタ
  virtual
  ~CptContAssignHSD();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtContAssignH の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief strength を返す．
  virtual
  const PtStrength*
  strength() const;

  /// @brief delay を返す．
  virtual
  const PtDelay*
  delay() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // strength
  PtStrength* mStrength;

  // delay
  PtDelay* mDelay;

};


//////////////////////////////////////////////////////////////////////
/// @brief continuous assign 文
//////////////////////////////////////////////////////////////////////
class CptContAssign :
  public PtContAssign
{
  friend class CptFactory;

protected:
  
  /// @brief コンストラクタ
  CptContAssign(PtExpr* lhs,
		PtExpr* rhs);
  
  /// @brief デストラクタ
  virtual
  ~CptContAssign();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtContAssign の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を取り出す．
  virtual
  FileRegion
  file_region() const;

  /// @brief 左辺式を取り出す．
  virtual
  const PtExpr*
  lhs() const;

  /// @brief 右辺式を取り出す．
  virtual
  const PtExpr*
  rhs() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 左辺式
  PtExpr* mLhs;

  // 右辺式
  PtExpr* mRhs;

};


//////////////////////////////////////////////////////////////////////
/// @brief inititial 文/always 文に共通なクラス
//////////////////////////////////////////////////////////////////////
class CptProcess :
  public CptItem
{
  friend class CptFactory;

protected:
  
  /// @brief コンストラクタ
  CptProcess(const FileRegion& file_region,
	     PtStmt* body);

  /// @brief デストラクタ
  virtual
  ~CptProcess();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を取り出す．
  virtual
  FileRegion
  file_region() const;

  /// @brief 本体のステートメントを返す．
  virtual
  const PtStmt*
  body() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭のファイル位置
  FileLoc mTopLoc;

  // 本体
  PtStmt* mBody;

};


//////////////////////////////////////////////////////////////////////
/// @brief inititial 文
//////////////////////////////////////////////////////////////////////
class CptInitial :
  public CptProcess
{
  friend class CptFactory;

protected:
  
  /// @brief コンストラクタ
  CptInitial(const FileRegion& file_region,
	     PtStmt* body);

  /// @brief デストラクタ
  virtual
  ~CptInitial();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  /// @note ここでは kPtItem_Initial を返す．
  virtual
  tPtItemType
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @brief always 文
//////////////////////////////////////////////////////////////////////
class CptAlways :
  public CptProcess
{
  friend class CptFactory;

protected:
  
  /// @brief コンストラクタ
  CptAlways(const FileRegion& file_region,
	    PtStmt* body);
  
  /// @brief デストラクタ
  virtual
  ~CptAlways();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  /// @note ここでは kPtItem_Always を返す．
  virtual
  tPtItemType
  type() const;
  
};


//////////////////////////////////////////////////////////////////////
/// @brief task と function に共通な基底クラス
//////////////////////////////////////////////////////////////////////
class CptTf :
  public CptItem
{
protected:
  
  /// @brief コンストラクタ
  CptTf(const FileRegion& file_region,
	const char* name,
	bool automatic,
	PtIOHeadArray iohead_array,
	PtDeclHeadArray paramhead_array,
	PtDeclHeadArray lparamhead_array,
	PtDeclHeadArray declhead_array,
	PtStmt* stmt);

  /// @brief デストラクタ
  virtual
  ~CptTf();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;
  
  /// @brief 名前を取り出す．
  virtual
  const char*
  name() const;

  /// @brief automatic なら true
  virtual
  bool
  automatic() const;
  
  /// @brief IO宣言の要素数の取得
  virtual
  ymuint32
  ioitem_num() const;
  
  /// @brief IO宣言リストの配列の取得
  virtual
  PtIOHeadArray
  iohead_array() const;

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
  
  /// @brief 本体を取り出す．
  virtual
  const PtStmt*
  body() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 名前
  const char* mName;

  // automatic 属性
  bool mAutomatic;
  
  // IO宣言の要素数
  ymuint32 mIOItemNum;
  
  // IO宣言の配列
  PtIOHeadArray mIOHeadArray;

  // parameter の宣言の配列
  PtDeclHeadArray mParamHeadArray;

  // localparam の宣言の配列
  PtDeclHeadArray mLparamHeadArray;

  // その他の宣言の配列
  PtDeclHeadArray mDeclHeadArray;
  
  // 本体
  PtStmt* mBody;

};


//////////////////////////////////////////////////////////////////////
/// @brief task を表すクラス
//////////////////////////////////////////////////////////////////////
class CptTask :
  public CptTf
{
  friend class CptFactory;

private:
  
  /// @brief コンストラクタ
  CptTask(const FileRegion& file_region,
	  const char* name,
	  bool automatic,
	  PtIOHeadArray iohead_array,
	  PtDeclHeadArray paramhead_array,
	  PtDeclHeadArray lparamhead_array,
	  PtDeclHeadArray declhead_array,
	  PtStmt* stmt);
  
  /// @brief デストラクタ
  virtual
  ~CptTask();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  /// @note ここでは kPtItem_Task を返す．
  virtual
  tPtItemType
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @brief function を表すクラス
//////////////////////////////////////////////////////////////////////
class CptFunction :
  public CptTf
{
  friend class CptFactory;

protected:
  
  /// @brief コンストラクタ
  CptFunction(const FileRegion& file_region,
	      const char* name,
	      bool automatic,
	      bool sign,
	      PtIOHeadArray iohead_array,
	      PtDeclHeadArray paramhead_array,
	      PtDeclHeadArray lparamhead_array,
	      PtDeclHeadArray declhead_array,
	      PtStmt* stmt);
  
  /// @brief デストラクタ
  virtual
  ~CptFunction();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  /// @note ここでは kPtItem_Function を返す．
  virtual
  tPtItemType
  type() const;


public:
  //////////////////////////////////////////////////////////////////////
  // PtTf の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 符号の有無
  virtual
  bool
  is_signed() const;

  /// @brief constant function の展開中の印をつける．
  virtual
  void
  set_in_use() const;

  /// @brief constant function の展開中の印を消す．
  virtual
  void
  clear_in_use() const;

  /// @brief 使用中(constant function として展開中)の時 true を返す．
  virtual
  bool
  is_in_use() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 符号の有無
  bool mSigned;

  // 使用中かどうかを示すフラグ
  mutable bool mInUse;

};


//////////////////////////////////////////////////////////////////////
/// @brief ビットベクタタイプの function を表すノード
//////////////////////////////////////////////////////////////////////
class CptSizedFunc :
  public CptFunction
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptSizedFunc(const FileRegion& file_region,
	       const char* name,
	       bool automatic,
	       bool sign,
	       PtExpr* left,
	       PtExpr* right,
	       PtIOHeadArray iohead_array,
	       PtDeclHeadArray paramhead_array,
	       PtDeclHeadArray lparamhead_array,
	       PtDeclHeadArray declhead_array,
	       PtStmt* stmt);
  
  /// @brief デストラクタ
  virtual
  ~CptSizedFunc();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtTf の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲の MSB を得る．
  virtual
  const PtExpr*
  left_range() const;

  /// @brief 範囲の LSB を得る．
  virtual
  const PtExpr*
  right_range() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 範囲のMSB
  PtExpr* mLeftRange;

  // 範囲のLSB
  PtExpr* mRightRange;

};


//////////////////////////////////////////////////////////////////////
/// @brief 組み込み型の function を表すノード
//////////////////////////////////////////////////////////////////////
class CptTypedFunc :
  public CptFunction
{
  friend class CptFactory;

protected:
  
  /// @brief コンストラクタ
  CptTypedFunc(const FileRegion& file_region,
	       const char* name,
	       bool automatic,
	       bool sign,
	       tVpiVarType dat_type,
	       PtIOHeadArray iohead_array,
	       PtDeclHeadArray paramhead_array,
	       PtDeclHeadArray lparamhead_array,
	       PtDeclHeadArray declhead_array,
	       PtStmt* stmt);
  
  /// @brief デストラクタ
  virtual
  ~CptTypedFunc();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 戻値の型を返す．
  virtual
  tVpiVarType
  data_type() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 戻値の型
  tVpiVarType mDataType;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_PT_COMPACT_CPTITEM_H
