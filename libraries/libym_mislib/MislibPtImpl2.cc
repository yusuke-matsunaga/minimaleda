
/// @file libym_mislib/MislibPtImpl2.cc
/// @brief MislibPtImpl2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MislibPtImpl2.cc 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.

#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include "MislibPtImpl2.h"


BEGIN_NAMESPACE_YM_MISLIB

//////////////////////////////////////////////////////////////////////
// NOT論理式を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
MislibPtNot::MislibPtNot(const FileRegion& loc,
			 MislibPt* child1) :
  MislibPt(loc),
  mChild1(child1)
{
}

// デストラクタ
MislibPtNot::~MislibPtNot()
{
}

// 種類を取り出す．
MislibPt::tType
MislibPtNot::type() const
{
  return kNot;
}

// @brief 論理式を表す型のときに true を返す．
bool
MislibPtNot::is_expr() const
{
  return true;
}

// 1番目の子供を取り出す．
MislibPt*
MislibPtNot::child1() const
{
  return mChild1;
}

// 内容を出力する．
// デバッグ用
void
MislibPtNot::dump(ostream& s) const
{
  s << "<NOT>" << endl;
  dump_loc(s);
  child1()->dump(s);
  s << "</NOT>" << endl;
}


//////////////////////////////////////////////////////////////////////
// AND論理式を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
MislibPtAnd::MislibPtAnd(const FileRegion& loc,
			 MislibPt* child1,
			 MislibPt* child2) :
  MislibPt(loc),
  mChild1(child1),
  mChild2(child2)
{
}

// デストラクタ
MislibPtAnd::~MislibPtAnd()
{
}

// 種類を取り出す．
MislibPt::tType
MislibPtAnd::type() const
{
  return kAnd;
}

// @brief 論理式を表す型のときに true を返す．
bool
MislibPtAnd::is_expr() const
{
  return true;
}

// 1番目の子供を取り出す．
MislibPt*
MislibPtAnd::child1() const
{
  return mChild1;
}

// 2番目の子供を取り出す．
MislibPt*
MislibPtAnd::child2() const
{
  return mChild2;
}

// 内容を出力する．
// デバッグ用
void
MislibPtAnd::dump(ostream& s) const
{
  s << "<AND>" << endl;
  dump_loc(s);
  child1()->dump(s);
  child2()->dump(s);
  s << "</AND>" << endl;
}


//////////////////////////////////////////////////////////////////////
// OR論理式を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
MislibPtOr::MislibPtOr(const FileRegion& loc,
		       MislibPt* child1,
		       MislibPt* child2) :
  MislibPt(loc),
  mChild1(child1),
  mChild2(child2)
{
}

// デストラクタ
MislibPtOr::~MislibPtOr()
{
}

// 種類を取り出す．
MislibPt::tType
MislibPtOr::type() const
{
  return kOr;
}

// @brief 論理式を表す型のときに true を返す．
bool
MislibPtOr::is_expr() const
{
  return true;
}

// 1番目の子供を取り出す．
MislibPt*
MislibPtOr::child1() const
{
  return mChild1;
}

// 2番目の子供を取り出す．
MislibPt*
MislibPtOr::child2() const
{
  return mChild2;
}

// 内容を出力する．
// デバッグ用
void
MislibPtOr::dump(ostream& s) const
{
  s << "<OR>" << endl;
  dump_loc(s);
  child1()->dump(s);
  child2()->dump(s);
  s << "</OR>" << endl;
}


//////////////////////////////////////////////////////////////////////
// ピンを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
MislibPtPin::MislibPtPin(const FileRegion& loc,
			 MislibPt* name,
			 MislibPt* phase,
			 MislibPt* input_load,
			 MislibPt* max_load,
			 MislibPt* rise_block_delay,
			 MislibPt* rise_fanout_delay,
			 MislibPt* fall_block_delay,
			 MislibPt* fall_fanout_delay) :
  MislibPt(loc),
  mName(name),
  mPhase(phase),
  mInputLoad(input_load),
  mMaxLoad(max_load),
  mRiseBlockDelay(rise_block_delay),
  mRiseFanoutDelay(rise_fanout_delay),
  mFallBlockDelay(fall_block_delay),
  mFallFanoutDelay(fall_fanout_delay)
{
  mNext = NULL;
}

// デストラクタ
MislibPtPin::~MislibPtPin()
{
}

