#ifndef LIBYM_VERILOG_PT_SIMPLE_SPTITEM_H
#define LIBYM_VERILOG_PT_SIMPLE_SPTITEM_H

/// @file libym_verilog/pt_simple/SptItem.h
/// @brief SptItem のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SptItem.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/FileRegion.h"

#include "ym_verilog/pt/PtItem.h"
#include "ym_verilog/pt/PtArray.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// item の基底クラス
//////////////////////////////////////////////////////////////////////
class SptItem :
  public PtItem
{
protected:

  /// コンストラクタ
  SptItem(const FileRegion& file_region,
	  tPtItemType type);

  /// デストラクタ
  ~SptItem();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// ファイル位置の取得
  virtual
  FileRegion
  file_region() const;

  /// 型の取得
  virtual
  tPtItemType
  type() const;

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

  /// @brief 名前の取得
  /// @return 名前
  /// @note このクラスでは NULL を返す．
  virtual
  const char*
  name() const;

  /// @brief automatic 情報の取得
  /// @retval true automatic 宣言された task/function
  /// @retval false 上記以外
  /// @note このクラスでは false を返す．
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
  /// @param[in] pos 位置番号 ( 0 <= pos < size() )
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



private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 型
  tPtItemType mType;

};


//////////////////////////////////////////////////////////////////////
// defparam 文のヘッダ
//////////////////////////////////////////////////////////////////////
class SptDefParamH :
  public SptItem
{
  friend class SptFactory;

private:

  /// コンストラクタ
  SptDefParamH(const FileRegion& file_region,
	       PtDefParamArray dp_array);

  /// デストラクタ
  virtual
  ~SptDefParamH();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  // 要素数を返す．
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

  // 要素の配列
  PtDefParamArray mArray;

};


//////////////////////////////////////////////////////////////////////
// defparam 文を表すクラス
//////////////////////////////////////////////////////////////////////
class SptDefParam :
  public PtDefParam
{
  friend class SptFactory;

private:

  // コンストラクタ
  // value は常に定数式
  SptDefParam(const FileRegion& file_region,
	      PtNameBranchArray nb_array,
	      const char* tail_name,
	      PtExpr* value);

  // デストラクタ
  virtual
  ~SptDefParam();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDefParam の仮想関数
  //////////////////////////////////////////////////////////////////////

  // ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;

  /// @brief 階層ブランチの取得
  virtual
  PtNameBranchArray
  namebranch_array() const;

  // 末尾の名前を返す．
  virtual
  const char*
  name() const;

  // 値を返す．
  virtual
  const PtExpr*
  expr() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 階層ブランチの配列
  PtNameBranchArray mNbArray;

  // 末尾の名前
  const char* mName;

  // 値
  PtExpr* mExpr;

};


//////////////////////////////////////////////////////////////////////
// continuous assign 文のヘッダ
//////////////////////////////////////////////////////////////////////
class SptContAssignH :
  public SptItem
{
  friend class SptFactory;

private:

  // コンストラクタ
  SptContAssignH(const FileRegion& file_region,
		 PtStrength* strength,
		 PtDelay* delay,
		 PtContAssignArray ca_array);

  // デストラクタ
  virtual
  ~SptContAssignH();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  // strength を返す．
  virtual
  const PtStrength*
  strength() const;

  // delay を返す．
  virtual
  const PtDelay*
  delay() const;

  // 要素数を得る．
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

  // strength
  PtStrength* mStrength;

  // delay
  PtDelay* mDelay;

  // 要素の配列
  PtContAssignArray mArray;

};


//////////////////////////////////////////////////////////////////////
// continuous assign 文
//////////////////////////////////////////////////////////////////////
class SptContAssign :
  public PtContAssign
{
  friend class SptFactory;

private:

  // コンストラクタ
  SptContAssign(const FileRegion& fr,
		PtExpr* lhs,
		PtExpr* rhs);

  // デストラクタ
  virtual
  ~SptContAssign();


public:
  //////////////////////////////////////////////////////////////////////
  // PtContAssign の仮想関数
  //////////////////////////////////////////////////////////////////////

  // ファイル位置を取り出す．
  virtual
  FileRegion
  file_region() const;

  // 左辺式を取り出す．
  virtual
  const PtExpr*
  lhs() const;

  // 右辺式を取り出す．
  virtual
  const PtExpr*
  rhs() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 左辺式
  PtExpr* mLhs;

  // 右辺式
  PtExpr* mRhs;

};


