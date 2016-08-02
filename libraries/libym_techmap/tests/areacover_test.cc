
/// @file libym_techmap/tests/areacover_test.cc
/// @brief AreaCover のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_bnet/BNetwork.h"
#include "ym_bnet/BNetBlifReader.h"
#include "ym_bnet/BNetDecomp.h"
#include "ym_bnet/BNet2Sbj.h"
#include "ym_sbj/SbjGraph.h"
#include "ym_techmap/CnGraph.h"
#include "ym_techmap/PatMgr.h"
#include "ym_utils/MsgHandler.h"


BEGIN_NONAMESPACE

const char* argv0 = NULL;

void
usage()
{
  using namespace std;

  cerr << "USAGE : " << argv0 << " <pattern-filename> <network-filename>"
       << endl;
}

END_NONAMESPACE


BEGIN_NAMESPACE_YM_TECHMAP

void
test(string pat_filename,
     string sbj_filename)
{
  PatMgr pat_mgr;
  {
    ifstream ifs;
    ifs.open(pat_filename.c_str(), ios::binary);
    if ( !ifs ) {
      // エラー
      cerr << "Could not open " << pat_filename << endl;
      return;
    }

    pat_mgr.load(ifs);
  }

  SbjGraph sbjgraph;
  {
    MsgHandler* msg_handler = new StreamMsgHandler(&cerr);
    BNetBlifReader reader;

    reader.add_msg_handler(msg_handler);

    BNetwork network;

    if ( !reader.read(sbj_filename, network) ) {
      cerr << "Error in reading " << sbj_filename << endl;
      return;
    }

    BNetDecomp decomp;

    decomp(network, 2);

    BNet2Sbj bnet2sbj;

    if ( !bnet2sbj(network, sbjgraph, cerr) ) {
      cerr << "Error occured in BNet2Sbj()" << endl;
      return;
    }
  }

  CnGraph mapnetwork;

  area_map(sbjgraph, pat_mgr, 0, mapnetwork);

  dump_verilog(cout, mapnetwork);
}

END_NAMESPACE_YM_TECHMAP


int
main(int argc,
     char** argv)
{
  argv0 = argv[0];

  if ( argc != 3 ) {
    usage();
    return 1;
  }

  nsYm::nsTechmap::test(argv[1], argv[2]);

  return 0;
}
