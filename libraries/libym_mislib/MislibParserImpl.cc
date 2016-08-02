
/// @file libym_mislib/MisLibParserImpl.cc
/// @brief MislibParserImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MislibParserImpl.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.

#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include "MislibParserImpl.h"
#include <ym_mislib/MislibHandler.h>
#include <ym_utils/FileDescMgr.h>

#include "MislibPtImpl1.h"
#include "MislibPtImpl2.h"


BEGIN_NAMESPACE_YM_MISLIB

#include "mislib_grammer.h"


//////////////////////////////////////////////////////////////////////
// MislibParserImpl
//////////////////////////////////////////////////////////////////////

// コンストラクタ
MislibParserImpl::MislibParserImpl() :
  mAlloc(4096),
  mLex(mMsgMgr)
{
}

// デストラクタ
MislibParserImpl::~MislibParserImpl()
{
  clear();
  
  for (list<MislibHandler*>::iterator p = mHandlerList.begin();
       p != mHandlerList.end(); ++ p) {
    MislibHandler* handler = *p;
    delete handler;
  }
}

// @brief mislib ファイルを読み込む
// @param[in] filename ファイル名
// @retval true 読み込みが成功した．
// @retval false 読み込みが失敗した．
bool
MislibParserImpl::read(const string& filename)
{
  int yyparse(MislibParserImpl& parser);
  
  ifstream input_stream;
  input_stream.open(filename.c_str());
  if ( !input_stream ) {
    // エラー
    ostringstream buf;
    buf << filename << " : No such file.";
    mMsgMgr.put_msg(__FILE__, __LINE__, FileRegion(),
		    kMsgFailure, "MISLIB_PARSER", buf.str());
    return false;
  }
  FileDescMgr fdmgr;
  const FileDesc* file_desc = fdmgr.new_file_desc(filename);

  mLex.init(input_stream, file_desc);

  // 初期化
  for (list<MislibHandler*>::iterator p = mHandlerList.begin();
       p != mHandlerList.end(); ++ p) {
    MislibHandler* handler = *p;
    handler->init();
  }
  mGateHash.clear();
  mError = false;
  
  yyparse(*this);
  
  if ( mError ) {
    // 異常終了
    for (list<MislibHandler*>::iterator p = mHandlerList.begin();
	 p != mHandlerList.end(); ++ p) {
      MislibHandler* handler = *p;
      handler->error_exit();
    }
    return false;
  }
  else {
    // 正常終了
    for (list<MislibHandler*>::iterator p = mHandlerList.begin();
	 p != mHandlerList.end(); ++ p) {
      MislibHandler* handler = *p;
      handler->normal_exit();
    }
    return true;
  }
}

// @brief イベントハンドラの登録
void
MislibParserImpl::add_handler(MislibHandler* handler)
{
  mHandlerList.push_back(handler);
  handler->mParser = this;
}

// 文字列ノードを生成する．
MislibPt*
MislibParserImpl::new_str(const FileRegion& loc,
			  ShString str)
{
  void* p = mAlloc.get_memory(sizeof(MislibPtStr));
  return new (p) MislibPtStr(loc, str);
}

// 数値ノードを生成する．
MislibPt*
MislibParserImpl::new_num(const FileRegion& loc,
			  double num)
{
  void* p = mAlloc.get_memory(sizeof(MislibPtNum));
  return new (p) MislibPtNum(loc, num);
}

// NONINV ノードを生成する．
MislibPt*
MislibParserImpl::new_noninv(const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(MislibPtNoninv));
  return new (p) MislibPtNoninv(loc);
}

// INV ノードを生成する．
MislibPt*
MislibParserImpl::new_inv(const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(MislibPtInv));
  return new (p) MislibPtInv(loc);
}

// UNKNOWN ノードを生成する．
MislibPt*
MislibParserImpl::new_unknown(const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(MislibPtUnknown));
  return new (p) MislibPtUnknown(loc);
}

// 定数0ノードを生成する．
MislibPt*
MislibParserImpl::new_const0(const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(MislibPtConst0));
  return new (p) MislibPtConst0(loc);
}

// 定数1ノードを生成する．
MislibPt*
MislibParserImpl::new_const1(const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(MislibPtConst1));
  return new (p) MislibPtConst1(loc);
}

