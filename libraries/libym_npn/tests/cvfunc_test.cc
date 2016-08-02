// $Id: cvfunc_test.cc 1594 2008-07-18 10:26:12Z matsunaga $

// CvFunc パッケージのテスト

#include "ymtools.h"

#include "ym_utils/random.h"
#include "ym_utils/Generator.h"
#include "ym_npn/CvFunc.h"

#include "ym_utils/StopWatch.h"


#if HAVE_POPT
#include <popt.h>
#endif

using namespace std;
using namespace nsYm;

bool verbose = false;
int nerr;

#if 0
BEGIN_NONAMESPACE

// ベクタの内容を出力する
void
dump_vect(ostream& s,
	  size_t ni,
	  const vector<int>& vect)
{
  size_t ni_pow = 1 << ni;
  for (size_t i = 0; i < ni_pow; ++ i) {
    if ( vect[i] ) {
      s << "1";
    }
    else {
      s << "0";
    }
  }
  s << endl;
}

// vect 中の 1 の数を数える．
size_t
count_onebits(size_t vect)
{
  size_t ans = 0;
  for (size_t i = 0; i < 32; ++ i) {
    if ( vect & 1 ) {
      ++ ans;
    }
    vect >>= 1;
  }
  return ans;
}

END_NONAMESPACE

BEGIN_NAMESPACE_YM_NPN

// 個別の関数とは無関係のテスト
void
check0(size_t ni)
{
  size_t ni_pow = 1 << ni;
  
  TvFunc zero = TvFunc::const_zero(ni);
  for (size_t i = 0; i < ni_pow; ++ i) {
    if ( zero.value(i) != 0 ) {
      cout << "Error: TvFunc::const_zero(" << ni << ")" << endl
	   << "  func: " << zero << endl;
      ++ nerr;
      break;
    }
  }
  
  TvFunc one = TvFunc::const_one(ni);
  for (size_t i = 0; i < ni_pow; ++ i) {
    if ( one.value(i) != 1 ) {
      cout << "Error: TvFunc::const_one(" << ni << ")" << endl
	   << "  func: " << one << endl;
      ++ nerr;
      break;
    }
  }
  
  for (size_t i = 0; i < ni; ++ i) {
    TvFunc pl = TvFunc::posi_literal(ni, i);
    bool error = false;
    for (size_t j = 0; j < ni_pow; ++ j) {
      if ( j & (1 << i) ) {
	if ( pl.value(j) != 1 ) {
	  error = true;
	  break;
	}
      }
      else {
	if ( pl.value(j) != 0 ) {
	  error = true;
	  break;
	}
      }
    }
    if ( error ) {
      cout << "Error: TvFunc::posi_literal(" << ni << ", "
	   << i << ")" << endl
	   << "  func:  " << pl << endl;
      ++ nerr;
    }
  }

  for (size_t i = 0; i < ni; ++ i) {
    TvFunc pl = TvFunc::nega_literal(ni, i);
    bool error = false;
    for (size_t j = 0; j < ni_pow; ++ j) {
      if ( j & (1 << i) ) {
	if ( pl.value(j) != 0 ) {
	  error = true;
	  break;
	}
      }
      else {
	if ( pl.value(j) != 1 ) {
	  error = true;
	  break;
	}
      }
    }
    if ( error ) {
      cout << "Error: TvFunc::nega_literal(" << ni << ", "
	   << i << ")" << endl
	   << "  func:  " << pl << endl;
      ++ nerr;
    }
  }
}

