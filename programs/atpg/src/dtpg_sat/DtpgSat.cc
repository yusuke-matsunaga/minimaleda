
/// @file atpg/src/dtpg_sat/DtpgSat.cc
/// @brief DtpgSat の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DtpgSat.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgSat.h"
#include "ym_tgnet/TgNetwork.h"
#include "ym_tgnet/TgNode.h"
#include "SaFault.h"
#include "TestVector.h"
#include "ym_sat/SatSolver.h"


BEGIN_NAMESPACE_YM_ATPG_DTPGSAT

// @brief コンストラクタ
DtpgSat::DtpgSat() :
  mMode(0)
{
}

// @brief デストラクタ
DtpgSat::~DtpgSat()
{
}


BEGIN_NONAMESPACE

/// @brief ノードの入出力の関係を表す CNF クローズを生成する．
/// @param[in] solver SAT ソルバ
/// @param[in] type ノードの型
/// @param[in] output 出力リテラル
/// @param[in] inputs 入力リテラル
void
make_cnf(SatSolver& solver,
	 tTgGateType type,
	 Literal output,
	 const vector<Literal>& inputs)
{
  switch ( type ) {
  case kTgUndef:
  case kTgInput:
    assert_not_reached(__FILE__, __LINE__);
    break;

  case kTgOutput:
  case kTgBuff:
    solver.add_clause(inputs[0], ~output);
    solver.add_clause(~inputs[0], output);
    break;

  case kTgNot:
    solver.add_clause(inputs[0], output);
    solver.add_clause(~inputs[0], ~output);
    break;

  case kTgAnd:
    {
      ymuint ni = inputs.size();
      vector<Literal> tmp(ni + 1);
      for (ymuint i = 0; i < ni; ++ i) {
	tmp[i] = ~inputs[i];
      }
      tmp[ni] = output;
      solver.add_clause(tmp);
      for (ymuint i = 0; i < ni; ++ i) {
	solver.add_clause(inputs[i], ~output);
      }
    }
    break;

  case kTgNand:
    {
      ymuint ni = inputs.size();
      vector<Literal> tmp(ni + 1);
      for (ymuint i = 0; i < ni; ++ i) {
	tmp[i] = ~inputs[i];
      }
      tmp[ni] = ~output;
      solver.add_clause(tmp);
      for (ymuint i = 0; i < ni; ++ i) {
	solver.add_clause(inputs[i], output);
      }
    }
    break;

  case kTgOr:
    {
      ymuint ni = inputs.size();
      vector<Literal> tmp(ni + 1);
      for (ymuint i = 0; i < ni; ++ i) {
	tmp[i] = inputs[i];
      }
      tmp[ni] = ~output;
      solver.add_clause(tmp);
      for (ymuint i = 0; i < ni; ++ i) {
	solver.add_clause(~inputs[i], output);
      }
    }
    break;

  case kTgNor:
    {
      ymuint ni = inputs.size();
      vector<Literal> tmp(ni + 1);
      for (ymuint i = 0; i < ni; ++ i) {
	tmp[i] = inputs[i];
      }
      tmp[ni] = output;
      solver.add_clause(tmp);
      for (ymuint i = 0; i < ni; ++ i) {
	solver.add_clause(~inputs[i], ~output);
      }
    }
    break;

  case kTgXor:
    {
      ymuint ni = inputs.size();
      vector<Literal> tmp(ni + 1);
      ymuint nip = (1U << ni);
      for (ymuint p = 0; p < nip; ++ p) {
	ymuint c = 0;
	for (ymuint i = 0; i < ni; ++ i) {
	  if ( p & (1U << i) ) {
	    tmp[i] = inputs[i];
	  }
	  else {
	    tmp[i] = ~inputs[i];
	    ++ c;
	  }
	}
	if ( (c % 2) == 0 ) {
	  tmp[ni] = ~output;
	}
	else {
	  tmp[ni] = output;
	}
	solver.add_clause(tmp);
      }
    }
    break;
    
  case kTgXnor:
    {
      ymuint ni = inputs.size();
      vector<Literal> tmp(ni + 1);
      ymuint nip = (1U << ni);
      for (ymuint p = 0; p < nip; ++ p) {
	ymuint c = 0;
	for (ymuint i = 0; i < ni; ++ i) {
	  if ( p & (1U << i) ) {
	    tmp[i] = inputs[i];
	  }
	  else {
	    tmp[i] = ~inputs[i];
	    ++ c;
	  }
	}
	if ( (c % 2) == 0 ) {
	  tmp[ni] = output;
	}
	else {
	  tmp[ni] = ~output;
	}
	solver.add_clause(tmp);
      }
    }
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
}

/// @brief ノードの入出力の関係を表す CNF クローズを生成する．
/// @param[in] solver SAT ソルバ
/// @param[in] lexp 論理式
/// @param[in] output 出力リテラル
/// @param[in] inputs 入力リテラル
void
make_cnf(SatSolver& solver,
	 const LogExpr& lexp,
	 Literal output,
	 const vector<Literal>& inputs)
{
  if ( lexp.is_constant() || lexp.is_literal() ) {
    assert_not_reached(__FILE__, __LINE__);
    return;
  }
  
  ymuint nc = lexp.child_num();
  vector<Literal> local_inputs(nc);
  for (ymuint i = 0; i < nc; ++ i) {
    LogExpr lexp1 = lexp.child(i);
    if ( lexp1.is_posiliteral() ) {
      local_inputs[i] = inputs[lexp1.varid()];
    }
    else if ( lexp1.is_negaliteral() ) {
      local_inputs[i] = ~inputs[lexp1.varid()];
    }
    else {
      ymuint new_varid = solver.new_var();
      local_inputs[i] = Literal(new_varid, kPolPosi);
      make_cnf(solver, lexp1, local_inputs[i], inputs);
    }
  }
  vector<Literal> tmp(nc + 1);
  if ( lexp.is_and() ) {
    for (ymuint i = 0; i < nc; ++ i) {
      tmp[i] = ~local_inputs[i];
    }
    tmp[nc] = output;
    solver.add_clause(tmp);
    for (ymuint i = 0; i < nc; ++ i) {
      solver.add_clause(local_inputs[i], ~output);
    }
  }
  else if ( lexp.is_or() ) {
    for (ymuint i = 0; i < nc; ++ i) {
      tmp[i] = local_inputs[i];
    }
    tmp[nc] = ~output;
    solver.add_clause(tmp);
    for (ymuint i = 0; i < nc; ++ i) {
      solver.add_clause(~local_inputs[i], output);
    }
  }
  else if ( lexp.is_xor() ) {
    ymuint np = (1U << nc);
    for (ymuint p = 0; p < np; ++ p) {
      ymuint c = 0;
      for (ymuint i = 0; i < nc; ++ i) {
	if ( p & (1U << i) ) {
	  tmp[i] = local_inputs[i];
	}
	else {
	  tmp[i] = ~local_inputs[i];
	  ++ c;
	}
      }
      if ( (c % 2) == 0 ) {
	tmp[nc] = ~output;
      }
      else {
	tmp[nc] = output;
      }
      solver.add_clause(tmp);
    }
  }
}

END_NONAMESPACE


// 故障 f に対するテストパタン生成を行う．
tStat
DtpgSat::run(const TgNetwork& network,
	     SaFault* f,
	     TestVector* tv)
{
  ymuint n = network.node_num();
  ymuint nl = network.logic_num();
  ymuint npo = network.output_num2();
  
  const TgNode* fnode = f->node();
  const TgNode* fsrc = fnode;
  if ( f->is_input_fault() ) {
    ymuint ipos = f->pos();
    fsrc = fnode->fanin(ipos);
  }
  
  mVarMap.clear();
  mVarMap.resize(n);
  for (ymuint i = 0; i < n; ++ i) {
    mVarMap[i].mMark = kNone;
  }
  
  // まず fnode の TFO にマークをつける．
  // 同時にマークの付いたノードは queue につまれる．
  vector<const TgNode*> queue;
  queue.reserve(n);
  mark(fnode) = kTFO;
  queue.push_back(fnode);
  ymuint rpos = 0;
  while ( rpos < queue.size() ) {
    const TgNode* node = queue[rpos];
    ++ rpos;
    ymuint nfo = node->fanout_num();
    for (ymuint i = 0; i < nfo; ++ i) {
      const TgNode* onode = node->fanout(i);
      if ( mark(onode) == kNone ) {
	mark(onode) = kTFO;
	queue.push_back(onode);
      }
    }
  }

  // マークの付いたノードの TFI に別のマークをつける．
  vector<const TgNode*> queue2;
  for (ymuint rpos = 0; rpos < queue.size(); ++ rpos) {
    const TgNode* node = queue[rpos];
    ymuint ni = node->ni();
    for (ymuint i = 0; i < ni; ++ i) {
      const TgNode* inode = node->fanin(i);
      if ( mark(inode) == kNone ) {
	// マークの付いていないファンインがあった．
	mark(inode) = kTFI;
	queue2.push_back(inode);
      }
    }
  }
  for (ymuint rpos2 = 0; rpos2 < queue2.size(); ++ rpos2) {
    const TgNode* node = queue2[rpos2];
    ymuint ni = node->ni();
    for (ymuint i = 0; i < ni; ++ i) {
      const TgNode* inode = node->fanin(i);
      if ( mark(inode) == kNone ) {
	// マークの付いていないファンインがあった．
	mark(inode) = kTFI;
	queue2.push_back(inode);
      }
    }
  }
  
  // 以降は kTFO か kTFI マークのついたノードのみを対象とする．

  SatSolver* solver_p = NULL;
  switch ( mMode ) {
  case 0:
    solver_p = SatSolverFactory::gen_solver();
    break;

  case 1:
    solver_p = SatSolverFactory::gen_recsolver(cout);
    break;

  case 2:
    solver_p = SatSolverFactory::gen_minisat();
    break;

  default:
    solver_p = SatSolverFactory::gen_solver();
    break;
  }
  SatSolver& solver = *solver_p;
  
  // 変数の生成 (glit, flit, dlit の3つを作る)
  // ちょっと分かりにくいが fsrc は普通のゲートにも
  // ファンアウトしているので kTFO マークは付かない．
  // ただし故障ゲートにファンアウトしているブランチの値
  // は故障値なので mFid を持つ．
  for (ymuint i = 0; i < n; ++ i) {
    const TgNode* node = network.node(i);
    Var& var = mVarMap[node->gid()];
    if ( var.mMark != kNone ) {
      var.mGid = solver.new_var();
      if ( node == fsrc || var.mMark == kTFO ) {
	var.mFid = solver.new_var();
	var.mDid = solver.new_var();
      }
    }
  }
  
  // 正常回路の CNF を生成
  for (ymuint i = 0; i < nl; ++ i) {
    const TgNode* node = network.sorted_logic(i);
    if ( mark(node) == kNone ) continue;
    ymuint ni = node->ni();
    vector<Literal> inputs(ni);
    for (ymuint j = 0; j < ni; ++ j) {
      const TgNode* inode = node->fanin(j);
      inputs[j] = Literal(gvar(inode), kPolPosi);
    }
    Literal output(gvar(node), kPolPosi);
    if ( node->is_cplx_logic() ) {
      LogExpr lexp = network.get_lexp(node);
      make_cnf(solver, lexp, output, inputs);
    }
    else {
      make_cnf(solver, node->type(), output, inputs);
    }
  }
  for (ymuint i = 0; i < npo; ++ i) {
    const TgNode* node = network.output(i);
    if ( mark(node) == kNone ) continue;
    const TgNode* inode = node->fanin(0);
    Literal input(gvar(inode), kPolPosi);
    Literal output(gvar(node), kPolPosi);
    solver.add_clause(~input,  output);
    solver.add_clause( input, ~output);
  }
  
  // 故障回路の CNF を生成
  if ( f->is_input_fault() ) {
    ymuint ipos = f->pos();
    fsrc = fnode->fanin(ipos);
    ymuint ni = fnode->ni();
    vector<Literal> inputs(ni);
    for (ymuint i = 0; i < ni; ++ i) {
      const TgNode* inode = fnode->fanin(i);
      if ( i == ipos ) {
	inputs[i] = Literal(fvar(inode), kPolPosi);
      }
      else {
	inputs[i] = Literal(gvar(inode), kPolPosi);
      }
    }
    Literal output(fvar(fnode), kPolPosi);
    if ( fnode->is_cplx_logic() ) {
      LogExpr lexp = network.get_lexp(fnode);
      make_cnf(solver, lexp, output, inputs);
    }
    else {
      make_cnf(solver, fnode->type(), output, inputs);
    }
    Literal dlit(dvar(fnode), kPolPosi);
    Literal glit(gvar(fnode), kPolPosi);
    solver.add_clause(~glit, ~output, ~dlit);
    solver.add_clause(~glit,  output,  dlit);
    solver.add_clause( glit, ~output,  dlit);
    solver.add_clause( glit,  output, ~dlit);
  }
  
  // mark の付いていないノードは正常回路，付いているノードは故障回路
  // を用いて CNF を作る．
  vector<Literal> odiff;
  odiff.reserve(npo);
  for (ymuint i = 1; i < queue.size(); ++ i) {
    const TgNode* node = queue[i];
    ymuint ni = node->ni();
    vector<Literal> inputs(ni);
    for (ymuint j = 0; j < ni; ++ j) {
      const TgNode* inode = node->fanin(j);
      if ( mark(inode) == kTFO ) {
	inputs[j] = Literal(fvar(inode), kPolPosi);
      }
      else {
	inputs[j] = Literal(gvar(inode), kPolPosi);
      }
    }
    Literal output(fvar(node), kPolPosi);
    if ( node->is_cplx_logic() ) {
      LogExpr lexp = network.get_lexp(node);
      make_cnf(solver, lexp, output, inputs);
    }
    else {
      make_cnf(solver, node->type(), output, inputs);
    }
    Literal glit(gvar(node), kPolPosi);
    Literal dlit(dvar(node), kPolPosi);
    solver.add_clause(~glit, ~output, ~dlit);
    solver.add_clause(~glit,  output,  dlit);
    solver.add_clause( glit, ~output,  dlit);
    solver.add_clause( glit,  output, ~dlit);
    if ( node->is_output() ) {
      odiff.push_back(dlit);
    }
    vector<Literal> dep;
    dep.reserve(ni + 1);
    for (ymuint j = 0; j < ni; ++ j) {
      const TgNode* inode = node->fanin(j);
      if ( mark(inode) == kTFO ) {
	dep.push_back(Literal(dvar(inode), kPolPosi));
      }
    }
    dep.push_back(~dlit);
    solver.add_clause(dep);
  }
  solver.add_clause(odiff);
  
  vector<Literal> assumptions(2);
  if ( f->val() ) {
    assumptions[0] = Literal(gvar(fsrc), kPolNega);
    assumptions[1] = Literal(fvar(fsrc), kPolPosi);
  }
  else {
    assumptions[0] = Literal(gvar(fsrc), kPolPosi);
    assumptions[1] = Literal(fvar(fsrc), kPolNega);
  }
  
  vector<Bool3> model;
  Bool3 stat = solver.solve(assumptions, model);
  tStat ans = kAbort;
  if ( stat == kB3True ) {
    tv->init();
    ymuint npi = network.input_num2();
    for (ymuint i = 0; i < npi; ++ i) {
      const TgNode* node = network.input(i);
      if ( mark(node) != kNone ) {
	ymuint idx = gvar(node);
	if ( model[idx] == kB3True ) {
	  tv->set_val(i, kVal1);
	}
	else {
	  tv->set_val(i, kVal0);
	}
      }
    }
    ans = kDetect;
  }
  else if ( stat == kB3False ) {
    ans = kUntest;
  }

  solver_p->get_stats(mStats);
  
  delete solver_p;
  
  return ans;
}

// @brief 直前の実行結果を得る．
const SatStats&
DtpgSat::stats() const
{
  return mStats;
}
  
// @brief 使用する SAT エンジンを指定する．
void
DtpgSat::set_mode(int mode)
{
  mMode = mode;
}

END_NAMESPACE_YM_ATPG_DTPGSAT
