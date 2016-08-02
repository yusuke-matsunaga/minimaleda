
/// @file libym_aig/tests/iscas89test.cc
/// @brief iscas89 ファイルの読み書きのテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: iscas89test.cc 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_aig/AigNetwork.h"
#include "ym_aig/AigIscas89Reader.h"
#include "ym_aig/AigBlifWriter.h"
#include "CutSweep.h"


int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm;
  using nsYm::nsAig::CutSweep;
  
  if ( argc != 2 ) {
    cerr << "USAGE : " << argv[0] << " iscas89-file" << endl;
    return 2;
  }
  string filename = argv[1];
  
  try {
    MsgHandler* msg_handler = new StreamMsgHandler(&cerr);
    AigIscas89Reader reader;

    reader.add_msg_handler(msg_handler);
    
    AigNetwork network;

    if ( !reader.read(filename, &network) ) {
      cerr << "Error in reading " << filename << endl;
      return 4;
    }

    AigBlifWriter writer;
    writer(cout, network);
    
    CutSweep op;

    size_t n = op.enum_all_cut(network, 4);
    cout << "Total " << n << " cuts" << endl;
  }
  catch ( AssertError x) {
    cout << x << endl;
  }

  return 0;
}
