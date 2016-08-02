
/// @file libym_lexp/LogExprWriter.cc
/// @brief LogExprWriter の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: LogExprWriter.cc 958 2007-08-28 05:38:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_lexp/LogExprWriter.h"
#include "LexpNode.h"


BEGIN_NAMESPACE_YM_LEXP

// @brief コンストラクタ
LogExprWriter::LogExprWriter()
{
  mOpStrArray[0] = "~";
  mOpStrArray[1] = "&";
  mOpStrArray[2] = "|";
  mOpStrArray[3] = "^";
}

// @brief デストラクタ
LogExprWriter::~LogExprWriter()
{
}

// ostream に対する書出し
ostream&
LogExprWriter::dump(ostream& s,
		    const LogExpr& expr,
		    const VarStrMap& var_names) const
{
  dump_sub(s, expr.root(), var_names);
  return s;
}

// @brief 内容を文字列にする．
// @param[in] expr 文字列
// @param[in] var_names 各変数番号から変数名への写像
// @note varid 番目の変数名として var_names[varid] を用いる．
// 登録されていなければデフォルトの表記を用いる．
string
LogExprWriter::dump_string(const LogExpr& expr,
			   const VarStrMap& var_names) const
{
  ostringstream buf;
  dump(buf, expr, var_names);
  return string(buf.str());
}

// 演算子文字列を設定する．
void
LogExprWriter::set_opstr(const vector<string>& op)
{
  for (size_t i = 0; i < 4; i ++) {
    mOpStrArray[i] = op[i];
  }
}

// 演算子文字列を取得する．
void
LogExprWriter::opstr(vector<string>& op) const
{
  op.resize(4);
  for (size_t i = 0; i < 4; i ++) {
    op[i] = mOpStrArray[i];
  }
}

// NOT演算子の文字列を得る．
const string&
LogExprWriter::not_str() const
{
  return mOpStrArray[0];
}

// AND演算子の文字列を得る．
const string&
LogExprWriter::and_str() const
{
  return mOpStrArray[1];
}

// OR演算子の文字列を得る．
const string&
LogExprWriter::or_str() const
{
  return mOpStrArray[2];
}

// XOR演算子の文字列を得る．
const string&
LogExprWriter::xor_str() const
{
  return mOpStrArray[3];
}

// 論理式の内容を ostream に書出す．
ostream&
operator<<(ostream& s,
	   const LogExpr& expr)
{
  LogExprWriter writer;
  // 空の map を渡す
  return writer.dump(s, expr, VarStrMap());
}


// dump() のサブルーティン
void
LogExprWriter::dump_sub(ostream& s,
			const LexpNode* node,
			const VarStrMap& var_names) const
{
  if ( node->is_zero() ) {
    s << "0";
  }
  else if ( node->is_one() ) {
    s << "1";
  }
  else if ( node->is_literal() ) {
    if ( node->is_negaliteral() ) {
      s << not_str();
    }
    tVarId id = node->varid();
    VarStrMap::const_iterator p = var_names.find(id);
    if ( p == var_names.end() ) {
      s << "V" << id;
    }
    else {
      s << p->second;
    }
  }
  else { // AND/OR/XOR
    string delim;
    switch ( node->type() ) {
    case kAnd: delim = and_str(); break;
    case kOr:  delim = or_str(); break;
    case kXor: delim = xor_str(); break;
    default:
      break;
    }
    s << "( ";
    string delim1 = "";
    size_t n = node->child_num();
    for (size_t i = 0; i < n; ++ i) {
      s << delim1;
      dump_sub(s, node->child(i), var_names);
      delim1 = " " + delim + " ";
    }
    s << " )";
  }
}

END_NAMESPACE_YM_LEXP