// NOT ノードを生成する．
MislibPt*
MislibParserImpl::new_not(const FileRegion& loc,
			  MislibPt* child1)
{
  void* p = mAlloc.get_memory(sizeof(MislibPtNot));
  return new (p) MislibPtNot(loc, child1);
}

// AND ノードを生成する．
MislibPt*
MislibParserImpl::new_and(const FileRegion& loc,
			  MislibPt* child1,
			  MislibPt* child2)
{
  void* p = mAlloc.get_memory(sizeof(MislibPtAnd));
  return new (p) MislibPtAnd(loc, child1, child2);
}

// OR ノードを生成する．
MislibPt*
MislibParserImpl::new_or(const FileRegion& loc,
			 MislibPt* child1,
			 MislibPt* child2)
{
  void* p = mAlloc.get_memory(sizeof(MislibPtOr));
  return new (p) MislibPtOr(loc, child1, child2);
}

// PIN ノードを生成する．
MislibPt*
MislibParserImpl::new_pin(const FileRegion& loc,
			  MislibPt* name,
			  MislibPt* phase,
			  MislibPt* input_load,
			  MislibPt* max_load,
			  MislibPt* rise_block_delay,
			  MislibPt* rise_fanout_delay,
			  MislibPt* fall_block_delay,
			  MislibPt* fall_fanout_delay)
{
  void* p = mAlloc.get_memory(sizeof(MislibPtPin));
  return new (p) MislibPtPin(loc,
			     name, phase,
			     input_load, max_load,
			     rise_block_delay, rise_fanout_delay,
			     fall_block_delay, fall_fanout_delay);
}

// PIN リストノードを生成する．
MislibPt*
MislibParserImpl::new_pinlist()
{
  void* p = mAlloc.get_memory(sizeof(MislibPtPinList));
  return new (p) MislibPtPinList();
}

