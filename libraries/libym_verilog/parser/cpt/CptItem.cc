
/// @file libym_verilog/pt_compact/CptItem.cc
/// @brief CptItem の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CptItem.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "CptItem.h"
#include "CptFactory.h"

#include "ym_verilog/pt/PtStmt.h"
#include "ym_verilog/pt/PtExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// item の基底クラス
//////////////////////////////////////////////////////////////////////
  
// @brief コンストラクタ
CptItem::CptItem()
{
}

// @brief デストラクタ
CptItem::~CptItem()
{
}

// @brief プリミティブタイプの取得
// @return プリミティブタイプ
// このクラスでは kVpiAndPrim を返す．
tVpiPrimType
CptItem::prim_type() const
{
  return kVpiAndPrim; // ダミー
}

// @brief strength の取得
// @return 信号強度
// このクラスでは NULL を返す．
const PtStrength*
CptItem::strength() const
{
  return NULL;
}

// @brief delay の取得
// @return 遅延
// このクラスでは NULL を返す．
const PtDelay*
CptItem::delay() const
{
  return NULL;
}
  
// @brief パラメータ割り当てリストの取得
PtConnectionArray
CptItem::paramassign_array() const
{
  return PtConnectionArray();
}

// @brief 要素数の取得
// @return 要素数
// このクラスでは 0 を返す．
ymuint32
CptItem::size() const
{
  return 0;
}

// @brief defparam 要素の取得
// @param[in] pos 位置番号 ( 0 <= pos < size() )
const PtDefParam*
CptItem::defparam(ymuint32 pos) const
{
  return NULL;
}

// @brief continuous assign 要素の取得
// @param[in] pos 位置番号 ( 0 <= pos < size() )
const PtContAssign*
CptItem::contassign(ymuint32 pos) const
{
  return NULL;
}

// @brief module/UDP/gate instance 要素の取得
// @param[in] pos 位置番号 ( 0 <= pos < size() )
const PtInst*
CptItem::inst(ymuint32 pos) const
{
  return NULL;
}

// @brief 名前の取得
// @return 名前
// このクラスでは NULL を返す．
const char*
CptItem::name() const
{
  return NULL;
}

// @brief automatic 情報の取得
// @retval true automatic 宣言された task/function
// @retval false 上記以外
// このクラスでは false を返す．
bool
CptItem::automatic() const
{
  return false;
}
  
// @brief IO宣言の要素数の取得
ymuint32
CptItem::ioitem_num() const
{
  return 0;
}
  
// @brief IO宣言リストの配列の取得
PtIOHeadArray
CptItem::iohead_array() const
{
  return PtIOHeadArray();
}

// @brief parameter 宣言ヘッダ配列の取得
PtDeclHeadArray
CptItem::paramhead_array() const
{
  return PtDeclHeadArray();
}
  
// @brief localparam 宣言ヘッダ配列の取得
PtDeclHeadArray
CptItem::localparamhead_array() const
{
  return PtDeclHeadArray();
}
  
// @brief 宣言ヘッダ配列の取得
PtDeclHeadArray
CptItem::declhead_array() const
{
  return PtDeclHeadArray();
}
  
// @brief item 配列の取得
PtItemArray
CptItem::item_array() const
{
  return PtItemArray();
}

// @brief 本体のステートメントの取得
// @return 本体のステートメント
// このクラスでは NULL を返す．
const PtStmt*
CptItem::body() const
{
  return NULL;
}

// @brief 符号の取得
// @retval true 符号つき
// @retval false 符号なし
// このクラスでは false を返す．
bool
CptItem::is_signed() const
{
  return false;
}

// @brief 範囲の左側の式の取得
// @return 範囲の左側の式
// このクラスでは NULL を返す．
const PtExpr*
CptItem::left_range() const
{
  return NULL;
}

// @brief 範囲の右側の式の取得
// @return 範囲の右側の式
// このクラスでは NULL を返す．
const PtExpr*
CptItem::right_range() const
{
  return NULL;
}

// @brief 戻値のデータ型の取得
// @return 戻値のデータ型
// このクラスでは kVpiVarNone を返す．
tVpiVarType
CptItem::data_type() const
{
  return kVpiVarNone;
}

// @brief constant function の展開中の印をつける．
// このクラスではなにもしない．
void
CptItem::set_in_use() const
{
}

