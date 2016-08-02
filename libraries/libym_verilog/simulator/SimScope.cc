// $Id: SimScope.cc 700 2007-05-31 00:41:30Z matsunaga $

// シミュレーション時のスコープの状態を表すクラス

#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include "SimScope.h"

#include <ym_verilog/vpi/VpiScope.h>
#include "SimEngine.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// シミュレーション時のスコープの状態を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SimScope::SimScope(SimEngine* engine,
		   const VpiScope* scope) :
  mEngine(engine),
  mScope(scope)
{
  mDisabled = true;
  // 分かりにくいけど親の SimScope
  //mParent = scope->scope()->find_simscope(engine);
  mEnd = NULL;
}

// デストラクタ
SimScope::~SimScope()
{
}

// 本当のスコープを返す．
const VpiScope*
SimScope::scope() const
{
  return mScope;
}

// enable する．
void
SimScope::enable()
{
  mDisabled = false;
}

// disable する．
void
SimScope::disable()
{
  mDisabled = true;
}

// このスコープを抜けるときに実行されるノードを登録する．
void
SimScope::set_end_node(SimNode* end)
{
  mEnd = end;
}

// disable されていたらこのスコープの実行を終了する．
void
SimScope::check()
{
  if ( is_disabled() ) {
    mEngine->reg_active_event(mEnd);
  }
}

// disable されていたら true を返す．
// このスコープを含む親のスコープに対して再帰的に調べる．
bool
SimScope::is_disabled() const
{
  for (const SimScope* s = this; s != NULL; s = s->mParent) {
    if ( s->mDisabled ) {
      return true;
    }
  }
  return false;
}

END_NAMESPACE_YM_VERILOG
