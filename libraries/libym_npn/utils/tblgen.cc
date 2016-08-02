#include <iostream>
#include <list>

using namespace std;

int
main()
{
  list<size_t> plist[21];
  for (size_t pat = 0; pat < 0x00100000; ++ pat) {
    size_t c = 0;
    for (size_t i = 0; i < 32; ++ i) {
      if ( pat & (1 << i) ) {
	++ c;
      }
    }
    plist[c].push_back(pat);
  }

  cout << "size_t plist[] = {" << endl;
  const char* comma = "";
  for (size_t i = 0; i <= 20; ++ i) {
    int count = 0;
    for (list<size_t>::iterator p = plist[i].begin();
	 p != plist[i].end(); ++ p) {
      cout << comma;
      comma = ", ";
      ++ count;
      if ( count == 8 ) {
	cout << endl;
	count = 0;
      }
      cout << *p;
    }
    if ( i < 20 ) {
      cout << "," << endl;
      comma = "";
    }
  }
  cout << endl
       << "};" << endl;

  cout << "size_t pidx[] = {" << endl;
  size_t n = 0;
  comma = "";
  for (size_t i = 0; i <= 20; ++ i) {
    cout << comma << n;
    comma = ", ";
    n += plist[i].size();
  }
  cout << comma << n << endl
       << "};" << endl;
}