// GATE ノードを生成する．
void
MislibParserImpl::new_gate1(const FileRegion& loc,
			    MislibPt* pt_name,
			    MislibPt* pt_area,
			    MislibPt* pt_oname,
			    MislibPt* pt_expr,
			    MislibPt* pt_ipin_list)
{
  assert_cond(pt_name, __FILE__, __LINE__);
  assert_cond(pt_name->type() == MislibPt::kStr, __FILE__, __LINE__);
  assert_cond(pt_area, __FILE__, __LINE__);
  assert_cond(pt_area->type() == MislibPt::kNum, __FILE__, __LINE__);
  assert_cond(pt_oname, __FILE__, __LINE__);
  assert_cond(pt_oname->type() == MislibPt::kStr, __FILE__, __LINE__);
  assert_cond(pt_expr, __FILE__, __LINE__);
  assert_cond(pt_expr->is_expr(), __FILE__, __LINE__);
  assert_cond(pt_ipin_list, __FILE__, __LINE__);
  assert_cond(pt_ipin_list->type() == MislibPt::kPinList, __FILE__, __LINE__);
  
  ShString name = pt_name->str();
  FileRegion name_loc = pt_name->loc();

  double area = pt_area->num();
  FileRegion area_loc = pt_area->loc();
  
  ShString oname = pt_oname->str();
  FileRegion oname_loc = pt_oname->loc();
  
  // ゲート名の重複チェック
  if ( reg_gate_name(name, name_loc) ) {
    return;
  }
  
  // 入力ピン数を数える
  size_t ni = 0;
  vector<MislibPin> inputs;
  for (MislibPt* tmp = pt_ipin_list->pin();
       tmp; tmp = tmp->next(), ++ ni) {
    assert_cond(tmp->type() == MislibPt::kPin, __FILE__, __LINE__);
    assert_cond(tmp->name(), __FILE__, __LINE__);
  }
  
  // 入力ピンの情報を inputs に入れる．
  inputs.resize(ni);
  size_t i = 0;
  for (MislibPt* tmp = pt_ipin_list->pin();
       tmp; tmp = tmp->next(), ++ i) {
    pt_to_pin(tmp, inputs[i]);
  }
    
  // 入力ピン名のチェックを行う．
  bool error = false;
  for (size_t i = 0; i < ni; ++ i) {
    const MislibPin& pin = inputs[i];
    ShString id1 = pin.name();
    if ( id1 == pt_oname->str() ) {
      // 出力ピン名と同じ名前だった．
      ostringstream buf;
      buf << id1 << " : Defined as both input and output."
	  << " Previous definition is " << pt_oname->loc() << ".";
      mMsgMgr.put_msg(__FILE__, __LINE__, pin.name_loc(),
		      kMsgError, "MislibParser-02", buf.str());
      error = true;
    }
    for (size_t j =0; j < i; ++ j) {
      const MislibPin& pin0 = inputs[j];
      ShString id0 = pin0.name();
      if ( id0 == id1 ) {
	ostringstream buf;
	buf << id0 << " : Defined twice."
	    << " Previous definition is " << pin0.name_loc() << ".";
	mMsgMgr.put_msg(__FILE__, __LINE__, pin.name_loc(),
			kMsgError, "MislibParser-03", buf.str());
	error = true;
	break;
      }
    }
  }
  if ( error ) {
    return;
  }
  
  // 論理式を作る．
  vector<MislibPt*> names;
  LogExpr expr = make_expr(pt_expr, names);

  // 論理式中に現れた名前とピン名の対応をとる．
  VarVarMap varmap;
  size_t nn = names.size();
  for (size_t i = 0; i < nn; ++ i) {
    ShString name = names[i]->str();
    bool found = false;
    for (size_t pos = 0; pos < ni; ++ pos) {
      if ( name == inputs[pos].name() ) {
	varmap.insert(make_pair(i, pos));
	found = true;
	break;
      }
    }
    if ( !found ) {
      // 論理式中に現れる名前がピンリストの中になかった．
      ostringstream buf;
      buf << name << ": No such pin.";
      mMsgMgr.put_msg(__FILE__, __LINE__, names[i]->loc(),
		      kMsgError, "MislibParser-04", buf.str());
    }
  }
  // ピン名が論理式中に現れているかチェックする．
  // 現れていなくても warning とする．
  for (size_t pos = 0; pos < ni; ++ pos) {
    ShString name = inputs[pos].name();
    bool found = false;
    for (size_t i = 0; i < nn; ++ i) {
      if ( name == names[i]->str() ) {
	found = true;
	break;
      }
    }
    if ( !found ) {
      ostringstream buf;
      buf << name << ": Unused.";
      mMsgMgr.put_msg(__FILE__, __LINE__, inputs[pos].name_loc(),
		      kMsgWarning, "MislibParser-05", buf.str());
    }
  }
  // ピン名の順番と論理式のリテラル番号を合わせる．
  expr = expr.remap_var(varmap);
  
  FileRegion expr_loc = pt_expr->loc();
  
  // ハンドラの処理関数を呼び出す．
  for (list<MislibHandler*>::iterator p = mHandlerList.begin();
       p != mHandlerList.end(); ++ p) {
    MislibHandler* handler = *p;
    handler->gate(loc, name, name_loc, area, area_loc,
		  oname, oname_loc, expr, expr_loc, inputs);
  }
  
  // MislibPt のオブジェクトを開放する．
  mAlloc.destroy();
}

// GATE ノードを生成する．
void
MislibParserImpl::new_gate2(const FileRegion& loc,
			    MislibPt* pt_name,
			    MislibPt* pt_area,
			    MislibPt* pt_oname,
			    MislibPt* pt_expr,
			    MislibPt* pt_ipin)
{
  assert_cond(pt_name, __FILE__, __LINE__);
  assert_cond(pt_name->type() == MislibPt::kStr, __FILE__, __LINE__);
  assert_cond(pt_area, __FILE__, __LINE__);
  assert_cond(pt_area->type() == MislibPt::kNum, __FILE__, __LINE__);
  assert_cond(pt_oname, __FILE__, __LINE__);
  assert_cond(pt_oname->type() == MislibPt::kStr, __FILE__, __LINE__);
  assert_cond(pt_expr, __FILE__, __LINE__);
  assert_cond(pt_expr->is_expr(), __FILE__, __LINE__);
  assert_cond(pt_ipin, __FILE__, __LINE__);
  assert_cond(pt_ipin->type() == MislibPt::kPin, __FILE__, __LINE__);
  assert_cond(pt_ipin->name() == NULL, __FILE__, __LINE__);
  
  ShString name = pt_name->str();
  FileRegion name_loc = pt_name->loc();

  double area = pt_area->num();
  FileRegion area_loc = pt_area->loc();
  
  ShString oname = pt_oname->str();
  FileRegion oname_loc = pt_oname->loc();
  
  // ゲート名の重複チェック
  if ( reg_gate_name(name, name_loc) ) {
    return;
  }
  
  // ワイルドカードなのでピン数はわからないので取り合えず
  // テンプレートの情報を tmp_input に入れておく．
  MislibPin tmp_input;
  pt_to_pin(pt_ipin, tmp_input);
  
  // 論理式を作る．
  vector<MislibPt*> names;
  LogExpr expr = make_expr(pt_expr, names);
  FileRegion expr_loc = pt_expr->loc();
  
  // その際に得られた名前からピンの情報を作る．
  size_t n = names.size();
  vector<MislibPin> inputs(n);
  for (size_t i = 0; i < n; ++ i) {
    MislibPin& pin = inputs[i];
    pin = tmp_input;
    pin.mName = names[i]->str();
    pin.mLocArray[1] = names[i]->loc();
  }

  // ハンドラの処理関数を呼び出す．
  for (list<MislibHandler*>::iterator p = mHandlerList.begin();
       p != mHandlerList.end(); ++ p) {
    MislibHandler* handler = *p;
    handler->gate(loc, name, name_loc, area, area_loc,
		  oname, oname_loc, expr, expr_loc, inputs);
  }
  
  // MislibPt のオブジェクトを開放する．
  mAlloc.destroy();
}

