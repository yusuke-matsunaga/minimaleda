
/// @file libym_techmap/tests/patgraph_test.cc
/// @brief PatGraph のテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "PatMgr.h"
#include "PatGraph.h"


BEGIN_NAMESPACE_YM_TECHMAP

void
dump_word(ostream& s,
	  ymuint val)
{
  static char buf[4];
  buf[0] = val & 255U; val >>= 8;
  buf[1] = val & 255U; val >>= 8;
  buf[2] = val & 255U; val >>= 8;
  buf[3] = val & 255U;

  s.write(buf, 4);
}

void
dump_input(ostream& s,
	   ymuint input_id)
{
  dump_word(s, static_cast<ymuint32>(PatMgr::kInput) | (input_id << 2));
  dump_word(s, 0);
  dump_word(s, 0);
}

void
dump_and(ostream& s,
	 ymuint l_id,
	 ymuint r_id,
	 bool l_inv,
	 bool r_inv)
{
  dump_word(s, static_cast<ymuint32>(PatMgr::kAnd));
  dump_word(s, (l_id << 1) | static_cast<ymuint32>(l_inv));
  dump_word(s, (r_id << 1) | static_cast<ymuint32>(r_inv));
}

void
dump_xor(ostream& s,
	 ymuint l_id,
	 ymuint r_id)
{
  dump_word(s, static_cast<ymuint32>(PatMgr::kXor));
  dump_word(s, (l_id << 1));
  dump_word(s, (r_id << 1));
}

void
test()
{
  const char* filename = "patgraph_test.data";
  ofstream os;
  os.open(filename, ios::binary);
  if ( !os ) {
    // エラー
    cerr << "Could not create " << filename << endl;
    return;
  }

  dump_word(os, 5); // ノード数
  dump_input(os, 0);
  dump_input(os, 1);
  dump_input(os, 2);
  dump_and(os, 0, 1, false, true);
  dump_xor(os, 3, 2);

  dump_word(os, 3); // 入力数
  dump_word(os, 0);
  dump_word(os, 1);
  dump_word(os, 2);

  dump_word(os, 1); // パタン数
  dump_word(os, 6);
  dump_word(os, 4);
  dump_word(os, 8);
  dump_word(os, 6);
  dump_word(os, 7);
  dump_word(os, 9);

  os.close();

  ifstream is;
  is.open(filename);
  if ( !is ) {
    // エラー
    cerr << "Could not open " << filename << endl;
    return;
  }

  PatMgr pat_mgr;

  pat_mgr.load(is);

  dump(cout, pat_mgr);
}

END_NAMESPACE_YM_TECHMAP


int
main(int argc,
     char* argv[])
{
  nsYm::nsTechmap::test();

  return 0;
}
