// $Id: randcombigen_test.cc 1978 2009-02-06 12:29:16Z matsunaga $

// RandCombiGen のテスト


#include "ym_utils/RandGen.h"


int
main(int argc,
     char** argv)
{
  using namespace nsYm;
  
  if ( argc != 3 ) {
    cerr << "USAGE: " << argv[0] << " <#elem> <#select>" << endl;
    return -1;
  }
  size_t n = atoi(argv[1]);
  size_t k = atoi(argv[2]);
  if ( n < k ) {
    cerr << "Error: the second argument should be less than or equal to the first argument"
	 << endl;
    return -2;
  }
  
  try {
    RandGen rg;
    RandCombiGen rcg(n, k);
    
    for (size_t i = 0; i < 1000; ++ i) {
      rcg.generate(rg);
      for (size_t j = 0; j < k; ++ j) {
	cout << " " << rcg.elem(j);
      }
      cout << endl;
    }
  }
  catch ( AssertError a ) {
    cerr << a << endl;
    return 255;
  }
  catch ( ... ) {
    cerr << "unknown exception" << endl;
    return 254;
  }

  return 0;
}