void
check_sub(const TvFunc& func,
	  size_t ibits)
{
  size_t ni = func.ni();
  size_t ni_pow = 1 << ni;

  for (size_t w = 0; w <= ni; ++ w) {
    int w0_p = func.walsh_w0(w, kPolPosi, ibits);
    int w0_n = func.walsh_w0(w, kPolNega, ibits);

    int ans1 = 0;
    for (size_t i = 0; i < ni_pow; ++ i) {
      if ( count_onebits(i) == w ) {
	if ( func.value(i ^ ibits) ) {
	  -- ans1;
	}
	else {
	  ++ ans1;
	}
      }
    }
    if ( w0_p != ans1 ) {
      cout << "Error: TvFunc::walsh_w0()" << endl
	   << "  func: " << func << endl
	   << "  func.walsh_w0(" << w << ", kPolPosi, ";
      for (size_t i = 0; i < ni; ++ i) {
	if ( ibits & (1 << (ni - 1 - i)) ) {
	  cout << "1";
	}
	else {
	  cout << "0";
	}
      }
      cout << ") = " << w0_p << endl
	   << "  true answer = " << ans1 << endl;
      ++ nerr;
    }
    if ( w0_n != -ans1 ) {
      cout << "Error: TvFunc::walsh_w0()" << endl
	   << "  func: " << func << endl
	   << "  func.walsh_w0(" << w << ", kPolNega, ";
      for (size_t i = 0; i < ni; ++ i) {
	if ( ibits & (1 << (ni - 1 - i)) ) {
	  cout << "1";
	}
	else {
	  cout << "0";
	}
      }
      cout << ") = " << w0_n << endl
	   << "  true answer = " << -ans1 << endl;
      ++ nerr;
    }

    for (size_t i = 0; i < ni; ++ i) {
      int w1_p = func.walsh_w1(i, w, kPolPosi, ibits);
      int w1_n = func.walsh_w1(i, w, kPolNega, ibits);
      int ans1 = 0;
      size_t check = 1 << i;
      for (size_t b = 0; b < ni_pow; ++ b) {
	if ( count_onebits(b) == w ) {
	  if ( b & check ) {
	    if ( func.value(b ^ ibits) ) {
	      ++ ans1;
	    }
	    else {
	      -- ans1;
	    }
	  }
	  else {
	    if ( func.value(b ^ ibits) ) {
	      -- ans1;
	    }
	    else {
	      ++ ans1;
	    }
	  }
	}
      }
      if ( w1_p != ans1 ) {
	cout << "Error: TvFunc::walsh_w1()" << endl
	     << "  func: " << func << endl
	     << "  func.walsh_w1(" << w << ", " << i << ", kPolPosi, ";
	for (size_t j = 0; j < ni; ++ j) {
	  if ( ibits & (1 << (ni - 1 - j)) ) {
	    cout << "1";
	  }
	  else {
	    cout << "0";
	  }
	}
	cout << ") = " << w1_p << endl
	     << "  true answer = " << ans1 << endl;
	++ nerr;
      }
      if ( w1_n != -ans1 ) {
	cout << "Error: TvFunc::walsh_w1()" << endl
	     << "  func: " << func << endl
	     << "  func.walsh_w1(" << w << ", " << i << ", kPolNega, ";
	for (size_t j = 0; j < ni; ++ j) {
	  if ( ibits & (1 << (ni - 1 - j)) ) {
	    cout << "1";
	  }
	  else {
	    cout << "0";
	  }
	}
	cout << ") = " << w1_n << endl
	     << "  true answer = " << -ans1 << endl;
	++ nerr;
      }
    }
  }
}

// 基本的なテスト
void
check_base(size_t ni,
	   const vector<int>& vect)
{
  TvFunc func(ni, vect);

  if ( func.ni() != ni ) {
    cout << "Error: TvFunc::ni()" << endl
	 << "  func: " << func << endl
	 << "  vect[]: ";
    dump_vect(cout, ni, vect);
    cout << "  func.ni() = " << func.ni() << endl
	 << "  ni = " << ni << endl;
    ++ nerr;
  }

  size_t ni_pow = 1 << ni;
  for (size_t i = 0; i < ni_pow; ++ i) {
    if ( func.value(i) != vect[i] ) {
      cout << "Error: TvFunc::value(size_t pos)" << endl
	   << "  func: " << func << endl
	   << "  vect[]: ";
      dump_vect(cout, ni, vect);
      cout << "  func.value(" << i << ") = " << func.value(i) << endl
	   << "  vect[" << i << "] = " << vect[i] << endl;
      ++ nerr;
    }
  }

  TvFunc func2(func);
  for (size_t i = 0; i < ni_pow; ++ i) {
    if ( func2.value(i) != vect[i] ) {
      cout << "Error: TvFunc::TvFunc(const TvFunc& src)" << endl
	   << "  func:  " << func << endl
	   << "  func2: " << func2 << endl;
      ++ nerr;
    }
  }

  TvFunc func3;
  func3 = func;
  for (size_t i = 0; i < ni_pow; ++ i) {
    if ( func3.value(i) != vect[i] ) {
      cout << "Error: operator=(const TvFunc& src)" << endl
	   << "  func:  " << func << endl
	   << "  func3: " << func3 << endl;
      ++ nerr;
    }
  }

  func2.negate();
  for (size_t i = 0; i < ni_pow; ++ i) {
    if ( func2.value(i) != (1 - vect[i]) ) {
      cout << "Error: TvFunc::negate()" << endl
	   << "  func:  " << func << endl
	   << "  func2: " << func2 << endl;
      ++ nerr;
    }
  }
}

