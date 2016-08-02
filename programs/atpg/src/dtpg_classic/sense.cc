
/// @file src/dtpg_classic/sense.cc
/// @brief unique sensitization を行う関数の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: sense.cc 2128 2009-03-12 07:25:33Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "Network.h"
#include "Gate.h"
#include "sense.h"
#include "SearchMgr.h"
#include "PGraph.h"


BEGIN_NAMESPACE_YM_ATPG_DTPGCL

int us_mode = 1;

static
vector<Gate*> dfs;

static
ymuint max_p;

static
ymuint max_s;

static
ymuint po_sideval;

static
ymuint max_level;

static
vector<Gate*> blocks;

static
void
mark_ppath(Gate* gate);

static
ymuint
mark_spath(Gate* gate);

static
void
set_block(Gate* gate,
	  Val3 val);

static
void
rst_block();

static
bool
chk_another_path(Gate* gate);


static
bool
get_df1(Gate* gate)
{
  bool ans = false;
  if ( gate->chk_pg() && !gate->chk_mark() ) {
    gate->set_mark();
    if ( gate->gval() == kValX || gate->fval() == kValX ) {
      dfs.push_back(gate);
    }
    else if ( gate->is_po() ) {
      ans = true;
    }
    else {
      ymuint no = gate->act_nfo();
      for (ymuint i = 0; i < no; ++ i) {
	Gate* o_gate = gate->act_fanout(i);
	if ( get_df1(o_gate) ) {
	  ans = true;
	}
      }
    }
  }
  return ans;
}

bool
sensitize(Gate* f_node)
{
  if ( !pgraph.check_changes() ) {
    return false;
  }
  pgraph.clear_changes();

  max_level = gn.max_level() + 1;
  dfs.clear();
  gn.clear_mark();
  ymuint f_no = f_node->act_nfo();
  bool reached = false;
  for (ymuint i = 0; i < f_no; ++ i) {
    Gate* o_gate = f_node->act_fanout(i);
    if ( o_gate->is_f_site() ) {
      if ( get_df1(o_gate) ) {
	reached = true;
      }
    }
  }
  if ( dfs.empty() || reached ) {
    return false;
  }
  
  // 'Primary Path' をマークする．
  gn.clear_mark();
  ymuint max_side_val = dfs[0]->level();
  mark_ppath(dfs[0]);
  for (ymuint i = 1; i < dfs.size(); ++ i) {
    ymuint side_val = mark_spath(dfs[i]);
    if ( max_side_val < side_val ) {
      max_side_val = side_val;
    }
  }
  
  bool assigned = false;
  Gate* gate = dfs[0];
  for ( ; ; ) {
    ymuint ni = gate->nfi();
    ymuint no = gate->act_nfo();
    if ( gate->level() >= max_side_val ) {
      // gate は single dominator
      if ( gate->gval() == kValX ) {
	if ( gate->fval() == kVal0 ) {
	  sm.eventq_add(gate, kVal1, kValX);
	  assigned = true;
	}
	else if ( gate->fval() == kVal1 ) {
	  sm.eventq_add(gate, kVal0, kValX);
	  assigned = true;
	} // gate->fval() == kValX
	else {
	  Val3 nc_val = gate->nc_val();
	  if ( nc_val != kValX ) {
	    for (ymuint i = 0; i < ni; ++ i) {
	      Gate* i_gate = gate->fanin(i);
	      if ( i_gate->gval() == kValX ) {
		if ( !i_gate->chk_fcone() ) {
		  sm.eventq_add(i_gate, nc_val, kValX);
		  assigned = true;
		}
		else if ( !i_gate->chk_dif() ) {
		  sm.eventq_add(i_gate, nc_val, nc_val);
		  assigned = true;
		}
	      }
	    }
	  }
	}
      }
      else if ( gate->fval() == kValX ) {
	if ( gate->gval() == kVal0 ) {
	  sm.eventq_add(gate, kValX, kVal1);
	  assigned = true;
	}
	else if ( gate->gval() == kVal1 ) {
	  sm.eventq_add(gate, kValX, kVal0);
	  assigned = true;
	}
      }
    }
    else if ( us_mode > 1 ) {
      // path controller を探す
      if ( ni >= 2 && gate->c_val() != kValX ) {
	for (ymuint i = 0; i < ni; ++ i) {
	  Gate* i_gate = gate->fanin(i);
	  if ( i_gate->chk_dif() ||
	       i_gate->gval() != kValX ) {
	    continue;
	  }
	  Gate* root = i_gate;
	  Val3 root_ncval = gate->nc_val();
	  Val3 root_cval = gate->c_val();
#ifndef NAIVE_SENSE
	  while ( root->nfi() == 1 ) {
	    if ( root->gate_type() == kTgNot ) {
	      root_cval = ~root_cval;
	      root_ncval = ~root_ncval;
	    }
	    root = root->fanin(0);
	  }
#endif
	  max_p = 0;
	  max_s = 0;
	  set_block(root, root_cval);
	  if ( max_s < max_p ) {
	    max_s = max_p;
	  }
	  if ( blocks.size() > 1) {
	    gn.clear_mark();
	    bool found = false;
	    for (ymuint j = 0; j < dfs.size(); ++ j) {
	      if ( chk_another_path(dfs[j]) ) {
		found = true;
		break;
	      }
	    }
	    if ( !found ) {
	      if ( root->chk_fcone() ) {
		sm.eventq_add(root, root_ncval, root_ncval);
		assigned = true;
	      }
	      else {
		sm.eventq_add(root, root_ncval, kValX);
		assigned = true;
	      }
	    }
	  }
	  rst_block();
	}
      }
    }
    if ( gate->is_po() ) {
      break;
    }
    if ( max_side_val < gate->side_val ) {
      max_side_val = gate->side_val;
    }

    // 次の primary path node を探す．
    for (ymuint i = 0; i < no; ++ i) {
      Gate* o_gate = gate->act_fanout(i);
      if ( o_gate->chk_ppath() ) {
	gate = o_gate;
	break;
      }
    }
  }
  return assigned;
}