// GATE ノードを生成する．
void
MislibParserImpl::new_gate3(const FileRegion& loc,
			    MislibPt* pt_name,
			    MislibPt* pt_area,
			    MislibPt* pt_oname,
			    MislibPt* pt_expr)
{
  assert_cond(pt_name, __FILE__, __LINE__);
  assert_cond(pt_name->type() == MislibPt::kStr, __FILE__, __LINE__);
  assert_cond(pt_area, __FILE__, __LINE__);
  assert_cond(pt_area->type() == MislibPt::kNum, __FILE__, __LINE__);
  assert_cond(pt_oname, __FILE__, __LINE__);
  assert_cond(pt_oname->type() == MislibPt::kStr, __FILE__, __LINE__);
  assert_cond(pt_expr, __FILE__, __LINE__);
  assert_cond(pt_expr->is_expr(), __FILE__, __LINE__);
  
  ShString name = pt_name->str();
  FileRegion name_loc = pt_name->loc();

  double area = pt_area->num();
  FileRegion area_loc = pt_area->loc();
  
  ShString oname = pt_oname->str();
  FileRegion oname_loc = pt_oname->loc();
  
  // ゲート名の重複チェック
  if ( reg_gate_name(name, name_loc) ) {
    return;
  }

  // 論理式が定数式かどうかチェックする．
  LogExpr expr;
  switch ( pt_expr->type() ) {
  case MislibPt::kConst0:
    expr = LogExpr::make_zero();
    break;
    
  case MislibPt::kConst1:
    expr = LogExpr::make_one();
    break;

  default:
    mMsgMgr.put_msg(__FILE__, __LINE__, pt_expr->loc(),
		    kMsgError, "MislibParser-06",
		    "Constant expression (CONST0 or CONST1) expected.");
    return;
  }
  FileRegion expr_loc = pt_expr->loc();
  
  // ハンドラの処理関数を呼び出す．
  for (list<MislibHandler*>::iterator p = mHandlerList.begin();
       p != mHandlerList.end(); ++ p) {
    MislibHandler* handler = *p;
    handler->gate(loc, name, name_loc, area, area_loc,
		  oname, oname_loc, expr, expr_loc,
		  vector<MislibPin>());
  }
  
  // MislibPt のオブジェクトを開放する．
  mAlloc.destroy();
}
  
// @brief ゲート名の登録
// @return 重複していたら true を返す．
bool
MislibParserImpl::reg_gate_name(const ShString& name,
				const FileRegion& name_loc)
{
  hash_map<ShString, FileRegion>::iterator p = mGateHash.find(name);
  if ( p != mGateHash.end() ) {
    ostringstream buf;
    buf << name << " : Defined twice."
	<< " Previous definition is " << p->second << ".";
    mMsgMgr.put_msg(__FILE__, __LINE__, name_loc,
		    kMsgError, "MislibParser-01", buf.str());
    return true;
  }
  mGateHash.insert(make_pair(name, name_loc));
  return false;
}

