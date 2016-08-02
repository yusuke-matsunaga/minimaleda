// $Id: npn_check3.cc 1594 2008-07-18 10:26:12Z matsunaga $

// NPN パッケージのテスト


#include "ymtools.h"

#if HAVE_POPT
#include <popt.h>
#endif

#include "ym_utils/random.h"
#include "ym_utils/Generator.h"
#include "ym_npn/TvFunc.h"
#include "ym_npn/NpnMgr.h"

#include "ym_utils/StopWatch.h"

//#define DEBUG

using namespace std;
using namespace nsYm;

bool verbose = false;

int check_count = 0;
int perm_count = 0;
int unresolved_count = 0;
size_t mag = 1;

StopWatch sw;

double acum_time = 0.0;
double max_time = 0;

bool
str2vect(size_t ni,
	 const char* buff,
	 vector<int>& v)
{
  size_t ni_pow = 1 << ni;
  v.resize(ni_pow);

  for ( ; *buff == ' '; ++ buff) { }
  for (size_t i = 0; i < ni_pow; ++ i) {
    char c = buff[i];
    if ( c == '1' ) {
      v[i] = 1;
    }
    else if ( c == '0' ) {
      v[i] = 0;
    }
    else {
      cerr << "Illegal char at " << i << " '" << c << "'("
	   << static_cast<int>(c) << ")" << endl;
      return false;
    }
  }
  return true;
}

void
gen(size_t ni,
    size_t limit,
    size_t flow)
{
  hash_set<TvFunc> repfunc_set;

  size_t ni_exp = 1 << ni;
  vector<int> buff(ni_exp);
  for (size_t i = 0; i < ni_exp; ++ i) {
    buff[i] = 0;
  }

  sw.reset();
  size_t frontier = 0;
  size_t num = 0;
  ymulong w2count_total = 0;
  for ( ; ; ) {
    if ( verbose ) {
      if ( buff[frontier] ) {
	for (size_t i = 0; i < ni_exp; ++ i) {
	  cout << buff[i];
	}
	cout << endl;
      }
    }
    if ( buff[frontier] ) {
      ++ frontier;
    }

    TvFunc func(ni, buff);
    NpnMgr mgr;
    NpnMap map;

    sw.start();
    for (size_t i = 0; i < mag; ++ i) {
      mgr.cannonical(func, map);
      if ( i == 0 ) {
	w2count_total += mgr.w2max_count();
      }
    }
    sw.stop();

    TvFunc orgfunc(ni, buff);
    TvFunc repfunc = orgfunc.xform(map);
    if ( repfunc_set.count(repfunc) == 0 ) {
      repfunc_set.insert(repfunc);
    }

    ++ num;

    bool carry = false;
    for (size_t i = 0; i < ni_exp; ++ i) {
      if ( buff[i] == 0 ) {
	buff[i] = 1;
	break;
      }
      buff[i] = 0;
      if ( i == ni_exp - 1 ) {
	carry = true;
      }
    }
    if ( carry ) {
      break;
    }
    if ( limit > 0 && frontier == limit ) {
      break;
    }
  }

  double usec = (sw.time().usr_time_usec() / num) / mag;
  cout << "Result of gen(ni = " << ni
       << ", mag = " << mag << ")"
       << endl
       << "w2max_recur:             "
       << static_cast<double>(w2count_total) / static_cast<double>(num)
       << endl
       << "NPN rep:                 " << repfunc_set.size() << endl
       << "AVE. CPU time(usec):     " << usec << endl;

  for (hash_set<TvFunc>::iterator p = repfunc_set.begin();
       p != repfunc_set.end(); ++ p) {
    cout << ni << " " << *p << endl;
  }
}