// count_one/count_zero のテスト
void
check_count_one(const TvFunc& func)
{
  size_t ni = func.ni();
  size_t ni_pow = 1 << ni;

  int c0 = func.count_zero();
  int c0_true = 0;
  for (size_t i = 0; i < ni_pow; ++ i) {
    if ( func.value(i) == 0 ) {
      ++ c0_true;
    }
  }
  if ( c0 != c0_true ) {
    cout << "Error: TvFunc::count_zero()" << endl
	 << "  func: " << func << endl
	 << "  func.count_zero() = " << c0 << endl
	 << "  true c0 = " << c0_true << endl;
    ++ nerr;
  }

  int c1 = func.count_one();
  int c1_true = 0;
  for (size_t i = 0; i < ni_pow; ++ i) {
    if ( func.value(i) == 1 ) {
      ++ c1_true;
    }
  }
  if ( c1 != c1_true ) {
    cout << "Error: TvFunc::count_one()" << endl
	 << "  func: " << func << endl
	 << "  func.count_one() = " << c1 << endl
	 << "  true c1 = " << c1_true << endl;
    ++ nerr;
  }
}

// walsh_0 のテスト
void
check_walsh_0(const TvFunc& func)
{
  size_t ni = func.ni();
  size_t ni_pow = 1 << ni;

  int w0 = func.walsh_0();
  int w0_true = 0;
  for (size_t i = 0; i < ni_pow; ++ i) {
    if ( func.value(i) == 0 ) {
      ++ w0_true;
    }
    else {
      -- w0_true;
    }
  }
  if ( w0 != w0_true ) {
    cout << "Error: TvFunc::walsh_0()" << endl
	 << "  func: " << func << endl
	 << "  func.walsh_0() = " << w0 << endl
	 << "  true w0 = " << w0_true << endl;
    ++ nerr;
  }
}

// walsh_1 のテスト
void
check_walsh_1(const TvFunc& func)
{
  size_t ni = func.ni();
  size_t ni_pow = 1 << ni;

  for (size_t i = 0; i < ni; ++ i) {
    int w1 = func.walsh_1(i);
    int w1_true = 0;
    for (size_t j = 0; j < ni_pow; ++ j) {
      if ( j & (1 << i) ) {
	if ( func.value(j) ) {
	  ++ w1_true;
	}
	else {
	  -- w1_true;
	}
      }
      else {
	if ( func.value(j) ) {
	  -- w1_true;
	}
	else {
	  ++ w1_true;
	}
      }
    }
    if ( w1 != w1_true ) {
      cout << "Error: TvFunc::walsh_1(size_t pos)" << endl
	   << "  func: " << func << endl
	   << "  func.walsh_1(" << i << ") = " << w1 << endl
	   << "  true w1 = " << w1_true << endl;
      ++ nerr;
    }
  }
}

// walsh_2 のテスト
void
check_walsh_2(const TvFunc& func)
{
  size_t ni = func.ni();
  size_t ni_pow = 1 << ni;

  for (size_t i = 0; i < ni; ++ i) {
    for (size_t j = 0; j < ni; ++ j) {
      if ( i == j ) continue;

      int w2 = func.walsh_2(i, j);

      int w2_true = 0;
      for (size_t k = 0; k < ni_pow; ++ k) {
	int flag = 0;
	if ( k & (1 << i) ) {
	  flag ^= 1;
	}
	if ( k & (1 << j) ) {
	  flag ^= 1;
	}
	if ( flag ) {
	  if ( func.value(k) ) {
	    ++ w2_true;
	  }
	  else {
	    -- w2_true;
	  }
	}
	else {
	  if ( func.value(k) ) {
	    -- w2_true;
	  }
	  else {
	    ++ w2_true;
	  }
	}
      }

      if ( w2 != w2_true ) {
	cout << "Error: TvFunc::walsh_2(size_t i, size_t j)" << endl
	     << "  func: " << func << endl
	     << "  func.walsh_2(" << i << ", " << j << ") = " << w2 << endl
	     << "  true w2 = " << w2_true << endl;
	++ nerr;
      }
    }
  }
}

