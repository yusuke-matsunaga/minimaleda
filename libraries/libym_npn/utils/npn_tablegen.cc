// $Id: npn_tablegen.cc 700 2007-05-31 00:41:30Z matsunaga $

// NPN パッケージのテスト

#include "ymtools.h"
#include "ym_utils/Generator.h"

//#define DEBUG

using namespace std;
using namespace nsYm;

void
naive(size_t ni)
{
  size_t ni_pow = 1 << ni;
  size_t end = 1 << ni_pow;
  PermGen pg(ni, ni);
  vector<size_t> table(end);
  vector<size_t> tmp_order(ni);
  list<size_t> mo_list;
  for (size_t fv = 0; fv < end; ++ fv) {
    // 並べ替えのために fv をベクタにばらす
    vector<int> fvect(ni_pow);
    for (size_t i = 0; i < ni_pow; ++ i) {
      if ( fv & (1 << i) ) {
	fvect[i] = 1;
      }
      else {
	fvect[i] = 0;
      }
    }

    // 反転用のマスク
    size_t omask = (1 << ni_pow) - 1;
  
    bool first = true;
    size_t max_vect = 0UL;
    for (PermGen::iterator p = pg.begin(); !p.is_end(); ++ p) {
      for (size_t i = 0; i < ni; ++ i) {
	tmp_order[i] = p(i);
      }
      for (size_t b = 0; b < ni_pow; ++ b) {
	// tmp_order, b にしたがって fv を変換する
	size_t tmp = 0UL;
	for (size_t i = 0; i < ni_pow; ++ i) {
	  if ( fvect[i ^ b] ) {
	    size_t new_i = 0;
	    for (size_t j = 0; j < ni; ++ j) {
	      if ( i & (1 << j) ) {
		new_i |= (1 << tmp_order[j]);
	      }
	    }
	    tmp |= 1 << new_i;
	  }
	}
	if ( first == true || max_vect < tmp ) {
	  first = false;
	  max_vect = tmp;
	  mo_list.clear();
	  size_t v = b << 1;
	  for (size_t i = 0; i < ni; ++ i) {
	    v |= (tmp_order[i] << (i * 2 + 5));
	  }
	  mo_list.push_back(v);
	}
	else if ( max_vect == tmp ) {
	  size_t v = b << 1;
	  for (size_t i = 0; i < ni; ++ i) {
	    v |= (tmp_order[i] << (i * 2 + 5));
	  }
	  mo_list.push_back(v);
	}
	tmp ^= omask;
	if ( max_vect < tmp ) {
	  max_vect = tmp;
	  mo_list.clear();
	  size_t v = (b << 1) | 1;
	  for (size_t i = 0; i < ni; ++ i) {
	    v |= (tmp_order[i] << (i * 2 + 5));
	  }
	  mo_list.push_back(v);
	}
	else if ( max_vect == tmp ) {
	  size_t v = (b << 1) | 1;
	  for (size_t i = 0; i < ni; ++ i) {
	    v |= (tmp_order[i] << (i * 2 + 5));
	  }
	  mo_list.push_back(v);
	}
      }
    }
    size_t v = max_vect | (mo_list.front() << 16);
    table[fv] = v;
#if 0
    cout << "original func: ";
    for (size_t i = 0; i < ni_pow; ++ i) {
      cout << fvect[i];
    }
    cout << endl;
    cout << "max func:      ";
    for (size_t i = 0; i < ni_pow; ++ i) {
      if ( max_vect & (1 << i) ) {
	cout << "1";
      }
      else {
	cout << "0";
      }
    }
    cout << endl;
    for (list<size_t>::const_iterator p = mo_list.begin();
	 p != mo_list.end(); ++ p) {
      size_t tmp = *p;
      size_t b = tmp & 0xFF;
      if ( b & 1 ) {
	cout << " ~(";
      }
      else {
	cout << "  (";
      }
      b >>= 1;
      const char* comma = "";
      for (size_t i = 0; i < ni; ++ i) {
	cout << comma;
	comma = ", ";
	if ( b & (1 << i) ) {
	  cout << "~";
	}
	else {
	  cout << " ";
	}
	size_t v = (tmp >> (i * 2 + 4)) & 0x3;
	cout << v;
      }
      cout << ")" << endl;
    }
    cout << endl;
#endif
  }

  bool first = true;
  for (size_t i = 0; i < end; ++ i) {
    if ( first ) {
      first = false;
    }
    else {
      cout << "," << endl;
    }
    cout << table[i];
  }
  cout << endl;
}

int
main(int argc, char** argv)
{
  try {
    if ( argc != 2 ) {
      return 2;
    }
      
    size_t ni = atoi(argv[1]);
    naive(ni);
  }
  catch ( nsYm::AssertError x ) {
    cerr << x;
  }
  return 0;
}
