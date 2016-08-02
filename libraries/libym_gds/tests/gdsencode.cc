
/// @file gdsprint/gdsencode.cc
/// @brief 16進データを生成するプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: gdsencode.cc 958 2007-08-28 05:38:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include <iostream>
#include <ctype.h>
#include <stdlib.h>


// c を16進数用の文字と見なして変換する
// 0-9a-hA-H 以外の場合には 16 を返す．
size_t
conv(char c)
{
  if ( '0' <= c && c <= '9' ) {
    return c - '0';
  }
  if ( 'a' <= c && c <= 'h' ) {
    return c - 'a' + 10;
  }
  if ( 'A' <= c && c <= 'H' ) {
    return c - 'A' + 10;
  }
  return 16;
}

int
main(int argc,
     char** argv)
{
  using std::cin;
  using std::cout;
  using std::cerr;
  using std::endl;

  // 何も考えずに4文字を16進数と見なす．

  size_t i = 0;
  size_t num = 0;
  char c;
  while ( cin.get(c) ) {
    if ( isspace(c) ) continue;
    size_t n = conv(c);
    if ( n == 16 ) {
      cerr << "illegal input: '" << c << "'" << endl;
      exit(1);
    }
    if ( i == 0 ) {
      num = (n << 12);
      ++ i;
    } else if ( i == 1 ) {
      num += (n << 8);
      ++ i;
    } else if ( i == 2 ) {
      num += (n << 4);
      ++ i;
    } else { // i == 3
      i = 0;
      num += n;
      char buf[2];
      buf[0] = num >> 8;
      buf[1] = num & 255;
      cout.write(buf, 2);
    }
  }
  if ( i > 0 ) {
    char buf[2];
    buf[0] = num >> 8;
    buf[1] = num & 255;
    cout.write(buf, 2);
  }
}