//////////////////////////////////////////////////////////////////////
// inititial 文/always 文に共通なクラス
//////////////////////////////////////////////////////////////////////
class SptProcess :
  public SptItem
{
  friend class SptFactory;

private:

  // コンストラクタ
  SptProcess(const FileRegion& file_region,
	     tPtItemType type,
	     PtStmt* body);

  // デストラクタ
  virtual
  ~SptProcess();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  // 本体のステートメントを返す．
  virtual
  const PtStmt*
  body() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 本体
  PtStmt* mBody;

};


//////////////////////////////////////////////////////////////////////
// task と function に共通な基底クラス
//////////////////////////////////////////////////////////////////////
class SptTf :
  public SptItem
{
  friend class SptFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] type 型
  /// @param[in] name 名前
  /// @param[in] automatic automatic 属性
  /// @param[in] sign 符号の有無
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] data_type 関数の戻値の型
  /// @param[in] iohead_array IO宣言のリスト
  /// @param[in] paramhead_array parameter 宣言のリスト
  /// @param[in] lparamhead_array localparam 宣言のリスト
  /// @param[in] declhead_array 宣言のリスト
  /// @param[in] stmt 本体のステートメント
  SptTf(const FileRegion& file_region,
	tPtItemType type,
	const char* name,
	bool automatic,
	bool sign,
	PtExpr* left,
	PtExpr* right,
	tVpiVarType var_type,
	PtIOHeadArray iohead_array,
	PtDeclHeadArray paramhead_array,
	PtDeclHeadArray localparamhead_array,
	PtDeclHeadArray declhead_array,
	PtStmt* stmt);

  // デストラクタ
  virtual
  ~SptTf();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  // 名前を取り出す．
  virtual
  const char*
  name() const;

  // automatic なら true
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

  // 本体を取り出す．
  virtual
  const PtStmt*
  body() const;

  // 符号の有無の取得
  virtual
  bool
  is_signed() const;

  // 範囲の MSB を得る． 指定なしなら NULL
  virtual
  const PtExpr*
  left_range() const;

  // 範囲の LSB を得る． 指定なしなら NULL
  virtual
  const PtExpr*
  right_range() const;

  // データ型の取得
  virtual
  tVpiVarType
  data_type() const;

  // constant function の展開中の印をつける．
  virtual
  void
  set_in_use() const;

  // constant function の展開中の印を消す．
  virtual
  void
  clear_in_use() const;

  // 使用中(constant function として展開中)の時 true を返す．
  virtual
  bool
  is_in_use() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  const char* mName;

  // automatic 属性
  bool mAutomatic;

  // 符号
  bool mSigned;

  // 範囲のMSB
  PtExpr* mLeftRange;

  // 範囲のLSB
  PtExpr* mRightRange;

  // データ型
  tVpiVarType mDataType;

  // IO宣言の要素数
  ymuint32 mIOItemNum;

  // IO宣言の配列
  PtIOHeadArray mIOHeadArray;

  // paramter 宣言の配列
  PtDeclHeadArray mParamHeadArray;

  // localparam 宣言の配列
  PtDeclHeadArray mLocalparamHeadArray;

  // その他の宣言の配列
  PtDeclHeadArray mDeclHeadArray;

  // 本体
  PtStmt* mBody;

  // 使用中かどうかを示すフラグ
  mutable
  bool mInUse;

};


