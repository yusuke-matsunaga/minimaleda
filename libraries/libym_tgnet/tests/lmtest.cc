
/// @file libym_tgnet/tests/lmtest.cc
/// @brief LogicMgr のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: lmtest.cc 1920 2008-12-20 15:52:42Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "../LogicMgr.h"


BEGIN_NAMESPACE_YM_TGNET

void
test1(const LogExpr& expr,
      LogicMgr& lm)
{
  cout << "Registering: " << expr << endl;
  TgGateTemplate id = lm.reg_logic(expr);
  cout << "  ID = " << id << endl;
}

END_NAMESPACE_YM_TGNET


///////////////////////////////////////////////////////////////////////////////
// Main.
///////////////////////////////////////////////////////////////////////////////

int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm;
  using namespace nsYm::nsTgnet;
  
  try {
    LogExpr v0 = LogExpr::make_posiliteral(0);
    LogExpr v1 = LogExpr::make_posiliteral(1);
    LogExpr v2 = LogExpr::make_posiliteral(2);
    LogExpr v3 = LogExpr::make_posiliteral(3);
    LogExpr v4 = LogExpr::make_posiliteral(4);
    LogExpr v5 = LogExpr::make_posiliteral(5);
    LogExpr v6 = LogExpr::make_posiliteral(6);
    LogExpr v7 = LogExpr::make_posiliteral(7);

    LogicMgr lm;
    
    LogExpr tmp0 = v0;
    test1(tmp0, lm);

    LogExpr tmp1 = (v0 & v1) | ~v1;
    test1(tmp1, lm);

    LogExpr tmp2 = v0 | ~v1;
    test1(tmp2, lm);

    LogExpr tmp3 = v0 & v1 | v2 & v3 | v4 & v5 | v6 & v7;
    test1(tmp3, lm);

    lm.dump(cout);
  }
  catch ( AssertError x) {
    cout << x << endl;
  }

  return 0;
}
