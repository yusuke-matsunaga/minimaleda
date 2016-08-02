// $Id: zstream_test.cc 700 2007-05-31 00:41:30Z matsunaga $

// zstream のテスト


#include "ym_zstream/zstream.h"


int
main(int argc,
     char** argv)
{
  using namespace nsYm;

  if ( argc != 4 ) {
    cerr << "usage: " << argv[0] << " c|d input-file output-file" << endl;
    return 250;
  }
  if ( strlen(argv[1]) != 1 || argv[1][0] != 'c' && argv[1][0] != 'd' ) {
    cerr << "argv[1] should be \"c\" or \"d\"" << endl;
    return 251;
  }
  try {
    if ( argv[1][0] == 'c' ) {
      // compress mode
      ifstream is;
      is.open(argv[2]);
      if ( !is.is_open() ) {
	cerr << "Could not open file: " << argv[2] << endl;
	return 252;
      }
      ofstream os;
      os.open(argv[3]);
      if ( !os.is_open() ) {
	cerr << "Could not open file: " << argv[3] << endl;
	return 253;
      }
      ozstream ozs(os);
      int c;
      while ( (c = is.get()) != EOF ) {
	ozs.put(static_cast<char>(c));
      }
    }
    else if ( argv[1][0] == 'd' ) {
      // decompress mode
      ifstream is;
      is.open(argv[2]);
      if ( !is.is_open() ) {
	cerr << "Could not open file: " << argv[2] << endl;
	return 252;
      }
      ofstream os;
      os.open(argv[3]);
      if ( !os.is_open() ) {
	cerr << "Could not open file: " << argv[3] << endl;
	return 253;
      }
      izstream izs(is);
      int c;
      while ( (c = izs.get()) != EOF ) {
	os.put(static_cast<char>(c));
      }
    }
  }
  catch ( AssertError a ) {
    cerr << a << endl;
    return 255;
  }
  catch ( zlib_error x ) {
    cerr << x.mLabel << ": " << x.mStatus;
    if ( x.mMsg ) {
      cerr << ": " << x.mMsg;
    }
    cerr << endl;
    return x.mStatus;
  }
  catch ( ... ) {
    cerr << "unknown exception" << endl;
    return 254;
  }

  return 0;
}
