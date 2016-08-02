
/// @file libym_cell/tests/mislib_test.cc
/// @brief MisLib のテスト用プログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: mislib_test.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "mislib/MislibParser.h"
#include "ym_utils/MsgHandler.h"
#include "ym_cell/CellLibrary.h"
#include "ym_cell/Cell.h"
#include "ym_cell/CellPin.h"
#include "ym_cell/CellTiming.h"
#include "ym_lexp/LogExpr.h"


int
main(int argc,
     const char** argv)
{
  using namespace std;
  using namespace nsYm;
  using namespace nsYm::nsCell;

  if ( argc != 2 ) {
    cerr << "USAGE: " << argv[0] << " <filename>" << endl;
    return 255;
  }

  MislibParser parser;

  MsgHandler* mh = new StreamMsgHandler(&cerr);
  mh->set_mask(MsgHandler::kMaskAll);
  mh->delete_mask(kMsgInfo);
  mh->delete_mask(kMsgDebug);
  parser.msg_mgr().reg_handler(mh);

  const CellLibrary* library = parser.read(argv[1]);
  if ( library == NULL) {
    return 1;
  }

#if 0
  display_library(cout, *library);
#else
  const char* filename = "celllibrary_test.data";
  ofstream os;
  os.open(filename, ios::binary);
  if ( !os ) {
    // エラー
    cerr << "Could not create " << filename << endl;
    return 1;
  }
  dump_library(os, *library);

  os.close();

  ifstream is;
  is.open(filename);
  if ( !is ) {
    // エラー
    cerr << "Could not open " << filename << endl;
    return 2;
  }

  const CellLibrary* library2 = restore_library(is);

  display_library(cout, *library2);

#endif

  return 0;
}
