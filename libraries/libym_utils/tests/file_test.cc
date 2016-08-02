// $Id: file_test.cc 700 2007-05-31 00:41:30Z matsunaga $

// PathName のテスト


#include "ym_utils/File.h"

using namespace nsYm;

void
test(const char* str)
{
  PathName path(str);

  cout << "Original string : " << str << endl;
  cout << "Path Type: ";
  switch ( path.type() ) {
  case PathName::kAbsolute: cout << "absolute"; break;
  case PathName::kHome:     cout << "home relative"; break;
  case PathName::kRelative: cout << "relative"; break;
  }
  cout << endl;
  cout << "composed string : " << path.str() << endl;
  cout << "expanded string : " << path.expand().str() << endl;

  cout << "head            : " << path.head().str() << endl;
  cout << "tail            : " << path.tail() << endl;
  cout << "body            : " << path.body() << endl;
  cout << "ext             : " << path.ext() << endl;
}

int
main(int argc, char** argv)
{
  char buff[1024];
  string tmp;
  try {
    while ( cin.getline(buff, 1024, '\n') ) {
      test(buff);
    }
  }
  catch ( AssertError a ) {
    cerr << a << endl;
    return 255;
  }

  return 0;
}
