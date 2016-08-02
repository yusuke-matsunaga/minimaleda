// $Source: /export/home/project/synth/cvsrepos/ymtools/src/libym_cell/tests/yacc_test.cc,v $
// $Author: matsunaga $
// $Revision: 1.1.1.1 $
// $Date: 2005/12/11 17:07:26 $

// Yacc のテストプログラム


#include "ym_utils/StopWatch.h"
#include "../mislib/Lex.h"
#include "../mislib/Pt.h"
#include "../utils/Msg.h"

#if HAVE_POPT
#include <popt.h>
#endif


BEGIN_NAMESPACE_YM_CELL

extern bool read_mislib(PtMgr* ptmgr, Lex* lex, const string& filename);

END_NAMESPACE_YM_CELL


int
main(int argc, const char** argv)
{
  using namespace nsYm;
  using namespace nsYm::nsCell;

  int verbose = 0;
  int all_msg = 0;

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

    { "all-msg", 'a', POPT_ARG_NONE, &all_msg, 0,
      "display all kind of messages", NULL },

    POPT_AUTOHELP

    { NULL, '\0', 0, NULL, 0, NULL, NULL }
  };

  // オプション解析用のコンテキストを生成する．
  poptContext popt_context = poptGetContext(NULL, argc, argv, options, 0);
  poptSetOtherOptionHelp(popt_context, "[OPTIONS]* <file-name> ...");
  
  // オプション解析を行う．
  int rc = poptGetNextOpt(popt_context);
  if ( rc < -1 ) {
    // エラーが起きた．
    fprintf(stderr, "%s: %s\n",
	    poptBadOption(popt_context, POPT_BADOPTION_NOALIAS),
	    poptStrerror(rc));
    return 1;
  }

  // 残りの引数を得る．
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
    if ( argv[i][0] == '-' ) continue;
    filename_list.push_back(argv[i]);
  }
#endif

  PtMgr* ptmgr = new PtMgr();
  Lex* lex = new Lex();

  tMsgMask msgmask = kMsgMaskError | kMsgMaskWarning;
  if ( all_msg ) {
    msgmask = kMsgMaskAll;
  }

  SimpleMsgHandler* tmh = new SimpleMsgHandler(msgmask);
  MsgMgr::the_mgr().reg_handler(tmh);

  try {
    StopWatch timer;
    timer.start();
    for (list<string>::const_iterator p = filename_list.begin();
	 p != filename_list.end(); ++ p) {
      string name = *p;
      if ( verbose ) {
	cerr << "Reading " << name;
	cerr.flush();
      }
      read_mislib(ptmgr, lex, name);
      if ( verbose ) {
	cerr << " end" << endl;
      }
    }
    timer.stop();
    USTime time = timer.time();
    if ( verbose ) {
      cerr << "Parsing time: " << time << endl;
    }

    const list<PtGate*>& gate_list = ptmgr->gate_list();
    for (list<PtGate*>::const_iterator p = gate_list.begin();
	 p != gate_list.end(); ++ p) {
      PtGate* gate = *p;
      gate->dump(cout);
    }

    int en = MsgMgr::the_mgr().error_num();
    switch ( en ) {
    case 0:
      cerr << "No errors" << endl;
      break;
    case 1:
      cerr << "Total 1 error" << endl;
      break;
      
    default:
      cerr << "Total " << en << " errors" << endl;
      break;
    }
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
  return 0;
}
