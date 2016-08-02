
/// @file libym_techmap/tests/bliftest.cc
/// @brief blif ファイルの読み込みのテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: bliftest.cc 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_bnet/BNetwork.h"
#include "ym_bnet/BNetBlifReader.h"
#include "ym_bnet/BNetDecomp.h"
#include "ym_bnet/BNet2Sbj.h"
#include "ym_sbj/SbjGraph.h"
#include "ym_utils/MsgHandler.h"
#include "ym_utils/StopWatch.h"


BEGIN_NONAMESPACE

const char* argv0 = NULL;

void
usage()
{
  using namespace std;

  cerr << "USAGE : " << argv0 << " input-size blif-file" << endl;
}

END_NONAMESPACE


int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm;

  argv0 = argv[0];
  if ( argc != 3 ) {
    usage();
    return 3;
  }

  ymuint k = atoi(argv[1]);
  string filename = argv[2];

#if 0
  try {
#endif
    MsgHandler* msg_handler = new StreamMsgHandler(&cerr);
    BNetBlifReader reader;

    reader.add_msg_handler(msg_handler);

    BNetwork network;

    if ( !reader.read(filename, network) ) {
      cerr << "Error in reading " << filename << endl;
      return 4;
    }

    BNetDecomp decomp;

    decomp(network, 2);

    BNet2Sbj bnet2sbj;

    SbjGraph sbjgraph;

    if ( !bnet2sbj(network, sbjgraph, cerr) ) {
      cerr << "Error occured in BNet2Sbj()" << endl;
      return 5;
    }

    StopWatch timer;
    vector<ymuint> depth_array;

    timer.reset();
    timer.start();
    ymuint d2 = sbjgraph.get_min_depth(k, depth_array);
    timer.stop();

    cout << "k = " << k << ", depth = " << d2 << endl;
    USTime t2 = timer.time();
    cout << t2 << endl;

#if 0
  }
  catch ( AssertError x) {
    cout << x << endl;
  }
#endif

  return 0;
}
