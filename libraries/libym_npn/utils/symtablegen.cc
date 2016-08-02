#include <iostream>


int
main(int argc,
     char** argv)
{
  using namespace std;
  
  for (size_t i = 1; i < 5; ++ i) {
    for (size_t j = 0; j < i; ++ j) {

      size_t mask1 = (1 << i);
      size_t mask2 = (1 << j);
      size_t mask3 = mask1 | mask2;
      size_t pat1 = 0;
      for (size_t v = 0; v < 32; ++ v) {
	if ( (v & mask3) == 0 ) {
	  pat1 |= (1 << v);
	}
      }
      cout << "(" << i << ", " << j << ")" << endl;
      cout << "pat1 = ";
      for (size_t x = 0; x < 8; ++ x) {
	size_t y = (pat1 >> ((7 - x) * 4)) & 15;
	if ( y < 10 ) {
	  cout << static_cast<char>(y + '0');
	}
	else {
	  cout << static_cast<char>(y - 10 + 'A');
	}
      }
      cout << endl;
    }
  }
  return 0;
}

