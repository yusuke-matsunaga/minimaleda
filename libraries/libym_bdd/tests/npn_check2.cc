// $Id: npn_check2.cc 700 2007-05-31 00:41:30Z matsunaga $

// NPN パッケージのテスト

#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include <ymtools/ymtools.h>
#include <ymtools/random.h>
#include <ymtools/Generator.h>
#include "TvFunc.h"
#include "WalshSigInt.h"

//#define DEBUG

bool verbose = false;

int check_count = 0;
int perm_count = 0;

using namespace std;
using namespace nsYm;
using namespace nsYm::nsBdd;
using namespace nsYm::nsBdd::nsNpn;

// 論理関数を表すクラス
struct Func {
  // オリジナルの関数
  TvFunc mOrigFunc;

  // 正規化後の関数
  TvFunc mNormFunc;
  
  // コンストラクタ
  Func(const TvFunc& ofunc, const TvFunc& nfunc);

};

// 同じ signature を持つ関数のグループ
struct FuncGroup {
  WalshSigInt mSignature;

  // 代表関数のリスト
  list<Func*> mRepList;

  FuncGroup* mLink;
  
  // コンストラクタ
  FuncGroup(const WalshSigInt& sig);

  // 関数を追加する．
  void add(const TvFunc& of, const TvFunc& nf);

};

class FuncGroupList
{
 public:

  // コンストラクタ
  FuncGroupList();

  // デストラクタ
  ~FuncGroupList();

  // 登録されている FuncGroup の数を返す．
  size_t num() const;

  // FuncGroup のリストを返す．
  const list<FuncGroup*>& fg_list() const;

  // 準代表関数の数を返す．
  size_t rep_num() const;

  // 関数を登録する．
  void reg(size_t ni, const vector<int>& v);

  // ハッシュ表に FuncGroup を追加する．
  void add(FuncGroup* fg);

 private:

  // size のテーブルを確保する．
  void resize(size_t size);

 private:
  // mTable のサイズ
  size_t mSize;

  // FuncGroup のハッシュ表
  FuncGroup** mTable;

  // mTable に登録されている要素数
  size_t mNum;

  // FuncGroup のリスト(順不同)
  list<FuncGroup*> mList;

  // 準代表関数を入れたハッシュ表
  hash_set<size_t> mRepList;

};


ostream&
operator<<(ostream& s, const Func& func)
{
  s << func.mOrigFunc << "\t"
    << func.mNormFunc << endl;
  return s;
}

// コンストラクタ
Func::Func(const TvFunc& ofunc, const TvFunc& nfunc) :
  mOrigFunc(ofunc),
  mNormFunc(nfunc)
{
}

// コンストラクタ
FuncGroup::FuncGroup(const WalshSigInt& sig) :
  mSignature(sig)
{
}

// 関数を追加する．
void
FuncGroup::add(const TvFunc& of, const TvFunc& nf)
{
  Func* func = new Func(of, nf);
  mRepList.push_back(func);
}

// コンストラクタ
FuncGroupList::FuncGroupList()
{
  mSize = 0;
  mTable = NULL;
  mNum = 0;
  resize(512);
}

// デストラクタ
FuncGroupList::~FuncGroupList()
{
  // 本当は中身の FuncGroup も削除する必要がある．
  delete [] mTable;
}

// 登録されている FuncGroup の数を返す．
size_t
FuncGroupList::num() const
{
  return mNum;
}

const list<FuncGroup*>&
FuncGroupList::fg_list() const
{
  return mList;
}

// 準代表関数の数を返す．
size_t
FuncGroupList::rep_num() const
{
  return mRepList.size();
}

