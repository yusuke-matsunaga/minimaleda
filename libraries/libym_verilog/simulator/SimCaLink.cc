// $Id: SimCaLink.cc 700 2007-05-31 00:41:30Z matsunaga $

// continuous assignment に対応するノード

#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include "SimCaLink.h"
#include "SimEngine.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// continuous assignment に対応するノード
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SimCaLink::SimCaLink(SimEngine* engine,
		     SimNode* start) :
  mEngine(engine),
  mStart(start)
{
  mActive = false;
  mChanged = false;
}

// デストラクタ
SimCaLink::~SimCaLink()
{
}

// 親の SimEngine を返す．
SimEngine*
SimCaLink::engine() const
{
  return mEngine;
}

// SimNode の開始点を返す．
SimNode*
SimCaLink::start() const
{
  return mStart;
}

// アクティブなら true を返す．
bool
SimCaLink::is_active() const
{
  return mActive;
}

// changed フラグを返す．
bool
SimCaLink::is_changed() const
{
  return mChanged;
}

// アクティブにする．
void
SimCaLink::activate()
{
  mActive = true;
}

// アクティブでなくす．
void
SimCaLink::deactivate()
{
  mActive = false;
}

// changed フラグをセットする．
void
SimCaLink::set_changed()
{
  if ( !mChanged ) {
    mChanged = true;
    engine()->reg_contassign(this);
  }
}

// changed フラグをリセットする．
void
SimCaLink::clear_changed()
{
  mChanged = false;
}

END_NAMESPACE_YM_VERILOG