// @brief constant function の展開中の印を消す．
// このクラスではなにもしない．
void
CptItem::clear_in_use() const
{
}

// @brief 使用中(constant function として展開中)のチェック
// @return 使用中の時 true を返す．
// このクラスでは false を返す．
bool
CptItem::is_in_use() const
{
  return false;
}

// @brief specify block item の種類の取得
// @return specify block item の種類
// このクラスでは kVpiPulsestyleOnEvent を返す．
tVpiSpecItemType
CptItem::specitem_type() const
{
  return kVpiPulsestyleOnEvent; // ダミー
}

// @brief specify block path の種類の取得
// @retun specify block path の種類
// このクラスでは kVpiSpecPathNull を返す．
tVpiSpecPathType
CptItem::specpath_type() const
{
  return kVpiSpecPathNull; // ダミー
}
  
// @brief ターミナルの取得
// @param[in] pos 位置番号 ( 0 <= pos < size() )
const PtExpr*
CptItem::terminal(ymuint32 pos) const
{
  return NULL;
}

// @brief パス記述の取得
// @return パス記述
// このクラスでは NULL を返す．
const PtPathDecl*
CptItem::path_decl() const
{
  return NULL;
}
  
// @brief 条件式の取得
// @return 条件式
// このクラスでは NULL を返す．
const PtExpr*
CptItem::expr() const
{
  return NULL;
}
  
// @brief 条件が成り立ったときに生成される宣言ヘッダ配列の取得
PtDeclHeadArray
CptItem::then_declhead_array() const
{
  return PtDeclHeadArray();
}
  
// @brief 条件が成り立ったときに生成される item 配列の取得
PtItemArray
CptItem::then_item_array() const
{
  return PtItemArray();
}
  
// @brief 条件が成り立たなかったときに生成される宣言ヘッダ配列の取得
PtDeclHeadArray
CptItem::else_declhead_array() const
{
  return PtDeclHeadArray();
}
  
// @brief 条件が成り立たなかったときに生成される item 配列の取得
PtItemArray
CptItem::else_item_array() const
{
  return PtItemArray();
}

// @brief case item の取得
// @param[in] pos 取得する case item の位置(最初の位置は 0)
// @return pos 番目の case item
// このクラスでは NULL を返す．
const PtGenCaseItem*
CptItem::caseitem(ymuint32 pos) const
{
  return NULL;
}

// @brief 繰り返し制御用の変数名の取得
// @return 繰り返し制御用の変数名
// このクラスでは NULL を返す．
const char*
CptItem::loop_var() const
{
  return NULL;
}

// @brief 初期化文の右辺の取得
// @return 初期化文の右辺
// このクラスでは NULL を返す．
const PtExpr*
CptItem::init_expr() const
{
  return NULL;
}

