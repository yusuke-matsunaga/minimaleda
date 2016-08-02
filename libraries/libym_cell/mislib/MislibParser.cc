
/// @file libym_cell/mislib/MislibParser.cc
/// @brief MislibParser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MislibParser.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MislibParser.h"
#include "ym_utils/FileDescMgr.h"
#include "ym_lexp/LogExpr.h"
#if 0
#include "ym_npn/TvFunc.h"
#endif

#include "MislibPtImpl1.h"
#include "MislibPtImpl2.h"

#include "../ci/CiLibrary.h"


BEGIN_NAMESPACE_YM_CELL

#include "mislib_grammer.h"

//////////////////////////////////////////////////////////////////////
// MislibParser
//////////////////////////////////////////////////////////////////////

// コンストラクタ
MislibParser::MislibParser() :
  mAlloc(4096),
  mLex(mMsgMgr)
{
}

// デストラクタ
MislibParser::~MislibParser()
{
  clear();
}


BEGIN_NONAMESPACE

// 論理式を表すパース木をスキャンして端子名のリストを作る．
// param[in] node パース木の根のノード
// param[out] name_list 名前が現れた順に格納されたリスト
// param[out] name_map 名前をキーにして name_list 中の位置を返す連想配列
void
dfs(const MislibPt* node,
    vector<ShString>& name_list,
    hash_map<ShString, ymuint>& name_map)
{
  switch ( node->type() ) {
  case MislibPt::kConst0:
  case MislibPt::kConst1:
    return;

  case MislibPt::kStr:
    {
      ShString name = node->str();
      if ( name_map.count(name) == 0 ) {
	// 登録する．
	name_map.insert(make_pair(name, name_list.size()));
	name_list.push_back(name);
      }
    }
    break;

  case MislibPt::kNot:
    dfs(node->child1(), name_list, name_map);
    break;

  case MislibPt::kAnd:
  case MislibPt::kOr:
    dfs(node->child1(), name_list, name_map);
    dfs(node->child2(), name_list, name_map);
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
  }
}

// MislibPt を論理式に変換する．
// param[in] node パース木の根のノード
// param[in] name_map 端子名をキーにして端子番号を返す連想配列
LogExpr
pt_to_expr(const MislibPt* node,
	   const hash_map<ShString, ymuint>& name_map)
{
  switch ( node->type() ) {
  case MislibPt::kConst0:
    return LogExpr::make_zero();

  case MislibPt::kConst1:
    return LogExpr::make_one();

  case MislibPt::kStr:
    {
      ShString name = node->str();
      hash_map<ShString, ymuint>::const_iterator p = name_map.find(name);
      assert_cond( p != name_map.end(), __FILE__, __LINE__);
      return LogExpr::make_posiliteral(p->second);
    }

  case MislibPt::kNot:
    return ~pt_to_expr(node->child1(), name_map);

  case MislibPt::kAnd:
    {
      LogExpr expr1 = pt_to_expr(node->child1(), name_map);
      LogExpr expr2 = pt_to_expr(node->child2(), name_map);
      return expr1 & expr2;
    }

  case MislibPt::kOr:
    {
      LogExpr expr1 = pt_to_expr(node->child1(), name_map);
      LogExpr expr2 = pt_to_expr(node->child2(), name_map);
      return expr1 | expr2;
    }

  default:
    assert_not_reached(__FILE__, __LINE__);
  }

  // ダミー
  return LogExpr::make_zero();
}