void
rgen(size_t ni,
     int rseed,
     size_t num,
     size_t flow)
{
  hash_set<TvFunc> repfunc_set;
  
  size_t ni_exp = 1 << ni;
  vector<int> buff(ni_exp);

  init_random_seed(rseed);

  size_t w2count_total = 0;
  sw.reset();
  for (size_t k = 0; k < num; ++ k) {
    for (size_t i = 0; i < ni_exp; ++ i) {
      buff[i] = random_num() & 1;
    }
    if ( verbose ) {
      for (size_t i = 0; i < ni_exp; ++ i) {
	cout << buff[i];
      }
      cout << endl;
    }

    TvFunc func(ni, buff);
    NpnMgr mgr;
    NpnMap map;
    
    sw.start();
    for (size_t i = 0; i < mag; ++ i) {
      mgr.cannonical(func, map);
      if ( i == 0 ) {
	w2count_total += mgr.w2max_count();
      }
    }
    sw.stop();

    if ( flow & 1024 ) {
      TvFunc repfunc = TvFunc(ni, buff).xform(map);
      if ( repfunc_set.count(repfunc) == 0 ) {
	repfunc_set.insert(repfunc);
      }
    }
  }

  double usec = (sw.time().usr_time_usec() / num) / mag;
  cout << "Result of rgen(ni = " << ni
       << ", rseed = " << rseed
       << ", rnum = " << num
       << ", mag = " << mag << ")"
       << endl
       << "w2max_recur:             "
       << static_cast<double>(w2count_total) / static_cast<double>(num)
       << endl
       << "NPN rep:                 " << repfunc_set.size() << endl
       << "AVE. CPU time(usec):     " << usec << endl;
}

void
rgen_walsh(size_t ni,
	   int rseed,
	   size_t num,
	   size_t flow)
{
  hash_set<TvFunc> repfunc_set;
  
  size_t ni_exp = 1 << ni;
  vector<int> buff(ni_exp);

  init_random_seed(rseed);

  sw.reset();
  for (size_t k = 0; k < num; ++ k) {
    for (size_t i = 0; i < ni_exp; ++ i) {
      buff[i] = random_num() & 1;
    }
    if ( verbose ) {
      for (size_t i = 0; i < ni_exp; ++ i) {
	cout << buff[i];
      }
      cout << endl;
    }

    TvFunc func(ni, buff);
    int w1[kNpnMaxNi];
    int w2[kNpnMaxNi * kNpnMaxNi];
    
    sw.start();
    for (size_t i = 0; i < mag; ++ i) {
      //func.walsh_012(w1, w2);
      func.walsh_01(w1);
    }
    sw.stop();
  }

  double usec = (sw.time().usr_time_usec() / num) / mag;
  cout << "Result of rgen(ni = " << ni
       << ", rseed = " << rseed
       << ", rnum = " << num
       << ", mag = " << mag << ")"
       << endl
       << "AVE. CPU time(usec):     " << usec << endl;
}

void
read_func(const char* filename,
	  size_t flow)
{
  ifstream fs;
  fs.open(filename);
  if ( !fs ) {
    cerr << "Could not open " << filename << endl;
    return;
  }

#define buff_size (1024 * 1024 + 100)

  char buff[buff_size];
  vector<int> vect;
  
  size_t num = 0;
  ymulong w2count_total = 0;
  sw.reset();
  while ( fs.peek() != EOF ) {
    size_t ni;
    fs >> ni;
    fs.getline(buff, buff_size);
    bool stat = str2vect(ni, buff, vect);
    if ( !stat ) {
      cerr << "Illegal data" << endl;
      return;
    }

    TvFunc func(ni, vect);
    NpnMgr mgr;
    NpnMap map;
    
    ++ num;
    sw.start();
    for (size_t i = 0; i < mag; ++ i) {
      mgr.cannonical(func, map);
      if ( i == 0 ) {
	w2count_total += mgr.w2max_count();
      }
    }
    sw.stop();
  }

  double usec = sw.time().usr_time_usec() / mag;
  cout << endl
       << "w2max_recur:             "
       << static_cast<double>(w2count_total) / static_cast<double>(num)
       << endl
       << "AVE. CPU time(usec):     " << usec << endl;
}

