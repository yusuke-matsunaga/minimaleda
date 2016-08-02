// $Id: strbuff_test.cc 700 2007-05-31 00:41:30Z matsunaga $

// StrBuff のテスト


#include "ym_utils/StrBuff.h"


BEGIN_NAMESPACE_YM

// StrBuff の内容を表示する
void
dump_strbuff(const char* label,
	     const StrBuff& x)
{
  cout << label
       << ", length: " << x.size()
       << ", contents: ";
  if ( x.c_str() ) {
    cout << x.c_str();
  }
  else {
    cout << "<null-ptr>";
  }
  cout << endl;
}

// find_first_of() の結果を表示する．
void
dump_pos(StrBuff::size_type pos)
{
  if ( pos == StrBuff::npos ) {
    cout << "none";
  }
  else {
    cout << pos;
  }
}

END_NAMESPACE_YM

int
main(int argc,
     char** argv)
{
  using namespace nsYm;

  try {
    // 空のコンストラクタのテスト
    StrBuff a1;
    dump_strbuff("a1", a1);

    // C文字列からのコピーコンストラクタのテスト
    StrBuff a2("constructor from C-string");
    dump_strbuff("a2", a2);

    // string からのコピーコンストラクタのテスト
    string tmp1("constructor from string");
    StrBuff a3(tmp1);
    dump_strbuff("a3", a3);

    // StrBuff からのコピーコンストラクタのテスト
    StrBuff a4(a1);
    dump_strbuff("a4(a1)", a4);
    StrBuff a5(a2);
    dump_strbuff("a5(a2)", a5);

    // clear() のテスト
    a1.clear();
    dump_strbuff("a1.clear()", a1);
    a2.clear();
    dump_strbuff("a2.clear()", a2);
    a3.clear();
    dump_strbuff("a3.clear()", a3);
    a4.clear();
    dump_strbuff("a4.clear()", a4);
    a5.clear();
    dump_strbuff("a5.clear()", a5);

    // C文字列からの代入演算子のテスト
    a1 = "assign from C-string";
    dump_strbuff("a1 = \"assign from C-String\"", a1);
    a2 = "abc";
    dump_strbuff("a2 = \"abc\"", a2);

    // string からの代入演算子のテスト
    string tmp2("assign from string");
    a3 = tmp2;
    dump_strbuff("a3 = string(\"assign from string\")", a3);
    string tmp3("0123456789012345678901234567890123456789");
    a3 = tmp3;
    dump_strbuff("a3 = string(\"0123456789012345678901234567890123456789\")", a3);

    // StrBuff からの代入演算子のテスト
    a4 = a1;
    dump_strbuff("a4 = a1", a4);
    a5 = a3;
    dump_strbuff("a5 = a3", a5);

    // 追加のテスト
    a4.put_char('x');
    dump_strbuff("a4.put_char('x')", a4);
    a4.put_str(a1);
    dump_strbuff("a4.put_str(a1)", a4);
    a4.put_str("xyz");
    dump_strbuff("a4.put_str(\"xyz\")", a4);
    string tmp4("pqr");
    a4.put_str(tmp4);
    dump_strbuff("a4.put_str(string(\"pqr\"))", a4);

    // operator[] のテスト
    size_t n = a5.size();
    for (size_t i = 0; i < n; ++ i) {
      cout << "a5[" << i << "] = " << a5[i] << endl;
    }

    // find_first_of のテスト
    cout << "a4.find_first_of('p') = ";
    dump_pos(a4.find_first_of('p'));
    cout << endl;
    cout << "a4.find_first_of('b') = ";
    dump_pos(a4.find_first_of('b'));
    cout << endl;

    // substr のテスト
    cout << "a4.substr(5, 10) = " << a4.substr(5, 10) << endl;

    // 等価演算子のテスト
    cout << "(a1 == a2) = " << (a1 == a2) << endl;
    cout << "(a1 == \"\") = " << (a1 == "") << endl;
    cout << "(a3 == a5) = " << (a3 == a5) << endl;
    cout << "(\"assign from C-string\" == a1) = " << ("assign from C-string" == a1) << endl;
    
  }
  catch ( AssertError a ) {
    cerr << a << endl;
    return 255;
  }
  catch ( ... ) {
    cerr << "unknown exception" << endl;
    return 254;
  }

  return 0;
}