// primary path にマークする．
static
void
mark_ppath(Gate* gate)
{
  gate->set_mark();
  gate->set_ppath();
  ymuint max_side_val = gate->level();
  if ( gate->is_po() ) {
    po_sideval = max_side_val;
    return;
  }
  
  Gate* best_fo = NULL;
  ymuint best_lvl = max_level + 1;
  ymuint no = gate->act_nfo();
  for (ymuint i = 0; i < no; ++ i) {
    Gate* gate1 = gate->act_fanout(i);
    if ( gate1->chk_pg() ) {
      if ( gate1->min_level() < best_lvl ) {
	best_lvl = gate1->min_level();
	best_fo = gate1;
      }
    }
    else {
      gate1->rst_ppath();
    }
  }
  if ( best_fo == NULL ) {
    fputs("sensitize: Internal error!\n", stderr);
    exit(1);
  }
  
  mark_ppath(best_fo);
  for (ymuint i = 0; i < no; ++ i) {
    Gate* gate1 = gate->act_fanout(i);
    if ( gate1->chk_pg() ) {
      if ( gate1 == best_fo ) continue;
      ymuint side_val1 = mark_spath(gate1);
      if ( max_side_val < side_val1 ) {
	max_side_val = side_val1;
      }
    }
    else {
      gate1->rst_ppath();
    }
  }
  gate->side_val = max_side_val;
}

// secondary path にマークする．
static
ymuint
mark_spath(Gate* gate)
{
  if ( gate->chk_mark() ) {
    if ( gate->chk_ppath() ) {
      return gate->level();
    }
    return gate->side_val;
  }
  gate->set_mark();
  gate->rst_ppath();
  if ( gate->is_po() 
#ifndef NAIVE_SENSE
      || gate->level() >= po_sideval
#endif
      ) {
    return gate->side_val = max_level;
  }
  
  ymuint max_side_val = 0;
  ymuint no = gate->act_nfo();
  for (ymuint i = 0; i < no; ++ i) {
    Gate* gate1 = gate->act_fanout(i);
    if ( gate1->chk_pg() ) {
      ymuint side_val1 = mark_spath(gate1);
      if ( max_side_val < side_val1 ) {
	max_side_val = side_val1;
      }
    }
  }
  return gate->side_val = max_side_val;
}

static
void
set_block(Gate* gate,
	  Val3 cval)
{
  blocks.clear();
  for (ymuint i = 0; i < gate->act_nfo(); ++ i) {
    Gate* o_gate = gate->act_fanout(i);
    if ( !o_gate->chk_pg() ) {
#ifndef NAIVE_SENSE
      if ( o_gate->nfi() != 1 ) {
	if ( o_gate->c_val() == cval ) {
	  set_block(o_gate, o_gate->o_val());
	}
      }
      else {
	if ( o_gate->gate_type() == kTgNot ) {
	  set_block(o_gate, ~cval);
	}
	else {
	  set_block(o_gate, cval);
	}
      }
#endif
    }
    else {
      if ( o_gate->c_val() != cval ) {
	continue;
      }
      o_gate->set_block();
      blocks.push_back(o_gate);
      if ( o_gate->chk_ppath() ) {
	if ( max_p < o_gate->level() ) {
	  max_p = o_gate->level();
	}
      }
      else {
	if ( max_s < o_gate->level() ) {
	  max_s = o_gate->level();
	}
      }
#ifndef NAIVE_SENSE
      set_block(o_gate, o_gate->o_val());
#endif
    }
  }
}

static
void
rst_block()
{
  for (vector<Gate*>::iterator p = blocks.begin();
       p != blocks.end(); ++ p) {
    (*p)->rst_block();
  }
}

static
bool
chk_another_path(Gate* gate)
{
  if ( gate->chk_block() ) {
    return false;
  }
  
  if ( gate->chk_mark() ) {
    return false;
  }
  gate->set_mark();
  
  if ( gate->is_po() ) {
    return true;
  }
  
  if ( gate->chk_ppath() ) {
    if ( gate->level() > max_p ) {
      return true;
    }
  }
  else {
    if ( gate->level() > max_s ) {
      return true;
    }
  }
  
  ymuint no = gate->act_nfo();
  for (ymuint i = 0; i < no; ++ i) {
    Gate* o_gate = gate->act_fanout(i);
    if ( o_gate->chk_pg() ) {
      if ( chk_another_path(o_gate) ) {
	return true;
      }
    }
  }
  return false;
}

END_NAMESPACE_YM_ATPG_DTPGCL