void
verify(size_t ni,
       size_t rseed,
       size_t num,
       size_t flow)
{
  init_random_seed(rseed);

  size_t ni_exp = 1 << ni;
  vector<int> buff(ni_exp);
  size_t c = 0;
  for (size_t k = 0; k < num; ++ k) {
    for (size_t i = 0; i < ni_exp; ++ i) {
      buff[i] = random_num() & 1;
    }
    if ( verbose ) {
      for (size_t i = 0; i < ni_exp; ++ i) {
	cout << buff[i];
      }
      cout << endl;
    }

    TvFunc orig_func(ni, buff);
    NpnMgr mgr;
    NpnMap map0;
    sw.start();
    mgr.cannonical(orig_func, map0);
    sw.stop();
    ++ c;

    TvFunc rep_func = orig_func.xform(map0);

    // すべての極性・順列組合わせのバリエーションをためして
    // おなじ代表関数になるかチェックする．
    size_t ni_pow = 2 << ni;
    for (size_t bits = 0; bits < ni_pow; ++ bits) {
      tPol opol = (bits & 1) ? kPolNega : kPolPosi;
      size_t ibits = bits >> 1;

      NpnMap pol_map(ni, opol);
      for (size_t i = 0; i < ni; ++ i) {
	if ( ibits & (1 << i) ) {
	  pol_map.set(i, i, kPolNega);
	}
	else {
	  pol_map.set(i, i, kPolPosi);
	}
      }

      PermGen pg(ni, ni);
      for (PermGen::iterator p = pg.begin(); !p.is_end(); ++ p) {
	NpnMap perm_map(ni);
	for (size_t i = 0; i < ni; ++ i) {
	  size_t j = p(i);
	  perm_map.set(i, j, kPolPosi);
	}

	NpnMap tmpmap = pol_map * perm_map;
	TvFunc tmp_func = orig_func.xform(tmpmap);
	NpnMgr mgr;
	NpnMap repmap;
	sw.start();
	mgr.cannonical(tmp_func, repmap);
	sw.stop();
	++ c;

	TvFunc rep_func2 = tmp_func.xform(repmap);

	if ( rep_func2 != rep_func ) {
	  cout << "Error" << endl
	       << " orig_func: " << orig_func << endl
	       << " rep_func:  " << rep_func << endl
	       << endl
	       << " tmp_map: " << tmpmap << endl
	       << " tmp_func:  " << tmp_func << endl
	       << " rep_func2: " << rep_func2 << endl;
	  exit(1);
	}
      }
    }
  }
  double usec = (sw.time().usr_time() / c);
  cout << "Result of verify(ni = " << ni
       << ", rseed = " << rseed
       << ", rnum = " << num
       << endl
#if 0
       << "Step1  undetermined num: " << ph1unum << endl
       << "       phase undet num:  " << ph1vnum << endl
       << "       permutations:     " << ph1pnum << endl
       << "Step2  undetermined num: " << ph2unum << endl
       << "       phase undet num:  " << ph2vnum << endl
       << "       permutations:     " << ph2pnum << endl
       << "Step3  undetermined num: " << ph3unum << endl
       << "       phase undet num:  " << ph3vnum << endl
       << "       permutations:     " << ph3pnum << endl
       << "Step4  undetermined num: " << ph4unum << endl
       << "       permutations:     " << ph4pnum << endl
#endif
       << "CPU time:                " << usec << endl;
}

