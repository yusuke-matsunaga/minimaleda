
/// @file libym_bnet/tests/makenode.cc
/// @brief BNode を生成するテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: makenode.cc 1343 2008-03-25 17:15:35Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include <ym_bnet/BNetwork.h>
#include <ym_bnet/BNetManip.h>


int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm;
  
  try {
    BNetwork network;
    BNetManip manip(&network);
    
    for (size_t i = 0; i < 1000000; ++ i) {
      manip.new_logic();
    }
  }
  catch ( AssertError x) {
    cout << x << endl;
  }

  return 0;
}
