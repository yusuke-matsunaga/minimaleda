
/// @file libym_npn/tests/TvFuncTestCmd.cc
/// @brief TvFuncTestCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: tvfunc_test.cc 1594 2008-07-18 10:26:12Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"

#if HAVE_POPT
#include <popt.h>
#endif

#include "ym_utils/random.h"
#include "ym_utils/Generator.h"
#include "ym_npn/TvFunc.h"

#include "TvFuncTest.h"


using namespace std;
using namespace nsYm;

bool verbose = false;

BEGIN_NAMESPACE_YM

void
check(size_t ni,
      const vector<int>& vect,
      size_t nc)
{
  TvFuncTest test(cout);

  size_t ni_pow = 1UL << ni;

  for ( ; ; ) {
    test.check_base(ni, vect);
    if ( test.nerr() ) break;

    TvFunc func(ni, vect);

    test.check_base2(func);
    if ( test.nerr() ) break;

    test.check_count_one(func);
    if ( test.nerr() ) break;

    test.check_walsh_0(func);
    if ( test.nerr() ) break;

    for (size_t i = 0; i < ni; ++ i) {
      test.check_walsh_1(func, i);
    }
    if ( test.nerr() ) break;

    for (size_t i = 0; i < ni; ++ i) {
      for (size_t j = 0; j < ni; ++ j) {
	test.check_walsh_2(func, i, j);
      }
    }
    if ( test.nerr() ) break;

    test.check_walsh_01(func);
    if ( test.nerr() ) break;
  
    test.check_walsh_012(func);
    if ( test.nerr() ) break;

    for (size_t i = 0; i < ni; ++ i) {
      test.check_check_sup(func, i);
    }
    if ( test.nerr() ) break;

    for (size_t i = 0; i < ni; ++ i) {
      for (size_t j = 0; j < ni; ++ j) {
	test.check_check_sym(func, i, j);
      }
    }
    if ( test.nerr() ) break;

    if ( nc == 0 ) {
      for (size_t ibits = 0; ibits < ni_pow; ++ ibits) {
	test.check_walsh_w0(func, ibits);
      }
    }
    else {
      size_t ibits0 = (1 << ni) - 1;
    
      // 全ての 0 のパタン
      test.check_walsh_w0(func, 0);
      // 全ての 1 のパタン
      test.check_walsh_w0(func, ibits0);
      // 2つだけ 1 のパタン (1つだけの場合も含む)
      for (size_t i = 0; i < ni; ++ i) {
	size_t ibits1 = 1 << i;
	for (size_t j = 0; j < ni; ++ j) {
	  size_t ibits = ibits1 | (1 << j);
	  test.check_walsh_w0(func, ibits);
	  test.check_walsh_w0(func, ibits ^ ibits0);
	}
      }
      
      for (size_t i = 0; i < nc; ++ i) {
	size_t ibits = random_num() & ((1 << ni) - 1);
	test.check_walsh_w0(func, ibits);
      }
    }
    if ( test.nerr() ) break;

    for (size_t i = 0; i < ni; ++ i) {
      if ( nc == 0 ) {
	for (size_t ibits = 0; ibits < ni_pow; ++ ibits) {
	  test.check_walsh_w1(func, i, ibits);
	}
      }
      else {
	size_t ibits0 = (1 << ni) - 1;
    
	// 全ての 0 のパタン
	test.check_walsh_w1(func, i, 0);
	// 全ての 1 のパタン
	test.check_walsh_w1(func, i, ibits0);
	// 2つだけ 1 のパタン (1つだけの場合も含む)
	for (size_t j = 0; j < ni; ++ j) {
	  size_t ibits1 = 1 << j;
	  for (size_t k = 0; k < ni; ++ k) {
	    size_t ibits = ibits1 | (1 << k);
	    test.check_walsh_w1(func, i, ibits);
	    test.check_walsh_w1(func, i, ibits ^ ibits0);
	  }
	}
      
	for (size_t j = 0; j < nc; ++ j) {
	  size_t ibits = random_num() & ((1 << ni) - 1);
	  test.check_walsh_w1(func, i, ibits);
	}
      }
    }
    if ( test.nerr() ) break;

    break;
  }
}

END_NAMESPACE_YM

void
gen(size_t ni,
    size_t limit)
{
  size_t ni_exp = 1 << ni;
  vector<int> buff(ni_exp);
  for (size_t i = 0; i < ni_exp; ++ i) {
    buff[i] = 0;
  }
  size_t frontier = 0;
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

    check(ni, buff, 0);

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
}

void
rgen(size_t ni,
     size_t num,
     size_t num2)
{
  size_t ni_exp = 1 << ni;
  vector<int> buff(ni_exp);
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

    check(ni, buff, num2);
  }
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
  int ni = 4;
  int rnum = 10000;
  int rseed = 0;
  size_t bnum = 10;

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

    { "ni", 'i', POPT_ARG_INT, &ni, 0,
      "number of inputs", NULL },
    
    { "rnum", 'n', POPT_ARG_INT, &rnum, 0,
      "number of functions to be generated", NULL },

    { "rseed", 's', POPT_ARG_INT, &rseed, 0,
      "random seed", NULL },

    { "bnum", 'b', POPT_ARG_INT, &bnum, 0,
      "number of bit-patterns to be tested", NULL },
    
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
    //check0(ni);

    switch ( mode ) {
    case 1: // gen
      gen(ni, 0);
      break;

    case 2: // rgen
      init_random_seed(rseed);
      rgen(ni, rnum, bnum);
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