// 種類を取り出す．
MislibPt::tType
MislibPtPin::type() const
{
  return kPin;
}

// ピン名を表すオブジェクトを取り出す．
MislibPt*
MislibPtPin::name() const
{
  return mName;
}

// 極性情報を表すオブジェクトを取り出す．
MislibPt*
MislibPtPin::phase() const
{
  return mPhase;
}

// 入力負荷を表すオブジェクトを取り出す．
MislibPt*
MislibPtPin::input_load() const
{
  return mInputLoad;
}

// 最大駆動負荷を表すオブジェクトを取り出す．
MislibPt*
MislibPtPin::max_load() const
{
  return mMaxLoad;
}

// 立ち上がり固定遅延を表すオブジェクトを取り出す．
MislibPt*
MislibPtPin::rise_block_delay() const
{
  return mRiseBlockDelay;
}

// 立ち上がり負荷依存遅延を表すオブジェクトを取り出す．
MislibPt*
MislibPtPin::rise_fanout_delay() const
{
  return mRiseFanoutDelay;
}

// 立ち下がり固定遅延を表すオブジェクトを取り出す．
MislibPt*
MislibPtPin::fall_block_delay() const
{
  return mFallBlockDelay;
}

// 立ち下がり負荷依存遅延を表すオブジェクトを取り出す．
MislibPt*
MislibPtPin::fall_fanout_delay() const
{
  return mFallFanoutDelay;
}

// 次のピンを設定する．
void
MislibPtPin::set_next(MislibPt* pin)
{
  mNext = pin;
}

// 次のピンを取り出す．
MislibPt*
MislibPtPin::next() const
{
  return mNext;
}

// 内容を出力する．
// デバッグ用
void
MislibPtPin::dump(ostream& s) const
{
  s << "<PIN>" << endl;
  dump_loc(s);
  s << "<NAME>" << endl;
  if ( name() ) {
    name()->dump(s);
  }
  else {
    s << "*" << endl;
  }
  s << "</NAME>" << endl;
  s << "<PHASE>" << endl;
  phase()->dump(s);
  s << "</PHASE>" << endl;
  s << "<INPUT_LOAD>" << endl;
  input_load()->dump(s);
  s << "</INPUT_LOAD>" << endl;
  s << "<MAX_LOAD>" << endl;
  max_load()->dump(s);
  s << "</MAX_LOAD>" << endl;
  s << "<RISE_BLOCK_DELAY>" << endl;
  rise_block_delay()->dump(s);
  s << "</RISE_BLOCK_DELAY>" << endl;
  s << "<RISE_FANOUT_DELAY>" << endl;
  rise_fanout_delay()->dump(s);
  s << "</RISE_FANOUT_DELAY>" << endl;
  s << "<FALL_BLOCK_DELAY>" << endl;
  fall_block_delay()->dump(s);
  s << "</FALL_BLOCK_DELAY>" << endl;
  s << "<FALLE_FANOUT_DELAY>" << endl;
  fall_fanout_delay()->dump(s);
  s << "</FALL_FANOUT_DELAY>" << endl;
  s << "</PIN>" << endl;
}


//////////////////////////////////////////////////////////////////////
/// ピンのリストを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
MislibPtPinList::MislibPtPinList() :
  MislibPt(FileRegion())
{
  mTop = NULL;
  mEnd = NULL;
}

// デストラクタ
MislibPtPinList::~MislibPtPinList()
{
}

// 種類を取り出す．
MislibPt::tType
MislibPtPinList::type() const
{
  return kPinList;
}

// 末尾にピンを追加する．
void
MislibPtPinList::push_back(MislibPt* pin)
{
  if ( mEnd ) {
    mEnd->set_next(pin);
    mEnd = pin;
  }
  else {
    mTop = mEnd = pin;
  }
}

// 先頭のピンを取り出す．
MislibPt*
MislibPtPinList::pin() const
{
  return mTop;
}

// 内容を出力する．
// デバッグ用
void
MislibPtPinList::dump(ostream& s) const
{
  s << "<PINLIST>" << endl;
  dump_loc(s);
  for (MislibPt* tmp = pin(); tmp; tmp = tmp->next()) {
    tmp->dump(s);
  }
  s << "</PINLIST>" << endl;
}

END_NAMESPACE_YM_MISLIB