// walsh_01 のテスト
void
check_walsh_01(const TvFunc& func)
{
  size_t ni = func.ni();
  size_t ni_pow = 1 << ni;

  int* w1_vec = new int[ni];
  int w0 = func.walsh_01(w1_vec);

  int w0_true = 0;
  for (size_t i = 0; i < ni_pow; ++ i) {
    if ( func.value(i) == 0 ) {
      ++ w0_true;
    }
    else {
      -- w0_true;
    }
  }
  if ( w0 != w0 ) {
    cout << "Error: TvFunc::walsh_01()" << endl
	 << "  func: " << func << endl
	 << "  func.walsh_01()'s w0 = " << w0 << endl
	 << "  true w0 = " << w0 << endl;
    ++ nerr;
  }

  for (size_t i = 0; i < ni; ++ i) {
    int w1_true = 0;
    for (size_t j = 0; j < ni_pow; ++ j) {
      if ( j & (1 << i) ) {
	if ( func.value(j) ) {
	  ++ w1_true;
	}
	else {
	  -- w1_true;
	}
      }
      else {
	if ( func.value(j) ) {
	  -- w1_true;
	}
	else {
	  ++ w1_true;
	}
      }
    }
    if ( w1_vec[i] != w1_true ) {
      cout << "Error: TvFunc::walsh_01()" << endl
	   << "  func: " << func << endl
	   << "  func.walsh_01()'s w1[" << i << "] = " << w1_vec[i] << endl
	   << "  true w1 = " << w1_true << endl;
      ++ nerr;
    }
  }
  delete [] w1_vec;
}

// check_sup のテスト
void
check_check_sup(const TvFunc& func)
{
  size_t ni = func.ni();
  size_t ni_pow = 1 << ni;

  for (size_t i = 0; i < ni; ++ i) {
    bool stat = func.check_sup(i);
    size_t check = 1 << i;
    bool stat_true = false;
    for (size_t j = 0; j < ni_pow; ++ j) {
      if ( j & check ) {
	if ( func.value(j) != func.value(j ^ check) ) {
	  stat_true = true;
	  break;
	}
      }
    }
    if ( stat != stat_true ) {
      cout << "Error: TvFunc::check_sup()" << endl
	   << "  func: " << func << endl
	   << "  func.check_sup(" << i << ") = " << stat << endl
	   << "  true answer = " << stat_true << endl;
      ++ nerr;
    }
  }
}

// check_sym のテスト
void
check_check_sym(const TvFunc& func)
{
  size_t ni = func.ni();
  size_t ni_pow = 1 << ni;

  for (size_t i = 0; i < ni; ++ i) {
    for (size_t j = 0; j < ni; ++ j) {
      if ( i == j ) continue;
      bool stat_p = func.check_sym(i, j, kPolPosi);
      bool stat_n = func.check_sym(i, j, kPolNega);
      size_t check1 = 1 << i;
      size_t check2 = 1 << j;
      size_t mask_00 = 0;
      size_t mask_01 = check1;
      size_t mask_11 = check1 | check2;
      bool stat_p_true = true;
      bool stat_n_true = true;
      for (size_t k = 0; k < ni_pow; ++ k) {
	if ( (k & mask_11) == mask_01 ) {
	  if ( func.value(k) != func.value(k ^ mask_11) ) {
	    stat_p_true = false;
	    break;
	  }
	}
      }
      for (size_t k = 0; k < ni_pow; ++ k) {
	if ( (k & mask_11) == mask_00 ) {
	  if ( func.value(k) != func.value(k ^ mask_11) ) {
	    stat_n_true = false;
	    break;
	  }
	}
      }
      if ( stat_p != stat_p_true ) {
	cout << "Error: TvFunc::check_sym()" << endl
	     << "  func: " << func << endl
	     << "  func.check_sym(" << i << ", " << j << ", kPolPosi) = "
	     << stat_p << endl
	     << "  true answer = " << stat_p_true << endl;
	++ nerr;
      }
      if ( stat_n != stat_n_true ) {
	cout << "Error: TvFunc::check_sym()" << endl
	     << "  func: " << func << endl
	     << "  func.check_sym(" << i << ", " << j << ", kPolNega) = "
	     << stat_n << endl
	     << "  true answer = " << stat_n_true << endl;
	++ nerr;
      }
    }
  }
}