// 関数を登録する．
void
FuncGroupList::reg(size_t ni, const vector<int>& buff)
{
  WalshSigInt sig;
  NpnMap wmap;
  TvFunc func(ni, buff);
  sig.normalize(func, wmap);

  size_t ni_pow = 1 << ni;
  {
    size_t v = 0;
    int ref = 0;
    if ( wmap.pol() == kPolPosi ) {
      ref = 1;
    }
    for (size_t m = 0; m < ni_pow; ++ m) {
      size_t n = 0;
      for (size_t i = 0; i < ni; ++ i) {
	tNpnImap nim = wmap.imap(i);
	if ( npnimap_pol(nim) == kPolPosi ) {
	  if ( m & (1 << i) ) {
	    n |= (1 << npnimap_pos(nim));
	  }
	}
	else {
	  if ( !(m & (1 << i)) ) {
	    n |= (1 << npnimap_pos(nim));
	  }
	}
      }
      if ( buff[m] == ref ) {
	v |= (1 << n);
      }
    }
    if ( mRepList.count(v) ) {
      // NPN 同値なので追加不要
      return;
    }
  }

  TvFunc norm_func = func.xform(wmap);
  size_t nig = sig.nig();
  PermGen pg(nig, nig);
  vector<size_t> rp(nig);
  vector<size_t> tmp(ni);
  vector<size_t> ivars(ni);
  vector<tNpnImap> imap(ni);
  vector<size_t> new_i(ni_pow);
  for (PermGen::iterator p = pg.begin(); !p.is_end(); ++ p) {
    bool not_equal = false;
    for (size_t i = 0; i < nig; ++ i) {
      size_t i1 = sig.top(i);
      size_t i2 = sig.top(p(i));
      if ( sig.c1(i1) != sig.c1(i2) ||
	   sig.gnum(i1) != sig.gnum(i2) ) {
	// Walsh 1次係数が合わない変数はマッチしない．
	not_equal = true;
	break;
      }
      for (size_t j = 0; j < ni; ++ j) {
	if ( sig.c2(i1, j) != sig.c2(i2, j) ) {
	  // Walsh 2次係数があわない変数はマッチしない．
	  not_equal = true;
	  break;
	}
      }
      if ( not_equal ) {
	break;
      }
      rp[p(i)] = i;
    }
    if ( not_equal ) {
      continue;
    }
    size_t n = 0;
    for (size_t i = 0; i < nig; ++ i) {
      size_t g = rp[i];
      size_t j = sig.top(g);
      size_t jend = j + sig.gnum(j);
      for ( ; j < jend; ++ j) {
	tmp[n] = j;
	++ n;
      }
    }
    size_t bitpos = 0;
    for (size_t i = 0; i < ni; ++ i) {
      if ( sig.c1(i) == 0 ) {
	ivars[i] = bitpos;
	++ bitpos;
      }
      else {
	ivars[i] = ni;
      }
    }
    
    for (size_t i = 0; i < ni_pow; ++ i) {
      size_t j = 0;
      for (size_t b = 0; b < ni; ++ b) {
	tVarId v = tmp[b];
	if ( i & (1 << b) ) {
	  j |= (1 << v);
	}
      }
      new_i[i] = j;
    }
    
    size_t bp = 1 << bitpos;
    for (size_t b = 0; b < bp; ++ b) {
      size_t imask = 0;
      for (size_t k = 0; k < ni; ++ k) {
	if ( ivars[k] < ni && ((b >> ivars[k]) & 1) ) {
	  imask |= (1 << k);
	}
      }
      
      size_t v1 = 0;
      for(size_t i = 0; i < ni_pow; ++ i) {
	if ( norm_func.value(i ^ imask) ) {
	  v1 |= (1 << new_i[i]);
	}
      }
      mRepList.insert(v1);
    }
    if ( sig.c0() == 0 ) {
      for (size_t b = 0; b < bp; ++ b) {
	size_t imask = 0;
	for (size_t k = 0; k < ni; ++ k) {
	  if ( ivars[k] < ni && ((b >> ivars[k]) & 1) ) {
	    ;
	  }
	  else {
	    imask |= (1 << k);
	  }
	}
	
	size_t v2 = 0;
	for(size_t i = 0; i < ni_pow; ++ i) {
	  if ( norm_func.value(i ^ imask) ) {
	    ;
	  }
	  else {
	    v2 |= (1 << new_i[i]);
	  }
	}
	mRepList.insert(v2);
      }
    }
  }

  size_t h = sig.hash() % mSize;
  FuncGroup* fg = NULL;
  for (fg = mTable[h]; fg; fg = fg->mLink) {
    if ( fg->mSignature == sig ) {
      fg->add(func, norm_func);
      return;
    }
  }
  if ( fg == NULL ) {
    // グループを作成し追加する．
    FuncGroup* new_group = new FuncGroup(sig);
    new_group->add(func, norm_func);
    add(new_group);
  } 
}

