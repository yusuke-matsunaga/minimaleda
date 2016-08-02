
/// @file libym_verilog/parser/Parser.cc
/// @brief Parser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Parser_udp.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "Parser.h"
#include "Parser_port.h"

#include "PtMgr.h"
#include "PtiFactory.h"
#include "PtiDecl.h"
#include "ym_verilog/pt/PtUdp.h"
#include "ym_verilog/pt/PtExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// UDP 関係
//////////////////////////////////////////////////////////////////////

// UDP を生成する．(Verilog-1995)
void
Parser::new_Udp1995(const FileRegion& file_region,
		    const char* udp_name,
		    const char* init_name,
		    const FileRegion& init_loc,
		    PtExpr* init_value,
		    PtrList<PtAttrInst>* ai_list)
{
  PtIOHeadArray iohead_array = get_io_array();
  PtDeclHeadArray decl_array = get_decl_array();

  const PtIOItem* out_item = NULL;
  bool is_seq = false;
  
  bool sane = true;
  
  // Verilog1995 の形式の時には YACC の文法では規定できないので，
  // 1. port_list に現れる名前が portdecl_list 中に存在すること．
  // 2. その最初の名前は output であること．
  // 3. それ以外はすべて input であること
  // の確認を行う．
  // まず portdecl_list の各要素を名前をキーにした連想配列に格納する．
  // ついでに output の数を数える．
  hash_map<string, const PtIOItem*> iomap;
  for (ymuint32 i = 0; i < iohead_array.size(); ++ i) {
    const PtIOHead* io = iohead_array[i];
    if ( io->type() == kPtIO_Output ) {
      if ( out_item ) {
	// 複数の出力宣言があった．
	put_msg(__FILE__, __LINE__,
		io->file_region(),
		kMsgError,
		"PARS",
		"More than two output declarations");
	sane = false;
	break;
      }
      
      // これは YACC の文法が正しくかけていれば成り立つはず．
      assert_cond(io->item_num() == 1, __FILE__, __LINE__);

      out_item = io->item(0);
 
      if ( io->aux_type() == kVpiAuxReg ) {
	is_seq = true;
      }
    }
    for (ymuint32 j = 0; j < io->item_num(); ++ j) {
      const PtIOItem* elem = io->item(j);
      if ( iomap.count(elem->name()) > 0 ) {
	// 二重登録
	ostringstream buf;
	buf << elem->name() << ": Defined more than once.";
	put_msg(__FILE__, __LINE__,
		elem->file_region(),
		kMsgError,
		"PARS",
		buf.str());
	sane = false;
	break;
      }
      iomap.insert(make_pair(elem->name(), elem));
    }
  }

  // port_list に現れる名前が iolist 中にあるか調べる．
  PtiPortArray port_array = get_port_array();
  for (ymuint32 i = 0; i < port_array.size(); ++ i) {
    const PtiPort* port = port_array[i];
    const char* port_name = port->ext_name();
    hash_map<string, const PtIOItem*>::iterator q = iomap.find(port_name);
    if ( q == iomap.end() ) {
      ostringstream buf;
      buf << "\"" << port_name << "\" undefined.";
      put_msg(__FILE__, __LINE__,
	      file_region,
	      kMsgError,
	      "PARS",
	      buf.str());
      sane = false;
      break;
    }
    if ( i == 0 ) {
      const PtIOItem* ioelem = q->second;
      if ( out_item != ioelem ) {
	// 最初の名前は output でなければならない．
	ostringstream buf;
	buf << port_name << " must be an output.";
	put_msg(__FILE__, __LINE__,
		ioelem->file_region(),
		kMsgError,
		"PARS",
		buf.str());
	sane = false;
	break;
      }
    }
    iomap.erase(q);
  }
    
  if ( !iomap.empty() ) {
    // iolist 中のみに現れる要素がある．
    for (hash_map<string, const PtIOItem*>::const_iterator q
	   = iomap.begin(); q != iomap.end(); ++ q) {
      const PtIOItem* ioelem = q->second;
      ostringstream buf;
      buf << "\"" << ioelem->name() << "\" does not appear in portlist.";
      put_msg(__FILE__, __LINE__,
	      ioelem->file_region(),
	      kMsgError,
	      "PARS",
	      buf.str());
    }
    sane = false;
  }

  // 次に decl_list の要素数が1以下であり，
  // さらにその要素が REG で名前が出力名と一致することを確認する．
  // ちなみに YACC の文法から REG 以外の宣言要素はありえない．
  if ( decl_array.size() > 1 ) {
    // 二つ以上の reg 宣言があった．
    put_msg(__FILE__, __LINE__,
	    decl_array[1]->file_region(),
	    kMsgError,
	    "PARS",
	    "More than two 'reg' declarations.");
    sane = false;
  }
  else if ( decl_array.size() == 1 ) {
    const PtDeclHead* reghead = decl_array[0];
    if ( reghead ) {
      is_seq = true;
      assert_cond(reghead->type() == kPtDecl_Reg, __FILE__, __LINE__);
      assert_cond(reghead->item_num() == 1, __FILE__, __LINE__);
      const PtDeclItem* regitem = reghead->item(0);
      assert_cond(regitem, __FILE__, __LINE__);
      if ( strcmp(regitem->name(), out_item->name()) != 0 ) {
	// output と名前が違う
	ostringstream buf;
	buf << "Reg name \"" << regitem->name()
	    << "\" differes from output name \""
	    << out_item->name() << "\".";
	put_msg(__FILE__, __LINE__,
		regitem->file_region(),
		kMsgError,
		"PARS",
		buf.str());
	sane = false;
      }
    }
  }
  
  if ( sane ) {
    new_Udp(file_region,
	    udp_name,
	    init_name,
	    init_loc,
	    init_value,
	    ai_list,
	    is_seq,
	    out_item,
	    port_array,
	    iohead_array);
  }
  end_udp();
}

