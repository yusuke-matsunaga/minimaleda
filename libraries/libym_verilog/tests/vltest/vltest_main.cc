
/// @file libym_verilog/tests/vltest/vltest_main.cc
/// @brief Verilog-HDL フロントエンドのテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: vltest_main.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#if HAVE_POPT
#include <popt.h>
#endif

#include <ym_verilog/verilog.h>



BEGIN_NAMESPACE_YM_VERILOG

void
rawlex_mode(const list<string>& filename_list,
	    const char* spath,
	    int watch_line,
	    bool verbose,
	    bool dump_token);
void
lex_mode(const list<string>& filename_list,
	 const char* spath,
	 int watch_line,
	 bool verbose,
	 int loop,
	 bool dump_token);

void
parse_mode(const list<string>& filename_list,
	   const char* spath,
	   int watch_line,
	   bool verbose,
	   bool profile,
	   int loop,
	   bool dump_pt);

void
elaborate_mode(const list<string>& filename_list,
	       bool all_msg,
	       const char* spath,
	       int watch_line,
	       bool verbose,
	       bool profile,
	       int loop,
	       bool dump_vpi);

END_NAMESPACE_YM_VERILOG


int
main(int argc,
     const char** argv)
{
  using namespace nsYm;
  using namespace nsYm::nsVerilog;

  int mode = 4;
  int verbose = 0;
  int dump = 0;
  int bit_expand = 0;
  int all_msg = 0;
  char* spath = NULL;
  int watch_line = 0;
  int loop = 0;
  int use_cpt = false;
  int profile = 0;

#if HAVE_POPT
  // オプション解析用のデータ
  const struct poptOption options[] = {
    // long-option
    // short-option
    // argument type
    // variable address
    // option tag
    // docstr
    // argstr
    { "verbose", 'v', POPT_ARG_NONE, &verbose, 0,
      "enable verbose mode", NULL },

    { "rawlex", '1', POPT_ARG_NONE, NULL, 1,
      "enable rawlex mode", NULL },
    
    { "lex", '2', POPT_ARG_NONE, NULL, 2,
      "enable lex mode", NULL },

    { "yacc", '3', POPT_ARG_NONE, NULL, 3,
      "enable yacc mode", NULL },

    { "elaborate", '4', POPT_ARG_NONE, NULL, 4,
      "enable elaborate mode", NULL },
    
    { "dump", 'd', POPT_ARG_NONE, &dump, 0,
      "set dump-flag", NULL },
    
    { "all-msg", 'a', POPT_ARG_NONE, &all_msg, 0,
      "display all kind of messages", NULL },

    { "search-path", 'p', POPT_ARG_STRING, &spath, 0,
      "set search path", "\"path list\"" },
    
    { "loop", 'l', POPT_ARG_INT, &loop, 0,
      "loop test", NULL },
    
    { "watch-line", 'w', POPT_ARG_INT, &watch_line, 0,
      "enable line watcher", "line number" },

    { "profile", 'q', POPT_ARG_NONE, &profile, 0,
      "show memory profile", NULL },
    
    POPT_AUTOHELP

    { NULL, '\0', 0, NULL, 0, NULL, NULL }
  };

  // オプション解析用のコンテキストを生成する．
  poptContext popt_context = poptGetContext(NULL, argc, argv, options, 0);
  poptSetOtherOptionHelp(popt_context, "[OPTIONS]* <file-name> ...");
  
  // オプション解析を行う．
  for ( ; ; ) {
    int rc = poptGetNextOpt(popt_context);
    if ( rc == -1 ) {
      break;
    }
    if ( rc < -1 ) {
      // エラーが起きた．
      fprintf(stderr, "%s: %s\n",
	      poptBadOption(popt_context, POPT_BADOPTION_NOALIAS),
	      poptStrerror(rc));
      return 1;
    }
    mode = rc;
  }

  // 残りの引数はすべてファイル名と見なす
  list<string> filename_list;
  for ( ; ; ) {
    const char* str = poptGetArg(popt_context);
    if ( str == NULL ) break;
    filename_list.push_back(string(str));
  }

  if ( filename_list.empty() ) {
    poptPrintUsage(popt_context, stderr, 0);
    return 1;
  }
#else
  // オプション解析なし
  list<string> filename_list;
  for (int i = 1; i < argc; ++ i) {
    if ( argv[i][0] == '-' ) {
      if ( strcmp(argv[i], "-1") == 0 ) {
	mode = 1;
	continue;
      }
      if ( strcmp(argv[i], "-2") == 0 ) {
	mode = 2;
	continue;
      }
      if ( strcmp(argv[i], "-3") == 0 ) {
	mode = 3;
	continue;
      }
      if ( strcmp(argv[i], "-4") == 0 ) {
	mode = 4;
	continue;
      }
      if ( strcmp(argv[i], "-d") == 0 ) {
	dump_pt = true;
	continue;
      }
      if ( strcmp(argv[i], "-D") == 0 ) {
	dump_vpi = true;
      }
      if ( strcmp(argv[i], "-v") == 0 ) {
	verbose = true;
      }
    }
    else {
      filename_list.push_back(argv[i]);
    }
  }
#endif

  switch ( mode ) {
  case 1:
    rawlex_mode(filename_list,
		spath,
		watch_line,
		verbose,
		dump);
    break;
    
  case 2:
    lex_mode(filename_list,
	     spath,
	     watch_line,
	     verbose,
	     loop,
	     dump);
    break;
    
  case 3:
    parse_mode(filename_list,
	       spath,
	       watch_line,
	       verbose,
	       profile,
	       loop,
	       dump);
    break;

  case 4:
    elaborate_mode(filename_list,
		   all_msg,
		   spath,
		   watch_line,
		   verbose,
		   profile,
		   loop,
		   dump);
    break;
  }
  return 0;
}
