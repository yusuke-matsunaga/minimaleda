
/// @file libym_verilog/elb_impl/EiTaskFunc.cc
/// @brief EiTaskFunc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiTaskFunc.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiTaskFunc.h"
#include "EiIODecl.h"
#include "ElbStmt.h"
#include "ElbDecl.h"

#include "ym_verilog/VlTime.h"
#include "ym_verilog/BitVector.h"
#include "ym_verilog/pt/PtDecl.h"
#include "ym_verilog/pt/PtItem.h"



BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief function を生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_item パース木の定義
// @param[in] left 範囲の MSB の式
// @param[in] right 範囲の LSB の式
// @param[in] left_val 範囲の MSB の値
// @param[in] right_val 範囲の LSB の値
ElbTaskFunc*
EiFactory::new_Function(const VlNamedObj* parent,
			const PtItem* pt_item,
			ElbExpr* left,
			ElbExpr* right,
			int left_val,
			int right_val)
{
  assert_cond( left != NULL && right != NULL, __FILE__, __LINE__);

  // IO数を数え配列を初期化する．
  ymuint32 io_num = pt_item->ioitem_num();
  void* q = mAlloc.get_memory(sizeof(EiIODecl) * io_num);
  EiIODecl* io_array = new (q) EiIODecl[io_num];

  void* p = mAlloc.get_memory(sizeof(EiFunctionV));
  EiFunction* func = new (p) EiFunctionV(parent, pt_item, io_num, io_array,
					 left, right, left_val, right_val);

  return func;
}

// @brief function を生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_item パース木の定義
ElbTaskFunc*
EiFactory::new_Function(const VlNamedObj* parent,
			const PtItem* pt_item)
{
  // IO数を数え配列を初期化する．
  ymuint32 io_num = pt_item->ioitem_num();
  void* q = mAlloc.get_memory(sizeof(EiIODecl) * io_num);
  EiIODecl* io_array = new (q) EiIODecl[io_num];

  void* p = mAlloc.get_memory(sizeof(EiFunction));
  EiFunction* func = new (p) EiFunction(parent, pt_item, io_num, io_array);

  return func;
}

// @brief task を生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_item パース木の定義
ElbTaskFunc*
EiFactory::new_Task(const VlNamedObj* parent,
		    const PtItem* pt_item)
{
  // IO数を数え配列を初期化する．
  ymuint32 io_num = pt_item->ioitem_num();
  void* q = mAlloc.get_memory(sizeof(EiIODecl) * io_num);
  EiIODecl* io_array = new (q) EiIODecl[io_num];

  void* p = mAlloc.get_memory(sizeof(EiTask));
  EiTask* task = new (p) EiTask(parent, pt_item, io_num, io_array);
  return task;
}


//////////////////////////////////////////////////////////////////////
// クラス EiTaskFunc
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ環境
// @param[in] pt_item パース木の定義
// @param[in] io_num IOの数
// @param[in] io_array IO の配列
EiTaskFunc::EiTaskFunc(const VlNamedObj* parent,
		       const PtItem* pt_item,
		       ymuint32 io_num,
		       EiIODecl* io_array):
  mParent(parent),
  mPtItem(pt_item),
  mIODeclNum(io_num),
  mIODeclList(io_array),
  mStmt(NULL)
{
}

// @brief デストラクタ
EiTaskFunc::~EiTaskFunc()
{
}

// @brief ファイル位置の取得
FileRegion
EiTaskFunc::file_region() const
{
  return mPtItem->file_region();
}

// @brief このオブジェクトの属しているスコープを返す．
const VlNamedObj*
EiTaskFunc::parent() const
{
  return mParent;
}

// @brief 名前の取得
const char*
EiTaskFunc::name() const
{
  return mPtItem->name();
}

// @brief automatic 宣言されていたら true を返す．
bool
EiTaskFunc::automatic() const
{
  return mPtItem->automatic();
}

// @brief 入出力数を得る．
ymuint32
EiTaskFunc::io_num() const
{
  return mIODeclNum;
}

// @brief 入出力を得る．
// @param[in] pos 位置番号 ( 0 <= pos < io_num() )
const VlIODecl*
EiTaskFunc::io(ymuint32 pos) const
{
  return &mIODeclList[pos];
}

