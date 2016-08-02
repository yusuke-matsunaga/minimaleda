
/// @file lutmap/src/lutmap.cc
/// @brief lutmap のメイン関数
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: bliftest.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_lutmap/lutmap_nsdef.h"
#include "ym_sbj/SbjGraph.h"
#include "ym_sbj/SbjDumper.h"
#include "ym_lutmap/LnGraph.h"
#if 0
#include "ym_lutmap/BNet2Sbj.h"
#include "ym_lutmap/Ln2BNet.h"
#include "ym_bnet/BNetwork.h"
#include "ym_bnet/BNetBlifReader.h"
#include "ym_bnet/BNetDecomp.h"
#else
#include "ym_mvn/Mvn2Sbj.h"
#include "ym_mvn/MvMgr.h"
#include "ym_mvn/MvVerilogReader.h"
#include "ym_mvn/MvNodeMap.h"
#endif
#include "ym_utils/MsgHandler.h"


int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm;

  if ( argc < 2 ) {
    cerr << "USAGE : " << argv[0] << " verilog-file" << endl;
    return 2;
  }

  bool dump1 = false;
  bool dump2 = false;
  bool dump3 = false;
  string dump1_file;
  string dump2_file;
  string dump3_file;

  list<string> filename_list;
  for (int i = 1; i < argc; ++ i) {
    if ( argv[i][0] == '-' ) {
      string opt = argv[i] + 1;
      if ( opt == "dump1" ) {
	if ( i + 1 >= argc ) {
	  cerr << "-dump1 <file-name>" << endl;
	  return 1;
	}
	dump1 = true;
	dump1_file = argv[i + 1];
	++ i;
      }
      else if ( opt == "dump2" ) {
	if ( i + 1 >= argc ) {
	  cerr << "-dump2 <file-name>" << endl;
	  return 1;
	}
	dump2 = true;
	dump2_file = argv[i + 1];
	++ i;
      }
      else if ( opt == "dump3" ) {
	if ( i + 1 >= argc ) {
	  cerr << "-dump3 <file-name>" << endl;
	  return 1;
	}
	dump3 = true;
	dump3_file = argv[i + 1];
	++ i;
      }
      else {
	cerr << argv[i] << ": illegal option" << endl;
	return 2;
      }
    }
    else {
      filename_list.push_back(argv[i]);
    }
  }

#if 0
  try {
#endif
    MvMgr mgr;
    MvVerilogReader reader;
    MsgHandler* mh = new StreamMsgHandler(&cerr);
    mh->set_mask(MsgHandler::kMaskAll);
    mh->delete_mask(kMsgInfo);
    mh->delete_mask(kMsgDebug);
    reader.add_msg_handler(mh);

    reader.set_ffname("KTECH_DFF", // セル名
		      "D",         // データ入力
		      "CK",        // クロック
		      "Q",         // ノーマル出力
		      "QN",        // 反転出力
		      "",          // セット
		      "");         // リセット

    // verilog ファイルの読み込み
    for (list<string>::const_iterator p = filename_list.begin();
	 p != filename_list.end(); ++ p) {
      const string& name = *p;
      cerr << "Reading " << name;
      cerr.flush();
      bool stat = reader.read(name);
      cerr << " end" << endl;
      if ( !stat ) {
	return -1;
      }
    }
    // MvNetwork に変換
    vector<pair<const VlDecl*, ymuint> > node_map;
    bool stat = reader.gen_network(mgr, node_map);
    if ( !stat ) {
      cerr << "error occured" << endl;
      return -2;
    }
    if ( dump1 ) {
      ofstream ofs;
      ofs.open(dump1_file.c_str());
      if ( !ofs.is_open() ) {
	cerr << dump1_file << ": could not open" << endl;
	return 3;
      }
      dump_verilog(ofs, mgr);
      dump_node_map(ofs, mgr, node_map);
    }

    // SbjGraph に変換
    SbjGraph sbj_network;
    MvNodeMap mvnode_map(mgr.max_node_id());
    Mvn2Sbj conv;

    conv(mgr, sbj_network, mvnode_map);

    if ( dump2 ) {
      ofstream ofs;
      ofs.open(dump2_file.c_str());
      if ( !ofs.is_open() ) {
	cerr << dump2_file << ": could not open" << endl;
	return 3;
      }
      SbjDumper d;
      d.dump_verilog(ofs, sbj_network);
      dump_mvnode_map(ofs, mgr, mvnode_map);
    }

    // LUT にマッピング
    LnGraph lut_network;
    ymuint lut_num;
    ymuint depth;
    delay_map(sbj_network,
	      4,            // LUT の入力数
	      0,            // スラック
	      3,            // モード(0から3の値)
	      lut_network,
	      lut_num,
	      depth);

    if ( dump3 ) {
      ofstream ofs;
      ofs.open(dump3_file.c_str());
      if ( !ofs ) {
	cerr << dump3_file << ": could not open" << endl;
	return 3;
      }
      dump_verilog(ofs, lut_network);
    }
#if 0
  }
  catch ( AssertError x) {
    cout << x << endl;
  }
#endif
  return 0;
}
