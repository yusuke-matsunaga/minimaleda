// $Id: tvfunc_test2.cc 1594 2008-07-18 10:26:12Z matsunaga $

// TvFunc パッケージのテスト

#include "ymtools.h"

#if HAVE_POPT
#include <popt.h>
#endif

#include "ym_utils/random.h"
#include "ym_utils/Generator.h"
#include "ym_utils/StopWatch.h"
#include "ym_npn/TvFunc.h"

using namespace std;
using namespace nsYm;

void
walsh_0(const TvFunc& func,
	int mag)
{
  StopWatch sw;

  sw.start();
  for (int i = 0; i < mag; ++ i) {
    func.walsh_0();
  }
  sw.stop();

  double usec = sw.time().usr_time() / mag;
  cout << "Result of walsh_0(ni = " << func.ni()
       << ", mag = " << mag << ")"
       << endl
       << "CPU time(avr.): " << usec << endl;
}

void
walsh_1(const TvFunc& func,
	int mag)
{
  StopWatch sw;

  size_t ni = func.ni();
  sw.start();
  for (size_t j = 0; j < ni; ++ j) {
    for (int i = 0; i < mag; ++ i) {
      func.walsh_1(j);
    }
  }
  sw.stop();

  double usec = sw.time().usr_time() / mag / ni;
  cout << "Result of walsh_1(ni = " << func.ni()
       << ", mag = " << mag << ")"
       << endl
       << "CPU time(avr.): " << usec << endl;
}

void
walsh_2(const TvFunc& func,
	int mag)
{
  StopWatch sw;

  size_t ni = func.ni();
  sw.start();
  for (size_t j = 0; j < ni; ++ j) {
    for (size_t k = 0; k < ni; ++ k) {
      for (int i = 0; i < mag; ++ i) {
	func.walsh_2(j, k);
      }
    }
  }
  sw.stop();

  double usec = sw.time().usr_time() / mag / (ni * ni);
  cout << "Result of walsh_0(ni = " << func.ni()
       << ", mag = " << mag << ")"
       << endl
       << "CPU time(avr.): " << usec << endl;
}

void
walsh_01(const TvFunc& func,
	 int mag)
{
  StopWatch sw;

  int vec1[kNpnMaxNi];
  
  sw.start();
  for (int i = 0; i < mag; ++ i) {
    func.walsh_01(vec1);
  }
  sw.stop();

  double usec = sw.time().usr_time() / mag;
  cout << "Result of walsh_01(ni = " << func.ni()
       << ", mag = " << mag << ")"
       << endl
       << "CPU time(avr.): " << usec << endl;
}

void
walsh_012(const TvFunc& func,
	  int mag)
{
  StopWatch sw;

  int vec1[kNpnMaxNi];
  int vec2[kNpnMaxNi * kNpnMaxNi];

  sw.start();
  for (int i = 0; i < mag; ++ i) {
    func.walsh_012(vec1, vec2);
  }
  sw.stop();

  double usec = sw.time().usr_time() / mag;
  cout << "Result of walsh_012(ni = " << func.ni()
       << ", mag = " << mag << ")"
       << endl
       << "CPU time(avr.): " << usec << endl;
}

void
usage(const char* argv0)
{
  // 嘘！
  cerr << "USAGE : " << argv0 << " check <# of inputs> <vect1> <vect2>"
       << endl
       << "        " << argv0 << " gen <# of inputs>" << endl
       << "        " << argv0 << " rgen <# of inputs> <# of functions>"
       << " <random seed>" << endl
       << "        " << argv0 << " signature <# of inputs>" << endl;
}

int
main(int argc,
     const char** argv)
{
#if HAVE_POPT
  int mode = 0;
  int ni = 4;
  int mag = 1;

  // オプション解析用のデータ
  const struct poptOption options[] = {
    // long-option
    // short-option
    // argument type
    // variable address
    // option tag
    // docstr
    // argstr
    { "ni", 'i', POPT_ARG_INT, &ni, 0,
      "number of inputs", NULL },

    { "mag", 'm', POPT_ARG_INT, &mag, 0,
      "magnify facctor", NULL },

    { "w0", '0', POPT_ARG_NONE, NULL, 1,
      "walsh_0 mode", NULL },
    
    { "w1", '1', POPT_ARG_NONE, NULL, 2,
      "walsh_1 mode", NULL },

    { "w2", '2', POPT_ARG_NONE, NULL, 3,
      "walsh_2 mode", NULL },
    
    { "w01", '3', POPT_ARG_NONE, NULL, 4,
      "walsh_01 mode", NULL },

    { "w012", '4', POPT_ARG_NONE, NULL, 5,
      "walsh_012 mode", NULL },

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

  if ( poptGetArg(popt_context) ) {
    usage(argv[0]);
    exit(1);
  }

  try {
    TvFunc func(ni);

    switch ( mode ) {
    case 1: // walsh_0;
      walsh_0(func, mag);
      break;

    case 2: // walsh_1;
      walsh_1(func, mag);
      break;

    case 3: // walsh_2;
      walsh_2(func, mag);
      break;

    case 4: // walsh_01;
      walsh_01(func, mag);
      break;

    case 5: // walsh_012;
      walsh_012(func, mag);
      break;

    default:
      usage(argv[0]);
      exit(1);
    }
  }

  catch ( nsYm::AssertError x ) {
    cerr << x;
  }

#else

  int base = 1;
  for ( ; base < argc; ++ base) {
    if ( argv[base][0] != '-' ) {
      break;
    }
    if ( strcmp(argv[base], "--verbose") == 0 ) {
      verbose = true;
    }
    else if ( strcmp(argv[base], "--dump") == 0 ) {
      dump_flag = true;
    }
    else if ( strcmp(argv[base], "--signature") == 0 ) {
      flow_end = 0;
    }
  }
  if ( argc - base <= 1 ) {
    usage(argv[0]);
    return 2;
  }

#endif

  return 0;
}
