
/// @file libym_techmap/patgen/pg_dump.cc
/// @brief pg_dump() の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "PgFuncMgr.h"
#include "PgFuncRep.h"
#include "PgFunc.h"
#include "PatGen.h"
#include "PgNode.h"
#include "PgHandle.h"
#include "ym_techmap/PatMgr.h"
#include "ym_cell/CellLibrary.h"
#include "ym_cell/Cell.h"


BEGIN_NAMESPACE_YM_TECHMAP_PATGEN

BEGIN_NONAMESPACE

void
display_edge(ostream& s,
	     PgNode* node,
	     ymuint fanin_pos)
{
  if ( node->fanin_inv(fanin_pos) ) {
    s << "~";
  }
  s << "Node#" << node->fanin(fanin_pos)->id();
}

END_NONAMESPACE

// @brief グラフ構造全体をダンプする．
// @param[in] s 出力先のストリーム
// @note ダンプされた情報はそのまま PatGraph で読み込むことができる．
void
pg_display(ostream& s,
	   const PgFuncMgr& pgf_mgr)
{
  s << "*** FUNCTION SECTION ***" << endl;
  for (ymuint i = 0; i < pgf_mgr.func_num(); ++ i) {
    const PgFunc* func = pgf_mgr.func(i);
    assert_cond( func->id() == i, __FILE__, __LINE__);
    s << "FUNC#" << i << ": REP#" << func->rep()->id()
      << ": " << func->map() << endl;
    const vector<const Cell*>& cell_list = func->cell_list();
    for (vector<const Cell*>::const_iterator p = cell_list.begin();
	 p != cell_list.end(); ++ p) {
      const Cell* cell = *p;
      s << "  " << cell->name() << endl;
    }
  }
  s << endl;

  s << "*** REPRESENTATIVE SECTION ***" << endl;
  for (ymuint i = 0; i < pgf_mgr.rep_num(); ++ i) {
    const PgFuncRep* rep = pgf_mgr.rep(i);
    assert_cond( rep->id() == i , __FILE__, __LINE__);
    s << "REP#" << i << ": ";
    rep->rep_func().dump(s, 2);
    s << endl;
    s << "  equivalence = ";
    for (ymuint j = 0; j < rep->func_num(); ++ j) {
      s << " FUNC#" << rep->func(j)->id();
    }
    s << endl;
    s << "  patterns = ";
    for (ymuint j = 0; j < rep->pat_num(); ++ j) {
      s << " PAT#" << rep->pat_id(j);
    }
    s << endl;
  }
  s << endl;

  const PatGen& pat_gen = pgf_mgr.pat_gen();

  s << "*** NODE SECTION ***" << endl;
  ymuint n = pat_gen.node_num();
  for (ymuint i = 0; i < n; ++ i) {
    PgNode* node = pat_gen.node(i);
    if ( node->is_locked() ) {
      s << "*";
    }
    else {
      s << " ";
    }
    s << "Node#" << node->id() << ": ";
    if ( node->is_input() ) {
      s << "Input#" << node->input_id();
    }
    else {
      if ( node->is_and() ) {
	s << "And";
      }
      else if ( node->is_xor() ) {
	s << "Xor";
      }
      else {
	assert_not_reached(__FILE__, __LINE__);
      }
      s << "( ";
      display_edge(s, node, 0);
      s << ", ";
      display_edge(s, node, 1);
      s << ")";
    }
    s << endl;
  }
  s << endl;

  s << "*** PATTERN SECTION ***" << endl;
  ymuint np = pat_gen.pat_num();
  for (ymuint i = 0; i < np; ++ i) {
    PgHandle root = pat_gen.pat_root(i);
    s << "Pat#" << i << ": ";
    if ( root.inv() ) {
      s << "~";
    }
    s << "Node#" << root.node()->id() << endl;
  }
  s << "*** END ***" << endl;
}


BEGIN_NONAMESPACE

void
dump_word(ostream& s,
	  ymuint val)
{
  static char buf[4];
  buf[0] = val & 255U; val >>= 8;
  buf[1] = val & 255U; val >>= 8;
  buf[2] = val & 255U; val >>= 8;
  buf[3] = val & 255U;

  s.write(buf, 4);
}

void
dump_func(ostream& s,
	  const TvFunc& f)
{
  ymuint ni = f.ni();
  dump_word(s, ni);
  ymuint nip = (1U << ni);
  ymuint32 v = 0U;
  ymuint base = 0;
  for (ymuint p = 0; p < nip; ++ p) {
    v |= (f.value(p) << (p - base));
    if ( (p % 32) == 31 ) {
      dump_word(s, v);
      base += 32;
      v = 0U;
    }
  }
  if ( ni <= 4 ) {
    dump_word(s, v);
  }
}