// ハッシュ表に FuncGroup を追加する．
void
FuncGroupList::add(FuncGroup* fg)
{
  if ( mNum >= mSize * 2 ) {
    resize(mSize * 2);
  }
  ++ mNum;
  size_t h = fg->mSignature.hash() % mSize;
  fg->mLink = mTable[h];
  mTable[h] = fg;
  mList.push_back(fg);
}

// size のテーブルを確保する．
void
FuncGroupList::resize(size_t size)
{
  typedef FuncGroup* FuncGroupPtr;
  FuncGroup** old_table = mTable;
  size_t old_size = mSize;

  mTable = new FuncGroupPtr[size];
  mSize = size;
  for (size_t i = 0; i < size; ++ i) {
    mTable[i] = NULL;
  }

  if ( old_size ) {
    for (size_t i = 0; i < old_size; ++ i) {
      FuncGroup* next;
      for(FuncGroup* fg = old_table[i]; fg; fg = next) {
	next = fg->mLink;
	size_t h = fg->mSignature.hash() % size;
	fg->mLink = mTable[h];
	mTable[h] = fg;
      }
    }
    delete [] old_table;
  }
}

void
gen(size_t ni, size_t limit)
{
  FuncGroupList func_group_list;
  
  size_t ni_exp = 1 << ni;
  vector<int> buff(ni_exp);
  for (size_t i = 0; i < ni_exp; ++ i) {
    buff[i] = 0;
  }
  size_t frontier = 0;
  for ( ; ; ) {
    if ( verbose ) {
      if ( buff[frontier] ) {
	for (size_t i = 0; i < ni_exp; ++ i) {
	  cout << buff[i];
	}
	cout << endl;
      }
    }
    if ( buff[frontier] ) {
      ++ frontier;
    }
    func_group_list.reg(ni, buff);
    bool carry = false;
    for (size_t i = 0; i < ni_exp; ++ i) {
      if ( buff[i] == 0 ) {
	buff[i] = 1;
	break;
      }
      buff[i] = 0;
      if ( i == ni_exp - 1 ) {
	carry = true;
      }
    }
    if ( carry ) {
      break;
    }
    if ( limit > 0 && frontier == limit ) {
      break;
    }
  }
  size_t n = 0;
  for (list<FuncGroup*>::const_iterator p = func_group_list.fg_list().begin();
       p != func_group_list.fg_list().end(); ++ p) {
    FuncGroup* func_group = *p;
    cout << func_group->mSignature << endl;
    for (list<Func*>::iterator q = func_group->mRepList.begin();
	 q != func_group->mRepList.end(); ++ q) {
      Func* func = *q;
      cout << "\t" << *func;
      ++ n;
    }
    cout << endl;
  }
  cout << "Total " << n << " NPN-representative function" << endl
       << "Total " << check_count << " checks" << endl
       << "Total " << func_group_list.num()
       << " different signatures" << endl
       << "Total " << perm_count << " permutations" << endl
       << "Total " << func_group_list.rep_num()
       << " permutations(2)" << endl;
}

