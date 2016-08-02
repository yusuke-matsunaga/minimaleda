// $Id: dllist_test.cc 700 2007-05-31 00:41:30Z matsunaga $

// DlList のテスト

#include "ym_utils/DlList.h"


using namespace std;
using namespace nsYm;

struct IntCell :
  public DlElem
{
  IntCell(int a) : mA(a) { }

  int mA;
};

DlList<IntCell>::iterator
find_pos(DlList<IntCell>& dlist,
	 int pos)
{
  for (DlList<IntCell>::iterator p = dlist.begin();
       p != dlist.end(); ++ p, -- pos) {
    if ( pos == 0 ) {
      return p;
    }
  }
  return dlist.end();
}

void
dump_cell(IntCell* cell)
{
  if ( cell ) {
    cout << cell->mA << endl;
  }
  else {
    cout << "<null>" << endl;
  }
}

void
dump_list(const DlList<IntCell>& list)
{
  cout << list.size() << ":";
  for (DlList<IntCell>::const_iterator p = list.begin(); p != list.end(); ++ p) {
    cout << " " << (*p)->mA;
  }
  cout << endl;
}

void
usage(const char* buff)
{
  cerr << "illegal command: " << buff << endl
       << "  possible command are" << endl
       << "    af<num> : push_front <num>" << endl
       << "    ab<num> : push_back <num>" << endl
       << "    df      : pop_front" << endl
       << "    db      : pop_back" << endl
       << "    i<num>  : insert <num>" << endl
       << "    e       : erase" << endl
       << "    p<num>  : locate iterator for i/e commands" << endl
       << "    f       : display front value" << endl
       << "    b       : display back value" << endl
       << "    c       : display current cell's value" << endl
       << "    l       : display all list value" << endl
       << "    q       : quit" << endl << endl;
}

int
main(int argc,
     char** argv)
{
  DlList<IntCell> dlist;
  vector<IntCell*> cell_list;

  char buff[1024];
  string tmp;
  try {
    bool go_on = true;
    DlList<IntCell>::iterator p = dlist.end();
    while ( go_on && cin.getline(buff, 1024, '\n') ) {
      IntCell* cell;
      switch ( buff[0] ) {
      case 'a':
      case 'A':
	switch ( buff[1] ) {
	case 'f':
	case 'F':
	  if ( !isdigit(buff[2]) ) {
	    usage(buff);
	    break;
	  }
	  cell = new IntCell(atoi(&buff[2]));
	  cell_list.push_back(cell);
	  dlist.push_front(cell);
	  break;
	case 'b':
	case 'B':
	  if ( !isdigit(buff[2]) ) {
	    usage(buff);
	    break;
	  }
	  cell = new IntCell(atoi(&buff[2]));
	  cell_list.push_back(cell);
	  dlist.push_back(cell);
	  break;
	default:
	  usage(buff);
	  break;
	}
	break;
      case 'd':
      case 'D':
	switch ( buff[1] ) {
	case 'f':
	case 'F':
	  if ( buff[2] != '\0' ) {
	    usage(buff);
	    break;
	  }
	  dlist.pop_front();
	  break;
	case 'b':
	case 'B':
	  if ( buff[2] != '\0' ) {
	    usage(buff);
	    break;
	  }
	  dlist.pop_back();
	  break;
	default:
	  usage(buff);
	  break;
	}
	break;
      case 'i':
      case 'I':
	if ( !isdigit(buff[1]) ) {
	  usage(buff);
	  break;
	}
	cell = new IntCell(atoi(&buff[1]));
	cell_list.push_back(cell);
	dlist.insert(p, cell);
	break;
      case 'e':
      case 'E':
	if ( buff[1] != '\0' ) {
	  usage(buff);
	  break;
	}
	dlist.erase(*p);
	p = dlist.end();
	break;
      case 'p':
      case 'P':
	if ( !isdigit(buff[1]) ) {
	  usage(buff);
	  break;
	}
	p = find_pos(dlist, atoi(&buff[1]));
	break;
      case 'f':
      case 'F':
	if ( buff[1] != '\0' ) {
	  usage(buff);
	  break;
	}
	dump_cell(dlist.front());
	break;
      case 'b':
      case 'B':
	if ( buff[1] != '\0' ) {
	  usage(buff);
	  break;
	}
	dump_cell(dlist.back());
	break;
      case 'c':
      case 'C':
	dump_cell(*p);
	break;
      case 'l':
      case 'L':
	if ( buff[1] != '\0' ) {
	  usage(buff);
	  break;
	}
	dump_list(dlist);
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