// walsh_w0/walsh_w1 のテスト
void
check_walsh_w0(const TvFunc& func,
	       size_t nc)
{
  size_t ni = func.ni();
  size_t ni_pow = 1 << ni;

  if ( nc == 0 ) {
    for (size_t ibits = 0; ibits < ni_pow; ++ ibits) {
      check_sub(func, ibits);
    }
  }
  else {
    size_t ibits0 = (1 << ni) - 1;
    
    // 全ての 0 のパタン
    check_sub(func, 0);
    // 全ての 1 のパタン
    check_sub(func, ibits0);
    // 2つだけ 1 のパタン (1つだけの場合も含む)
    for (size_t i = 0; i < ni; ++ i) {
      size_t ibits1 = 1 << i;
      for (size_t j = 0; j < ni; ++ j) {
	size_t ibits = ibits1 | (1 << j);
	check_sub(func, ibits);
	check_sub(func, ibits ^ ibits0);
      }
    }
    
    for (size_t i = 0; i < nc; ++ i) {
      size_t ibits = random_num() & ((1 << ni) - 1);
      check_sub(func, ibits);
    }
  }
}

void
check(size_t ni,
      const vector<int>& vect,
      size_t nc)
{
  check_base(ni, vect);
  if ( nerr ) return;

  TvFunc func(ni, vect);

  check_count_one(func);
  if ( nerr ) return;

  check_walsh_0(func);
  if ( nerr ) return;

  check_walsh_1(func);
  if ( nerr ) return;

  check_walsh_2(func);
  if ( nerr ) return;

  check_walsh_01(func);
  if ( nerr ) return;

  check_check_sup(func);
  if ( nerr ) return;

  check_check_sym(func);
  if ( nerr ) return;

  check_walsh_w0(func, nc);
  
}

END_NAMESPACE_YM_NPN
#endif

#if 0
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

void
str2vect(size_t ni,
	 char* buff,
	 vector<int>& v)
{
  size_t ni_pow = 1 << ni;
  for (size_t i = 0; i < ni_pow; ++ i) {
    v[i] = buff[i] == '1' ? 1 : 0;
  }
}
#endif

int
main(int argc,
     const char** argv)
{
#if 0
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
    check0(ni);

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

#else

  try {

#if 1
    CvFunc func(20, 16);
    for (size_t i = 0; i < 16; ++ i) {
      for (size_t j = 0; j < 4; ++ j) {
	size_t pat = 0;
	if ( i & (1 << j) ) {
	  pat = 1UL;
	}
	else {
	  pat = 2UL;
	}
	func.set_pat(i, j, pat);
      }
      func.set_pat(i, i + 4, 1UL);
    }
#else
    CvFunc func(3, 2);
    func.set_pat(0, 0, 2UL); // x0'
    func.set_pat(0, 1, 1UL); // x1
    func.set_pat(1, 0, 1UL); // x0
    func.set_pat(1, 2, 1UL); // x2
#endif
    cout << func << endl;

    StopWatch sw;

    double time1 = sw.time().usr_time() * 1000.0 * 1000.0;
    sw.start();
    int vec[20];
    int w0;
    for (size_t i = 0; i < 1000; ++ i) {
      w0 = func.walsh_01(vec);
    }
    sw.stop();

    double time2 = sw.time().usr_time() * 1000.0 * 1000.0;
    double timediff = time2 - time1;

#if 0
    cout << "w0 = " << hex << w0 << endl;
    for (size_t i = 0; i < 20; ++ i) {
      cout << "w1(" << i << ") = " << hex << vec[i] << endl;
    }
#endif
    cout << "CPU time = " << timediff / 1000 << "usec" << endl;
  }

  catch ( nsYm::AssertError x ) {
    cerr << x << endl;
  }
  
#endif
  return 0;
}
