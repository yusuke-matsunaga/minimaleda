
/// @file libym_aig/tests/bliftest.cc
/// @brief blif ファイルの読み書きのテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: bliftest.cc 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_aig/AigNetwork.h"
#include "ym_aig/AigBlifReader.h"
#include "ym_aig/AigBlifWriter.h"
#include "../LrMgr.h"


int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm;
  using nsYm::nsAig::LrMgr;
  
  if ( argc != 2 ) {
    cerr << "USAGE : " << argv[0] << " blif-file" << endl;
    return 2;
  }
  string filename = argv[1];
  
  try {
    MsgHandler* msg_handler = new StreamMsgHandler(&cerr);
    AigBlifReader reader;

    reader.add_msg_handler(msg_handler);
    
    AigNetwork network;

    if ( !reader.read(filename, &network) ) {
      cerr << "Error in reading " << filename << endl;
      return 4;
    }
    
    AigBlifWriter writer;
    writer(cout, network);
    
    size_t na_init = network.andnode_num();


    size_t na_cur = na_init;
    for ( ; ; ) {
      LrMgr lr_mgr;
      lr_mgr.local_rewrite(network, 4);
      
      writer(cout, network);

      size_t na_after = network.andnode_num();
      if ( na_after == na_cur ) {
	break;
      }
      na_cur = na_after;
    }
    
    cout << "#nodes before: " << na_init << endl
	 << "#nodes after:  " << network.andnode_num() << endl;
    
  }
  catch ( AssertError x) {
    cout << x << endl;
  }

  return 0;
}