#if 0
// @brief LogExpr を TvFunc に変換する．
// @param[in] expr 対象の論理式
// @param[in] ni 全入力数
TvFunc
expr_to_tvfunc(const LogExpr& expr,
	       ymuint ni)
{
  if ( expr.is_zero() ) {
    return TvFunc::const_zero(ni);
  }
  if ( expr.is_one() ) {
    return TvFunc::const_one(ni);
  }
  if ( expr.is_posiliteral() ) {
    return TvFunc::posi_literal(ni, expr.varid());
  }
  if ( expr.is_negaliteral() ) {
    return TvFunc::nega_literal(ni, expr.varid());
  }
  // あとは AND/OR/XOR のみ
  ymuint n = expr.child_num();
  vector<TvFunc> child_func(n);
  for (ymuint i = 0; i < n; ++ i) {
    child_func[i] = expr_to_tvfunc(expr.child(i), ni);
  }
  TvFunc func = child_func[0];
  if ( expr.is_and() ) {
    for (ymuint i = 1; i < n; ++ i) {
      func &= child_func[i];
    }
  }
  else if ( expr.is_or() ) {
    for (ymuint i = 1; i < n; ++ i) {
      func |= child_func[i];
    }
  }
  else if ( expr.is_xor() ) {
    for (ymuint i = 1; i < n; ++ i) {
      func ^= child_func[i];
    }
  }
  else {
    assert_not_reached(__FILE__, __LINE__);
  }
  return func;
}
#endif

END_NONAMESPACE

