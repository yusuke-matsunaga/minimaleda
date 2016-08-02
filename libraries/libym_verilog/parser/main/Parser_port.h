#ifndef LIBYM_VERILOG_PARSER_MAIN_PARSER_PORT_H
#define LIBYM_VERILOG_PARSER_MAIN_PARSER_PORT_H

/// @file libym_verilog/parser/main/Parser_port.h
/// @brief Parser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Parser_port.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "Parser.h"
#include "PtiDecl.h"



BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// PtiPort の生成
//////////////////////////////////////////////////////////////////////

// @brief 空のポートの生成
inline
void
Parser::new_Port()
{
  add_port( mFactory.new_Port(FileRegion()) );
}

// @brief ポートの生成 (内側の式のみ指定するタイプ)
// @param[in] file_region ファイル位置
// @param[in] name 外側の名前
inline
void
Parser::new_Port1(const FileRegion& file_region)
{
  PtiPortRefArray port_expression = get_portref_array();
  // 明示的に外の名前がついていなくても内側の名前が1つで
  // 範囲指定が無いときには内側の名前を外側の名前とする．
  const char* name = NULL;
  if ( port_expression.size() == 1 ) {
    PtiPortRef* top = port_expression[0];
    if ( top->index() == NULL && top->left_range() == NULL ) {
      name = top->name();
    }
  }
  add_port( mFactory.new_Port(file_region, port_expression, name) );
}

// @brief ポートの生成 (外側の名前のみ指定するタイプ)
// @param[in] file_region ファイル位置
// @param[in] name 外側の名前
inline
void
Parser::new_Port2(const FileRegion& file_region,
		  const char* name)
{
  add_port( mFactory.new_Port(file_region, name) );
}

// @brief ポートの生成 (外側の名前と内側の式を指定するタイプ)
// @param[in] file_region ファイル位置
// @param[in] name 外側の名前
inline
void
Parser::new_Port3(const FileRegion& file_region,
		  const char* name)
{
  PtiPortRefArray port_expression = get_portref_array();
  add_port( mFactory.new_Port(file_region, port_expression, name) );
}

// @brief ポートリストにポートを追加する．
inline
void
Parser::add_port(PtiPort* port)
{
  mPortList.push_back(port);
}

// @brief ポートリストを配列に変換する．
inline
PtiPortArray
Parser::get_port_array()
{
  return mPortList.to_array(mAlloc);
}

//////////////////////////////////////////////////////////////////////
// PtiPortRef の生成
//////////////////////////////////////////////////////////////////////

// @brief ポート参照式の生成
// @param[in] fr ファイル位置の情報
// @param[in] name ポートに接続している内部の識別子名
inline
void
Parser::new_PortRef(const FileRegion& fr,
		    const char* name)
{
  add_portref( mFactory.new_PortRef(fr, name) );
}

// @brief ビット指定つきポート参照式の生成
// @param[in] fr ファイル位置の情報
// @param[in] name ポートに接続している内部の識別子名
// @param[in] index ビット指定用の式
inline
void
Parser::new_PortRef(const FileRegion& fr,
		    const char* name,
		    PtExpr* index)
{
  add_portref( mFactory.new_PortRef(fr, name, index) );
}

// @brief 範囲指定付きポート参照式の生成
// @param[in] fr ファイル位置の情報
// @param[in] name ポートに接続している内部の識別子名
// @param[in] range_mode 範囲指定のモード
// @param[in] left 範囲指定の左側の式
// @param[in] right 範囲指摘の右側の式
inline
void
Parser::new_PortRef(const FileRegion& fr,
		    const char* name,
		    tVpiRangeMode range_mode,
		    PtExpr* left,
		    PtExpr* right)
{
  add_portref( mFactory.new_PortRef(fr, name, range_mode, left, right) );
}

// @brief ポート参照リストに要素を追加する．
inline
void
Parser::add_portref(PtiPortRef* portref)
{
  mPortRefList.push_back(portref);
}

// @brief ポート参照リストを初期化する．
inline
void
Parser::init_portref_list()
{
  mPortRefList.clear();
}

// @brief ポート参照リストを配列に変換する
inline
PtiPortRefArray
Parser::get_portref_array()
{
  return mPortRefList.to_array(mAlloc);
}

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_PARSER_MAIN_PARSER_PORT_H
