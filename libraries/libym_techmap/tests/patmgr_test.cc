
/// @file libym_techmap/test/patmgr_test.cc
/// @brief PatMgr のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "patgen/patgen_nsdef.h"
#include "patgen/PgFuncMgr.h"
#include "ym_techmap/PatMgr.h"
#include "ym_cell/CellMislibReader.h"


int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm;
  using namespace nsYm::nsCell;
  using namespace nsYm::nsTechmap;
  using namespace nsYm::nsTechmap::nsPatgen;

  const char* filename = argv[1];
  CellMislibReader reader;
  const CellLibrary* library = reader.read(filename);
  if ( library == NULL ) {
    return 1;
  }

  const char* datafile = "patdata.bin";
  {
    PgFuncMgr pgf_mgr;
    pgf_mgr.set_library(library);

    pg_display(cout, pgf_mgr);

    ofstream os;
    os.open(datafile, ios::binary);
    if ( !os ) {
      // エラー
      cerr << "Could not create " << datafile << endl;
      return 2;
    }

    pg_dump(os, pgf_mgr);
    os.close();
  }

  PatMgr pat_mgr;
  {
    ifstream ifs;
    ifs.open(datafile, ios::binary);
    if ( !ifs ) {
      // エラー
      cerr << "Could not open " << filename << endl;
      return 3;
    }

    pat_mgr.load(ifs);
  }

  dump(cout, pat_mgr);
}