// @brief 増加文の右辺の取得
// @return 増加文の右辺
// このクラスでは NULL を返す．
const PtExpr*
CptItem::next_expr() const
{
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// defparam 文のヘッダ
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptDefParamH::CptDefParamH(const FileRegion& file_region,
			   PtDefParamArray dp_array) :
  mFileRegion(file_region),
  mArray(dp_array)
{
}
  
// デストラクタ
CptDefParamH::~CptDefParamH()
{
}

// ファイル位置を返す．
FileRegion
CptDefParamH::file_region() const
{
  return mFileRegion;
}

// 型を返す．
tPtItemType
CptDefParamH::type() const
{
  return kPtItem_DefParam;
}

// 要素数を返す．
ymuint32
CptDefParamH::size() const
{
  return mArray.size();
}

// @brief defparam 要素の取得
// @param[in] pos 位置番号 ( 0 <= pos < size() )
const PtDefParam*
CptDefParamH::defparam(ymuint32 pos) const
{
  return mArray[pos];
}


//////////////////////////////////////////////////////////////////////
// defparam 文のベース実装クラス
//////////////////////////////////////////////////////////////////////
  
// コンストラクタ
CptDefParam::CptDefParam(const FileRegion& file_region,
			 PtNameBranchArray nb_array,
			 const char* tail_name,
			 PtExpr* value) :
  mTopLoc(file_region.start_loc()),
  mNbArray(nb_array),
  mName(tail_name),
  mExpr(value)
{
  assert_cond(value, __FILE__, __LINE__);
}

// デストラクタ
CptDefParam::~CptDefParam()
{
}

// ファイル位置を返す．
FileRegion
CptDefParam::file_region() const
{
  return FileRegion(mTopLoc, mExpr->file_region().end_loc());
}
  
// @brief 階層ブランチの取得
PtNameBranchArray
CptDefParam::namebranch_array() const
{
  return mNbArray;
}

// 末尾の名前を返す．
const char*
CptDefParam::name() const
{
  return mName;
}

// 値を返す．
const PtExpr*
CptDefParam::expr() const
{
  return mExpr;
}


//////////////////////////////////////////////////////////////////////
// continuous assign 文のヘッダ
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptContAssignH::CptContAssignH(const FileRegion& file_region,
			       PtContAssignArray ca_array) :
  mFileRegion(file_region),
  mArray(ca_array)
{
}
  
// デストラクタ
CptContAssignH::~CptContAssignH()
{
}

// ファイル位置を返す．
FileRegion
CptContAssignH::file_region() const
{
  return mFileRegion;
}

// 型を返す．
tPtItemType
CptContAssignH::type() const
{
  return kPtItem_ContAssign;
}

// 要素数を返す．
ymuint32
CptContAssignH::size() const
{
  return mArray.size();
}

// @brief continuous assign 要素の取得
// @param[in] pos 位置番号 ( 0 <= pos < size() )
const PtContAssign*
CptContAssignH::contassign(ymuint32 pos) const
{
  return mArray[pos];
}


//////////////////////////////////////////////////////////////////////
// strength つきの continuous assign 文のヘッダ
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptContAssignHS::CptContAssignHS(const FileRegion& file_region,
				 PtStrength* strength,
				 PtContAssignArray ca_array) :
  CptContAssignH(file_region, ca_array),
  mStrength(strength)
{
}
  
// デストラクタ
CptContAssignHS::~CptContAssignHS()
{
}

// strength を返す．
const PtStrength*
CptContAssignHS::strength() const
{
  return mStrength;
}


//////////////////////////////////////////////////////////////////////
// delay つきの continuous assign 文のヘッダ
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptContAssignHD::CptContAssignHD(const FileRegion& file_region,
				 PtDelay* delay,
				 PtContAssignArray ca_array) :
  CptContAssignH(file_region, ca_array),
  mDelay(delay)
{
}
  
// デストラクタ
CptContAssignHD::~CptContAssignHD()
{
}

// delay を返す．
const PtDelay*
CptContAssignHD::delay() const
{
  return mDelay;
}


//////////////////////////////////////////////////////////////////////
// strength と delay つきの continuous assign 文のヘッダ
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptContAssignHSD::CptContAssignHSD(const FileRegion& file_region,
				   PtStrength* strength,
				   PtDelay* delay,
				   PtContAssignArray ca_array) :
  CptContAssignH(file_region, ca_array),
  mStrength(strength),
  mDelay(delay)
{
}
  
// デストラクタ
CptContAssignHSD::~CptContAssignHSD()
{
}

// strength を返す．
const PtStrength*
CptContAssignHSD::strength() const
{
  return mStrength;
}

// delay を返す．
const PtDelay*
CptContAssignHSD::delay() const
{
  return mDelay;
}


//////////////////////////////////////////////////////////////////////
// continuous assign文 のベース実装クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptContAssign::CptContAssign(PtExpr* lhs,
			     PtExpr* rhs) :
  mLhs(lhs),
  mRhs(rhs)
{
}

// デストラクタ
CptContAssign::~CptContAssign()
{
}

// ファイル位置を取り出す．
FileRegion
CptContAssign::file_region() const
{
  return FileRegion(lhs()->file_region(), rhs()->file_region());
}

// 左辺式を取り出す．
const PtExpr*
CptContAssign::lhs() const
{
  return mLhs;
}

// 右辺式を取り出す．
const PtExpr*
CptContAssign::rhs() const
{
  return mRhs;
}


//////////////////////////////////////////////////////////////////////
// initial文/always文に共通なクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptProcess::CptProcess(const FileRegion& file_region,
		       PtStmt* body) :
  mTopLoc(file_region.start_loc()),
  mBody(body)
{
  assert_cond(body, __FILE__, __LINE__);
}

// デストラクタ
CptProcess::~CptProcess()
{
}

// ファイル位置を返す．
FileRegion
CptProcess::file_region() const
{
  return FileRegion(mTopLoc, mBody->file_region().end_loc());
}