void
usage(const char* argv0)
{
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
  int step1mode = 1;
  int step2mode = 1;
  int step3mode = 1;
  int step4mode = 1;
  int table = 0;
  int finish = 0;
  int ni = 4;
  int rnum = 10000;
  int rseed = 0;
  
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

    { "gen", 'g', POPT_ARG_NONE, NULL, 1,
      "generate all NPN equivalent functions mode", NULL },
    
    { "rgen", 'r', POPT_ARG_NONE, NULL, 2,
      "randomly generate NPN equivalent functions mode", NULL },
    
    { "verify", 'c', POPT_ARG_NONE, NULL, 3,
      "verify mode", NULL },

    { "func", 'f', POPT_ARG_NONE, NULL, 4,
      "read function mode", NULL },
    
    { "rgen_walsh", 'w', POPT_ARG_NONE, NULL, 5,
      "randomly generate NPN equivalent functions mode", NULL },
    
    { "step1", '1', POPT_ARG_INT, &step1mode, 0,
      "step1 submode", "(0/1)" },

    { "step2", '2', POPT_ARG_INT, &step2mode, 0,
      "step2 submode",  "(0/1)" },

    { "step3", '3', POPT_ARG_INT, &step3mode, 0,
      "step3 submode", "(0/1)" },

    { "step4", '4', POPT_ARG_INT, &step4mode, 0,
      "step4 submode", "(0/1)" },

    { "table", 't', POPT_ARG_NONE, &table, 0,
      "table look-up mode", NULL },
    
    { "finish", 'f', POPT_ARG_INT, &finish, 0,
      "finish", NULL },
    
    { "ni", 'i', POPT_ARG_INT, &ni, 0,
      "number of inputs", NULL },
    
    { "rnum", 'n', POPT_ARG_INT, &rnum, 0,
      "number of functions to be generated", NULL },

    { "rseed", 's', POPT_ARG_INT, &rseed, 0,
      "random seed", NULL },

    { "mag", 'm', POPT_ARG_INT, &mag, 0,
      "magnitude", NULL },

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

  const char* argname = poptGetArg(popt_context);
  if ( (mode == 4 && argname == NULL) ||
       (mode != 4 && argname != NULL) ) {
    usage(argv[0]);
    exit(1);
  }

  int flow = 0;
  if ( step1mode ) {
    flow |= 1;
  }
  if ( step2mode ) {
    flow |= 2;
  }
  if ( step3mode ) {
    flow |= 4;
  }
  if ( step4mode ) {
    flow |= 8;
  }
  if ( table ) {
    flow |= 0x10000000;
  }
  if ( finish ) {
    flow |= 1024;
  }
  try {
    switch ( mode ) {
    case 1: // gen
      gen(ni, 0, flow);
      break;

    case 2: // rgen
      init_random_seed(rseed);
      rgen(ni, rseed, rnum, flow);
      break;

    case 3: // verify
      verify(ni, rseed, rnum, flow);
      break;

    case 4: // function
      read_func(argname, flow);
      break;

    case 5: // rgen_walsh
      init_random_seed(rseed);
      rgen_walsh(ni, rseed, rnum, flow);
      break;

    default:
      usage(argv[0]);
      exit(1);
    }
  }

  catch ( nsYm::AssertError x ) {
    cerr << x << endl;
  }

#else

  int base = 1;
  size_t flow_end = 1024;
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

  try {
    if ( strcmp(argv[base], "signature") == 0 ) {
      if ( argc - base != 2 ) {
	usage(argv[0]);
	return 2;
      }
      
      size_t ni = atoi(argv[base + 1]);
      NpnSigMgr npnsigmgr;
      size_t ni_pow = 1 << ni;
      char buff[256];
      while ( cin.getline(buff, sizeof(buff)) ) {
	vector<int> tmp(ni_pow);
	str2vect(ni, buff, tmp);
	reg(npnsigmgr, ni, tmp, 0);
      }
    }
    else if ( strcmp(argv[base], "check") == 0 ) {
      if ( argc - base != 4 ) {
	usage(argv[0]);
	return 2;
      }
      
      size_t ni = atoi(argv[base + 1]);
      NpnSigMgr npnsigmgr;
      vector<int> tmp(1 << ni);
      str2vect(ni, argv[base + 2], tmp);
      reg(npnsigmgr, ni, tmp, 0);
      str2vect(ni, argv[base + 3], tmp);
      reg(npnsigmgr, ni, tmp, 0);
    }
    else if ( strcmp(argv[base], "check1") == 0 ) {
      if ( argc - base != 4 ) {
	usage(argv[0]);
	return 2;
      }
      
      size_t ni = atoi(argv[base + 1]);
      NpnSigMgr npnsigmgr;
      vector<int> tmp(1 << ni);
      str2vect(ni, argv[base + 2], tmp);
      reg(npnsigmgr, ni, tmp, 1);
      str2vect(ni, argv[base + 3], tmp);
      reg(npnsigmgr, ni, tmp, 1);
    }
    else if ( strcmp(argv[base], "gen") == 0 ) {
      if ( argc - base != 3 ) {
	usage(argv[0]);
	return 2;
      }
      
      size_t ni = atoi(argv[base + 1]);
      size_t flow = atoi(argv[base + 2]) | flow_end;
      gen(ni, 0, flow, 2);
    }
    else if ( strcmp(argv[base], "ngen") == 0 ) {
      if ( argc - base != 3 ) {
	usage(argv[0]);
	return 2;
      }
      
      size_t ni = atoi(argv[base + 1]);
      size_t n = atoi(argv[base + 2]);
      gen(ni, n, 0, 2);
    }
    else if ( strcmp(argv[base], "rgen") == 0 ) {
      if ( argc - base != 5 ) {
	usage(argv[0]);
	return 2;
      }
      
      size_t ni = atoi(argv[base + 1]);
      size_t flow = atoi(argv[base + 2]) | flow_end;
      size_t num = atoi(argv[base + 3]);
      int seed = atoi(argv[base + 4]);
      init_random_seed(seed);
      rgen(ni, num, flow, 2);
    }
    else if ( strcmp(argv[base], "verify") == 0 ) {
      if ( argc - base != 5 ) {
	usage(argv[0]);
	return 2;
      }
      
      size_t ni = atoi(argv[base + 1]);
      size_t flow = atoi(argv[base + 2]) | flow_end;
      size_t num = atoi(argv[base + 3]);
      int seed = atoi(argv[base + 4]);
      init_random_seed(seed);
      verify(ni, num, flow, 2);
    }
    else {
      usage(argv[0]);
      return 2;
    }
  }
  catch ( nsYm::AssertError x ) {
    cerr << x;
  }
#endif

  return 0;
}