// UDP を生成する．(Verilog-2001)
void
Parser::new_Udp2001(const FileRegion& file_region,
		    const char* udp_name,
		    const char* init_name,
		    const FileRegion& init_loc,
		    PtExpr* init_value,
		    PtrList<PtAttrInst>* ai_list)
{
  PtIOHeadArray iohead_array = get_io_array();
  
  bool is_seq = false;

  // YACC の文法が正しく書かれていれば最初のヘッダが出力で
  // 要素数が1となっているはず．
  assert_cond(iohead_array.size() > 0, __FILE__, __LINE__);
  const PtIOHead* out_head = iohead_array[0];
  assert_cond(out_head->type() == kPtIO_Output, __FILE__, __LINE__);
  assert_cond(out_head->item_num() == 1, __FILE__, __LINE__);
  const PtIOItem* out_item = out_head->item(0);
  
  if ( out_head->aux_type() == kVpiAuxReg ) {
    is_seq = true;
  }
  // 残りの要素は入力になっているはず．

  // iohead_array から port_array を生成する．
  PtiPortArray port_array = new_PortArray(iohead_array);
  
  new_Udp(file_region,
	  udp_name,
	  init_name,
	  init_loc,
	  init_value,
	  ai_list,
	  is_seq,
	  out_item,
	  port_array,
	  iohead_array);
  end_udp();
}

