
/// @file libym_mvn/tests/read_test.cc
/// @brief MvNode を生成するテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: read.cc 1343 2008-03-25 17:15:35Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/MvMgr.h"
#include "ym_mvn/MvVerilogReader.h"


int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm;

  list<string> filename_list;
  for (ymuint i = 1; i < argc; ++ i) {
    filename_list.push_back(argv[i]);
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

    for (list<string>::const_iterator p = filename_list.begin();
	 p != filename_list.end(); ++ p) {
      const string& name = *p;
      cerr << "Reading " << name;
      cerr.flush();
      bool stat = reader.read(name);
      cerr << " end" << endl;
      if ( !stat ) {
	return 1;
      }
    }
    cerr << "Generating MvNetwork" << endl;
    vector<pair<const VlDecl*, ymuint> > node_map;
    bool stat = reader.gen_network(mgr, node_map);
    cerr << " End" << endl;
    if ( !stat ) {
      cerr << "error occured" << endl;
      return 2;
    }

    //dump(cout, mgr);
    dump_verilog(cout, mgr);
    dump_node_map(cout, mgr, node_map);
#if 0
  }
  catch ( AssertError x) {
    cout << x << endl;
  }
#endif
  return 0;
}
