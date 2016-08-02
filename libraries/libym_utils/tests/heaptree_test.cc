// $Id: heaptree_test.cc 700 2007-05-31 00:41:30Z matsunaga $

// HeapTree のテスト


#include "ym_utils/HeapTree.h"


using namespace std;
using namespace nsYm;

struct CompFunc1
{
  int operator()(int a,
		 int b)
  {
    return a - b;
  }
};

struct CompFunc2
{
  int operator()(const char* a,
		 const char* b)
  {
    return strcmp(a, b);
  }
};

void
test1()
{
  
  HeapTree<int, CompFunc1> ht;

  ht.put(1);
  ht.put(3);
  ht.put(5);
  ht.put(2);
  ht.put(3);
  ht.put(9);
  ht.put(6);
  ht.put(7);
  ht.put(4);

  cout << "===<test1>===" << endl;
  while ( !ht.empty() ) {
    int a = ht.getmin();
    cout << a << endl;
    ht.popmin();
  }
  cout << endl;
}

void
test2()
{
  HeapTree<const char*, CompFunc2> ht;

  ht.put("abc");
  ht.put("ab");
  ht.put("a");
  ht.put("xyz");
  ht.put("pqr");
  ht.put("this");
  ht.put("quick");
  ht.put("brown");
  ht.put("fox");
  ht.put("jumped");
  ht.put("over");
  ht.put("a");
  ht.put("lazy");
  ht.put("dog");

  cout << "===<test2>===" << endl;
  while ( !ht.empty() ) {
    const char* a = ht.getmin();
    cout << a << endl;
    ht.popmin();
  }
  cout << endl;
}

void
test3()
{
  SimpleHeapTree<const char*> ht;

  ht.put("abc", 3);
  ht.put("ab", 2);
  ht.put("a", 1);
  ht.put("xyz", 3);
  ht.put("pqr", 3);
  ht.put("this", 4);
  ht.put("quick", 5);
  ht.put("brown", 5);
  ht.put("fox", 3);
  ht.put("jumped", 6);
  ht.put("over", 4);
  ht.put("a", 1);
  ht.put("lazy", 4);
  ht.put("dog", 3);

  cout << "===<test3>===" << endl;
  while ( !ht.empty() ) {
    const char* a = ht.getmin();
    cout << a << endl;
    ht.popmin();
  }
  cout << endl;
}

int
main(int argc,
     char** argv)
{
  try {
    test1();
    test2();
    test3();
  }
  catch ( AssertError a ) {
    cerr << a << endl;
    return 255;
  }

  return 0;
}