// @brief MislibPt から MislibPin へ情報を移す．
void
MislibParserImpl::pt_to_pin(MislibPt* pt_pin,
			    MislibPin& pin)
{
  if ( pt_pin->name() ) {
    pin.mName = pt_pin->name()->str();
    pin.mLocArray[1] = pt_pin->name()->loc();
  }
  else {
    // この時の値は未定
    // 実際には後で設定されるはず．
  }
	
  switch ( pt_pin->phase()->type() ) {
  case MislibPt::kNoninv:
    pin.mPhase = MislibPin::kNoninv;
    break;
	  
  case MislibPt::kInv:
    pin.mPhase = MislibPin::kInv;
    break;
    
  case MislibPt::kUnknown:
    pin.mPhase = MislibPin::kUnknown;
    break;
	  
  default:
    assert_not_reached(__FILE__, __LINE__);
  }
      
  pin.mInputLoad = pt_pin->input_load()->num();
  pin.mMaxLoad = pt_pin->max_load()->num();
  pin.mRiseBlockDelay = pt_pin->rise_block_delay()->num();
  pin.mRiseFanoutDelay = pt_pin->rise_fanout_delay()->num();
  pin.mFallBlockDelay = pt_pin->fall_block_delay()->num();
  pin.mFallFanoutDelay = pt_pin->fall_fanout_delay()->num();
  pin.mLocArray[0] = pt_pin->loc();
  pin.mLocArray[2] = pt_pin->phase()->loc();
  pin.mLocArray[3] = pt_pin->input_load()->loc();
  pin.mLocArray[4] = pt_pin->max_load()->loc();
  pin.mLocArray[5] = pt_pin->rise_block_delay()->loc();
  pin.mLocArray[6] = pt_pin->rise_fanout_delay()->loc();
  pin.mLocArray[7] = pt_pin->fall_block_delay()->loc();
  pin.mLocArray[8] = pt_pin->fall_fanout_delay()->loc();
}
  
// @brief MislibPt を論理式に変換する．
// @param[in] pt 論理式の根のノード
// @param[out] names 論理式中に現れた名前を入れる配列
// @return 論理式を返す．
LogExpr
MislibParserImpl::make_expr(MislibPt* pt,
			    vector<MislibPt*>& names)
{
  switch ( pt->type() ) {
  case MislibPt::kConst0:
    return LogExpr::make_zero();
    
  case MislibPt::kConst1:
    return LogExpr::make_one();

  case MislibPt::kStr:
    for (size_t i = 0; i < names.size(); ++ i) {
      if ( pt->str() == names[i]->str() ) {
	return LogExpr::make_posiliteral(i);
      }
    }
    names.push_back(pt);
    return LogExpr::make_posiliteral(names.size() - 1);

  case MislibPt::kNot:
    return ~make_expr(pt->child1(), names);
    
  case MislibPt::kAnd:
    return make_expr(pt->child1(), names) & make_expr(pt->child2(), names);
    
  case MislibPt::kOr:
    return make_expr(pt->child1(), names) | make_expr(pt->child2(), names);

  default:
    break;
  }
  assert_not_reached(__FILE__, __LINE__);
  // ダミー
  return LogExpr::make_zero();
}

// 今までに生成したすべてのオブジェクトを解放する．
void
MislibParserImpl::clear()
{
  mAlloc.destroy();
}
  
// @brief 字句解析を行う．
// @param[out] lval トークンの値を格納する変数
// @param[out] lloc トークンの位置を格納する変数
// @return トークンの型を返す．
int
MislibParserImpl::scan(MislibPt*& lval,
		       FileRegion& lloc)
{
  int tok = mLex.read_token();
  lloc = mLex.cur_loc();
  
  switch ( tok ) {
  case STR:
    lval = new_str(lloc, ShString(mLex.cur_string()).id());
    break;
    
  case NUM:
    lval = new_num(lloc, mLex.cur_num());
    break;
    
  case NONINV:
    lval = new_noninv(lloc);
    break;
    
  case INV:
    lval = new_inv(lloc);
    break;
    
  case UNKNOWN:
    lval = new_unknown(lloc);
    break;
    
  case CONST0:
    lval = new_const0(lloc);
    break;
    
  case CONST1:
    lval = new_const1(lloc);
    break;
    
  }
  return tok;
}

END_NAMESPACE_YM_MISLIB