// @brief mislib ファイルを読み込んでライブラリを生成する．
// @param[in] filename ファイル名
// @return 生成したライブラリを返す．
// @note 読み込みが失敗したら NULL を返す．
const CellLibrary*
MislibParser::read(const string& filename)
{
  int yyparse(MislibParser& parser);

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
  clear();
  mGateList = new_list();
  mError = false;

  // パース木を作る．
  // 結果は mGateList に入っている．
  yyparse(*this);

  if ( mError ) {
    // 異常終了
    return NULL;
  }

  // 読み込みまではうまくいった．

  // 名前が重複していないかチェック
  hash_map<ShString, const MislibPt*> gate_map;
  for (const MislibPt* gate = mGateList->top(); gate; gate = gate->next()) {
    ShString name = gate->name()->str();
    hash_map<ShString, const MislibPt*>::iterator p = gate_map.find(name);
    if ( p != gate_map.end() ) {
      // TODO エラーメッセージをちゃんとする．
      cerr << name << " is already defined" << endl;
      mError = true;
    }
    else {
      gate_map.insert(make_pair(name, gate));
    }
  }
  if ( mError ) {
    return NULL;
  }

  // ライブラリの生成
  CiLibrary* library = new CiLibrary(filename);

  // セル数の設定
  ymuint cell_num = gate_map.size();
  library->set_cell_num(cell_num);

  // セルの内容の設定
  ymuint cell_id = 0;
  for (const MislibPt* gate = mGateList->top(); gate;
       gate = gate->next(), ++ cell_id) {
    ShString name = gate->name()->str();
    CellArea area(gate->area()->num());
    ShString opin_name = gate->opin_name()->str();
    const MislibPt* opin_expr = gate->opin_expr();
    const MislibPt* ipin_list = gate->ipin_list();
    vector<const MislibPt*> ipin_array;
    vector<ShString> ipin_name_list;
    hash_map<ShString, ymuint> ipin_name_map;
    if ( ipin_list->type() == MislibPt::kList ) {
      // 通常の入力ピン定義がある場合
      // ipin_list の順に入力ピンを作る．
      for (const MislibPt* pin = ipin_list->top(); pin; pin = pin->next()) {
	assert_cond( pin->type() == MislibPt::kPin, __FILE__, __LINE__);
	ShString name = pin->name()->str();
	if ( ipin_name_map.count(name) > 0 ) {
	  // 同じ名前のピンが既にある．
	  cerr << name << " is appeared more than once" << endl;
	  mError = true;
	}
	else {
	  ipin_name_map.insert(make_pair(name, ipin_array.size()));
	  ipin_array.push_back(pin);
	  ipin_name_list.push_back(name);
	}
      }
      // 念のため，論理式に現れるピン名と比較してみる．
      vector<ShString> ipin_name_list2;
      hash_map<ShString, ymuint> ipin_name_map2;
      dfs(opin_expr, ipin_name_list2, ipin_name_map2);
      for (ymuint i = 0; i < ipin_array.size(); ++ i) {
	ShString name = ipin_array[i]->name()->str();
	if ( ipin_name_map2.count(name) == 0 ) {
	  // 入力ピン記述には現れているが，論理式には現れない
	  // これは警告
	  cerr << name << " is not appeared in the logic expression" << endl;
	}
      }
      for (ymuint i = 0; i < ipin_name_list2.size(); ++ i) {
	ShString name = ipin_name_list2[i];
	if ( ipin_name_map.count(name) == 0 ) {
	  // 論理式に現れる名前の入力ピンが存在しない．
	  // これはエラー
	  cerr << name << " is appeared in the logic expression,"
	       << " but has no pin definition" << endl;
	  mError = true;
	}
      }
    }
    else if ( ipin_list->type() == MislibPt::kPin ) {
      // ワイルドカードの場合
      // 論理式に現れる順に入力ピンを作る．
      dfs(opin_expr, ipin_name_list, ipin_name_map);
      for (ymuint i = 0; i < ipin_name_list.size(); ++ i) {
	ipin_array.push_back(ipin_list);
      }
    }
    else {
      assert_not_reached(__FILE__, __LINE__);
    }
    LogExpr function = pt_to_expr(opin_expr, ipin_name_map);
    ymuint ni = ipin_name_list.size();
    CiCell* cell = library->new_cell(cell_id, name, area, ni +  1, 0, 0);
    for (ymuint i = 0; i < ni; ++ i) {
      // 入力ピンの設定
      ShString name = ipin_name_list[i];
      const MislibPt* pin = ipin_array[i];
      CellCapacitance load(pin->input_load()->num());
      library->new_cell_input(cell, i, name, load, load, load);
    }
    // 出力ピンの設定
    CiOutputPin* opin = library->new_cell_output(cell, ni, opin_name,
						 CellCapacitance::infty(),
						 CellCapacitance(0.0),
						 CellCapacitance::infty(),
						 CellCapacitance(0.0),
						 CellTime::infty(),
						 CellTime(0.0));
    library->set_opin_function(opin, function);
#if 0
    TvFunc tv_function = expr_to_tvfunc(function, ni);
#endif
    for (ymuint i = 0; i < ni; ++ i) {
      // タイミング情報の設定
      const MislibPt* pt_pin = ipin_array[i];
#if 0
      int unate = tv_function.check_unateness(i);
#else
      tCellTimingSense sense;
      switch ( pt_pin->phase()->type() ) {
      case MislibPt::kNoninv:  sense = kSensePosiUnate; break;
      case MislibPt::kInv:     sense = kSenseNegaUnate; break;
      case MislibPt::kUnknown: sense = kSenseNonUnate; break;
      default: assert_not_reached(__FILE__, __LINE__); break;
      }
#endif
      CellTime r_i(pt_pin->rise_block_delay()->num());
      CellResistance r_r(pt_pin->rise_fanout_delay()->num());
      CellTime f_i(pt_pin->fall_block_delay()->num());
      CellResistance f_r(pt_pin->fall_fanout_delay()->num());
      CellTiming* timing = library->new_timing(i, kTimingCombinational,
					       r_i, f_i,
					       CellTime(0.0), CellTime(0.0),
					       r_r, f_r);
      library->set_opin_timing(opin, i, sense, timing);
    }
  }

  return library;
}

// 文字列ノードを生成する．
MislibPt*
MislibParser::new_str(const FileRegion& loc,
		      ShString str)
{
  void* p = mAlloc.get_memory(sizeof(MislibPtStr));
  return new (p) MislibPtStr(loc, str);
}

// 数値ノードを生成する．
MislibPt*
MislibParser::new_num(const FileRegion& loc,
		      double num)
{
  void* p = mAlloc.get_memory(sizeof(MislibPtNum));
  return new (p) MislibPtNum(loc, num);
}

// NONINV ノードを生成する．
MislibPt*
MislibParser::new_noninv(const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(MislibPtNoninv));
  return new (p) MislibPtNoninv(loc);
}

// INV ノードを生成する．
MislibPt*
MislibParser::new_inv(const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(MislibPtInv));
  return new (p) MislibPtInv(loc);
}

