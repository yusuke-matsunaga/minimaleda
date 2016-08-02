
/// @file gdsprint/gdsprint.cc
/// @brief GDS-II ファイルダンププログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: gdsprint.cc 1343 2008-03-25 17:15:35Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_gds/GdsScanner.h"
#include "ym_gds/GdsRecMgr.h"
#include "ym_gds/GdsRecord.h"
#include "ym_gds/GdsDumper.h"
#include "ym_gds/Msg.h"

int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm::nsGds;

  GdsRecMgr mgr;
  GdsScanner scanner(cin, mgr);
  GdsDumper dumper(cout);
  
  MsgMgr& msgmgr = MsgMgr::the_mgr();
  tMsgMask msgmask = kMsgMaskError | kMsgMaskWarning;
  TestMsgHandler* tmh = new TestMsgHandler(msgmask);
  msgmgr.reg_handler(tmh);

  GdsRecord* rec;
  while ( (rec = scanner.read_rec()) ) {
    dumper(*rec);
    mgr.free_rec(rec);
  }

  return 0;
}
