
/// @file libym_techmap/PatMgr.cc
/// @brief PatMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_techmap/PatMgr.h"
#include "PatGraph.h"
#include "RepFunc.h"
#include "FuncGroup.h"
#include "ym_cell/CellLibrary.h"
#include "ym_cell/Cell.h"


BEGIN_NAMESPACE_YM_TECHMAP

BEGIN_NONAMESPACE

ymuint
read_word(istream& s)
{
  static ymuint8 buf[4];
  s.read(reinterpret_cast<char*>(buf), 4);
  ymuint ans =
    static_cast<ymuint>(buf[0]) |
    (static_cast<ymuint>(buf[1]) << 8) |
    (static_cast<ymuint>(buf[2]) << 16) |
    (static_cast<ymuint>(buf[3]) << 24);
  return ans;
}

void
read_map(istream& s,
	 NpnMap& map)
{
  ymuint32 tmp = read_word(s);
  ymuint ni = (tmp >> 1);
  map.resize(ni);
  tPol opol = (tmp & 1U) ? kPolNega : kPolPosi;
  map.set_opol(opol);
  for (ymuint i = 0; i < ni; ++ i) {
    ymuint32 tmp = read_word(s);
    ymuint pos = (tmp >> 1);
    tPol ipol = (tmp & 1U) ? kPolNega : kPolPosi;
    map.set(i, pos, ipol);
  }
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス FuncGroup
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
FuncGroup::FuncGroup() :
  mCellNum(0),
  mCellList(NULL)
{
}

// @brief デストラクタ
FuncGroup::~FuncGroup()
{
}


//////////////////////////////////////////////////////////////////////
// クラス RepFunc
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
RepFunc::RepFunc() :
  mFuncNum(0U),
  mFuncArray(NULL),
  mPatNum(0U),
  mPatArray(NULL)
{
}

// @brief デストラクタ
RepFunc::~RepFunc()
{
  delete [] mFuncArray;
  delete [] mPatArray;
}

// @brief バイナリ形式のファイルを読み込む．
// @param[in] s 入力ストリーム
// @return 読み込みが成功したら true を返す．
bool
RepFunc::load(istream& s)
{
  mFuncNum = read_word(s);
  if ( mFuncNum > 0 ) {
    mFuncArray = new ymuint32[mFuncNum];
    for (ymuint i = 0; i < mFuncNum; ++ i) {
      mFuncArray[i] = read_word(s);
    }
  }
  else {
    mFuncArray = NULL;
  }

  mPatNum = read_word(s);
  if ( mPatNum > 0 ) {
    mPatArray = new ymuint32[mPatNum];
    for (ymuint i = 0; i < mPatNum; ++ i) {
      mPatArray[i] = read_word(s);
    }
  }
  else {
    mPatArray = NULL;
  }

  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス PatGraph
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PatGraph::PatGraph() :
  mInputNum(0U),
  mEdgeNum(0U),
  mEdgeList(NULL)
{
}

// @brief デストラクタ
PatGraph::~PatGraph()
{
  delete [] mEdgeList;
}

// @brief データを読み込んでセットする．
// @param[in] s 入力元のストリーム
// @retval true 読み込みが成功した．
// @retval false 読み込みが失敗した．
bool
PatGraph::load(istream& s)
{
  mInputNum = read_word(s);
  mEdgeNum = read_word(s);
  if ( mEdgeNum > 0 ) {
    mEdgeList = new ymuint32[mEdgeNum];
    for (ymuint i = 0; i < mEdgeNum; ++ i) {
      mEdgeList[i] = read_word(s);
    }
  }
  else {
    mEdgeList = NULL;
  }

  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス PatMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PatMgr::PatMgr() :
  mLibrary(NULL),
  mNodeNum(0U),
  mNodeTypeArray(NULL),
  mEdgeArray(NULL),
  mPatNum(0),
  mPatArray(NULL)
{
}

// @brief デストラクタ
PatMgr::~PatMgr()
{
  init();
}

// @brief 内容を初期化する．
// @note 以前確保されたメモリは開放される．
void
PatMgr::init()
{
  delete mLibrary;
  delete [] mNodeTypeArray;
  delete [] mEdgeArray;
  delete [] mPatArray;
  mNodeNum = 0U;
  mPatNum = 0U;
}

// @brief データを読み込んでセットする．
// @param[in] s 入力元のストリーム
// @retval true 読み込みが成功した．
// @retval false 読み込みが失敗した．
bool
PatMgr::load(istream& s)
{
  // 以前の内容を捨てる．
  init();

  // ライブラリを読み込む．
  mLibrary = nsYm::nsCell::restore_library(s);

  // 関数の情報を読み込む．
  mFuncNum = read_word(s);
  mFuncArray = new FuncGroup[mFuncNum];
  for (ymuint i = 0; i < mFuncNum; ++ i) {
    FuncGroup& func = mFuncArray[i];
    read_map(s, func.mNpnMap);
    ymuint n = read_word(s);
    func.mCellNum = n;
    if ( n > 0 ) {
      func.mCellList = new const Cell*[n];
      for (ymuint j = 0; j < n; ++ j) {
	ymuint id = read_word(s);
	func.mCellList[j] = mLibrary->cell(id);
      }
    }
    else {
      func.mCellList = NULL;
    }
  }

  // 代表関数の情報を読み込む．
  mRepNum = read_word(s);
  mRepArray = new RepFunc[mRepNum];
  for (ymuint i = 0; i < mRepNum; ++ i) {
    if ( !mRepArray[i].load(s) ) {
      return false;
    }
  }

  // ノードと枝の情報を読み込む．
  mNodeNum = read_word(s);
  mNodeTypeArray = new ymuint32[mNodeNum];
  mEdgeArray = new ymuint32[mNodeNum * 2];
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    mNodeTypeArray[i] = read_word(s);
    mEdgeArray[i * 2] = read_word(s);
    mEdgeArray[i * 2 + 1] = read_word(s);
    if ( node_type(i) == kInput ) {
      assert_cond( input_id(i) == i, __FILE__, __LINE__);
    }
  }

  // パタングラフの情報を読み込む．
  mPatNum = read_word(s);
  mPatArray = new PatGraph[mPatNum];
  for (ymuint i = 0; i < mPatNum; ++ i) {
    if ( !mPatArray[i].load(s) ) {
      return false;
    }
  }

  // 代表関数とパタングラフの対応関係をとる．
  for (ymuint i = 0; i < mRepNum; ++ i) {
    RepFunc& rep = mRepArray[i];
    for (ymuint j = 0; j < rep.mPatNum; ++ j) {
      PatGraph& pat = mPatArray[rep.mPatArray[j]];
      pat.mRepId = i;
    }
  }

  return true;
}

// @brief このセルライブラリに含まれるセルの最大の入力数を得る．
ymuint
PatMgr::max_input() const
{
  ymuint ans = 0;
  for (ymuint i = 0; i < rep_num(); ++ i) {
    const RepFunc& rep = this->rep(i);
    if ( rep.pat_num() == 0 ) {
      // これは定数かインバーターだから無視しても大丈夫
      continue;
    }
    const PatGraph& pat = this->pat(rep.pat_id(0));
    ymuint ni = pat.input_num();
    if ( ans < ni ) {
      ans = ni;
    }
  }
  return ans;
}

// @brief 関数グループを返す．
// @param[in] id 関数番号　( 0 <= id < func_num() )
const FuncGroup&
PatMgr::func_group(ymuint id) const
{
  return mFuncArray[id];
}

// @brief 代表関数を返す．
// @param[in] id 代表関数番号
const RepFunc&
PatMgr::rep(ymuint id) const
{
  return mRepArray[id];
}

// @brief パタンを返す．
// @param[in] id パタン番号 ( 0 <= id < pat_num() )
const PatGraph&
PatMgr::pat(ymuint id) const
{
  return mPatArray[id];
}

// @relates PatMgr
// @brief PatMgr の内容を出力する．
// @param[in] s 出力先のストリーム
// @param[in] pat_mgr パタングラフ
void
dump(ostream& s,
     const PatMgr& pat_mgr)
{
  s << "==== PatMgr dump start ====" << endl;

  // 関数情報の出力
  const FuncGroup& func0 = pat_mgr.const0_func();
  const FuncGroup& func1 = pat_mgr.const1_func();
  const FuncGroup& func2 = pat_mgr.buf_func();
  const FuncGroup& func3 = pat_mgr.inv_func();
  ymuint nf = pat_mgr.func_num();
  for (ymuint i = 0; i < nf; ++ i) {
    const FuncGroup& func = pat_mgr.func_group(i);
    if ( &func == &func0 ) {
      s << "Const0" << endl;
    }
    else if ( &func == &func1 ) {
      s << "Const1" << endl;
    }
    else if ( &func == &func2 ) {
      s << "Buffer" << endl;
    }
    else if ( &func == &func3 ) {
      s << "Inverter" << endl;
    }
    else {
      const NpnMap& map = func.npn_map();
      s << "Func#" << i << ": " << map << endl;
    }
    ymuint nc = func.cell_num();
    for (ymuint j = 0; j < nc; ++ j) {
      const Cell* cell = func.cell(j);
      s << "  " << cell->name() << endl;
    }
  }
  s << endl;

  // 代表関数情報の出力
  ymuint nr = pat_mgr.rep_num();
  for (ymuint i = 0; i < nr; ++ i) {
    const RepFunc& rep = pat_mgr.rep(i);
    s << "Rep#" << i << endl;
    s << "  equivalence = ";
    ymuint nf = rep.func_num();
    for (ymuint j = 0; j < nf; ++ j) {
      s << " Func#" << rep.func_id(j);
    }
    s << endl;
    ymuint np = rep.pat_num();
    s << "  patterns = ";
    for (ymuint j = 0; j < np; ++ j) {
      s << " Pat#" << rep.pat_id(j);
    }
    s << endl;
  }

  // ノードの種類の出力
  ymuint nn = pat_mgr.node_num();
  for (ymuint i = 0; i < nn; ++ i) {
    s << "Node#" << i << ": ";
    switch ( pat_mgr.node_type(i) ) {
    case PatMgr::kInput: s << "INPUT#" << pat_mgr.input_id(i) ; break;
    case PatMgr::kAnd:   s << "AND"; break;
    case PatMgr::kXor:   s << "XOR"; break;
    default: assert_not_reached(__FILE__, __LINE__);
    }
    s << endl;
  }
  s << endl;

  // 枝の情報の出力
  ymuint ne = pat_mgr.edge_num();
  for (ymuint i = 0; i < ne; ++ i) {
    s << "Edge#" << i << ": " << pat_mgr.edge_from(i)
      << " -> " << pat_mgr.edge_to(i) << "(" << pat_mgr.edge_pos(i) << ")";
    if ( pat_mgr.edge_inv(i) ) {
      s << " ***";
    }
    s << endl;
  }
  s << endl;

  // パタングラフの情報の出力
  ymuint np = pat_mgr.pat_num();
  for (ymuint i = 0; i < np; ++ i) {
    const PatGraph& pat = pat_mgr.pat(i);
    s << "Pat#" << i << ": "
      << "Rep#" << pat.rep_id() << ": ";
    if ( pat.root_inv() ) {
      s << "[inv]";
    }
    s << "(" << pat.input_num() << "), ";
    ymuint n = pat.edge_num();
    for (ymuint i = 0; i < n; ++ i) {
      s << " " << pat.edge(i);
    }
    s << endl;
  }

  s << "==== PatMgr dump end ====" << endl;
}

END_NAMESPACE_YM_TECHMAP
