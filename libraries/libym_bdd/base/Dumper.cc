
/// @file libym_bdd/base/Dumper.cc
/// @brief Dumper の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Dumper.cc 700 2007-05-31 00:41:30Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include "Dumper.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
// IdMgr
//////////////////////////////////////////////////////////////////////

// コンストラクタ
IdMgr::IdMgr()
{
  mNext = 0;
}

// デストラクタ
IdMgr::~IdMgr()
{
}

// 登録された節点数を返す．
size_t
IdMgr::num() const
{
  return mNext;
}

// e がすでに ID 番号を持っていたら true を返す．
bool
IdMgr::has_id(tBddEdge e) const
{
  tBddEdge e_p = normalize(e);
  return mMap.count(e_p);
}

// e の ID 番号を返す．
// 未登録ならば新しい番号を割り振る．
size_t
IdMgr::id(tBddEdge e)
{
  tBddEdge e_p = normalize(e);
  size_t id;
  hash_map<tBddEdge, size_t>::iterator p = mMap.find(e_p);
  if ( p == mMap.end() ) {
    id = mNext;
    ++ mNext;
    mMap.insert(make_pair(e_p, id));
  }
  else {
    id = p->second;
  }
  return id;
}

// クリアする．
void
IdMgr::clear()
{
  mMap.clear();
  mNext = 0;
}


//////////////////////////////////////////////////////////////////////
// Displayer
//////////////////////////////////////////////////////////////////////

// コンストラクタ
Displayer::Displayer(BddMgr* mgr, ostream& s) :
  mMgr(mgr),
  mStream(s)
{
}

// デストラクタ
Displayer::~Displayer()
{
}

// e の ID 番号を出力する．
void
Displayer::display_id(tBddEdge e)
{
  size_t id = mIdMgr.id(e);
  ios::fmtflags save = mStream.flags();
  mStream << setw(9) << id;
  mStream.flags(save);
}

// e の内容を出力する．
void
Displayer::display_name(tBddEdge e)
{
  if ( check_zero(e) ) {
    mStream << "     val_0 ";
  }
  else if ( check_one(e) ) {
    mStream << "     val_1 ";
  }
  else if ( check_error(e) ) {
    mStream << "     error ";
  }
  else if ( check_overflow(e) ) {
    mStream << "  overflow ";
  }
  else {
    mStream << " ";
    display_id(e);
    tPol p = get_pol(e);
    mStream << ((p == kPolPosi) ? ' ' : '~');
  }
}

// display_root の下請関数
void
Displayer::display_step(tBddEdge e)
{
  if ( check_leaf(e) ) {
    return;
  }
  tBddEdge e_p = normalize(e);
  if ( mMark.count(e_p) ) {
    return;
  }
  mMark.insert(e_p);
  display_id(e_p);
  tBddEdge e0;
  tBddEdge e1;
  tVarId id = mMgr->root_decomp(e_p, e0, e1);
  tLevel level = mMgr->level(id);
  ios::fmtflags save = mStream.flags();
  mStream << ":IDX=" << setw(3) << id
	  << ":LVL=" << setw(3) << level
	  << ": EDGE0=";
  mStream.flags(save);
  display_name(e0);
  mStream << ": EDGE1=";
  display_name(e1);
  mStream << endl;
  display_step(e0);
  display_step(e1);
}

// e を根とするBDDの内容を出力する．
void
Displayer::display_root(tBddEdge e)
{
  display_name(e);
  mStream << "\tmgr: " << mMgr->name() << endl;
  display_step(e);
}

// 登録された節点数を返す．
size_t
Displayer::num() const
{
  return mIdMgr.num();
}


//////////////////////////////////////////////////////////////////////
// Dumper
//////////////////////////////////////////////////////////////////////

// コンストラクタ
Dumper::Dumper(BddMgr* mgr, ostream& s) :
  mMgr(mgr),
  mStream(s)
{
}

// デストラクタ
Dumper::~Dumper()
{
  mStream.flush();
}

// e を根とするBDDの内容を出力する．
void
Dumper::dump(tBddEdge e)
{
  if ( check_leaf(e) || mIdMgr.has_id(e) ) {
    return;
  }
  tBddEdge e_p = normalize(e);
  tBddEdge e0;
  tBddEdge e1;
  tVarId varid = mMgr->root_decomp(e_p, e0, e1);
  dump(e0);
  dump(e1);
  mIdMgr.id(e_p);
  mStream << varid << ":";
  dump_edge(e0);
  mStream << ":";
  dump_edge(e1);
  mStream << endl;
}

// 枝の内容を書き出す．
void
Dumper::dump_edge(tBddEdge e)
{
  if ( check_zero(e) ) {
    mStream << "F";
  }
  else if ( check_one(e) ) {
    mStream << "T";
  }
  else if ( check_error(e) ) {
    mStream << "E";
  }
  else if ( check_overflow(e) ) {
    mStream << "O";
  }
  else {
    tPol p = get_pol(e);
    mStream << ((p == kPolPosi) ? "P" : "N");
    size_t id = mIdMgr.id(e);
    mStream << id;
  }
}


//////////////////////////////////////////////////////////////////////
// Restorer
//////////////////////////////////////////////////////////////////////

// コンストラクタ
Restorer::Restorer(BddMgr* mgr, istream& s) :
  mMgr(mgr),
  mStream(s)
{
}

// デストラクタ
Restorer::~Restorer()
{
}

// 読み込む．
// 読み込んだBDDの根の枝の数を返す．
// エラーが起きたら 0 を返す．
size_t
Restorer::read()
{
  mRootVector.clear();
  mEdgeVector.clear();
  char buff[1024];
  while ( mStream.getline(buff, sizeof(buff), '\n') ) {
    if ( '0' <= buff[0] && buff[0] <= '9' ) {
      // 内部のノードの記述
      char* p = strtok(buff, ": \t");
      size_t vid = atoi(p);
      p = strtok(NULL, ": \t");
      tBddEdge e0 = find_edge(p);
      p = strtok(NULL, ": \t");
      tBddEdge e1 = find_edge(p);
      tBddEdge ans = mMgr->make_bdd(vid, e0, e1);
      mEdgeVector.push_back(ans);
    }
    else {
      // 根のノードの記述
      mRootVector.push_back(find_edge(buff));
    }
  }
  return mRootVector.size();
}

// pos 番目の枝を返す．
tBddEdge
Restorer::root(size_t pos)
{
  if ( pos < mRootVector.size() ) {
    return mRootVector[pos];
  }
  return kEdgeError;
}

// 内部の枝を指す文字列から枝を得る．
// 見つからなければ kEdgeError を返す．
tBddEdge
Restorer::find_edge(const char* str) const
{
  bool negated = false;
  switch ( str[0] ) {
  case 'T': return kEdge1;
  case 'F': return kEdge0;
  case 'E': return kEdgeError;
  case 'O': return kEdgeOverflow;
  case 'P': break;
  case 'N': negated = true; break;
  default: return kEdgeError;
  }
  size_t pos = atoi(str + 1);
  if ( pos < mEdgeVector.size() ) {
    tBddEdge ans = mEdgeVector[pos];
    if ( negated ) {
      ans = negate(ans);
    }
    return ans;
  }
  return kEdgeError;
}

END_NAMESPACE_YM_BDD