// @brief new_Udp の下請け関数
void
Parser::new_Udp(const FileRegion& file_region,
		const char* udp_name,
		const char* init_name,
		const FileRegion& init_loc,
		PtExpr* init_value,
		PtrList<PtAttrInst>* ai_list,
		bool is_seq,
		const PtIOItem* out_item,
		PtiPortArray port_array,
		PtIOHeadArray iohead_array)
{
  PtUdp* udp = NULL;
  if ( is_seq ) {
    // 初期値の設定がある．
    if ( init_name ) {
      
      // init_name が正しいかチェックする．
      if ( strcmp(init_name, out_item->name()) != 0 ) {
	// output 文の名前と異なる．
	ostringstream buf;
	buf << "Lhs of initial \"" << init_name
	    << "\" differes from output name \""
	    << out_item->name() << "\".";
	put_msg(__FILE__, __LINE__,
		init_loc,
		kMsgError,
		"PARS",
		buf.str());
	return;
      }
      
      if ( out_item->init_value() ) {
	// output 文にも初期値割り当てがある．
	// これは warning にする．
	put_msg(__FILE__, __LINE__,
		init_value->file_region(),
		kMsgWarning,
		"PARS",
		"Both output declaration and initial block"
		" have the initial values,"
		" output declarations's initial value is ignored.");
      }
    }
  
    // このあと elaboration で注意が必要なのは init_value.
    // 場合によってはこれが NULLで outhead->top()->init_value()
    // が空でない場合がある．
    udp = mFactory.new_SeqUdp(file_region,
			      udp_name,
			      port_array,
			      iohead_array,
			      init_value,
			      get_udp_entry_array());
  }
  else {
    if ( init_name ) {
      // sequential primitive でなければ初期値を持てない．
      put_msg(__FILE__, __LINE__,
	      init_loc,
	      kMsgError,
	      "PARS",
	      "Combinational primitive can not have the initial value.");
      return;
    }
    udp = mFactory.new_CmbUdp(file_region,
			      udp_name,
			      port_array,
			      iohead_array,
			      get_udp_entry_array());
  }
  
  mPtMgr.reg_udp(udp);
  reg_attrinst(udp, ai_list);
}

// @brief combinational UDP 用のテーブルエントリの生成
// @param[in] fr ファイル位置の情報
// @param[in] output_loc 出力記号の位置
// @param[in] output_symbol 出力記号
void
Parser::new_UdpEntry(const FileRegion& fr,
		     const FileRegion& output_loc,
		     tVpiUdpVal output_symbol)
{
  PtUdpValue* output = mFactory.new_UdpValue(output_loc, output_symbol);
  add_udp_entry( mFactory.new_UdpEntry(fr, get_udp_value_array(), output) );
}

// @brief sequential UDP 用のテーブルエントリの生成
// @param[in] fr ファイル位置の情報
// @param[in] current_loc 現状態記号の位置
// @param[in] current_symbol 現状態記号
// @param[in] output_loc 出力記号の位置
// @param[in] output_symbol 出力記号
void
Parser::new_UdpEntry(const FileRegion& fr,
		     const FileRegion& current_loc,
		     tVpiUdpVal current_symbol,
		     const FileRegion& output_loc,
		     tVpiUdpVal output_symbol)
{
  PtUdpValue* current = mFactory.new_UdpValue(current_loc, current_symbol);
  PtUdpValue* output = mFactory.new_UdpValue(output_loc, output_symbol);
  add_udp_entry( mFactory.new_UdpEntry(fr, get_udp_value_array(),
				       current, output) );
}

// @brief UdpEntry を追加する．
inline
void
Parser::add_udp_entry(PtUdpEntry* entry)
{
  mUdpEntryList.push_back(entry);
}

// @brief UdpEntry のリストを配列に変換する．
inline
PtUdpEntryArray
Parser::get_udp_entry_array()
{
  return mUdpEntryList.to_array(mAlloc);
}
  
// @brief UDP のテーブルエントリの要素の値の生成
// @param[in] fr ファイル位置の情報
// @param[in] symbol シンボル
// @return 生成された値
void
Parser::new_UdpValue(const FileRegion& fr,
		     tVpiUdpVal symbol)
{
  add_udp_value( mFactory.new_UdpValue(fr, symbol) );
}

// @brief UdpValue のリストを初期化する．
void
Parser::init_udp_value_list()
{
  mUdpValueList.clear();
}

// @brief UdpValue を追加する．
inline
void
Parser::add_udp_value(PtUdpValue* value)
{
  mUdpValueList.push_back(value);
}

// @brief UdpValue のリストを配列に変換する．
inline
PtUdpValueArray
Parser::get_udp_value_array()
{
  return mUdpValueList.to_array(mAlloc);
}

END_NAMESPACE_YM_VERILOG
