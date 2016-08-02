
/// @file gdsprint/gdsparse.cc
/// @brief GDS-II ファイルのパーサーテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: gdsparse.cc 1343 2008-03-25 17:15:35Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_gds/gds_nsdef.h"
#include "ym_gds/GdsScanner.h"
#include "ym_gds/GdsRecMgr.h"


BEGIN_NAMESPACE_YM_GDS_PARSER

void
set_scanner(GdsScanner* scanner);

int
yyparse();

END_NAMESPACE_YM_GDS_PARSER

int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm::nsGds;

  GdsRecMgr mgr;
  GdsScanner scanner(cin, mgr);
  //GdsDumper dumper(cout);

  nsParser::set_scanner(&scanner);

  nsParser::yyparse();

  return 0;
}
