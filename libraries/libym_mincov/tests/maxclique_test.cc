
/// @file libym_mincov/tests/maxclique_test.cc
/// @brief MaxCliqueのテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: maxclique_test.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MaxClique.h"
#include "ym_mincov/MincovCost.h"


// ファイルを読み込む．
// 形式は
// N <ノード数>
// C <ノード番号> <コスト>
// E <ノード番号1> <ノード番号2>
// ......
// Q

int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm;
  using namespace nsYm::nsMincov;

  ymuint32 n = 0;
  MaxClique* max_clique = NULL;
  vector<bool> cmatrix;
  try {
    char buff[1024];
    while ( cin.getline(buff, 1024, '\n') ) {
      char* p = strtok(buff, " \t");
      if ( !p ) continue;
      if ( p[1] != '\0' ) {
	cerr << "ERROR: illegal format" << endl;
	return 2;
      }
      switch ( p[0] ) {
      case 'N':
	{
	  if ( max_clique ) {
	    cerr << "ERROR: 'N' appeared twice" << endl;
	    return 2;
	  }
	  p = strtok(NULL, " \t");
	  if ( !p ) {
	    cerr << "ERROR: expected \"N <#-of-nodes>\"" << endl;
	    return 2;
	  }
	  n = atoi(p);
	  cout << "# of nodes: " << n << endl;
	  max_clique = new MaxClique(n);
	  cmatrix.resize(n * n, false);
	}
	break;

      case 'C':
	{
	  p = strtok(NULL, " \t");
	  if ( !p ) {
	    cerr << "ERROR: expected \"C <node-id> <cost>\"" << endl;
	    return 2;
	  }
	  ymuint id = atoi(p);
	  p = strtok(NULL, " \t");
	  if ( !p ) {
	    cerr << "ERROR: expected \"C <node-id> <cost>\"" << endl;
	    return 2;
	  }
	  double cv = strtod(p, NULL);
	  MincovCost* cost = new MincovCost(cv);
	  max_clique->set_cost(id, cost);
	}
	break;

      case 'E':
	{
	  p = strtok(NULL, " \t");
	  if ( !p ) {
	    cerr << "ERROR: expected \"E <node-id1> <node-id2>\"" << endl;
	    return 2;
	  }
	  ymuint id1 = atoi(p);
	  p = strtok(NULL, " \t");
	  if ( !p ) {
	    cerr << "ERROR: expected \"E <node-id1> <node-id2>\"" << endl;
	    return 2;
	  }
	  ymuint id2 = atoi(p);
	  max_clique->connect(id1, id2);
	  cmatrix[id1 * n + id2] = true;
	  cmatrix[id2 * n + id1] = true;
	}
	break;

      case 'Q':
	{
	  vector<ymuint32> ans;
	  max_clique->solve(ans);
	  sort(ans.begin(), ans.end());
	  cout << "max clique: ";
	  for (vector<ymuint32>::iterator p = ans.begin();
	       p != ans.end(); ++ p) {
	    cout << " " << *p;
	  }
	  cout << endl;
	  // クリークか検証しておく
	  ymuint k = ans.size();
	  for (ymuint i = 0; i < k - 1; ++ i) {
	    ymuint32 id1 = ans[i];
	    for (ymuint j = i + 1; j < k; ++ j) {
	      ymuint32 id2 = ans[j];
	      if ( !cmatrix[id1 * n + id2] ) {
		cerr << "ERROR: not a clique!" << endl;
		return 3;
	      }
	    }
	  }
	}
	break;
      }
    }
  }
  catch ( AssertError a ) {
    cerr << a << endl;
    return 255;
  }
  return 0;
}

