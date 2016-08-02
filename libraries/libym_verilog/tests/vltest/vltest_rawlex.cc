
/// @file libym_verilog/tests/vltest/vltest_rawlex.cc
/// @brief rawlex モードのテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: vltest_rawlex.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include <ym_utils/StopWatch.h>
#include "VlTestLineWatcher.h"


#include "Lex.h"
#include "print_token.h"
#include "LexModuleState.h"

#include "parser.h"


BEGIN_NAMESPACE_YM_VERILOG

void
rawlex_mode(const list<string>& filename_list,
	    const char* spath,
	    int watch_line,
	    bool verbose,
	    bool dump_token)
{
  MsgMgr msg_mgr;
  MsgHandler* handler = new StreamMsgHandler(&cerr);
  msg_mgr.reg_handler(handler);
  FileDescMgr fd_mgr;
  Lex lex(msg_mgr, fd_mgr);
  
  if ( spath ) {
    SearchPathList splist(spath);
    lex.set_searchpath(splist);
  }
  if ( watch_line ) {
    VlTestLineWatcher* mw = new VlTestLineWatcher(watch_line);
    lex.reg_watcher(mw);
  }

  StopWatch timer;
  timer.start();

  for (list<string>::const_iterator p = filename_list.begin();
       p != filename_list.end(); ++ p) {
    const string& filename = *p;
    if ( !lex.open_file(filename) ) {
      cerr << filename << " : cannot open" << endl;
      return;
    }

    for ( ; ; ) {
      int id = lex.get_raw_token();
      if ( id == EOF ) {
	break;
      }
      if ( dump_token ) {
	cout << lex.cur_token_loc() << ": ";
	print_token(cout, id, lex.cur_string());
	cout << endl;
      }
    }
  }

  timer.stop();
  if ( verbose ) {
    USTime time = timer.time();
    cout << "Time: " << time << endl;
  }
}

END_NAMESPACE_YM_VERILOG