void
rgen(size_t ni, size_t num)
{
  FuncGroupList func_group_list;
  
  size_t ni_exp = 1 << ni;
  vector<int> buff(ni_exp);
  for (size_t k = 0; k < num; ++ k) {
    for (size_t i = 0; i < ni_exp; ++ i) {
      buff[i] = random_num() & 1;
    }
    if ( verbose ) {
      for (size_t i = 0; i < ni_exp; ++ i) {
	cout << buff[i];
      }
      cout << endl;
    }
    func_group_list.reg(ni, buff);
  }
  if ( verbose ) {
    size_t n = 0;
    for (list<FuncGroup*>::const_iterator p = func_group_list.fg_list().begin();
	 p != func_group_list.fg_list().end(); ++ p) {
      FuncGroup* func_group = *p;
      cout << func_group->mSignature << endl;
      for (list<Func*>::iterator q = func_group->mRepList.begin();
	   q != func_group->mRepList.end(); ++ q) {
	Func* func = *q;
	cout << "\t" << *func;
	++ n;
      }
      cout << endl;
    }
  }
  size_t n = 0;
  for (list<FuncGroup*>::const_iterator p = func_group_list.fg_list().begin();
       p != func_group_list.fg_list().end(); ++ p) {
    FuncGroup* func_group = *p;
    n += func_group->mRepList.size();
  }
  cout << "Total " << num << " functions" << endl
       << "Total " << n << " NPN-representative functions" << endl
       << "Total " << check_count << " checks" << endl
       << "Total " << func_group_list.num()
       << " different signatures" << endl
       << "Total " << perm_count << " permutations" << endl
       << "Total " << func_group_list.rep_num()
       << " permutations(2)" << endl;
}

void
usage(const char* argv0)
{
  cerr << "USAGE : " << argv0 << " check <# of inputs> <vect1> <vect2>"
       << endl
       << "        " << argv0 << " gen <# of inputs>" << endl
       << "        " << argv0 << " rgen <# of inputs> <# of functions>"
       << " <random seed>" << endl
       << "        " << argv0 << " signature <# of inputs>" << endl;
}

void
str2vect(size_t ni, char* buff, vector<int>& v)
{
  size_t ni_pow = 1 << ni;
  for (size_t i = 0; i < ni_pow; ++ i) {
    v[i] = buff[i] == '1' ? 1 : 0;
  }
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
  if ( argc - base <= 1 ) {
    usage(argv[0]);
    return 2;
  }
  
  if ( strcmp(argv[base], "signature") == 0 ) {
    if ( argc - base != 2 ) {
      usage(argv[0]);
      return 2;
    }

    size_t ni = atoi(argv[base + 1]);
    size_t ni_pow = 1 << ni;
    char buff[256];
    while ( cin.getline(buff, sizeof(buff)) ) {
      vector<int> tmp(ni_pow);
      str2vect(ni, buff, tmp);
      TvFunc func(ni, tmp);
      WalshSigInt sig;
      NpnMap wmap;
      sig.normalize(func, wmap);
      TvFunc nfunc = func.xform(wmap);
      cout << func << "\t" << nfunc << "\t" << sig << endl;
    }
  }
  else if ( strcmp(argv[base], "check") == 0 ) {
    if ( argc - base != 4 ) {
      usage(argv[0]);
      return 2;
    }

    FuncGroupList tmp_list;
    size_t ni = atoi(argv[base + 1]);
    vector<int> tmp(1 << ni);
    str2vect(ni, argv[base + 2], tmp);
    tmp_list.reg(ni, tmp);
    str2vect(ni, argv[base + 3], tmp);
    tmp_list.reg(ni, tmp);
  }
  else if ( strcmp(argv[base], "gen") == 0 ) {
    if ( argc - base != 2 ) {
      usage(argv[0]);
      return 2;
    }

    size_t ni = atoi(argv[base + 1]);
    gen(ni, 0);
  }
  else if ( strcmp(argv[base], "ngen") == 0 ) {
    if ( argc - base != 3 ) {
      usage(argv[0]);
      return 2;
    }

    size_t ni = atoi(argv[base + 1]);
    size_t n = atoi(argv[base + 2]);
    gen(ni, n);
  }
  else if ( strcmp(argv[base], "rgen") == 0 ) {
    if ( argc - base != 4 ) {
      usage(argv[0]);
      return 2;
    }

    size_t ni = atoi(argv[base + 1]);
    size_t num = atoi(argv[base + 2]);
    int seed = atoi(argv[base + 3]);
    init_random_seed(seed);
    rgen(ni, num);
  }
  else {
    usage(argv[0]);
    return 2;
  }
  return 0;
}
