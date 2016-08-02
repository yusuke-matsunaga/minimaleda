#ifndef LIBYM_VERILOG_SIMULATOR_SIMCALINK_H
#define LIBYM_VERILOG_SIMULATOR_SIMCALINK_H

// $Id: SimCaLink.h 232 2006-09-19 08:45:43Z matsunaga $

// continuous assignment に対応するノード

#include <ym_verilog/verilog.h>


BEGIN_NAMESPACE_YM_VERILOG

class SimEngine;
class SimNode;

//////////////////////////////////////////////////////////////////////
// continuous assignment に対応するノード
//////////////////////////////////////////////////////////////////////
class SimCaLink
{
  friend class SimEngine;
private:

  // コンストラクタ
  SimCaLink(SimEngine* engine,
	    SimNode* start);

  // デストラクタ
  virtual
  ~SimCaLink();
  

public:

  // 親の SimEngine を返す．
  SimEngine* engine() const;
  
  // SimNode の開始点を返す．
  SimNode* start() const;

  // アクティブなら true を返す．
  bool is_active() const;

  // changed フラグを返す．
  bool is_changed() const;

  // アクティブにする．
  void activate();

  // アクティブでなくす．
  void deactivate();

  // changed フラグをセットする．
  void set_changed();

  // changed フラグをリセットする．
  void clear_changed();
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親の SimEngine
  SimEngine* mEngine;
  
  // 対応した SimNode の開始点
  SimNode* mStart;
  
  // 現在, アクティブかどうかを示すフラグ
  bool mActive;

  // 右辺が変化していたら true となるフラグ
  bool mChanged;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_SIMULATOR_SIMCALINK_H
