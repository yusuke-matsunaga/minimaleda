// $Id: multicombigen_test.cc 700 2007-05-31 00:41:30Z matsunaga $

// nsMagus::MultiCombiGen のテスト


#include "ym_utils/Generator.h"

int
main(int argc,
     char** argv)
{
  using namespace nsYm;
  try {
    char buff[1024];
    while ( cin.getline(buff, 1024, '\n') ) {
      char* p = strtok(buff, " \t");
      if ( !p ) {
	cerr << "ERROR: ngrp must be specified" << endl;
	return 2;
      }
      size_t ngrp = atoi(p);
      if ( ngrp == 0 ) {
	cerr << "ERROR: ngrp == 0" << endl;
	return 2;
      }
      vector<pair<size_t, size_t> > nk_array(ngrp);
      for (size_t g = 0; g < ngrp; ++ g) {
	char* p = strtok(NULL, " \t");
	if ( !p ) {
	  cerr << "ERROR: n must be specified" << endl;
	  return 2;
	}
	size_t n = atoi(p);
	if ( n == 0 ) {
	  cerr << "ERROR: n == 0" << endl;
	  return 2;
	}
	p = strtok(NULL, " \t");
	if ( !p ) {
	  cerr << "ERROR: k must be specified" << endl;
	  return 2;
	}
	size_t k = atoi(p);
	if ( k == 0 ) {
	  cerr << "ERROR: k == 0" << endl;
	  return 2;
	}
	if ( n < k ) {
	  cerr << "ERROR: n(" << n << ") < k(" << k << ")" << endl;
	  return 1;
	}
	nk_array[g] = make_pair(n, k);
      }
      MultiCombiGen mcg(nk_array);
      for (MultiCombiGen::iterator p = mcg.begin(); !p.is_end(); ++ p) {
	for (size_t g = 0; g < ngrp; ++ g) {
	  size_t k = nk_array[g].second;
	  for (size_t i = 0; i < k; ++ i) {
	    cout << " " << p(g, i);
	  }
	  cout << " |";
	}
	cout << endl;
      }
      cout << "---" << endl;
    }
  }
  catch ( AssertError a ) {
    cerr << a << endl;
    return 255;
  }

  return 0;
}
