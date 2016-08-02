#include <iostream>

using namespace std;

int
main(int argc,
     char** argv)
{
  int n;
  
  cout << hex;
  while ( cin ) {
    cin >> n;
    size_t np = 1 << n;
    size_t pat = 0UL;
    for (size_t i = 0; i < np; ++ i) {
      char c;
      cin >> c;
      if ( c == '1' ) {
	pat |= (1UL << i);
      }
    }
    cout << "  0x";
    size_t pat3 = pat >> 12;
    cout << pat3;
    size_t pat2 = (pat >> 8) & 0xF;
    cout << pat2;
    size_t pat1 = (pat >> 4) & 0xF;
    cout << pat1;
    size_t pat0 = pat & 0xF;
    cout << pat0;
    cout << "U,";
    cout << endl;
  }
  return 0;
}