// 本体のステートメントを返す．
const PtStmt*
CptProcess::body() const
{
  return mBody;
}


//////////////////////////////////////////////////////////////////////
// initial文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptInitial::CptInitial(const FileRegion& file_region,
		       PtStmt* body) :
  CptProcess(file_region, body)
{
}

// デストラクタ
CptInitial::~CptInitial()
{
}

// 型を返す．
tPtItemType
CptInitial::type() const
{
  return kPtItem_Initial;
}


//////////////////////////////////////////////////////////////////////
// always文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptAlways::CptAlways(const FileRegion& file_region,
		     PtStmt* body) :
  CptProcess(file_region, body)
{
}

// デストラクタ
CptAlways::~CptAlways()
{
}

// 型を返す．
tPtItemType
CptAlways::type() const
{
  return kPtItem_Always;
}


//////////////////////////////////////////////////////////////////////
// function と task の共通の親クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptTf::CptTf(const FileRegion& file_region,
	     const char* name,
	     bool automatic,
	     PtIOHeadArray iohead_array,
	     PtDeclHeadArray paramhead_array,
	     PtDeclHeadArray lparamhead_array,
	     PtDeclHeadArray declhead_array,
	     PtStmt* stmt) :
  mFileRegion(file_region),
  mName(name),
  mAutomatic(automatic),
  mIOHeadArray(iohead_array),
  mParamHeadArray(paramhead_array),
  mLparamHeadArray(lparamhead_array),
  mDeclHeadArray(declhead_array),
  mBody(stmt)
{
  ymuint32 n = 0;
  for (ymuint32 i = 0; i < iohead_array.size(); ++ i) {
    n += iohead_array[i]->item_num();
  }
  mIOItemNum = n;
}
  
// デストラクタ
CptTf::~CptTf()
{
}

// ファイル位置を返す．
FileRegion
CptTf::file_region() const
{
  return mFileRegion;
}

// 名前を返す．
const char*
CptTf::name() const
{
  return mName;
}

// automatic なら true
bool
CptTf::automatic() const
{
  return mAutomatic;
}
  
// @brief IO宣言の要素数の取得
ymuint32
CptTf::ioitem_num() const
{
  return mIOItemNum;
}

// @brief IO宣言リストの配列の取得
PtIOHeadArray
CptTf::iohead_array() const
{
  return mIOHeadArray;
}

// @brief parameter 宣言ヘッダ配列の取得
PtDeclHeadArray
CptTf::paramhead_array() const
{
  return mParamHeadArray;
}
  
// @brief localparam 宣言ヘッダ配列の取得
PtDeclHeadArray
CptTf::localparamhead_array() const
{
  return mLparamHeadArray;
}
  
// @brief 宣言ヘッダ配列の取得
PtDeclHeadArray
CptTf::declhead_array() const
{
  return mDeclHeadArray;
}

// 本体を取り出す．
const PtStmt*
CptTf::body() const
{
  return mBody;
}


//////////////////////////////////////////////////////////////////////
// task を表すノード
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptTask::CptTask(const FileRegion& file_region,
		 const char* name,
		 bool automatic,
		 PtIOHeadArray iohead_array,
		 PtDeclHeadArray paramhead_array,
		 PtDeclHeadArray lparamhead_array,
		 PtDeclHeadArray declhead_array,
		 PtStmt* stmt) :
  CptTf(file_region,
	name, automatic,
	iohead_array,
	paramhead_array,
	lparamhead_array,
	declhead_array,
	stmt)
{
}

// デストラクタ
CptTask::~CptTask()
{
}

// 型を返す．
tPtItemType
CptTask::type() const
{
  return kPtItem_Task;
}


//////////////////////////////////////////////////////////////////////
// function を表すノード
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptFunction::CptFunction(const FileRegion& file_region,
			 const char* name,
			 bool automatic,
			 bool sign,
			 PtIOHeadArray iohead_array,
			 PtDeclHeadArray paramhead_array,
			 PtDeclHeadArray lparamhead_array,
			 PtDeclHeadArray declhead_array,
			 PtStmt* stmt) :
  CptTf(file_region,
	name, automatic,
	iohead_array,
	paramhead_array,
	lparamhead_array,
	declhead_array,
	stmt),
  mSigned(sign)
{
  mInUse = false;
}
  