// UNKNOWN ノードを生成する．
MislibPt*
MislibParser::new_unknown(const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(MislibPtUnknown));
  return new (p) MislibPtUnknown(loc);
}

// 定数0ノードを生成する．
MislibPt*
MislibParser::new_const0(const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(MislibPtConst0));
  return new (p) MislibPtConst0(loc);
}

// 定数1ノードを生成する．
MislibPt*
MislibParser::new_const1(const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(MislibPtConst1));
  return new (p) MislibPtConst1(loc);
}

// NOT ノードを生成する．
MislibPt*
MislibParser::new_not(const FileRegion& loc,
		      MislibPt* child1)
{
  void* p = mAlloc.get_memory(sizeof(MislibPtNot));
  return new (p) MislibPtNot(loc, child1);
}

// AND ノードを生成する．
MislibPt*
MislibParser::new_and(const FileRegion& loc,
		      MislibPt* child1,
		      MislibPt* child2)
{
  void* p = mAlloc.get_memory(sizeof(MislibPtAnd));
  return new (p) MislibPtAnd(loc, child1, child2);
}

// OR ノードを生成する．
MislibPt*
MislibParser::new_or(const FileRegion& loc,
		     MislibPt* child1,
		     MislibPt* child2)
{
  void* p = mAlloc.get_memory(sizeof(MislibPtOr));
  return new (p) MislibPtOr(loc, child1, child2);
}

// PIN ノードを生成する．
MislibPt*
MislibParser::new_pin(const FileRegion& loc,
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

// リストノードを生成する．
MislibPt*
MislibParser::new_list()
{
  void* p = mAlloc.get_memory(sizeof(MislibPtList));
  return new (p) MislibPtList();
}

// GATE ノードを生成する．
void
MislibParser::new_gate1(const FileRegion& loc,
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
  assert_cond(pt_ipin_list->type() == MislibPt::kList, __FILE__, __LINE__);

  void* p = mAlloc.get_memory(sizeof(MislibPtGate));
  MislibPt* gate = new (p) MislibPtGate(loc, pt_name, pt_area,
					pt_oname, pt_expr, pt_ipin_list);

  mGateList->push_back(gate);
}

// GATE ノードを生成する．
void
MislibParser::new_gate2(const FileRegion& loc,
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

  void* p = mAlloc.get_memory(sizeof(MislibPtGate));
  MislibPt* gate = new (p) MislibPtGate(loc, pt_name, pt_area,
					pt_oname, pt_expr, pt_ipin);
  mGateList->push_back(gate);
}

// GATE ノードを生成する．
void
MislibParser::new_gate3(const FileRegion& loc,
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

  void* p = mAlloc.get_memory(sizeof(MislibPtGate));
  MislibPt* gate = new (p) MislibPtGate(loc, pt_name, pt_area,
					pt_oname, pt_expr, new_list());
  mGateList->push_back(gate);
}

// 今までに生成したすべてのオブジェクトを解放する．
void
MislibParser::clear()
{
  mAlloc.destroy();
  mGateList = NULL;
}

// @brief 字句解析を行う．
// @param[out] lval トークンの値を格納する変数
// @param[out] lloc トークンの位置を格納する変数
// @return トークンの型を返す．
int
MislibParser::scan(MislibPt*& lval,
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

// @brief エラーメッセージを出力する．
// @note 副作用で mError が true にセットされる．
void
MislibParser::error(const FileRegion& loc,
		    const char* msg)
{
  string buff;
  const char* msg2;
  // 好みの問題だけど "parse error" よりは "syntax error" の方が好き．
  if ( !strncmp(msg, "parse error", 11) ) {
    buff ="syntax error";
    buff += (msg + 11);
    msg2 = buff.c_str();
  }
  else {
    msg2 = msg;
  }

  msg_mgr().put_msg(__FILE__, __LINE__, loc,
		    kMsgError, "MISLIB_PARSER", msg2);
  mError = true;
}

END_NAMESPACE_YM_CELL
