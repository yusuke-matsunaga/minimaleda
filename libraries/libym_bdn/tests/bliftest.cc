
/// @file libym_bdn/tests/bliftest.cc
/// @brief blif ファイルの読み書きのテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: bliftest.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_blif/BlifNetwork.h"
#include "ym_blif/BlifNetworkReader.h"
#include "ym_bdn/BdNetwork.h"
#include "ym_bdn/BlifBdnConv.h"
#include "ym_bdn/BdnBlifWriter.h"


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

#if 0
  try {
#endif
    MsgHandler* msg_handler = new StreamMsgHandler(&cerr);
    BlifNetworkReader reader;

    reader.add_msg_handler(msg_handler);

    BlifNetwork blif_network;

    if ( !reader.read(filename, blif_network) ) {
      cerr << "Error in reading " << filename << endl;
      return 4;
    }
    
    BdNetwork network;
    BlifBdnConv conv;
    bool stat = conv(blif_network, network);
    if ( !stat ) {
      cerr << "Error in converting from BlifNetwork to BdNetwork" << endl;
      return 5;
    }
    
    //dump(cout, bdn);
    BdnBlifWriter blif_writer;

    blif_writer(cout, network);

#if 0
  }
  catch ( AssertError x) {
    cout << x << endl;
  }
#endif
  
  return 0;
}
