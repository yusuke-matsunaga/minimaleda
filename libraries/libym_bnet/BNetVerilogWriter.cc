
/// @file libym_bnet/BNetVerilogWriter.cc
/// @brief ブーリアンネットワークの内容を出力する関数の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BNetVerilogWriter.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_bnet/BNetVerilogWriter.h"
#include "ym_bnet/BNetwork.h"
#include "ym_lexp/LogExprWriter.h"


BEGIN_NAMESPACE_YM_BNET

BEGIN_NONAMESPACE

// Verilog-HDL で許された識別子名に変換する
string
verilog_name(const string& name)
{
  string::const_iterator p = name.begin();
  char c = *p ++;
  if ( !isalpha(c) && c != '_' ) {
    goto need_escape;
  }
  for ( ; p != name.end(); ++ p) {
    char c = *p;
    if ( !isalnum(c) && c != '_' ) {
      goto need_escape;
    }
  }
  return name;

 need_escape:
  // 先頭が '\' ならば次の空白までを識別子と見なす(らしい)
  string ans = "\\" + name + " ";
  return ans;
}

END_NONAMESPACE


// @brief コンストラクタ
BNetVerilogWriter::BNetVerilogWriter()
{
}

// @brief デストラクタ
BNetVerilogWriter::~BNetVerilogWriter()
{
}

// ブーリアンネットワークをVerilog形式で表示
void
BNetVerilogWriter::dump(ostream& s,
			const BNetwork& network) const
{
  // module 文の処理
  s << "module\t" << verilog_name(network.model_name());
  string comma = "";
  s <<"( ";
  for (BNodeList::const_iterator p = network.inputs_begin();
       p != network.inputs_end(); ++p) {
    BNode* node = *p;
    s << comma;
    comma = ", ";
    s << verilog_name(node->name());
  }
  for (BNodeList::const_iterator p = network.outputs_begin();
       p != network.outputs_end(); ++p) {
    BNode* node = *p;
    BNode* inode = node->fanin(0);
    s << comma;
    comma = ", ";
    if ( inode->name() == node->name() ) {
      s << verilog_name(node->name());
    }
    else {
      // 複数のファンアウトを持ち，その中に外部出力が含まれる場合には
      // ポート結合が複雑になる．
      // inode が外部入力でも同様
      s << "." << verilog_name(node->name()) << "("
	<< verilog_name(inode->name()) << ")";
    }
  }
  s << " );" << endl;

  // input文の処理
  comma = "input\t";
  for (BNodeList::const_iterator p = network.inputs_begin();
       p != network.inputs_end(); ++p) {
    BNode* node = *p;
    s << comma;
    comma = ", ";
    s << verilog_name(node->name());
  }
  if ( comma != "input\t" ) {
    s << ";" << endl;
  }

  // output文の処理
  comma = "output\t";
  for (BNodeList::const_iterator p = network.outputs_begin();
       p != network.outputs_end(); ++p) {
    BNode* node = *p;
    s << comma;
    comma = ", ";
    s << verilog_name(node->name());
  }
  if ( comma != "output\t" ) {
    s << ";" << endl;
  }

  // wire文の処理
  size_t num = 0;
  for (BNodeList::const_iterator p = network.logic_nodes_begin();
       p != network.logic_nodes_end(); ++p) {
    BNode* node = *p;
    if ( num == 0 ) {
      s << "\twire\t";
    }
    else if ( (num % 8) == 0 ) {
      s << ",\n\t\t";
    }
    else {
      s << ", ";
    }
    ++ num;
    s << verilog_name(node->name());
  }
  if ( num > 0 ) {
    s << ";" << endl;
  }

  // 各内部節点の処理
  LogExprWriter writer;
  VarStrMap fanin_name;
  for (BNodeList::const_iterator p = network.logic_nodes_begin();
       p != network.logic_nodes_end(); ++p) {
    BNode* node = *p;
    string name = verilog_name(node->name());
    s << "\tassign\t" << name << " = ";
    size_t n = node->ni();
    fanin_name.clear();
    for (size_t i = 0; i < n; i ++) {
      BNode* inode = node->fanin(i);
      assert_cond( inode != 0, __FILE__, __LINE__);
      fanin_name.insert(make_pair(i, verilog_name(inode->name())));
    }
    const LogExpr& f = node->func();
    writer.dump(s, f, fanin_name);
    s << ";" << endl;
  }

  // endmodule 文の処理
  s << "endmodule" << endl;
}

END_NAMESPACE_YM_BNET