// @brief 本体のステートメントを得る．
const VlStmt*
EiTaskFunc::stmt() const
{
  return mStmt;
}

// @brief 入出力の初期設定を行う．
// @param[in] pos 位置番号
// @param[in] pos 位置番号
// @param[in] head ヘッダ
// @param[in] pt_item パース木のIO宣言要素
// @param[in] decl 対応する宣言要素
void
EiTaskFunc::init_iodecl(ymuint32 pos,
			ElbIOHead* head,
			const PtIOItem* pt_item,
			ElbDecl* decl)
{
  mIODeclList[pos].init(head, pt_item, decl);
}

// @brief 本体のステートメントをセットする．
void
EiTaskFunc::set_stmt(ElbStmt* stmt)
{
  mStmt = stmt;
}

// @brief 入出力を得る．
// @param[in] pos 位置番号 ( 0 <= pos < io_num() )
ElbIODecl*
EiTaskFunc::_io(ymuint32 pos) const
{
  return &mIODeclList[pos];
}

// @brief 本体の ElbStmt を得る．
ElbStmt*
EiTaskFunc::_stmt() const
{
  return mStmt;
}


//////////////////////////////////////////////////////////////////////
// クラス EiTask
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ環境
// @param[in] pt_item パース木の定義
// @param[in] io_num IOの数
// @param[in] io_array IO の配列
EiTask::EiTask(const VlNamedObj* parent,
	       const PtItem* pt_item,
	       ymuint32 io_num,
	       EiIODecl* io_array) :
  EiTaskFunc(parent, pt_item, io_num, io_array)
{
}

// @brief デストラクタ
EiTask::~EiTask()
{
}

// @brief 型の取得
tVpiObjType
EiTask::type() const
{
  return kVpiTask;
}

// @brief function type を返す．
tVpiFuncType
EiTask::func_type() const
{
  return kVpiIntFunc;
}

// @brief 出力のビット幅を返す．
ymuint32
EiTask::bit_size() const
{
  return 0;
}

// @brief 符号付きの時 true を返す．
bool
EiTask::is_signed() const
{
  return false;
}

// @brief 範囲のMSBを返す．
const VlExpr*
EiTask::left_range() const
{
  return NULL;
}

// @brief 範囲のLSBを返す．
const VlExpr*
EiTask::right_range() const
{
  return NULL;
}

// @brief 範囲の MSB の値を返す．
// @retval 範囲のMSBの値 範囲指定を持つとき
// @retval -1 範囲指定を持たないとき
// @note このクラスでは -1 を返す．
int
EiTask::left_range_const() const
{
  return -1;
}

// @brief 範囲の LSB の値を返す．
// @retval 範囲のLSBの値 範囲指定を持つとき
// @retval -1 範囲指定を持たないとき
// @note このクラスでは -1 を返す．
int
EiTask::right_range_const() const
{
  return -1;
}

// @brief 出力変数をセットする．
// @param[in] ovar 出力変数
void
EiTask::set_ovar(ElbDecl* ovar)
{
  assert_not_reached(__FILE__, __LINE__);
}

// @brief constant function の時に true を返す．
bool
EiTask::is_constant_function() const
{
  assert_not_reached(__FILE__, __LINE__);
  return false;
}

// @brief スカラー値を返す．
// @param[in] arg_list 引数のリスト
// @note constant function の場合のみ意味を持つ．
tVpiScalarVal
EiTask::eval_scalar(const vector<ElbExpr*>& arg_list) const
{
  assert_not_reached(__FILE__, __LINE__);
  return kVpiScalar0;
}

// @brief 論理値を返す．
// @param[in] arg_list 引数のリスト
// @note constant function の場合のみ意味を持つ．
tVpiScalarVal
EiTask::eval_logic(const vector<ElbExpr*>& arg_list) const
{
  assert_not_reached(__FILE__, __LINE__);
  return kVpiScalar0;
}

// @brief real 型の値を返す．
// @param[in] arg_list 引数のリスト
// @note constant function の場合のみ意味を持つ．
double
EiTask::eval_real(const vector<ElbExpr*>& arg_list) const
{
  assert_not_reached(__FILE__, __LINE__);
  return 0.0;
}

