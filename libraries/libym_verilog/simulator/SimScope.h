#ifndef LIBYM_VERILOG_SIMULATOR_SIMSCOPE_H
#define LIBYM_VERILOG_SIMULATOR_SIMSCOPE_H

// $Id: SimScope.h 232 2006-09-19 08:45:43Z matsunaga $

// シミュレーション時のスコープの状態を表すクラス

#include <ym_verilog/verilog.h>


BEGIN_NAMESPACE_YM_VERILOG

class VpiScope;
class SimEngine;
class SimNode;

//////////////////////////////////////////////////////////////////////
// シミュレーション時のスコープの状態を表すクラス
//////////////////////////////////////////////////////////////////////
class SimScope
{
  friend class VpiScope;
private:

  // コンストラクタ
  SimScope(SimEngine* engine,
	   const VpiScope* scope);

  // デストラクタ
  ~SimScope();


public:

  // 本当のスコープを返す．
  const VpiScope* scope() const;

  // enable する．
  void enable();

  // disable する．
  void disable();

  // disable されていたらこのスコープの実行を終了する．
  void check();

  // このスコープを抜けるときに実行されるノードを登録する．
  void set_end_node(SimNode* end);


private:

  // disable されていたら true を返す．
  // このスコープを含む親のスコープに対して再帰的に調べる．
  bool is_disabled() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親の SimEngine
  SimEngine* mEngine;
  
  // 本当のスコープ
  const VpiScope* mScope;

  // disable されていたら true となるフラグ
  bool mDisabled;

  // 親のスコープ (NULLもありうる)
  SimScope* mParent;

  // 出口のノード
  SimNode* mEnd;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_SIMULATOR_SIMSCOPE_H
