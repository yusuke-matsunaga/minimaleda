
/// @file libym_verilog/tests/vltest/vltest_elaborate.cc
/// @brief elabrate モードのテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: vltest_elaborate.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include <ym_utils/StopWatch.h>
#include "VlTestLineWatcher.h"
#include <ym_verilog/VlMgr.h>
#include "VlDumper.h"



BEGIN_NAMESPACE_YM_VERILOG

void
elaborate_mode(const list<string>& filename_list,
	       bool all_msg,
	       const char* spath,
	       int watch_line,
	       bool verbose,
	       bool profile,
	       int loop,
	       bool dump_vpi)
{
  MsgMgr msgmgr;
  MsgHandler* tmh = new StreamMsgHandler(&cerr);
  if ( all_msg ) {
    tmh->set_mask(MsgHandler::kMaskAll);
  }
  else {
    tmh->set_mask(MsgHandler::kMaskAll);
    tmh->delete_mask(kMsgInfo);
    tmh->delete_mask(kMsgDebug);
  }
  msgmgr.reg_handler(tmh);

  SearchPathList splist;
  if ( spath ) {
    splist.set(spath);
  }

  VlTestLineWatcher watcher(watch_line);
  list<VlLineWatcher*> watcher_list;
  if ( watch_line ) {
    watcher_list.push_back(&watcher);
  }

  ymuint32 c = loop + 1;
  for (ymuint32 i = 0; i < c; ++ i) {
    
#if 0
    try {
#endif
      
      StopWatch timer;
      timer.start();
      VlMgr vlmgr(msgmgr);
      for (list<string>::const_iterator p = filename_list.begin();
	   p != filename_list.end(); ++ p) {
	const string& name = *p;
	if ( verbose ) {
	  cerr << "Reading " << name;
	  cerr.flush();
	}
	vlmgr.read_file(name, splist, watcher_list);
	if ( verbose ) {
	  cerr << " end" << endl;
	}
      }

      timer.stop();
      USTime time = timer.time();
      if ( verbose ) {
	cerr << "Parsing time: " << time << endl;
      }

      if ( !msgmgr.error_num() ) {
	StopWatch timer;
	timer.start();

	vlmgr.elaborate();
	
	timer.stop();
	USTime time = timer.time();
	if ( verbose ) {
	  cerr << "Elaborating time: " << time << endl;
	}
	if ( profile ) {
	  cout << "VlMgr:   " << vlmgr.allocated_size() / (1024 * 1024)
	       << "M bytes" << endl;
	  sleep(20);
	}
      
	if ( msgmgr.error_num() == 0 && dump_vpi ) {
	  VlDumper dumper(cout);
	  dumper(vlmgr);
	}
      }
      switch ( msgmgr.error_num() ) {
      case 0:
	cerr << "No errors" << endl;
	break;
      case 1:
	cerr << "Total 1 error" << endl;
	break;
	
      default:
	cerr << "Total " << msgmgr.error_num() << " errors" << endl;
	break;
      }
      
#if 0
    }
    catch ( AssertError x ) {
      cerr << x << endl;
    }
    catch ( std::bad_alloc x ) {
      cerr << "no more memory" << endl;
      exit(1);
    }
    catch (...) {
      cerr << "unkown exception" << endl;
    }
#endif
  }
}

END_NAMESPACE_YM_VERILOG
