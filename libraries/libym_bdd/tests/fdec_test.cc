
/// @file libym_bdd/tests/fdec_test.cc
/// @brief 関数分解用のテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: fdec_test.cc 700 2007-05-31 00:41:30Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include <ym_bdd/Dg.h>

#include "bddtest.h"

using namespace std;
using namespace nsYm::nsBdd;

int
main(int argc, char** argv)
{
  if ( argc != 2 ) {
    cout << "USAGE : " << argv[0] << " <expr>" << endl;
    return 2;
  }

  BddMgrRef mgr;
  Bdd f = str2bdd(mgr, argv[1]);
  cout << "f:" << f.sop() << endl << endl;

  DgMgr dgmgr(mgr);
  Dg dg = dgmgr.decomp(f);

  dg.display(cout);
  cout << endl;

  PrimDecList dec_list;
  dg.enum_djdec(0, 4, dec_list);
  dump_primdec_list(dec_list, cout);

  return 0;
}
