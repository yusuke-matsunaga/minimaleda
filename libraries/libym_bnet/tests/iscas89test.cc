
/// @file libym_bnet/tests/iscas89test.cc
/// @brief iscas89 ファイルの読み書きのテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: iscas89test.cc 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include <ym_bnet/BNetwork.h>
#include <ym_bnet/BNetIscas89Reader.h>
#include <ym_bnet/BNetBlifWriter.h>
#include <ym_utils/MsgHandler.h>


int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm;
  
  if ( argc != 2 ) {
    cerr << "USAGE : " << argv[0] << " iscas89-file" << endl;
    return 2;
  }
  string filename = argv[1];
  
  try {
    MsgHandler* msg_handler = new StreamMsgHandler(&cerr);
    BNetIscas89Reader reader;

    reader.add_msg_handler(msg_handler);
    
    BNetwork network;

    if ( !reader.read(filename, network) ) {
      cerr << "Error in reading " << filename << endl;
      return 4;
    }

    BNetwork network2 = network;

    BNetBlifWriter writer;
    writer.dump(cout, network2);
  }
  catch ( AssertError x) {
    cout << x << endl;
  }

  return 0;
}
