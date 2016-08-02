
/// @file libym_verilog/pt_simple/SptItem.cc
/// @brief SptItem の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SptItem.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "SptItem.h"
#include "SptFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// item の基底クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
// @param type 種類
SptItem::SptItem(const FileRegion& file_region,
		 tPtItemType type) :
  mFileRegion(file_region),
  mType(type)
{
}

// デストラクタ
SptItem::~SptItem()
{
}

// ファイル位置の取得
// @return ファイル位置
FileRegion
SptItem::file_region() const
{
  return mFileRegion;
}

// 型の取得
// @return 型
tPtItemType
SptItem::type() const
{
  return mType;
}

// @brief プリミティブタイプの取得
// @return プリミティブタイプ
// このクラスでは kVpiAndPrim を返す．
tVpiPrimType
SptItem::prim_type() const
{
  return kVpiAndPrim; // ダミー
}

// @brief strength の取得
// @return 信号強度
// このクラスでは NULL を返す．
const PtStrength*
SptItem::strength() const
{
  return NULL;
}

// @brief delay の取得
// @return 遅延
// このクラスでは NULL を返す．
const PtDelay*
SptItem::delay() const
{
  return NULL;
}

// @brief パラメータ割り当てリストの取得
PtConnectionArray
SptItem::paramassign_array() const
{
  return PtConnectionArray();
}

// @brief 要素数の取得
// @return 要素数
// このクラスでは 0 を返す．
ymuint32
SptItem::size() const
{
  return 0;
}

// @brief defparam 要素の取得
// @param[in] pos 位置番号 ( 0 <= pos < size() )
const PtDefParam*
SptItem::defparam(ymuint32 pos) const
{
  return NULL;
}

// @brief continuous assign 要素の取得
// @param[in] pos 位置番号 ( 0 <= pos < size() )
const PtContAssign*
SptItem::contassign(ymuint32) const
{
  return NULL;
}

// @brief module/UDP/gate instance 要素の取得
// @param[in] pos 位置番号 ( 0 <= pos < size() )
const PtInst*
SptItem::inst(ymuint32 pos) const
{
  return NULL;
}

// @brief 名前の取得
// @return 名前
// このクラスでは NULL を返す．
const char*
SptItem::name() const
{
  return NULL;
}

// @brief automatic 情報の取得
// @retval true automatic 宣言された task/function
// @retval false 上記以外
// このクラスでは false を返す．
bool
SptItem::automatic() const
{
  return false;
}

// @brief IO宣言の要素数の取得
ymuint32
SptItem::ioitem_num() const
{
  return 0;
}

// @brief IO宣言リストの配列の取得
PtIOHeadArray
SptItem::iohead_array() const
{
  return PtIOHeadArray();
}

// @brief parameter 宣言ヘッダ配列の取得
PtDeclHeadArray
SptItem::paramhead_array() const
{
  return PtDeclHeadArray();
}

// @brief localparam 宣言ヘッダ配列の取得
PtDeclHeadArray
SptItem::localparamhead_array() const
{
  return PtDeclHeadArray();
}

// @brief 宣言ヘッダ配列の取得
PtDeclHeadArray
SptItem::declhead_array() const
{
  return PtDeclHeadArray();
}

// @brief item 配列の取得
PtItemArray
SptItem::item_array() const
{
  return PtItemArray();
}

// @brief 本体のステートメントの取得
// @return 本体のステートメント
// このクラスでは NULL を返す．
const PtStmt*
SptItem::body() const
{
  return NULL;
}

// @brief 符号の取得
// @retval true 符号つき
// @retval false 符号なし
// このクラスでは false を返す．
bool
SptItem::is_signed() const
{
  return false;
}

// @brief 範囲の左側の式の取得
// @return 範囲の左側の式
// このクラスでは NULL を返す．
const PtExpr*
SptItem::left_range() const
{
  return NULL;
}

// @brief 範囲の右側の式の取得
// @return 範囲の右側の式
// このクラスでは NULL を返す．
const PtExpr*
SptItem::right_range() const
{
  return NULL;
}

// @brief 戻値のデータ型の取得
// @return 戻値のデータ型
// このクラスでは kVpiVarNone を返す．
tVpiVarType
SptItem::data_type() const
{
  return kVpiVarNone;
}

// @brief constant function の展開中の印をつける．
// このクラスではなにもしない．
void
SptItem::set_in_use() const
{
}

// @brief constant function の展開中の印を消す．
// このクラスではなにもしない．
void
SptItem::clear_in_use() const
{
}

// @brief 使用中(constant function として展開中)のチェック
// @return 使用中の時 true を返す．
// このクラスでは false を返す．
bool
SptItem::is_in_use() const
{
  return false;
}

// @brief specify block item の種類の取得
// @return specify block item の種類
// このクラスでは kVpiPulsestyleOnEvent を返す．
tVpiSpecItemType
SptItem::specitem_type() const
{
  return kVpiPulsestyleOnEvent; // ダミー
}

// @brief specify block path の種類の取得
// @retun specify block path の種類
// このクラスでは kVpiSpecPathNull を返す．
tVpiSpecPathType
SptItem::specpath_type() const
{
  return kVpiSpecPathNull; // ダミー
}

// @brief ターミナルの取得
// @param[in] pos 位置番号 ( 0 <= pos < size() )
const PtExpr*
SptItem::terminal(ymuint32 pos) const
{
  return 0;
}

// @brief パス記述の取得
// @return パス記述
// このクラスでは NULL を返す．
const PtPathDecl*
SptItem::path_decl() const
{
  return NULL;
}

// @brief 条件式の取得
// @return 条件式
// このクラスでは NULL を返す．
const PtExpr*
SptItem::expr() const
{
  return NULL;
}

