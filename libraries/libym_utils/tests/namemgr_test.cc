// $Id: namemgr_test.cc 958 2007-08-28 05:38:02Z matsunaga $

// NameMgr のテスト


#include "ym_utils/NameMgr.h"

int
main(int argc,
     char** argv)
{
  using namespace nsYm;

  NameMgr nm("<", ">");
  char buff[1024];
  //string tmp;
  StrBuff tmp;
  try {
    bool go_on = true;
    while ( go_on && cin.getline(buff, 1024, '\n') ) {
      switch ( buff[0] ) {
      case 'a':
      case 'A':
	tmp = &buff[1];
	cout << "Adding " << tmp << endl;
	nm.add(tmp.c_str());
	break;
      case 'c':
      case 'C':
	cout << "Clearing" << endl;
	nm.clear();
	break;
      case 'd':
      case 'D':
	nm.dump(cout);
	break;
      case 'e':
      case 'E':
	tmp = &buff[1];
	cout << "Erasing " << tmp << endl;
	nm.erase(&buff[1]);
	break;
      case 'n':
      case 'N':
	tmp = nm.new_name(true);
	cout << "Creating " << tmp << endl;
	break;
      case 'q':
      case 'Q':
	go_on = false;
	break;
      default:
	break;
      }
    }
  }
  catch ( AssertError a ) {
    cerr << a << endl;
    return 255;
  }

  return 0;
}