//////////////////////////////////////////////////////////////////////
// gate header のベース実装クラス
//////////////////////////////////////////////////////////////////////
class SptGateH :
  public SptItem
{
  friend class SptFactory;

private:

  // コンストラクタ
  SptGateH(const FileRegion& file_region,
	   tVpiPrimType prim_type,
	   PtStrength* strength,
	   PtDelay* delay,
	   PtInstArray elem_array);

  // デストラクタ
  virtual
  ~SptGateH();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  // プリミティブタイプ(vpiAndPrim など)を返す．
  virtual
  tVpiPrimType
  prim_type() const;

  // strength を返す．
  virtual
  const PtStrength*
  strength() const;

  // delay を返す．
  virtual
  const PtDelay*
  delay() const;

  // 要素数を取り出す．
  virtual
  ymuint32
  size() const;

  /// @brief module/UDP/gate instance 要素の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < size() )
  virtual
  const PtInst*
  inst(ymuint32 pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // プリミティブタイプ
  tVpiPrimType mPrimType;

  // strength
  PtStrength* mStrength;

  // delay
  PtDelay* mDelay;

  // 要素の配列
  PtInstArray mElemArray;

};


//////////////////////////////////////////////////////////////////////
// module/udp header のベース実装クラス
//////////////////////////////////////////////////////////////////////
class SptMuH :
  public SptItem
{
  friend class SptFactory;

private:

  // コンストラクタ
  SptMuH(const FileRegion& file_region,
	 const char* def_name,
	 PtConnectionArray con_array,
	 PtStrength* strength,
	 PtDelay* delay,
	 PtInstArray elem_array);

  // デストラクタ
  virtual
  ~SptMuH();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  // 定義名を返す．
  virtual
  const char*
  name() const;

  // strength を返す．
  virtual
  const PtStrength*
  strength() const;

  // delay を返す．
  virtual
  const PtDelay*
  delay() const;

  /// @brief パラメータ割り当てリストの取得
  virtual
  PtConnectionArray
  paramassign_array() const;

  // 要素数を取り出す．
  virtual
  ymuint32
  size() const;

  /// @brief module/UDP/gate instance 要素の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < size() )
  virtual
  const PtInst*
  inst(ymuint32 pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 定義名
  const char* mName;

  // パラメータ割り当ての配列
  PtConnectionArray mParamArray;

  // strength
  PtStrength* mStrength;

  // delay
  PtDelay* mDelay;

  // 要素のリスト
  PtInstArray mElemArray;

};


//////////////////////////////////////////////////////////////////////
// module instance/UDP instance のベース実装クラス
//////////////////////////////////////////////////////////////////////
class SptInst :
  public PtInst
{
  friend class SptFactory;

private:

  // コンストラクタ
  SptInst(const FileRegion& file_region,
	  const char* name,
	  PtExpr* left,
	  PtExpr* right,
	  PtConnectionArray con_array);

  // デストラクタ
  virtual
  ~SptInst();


public:
  //////////////////////////////////////////////////////////////////////
  // PtInst の仮想関数
  //////////////////////////////////////////////////////////////////////

  // ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;

  // 名前を返す．
  virtual
  const char*
  name() const;

  // range の MSB を取出す．
  virtual
  const PtExpr*
  left_range() const;

  // range の LSB を取出す．
  virtual
  const PtExpr*
  right_range() const;

  /// @brief ポート数の取得
  /// @return ポート数
  virtual
  ymuint32
  port_num() const;

  /// @brief ポートの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < port_num() )
  virtual
  const PtConnection*
  port(ymuint32 pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 名前
  const char* mName;

  // 範囲のMSB
  PtExpr* mLeftRange;

  // 範囲のLSB
  PtExpr* mRightRange;

  // ポート割り当ての配列
  PtConnectionArray mPortArray;

};


//////////////////////////////////////////////////////////////////////
// generate 文で生成される要素を持つ本体
//////////////////////////////////////////////////////////////////////
class SptGenBody
{
public:

  // コンストラクタ
  SptGenBody(PtDeclHeadArray decl_array,
	     PtItemArray item_array);

  // デストラクタ
  ~SptGenBody();


public:

  /// @brief 宣言ヘッダ配列の取得
  PtDeclHeadArray
  declhead_array() const { return mDeclArray; }

  /// @brief item 配列の取得
  PtItemArray
  item_array() const { return mItemArray; }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 宣言の配列
  PtDeclHeadArray mDeclArray;

  // 要素の配列
  PtItemArray mItemArray;

};


//////////////////////////////////////////////////////////////////////
// generate ブロックの基底クラス
//////////////////////////////////////////////////////////////////////
class SptGenerate :
  public SptItem
{
  friend class SptFactory;

private:

  // コンストラクタ
  SptGenerate(const FileRegion& file_region,
	      tPtItemType type,
	      const char* name,
	      PtDeclHeadArray decl_array,
	      PtItemArray item_array);

  // デストラクタ
  virtual
  ~SptGenerate();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  // 名前を返す．
  virtual
  const char*
  name() const;

  /// @brief 宣言のリストの取得
  virtual
  PtDeclHeadArray
  declhead_array() const;

  /// @brief item リストの取得
  virtual
  PtItemArray
  item_array() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  const char* mName;

  // 本体
  SptGenBody mBody;

};


//////////////////////////////////////////////////////////////////////
// gen_if 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class SptGenIf :
  public SptItem
{
  friend class SptFactory;

private:

  // コンストラクタ
  SptGenIf(const FileRegion& file_region,
	   PtExpr* cond,
	   PtDeclHeadArray then_decl_array,
	   PtItemArray then_item_array,
	   PtDeclHeadArray else_decl_array,
	   PtItemArray else_item_array);

  // デストラクタ
  virtual
  ~SptGenIf();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  // 条件式を返す．
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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 条件式
  PtExpr* mCond;

  // 成り立ったとき生成される本体
  SptGenBody mThenBody;

  // 成り立たなかったとき生成される本体
  SptGenBody mElseBody;

};


//////////////////////////////////////////////////////////////////////
// gen_case 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class SptGenCase :
  public SptItem
{
  friend class SptFactory;

private:

  // コンストラクタ
  SptGenCase(const FileRegion& file_region,
	     PtExpr* expr,
	     PtGenCaseItemArray item_array);

  // デストラクタ
  virtual
  ~SptGenCase();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  // 比較式を返す．
  virtual
  const PtExpr*
  expr() const;

  // case item の要素数を返す．
  virtual
  ymuint32
  size() const;

  // case item を返す．
  virtual
  const PtGenCaseItem*
  caseitem(ymuint32 pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 比較式
  PtExpr* mExpr;

  // case item の配列
  PtGenCaseItemArray mCaseItemArray;

};


//////////////////////////////////////////////////////////////////////
// gencaseitemを表すクラス
//////////////////////////////////////////////////////////////////////
class SptGenCaseItem :
  public PtGenCaseItem
{
  friend class SptFactory;

private:

  // コンストラクタ
  SptGenCaseItem(const FileRegion& file_region,
		 PtExprArray label_array,
		 PtDeclHeadArray decl_array,
		 PtItemArray item_array);

  // デストラクタ
  virtual
  ~SptGenCaseItem();


public:
  //////////////////////////////////////////////////////////////////////
  // PtGenCaseItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  // ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;

  /// @brief ラベルの数の取得
  /// @return ラベル数\n
  /// 0 の時は default の意味
  virtual
  ymuint32
  label_num() const;

  /// @brief ラベルの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < label_num() )
  virtual
  const PtExpr*
  label(ymuint32 pos) const;

  /// @brief 宣言のリストの取得
  virtual
  PtDeclHeadArray
  declhead_array() const;

  /// @brief item リストの取得
  virtual
  PtItemArray
  item_array() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // ラベルの配列
  PtExprArray mLabelArray;

  // 生成される本体
  SptGenBody mBody;

};


//////////////////////////////////////////////////////////////////////
// gen_for 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class SptGenFor :
  public SptItem
{
  friend class SptFactory;

private:

  // コンストラクタ
  SptGenFor(const FileRegion& file_region,
	    const char* loop_var,
	    PtExpr* init_expr,
	    PtExpr* cond,
	    PtExpr* next_expr,
	    const char* block_name,
	    PtDeclHeadArray decl_array,
	    PtItemArray item_array);

  // デストラクタ
  virtual
  ~SptGenFor();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  // 名前を返す．
  virtual
  const char*
  name() const;

  /// @brief 宣言のリストの取得
  virtual
  PtDeclHeadArray
  declhead_array() const;

  /// @brief item リストの取得
  virtual
  PtItemArray
  item_array() const;

  // 繰り返し制御用の変数名を返す．
  virtual
  const char*
  loop_var() const;

  // 初期化文の右辺を返す．
  virtual
  const PtExpr*
  init_expr() const;

  // 繰り返し条件を返す．
  virtual
  const PtExpr*
  expr() const;

  // 増加文の右辺を返す．
  virtual
  const PtExpr*
  next_expr() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  const char* mName;

  // 繰り返し制御用の変数名
  const char* mLoopVar;

  // 初期化文の右辺
  PtExpr* mInitExpr;

  // 繰り返し条件
  PtExpr* mCond;

  // 増加文の右辺
  PtExpr* mNextExpr;

  // 生成される本体
  SptGenBody mBody;

};


//////////////////////////////////////////////////////////////////////
// specify_block item を表すクラス
//////////////////////////////////////////////////////////////////////
class SptSpecItem :
  public SptItem
{
  friend class SptFactory;

private:

  // コンストラクタ
  SptSpecItem(const FileRegion& file_region,
	      tVpiSpecItemType id,
	      PtExprArray terminal_array);

  // デストラクタ
  virtual
  ~SptSpecItem();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  // specify block item の種類を返す．
  virtual
  tVpiSpecItemType
  specitem_type() const;

  // ターミナルリストの要素数を返す．
  virtual
  ymuint32
  size() const;

  /// @brief ターミナルの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < size() )
  virtual
  const PtExpr*
  terminal(ymuint32 pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // トークン番号
  tVpiSpecItemType mId;

  // ターミナルリスト
  PtExprArray mTerminals;

};


//////////////////////////////////////////////////////////////////////
// path 仕様 を表すクラス
//////////////////////////////////////////////////////////////////////
class SptSpecPath :
  public SptItem
{
  friend class SptFactory;

private:

  // コンストラクタ
  SptSpecPath(const FileRegion& file_region,
	      tVpiSpecPathType id,
	      PtExpr* expr,
	      PtPathDecl* path_decl);

  // デストラクタ
  virtual
  ~SptSpecPath();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  // specify block path の種類を返す．
  virtual
  tVpiSpecPathType
  specpath_type() const;

  // モジュールパスの式を返す．
  virtual
  const PtExpr*
  expr() const;

  // パス記述を返す．
  virtual
  const PtPathDecl*
  path_decl() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // トークン番号
  tVpiSpecPathType mId;

  // モジュールパスの式
  PtExpr* mExpr;

  // パス記述
  PtPathDecl* mPathDecl;

};


//////////////////////////////////////////////////////////////////////
// path_delay_declaration を表すクラス
//////////////////////////////////////////////////////////////////////
class SptPathDecl :
  public PtPathDecl
{
  friend class SptFactory;

private:

  // コンストラクタ
  SptPathDecl(const FileRegion& file_region,
	      int edge,
	      PtExprArray input_array,
	      int input_pol,
	      int op,
	      PtExprArray output_array,
	      int output_pol,
	      PtExpr* expr,
	      PtPathDelay* path_delay);

  // デストラクタ
  virtual
  ~SptPathDecl();


public:
  //////////////////////////////////////////////////////////////////////
  // PtPathDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  // ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;

  // edge_descriptor を取り出す．
  // 0の場合もある．
  virtual
  int
  edge() const;

  /// @brief 入力リストの要素数の取得
  /// @return 入力リストの要素数
  virtual
  ymuint32
  input_num() const;

  /// @brief 入力の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
  virtual
  const PtExpr*
  input(ymuint32 pos) const;

  // 入力の極性を取り出す．
  // 0の場合もありうる．
  virtual
  int
  input_pol() const;

  // パス記述子(?)を得る．vpiParallel か vpiFull
  virtual
  int
  op() const;

  /// @brief 出力リストの要素数の取得
  /// @return 出力リストの要素数
  virtual
  ymuint32
  output_num() const;

  /// @brief 出力の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < output_num() )
  virtual
  const PtExpr*
  output(ymuint32 pos) const;

  // 出力の極性を取り出す．
  // 0の場合もありうる．
  virtual
  int
  output_pol() const;

  // 式を取り出す．
  // NULL の場合もありうる．
  virtual
  const PtExpr*
  expr() const;

  // path_delay_value を取り出す．
  virtual
  const PtPathDelay*
  path_delay() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  int mEdge;
  PtExprArray mInputs;
  int mInputPol;
  int mOp;
  PtExprArray mOutputs;
  int mOutputPol;
  PtExpr* mExpr;
  PtPathDelay* mPathDelay;
};


//////////////////////////////////////////////////////////////////////
// path_delay_value を表すクラス
//////////////////////////////////////////////////////////////////////
class SptPathDelay :
  public PtPathDelay
{
  friend class SptFactory;

private:

  // コンストラクタ
  SptPathDelay(const FileRegion& file_region,
	       PtExpr* value1);
  SptPathDelay(const FileRegion& file_region,
	       PtExpr* value1,
	       PtExpr* value2);
  SptPathDelay(const FileRegion& file_region,
	       PtExpr* value1,
	       PtExpr* value2,
	       PtExpr* value3);
  SptPathDelay(const FileRegion& file_region,
	       PtExpr* value1,
	       PtExpr* value2,
	       PtExpr* value3,
	       PtExpr* value4,
	       PtExpr* value5,
	       PtExpr* value6);
  SptPathDelay(const FileRegion& file_region,
	       PtExpr* value1,
	       PtExpr* value2,
	       PtExpr* value3,
	       PtExpr* value4,
	       PtExpr* value5,
	       PtExpr* value6,
	       PtExpr* value7,
	       PtExpr* value8,
	       PtExpr* value9,
	       PtExpr* value10,
	       PtExpr* value11,
	       PtExpr* value12);

  // デストラクタ
  virtual
  ~SptPathDelay();


public:
  //////////////////////////////////////////////////////////////////////
  // PtPathDelay の仮想関数
  //////////////////////////////////////////////////////////////////////

  // ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;

  // 値を取り出す．
  // 0の場合もある．
  virtual
  const PtExpr*
  value(ymuint32 pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // ディレイ値
  PtExpr* mValues[12];

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_PT_SIMPLE_SPTITEM_H
