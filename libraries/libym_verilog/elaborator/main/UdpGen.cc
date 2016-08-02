
/// @file libym_verilog/elaborator/UdpGen.cc
/// @brief UdpGen の実装ファイル (UDPのインスタンス化関係)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: UdpGen.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "UdpGen.h"
#include "ElbEnv.h"

#include "ym_verilog/pt/PtUdp.h"
#include "ym_verilog/pt/PtPort.h"
#include "ym_verilog/pt/PtDecl.h"
#include "ym_verilog/pt/PtExpr.h"
#include "ym_verilog/pt/PtArray.h"
#include "ym_verilog/vl/VlExpr.h"

#include "ElbUdp.h"
#include "ElbExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス UdpGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] elab 生成器
// @param[in] elb_mgr Elbオブジェクトを管理するクラス
// @param[in] elb_factory Elbオブジェクトを生成するファクトリクラス
UdpGen::UdpGen(Elaborator& elab,
	       ElbMgr& elb_mgr,
	       ElbFactory& elb_factory) :
  ElbProxy(elab, elb_mgr, elb_factory)
{
}

// @brief デストラクタ
UdpGen::~UdpGen()
{
}

// @brief UDP定義を生成する．
// @param[in] pt_udp パース木の UDP 定義
void
UdpGen::instantiate_udp(const PtUdp* pt_udp)
{
  typedef hash_map<string, pair<const PtIOHead*, const PtIOItem*> > IODict;
  
  const FileRegion& file_region = pt_udp->file_region();
  const char* def_name = pt_udp->name();
  
  ostringstream buf;
  buf << "instantiating UDP \"" << def_name << "\".";
  put_msg(__FILE__, __LINE__,
	  file_region,
	  kMsgInfo,
	  "ELAB",
	  buf.str());

  ymuint io_size = pt_udp->port_num();

  ymuint isize = io_size - 1;
  tVpiPrimType ptype = pt_udp->prim_type();
  bool is_protected = true; // 何これ?

  ElbUdpDefn* udp = factory().new_UdpDefn(pt_udp, is_protected);

  // 最初のポート名 = 出力のポート名
  const char* outname = pt_udp->port(0)->ext_name();
  
  // ポート名をキーにしたIOテンプレートの辞書を作る．
  IODict iodict;
  const PtIOHead* outhead = NULL;
  for (ymuint i = 0; i < pt_udp->iohead_array().size(); ++ i) {
    const PtIOHead* iohead = pt_udp->iohead_array()[i];
    for (ymuint j = 0; j < iohead->item_num(); ++ j) {
      const PtIOItem* elem = iohead->item(j);
      const char* name = elem->name();
      iodict.insert(make_pair(name, make_pair(iohead, elem)));
      if ( strcmp(name, outname) == 0 ) {
	outhead = iohead;
      }
    }
  }

  // IOポートを実体化する．
  // ただし port_list に現れる名前の順番にしたがって実体化しなければならない．
  for (ymuint i = 0; i < io_size; ++ i) {
    const PtPort* port = pt_udp->port(i);
    const char* name = port->ext_name();
    IODict::const_iterator q = iodict.find(name);
    assert_cond(q != iodict.end(), __FILE__, __LINE__);
    const PtIOHead* pt_header = q->second.first;
    const PtIOItem* pt_item = q->second.second;
    udp->set_io(i, pt_header, pt_item);
  }
  
  // 初期化文を実体化させる．
  // initial 文がある場合と変数宣言の中に初期化式が含まれている場合がある．
  const PtExpr* pt_init_value = pt_udp->init_value();
  if ( pt_init_value == NULL ) {
    pt_init_value = outhead->item(0)->init_value();
  }
  if ( pt_init_value ) {
    // このチェックはパース時に済んでいるはずなので念のため．
    assert_cond(ptype == kVpiSeqPrim, __FILE__, __LINE__);

    const FileRegion& ifr = pt_init_value->file_region();

    // 式の生成
    ElbExpr* init_expr = instantiate_constant_expr(NULL, pt_init_value);
    if ( !init_expr ) {
      put_msg(__FILE__, __LINE__,
	      ifr,
	      kMsgError,
	      "ELAB",
	      "Constant value is required.");
      return;
    }

    // その値は 1ビットの 0 か 1 か X でなければならない．
    tVpiValueType type = init_expr->value_type();
    if ( type == kVpiValueReal ) {
      put_msg(__FILE__, __LINE__,
	      ifr,
	      kMsgError,
	      "ELAB",
	      "Only 1-bit constants are allowed here.");
      return;
    }
    if ( is_sized_type(type) && unpack_size(type) != 1 ) {
      put_msg(__FILE__, __LINE__,
	      ifr,
	      kMsgError,
	      "ELAB",
	      "Only 1-bit constants are allowed here.");
      return;
    }
    tVpiScalarVal val = init_expr->eval_scalar();

    // 初期値を設定する．
    udp->set_initial(init_expr, val);
  }

  // テーブルの中身を作る．
  // 似た様なことをしているが単純にするために組合わせ回路と順序回路を
  // 分ける．
  if ( ptype == kVpiCombPrim ) {
    // 組合わせ回路

    // 一行のサイズは入出力数と一致する．
    ymuint row_size = io_size;

    // 出力値の位置
    ymuint opos = row_size - 1;
  
    // 一行文のデータを保持しておくためのバッファ
    vector<tVpiUdpVal> row_data(row_size);
    
    PtUdpEntryArray table = pt_udp->table_array();
    for (ymuint i = 0; i < table.size(); ++ i) {
      const PtUdpEntry* pt_udp_entry = table[i];
      const FileRegion& tfr = pt_udp_entry->file_region();
      PtUdpValueArray input_array = pt_udp_entry->input_array();
      if ( input_array.size() != isize ) {
	// サイズが合わない．
	put_msg(__FILE__, __LINE__,
		tfr,
		kMsgError,
		"ELAB",
		"Number of input symbols mimatch.");
	return;
      }

      // 入力
      for (ymuint j = 0; j < isize; ++ j) {
	const PtUdpValue* pt_v = input_array[j];
	tVpiUdpVal symbol = pt_v->symbol();

	if ( is_edge_symbol(symbol) ) {
	  // 組合せ回路の場合にはエッジタイプの値は使えない．
	  ostringstream buf;
	  buf << symbol2string(symbol)
	      << " : transition symbol for combinational UDP";
	  put_msg(__FILE__, __LINE__,
		  pt_v->file_region(),
		  kMsgError,
		  "ELAB",
		  buf.str());
	  return;
	}
	if ( symbol == kVpiUdpValNC ) {
	  // NC は状態出力にしか使えない
	  ostringstream buf;
	  buf << symbol2string(symbol) << " : illegal symbol for input field.";
	  put_msg(__FILE__, __LINE__,
		  pt_v->file_region(),
		  kMsgError,
		  "ELAB",
		  buf.str());
	  return;
	}

	row_data[j] = symbol;
      }

      { // 現状態
	if ( pt_udp_entry->current() ) {
	  put_msg(__FILE__, __LINE__,
		  pt_udp_entry->file_region(),
		  kMsgError,
		  "ELAB",
		  "Combinational UDP should not have "
		  "\'current state\' value.");
	  return;
	}
      }

      { // 出力
	const PtUdpValue* pt_v = pt_udp_entry->output();
	tVpiUdpVal symbol = pt_v->symbol();

	if ( symbol == kVpiUdpValB || symbol == kVpiUdpValQ ) {
	  // 出力には複合値は使えない
	  ostringstream buf;
	  buf << symbol2string(symbol) << " : illegal symbol for output field.";
	  put_msg(__FILE__, __LINE__,
		  pt_v->file_region(),
		  kMsgError,
		  "ELAB",
		  buf.str());
	  return;
	}

	row_data[opos] = symbol;
      }

      // 一行文のデータを設定する．
      udp->set_tableentry(i, pt_udp_entry, row_data);
    }
  }
  else {
    // 順序回路

    // 一行のサイズは入出力 + 現状態
    ymuint row_size = io_size + 1;

    // 現状態値の位置
    ymuint cpos = isize;
    
    // 出力値の位置
    ymuint opos = io_size;
  
    // 一行文のデータを保持しておくためのバッファ
    vector<tVpiUdpVal> row_data(row_size);
    
    PtUdpEntryArray table = pt_udp->table_array();
    for (ymuint i = 0; i < table.size(); ++ i) {
      const PtUdpEntry* pt_udp_entry = table[i];
      const FileRegion& tfr = pt_udp_entry->file_region();
      PtUdpValueArray input_array = pt_udp_entry->input_array();
      
      if ( input_array.size() != isize ) {
	// サイズが合わない．
	put_msg(__FILE__, __LINE__,
		tfr,
		kMsgError,
		"ELAB",
		"Number of input symbols mimatch.");
	return;
      }

      // 一行中に含まれるエッジタイプのシンボルの数
      ymuint nt = 0;

      // 入力
      for (ymuint j = 0; j < isize; ++ j) {
	const PtUdpValue* pt_v = input_array[j];
	tVpiUdpVal symbol = pt_v->symbol();

	if ( is_edge_symbol(symbol) ) {
	  ++ nt;
	  if ( nt > 1 ) {
	    put_msg(__FILE__, __LINE__,
		    pt_v->file_region(),
		    kMsgError,
		    "ELAB",
		    "More than one transition symbols "
		    "in the same row.");
	    return;
	  }
	}

	// 順序回路の入力は全ての値/シンボルが使用可
	row_data[j] = symbol;
      }

      { // 現状態
	const PtUdpValue* pt_v = pt_udp_entry->current();
	if ( !pt_v ) {
	  put_msg(__FILE__, __LINE__,
		  tfr,
		  kMsgError,
		  "ELAB",
		  "Sequential UDP requires \'current state\' value.");
	  return;
	}

	tVpiUdpVal symbol = pt_v->symbol();
	
	if ( is_edge_symbol(symbol) ) {
	  // エッジタイプの値は使えない．
	  ostringstream buf;
	  buf << symbol2string(symbol)
	      << " : transition symbol for current state field.";
	  put_msg(__FILE__, __LINE__,
		  pt_v->file_region(),
		  kMsgError,
		  "ELAB",
		  buf.str());
	  return;
	}
	if ( symbol == kVpiUdpValNC ) {
	  // NC は状態出力にしか使えない
	  ostringstream buf;
	  buf << symbol2string(symbol)
	      << " : illegal symbol for current state field.";
	  put_msg(__FILE__, __LINE__,
		  pt_v->file_region(),
		  kMsgError,
		  "ELAB",
		  buf.str());
	  return;
	}

	row_data[cpos] = symbol;
      }
	
      { // 出力
	const PtUdpValue* pt_v = pt_udp_entry->output();
	tVpiUdpVal symbol = pt_v->symbol();

	if ( is_edge_symbol(symbol) ) {
	  // エッジタイプの値は使えない．
	  ostringstream buf;
	  buf << symbol2string(symbol)
	      << " : transition symbol for output field.";
	  put_msg(__FILE__, __LINE__,
		  pt_v->file_region(),
		  kMsgError,
		  "ELAB",
		  buf.str());
	  return;
	}
	if ( symbol == kVpiUdpValB || symbol == kVpiUdpValQ ) {
	  // 出力には複合値は使えない
	  ostringstream buf;
	  buf << symbol2string(symbol)
	      << " : illegal symbol for output field.";
	  put_msg(__FILE__, __LINE__,
		  pt_v->file_region(),
		  kMsgError,
		  "ELAB",
		  buf.str());
	  return;
	}

	row_data[opos] = symbol;
      }

      // 一行文のデータを設定する．
      udp->set_tableentry(i, pt_udp_entry, row_data);
    }
  }

  // UDP を登録する．
  reg_udp(def_name, udp);
}

END_NAMESPACE_YM_VERILOG
