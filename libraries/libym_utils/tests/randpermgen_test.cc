// $Id: randpermgen_test.cc 1978 2009-02-06 12:29:16Z matsunaga $

// RandPermGen のテスト


#include "ym_utils/RandGen.h"


int
main(int argc,
     char** argv)
{
  using namespace nsYm;
  
  if ( argc != 2 ) {
    cerr << "USAGE: " << argv[0] << " <#elem>" << endl;
    return -1;
  }
  size_t n = atoi(argv[1]);
  
  try {
    
    RandGen rg;
    RandPermGen rpg(n);
    
    for (size_t i = 0; i < 1000; ++ i) {
      rpg.generate(rg);
      for (size_t j = 0; j < n; ++ j) {
	cout << " " << rpg.elem(j);
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
