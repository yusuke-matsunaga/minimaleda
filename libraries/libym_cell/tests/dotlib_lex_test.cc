
/// @file ym_cell/tests/dotlib_lex_test.cc
/// @brief DotLibLex のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: dotlib_lex_test.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include <iomanip>

#include "ym_utils/StopWatch.h"
#include "../dotlib/DotLibLex.h"
#include "ym_utils/MsgHandler.h"


int
main(int argc,
     char** argv)
{
  using namespace nsYm;
  using namespace nsYm::nsCell;
  
  if ( argc < 2 ) {
    cerr << "USAGE: " << argv[0]
	 << " <file-name> ..." << endl;
    return -1;
  }
  
  MsgMgr msg_mgr;
  DotLibLex lex(msg_mgr);
  
  StopWatch timer;
  timer.start();
  
  for (int i = 1; i < argc; ++ i) {
    if ( !lex.open_file(argv[i]) ) {
      cerr << argv[i] << " : cannot open" << endl;
      return -2;
    }

    for ( ; ; ) {
      int id = lex.read_token();
      if ( id == EOF ) {
	break;
      }
      cout << "line: " << setw(4) << lex.cur_line()
	   << ", column: " << setw(3) << lex.first_column()
	   << " - " << setw(3) << lex.cur_column() << "\t";
      switch ( id ) {
      case STR:
	cout << "STR(\"" << lex.cur_string() << "\")";
	break;
      case ERROR:
	cout << "ERROR";
	break;
      default:
	cout << static_cast<char>(id);
	break;
      }
      cout << endl;
    }
  }

  timer.stop();
  USTime time = timer.time();
  cout << "Time: " << time << endl;
  
  return 0;
}