// @brief 条件が成り立ったときに生成される宣言ヘッダ配列の取得
PtDeclHeadArray
SptItem::then_declhead_array() const
{
  return PtDeclHeadArray();
}

// @brief 条件が成り立ったときに生成される item 配列の取得
PtItemArray
SptItem::then_item_array() const
{
  return PtItemArray();
}

// @brief 条件が成り立たなかったときに生成される宣言ヘッダ配列の取得
PtDeclHeadArray
SptItem::else_declhead_array() const
{
  return PtDeclHeadArray();
}

/// @brief 条件が成り立たなかったときに生成される item 配列の取得
PtItemArray
SptItem::else_item_array() const
{
  return PtItemArray();
}

// @brief case item の取得
// @return 先頭の case item
// このクラスでは NULL を返す．
const PtGenCaseItem*
SptItem::caseitem(ymuint32 /* pos */) const
{
  return NULL;
}

// @brief 繰り返し制御用の変数名の取得
// @return 繰り返し制御用の変数名
// このクラスでは NULL を返す．
const char*
SptItem::loop_var() const
{
  return NULL;
}

// @brief 初期化文の右辺の取得
// @return 初期化文の右辺
// このクラスでは NULL を返す．
const PtExpr*
SptItem::init_expr() const
{
  return NULL;
}