// デストラクタ
CptFunction::~CptFunction()
{
}

// 型を返す．
tPtItemType
CptFunction::type() const
{
  return kPtItem_Func;
}

// 符号の有無を返す．
bool
CptFunction::is_signed() const
{
  return mSigned;
}

// constant function の展開中の印をつける．
void
CptFunction::set_in_use() const
{
  mInUse = true;
}

// constant function の展開中の印を消す．
void
CptFunction::clear_in_use() const
{
  mInUse = false;
}

// 使用中(constant function として展開中)の時 true を返す．
bool
CptFunction::is_in_use() const
{
  return mInUse;
}


//////////////////////////////////////////////////////////////////////
// ビットベクタタイプの function を表すノード
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptSizedFunc::CptSizedFunc(const FileRegion& file_region,
			   const char* name,
			   bool automatic,
			   bool sign,
			   PtExpr* left,
			   PtExpr* right,
			   PtIOHeadArray iohead_array,
			   PtDeclHeadArray paramhead_array,
			   PtDeclHeadArray lparamhead_array,
			   PtDeclHeadArray declhead_array,
			   PtStmt* stmt) :
  CptFunction(file_region,
	      name, automatic, sign,
	      iohead_array,
	      paramhead_array,
	      lparamhead_array,
	      declhead_array,
	      stmt),
  mLeftRange(left),
  mRightRange(right)
{
}

// デストラクタ
CptSizedFunc::~CptSizedFunc()
{
}

// 範囲の MSB を得る．
const PtExpr*
CptSizedFunc::left_range() const
{
  return mLeftRange;
}

// 範囲の LSB を得る．
const PtExpr*
CptSizedFunc::right_range() const
{
  return mRightRange;
}


//////////////////////////////////////////////////////////////////////
/// 組み込み型の function を表すノード
//////////////////////////////////////////////////////////////////////
  
// コンストラクタ
CptTypedFunc::CptTypedFunc(const FileRegion& file_region,
			   const char* name,
			   bool automatic,
			   bool sign,
			   tVpiVarType data_type,
			   PtIOHeadArray iohead_array,
			   PtDeclHeadArray paramhead_array,
			   PtDeclHeadArray lparamhead_array,
			   PtDeclHeadArray declhead_array,
			   PtStmt* stmt) :
  CptFunction(file_region,
	      name, automatic, sign,
	      iohead_array,
	      paramhead_array,
	      lparamhead_array,
	      declhead_array,
	      stmt),
  mDataType(data_type)
{
}
  
/// デストラクタ
CptTypedFunc::~CptTypedFunc()
{
}

// 戻値の型を返す．
tVpiVarType
CptTypedFunc::data_type() const
{
  return mDataType;
}

  
//////////////////////////////////////////////////////////////////////
// item 関係
//////////////////////////////////////////////////////////////////////

// defparam 文のヘッダを生成する．
PtItem*
CptFactory::new_DefParamH(const FileRegion& file_region,
			  PtDefParamArray elem_array)
{
  ++ mNumDefParamH;
  void* p = alloc().get_memory(sizeof(CptDefParamH));
  return new (p) CptDefParamH(file_region, elem_array);
}

// defparam 文の要素を生成する．
PtDefParam*
CptFactory::new_DefParam(const FileRegion& file_region,
			 const char* name,
			 PtExpr* value)
{
  ++ mNumDefParam;
  void* p = alloc().get_memory(sizeof(CptDefParam));
  return new (p) CptDefParam(file_region, PtNameBranchArray(), name, value);
}

PtDefParam*
CptFactory::new_DefParam(const FileRegion& file_region,
			 PtNameBranchArray nb_array,
			 const char* tail_name,
			 PtExpr* value)
{
  ++ mNumDefParam;
  void* p = alloc().get_memory(sizeof(CptDefParam));
  return new (p) CptDefParam(file_region, nb_array, tail_name, value);
}

// continuous assign 文のヘッダを生成する．
PtItem*
CptFactory::new_ContAssignH(const FileRegion& file_region,
			    PtContAssignArray elem_array)
{
  ++ mNumContAssignH;
  void* p = alloc().get_memory(sizeof(CptContAssignH));
  return new (p) CptContAssignH(file_region, elem_array);
}

