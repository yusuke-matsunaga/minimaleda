
/// @file libym_bnet/BNetBlifWriter.cc
/// @brief ブーリアンネットワークの内容を出力する関数の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BNetBlifWriter.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_bnet/BNetBlifWriter.h"
#include "ym_bnet/BNetwork.h"
#include "ym_bnet/BNetSopDecomp.h"


BEGIN_NAMESPACE_YM_BNET

BEGIN_NONAMESPACE

// LogExpr から積和形論理式に変換し出力する．
bool
write_cover(size_t ni,
	    const LogExpr& f,
	    ostream& s)
{
  if ( f.is_zero() ) {
      return true;
  }
  if ( f.is_one() ) {
    // 定数1の場合
    for (size_t i = 0; i < ni; ++ i) {
      s << '-';
    }
    s << " 1" << endl;
    return true;
  }

  if ( f.is_literal() ) {
    // リテラルの場合
    tVarId pos = f.varid();
    if ( pos >= ni ) {
      return false;
    }
    for (size_t i = 0; i < ni; ++ i) {
      if ( i == pos ) {
	if ( f.is_posiliteral() ) {
	  s << '1';
	}
	else {
	  s << '0';
	}
      }
      else {
	s << '-';
      }
    }
    s << " 1" << endl;
    return true;
  }

  if ( f.is_and() ) {
    static vector<char> buff;
    buff.clear();
    buff.resize(ni);
    for (size_t i = 0; i < ni; ++ i) {
      buff[i] = '-';
    }
    size_t nc = f.child_num();
    for (size_t i = 0; i < nc; i ++) {
      LogExpr opr1 = f.child(i);
      if ( !opr1.is_literal() ) {
	return false;
      }
      tVarId pos = opr1.varid();
      if ( pos >= ni ) {
	return false;
      }
      if ( opr1.is_posiliteral() ) {
	buff[pos] = '1';
      }
      else {
	buff[pos] = '0';
      }
    }
    for (size_t i = 0; i < ni; ++ i) {
      s << buff[i];
    }
    s << " 1" << endl;
    return true;
  }

  if ( f.is_or() ) {
    size_t nc = f.child_num();
    for (size_t i = 0; i < nc; i ++) {
      LogExpr opr1 = f.child(i);
      bool stat = write_cover(ni, opr1, s);
      if ( !stat ) {
	return false;
      }
    }
    return true;
  }

  return false;
}

// 一つの節点の内容を表示する．
void
write_names(const BNode* node,
	    ostream& s)
{
  s << ".names";
  size_t ni = node->ni();
  for (size_t i = 0; i < ni; i ++) {
    BNode* inode = node->fanin(i);
    assert_cond(inode != NULL, __FILE__, __LINE__);
    s << " " << inode->name();
  }

  s << " " << node->name() << endl;

  // factored form をカバーに変換して出力する．
  const LogExpr& ff = node->func();
  bool stat = write_cover(ni, ff, s);
  assert_cond(stat, __FILE__, __LINE__);
}

END_NONAMESPACE


// @brief コンストラクタ
BNetBlifWriter::BNetBlifWriter()
{
}

// @brief デストラクタ
BNetBlifWriter::~BNetBlifWriter()
{
}

// ブーリアンネットワークをblif形式で表示
void
BNetBlifWriter::dump(ostream& s,
		     const BNetwork& network) const
{
  // SOP形式の論理式でないと出力できないのでそうでない場合には
  // SOP 分解を行なう．
  bool all_sop = true;
  for (BNodeList::const_iterator p = network.logic_nodes_begin();
       p != network.logic_nodes_end(); ++ p) {
    BNode* node = *p;
    if ( !node->is_sop() ) {
      all_sop = false;
      break;
    }
  }
  if ( !all_sop ) {
    BNetwork dummy = network;
    BNetSopDecomp decomp;
    decomp(dummy);
    dump(s, dummy);
    return;
  }

  // .model 文の処理
  s << ".model " << network.model_name() << endl;

  // .inputs 文の処理
  s << ".inputs";
  for (BNodeList::const_iterator p = network.inputs_begin();
       p != network.inputs_end(); ++ p) {
    BNode* node = *p;
    s << " " << node->name();
  }
  s << endl;

  // .outputs 文の処理
  s << ".outputs";
  for (BNodeList::const_iterator p = network.outputs_begin();
       p != network.outputs_end(); ++ p) {
    BNode* node = *p;
    s << " " << node->name();
  }
  s << endl;

  // .latch 文の処理
  for (BNodeList::const_iterator p = network.latch_nodes_begin();
       p != network.latch_nodes_end(); ++ p) {
    BNode* node = *p;
    BNode* inode = node->fanin(0);
    int reset_val = node->reset_value();
    s << ".latch " << inode->name() << " " << node->name();
    switch ( reset_val ) {
    case 0:
      s << " 0";
      break;
    case 1:
      s << " 1";
      break;
    }
    s << endl;
  }

  // 外部出力名とそのファンインの名前が異なっていて，なおかつその外部
  // 出力以外の他の外部出力にもファンアウトしていたらバッファを入れる．
  for (BNodeList::const_iterator p = network.outputs_begin();
       p != network.outputs_end(); ++ p) {
    BNode* node = *p;
    BNode* inode = node->fanin(0);
    if ( strcmp(node->name(), inode->name()) != 0 ) {
      s << ".names " << inode->name() << " " << node->name() << endl
	<< " 1 1" << endl;
    }
  }
  
  // 各内部節点の .names文/ .gate文の処理
  for (BNodeList::const_iterator p = network.logic_nodes_begin();
       p != network.logic_nodes_end(); ++ p) {
    BNode* node = *p;
    write_names(node, s);
  }

  // .end 文の処理
  s << ".end" << endl;
}

END_NAMESPACE_YM_BNET
