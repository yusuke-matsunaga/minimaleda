// $Id: npn_test.cc 700 2007-05-31 00:41:30Z matsunaga $

// NPN パッケージのテスト

#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include <ymtools/bdd/Npn.h>
#include <ymtools/bdd/Dg.h>

#include "bddtest.h"

using namespace std;
using namespace nsYm::nsBdd;

bool verbose = false;

Bdd
vect2bdd(BddMgrRef mgr, size_t vect, size_t ni)
{
  size_t ni_exp = 1 << ni;
  Bdd ans = mgr.make_zero();
  for (size_t i = 0; i < ni_exp; ++ i) {
    if ( vect & (1 << i) ) {
      Bdd minterm = mgr.make_one();
      for (size_t j = 0; j < ni; ++ j) {
	if ( i & (1 << j) ) {
	  minterm &= mgr.make_posiliteral(j);
	}
	else {
	  minterm &= mgr.make_negaliteral(j);
	}
      }
      ans |= minterm;
    }
  }
  return ans;
}

Bdd
vectstr2bdd(BddMgrRef mgr, size_t ni, char vectstr[])
{
  size_t ni_exp = 1 << ni;
  Bdd ans = mgr.make_zero();
  for (size_t i = 0; i < ni_exp; ++ i) {
    if ( vectstr[i] == '1' ) {
      Bdd minterm = mgr.make_one();
      for (size_t j = 0; j < ni; ++ j) {
	if ( i & (1 << j) ) {
	  minterm &= mgr.make_posiliteral(j);
	}
	else {
	  minterm &= mgr.make_negaliteral(j);
	}
      }
      ans |= minterm;
    }
  }
  return ans;
}

void
dump_func(size_t vect, size_t ni)
{
  size_t ni_exp = 1 << ni;
  for (size_t i = 0; i < ni_exp; ++ i) {
    if ( vect & (1 << i) ) {
      cout << "1";
    }
    else {
      cout << "0";
    }
  }
  cout << endl;
}

void
check(const Bdd& f1, const Bdd& f2)
{
  cout << "f1:" << f1.sop() << endl;
  cout << "f2:" << f2.sop() << endl;

  NpnMgr npnmgr;
  NpnFuncId id1 = npnmgr.reg_func(f1, true);
  NpnFuncId id2 = npnmgr.reg_func(f2, true);

  list<NpnMap> map_list1;
  npnmgr.find_map(id1, id2, map_list1);
  if ( !map_list1.empty() ) {
    cout << "f1 and f2 are NPN equivalence" << endl;
    for (list<NpnMap>::iterator p = map_list1.begin();
	 p != map_list1.end(); ++p) {
      NpnMap& m = *p;
      cout << m << endl;
    }
  }
}

void
gen(size_t ni)
{
  BddMgrRef mgr;
#if 0
  DgMgr dgmgr(mgr);
#endif
  
  list<Bdd> rep_list;
  list<size_t> vect_list;
  list<NpnFuncId> id_list;
  NpnMgr npnmgr;
  
  size_t ni_exp = 1 << ni;
  size_t ni_exp_exp = 1 << ni_exp;
  for (size_t vect = 0; vect < ni_exp_exp; ++ vect) {
    if ( verbose ) {
      dump_func(vect, ni);
    }
    Bdd f = vect2bdd(mgr, vect, ni);
    bool new_func, new_rep;
    NpnFuncId id = npnmgr.reg_func(f, new_func, new_rep, true);
    id_list.push_back(id);
    if ( new_rep ) {
      Bdd rep = id.repfunc_bdd();
      rep_list.push_back(rep);
      vect_list.push_back(vect);
#if 0
      Dg dg = dgmgr.bidecomp(rep);
      if ( !dg.has_decomp() ) {
	cout << rep.sop() << endl;
      }
#endif
    }
  }

  cout << "representative functions" << endl;
  for (list<size_t>::const_iterator p = vect_list.begin();
       p != vect_list.end(); ++ p) {
    size_t vect = *p;
    dump_func(vect, ni);
  }

  cout << "total " << ni_exp_exp << " " << ni << " inputs functions" << endl
       << "total " << rep_list.size() << " NPN representative functions"
       << endl;
}

void
usage(const char* argv0)
{
  cerr << "USAGE : " << argv0 << " check1 <expr1> <expr2>" << endl
       << "        " << argv0 << " check2 <# of inputs> <vect1> <vect2>"
       << endl
       << "        " << argv0 << " gen <# of inputs>" << endl;
}

int
main(int argc, char** argv)
{
  int base = 1;
  for ( ; base < argc; ++ base) {
    if ( argv[base][0] != '-' ) {
      break;
    }
    if ( strcmp(argv[base], "--verbose") == 0 ) {
      verbose = true;
    }
  }
  if ( argc - base < 1 ) {
    usage(argv[0]);
    return 2;
  }

  if ( strcmp(argv[base], "check1") == 0 ) {
    if ( argc - base != 3 ) {
      usage(argv[0]);
      return 2;
    }

    BddMgrRef mgr;
    Bdd f1 = str2bdd(mgr, argv[base + 1]);
    Bdd f2 = str2bdd(mgr, argv[base + 2]);
    check(f1, f2);
  }
  else if ( strcmp(argv[base], "check2") == 0 ) {
    if ( argc - base != 4 ) {
      usage(argv[0]);
      return 2;
    }

    size_t ni = atoi(argv[base + 1]);
    BddMgrRef mgr;
    Bdd f1 = vectstr2bdd(mgr, ni, argv[base + 2]);
    Bdd f2 = vectstr2bdd(mgr, ni, argv[base + 3]);
    check(f1, f2);
  }
  else if ( strcmp(argv[base], "gen") == 0 ) {
    if ( argc - base != 2 ) {
      usage(argv[0]);
      return 2;
    }

    size_t ni = atoi(argv[base + 1]);
    gen(ni);
  }

  return 0;
}
