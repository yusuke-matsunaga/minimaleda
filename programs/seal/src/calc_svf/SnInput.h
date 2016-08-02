#ifndef CALC_SVF_SNINPUT_H
#define CALC_SVF_SNINPUT_H

/// @file calc_svf/SnInput.h
/// @brief SnInput のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: SnInput.h 1920 2008-12-20 15:52:42Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.

#include "SimNode.h"
#include "SnGate.h"


BEGIN_NAMESPACE_YM_SEAL_SVF

//////////////////////////////////////////////////////////////////////
/// @class SnInput SimNode.h
/// @brief 入力ノード
//////////////////////////////////////////////////////////////////////
class SnInput :
  public SimNode
{
public:

  /// @brief コンストラクタ
  explicit
  SnInput(ymuint32 id);

  /// @brief デストラクタ
  virtual
  ~SnInput();


public:

  /// @brief ファンイン数を得る．
  virtual
  size_t
  nfi() const;

  /// @brief pos 番めのファンインを得る．
  virtual
  SimNode*
  fanin(size_t pos) const;

  /// @brief 正常値の計算を行う．
  virtual
  tPackedVal
  _calc_gval();

  /// @brief 故障値の計算を行う．
  virtual
  tPackedVal
  _calc_fval();

  /// @brief 入力の擬似最小 obs を計算する．
  virtual
  void
  calc_pseudo_min_iobs();
  
  /// @brief 入力の最大 obs を計算する．
  virtual
  void
  calc_max_iobs();
  
  /// @brief 入力の obs を計算する．
  virtual
  void
  _calc_iobs(tPackedVal obs);

  /// @brief 内容をダンプする．
  virtual
  void
  dump(ostream& s) const;

};


//////////////////////////////////////////////////////////////////////
/// @class SnBuff SimNode.h
/// @brief BUFFノード
//////////////////////////////////////////////////////////////////////
class SnBuff :
  public SnGate1
{
public:

  /// @brief コンストラクタ
  SnBuff(ymuint32 id,
	 const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnBuff();


public:

  /// @brief 正常値の計算を行う．
  virtual
  tPackedVal
  _calc_gval();

  /// @brief 故障値の計算を行う．
  virtual
  tPackedVal
  _calc_fval();

  /// @brief 入力の擬似最小 obs を計算する．
  virtual
  void
  calc_pseudo_min_iobs();
  
  /// @brief 入力の最大 obs を計算する．
  virtual
  void
  calc_max_iobs();
  
  /// @brief 入力の obs を計算する．
  virtual
  void
  _calc_iobs(tPackedVal obs);

  /// @brief 内容をダンプする．
  virtual
  void
  dump(ostream& s) const;

};


//////////////////////////////////////////////////////////////////////
/// @class SnNot SimNode.h
/// @brief NOTノード
//////////////////////////////////////////////////////////////////////
class SnNot :
  public SnBuff
{
public:

  /// @brief コンストラクタ
  SnNot(ymuint32 id,
	const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnNot();


public:

  /// @brief 正常値の計算を行う．
  virtual
  tPackedVal
  _calc_gval();

  /// @brief 故障値の計算を行う．
  virtual
  tPackedVal
  _calc_fval();

  /// @brief 内容をダンプする．
  virtual
  void
  dump(ostream& s) const;

};

END_NAMESPACE_YM_SEAL_SVF

#endif // CALC_SVF_SNINPUT_H
