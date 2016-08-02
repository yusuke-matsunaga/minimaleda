// $Source: /export/home/project/synth/cvsrepos/ymtools/src/libym_cell/tests/lex_test.cc,v $
// $Author: matsunaga $
// $Revision: 1.1.1.1 $
// $Date: 2005/12/11 17:07:26 $

// Lex のテストプログラム


#include <iomanip>

#include "ymtools/StopWatch.h"
#include "../mislib/Lex.h"


BEGIN_NAMESPACE_YM_CELL

#include "../mislib/mislib_parser.h"

END_NAMESPACE_YM_CELL

int
main(int argc, char** argv)
{
  using namespace nsYm;
  using namespace nsYm::nsCell;
  
  if ( argc < 2 ) {
    cerr << "USAGE: " << argv[0]
	 << " <file-name> ..." << endl;
    return -1;
  }

  Lex lex;
  
  StopWatch timer;
  timer.start();
  
  for (int i = 1; i < argc; ++ i) {
    if ( !lex.open_file(argv[i]) ) {
      cerr << argv[i] << " : cannot open" << endl;
      return -2;
    }

    for ( ; ; ) {
      int id = lex.read_token();
      if ( id == EOF ) {
	break;
      }
      cout << "line: " << setw(4) << lex.cur_line()
	   << ", column: " << setw(3) << lex.first_column()
	   << " - " << setw(3) << lex.cur_column() << "\t";
      switch ( id ) {
      case STR:
	cout << "STR(\"" << lex.cur_string() << "\")";
	break;
      case NUM:
	cout << "NUM(" << lex.cur_string() << ")";
	break;
      case NONINV:
	cout << "NONINV";
	break;
      case INV:
	cout << "INV";
	break;
      case UNKNOWN:
	cout << "UNKNOWN";
	break;
      case CONST0:
	cout << "CONST0";
	break;
      case CONST1:
	cout << "CONST1";
	break;
      case GATE:
	cout << "GATE";
	break;
      case PIN:
	cout << "PIN";
	break;
      case NOT:
	cout << "!";
	break;
      case AND:
	cout << "*";
	break;
      case OR:
	cout << "+";
	break;
      case EQ:
	cout << "=";
	break;
      case SEMI:
	cout << ";";
	break;
      case LP:
	cout << "(";
	break;
      case RP:
	cout << ")";
	break;
      case EOF:
	cout << "EOF";
	break;
      case ERROR:
	cout << "ERROR";
	break;
      default:
	cout << "unkown(" << id << ")";
	break;
      }
      cout << endl;
    }
  }

  timer.stop();
  USTime time = timer.time();
  cout << "Time: " << time << endl;
  
  return 0;
}
