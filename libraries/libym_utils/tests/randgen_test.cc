// $Id: randgen_test.cc 1978 2009-02-06 12:29:16Z matsunaga $

// RandGen のテスト


#include "ym_utils/RandGen.h"


int
main(int argc,
     char** argv)
{
  using namespace nsYm;

  try {
    RandGen rg;

    for (size_t i = 0; i < 1000; ++ i) {
      unsigned long num = rg.int32();
      cout << num << endl;
    }

    for (size_t i = 0; i < 1000; ++ i) {
      double num = rg.real2();
      cout << num << endl;
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
