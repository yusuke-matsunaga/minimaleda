// $Id: npn_check.cc 700 2007-05-31 00:41:30Z matsunaga $

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
bool dump_flag = false;

int check_count = 0;
int perm_count = 0;
int unresolved_count = 0;

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

  // すでに登録されている関数に一致するか調べる．
  bool check(const TvFunc& f) const;

  // 新たな関数を追加する．
  void add(const TvFunc& f, const TvFunc& nf);

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

  // 関数を登録する．
  void reg(size_t ni, const vector<int>& v);

 private:

  // ハッシュ表に FuncGroup を追加する．
  void add(FuncGroup* fg);

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

// すでに登録されている関数に一致するか調べる．
bool
FuncGroup::check(const TvFunc& func1) const
{
  if ( verbose ) {
    cout << "FuncGroup::check(" << func1 << ")" << endl;
    for (list<Func*>::const_iterator p = mRepList.begin();
	 p != mRepList.end(); ++ p) {
      cout << **p << endl;
    }
  }

  for (list<Func*>::const_iterator p = mRepList.begin();
       p != mRepList.end(); ++ p) {
    if ( func1 == (*p)->mNormFunc ) {
      if ( verbose ) {
	cout << " same normalized func" << endl;
      }
      return true;
    }
  }

  size_t pn = mSignature.num();
  if ( pn > 1 ) {
    ++ check_count;
    //perm_count += pn;
  }
  size_t ni = func1.ni();
  size_t ni_pow = 1 << ni;
  size_t nig = mSignature.nig();
  PermGen pg(nig, nig);
  vector<size_t> rp(nig);
  vector<size_t> tmp(ni);
  vector<size_t> ivars(ni);
  vector<tNpnImap> imap(ni);
  vector<size_t> new_i(ni_pow);
  for (PermGen::iterator p = pg.begin(); !p.is_end(); ++ p) {
    bool not_equal = false;
    for (size_t i = 0; i < nig; ++ i) {
      size_t i1 = mSignature.top(i);
      size_t i2 = mSignature.top(p(i));
      if ( mSignature.c1(i1) != mSignature.c1(i2) ||
	   mSignature.c2sum(i1) != mSignature.c2sum(i2) ||
	   mSignature.gnum(i1) != mSignature.gnum(i2) ) {
	// Walsh 1次係数が合わない変数はマッチしない．
	not_equal = true;
	break;
      }
#if 0
      for (size_t j = 0; j < ni; ++ j) {
	if ( mSignature.c2(i1, j) != mSignature.c2(i2, j) ) {
	  // Walsh 2次係数があわない変数はマッチしない．
	  not_equal = true;
	  break;
	}
      }
#endif
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
      size_t j = mSignature.top(g);
      size_t jend = j + mSignature.gnum(j);
      for ( ; j < jend; ++ j) {
	tmp[n] = j;
	++ n;
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
    
    size_t bitpos = 0;
    for (size_t i = 0; i < ni; ++ i) {
      if ( mSignature.c1(i) == 0 ) {
	ivars[i] = bitpos;
	++ bitpos;
      }
      else {
	ivars[i] = ni;
      }
    }
    size_t bp = 1 << bitpos;
    for (size_t b = 0; b < bp; ++ b) {
      size_t imask = 0;
      for (size_t k = 0; k < ni; ++ k) {
	if ( ivars[k] < ni && ((b >> ivars[k]) & 1) ) {
	  imask |= (1 << k);
	}
      }
      //++ perm_count;
      for (list<Func*>::const_iterator p = mRepList.begin();
	   p != mRepList.end(); ++ p) {
	Func* f = *p;
	bool dif = false;
	for (size_t i = 0; i < ni_pow; ++ i) {
	  if ( f->mNormFunc.value(new_i[i]) != func1.value(i ^ imask) ) {
	    dif = true;
	    break;
	  }
	}
	if ( !dif ) {
	  if ( verbose ) {
	    cout << " match" << endl;
	  }
	  return true;
	}
      }
    }
    if ( mSignature.c0() == 0 ) {
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
	//++ perm_count;
	for (list<Func*>::const_iterator p = mRepList.begin();
	     p != mRepList.end(); ++ p) {
	  Func* f = *p;
	  bool dif = false;
	  for (size_t i = 0; i < ni_pow; ++ i) {
	    if ( f->mNormFunc.value(new_i[i]) == func1.value(i ^ imask) ) {
	      dif = true;
	      break;
	    }
	  }
	  if ( !dif ) {
	    if ( verbose ) {
	      cout << " match with inversion" << endl;
	    }
	    return true;
	  }
	}
      }
    }
  }
  if ( verbose ) {
    cout << " not match" << endl;
  }
  return false;
}

// 新たな関数を追加する．
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

// 関数を登録する．
void
FuncGroupList::reg(size_t ni, const vector<int>& buff)
{
  WalshSigInt sig;
  NpnMap wmap;
  TvFunc func(ni, buff);
  sig.normalize(func, wmap);
  TvFunc norm_func = func.xform(wmap);

  if ( verbose ) {
    cout << "reg()" << endl
	 << " original func: " << func << endl
	 << " normalized func: " << norm_func << endl
	 << " signature: " << sig << endl;
  }
  size_t h = sig.hash() % mSize;
  FuncGroup* fg = NULL;
  for (fg = mTable[h]; fg; fg = fg->mLink) {
    if ( fg->mSignature == sig ) {
      if ( verbose ) {
	cout << "check with fg" << endl;
      }
      if ( fg->check(norm_func) ) {
	// NPN 同値類なので追加の必要なし
	if ( verbose ) {
	  cout << " same class" << endl;
	}
	return;
      }
      if ( verbose ) {
	cout << " new class" << endl;
      }
      fg->add(func, norm_func);
      return;
    }
  }
  if ( fg == NULL ) {
    if ( verbose ) {
      cout << " new group" << endl;
    }
    {
      size_t p = sig.num();
      if ( p > 1 ) {
	++ unresolved_count;
	perm_count += p;
      }
    }
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
  if ( dump_flag ) {
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
  cout << "Total " << n << " NPN-representative function" << endl
       << "Total " << check_count << " checks" << endl
       << "Total " << func_group_list.num() << " different signatures" << endl
       << "Total " << unresolved_count << " unresolved signatures" << endl
       << "Total " << perm_count << " permutations" << endl;
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
  if ( dump_flag ) {
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
       << "Total " << func_group_list.num() << " different signatures" << endl
       << "Total " << unresolved_count << " unresolved signatures" << endl
       << "Total " << perm_count << " permutations" << endl;
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
  try {
    int base = 1;
    for ( ; base < argc; ++ base) {
      if ( argv[base][0] != '-' ) {
	break;
      }
      if ( strcmp(argv[base], "--verbose") == 0 ) {
	verbose = true;
      }
      else if ( strcmp(argv[base], "--dump") == 0 ) {
	dump_flag = true;
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
  }
  catch ( nsYm::AssertError x ) {
    cerr << x;
  }
  return 0;
}