// @brief bitvector 型の値を返す．
// @param[in] arg_list 引数のリスト
// @note constant function の場合のみ意味を持つ．
void
EiTask::eval_bitvector(const vector<ElbExpr*>& arg_list,
		       BitVector& bitvector,
		       tVpiValueType req_type) const
{
  assert_not_reached(__FILE__, __LINE__);
}

// @brief 範囲のMSBの取得
// @retval 範囲のMSB 範囲を持つとき
// @retval NULL 範囲を持たないとき
// @note このクラスでは NULL を返す．
ElbExpr*
EiTask::_left_range() const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief 範囲のLSBの取得
// @retval 範囲のLSB 範囲を持つとき
// @retval NULL 範囲を持たないとき
// @note このクラスでは NULL を返す．
ElbExpr*
EiTask::_right_range() const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス EiFunction
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ環境
// @param[in] pt_item パース木の定義
// @param[in] io_num IOの数
// @param[in] io_array IO の配列
EiFunction::EiFunction(const VlNamedObj* parent,
		       const PtItem* pt_item,
		       ymuint32 io_num,
		       EiIODecl* io_array) :
  EiTaskFunc(parent, pt_item, io_num, io_array)
{
}

// @brief デストラクタ
EiFunction::~EiFunction()
{
}

// @brief 型の取得
tVpiObjType
EiFunction::type() const
{
  return kVpiFunction;
}

// @brief function type を返す．
tVpiFuncType
EiFunction::func_type() const
{
  switch ( pt_item()->data_type() ) {
  case kVpiVarNone:
    return kVpiSizedFunc;

  case kVpiVarInteger:
    return kVpiIntFunc;

  case kVpiVarReal:
    return kVpiRealFunc;

  case kVpiVarTime:
    return kVpiTimeFunc;

  case kVpiVarRealtime:
    return kVpiRealtimeFunc;

  default:
    break;
  }
  assert_not_reached(__FILE__, __LINE__);
  return kVpiIntFunc;
}

// @brief 出力のビット幅を返す．
ymuint32
EiFunction::bit_size() const
{
  switch ( pt_item()->data_type() ) {
  case kVpiVarNone:
    return 1;

  case kVpiVarInteger:
    return kVpiSizeInteger;

  case kVpiVarReal:
  case kVpiVarRealtime:
    return kVpiSizeReal;

  case kVpiVarTime:
    return kVpiSizeTime;

  default:
    break;
  }
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}

// @brief 符号付きの時 true を返す．
bool
EiFunction::is_signed() const
{
  return pt_item()->is_signed();
}

// @brief 範囲のMSBを返す．
const VlExpr*
EiFunction::left_range() const
{
  return _left_range();
}

// @brief 範囲のLSBを返す．
const VlExpr*
EiFunction::right_range() const
{
  return _right_range();
}

// @brief 範囲の MSB の値を返す．
// @retval 範囲のMSBの値 範囲指定を持つとき
// @retval -1 範囲指定を持たないとき
// @note このクラスでは -1 を返す．
int
EiFunction::left_range_const() const
{
  return -1;
}

// @brief 範囲の LSB の値を返す．
// @retval 範囲のLSBの値 範囲指定を持つとき
// @retval -1 範囲指定を持たないとき
// @note このクラスでは -1 を返す．
int
EiFunction::right_range_const() const
{
  return -1;
}

// @brief 出力変数をセットする．
// @param[in] ovar 出力変数
// @note 関数の場合のみ意味を持つ．
void
EiFunction::set_ovar(ElbDecl* ovar)
{
  mOvar = ovar;
}

// @brief 範囲のMSBの取得
// @retval 範囲のMSB 範囲を持つとき
// @retval NULL 範囲を持たないとき
// @note このクラスでは NULL を返す．
ElbExpr*
EiFunction::_left_range() const
{
  return NULL;
}

// @brief 範囲のLSBの取得
// @retval 範囲のLSB 範囲を持つとき
// @retval NULL 範囲を持たないとき
// @note このクラスでは NULL を返す．
ElbExpr*
EiFunction::_right_range() const
{
  return NULL;
}