void
dump_map(ostream& s,
	 const NpnMap& map)
{
  ymuint ni = map.ni();
  ymuint32 v = (ni << 1);
  if ( map.opol() == kPolNega ) {
    v |= 1U;
  }
  dump_word(s, v);
  for (ymuint i = 0; i < ni; ++ i) {
    tNpnImap imap = map.imap(i);
    // 手抜きでは imap を ymuint32 にキャストすればよい．
    ymuint j = npnimap_pos(imap);
    ymuint32 v = (j << 1);
    if ( npnimap_pol(imap) ) {
      v |= 1U;
    }
    dump_word(s, v);
  }
}

void
dump_edge(ostream& s,
	  PgNode* node,
	  ymuint fanin_pos)
{
  ymuint v = 0U;
  if ( !node->is_input() ) {
    v = node->fanin(fanin_pos)->id() * 2;
    if ( node->fanin_inv(fanin_pos) ) {
      v |= 1U;
    }
  }
  dump_word(s, v);
}

void
dump_dfs(ostream& s,
	 PgNode* node,
	 vector<bool>& vmark,
	 vector<ymuint>& val_list,
	 ymuint& max_input)
{
  if ( node->is_input() ) {
    if ( max_input < node->input_id() ) {
      max_input = node->input_id();
    }
    return;
  }
  if ( vmark[node->id()] ) {
    return;
  }
  vmark[node->id()] = true;
  val_list.push_back(node->id() * 2);
  dump_dfs(s, node->fanin(0), vmark, val_list, max_input);
  val_list.push_back(node->id() * 2 + 1);
  dump_dfs(s, node->fanin(1), vmark, val_list, max_input);
}

END_NONAMESPACE

// @brief グラフ構造全体をダンプする．
// @param[in] s 出力先のストリーム
// @note ダンプされた情報はそのまま PatGraph で読み込むことができる．
void
pg_dump(ostream& s,
	const PgFuncMgr& pgf_mgr)
{
  nsYm::nsCell::dump_library(s, pgf_mgr.library());

  ymuint nf = pgf_mgr.func_num();
  dump_word(s, nf);
  for (ymuint i = 0; i < nf; ++ i) {
    const PgFunc* func = pgf_mgr.func(i);
    assert_cond( func->id() == i, __FILE__, __LINE__);
    dump_map(s, func->map());
    const vector<const Cell*>& cell_list = func->cell_list();
    ymuint nc = cell_list.size();
    dump_word(s, nc);
    for (ymuint i = 0; i < nc; ++ i) {
      const Cell* cell = cell_list[i];
      dump_word(s, cell->id());
    }
  }

  ymuint nr = pgf_mgr.rep_num();
  dump_word(s, nr);
  for (ymuint i = 0; i < nr; ++ i) {
    const PgFuncRep* rep = pgf_mgr.rep(i);
    assert_cond( rep->id() == i , __FILE__, __LINE__);
    ymuint ne = rep->func_num();
    dump_word(s, ne);
    for (ymuint j = 0; j < ne; ++ j) {
      dump_word(s, rep->func(j)->id());
    }
    ymuint np = rep->pat_num();
    dump_word(s, np);
    for (ymuint j = 0; j < np; ++ j) {
      dump_word(s, rep->pat_id(j));
    }
  }

  const PatGen& pat_gen = pgf_mgr.pat_gen();

  ymuint nn = pat_gen.node_num();
  dump_word(s, nn);
  for (ymuint i = 0; i < nn; ++ i) {
    PgNode* node = pat_gen.node(i);
    ymuint v = 0U;
    if ( node->is_input() ) {
      v = PatMgr::kInput | (node->input_id() << 2);
    }
    else if ( node->is_and() ) {
      v = PatMgr::kAnd;
    }
    else if ( node->is_xor() ) {
      v = PatMgr::kXor;
    }
    dump_word(s, v);
    dump_edge(s, node, 0);
    dump_edge(s, node, 1);
  }

  vector<bool> vmark(nn);
  vector<ymuint> val_list;
  val_list.reserve(nn * 2);

  ymuint np = pat_gen.pat_num();
  dump_word(s, np);
  for (ymuint i = 0; i < np; ++ i) {
    PgHandle root = pat_gen.pat_root(i);
    vmark.clear();
    vmark.resize(nn, false);
    val_list.clear();
    ymuint max_input = 0;
    dump_dfs(s, root.node(), vmark, val_list, max_input);
    ymuint v = (max_input + 1) << 1;
    if ( root.inv() ) {
      v |= 1U;
    }
    dump_word(s, v);
    ymuint ne = val_list.size();
    dump_word(s, ne);
    for (ymuint i = 0; i < ne; ++ i) {
      dump_word(s, val_list[i]);
    }
  }
}

END_NAMESPACE_YM_TECHMAP_PATGEN