// continuous assign 文のヘッダを生成する．
PtItem*
CptFactory::new_ContAssignH(const FileRegion& file_region,
			    PtStrength* strength,
			    PtContAssignArray elem_array)
{
  ++ mNumContAssignHS;
  void* p = alloc().get_memory(sizeof(CptContAssignHS));
  return new (p) CptContAssignHS(file_region, strength, elem_array);
}

// continuous assign 文のヘッダを生成する．
PtItem*
CptFactory::new_ContAssignH(const FileRegion& file_region,
			    PtDelay* delay,
			    PtContAssignArray elem_array)
{
  ++ mNumContAssignHD;
  void* p = alloc().get_memory(sizeof(CptContAssignHD));
  return new (p) CptContAssignHD(file_region, delay, elem_array);
}

// continuous assign 文のヘッダを生成する．
PtItem*
CptFactory::new_ContAssignH(const FileRegion& file_region,
			    PtStrength* strength,
			    PtDelay* delay,
			    PtContAssignArray elem_array)
{
  ++ mNumContAssignHSD;
  void* p = alloc().get_memory(sizeof(CptContAssignHSD));
  return new (p) CptContAssignHSD(file_region, strength, delay, elem_array);
}

// continuous assign 文の要素を生成する．
PtContAssign*
CptFactory::new_ContAssign(const FileRegion& file_region,
			   PtExpr* lhs,
			   PtExpr* rhs)
{
  ++ mNumContAssign;
  // 実は file_region は不要
  void* p = alloc().get_memory(sizeof(CptContAssign));
  return new (p) CptContAssign(lhs, rhs);
}

// initial 文を生成する．
PtItem*
CptFactory::new_Initial(const FileRegion& file_region,
			PtStmt* body)
{
  ++ mNumInitial;
  void* p = alloc().get_memory(sizeof(CptInitial));
  return new (p) CptInitial(file_region, body);
}

// always 文を生成する．
PtItem*
CptFactory::new_Always(const FileRegion& file_region,
		       PtStmt* body)
{
  ++ mNumAlways;
  void* p = alloc().get_memory(sizeof(CptAlways));
  return new (p) CptAlways(file_region, body);
}

// task 文を生成する．
PtItem*
CptFactory::new_Task(const FileRegion& file_region,
		     const char* name,
		     bool automatic,
		     PtIOHeadArray iohead_array,
		     PtDeclHeadArray paramhead_array,
		     PtDeclHeadArray lparamhead_array,
		     PtDeclHeadArray declhead_array,
		     PtStmt* stmt)
{
  ++ mNumTask;
  void* p = alloc().get_memory(sizeof(CptTask));
  return new (p) CptTask(file_region, name, automatic,
			 iohead_array,
			 paramhead_array,
			 lparamhead_array,
			 declhead_array,
			 stmt);
}
  
// function 文を生成する．
PtItem*
CptFactory::new_Function(const FileRegion& file_region,
			 const char* name,
			 bool automatic,
			 bool sign,
			 PtIOHeadArray iohead_array,
			 PtDeclHeadArray paramhead_array,
			 PtDeclHeadArray lparamhead_array,
			 PtDeclHeadArray declhead_array,
			 PtStmt* stmt)
{
  ++ mNumFunction;
  void* p = alloc().get_memory(sizeof(CptFunction));
  return new (p) CptFunction(file_region, name, automatic, sign,
			     iohead_array,
			     paramhead_array,
			     lparamhead_array,
			     declhead_array,
			     stmt);
}

// ビットベクタ型 function の生成
PtItem*
CptFactory::new_SizedFunc(const FileRegion& file_region,
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
  ++ mNumSizedFunc;
  void* p = alloc().get_memory(sizeof(CptSizedFunc));
  return new (p) CptSizedFunc(file_region,
			      name, automatic,
			      sign, left, right,
			      iohead_array,
			      paramhead_array,
			      lparamhead_array,
			      declhead_array,
			      stmt);
}

// 組み込み型 function の生成
PtItem*
CptFactory::new_TypedFunc(const FileRegion& file_region,
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
  ++ mNumTypedFunc;
  void* p = alloc().get_memory(sizeof(CptTypedFunc));
  return new (p) CptTypedFunc(file_region, name,
			      automatic, sign,
			      func_type,
			      iohead_array,
			      paramhead_array,
			      lparamhead_array,
			      declhead_array,
			      stmt);
}

END_NAMESPACE_YM_VERILOG
