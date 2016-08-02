#include <iostream>

using namespace std;

int
main(int argc,
     char** argv)
{
  cout << "20 ";
  size_t ni_pow = 1UL << 20;
  for (size_t i = 0; i < ni_pow; ++ i) {
    size_t c = i & 0xF;
    if ( i & (1UL << (c + 4)) ) {
      cout << "1";
    }
    else {
      cout << "0";
    }
  }
  cout << endl;
}
