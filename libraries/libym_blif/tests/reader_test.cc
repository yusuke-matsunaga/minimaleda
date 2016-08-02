
/// @file libym_blif/tests/reader_test.cc
/// @brief BlifNetworkReader のテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: reader_test.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_blif/BlifNetwork.h"
#include "ym_blif/BlifNetworkReader.h"
#include "ym_utils/MsgHandler.h"


BEGIN_NAMESPACE_YM

END_NAMESPACE_YM


int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm;
  
  if ( argc != 2 ) {
    cerr << "USAGE : " << argv[0] << " blif-file" << endl;
    return 2;
  }
  string filename = argv[1];

  try {
    BlifNetworkReader reader;
    StreamMsgHandler* msg_handler = new StreamMsgHandler(&cerr);
    reader.add_msg_handler(msg_handler);

    BlifNetwork network;
    
    if ( !reader.read(filename, network) ) {
      cerr << "Error in reading " << filename << endl;
      return 4;
    }

    network.write_blif(cout);
    
  }
  catch ( const AssertError& x) {
    cout << x << endl;
  }

  return 0;
}
