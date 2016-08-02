
/// @file libym_verilog/tests/vltest/vltest_lex.cc
/// @brief lex モードのテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: vltest_lex.cc 2507 2009-10-17 16:24:02Z matsunaga $
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
lex_mode(const list<string>& filename_list,
	 const char* spath,
	 int watch_line,
	 bool verbose,
	 int loop,
	 bool dump_token)
{
  ymuint32 c = loop + 1;
  for (ymuint32 i = 0; i < c; ++ i) {
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
  
    int nt_id = 0;
    int nt_sid = 0;
    int nt_un = 0;
    int nt_unb = 0;
    int nt_uni = 0;
    int nt_rn = 0;
    int nt_s = 0;
    int ns = 0;
    for (list<string>::const_iterator p = filename_list.begin();
	 p != filename_list.end(); ++ p) {
      const string& filename = *p;
      if ( !lex.open_file(filename) ) {
	cerr << filename << " : cannot open" << endl;
	return;
      }

      for ( ; ; ) {
	int id = lex.get_token();
	if ( id == EOF ) {
	  break;
	}
	if ( dump_token ) {
	  cout << lex.cur_token_loc() << ": ";
	  print_token(cout, id, lex.cur_string());
	  cout << endl;
	}
      
	switch ( id ) {
	case IDENTIFIER:
	  ++ nt_id;
	  break;
	case SYS_IDENTIFIER:
	  ++ nt_sid;
	  break;
	case UNUMBER:
	  ++ nt_un;
	  break;
	case UNUM_BIG:
	  ++ nt_unb;
	case UNUM_INT:
	  ++ nt_uni;
	  break;
	case RNUMBER:
	  ++ nt_rn;
	  break;
	case STRING:
	  ++ nt_s;
	  break;
	default:
	  ++ ns;
	  break;
	}
      }
    }

    sleep(10);

    timer.stop();
    USTime time = timer.time();
    
    if ( verbose ) {
      cout << "Total " << setw(10) << nt_id << " IDENTIFIER tokens" << endl
	   << "      " << setw(10) << nt_sid << " SYS_IDENTIFIER tokens" << endl
	   << "      " << setw(10) << nt_un << " UNUMBER tokens" << endl
	   << "      " << setw(10) << nt_unb << " UNUM BIG" << endl
	   << "      " << setw(10) << nt_uni << " UNUM INT" << endl
	   << "      " << setw(10) << nt_rn << " RNUMBER tokens" << endl
	   << "      " << setw(10) << nt_s << " STRING tokens" << endl
	   << "      " << setw(10) << ns << " symbols" << endl
	   << "Time: " << time << endl;
    }
  }
}

END_NAMESPACE_YM_VERILOG