// @brief 増加文の右辺の取得
// @return 増加文の右辺
// このクラスでは NULL を返す．
const PtExpr*
SptItem::next_expr() const
{
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// defparam 文のヘッダ
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
// @param dplist defparam のリスト
// dplist は削除される．
SptDefParamH::SptDefParamH(const FileRegion& file_region,
			   PtDefParamArray dp_array) :
  SptItem(file_region, kPtItem_DefParam),
  mArray(dp_array)
{
}

// デストラクタ
SptDefParamH::~SptDefParamH()
{
}

// 要素数の取得
// @return 要素数
ymuint32
SptDefParamH::size() const
{
  return mArray.size();
}

// @brief defparam 要素の取得
// @param[in] pos 位置番号 ( 0 <= pos < size() )
const PtDefParam*
SptDefParamH::defparam(ymuint32 pos) const
{
  return mArray[pos];
}


//////////////////////////////////////////////////////////////////////
// defparam 文のベース実装クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
// @param nblist 階層名のリスト
// @param tail_name 末尾の名前
// @param value 値を表す式
SptDefParam::SptDefParam(const FileRegion& file_region,
			 PtNameBranchArray nb_array,
			 const char* tail_name,
			 PtExpr* value) :
  mFileRegion(file_region),
  mNbArray(nb_array),
  mName(tail_name),
  mExpr(value)
{
  assert_cond(value, __FILE__, __LINE__);
}

// デストラクタ
SptDefParam::~SptDefParam()
{
}

// ファイル位置の取得
// @return ファイル位置
FileRegion
SptDefParam::file_region() const
{
  return mFileRegion;
}

// @brief 階層ブランチの取得
PtNameBranchArray
SptDefParam::namebranch_array() const
{
  return mNbArray;
}

// 末尾の名前の取得
// @return 末尾の名前
const char*
SptDefParam::name() const
{
  return mName;
}

// 値の取得
// @return 値を表す式
const PtExpr*
SptDefParam::expr() const
{
  return mExpr;
}


//////////////////////////////////////////////////////////////////////
// continuous assign 文のヘッダ
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
// @param strength 信号強度
// @param delay 遅延
// @param ca_list continuous assignment のリスト
SptContAssignH::SptContAssignH(const FileRegion& file_region,
			       PtStrength* strength,
			       PtDelay* delay,
			       PtContAssignArray ca_array) :
  SptItem(file_region, kPtItem_ContAssign),
  mStrength(strength),
  mDelay(delay),
  mArray(ca_array)
{
}

// デストラクタ
SptContAssignH::~SptContAssignH()
{
}

// strength の取得
// @return 信号強度
const PtStrength*
SptContAssignH::strength() const
{
  return mStrength;
}

// delay の取得
// @return 遅延
const PtDelay*
SptContAssignH::delay() const
{
  return mDelay;
}

// 要素数の取得
// @return 要素数
ymuint32
SptContAssignH::size() const
{
  return mArray.size();
}

// @brief continuous assign 要素の取得
// @param[in] pos 位置番号 ( 0 <= pos < size() )
const PtContAssign*
SptContAssignH::contassign(ymuint32 pos) const
{
  return mArray[pos];
}


//////////////////////////////////////////////////////////////////////
// continuous assign文 のベース実装クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
// @param lhs 左辺式
// @param rhs 右辺式
SptContAssign::SptContAssign(const FileRegion& file_region,
			     PtExpr* lhs,
			     PtExpr* rhs) :
  mFileRegion(file_region),
  mLhs(lhs),
  mRhs(rhs)
{
}

// デストラクタ
SptContAssign::~SptContAssign()
{
}

// ファイル位置の取得
// @return ファイル位置
FileRegion
SptContAssign::file_region() const
{
  return mFileRegion;
}

// 左辺式の取得
// @return 左辺式
const PtExpr*
SptContAssign::lhs() const
{
  return mLhs;
}

// 右辺式の取得
// @return 右辺式
const PtExpr*
SptContAssign::rhs() const
{
  return mRhs;
}


//////////////////////////////////////////////////////////////////////
// initial文/always文に共通なクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
// @param type 型(kInitial/kAlways)
// @param body 本体のステートメント
SptProcess::SptProcess(const FileRegion& file_region,
		       tPtItemType type,
		       PtStmt* body) :
  SptItem(file_region, type),
  mBody(body)
{
  assert_cond(body, __FILE__, __LINE__);
}

// デストラクタ
SptProcess::~SptProcess()
{
}

// 本体のステートメントの取得
// @return 本体のステートメント
const PtStmt*
SptProcess::body() const
{
  return mBody;
}


//////////////////////////////////////////////////////////////////////
// function と task の共通の親クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
// @param type 型
// @param name 名前
// @param automatic automatic 属性
// @param sign 符号の有無
// @param left 範囲の左側の式
// @param right 範囲の右側の式
// @param data_type 関数の戻値の型
// @param portdecl_list ポート宣言のリスト
// @param decl_list 宣言のリスト
// @param stmt 本体のステートメント
SptTf::SptTf(const FileRegion& file_region,
	     tPtItemType type,
	     const char* name,
	     bool automatic,
	     bool sign,
	     PtExpr* left,
	     PtExpr* right,
	     tVpiVarType data_type,
	     PtIOHeadArray iohead_array,
	     PtDeclHeadArray paramhead_array,
	     PtDeclHeadArray localparamhead_array,
	     PtDeclHeadArray declhead_array,
	     PtStmt* stmt) :
  SptItem(file_region, type),
  mName(name),
  mAutomatic(automatic),
  mSigned(sign),
  mLeftRange(left),
  mRightRange(right),
  mDataType(data_type),
  mIOHeadArray(iohead_array),
  mParamHeadArray(paramhead_array),
  mLocalparamHeadArray(localparamhead_array),
  mDeclHeadArray(declhead_array),
  mBody(stmt)
{
  mInUse = false;
  ymuint32 n = 0;
  for (ymuint32 i = 0; i < iohead_array.size(); ++ i) {
    n += iohead_array[i]->item_num();
  }
  mIOItemNum = n;
}

// デストラクタ
SptTf::~SptTf()
{
}

// 名前の取得
// @return 名前
const char*
SptTf::name() const
{
  return mName;
}

// automatic 情報の取得
// @retval true automatic 宣言された task/function
// @retval false 上記以外
bool
SptTf::automatic() const
{
  return mAutomatic;
}

// @brief IO宣言の要素数の取得
ymuint32
SptTf::ioitem_num() const
{
  return mIOItemNum;
}

// @brief IO宣言リストの配列の取得
PtIOHeadArray
SptTf::iohead_array() const
{
  return mIOHeadArray;
}

// @brief parameter 宣言ヘッダ配列の取得
PtDeclHeadArray
SptTf::paramhead_array() const
{
  return mParamHeadArray;
}

// @brief localparam 宣言ヘッダ配列の取得
PtDeclHeadArray
SptTf::localparamhead_array() const
{
  return mLocalparamHeadArray;
}

// @brief 宣言ヘッダ配列の取得
PtDeclHeadArray
SptTf::declhead_array() const
{
  return mDeclHeadArray;
}

// 本体のステートメントの取得
// @return 本体のステートメントの取得
const PtStmt*
SptTf::body() const
{
  return mBody;
}

// 符号の取得
// @retval true 符号つき
// @retval false 符号なし
bool
SptTf::is_signed() const
{
  return mSigned;
}

// 範囲の左側の式の取得
// @return 範囲の左側の式
const PtExpr*
SptTf::left_range() const
{
  return mLeftRange;
}

// 範囲の右側の式の取得
// @return 範囲の右側の式
const PtExpr*
SptTf::right_range() const
{
  return mRightRange;
}

// データ型の取得
// @retval データ型 kParam, kLocalParam, kVar の場合
// @retval kVpiVarNone 上記以外
tVpiVarType
SptTf::data_type() const
{
  return mDataType;
}

// constant function の展開中の印をつける．
void
SptTf::set_in_use() const
{
  mInUse = true;
}

// constant function の展開中の印を消す．
void
SptTf::clear_in_use() const
{
  mInUse = false;
}

// 使用中(constant function として展開中)のチェック
// @return 使用中の時 true を返す．
bool
SptTf::is_in_use() const
{
  return mInUse;
}


//////////////////////////////////////////////////////////////////////
// gate instance のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
// @param prim_type プリミティブタイプ
// @param strength 信号強度
// @param delay 遅延
// @param elem_list 要素のリスト
SptGateH::SptGateH(const FileRegion& file_region,
		   tVpiPrimType prim_type,
		   PtStrength* strength,
		   PtDelay* delay,
		   PtInstArray elem_array) :
  SptItem(file_region, kPtItem_GateInst),
  mPrimType(prim_type),
  mStrength(strength),
  mDelay(delay),
  mElemArray(elem_array)
{
}

// デストラクタ
SptGateH::~SptGateH()
{
}

// プリミティブタイプの取得
// @return プリミティブタイプ
tVpiPrimType
SptGateH::prim_type() const
{
  return mPrimType;
}

// strength の取得
// @return strength
const PtStrength*
SptGateH::strength() const
{
  return mStrength;
}

// delay の取得
// @return delay
const PtDelay*
SptGateH::delay() const
{
  return mDelay;
}

// 要素数の取得
// @return 要素数
ymuint32
SptGateH::size() const
{
  return mElemArray.size();
}

// @brief module/UDP/gate instance 要素の取得
// @param[in] pos 位置番号 ( 0 <= pos < size() )
const PtInst*
SptGateH::inst(ymuint32 pos) const
{
  return mElemArray[pos];
}


//////////////////////////////////////////////////////////////////////
// module instance/UDP instance のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
// @param def_name 定義名
// @param con_array ポーと割当のリスト
// @param strength 信号強度
// @param delay 遅延
// @param elem_list
SptMuH::SptMuH(const FileRegion& file_region,
	       const char* def_name,
	       PtConnectionArray con_array,
	       PtStrength* strength,
	       PtDelay* delay,
	       PtInstArray elem_array) :
  SptItem(file_region, kPtItem_MuInst),
  mName(def_name),
  mParamArray(con_array),
  mStrength(strength),
  mDelay(delay),
  mElemArray(elem_array)
{
}

// デストラクタ
SptMuH::~SptMuH()
{
}

// 定義名の取得
// @return 定義名
const char*
SptMuH::name() const
{
  return mName;
}

// strength の取得
// @return strength
const PtStrength*
SptMuH::strength() const
{
  return mStrength;
}

// delay の取得
// @return delay
const PtDelay*
SptMuH::delay() const
{
  return mDelay;
}

// @brief パラメータ割り当てリストの取得
PtConnectionArray
SptMuH::paramassign_array() const
{
  return mParamArray;
}

// 要素数の取得
// @return 要素数
ymuint32
SptMuH::size() const
{
  return mElemArray.size();
}

// @brief module/UDP/gate instance 要素の取得
// @param[in] pos 位置番号 ( 0 <= pos < size() )
const PtInst*
SptMuH::inst(ymuint32 pos) const
{
  return mElemArray[pos];
}


//////////////////////////////////////////////////////////////////////
// module instance/UDP/gate instance のベース実装クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
// @param name 名前
// @param left 範囲の左側の式
// @param right 範囲の右側の式
// @param con_array ポート割り当てリスト
SptInst::SptInst(const FileRegion& file_region,
		 const char* name,
		 PtExpr* left,
		 PtExpr* right,
		 PtConnectionArray con_array) :
  mFileRegion(file_region),
  mName(name),
  mLeftRange(left),
  mRightRange(right),
  mPortArray(con_array)
{
}

// デストラクタ
SptInst::~SptInst()
{
}

// ファイル位置の取得
// @return ファイル位置
FileRegion
SptInst::file_region() const
{
  return mFileRegion;
}

// 名前の取得
// @return 名前
const char*
SptInst::name() const
{
  return mName;
}

// 範囲の左側の式の取得
// @return 範囲の左側の式
const PtExpr*
SptInst::left_range() const
{
  return mLeftRange;
}

// 範囲の右側の式の取得
// @return 範囲の右側の式
const PtExpr*
SptInst::right_range() const
{
  return mRightRange;
}

// @brief ポート数の取得
// @return ポート数
ymuint32
SptInst::port_num() const
{
  return mPortArray.size();
}

// @brief ポートの取得
// @param[in] pos 位置番号 ( 0 <= pos < port_num() )
const PtConnection*
SptInst::port(ymuint32 pos) const
{
  return mPortArray[pos];
}


//////////////////////////////////////////////////////////////////////
// item 関係
//////////////////////////////////////////////////////////////////////

// @brief defparam 文のヘッダの生成
// @param[in] file_region ファイル位置の情報
// @param[in] elem_array 要素のリスト
// @return 生成された defparam ヘッダ
PtItem*
SptFactory::new_DefParamH(const FileRegion& file_region,
			  PtDefParamArray elem_array)
{
  void* p = alloc().get_memory(sizeof(SptDefParamH));
  return new (p) SptDefParamH(file_region, elem_array);
}

// @brief defparam 文の要素の生成
// @param[in] file_region ファイル位置の情報
// @param[in] name 名前
// @param[in] value 値を表す式
// @return 生成された要素
PtDefParam*
SptFactory::new_DefParam(const FileRegion& file_region,
			 const char* name,
			 PtExpr* value)
{
  void* p = alloc().get_memory(sizeof(SptDefParam));
  return new (p) SptDefParam(file_region, PtNameBranchArray(), name, value);
}

// @brief defparam 文の要素の生成 (階層つき識別子)
// @param[in] file_region ファイル位置の情報
// @param[in] nb_array 階層名のリスト
// @param[in] tail_name 名前
// @param[in] value 値を表す式
// @return 生成された要素
PtDefParam*
SptFactory::new_DefParam(const FileRegion& file_region,
			 PtNameBranchArray nb_array,
			 const char* tail_name,
			 PtExpr* value)
{
  void* p = alloc().get_memory(sizeof(SptDefParam));
  return new (p) SptDefParam(file_region, nb_array, tail_name, value);
}

// @brief continuous assign 文のヘッダの生成
// @param[in] file_region ファイル位置の情報
// @param[in] elem_array 要素のリスト
// @return 生成された continuous assign 文のヘッダ
PtItem*
SptFactory::new_ContAssignH(const FileRegion& file_region,
			    PtContAssignArray elem_array)
{
  void* p = alloc().get_memory(sizeof(SptContAssignH));
  return new (p) SptContAssignH(file_region, NULL, NULL, elem_array);
}

// @brief continuous assign 文のヘッダの生成 (strengthつき)
// @param[in] file_region ファイル位置の情報
// @param[in] strength 信号強度
// @param[in] elem_array 要素のリスト
// @return 生成された continuous assign 文のヘッダ
PtItem*
SptFactory::new_ContAssignH(const FileRegion& file_region,
			    PtStrength* strength,
			    PtContAssignArray elem_array)
{
  void* p = alloc().get_memory(sizeof(SptContAssignH));
  return new (p) SptContAssignH(file_region, strength, NULL, elem_array);
}

// @brief continuous assign 文のヘッダの生成 (遅延付き)
// @param[in] file_region ファイル位置の情報
// @param[in] delay 遅延値
// @param[in] elem_array 要素のリスト
// @return 生成された continuous assign 文のヘッダ
PtItem*
SptFactory::new_ContAssignH(const FileRegion& file_region,
			    PtDelay* delay,
			    PtContAssignArray elem_array)
{
  void* p = alloc().get_memory(sizeof(SptContAssignH));
  return new (p) SptContAssignH(file_region, NULL, delay, elem_array);
}

// @brief continuous assign 文のヘッダの生成 (strength, 遅延付き)
// @param[in] file_region ファイル位置の情報
// @param[in] strength 信号強度
// @param[in] delay 遅延値
// @param[in] elem_array 要素のリスト
// @return 生成された continuous assign 文のヘッダ
PtItem*
SptFactory::new_ContAssignH(const FileRegion& file_region,
			    PtStrength* strength,
			    PtDelay* delay,
			    PtContAssignArray elem_array)
{
  void* p = alloc().get_memory(sizeof(SptContAssignH));
  return new (p) SptContAssignH(file_region, strength, delay, elem_array);
}

// @brief continuous assign 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] lhs 左辺式
// @param[in] rhs 右辺式
// @return 生成された continuous assign 文
PtContAssign*
SptFactory::new_ContAssign(const FileRegion& file_region,
			   PtExpr* lhs,
			   PtExpr* rhs)
{
  void* p = alloc().get_memory(sizeof(SptContAssign));
  return new (p) SptContAssign(file_region, lhs, rhs);
}

// @brief initial 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] body 本体のステートメント
// @return 生成された initial 文
PtItem*
SptFactory::new_Initial(const FileRegion& file_region,
			PtStmt* body)
{
  void* p = alloc().get_memory(sizeof(SptProcess));
  return new (p) SptProcess(file_region,
			    kPtItem_Initial,
			    body);
}

// @brief always 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] body 本体のステートメント
// @return 生成された always 文
PtItem*
SptFactory::new_Always(const FileRegion& file_region,
		       PtStmt* body)
{
  void* p = alloc().get_memory(sizeof(SptProcess));
  return new (p) SptProcess(file_region,
			    kPtItem_Always,
			    body);
}

// @brief task 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] name task 名
// @param[in] automatic automatic task の時に true となるフラグ
// @param[in] iohead_array ポート宣言のリスト
// @param[in] paramhead_array parameter 宣言のリスト
// @param[in] localparamhead_array localparam 宣言のリスト
// @param[in] declhead_array 宣言のリスト (IO宣言を含む)
// @param[in] stmt 本体のステートメント
// @return 生成された task 文
PtItem*
SptFactory::new_Task(const FileRegion& file_region,
		     const char* name,
		     bool automatic,
		     PtIOHeadArray iohead_array,
		     PtDeclHeadArray paramhead_array,
		     PtDeclHeadArray localparamhead_array,
		     PtDeclHeadArray declhead_array,
		     PtStmt* stmt)
{
  void* p = alloc().get_memory(sizeof(SptTf));
  return new (p) SptTf(file_region,
		       kPtItem_Task,
		       name, automatic,
		       false, NULL, NULL,
		       kVpiVarNone,
		       iohead_array,
		       paramhead_array,
		       localparamhead_array,
		       declhead_array,
		       stmt);
}

// @brief 1ビット型 function 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] name function 名
// @param[in] automatic automatic task の時に true となるフラグ
// @param[in] sign 符号つきの時に true となるフラグ
// @param[in] iohead_array ポート宣言のリスト
// @param[in] paramhead_array parameter 宣言のリスト
// @param[in] lparamhead_array localparam 宣言のリスト
// @param[in] declhead_array 宣言のリスト (IO宣言を含む)
// @param[in] stmt 本体のステートメント
// @return 生成された func 文
PtItem*
SptFactory::new_Function(const FileRegion& file_region,
			 const char* name,
			 bool automatic,
			 bool sign,
			 PtIOHeadArray iohead_array,
			 PtDeclHeadArray paramhead_array,
			 PtDeclHeadArray lparamhead_array,
			 PtDeclHeadArray declhead_array,
			 PtStmt* stmt)
{
  void* p = alloc().get_memory(sizeof(SptTf));
  return new (p) SptTf(file_region,
		       kPtItem_Func,
		       name, automatic,
		       sign, NULL, NULL,
		       kVpiVarNone,
		       iohead_array,
		       paramhead_array, lparamhead_array, declhead_array,
		       stmt);
}

// @brief 範囲指定型 function 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] name function 名
// @param[in] automatic automatic task の時に true となるフラグ
// @param[in] sign 符号つきの時に true にするフラグ
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @param[in] iohead_array ポート宣言のリスト
// @param[in] paramhead_array parameter 宣言のリスト
// @param[in] lparamhead_array localparam 宣言のリスト
// @param[in] declhead_array 宣言のリスト (IO宣言を含む)
// @param[in] stmt 本体のステートメント
// @return 生成された func 文
PtItem*
SptFactory::new_SizedFunc(const FileRegion& file_region,
			  const char* name,
			  bool automatic,
			  bool sign,
			  PtExpr* left,
			  PtExpr* right,
			  PtIOHeadArray iohead_array,
			  PtDeclHeadArray paramhead_array,
			  PtDeclHeadArray lparamhead_array,
			  PtDeclHeadArray declhead_array,
			  PtStmt* stmt)
{
  void* p = alloc().get_memory(sizeof(SptTf));
  return new (p) SptTf(file_region,
		       kPtItem_Func,
		       name, automatic,
		       sign, left, right,
		       kVpiVarNone,
		       iohead_array,
		       paramhead_array, lparamhead_array, declhead_array,
		       stmt);
}

// @brief 組み込み型 function 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] name function 名
// @param[in] automatic automatic task の時に true となるフラグ
// @param[in] sign 符号つきの時に true にするフラグ
// @param[in] func_type 関数の戻り値の型
// @param[in] iohead_array ポート宣言のリスト
// @param[in] paramhead_array parameter 宣言のリスト
// @param[in] lparamhead_array localparam 宣言のリスト
// @param[in] declhead_array 宣言のリスト (IO宣言を含む)
// @param[in] stmt 本体のステートメント
// @return 生成された func 文
PtItem*
SptFactory::new_TypedFunc(const FileRegion& file_region,
			  const char* name,
			  bool automatic,
			  bool sign,
			  tVpiVarType func_type,
			  PtIOHeadArray iohead_array,
			  PtDeclHeadArray paramhead_array,
			  PtDeclHeadArray lparamhead_array,
			  PtDeclHeadArray declhead_array,
			  PtStmt* stmt)
{
  void* p = alloc().get_memory(sizeof(SptTf));
  return new (p) SptTf(file_region,
		       kPtItem_Func,
		       name, automatic,
		       sign, NULL, NULL,
		       func_type,
		       iohead_array,
		       paramhead_array, lparamhead_array, declhead_array,
		       stmt);
}

// @brief gate instance 文のヘッダの生成
// @param[in] file_region ファイル位置の情報
// @param[in] type primitive の型
// @param[in] elem_array 要素のリスト
// @return 生成された gate instance 文のヘッダ
PtItem*
SptFactory::new_GateH(const FileRegion& file_region,
		      tVpiPrimType type,
		      PtInstArray elem_array)
{
  void* p = alloc().get_memory(sizeof(SptGateH));
  return new (p) SptGateH(file_region,
			  type,
			  NULL, NULL,
			  elem_array);
}

// @brief gate instance 文のヘッダの生成 (strength付き)
// @param[in] file_region ファイル位置の情報
// @param[in] type primitive の型
// @param[in] strength 信号強度
// @param[in] elem_array 要素のリスト
// @return 生成された gate instance 文のヘッダ
PtItem*
SptFactory::new_GateH(const FileRegion& file_region,
		      tVpiPrimType type,
		      PtStrength* strength,
		      PtInstArray elem_array)
{
  void* p = alloc().get_memory(sizeof(SptGateH));
  return new (p) SptGateH(file_region,
			  type,
			  strength, NULL,
			  elem_array);
}

// @brief gate instance 文のヘッダの生成 (遅延付き)
// @param[in] file_region ファイル位置の情報
// @param[in] type primitive の型
// @param[in] delay 遅延値
// @param[in] elem_array 要素のリスト
// @return 生成された gate instance 文のヘッダ
PtItem*
SptFactory::new_GateH(const FileRegion& file_region,
		      tVpiPrimType type,
		      PtDelay* delay,
		      PtInstArray elem_array)
{
  void* p = alloc().get_memory(sizeof(SptGateH));
  return new (p) SptGateH(file_region,
			  type,
			  NULL, delay,
			  elem_array);
}

// @brief gate instance 文のヘッダの生成 (strength, 遅延付き)
// @param[in] file_region ファイル位置の情報
// @param[in] type primitive の型
// @param[in] strength 信号強度
// @param[in] delay 遅延値
// @param[in] elem_array 要素のリスト
// @return 生成された gate instance 文のヘッダ
PtItem*
SptFactory::new_GateH(const FileRegion& file_region,
		      tVpiPrimType type,
		      PtStrength* strength,
		      PtDelay* delay,
		      PtInstArray elem_array)
{
  void* p = alloc().get_memory(sizeof(SptGateH));
  return new (p) SptGateH(file_region,
			  type,
			  strength, delay,
			  elem_array);
}

// @brief module instance/UDP instance 文のヘッダの生成
// @param[in] file_region ファイル位置の情報
// @param[in] def_name 定義名
// @param[in] elem_array 要素のリスト
// @return 生成された module instance/UDP instance 文のヘッダ
PtItem*
SptFactory::new_MuH(const FileRegion& file_region,
		    const char* def_name,
		    PtInstArray elem_array)
{
  void* p = alloc().get_memory(sizeof(SptMuH));
  return new (p) SptMuH(file_region, def_name,
			PtConnectionArray(),
			NULL, NULL,
			elem_array);
}

// @brief module instance/UDP instance 文のヘッダの生成 (strength付き)
// @param[in] file_region ファイル位置の情報
// @param[in] def_name 定義名
// @param[in] strength 信号強度
// @param[in] elem_array 要素のリスト
// @return 生成された module instance/UDP instance 文のヘッダ
PtItem*
SptFactory::new_MuH(const FileRegion& file_region,
		    const char* def_name,
		    PtStrength* strength,
		    PtInstArray elem_array)
{
  void* p = alloc().get_memory(sizeof(SptMuH));
  return new (p) SptMuH(file_region, def_name,
			PtConnectionArray(),
			strength, NULL,
			elem_array);
}

// @brief module instance/UDP instance 文のヘッダの生成 (遅延付き)
// @param[in] file_region ファイル位置の情報
// @param[in] def_name 定義名
// @param[in] delay 遅延値
// @param[in] elem_array 要素のリスト
// @return 生成された module instance/UDP instance 文のヘッダ
PtItem*
SptFactory::new_MuH(const FileRegion& file_region,
		    const char* def_name,
		    PtDelay* delay,
		    PtInstArray elem_array)
{
  void* p = alloc().get_memory(sizeof(SptMuH));
  return new (p) SptMuH(file_region, def_name,
			PtConnectionArray(),
			NULL, delay,
			elem_array);
}

// @brief module instance/UDP instance 文のヘッダの生成 (strength, 遅延付き)
// @param[in] file_region ファイル位置の情報
// @param[in] def_name 定義名
// @param[in] strength 信号強度
// @param[in] delay 遅延値
// @param[in] elem_array 要素のリスト
// @return 生成された module instance/UDP instance 文のヘッダ
PtItem*
SptFactory::new_MuH(const FileRegion& file_region,
		    const char* def_name,
		    PtStrength* strength,
		    PtDelay* delay,
		    PtInstArray elem_array)
{
  void* p = alloc().get_memory(sizeof(SptMuH));
  return new (p) SptMuH(file_region, def_name,
			PtConnectionArray(),
			strength, delay,
			elem_array);
}

// @brief module instance/UDP instance 文のヘッダの生成
// @param[in] file_region ファイル位置の情報
// @param[in] def_name 定義名
// @param[in] con_array ポート割り当てリスト
// @param[in] elem_array 要素のリスト
// @return 生成された module instance/UDP instance 文のヘッダ
PtItem*
SptFactory::new_MuH(const FileRegion& file_region,
		    const char* def_name,
		    PtConnectionArray con_array,
		    PtInstArray elem_array)
{
  void* p = alloc().get_memory(sizeof(SptMuH));
  return new (p) SptMuH(file_region, def_name,
			con_array,
			NULL, NULL,
			elem_array);
}

// @brief module instance/UDP/gate instance の要素の生成
// @param[in] file_region ファイル位置の情報
// @param[in] con_list ポート割り当てリスト
// @return 生成された module instance/UDP instance の要素
PtInst*
SptFactory::new_Inst(const FileRegion& file_region,
		     PtConnectionArray con_array)
{
  void* p = alloc().get_memory(sizeof(SptInst));
  return new (p) SptInst(file_region, NULL, NULL, NULL, con_array);
}

// @brief 名前付き module instance/UDP/gate instance の要素の生成
// @param[in] file_region ファイル位置の情報
// @param[in] name 名前
// @param[in] con_list ポート割り当てリスト
// @return 生成された module instance/UDP instance の要素
PtInst*
SptFactory::new_InstN(const FileRegion& file_region,
		      const char* name,
		      PtConnectionArray con_array)
{
  void* p = alloc().get_memory(sizeof(SptInst));
  return new (p) SptInst(file_region, name, NULL, NULL, con_array);
}

// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
// @param[in] file_region ファイル位置の情報
// @param[in] name 名前
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @param[in] con_list ポート割り当てリスト
// @return 生成された module instance/UDP instance の要素
PtInst*
SptFactory::new_InstV(const FileRegion& file_region,
		      const char* name,
		      PtExpr* left,
		      PtExpr* right,
		      PtConnectionArray con_array)
{
  void* p = alloc().get_memory(sizeof(SptInst));
  return new (p) SptInst(file_region, name, left, right, con_array);
}

// @brief generate 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] declhead_array 宣言のリスト
// @param[in] item_array 要素のリスト
// @return 生成された generate 文
PtItem*
SptFactory::new_Generate(const FileRegion& file_region,
			 PtDeclHeadArray declhead_array,
			 PtItemArray item_array)
{
  void* p = alloc().get_memory(sizeof(SptGenerate));
  return new (p) SptGenerate(file_region, kPtItem_Generate, NULL,
			     declhead_array, item_array);
}

// @brief generate block 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] declhead_array 宣言のリスト
// @param[in] item_array 要素のリスト
// @return 生成された generate block 文
PtItem*
SptFactory::new_GenBlock(const FileRegion& file_region,
			 PtDeclHeadArray declhead_array,
			 PtItemArray item_array)
{
  void* p = alloc().get_memory(sizeof(SptGenerate));
  return new (p) SptGenerate(file_region, kPtItem_GenBlock, NULL,
			     declhead_array, item_array);
}

// @brief 名前付き generate block 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] name 名前
// @param[in] declhead_array 宣言のリスト
// @param[in] item_array 要素のリスト
// @return 生成された generate block 文
PtItem*
SptFactory::new_GenBlock(const FileRegion& file_region,
			 const char* name,
			 PtDeclHeadArray declhead_array,
			 PtItemArray item_array)
{
  void* p = alloc().get_memory(sizeof(SptGenerate));
  return new (p) SptGenerate(file_region, kPtItem_GenBlock, name,
			     declhead_array, item_array);
}

// @brief generate if 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] cond 条件を表す式
// @param[in] then_declhead_array 条件が成り立った時に生成する宣言のリスト
// @param[in] then_item_array 条件が成り立った時に生成する要素のリスト
// @param[in] else_declhead_array 条件が成り立たなかった時に生成する宣言のリスト
// @param[in] else_item_array 条件が成り立たなかった時に生成する要素のリスト
// @return 生成された generate if 文
PtItem*
SptFactory::new_GenIf(const FileRegion& file_region,
		      PtExpr* cond,
		      PtDeclHeadArray then_declhead_array,
		      PtItemArray then_item_array,
		      PtDeclHeadArray else_declhead_array,
		      PtItemArray else_item_array)
{
  void* p = alloc().get_memory(sizeof(SptGenerate));
  return new (p) SptGenIf(file_region, cond,
			  then_declhead_array, then_item_array,
			  else_declhead_array, else_item_array);
}

// @brief generate case 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] expr 選択式
// @param[in] item_array generate case item のリスト
// @return 生成された generate case 文
PtItem*
SptFactory::new_GenCase(const FileRegion& file_region,
			PtExpr* expr,
			PtGenCaseItemArray item_array)
{
  void* p = alloc().get_memory(sizeof(SptGenCase));
  return new (p) SptGenCase(file_region, expr, item_array);
}

// @brief generate case の要素の生成
// @param[in] file_region ファイル位置の情報
// @param[in] label 比較式
// @param[in] declhead_array 宣言のリスト
// @param[in] item_array 要素のリスト
// @return 生成された generate case item
PtGenCaseItem*
SptFactory::new_GenCaseItem(const FileRegion& file_region,
			    PtExprArray label_array,
			    PtDeclHeadArray declhead_array,
			    PtItemArray item_array)
{
  void* p = alloc().get_memory(sizeof(SptGenCaseItem));
  return new (p) SptGenCaseItem(file_region, label_array,
				declhead_array, item_array);
}

// @brief generate for 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] loop_var ループ変数
// @param[in] init_expr 初期化式
// @param[in] cond ループ条件式
// @param[in] next_expr 増加式
// @param[in] block_name ブロック名
// @param[in] declhead_array 宣言のリスト
// @param[in] item_array 要素のリスト
// @return 生成された generate for 文
PtItem*
SptFactory::new_GenFor(const FileRegion& file_region,
		       const char* loop_var,
		       PtExpr* init_expr,
		       PtExpr* cond,
		       PtExpr* next_expr,
		       const char* block_name,
		       PtDeclHeadArray declhead_array,
		       PtItemArray item_array)
{
  void* p = alloc().get_memory(sizeof(SptGenFor));
  return new (p) SptGenFor(file_region, loop_var, init_expr, cond,
			   next_expr, block_name,
			   declhead_array, item_array);
}

// @brief specify block item の生成
// @param[in] file_region ファイル位置の情報
// @param[in] id specify block item の種類
// @param[in] terminal_list 端子のリスト
// @return 生成された specify block item
PtItem*
SptFactory::new_SpecItem(const FileRegion& file_region,
			 tVpiSpecItemType id,
			 PtExprArray terminal_array)
{
  void* p = alloc().get_memory(sizeof(SptSpecItem));
  return new (p) SptSpecItem(file_region, id, terminal_array);
}

// @brief path 仕様を生成する．
// @param[in] file_region ファイル位置の情報
// @param[in] id spec path の種類
// @param[in] expr 条件式
// @param[in] path_decl パス記述
// @return 生成された spec path
PtItem*
SptFactory::new_SpecPath(const FileRegion& file_region,
			 tVpiSpecPathType id,
			 PtExpr* expr,
			 PtPathDecl* path_decl)
{
  void* p = alloc().get_memory(sizeof(SptSpecPath));
  return new (p) SptSpecPath(file_region, id, expr, path_decl);
}

// @brief パス記述の生成
// @param[in] file_region ファイル位置の情報
// @param[in] edge
// @param[in] input_array
// @param[in] input_pol
// @param[in] op
// @param[in] output_array
// @param[in] output_pol
// @param[in] expr
// @param[in] path_delay
// @return 生成されたパス記述
PtPathDecl*
SptFactory::new_PathDecl(const FileRegion& file_region,
			 int edge,
			 PtExprArray input_array,
			 int input_pol,
			 int op,
			 PtExprArray output_array,
			 int output_pol,
			 PtExpr* expr,
			 PtPathDelay* path_delay)
{
  void* p = alloc().get_memory(sizeof(SptPathDecl));
  return new (p) SptPathDecl(file_region, edge, input_array, input_pol,
			     op, output_array, output_pol,
			     expr, path_delay);
}

// @brief path delay value の生成 (値が1つ)
// @param[in] file_region ファイル位置の情報
// @param[in] value 値
// @return 生成された path delay value
PtPathDelay*
SptFactory::new_PathDelay(const FileRegion& file_region,
			  PtExpr* value)
{
  void* p = alloc().get_memory(sizeof(SptPathDelay));
  return new (p) SptPathDelay(file_region, value);
}

// @brief path delay value の生成 (値が2つ)
// @param[in] file_region ファイル位置の情報
// @param[in] value1 値1
// @param[in] value2 値2
// @return 生成された path delay value
PtPathDelay*
SptFactory::new_PathDelay(const FileRegion& file_region,
			  PtExpr* value1,
			  PtExpr* value2)
{
  void* p = alloc().get_memory(sizeof(SptPathDelay));
  return new (p) SptPathDelay(file_region, value1, value2);
}

// @brief path delay value の生成 (値が3つ)
// @param[in] file_region ファイル位置の情報
// @param[in] value1 値1
// @param[in] value2 値2
// @param[in] value3 値3
// @return 生成された path delay value
PtPathDelay*
SptFactory::new_PathDelay(const FileRegion& file_region,
			  PtExpr* value1,
			  PtExpr* value2,
			  PtExpr* value3)
{
  void* p = alloc().get_memory(sizeof(SptPathDelay));
  return new (p) SptPathDelay(file_region, value1, value2, value3);
}

// @brief path delay value の生成 (値が6つ)
// @param[in] file_region ファイル位置の情報
// @param[in] value1 値1
// @param[in] value2 値2
// @param[in] value3 値3
// @param[in] value4 値4
// @param[in] value5 値5
// @param[in] value6 値6
// @return 生成された path delay value
PtPathDelay*
SptFactory::new_PathDelay(const FileRegion& file_region,
			  PtExpr* value1,
			  PtExpr* value2,
			  PtExpr* value3,
			  PtExpr* value4,
			  PtExpr* value5,
			  PtExpr* value6)
{
  void* p = alloc().get_memory(sizeof(SptPathDelay));
  return new (p) SptPathDelay(file_region, value1, value2, value3,
			      value4, value5, value6);
}

// @brief path delay value の生成 (値が12個)
// @param[in] file_region ファイル位置の情報
// @param[in] value1 値1
// @param[in] value2 値2
// @param[in] value3 値3
// @param[in] value4 値4
// @param[in] value5 値5
// @param[in] value6 値6
// @param[in] value7 値7
// @param[in] value8 値8
// @param[in] value9 値9
// @param[in] value10 値10
// @param[in] value11 値11
// @param[in] value12 値12
// @return 生成された path delay value
PtPathDelay*
SptFactory::new_PathDelay(const FileRegion& file_region,
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
			  PtExpr* value12)
{
  void* p = alloc().get_memory(sizeof(SptPathDelay));
  return new (p) SptPathDelay(file_region, value1, value2, value3,
			      value4, value5, value6,
			      value7, value8, value9,
			      value10, value11, value12);
}

END_NAMESPACE_YM_VERILOG