// @brief constant function の時に true を返す．
bool
EiFunction::is_constant_function() const
{
  return false;
}

// @brief スカラー値を返す．
// @param[in] arg_list 引数のリスト
// @note constant function の場合のみ意味を持つ．
tVpiScalarVal
EiFunction::eval_scalar(const vector<ElbExpr*>& arg_list) const
{
  evaluate(arg_list);
  return mOvar->get_scalar();
}

// @brief 論理値を返す．
// @param[in] arg_list 引数のリスト
// @note constant function の場合のみ意味を持つ．
tVpiScalarVal
EiFunction::eval_logic(const vector<ElbExpr*>& arg_list) const
{
  evaluate(arg_list);
  return mOvar->get_logic();
}

// @brief real 型の値を返す．
// @param[in] arg_list 引数のリスト
// @note constant function の場合のみ意味を持つ．
double
EiFunction::eval_real(const vector<ElbExpr*>& arg_list) const
{
  evaluate(arg_list);
  return mOvar->get_real();
}

// @brief bitvector 型の値を返す．
// @param[in] arg_list 引数のリスト
// @note constant function の場合のみ意味を持つ．
void
EiFunction::eval_bitvector(const vector<ElbExpr*>& arg_list,
			   BitVector& bitvector,
			   tVpiValueType req_type) const
{
  evaluate(arg_list);
  mOvar->get_bitvector(bitvector, req_type);
}

// @brief 関数の値の評価を行う．
// @param[in] arg_list 引数のリスト
// @note constant function の場合のみ意味を持つ．
void
EiFunction::evaluate(const vector<ElbExpr*>& arg_list) const
{
  assert_cond(arg_list.size() == io_num(), __FILE__, __LINE__);
  ymuint32 n = arg_list.size();
  for (ymuint32 i = 0; i < n; ++ i) {
    ElbExpr* expr = arg_list[i];
    ElbIODecl* io_decl = _io(i);
    ElbDecl* decl = io_decl->_decl();
    if ( decl->bit_size() == 1 ) {
      decl->set_scalar(expr->eval_scalar());
    }
    else if ( decl->value_type() == kVpiValueReal ) {
      decl->set_real(expr->eval_real());
    }
    else {
      BitVector tmp;
      expr->eval_bitvector(tmp, decl->value_type());
      decl->set_bitvector(tmp);
    }
  }
  _stmt()->func_exec(true);
}


//////////////////////////////////////////////////////////////////////
// クラス EiFunctionV
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ環境
// @param[in] pt_item パース木の定義
// @param[in] io_num IOの数
// @param[in] io_array IO の配列
// @param[in] left 範囲の MSB の式
// @param[in] right 範囲の LSB の式
// @param[in] left_val 範囲の MSB の値
// @param[in] right_val 範囲の LSB の値
EiFunctionV::EiFunctionV(const VlNamedObj* parent,
			 const PtItem* pt_item,
			 ymuint32 io_num,
			 EiIODecl* io_array,
			 ElbExpr* left,
			 ElbExpr* right,
			 int left_val,
			 int right_val) :
  EiFunction(parent, pt_item, io_num, io_array)
{
  mRange.set(left, right, left_val, right_val);
}

// @brief デストラクタ
EiFunctionV::~EiFunctionV()
{
}

// @brief 範囲の MSB の値を返す．
// @retval 範囲のMSBの値 範囲指定を持つとき
// @retval -1 範囲指定を持たないとき
int
EiFunctionV::left_range_const() const
{
  return mRange.left_range_const();
}

// @brief 範囲の LSB の値を返す．
// @retval 範囲のLSBの値 範囲指定を持つとき
// @retval -1 範囲指定を持たないとき
int
EiFunctionV::right_range_const() const
{
  return mRange.right_range_const();
}

// @brief 出力のビット幅を返す．
ymuint32
EiFunctionV::bit_size() const
{
  return mRange.size();
}

// @brief 範囲のMSBを返す．
ElbExpr*
EiFunctionV::_left_range() const
{
  return mRange.left_range();
}

// @brief 範囲のLSBを返す．
ElbExpr*
EiFunctionV::_right_range() const
{
  return mRange.right_range();
}

END_NAMESPACE_YM_VERILOG
